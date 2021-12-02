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
/*! @file u_sm_audio_hdlr.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Audio Stream handler specific definitions,
 *         which are controled by Stream Manager.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_STRM_MNGR_AUDIO_HDLR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SM_AUDIO_HDLR_H_
#define _U_SM_AUDIO_HDLR_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_sm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/


/* command sets *************************************************************/
/* primary commands */
#define SM_AS_CMD_CODE_SET_AUDIO_DECODER                    \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 0))    /**<        */
#define SM_AS_CMD_CODE_SET_FORCE_PID                        \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 1))    /**<        */
#define SM_AS_CMD_CODE_SET_FORCE_SYNC_MODE                  \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 2))    /**<        */
#define SM_AS_CMD_CODE_SET_MM_CRYPT_INFO                    \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 3))    /**<        */
#define SM_AS_CMD_CODE_SET_FORCE_CODEC                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 4))    /**<        */
#ifdef SYS_MHP_SUPPORT
#define SM_AS_CMD_CODE_SET_FORCE_NO_CA_REQ                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 5))    /**<        */
#endif
#define SM_AS_CMD_CODE_SET_AD_MODE                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 6))    /**<        */
      
#define SM_AS_CMD_CODE_SET_MS10_MIX_FLAG                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 7))    /**<        */
      
#define SM_AS_CMD_CODE_SET_MS10_MIX_ID                     \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 8))    /**<        */
#define SM_AS_CMD_CODE_SET_FORCE_DOWNMIX_MODE                  \
	  ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 9))	 /**<		 */

/* subsequence commands */
#define SM_AS_CMD_CODE_SET_LOGICAL_DESC  ((SM_CMD_CODE_T) 1)    /**<        */
#define SM_AS_CMD_CODE_SET_HANDLE        ((SM_CMD_CODE_T) 2)    /**<        */
#define SM_AS_CMD_CODE_SET_COMP_ID       ((SM_CMD_CODE_T) 3)    /**<        */
#define SM_AS_CMD_CODE_SET_IN_PORT       ((SM_CMD_CODE_T) 4)    /**<        */

/* set types *****************************************************************/
#define SM_GRP_ASH                 (( SM_GRP_LAST_ENTRY + 5 ) << SM_CODE_BITS)    /**<        */
#define SM_GRP_TUNL_AUDIO_DEC      (( SM_GRP_LAST_ENTRY + 6 ) << SM_CODE_BITS)    /**<        */

#define SM_ASH_SET_TYPE_MUTE       ((SM_SET_TYPE_T)(SM_GRP_ASH | 0))    /**<        */
#define SM_ASH_SET_TYPE_UNMUTE     ((SM_SET_TYPE_T)(SM_GRP_ASH | 1))    /**<        */
#define SM_ASH_SET_TYPE_VOLUME     ((SM_SET_TYPE_T)(SM_GRP_ASH | 2))    /**<        */

#define SM_ASH_SET_TYPE_SPEED      ((SM_SET_TYPE_T)(SM_GRP_ASH | 3))    /**<        */
#define SM_ASH_SET_TYPE_PAUSE           ((SM_SET_TYPE_T)(SM_GRP_ASH | 4))    /**<        */
#define SM_ASH_SET_TYPE_RESUME          ((SM_SET_TYPE_T)(SM_GRP_ASH | 5))    /**<        */
#define SM_ASH_SET_TYPE_CODEC_SPEC_INFO ((SM_SET_TYPE_T)(SM_GRP_ASH | 6))    /**<        */
#define SM_ASH_SET_TYPE_AUDIO_ONLY_INFO ((SM_SET_TYPE_T)(SM_GRP_ASH | 7))    /**<        */
#define SM_ASH_SET_TYPE_MS10_MIX_ID     ((SM_SET_TYPE_T)(SM_GRP_ASH | 8))    /**<        */
#define SM_ASH_SET_TYPE_DOWN_MIX_MODE ((SM_SET_TYPE_T)(SM_GRP_ASH | 9))    /**<        */


/* get types *****************************************************************/
#define SM_ASH_GET_TYPE_VOLUME     ((SM_GET_TYPE_T)(SM_GRP_ASH | 0))    /**<        */
#define SM_ASH_GET_TYPE_MUTE       ((SM_GET_TYPE_T)(SM_GRP_ASH | 1))    /**<        */
#define SM_ASH_GET_TYPE_AUDIO_DEC  ((SM_GET_TYPE_T)(SM_GRP_ASH | 2))    /**<        */
#define SM_ASH_GET_TYPE_AUDIO_INFO ((SM_GET_TYPE_T)(SM_GRP_ASH | 3))    /**<        */
#define SM_ASH_GET_TYPE_AUDIO_PTS ((SM_GET_TYPE_T)(SM_GRP_ASH | 4))    /**<        */
#define SM_ASH_GET_TYPE_DOWN_MIX_MODE ((SM_GET_TYPE_T)(SM_GRP_ASH | 5))    /**<        */
#define SM_ASH_GET_TYPE_AFIFO_AMOUNT ((SM_GET_TYPE_T)(SM_GRP_ASH | 6))    /**<        */


