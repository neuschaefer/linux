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
/*----------------------------------------------------------------------------*/
/*! @file u_scc.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Stream Component Control specific
 *          definitions, which are exported to applications.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_STRM_MNGR_SCC
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SCC_
#define _U_SCC_    /**<        */


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_sm.h"
#include "u_sm_audio_hdlr.h"
#include "u_sm_video_hdlr.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/* component group command sets **********************************************/
/* primary commands */
#define SCC_CMD_CODE_GRP_TYPE                           \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 0))    /**<        */

/* subsequence commands, all */
#define SCC_CMD_CODE_NAME                    ((SM_CMD_CODE_T) 16)    /**<        */
#define SCC_CMD_CODE_STRM_HANDLE             ((SM_CMD_CODE_T) 17)    /**<        */

/* subsequence commands, SCC_CMD_TYPE_VIDEO */
#define SCC_CMD_CODE_VIDEO_BACKGROUND        ((SM_CMD_CODE_T) 48)    /**<        */
#define SCC_CMD_CODE_VIDEO_MODE              ((SM_CMD_CODE_T) 49)    /**<        */
#define SCC_CMD_CODE_VIDEO_SCREEN_MODE       ((SM_CMD_CODE_T) 50)    /**<        */
#define SCC_CMD_CODE_VIDEO_ENHANCE           ((SM_CMD_CODE_T) 51)    /**<        */
#define SCC_CMD_CODE_VIDEO_BLUR              ((SM_CMD_CODE_T) 52)    /**<        */
#define SCC_CMD_CODE_VIDEO_OVERSCAN_CLIPPER  ((SM_CMD_CODE_T) 53)    /**<        */
#define SCC_CMD_CODE_VIDEO_CTI               ((SM_CMD_CODE_T) 54)    /**<        */
#define SCC_CMD_CODE_VIDEO_ETI               ((SM_CMD_CODE_T) 55)    /**<        */
#define SCC_CMD_CODE_VIDEO_SHARPNESS         ((SM_CMD_CODE_T) 56)    /**<        */
#define SCC_CMD_CODE_VIDEO_COLOR_SUPPRESS    ((SM_CMD_CODE_T) 57)    /**<        */
#define SCC_CMD_CODE_VIDEO_NR                ((SM_CMD_CODE_T) 58)    /**<        */
#define SCC_CMD_CODE_VIDEO_BLACK_LVL_EXT     ((SM_CMD_CODE_T) 59)    /**<        */
#define SCC_CMD_CODE_VIDEO_WHITE_PEAK_RDCT   ((SM_CMD_CODE_T) 60)    /**<        */
#define SCC_CMD_CODE_VIDEO_FLESH_TONE        ((SM_CMD_CODE_T) 61)    /**<        */
#define SCC_CMD_CODE_VIDEO_LUMA              ((SM_CMD_CODE_T) 62)    /**<        */
#define SCC_CMD_CODE_VIDEO_3D_NR             ((SM_CMD_CODE_T) 63)    /**<        */
#define SCC_CMD_CODE_VIDEO_LETTER_BOX_DETECT ((SM_CMD_CODE_T) 64)    /**<        */
#define SCC_CMD_CODE_VIDEO_DYNAMIC_SCALING   ((SM_CMD_CODE_T) 65)    /**<        */
#define SCC_CMD_CODE_VIDEO_DEBLOCKING        ((SM_CMD_CODE_T) 66)    /**<        */
#define SCC_CMD_CODE_VIDEO_SPLIT_SCRN_DEMO   ((SM_CMD_CODE_T) 67)    /**<        */
#define SCC_CMD_CODE_VIDEO_H_POSITION        ((SM_CMD_CODE_T) 68)    /**<        */
#define SCC_CMD_CODE_VIDEO_V_POSITION        ((SM_CMD_CODE_T) 69)    /**<        */
#define SCC_CMD_CODE_VIDEO_H_SIZE            ((SM_CMD_CODE_T) 70)    /**<        */
#define SCC_CMD_CODE_VIDEO_V_SIZE            ((SM_CMD_CODE_T) 71)    /**<        */
#define SCC_CMD_CODE_VIDEO_FILM_MODE         ((SM_CMD_CODE_T) 72)    /**<        */
#define SCC_CMD_CODE_VIDEO_FORCE_UNMUTE_ON_NO_SIG ((SM_CMD_CODE_T) 73)    /**<        */
#define SCC_CMD_CODE_VIDEO_PIC_MODE          ((SM_CMD_CODE_T) 74)    /**<        */
#define SCC_CMD_CODE_VIDEO_HDMI_MODE         ((SM_CMD_CODE_T) 75)    /**<        */
#define SCC_CMD_CODE_VIDEO_COLOR_SYS         ((SM_CMD_CODE_T) 76)    /**<        */
#define SCC_CMD_CODE_VIDEO_LB_DETECT_CONFIG  ((SM_CMD_CODE_T) 77)    /**<        */
#define SCC_CMD_CODE_VIDEO_RESET_VGA_TIMING_OPTION ((SM_CMD_CODE_T) 78)    /**<        */
#define SCC_CMD_CODE_VIDEO_AMBIG_VGA_TIMING_CTRL ((SM_CMD_CODE_T) 79)    /**<        */
#define SCC_CMD_CODE_VIDEO_DI_FILM_MODE      ((SM_CMD_CODE_T) 80) /* ui4_data */    /**<        */
#define SCC_CMD_CODE_VIDEO_DI_MA             ((SM_CMD_CODE_T) 81) /* ui4_data */    /**<        */
#define SCC_CMD_CODE_VIDEO_DI_EDGE_PRESERVE  ((SM_CMD_CODE_T) 82) /* ui4_data */    /**<        */
#define SCC_CMD_CODE_VIDEO_MJC               ((SM_CMD_CODE_T) 83) /* pv_data */    /**<        */
#define SCC_CMD_CODE_VIDEO_XVYCC             ((SM_CMD_CODE_T) 84) /* ui4_data */    /**<        */
#define SCC_CMD_CODE_VIDEO_WCG               ((SM_CMD_CODE_T) 85) /* ui4_data */    /**<        */
#define SCC_CMD_CODE_VIDEO_GAME_MODE         ((SM_CMD_CODE_T) 86) /* ui4_data */    /**<        */
#define SCC_CMD_CODE_VIDEO_BLUE_STRETCH      ((SM_CMD_CODE_T) 87) /* ui4_data */    /**<        */
#define SCC_CMD_CODE_VIDEO_MPEG_NR           ((SM_CMD_CODE_T) 88) /* ui4_data */    /**<        */
#define SCC_CMD_CODE_VIDEO_SUPER_BLANK       ((SM_CMD_CODE_T) 89) /* pv_data */    /**<        */
#define SCC_CMD_CODE_VIDEO_MODE_EX           ((SM_CMD_CODE_T) 90)    /**<        */
#define SCC_CMD_CODE_VIDEO_SIG_BOOSTER       ((SM_CMD_CODE_T) 91)    /**<        */
#define SCC_CMD_CODE_VIDEO_BLACK_STRETCH     ((SM_CMD_CODE_T) 92)    /**<        */
#define SCC_CMD_CODE_VIDEO_WHITE_STRETCH     ((SM_CMD_CODE_T) 93)    /**<        */
#define SCC_CMD_CODE_VIDEO_DISP_REGION       ((SM_CMD_CODE_T) 94)    /**<        */
#define SCC_CMD_CODE_VIDEO_VDP_ATOMIC_GRP    ((SM_CMD_CODE_T) 95)    /**<        */
#define SCC_CMD_CODE_VIDEO_COLORSPACE        ((SM_CMD_CODE_T) 96)    /**<        */
#define SCC_CMD_CODE_VIDEO_FLIP              ((SM_CMD_CODE_T) 97)    /**<        */
#define SCC_CMD_CODE_VIDEO_MIRROR            ((SM_CMD_CODE_T) 98)    /**<        */
#define SCC_CMD_CODE_VIDEO_SUPER_FREEZE      ((SM_CMD_CODE_T) 99)    /**<        */
#define SCC_CMD_CODE_VIDEO_SIG_BOOSTER_FLAG  ((SM_CMD_CODE_T) 100)   /**<        */
#define SCC_CMD_CODE_VIDEO_BLACK_STRETCH_VAL ((SM_CMD_CODE_T) 101)    /**<        */
#define SCC_CMD_CODE_VIDEO_WHITE_STRETCH_VAL ((SM_CMD_CODE_T) 102)    /**<        */
#define SCC_CMD_CODE_VIDEO_3D_MODE           ((SM_CMD_CODE_T) 103)    /**<        */
#define SCC_CMD_CODE_VIDEO_3D_PARALLAX       ((SM_CMD_CODE_T) 104)    /**<        */
#define SCC_CMD_CODE_VIDEO_3D_FLD_DEPTH      ((SM_CMD_CODE_T) 105)    /**<        */
#define SCC_CMD_CODE_VIDEO_3D_LR_SWITCH      ((SM_CMD_CODE_T) 106)    /**<        */
#define SCC_CMD_CODE_VIDEO_3D_TO_2D          ((SM_CMD_CODE_T) 107)    /**<        */
#define SCC_CMD_CODE_VIDEO_3D_FPR            ((SM_CMD_CODE_T) 108)    /**<        */

/* subsequence commands, SCC_CMD_TYPE_AUDIO */
#define SCC_CMD_CODE_AUDIO_VOLUME            ((SM_CMD_CODE_T) 80)    /**<        */
#define SCC_CMD_CODE_AUDIO_MUTE              ((SM_CMD_CODE_T) 81)    /**<        */
#define SCC_CMD_CODE_AUDIO_SPDIF_TYPE        ((SM_CMD_CODE_T) 82)    /**<        */
#define SCC_CMD_CODE_AUDIO_HDCD_FLTR         ((SM_CMD_CODE_T) 83)    /**<        */
#define SCC_CMD_CODE_AUDIO_LOUD_SPEAKER      ((SM_CMD_CODE_T) 84)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_TRIM           ((SM_CMD_CODE_T) 85)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_SUPERBASS      ((SM_CMD_CODE_T) 86)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_EQUALIZER      ((SM_CMD_CODE_T) 87)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_REVERB         ((SM_CMD_CODE_T) 88)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_BASS           ((SM_CMD_CODE_T) 89)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_TREBLE         ((SM_CMD_CODE_T) 90)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_BALANCE        ((SM_CMD_CODE_T) 91)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_POSTDR         ((SM_CMD_CODE_T) 92)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_VOLUME         ((SM_CMD_CODE_T) 93)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_SURROUND       ((SM_CMD_CODE_T) 94)    /**<        */
#define SCC_CMD_CODE_AUDIO_DOWNMIX_MODE      ((SM_CMD_CODE_T) 95)    /**<        */
#define SCC_CMD_CODE_AUDIO_DOLBY_DRC         ((SM_CMD_CODE_T) 96)    /**<        */
#define SCC_CMD_CODE_AUDIO_OUT_PORT          ((SM_CMD_CODE_T) 97)    /**<        */
#define SCC_CMD_CODE_AUDIO_CMPSS_MODE        ((SM_CMD_CODE_T) 98)    /**<        */
#define SCC_CMD_CODE_AUDIO_AGC               ((SM_CMD_CODE_T) 99)    /**<        */
#define SCC_CMD_CODE_AUDIO_CHNL_DELAY        ((SM_CMD_CODE_T)100)    /**<        */
#define SCC_CMD_CODE_AUDIO_MTS               ((SM_CMD_CODE_T)101)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_EQ_USER_LVL    ((SM_CMD_CODE_T)102)    /**<        */
#define SCC_CMD_CODE_AUDIO_PL2_SWITCH        ((SM_CMD_CODE_T)103)    /**<        */
#define SCC_CMD_CODE_AUDIO_PL2_MODE          ((SM_CMD_CODE_T)104)    /**<        */
#define SCC_CMD_CODE_AUDIO_PL2_PANORAMA      ((SM_CMD_CODE_T)105)    /**<        */
#define SCC_CMD_CODE_AUDIO_PL2_DIMENSION     ((SM_CMD_CODE_T)106)    /**<        */
#define SCC_CMD_CODE_AUDIO_PL2_C_WIDTH       ((SM_CMD_CODE_T)107)    /**<        */
#define SCC_CMD_CODE_AUDIO_HDPHONE_NFY       ((SM_CMD_CODE_T)108)    /**<        */
#define SCC_CMD_CODE_AUDIO_MUTE_EX           ((SM_CMD_CODE_T)109)    /**<        */
#define SCC_CMD_CODE_AUDIO_VOLUME_EX         ((SM_CMD_CODE_T)110)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_EQ_USER_LVL_EX ((SM_CMD_CODE_T)111)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_SURROUND_EX       ((SM_CMD_CODE_T) 112)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_LINOUT_VOL_MODE       ((SM_CMD_CODE_T) 113)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_SPH_MODE       ((SM_CMD_CODE_T) 114)    /**<        */
#define SCC_CMD_CODE_AUDIO_VOLUME_OFFSET      ((SM_CMD_CODE_T) 115)    /**<        */
#define SCC_CMD_CODE_AUDIO_SET_PEQ_INFO       ((SM_CMD_CODE_T) 116)    /**<        */
#define SCC_CMD_CODE_AUDIO_SE_SOUND_MODE       ((SM_CMD_CODE_T) 117)    /**<        */
#define SCC_CMD_CODE_AUDIO_SET_OVM                 ((SM_CMD_CODE_T) 118)    /**<        */
#define SCC_CMD_CODE_AUDIO_CLIP_VOLUME            ((SM_CMD_CODE_T) 119)    /**<        */



/* subsequence commands, SCC_CMD_TYPE_DISP */
#define SCC_CMD_CODE_DISP_CTRL               ((SM_CMD_CODE_T)112)    /**<        */
#define SCC_CMD_CODE_DISP_BRIGHTNESS         ((SM_CMD_CODE_T)113)    /**<        */
#define SCC_CMD_CODE_DISP_CONTRAST           ((SM_CMD_CODE_T)114)    /**<        */
#define SCC_CMD_CODE_DISP_HUE                ((SM_CMD_CODE_T)115)    /**<        */
#define SCC_CMD_CODE_DISP_SATURATION         ((SM_CMD_CODE_T)116)    /**<        */
#define SCC_CMD_CODE_DISP_GAMMA              ((SM_CMD_CODE_T)117)    /**<        */
#define SCC_CMD_CODE_DISP_RESOLUTION         ((SM_CMD_CODE_T)118)    /**<        */
#define SCC_CMD_CODE_DISP_ASPECT_RATIO       ((SM_CMD_CODE_T)119)    /**<        */
#define SCC_CMD_CODE_DISP_FMT                ((SM_CMD_CODE_T)120)    /**<        */
#define SCC_CMD_CODE_DISP_DIGIT_OUTPUT       ((SM_CMD_CODE_T)121)    /**<        */
#define SCC_CMD_CODE_DISP_BACKGROUND         ((SM_CMD_CODE_T)122)    /**<        */
#define SCC_CMD_CODE_DISP_TV_TYPE            ((SM_CMD_CODE_T)123)    /**<        */
#define SCC_CMD_CODE_DISP_DIGIT_OUTPUT_FMT   ((SM_CMD_CODE_T)124)    /**<        */
#define SCC_CMD_CODE_DISP_DITHER             ((SM_CMD_CODE_T)125)    /**<        */
#define SCC_CMD_CODE_DISP_COLOR_GAIN         ((SM_CMD_CODE_T)126)    /**<        */
#define SCC_CMD_CODE_DISP_COLOR_OFFSET       ((SM_CMD_CODE_T)127)    /**<        */
#define SCC_CMD_CODE_DISP_BACK_LIGHT         ((SM_CMD_CODE_T)128)    /**<        */
#define SCC_CMD_CODE_DISP_ADP_BACK_LIGHT     ((SM_CMD_CODE_T)129)    /**<        */
#define SCC_CMD_CODE_DISP_TV_MODE            ((SM_CMD_CODE_T)130)    /**<        */
#define SCC_CMD_CODE_DISP_MJC                ((SM_CMD_CODE_T)131)    /**<        */
#define SCC_CMD_CODE_DISP_TEST_PATTERN       ((SM_CMD_CODE_T)132)    /**<        */
#define SCC_CMD_CODE_DISP_BACK_LIGHT_CTRL    ((SM_CMD_CODE_T)133)    /**<        */
#ifdef MW_TV_AV_BYPASS_SUPPORT
#define SCC_CMD_CODE_DISP_BYPASS_MODE        ((SM_CMD_CODE_T)134)    /**<        */
#endif
#define SCC_CMD_CODE_DISP_ADP_BACK_LIGHT_RNG ((SM_CMD_CODE_T)135)    /**<        */
#define SCC_CMD_CODE_DISP_BACK_LIGHT_ENERGY_SAVE ((SM_CMD_CODE_T)136)    /**<        */
#ifdef MW_TV_AV_BYPASS_SUPPORT
#define SCC_CMD_CODE_DISP_TVE_CTRL_MODE      ((SM_CMD_CODE_T)137)    /**<        */
#endif

/* MAX value of all attributes defined by SCC API */
#define SCC_ATTRIB_VALUE_MAX                ((UINT16) 10000)    /**<        */

/*------------------------------------------------------------------*/
/*! @enum SCC_AUD_BBE_MODE_T 
 *  @brief  Specify post-processing mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_BBE_MODE_OFF = 0,
 *      SCC_AUD_BBE_MODE_BBE,
 *      SCC_AUD_BBE_MODE_BBE_VIVA
 *  }   SCC_AUD_BBE_MODE_T;
 *  @endcode
 *  @li@c  SCC_AUD_BBE_MODE_OFF                              -post-processing mode off
 *  @li@c  SCC_AUD_BBE_MODE_BBE                              -post-processing mode BBE type.
 *  @li@c  SCC_AUD_BBE_MODE_BBE_VIVA                         -post-processing mode VIVA type.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_BBE_MODE_OFF = 0,
    SCC_AUD_BBE_MODE_BBE,
    SCC_AUD_BBE_MODE_BBE_VIVA
}   SCC_AUD_BBE_MODE_T;

/* Increase/Decrease arguments */
/* SCC_CMD_CODE_DISP_COLOR_GAIN */
/*------------------------------------------------------------------*/
/*! @enum SCC_DISP_COLOR_GAIN_ARG_T
 *  @brief  Specify color gain argument
 *  @code
 *  typedef enum
 *  {
 *      SCC_DISP_COLOR_GAIN_ARG_R = 0,
 *      SCC_DISP_COLOR_GAIN_ARG_G,
 *      SCC_DISP_COLOR_GAIN_ARG_B
 *  } SCC_DISP_COLOR_GAIN_ARG_T;
 *  @endcode
 *  @li@c  SCC_DISP_COLOR_GAIN_ARG_R                         -color gain argument R
 *  @li@c  SCC_DISP_COLOR_GAIN_ARG_G                         -color gain argument G
 *  @li@c  SCC_DISP_COLOR_GAIN_ARG_B                         -color gain argument B
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_DISP_COLOR_GAIN_ARG_R = 0,
    SCC_DISP_COLOR_GAIN_ARG_G,
    SCC_DISP_COLOR_GAIN_ARG_B
} SCC_DISP_COLOR_GAIN_ARG_T;


/* Common definitions */
/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_CHANNEL_T
 *  @brief  Specify audio channel .
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_CHANNEL_ALL = 0,
 *      SCC_AUD_CHANNEL_FRONT_LEFT,
 *      SCC_AUD_CHANNEL_FRONT_RIGHT,
 *      SCC_AUD_CHANNEL_REAR_LEFT,
 *      SCC_AUD_CHANNEL_REAR_RIGHT,
 *      SCC_AUD_CHANNEL_CENTER,
 *      SCC_AUD_CHANNEL_SUB_WOOFER
 *  } SCC_AUD_CHANNEL_T;
 *  @endcode
 *  @li@c  SCC_AUD_CHANNEL_ALL                               -operation for all channel.
 *  @li@c  SCC_AUD_CHANNEL_FRONT_LEFT                        -operation for front left channel.
 *  @li@c  SCC_AUD_CHANNEL_FRONT_RIGHT                       -operation for front right channel.
 *  @li@c  SCC_AUD_CHANNEL_REAR_LEFT                         -operation for rear left channel.
 *  @li@c  SCC_AUD_CHANNEL_REAR_RIGHT                        -operation for rear right channel.
 *  @li@c  SCC_AUD_CHANNEL_CENTER                            -operation for center channel.
 *  @li@c  SCC_AUD_CHANNEL_SUB_WOOFER                        -operation for sub-woofer channel.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_CHANNEL_ALL = 0,
    SCC_AUD_CHANNEL_FRONT_LEFT,
    SCC_AUD_CHANNEL_FRONT_RIGHT,
    SCC_AUD_CHANNEL_REAR_LEFT,
    SCC_AUD_CHANNEL_REAR_RIGHT,
    SCC_AUD_CHANNEL_CENTER,
    SCC_AUD_CHANNEL_SUB_WOOFER
} SCC_AUD_CHANNEL_T;

