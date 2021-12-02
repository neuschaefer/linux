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
/**
 * @file sv_const.h
 *
 * This file defines const value
 *
 * @author Mtk00728
 */

#ifndef _SV_CONST_H_
#define _SV_CONST_H_

//lint -e621 Identifier clash

/****************************************************************************
                      General Constants
 ****************************************************************************/

#define SV_TRUE            1
#define SV_FALSE           0

#define SV_SUCCESS         1
#define SV_FAIL            0
#define SV_NO_CHANGE       2

#define SV_ON              1
#define SV_OFF             0

#define SV_LEFT     1
#define SV_RIGHT    0

#define SV_LEVEL1             1
#define SV_LEVEL2          2
#define SV_LEVEL3             3
#define SV_LEVEL4          4

/* const for Mode detect status */
enum {
    SV_WAIT,
    SV_DONE,
    SV_NOSIGNAL,
    SV_IDLE
};

/****************************************************************************
                      Constants for Video API
 ****************************************************************************/
/* Video Path (VP) */
typedef enum 
{
    SV_VP_MAIN,
    SV_VP_PIP,
    SV_VP_NA,
    #ifdef  SUPPORT_AV_COMP_AUTO
	SV_VP_MON,
    #endif
    SV_VP_MAX
} SV_VP_T;

/* Color system */
// 12/25/03 Cindy: Change the order of color system defination to match hardware define
enum {
    SV_CS_PAL_N,
    SV_CS_PAL,
    SV_CS_PAL_M,
    SV_CS_NTSC358,
    SV_CS_SECAM,
    SV_CS_PAL_60,
    SV_CS_UNSTABLE,        // Add by cindy to represent video signal is not stable yet! In Hardware, it's reserved
    SV_CS_NTSC443,
    SV_CS_AUTO,
    SV_CS_NONE,
    SV_CS_MAX
};

/* wide size of picture */
enum {
    SV_WS_NORMAL,
    SV_WS_FULL,
    SV_WS_OPT_W1,
    SV_WS_OPT_W2,
    SV_WS_OPT_W3,
    SV_WS_4W_3H,
    SV_WS_16W_9H,
    SV_WS_NO_SCALE,
    SV_WS_PANORAMIC,
    SV_WS_LB_16W_9H,
    SV_WS_LB_15W_8H,
    SV_WS_LB_14W_6H,
    SV_WS_AUTO,
    SV_WS_MAX
};


/* Video color enhancement features: FBG(Flesh Tone, Blue Sky, Green Grass) */

/* Gamma correction coefficients */
enum {
    SV_RGB_R,
    SV_RGB_G,
    SV_RGB_B,
    SV_RGB_RGB,
    SV_RGB_MAX
};

/* Video Decoders */
enum
{
    SV_VD_NA,    /* SV_VD_NA must be 0 */
    SV_VD_TVD3D,
    SV_VD_YPBPR,
    SV_VD_VGA,
    SV_VD_DVI,
    SV_VD_MPEGHD, // Add for 5371
    SV_VD_MPEGSD,
    SV_VD_MAX
};


/* signal type */
enum
{
    SV_ST_TV,
    SV_ST_AV,
    SV_ST_SV,
    SV_ST_YP,
    SV_ST_VGA,
    SV_ST_DVI,
    SV_ST_MPEG, // add for 5371
    SV_ST_MAX
};

/* EXT Decoders */
//For Digital Port
#define SV_DV_NONE 0x00
#define SV_DV_DVI 0x01 // [0]
#define SV_DV_HDMI 0x02 // [1]
#define SV_DV_AD_YPbPr 0x04 // [2]
#define SV_DV_AD_RGB 0x08 // [3]
#define SV_DV_CCIR 0x10 // [4]
#define SV_DV_FROM_CCIR 0x20 // [5]
#define SV_DV_DTV 0x40 //[6]


/*****************************************************************************/
/* Video gamma */
enum
{
    SV_V_GAMMA_OFF,
    SV_V_GAMMA_DARK,
    SV_V_GAMMA_MIDDLE,
    SV_V_GAMMA_BRIGHT
};

enum
{
    SV_V_SCE_OFF,
    SV_V_SCE_DEFAULT
};


/* Video Signal Status -- Add by Cindy */
enum VIDE_SIGNAL_STATUS_LIST
{
    SV_VDO_NOSIGNAL        = 0,
    SV_VDO_NOSUPPORT,
    SV_VDO_UNKNOWN,        // Still doing mode detect
    SV_VDO_STABLE
};

