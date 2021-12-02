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
 * $RCSfile: mttype.h,v $
 * $Revision: #4 $
 * $Author: xiongfeng.wen $
 * $SWAuthor:    $
 * $MD5HEX:  $
 *---------------------------------------------------------------------------*/

/** @file MtType.h
 *  This header file declares MediaTek TAL API type.
 */

#ifndef MT_TYPE_H
#define MT_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <x_typedef.h>
#ifdef __KERNEL__
    #include "x_linux.h"
    #include <linux/types.h>
    #include <linux/fs.h>
    /* this is for linux kernel */
#ifndef printf    
    #define printf(fmt...) printk(fmt)
#endif    
#else
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/types.h>
    #include <sys/ipc.h>
    #ifndef ANDROID_TOOLCHAIN
#ifndef ANDROID
        #include <sys/sem.h>
        #include <sys/shm.h>
    #endif
    #endif
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/mman.h>
    #include <errno.h>
    #include <sys/time.h>
    #include <time.h>
    #include <stdint.h>
#endif

extern unsigned long u4MtalLogMask;
extern UINT8 u1IfATVOnlyStatus;

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define MTADAC_LOG_MASK             (1 << 0)
#define MTAUDDEC_LOG_MASK           (1 << 1)
#define MTCI_LOG_MASK               (1 << 2)
#define MTDMX_LOG_MASK              (1 << 3)
#define MTFE_ATSC_LOG_MASK          (1 << 4)
#define MTFE_DVBT_LOG_MASK          (1 << 5)
#define MTFE_NTSC_LOG_MASK          (1 << 6)
#define MTFE_PAL_LOG_MASK           (1 << 7)
#define MTGFX_LOG_MASK              (1 << 8)
#define MTMPLAYER_LOG_MASK          (1 << 9)
#define MTOSD_LOG_MASK              (1 << 10)
#define MTPERIPHERAL_LOG_MASK       (1 << 11)
#define MTPMX_LOG_MASK              (1 << 12)
#define MTSCART_LOG_MASK            (1 << 13)
#define MTVDEC_LOG_MASK             (1 << 14)
#define MTVDECEX_LOG_MASK           (1 << 15)
#define MTVDO_LOG_MASK              (1 << 16)
#define MTVFE_LOG_MASK              (1 << 17)
#define MTVBIEX_LOG_MASK            (1 << 18)
#define MTTVE_LOG_MASK              (1 << 19)
#define MTCB_LOG_MASK              (1 << 20)
#define MTCOMMON_LOG_MASK     (1 << 21)
#define MTSC_LOG_MASK              (1 << 22)
#define MTFE_ISDB_LOG_MASK        (1 << 23)
#define MTFE_ISDBT_LOG_MASK      (1 << 24)
#define MTFE_ISDBS_LOG_MASK      (1 << 25)
#define MTFE_SBTVD_LOG_MASK      (1 << 26)
#define MTFE_DVBC_LOG_MASK          (1 << 27)
#define MTDREC_LOG_MASK              (1 << 28)
#define MTPVR_LOG_MASK              (1 << 29)
#define MTFE_DTMB_LOG_MASK          (1 << 30)
#define MTSTC_LOG_MASK              (1 << 31)
#define MTB2R_LOG_MASK              (1 << 32)
#define MTVSS_LOG_MASK              (1 << 33)
#define IS_MTADAC_LOG_ENABLE        ((u4MtalLogMask & MTADAC_LOG_MASK) > 0)
#define IS_MTAUDDEC_LOG_ENABLE      ((u4MtalLogMask & MTAUDDEC_LOG_MASK) > 0)
#define IS_MTCI_LOG_ENABLE          ((u4MtalLogMask & MTCI_LOG_MASK) > 0)
#define IS_MTDMX_LOG_ENABLE         ((u4MtalLogMask & MTDMX_LOG_MASK) > 0)
#define IS_MTFE_ATSC_LOG_ENABLE     ((u4MtalLogMask & MTFE_ATSC_LOG_MASK) > 0)
#define IS_MTFE_DVBT_LOG_ENABLE     ((u4MtalLogMask & MTFE_DVBT_LOG_MASK) > 0)
#define IS_MTFE_DVBC_LOG_ENABLE     ((u4MtalLogMask & MTFE_DVBC_LOG_MASK) > 0)
#define IS_MTFE_NTSC_LOG_ENABLE     ((u4MtalLogMask & MTFE_NTSC_LOG_MASK) > 0)
#define IS_MTFE_PAL_LOG_ENABLE      ((u4MtalLogMask & MTFE_PAL_LOG_MASK) > 0)
#define IS_MTGFX_LOG_ENABLE         ((u4MtalLogMask & MTGFX_LOG_MASK) > 0)
#define IS_MTMPLAYER_LOG_ENABLE     ((u4MtalLogMask & MTMPLAYER_LOG_MASK) > 0)
#define IS_MTOSD_LOG_ENABLE         ((u4MtalLogMask & MTOSD_LOG_MASK) > 0)
#define IS_MTPERIPHERAL_LOG_ENABLE  ((u4MtalLogMask & MTPERIPHERAL_LOG_MASK) > 0)
#define IS_MTPMX_LOG_ENABLE         ((u4MtalLogMask & MTPMX_LOG_MASK) > 0)
#define IS_MTSCART_LOG_ENABLE       ((u4MtalLogMask & MTSCART_LOG_MASK) > 0)
#define IS_MTVDEC_LOG_ENABLE        ((u4MtalLogMask & MTVDEC_LOG_MASK) > 0)
#define IS_MTVDECEX_LOG_ENABLE      ((u4MtalLogMask & MTVDECEX_LOG_MASK) > 0)
#define IS_MTVDO_LOG_ENABLE         ((u4MtalLogMask & MTVDO_LOG_MASK) > 0)
#define IS_MTVFE_LOG_ENABLE         ((u4MtalLogMask & MTVFE_LOG_MASK) > 0)
#define IS_MTVBIEX_LOG_ENABLE       ((u4MtalLogMask & MTVBIEX_LOG_MASK) > 0)
#define IS_MTTVE_LOG_ENABLE         ((u4MtalLogMask & MTTVE_LOG_MASK) > 0)
#define IS_MTCB_LOG_ENABLE         ((u4MtalLogMask & MTCB_LOG_MASK) > 0)
#define IS_MTCOMMON_LOG_ENABLE       ((u4MtalLogMask & MTCOMMON_LOG_MASK) > 0)
#define IS_MTSC_LOG_ENABLE         ((u4MtalLogMask & MTSC_LOG_MASK) > 0)
#define IS_MTFE_ISDB_LOG_ENABLE     ((u4MtalLogMask & MTFE_ISDB_LOG_MASK) > 0)
#define IS_MTFE_ISDBT_LOG_ENABLE     ((u4MtalLogMask & MTFE_ISDBT_LOG_MASK) > 0)
#define IS_MTFE_ISDBS_LOG_ENABLE     ((u4MtalLogMask & MTFE_ISDBS_LOG_MASK) > 0)
#define IS_MTFE_SBTVD_LOG_ENABLE     ((u4MtalLogMask & MTFE_SBTVD_LOG_MASK) > 0)
#define IS_MTDREC_LOG_ENABLE         ((u4MtalLogMask & MTDREC_LOG_MASK) > 0)
#define IS_MTPVR_LOG_ENABLE          ((u4MtalLogMask & MTPVR_LOG_MASK) > 0)
#define IS_MTFE_DTMB_LOG_ENABLE     ((u4MtalLogMask & MTFE_DTMB_LOG_MASK) > 0)
#define IS_MTSTC_LOG_ENABLE         ((u4MtalLogMask & MTB2R_LOG_MASK) > 0)
#define IS_MTB2R_LOG_ENABLE         ((u4MtalLogMask & MTSTC_LOG_MASK) > 0)
#define IS_MTVSS_LOG_ENABLE         ((u4MtalLogMask & MTVSS_LOG_MASK) > 0)