/* SM_ASH_SET_TYPE_SET_VOLUME data info **************************************/
/*------------------------------------------------------------------*/
/*! @enum  ASH_CHANNEL_T
 *  @brief  For operation  audio channel 
 *  @code
 *  typedef enum
 *  {
 *      ASH_CHANNEL_ALL = 0,
 *      ASH_CHANNEL_FRONT_LEFT,
 *      ASH_CHANNEL_FRONT_RIGHT,
 *      ASH_CHANNEL_REAR_LEFT,
 *      ASH_CHANNEL_REAR_RIGHT,
 *      ASH_CHANNEL_CENTER,
 *      ASH_CHANNEL_SUB_WOOFER
 *  } ASH_CHANNEL_T;
 *  @endcode
 *  @li@c  ASH_CHANNEL_ALL                                       -For operation all audio channel 
 *  @li@c  ASH_CHANNEL_FRONT_LEFT                            -For operation front left audio channel 
 *  @li@c  ASH_CHANNEL_FRONT_RIGHT                           -For operation front right audio channel 
 *  @li@c  ASH_CHANNEL_REAR_LEFT                             -For operation rear left audio channel 
 *  @li@c  ASH_CHANNEL_REAR_RIGHT                            -For operation rear right audio channel 
 *  @li@c  ASH_CHANNEL_CENTER                                -For operation center audio channel 
 *  @li@c  ASH_CHANNEL_SUB_WOOFER                            -For operation sub woofer audio channel 
 */
/*------------------------------------------------------------------*/
typedef enum
{
    ASH_CHANNEL_ALL = 0,
    ASH_CHANNEL_FRONT_LEFT,
    ASH_CHANNEL_FRONT_RIGHT,
    ASH_CHANNEL_REAR_LEFT,
    ASH_CHANNEL_REAR_RIGHT,
    ASH_CHANNEL_CENTER,
    ASH_CHANNEL_SUB_WOOFER
} ASH_CHANNEL_T;


/*------------------------------------------------------------------*/
/*! @enum ASH_AV_SYNC_INFO_T
 *  @brief Define audio sync mode.
 *  @code
 *  typedef enum
 *  {
 *      ASH_AV_SYNC_FREE_RUN = 0,
 *      ASH_AV_SYNC_AUD_SLAVE,
 *      ASH_AV_SYNC_AUD_MASTER
 *  } ASH_AV_SYNC_INFO_T;
 *  @endcode
 *  @li@c  ASH_AV_SYNC_FREE_RUN                              -Free run sync mode.
 *  @li@c  ASH_AV_SYNC_AUD_SLAVE                             -audio is slave sync mode
 *  @li@c  ASH_AV_SYNC_AUD_MASTER                            -audio is master sync mode
 */
/*------------------------------------------------------------------*/
typedef enum
{
    ASH_AV_SYNC_FREE_RUN = 0,
    ASH_AV_SYNC_AUD_SLAVE,
    ASH_AV_SYNC_AUD_MASTER
} ASH_AV_SYNC_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum ASH_NO_SIG_AUTO_MUTE_T
 *  @brief Define auto mute 
 *  @code
 *  typedef enum
 *  {
 *      ASH_NO_SIG_AUTO_MUTE_UNKNOWN = 0,
 *      ASH_NO_SIG_AUTO_MUTE_MUTE ,
 *      ASH_NO_SIG_AUTO_MUTE_UNMUTE
 *  } ASH_NO_SIG_AUTO_MUTE_T;
 *  @endcode
 *  @li@c  ASH_NO_SIG_AUTO_MUTE_UNKNOWN                      -Auto mute in unsure status.
 *  @li@c  ASH_NO_SIG_AUTO_MUTE_MUTE                         - Auto mute in mute status
 *  @li@c  ASH_NO_SIG_AUTO_MUTE_UNMUTE                       -Auto mute in unmute status.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    ASH_NO_SIG_AUTO_MUTE_UNKNOWN = 0,
    ASH_NO_SIG_AUTO_MUTE_MUTE ,
    ASH_NO_SIG_AUTO_MUTE_UNMUTE
} ASH_NO_SIG_AUTO_MUTE_T;