enum {
    SV_DCLK_48HZ,
    SV_DCLK_50HZ,
    SV_DCLK_60HZ,
    SV_DCLK_OTHERS,
    SV_DCLK_UNKNOW = 0xff
};


enum {
// Separate SDTV/HDTV, Analog/Digital of YPbPr source.
    SV_COLOR_YPBPR_ANALOG,
    SV_COLOR_YPBPR_DIGITAL,
    SV_COLOR_YPBPR_DTV_SD,
    SV_COLOR_YPBPR_DTV_HD,

    SV_COLOR_YCBCR,
    SV_COLOR_RGB,
    SV_COLOR_RGB_VGA,
    SV_COLOR_YUV,
    SV_COLOR_GBR,        // SCART RGB through YPbPr port, the mapping is GBR -> YUV
    SV_COLOR_BGR,        // SCART RGB through YPbPr port under fast blanking mode, the mapping is BGR -> YUV
    SV_COLOR_YCBCR_RGB,
    SV_COLOR_YCBCR_NTSC,
    SV_COLOR_YCBCR_NTSCJ,
    SV_COLOR_YCBCR_PAL,
    SV_COLOR_RGB_YUV,

    //mtk01140 20070705 copy from 8226
    SV_COLOR_HDMI_RGB,
    SV_COLOR_HDMI_422_601,
    SV_COLOR_HDMI_422_709,
    SV_COLOR_HDMI_444_601,
    SV_COLOR_HDMI_444_709,
    SV_COLOR_HDMI_XVYCC_601,
    SV_COLOR_HDMI_XVYCC_709
};

enum
{
    SV_COLORTRANS_RGBF_2_RGBF = 0,  // 0    RGB(Full) -> RGB (Full)
    SV_COLORTRANS_RGBL_2_RGBF,      // 1    RGB(Limit) -> RGB (Full)
    SV_COLORTRANS_RGBF_2_YCBCR601L,     // 2    RGB(Full) -> YCbCr601(Limit)
    SV_COLORTRANS_RGBL_2_YCBCR601L,     // 3    RGB(Limit) -> YCbCr601(Limit)
    SV_COLORTRANS_YCBCR601L_2_YCBCR601L,        // 4    YCbCr601(Limit) -> YCbCr601(Limit)
    SV_COLORTRANS_YCBCR601F_2_YCBCR601L,        // 5    YCbCr601(Full) -> YCbCr601(Limit)
    SV_COLORTRANS_YCBCR601L_2_RGBF,     // 6    YCbCr601(Limit) -> RGB(Full)
    SV_COLORTRANS_YCBCR601F_2_RGBF,     // 7    YCbCr601(Full) -> RGB(Full)
    SV_COLORTRANS_YCBCR709L_2_YCBCR601L,        // 8    YCbCr709(Limit) -> YCbCr601(Limit)
    SV_COLORTRANS_YCBCR709F_2_YCBCR601L,        // 9    YCbCr709(Full) -> YCbCr601(Limit)
    SV_COLORTRANS_YCBCR709L_2_RGBF,     // 10   YCbCr709(Limit) -> RGB(Full)    
    SV_COLORTRANS_YCBCR709F_2_RGBF,     // 11   YCbCr709(Full) -> RGB(Full)
    SV_COLORTRANS_YUVNTSC_2_YCBCR601L,      // 12   TVD NTSC YUV -> YCbCr601(Limit)
    SV_COLORTRANS_YUVNTSCJ_2_YCBCR601L,     // 13   TVD NTSC_J YUV -> YCbCr601(Limit)
    SV_COLORTRANS_YUVPAL_2_YCBCR601L,       // 14   TVD PAL YUV -> YCbCr601(Limit)
    SV_COLORTRANS_YUVPAL_2_YCBCR601L_NOULTRABLK,        // 15   TVD PAL YUV -> YCbCr601(Limit) //not support ultra black
    SV_COLORTRANS_SCARTBGR_2_YCBCR601L,     // 16   SCART RGB, BGR->YCbCr(Limit)
    SV_COLORTRANS_SCARTGBR_2_YCBCR601L,     // 17   SCART RGB, GBR -> YCbCr601(Limit)
    SV_COLORTRANS_RGBF_2_YCBCR709L,     // 18    RGB(Full) -> YCbCr709(Limit)
    SV_COLORTRANS_RGBL_2_YCBCR709L,     // 19    RGB(Limit) -> YCbCr709(Limit)
    SV_COLORTRANS_MAX
};