#define MTAL_LOG(ENABLE, MOD, fmt...) \
    do \
    {\
        if((ENABLE)) \
        { \
            printf("["MOD"]:%s\n" , __FUNCTION__); \
            printf(fmt); \
        } \
									} while (0)


#define MTADAC_PRINT(fmt...)          MTAL_LOG(IS_MTADAC_LOG_ENABLE, "MTADAC", fmt)
#define MTAUDDEC_PRINT(fmt...)        MTAL_LOG(IS_MTAUDDEC_LOG_ENABLE, "MTAUDDEC", fmt)
#define MTCI_PRINT(fmt...)            MTAL_LOG(IS_MTCI_LOG_ENABLE, "MTCI", fmt)
#define MTDMX_PRINT(fmt...)           MTAL_LOG(IS_MTDMX_LOG_ENABLE, "MTDMX", fmt)
#define MTFE_ATSC_PRINT(fmt...)       MTAL_LOG(IS_MTFE_ATSC_LOG_ENABLE, "MTFE_ATSC", fmt)
#define MTFE_ISDB_PRINT(fmt...)       MTAL_LOG(IS_MTFE_ISDB_LOG_ENABLE, "MTFE_ISDB", fmt)
#define MTFE_ISDBT_PRINT(fmt...)       MTAL_LOG(IS_MTFE_ISDBT_LOG_ENABLE, "MTFE_ISDBT", fmt)
#define MTFE_ISDBS_PRINT(fmt...)       MTAL_LOG(IS_MTFE_ISDBS_LOG_ENABLE, "MTFE_ISDBS", fmt)
#define MTFE_DVBT_PRINT(fmt...)       MTAL_LOG(IS_MTFE_DVBT_LOG_ENABLE, "MTFE_DVBT", fmt)
#define MTFE_DVBC_PRINT(fmt...)       MTAL_LOG(IS_MTFE_DVBC_LOG_ENABLE, "MTFE_DVBC", fmt)
#define MTFE_DTMB_PRINT(fmt...)       MTAL_LOG(IS_MTFE_DTMB_LOG_ENABLE, "MTFE_DTMB", fmt)
#define MTFE_NTSC_PRINT(fmt...)       MTAL_LOG(IS_MTFE_NTSC_LOG_ENABLE, "MTFE_NTSC", fmt)
#define MTFE_PAL_PRINT(fmt...)        MTAL_LOG(IS_MTFE_PAL_LOG_ENABLE, "MTFE_PAL", fmt)
#define MTFE_SBTVD_PRINT(fmt...)      MTAL_LOG(IS_MTFE_SBTVD_LOG_ENABLE, "MTFE_SBTVD", fmt)
#define MTGFX_PRINT(fmt...)           MTAL_LOG(IS_MTGFX_LOG_ENABLE, "MTGFX", fmt)
#define MTMPLAYER_PRINT(fmt...)       MTAL_LOG(IS_MTMPLAYER_LOG_ENABLE, "MTMPLAYE", fmt)
#define MTOSD_PRINT(fmt...)           MTAL_LOG(IS_MTOSD_LOG_ENABLE, "MTOSD", fmt)
#define MTPERIPHERAL_PRINT(fmt...)    MTAL_LOG(IS_MTPERIPHERAL_LOG_ENABLE, "MTPERIPHERAL", fmt)
#define MTPMX_PRINT(fmt...)           MTAL_LOG(IS_MTPMX_LOG_ENABLE, "MTPMX", fmt)
#define MTSCART_PRINT(fmt...)         MTAL_LOG(IS_MTSCART_LOG_ENABLE, "MTSCART", fmt)
#define MTVDEC_PRINT(fmt...)          MTAL_LOG(IS_MTVDEC_LOG_ENABLE, "MTVDEC", fmt)
#define MTVDECEX_PRINT(fmt...)        MTAL_LOG(IS_MTVDECEX_LOG_ENABLE, "MTVDECEX", fmt)
#define MTVDO_PRINT(fmt...)           MTAL_LOG(IS_MTVDO_LOG_ENABLE, "MTVDO", fmt)
#define MTVFE_PRINT(fmt...)           MTAL_LOG(IS_MTVFE_LOG_ENABLE, "MTVFE", fmt)									
#define MTVBIEX_PRINT(fmt...)         MTAL_LOG(IS_MTVBIEX_LOG_ENABLE, "MTVBIEX", fmt)									
#define MTTVE_PRINT(fmt...)           MTAL_LOG(IS_MTTVE_LOG_ENABLE, "MTTVE", fmt)	
#define MTCB_PRINT(fmt...)           MTAL_LOG(IS_MTCB_LOG_ENABLE, "MTCB", fmt)									
#define MTCOMMON_PRINT(fmt...)         MTAL_LOG(IS_MTCOMMON_LOG_ENABLE, "MTCOMMON", fmt)									
#define MTSC_PRINT(fmt...)           MTAL_LOG(IS_MTSC_LOG_ENABLE, "MTSC", fmt)	
#define MTDREC_PRINT(fmt...)           MTAL_LOG(IS_MTDREC_LOG_ENABLE, "MTDREC", fmt)
#define MTPVR_PRINT(fmt...)           MTAL_LOG(IS_MTPVR_LOG_ENABLE, "MTPVR", fmt)	
#define MTSTC_PRINT(fmt...)           MTAL_LOG(IS_MTSTC_LOG_ENABLE, "MTSTC", fmt)
#define MTB2R_PRINT(fmt...)           MTAL_LOG(IS_MTB2R_LOG_ENABLE, "MTB2R", fmt)
#define MTVSS_PRINT(fmt...)           MTAL_LOG(IS_MTVSS_LOG_ENABLE, "MTVSS", fmt)	

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef enum
{
	MTPQL_HDMI_RGB = 0,
	MTPQL_HDMI_YCBCR,
	MTPQL_HDMI_XVYCC,
	MTPQL_DVI_RGB,
	MTPQL_HDMI_UNKNOWN,
}MTPQL_HDMI_COLOR_DOMAIN_T;