/*------------------------------------------------------------------*/
/*! @enum SCC_DISP_ATTRIB_T
 *  @brief  Specify display attribute type
 *  @code
 *  typedef enum
 *  {
 *      SCC_DISP_ATTRIB_BRIGHTNESS,
 *      SCC_DISP_ATTRIB_CONTRAST,
 *      SCC_DISP_ATTRIB_HUE,
 *      SCC_DISP_ATTRIB_SATURATION,
 *      SCC_DISP_ATTRIB_GAMMA,
 *      SCC_DISP_ATTRIB_DITHER,
 *      SCC_DISP_ATTRIB_BACK_LIGHT,
 *      SCC_DISP_ATTRIB_ADP_BACK_LIGHT,
 *      SCC_DISP_ATTRIB_BACK_LIGHT_ENERGY_SAVE
 *  } SCC_DISP_ATTRIB_T;
 *  @endcode
 *  @li@c  SCC_DISP_ATTRIB_BRIGHTNESS                         -display attribute brightness
 *  @li@c  SCC_DISP_ATTRIB_CONTRAST                            -display attribute contrast
 *  @li@c  SCC_DISP_ATTRIB_HUE                                      -display attribute hue
 *  @li@c  SCC_DISP_ATTRIB_SATURATION                         -display attribute saturation
 *  @li@c  SCC_DISP_ATTRIB_GAMMA                                 -display attribute gamma
 *  @li@c  SCC_DISP_ATTRIB_DITHER                                 -display attribute dither
 *  @li@c  SCC_DISP_ATTRIB_BACK_LIGHT                         -display attribute back light
 *  @li@c  SCC_DISP_ATTRIB_ADP_BACK_LIGHT                 -display attribute adapt back light
 *  @li@c  SCC_DISP_ATTRIB_BACK_LIGHT_ENERGY_SAVE  -display attribute back light energy save
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_DISP_ATTRIB_BRIGHTNESS,
    SCC_DISP_ATTRIB_CONTRAST,
    SCC_DISP_ATTRIB_HUE,
    SCC_DISP_ATTRIB_SATURATION,
    SCC_DISP_ATTRIB_GAMMA,
    SCC_DISP_ATTRIB_DITHER,
    SCC_DISP_ATTRIB_BACK_LIGHT,
    SCC_DISP_ATTRIB_ADP_BACK_LIGHT,
    SCC_DISP_ATTRIB_BACK_LIGHT_ENERGY_SAVE
} SCC_DISP_ATTRIB_T;


/* Command SCC_CMD_CODE_GRP_TYPE *****************************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_CMD_TYPE_T
 *  @brief  Specify SCC command type
 *  @code
 *  typedef enum
 *  {
 *      SCC_CMD_TYPE_VIDEO,
 *      SCC_CMD_TYPE_AUDIO,
 *      SCC_CMD_TYPE_DISP
 *  } SCC_CMD_TYPE_T;
 *  @endcode
 *  @li@c  SCC_CMD_TYPE_VIDEO                                -SCC command type video
 *  @li@c  SCC_CMD_TYPE_AUDIO                                -SCC command type audio
 *  @li@c  SCC_CMD_TYPE_DISP                                   -SCC command type display
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_CMD_TYPE_VIDEO,
    SCC_CMD_TYPE_AUDIO,
    SCC_CMD_TYPE_DISP
} SCC_CMD_TYPE_T;

/* Command SCC_CMD_CODE_VIDEO_MODE ***************************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_MODE_T
 *  @brief  Specify video mode type
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_NORMAL = 0,
 *      SCC_VID_BLANK,
 *      SCC_VID_QUAD_VIDEO,
 *      SCC_VID_DEINT,
 *      SCC_VID_FREEZE
 *  }   SCC_VID_MODE_T;
 *  @endcode
 *  @li@c  SCC_VID_NORMAL                                  -video mode type normal
 *  @li@c  SCC_VID_BLANK                                     -video mode type blank
 *  @li@c  SCC_VID_QUAD_VIDEO                           -video mode type quad video
 *  @li@c  SCC_VID_DEINT                                      -video mode type de-interlace
 *  @li@c  SCC_VID_FREEZE                                    -video mode type freeze
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_NORMAL = 0,
    SCC_VID_BLANK,
    SCC_VID_QUAD_VIDEO,
    SCC_VID_DEINT,
    SCC_VID_FREEZE
}   SCC_VID_MODE_T;

/* Command SCC_CMD_CODE_VIDEO_BACKGROUND *********************************/
/* Command SCC_CMD_CODE_DISP_BACKGROUND **********************************/
/*------------------------------------------------------------------*/
/*! @struct SCC_BG_COLOR_T
 *  @brief  Contains background color info
 *  @code
 *  typedef struct _SCC_BG_COLOR_T
 *  {
 *      UINT8       ui1_red;
 *      UINT8       ui1_green;
 *      UINT8       ui1_blue;
 *  }   SCC_BG_COLOR_T;
 *  @endcode
 *  @li@c  ui1_red                                           -background color red value
 *  @li@c  ui1_green                                        -background color green value
 *  @li@c  ui1_blue                                          -background color blue value
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_BG_COLOR_T
{
    UINT8       ui1_red;
    UINT8       ui1_green;
    UINT8       ui1_blue;
}   SCC_BG_COLOR_T;

/* Command SCC_CMD_CODE_VIDEO_MODE_EX ************************************/
/*------------------------------------------------------------------*/
/*! @struct SCC_VID_MODE_EX_T
 *  @brief  Contains video mode extend info
 *  @code
 *  typedef struct _SCC_VID_MODE_EX_T
 *  {
 *      SCC_VID_MODE_T          e_mode;
 *      VSH_REGION_INFO_T       t_disp_region;
 *  }   SCC_VID_MODE_EX_T;
 *  @endcode
 *  @li@c  e_mode                                            -video extended mode
 *  @li@c  t_disp_region                                    -video display region
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_VID_MODE_EX_T
{
    SCC_VID_MODE_T          e_mode;
    VSH_REGION_INFO_T       t_disp_region;
}   SCC_VID_MODE_EX_T;

/*------------------------------------------------------------------*/
/*! @enum SCC_AUD_DEC_LINEOUT_VOL_MODE_T
 *  @brief  Specify lineout volume mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_DEC_LINEOUT_VOL_MODE_FIXED = 0,
 *      SCC_AUD_DEC_LINEOUT_VOL_MODE_VARIABLE
 *  }   SCC_AUD_DEC_LINEOUT_VOL_MODE_T;
 *  @endcode
 *  @li@c  SCC_AUD_DEC_LINEOUT_VOL_MODE_FIXED                -lineout volume fixed mode.
 *  @li@c  SCC_AUD_DEC_LINEOUT_VOL_MODE_VARIABLE             -lineout volume variable mode.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_DEC_LINEOUT_VOL_MODE_FIXED = 0,
    SCC_AUD_DEC_LINEOUT_VOL_MODE_VARIABLE
}   SCC_AUD_DEC_LINEOUT_VOL_MODE_T;

/*SCC_CMD_CODE_AUDIO_SE_LINOUT_VOL_MODE*/
/*------------------------------------------------------------------*/
/*! @struct  SCC_AUD_DEC_LINEOUT_VOL_T
 *  @brief  Contains lineout volume info.
 *  @code
 *  typedef struct _SCC_AUD_DEC_LINEOUT_VOL_T
 *  {
 *      SCC_AUD_DEC_LINEOUT_VOL_MODE_T           e_mode;
 *      UINT8                                                       ui1_level;
 *      BOOL                                                        b_mute;
 *  }   SCC_AUD_DEC_LINEOUT_VOL_T;
 *  @endcode
 *  @li@c  e_mode                                            - Specify lineout mode.
 *  @li@c  ui1_level                                         -lineout level.
 *  @li@c  b_mute                                            -whether mute.
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AUD_DEC_LINEOUT_VOL_T
{
    SCC_AUD_DEC_LINEOUT_VOL_MODE_T           e_mode;
    UINT8                                                       ui1_level;
    BOOL                                                        b_mute;
}   SCC_AUD_DEC_LINEOUT_VOL_T;


/* Command SCC_CMD_CODE_VIDEO_SCREEN_MODE ********************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_SCREEN_MODE_T
 *  @brief  Specify screen mode type
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_SCREEN_MODE_UNKNOWN = 0,
 *      SCC_VID_SCREEN_MODE_NORMAL,
 *      SCC_VID_SCREEN_MODE_LETTERBOX,
 *      SCC_VID_SCREEN_MODE_PAN_SCAN,
 *      SCC_VID_SCREEN_MODE_USER_DEFINED,
 *      SCC_VID_SCREEN_MODE_NON_LINEAR_ZOOM,
 *      SCC_VID_SCREEN_MODE_DOT_BY_DOT,
 *      SCC_VID_SCREEN_MODE_CUSTOM_DEF_0,
 *      SCC_VID_SCREEN_MODE_CUSTOM_DEF_1,
 *      SCC_VID_SCREEN_MODE_CUSTOM_DEF_2,
 *      SCC_VID_SCREEN_MODE_CUSTOM_DEF_3,
 *      SCC_VID_SCREEN_MODE_CUSTOM_DEF_4,
 *      SCC_VID_SCREEN_MODE_CUSTOM_DEF_5,
 *      SCC_VID_SCREEN_MODE_CUSTOM_DEF_6,
 *      SCC_VID_SCREEN_MODE_CUSTOM_DEF_7,
 *      SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_0, 
 *      SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_1, 
 *      SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_2, 
 *      SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_3  
 *  }   SCC_VID_SCREEN_MODE_T;
 *  @endcode
 *  @li@c  SCC_VID_SCREEN_MODE_UNKNOWN                       -screen mode type unknown
 *  @li@c  SCC_VID_SCREEN_MODE_NORMAL                          -screen mode type normal
 *  @li@c  SCC_VID_SCREEN_MODE_LETTERBOX                     -screen mode type letterbox
 *  @li@c  SCC_VID_SCREEN_MODE_PAN_SCAN                      -screen mode type scan
 *  @li@c  SCC_VID_SCREEN_MODE_USER_DEFINED                -screen mode type user defined
 *  @li@c  SCC_VID_SCREEN_MODE_NON_LINEAR_ZOOM         -screen mode type non-linear zoom
 *  @li@c  SCC_VID_SCREEN_MODE_DOT_BY_DOT                  -screen mode type dot by dot
 *  @li@c  SCC_VID_SCREEN_MODE_CUSTOM_DEF_0               -screen mode type custom defined 0
 *  @li@c  SCC_VID_SCREEN_MODE_CUSTOM_DEF_1               -screen mode type custom defined 1
 *  @li@c  SCC_VID_SCREEN_MODE_CUSTOM_DEF_2               -screen mode type custom defined 2
 *  @li@c  SCC_VID_SCREEN_MODE_CUSTOM_DEF_3               -screen mode type custom defined 3
 *  @li@c  SCC_VID_SCREEN_MODE_CUSTOM_DEF_4               -screen mode type custom defined 4
 *  @li@c  SCC_VID_SCREEN_MODE_CUSTOM_DEF_5               -screen mode type custom defined 5
 *  @li@c  SCC_VID_SCREEN_MODE_CUSTOM_DEF_6               -screen mode type custom defined 6
 *  @li@c  SCC_VID_SCREEN_MODE_CUSTOM_DEF_7               -screen mode type custom defined 7
 *  @li@c  SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_0        -screen mode type none linear zoom 0
 *  @li@c  SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_1        -screen mode type none linear zoom 1
 *  @li@c  SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_2        -screen mode type none linear zoom 2
 *  @li@c  SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_3        -screen mode type none linear zoom 3
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_SCREEN_MODE_UNKNOWN = 0,
    SCC_VID_SCREEN_MODE_NORMAL,
    SCC_VID_SCREEN_MODE_LETTERBOX,
    SCC_VID_SCREEN_MODE_PAN_SCAN,
    SCC_VID_SCREEN_MODE_USER_DEFINED,
    SCC_VID_SCREEN_MODE_NON_LINEAR_ZOOM,
    SCC_VID_SCREEN_MODE_DOT_BY_DOT,
    SCC_VID_SCREEN_MODE_CUSTOM_DEF_0,
    SCC_VID_SCREEN_MODE_CUSTOM_DEF_1,
    SCC_VID_SCREEN_MODE_CUSTOM_DEF_2,
    SCC_VID_SCREEN_MODE_CUSTOM_DEF_3,
    SCC_VID_SCREEN_MODE_CUSTOM_DEF_4,
    SCC_VID_SCREEN_MODE_CUSTOM_DEF_5,
    SCC_VID_SCREEN_MODE_CUSTOM_DEF_6,
    SCC_VID_SCREEN_MODE_CUSTOM_DEF_7,
    SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_0, /* NONE LINEAR ZOOM */
    SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_1, /* NONE LINEAR ZOOM */
    SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_2, /* NONE LINEAR ZOOM */
    SCC_VID_SCREEN_MODE_NLZ_CUSTOM_DEF_3  /* NONE LINEAR ZOOM */
}   SCC_VID_SCREEN_MODE_T;

/* Command SCC_CMD_CODE_VIDEO_OVERSCAN_CLIPPER ***************************/
/*------------------------------------------------------------------*/
/*! @struct SCC_VID_OVER_SCAN_CLIPPER_T
 *  @brief  Contain over scan clipper info
 *  @code
 *  typedef struct _SCC_VID_OVER_SCAN_CLIPPER_T
 *  {
 *      UINT32      ui4_top;
 *      UINT32      ui4_bottom;
 *      UINT32      ui4_left;
 *      UINT32      ui4_right;
 *  }   SCC_VID_OVER_SCAN_CLIPPER_T;
 *  @endcode
 *  @li@c  ui4_top                                           -over scan clipper top value
 *  @li@c  ui4_bottom                                     -over scan clipper bottom value
 *  @li@c  ui4_left                                           -over scan clipper left value
 *  @li@c  ui4_right                                         -over scan clipper right value
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_VID_OVER_SCAN_CLIPPER_T
{
    UINT32      ui4_top;
    UINT32      ui4_bottom;
    UINT32      ui4_left;
    UINT32      ui4_right;
}   SCC_VID_OVER_SCAN_CLIPPER_T;

/*------------------------------------------------------------------*/
/*! @enum SCC_VID_VDRIPS_PLAY_CTRL_T
 *  @brief  Specify play control type
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_VDRIPS_CTRL_RESET,
 *      SCC_VID_VDRIPS_CTRL_STOP,
 *      SCC_VID_VDRIPS_CTRL_PLAY_VDRIPS
 *  } SCC_VID_VDRIPS_PLAY_CTRL_T;
 *  @endcode
 *  @li@c  SCC_VID_VDRIPS_CTRL_RESET                         -
 *  @li@c  SCC_VID_VDRIPS_CTRL_STOP                          -
 *  @li@c  SCC_VID_VDRIPS_CTRL_PLAY_VDRIPS              -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_VDRIPS_CTRL_RESET,
    SCC_VID_VDRIPS_CTRL_STOP,
    SCC_VID_VDRIPS_CTRL_PLAY_VDRIPS
} SCC_VID_VDRIPS_PLAY_CTRL_T;

/*------------------------------------------------------------------*/
/*! @struct SCC_VID_VDRIPS_BUFF_T
 *  @brief  Contain buffer info
 *  @code
 *  typedef struct SCC_VID_VDRIPS_BUFF_T
 *  {
 *      UCHAR* puc_vdrips_buff;
 *      SIZE_T z_vdrips_size;
 *  } SCC_VID_VDRIPS_BUFF_T;
 *  @endcode
 *  @li@c  puc_vdrips_buff                                  -
 *  @li@c  z_vdrips_size                                     -
 */
/*------------------------------------------------------------------*/
typedef struct SCC_VID_VDRIPS_BUFF_T
{
    UCHAR* puc_vdrips_buff;
    SIZE_T z_vdrips_size;
} SCC_VID_VDRIPS_BUFF_T;

/*------------------------------------------------------------------*/
/*! @enum SCC_VID_VDRIPS_COND_T
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_VDRIPS_COND_ERROR = -1,
 *      SCC_VID_VDRIPS_COND_CTRL_DONE,
 *      SCC_VID_VDRIPS_DONE
 *  } SCC_VID_VDRIPS_COND_T;
 *  @endcode
 *  @li@c  SCC_VID_VDRIPS_COND_ERROR                         -
 *  @li@c  SCC_VID_VDRIPS_COND_CTRL_DONE                     -
 *  @li@c  SCC_VID_VDRIPS_DONE                               -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_VDRIPS_COND_ERROR = -1,
    SCC_VID_VDRIPS_COND_CTRL_DONE,
    SCC_VID_VDRIPS_DONE
} SCC_VID_VDRIPS_COND_T;


/* iframe SCC notify function */
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_scc_vid_vdrips_nfy_fct) (
                                   HANDLE_T                    h_aud,
                                   SCC_VID_VDRIPS_COND_T     e_nfy_cond,
                                   VOID*                       pv_nfy_tag,
                                   UINT32                      ui4_data_1,
                                   UINT32                      ui4_data_2 );

/*------------------------------------------------------------------*/
/*! @enum SCC_VID_IFRAME_CTRL_T
 *  @brief  Specify i-frame control type
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_IFRAME_CTRL_RESET,
 *      SCC_VID_IFRAME_CTRL_END,
 *      SCC_VID_IFRAME_CTRL_BEGIN
 *  } SCC_VID_IFRAME_CTRL_T;
 *  @endcode
 *  @li@c  SCC_VID_IFRAME_CTRL_RESET                         -i-frame control type reset
 *  @li@c  SCC_VID_IFRAME_CTRL_END                           -i-frame control type end
 *  @li@c  SCC_VID_IFRAME_CTRL_BEGIN                         -i-frame control type begin
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_IFRAME_CTRL_RESET,
    SCC_VID_IFRAME_CTRL_END,
    SCC_VID_IFRAME_CTRL_BEGIN
} SCC_VID_IFRAME_CTRL_T;

/*------------------------------------------------------------------*/
/*! @enum SCC_VID_IFRAME_CODEC_T
 *  @brief  Specify i-frame codec type
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_IFRAME_CODEC_UNKNOWN = 0,
 *      SCC_VID_IFRAME_CODEC_MPEG_1,
 *      SCC_VID_IFRAME_CODEC_MPEG_2,
 *      SCC_VID_IFRAME_CODEC_MPEG_4,
 *      SCC_VID_IFRAME_CODEC_DIVX_311,
 *      SCC_VID_IFRAME_CODEC_DIVX_4,
 *      SCC_VID_IFRAME_CODEC_DIVX_5,
 *      SCC_VID_IFRAME_CODEC_XVID,
 *      SCC_VID_IFRAME_CODEC_WMV1,
 *      SCC_VID_IFRAME_CODEC_WMV2,
 *      SCC_VID_IFRAME_CODEC_WMV3,
 *      SCC_VID_IFRAME_CODEC_WVC1,
 *      SCC_VID_IFRAME_CODEC_H264
 *  }   SCC_VID_IFRAME_CODEC_T;
 *  @endcode
 *  @li@c  SCC_VID_IFRAME_CODEC_UNKNOWN                    -i-frame codec type unknown
 *  @li@c  SCC_VID_IFRAME_CODEC_MPEG_1                       -i-frame codec type mpeg1
 *  @li@c  SCC_VID_IFRAME_CODEC_MPEG_2                       -i-frame codec type mpeg2
 *  @li@c  SCC_VID_IFRAME_CODEC_MPEG_4                       -i-frame codec type mpeg4
 *  @li@c  SCC_VID_IFRAME_CODEC_DIVX_311                     -i-frame codec type DIVX311
 *  @li@c  SCC_VID_IFRAME_CODEC_DIVX_4                       -i-frame codec type DIVX4
 *  @li@c  SCC_VID_IFRAME_CODEC_DIVX_5                       -i-frame codec type DIVX5
 *  @li@c  SCC_VID_IFRAME_CODEC_XVID                          -i-frame codec type XVID
 *  @li@c  SCC_VID_IFRAME_CODEC_WMV1                         -i-frame codec type WMV1 
 *  @li@c  SCC_VID_IFRAME_CODEC_WMV2                         -i-frame codec type WMV2 
 *  @li@c  SCC_VID_IFRAME_CODEC_WMV3                         -i-frame codec type WMV3 
 *  @li@c  SCC_VID_IFRAME_CODEC_WVC1                         -i-frame codec type WVC1
 *  @li@c  SCC_VID_IFRAME_CODEC_H264                          -i-frame codec type H264
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_IFRAME_CODEC_UNKNOWN = 0,
    SCC_VID_IFRAME_CODEC_MPEG_1,
    SCC_VID_IFRAME_CODEC_MPEG_2,
    SCC_VID_IFRAME_CODEC_MPEG_4,
    SCC_VID_IFRAME_CODEC_DIVX_311,
    SCC_VID_IFRAME_CODEC_DIVX_4,
    SCC_VID_IFRAME_CODEC_DIVX_5,
    SCC_VID_IFRAME_CODEC_XVID,
    SCC_VID_IFRAME_CODEC_WMV1,
    SCC_VID_IFRAME_CODEC_WMV2,
    SCC_VID_IFRAME_CODEC_WMV3,
    SCC_VID_IFRAME_CODEC_WVC1,
    SCC_VID_IFRAME_CODEC_H264
}   SCC_VID_IFRAME_CODEC_T;

/*------------------------------------------------------------------*/
/*! @struct SCC_VID_IFRAME_BUFF_T
 *  @brief  Contain i-frame buffer info
 *  @code
 *  typedef struct _SCC_VID_IFRAME_BUFF_T
 *  {
 *      UCHAR* puc_iframe_buff;
 *      SIZE_T z_iframe_size;
 *  } SCC_VID_IFRAME_BUFF_T;
 *  @endcode
 *  @li@c  puc_iframe_buff                                   -i-frame buffer pointer
 *  @li@c  z_iframe_size                                     -i-frame buffer size
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_VID_IFRAME_BUFF_T
{
    UCHAR* puc_iframe_buff;
    SIZE_T z_iframe_size;
} SCC_VID_IFRAME_BUFF_T;

/*------------------------------------------------------------------*/
/*! @enum SCC_VID_IFRAME_COND_T
 *  @brief Specify i-frame buffer condition type
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_IFRAME_COND_ERROR = -1,
 *      SCC_VID_IFRAME_COND_CTRL_DONE,
 *      SCC_VID_IFRAME_COND_SET_BUFF_DONE,
 *      SCC_VID_IFRAME_COND_DECODE_DONE
 *  } SCC_VID_IFRAME_COND_T;
 *  @endcode
 *  @li@c  SCC_VID_IFRAME_COND_ERROR                         -i-frame buffer condition error
 *  @li@c  SCC_VID_IFRAME_COND_CTRL_DONE                  -i-frame buffer condition control done
 *  @li@c  SCC_VID_IFRAME_COND_SET_BUFF_DONE           -i-frame buffer condition set done
 *  @li@c  SCC_VID_IFRAME_COND_DECODE_DONE             -i-frame buffer condition decode done
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_IFRAME_COND_ERROR = -1,
    SCC_VID_IFRAME_COND_CTRL_DONE,
    SCC_VID_IFRAME_COND_SET_BUFF_DONE,
    SCC_VID_IFRAME_COND_DECODE_DONE,
    SCC_VID_IFRAME_COND_THUMBNAIL_DONE 
} SCC_VID_IFRAME_COND_T;

#define SCC_VID_IFRAME_COND_RESET_CTRL_DONE            ((UINT32) (0) )    /**<        */
#define SCC_VID_IFRAME_COND_END_CTRL_DONE              ((UINT32) (1) )    /**<        */
#define SCC_VID_IFRAME_COND_BEGIN_CTRL_DONE            ((UINT32) (8) )    /**<        */
/* iframe SCC notify function */
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_scc_vid_iframe_nfy_fct) (
                                   HANDLE_T                    h_aud,
                                   SCC_VID_IFRAME_COND_T     e_nfy_cond,
                                   VOID*                       pv_nfy_tag,
                                   UINT32                      ui4_data_1,
                                   UINT32                      ui4_data_2 );