/* HDMI Range Mode */
enum
{
    SV_HDMI_RANGE_DEFAULT,
    SV_HDMI_RANGE_LIMIT,
    SV_HDMI_RANGE_FULL,
    SV_HDMI_RANGE_RESERVED
};


/* video Film mode */
enum VIDEO_CINEMA_LIST
{
    SV_CN_AUTO,// Means On in defensive Mode
    SV_CN_OFF, // Maans Turn Off
    SV_CN_MOVIE,//Means Turn On in Aggresive Mode
    SV_CN_MAX
};

enum EDGE_MODE_LIST
{
    SV_V_EDGE_OFF, //Not this Mode implemented
    SV_V_EDGE_LOW, //Defensive Mode
    SV_V_EDGE_HIGH //Aggressive Mode
};

enum {
    SV_MUTE_BLACK,
    SV_MUTE_BLUE,
    SV_MUTE_RED,
    SV_MUTE_WHITE,
    SV_MUTE_GREEN
};

enum DI_MODE_LIST
{
    SV_MA_SLOW,// Tend 3D
    SV_MA_MOTION// Tend 2D
};

/* Video Timing */
enum {
    SV_VIDEO_TM_480I,
    SV_VIDEO_TM_576I,
    SV_VIDEO_TM_480P,
    SV_VIDEO_TM_576P,
    SV_VIDEO_TM_720P,
    SV_VIDEO_TM_1080I,
    SV_VIDEO_TM_1080P,
    SV_VIDEO_TM_MAX
};

// SCART function switch condition
#define FS_NO_CHANGE        0
#define FS_SCALE_CHANGE     1
#define FS_ON_TO_OFF        2
#define FS_OFF_TO_ON        3
#define FS_NO_SUPPORT       0xff

// SCART Input Type
#define SV_SCART_AUTO       0x00
#define SV_SCART_CVBS_RGB   0x01
#define SV_SCART_CVBS       0x02
#define SV_SCART_RGB        0x03
#define SV_SCART_SV         0x04

///////////////////////////////////////////////////////////////////////////////////
// The following is source type/timing setting.
// Also refer to vdo_misc.c, bDrvVideoGetSourceTypeTiming() function.
enum {
    SOURCE_TYPE_TIMING_RFTV_NTSC_358,
    SOURCE_TYPE_TIMING_RFTV_NTSC_443,
    SOURCE_TYPE_TIMING_RFTV_PAL,
    SOURCE_TYPE_TIMING_RFTV_PAL_M,
    SOURCE_TYPE_TIMING_RFTV_PAL_N,
    SOURCE_TYPE_TIMING_RFTV_PAL_60,

    SOURCE_TYPE_TIMING_CVBS_NTSC_358,
    SOURCE_TYPE_TIMING_CVBS_NTSC_443,
    SOURCE_TYPE_TIMING_CVBS_PAL,
    SOURCE_TYPE_TIMING_CVBS_PAL_M,
    SOURCE_TYPE_TIMING_CVBS_PAL_N,
    SOURCE_TYPE_TIMING_CVBS_PAL_60,

    SOURCE_TYPE_TIMING_SV_NTSC_358,
    SOURCE_TYPE_TIMING_SV_NTSC_443,
    SOURCE_TYPE_TIMING_SV_PAL,
    SOURCE_TYPE_TIMING_SV_PAL_M,
    SOURCE_TYPE_TIMING_SV_PAL_N,
    SOURCE_TYPE_TIMING_SV_PAL_60,

    SOURCE_TYPE_TIMING_CVBS_NTSC_358_OS,
    SOURCE_TYPE_TIMING_CVBS_NTSC_443_OS,
    SOURCE_TYPE_TIMING_CVBS_PAL_OS,
    SOURCE_TYPE_TIMING_CVBS_PAL_M_OS,
    SOURCE_TYPE_TIMING_CVBS_PAL_N_OS,
    SOURCE_TYPE_TIMING_CVBS_PAL_60_OS,

    SOURCE_TYPE_TIMING_SV_NTSC_358_OS,
    SOURCE_TYPE_TIMING_SV_NTSC_443_OS,
    SOURCE_TYPE_TIMING_SV_PAL_OS,
    SOURCE_TYPE_TIMING_SV_PAL_M_OS,
    SOURCE_TYPE_TIMING_SV_PAL_N_OS,
    SOURCE_TYPE_TIMING_SV_PAL_60_OS,