/**MTAL error code
*/
typedef enum
{
    MTR_BUFFER_FULL           =  5,
    MTR_CARD_READY            =  4,
    MTR_POD_CI_ACTIVE         =  3,
    MTR_DA_HIGH               =  2,
    MTR_CARD_DETECTED         =  1,
    MTR_OK                    =  0,   ///< Success
    MTR_NOT_OK                = -1,   ///< Not ok
    MTR_NOT_SUPPORTED         = -2,   ///< Not support
    MTR_PARAMETER_ERROR       = -3,   ///< Parameter error
    MTR_DONT_CARE             = -4,   ///< Don't care
    MTR_ERR_INV               = -5,   ///< Invalid argument passed
    MTR_ERR_TIMEDOUT          = -6,    ///< Timeout occured
    MTR_PARAMETER_NOCHANGE    = -7,  /// Paramenter no change

    MTR_OSD_NO_REGION         = -11,  ///< No region attach
    MTR_OSD_INV_REGION        = -12,  ///< Invalid region
    MTR_OSD_INV_LIST          = -13,  ///< Invalid list
    MTR_OSD_INV_PLANE         = -14,  ///< Invalid plane
    MTR_OSD_INV_SCALER        = -15,  ///< Invalid scaler
    MTR_OSD_INV_ATTRIBUTE     = -16  ///< Invalid region attribute
} MT_RESULT_T;