/* Command SCC_CMD_CODE_VIDEO_PIC_MODE ***********************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_PIC_MODE_T
 *  @brief  Specify picture mode type
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_PIC_MODE_DEFAULT = 0,
 *      SCC_VID_PIC_MODE_CUSTOM_1,
 *      SCC_VID_PIC_MODE_CUSTOM_2,
 *      SCC_VID_PIC_MODE_CUSTOM_3,
 *      SCC_VID_PIC_MODE_CUSTOM_4,
 *      SCC_VID_PIC_MODE_CUSTOM_5,
 *      SCC_VID_PIC_MODE_CUSTOM_6,
 *      SCC_VID_PIC_MODE_CUSTOM_7,
 *      SCC_VID_PIC_MODE_CUSTOM_8,
 *      SCC_VID_PIC_MODE_CUSTOM_9
 *  }   SCC_VID_PIC_MODE_T;
 *  @endcode
 *  @li@c  SCC_VID_PIC_MODE_DEFAULT                          -picture mode type default
 *  @li@c  SCC_VID_PIC_MODE_CUSTOM_1                         -picture mode type custom 1
 *  @li@c  SCC_VID_PIC_MODE_CUSTOM_2                         -picture mode type custom 2
 *  @li@c  SCC_VID_PIC_MODE_CUSTOM_3                         -picture mode type custom 3
 *  @li@c  SCC_VID_PIC_MODE_CUSTOM_4                         -picture mode type custom 4
 *  @li@c  SCC_VID_PIC_MODE_CUSTOM_5                         -picture mode type custom 5
 *  @li@c  SCC_VID_PIC_MODE_CUSTOM_6                         -picture mode type custom 6
 *  @li@c  SCC_VID_PIC_MODE_CUSTOM_7                         -picture mode type custom 7
 *  @li@c  SCC_VID_PIC_MODE_CUSTOM_8                         -picture mode type custom 8
 *  @li@c  SCC_VID_PIC_MODE_CUSTOM_9                         -picture mode type custom 9
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_PIC_MODE_DEFAULT = 0,
    SCC_VID_PIC_MODE_CUSTOM_1,
    SCC_VID_PIC_MODE_CUSTOM_2,
    SCC_VID_PIC_MODE_CUSTOM_3,
    SCC_VID_PIC_MODE_CUSTOM_4,
    SCC_VID_PIC_MODE_CUSTOM_5,
    SCC_VID_PIC_MODE_CUSTOM_6,
    SCC_VID_PIC_MODE_CUSTOM_7,
    SCC_VID_PIC_MODE_CUSTOM_8,
    SCC_VID_PIC_MODE_CUSTOM_9
}   SCC_VID_PIC_MODE_T;


/* Command SCC_CMD_CODE_VIDEO_COLOR_SYS **********************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_COLOR_SYS_T
 *  @brief  Specify color system type
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_COLOR_SYS_UNKNOWN = -1,
 *      SCC_VID_COLOR_SYS_NTSC,
 *      SCC_VID_COLOR_SYS_NTSC_443,
 *      SCC_VID_COLOR_SYS_PAL,
 *      SCC_VID_COLOR_SYS_PAL_N,
 *      SCC_VID_COLOR_SYS_PAL_M,
 *      SCC_VID_COLOR_SYS_PAL_60,
 *      SCC_VID_COLOR_SYS_SECAM
 *  }   SCC_VID_COLOR_SYS_T;
 *  @endcode
 *  @li@c  SCC_VID_COLOR_SYS_UNKNOWN                     -color system type unknown
 *  @li@c  SCC_VID_COLOR_SYS_NTSC                            -color system type NTSC
 *  @li@c  SCC_VID_COLOR_SYS_NTSC_443                     -color system type NTSC 443
 *  @li@c  SCC_VID_COLOR_SYS_PAL                               -color system type PAL
 *  @li@c  SCC_VID_COLOR_SYS_PAL_N                           -color system type PAL_N
 *  @li@c  SCC_VID_COLOR_SYS_PAL_M                           -color system type PAL_M
 *  @li@c  SCC_VID_COLOR_SYS_PAL_60                          -color system type PAL_60
 *  @li@c  SCC_VID_COLOR_SYS_SECAM                           -color system type SECAM
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_COLOR_SYS_UNKNOWN = -1,
    SCC_VID_COLOR_SYS_NTSC,
    SCC_VID_COLOR_SYS_NTSC_443,
    SCC_VID_COLOR_SYS_PAL,
    SCC_VID_COLOR_SYS_PAL_N,
    SCC_VID_COLOR_SYS_PAL_M,
    SCC_VID_COLOR_SYS_PAL_60,
    SCC_VID_COLOR_SYS_SECAM
}   SCC_VID_COLOR_SYS_T;


/* Command SCC_CMD_CODE_VIDEO_HDMI_MODE **********************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_HDMI_MODE_T
 *  @brief  Specify HDMI mode type
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_HDMI_MODE_UNKNOWN = 0,
 *      SCC_VID_HDMI_MODE_AUTO,
 *      SCC_VID_HDMI_MODE_GRAPHIC,
 *      SCC_VID_HDMI_MODE_VIDEO
 *  }   SCC_VID_HDMI_MODE_T;
 *  @endcode
 *  @li@c  SCC_VID_HDMI_MODE_UNKNOWN                     -HDMI mode type unknown
 *  @li@c  SCC_VID_HDMI_MODE_AUTO                            -HDMI mode type auto
 *  @li@c  SCC_VID_HDMI_MODE_GRAPHIC                       -HDMI mode type graphic
 *  @li@c  SCC_VID_HDMI_MODE_VIDEO                           -HDMI mode type video
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_HDMI_MODE_UNKNOWN = 0,
    SCC_VID_HDMI_MODE_AUTO,
    SCC_VID_HDMI_MODE_GRAPHIC,
    SCC_VID_HDMI_MODE_VIDEO
}   SCC_VID_HDMI_MODE_T;


/* Command SCC_CMD_CODE_VIDEO_DEBLOCKING *********************************/
/*------------------------------------------------------------------*/
/*! @struct SCC_VID_REGION_T
 *  @brief  Contain video region info
 *  @code
 *  typedef struct _SCC_VID_REGION_T
 *  {
 *      UINT16        ui4_x;    
 *      UINT16        ui4_y;    
 *      UINT16        ui4_h;    
 *      UINT16        ui4_w;    
 *  } SCC_VID_REGION_T;
 *  @endcode
 *  @li@c  ui4_x                                             -video region start X value
 *  @li@c  ui4_y                                             -video region start Y value
 *  @li@c  ui4_h                                             -video region height value
 *  @li@c  ui4_w                                             -video region wight value
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_VID_REGION_T
{
    UINT16        ui4_x;    /* in 0.1 percent unit */
    UINT16        ui4_y;    /* in 0.1 percent unit */
    UINT16        ui4_h;    /* in 0.1 percent unit */
    UINT16        ui4_w;    /* in 0.1 percent unit */
} SCC_VID_REGION_T;

/*------------------------------------------------------------------*/
/*! @struct SCC_VID_DEBLOCKING_INFO_T
 *  @brief  Contain de-blocking info
 *  @code
 *  typedef struct _SCC_VID_DEBLOCKING_INFO_T
 *  {
 *      BOOL                       b_is_on;
 *      union
 *      {
 *          SCC_VID_REGION_T       t_region;
 *      } u;
 *  }   SCC_VID_DEBLOCKING_INFO_T;
 *  @endcode
 *  @li@c  b_is_on                                           -de-blocking is on or not
 *  @li@c  t_region                                          -de-blocking region info
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_VID_DEBLOCKING_INFO_T
{
    BOOL                       b_is_on;

    union
    {
        SCC_VID_REGION_T       t_region;
    } u;
}   SCC_VID_DEBLOCKING_INFO_T;


/* Command SCC_CMD_CODE_VIDEO_SPLIT_SCRN_DEMO ****************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_SPLIT_SCRN_DEMO_T
 *  @brief  Specify split screen demo type
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_SPLIT_SCRN_DEMO_OFF = 0,
 *      SCC_VID_SPLIT_SCRN_DEMO_LEFT,
 *      SCC_VID_SPLIT_SCRN_DEMO_RIGHT
 *  }   SCC_VID_SPLIT_SCRN_DEMO_T;
 *  @endcode
 *  @li@c  SCC_VID_SPLIT_SCRN_DEMO_OFF                       -split screen demo type off
 *  @li@c  SCC_VID_SPLIT_SCRN_DEMO_LEFT                      -split screen demo type left
 *  @li@c  SCC_VID_SPLIT_SCRN_DEMO_RIGHT                    -split screen demo type right
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_SPLIT_SCRN_DEMO_OFF = 0,
    SCC_VID_SPLIT_SCRN_DEMO_LEFT,
    SCC_VID_SPLIT_SCRN_DEMO_RIGHT
}   SCC_VID_SPLIT_SCRN_DEMO_T;

/*------------------------------------------------------------------*/
/*! @enum SCC_VID_SRM_MODE_T
 *  @brief  Specify video mode type
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_SRM_MODE_OFF = 0,
 *      SCC_VID_SRM_MODE_THUMBNAIL,
 *      SCC_VID_SRM_MODE_VIDEO
 *  }   SCC_VID_SRM_MODE_T;
 *  @endcode
 *  @li@c  SCC_VID_SRM_MODE_OFF                              -mode type off
 *  @li@c  SCC_VID_SRM_MODE_THUMBNAIL                   -mode type thumbnail
 *  @li@c  SCC_VID_SRM_MODE_VIDEO                          -mode type video
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_SRM_MODE_OFF = 0,
    SCC_VID_SRM_MODE_THUMBNAIL,
    SCC_VID_SRM_MODE_VIDEO
}   SCC_VID_SRM_MODE_T;

/* Command SCC_CMD_CODE_VIDEO_LB_DETECT_CONFIG ***************************/
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_scc_vid_lb_detect_nfy_fct) (
    VOID*                   pv_nfy_tag,
    /* original picture size */
    UINT32                  ui4_src_width,
    UINT32                  ui4_src_height,
    /* active (no black bar) region information */
    UINT32                  ui4_active_width,
    UINT32                  ui4_active_height,
    UINT32                  ui4_active_x,
    UINT32                  ui4_active_y );

/*------------------------------------------------------------------*/
/*! @struct SCC_VID_LB_DETECT_NFY_INFO_T
 *  @brief Contain letter box detect notify info
 *  @code
 *  typedef struct _SCC_VID_LB_DETECT_NFY_INFO_T
 *  {
 *      x_scc_vid_lb_detect_nfy_fct    pf_lb_detect_nfy;
 *      VOID*                          pv_nfy_tag;
 *  } SCC_VID_LB_DETECT_NFY_INFO_T;
 *  @endcode
 *  @li@c  pf_lb_detect_nfy                                -letter box detect notify function pointer
 *  @li@c  pv_nfy_tag                                        -letter box detect notify tag pointer
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_VID_LB_DETECT_NFY_INFO_T
{
    x_scc_vid_lb_detect_nfy_fct    pf_lb_detect_nfy;
    VOID*                          pv_nfy_tag;
} SCC_VID_LB_DETECT_NFY_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct SCC_VID_LB_DETECT_CONFIG_T
 *  @brief  Contain letter box detect config info
 *  @code
 *  typedef struct _SCC_VID_LB_DETECT_CONFIG_T
 *  {
 *      UINT32                          ui4_time_threshold;
 *      UINT32                          ui4_bounds_top;
 *      UINT32                          ui4_bounds_bottom;
 *      UINT32                          ui4_bounds_left;
 *      UINT32                          ui4_bounds_right;
 *      BOOL                            b_manual_mode;          
 *      SCC_VID_LB_DETECT_NFY_INFO_T    t_lb_detect_nfy_info;   
 *  }   SCC_VID_LB_DETECT_CONFIG_T;
 *  @endcode
 *  @li@c  ui4_time_threshold                                -letter box detect config time threshold
 *  @li@c  ui4_bounds_top                                    -letter box detect config bounds top
 *  @li@c  ui4_bounds_bottom                              -letter box detect config bounds bottom
 *  @li@c  ui4_bounds_left                                   -letter box detect config bounds left 
 *  @li@c  ui4_bounds_right                                  -letter box detect config bounds right
 *  @li@c  b_manual_mode                                   -letter box detect config is manual mode or not
 *  @li@c  t_lb_detect_nfy_info                              -letter box detect config notify info
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_VID_LB_DETECT_CONFIG_T
{
    UINT32                          ui4_time_threshold;
    UINT32                          ui4_bounds_top;
    UINT32                          ui4_bounds_bottom;
    UINT32                          ui4_bounds_left;
    UINT32                          ui4_bounds_right;
    /* enhancement for manual mode */
    BOOL                            b_manual_mode;          /* false means auto mode */
    SCC_VID_LB_DETECT_NFY_INFO_T    t_lb_detect_nfy_info;   /* callback function information (SET only) */
}   SCC_VID_LB_DETECT_CONFIG_T;


/* Command SCC_CMD_CODE_VIDEO_DI_FILM_MODE *******************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_DI_FILM_MODE_T
 *  @brief  Specify film mode type
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_DI_FILM_MODE_OFF = 0,
 *      SCC_VID_DI_FILM_MODE_SLOW_PICTURE,
 *      SCC_VID_DI_FILM_MODE_ACTION_PICTURE
 *  }   SCC_VID_DI_FILM_MODE_T;
 *  @endcode
 *  @li@c  SCC_VID_DI_FILM_MODE_OFF                          -film mode type off
 *  @li@c  SCC_VID_DI_FILM_MODE_SLOW_PICTURE        -film mode type slow picture
 *  @li@c  SCC_VID_DI_FILM_MODE_ACTION_PICTURE     -film mode type action picture
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_DI_FILM_MODE_OFF = 0,
    SCC_VID_DI_FILM_MODE_SLOW_PICTURE,
    SCC_VID_DI_FILM_MODE_ACTION_PICTURE
}   SCC_VID_DI_FILM_MODE_T;


/* Command SCC_CMD_CODE_VIDEO_DI_MA **************************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_DI_MA_T
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_DI_MA_SLOW_PICTURE = 0,
 *      SCC_VID_DI_MA_ACTION_PICTURE
 *  }   SCC_VID_DI_MA_T;
 *  @endcode
 *  @li@c  SCC_VID_DI_MA_SLOW_PICTURE                        -
 *  @li@c  SCC_VID_DI_MA_ACTION_PICTURE                      -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_DI_MA_SLOW_PICTURE = 0,
    SCC_VID_DI_MA_ACTION_PICTURE
}   SCC_VID_DI_MA_T;


/* Command SCC_CMD_CODE_VIDEO_DI_EDGE_PRESERVE ***************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_DI_EDGE_PRESERVE_T
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_DI_EDGE_PRESERVE_WEAK = 0,
 *      SCC_VID_DI_EDGE_PRESERVE_STRONG
 *  }   SCC_VID_DI_EDGE_PRESERVE_T;
 *  @endcode
 *  @li@c  SCC_VID_DI_EDGE_PRESERVE_WEAK                     -
 *  @li@c  SCC_VID_DI_EDGE_PRESERVE_STRONG                   -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_DI_EDGE_PRESERVE_WEAK = 0,
    SCC_VID_DI_EDGE_PRESERVE_STRONG
}   SCC_VID_DI_EDGE_PRESERVE_T;


/* Command SCC_CMD_CODE_VIDEO_MJC ****************************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_MJC_EFFECT_T
 *  @brief  Specify video MJC effect.
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_MJC_EFFECT_OFF = 0,
 *      SCC_VID_MJC_EFFECT_LOW,
 *      SCC_VID_MJC_EFFECT_MIDDLE,
 *      SCC_VID_MJC_EFFECT_HIGH 
 *  }   SCC_VID_MJC_EFFECT_T;
 *  @endcode
 *  @li@c  SCC_VID_MJC_EFFECT_OFF                          -MJC effect off
 *  @li@c  SCC_VID_MJC_EFFECT_LOW                          -MJC effect low
 *  @li@c  SCC_VID_MJC_EFFECT_MIDDLE                       -MJC effect middle
 *  @li@c  SCC_VID_MJC_EFFECT_HIGH                         -MJC effect high
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_MJC_EFFECT_OFF = 0,
    SCC_VID_MJC_EFFECT_LOW,
    SCC_VID_MJC_EFFECT_MIDDLE,
    SCC_VID_MJC_EFFECT_HIGH 
}   SCC_VID_MJC_EFFECT_T;

/*------------------------------------------------------------------*/
/*! @enum SCC_VID_MJC_MODE_T
 *  @brief  Specify video MJC mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_MJC_MODE_0 = 0, 
 *      SCC_VID_MJC_MODE_1,     
 *      SCC_VID_MJC_MODE_2      
 *  }   SCC_VID_MJC_MODE_T;
 *  @endcode
 *  @li@c  SCC_VID_MJC_MODE_0                                -MJC mode 0
 *  @li@c  SCC_VID_MJC_MODE_1                                -MJC mode 1
 *  @li@c  SCC_VID_MJC_MODE_2                                -MJC mode 2
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_MJC_MODE_0 = 0, /* NORMAL */
    SCC_VID_MJC_MODE_1,     /* FALLBACK */
    SCC_VID_MJC_MODE_2      /* NO_FILM */
}   SCC_VID_MJC_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum SCC_VID_MJC_DEMO_T
 *  @brief  Specify video MJC demo.
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_MJC_DEMO_OFF = 0,
 *      SCC_VID_MJC_DEMO_RIGHT,
 *      SCC_VID_MJC_DEMO_LEFT
 *  }   SCC_VID_MJC_DEMO_T;
 *  @endcode
 *  @li@c  SCC_VID_MJC_DEMO_OFF                                -MJC demo off
 *  @li@c  SCC_VID_MJC_DEMO_RIGHT                              -MJC demo right
 *  @li@c  SCC_VID_MJC_DEMO_LEFT                               -MJC demo left
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_MJC_DEMO_OFF = 0,
    SCC_VID_MJC_DEMO_RIGHT,
    SCC_VID_MJC_DEMO_LEFT
}   SCC_VID_MJC_DEMO_T;