    SOURCE_TYPE_TIMING_SECAM_RFTV,
    SOURCE_TYPE_TIMING_SECAM_CVBS,
    SOURCE_TYPE_TIMING_SECAM_SV,

    SOURCE_TYPE_TIMING_SCART_RGB,
    SOURCE_TYPE_TIMING_SCART_AV_NTSC_358,
    SOURCE_TYPE_TIMING_SCART_AV_NTSC_443,
    SOURCE_TYPE_TIMING_SCART_AV_PAL,
    SOURCE_TYPE_TIMING_SCART_AV_PAL_M,
    SOURCE_TYPE_TIMING_SCART_AV_PAL_N,
    SOURCE_TYPE_TIMING_SCART_AV_PAL_60,
    SOURCE_TYPE_TIMING_SCART_AV_SECAM,

    SOURCE_TYPE_TIMING_SCART_SV_NTSC_358,
    SOURCE_TYPE_TIMING_SCART_SV_NTSC_443,
    SOURCE_TYPE_TIMING_SCART_SV_PAL,
    SOURCE_TYPE_TIMING_SCART_SV_PAL_M,
    SOURCE_TYPE_TIMING_SCART_SV_PAL_N,
    SOURCE_TYPE_TIMING_SCART_SV_PAL_60,
    SOURCE_TYPE_TIMING_SCART_SV_SECAM,

    SOURCE_TYPE_TIMING_YPBPR_SD_I_50,
    SOURCE_TYPE_TIMING_YPBPR_SD_I_60,
    SOURCE_TYPE_TIMING_YPBPR_SD_I_50_OS,
    SOURCE_TYPE_TIMING_YPBPR_SD_I_60_OS,

    SOURCE_TYPE_TIMING_YPBPR_SD_P_50,
    SOURCE_TYPE_TIMING_YPBPR_SD_P_60,
    SOURCE_TYPE_TIMING_YPBPR_SD_P_50_OS,
    SOURCE_TYPE_TIMING_YPBPR_SD_P_60_OS,

    SOURCE_TYPE_TIMING_YPBPR_720P_24,
    SOURCE_TYPE_TIMING_YPBPR_720P_25,
    SOURCE_TYPE_TIMING_YPBPR_720P_50,
    SOURCE_TYPE_TIMING_YPBPR_720P_60,
    
    SOURCE_TYPE_TIMING_YPBPR_1080I_50,
    SOURCE_TYPE_TIMING_YPBPR_1080I_60,

    SOURCE_TYPE_TIMING_YPBPR_1080P_24,
    SOURCE_TYPE_TIMING_YPBPR_1080P_25,   
    SOURCE_TYPE_TIMING_YPBPR_1080P_50,
    SOURCE_TYPE_TIMING_YPBPR_1080P_60,

    SOURCE_TYPE_TIMING_DIGI_SD_I_50,
    SOURCE_TYPE_TIMING_DIGI_SD_I_60,
    SOURCE_TYPE_TIMING_3D_480I_60_FP,
    SOURCE_TYPE_TIMING_3D_576I_60_FP, 
   
    SOURCE_TYPE_TIMING_DIGI_SD_P_50,
    SOURCE_TYPE_TIMING_DIGI_SD_P_60,
    SOURCE_TYPE_TIMING_3D_576p_50_FP,
    SOURCE_TYPE_TIMING_3D_480p_60_FP,    

    SOURCE_TYPE_TIMING_DIGI_720P_24,
    SOURCE_TYPE_TIMING_DIGI_720P_25,    
    SOURCE_TYPE_TIMING_DIGI_720P_50,
    SOURCE_TYPE_TIMING_DIGI_720P_60,
    SOURCE_TYPE_TIMING_3D_720p_24_FP,
    SOURCE_TYPE_TIMING_3D_720p_30_FP,
    SOURCE_TYPE_TIMING_3D_720p_50_FP, 
    SOURCE_TYPE_TIMING_3D_720p_60_FP,
    
    SOURCE_TYPE_TIMING_DIGI_1080I_50,
    SOURCE_TYPE_TIMING_DIGI_1080I_60,
    SOURCE_TYPE_TIMING_3D_1080I_50_FP,
    SOURCE_TYPE_TIMING_3D_1080I_60_FP, 