/*------------------------------------------------------------------*/
/*! @struct ASH_VOLUME_INFO_T
 *  @brief Contains audio info.
 *  @code
 *  typedef struct
 *  {
 *      ASH_CHANNEL_T   e_channel;
 *      UINT8           ui1_volumn; 
 *  } ASH_VOLUME_INFO_T;
 *  @endcode
 *  @li@c  e_channel                                         -specify channel info.
 *  @li@c  ui1_volumn                                        -specify volume value.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    ASH_CHANNEL_T   e_channel;
    UINT8           ui1_volumn; /* 0-100 */
} ASH_VOLUME_INFO_T;


/* SM_ASH_GET_TYPE_AUDIO_INFO data info **************************************/
/*------------------------------------------------------------------*/
/*! @enum ASH_AUDIO_FMT_T
 *  @brief  Contains audio format.
 *  @code
 *  typedef enum
 *  {
 *      ASH_AUDIO_FMT_UNKNOWN = 0,
 *      ASH_AUDIO_FMT_MPEG,
 *      ASH_AUDIO_FMT_AC3,
 *      ASH_AUDIO_FMT_E_AC3,
 *      ASH_AUDIO_FMT_PCM,
 *      ASH_AUDIO_FMT_MP3,
 *      ASH_AUDIO_FMT_AAC,
 *      ASH_AUDIO_FMT_DTS,
 *      ASH_AUDIO_FMT_WMA,
 *      ASH_AUDIO_FMT_RA,
 *      ASH_AUDIO_FMT_HDCD,
 *      ASH_AUDIO_FMT_MLP,
 *      ASH_AUDIO_FMT_MTS,
 *      ASH_AUDIO_FMT_EU_CANAL_PLUS,
 *      ASH_AUDIO_FMT_TV_SYS,
 *      ASH_AUDIO_FMT_COOK  // megaa
 *  } ASH_AUDIO_FMT_T;
 *  @endcode
 *  @li@c  ASH_AUDIO_FMT_UNKNOWN                             -
 *  @li@c  ASH_AUDIO_FMT_MPEG                                -
 *  @li@c  ASH_AUDIO_FMT_AC3                                 -
 *  @li@c  ASH_AUDIO_FMT_E_AC3                               -
 *  @li@c  ASH_AUDIO_FMT_PCM                                 -
 *  @li@c  ASH_AUDIO_FMT_MP3                                 -
 *  @li@c  ASH_AUDIO_FMT_AAC                                 -
 *  @li@c  ASH_AUDIO_FMT_DTS                                 -
 *  @li@c  ASH_AUDIO_FMT_WMA                                 -
 *  @li@c  ASH_AUDIO_FMT_RA                                  -
 *  @li@c  ASH_AUDIO_FMT_HDCD                                -
 *  @li@c  ASH_AUDIO_FMT_MLP                                 -
 *  @li@c  ASH_AUDIO_FMT_MTS                                 -
 *  @li@c  ASH_AUDIO_FMT_EU_CANAL_PLUS                       -
 *  @li@c  ASH_AUDIO_FMT_TV_SYS                              -
 *  @li@c  ASH_AUDIO_FMT_COOK  // megaa                      -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    ASH_AUDIO_FMT_UNKNOWN = 0,
    ASH_AUDIO_FMT_MPEG,
    ASH_AUDIO_FMT_AC3,
    ASH_AUDIO_FMT_E_AC3,
    ASH_AUDIO_FMT_PCM,
    ASH_AUDIO_FMT_MP3,
    ASH_AUDIO_FMT_AAC,
    ASH_AUDIO_FMT_DTS,
    ASH_AUDIO_FMT_WMA,
    ASH_AUDIO_FMT_RA,
    ASH_AUDIO_FMT_HDCD,
    ASH_AUDIO_FMT_MLP,
    ASH_AUDIO_FMT_MTS,
    ASH_AUDIO_FMT_EU_CANAL_PLUS,
    ASH_AUDIO_FMT_TV_SYS,
    ASH_AUDIO_FMT_COOK,
    ASH_AUDIO_FMT_VORBIS,
    ASH_AUDIO_FMT_HE_AAC,
    ASH_AUDIO_FMT_HE_AAC_V2,
    ASH_AUDIO_FMT_WMAPRO,
    ASH_AUDIO_FMT_WMALOSSLESS,
    ASH_AUDIO_FMT_AMR,
    ASH_AUDIO_FMT_AWB
} ASH_AUDIO_FMT_T;

/* audio decoder types. */
typedef enum
{
    ASH_DECODE_TYPE_UNKNOWN = 0,
    ASH_DECODE_TYPE_AC3,
    ASH_DECODE_TYPE_EAC3
} ASH_DECODE_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum  ASH_CHANNELS_T
 *  @brief   Contains audio channels info.
 *  @code
 *  typedef enum
 *  {
 *      ASH_CHANNELS_UNKNOWN = 0,
 *      ASH_CHANNELS_MONO,                   
 *      ASH_CHANNELS_MONO_SUB,               
 *      ASH_CHANNELS_DUAL_MONO,              
 *      ASH_CHANNELS_STEREO,                 
 *      ASH_CHANNELS_STEREO_SUB,             
 *      ASH_CHANNELS_STEREO_DOLBY_SURROUND,  
 *      ASH_CHANNELS_SURROUND_2CH,           
 *      ASH_CHANNELS_SURROUND,               
 *      ASH_CHANNELS_3_0,                    
 *      ASH_CHANNELS_4_0,                    
 *      ASH_CHANNELS_5_1,                    
 *      ASH_CHANNELS_7_1,                    
 *      ASH_CHANNELS_FM_MONO_NICAM_MONO,
 *      ASH_CHANNELS_FM_MONO_NICAM_STEREO,
 *      ASH_CHANNELS_FM_MONO_NICAM_DUAL,
 *      ASH_CHANNELS_OTHERS,
 *      ASH_CHANNELS_5_0,                    
 *      ASH_CHANNELS_JOINT_STEREO,
 *      ASH_CHANNELS_FMRDO_MONO,
 *      ASH_CHANNELS_FMRDO_STEREO
 *  } ASH_CHANNELS_T;
 *  @endcode
 *  @li@c  ASH_CHANNELS_UNKNOWN                              -
 *  @li@c  ASH_CHANNELS_MONO                                 -
 *  @li@c  ASH_CHANNELS_MONO_SUB                             -
 *  @li@c  ASH_CHANNELS_DUAL_MONO                            -
 *  @li@c  ASH_CHANNELS_STEREO                               -
 *  @li@c  ASH_CHANNELS_STEREO_SUB                           -
 *  @li@c  ASH_CHANNELS_STEREO_DOLBY_SURROUND                -
 *  @li@c  ASH_CHANNELS_SURROUND_2CH                         -
 *  @li@c  ASH_CHANNELS_SURROUND                             -
 *  @li@c  ASH_CHANNELS_3_0                                  -
 *  @li@c  ASH_CHANNELS_4_0                                  -
 *  @li@c  ASH_CHANNELS_5_1                                  -
 *  @li@c  ASH_CHANNELS_7_1                                  -
 *  @li@c  ASH_CHANNELS_FM_MONO_NICAM_MONO                   -
 *  @li@c  ASH_CHANNELS_FM_MONO_NICAM_STEREO                 -
 *  @li@c  ASH_CHANNELS_FM_MONO_NICAM_DUAL                   -
 *  @li@c  ASH_CHANNELS_OTHERS                               -
 *  @li@c  ASH_CHANNELS_5_0                                  -
 *  @li@c  ASH_CHANNELS_JOINT_STEREO                         -
 *  @li@c  ASH_CHANNELS_FMRDO_MONO                           -
 *  @li@c  ASH_CHANNELS_FMRDO_STEREO                         -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    ASH_CHANNELS_UNKNOWN = 0,
    ASH_CHANNELS_MONO,                   /* 1/0 */
    ASH_CHANNELS_MONO_SUB,               /* 1+sub-language */
    ASH_CHANNELS_DUAL_MONO,              /* 1+1 */
    ASH_CHANNELS_STEREO,                 /* 2/0 */
    ASH_CHANNELS_STEREO_SUB,             /* 2+sub-language */
    ASH_CHANNELS_STEREO_DOLBY_SURROUND,  /* 2/0, dolby surround */
    ASH_CHANNELS_SURROUND_2CH,           /* 2/1 */
    ASH_CHANNELS_SURROUND,               /* 3/1 */
    ASH_CHANNELS_3_0,                    /* 3/0 */
    ASH_CHANNELS_4_0,                    /* 2/2 */
    ASH_CHANNELS_5_1,                    /* 3/2.L */
    ASH_CHANNELS_7_1,                    /* 5/2.L*/
    ASH_CHANNELS_FM_MONO_NICAM_MONO,
    ASH_CHANNELS_FM_MONO_NICAM_STEREO,
    ASH_CHANNELS_FM_MONO_NICAM_DUAL,
    ASH_CHANNELS_5_0,                    /* 3/2 */
    ASH_CHANNELS_JOINT_STEREO,
    ASH_CHANNELS_FMRDO_MONO,
    ASH_CHANNELS_FMRDO_STEREO,
    ASH_CHANNELS_MONO_LFE,               /* 1/0.L */
    ASH_CHANNELS_DUAL_MONO_LFE,          /* 1+1.L */
    ASH_CHANNELS_STEREO_LFE,             /* 2/0.L */
    ASH_CHANNELS_SURROUND_2CH_LFE,       /* 2/1.L */
    ASH_CHANNELS_SURROUND_LFE,           /* 3/1.L */
    ASH_CHANNELS_3_1,                    /* 3/0.L */
    ASH_CHANNELS_4_1,                    /* 2/2.L */

    ASH_CHANNELS_OTHERS                  /* please reserve this entry as the last one. */

} ASH_CHANNELS_T;