/*------------------------------------------------------------------*/
/*! @struct SCC_VID_MJC_T
 *  @brief  Contains video MJC.
 *  @code
 *  typedef struct _SCC_VID_MJC_T
 *  {
 *      SCC_VID_MJC_EFFECT_T      e_effect;
 *      SCC_VID_MJC_MODE_T        e_mode;
 *      SCC_VID_MJC_DEMO_T        e_demo;
 *  }   SCC_VID_MJC_T;
 *  @endcode
 *  @li@c  e_effect                                          -MJC effect
 *  @li@c  e_mode                                            -MJC mode
 *  @li@c  e_demo                                            -MJC demo
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_VID_MJC_T
{
    SCC_VID_MJC_EFFECT_T      e_effect;
    SCC_VID_MJC_MODE_T        e_mode;
    SCC_VID_MJC_DEMO_T        e_demo;
}   SCC_VID_MJC_T;


/* Command SCC_CMD_CODE_VIDEO_XVYCC **************************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_XVYCC_T
 *  @brief  Specify video XVYCC.
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_XVYCC_OFF = 0,
 *      SCC_VID_XVYCC_ON
 *  }   SCC_VID_XVYCC_T;
 *  @endcode
 *  @li@c  SCC_VID_XVYCC_OFF                                 -XVYCC off
 *  @li@c  SCC_VID_XVYCC_ON                                  -XVYCC on
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_XVYCC_OFF = 0,
    SCC_VID_XVYCC_ON
}   SCC_VID_XVYCC_T;


/* Command SCC_CMD_CODE_VIDEO_WCG ****************************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_WCG_T
 *  @brief  Specify video WCG.
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_WCG_OFF = 0,
 *      SCC_VID_WCG_ON
 *  }   SCC_VID_WCG_T;
 *  @endcode
 *  @li@c  SCC_VID_WCG_OFF                                   -WCG off
 *  @li@c  SCC_VID_WCG_ON                                    -WCG on
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_WCG_OFF = 0,
    SCC_VID_WCG_ON
}   SCC_VID_WCG_T;


/* Command SCC_CMD_CODE_VIDEO_GAME_MODE **********************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_GAME_MODE_T
 *  @brief  Specify video game mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_GAME_MODE_OFF = 0,
 *      SCC_VID_GAME_MODE_ON
 *  }   SCC_VID_GAME_MODE_T;
 *  @endcode
 *  @li@c  SCC_VID_GAME_MODE_OFF                             -game mode off
 *  @li@c  SCC_VID_GAME_MODE_ON                              -game mode on
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_GAME_MODE_OFF = 0,
    SCC_VID_GAME_MODE_ON
}   SCC_VID_GAME_MODE_T;


/* Command SCC_CMD_CODE_VIDEO_BLUE_STRETCH *******************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_BLUE_STRETCH_T
 *  @brief  Specify video blue stretch.
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_BLUE_STRETCH_OFF = 0,
 *      SCC_VID_BLUE_STRETCH_BLUE,
 *      SCC_VID_BLUE_STRETCH_RED,
 *      SCC_VID_BLUE_STRETCH_GREEN
 *  }   SCC_VID_BLUE_STRETCH_T;
 *  @endcode
 *  @li@c  SCC_VID_BLUE_STRETCH_OFF                          -blue stretch off
 *  @li@c  SCC_VID_BLUE_STRETCH_BLUE                         -blue stretch blue
 *  @li@c  SCC_VID_BLUE_STRETCH_RED                          -blue stretch red
 *  @li@c  SCC_VID_BLUE_STRETCH_GREEN                        -blue stretchgreen
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_BLUE_STRETCH_OFF = 0,
    SCC_VID_BLUE_STRETCH_BLUE,
    SCC_VID_BLUE_STRETCH_RED,
    SCC_VID_BLUE_STRETCH_GREEN
}   SCC_VID_BLUE_STRETCH_T;


/* Command SCC_CMD_CODE_VIDEO_MPEG_NR ************************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_MPEG_NR_T
 *  @brief  Specify video MPEG NR.
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_MPEG_NR_OFF = 0,
 *      SCC_VID_MPEG_NR_LOW,
 *      SCC_VID_MPEG_NR_MIDDLE,
 *      SCC_VID_MPEG_NR_HIGH 
 *  }   SCC_VID_MPEG_NR_T;
 *  @endcode
 *  @li@c  SCC_VID_MPEG_NR_OFF                               -MPEG NR off
 *  @li@c  SCC_VID_MPEG_NR_LOW                               -MPEG NR low
 *  @li@c  SCC_VID_MPEG_NR_MIDDLE                            -MPEG NR middle
 *  @li@c  SCC_VID_MPEG_NR_HIGH                              -MPEG NR high
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_MPEG_NR_OFF = 0,
    SCC_VID_MPEG_NR_LOW,
    SCC_VID_MPEG_NR_MIDDLE,
    SCC_VID_MPEG_NR_HIGH 
}   SCC_VID_MPEG_NR_T;


/* Command SCC_CMD_CODE_VIDEO_SUPER_BLANK *********************************/
/*------------------------------------------------------------------*/
/*! @struct SCC_VID_SUPER_BLANK_T
 *  @brief  Contains video super blank.
 *  @code
 *  typedef struct _SCC_VID_SUPER_BLANK_T
 *  {
 *      BOOL                 b_enable;
 *      SCC_BG_COLOR_T       t_bg_color;
 *      VSH_REGION_INFO_T    t_disp_region;
 *  }   SCC_VID_SUPER_BLANK_T;
 *  @endcode
 *  @li@c  b_enable                                          -enable or disable
 *  @li@c  t_bg_color                                        -background color
 *  @li@c  t_disp_region                                     -display region
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_VID_SUPER_BLANK_T
{
    BOOL                 b_enable;
    SCC_BG_COLOR_T       t_bg_color;
    VSH_REGION_INFO_T    t_disp_region;
}   SCC_VID_SUPER_BLANK_T;

/* Command SCC_CMD_CODE_VIDEO_BLACK_STRETCH *******************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_BLACK_STRETCH_T
 *  @brief  Specify video black stretch.
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_BLACK_STRETCH_OFF = 0,
 *      SCC_VID_BLACK_STRETCH_LOW,
 *      SCC_VID_BLACK_STRETCH_MID,
 *      SCC_VID_BLACK_STRETCH_HIGH
 *  }   SCC_VID_BLACK_STRETCH_T;
 *  @endcode
 *  @li@c  SCC_VID_BLACK_STRETCH_OFF                         -black stretch off
 *  @li@c  SCC_VID_BLACK_STRETCH_LOW                         -black stretch low
 *  @li@c  SCC_VID_BLACK_STRETCH_MID                         -black stretch middle
 *  @li@c  SCC_VID_BLACK_STRETCH_HIGH                        -black stretch high
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_BLACK_STRETCH_OFF = 0,
    SCC_VID_BLACK_STRETCH_LOW,
    SCC_VID_BLACK_STRETCH_MID,
    SCC_VID_BLACK_STRETCH_HIGH
}   SCC_VID_BLACK_STRETCH_T;

/* Command SCC_CMD_CODE_VIDEO_WHITE_STRETCH *******************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_WHITE_STRETCH_T
 *  @brief  Specify video white stretch.
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_WHITE_STRETCH_OFF = 0,
 *      SCC_VID_WHITE_STRETCH_LOW,
 *      SCC_VID_WHITE_STRETCH_MID,
 *      SCC_VID_WHITE_STRETCH_HIGH
 *  }   SCC_VID_WHITE_STRETCH_T;
 *  @endcode
 *  @li@c  SCC_VID_WHITE_STRETCH_OFF                         -white stretch off
 *  @li@c  SCC_VID_WHITE_STRETCH_LOW                         -white stretch low
 *  @li@c  SCC_VID_WHITE_STRETCH_MID                         -white stretch middle
 *  @li@c  SCC_VID_WHITE_STRETCH_HIGH                        -white stretch high
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_WHITE_STRETCH_OFF = 0,
    SCC_VID_WHITE_STRETCH_LOW,
    SCC_VID_WHITE_STRETCH_MID,
    SCC_VID_WHITE_STRETCH_HIGH
}   SCC_VID_WHITE_STRETCH_T;

/* Command SCC_CMD_CODE_VIDEO_COLORSPACE **********************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_COLORSPACE_T
 *  @brief  Specify video colorspace.
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_COLORSPACE_AUTO = 0,
 *      SCC_VID_COLORSPACE_FORCE_RGB,
 *      SCC_VID_COLORSPACE_FORCE_YCBCR
 *  } SCC_VID_COLORSPACE_T;
 *  @endcode
 *  @li@c  SCC_VID_COLORSPACE_AUTO                           -auto colorspace
 *  @li@c  SCC_VID_COLORSPACE_FORCE_RGB                      -RGB colorspace
 *  @li@c  SCC_VID_COLORSPACE_FORCE_YCBCR                    -YCbCr colorspace
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_COLORSPACE_AUTO = 0,
    SCC_VID_COLORSPACE_FORCE_RGB,
    SCC_VID_COLORSPACE_FORCE_YCBCR
} SCC_VID_COLORSPACE_T;

/* Command SCC_CMD_CODE_VIDEO_SUPER_FREEZE *********************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_VID_SUPER_FREEZE_TYPE_T
 *  @brief  Specify video super freeze type.
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_SUPER_FREEZE_TYPE_MEMO,
 *      SCC_VID_SUPER_FREEZE_TYPE_FORCE
 *  } SCC_VID_SUPER_FREEZE_TYPE_T;
 *  @endcode
 *  @li@c  SCC_VID_SUPER_FREEZE_TYPE_MEMO                       -memo freeze
 *  @li@c  SCC_VID_SUPER_FREEZE_TYPE_FORCE                      -force freeze
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_SUPER_FREEZE_TYPE_MEMO = 0,
    SCC_VID_SUPER_FREEZE_TYPE_FORCE    /* for super freeze */
}   SCC_VID_SUPER_FREEZE_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum SCC_VID_SUPER_FREEZE_T
 *  @brief  Contain video super freeze info.
 *  @code
 *  typedef struct _SCC_VID_SUPER_FREEZE_T
 *  {
 *      SCC_VID_SUPER_FREEZE_TYPE_T e_type;
 *      BOOL                        b_enable;
 *  } SCC_VID_SUPER_FREEZE_T;
 *  @endcode
 *  @li@c  e_type                       -super freeze type
 *  @li@c  b_enable                     -force freeze is enable or not
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_VID_SUPER_FREEZE_T
{
    SCC_VID_SUPER_FREEZE_TYPE_T e_type;
    BOOL                        b_enable;
} SCC_VID_SUPER_FREEZE_T;

/* FORCE UNMUTE********************************************************/
typedef enum
{
    SCC_VID_FORCE_UNMUTE_TYPE_ON_NO_SIG = 0,
    SCC_VID_FORCE_UNMUTE_TYPE_ATV_CHG_CH
}   SCC_VID_FORCE_UNMUTE_TYPE_T;

/* Local dimming */
typedef enum
{
    SCC_VID_LOCAL_DIMMING_TYPE_OFF = 0,
    SCC_VID_LOCAL_DIMMING_TYPE_ON
} SCC_VID_LOCAL_DIMMING_TYPE_T;


/*------------------------------------------------------------------*/
/*! @enum SCC_VID_SRC_DETECT_NFY_COND_T
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      SCC_VID_SRC_DETECT_NFY_COND_ERROR = -1,
 *      SCC_VID_SRC_DETECT_NFY_COND_DONE
 *  } SCC_VID_SRC_DETECT_NFY_COND_T;
 *  @endcode
 *  @li@c  SCC_VID_SRC_DETECT_NFY_COND_ERROR                         -
 *  @li@c  SCC_VID_SRC_DETECT_NFY_COND_DONE                           -  
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_VID_SRC_DETECT_NFY_COND_ERROR = -1,
    SCC_VID_SRC_DETECT_NFY_COND_DONE
} SCC_VID_SRC_DETECT_NFY_COND_T;

/* source detect SCC notify function */
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_scc_vid_src_detect_nfy_fct) (
                               HANDLE_T                         h_vid,
                               SCC_VID_SRC_DETECT_NFY_COND_T    e_nfy_cond,
                               VOID*                            pv_nfy_tag,
                               UINT32                           ui4_data_1,
                               UINT32                           ui4_data_2 );

/* 3D navigation setting */
typedef enum
{
    SCC_3D_NAV_OFF = 0,
    SCC_3D_NAV_ON
}   SCC_3D_NAV_T;

/* 3D mode setting */
typedef enum
{
    SCC_3D_MODE_OFF = 0,
    SCC_3D_MODE_AUTO,
    SCC_3D_MODE_2D_TO_3D,
    SCC_3D_MODE_FRM_SEQ,
    SCC_3D_MODE_SIDE_SIDE,
    SCC_3D_MODE_TOP_AND_BTM,
    SCC_3D_MODE_REALD,
    SCC_3D_MODE_SENSIO,
    SCC_3D_MODE_LINE_INTERLEAVE,
    SCC_3D_MODE_DOT_ALT,
    SCC_3D_MODE_CHK_BOARD,
    SCC_3D_MODE_END
}   SCC_3D_MODE_T;

/* 3D LR Switch */
typedef enum
{
    SCC_3D_LR_SWITCH_OFF = 0,
    SCC_3D_LR_SWITCH_ON
}   SCC_3D_LR_SWITCH_T;

/* 3D_TO_2D setting */
typedef enum
{
    SCC_3D_TO_2D_OFF = 0,
    SCC_3D_TO_2D_LEFT,
    SCC_3D_TO_2D_RIGHT
}   SCC_3D_TO_2D_T;

/* 3D FPR setting*/
typedef enum
{
    SCC_3D_FPR_NORMAL = 0,
    SCC_3D_FPR_FPR1,
    SCC_3D_FPR_FPR2
}   SCC_3D_FPR_T;

/* 3D panel type */
typedef enum
{
    SCC_2D_PANEL = 0,
    SCC_3D_PR_PANEL,
    SCC_3D_SG_PANEL
}   SCC_3D_PANEL_TYPE_T;

/* angular sce setting */
typedef struct _SCC_ANGULAR_SCE_T
{
    UINT8   ui1_color;
    UINT8   ui1_ysh;
    UINT8   ui1_value;    
} SCC_ANGULAR_SCE_T;

/*------------------------------------------------------------------*/
/*! @enum _SCC_VID_DEC_CAPABILITY_INFO_T
 *  @brief  Contain video decoder capability info.
 *  @code
 *  typedef struct _SCC_VID_DEC_CAPABILITY_INFO_T
 *  {
 *      UINT32      ui4_fmt_timing;
 *      UINT32      ui4_codec_type;
 *      BOOL        b_3d_multi_track_available;
 *  } SCC_VID_DEC_CAPABILITY_INFO_T;
 *  @endcode
 *  @li@c  ui4_fmt_timing                  -format timing
 *  @li@c  ui4_codec_type                  -codec type
 *  @li@c  b_3d_multi_track_available      -3d multi-track available is enable or not
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_VID_DEC_CAPABILITY_INFO_T
{
    UINT32      ui4_fmt_timing;
    UINT32      ui4_codec_type;
    BOOL        b_3d_multi_track_available;
} SCC_VID_DEC_CAPABILITY_INFO_T;

/* HDMI RANGE setting*/
typedef enum
{
  SCC_HDMI_RANGE_AUTO = 0,
  SCC_HDMI_RANGE_LIMIT,
  SCC_HDMI_RANGE_FULL
} SCC_HDMI_RANGE_T;

/* VGA detect setting*/
typedef enum
{
  SCC_VGA_ENABLE_DETECTION,
  SCC_VGA_DISABLE_DETECTION
} SCC_VGA_DETECT_T;

/* VGA status */
typedef enum
{
  SCC_VGA_SIGNAL_STABLE,
  SCC_VGA_SIGNAL_NO_SIGNAL,
  SCC_VGA_SIGNAL_NOT_SUPPORT
} SCC_VGA_STATUS_T;

/*------------------------------------------------------------------*/
/*! @struct SCC_VID_FREQ_T
 *  @brief  Contains video v-freq and h-freq.
 *  @code
 *  typedef struct _SCC_VID_FREQ_T
 *  {
 *      BOOL          b_valid;
 *      UINT16        ui2_v_freq;
 *      UINT16        ui2_h_freq;
 *  }   SCC_VID_FREQ_T;
 *  @endcode
 *  @li@c  b_valid                                      -valid or not
 *  @li@c  ui2_v_freq                                   -video v-freqency
 *  @li@c  ui2_h_freq                                   -video h-freqency
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_VID_FREQ_T
{
    BOOL          b_valid;       /* FALSE: not vga source or no signal  */
    UINT16        ui2_v_freq;
    UINT16        ui2_h_freq;    /* AP can use ( ui2_h_freq/10 *1000 ) to get KHz value */
}   SCC_VID_FREQ_T;

/* 3D nav auto change enable */
typedef enum
{
    SCC_3D_NAV_AUTO_CHG_MANUAL = 0,
    SCC_3D_NAV_AUTO_CHG_SEMI_AUTO,
    SCC_3D_NAV_AUTO_CHG_AUTO
} SCC_3D_NAV_AUTO_CHG_T;

/* 3D image safety enable */
typedef enum
{
    SCC_3D_IMAGE_SAFETY_OFF,
    SCC_3D_IMAGE_SAFETY_LOW,
    SCC_3D_IMAGE_SAFETY_MID,
    SCC_3D_IMAGE_SAFETY_HIGH
} SCC_3D_IMAGE_SAFETY_T;

/* 3D nav tag */
typedef enum
{
    SCC_3D_NAV_TAG_2D = 0,
    SCC_3D_NAV_TAG_MVC,          /* MVC = Multi-View Codec */
    SCC_3D_NAV_TAG_FP,           /* FP = Frame Packing */
    SCC_3D_NAV_TAG_FS,           /* FS = Frame Sequential */
    SCC_3D_NAV_TAG_TB,           /* TB = Top-and-Bottom */
    SCC_3D_NAV_TAG_SBS,          /* SBS = Side-by-Side */
    SCC_3D_NAV_TAG_REALD,
    SCC_3D_NAV_TAG_SENSIO,
    SCC_3D_NAV_TAG_LA,           /* LA = Line Alternative */
    SCC_3D_NAV_TAG_TTDO,         /* TTD only */
    SCC_3D_NAV_TAG_NOT_SUPPORT
} SCC_3D_NAV_TAG_T;

typedef struct _SCC_VID_SRC_DET_STATUS_T
{
    UINT8 ui2_av1_status;
    UINT8 ui2_av2_status;
    UINT8 ui2_ypbpr1_status;
    UINT8 ui2_hdmi1_status;
    UINT8 ui2_hdmi2_status;
    UINT8 ui2_hdmi3_status;
    UINT8 ui2_hdmi4_status;
    UINT8 ui2_vga_status;
} SCC_VID_SRC_DET_STATUS_T;

typedef struct _SCC_VID_SCR_MODE_T
{
    UINT16  ui2_scr_mode;        /* screen mode type */
    BOOL    b_scr_mode_enable;   /* whether support designated screen mode */
} SCC_VID_SCR_MODE_T;

/* Command SCC_CMD_CODE_DISP_CTRL ****************************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_DISP_CTRL_TYPE_T
 *  @brief  Specify display control type.
 *  @code
 *  typedef enum
 *  {
 *      SCC_DISP_CTRL_RESET,
 *      SCC_DISP_CTRL_ENABLE,
 *      SCC_DISP_CTRL_DISABLE
 *  } SCC_DISP_CTRL_TYPE_T;
 *  @endcode
 *  @li@c  SCC_DISP_CTRL_RESET                                -control reset
 *  @li@c  SCC_DISP_CTRL_ENABLE                               -control enable
 *  @li@c  SCC_DISP_CTRL_DISABLE                              -control disable
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_DISP_CTRL_RESET,
    SCC_DISP_CTRL_ENABLE,
    SCC_DISP_CTRL_DISABLE
} SCC_DISP_CTRL_TYPE_T;


/* Command SCC_CMD_CODE_DISP_RESOLUTION **********************************/
/*------------------------------------------------------------------*/
/*! @struct SCC_DISP_RESOLUTION_T
 *  @brief  Contains display resolution.
 *  @code
 *  typedef struct _SCC_DISP_RESOLUTION_T
 *  {
 *      UINT16               ui2_width;
 *      UINT16               ui2_height;
 *      BOOL                 b_is_progressive;
 *      UINT16               ui2_curr_v_clk;
 *      UINT16               ui2_v_clk_max;
 *      UINT16               ui2_v_clk_min;
 *  } SCC_DISP_RESOLUTION_T;
 *  @endcode
 *  @li@c  ui2_width                                         -display width
 *  @li@c  ui2_height                                        -display Height
 *  @li@c  b_is_progressive                                  -progressive or interlace
 *  @li@c  ui2_curr_v_clk                                    -current vertical clock
 *  @li@c  ui2_v_clk_max                                     -maximum vertical clock
 *  @li@c  ui2_v_clk_min                                     -minimum vertical clock
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_DISP_RESOLUTION_T
{
    UINT16               ui2_width;
    UINT16               ui2_height;
    BOOL                 b_is_progressive;
    UINT16               ui2_curr_v_clk;
    UINT16               ui2_v_clk_max; /* Ignored in command or set function */
    UINT16               ui2_v_clk_min; /* Ignored in command or set function */
    UINT16               ui2_panel_width;
    UINT16               ui2_panel_height;
} SCC_DISP_RESOLUTION_T;

typedef struct _SCC_DISP_SCREEN_RESOLUTION_T
{
    UINT16               ui2_screen_width;
    UINT16               ui2_screen_height;
} SCC_DISP_SCREEN_RESOLUTION_T;

/* Command SCC_CMD_CODE_DISP_ASPECT_RATIO ********************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_DISP_ASPECT_RATIO_T
 *  @brief  Specify display aspect ratio.
 *  @code
 *  typedef enum
 *  {
 *      SCC_DISP_ASPECT_RATIO_UNKNOWN = 0,
 *      SCC_DISP_ASPECT_RATIO_4_3,
 *      SCC_DISP_ASPECT_RATIO_16_9,
 *      SCC_DISP_ASPECT_RATIO_2_21_1
 *  } SCC_DISP_ASPECT_RATIO_T;
 *  @endcode
 *  @li@c  SCC_DISP_ASPECT_RATIO_UNKNOWN                   -unknown ratio
 *  @li@c  SCC_DISP_ASPECT_RATIO_4_3                       -4:3 ratio
 *  @li@c  SCC_DISP_ASPECT_RATIO_16_9                      -16:9 ratio
 *  @li@c  SCC_DISP_ASPECT_RATIO_2_21_1                    -2_21_1 ratio
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_DISP_ASPECT_RATIO_UNKNOWN = 0,
    SCC_DISP_ASPECT_RATIO_4_3,
    SCC_DISP_ASPECT_RATIO_16_9,
    SCC_DISP_ASPECT_RATIO_2_21_1
} SCC_DISP_ASPECT_RATIO_T;


/* Command SCC_CMD_CODE_DISP_FMT *****************************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_DISP_FMT_T
 *  @brief  Specify display format.
 *  @code
 *  typedef enum
 *  {
 *      SCC_DISP_FMT_NULL     = 0x00,
 *      SCC_DISP_FMT_CVBS     = 0x01,
 *      SCC_DISP_FMT_S_VIDEO  = 0x02,
 *      SCC_DISP_FMT_YBR      = 0x04,
 *      SCC_DISP_FMT_RGB      = 0x08
 *  }   SCC_DISP_FMT_T;
 *  @endcode
 *  @li@c  SCC_DISP_FMT_NULL                                 -NULL
 *  @li@c  SCC_DISP_FMT_CVBS                                 -CVBS format
 *  @li@c  SCC_DISP_FMT_S_VIDEO                              -S_VIDEO format
 *  @li@c  SCC_DISP_FMT_YBR                                  -YBR format
 *  @li@c  SCC_DISP_FMT_RGB                                  -RGB format
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_DISP_FMT_NULL     = 0x00,
    SCC_DISP_FMT_CVBS     = 0x01,
    SCC_DISP_FMT_S_VIDEO  = 0x02,
    SCC_DISP_FMT_YBR      = 0x04,
    SCC_DISP_FMT_RGB      = 0x08
}   SCC_DISP_FMT_T;

typedef UINT32 SCC_DISP_FMT_CAP_T;    /**<        */

/* Command SCC_CMD_CODE_DISP_DIGIT_OUTPUT ********************************/
#define SCC_DISP_DIGIT_OUT_NONE             ((UINT32) 0)    /**<        */
#define SCC_DISP_DIGIT_OUT_DIGITAL          MAKE_BIT_MASK_32( 0 )    /**<        */
#define SCC_DISP_DIGIT_OUT_LVDS             MAKE_BIT_MASK_32( 1 )    /**<        */
#define SCC_DISP_DIGIT_OUT_CRT              MAKE_BIT_MASK_32( 2 )    /**<        */

typedef UINT32 SCC_DISP_DIGIT_OUT_MASK_T;    /**<        */