    SOURCE_TYPE_TIMING_DIGI_1080P_24,
    SOURCE_TYPE_TIMING_DIGI_1080P_25,
    SOURCE_TYPE_TIMING_3D_1080p_24_FP,
    SOURCE_TYPE_TIMING_3D_1080p_30_FP,    
    SOURCE_TYPE_TIMING_3D_1080p_50_FP,    
    SOURCE_TYPE_TIMING_3D_1080p_60_FP,
    SOURCE_TYPE_TIMING_3D_1650_750_60_FP,
    SOURCE_TYPE_TIMING_3D_1650_1500_30_FP,

	SOURCE_TYPE_TIMING_3D_640_480p_60_FP,
    SOURCE_TYPE_TIMING_3D_1440_240p_60_FP,
    SOURCE_TYPE_TIMING_3D_1440_288p_50_FP,
    SOURCE_TYPE_TIMING_3D_1440_576p_50_FP,
    SOURCE_TYPE_TIMING_3D_720p_25_FP,
    SOURCE_TYPE_TIMING_3D_1080p_25_FP,
    SOURCE_TYPE_TIMING_3D_1080I_1250TOTAL_50_FP,
    
    SOURCE_TYPE_TIMING_3D_1080p_24_SBS_FULL,
	SOURCE_TYPE_TIMING_3D_1080p_30_SBS_FULL,
	SOURCE_TYPE_TIMING_3D_1080I_50_SBS_FULL,
	SOURCE_TYPE_TIMING_3D_1080I_60_SBS_FULL,
	SOURCE_TYPE_TIMING_3D_720p_24_SBS_FULL,
	SOURCE_TYPE_TIMING_3D_720p_30_SBS_FULL,
	SOURCE_TYPE_TIMING_3D_720p_50_SBS_FULL,
	SOURCE_TYPE_TIMING_3D_720p_60_SBS_FULL,
	SOURCE_TYPE_TIMING_3D_480p_60_SBS_FULL,
	SOURCE_TYPE_TIMING_3D_576p_50_SBS_FULL,
	SOURCE_TYPE_TIMING_3D_480I_60_SBS_FULL,
	SOURCE_TYPE_TIMING_3D_576I_50_SBS_FULL,
	
    SOURCE_TYPE_TIMING_DIGI_1080P_50,
    SOURCE_TYPE_TIMING_DIGI_1080P_60,

    SOURCE_TYPE_TIMING_DTV_SD_I_50,
    SOURCE_TYPE_TIMING_DTV_SD_I_60,
    
    SOURCE_TYPE_TIMING_DTV_SD_P_50,
    SOURCE_TYPE_TIMING_DTV_SD_P_60,
    
    SOURCE_TYPE_TIMING_DTV_720P_24,
    SOURCE_TYPE_TIMING_DTV_720P_25,    
    SOURCE_TYPE_TIMING_DTV_720P_50,
    SOURCE_TYPE_TIMING_DTV_720P_60,
    
    SOURCE_TYPE_TIMING_DTV_1080I_50,
    SOURCE_TYPE_TIMING_DTV_1080I_60,
    SOURCE_TYPE_TIMING_DTV_1080P_24,
    SOURCE_TYPE_TIMING_DTV_1080P_25,    
    SOURCE_TYPE_TIMING_DTV_1080P_50,
    SOURCE_TYPE_TIMING_DTV_1080P_60,
 
    SOURCE_TYPE_TIMING_MM_SD_I,
    SOURCE_TYPE_TIMING_MM_SD_P,
    SOURCE_TYPE_TIMING_MM_720P_SD,
    SOURCE_TYPE_TIMING_MM_720P_HD,
    SOURCE_TYPE_TIMING_MM_1080IP_SD, 
    SOURCE_TYPE_TIMING_MM_1080IP_HD, 
    
    SOURCE_TYPE_TIMING_PC_RGB,
    SOURCE_TYPE_TIMING_PC_YCBCR,

    SOURCE_TYPE_TIMING_MAX
};
// End of SOURCE_TYPE_TIMING setting.
///////////////////////////////////////////////////////////////////////////////////


/* new API to get/set eaten porch decoder+nr+di+scpos
    accrording to current video timing.
    20070612 sv_porch_t*/
enum
{
    SV_HPORCH_CURRENT,
    SV_HPORCH_DEFAULT,
    SV_HPORCH_MIN,
    SV_HPORCH_MAX,
    SV_VPORCH_CURRENT,
    SV_VPORCH_DEFAULT,
    SV_VPORCH_MIN,
    SV_VPORCH_MAX
} ;