/*------------------------------------------------------------------*/
/*! @enum ASH_AUDIO_DECODE_STATUS_T
 *  @brief  Identify audio decode status.
 *  @code
 *  typedef enum
 *  {
 *      ASH_AUDIO_DECODE_UNKNOWN = -1,
 *      ASH_AUDIO_DECODE_NO_DATA,
 *      ASH_AUDIO_DECODE_NORMAL,
 *      ASH_AUDIO_DECODE_ERROR,
 *      ASH_AUDIO_DECODE_NOT_SUPPORT,
 *      ASH_AUDIO_DECODE_FMT_CONFLICT
 *  } ASH_AUDIO_DECODE_STATUS_T;
 *  @endcode
 *  @li@c  ASH_AUDIO_DECODE_UNKNOWN                          -audio decode in unknown status.
 *  @li@c  ASH_AUDIO_DECODE_NO_DATA                          -audio decode send no data info.
 *  @li@c  ASH_AUDIO_DECODE_NORMAL                           -audio decode send normal info.
 *  @li@c  ASH_AUDIO_DECODE_ERROR                            -audio decode send error info.
 *  @li@c  ASH_AUDIO_DECODE_NOT_SUPPORT                      -audio decode send not support info.
 *  @li@c  ASH_AUDIO_DECODE_FMT_CONFLICT                     -audio decode send format conflict info.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    ASH_AUDIO_DECODE_UNKNOWN = -1,
    ASH_AUDIO_DECODE_NO_DATA,
    ASH_AUDIO_DECODE_NORMAL,
    ASH_AUDIO_DECODE_ERROR,
    ASH_AUDIO_DECODE_NOT_SUPPORT,
    ASH_AUDIO_DECODE_FMT_CONFLICT
} ASH_AUDIO_DECODE_STATUS_T;

/*------------------------------------------------------------------*/
/*! @enum  SCC_AUD_DOWNMIX_MODE_T
 *  @brief  Specify audio downmix mode.
 *  @code
 *  typedef enum
 *  {
 *      ASH_AUD_DOWNMIX_MODE_OFF = 0,
 *      ASH_AUD_DOWNMIX_MODE_LT_RT,
 *      ASH_AUD_DOWNMIX_MODE_STEREO,
 *      ASH_AUD_DOWNMIX_MODE_VIR_SURR,
 *      ASH_AUD_DOWNMIX_MODE_MONO,
 *      ASH_AUD_DOWNMIX_MODE_DUAL1,
 *      ASH_AUD_DOWNMIX_MODE_DUAL2,
 *      ASH_AUD_DOWNMIX_MODE_DUAL_MIX,
 *      ASH_AUD_DOWNMIX_MODE_3_STEREO
 *  }   ASH_AUD_DOWNMIX_MODE_T;
 *  @endcode
 *  @li@c  ASH_AUD_DOWNMIX_MODE_OFF                          -off
 *  @li@c  ASH_AUD_DOWNMIX_MODE_LT_RT                        -LT_RT
 *  @li@c  ASH_AUD_DOWNMIX_MODE_STEREO                       -Stereo
 *  @li@c  ASH_AUD_DOWNMIX_MODE_VIR_SURR                     -virtual surround
 *  @li@c  ASH_AUD_DOWNMIX_MODE_MONO                         -MONO
 *  @li@c  ASH_AUD_DOWNMIX_MODE_DUAL1                        -default 1
 *  @li@c  ASH_AUD_DOWNMIX_MODE_DUAL2                        -default 2
 *  @li@c  ASH_AUD_DOWNMIX_MODE_DUAL_MIX                     -dual mix
 *  @li@c  ASH_AUD_DOWNMIX_MODE_3_STEREO                     -3 Stereo
 */