/* Command SCC_CMD_CODE_DISP_TV_TYPE *************************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_DISP_TV_TYPE_T
 *  @brief  Specify display TV type.
 *  @code
 *  typedef enum
 *  {
 *      SCC_DISP_TV_TYPE_NTSC_M,
 *      SCC_DISP_TV_TYPE_PAL_B,
 *      SCC_DISP_TV_TYPE_PAL_G,
 *      SCC_DISP_TV_TYPE_PAL_H,
 *      SCC_DISP_TV_TYPE_PAL_I,
 *      SCC_DISP_TV_TYPE_PAL_D,
 *      SCC_DISP_TV_TYPE_PAL_N,
 *      SCC_DISP_TV_TYPE_PAL_M,
 *      SCC_DISP_TV_TYPE_SECAM_B,
 *      SCC_DISP_TV_TYPE_SECAM_G,
 *      SCC_DISP_TV_TYPE_SECAM_H,
 *      SCC_DISP_TV_TYPE_SECAM_D,
 *      SCC_DISP_TV_TYPE_SECAM_K,
 *      SCC_DISP_TV_TYPE_SECAM_K1,
 *      SCC_DISP_TV_TYPE_SECAM_L
 *  }   SCC_DISP_TV_TYPE_T;
 *  @endcode
 *  @li@c  SCC_DISP_TV_TYPE_NTSC_M                             -NTSC_M type
 *  @li@c  SCC_DISP_TV_TYPE_PAL_B                              -PAL_B type
 *  @li@c  SCC_DISP_TV_TYPE_PAL_G                              -PAL_G type
 *  @li@c  SCC_DISP_TV_TYPE_PAL_H                              -PAL_H type
 *  @li@c  SCC_DISP_TV_TYPE_PAL_I                              -PAL_I type
 *  @li@c  SCC_DISP_TV_TYPE_PAL_D                              -PAL_D type
 *  @li@c  SCC_DISP_TV_TYPE_PAL_N                              -PAL_N type
 *  @li@c  SCC_DISP_TV_TYPE_PAL_M                              -PAL_M type
 *  @li@c  SCC_DISP_TV_TYPE_SECAM_B                            -SECAM_B type
 *  @li@c  SCC_DISP_TV_TYPE_SECAM_G                            -SECAM_G type
 *  @li@c  SCC_DISP_TV_TYPE_SECAM_H                            -SECAM_H type
 *  @li@c  SCC_DISP_TV_TYPE_SECAM_D                            -SECAM_D type
 *  @li@c  SCC_DISP_TV_TYPE_SECAM_K                            -SECAM_K type
 *  @li@c  SCC_DISP_TV_TYPE_SECAM_K1                           -SECAM_K1 type
 *  @li@c  SCC_DISP_TV_TYPE_SECAM_L                            -SECAM_L type
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_DISP_TV_TYPE_NTSC_M,
    SCC_DISP_TV_TYPE_PAL_B,
    SCC_DISP_TV_TYPE_PAL_G,
    SCC_DISP_TV_TYPE_PAL_H,
    SCC_DISP_TV_TYPE_PAL_I,
    SCC_DISP_TV_TYPE_PAL_D,
    SCC_DISP_TV_TYPE_PAL_N,
    SCC_DISP_TV_TYPE_PAL_M,
    SCC_DISP_TV_TYPE_SECAM_B,
    SCC_DISP_TV_TYPE_SECAM_G,
    SCC_DISP_TV_TYPE_SECAM_H,
    SCC_DISP_TV_TYPE_SECAM_D,
    SCC_DISP_TV_TYPE_SECAM_K,
    SCC_DISP_TV_TYPE_SECAM_K1,
    SCC_DISP_TV_TYPE_SECAM_L
}   SCC_DISP_TV_TYPE_T;

/* Command SCC_CMD_CODE_DISP_DIGIT_OUTPUT_FMT ****************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_DISP_DIGIT_OUTPUT_FMT_T
 *  @brief  Specify display digit output format.
 *  @code
 *  typedef enum
 *  {
 *      SCC_DISP_DIGIT_OUTPUT_FMT_YBR,
 *      SCC_DISP_DIGIT_OUTPUT_FMT_RGB
 *  } SCC_DISP_DIGIT_OUTPUT_FMT_T;
 *  @endcode
 *  @li@c  SCC_DISP_DIGIT_OUTPUT_FMT_YBR                     -YBR format
 *  @li@c  SCC_DISP_DIGIT_OUTPUT_FMT_RGB                     -RGB format
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_DISP_DIGIT_OUTPUT_FMT_YBR,
    SCC_DISP_DIGIT_OUTPUT_FMT_RGB
} SCC_DISP_DIGIT_OUTPUT_FMT_T;

/* Command SCC_CMD_CODE_DISP_COLOR_GAIN **********************************/
/* Command SCC_CMD_CODE_DISP_COLOR_OFFSET ********************************/
/*------------------------------------------------------------------*/
/*! @struct SCC_DISP_COLOR_GAIN_T
 *  @brief  Contains display color gain.
 *  @code
 *  typedef struct _SCC_DISP_COLOR_GAIN_T
 *  {
 *      UINT16      ui2_r_gain;
 *      UINT16      ui2_g_gain;
 *      UINT16      ui2_b_gain;
 *  } SCC_DISP_COLOR_GAIN_T;
 *  @endcode
 *  @li@c  ui2_r_gain                                        -red gain
 *  @li@c  ui2_g_gain                                        -green gain
 *  @li@c  ui2_b_gain                                        -blue gain
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_DISP_COLOR_GAIN_T
{
    UINT16      ui2_r_gain;
    UINT16      ui2_g_gain;
    UINT16      ui2_b_gain;
} SCC_DISP_COLOR_GAIN_T;

/* Command SCC_CMD_CODE_DISP_TV_MODE *************************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_TV_MODE_T
 *  @brief  Specify TV mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_TV_MODE_NORMAL = 0,
 *      SCC_TV_MODE_PIP,
 *      SCC_TV_MODE_POP
 *  }   SCC_TV_MODE_T;
 *  @endcode
 *  @li@c  SCC_TV_MODE_NORMAL                                -normal mode
 *  @li@c  SCC_TV_MODE_PIP                                   -PIP mode
 *  @li@c  SCC_TV_MODE_POP                                   -POP mode
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_TV_MODE_NORMAL = 0,
    SCC_TV_MODE_PIP,
    SCC_TV_MODE_POP
}   SCC_TV_MODE_T;

/* Command SCC_CMD_CODE_DISP_MJC *****************************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_DISP_MJC_T
 *  @brief  Specify display MJC.
 *  @code
 *  typedef enum
 *  {
 *      SCC_DISP_MJC_OFF = 0,
 *      SCC_DISP_MJC_ON,
 *      SCC_DISP_MJC_DNM
 *  }   SCC_DISP_MJC_T;
 *  @endcode
 *  @li@c  SCC_DISP_MJC_OFF                                  -MJC off
 *  @li@c  SCC_DISP_MJC_ON                                   -MJC on
 *  @li@c  SCC_DISP_MJC_DNM                                  -MJC dynamic
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_DISP_MJC_OFF = 0,
    SCC_DISP_MJC_ON,
    SCC_DISP_MJC_DNM
}   SCC_DISP_MJC_T;

/* Command SCC_CMD_CODE_DISP_TEST_PATTERN ********************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_DISP_TEST_PATTERN_T
 *  @brief  Specify display test pattern.
 *  @code
 *  typedef enum
 *  {
 *      SCC_DISP_TEST_PATTERN_DISABLE = 0,
 *      SCC_DISP_TEST_PATTERN_1,
 *      SCC_DISP_TEST_PATTERN_2,
 *      SCC_DISP_TEST_PATTERN_3,
 *      SCC_DISP_TEST_PATTERN_4,
 *      SCC_DISP_TEST_PATTERN_5,
 *      SCC_DISP_TEST_PATTERN_6,
 *      SCC_DISP_TEST_PATTERN_7,
 *      SCC_DISP_TEST_PATTERN_8
 *  }   SCC_DISP_TEST_PATTERN_T;
 *  @endcode
 *  @li@c  SCC_DISP_TEST_PATTERN_DISABLE                          -test pattern disable
 *  @li@c  SCC_DISP_TEST_PATTERN_1                                -test pattern 1
 *  @li@c  SCC_DISP_TEST_PATTERN_2                                -test pattern 2
 *  @li@c  SCC_DISP_TEST_PATTERN_3                                -test pattern 3
 *  @li@c  SCC_DISP_TEST_PATTERN_4                                -test pattern 4
 *  @li@c  SCC_DISP_TEST_PATTERN_5                                -test pattern 5
 *  @li@c  SCC_DISP_TEST_PATTERN_6                                -test pattern 6
 *  @li@c  SCC_DISP_TEST_PATTERN_7                                -test pattern 7
 *  @li@c  SCC_DISP_TEST_PATTERN_8                                -test pattern 8
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_DISP_TEST_PATTERN_DISABLE = 0,
    SCC_DISP_TEST_PATTERN_1,
    SCC_DISP_TEST_PATTERN_2,
    SCC_DISP_TEST_PATTERN_3,
    SCC_DISP_TEST_PATTERN_4,
    SCC_DISP_TEST_PATTERN_5,
    SCC_DISP_TEST_PATTERN_6,
    SCC_DISP_TEST_PATTERN_7,
    SCC_DISP_TEST_PATTERN_8
}   SCC_DISP_TEST_PATTERN_T;

#ifdef MW_TV_AV_BYPASS_SUPPORT
/* Command SCC_CMD_CODE_DISP_BYPASS_MODE *********************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_DISP_BYPASS_MODE_T
 *  @brief   Specify display bypass mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_DISP_BYPASS_NORMAL  = 0x0,
 *      SCC_DISP_BYPASS_BLANK   = 0x1
 *  }   SCC_DISP_BYPASS_MODE_T;
 *  @endcode
 *  @li@c  SCC_DISP_BYPASS_NORMAL                            -bypass normal mode
 *  @li@c  SCC_DISP_BYPASS_BLANK                             -bypass blank mode
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_DISP_BYPASS_NORMAL  = 0x0,
    SCC_DISP_BYPASS_BLANK   = 0x1
}   SCC_DISP_BYPASS_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum SCC_DISP_TVE_CTRL_MODE_T
 *  @brief  Specify display TVE control mode.
 *  @code
 *  typedef enum
 *  {
 *     SCC_DISP_TVE_CTRL_ENABLE  = 0x0,
 *     SCC_DISP_TVE_CTRL_DISABLE = 0x1,
 *  }  SCC_DISP_TVE_CTRL_MODE_T;
 *  @endcode
 *  @li@c  SCC_DISP_TVE_CTRL_ENABLE                          -TVE control enable
 *  @li@c  SCC_DISP_TVE_CTRL_DISABLE                         -TVE control disable
 */
/*------------------------------------------------------------------*/
typedef enum
{
   SCC_DISP_TVE_CTRL_ENABLE  = 0x0,
   SCC_DISP_TVE_CTRL_DISABLE = 0x1,
}  SCC_DISP_TVE_CTRL_MODE_T;
#endif

/* Command SCC_CMD_CODE_DISP_ADP_BACK_LIGHT_RNG **************************/
/*------------------------------------------------------------------*/
/*! @struct SCC_DISP_RANGE_INFO_T
 *  @brief  Contains display range info.
 *  @code
 *  typedef struct _SCC_DISP_RANGE_INFO_T
 *  {
 *      UINT32  ui4_min_value;
 *      UINT32  ui4_max_value;
 *  } SCC_DISP_RANGE_INFO_T;
 *  @endcode
 *  @li@c  ui4_min_value                                     -minimum value
 *  @li@c  ui4_max_value                                     -maximum value
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_DISP_RANGE_INFO_T
{
    UINT32  ui4_min_value;
    UINT32  ui4_max_value;
} SCC_DISP_RANGE_INFO_T;

/* Command SCC_CMD_CODE_AUDIO_SPDIF_TYPE *********************************/
/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_SPDIF_FMT_T
 *  @brief  Specify audio spdif format.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_SPDIF_FMT_OFF = 0,
 *      SCC_AUD_SPDIF_FMT_RAW,
 *      SCC_AUD_SPDIF_FMT_PCM_16,
 *      SCC_AUD_SPDIF_FMT_PCM_24
 *  }   SCC_AUD_SPDIF_FMT_T;
 *  @endcode
 *  @li@c  SCC_AUD_SPDIF_FMT_OFF                             -off
 *  @li@c  SCC_AUD_SPDIF_FMT_RAW                             -raw format
 *  @li@c  SCC_AUD_SPDIF_FMT_PCM_16                          -PCM 16 format
 *  @li@c  SCC_AUD_SPDIF_FMT_PCM_24                          -PCM 24 format
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_SPDIF_FMT_OFF = 0,
    SCC_AUD_SPDIF_FMT_RAW,
    SCC_AUD_SPDIF_FMT_PCM_16,
    SCC_AUD_SPDIF_FMT_PCM_24
}   SCC_AUD_SPDIF_FMT_T;

/* Command SCC_CMD_CODE_AUDIO_HDCD_FLTR **********************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_AUD_HDCD_FLTR_T
 *  @brief  Specify audio HDCD filter type.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_HDCD_FLTR_OFF = 0,
 *      SCC_AUD_HDCD_FLTR_1X,
 *      SCC_AUD_HDCD_FLTR_2X
 *  }   SCC_AUD_HDCD_FLTR_T;
 *  @endcode
 *  @li@c  SCC_AUD_HDCD_FLTR_OFF                             -off
 *  @li@c  SCC_AUD_HDCD_FLTR_1X                              -1X
 *  @li@c  SCC_AUD_HDCD_FLTR_2X                              -2X
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_HDCD_FLTR_OFF = 0,
    SCC_AUD_HDCD_FLTR_1X,
    SCC_AUD_HDCD_FLTR_2X
}   SCC_AUD_HDCD_FLTR_T;

/* Command SCC_CMD_CODE_AUDIO_LOUD_SPEAKER ******************************/
/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_LOUD_SPEAKER_MODE_T
 *  @brief  Specify audio loud speaker mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_LOUD_SPEAKER_MODE_OFF = 0,
 *      SCC_AUD_LOUD_SPEAKER_MODE_LARGE_FREQ_RANGE,
 *      SCC_AUD_LOUD_SPEAKER_MODE_SMALL_FREQ_RANGE
 *  }   SCC_AUD_LOUD_SPEAKER_MODE_T;
 *  @endcode
 *  @li@c  SCC_AUD_LOUD_SPEAKER_MODE_OFF                     -off
 *  @li@c  SCC_AUD_LOUD_SPEAKER_MODE_LARGE_FREQ_RANGE        -large frequency range
 *  @li@c  SCC_AUD_LOUD_SPEAKER_MODE_SMALL_FREQ_RANGE        -small frequency range
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_LOUD_SPEAKER_MODE_OFF = 0,
    SCC_AUD_LOUD_SPEAKER_MODE_LARGE_FREQ_RANGE,
    SCC_AUD_LOUD_SPEAKER_MODE_SMALL_FREQ_RANGE
}   SCC_AUD_LOUD_SPEAKER_MODE_T;

/*------------------------------------------------------------------*/
/*! @struct  SCC_AUD_LOUD_SPEAKER_T
 *  @brief  Contains audio loud speaker info.
 *  @code
 *  typedef struct _SCC_AUD_LOUD_SPEAKER_T
 *  {
 *      SCC_AUD_CHANNEL_T             e_channel;
 *      SCC_AUD_LOUD_SPEAKER_MODE_T   e_mode;
 *  }   SCC_AUD_LOUD_SPEAKER_T;
 *  @endcode
 *  @li@c  e_channel                                         -channel
 *  @li@c  e_mode                                            -mode
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AUD_LOUD_SPEAKER_T
{
    SCC_AUD_CHANNEL_T             e_channel;
    SCC_AUD_LOUD_SPEAKER_MODE_T   e_mode;
}   SCC_AUD_LOUD_SPEAKER_T;

/* Command SCC_CMD_CODE_AUDIO_SE_TRIM *******************************/
/*------------------------------------------------------------------*/
/*! @struct  SCC_AUD_SE_TRIM_T
 *  @brief Contains audio sound effect trim info.
 *  @code
 *  typedef struct _SCC_AUD_SE_TRIM_T
 *  {
 *      SCC_AUD_CHANNEL_T   e_channel;
 *      UINT8               ui1_level;
 *  }   SCC_AUD_SE_TRIM_T;
 *  @endcode
 *  @li@c  e_channel                                         -channel
 *  @li@c  ui1_level                                         -level
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AUD_SE_TRIM_T
{
    SCC_AUD_CHANNEL_T   e_channel;
    UINT8               ui1_level;
}   SCC_AUD_SE_TRIM_T;

/* Command SCC_CMD_CODE_AUDIO_SE_SUPERBASS *******************************/
/* BOOL */

/* Command SCC_CMD_CODE_AUDIO_SE_EQUALIZER *******************************/
/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_SE_EQ_T
 *  @brief  Specify audio sound effect EQ type.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_SE_EQ_OFF = 0,
 *      SCC_AUD_SE_EQ_ROCK,
 *      SCC_AUD_SE_EQ_POP,
 *      SCC_AUD_SE_EQ_LIVE,
 *      SCC_AUD_SE_EQ_DANCE,
 *      SCC_AUD_SE_EQ_TECHNO,
 *      SCC_AUD_SE_EQ_CLASSIC,
 *      SCC_AUD_SE_EQ_SOFT,
 *      SCC_AUD_SE_EQ_USER_1,
 *      SCC_AUD_SE_EQ_USER_2,
 *      SCC_AUD_SE_EQ_USER_3,
 *      SCC_AUD_SE_EQ_USER_4,
 *      SCC_AUD_SE_EQ_USER_5,
 *      SCC_AUD_SE_EQ_USER_6,
 *      SCC_AUD_SE_EQ_USER_7,
 *      SCC_AUD_SE_EQ_USER_8,
 *      SCC_AUD_SE_EQ_BBE_TV,
 *      SCC_AUD_SE_EQ_BBE_AV
 *  }   SCC_AUD_SE_EQ_T;
 *  @endcode
 *  @li@c  SCC_AUD_SE_EQ_OFF                                 -off
 *  @li@c  SCC_AUD_SE_EQ_ROCK                                -rock
 *  @li@c  SCC_AUD_SE_EQ_POP                                 -pop
 *  @li@c  SCC_AUD_SE_EQ_LIVE                                -live
 *  @li@c  SCC_AUD_SE_EQ_DANCE                               -dance
 *  @li@c  SCC_AUD_SE_EQ_TECHNO                              -techno
 *  @li@c  SCC_AUD_SE_EQ_CLASSIC                             -classic
 *  @li@c  SCC_AUD_SE_EQ_SOFT                                -soft
 *  @li@c  SCC_AUD_SE_EQ_USER_1                              -user1
 *  @li@c  SCC_AUD_SE_EQ_USER_2                              -user2
 *  @li@c  SCC_AUD_SE_EQ_USER_3                              -user3
 *  @li@c  SCC_AUD_SE_EQ_USER_4                              -user4
 *  @li@c  SCC_AUD_SE_EQ_USER_5                              -user5
 *  @li@c  SCC_AUD_SE_EQ_USER_6                              -user6
 *  @li@c  SCC_AUD_SE_EQ_USER_7                              -user7
 *  @li@c  SCC_AUD_SE_EQ_USER_8                              -user8
 *  @li@c  SCC_AUD_SE_EQ_BBE_TV                              -BBE TV
 *  @li@c  SCC_AUD_SE_EQ_BBE_AV                              -BBE AV
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_SE_EQ_OFF = 0,
    SCC_AUD_SE_EQ_ROCK,
    SCC_AUD_SE_EQ_POP,
    SCC_AUD_SE_EQ_LIVE,
    SCC_AUD_SE_EQ_DANCE,
    SCC_AUD_SE_EQ_TECHNO,
    SCC_AUD_SE_EQ_CLASSIC,
    SCC_AUD_SE_EQ_SOFT,
    SCC_AUD_SE_EQ_USER_1,
    SCC_AUD_SE_EQ_USER_2,
    SCC_AUD_SE_EQ_USER_3,
    SCC_AUD_SE_EQ_USER_4,
    SCC_AUD_SE_EQ_USER_5,
    SCC_AUD_SE_EQ_USER_6,
    SCC_AUD_SE_EQ_USER_7,
    SCC_AUD_SE_EQ_USER_8,
    SCC_AUD_SE_EQ_BBE_TV,
    SCC_AUD_SE_EQ_BBE_AV
}   SCC_AUD_SE_EQ_T;

/* Command SCC_CMD_CODE_AUDIO_SE_REVERB *******************************/
/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_SE_REVERB_MODE_T
 *  @brief  Specify audio sound effect reverb mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_SE_REVERB_OFF = 0,
 *      SCC_AUD_SE_REVERB_CONCERT,
 *      SCC_AUD_SE_REVERB_LIVINGROOM,
 *      SCC_AUD_SE_REVERB_HALL,
 *      SCC_AUD_SE_REVERB_BATHROOM,
 *      SCC_AUD_SE_REVERB_CAVE,
 *      SCC_AUD_SE_REVERB_ARENA,
 *      SCC_AUD_SE_REVERB_CHURCH
 *  }   SCC_AUD_SE_REVERB_MODE_T;
 *  @endcode
 *  @li@c  SCC_AUD_SE_REVERB_OFF                             -off
 *  @li@c  SCC_AUD_SE_REVERB_CONCERT                         -concert
 *  @li@c  SCC_AUD_SE_REVERB_LIVINGROOM                      -livingroom
 *  @li@c  SCC_AUD_SE_REVERB_HALL                            -hall
 *  @li@c  SCC_AUD_SE_REVERB_BATHROOM                        -bathroom
 *  @li@c  SCC_AUD_SE_REVERB_CAVE                            -cave
 *  @li@c  SCC_AUD_SE_REVERB_ARENA                           -arena
 *  @li@c  SCC_AUD_SE_REVERB_CHURCH                          -church
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_SE_REVERB_OFF = 0,
    SCC_AUD_SE_REVERB_CONCERT,
    SCC_AUD_SE_REVERB_LIVINGROOM,
    SCC_AUD_SE_REVERB_HALL,
    SCC_AUD_SE_REVERB_BATHROOM,
    SCC_AUD_SE_REVERB_CAVE,
    SCC_AUD_SE_REVERB_ARENA,
    SCC_AUD_SE_REVERB_CHURCH
}   SCC_AUD_SE_REVERB_MODE_T;


/* Command SCC_CMD_CODE_AUDIO_SE_SOUND_MODE *******************************/
/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_SE_SOUND_MODE_T
 *  @brief  Specify sound effect sound mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_SE_SOUND_MODE_DEFAULT = 0,
 *      SCC_AUD_SE_SOUND_MODE_STANDARD ,
 *      SCC_AUD_SE_SOUND_MODE_DYNAMIC,
 *      SCC_AUD_SE_SOUND_MODE_CLEAR_VOICE,
 *      SCC_AUD_SE_SOUND_MODE_CUSTOM_1,
 *      SCC_AUD_SE_SOUND_MODE_CUSTOM_2,
 *      SCC_AUD_SE_SOUND_MODE_CUSTOM_3,
 *      SCC_AUD_SE_SOUND_MODE_CUSTOM_4,
 *      SCC_AUD_SE_SOUND_MODE_CUSTOM_5
 *  }   SCC_AUD_SE_SOUND_MODE_T;
 *  @endcode
 *  @li@c  SCC_AUD_SE_SOUND_MODE_DEFAULT                     -default
 *  @li@c  SCC_AUD_SE_SOUND_MODE_STANDARD                    -standard
 *  @li@c  SCC_AUD_SE_SOUND_MODE_DYNAMIC                     -dynamic
 *  @li@c  SCC_AUD_SE_SOUND_MODE_CLEAR_VOICE                 -clear voice
 *  @li@c  SCC_AUD_SE_SOUND_MODE_CUSTOM_1                    -customer 1
 *  @li@c  SCC_AUD_SE_SOUND_MODE_CUSTOM_2                    -customer 2
 *  @li@c  SCC_AUD_SE_SOUND_MODE_CUSTOM_3                    -customer 3
 *  @li@c  SCC_AUD_SE_SOUND_MODE_CUSTOM_4                    -customer 4
 *  @li@c  SCC_AUD_SE_SOUND_MODE_CUSTOM_5                    -customer 5
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_SE_SOUND_MODE_DEFAULT = 0,
    SCC_AUD_SE_SOUND_MODE_STANDARD ,
    SCC_AUD_SE_SOUND_MODE_DYNAMIC,
    SCC_AUD_SE_SOUND_MODE_CLEAR_VOICE,
    SCC_AUD_SE_SOUND_MODE_CUSTOM_1,
    SCC_AUD_SE_SOUND_MODE_CUSTOM_2,
    SCC_AUD_SE_SOUND_MODE_CUSTOM_3,
    SCC_AUD_SE_SOUND_MODE_CUSTOM_4,
    SCC_AUD_SE_SOUND_MODE_CUSTOM_5
}   SCC_AUD_SE_SOUND_MODE_T;

/* Command SCC_CMD_CODE_AUDIO_SE_BASS *******************************/
/* BOOL */

/* Command SCC_CMD_CODE_AUDIO_SE_POSTDRC *******************************/
/* BOOL */

/* Command SCC_CMD_CODE_AUDIO_SE_VOLUME *******************************/
/* SCC_AUD_VOLUME_INFO_T */