/**The video codec format
*/
typedef enum
{
    MTVDEC_FMT_MPEG1_MPEG2,
    MTVDEC_FMT_MPEG4,
    MTVDEC_FMT_H264,
    MTVDEC_FMT_VC1,
    MTVDEC_FMT_RM,
    MTVDEC_FMT_MJPEG,
    MTVDEC_FMT_AVS,
    MTVDEC_FMT_VP8,
    MTVDEC_FMT_VP6,
    MTVDEC_FMT_UNKNOWN,    
    MTVDEC_FMT_MAX
}MTVDEC_FMT_T;


/** Video Path
*/
typedef enum {
  MTVDO_MAIN,
  MTVDO_SUB,
  MTVDO_MAX
} MTVDO_PATH_T;

/**Audio decoder format.
*/
typedef enum
{
    MTAUDDEC_FMT_UNKNOWN = 0,
    MTAUDDEC_FMT_MPEG,
    MTAUDDEC_FMT_AC3,
    MTAUDDEC_FMT_PCM,
    MTAUDDEC_FMT_MP3,
    MTAUDDEC_FMT_AAC,
    MTAUDDEC_FMT_DTS,
    MTAUDDEC_FMT_WMA,
    MTAUDDEC_FMT_RA,
    MTAUDDEC_FMT_HDCD,
    MTAUDDEC_FMT_MLP,
    MTAUDDEC_FMT_MTS,          ///< For SIF_BTSC
    MTAUDDEC_FMT_A2,           ///< For SIF_BG_A2, SIF_DK1_A2, SIF_DK2_A2, and SIF_DK3_A2
    MTAUDDEC_FMT_EIAJ,         ///< For SIF_EIAJ
    MTAUDDEC_FMT_NICAM,        ///< For SIF_BG, SIF_I, SIF_DK, and SIF_L
    MTAUDDEC_FMT_NUM
}MTAUDDEC_FMT_T;