/*------------------------------------------------------------------*/
typedef enum
{
    ASH_AUD_DOWNMIX_MODE_OFF = 0,
    ASH_AUD_DOWNMIX_MODE_LT_RT,
    ASH_AUD_DOWNMIX_MODE_STEREO,
    ASH_AUD_DOWNMIX_MODE_VIR_SURR,
    ASH_AUD_DOWNMIX_MODE_MONO,
    ASH_AUD_DOWNMIX_MODE_DUAL1,
    ASH_AUD_DOWNMIX_MODE_DUAL2,
    ASH_AUD_DOWNMIX_MODE_DUAL_MIX,
    ASH_AUD_DOWNMIX_MODE_3_STEREO
}   ASH_AUD_DOWNMIX_MODE_T;


/*------------------------------------------------------------------*/

/*! @struct  ASH_AUDIO_INFO_T
 *  @brief Contains audio infomation.
 *  @code
 *  typedef struct
 *  {
 *      ASH_AUDIO_FMT_T      e_aud_fmt;
 *      ASH_CHANNELS_T       e_channels;
 *      UINT32               ui4_sample_rate;
 *      UINT32               ui4_data_rate;
 *      UINT8                ui1_bit_depth;
 *  } ASH_AUDIO_INFO_T;
 *  @endcode
 *  @li@c  e_aud_fmt                                         -Audio format.
 *  @li@c  e_channels                                        -Audio channels.
 *  @li@c  ui4_sample_rate                                   -Audio sample rate.
 *  @li@c  ui4_data_rate                                     -Audio data rate.
 *  @li@c  ui1_bit_depth                                     -Audio bit depth.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    ASH_AUDIO_FMT_T      e_aud_fmt;
    ASH_CHANNELS_T       e_channels;
    UINT32               ui4_sample_rate;
    UINT32               ui4_data_rate;
    UINT8                ui1_bit_depth;
    ASH_DECODE_TYPE_T    e_dec_type;
} ASH_AUDIO_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct  ASH_AUD_SYS_INFO_T
 *  @brief Contains audio system infomation.
 *  @code
 *  typedef struct
 *  {
 *      UINT32         ui4_aud_sys;
 *      UINT32         ui4_tv_sys;
 *  } ASH_AUD_SYS_INFO_T;
 *  @endcode
 *  @li@c  ui4_aud_sys                                         -Audio system.
 *  @li@c  ui4_tv_sys                                        -tv system.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32         ui4_aud_sys;
    UINT32         ui4_tv_sys;
} ASH_AUD_SYS_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct ASH_AUDIO_WMA_INFO_T
 *  @brief   Contains WMA audio info.
 *  @code
 *  typedef struct _ASH_AUDIO_WMA_INFO_T
 *  {
 *      UINT16 ui2_hdr_src;       
 *      UINT16 ui2_num_ch;        
 *      UINT32 ui4_sample_per_sec; 
 *      UINT32 ui4_bits_per_sec;   
 *      UINT32 ui4_block_sz;       
 *      UINT16 ui2_encoder_opt;    
 *  } ASH_AUDIO_WMA_INFO_T;
 *  @endcode
 *  @li@c  ui2_hdr_src                                       -
 *  @li@c  ui2_num_ch                                        -
 *  @li@c  ui4_sample_per_sec                                -
 *  @li@c  ui4_bits_per_sec                                  -
 *  @li@c  ui4_block_sz                                      -
 *  @li@c  ui2_encoder_opt                                   -
 */