/* Command SCC_CMD_CODE_AUDIO_DOWNMIX_MODE *******************************/
/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_DOWNMIX_MODE_T
 *  @brief  Specify audio downmix mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_DOWNMIX_MODE_OFF = 0,
 *      SCC_AUD_DOWNMIX_MODE_LT_RT,
 *      SCC_AUD_DOWNMIX_MODE_STEREO,
 *      SCC_AUD_DOWNMIX_MODE_VIR_SURR,
 *      SCC_AUD_DOWNMIX_MODE_MONO,
 *      SCC_AUD_DOWNMIX_MODE_DUAL1,
 *      SCC_AUD_DOWNMIX_MODE_DUAL2,
 *      SCC_AUD_DOWNMIX_MODE_DUAL_MIX,
 *      SCC_AUD_DOWNMIX_MODE_3_STEREO
 *  }   SCC_AUD_DOWNMIX_MODE_T;
 *  @endcode
 *  @li@c  SCC_AUD_DOWNMIX_MODE_OFF                          -off
 *  @li@c  SCC_AUD_DOWNMIX_MODE_LT_RT                        -LT_RT
 *  @li@c  SCC_AUD_DOWNMIX_MODE_STEREO                       -Stereo
 *  @li@c  SCC_AUD_DOWNMIX_MODE_VIR_SURR                     -virtual surround
 *  @li@c  SCC_AUD_DOWNMIX_MODE_MONO                         -MONO
 *  @li@c  SCC_AUD_DOWNMIX_MODE_DUAL1                        -default 1
 *  @li@c  SCC_AUD_DOWNMIX_MODE_DUAL2                        -default 2
 *  @li@c  SCC_AUD_DOWNMIX_MODE_DUAL_MIX                     -dual mix
 *  @li@c  SCC_AUD_DOWNMIX_MODE_3_STEREO                     -3 Stereo
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_DOWNMIX_MODE_OFF = 0,
    SCC_AUD_DOWNMIX_MODE_LT_RT,
    SCC_AUD_DOWNMIX_MODE_STEREO,
    SCC_AUD_DOWNMIX_MODE_VIR_SURR,
    SCC_AUD_DOWNMIX_MODE_MONO,
    SCC_AUD_DOWNMIX_MODE_DUAL1,
    SCC_AUD_DOWNMIX_MODE_DUAL2,
    SCC_AUD_DOWNMIX_MODE_DUAL_MIX,
    SCC_AUD_DOWNMIX_MODE_3_STEREO,
    SCC_AUD_DOWNMIX_MODE_DUAL_OFF
}   SCC_AUD_DOWNMIX_MODE_T;

/* Command SCC_CMD_CODE_AUDIO_DOLBY_DRC **********************************/
/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_DOLBY_DRC_T
 *  @brief  Specify audio dolby DRC mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_DOLBY_DRC_OFF = 0,
 *      SCC_AUD_DOLBY_DRC_7_8,
 *      SCC_AUD_DOLBY_DRC_3_4,
 *      SCC_AUD_DOLBY_DRC_5_8,
 *      SCC_AUD_DOLBY_DRC_1_2,
 *      SCC_AUD_DOLBY_DRC_3_8,
 *      SCC_AUD_DOLBY_DRC_1_4,
 *      SCC_AUD_DOLBY_DRC_1_8,
 *      SCC_AUD_DOLBY_DRC_FULL
 *  }   SCC_AUD_DOLBY_DRC_T;
 *  @endcode
 *  @li@c  SCC_AUD_DOLBY_DRC_OFF                             -DOLBY off
 *  @li@c  SCC_AUD_DOLBY_DRC_7_8                             -DOLBY 7_8
 *  @li@c  SCC_AUD_DOLBY_DRC_3_4                             -DOLBY 3_4
 *  @li@c  SCC_AUD_DOLBY_DRC_5_8                             -DOLBY 5_8
 *  @li@c  SCC_AUD_DOLBY_DRC_1_2                             -DOLBY 1_2
 *  @li@c  SCC_AUD_DOLBY_DRC_3_8                             -DOLBY 3_8
 *  @li@c  SCC_AUD_DOLBY_DRC_1_4                             -DOLBY 1_4
 *  @li@c  SCC_AUD_DOLBY_DRC_1_8                             -DOLBY 1_8
 *  @li@c  SCC_AUD_DOLBY_DRC_FULL                            -DOLBY full
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_DOLBY_DRC_OFF = 0,
    SCC_AUD_DOLBY_DRC_10,
    SCC_AUD_DOLBY_DRC_20,
    SCC_AUD_DOLBY_DRC_30,
    SCC_AUD_DOLBY_DRC_40,
    SCC_AUD_DOLBY_DRC_50,
    SCC_AUD_DOLBY_DRC_60,
    SCC_AUD_DOLBY_DRC_70,
    SCC_AUD_DOLBY_DRC_80,
    SCC_AUD_DOLBY_DRC_90,
    SCC_AUD_DOLBY_DRC_100
}   SCC_AUD_DOLBY_DRC_T;
#define SCC_AUD_DOLBY_DRC_FULL SCC_AUD_DOLBY_DRC_100

/* Compression Dolby Dynamic Range */
typedef enum
{
    SCC_AUD_CMPSS_DRC_MODE_COMPRESSED = 0,
    SCC_AUD_CMPSS_DRC_MODE_STANDARD,
    SCC_AUD_CMPSS_DRC_MODE_OFF
}   SCC_AUD_CMPSS_DRC_MODE_T;

/* Command SCC_CMD_CODE_AUDIO_OUT_PORT ***********************************/
/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_OUT_PORT_T
 *  @brief  Specify audio out port bitmask.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_OUT_PORT_OFF            = 0x00,
 *      SCC_AUD_OUT_PORT_2_CH           = 0x01,
 *      SCC_AUD_OUT_PORT_5_1_CH         = 0x02,
 *      SCC_AUD_OUT_PORT_SPDIF          = 0x04,
 *      SCC_AUD_OUT_PORT_2_CH_BY_PASS   = 0x08,
 *      SCC_AUD_OUT_PORT_SPEAKER        = 0x10,
 *      SCC_AUD_OUT_PORT_HEADPHONE      = 0x20,
 *      SCC_AUD_OUT_PORT_TVSCART        = 0x40
 *  }   SCC_AUD_OUT_PORT_T;
 *  @endcode
 *  @li@c  SCC_AUD_OUT_PORT_OFF                              -off
 *  @li@c  SCC_AUD_OUT_PORT_2_CH                             -2 channel
 *  @li@c  SCC_AUD_OUT_PORT_5_1_CH                           -5.1 channel
 *  @li@c  SCC_AUD_OUT_PORT_SPDIF                            -SPDIF
 *  @li@c  SCC_AUD_OUT_PORT_2_CH_BY_PASS                     -2 channel bypass
 *  @li@c  SCC_AUD_OUT_PORT_SPEAKER                          -speaker
 *  @li@c  SCC_AUD_OUT_PORT_HEADPHONE                        -headphone
 *  @li@c  SCC_AUD_OUT_PORT_TVSCART                          -TVSCART
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_OUT_PORT_OFF            = 0x00,
    SCC_AUD_OUT_PORT_2_CH           = 0x01,
    SCC_AUD_OUT_PORT_5_1_CH         = 0x02,
    SCC_AUD_OUT_PORT_SPDIF          = 0x04,
    SCC_AUD_OUT_PORT_2_CH_BY_PASS   = 0x08,
    SCC_AUD_OUT_PORT_SPEAKER        = 0x10,
    SCC_AUD_OUT_PORT_HEADPHONE      = 0x20,
    SCC_AUD_OUT_PORT_TVSCART        = 0x40
}   SCC_AUD_OUT_PORT_T;

/* Command SCC_CMD_CODE_AUDIO_VOLUME *************************************/
/*------------------------------------------------------------------*/
/*! @struct  SCC_AUD_VOLUME_INFO_T
 *  @brief  Contains audio volume info.
 *  @code
 *  typedef struct _SCC_AUD_VOLUME_INFO_T
 *  {
 *      SCC_AUD_OUT_PORT_T  e_out_port;
 *      SCC_AUD_CHANNEL_T   e_ch;
 *      UINT8               ui1_volumn;
 *  } SCC_AUD_VOLUME_INFO_T;
 *  @endcode
 *  @li@c  e_out_port                                        -Specify out port.
 *  @li@c  e_ch                                              -Specify chennel.
 *  @li@c  ui1_volumn                                        -volume value.
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AUD_VOLUME_INFO_T
{
    SCC_AUD_OUT_PORT_T  e_out_port;
    SCC_AUD_CHANNEL_T   e_ch;
    UINT8               ui1_volumn;
} SCC_AUD_VOLUME_INFO_T;

/* Command SCC_CMD_CODE_AUDIO_CMPSS_MODE *********************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_AUD_CMPSS_MDOE_T
 *  @brief  Contains audio compression mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_CMPSS_MDOE_LINE = 0,
 *      SCC_AUD_CMPSS_MDOE_CUSTOM_1,
 *      SCC_AUD_CMPSS_MDOE_CUSTOM_2,
 *      SCC_AUD_CMPSS_MDOE_RF
 *  }   SCC_AUD_CMPSS_MDOE_T;
 *  @endcode
 *  @li@c  SCC_AUD_CMPSS_MDOE_LINE                           -line
 *  @li@c  SCC_AUD_CMPSS_MDOE_CUSTOM_1                       -customer 1
 *  @li@c  SCC_AUD_CMPSS_MDOE_CUSTOM_2                       -customer 2
 *  @li@c  SCC_AUD_CMPSS_MDOE_RF                             -RF
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_CMPSS_MDOE_LINE = 0,
    SCC_AUD_CMPSS_MDOE_CUSTOM_1,
    SCC_AUD_CMPSS_MDOE_CUSTOM_2,
    SCC_AUD_CMPSS_MDOE_RF
}   SCC_AUD_CMPSS_MDOE_T;

/* Command SCC_CMD_CODE_AUDIO_CHNL_DELAY *********************************/
/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_CHNL_DELAY_TYPE_T
 *  @brief Specify audio channel delay type.
 *  @code 
 *  typedef enum
 *  {
 *      SCC_AUD_CHNL_DELAY_TYPE_ALL = 0,
 *      SCC_AUD_CHNL_DELAY_TYPE_INDIVIDUAL_CH,
 *      SCC_AUD_CHNL_DELAY_TYPE_ON,
 *      SCC_AUD_CHNL_DELAY_TYPE_OFF
 *  } SCC_AUD_CHNL_DELAY_TYPE_T;
 *  @endcode
 *  @li@c  SCC_AUD_CHNL_DELAY_TYPE_ALL                       -all
 *  @li@c  SCC_AUD_CHNL_DELAY_TYPE_INDIVIDUAL_CH             -individual channel
 *  @li@c  SCC_AUD_CHNL_DELAY_TYPE_ON                        -on
 *  @li@c  SCC_AUD_CHNL_DELAY_TYPE_OFF                       -off
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_CHNL_DELAY_TYPE_ALL = 0,
    SCC_AUD_CHNL_DELAY_TYPE_INDIVIDUAL_CH,
    SCC_AUD_CHNL_DELAY_TYPE_ON,
    SCC_AUD_CHNL_DELAY_TYPE_OFF
} SCC_AUD_CHNL_DELAY_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct  SCC_AUD_CHNL_DELAY_IND_T
 *  @brief  Contains audio channel delay  index .
 *  @code
 *  typedef struct _SCC_AUD_CHNL_DELAY_IND_T
 *  {
 *      UINT16                   ui2_fl_delay; 
 *      UINT16                   ui2_fr_delay; 
 *      UINT16                   ui2_c_delay;  
 *      UINT16                   ui2_rl_delay; 
 *      UINT16                   ui2_rr_delay; 
 *      UINT16                   ui2_sw_delay; 
 *  } SCC_AUD_CHNL_DELAY_IND_T;
 *  @endcode
 *  @li@c  ui2_fl_delay                                      -fleft delay
 *  @li@c  ui2_fr_delay                                      -fright delay
 *  @li@c  ui2_c_delay                                       -c delay
 *  @li@c  ui2_rl_delay                                      -rleft delay
 *  @li@c  ui2_rr_delay                                      -rright delay
 *  @li@c  ui2_sw_delay                                      -sw delay
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AUD_CHNL_DELAY_IND_T
{
    UINT16                   ui2_fl_delay; /* front left */
    UINT16                   ui2_fr_delay; /* front right */
    UINT16                   ui2_c_delay;  /* center */
    UINT16                   ui2_rl_delay; /* rear left */
    UINT16                   ui2_rr_delay; /* rear right */
    UINT16                   ui2_sw_delay; /* sub-woofer */
} SCC_AUD_CHNL_DELAY_IND_T;

/*------------------------------------------------------------------*/
/*! @struct SCC_AUD_CHNL_DELAY_T
 *  @brief  Contains audio channel delay info.
 *  @code
 *  typedef struct _SCC_AUD_CHNL_DELAY_T
 *  {
 *      SCC_AUD_CHNL_DELAY_TYPE_T   e_ch_delay_type;
 *      SCC_AUD_OUT_PORT_T          e_out_port;
 *      union
 *      {
 *          UINT16                       ui2_delay;
 *          SCC_AUD_CHNL_DELAY_IND_T     t_ind_delay;
 *      } u;
 *  } SCC_AUD_CHNL_DELAY_T;
 *  @endcode
 *  @li@c  e_ch_delay_type                                   -channel delay type
 *  @li@c  e_out_port                                        -out port
 *  @li@c  ui2_delay                                         -delay
 *  @li@c  t_ind_delay                                       -individual delay
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AUD_CHNL_DELAY_T
{
    SCC_AUD_CHNL_DELAY_TYPE_T   e_ch_delay_type;
    SCC_AUD_OUT_PORT_T          e_out_port;

    union
    {
        UINT16                       ui2_delay;
        SCC_AUD_CHNL_DELAY_IND_T     t_ind_delay;
    } u;
} SCC_AUD_CHNL_DELAY_T;

/* Command SCC_CMD_CODE_AUDIO_PL2 ****************************************/

/*------------------------------------------------------------------*/
/*! @enum SCC_AUD_PL2_SWITCH_T
 *  @brief Specify audio PL2 switch mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_PL2_SWITCH_OFF = 0,
 *      SCC_AUD_PL2_SWITCH_ON,
 *      SCC_AUD_PL2_SWITCH_AUTO
 *  }   SCC_AUD_PL2_SWITCH_T;
 *  @endcode
 *  @li@c  SCC_AUD_PL2_SWITCH_OFF                            -off
 *  @li@c  SCC_AUD_PL2_SWITCH_ON                             -on
 *  @li@c  SCC_AUD_PL2_SWITCH_AUTO                           -auto
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_PL2_SWITCH_OFF = 0,
    SCC_AUD_PL2_SWITCH_ON,
    SCC_AUD_PL2_SWITCH_AUTO
}   SCC_AUD_PL2_SWITCH_T;

/*------------------------------------------------------------------*/
/*! @enum SCC_AUD_PL2_MODE_T
 *  @brief Specify audio PL2 mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_PL2_MODE_PROLGIC_EMULATION = 0,
 *      SCC_AUD_PL2_MODE_VIRTUAL,
 *      SCC_AUD_PL2_MODE_MUSIC,
 *      SCC_AUD_PL2_MODE_MOVIE,
 *      SCC_AUD_PL2_MODE_MATRIX,
 *      SCC_AUD_PL2_MODE_CUSTOM
 *  }   SCC_AUD_PL2_MODE_T;
 *  @endcode
 *  @li@c  SCC_AUD_PL2_MODE_PROLGIC_EMULATION                -pro Logic emulation
 *  @li@c  SCC_AUD_PL2_MODE_VIRTUAL                          -virtual
 *  @li@c  SCC_AUD_PL2_MODE_MUSIC                            -music
 *  @li@c  SCC_AUD_PL2_MODE_MOVIE                            -movie
 *  @li@c  SCC_AUD_PL2_MODE_MATRIX                           -matrix
 *  @li@c  SCC_AUD_PL2_MODE_CUSTOM                           -customer
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_PL2_MODE_PROLGIC_EMULATION = 0,
    SCC_AUD_PL2_MODE_VIRTUAL,
    SCC_AUD_PL2_MODE_MUSIC,
    SCC_AUD_PL2_MODE_MOVIE,
    SCC_AUD_PL2_MODE_MATRIX,
    SCC_AUD_PL2_MODE_CUSTOM
}   SCC_AUD_PL2_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_PL2_PANORAMA_T
 *  @brief Specify audio PL2 panorama mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_PL2_PANORAMA_OFF = 0,
 *      SCC_AUD_PL2_PANORAMA_ON
 *  }   SCC_AUD_PL2_PANORAMA_T;
 *  @endcode
 *  @li@c  SCC_AUD_PL2_PANORAMA_OFF                          -off
 *  @li@c  SCC_AUD_PL2_PANORAMA_ON                           -on
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_PL2_PANORAMA_OFF = 0,
    SCC_AUD_PL2_PANORAMA_ON
}   SCC_AUD_PL2_PANORAMA_T;


/*------------------------------------------------------------------*/
/*! @enum   SCC_AUD_PL2_DIMENSION_T
 *  @brief  Specify audio PL2 dimension level.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_PL2_DIMENSION_LEVEL0_BACK = 0,
 *      SCC_AUD_PL2_DIMENSION_LEVEL1,
 *      SCC_AUD_PL2_DIMENSION_LEVEL2,
 *      SCC_AUD_PL2_DIMENSION_LEVEL3,
 *      SCC_AUD_PL2_DIMENSION_LEVEL4,
 *      SCC_AUD_PL2_DIMENSION_LEVEL5,
 *      SCC_AUD_PL2_DIMENSION_LEVEL6_FRONT
 *  }   SCC_AUD_PL2_DIMENSION_T;
 *  @endcode
 *  @li@c  SCC_AUD_PL2_DIMENSION_LEVEL0_BACK                 -level0 back
 *  @li@c  SCC_AUD_PL2_DIMENSION_LEVEL1                      -level1
 *  @li@c  SCC_AUD_PL2_DIMENSION_LEVEL2                      -level2
 *  @li@c  SCC_AUD_PL2_DIMENSION_LEVEL3                      -level3
 *  @li@c  SCC_AUD_PL2_DIMENSION_LEVEL4                      -level4
 *  @li@c  SCC_AUD_PL2_DIMENSION_LEVEL5                      -level5
 *  @li@c  SCC_AUD_PL2_DIMENSION_LEVEL6_FRONT                -level6 front
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_PL2_DIMENSION_LEVEL0_BACK = 0,
    SCC_AUD_PL2_DIMENSION_LEVEL1,
    SCC_AUD_PL2_DIMENSION_LEVEL2,
    SCC_AUD_PL2_DIMENSION_LEVEL3,
    SCC_AUD_PL2_DIMENSION_LEVEL4,
    SCC_AUD_PL2_DIMENSION_LEVEL5,
    SCC_AUD_PL2_DIMENSION_LEVEL6_FRONT
}   SCC_AUD_PL2_DIMENSION_T;

/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_PL2_C_WIDTH_T
 *  @brief  Specify audio PL2-C width.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_PL2_C_WIDTH_LEVEL0_MIN_SPREAD = 0,
 *      SCC_AUD_PL2_C_WIDTH_LEVEL1,
 *      SCC_AUD_PL2_C_WIDTH_LEVEL2,
 *      SCC_AUD_PL2_C_WIDTH_LEVEL3,
 *      SCC_AUD_PL2_C_WIDTH_LEVEL4,
 *      SCC_AUD_PL2_C_WIDTH_LEVEL5,
 *      SCC_AUD_PL2_C_WIDTH_LEVEL6,
 *      SCC_AUD_PL2_C_WIDTH_LEVEL7_MAX_SPREAD
 *  }   SCC_AUD_PL2_C_WIDTH_T;
 *  @endcode
 *  @li@c  SCC_AUD_PL2_C_WIDTH_LEVEL0_MIN_SPREAD             -level0 minimum spread
 *  @li@c  SCC_AUD_PL2_C_WIDTH_LEVEL1                        -level1
 *  @li@c  SCC_AUD_PL2_C_WIDTH_LEVEL2                        -level2
 *  @li@c  SCC_AUD_PL2_C_WIDTH_LEVEL3                        -level3
 *  @li@c  SCC_AUD_PL2_C_WIDTH_LEVEL4                        -level4
 *  @li@c  SCC_AUD_PL2_C_WIDTH_LEVEL5                        -level5
 *  @li@c  SCC_AUD_PL2_C_WIDTH_LEVEL6                        -level6
 *  @li@c  SCC_AUD_PL2_C_WIDTH_LEVEL7_MAX_SPREAD             -level7 maximum spread
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_PL2_C_WIDTH_LEVEL0_MIN_SPREAD = 0,
    SCC_AUD_PL2_C_WIDTH_LEVEL1,
    SCC_AUD_PL2_C_WIDTH_LEVEL2,
    SCC_AUD_PL2_C_WIDTH_LEVEL3,
    SCC_AUD_PL2_C_WIDTH_LEVEL4,
    SCC_AUD_PL2_C_WIDTH_LEVEL5,
    SCC_AUD_PL2_C_WIDTH_LEVEL6,
    SCC_AUD_PL2_C_WIDTH_LEVEL7_MAX_SPREAD
}   SCC_AUD_PL2_C_WIDTH_T;

/* Command SCC_CMD_CODE_AUDIO_MTS ****************************************/
/*------------------------------------------------------------------*/
/*! @enum SCC_AUD_MTS_T
 *  @brief  Specify audio MTS mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_MTS_UNKNOWN = 0,
 *      SCC_AUD_MTS_MONO,
 *      SCC_AUD_MTS_STEREO,
 *      SCC_AUD_MTS_SUB_LANG,
 *      SCC_AUD_MTS_DUAL1,
 *      SCC_AUD_MTS_DUAL2,
 *      SCC_AUD_MTS_NICAM_MONO,
 *      SCC_AUD_MTS_NICAM_STEREO,
 *      SCC_AUD_MTS_NICAM_DUAL1,
 *      SCC_AUD_MTS_NICAM_DUAL2,
 *      SCC_AUD_MTS_FM_MONO,
 *      SCC_AUD_MTS_FM_STEREO
 *  } SCC_AUD_MTS_T;
 *  @endcode
 *  @li@c  SCC_AUD_MTS_UNKNOWN                               -unknown
 *  @li@c  SCC_AUD_MTS_MONO                                  -MONO
 *  @li@c  SCC_AUD_MTS_STEREO                                -Stereo
 *  @li@c  SCC_AUD_MTS_SUB_LANG                              -sub lang
 *  @li@c  SCC_AUD_MTS_DUAL1                                 -default1
 *  @li@c  SCC_AUD_MTS_DUAL2                                 -default2
 *  @li@c  SCC_AUD_MTS_NICAM_MONO                            -NICAM MONO
 *  @li@c  SCC_AUD_MTS_NICAM_STEREO                          -NICAM Stereo
 *  @li@c  SCC_AUD_MTS_NICAM_DUAL1                           -NICAM default1
 *  @li@c  SCC_AUD_MTS_NICAM_DUAL2                           -NICAM default2
 *  @li@c  SCC_AUD_MTS_FM_MONO                               -FM MONO
 *  @li@c  SCC_AUD_MTS_FM_STEREO                             -FM Stereo
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_MTS_UNKNOWN = 0,
    SCC_AUD_MTS_MONO,
    SCC_AUD_MTS_STEREO,
    SCC_AUD_MTS_SUB_LANG,
    SCC_AUD_MTS_DUAL1,
    SCC_AUD_MTS_DUAL2,
    SCC_AUD_MTS_NICAM_MONO,
    SCC_AUD_MTS_NICAM_STEREO,
    SCC_AUD_MTS_NICAM_DUAL1,
    SCC_AUD_MTS_NICAM_DUAL2,
    SCC_AUD_MTS_FM_MONO,
    SCC_AUD_MTS_FM_STEREO
} SCC_AUD_MTS_T;

/*Command SCC_CMD_CODE_AUDIO_SE_SURROUND_EX*/
/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_SE_SURROUND_MODE_T
 *  @brief  Specify audio sound effect surround mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_SE_SURROUND_MODE_OFF = 0,
 *      SCC_AUD_SE_SURROUND_MODE_MVS_DEFAULT,
 *      SCC_AUD_SE_SURROUND_MODE_MONO_TO_STEREO,
 *      SCC_AUD_SE_SURROUND_MODE_USER_1,
 *      SCC_AUD_SE_SURROUND_MODE_USER_2,
 *      SCC_AUD_SE_SURROUND_MODE_USER_3,
 *      SCC_AUD_SE_SURROUND_MODE_USER_4,
 *      SCC_AUD_SE_SURROUND_MODE_USER_5,
 *      SCC_AUD_SE_SURROUND_MODE_USER_6,
 *  }     SCC_AUD_SE_SURROUND_MODE_T;
 *  @endcode
 *  @li@c  SCC_AUD_SE_SURROUND_MODE_OFF                      -off
 *  @li@c  SCC_AUD_SE_SURROUND_MODE_MVS_DEFAULT              -MVS default
 *  @li@c  SCC_AUD_SE_SURROUND_MODE_MONO_TO_STEREO           -MONO to Stereo
 *  @li@c  SCC_AUD_SE_SURROUND_MODE_USER_1                   -user1
 *  @li@c  SCC_AUD_SE_SURROUND_MODE_USER_2                   -user2
 *  @li@c  SCC_AUD_SE_SURROUND_MODE_USER_3                   -user3
 *  @li@c  SCC_AUD_SE_SURROUND_MODE_USER_4                   -user4
 *  @li@c  SCC_AUD_SE_SURROUND_MODE_USER_5                   -user5
 *  @li@c  SCC_AUD_SE_SURROUND_MODE_USER_6                   -user6
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_SE_SURROUND_MODE_OFF = 0,
    SCC_AUD_SE_SURROUND_MODE_MVS_DEFAULT,
    SCC_AUD_SE_SURROUND_MODE_MONO_TO_STEREO,
    SCC_AUD_SE_SURROUND_MODE_USER_1,
    SCC_AUD_SE_SURROUND_MODE_USER_2,
    SCC_AUD_SE_SURROUND_MODE_USER_3,
    SCC_AUD_SE_SURROUND_MODE_USER_4,
    SCC_AUD_SE_SURROUND_MODE_USER_5,
    SCC_AUD_SE_SURROUND_MODE_USER_6
}     SCC_AUD_SE_SURROUND_MODE_T;
/*------------------------------------------------------------------*/
/*! @enum SCC_AUD_SE_SURROUND_SET_TYPE_T
 *  @brief  Specify audio sound effect surround set type.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_SE_SURROUND_SET_TYPE_UNKOWN = 0,
 *      SCC_AUD_SE_SURROUND_SET_TYPE_USER,
 *      SCC_AUD_SE_SURROUND_SET_TYPE_VALUE
 *  }     SCC_AUD_SE_SURROUND_SET_TYPE_T;
 *  @endcode
 *  @li@c  SCC_AUD_SE_SURROUND_SET_TYPE_UNKOWN            -unknown
 *  @li@c  SCC_AUD_SE_SURROUND_SET_TYPE_USER              -user
 *  @li@c  SCC_AUD_SE_SURROUND_SET_TYPE_VALUE             -value
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_SE_SURROUND_SET_TYPE_UNKOWN = 0,
    SCC_AUD_SE_SURROUND_SET_TYPE_USER,
    SCC_AUD_SE_SURROUND_SET_TYPE_VALUE
}     SCC_AUD_SE_SURROUND_SET_TYPE_T;
/*------------------------------------------------------------------*/
/*! @struct  SCC_AUD_SE_SURROUND_VALUE_T
 *  @brief  Contains audio sound effect surround value info.
 *  @code
 *  typedef struct _SCC_AUD_SE_SURROUND_VALUE_T
 *  {
 *        UINT8           ui1Clarity;
 *        UINT8           ui1Width;
 *        UINT8           ui1LRGain;
 *        UINT8           ui1Xtalk;
 *        UINT8           ui1OutputGain;
 *        UINT8           ui1BassGain;
 *       UINT8           ui1Fo1;
 *       UINT8           ui1Fo2;
 *       UINT8           ui1Fo3;
 *       UINT8           ui1Mode;
 *  }   SCC_AUD_SE_SURROUND_VALUE_T;
 *  @endcode
 *  @li@c  ui1Clarity                                   -clarity
 *  @li@c  ui1Width                                     -width
 *  @li@c  ui1LRGain                                    -Left/Right gain
 *  @li@c  ui1Xtalk                                     -X talk
 *  @li@c  ui1OutputGain                                -output gain
 *  @li@c  ui1BassGain                                  -bass gain
 *  @li@c  ui1Fo1                                       -Fo1
 *  @li@c  ui1Fo2                                       -Fo2
 *  @li@c  ui1Fo3                                       -Fo3
 *  @li@c  ui1Mode                                      -mode
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AUD_SE_SURROUND_VALUE_T
{
   UINT8           ui1Clarity;
   UINT8           ui1Width;
   UINT8           ui1LRGain;
   UINT8           ui1Xtalk;
   UINT8           ui1OutputGain;
   UINT8           ui1BassGain;
   UINT8           ui1Fo1;
   UINT8           ui1Fo2;
   UINT8           ui1Fo3;
   UINT8           ui1Mode;
}  SCC_AUD_SE_SURROUND_VALUE_T;
/*------------------------------------------------------------------*/
/*! @struct  SCC_AUD_SE_SURROUND_INFO_T
 *  @brief  Contains audio sound effect surround info.
 *  @code
 *  typedef struct _SCC_AUD_SE_SURROUND_INFO_T
 *  {
 *        SCC_AUD_SE_SURROUND_SET_TYPE_T  e_surround_set_type;
 *        union
 *           {
 *                 SCC_AUD_SE_SURROUND_MODE_T      e_se_surround_mode; 
 *                SCC_AUD_SE_SURROUND_VALUE_T     t_se_srround_value;
 *           }u;
 *  }   SCC_AUD_SE_SURROUND_INFO_T;
 *  @endcode
 *  @li@c  SCC_AUD_SE_SURROUND_SET_TYPE_T                   -type
 *  @li@c  SCC_AUD_SE_SURROUND_MODE_T                       -mode
 *  @li@c  SCC_AUD_SE_SURROUND_VALUE_T                      -value
 */