/**AV sync mode.
*/
typedef enum  
{
    MTAV_SYNC_FREE_RUN = 0,   ///< no need to syn
    MTAV_SYNC_SLAVE,          ///< syn to STC
    MTAV_SYNC_MASTER    ///< update A-PTS to A-STC, may use in PCR or hard disc playback
}MTAV_SYNC_MODE_T;

/** HDMI
*/
enum 
{
  MTHDMI_InfoFrame_AVI,
  MTHDMI_InfoFrame_SPD,
  MTHDMI_InfoFrame_AUDIO,
  MTHDMI_InfoFrame_MPEG,
  MTHDMI_InfoFrame_UNREC,
  MTHDMI_InfoFrame_GAMUT,
  MTHDMI_InfoFrame_ACP,
  MTHDMI_InfoFrame_VSDB
};

typedef struct
{
  UINT8 HDMI_FrameSize;
  UINT32 HDMI_FrameData[10];
}MTVDECEX_HDMI_INFOFRAME;


/**caption format.
*/
typedef enum
{
    MTMPLAYER_CAPTION_MPEG2 = 0,
    MTMPLAYER_CAPTION_AVI_TEXT,
    MTMPLAYER_CAPTION_AVI_BITMAP
}MTMPLAYER_CAPTION_FMT_T;

/* PCM channel pair*/
typedef enum
{
    MTAUDDEC_PCM_CH_L_R = 0,
    MTAUDDEC_PCM_CH_LS_RS,
    MTAUDDEC_PCM_CH_C_SW,
    MTAUDDEC_PCM_CH_BYPASS_L_R,
    MTAUDDEC_PCM_CH_DOWNMIX_L_R,
    MTAUDDEC_PCM_CH_SPEC_BYPASS_L_R, ///< Specific bypass L/R output
    MTAUDDEC_PCM_CH_TV_SCART_L_R,    ///< 2nd(TV) SCART output
    MTAUDDEC_PCM_CH_HP_L_R,
    MTAUDDEC_PCM_CH_MAX
}   MTAUDDEC_PCM_CH_T;


typedef enum MTVDO_3DMode
{

    MTVDO_3DMode_OFF= 0X0, 
    MTVDO_3DMode_AUTO= 0x1,
    MTVDO_3DMode_TTD  =0X2,//2D -> 3D Conversion
    MTVDO_3DMode_SBS= 0x4, 
    MTVDO_3DMode_TB= 0x5,
    MTVDO_3DMode_MAX= 0xFF    
    
} MTVDO_3DMode;

typedef enum MTVDO_2DLRMode
{
    
    MTVDO_3D_2_2D_ISOFF  =0,
    MTVDO_3D_2_2D_L    =1,
    MTVDO_3D_2_2D_R    =2
    
} MTVDO_2DLRMode;

typedef enum MTVDO_3DInverse
{    
   MTVDO_3D_LR_NORMAL  =0,
   MTVDO_3D_LR_INVERSE =1    
    
} MTVDO_3DInverse;


typedef enum MTVDO_3DDepth
{    
   MTVDO_3D_DEPTH_LOW  =0,
   MTVDO_3D_DEPTH_MID  =1,
   MTVDO_3D_DEPTH_HIGH =2,
   MTVDO_3D_DEPTH_OFF  =3,
    
} MTVDO_3DDepth;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif //MT_TYPE_H