/*------------------------------------------------------------------*/
typedef struct _ASH_AUDIO_WMA_INFO_T
{
    UINT16 ui2_hdr_src;       /*W_WMA_HEADER_SOURCE*/
    UINT16 ui2_num_ch;        /*W_WMA_NUM_CH*/
    UINT32 ui4_sample_per_sec; /*D_WMA_SAMPLE_PER_SEC*/
    UINT32 ui4_bits_per_sec;   /*D_WMA_BYTE_PER_SEC*/
    UINT32 ui4_block_sz;       /*D_WMA_BLOCK_SIZE*/
    UINT16 ui2_encoder_opt;    /*W_WMA_ENCODER_OPTION*/
    UINT32 ui4_channel_mask;   /*D_WMA_CHANNEL_MASK*/
    UINT16 ui2_valid_bits;     /*W_WMA_VALID_BITS_PER_SAMPLE*/
    UINT16 ui2_adv_option;     /*W_WMA_ADVANCED_ENCODE_OPT*/
    UINT32 ui4_adv_option2;     /*W_WMA_ADVANCED_ENCODE_OPT2*/
} ASH_AUDIO_WMA_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct  ASH_AUDIO_AAC_INFO_T
 *  @brief Contains AAC audio info.
 *  @code
 *  typedef struct _ASH_AUDIO_AAC_INFO_T
 *  {
 *      UINT32 ui4_num_ch;
 *      UINT32 ui4_sample_rate;
 *  } ASH_AUDIO_AAC_INFO_T;
 *  @endcode
 *  @li@c  ui4_num_ch                                        -
 *  @li@c  ui4_sample_rate                                   -
 */