/*------------------------------------------------------------------*/

typedef struct _SCC_AUD_SE_SURROUND_INFO_T
{
    SCC_AUD_SE_SURROUND_SET_TYPE_T  e_surround_set_type;
    union
        {
             SCC_AUD_SE_SURROUND_MODE_T      e_se_surround_mode; 
             SCC_AUD_SE_SURROUND_VALUE_T     t_se_srround_value;
        }u;
   
}   SCC_AUD_SE_SURROUND_INFO_T;

/*Command SCC_CMD_CODE_AUDIO_SE_SPH_MODE*/
/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_DEC_SE_SPH_MODE_T
 *  @brief  Specify audio decode sound effect speaker height mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_DEC_SE_SPH_MODE_OFF = 0,
 *      SCC_AUD_DEC_SE_SPH_MODE_DEFAULT,
 *      SCC_AUD_DEC_SE_SPH_MODE_MODE1,
 *      SCC_AUD_DEC_SE_SPH_MODE_MODE2
 *   }   SCC_AUD_DEC_SE_SPH_MODE_T;
 *  @endcode
 *  @li@c  SCC_AUD_DEC_SE_SPH_MODE_OFF                       -off
 *  @li@c  SCC_AUD_DEC_SE_SPH_MODE_DEFAULT                   -default
 *  @li@c  SCC_AUD_DEC_SE_SPH_MODE_MODE1                     -mode1
 *  @li@c  SCC_AUD_DEC_SE_SPH_MODE_MODE2                     -mode2
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_DEC_SE_SPH_MODE_OFF = 0,
    SCC_AUD_DEC_SE_SPH_MODE_DEFAULT,
    SCC_AUD_DEC_SE_SPH_MODE_MODE1,
    SCC_AUD_DEC_SE_SPH_MODE_MODE2
 }   SCC_AUD_DEC_SE_SPH_MODE_T;
/*Command SCC_CMD_CODE_AUDIO_FM_RADIO_MODE*/
/*------------------------------------------------------------------*/
/*! @enum SCC_AUD_DEC_FM_RADIO_MODE_T
 *  @brief Specify audio decode FM radio mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_DEC_FM_RADIO_MODE = 0,
 *      SCC_AUD_DEC_FM_RADIO_MODE_MONO,
 *      SCC_AUD_DEC_FM_RADIO_MODE_STEREO
 *   }   SCC_AUD_DEC_FM_RADIO_MODE_T;
 *  @endcode
 *  @li@c  SCC_AUD_DEC_FM_RADIO_MODE                         -radio mode
 *  @li@c  SCC_AUD_DEC_FM_RADIO_MODE_MONO                    -radio mode MONO
 *  @li@c  SCC_AUD_DEC_FM_RADIO_MODE_STEREO                  -radio mode Stereo
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_DEC_FM_RADIO_MODE = 0,
    SCC_AUD_DEC_FM_RADIO_MODE_MONO,
    SCC_AUD_DEC_FM_RADIO_MODE_STEREO
 }   SCC_AUD_DEC_FM_RADIO_MODE_T;

/*Command SCC_CMD_CODE_AUDIO_SET_PEQ_INFO*/
/*------------------------------------------------------------------*/
/*! @struct  SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T
 *  @brief Contains audio decode set parameter EQ info.
 *  @code
 *  typedef struct _SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T
 *  {
 *      UINT8           ui1_PEQSet;
 *      UINT32         ui4_Frequency;
 *      UINT16         ui2_BW;
 *      INT16           i2_Gain;
 *  }   SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T;
 *  @endcode
 *  @li@c  ui1_PEQSet                                        -parameter EQ
 *  @li@c  ui4_Frequency                                     -frequency
 *  @li@c  ui2_BW                                            -BW
 *  @li@c  i2_Gain                                           -gain
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T
{
    UINT8           ui1_PEQSet;
    UINT32         ui4_Frequency;
    UINT16         ui2_BW;
    INT16           i2_Gain;
}   SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T;


/* Commmand SCC_CMD_CODE_AUDIO_SE_EQ_USER_LVL ****************************/
/*------------------------------------------------------------------*/
/*! @struct  SCC_AUD_SE_QA_USER_LVL_T
 *  @brief  Specify audio sound effect EQ info.
 *  @code
 *  typedef struct _SCC_AUD_SE_QA_USER_LVL_T
 *  {
 *      SCC_AUD_SE_EQ_T  e_eq_type;
 *      UINT8            ui1_band_idx;
 *      INT8             i1_level;
 *      INT16            i2_level_ex;
 *      BOOL             b_is_level_ex;
 *  } SCC_AUD_SE_QA_USER_LVL_T;
 *  @endcode
 *  @li@c  e_eq_type                                         -EQ type
 *  @li@c  ui1_band_idx                                      -band index
 *  @li@c  i1_level                                          -EQ level
 *  @li@c  i2_level_ex                                       -EQ level extern
 *  @li@c  b_is_level_ex                                     -EQ level externed or not
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AUD_SE_QA_USER_LVL_T
{
    SCC_AUD_SE_EQ_T  e_eq_type;
    UINT8            ui1_band_idx;
    INT8             i1_level;
    INT16            i2_level_ex;
    BOOL             b_is_level_ex;
} SCC_AUD_SE_QA_USER_LVL_T;

/* Commmand SCC_CMD_CODE_AUDIO_HDPHONE_NFY *******************************/
/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_HDPHONE_COND_T
 *  @brief  Specify audio headphone condition.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_HDPHONE_COND_UNPLUGED = 0,
 *      SCC_AUD_HDPHONE_COND_PLUGED
 *  } SCC_AUD_HDPHONE_COND_T;
 *  @endcode
 *  @li@c  SCC_AUD_HDPHONE_COND_UNPLUGED                     -head phone unplugged
 *  @li@c  SCC_AUD_HDPHONE_COND_PLUGED                       -head phone plugged
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_HDPHONE_COND_UNPLUGED = 0,
    SCC_AUD_HDPHONE_COND_PLUGED
} SCC_AUD_HDPHONE_COND_T;

/*------------------------------------------------------------------*/
/*! @brief  audio headphone detect notify function
 *  @param [in]  pv_nfy_tag client's tag value.
 *  @param [in]  e_nfy_cond  Condition for the notification.
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_scc_aud_hdphone_detect_nfy_fct) (
    VOID*                     pv_nfy_tag,
    SCC_AUD_HDPHONE_COND_T    e_nfy_cond,
    UINT32                    ui4_data_1,
    UINT32                    ui4_data_2 );

/*------------------------------------------------------------------*/
/*! @brief  audio upload data notify function
 *  @param [in]  pv_nfy_tag client's tag value.
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_scc_aud_upload_data_nfy_fct) (
    VOID*                     pv_nfy_tag,
    UINT32                    ui4_data_1,
    UINT32                    ui4_data_2 );

/*------------------------------------------------------------------*/
/*! @struct SCC_AUD_HDPHONE_DETECT_NFT_INFO_T
 *  @brief  Contains audio headphone detect notify info.
 *  @code
 *  typedef struct _SCC_AUD_HDPHONE_DETECT_NFT_INFO_T
 *  {
 *      x_scc_aud_hdphone_detect_nfy_fct    pf_hdphone_detect_nfy;
 *      VOID*                               pv_nfy_tag;
 *  } SCC_AUD_HDPHONE_DETECT_NFT_INFO_T;
 *  @endcode
 *  @li@c  pf_hdphone_detect_nfy                             -head phone detect notify function
 *  @li@c  pv_nfy_tag                                        -notify function tag
 */
/*------------------------------------------------------------------*/
typedef struct SCC_AUD_UPLOAD_DATA_NFT_INFO_T
{
    x_scc_aud_upload_data_nfy_fct    pf_upload_data_nfy;
    VOID*                               pv_nfy_tag;
} SCC_AUD_UPLOAD_DATA_NFT_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct SCC_AUD_HDPHONE_DETECT_NFT_INFO_T
 *  @brief  Contains audio headphone detect notify info.
 *  @code
 *  typedef struct _SCC_AUD_HDPHONE_DETECT_NFT_INFO_T
 *  {
 *      x_scc_aud_hdphone_detect_nfy_fct    pf_hdphone_detect_nfy;
 *      VOID*                               pv_nfy_tag;
 *  } SCC_AUD_HDPHONE_DETECT_NFT_INFO_T;
 *  @endcode
 *  @li@c  pf_hdphone_detect_nfy                             -head phone detect notify function
 *  @li@c  pv_nfy_tag                                        -notify function tag
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AUD_HDPHONE_DETECT_NFT_INFO_T
{
    x_scc_aud_hdphone_detect_nfy_fct    pf_hdphone_detect_nfy;
    VOID*                               pv_nfy_tag;
} SCC_AUD_HDPHONE_DETECT_NFT_INFO_T;

/* Commmand SCC_CMD_CODE_AUDIO_MUTE_EX ***********************************/
/*------------------------------------------------------------------*/
/*! @struct SCC_AUD_MUTE_EX_INFO_T
 *  @brief  Contains audio mute-ex out port mask info.
 *  @code
 *  typedef struct _SCC_AUD_MUTE_EX_INFO_T
 *  {
 *      UINT32            ui4_muted_out_port_msk;
 *  } SCC_AUD_MUTE_EX_INFO_T;
 *  @endcode
 *  @li@c  ui4_muted_out_port_msk                            -outport mask
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AUD_MUTE_EX_INFO_T
{
    UINT32            ui4_muted_out_port_msk;
} SCC_AUD_MUTE_EX_INFO_T;


/*************************************************************************/
/*------------------------------------------------------------------*/
/*! @struct  SCC_AU_SE_QA_USER_BAND_INFO_T
 *  @brief  Contains audio sound effect user band info.
 *  @code
 *  typedef struct _SCC_AU_SE_QA_USER_BAND_INFO_T
 *  {
 *      SCC_AUD_SE_EQ_T  e_eq_type;
 *      UINT8            ui1_band_idx;
 *      INT8             i1_max_lvl;
 *      INT8             i1_min_lvl;
 *      UINT32           ui4_freq_lower_bound;
 *      UINT32           ui4_freq_upper_bound;
 *  } SCC_AU_SE_QA_USER_BAND_INFO_T;
 *  @endcode
 *  @li@c  e_eq_type                                         -EQ type
 *  @li@c  ui1_band_idx                                      -band index
 *  @li@c  i1_max_lvl                                        -maximum level
 *  @li@c  i1_min_lvl                                        -minimum level
 *  @li@c  ui4_freq_lower_bound                              -frequency lower bound
 *  @li@c  ui4_freq_upper_bound                              -frequency upper bound
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AU_SE_QA_USER_BAND_INFO_T
{
    SCC_AUD_SE_EQ_T  e_eq_type;
    UINT8            ui1_band_idx;
    INT8             i1_max_lvl;
    INT8             i1_min_lvl;
    UINT32           ui4_freq_lower_bound;
    UINT32           ui4_freq_upper_bound;
} SCC_AU_SE_QA_USER_BAND_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum SCC_AUD_CTRL_MODE_T
 *  @brief  Specify audio control mode.
 *  @code 
 *  typedef enum
 *  {
 *      SCC_AUD_CTRL_MODE_STOP = 0,
 *      SCC_AUD_CTRL_MODE_PLAY,
 *      SCC_AUD_CTRL_MODE_RESUME,
 *      SCC_AUD_CTRL_MODE_PAUSE
 *  }   SCC_AUD_CTRL_MODE_T;
 *  @endcode
 *  @li@c  SCC_AUD_CTRL_MODE_STOP                            -stop
 *  @li@c  SCC_AUD_CTRL_MODE_PLAY                            -play
 *  @li@c  SCC_AUD_CTRL_MODE_RESUME                          -resume
 *  @li@c  SCC_AUD_CTRL_MODE_PAUSE                           -pause
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_CTRL_MODE_STOP = 0,
    SCC_AUD_CTRL_MODE_PLAY,
    SCC_AUD_CTRL_MODE_RESUME,
    SCC_AUD_CTRL_MODE_PAUSE,
    SCC_AUD_CTRL_MODE_PLAY_SYNC,
    SCC_AUD_CTRL_MODE_STOP_SYNC,
    SCC_AUD_CTRL_MODE_PLAY_MIXSND
}   SCC_AUD_CTRL_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum SCC_AUD_CLIP_TYPE_T
 *  @brief Specify audio clip type.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_CLIP_TYPE_UNKNOWN = 0,
 *      SCC_AUD_CLIP_TYPE_TONE,
 *      SCC_AUD_CLIP_TYPE_CLIP_BUFF
 *  }   SCC_AUD_CLIP_TYPE_T;
 *  @endcode
 *  @li@c  SCC_AUD_CLIP_TYPE_UNKNOWN                         -unknown
 *  @li@c  SCC_AUD_CLIP_TYPE_TONE                            -Tone
 *  @li@c  SCC_AUD_CLIP_TYPE_CLIP_BUFF                       -Clip buffer
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_CLIP_TYPE_UNKNOWN = 0,
    SCC_AUD_CLIP_TYPE_TONE,
    SCC_AUD_CLIP_TYPE_CLIP_BUFF,
    SCC_AUD_CLIP_TYPE_CLIP_CUSTOM,
    SCC_AUD_CLIP_TYPE_CLIP_MIXSOUND
}   SCC_AUD_CLIP_TYPE_T;

/* audio presentation types, mono / stereo */
/*------------------------------------------------------------------*/
/*! @enum SCC_AUD_CHANNEL_TYPE_T
 *  @brief  Specify audio channel type.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_CHANNEL_TYPE_UNKNOWN = 0,
 *      SCC_AUD_CHANNEL_TYPE_MONO,
 *      SCC_AUD_CHANNEL_TYPE_STEREO
 *  }   SCC_AUD_CHANNEL_TYPE_T;
 *  @endcode
 *  @li@c  SCC_AUD_CHANNEL_TYPE_UNKNOWN                      -unknown
 *  @li@c  SCC_AUD_CHANNEL_TYPE_MONO                         -MONO
 *  @li@c  SCC_AUD_CHANNEL_TYPE_STEREO                       -Stereo
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_CHANNEL_TYPE_UNKNOWN = 0,
    SCC_AUD_CHANNEL_TYPE_MONO,
    SCC_AUD_CHANNEL_TYPE_STEREO
}   SCC_AUD_CHANNEL_TYPE_T;

typedef enum
{
    SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_POWER_ON =0 ,
    SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_POWER_OFF  
}   SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_T;

typedef enum
{
    SCC_AUD_CLIP_CUSTOM_SETTING_MODE_ON = 0,
    SCC_AUD_CLIP_CUSTOM_SETTING_MODE_OFF  
}   SCC_AUD_CLIP_CUSTOM_SETTING_MODE_T;

/*------------------------------------------------------------------*/
/*! @struct SCC_AUD_CLIP_BUFF_T
 *  @brief  Contains audio clip buffer info.
 *  @code
 *  typedef struct _SCC_AUD_CLIP_BUFF_T
 *  {
 *      ASH_AUDIO_FMT_T  e_fmt;
 *      UINT8*           pui1_clip_buff;
 *      SIZE_T           z_clip_buff_size;
 *      SCC_AUD_CHANNEL_TYPE_T  e_PCM_channel_type;
 *      UINT32                  ui4_PCM_SampleRate;
 *      UINT8                   ui1_PCM_BitDepth;
 *  } SCC_AUD_CLIP_BUFF_T;
 *  @endcode
 *  @li@c  e_fmt                                             -format
 *  @li@c  pui1_clip_buff                                    -clip buffer
 *  @li@c  z_clip_buff_size                                  -clip buffer size
 *  @li@c  e_PCM_channel_type                                -PCM channel type
 *  @li@c  ui4_PCM_SampleRate                                -PCM sample rate
 *  @li@c  ui1_PCM_BitDepth                                  -PCM Bit Depth
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AUD_CLIP_BUFF_T
{
    ASH_AUDIO_FMT_T  e_fmt;
    UINT8*           pui1_clip_buff;
    SIZE_T           z_clip_buff_size;
    SCC_AUD_CHANNEL_TYPE_T  e_PCM_channel_type;
    UINT32                  ui4_PCM_SampleRate;
    UINT8                   ui1_PCM_BitDepth;

} SCC_AUD_CLIP_BUFF_T;

#define SCC_AUD_CH_MASK_NONE             ((UINT32) (0) )    /**<        */
#define SCC_AUD_CH_MASK_FRONT_LEFT       MAKE_BIT_MASK_32(0)    /**<        */
#define SCC_AUD_CH_MASK_FRONT_RIGHT      MAKE_BIT_MASK_32(1)    /**<        */
#define SCC_AUD_CH_MASK_REAR_LEFT        MAKE_BIT_MASK_32(2)    /**<        */
#define SCC_AUD_CH_MASK_REAR_RIGHT       MAKE_BIT_MASK_32(3)    /**<        */
#define SCC_AUD_CH_MASK_CENTER           MAKE_BIT_MASK_32(4)    /**<        */
#define SCC_AUD_CH_MASK_SUB_WOOFER       MAKE_BIT_MASK_32(5)    /**<        */