#define SV_PORCHTUNE_DEC    0x1    /* only get/set decoder porch */
#define SV_PORCHTUNE_SCPOS  0x2    /* only get/set scaler porch */
#define SV_PORCHTUNE_MIX    (SV_PORCHTUNE_DEC|SV_PORCHTUNE_SCPOS)    /* only get/set mix dec/scpos porch */


enum
{
    SV_HDMI_MODE_AUTO,
    SV_HDMI_MODE_GRAPHIC,
    SV_HDMI_MODE_VIDEO
};

enum
{
    SV_CHANGE_CHANNEL_MODE,
    SV_SCAN_MODE,
    SV_FINE_TUNE_MODE
};

enum
{
    SV_HDMI_RANGE_FORCE_AUTO = 0,
    SV_HDMI_RANGE_FORCE_LIMIT,
    SV_HDMI_RANGE_FORCE_FULL
};

enum
{
    SV_ANG_SCE_SAT,
    SV_ANG_SCE_HUE,
    SV_ANG_SCE_BRI
};

enum
{
    SV_FORCE_UNMUTE_TYPE_ON_NO_SIG,
    SV_FORCE_UNMUTE_TYPE_ATV_CHG_CH
};


/* new API to force color space */
#if 0
#define FIXCOLORSPACE_ATTRMASK 0xF0
#define FIXCOLORSPACE_ONCEMASK 0x10
#define FIXCOLORSPACE_WAITMASK 0x80

#define FIXCOLORSPACE_MODEMASK 0x0f
enum
{
    SV_FIXCOLORSPACE_OFF=0,        /* no force, means auto */
    /* color mode part */
    SV_FIXCOLORSPACE_RGB_444=1,        /* always RGB */
    SV_FIXCOLORSPACE_RGB_422=2,        /* always RGB */
    SV_FIXCOLORSPACE_YCBCR=3,        /* always YPBPR */
    SV_FIXCOLORSPACE_YCBCR_TO_RGB=4,        /* always YPBPR */
    SV_FIXCOLORSPACE_RGB_TO_YCBCR=5,
    /*  color mode once part*/
    SV_FIXCOLORSPACE_RBG444_ONCE=SV_FIXCOLORSPACE_RGB_444|FIXCOLORSPACE_ONCEMASK,    /* only force RGB for this mode detect,*/
    SV_FIXCOLORSPACE_RBG422_ONCE=SV_FIXCOLORSPACE_RGB_422|FIXCOLORSPACE_ONCEMASK,    /* only force RGB for this mode detect,*/
    SV_FIXCOLORSPACE_YCBCR_ONCE=SV_FIXCOLORSPACE_YCBCR|FIXCOLORSPACE_ONCEMASK,    /* only force YPBPR for this mode detect,*/
    SV_FIXCOLORSPACE_YCBCR_TO_RGB_ONCE=SV_FIXCOLORSPACE_YCBCR_TO_RGB|FIXCOLORSPACE_ONCEMASK,    /* only force YPBPR for this mode detect,*/
    SV_FIXCOLORSPACE_RGB_TO_YCBCR_ONCE=SV_FIXCOLORSPACE_RGB_TO_YCBCR|FIXCOLORSPACE_ONCEMASK
};
#endif

typedef enum
{
    SV_FIXCOLORSPACE_OFF=0,             /* no force, means auto */
    SV_FIXCOLORSPACE_RGB_444=1,         /* RGB TO RGB */
    SV_FIXCOLORSPACE_RGB_422=2,         /* RGB to YCBCR */    
    SV_FIXCOLORSPACE_YCBCR=3,           /* YCBCR TO YCBCR */
    SV_FIXCOLORSPACE_YCBCR_TO_RGB=4,    /* YCBCR TO RGB */
    SV_FIXCOLORSPACE_RGB_TO_YCBCR=5,    /* RGB to YCBCR */    
}SV_FIXCOLORSPACE_MODE;

enum {
    SV_DDDS_FRAMETRACK_FAST1=0,
	SV_DDDS_FRAMETRACK_FAST2,
	SV_DDDS_FRAMETRACK_SLOW1,
	SV_DDDS_FRAMETRACK_SLOW2
};

enum {
        SV_DISABLE_DEC_STABLE_CHK=0,
	SV_ENABLE_DEC_STABLE_CHK,
	SV_CHECK_DEC_HV
};

#define SV_BOOT_DEBUG_MAGIC 0xAE

#endif                /* _SV_CONST_H_ */