/*------------------------------------------------------------------*/
typedef struct _ASH_AUDIO_AAC_INFO_T
{
    UINT32 ui4_num_ch;
    UINT32 ui4_sample_rate;
    UINT32 ui4_aac_profile;
    BOOL   b_frame_length;
} ASH_AUDIO_AAC_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum ASH_AUDIO_PCM_TYPE_T
 *  @brief  Identify PCM audio type.
 *  @code
 *  typedef enum
 *  {
 *      ASH_AUDIO_PCM_TYPE_NORMAL,
 *      ASH_AUDIO_PCM_TYPE_MS_ADPCM,
 *      ASH_AUDIO_PCM_TYPE_IMA_ADPCM
 *  } ASH_AUDIO_PCM_TYPE_T;
 *  @endcode
 *  @li@c  ASH_AUDIO_PCM_TYPE_NORMAL                         -
 *  @li@c  ASH_AUDIO_PCM_TYPE_MS_ADPCM                       -
 *  @li@c  ASH_AUDIO_PCM_TYPE_IMA_ADPCM                      -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    ASH_AUDIO_PCM_TYPE_NORMAL,
    ASH_AUDIO_PCM_TYPE_MS_ADPCM,
    ASH_AUDIO_PCM_TYPE_IMA_ADPCM,
    ASH_AUDIO_PCM_TYPE_A_PCM,
    ASH_AUDIO_PCM_TYPE_U_PCM
} ASH_AUDIO_PCM_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct  ASH_AUDIO_PCM_INFO_T
 *  @brief   Contains PCM audio info.
 *  @code
 *  typedef struct _ASH_AUDIO_PCM_INFO_T
 *  {
 *      ASH_AUDIO_PCM_TYPE_T e_pcm_type;
 *      UINT32                ui4_num_ch;
 *      UINT32                ui4_sample_rate;
 *      UINT16                ui2_block_align;
 *      UINT16                ui2_bits_per_sample;
 *      BOOL                  b_big_endian;
 *      BOOL   b_info_not_specific;
 *  } ASH_AUDIO_PCM_INFO_T;
 *  @endcode
 *  @li@c  e_pcm_type                                        -
 *  @li@c  ui4_num_ch                                        -
 *  @li@c  ui4_sample_rate                                   -
 *  @li@c  ui2_block_align                                   -
 *  @li@c  ui2_bits_per_sample                               -
 *  @li@c  b_big_endian                                      -
 *  @li@c  b_info_not_specific                               -
 */
/*------------------------------------------------------------------*/
typedef struct _ASH_AUDIO_PCM_INFO_T
{
    ASH_AUDIO_PCM_TYPE_T e_pcm_type;
    UINT32                ui4_num_ch;
    UINT32                ui4_sample_rate;
    UINT16                ui2_block_align;
    UINT16                ui2_bits_per_sample;
    BOOL                  b_big_endian;
    BOOL   b_info_not_specific;/*pcm info from dmx*/
    BOOL                  b_signed;
} ASH_AUDIO_PCM_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct  ASH_AUDIO_COOK_INFO_T
 *  @brief Contains COOK audio info.
 *  @code
 *  typedef struct _ASH_AUDIO_COOK_INFO_T
 *  {
 *      UINT16          ui2_num_channel;
 *      UINT16          ui2_flavor_idx;
 *      UINT16          ui2_codec_frm_sz;
 *      UINT16          ui2_intlv_factor;
 *      UINT16          ui2_intlv_blk_sz;
 *      UINT32          ui4_sample_rate;
 *      void*           pv_codec_opaque_data;
 *  } ASH_AUDIO_COOK_INFO_T;
 *  @endcode
 *  @li@c  ui2_num_channel                                   -
 *  @li@c  ui2_flavor_idx                                    -
 *  @li@c  ui2_codec_frm_sz                                  -
 *  @li@c  ui2_intlv_factor                                  -
 *  @li@c  ui2_intlv_blk_sz                                  -
 *  @li@c  ui4_sample_rate                                   -
 *  @li@c  pv_codec_opaque_data                              -
 */
/*------------------------------------------------------------------*/
typedef struct _ASH_AUDIO_COOK_INFO_T
{
    UINT16          ui2_num_channel;
    UINT16          ui2_flavor_idx;
    UINT16          ui2_codec_frm_sz;
    UINT16          ui2_intlv_factor;
    UINT16          ui2_intlv_blk_sz;
    UINT32          ui4_sample_rate;
    void*           pv_codec_opaque_data;
} ASH_AUDIO_COOK_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct  ASH_AUDIO_VORBIS_INFO_T
 *  @brief Contains VORBIS audio info.
 *  @code
 *typedef struct _ASH_ADUIO_VORBIS_INFO_T
 *{
 *   UINT8       ui1_vorbis_id[7];
 *   UINT32     ui4_vorbis_version;
 *   UINT8       ui1_audio_channel;
 *   UINT32     ui4_audio_sample_rate;
 *   UINT32     ui4_bitrate_maximun;
 *   UINT32     ui4_bitrate_norminal;
 *   UINT32     ui4_bitrate_minimun;
 *   UINT8       blocksize;
 *   UINT8       frameflag;
 *} ASH_ADUIO_VORBIS_INFO_T;
 */