#define SCC_AUD_OUT_PORT_MASK_NONE          ((UINT32) (0) )    /**<        */
#define SCC_AUD_OUT_PORT_MASK_2_CH          MAKE_BIT_MASK_32(0)    /**<        */
#define SCC_AUD_OUT_PORT_MASK_5_1_CH        MAKE_BIT_MASK_32(1)    /**<        */
#define SCC_AUD_OUT_PORT_MASK_SPDIF         MAKE_BIT_MASK_32(2)    /**<        */
#define SCC_AUD_OUT_PORT_MASK_2_CH_BY_PASS  MAKE_BIT_MASK_32(3)    /**<        */
#define SCC_AUD_OUT_PORT_MASK_SPEAKER       MAKE_BIT_MASK_32(4)    /**<        */

#define SCC_AUD_CLIP_REPEAT_CNT_FOREVER    ((UINT32) 0)    /**<        */

typedef enum
{
    SCC_AUD_CLIP_CUSTOM_MODE_CUSTOM1= 0,
    SCC_AUD_CLIP_CUSTOM_MODE_CUSTOM2,
    SCC_AUD_CLIP_CUSTOM_MODE_CUSTOM3,
    SCC_AUD_CLIP_CUSTOM_MODE_CUSTOM4
} SCC_AUD_CLIP_CUSTOM_MODE_T;


/*------------------------------------------------------------------*/
/*! @struct SCC_AUD_CLIP_T
 *  @brief Contains audio clip info.
 *  @code
 *  typedef struct _SCC_AUD_CLIP_T
 *  {
 *      SCC_AUD_CLIP_TYPE_T    e_type;
 *      UINT32                 ui4_out_port_mask;
 *      UINT32                 ui4_ch_mask;
 *      INT32                  ui4_repeat_cnt;
 *      union
 *      {
 *          UINT32                   ui4_tone_freq; 
 *          SCC_AUD_CLIP_BUFF_T      t_clip_buff;
 *      } u;
 *  } SCC_AUD_CLIP_T;
 *  @endcode
 *  @li@c  e_type                                            -type
 *  @li@c  ui4_out_port_mask                                 -out port mask
 *  @li@c  ui4_ch_mask                                       -channel mask
 *  @li@c  ui4_repeat_cnt                                    -repeat count
 *  @li@c  ui4_tone_freq                                     -tone frequency
 *  @li@c  t_clip_buff                                       -clip buffer
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AUD_CLIP_T
{
    SCC_AUD_CLIP_TYPE_T    e_type;
    UINT32                 ui4_out_port_mask;
    UINT32                 ui4_ch_mask;
    INT32                  ui4_repeat_cnt;

    union
    {
        UINT32                   ui4_tone_freq; /* Hz */
        SCC_AUD_CLIP_BUFF_T      t_clip_buff;
        SCC_AUD_CLIP_CUSTOM_MODE_T  e_clip_custom_mode;
    } u;
} SCC_AUD_CLIP_T;


/*------------------------------------------------------------------*/
/*! @enum SCC_AUD_CLIP_COND_T
 *  @brief Specify audio clip condition.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_CLIP_COND_ERROR = -1,
 *      SCC_AUD_CLIP_COND_CTRL_DONE,
 *      SCC_AUD_CLIP_COND_AUD_CLIP_DONE
 *  } SCC_AUD_CLIP_COND_T;
 *  @endcode
 *  @li@c  SCC_AUD_CLIP_COND_ERROR                           -error
 *  @li@c  SCC_AUD_CLIP_COND_CTRL_DONE                       -control done
 *  @li@c  SCC_AUD_CLIP_COND_AUD_CLIP_DONE                   -clip done
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_CLIP_COND_ERROR = -1,
    SCC_AUD_CLIP_COND_CTRL_DONE,
    SCC_AUD_CLIP_COND_AUD_CLIP_DONE

} SCC_AUD_CLIP_COND_T;

#define SCC_AUD_CLIP_COND_STOP_CTRL_DONE             ((UINT32) (1) )    /**<        */
#define SCC_AUD_CLIP_COND_PLAY_CTRL_DONE             ((UINT32) (4) )    /**<        */
#define SCC_AUD_CLIP_COND_PAUSE_CTRL_DONE            ((UINT32) (6) )    /**<        */
#define SCC_AUD_CLIP_COND_RESUME_CTRL_DONE           ((UINT32) (7) )    /**<        */

/* AUDIO SCC notify function */
/*------------------------------------------------------------------*/
/*! @brief audio clip notify function.
 *  @param [in] h_aud  handle to the audio component.
 *  @param [in] e_nfy_cond  Condition for the notification.
 *  @param [in] pv_nfy_tag  client's tag value.
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_scc_aud_clip_nfy_fct) (
                                   HANDLE_T            h_aud,
                                   SCC_AUD_CLIP_COND_T e_nfy_cond,
                                   VOID*               pv_nfy_tag,
                                   UINT32              ui4_data_1,
                                   UINT32              ui4_data_2 );

/* Capability channels. */
#define SCC_AUD_CAP_TYPE_UNKNOWN     (((UINT32) 1) << 0)    /**<        */
#define SCC_AUD_CAP_TYPE_MONO        (((UINT32) 1) << 1)    /**<        */
#define SCC_AUD_CAP_TYPE_DUAL_MONO   (((UINT32) 1) << 2)    /**<        */
#define SCC_AUD_CAP_TYPE_STEREO      (((UINT32) 1) << 3)    /**<        */
#define SCC_AUD_CAP_TYPE_STEREO_DOLBY_SURROUND      (((UINT32) 1) << 4)    /**<        */
#define SCC_AUD_CAP_TYPE_3_0         (((UINT32) 1) << 5)    /**<        */
#define SCC_AUD_CAP_TYPE_4_0         (((UINT32) 1) << 6)    /**<        */
#define SCC_AUD_CAP_TYPE_5_1         (((UINT32) 1) << 7)    /**<        */
#define SCC_AUD_CAP_TYPE_7_1         (((UINT32) 1) << 8)    /**<        */
#define SCC_AUD_CAP_TYPE_OTHERS      (((UINT32) 1) << 9)    /**<        */


/* Capability downmix mode. */
#define SCC_AUD_CAP_DM_OFF           (((UINT32) 1) << 0)    /**<        */
#define SCC_AUD_CAP_DM_LT_RT         (((UINT32) 1) << 1)    /**<        */
#define SCC_AUD_CAP_DM_STEREO        (((UINT32) 1) << 2)    /**<        */
#define SCC_AUD_CAP_DM_VIR_SURR      (((UINT32) 1) << 3)    /**<        */


/* Capability sound effect. */
#define SCC_AUD_CAP_SE_TRIM          (((UINT32) 1) << 0)    /**<        */
#define SCC_AUD_CAP_SE_DELAY         (((UINT32) 1) << 1)    /**<        */
#define SCC_AUD_CAP_SE_SUPERBASS     (((UINT32) 1) << 2)    /**<        */
#define SCC_AUD_CAP_SE_EQUALIZER     (((UINT32) 1) << 3)    /**<        */
#define SCC_AUD_CAP_SE_REVERB        (((UINT32) 1) << 4)    /**<        */
#define SCC_AUD_CAP_SE_BASS          (((UINT32) 1) << 5)    /**<        */
#define SCC_AUD_CAP_SE_TREBLE        (((UINT32) 1) << 6)    /**<        */
#define SCC_AUD_CAP_SE_BALANCE       (((UINT32) 1) << 7)    /**<        */
#define SCC_AUD_CAP_SE_POSTDRC       (((UINT32) 1) << 8)    /**<        */
#define SCC_AUD_CAP_SE_VOLUME        (((UINT32) 1) << 9)    /**<        */
#define SCC_AUD_CAP_SE_SURROUND      (((UINT32) 1) << 10)    /**<        */


/* Capability format. */
#define SCC_AUD_CAP_FMT_MPEG      (((UINT32) 1) << 1)    /**<        */
#define SCC_AUD_CAP_FMT_AC3       (((UINT32) 1) << 2)    /**<        */
#define SCC_AUD_CAP_FMT_PCM       (((UINT32) 1) << 3)    /**<        */
#define SCC_AUD_CAP_FMT_MP3       (((UINT32) 1) << 4)    /**<        */
#define SCC_AUD_CAP_FMT_AAC       (((UINT32) 1) << 5)    /**<        */
#define SCC_AUD_CAP_FMT_DTS       (((UINT32) 1) << 6)    /**<        */
#define SCC_AUD_CAP_FMT_WMA       (((UINT32) 1) << 7)    /**<        */
#define SCC_AUD_CAP_FMT_RA        (((UINT32) 1) << 8)    /**<        */
#define SCC_AUD_CAP_FMT_HDCD      (((UINT32) 1) << 9)    /**<        */
#define SCC_AUD_CAP_FMT_MLP       (((UINT32) 1) << 10)    /**<        */


/*------------------------------------------------------------------*/
/*! @struct  SCC_AUD_CAPABILITY_T
 *  @brief  Contains audio capability info.
 *  @code
 *  typedef struct _SCC_AUD_CAPABILITY_T
 *  {
 *      UINT32              ui4_fmt;
 *      UINT32              ui4_channel_type;
 *      UINT32              ui4_sound_effect;
 *      UINT32              ui4_downmix_mode;
 *      BOOL                b_is_outport_adj;
 *      UINT32              ui4_output_mask;
 *  }   SCC_AUD_CAPABILITY_T;
 *  @endcode
 *  @li@c  ui4_fmt                                           -format
 *  @li@c  ui4_channel_type                                  -channel type
 *  @li@c  ui4_sound_effect                                  -sound effect
 *  @li@c  ui4_downmix_mode                                  -downmix mode
 *  @li@c  b_is_outport_adj                                  -outport adjust or not
 *  @li@c  ui4_output_mask                                   -output mask
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AUD_CAPABILITY_T
{
    UINT32              ui4_fmt;
    UINT32              ui4_channel_type;
    UINT32              ui4_sound_effect;
    UINT32              ui4_downmix_mode;
    BOOL                b_is_outport_adj;
    UINT32              ui4_output_mask;
}   SCC_AUD_CAPABILITY_T;

/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_ENDIAN_MODE_T
 *  @brief  Specify endian mode in DTS files.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_ENDIAN_MODE_LITTLE = 0,
 *      SCC_AUD_ENDIAN_MODE_BIG
 *  } SCC_AUD_ENDIAN_MODE_T;
 *  @endcode
 *  @li@c  SCC_AUD_ENDIAN_MODE_LITTLE                     -little endian
 *  @li@c  SCC_AUD_ENDIAN_MODE_BIG                       -big endian
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_ENDIAN_MODE_LITTLE = 0,
    SCC_AUD_ENDIAN_MODE_BIG
} SCC_AUD_ENDIAN_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_ENDIAN_MODE_T
 *  @brief  Specify audio out device 
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_OUT_DEVICE_BUILD_SPEAKER = 0,
 *      SCC_AUD_OUT_DEVICE_USB_HEADPHONE,
 *      SCC_AUD_OUT_DEVICE_BLUETOOTH_EARPHONE
 *  } SCC_AUD_OUT_DEVICE_T;
 *  @endcode
 *  @li@c  SCC_AUD_OUT_DEVICE_BUILD_SPEAKER                     -build speaker
 *  @li@c  SCC_AUD_OUT_DEVICE_USB_HEADPHONE                    -usb headphone
 *  @li@c  SCC_AUD_OUT_DEVICE_BLUETOOTH_EARPHONE           -bluetooth earphone
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_OUT_DEVICE_BUILD_SPEAKER = 0,
    SCC_AUD_OUT_DEVICE_USB_HEADPHONE,
    SCC_AUD_OUT_DEVICE_BLUETOOTH_EARPHONE
} SCC_AUD_OUT_DEVICE_T;

/*------------------------------------------------------------------*/
/*! @enum SCC_AUD_BTN_SOUND_MODE_T
 *  @brief  Specify audio button sound mode.
 *  @code
 *  typedef enum
 *  {
 *      SCC_AUD_BTNSND_MODE_CUSTOM1,
 *      SCC_AUD_BTNSND_MODE_CUSTOM2,
 *      SCC_AUD_BTNSND_MODE_CUSTOM3,
 *      SCC_AUD_BTNSND_MODE_CUSTOM4,
 *  } SCC_AUD_BTNSND_MODE_T;
 *  @endcode
 *  @li@c  SCC_AUD_BTNSND_MODE_CUSTOM1                                  -CUSTOM1
 *  @li@c  SCC_AUD_BTNSND_MODE_CUSTOM2                                -CUSTOM2
 *  @li@c  SCC_AUD_BTNSND_MODE_CUSTOM3                              -CUSTOM3
 *  @li@c  SCC_AUD_BTNSND_MODE_CUSTOM4                                 -CUSTOM4
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_BTNSND_MODE_CUSTOM1= 0,
    SCC_AUD_BTNSND_MODE_CUSTOM2,
    SCC_AUD_BTNSND_MODE_CUSTOM3,
    SCC_AUD_BTNSND_MODE_CUSTOM4
} SCC_AUD_BTNSND_MODE_T;

typedef enum
{
    SCC_AUD_BTNSND_ENABLE_OFF= 0,
    SCC_AUD_BTNSND_ENABLE_ON
} SCC_AUD_BTNSND_ENABLE_T;

typedef enum
{
    SCC_AUD_DEC_AV_SYNC_FREE_RUN = 0,
    SCC_AUD_DEC_AV_SYNC_AUD_SLAVE,
    SCC_AUD_DEC_AV_SYNC_AUD_MASTER
} SCC_AUD_DEC_AV_SYNC_INFO_T;

/* for bootup status query */
#define SCC_BOOTUP_SOUND    MAKE_BIT_MASK_8 (0)
#define SCC_BOOTUP_LOGO     MAKE_BIT_MASK_8 (1)

/*----------------------------------------------------------------------------*/
/*! @enum  SCC_CRNT_INP_SRC_TYPE_T
 *  @brief SCC_CRNT_INP_SRC_TYPE_T
 *
 *  @code
 *  typedef enum
 *  {
 *     SCC_CRNT_INP_SRC_TV = 0,
 *     SCC_CRNT_INP_SRC_COMP_VIDEO,
 *     SCC_CRNT_INP_SRC_S_VIDEO,
 *     SCC_CRNT_INP_SRC_Y_PB_PR,
 *     SCC_CRNT_INP_SRC_VGA,
 *     SCC_CRNT_INP_SRC_SCART,
 *     SCC_CRNT_INP_SRC_HDMI
 *  }SCC_CRNT_INP_SRC_TYPE_T;
 *  @endcode
 *
 *  @li@c  SCC_CRNT_INP_SRC_TV                -
 *  @li@c  SCC_CRNT_INP_SRC_COMP_VIDEO        -
 *  @li@c  SCC_CRNT_INP_SRC_S_VIDEO           -
 *  @li@c  SCC_CRNT_INP_SRC_Y_PB_PR           -
 *  @li@c  SCC_CRNT_INP_SRC_VGA               -
 *  @li@c  SCC_CRNT_INP_SRC_SCART             -
 *  @li@c  SCC_CRNT_INP_SRC_HDMI              -
 */
/*----------------------------------------------------------------------------*/
typedef enum
{
    SCC_CRNT_INP_SRC_TV = 0,
    SCC_CRNT_INP_SRC_COMP_VIDEO,
    SCC_CRNT_INP_SRC_S_VIDEO,
    SCC_CRNT_INP_SRC_Y_PB_PR,
    SCC_CRNT_INP_SRC_VGA,
    SCC_CRNT_INP_SRC_SCART,
    SCC_CRNT_INP_SRC_HDMI
}SCC_CRNT_INP_SRC_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct  SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T
 *  @brief  Contains audio upload data buffer info.
 *  @code
 *  typedef struct _SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T
 *  {
 *      UINT32              ui4_buffer_start_addr;
 *      UINT32              ui4_buffer_size;
 *  }   SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T;
 *  @endcode
 *  @li@c  ui4_buffer_start_addr                                           -buffer start address
 *  @li@c  ui4_buffer_size                                  -buffer size
 */
/*------------------------------------------------------------------*/
typedef struct _SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T
{
    UINT32              ui4_buffer_start_addr;
    UINT32              ui4_buffer_size;
 }   SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T;

/* upload pcm information */
typedef struct
{
    UINT32 ui4_pcm_bits;
    UINT32 ui4_pcm_channels;
    UINT32 ui4_pcm_sample_rate;
} SCC_AUD_UPLOAD_PCM_INFO_T;

/* audio protect information */
typedef enum
{
    SCC_PROTECT_INFO_UNKNOWN = -1,
    SCC_PROTECT_INFO_DMUTE_AMUTE,         /* Digtal mute and analog mute */
    SCC_PROTECT_INFO_DUNMUTE_AMUTE,       /* Digtal unmute and analog mute */
    SCC_PROTECT_INFO_DUNMUTE_AUNMUTE,     /* Digtal unmute and analog unmute */
    SCC_PROTECT_INFO_DMUTE_AUNMUTE        /* Digtal mute and analog unmute */
}   SCC_PROTECT_INFO_T;

/*----------------------------------------------------------------------------*/
/*! @enum  SCC_AUD_KARAOKE_CTRL_T
 *  @brief SCC_AUD_KARAOKE_CTRL_T
 *
 *  @code
 *  typedef enum
 *  {
 *     SCC_AUD_KARAOKE_CONNECT = 0,
 *     SCC_AUD_KARAOKE_ENABLE,
 *     SCC_AUD_KARAOKE_ECHO,
 *     SCC_AUD_KARAOKE_VOCAL_CANEL,
 *     SCC_AUD_KARAOKE_KEY_SHIFT,
 *     SCC_AUD_KARAOKE_MIC_VOL
 *  }SCC_AUD_KARAOKE_CTRL_T;
 *  @endcode
 *
 *  @li@c  SCC_AUD_KARAOKE_CONNECT    -
 *  @li@c  SCC_AUD_KARAOKE_ENABLE        -
 *  @li@c  SCC_AUD_KARAOKE_ECHO           -
 *  @li@c  SCC_AUD_KARAOKE_VOCAL_CANEL   -
 *  @li@c  SCC_AUD_KARAOKE_KEY_SHIFT         -
 *  @li@c  SCC_AUD_KARAOKE_MIC_VOL           -
 */
/*----------------------------------------------------------------------------*/
typedef enum
{
    SCC_AUD_KARAOKE_CONNECT = 0,
    SCC_AUD_KARAOKE_ENABLE,
    SCC_AUD_KARAOKE_ECHO,
    SCC_AUD_KARAOKE_VOCAL_CANEL,
    SCC_AUD_KARAOKE_KEY_SHIFT,
    SCC_AUD_KARAOKE_MIC_VOL1,
    SCC_AUD_KARAOKE_MIC_VOL2
} SCC_AUD_KARAOKE_CTRL_T;

/*------------------------------------------------------------------*/
/*! @struct SCC_AUD_KARAOKE_INFO_T
 *  @brief Contains set karaoke info.
 *  @code
 *  typedef struct _AUD_KARAOKE_INFO_T
 *  {
 *      SCC_AUD_KARAOKE_CTRL_T    e_karaoke_type;
 *      union
 *      {
 *          BOOL         b_karaoke_enable;
 *          UINT16       ui2_echo_val;
 *          BOOL         b_vocal_cancel;
 *          UINT16       ui2_key_shift;
 *          UINT16       ui2_mic_vol1;
 *          UINT16       ui2_mic_vol2;
 *      } u;
 *  } SCC_AUD_KARAOKE_INFO_T;
 *  @endcode
 *  @li@c  e_karaoke_type           -type
 *  @li@c  b_karaoke_enable         -set karaoke enable or disable
 */
/*------------------------------------------------------------------*/

typedef struct _AUD_KARAOKE_INFO_T
{
    SCC_AUD_KARAOKE_CTRL_T   e_karaoke_type;
    union
    {
         BOOL       b_karaoke_enable;
         UINT16     ui2_echo_val;
         BOOL       b_vocal_cancel;
         UINT16     ui2_key_shift;
         UINT16     ui2_mic_vol1;
         UINT16     ui2_mic_vol2;
    } u;
} SCC_AUD_KARAOKE_INFO_T;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @} */                                             
/* end of groupMW_STRM_MNGR_SCC */
/*----------------------------------------------------------------------------*/

/*! @page pageMW_SCC SCC Detailed Description
 *  @section secMW_SCC_1  1. Introduction
 *  @par
 *      SCC is a stream manager component,
 *      which provides an interface for application to
 *      control driver components directly through the resource manager.
 *      There are three component type for SCC API.
 *      1. Video component:
 *      Directed to video plane driver,
 *      control video related settings like screen mode.
 *      2. Display component:
 *      Directed to plane mixer and TV encoder driver,
 *      control display related settings like contrast, brightness.
 *      3. Audio component:
 *      Directed to audio decoder driver,
 *      control audio related settings like volume, mute.
 *  @par
 *  @section secMW_SCC_2  2. Concept
 *  @par
 *      example code for how to set video and display region:
 *  @par
 *      @include scc_sample_code.c
 */

#endif /* _U_SCC_ */

