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
 * $RCSfile: mtauddec.h,v $
 * $Revision: #3 $
 *---------------------------------------------------------------------------*/

/** @file mtauddec.h
 *  This header file declares exported APIs of audio decoder module.
 */

#ifndef MT_AUDDEC_H
#define MT_AUDDEC_H

#ifdef __cplusplus
extern "C"
{
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mttype.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#define MIXSOUND_MEMPLAY_USER_SPACE_BUFFER 1

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define MT_AUD_DEC_MAIN    (0)
#define MT_AUD_DEC_AUX     (1)
#define MT_AUD_DEC_NUM     (2)

// Decoder define for EU model
#define MT_AUD_DEC_EU_MAIN                 (0)
#define MT_AUD_DEC_EU_SCART                (1)
#define MT_AUD_DEC_EU_AUDIO_DESCRIPTION    (2)
#define MT_AUD_DEC_EU_NUM                  (3)

// Copyright constant
#define MT_CGMS_COPY_FREELY                (0x00)
#define MT_CGMS_COPY_NO_MORE               (0x01)
#define MT_CGMS_COPY_ONCE                  (0x02)
#define MT_CGMS_COPY_NEVER                 (0x03)

// IEC category code
#define MT_IEC_ORIGINAL_GENERAL            (0x0)
#define MT_IEC_ORIGINAL_BROAD_CAST         (0x64)
#define MT_IEC_NON_ORIGINAL_BROAD_CAST     (0xe4)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

    /**Line-in mux selection.
    */
    typedef enum
    {
        MTAUDDEC_INPUT_ID_COMP_VID_0 = 0,
        MTAUDDEC_INPUT_ID_COMP_VID_1,
        MTAUDDEC_INPUT_ID_COMP_VID_2,
        MTAUDDEC_INPUT_ID_COMP_VID_3,
        MTAUDDEC_INPUT_ID_S_VID_0,
        MTAUDDEC_INPUT_ID_S_VID_1,
        MTAUDDEC_INPUT_ID_S_VID_2,
        MTAUDDEC_INPUT_ID_YPBPR_0,
        MTAUDDEC_INPUT_ID_YPBPR_1,
        MTAUDDEC_INPUT_ID_YPBPR_2,
        MTAUDDEC_INPUT_ID_VGA_0,
        MTAUDDEC_INPUT_ID_VGA_1,
        MTAUDDEC_INPUT_ID_HDMI_0,
        MTAUDDEC_INPUT_ID_HDMI_1,
        MTAUDDEC_INPUT_ID_HDMI_2,
        MTAUDDEC_INPUT_ID_HDMI_3,
        MTAUDDEC_INPUT_ID_HDMI_4,
        MTAUDDEC_INPUT_ID_DVI_0,
        MTAUDDEC_INPUT_ID_DVI_1,
        MTAUDDEC_INPUT_ID_DVI_2,
        MTAUDDEC_INPUT_ID_DVI_3,
        MTAUDDEC_INPUT_ID_DVI_4,
        MTAUDDEC_INPUT_ID_SCART_0,
        MTAUDDEC_INPUT_ID_SCART_1,
        MTAUDDEC_INPUT_ID_SCART_2,
        MTAUDDEC_INPUT_ID_SCART_3,
        MTAUDDEC_INPUT_ID_AUXIN_0,
        MTAUDDEC_INPUT_ID_AUXIN_1,
        MTAUDDEC_INPUT_ID_MAX
    }MTAUDDEC_INPUT_ID_T;

    /**Audio stream source selection.
    */
    typedef enum
    {
        MTAUDDEC_STREAM_FROM_OTHERS = 0,         ///< ex. Pink Noise Generator
        MTAUDDEC_STREAM_FROM_DIGITAL_TUNER,      ///< Digital tuner. TS
        MTAUDDEC_STREAM_FROM_ANALOG_TUNER,       ///< ex. Analog tuner. SIF/AF
        MTAUDDEC_STREAM_FROM_SPDIF,              ///< SPDIF-in
        MTAUDDEC_STREAM_FROM_LINE_IN,            ///< Line in
        MTAUDDEC_STREAM_FROM_HDMI,               ///< HDMI in
        MTAUDDEC_STREAM_FROM_MEMORY,             ///< Memory, ex. memory clip
        MTAUDDEC_STREAM_FROM_MPLAYER,            ///< Media Player
        MTAUDDEC_STREAM_FROM_INTERNAL_BUF,       ///< Internal buffer
        MTAUDDEC_STREAM_FROM_NUM                 ///< Number of stream source
    }MTAUDDEC_DEC_STREAM_FROM_T;


    /**AC3 downmix mode.
    */
    typedef enum
    {
        MTAUDDEC_AC3_DOWNMIX_LTRT = 0,         ///< Lt/Rt downmix
        MTAUDDEC_AC3_DOWNMIX_LORO,             ///< LoRo downmix
        MTAUDDEC_AC3_DOWNMIX_NUM
    }MTAUDDEC_AC3_DOWNMIX_MODE;

    /**AC3 compression mode.
    */
    typedef enum
    {
        MTAUDDEC_AC3_COMP_CUSTOM_0 = 0,         ///< Customer 0
        MTAUDDEC_AC3_COMP_CUSTOM_1,             ///< Customer 1
        MTAUDDEC_AC3_COMP_LINE,                 ///< LINE mode
        MTAUDDEC_AC3_COMP_RF,                   ///< RF mode
        MTAUDDEC_AC3_COMP_NUM,
    }MTAUDDEC_AC3_COMP_MODE_T;

    /**AC3 dual mono mode.
    */
    typedef enum
    {
        MTAUDDEC_AC3_DUALMONO_STEREO = 0,       ///< Both channel as stereo
        MTAUDDEC_AC3_DUALMONO_MONO_L,           ///< Channel L only
        MTAUDDEC_AC3_DUALMONO_MONO_R,           ///< Channel R only
        MTAUDDEC_AC3_DUALMONO_MIX_MONO,         ///< Both channel mix together
        MTAUDDEC_AC3_DUALMONO_NUM,
    }MTAUDDEC_AC3_DUALMONO_MODE;

    /**Analog TV detect mode.
    */
    typedef enum
    {
        MTAUDDEC_ATV_DETECT_UNKOWN = 0,     ///< Unkown
        MTAUDDEC_ATV_DETECT_MONO,           ///< MTS/A2/EIAJ/FM-Mono mono
        MTAUDDEC_ATV_DETECT_STEREO,         ///< MTS/A2/EIAJ  stereo
        MTAUDDEC_ATV_DETECT_STEREO_SAP,     ///< MTS stereo + SAP
        MTAUDDEC_ATV_DETECT_SAP,            ///< MTS SAP
        MTAUDDEC_ATV_DETECT_DUAL,           ///< A2/EIAJ  dual
        MTAUDDEC_ATV_DETECT_NICAM_MONO,     ///< NICAM mono (with FM/AM mono)
        MTAUDDEC_ATV_DETECT_NICAM_STEREO,   ///< NICAM stereo (with FM/AM mono)
        MTAUDDEC_ATV_DETECT_NICAM_DUAL,     ///< NICAM dual (with FM/AM mono)
        MTAUDDEC_ATV_DETECT_MODE_NUM        ///< Number of atv detect mode
    }MTAUDDEC_ATV_DETECT_MODE_T;

    /**Analog TV output mode.
    */
    typedef enum
    {
        MTAUDDEC_SOUND_MODE_UNKNOWN = 0,    ///< Unkown
        MTAUDDEC_SOUND_MODE_MONO,           ///< MTS/A2/EIAJ/FM-Mono/AM-Mono mono
        MTAUDDEC_SOUND_MODE_STEREO,         ///< MTS/A2/EIAJ stereo
        MTAUDDEC_SOUND_MODE_SAP,            ///< MTS SAP
        MTAUDDEC_SOUND_MODE_DUAL1,          ///< A2/EIAJ dual 1
        MTAUDDEC_SOUND_MODE_DUAL2,          ///< A2/EIAJ dual 2
        MTAUDDEC_SOUND_MODE_NICAM_MONO,     ///< NICAM mono
        MTAUDDEC_SOUND_MODE_NICAM_STEREO,   ///< NICAM stereo
        MTAUDDEC_SOUND_MODE_NICAM_DUAL1,    ///< NICAM L:dual 1, R: dual 1
        MTAUDDEC_SOUND_MODE_NICAM_DUAL2,    ///< NICAM L:dual 2, R: dual 2
        MTAUDDEC_SOUND_MODE_A2_DUAL1_DUAL2, ///< A2/EIAJ L:dual 1, R: dual 2
        MTAUDDEC_SOUND_MODE_NUM             ///< Number of sound mode
    }MTAUDDEC_SOUND_MODE_T;


    /**SIF standard.
    * Should set to audio decoder before issue playback at EU/KOREA/EIAJ model.
    */
    typedef enum
    {
        MTAUDDEC_SIF_UNKNOWN = 0,
        MTAUDDEC_SIF_BG,           ///< TV-Sys: B/G, Audio Modulation: FM-Mono/NICAM
        MTAUDDEC_SIF_BG_A2,        ///< TV-Sys: B/G, Audio Modulation: FM-A2
        MTAUDDEC_SIF_I,            ///< TV-Sys: I,   Audio Modulation: FM-Mono/NICAM
        MTAUDDEC_SIF_DK,           ///< TV-Sys: D/K, Audio Modulation: FM-Mono/NICAM (D/K)
        MTAUDDEC_SIF_DK1_A2,       ///< TV-Sys: D/K, Audio Modulation: FM-A2 (D/K1)
        MTAUDDEC_SIF_DK2_A2,       ///< TV-Sys: D/K, Audio Modulation: FM-A2 (D/K2)
        MTAUDDEC_SIF_DK3_A2,       ///< TV-Sys: D/K, Audio Modulation: FM-A2 (D/K3)
        MTAUDDEC_SIF_L,            ///< TV-Sys: L,   Audio Modulation: AM-Mono/NICAM
        MTAUDDEC_SIF_A2_KOREA,     ///< TV-Sys: M/N, Audio Modulation: FM-Stereo
        MTAUDDEC_SIF_BTSC,         ///< TV-Sys: M/N, Audio Modulation: BTSC-Stereo + SAP
        MTAUDDEC_SIF_EIAJ,         ///< TV-Sys: M/N, Audio Modulation: FM-FM (EIAJ)
        MTAUDDEC_SIF_NUM
    }MTAUDDEC_SIF_STANDARD_T;

    /**Digital audio type.
    */
    typedef enum
    {
        MTAUDDEC_MODE_UNKNOWN = 0,
        MTAUDDEC_MODE_MONO,                 ///< 1/0
        MTAUDDEC_MODE_MONO_SUB,             ///< 1+sub-language
        MTAUDDEC_MODE_DUAL_MONO,            ///< 1+1
        MTAUDDEC_MODE_STEREO,               ///< 2/0
        MTAUDDEC_MODE_STEREO_SUB,           ///< 2+sub-language
        MTAUDDEC_MODE_STEREO_DOLBY_SURROUND,///< 2/0, dolby surround
        MTAUDDEC_MODE_SURROUND_2CH,         ///< 2/1
        MTAUDDEC_MODE_SURROUND,             ///< 3/1
        MTAUDDEC_MODE_3_0,                  ///< 3/0
        MTAUDDEC_MODE_4_0,                  ///< 2/2
        MTAUDDEC_MODE_5_1,                  ///< 3/2
        MTAUDDEC_MODE_NUM                   ///< Number of decode type in AC3
    }MTAUDDEC_MODE_T;

    /**Individual audio channels
    */
    typedef enum
    {
        MTAUDDEC_CH_FRONT_LEFT = 0,
        MTAUDDEC_CH_FRONT_RIGHT,
        MTAUDDEC_CH_REAR_LEFT,
        MTAUDDEC_CH_REAR_RIGHT,
        MTAUDDEC_CH_CENTER,
        MTAUDDEC_CH_SUB_WOOFER,
        MTAUDDEC_CH_BYPASS_LEFT,
        MTAUDDEC_CH_BYPASS_RIGHT,
        MTAUDDEC_CH_DMX_LEFT,
        MTAUDDEC_CH_DMX_RIGHT,
        MTAUDDEC_CH_AUX_FRONT_LEFT,
        MTAUDDEC_CH_AUX_FRONT_RIGHT,
        MTAUDDEC_CH_INPUT_SRC,              ///< For input source volume control
        MTAUDDEC_CH_ALL,                    ///< For master volume control
        MTAUDDEC_CH_LFE,
        MTAUDDEC_CH_NUM
    }MTAUDDEC_CH_T;

    /**IEC format
    */
    typedef enum
    {
        MTAUDDEC_IEC_CFG_PCM = 0,
        MTAUDDEC_IEC_CFG_RAW
    }MTAUDDEC_IEC_T;

    /**Speaker output mode
    */
    typedef enum
    {
        MTAUDDEC_SPK_MODE_LR = 0,
        MTAUDDEC_SPK_MODE_LL,
        MTAUDDEC_SPK_MODE_RR,
        MTAUDDEC_SPK_MODE_MIX
    }MTAUDDEC_SKP_OUTPUT_MODE_T;

    /**PCM sampling rate. Used in audio clip playback.
    */
    typedef enum
    {
        MTAUDDEC_FS_8K = 0,
        MTAUDDEC_FS_11K,
        MTAUDDEC_FS_12K,
        MTAUDDEC_FS_16K,
        MTAUDDEC_FS_22K,
        MTAUDDEC_FS_24K,
        MTAUDDEC_FS_32K,
        MTAUDDEC_FS_44K,
        MTAUDDEC_FS_48K,
        MTAUDDEC_FS_64K,
        MTAUDDEC_FS_88K,
        MTAUDDEC_FS_96K,
        MTAUDDEC_FS_176K,
        MTAUDDEC_FS_192K,
        MTAUDDEC_FS_NUM
    }MTAUDDEC_SAMPLE_FREQ_T;

    /**PCM data format. Used in audio clip playback.
    */
    typedef enum
    {
        MTAUDDEC_PCM_24_BITS = 0,           ///< 24 bits (only support big endian)
        MTAUDDEC_PCM_16_BITS_BIG_ENDIAN,    ///< 16 bits with big endian
        MTAUDDEC_PCM_16_BITS_LITTLE_ENDIAN, ///< 16 bits with little endian
        MTAUDDEC_PCM_NUM
    }MTAUDDEC_PCM_INPUT_TYPE_T;

    /**MTK surround parameters setup
    */
    typedef enum
    {
        MTAUDDEC_SUR_CLARITY = 0,
        MTAUDDEC_SUR_WIDTH,
        MTAUDDEC_SUR_LRGAIN,
        MTAUDDEC_SUR_XTALK,
        MTAUDDEC_SUR_OUTPUT_GAIN,
        MTAUDDEC_SUR_BASS_GAIN,
        MTAUDDEC_SUR_FO,
        MTAUDDEC_SUR_MAX
    }   MTAUDDEC_VSURR_CFG_TYPE_T;

    /**AVC parameters setup
    */
    typedef enum
    {
        MTAUDDEC_AVC_TARGET_LEVEL = 0,
        MTAUDDEC_AVC_MAX_GAIN_UP,
        MTAUDDEC_AVC_ATTACK_RATE,
        MTAUDDEC_AVC_RELEASE_RATE,
        MTAUDDEC_AVC_FC,
        MTAUDDEC_AVC_MAX
    }   MTAUDDEC_AVC_CFG_TYPE_T;

    /**Bass enhancer parameters setup
    */
    typedef enum
    {
        MTAUDDEC_BE_OUTPUT_GAIN = 0,
        MTAUDDEC_BE_BASS_GAIN,
        MTAUDDEC_BE_PARAM3,
        MTAUDDEC_BE_PARAM4,
        MTAUDDEC_BE_PARAM5,
        MTAUDDEC_BE_PARAM6,
        MTAUDDEC_BE_MAX
    }   MTAUDDEC_BE_PARAM_TYPE_T;

    /**PEQ Q value
    */
    typedef enum
    {
        MTAUDDEC_PEQ_Q_0_33 = 0,
        MTAUDDEC_PEQ_Q_0_43,
        MTAUDDEC_PEQ_Q_0_56,
        MTAUDDEC_PEQ_Q_0_75,
        MTAUDDEC_PEQ_Q_1_00,
        MTAUDDEC_PEQ_Q_1_20,
        MTAUDDEC_PEQ_Q_1_50,
        MTAUDDEC_PEQ_Q_1_80,
        MTAUDDEC_PEQ_Q_2_20,
        MTAUDDEC_PEQ_Q_2_27,
        MTAUDDEC_PEQ_Q_3_30,
        MTAUDDEC_PEQ_Q_3_39,
        MTAUDDEC_PEQ_Q_4_70,
        MTAUDDEC_PEQ_Q_5_60,
        MTAUDDEC_PEQ_Q_6_80,
        MTAUDDEC_PEQ_Q_8_20,
        MTAUDDEC_PEQ_Q_MAX
    }   MTAUDDEC_PEQ_Q_TYPE_T;

    /**PCM setting. Used in audio clip playback.
    */
    typedef struct
    {
        MTAUDDEC_SAMPLE_FREQ_T      eSampleFreq;  ///< PCM Sampling Rate.
        MTAUDDEC_PCM_INPUT_TYPE_T   ePcmDataInfo; ///< PCM data information
    }MTAUDDEC_PCM_SETTING_T;

    /**Audio decoder notification function type id.
    */
    typedef enum
    {
        MTAUDDEC_CB_FUNC_MODE_CHG_ID = 0,   ///< Audio mode change
        MTAUDDEC_CB_FUNC_MP3_ERROR_ID,  ///< MP3 Error
        MTAUDDEC_CB_FUNC_MIXSOUND_CTRL_ID,
        MTAUDDEC_CB_FUNC_CLIP_DONE_ID,        
        MTAUDDEC_CB_FUNC_HP_ID,
        MTAUDDEC_CB_FUNC_ID_MAX
    }MTAUDDEC_CB_FUNC_ID_T;

    /**Mp3 error type.
    */
    typedef enum
    {
        MTAUDDEC_MP3_ERROR_HEADER,     ///< Header error
        MTAUDDEC_MP3_ERROR_CRC,        ///< CRC Error
        MTAUDDEC_MP3_ERROR_FIELD_SIZE, ///< Field size Error
        MTAUDDEC_MP3_ERROR_MAX
    }MTAUDDEC_MP3_ERROR_T;

    /**Mixsound ctrl type.
    */
    typedef enum
    {
        MTAUDDEC_MIXSOUND_CTRL_PLAY_DONE,
        MTAUDDEC_MIXSOUND_CTRL_MAX
    }MTAUDDEC_MIXSOUND_CTRL_T;

    /**Audio mode change notify function type
    */
    typedef void (*MTAUDDEC_MODE_CHG)(
        UINT8  u1DecId,
        MTAUDDEC_MODE_T eMode,
        MTAUDDEC_SAMPLE_FREQ_T eSamFreq
    );

    /**Mp3 error function type
    */
    typedef void (*MTAUDDEC_MP3_ERROR)(
        UINT8  u1DecId,
        MTAUDDEC_MP3_ERROR_T eErrorType
    );

    /**Mixsound ctrl function type
    */
    typedef void (*MTAUDDEC_MIXSOUND_CTRL)(
        MTAUDDEC_MIXSOUND_CTRL_T eCtrlType
    );

    /**Clip done ctrl function type
    */
    typedef void (*MTAUDDEC_CLIP_DONE_CTRL)(
        void
    );
   /**HP connect function type
    */
    typedef void (*MTAUDDEC_HP_CTRL)(
        BOOL fgConnect
    );
    /**Audio callback structure
    */
    typedef struct
    {
        UINT32 u4Arg1;
        UINT32 u4Arg2;
        UINT32 u4Arg3;
        UINT32 u4Arg4;
        UINT32 u4Arg5;
        UINT32 u4Arg6;
        UINT32 u4Arg7;
        UINT32 u4Arg8;
    }MTAUDDEC_CB_T;

    /**
     * AC3 ES Info
     *
     * @see
    */
    typedef struct MTAUDDEC_AC3_ES_INFO
    {
        UINT8 bitRate;
        UINT8 sampleRate;
        UINT8 channelNum;/* it is not fixed yet depend on mstar definition mono=0x0,Lo/Ro= 0x1,L/R/=0x2,L/R/LS/RS =0x3*/
        UINT8  EAC3;       /* AC3 0x0, EAC3 0x1*/
    }MTAUDDEC_AC3_ES_INFO_T;

    /**
     * MPEG ES Info
     *
     * @see
    */
    typedef struct MTAUDDEC_MPEG_ES_INFO
    {
        UINT8 bitRate;
        UINT8 sampleRate;
        UINT8 layer;
        UINT8  channelNum; /* mono =0x0,stereo=0x1,multi-channel =0x2*/
    }MTAUDDEC_MPEG_ES_INFO_T;

    /**
     * HE-AAC ES Info
     *
     * @see
    */
    typedef struct MTAUDDEC_HEAAC_ES_INFO
    {
        UINT8 version;    		  /*AAC 0x0  HE-AACv1= 0x1,HE-AACv2=0x2 */
        UINT8 transmissionformat;     /* LOAS/LATM =0x0 , ADTS=0x1*/
        UINT8 channelNum;
    }MTAUDDEC_HEAAC_ES_INFO_T;
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

    /**Initialize Auio Decider module.
    *@param void
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_Init (void);


    /**Get audio decoder type and stream source configuration.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param peStreamFrom               Stream source.
    *@param prDecType                  Decoder format.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetDecType (UINT8 u1DecId,
            MTAUDDEC_DEC_STREAM_FROM_T *peStreamFrom,
            MTAUDDEC_FMT_T *prDecType);

    /**Set audio decoder type and stream source.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eStreamFrom                Stream source.
    *@param eDecFmt                    Decoder format.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetDecType (UINT8 u1DecId,
            MTAUDDEC_DEC_STREAM_FROM_T eStreamFrom,
            MTAUDDEC_FMT_T eDecFmt);

    /**Audio decoder playback.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_Play (UINT8 u1DecId);


    /**Resume audio decoder.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_Resume (UINT8 u1DecId);


    /**Pause audio decoder.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_Pause (UINT8 u1DecId);


    /**Stop audio decoder.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_Stop (UINT8 u1DecId);

    EXTERN MT_RESULT_T  MTAUDDEC_StopAll (UINT8 u1DecId,UINT8 u1Dec2Id);

    EXTERN MT_RESULT_T  MTAUDDEC_SetHDevModeEnable (BOOL fgEnable);

    /**Get the information of audio decoder.
    *  Since in some case, even application issue play to decoder, but if the
    *  bitstream is not feed to decoder, there is no audio output yet.
    *  This function indicates the status that if the decoding is correctly getting
    *  bistream and successfully decoded.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param pfgPlay                    Is decoder real play
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetDecOnPlayingStatus (UINT8 u1DecId,
            BOOL* pfgOnPlay);

    /**Get audio decoder pts.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param pPts                          pts.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_GetAudioPTS(UINT8 u1DecId, UINT32 *pPts);

    /**Set AV synchronization mode.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eSynMode                   AV sync mode or just free run in DTV playback.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetAvSynMode (UINT8 u1DecId,
            MTAV_SYNC_MODE_T eSynMode);
    /**Set AV synchronization mode1.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eSynMode                   AV sync mode or just free run.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetAvSynMode1 (UINT8 u1DecId, UINT8 OnOff);

    /**Setup AC3 compression mode.
    * US/Austria model specific function
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eCompMode                  Compression mode.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetAc3CompMode (UINT8 u1DecId,
            MTAUDDEC_AC3_COMP_MODE_T eCompMode);


    /**Setup AC3 downmix mode.
    * US/Austria model specific function
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eDownmixMode               Downmix mode.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetAc3DownmixMode (UINT8 u1DecId,
            MTAUDDEC_AC3_DOWNMIX_MODE eDownmixMode);


    /**Get AC3 downmix mode.
    * US/Austria model specific function
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eDownmixMode               Downmix mode.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetAc3DownmixMode (UINT8 u1DecId,
            MTAUDDEC_AC3_DOWNMIX_MODE * peDownmixMode);

    /**Get AC3 audio mode.
    * US/Austria model specific function
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param peMode                     AC3 channel information.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetAc3Mode (UINT8 u1DecId,
            MTAUDDEC_MODE_T * peMode);

    /**Get audio decoder ac3 info
    *@param pAudioESInfo            audio es info.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_GetAC3ESInfo(UINT8 u1DecId, MTAUDDEC_AC3_ES_INFO_T *pAC3ESInfo);

    /**Get audio decoder mpeg esinfo.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param pAC3ESInfo            audio es info.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_GetMPEGESInfo(UINT8 u1DecId, MTAUDDEC_MPEG_ES_INFO_T *pMPEGESInfo);

    /**Get audio decoder HEAAC esinfo.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param pHEAACESInfo           audio es info.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_GetHEAACESInfo(UINT8 u1DecId, MTAUDDEC_HEAAC_ES_INFO_T *pHEAACESInfo);

    /**Get audio sampling rate.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param pSmpRate                   Sampling rate.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetSamplingRate (UINT8 u1DecId,
            MTAUDDEC_SAMPLE_FREQ_T * pSmpRate);


    /**Setup range for dynamix range compression.
    * US/Austria model specific function
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param u1DrcLevel                 DRC range.
    *                                  0 ~ 8 (step size is 0.125)
    *                                  0 -> 0.0
    *                                  1 -> 0.125
    *                                  8 -> 1.0
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetAc3DrcRange (UINT8 u1DecId, UINT8 u1DrcLevel);

    /**Setup 0~100 range for dynamix range compression.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param u1DrcLevel                 DRC range 0~100.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetAc3DrcRange100(UINT8 u1DecId, UINT8 u1DrcLevel);

    /**Setup AC3 dual mono mode.
    * US/Austria model specific function
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eDualMonoMode              Dual mono mode
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetAc3DualMono (UINT8 u1DecId,
            MTAUDDEC_AC3_DUALMONO_MODE eDualMonoMode);


    /**Setup AC3 dual mono mode.
    * US/Austria model specific function
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eDualMonoMode              Dual mono mode
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetAc3DualMono (UINT8 u1DecId,
            MTAUDDEC_AC3_DUALMONO_MODE * peDualMonoMode);


    /**Get the analogy TV stereo mode of current channel.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param peATVMode                  Detected mode
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetAtvDetectMode (UINT8 u1DecId,
            MTAUDDEC_ATV_DETECT_MODE_T *peATVMode);


    /**Setup analog TV output mode.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eSoundMode                 UNKNOWN = 0,
    *                                  MONO,
    *                                  STEREO,
    *                                  SAP,
    *                                  DUAL1,
    *                                  DUAL2,
    *                                  NICAM_MONO,
    *                                  NICAM_STEREO,
    *                                  NICAM_DUAL1,
    *                                  NICAM_DUAL2,
    *                                  A2_DUAL1,
    *                                  A2_DUAL2,
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetAtvOutputMode (UINT8 u1DecId,
            MTAUDDEC_SOUND_MODE_T eSoundMode);

    /**Setup SIF input mode
    * DVB EU model specific function
    * Should set to audio decoder before issue playback at EU/KOREA/EIAJ model.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eSIFMode                   SIF Mode
    *                                  NICAM BG,
    *                                  FM BG,
    *                                  A2 BG,
    *                                  NICAM I,
    *                                  FM I,
    *                                  NICAM DK,
    *                                  FM DK,
    *                                  A2 DK1,
    *                                  A2 DK3,
    *                                  NICAM L,
    *                                  AM L,
    *                                  A2 KOREA
    *                                  BTSC
    *                                  EIAJ
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetSIFMode (UINT8 u1DecId,
            MTAUDDEC_SIF_STANDARD_T eSIFMode);

    /**Get detected SIF mode
    * DVB EU model specific function
    * Issue decoder to do SIF mode detection. It may take some time for the detection.
    * Notes that application should stop decoder before invoking this function.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eSIFMode                   FM DK or AM L
    *                                  Since audio decoder cannot distinguish from
    *                                  FM DK and AM L, so application should set this
    *                                  information to audio decoder at first before
    *                                  issue decoder to do detection.
    *@param peSIFMode                  Detection result
    *                                  MODE_DETECT = 0,
    *                                  NICAM BG,
    *                                  FM BG,
    *                                  A2 BG,
    *                                  NICAM I,
    *                                  FM I,
    *                                  NICAM DK,
    *                                  FM DK,
    *                                  A2 DK1,
    *                                  A2 DK3,
    *                                  NICAM L,
    *                                  AM L,
    *                                  A2 KOREA
    *                                  BTSC
    *                                  EIAJ
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */

    EXTERN MT_RESULT_T  MTAUDDEC_GetDetectSIFMode (UINT8 u1DecId,
            MTAUDDEC_SIF_STANDARD_T eSIFMode,
            MTAUDDEC_SIF_STANDARD_T* peSIFMode);


    /**Get the analogy TV sound system of current channel.
    *@param peATVSoundSys              Sound system
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetAtvSoundSystem (UINT8 u1DecId,
            MTAUDDEC_SIF_STANDARD_T *peATVSoundSys);

    /**Get the analogy TV sound system strength of current channel.
    *@param eATVSoundSys              Sound system
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetAtvSoundSystemStrength ( MTAUDDEC_SIF_STANDARD_T eATVSoundSys, UINT32 *peStrength);

    /**Setup PCM information.
    * Used in audio clip playback.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param prPcmSetting               PCM setting
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetPcmInfo (UINT8 u1DecId,
            MTAUDDEC_PCM_SETTING_T* prPcmSetting);

#if 1
    /**LoadAudioClip.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param u1bufSize                 u1bufSize
    *@param u1ClipBufferPointer   u1ClipBufferPointer
    *@param u1DecFormat            decoderformat
    *@param u1repeatNumber       repeat number

    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK            Fail.
    */
    EXTERN MT_RESULT_T	MTAUDDEC_LoadAudioClip(UINT8 u1DecId, UINT32 u1bufSize, const UINT8* u1ClipBufferPointer, MTAUDDEC_FMT_T u1DecFormat,UINT32 u1repeatNumber);

#else
    /**LoadAudioClip.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param u1bufSize                 u1bufSize
    *@param u1ClipBufferPointer   u1ClipBufferPointer
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK            Fail.
    */
//EXTERN MT_RESULT_T MTAUDDEC_LoadAudioClip(UINT8 u1DecId, UINT32 u1bufSize, UINT32 *u1ClipBufferPointer);

    /**PlayAudioClip.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param u1ReaptNumber         clip ReaptNumber
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK            Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_PlayAudioClip(UINT8 u1DecId, UINT32 u1ReaptNumber);
#endif

    /**Stop audio clip.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  _MTAUD_StopAudioClip (UINT8 u1DecId);

    /**DeleteAudioClip.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_DeleteAudioClip(void);

    /**Setup individual channel delay
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eChIndex                   Individual channel.
    *@param u2Delay                    Delay (0~300 ms with 1 ms/step).
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetChannelDelay (UINT8 u1DecId, MTAUDDEC_CH_T eChIndex,
            UINT16 u2Delay);

    /**Setup individual channel delay no uop
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eChIndex                   Individual channel.
    *@param u2Delay                    Delay (0~300 ms with 1 ms/step).
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetChannelDelayNoUop (UINT8 u1DecId, MTAUDDEC_CH_T eChIndex,
            UINT16 u2Delay);
    /**Get individual channel delay
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eChIndex                   Individual channel.
    *@param u2Delay                    Delay (0~300 ms with 1 ms/step).
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetChannelDelay (UINT8 u1DecId, MTAUDDEC_CH_T eChIndex,
            UINT16 *u2Delay);
    /**Setup individual channel volume.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eChIndex                   Individual channel.
    *@param u1Value                    0~100.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetChannelVolume (UINT8 u1DecId, MTAUDDEC_CH_T eChannel,
            UINT8 u1Value);



    /**Setup individual channel volume no uop.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eChIndex                   Individual channel.
    *@param u1Value                    0~100.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */

    EXTERN MT_RESULT_T  MTAUDDEC_SetChannelVolumeNoUop (UINT8 u1DecId, MTAUDDEC_CH_T eChannel,
            UINT8 u1Value);

    /**Setup individual channel volume.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eChIndex                   Individual channel.
    *@param i2Value                    Volume value in 0.25dB/step.
    *                                  Range: 80 ~ -512 (+20dB ~ -128dB)
    *                                  80   ->  +20   dB
    *                                  79   ->  +19.75dB
    *                                  78   ->  +19.5 dB
    *                                  77   ->  +19.25dB
    *                                  76   ->  +19   dB
    *                                   :        :
    *                                  0    ->  0     dB
    *                                  -1   ->  -0.25 dB
    *                                  -2   ->  -0.5  dB
    *                                  -3   ->  -0.75 dB
    *                                  -4   ->  -1    dB
    *                                   :       :
    *                                  -512 ->  -128  dB
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetChannelVolumeInDB (UINT8 u1DecId,
            MTAUDDEC_CH_T eChannel, INT16 i2Value);

    /**Mute individual channel
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eChannel                   Individual channel.
    *@param fgMute                     Mute or not.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetChannelMute (UINT8 u1DecId, MTAUDDEC_CH_T eChannel,
            BOOL fgMute);

    /**Get individual channel mute status
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eChannel                   Individual channel.
    *@param pfgMute                    Mute or not. (TRUE: Mute, FALSE: Un-Mute)
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetChannelMuteStatus (UINT8 u1DecId,
            MTAUDDEC_CH_T eChannel, BOOL * pfgMute);

    /**Get speaker out put mode
    *@param Speakeroutmode      LR/LL/RR/MIX
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetSpeakerOutMode (MTAUDDEC_SKP_OUTPUT_MODE_T *peSpeakeroutmode);

    /**Setup extra gain for individual channel.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eChannel                   Individual channel.
    *@param i4Value                    -256~96. (-128 ~ 48 dB with 0.5 dB/step)
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetChannelVolGain (UINT8 u1DecId, MTAUDDEC_CH_T eChannel,
            INT32 i4Value);

    /**Setup volume balance.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param u1Balance                  Balance value (0~100).
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetBalance (UINT8 u1DecId, UINT8 u1Balance);

    /**Setup balance value in dB.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param i2LeftDB                   Volume value in 0.25dB/step.
    *                                  Range: 0 ~ -512
    *                                  0    ->  0     dB
    *                                  -1   ->  -0.25 dB
    *                                  -2   ->  -0.5  dB
    *                                  -3   ->  -0.75 dB
    *                                  -4   ->  -1    dB
    *                                   :       :
    *                                  -512 ->  -128  dB
    *@param i2LeftDB                   Volume value in 0.25dB/step.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetBalanceInDB (UINT8 u1DecId, INT16 i2LeftDB,
            INT16 i2RightDB);

    /**Setup master volume.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param u1Volume                   Volume value (0~100).
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetMasterVolume (UINT8 u1DecId, UINT8 u1Volume);

    /**Get master volume.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param u1Volume                   Volume value (0~100).
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetMasterVolume (UINT8 u1DecId, UINT8 * u1Volume);

    /**Setup source volume.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    MT_RESULT_T  MTAUDDEC_SetSourceVolume (UINT8 u1DecId,  MTAUDDEC_DEC_STREAM_FROM_T InPutSrc);


    /**Setup master volume in dB.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param i2Volume                   Volume value in 0.25dB/step.
    *                                  Range: 0 ~ -512
    *                                  0    ->  0     dB
    *                                  -1   ->  -0.25 dB
    *                                  -2   ->  -0.5  dB
    *                                  -3   ->  -0.75 dB
    *                                  -4   ->  -1    dB
    *                                   :       :
    *                                  -512 ->  -128  dB
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetMasterVolumeInDB (UINT8 u1DecId, INT16 i2Volume);

    /**Setup master volume for bypass channel in dB.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param i2Volume                   Volume value in 0.25dB/step.
    *                                  Range: 0 ~ -512
    *                                  0    ->  0     dB
    *                                  -1   ->  -0.25 dB
    *                                  -2   ->  -0.5  dB
    *                                  -3   ->  -0.75 dB
    *                                  -4   ->  -1    dB
    *                                   :       :
    *                                  -512 ->  -128  dB
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetMasterVolumeForBypassChannelInDB (UINT8 u1DecId,
            INT16 i2Volume);

    /**Setup clipper threshold in dB.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eChIndex                   Individual channel.
    *@param i2Value                    Volume value in 0.25dB/step.
    *                                  Range: 0 ~ -512 (0dB ~ -128dB)
    *                                  0    ->  0     dB
    *                                  -1   ->  -0.25 dB
    *                                  -2   ->  -0.5  dB
    *                                  -3   ->  -0.75 dB
    *                                  -4   ->  -1    dB
    *                                   :       :
    *                                  -512 ->  -128  dB
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetClipperThresholdInDB (UINT8 u1DecId,
            MTAUDDEC_CH_T eChannel, INT16 i2Value);

    /**Setup pre-gain in dB.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param i2Value                    Volume value in 0.25dB/step.
    *                                  Range: 0 ~ -512 (0dB ~ -128dB)
    *                                  0    ->  0     dB
    *                                  -1   ->  -0.25 dB
    *                                  -2   ->  -0.5  dB
    *                                  -3   ->  -0.75 dB
    *                                  -4   ->  -1    dB
    *                                   :       :
    *                                  -512 ->  -128  dB
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetPreGainInDB (UINT8 u1DecId, INT16 i2Value);

    /**Setup Auto volume control (AVC).
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param fgEnable                   on/off.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetAvc(UINT8 u1DecId, BOOL fgEnable);

    /**Setup AVC parameters
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eAVCCfgType                Parameter type
    *@param i4Parameter                Parameter value
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_SetAvcCfg(UINT8 u1DecId,
                                          MTAUDDEC_AVC_CFG_TYPE_T eAVCCfgType, INT32 i4Parameter);

    /**Setup 2 band AVC parameters
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param u1Band                     Band number (0: Low band, 1: High band)
    *@param eAVCCfgType                Parameter type
    *@param i4Parameter                Parameter value
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_Set2BandAvcCfg(UINT8 u1DecId, UINT8 u1Band,
            MTAUDDEC_AVC_CFG_TYPE_T eAVCCfgType, INT32 i4Parameter);

    /**Setup BBE
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param fgEnable                   on/off.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetBBE(UINT8 u1DecId, BOOL fgEnable);


    /**Setup SRS
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param fgEnable                   on/off.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetSRS(UINT8 u1DecId, BOOL fgEnable);


    EXTERN MT_RESULT_T  MTAUDDEC_SetADChannelMode(UINT8 u1DecId, UINT16 u2Mode);

    EXTERN MT_RESULT_T  MTAUDDEC_ADEnable(UINT8 u1DecId, BOOL fgEnable);

    EXTERN MT_RESULT_T  MTAUDDEC_SetClearVoice(UINT8 u1DecId, BOOL fgEnable);

    EXTERN MT_RESULT_T  MTAUDDEC_SetCVParameter (UINT32 *u4Parameter, UINT8 u1Size);

    /**Setup virtural surround
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param fgEnable                   on/off.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetVirtualSurround(UINT8 u1DecId, BOOL fgEnable);


    /**Setup virtural surround parameters
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eVSCfgType                 Parameter type
    *@param u4Parameter                Parameter value
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_SetVirtualSurroundCfg(UINT8 u1DecId,
            MTAUDDEC_VSURR_CFG_TYPE_T eVSCfgType, UINT32 u4Parameter);

    /**Setup bass boost.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param fgEnable                   on/off.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetBassBoost (UINT8 u1DecId, BOOL fgEnable);

    /**Setup bass
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param i2Bass                     bass value  ( +-14 steps, with 0.5 dB/step)
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetBass (UINT8 u1DecId, INT16 i2Bass);


    /**Setup treble boost.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param u2CompMode                 Compression mode.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetTrebleBoost(UINT8 u1DecId, BOOL fgEnable);

    /**Setup Treble
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param i2Treble                   Treble value ( +-14 steps, with 0.5 dB/step)
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetTreble (UINT8 u1DecId, INT16 i2Treble);

    /**Setup EQ.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param fgEnable                   on/off.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetEqualizer(UINT8 u1DecId, BOOL fgEnable);

    /**Setup EQ band configuration.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param fgEnable                   on/off.
    *@param i2Band0                    band 0 setup ( +-28, with 0.5 dB/step)
    *@param i2Band1                    band 1 setup ( +-28, with 0.5 dB/step)
    *@param i2Band2                    band 2 setup ( +-28, with 0.5 dB/step)
    *@param i2Band3                    band 3 setup ( +-28, with 0.5 dB/step)
    *@param i2Band4                    band 4 setup ( +-28, with 0.5 dB/step)
    *@param i2Band5                    band 5 setup ( +-28, with 0.5 dB/step)
    *@param i2Band6                    band 6 setup ( +-28, with 0.5 dB/step)
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetEqualizerBandConfig(UINT8 u1DecId, INT16 i2Band0,
            INT16 i2Band1, INT16 i2Band2, INT16 i2Band3,
            INT16 i2Band4, INT16 i2Band5, INT16 i2Band6);

    /**Setup line-in input mux
    *@param  eInputId                  Input mux id.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetAvInputMuxSel (MTAUDDEC_INPUT_ID_T eInputId);

    /**Setup line-in input mux by ADC
    *@param  eADNum                 Input mux id.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetAvInputMuxByADC(UINT8 eADNum);

    /**Setup IEC configuration.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eIecCfg                    IEC format.
    *@param fgEnable                   on/off.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetIecConfig (UINT8 u1DecId, MTAUDDEC_IEC_T eIecCfg,
            BOOL fgEnable);

    /**Get IEC configuration.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param peIecCfg                   IEC format.
    *@param pfgEnable                  on/off. (TRUE: ON, FALSE: OFF)
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetIecConfig (UINT8 u1DecId, MTAUDDEC_IEC_T * peIecCfg,
            BOOL * pfgEnable);

    /**Setup IEC refer to which channel.
    *@param eIecChannel                Channel number.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetIecChannel (MTAUDDEC_PCM_CH_T eChannel);

    /**Setup IEC copyright.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param fgEnable                   on/off.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetIecCopyright(UINT8 u1DecId, BOOL fgEnable);

    /**Setup SPDIF scms
    *@param spdif_ctrl                  spdif control
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetSpdifScms(UINT8 spdif_ctrl);

    /**Get IEC copyright.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param pfgEnable                  on/off.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetIecCopyright(UINT8 u1DecId, BOOL * pfgEnable);


    /**Setup IEC category code
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param u1CategoryCode             Category code.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetIecCategoryCode(UINT8 u1DecId, UINT8 u1CategoryCode);

    /**Get IEC category code
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param pu1CategoryCode            Category code.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetIecCategoryCode(UINT8 u1DecId, UINT8 * pu1CategoryCode);

    /**Setup IEC category code
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param u1WordLength               Word length.(in bits, range: 16/24)
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetIecWordLength(UINT8 u1DecId, UINT8 u1WordLength);

    /**Get IEC category code
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param pu1WordLength              Word length.(in bits, range: 16/24)
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_GetIecWordLength(UINT8 u1DecId, UINT8 * pu1WordLength);



    /**Setup Speaker output mode.
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eOutputMode                L+R = 0,
    *                                  L+L,
    *                                  R+R,
    *                                  MIX
    *
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetSpeakerOutMode (UINT8 u1DecId,
            MTAUDDEC_SKP_OUTPUT_MODE_T eOutputMode);

    /**Setup bass enhancer switch
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param fgEnable                   on/off.
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T  MTAUDDEC_SetBassEnhancer (UINT8 u1DecId, BOOL fgEnable);

    /**Setup bass enhancer parameers
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param eBassEnhancerType          Parameter type
    *@param u4Parameter                Parameter value
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_SetBassEnhancerCfg(UINT8 u1DecId,
            MTAUDDEC_BE_PARAM_TYPE_T eBassEnhancerType, UINT32 u4Parameter);


    /**Setup PEQ switch on/off
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param fgEnable                   Enable
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_SetPEQ(UINT8 u1DecId, BOOL fgEnable);


    /**Setup PEQ parameters
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param u1Set                      PEQ set (0 ~ 7)
    *@param u4Frequency                Frequency
    *@param eQValue                    Q
    *@param i2Gain                     Gain (+-12dB) with 0.5dB/step
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_SetPEQCfg(UINT8 u1DecId, UINT8 u1Set,
                                          UINT32 u4Frequency, MTAUDDEC_PEQ_Q_TYPE_T eQValue, INT16 i2Gain);


    /**Setup Limiter switch on/off
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param fgEnable                   Enable
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_SetLimiter(UINT8 u1DecId, BOOL fgEnable);


    /**Setup Limiter configuration
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param u1Mode                     limiter mode
    *                                  (0: off, 1: adaptive mode, 2: fixed mode).
    *@param u1Flag                     limiter flag (0: off, 1: on)
    *                                  bit 0: vol
    *                                  bit 1: vsurr
    *                                  bit 2: sbass
    *                                  bit 3: EQ
    *                                  bit 4: reverb
    *                                  bit 5: virtual bass
    *@param  u4Thresh                  limiter threshold (0x0 ~ 0x7fffff)
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_SetLimiterCfg(UINT8 u1DecId, UINT8 u1Mode,
            UINT16 u2Flag, UINT32 u4Thresh);


    /**Setup HPF Fc switch
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param fgEnable                   On/Off
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_SetHPF(UINT8 u1DecId, BOOL fgEnable);


    /**Setup HPF Fc
    *@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
    *@param u2Fc                       Frequency (50 ~ 300 Hz)
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_SetHPFFc(UINT8 u1DecId, UINT16 u2Fc);


    /**Mixound Play
    *@param u4addr
    *@param u4len
    *@param u4gain                     0x20000 means 0dB
    *@param u2config                   bit  0:3  samplerate  0: 48k   1: 44.1k 2: 32k 3: 24k 4:16k 5:12k
    *                                            bit  4: source mode : 0->mono, 1->stereo
    *                                            bit  5: 0: 8bit, 1 16bits
    *                                            bit  6: 0: bigendian  1:littleendian
    *                                            bit  7: 0: no repeat   1:repeat
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_MixSoundPlay(UINT32 u4Addr, UINT32 u4Len, UINT32 u4Gain, UINT16 u2Config);

    /**Mixsound Stop
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_MixSoundStop(void);

    /**Mixsound Pause
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_MixSoundPause(void);

    /**Mixsound Resume
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_MixSoundResume(void);

	/**set DSP speed
    *@param u4Mode                   step is 0.1, 10: normal speed; 5: 0.5X speed ; 20: 2X speed
    *@retval MTR_OK                    Success.
    *@retval MTR_NOT_OK                Fail.
    */
    EXTERN MT_RESULT_T MTAUDDEC_SetDspSpeedMode(UINT32 u1Mode);

    EXTERN MT_RESULT_T MTAUDDEC_RecordPCM(UINT32 u4ClipBufferPointer, UINT32 u4BufSize);
    EXTERN MT_RESULT_T MTAUDDEC_DownloadDram(UINT32 u4BufPtr, UINT32 u4Addr, UINT32 u4Size);
    EXTERN MT_RESULT_T MTAUDDEC_UploadDram(UINT32 u4BufPtr, UINT32 u4Addr, UINT32 u4Size);
    
	/**Set the S/PDIF Serial Copying Management System (SCMS) output bits.
	  *@param c_bit 				  bit2
	  *@param l_bit 				   bit15
	  *@retval MTR_OK				Success.
	  *@retval MTR_NOT_OK		Fail.
	  */
	  EXTERN MT_RESULT_T MTAUDDEC_SetSCMS(BOOL c_bit, BOOL l_bit);
#ifdef __cplusplus
}
#endif

#endif //MT_AUDDEC_H