/*------------------------------------------------------------------*/

typedef struct _ASH_ADUIO_VORBIS_INFO_T
{
    UINT8      ui1_vorbis_id[7];
    UINT32     ui4_vorbis_version;
    UINT8      ui1_audio_channel;
    UINT32     ui4_audio_sample_rate;
    UINT32     ui4_bitrate_maximun;
    UINT32     ui4_bitrate_norminal;
    UINT32     ui4_bitrate_minimun;
    UINT8      blocksize;
    UINT8      frameflag;
} ASH_ADUIO_VORBIS_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct  ASH_AUDIO_AC3_INFO_T
 *  @brief Contains AC3 audio info.
 *  @code
 *typedef struct _ASH_ADUIO_Ac3_INFO_T
 *{
 *   UINT8       ui1_endian; // 0 = little, 1 = big ;
 *} ASH_ADUIO_AC3_INFO_T;
 */
/*------------------------------------------------------------------*/

typedef struct _ASH_ADUIO_AC3_INFO_T
{
    UINT8       ui1_endian; // 0 = little, 1 = big ;
} ASH_ADUIO_AC3_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct ASH_AUDIO_CODEC_SPEC_INFO_T
 *  @brief  Contains audio codec specification info.
 *  @code
 *  typedef struct _ASH_AUDIO_CODEC_SPEC_INFO_T
 *  {
 *      ASH_AUDIO_FMT_T  e_fmt;
 *      union
 *      {
 *          ASH_AUDIO_WMA_INFO_T t_wma_info;
 *          ASH_AUDIO_AAC_INFO_T t_aac_info;
 *          ASH_AUDIO_PCM_INFO_T t_pcm_info;
 *          ASH_AUDIO_COOK_INFO_T t_cook_info;
 *      } u;
 *  } ASH_AUDIO_CODEC_SPEC_INFO_T;
 *  @endcode
 *  @li@c  e_fmt                                             -
 *  @li@c  t_wma_info                                        -
 *  @li@c  t_aac_info                                        -
 *  @li@c  t_pcm_info                                        -
 *  @li@c  t_cook_info                                       -
 */
/*------------------------------------------------------------------*/
typedef struct _ASH_AUDIO_CODEC_SPEC_INFO_T
{
    ASH_AUDIO_FMT_T  e_fmt;
    union
    {
        ASH_AUDIO_WMA_INFO_T    t_wma_info;
        ASH_AUDIO_AAC_INFO_T    t_aac_info;
        ASH_AUDIO_PCM_INFO_T    t_pcm_info;
        ASH_AUDIO_COOK_INFO_T   t_cook_info;
        ASH_ADUIO_VORBIS_INFO_T t_vorbis_info;
        ASH_ADUIO_AC3_INFO_T    t_ac3_info;
    } u;
} ASH_AUDIO_CODEC_SPEC_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum ASH_AUD_DEC_DDC_ID_T
 *  @brief  Identify PCM audio type.
 *  @code
 *  typedef enum
 *  {
 *      ASH_AUD_DEC_DDC_ID_UNKNOWN,
 *      ASH_AUD_DEC_DDC_ID_1,
 *      ASH_AUD_DEC_DDC_ID_2,
 *      ASH_AUD_DEC_DDC_ID_3,
 *  } ASH_AUD_DEC_DDC_ID_T;
 *  @endcode
 *  @li@c  ASH_AUD_DEC_DDC_ID_UNKNOWN                         -
 *  @li@c  ASH_AUD_DEC_DDC_ID_1                       -
 *  @li@c  ASH_AUD_DEC_DDC_ID_2                      -
 *  @li@c  ASH_AUD_DEC_DDC_ID_3                       -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    ASH_AUD_DEC_DDC_ID_UNKNOWN = 0,
    ASH_AUD_DEC_DDC_ID_1, 
    ASH_AUD_DEC_DDC_ID_2,
    ASH_AUD_DEC_DDC_ID_3
} ASH_AUD_DEC_DDC_ID_T;


/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

#endif /* _U_SM_AUDIO_HDLR_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_STRM_MNGR_AUDIO_HDLR */
/*----------------------------------------------------------------------------*/

