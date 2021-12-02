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
 * $Date: 2012/04/27 $
 * $RCSfile: aud_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_if.h
 * Declare audio related structure and function table.
 */

#ifndef X_AUD_IF_H
#define X_AUD_IF_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "dmx_if.h"
#include "feeder_if.h"
#include "sif_if.h"

//-----------------------------------------------------------------------------
// Audio table enum and structure
//-----------------------------------------------------------------------------


#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
   #define AUD_DEC_NUM     ((UINT8)2)
#else
    #define AUD_DEC_NUM     ((UINT8)3)
#endif

// Decoder ID check macro
#define AUD_DEC_ID_VALIDATE(u1DecId)    VERIFY(u1DecId < AUD_DEC_NUM)

#define AUD_AOUT_NO     ((UINT8)2)

#define UNUSED_GPIO     (0xffffffff)
#define SW_NO_USED      (0xff)
#define NO_USED         (0xff)

#define MUX_SW_0        1, 1
#define MUX_SW_1        0, 1
#define MUX_SW_2        1, 0
#define MUX_SW_3        0, 0

#define AUD_OLDFIFO_INTF

#define DEFAULT_0_DB    0x20000

#ifdef CC_AUD_7BAND_EQUALIZER
#define CH_EQ_BAND_NO   7
#define AUD_EQ_IIR_BAND_NUM     7
#else
#define CH_EQ_BAND_NO   5
#define AUD_EQ_IIR_BAND_NUM     8
#endif

//#define CC_AUD_SOUND_MODE
#define PEQ_SETTING_MAX 9

#ifdef CC_AUD_EQ_SETA
#define EQ_BAND_GAIN_DB_MAX             10
#else
#define EQ_BAND_GAIN_DB_MAX             14
#endif

#ifdef CC_AUD_3_5K_TREBLE
// Bass/Treble constant
#define BASS_TREBLE_GAIN_DB_MAX         14
// Bass Treble
#define BASS_TREBLE_GAIN_MAX            ((BASS_TREBLE_GAIN_DB_MAX*4) + 1) // range -14 ~ +14 dB, 0.5dB/Step
#else
// Bass/Treble constant
#define BASS_TREBLE_GAIN_DB_MAX         14
// Bass Treble
#define BASS_TREBLE_GAIN_MAX ( (14*2) + 1) // range -14 ~ +14 dB, 1 dB/Step
#endif

#define BBE_LEVEL_MAX                   5
#define BBE_PROCESS_MIN                 (3*2)
#define BBE_PROCESS_MAX                 (12*2)
#define BBE_LO_CONTOUR_MIN              (3*2)
#define BBE_LO_CONTOUR_MAX              (12*2)
#define BBE_3D_GAIN_MAX                 100

#define AUD_VOLUME_OFFSET_IDX_NUM 13
// Copyright constant
#define IEC_COPY_FREELY                 (0x00)
#define IEC_COPY_NO_MORE                (0x01)
#define IEC_COPY_ONCE                   (0x02)
#define IEC_COPY_NEVER                  (0x03)

// IEC category code
#define IEC_ORIGINAL_GENERAL            (0x00)
#define IEC_ORIGINAL_BROAD_CAST         (0x64)
#define IEC_NON_ORIGINAL_BROAD_CAST     (0xe4)
#define IEC_ORIGINAL_BROAD_CAST_EU      (0x0c)
#define IEC_CH_STATUS_BY_SOURCE         (0xff)

// IEC word length (defined in register map order)
#define IEC_WORD_LENGTH_24BIT           (0 << 0)
#define IEC_WORD_LENGTH_20BIT           (2 << 0)
#define IEC_WORD_LENGTH_16BIT           (3 << 0)

// PROM ID
#define DspPROMTstAddr   0x39           // PROM test address
#define DspPROMRdAddr    0x3A           // PROM read port address
#define DspPROMMsk       0xFFFF         // PROM intruction mask
#define DspPROMEnTst     0x100000       // PROM test enable
#define DspPROMVerOffset 0x78           // PROM ID start address
// DROM ID
#define DspDROMRdAddr    0x4000         // DROM read port address
#define DspDROMMsk       0xFFFFFF       // DROM data mask
#define DspDROMVerOffset 0x0            // DROM ID start address

#define DSP_WMA_MWIF
#define CC_SET_VOLUME
//#define CC_AUD_NEW_CV_TABLE //xiaoyi

// Downmix channel position
#define DOWNMIX_CH_DO_ALL_POST_PROC     ((UINT8)0x00)
#define DOWNMIX_CH_BEFORE_AVC           ((UINT8)0x01)
#define DOWNMIX_CH_BYPASS_POST_PROC     ((UINT8)0x03)
/*For Power ON/OFF music flag in EEP*/
#define AUD_POWER_ON_MUSIC_MASK            (0x1 << 0)
#define AUD_POWER_OFF_MUSIC_MASK           (0x1 << 1)

typedef enum
{
    FORMAT_RJ,
    FORMAT_LJ,  
    FORMAT_I2S
}   DATA_FORMAT_T;

typedef enum
{
    MCLK_128FS,
    MCLK_192FS,
    MCLK_256FS,
    MCLK_384FS,
    MCLK_512FS,
    MCLK_768FS
}   MCLK_FREQUENCY_T;

typedef enum
{ 
    LRCK_CYC_16,
    LRCK_CYC_24,
    LRCK_CYC_32
}   LRCK_CYC_T;

typedef enum
{
    DAC_16_BIT,
    DAC_18_BIT,
    DAC_20_BIT,
    DAC_24_BIT
}   DAC_DATA_NUMBER_T;

typedef enum
{
    AUD_STREAM_FROM_OTHERS = 0,         ///< ex. Pink Noise Generator
    AUD_STREAM_FROM_DIGITAL_TUNER,      ///< Digital tuner. TS
    AUD_STREAM_FROM_ANALOG_TUNER,       ///< ex. Analog tuner. SIF/AF
    AUD_STREAM_FROM_SPDIF,              ///< SPDIF-in
    AUD_STREAM_FROM_LINE_IN,            ///< Line in
    AUD_STREAM_FROM_HDMI,               ///< HDMI in
    AUD_STREAM_FROM_MEMORY,             ///< Memory
    AUD_STREAM_FROM_BUF_AGT,            ///< Buffer agent
    AUD_STREAM_FROM_FEEDER,             ///< Feeder    
    AUD_STREAM_FROM_MULTI_MEDIA,    /// < Multi-Media
    AUD_STREAM_FROM_NUM                 ///< Number of stream source
}   AUD_DEC_STREAM_FROM_T;

typedef struct 
{
    DATA_FORMAT_T      eFormat;         ///< format of alignment
    DAC_DATA_NUMBER_T  eBits;           ///< number of bits per sample
    LRCK_CYC_T         eCycle;          ///< cycles per sample
    MCLK_FREQUENCY_T   eSampleFreq;     ///< DAC sampling frequence
    BOOL               fgDataInvert;    ///< Invert audio output for OP phase
    BOOL               fgLRInvert;      ///< Invert L/R audio output
}   AOUT_CFG_T;

typedef enum
{
    AUD_INPUT_ID_COMP_VID_0 = 0,
    AUD_INPUT_ID_COMP_VID_1,
    AUD_INPUT_ID_COMP_VID_2,
    AUD_INPUT_ID_COMP_VID_3,
    AUD_INPUT_ID_S_VID_0,
    AUD_INPUT_ID_S_VID_1,
    AUD_INPUT_ID_S_VID_2,
    AUD_INPUT_ID_YPBPR_0,
    AUD_INPUT_ID_YPBPR_1,
    AUD_INPUT_ID_YPBPR_2,
    AUD_INPUT_ID_VGA_0,
    AUD_INPUT_ID_VGA_1,
    AUD_INPUT_ID_HDMI_0,
    AUD_INPUT_ID_HDMI_1,
    AUD_INPUT_ID_HDMI_2,
    AUD_INPUT_ID_HDMI_3,
    AUD_INPUT_ID_HDMI_4,
    AUD_INPUT_ID_DVI_0,
    AUD_INPUT_ID_DVI_1,
    AUD_INPUT_ID_DVI_2,
    AUD_INPUT_ID_DVI_3,
    AUD_INPUT_ID_DVI_4,
    AUD_INPUT_ID_SCART_0,
    AUD_INPUT_ID_SCART_1,
    AUD_INPUT_ID_SCART_2, 
    AUD_INPUT_ID_SCART_3,
    AUD_INPUT_ID_AUXIN_0,
    AUD_INPUT_ID_AUXIN_1,
    AUD_INPUT_ID_MAX
}AUD_INPUT_ID_T;

typedef enum
{
    AUD_CHL_L_R = 0,
    AUD_CHL_LS_RS,
    AUD_CHL_C_SW,
    AUD_CHL_BYPASS,
    AUD_CHL_DOWNMIX,  
    AUD_CHL_AUX
}   AUD_CHL_DEF_T;

typedef void (*CODEC_INIT)(void);
typedef void (*CODEC_MUTE)(UINT8 u1DacId,BOOL fgMute);
typedef void (*CODEC_SET_DAC_IN_FMT)(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
typedef void (*CODEC_SET_ADC_OUT_FMT)(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
typedef void (*CODEC_SET_DAC_GAIN)(UINT8 u1DacId, INT16 i2Gain);
typedef void (*CODEC_GET_DAC_GAIN)(UINT8 u1DacId, INT16* pi2Gain);
typedef void (*CODEC_SET_ADC_GAIN)(UINT8 u1DacId, INT16 i2Gain);
typedef void (*CODEC_GET_ADC_GAIN)(UINT8 u1DacId, INT16* pi2Gain);
typedef void (*CODEC_SET_HP_GAIN)(UINT8 u1HpId, INT16 i2Gain);
typedef void (*CODEC_GET_HP_GAIN)(UINT8 u1HpId, INT16* i2Gain);
typedef void (*CODEC_SET_ADC_MUX_SEL)(UINT8 u1Sel);
typedef void (*CODEC_REG_WRITE)(UINT8 u1Offset, UINT8 u1Data);
typedef UINT8 (*CODEC_REG_READ)(UINT8 u1Offset);

typedef void (*ADAC_INIT)(void);
typedef void (*ADAC_MUTE)(BOOL fgMute);
typedef void (*ADAC_ENABLE)(BOOL fgEnable);
typedef void (*ADAC_CHL_SEL)(UINT8 u1DacId, UINT8 eChl);
typedef void (*ADAC_SET_DAC_IN_FMT)(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
typedef void (*ADAC_SET_DAC_GAIN)(UINT8 u1HpId, INT16 i2Gain);
typedef void (*ADAC_REG_WRITE)(UINT8 u1Offset, UINT8 u1Data);
typedef UINT8 (*ADAC_REG_READ)(UINT8 u1Offset);

typedef void (*AMP_INIT)(void);
typedef void (*AMP_MUTE)(BOOL fgMute);
typedef void (*AMP_SET_DAC_IN_FMT)(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
typedef void (*AMP_SET_DAC_GAIN)(UINT8 u1HpId, INT16 i2Gain);
typedef void (*AMP_REG_WRITE)(UINT8 u1Offset, UINT8 u1Data);
typedef UINT8 (*AMP_REG_READ)(UINT8 u1Offset);

/// Audio codec function table.
typedef struct
{
    CODEC_INIT             pfnInit;
    CODEC_MUTE             pfnMute;
    CODEC_SET_DAC_IN_FMT   pfnSetDacInFmt;
    CODEC_SET_ADC_OUT_FMT  pfnSetAdcOutFmt;
    CODEC_SET_DAC_GAIN     pfnSetDacGain;
    CODEC_GET_DAC_GAIN     pfnGetDacGain;
    CODEC_SET_ADC_GAIN     pfnSetAdcGain;
    CODEC_GET_ADC_GAIN     pfnGetAdcGain;
    CODEC_SET_HP_GAIN      pfnSetHpGain;
    CODEC_GET_HP_GAIN      pfnGetHpGain;
    CODEC_SET_ADC_MUX_SEL  pfnSetAdcMuxSel;
    CODEC_REG_WRITE        pfnRegWrite;
    CODEC_REG_READ         pfnRegRead;
} CODEC_FUNCTBL_T;

/// Codec configuration table.
typedef struct
{
    CHAR             szName[32];
    INT16            i2MaxHpGain;
    INT16            i2MinHpGain;
    INT16            i2MaxDacGain;
    INT16            i2MinDacGain;
    UINT8            u1AdcMuxNum;
    UINT8            u1DacNum;
    UINT8            u1HpNum;
    CODEC_FUNCTBL_T* prCodecFuncTbl;
} CODEC_T;

/// Internal dac function table.
typedef struct 
{
    ADAC_INIT           pfnInit;
    ADAC_MUTE           pfnMute;
    ADAC_ENABLE         pfnEnable;
    ADAC_CHL_SEL        pfnChlSelect;
    ADAC_SET_DAC_IN_FMT pfnSetDacInFmt;
    ADAC_SET_DAC_GAIN   pfnSetDacGain;
    ADAC_REG_WRITE      pfnRegWrite;
    ADAC_REG_READ       pfnRegRead;
} ADAC_FUNCTBL_T;

/// Internal dac configuration table.
typedef struct
{
    CHAR            szName[32];
    INT16           i2MaxGain;
    INT16           i2MinGain;
    UINT8           u1DacNum;
    BOOL            fInterDac;
    ADAC_FUNCTBL_T* prDacFuncTbl;
} ADAC_T;

/// Audio amplify function table.
typedef struct 
{
    AMP_INIT           pfnInit;
    AMP_MUTE           pfnMute;
    AMP_SET_DAC_IN_FMT pfnSetDacInFmt;
    AMP_SET_DAC_GAIN   pfnSetDacGain;
    AMP_REG_WRITE      pfnRegWrite;
    AMP_REG_READ       pfnRegRead;
} AMP_FUNCTBL_T;

/// Audio amplify configuration table.
typedef struct
{
    CHAR            szName[32];
    INT16           i2MaxGain;
    INT16           i2MinGain;
    UINT8           u1DacNum;
    AMP_FUNCTBL_T* prAmpFunc;
} AMP_T;

/// Audio codec select table.
typedef enum
{
    AUD_CODEC_MT8291 = 0,
    AUD_CODEC_WM8776,
    AUD_CODEC_MT538xInternal,
    AUD_CODEC_MT536xInternal,
    AUD_CODEC_MT539xInternal,
    AUD_CODEC_MT5387,
    AUD_CODEC_MT8292,
    AUD_CODEC_OTHERS,
    AUD_CODEC_NULL
} AUD_CODEC_T;

/// Audio codec select table2.
typedef enum
{
    AUD_ADAC_MT537X_INTERNAL = 0,
    AUD_ADAC_WM8766,
    AUD_ADAC_CSXXX,
    AUD_ADAC_OTHERS,
    AUD_ADAC_NULL
} AUD_ADAC_T;

/// Audio channel table.
typedef enum
{
    AUD_CH_FRONT_LEFT = 0,
    AUD_CH_FRONT_RIGHT,
    AUD_CH_REAR_LEFT,
    AUD_CH_REAR_RIGHT,
    AUD_CH_CENTER,
    AUD_CH_SUB_WOOFER,
    AUD_CH_BYPASS_LEFT,
    AUD_CH_BYPASS_RIGHT,
    AUD_CH_DMX_LEFT,
    AUD_CH_DMX_RIGHT,
    AUD_CH_AUX_FRONT_LEFT,
    AUD_CH_AUX_FRONT_RIGHT,
    AUD_CH_INPUT_SRC,                   ///< For input source volume control
    AUD_CH_ALL,                         ///< For master volume control
    AUD_CH_LFE,
    AUD_CH_NUM
}   AUD_CH_T;

/// Audio digital amplify table.
typedef enum
{
    AUD_AMP_APOGEE_DDX_2051 = 0,
    AUD_AMP_AD82581,
    AUD_AMP_WOLFSON_XXX,
    AUD_AMP_PHILIPS_XXX,
    AUD_AMP_OTHERS,
    AUD_AMP_AD8356,
    AUD_AMP_NULL
} AUD_AMP_T;

typedef enum
{
    AUD_AMP_ID_LR = 0,
    AUD_AMP_ID_LsRs,
    AUD_AMP_ID_CSw,
    AUD_AMP_ID_NUM
} AUD_AMP_ID_T;

/// Audio opctrl polarity table.
typedef enum
{
    AUD_OPCTRL_MUTE_GPIO_LOW_ENALBE = 0,
    AUD_OPCTRL_MUTE_GPIO_HIGH_ENALBE = 1
}   AUD_OPCTRL_MUTE_GPIO_POLARITY_T;


/// Audio codec mutel polarity table.
typedef enum
{
    AUD_MUTE_GPIO_LOW_ENALBE = 0,
    AUD_MUTE_GPIO_HIGH_ENALBE = 1
}   AUD_MUTE_GPIO_POLARITY_T;

/// Audio amplify enable polarity table.
typedef enum
{
    AUD_AMP_GPIO_LOW_ENALBE = 0,
    AUD_AMP_GPIO_HIGH_ENALBE = 1
}   AUD_AMP_GPIO_POLARITY_T;

/// Audio gpio enable polarity table.
typedef enum
{
    AUD_GPIO_LOW_ENALBE = 0,
    AUD_GPIO_HIGH_ENALBE = 1
}   AUD_GPIO_POLARITY_T;

/// Audio DVD gpio enable polarity table.
typedef enum
{
    AUD_DVD_GPIO_LOW_ENALBE = 0,
    AUD_DVD_GPIO_HIGH_ENALBE = 1
}   AUD_DVD_GPIO_POLARITY_T;

/// Audio EQ table.
typedef enum
{
    AUD_EQ_OFF = 0,
    AUD_EQ_ROCK,
    AUD_EQ_POP,
    AUD_EQ_LIVE,
    AUD_EQ_DANCE,
    AUD_EQ_TECHNO,
    AUD_EQ_CLASSIC,
    AUD_EQ_SOFT,
    AUD_EQ_CUST1,
    AUD_EQ_CUST2,
    AUD_EQ_CUST3,
    AUD_EQ_CUST4,
    AUD_EQ_CUST5,
    AUD_EQ_CUST6,
    AUD_EQ_CUST7,
    AUD_EQ_CUST8,    
    AUD_EQ_BBE_TV,
    AUD_EQ_BBE_AV,
    AUD_EQ_SBASS,
    AUD_EQ_NUM
}   AUD_EQ_TYPE_T;

/// Audio limiter flag table.
typedef enum
{
    AUD_LIMITER_VOL = 0,
    AUD_LIMITER_VSURR,
    AUD_LIMITER_SBASS,
    AUD_LIMITER_EQ,
    AUD_LIMITER_REVERB,
    AUD_LIMITER_SPEAKER,
    AUD_LIMITER_VOICE,
    AUD_LIMITER_NUM,
} AUD_LIMITER_FLAG_T;

/// Audio input mux select table.
typedef struct
{
    AUD_INPUT_ID_T  eAudInputId;
    UINT8           u1AdcMuxSel;        ///< ADC mux select, ex. WM8776(0~5)
    UINT8           u1SwGpioNum1Val;    ///< 0: Low, 1: High
    UINT8           u1SwGpioNum2Val;    ///< 0: Low, 1: High
    UINT8           u1SwGpioNum3Val;    ///< 0: Low, 1: High
    UINT8           u1SwGpioNum4Val;    ///< 0: Low, 1: High
} AUD_INPUT_MUX_SEL_T;

/// Audio input mux select table with size table.
typedef struct
{
    UINT32                     u4Size;
    const AUD_INPUT_MUX_SEL_T* prAudInMuxSel;
} AUD_INPUT_MUX_SEL_TABLE_T;

/// Audio input mux select switch table.
typedef struct
{
    UINT32      u4AudInSwGpioNum1;      ///< ex.GPIO 17
    UINT32      u4AudInSwGpioNum2;      ///< ex.GPIO 15
    UINT32      u4AudInSwGpioNum3;      ///< ex.GPIO xx
    UINT32      u4AudInSwGpioNum4;      ///< ex.GPIO yy
}   AUD_INPUT_SW_GPIO_T;

/// Audio dac volume table.
typedef struct
{
    UINT32      u4LineOutVol;
    UINT32      u4HeadphoneVol;
    UINT32      u4SpeakerVol;
} AUD_ADAC_VOL_CFG_T;

/// Audio dac usage table.
typedef struct
{
    AUD_ADAC_T  eAdac1;
    AUD_ADAC_T  eAdac2;
    AUD_ADAC_T  eAdac3;
    AUD_ADAC_T  eAdac4;
    AUD_CODEC_T eCodec1;
    AUD_CODEC_T eCodec2;
    AUD_AMP_T   eAudAmp1;
    AUD_AMP_T   eAudAmp2;
} AUD_ADAC_USAGE_T;

/// Audio aout PAD config table.
typedef struct
{
    AUD_CH_T    ePadSdata0;
    AUD_CH_T    ePadSdata1;
    AUD_CH_T    ePadSdata2;
    AUD_CH_T    ePadSdata3;
    AUD_CH_T    ePadSdata4;
    AUD_CH_T    ePadSdata5;
} AUD_AOUT_PAD_CFG_T;

/// Audio internal dac channel config table.
typedef struct
{
    AUD_CHL_DEF_T eDac0ChlSel;
    AUD_CHL_DEF_T eDac1ChlSel;
    AUD_CHL_DEF_T eDac2ChlSel;
} AUD_INTER_DAC_CFG_T;

typedef struct
{
    UINT32 u4Size;
    const UINT8* prAudSetupMelody;
} AUD_SETUP_MELODY_TABLE_T;


/// Audio ATV decoder type table.
typedef enum
{
    AUD_ATV_DECODER_PAL,
    AUD_ATV_DECODER_A2,
    AUD_ATV_DECODER_MTS,
    AUD_ATV_DECODER_FMFM,    
    AUD_ATV_DECODER_NUM
}AUD_ATV_DECODER_TYPE_T;

/// Audio factory MTS detection item table.
typedef enum
{
    AUD_FACTORY_MTS_NUM_CHECK,          ///< numbers of check
    AUD_FACTORY_MTS_NUM_PILOT,          ///< numbers of pilot
    AUD_FACTORY_MTS_PILOT_HIGH,         ///< pilot higher threshold
    AUD_FACTORY_MTS_PILOT_LOW,          ///< pilot lower threshold
    AUD_FACTORY_MTS_NUM_SAP,            ///< numbers of sap
    AUD_FACTORY_MTS_SAP_HIGH,           ///< sap higher threshold
    AUD_FACTORY_MTS_SAP_LOW             ///< sap lower threshold
} AUD_FACTORY_MTS_DETECTION_ITEM_T;

/// Audio factory FM mute table.
typedef enum
{
    AUD_FACTORY_FM_MUTE,                ///< carrier sense mute mode on/off
    AUD_FACTORY_FM_MUTE_HIGH,           ///< higher carrier sense mute threshold
    AUD_FACTORY_FM_MUTE_LOW             ///< lower carrier sense mute threshold
} AUD_FACTORY_FM_MUTE_ITEM_T;

/// Audio factory A2 detection table.
typedef enum
{
    AUD_FACTORY_A2_NUM_CHECK,           ///< numbers of check
    AUD_FACTORY_A2_NUM_DOUBLE,          ///< numbers of double check
    AUD_FACTORY_A2_MONO_WEIGHT,         ///< mono wieght
    AUD_FACTORY_A2_STEREO_WEIGHT,       ///< stereo wieght
    AUD_FACTORY_A2_DUAL_WEIGHT          ///< dual wieght
} AUD_FACTORY_A2_DETECTION_ITEM_T;

/// Audio factory A2 threshold table.
typedef enum
{
    AUD_FACTORY_A2_MONO2STEREO,           
    AUD_FACTORY_A2_STEREO2MONO,       
} AUD_FACTORY_A2_THRESHOLD_ITEM_T;

/// Audio factory PAL detection table.
typedef enum
{
    AUD_FACTORY_PAL_CORRECT,            ///< correct threshold
    AUD_FACTORY_PAL_SYNC_LOOP,          ///< total sync loop
    AUD_FACTORY_PAL_ERROR,              ///< error threshold
    AUD_FACTORY_PAL_PARITY_ERROR,       ///< parity error threshold
    AUD_FACTORY_PAL_NUM_FRAMES          ///< every number frames
} AUD_FACTORY_PAL_DETECTION_ITEM_T;

/// Audio factory PAL_AM mute table.
typedef enum
{
    AUD_FACTORY_PAL_AM_MUTE,            ///< AM mute mode on/off
    AUD_FACTORY_PAL_AM_MUTE_HIGH,       ///< higher AM mute threshold
    AUD_FACTORY_PAL_AM_MUTE_LOW         ///< lower AM mute threshold
} AUD_FACTORY_PAL_AM_MUTE_ITEM_T;

/// Audio factory FM_FM detection table.
typedef enum
{
    AUD_FACTORY_FMFM_NUM_CHECK,         ///< numbers of check
    AUD_FACTORY_FMFM_NUM_DOUBLE,        ///< numbers of double check
    AUD_FACTORY_FMFM_MONO_WEIGHT,       ///< mono wieght
    AUD_FACTORY_FMFM_STEREO_WEIGHT,     ///< stereo wieght
    AUD_FACTORY_FMFM_DUAL_WEIGHT,       ///< dual wieght
    AUD_FACTORY_FMFM_DETECT_CONFID      
} AUD_FACTORY_FMFM_DETECTION_ITEM_T;

typedef enum
{
    AUD_FACTORY_SAP_MUTE_LOWER,         /* higher AM mute threshold */
    AUD_FACTORY_SAP_MUTE_HIGHER,         /* lower AM mute threshold */
    AUD_FACTORY_SAP_HP_MUTE_HIGHER,  /* higher SAP HPF mute threshold */
    AUD_FACTORY_SAP_HP_MUTE_LOWER,   /* higher SAP HPF mute threshold */
    AUD_FACTORY_SAP_FILTER_SEL,             /* SAP filter selection */
    AUD_FACTORY_SAP_RATIO                      /* SAP SNR  */
} AUD_FACTORY_SAP_MUTE_ITEM_T;

typedef enum
{
    AUD_FACTORY_SATU_MUTE_LOWER,         /* higher satutaion mute threshold */
    AUD_FACTORY_SATU_MUTE_HIGHER         /* lower satutaion mute threshold */
} AUD_FACTORY_SATU_MUTE_ITEM_T;

/* BUF_AGT_SET_TYPE_NFY_FCT */
typedef enum
{
    AUD_BUF_AGT_NFY_COND_UNKNOWN = 0,
    AUD_BUF_AGT_NFY_COND_DATA_REQ_DONE,
    AUD_BUF_AGT_NFY_COND_DATA_REQ_ERROR,
    AUD_BUF_AGT_NFY_COND_DATA_REQ_CANCELED,
    AUD_BUF_AGT_NFY_COND_FLUSH_DONE,
    AUD_BUF_AGT_NFY_COND_ALIGN_INFO,
    AUD_BUF_AGT_NFY_COND_NONE    // for buffer agent thread
} AUD_BUF_AGT_NFY_COND_T;

typedef enum
{
    AUD_SM_CASE0 = 0,
    AUD_SM_CASE1,
    AUD_SM_CASE2,
    AUD_SM_CASE3,
    AUD_SM_CASE4,
    AUD_SM_CASE5,
    AUD_SM_CASE6,
    AUD_SM_CASE7,
    AUD_SM_CASE8,
    AUD_SM_CASE9,
    AUD_SM_CASE10,
    AUD_SM_CASE11,
    AUD_SM_CASE12,
    AUD_SM_CASE13,
    AUD_SM_CASE14,
    AUD_SM_CASE15,
    AUD_SM_NUM
}   AUD_POST_SM_T;
typedef enum
{
    AUD_SOUND_MODE_DEFAULT = 0,
    AUD_SOUND_MODE_STANDARD ,
    AUD_SOUND_MODE_DYNAMIC,
    AUD_SOUND_MODE_CLEAR_VOICE,
    AUD_SOUND_MODE_NUM
}   AUD_US_SOUND_MODE_T;

typedef enum
{
    AUD_VS_CASE1 = 0,
    AUD_VS_CASE2,
    AUD_VS_CASE3,
    AUD_VS_CASE4,
    AUD_VS_CASE5,
    AUD_VS_NUM
}   AUD_VS_MODE_T;

/* BUF_AGT_GET_TYPE_SRC_ALIGN_INFO */
typedef struct 
{
    UINT32     ui4_align;
    UINT32     ui4_min_blk_size;
    UINT16      ui2_buf_agt_id;
}   AUD_BUF_AGT_SRC_ALIGN_INFO_T;

typedef struct
{
    UINT16 u2NumCheck;
    UINT16 u2NumPilot;              // stereo detectio
    UINT16 u2PilotHigherThd;
    UINT16 u2PilotLowerThd;
    UINT16 u2NumSap;              // SAP detectio
    UINT16 u2SapHigherThd;
    UINT16 u2SapLowerThd;
    BOOL fgHdevMode;
    BOOL fgCarrierShift;
    BOOL fgSatuMuteMode;
    UINT8 u1SatuMuteHigherThd;
    UINT8 u1SatuMuteLowerThd;
    BOOL fgCarrierMuteMode;
    UINT8 u1CarrierMuteHigherThd;
    UINT8 u1CarrierMuteLowerThd;
    UINT8 u1MonoStereoPrescale;
    UINT8 u1SapPrescale;
    BOOL fgPilotOffsetDetect;
    UINT8 u1SapNoiseMuteHigh;
    UINT8 u1SapNoiseMuteLow;
    UINT32 u2OutputLevelGain;
}  AUD_MTS_FAC_T;

typedef struct
{
    UINT16 u2NumCheck;
    UINT16 u2NumDoubleCheck;
    UINT16 u2MonoWeight;
    UINT16 u2StereoWeight;
    UINT16 u2DualWeight;  
    BOOL fgHdevMode;
    BOOL fgCarrierShift;
    BOOL fgCarrierMuteMode;
    UINT8 u1CarrierMuteHigherThd;
    UINT8 u1CarrierMuteLowerThd;
    UINT8 u1A2Prescale;
    BOOL fgSatuMuteMode;
    BOOL fgNonEuMode;
    UINT16 u2Mono2StereoEU;
    UINT16 u2Stereo2MonoEU;
    UINT16 u2Mono2StereoM;
    UINT16 u2Stereo2MonoM;
}  AUD_A2_FAC_T;

typedef struct
{
    UINT16 u2CorrectThd;
    UINT16 u2TotalSyncLoop;
    UINT16 u2ErrorThd;
    UINT16 u2ParityErrorThd;
    UINT16 u2EveryNumFrames;
    BOOL fgHdevMode;
    BOOL fgAmMuteMode;
    UINT8 u1AmMuteHigherThd;
    UINT8 u1AmMuteLowerThd;
    BOOL fgCarrierShift;
    BOOL fgCarrierMuteMode;
    UINT8 u1CarrierMuteHigherThd;
    UINT8 u1CarrierMuteLowerThd;
    UINT8 u1PalPrescale;
    UINT8 u1AmPrescale;
    UINT8 u1NicamPrescale;
    BOOL fgSatuMuteMode;
    BOOL fgNonEuMode;
}  AUD_PAL_FAC_T;

typedef struct
{
    BOOL fgCLK;
    BOOL fgAOSDATA0;
    BOOL fgAOSDATA1;
    BOOL fgAOSDATA2;
    BOOL fgAOSDATA3;
    BOOL fgAOSDATA4;	
}AUD_DIGITAL_DATA_T;

typedef enum
{
    AUD_PEQ_Q_0_33 = 0,
    AUD_PEQ_Q_0_43,
    AUD_PEQ_Q_0_56,
    AUD_PEQ_Q_0_75,
    AUD_PEQ_Q_1_00,
    AUD_PEQ_Q_1_20,
    AUD_PEQ_Q_1_50,
    AUD_PEQ_Q_1_80,
    AUD_PEQ_Q_2_20,
    AUD_PEQ_Q_2_27,
    AUD_PEQ_Q_3_30,
    AUD_PEQ_Q_3_39,
    AUD_PEQ_Q_4_70,
    AUD_PEQ_Q_5_60,
    AUD_PEQ_Q_6_80,
    AUD_PEQ_Q_8_20,
    AUD_PEQ_Q_MAX
} AUD_PEQ_Q_TYPE_T;

//---------------------------------------------------------------------------
// Global Variable
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

/* Note that this enumeration should be consistent with middleware x_aud_dec.h  */
typedef enum
{
    AUD_FMT_UNKNOWN = 0,
    AUD_FMT_MPEG,
    AUD_FMT_AC3,
    AUD_FMT_PCM,
    AUD_FMT_MP3,
    AUD_FMT_AAC,
    AUD_FMT_DTS,
    AUD_FMT_WMA,
    AUD_FMT_RA,
    AUD_FMT_HDCD,
    AUD_FMT_MLP,     // 10
    AUD_FMT_MTS,
    AUD_FMT_EU_CANAL_PLUS,
    AUD_FMT_PAL,
    AUD_FMT_A2,
    AUD_FMT_FMFM,
    AUD_FMT_NICAM,
    AUD_FMT_TTXAAC,
    AUD_FMT_DETECTOR,
    AUD_FMT_MINER,
    AUD_FMT_LPCM,   // 20
    AUD_FMT_FMRDO,
    AUD_FMT_FMRDO_DET,
    AUD_FMT_SBC,
    AUD_FMT_JPEG,   //24, CC_AUDIO_DSP_JPG_SUPPORT, ADSP_JPEG_DEC2
}   AUD_FMT_T;

/* Move AUD_DEC_STREAM_FROM_T to drv_cust/drvcust_if.h */

// Macro to create bit mask
#define MAKE_BIT_MASK(_val)     (((UINT32) 1) << (UINT32)_val)

/* The following enumeration and bitmasks are used to convey */
/* TV-System type information.                               */
/* Note that this enumeration should be consistent with middleware x_aud_dec.h  */
typedef enum
{
    AUD_TV_SYS_UNKNOWN = -1, /* Must be set to '-1' else I loose an entry in the bit mask. */
    AUD_TV_SYS_A = 0,
    AUD_TV_SYS_B = 1,
    AUD_TV_SYS_C = 2,
    AUD_TV_SYS_D = 3,
    AUD_TV_SYS_E = 4,
    AUD_TV_SYS_F = 5,
    AUD_TV_SYS_G = 6,
    AUD_TV_SYS_H = 7,
    AUD_TV_SYS_I = 8,
    AUD_TV_SYS_J = 9,
    AUD_TV_SYS_K = 10,
    AUD_TV_SYS_K_PRIME = 11,
    AUD_TV_SYS_L = 12,
    AUD_TV_SYS_L_PRIME = 13,
    AUD_TV_SYS_M = 14,
    AUD_TV_SYS_N = 15,
    AUD_TV_SYS_A2 = 16,
    AUD_TV_SYS_PAL = 17,
    AUD_TV_SYS_NICAM = 18,
    AUD_TV_SYS_SECAM = 19
}   AUD_TV_SYS_T;

#define AUD_TV_SYS_MASK_A               MAKE_BIT_MASK (AUD_TV_SYS_A)
#define AUD_TV_SYS_MASK_B               MAKE_BIT_MASK(AUD_TV_SYS_B)
#define AUD_TV_SYS_MASK_C               MAKE_BIT_MASK(AUD_TV_SYS_C)
#define AUD_TV_SYS_MASK_D               MAKE_BIT_MASK(AUD_TV_SYS_D)
#define AUD_TV_SYS_MASK_E               MAKE_BIT_MASK(AUD_TV_SYS_E)
#define AUD_TV_SYS_MASK_F               MAKE_BIT_MASK(AUD_TV_SYS_F)
#define AUD_TV_SYS_MASK_G               MAKE_BIT_MASK(AUD_TV_SYS_G)
#define AUD_TV_SYS_MASK_H               MAKE_BIT_MASK(AUD_TV_SYS_H)
#define AUD_TV_SYS_MASK_I               MAKE_BIT_MASK(AUD_TV_SYS_I)
#define AUD_TV_SYS_MASK_J               MAKE_BIT_MASK(AUD_TV_SYS_J)
#define AUD_TV_SYS_MASK_K               MAKE_BIT_MASK(AUD_TV_SYS_K)
#define AUD_TV_SYS_MASK_K_PRIME         MAKE_BIT_MASK(AUD_TV_SYS_K_PRIME)
#define AUD_TV_SYS_MASK_L               MAKE_BIT_MASK(AUD_TV_SYS_L)
#define AUD_TV_SYS_MASK_L_PRIME         MAKE_BIT_MASK(AUD_TV_SYS_L_PRIME)
#define AUD_TV_SYS_MASK_M               MAKE_BIT_MASK(AUD_TV_SYS_M)
#define AUD_TV_SYS_MASK_N               MAKE_BIT_MASK(AUD_TV_SYS_N)

#define AUD_TV_SYS_MASK_A2              MAKE_BIT_MASK(AUD_TV_SYS_A2)
#define AUD_TV_SYS_MASK_PAL             MAKE_BIT_MASK(AUD_TV_SYS_PAL)
#define AUD_TV_SYS_MASK_NICAM           MAKE_BIT_MASK(AUD_TV_SYS_NICAM)
#define AUD_TV_SYS_MASK_SECAM           MAKE_BIT_MASK(AUD_TV_SYS_SECAM)

/* The following enumeration and bitmasks are used to convey */
/* Audio-System type information.                            */
/* Note that this enumeration should be consistent with middleware x_aud_dec.h */
typedef enum
{
    AUD_SYS_UNKNOWN = -1, /* Must be set to '-1' else I loose an entry in the bit mask. */
    AUD_SYS_AM,
    AUD_SYS_FM_MONO,
    AUD_SYS_FM_EIA_J,
    AUD_SYS_FM_A2,
    AUD_SYS_FM_A2_DK1,
    AUD_SYS_FM_A2_DK2,
    AUD_SYS_FM_RADIO,
    AUD_SYS_NICAM,
    AUD_SYS_BTSC
}   AUD_SYS_T;

#define AUD_SYS_MASK_AM                 MAKE_BIT_MASK (AUD_SYS_AM)
#define AUD_SYS_MASK_FM_MONO            MAKE_BIT_MASK (AUD_SYS_FM_MONO)
#define AUD_SYS_MASK_FM_EIA_J           MAKE_BIT_MASK (AUD_SYS_FM_EIA_J)
#define AUD_SYS_MASK_FM_A2              MAKE_BIT_MASK (AUD_SYS_FM_A2)
#define AUD_SYS_MASK_FM_A2_DK1          MAKE_BIT_MASK (AUD_SYS_FM_A2_DK1)
#define AUD_SYS_MASK_FM_A2_DK2          MAKE_BIT_MASK (AUD_SYS_FM_A2_DK2)
#define AUD_SYS_MASK_FM_RADIO           MAKE_BIT_MASK (AUD_SYS_FM_RADIO)
#define AUD_SYS_MASK_NICAM              MAKE_BIT_MASK (AUD_SYS_NICAM)
#define AUD_SYS_MASK_BTSC               MAKE_BIT_MASK (AUD_SYS_BTSC)

/* Note that this enumeration should be consistent with middleware x_aud_dec.h */
/// Audio middleware system table.
typedef struct _MW_TV_AUD_SYS_T
{
    UINT32  u4TvSysMask;
    UINT32  u4AudSysMask;
} MW_TV_AUD_SYS_T;

#ifndef _TV_AUD_SYS_T
#define _TV_AUD_SYS_T
typedef enum {
    SV_NONE_DETECTED = 0x0,     //0x0
    SV_MTS           = 0x1,     //0x1
    SV_FM_FM         = 0x2,     //0x2
    SV_NTSC_M        = 0x3,     //0x3
    SV_A2_BG         = 0x4,     //0x4
    SV_A2_DK1        = 0x5,     //0x5
    SV_A2_DK2        = 0x6,     //0x6
    SV_A2_DK3        = 0x7,     //0x7
    SV_PAL_I         = 0x8,     //0x8
    SV_PAL_BG        = 0x9,     //0x09
    SV_PAL_DK        = 0xa,     //0x0a
    SV_SECAM_L       = 0xb,     //0x0b
    SV_SECAM_L_PLUM  = 0xc,     //0x0c
    SV_SECAM_BG      = 0xd,     //0x0d, 0x0c is reserved for SECAM L' setting for tuner
    SV_SECAM_DK      = 0xe,      //0x0e
    SV_AUD_SYS_NUM   = 0xf
} TV_AUD_SYS_T;
#endif

/* Note that this enumeration should be consistent with AUD_DEC_MTS_T in middleware x_aud_dec.h */
typedef enum
{
    AUD_SOUND_MODE_UNKNOWN = 0,
    AUD_SOUND_MODE_MONO,
    AUD_SOUND_MODE_STEREO,
    AUD_SOUND_MODE_SUB_LANG,
    AUD_SOUND_MODE_DUAL1,
    AUD_SOUND_MODE_DUAL2,
    AUD_SOUND_MODE_NICAM_MONO,
    AUD_SOUND_MODE_NICAM_STEREO,
    AUD_SOUND_MODE_NICAM_DUAL1,
    AUD_SOUND_MODE_NICAM_DUAL2,
    AUD_SOUND_MODE_A2_DUAL1_DUAL2,
    AUD_SOUND_MODE_NICAM_DUAL1_DUAL2
} AUD_SOUND_MODE_T;

typedef enum 
{
    AUD_MATRIX_OUT_OFF = 0,      
    AUD_MATRIX_OUT_L_MONO,
    AUD_MATRIX_OUT_R_MONO,
    AUD_MATRIX_OUT_LR_MIX
} AUD_MATRIX_OUT_T;

/// Audio mem buffer information table.
typedef struct
{
    UINT8* pData;
    UINT32 u4Length;
    UINT32 u4LoopCount;
}   MEM_BUFFER_INFO_T;

typedef enum 
{
    AUD_DUAL_DEC_UNINITIALIZED = 0,      
    AUD_DUAL_DEC_INIT_AUX_DECODER,
    AUD_DUAL_DEC_PLAY,
    AUD_DUAL_DEC_STOP,
    AUD_DUAL_DEC_IDLE
}   AUD_DUAL_DEC_DRV_STATE_T;

typedef enum 
{
    AUD_UNINITIALIZED = 0,      
    AUD_TRIGGER_ADSP,
    AUD_WAIT_POWER_ON,
    AUD_INIT,  
    AUD_WAIT_CMD,
    AUD_WAIT_PLAY,
    AUD_WAIT_PAUSE,
    AUD_WAIT_RESUME,
    AUD_WAIT_STOP,
    AUD_WAIT_STOPSTREAM,
    AUD_IDLE,
    AUD_ON_PLAY,
    AUD_ON_PAUSE,
    AUD_STOPPED,
    AUD_DECODE_INIT,
    AUD_CHANGE_FORMAT
}   AUD_DRV_STATE_T;

typedef enum  
{
    AV_SYNC_FREE_RUN = 0,       // no need to syn
    AV_SYNC_SLAVE,              // syn to STC
    AV_SYNC_AUDIO_MASTER       // update A-PTS to A-STC, may use in PCR or HardDisk playback
}   AV_SYNC_MODE_T;

typedef enum  
{
    AV_SYNC_STC_A1 = 0, 
    AV_SYNC_STC_A2,       
    AV_SYNC_STC_V1,
    AV_SYNC_STC_V2
}   AV_SYNC_STC_SELECT_T;

/* Audio command types. */
typedef enum
{
    AUD_CMD_PLAY       = 0,
    AUD_CMD_STOP       = 1,
    AUD_CMD_RESET      = 2,
    AUD_CMD_PAUSE      = 3,
    AUD_CMD_AVSYNC     = 4,
    AUD_CMD_LOADCODE   = 5,
    AUD_CMD_RESUME  = 6,
    AUD_CMD_CHANGE_FORMAT = 7,
    AUD_CMD_STOPSTREAM = 8
}   AUD_DEC_CMD_T;

/* Audio dual decoder command types. */
typedef enum
{
    AUD_DUAL_DEC_CMD_PLAY    = 0,
    AUD_DUAL_DEC_CMD_STOP    = 1
}   AUD_DUAL_DEC_CMD_T;

/* IEC types. */
typedef enum
{
    AUD_IEC_CFG_PCM = 0,
    AUD_IEC_CFG_RAW
}   AUD_IEC_T;

/* IEC PCM Channel */
typedef enum
{
    AUD_IEC_PCM_CH_L_R = 0,
    AUD_IEC_PCM_CH_LS_RS,
    AUD_IEC_PCM_CH_C_SW,
    AUD_IEC_PCM_CH_7_8,
    AUD_IEC_LINE_IN,
    AUD_IEC_PCM_CH_9_10,
    AUD_IEC_AUX
}   AUD_IEC_CH_T;

typedef enum
{
    FS_16K = 0x00,
    FS_22K,
    FS_24K,
    FS_32K,
    FS_44K,
    FS_48K,
    FS_64K,
    FS_88K,
    FS_96K,
    FS_176K,
    FS_192K,
    FS_8K, // appended since 09/10/2007, don't change the order
    FS_11K, // appended since 09/10/2007, don't change the order
    FS_12K// appended since 09/10/2007, don't change the order
}   SAMPLE_FREQ_T;

/* Audio types. */
typedef enum
{
    AUD_TYPE_UNKNOWN = 0,
    AUD_TYPE_MONO,                  /* 1/0 */
    AUD_TYPE_MONO_SUB,              /* 1+sub-language */
    AUD_TYPE_DUAL_MONO,             /* 1+1 */
    AUD_TYPE_STEREO,                /* 2/0 */
    AUD_TYPE_STEREO_SUB,            /* 2+sub-language */
    AUD_TYPE_STEREO_DOLBY_SURROUND, /* 2/0, dolby surround */
    AUD_TYPE_SURROUND_2CH,          /* 2/1 */
    AUD_TYPE_SURROUND,              /* 3/1 */
    AUD_TYPE_3_0,                   /* 3/0 */
    AUD_TYPE_4_0,                   /* 2/2 */
    AUD_TYPE_5_1,                   /* 3/2 +SW */
    AUD_TYPE_7_1,                   /* 5/2 */
    AUD_TYPE_FM_MONO_NICAM_MONO,
    AUD_TYPE_FM_MONO_NICAM_STEREO,
    AUD_TYPE_FM_MONO_NICAM_DUAL,    
    AUD_TYPE_OTHERS,
    AUD_TYPE_5_0,                    /* 3/2 */
    AUD_TYPE_JOINT_STEREO,
    AUD_TYPE_FMRDO_MONO,
    AUD_TYPE_FMRDO_STEREO
}   AUD_TYPE_T;

/// Audio ain configuration table.
typedef struct
{
    AUD_DEC_STREAM_FROM_T eStrSrc;      ///< Stream source
    DATA_FORMAT_T      eFormat;         ///< format of alignment
    DAC_DATA_NUMBER_T  eBits;           ///< number of bits per sample
    LRCK_CYC_T         eCycle;          ///< cycles per sample
    MCLK_FREQUENCY_T   eSampleFreq;     ///< DAC sampling frequence
    BOOL               fgDataInvert;    ///< Invert audio output for OP phase
    BOOL               fgLRInvert;      ///< Invert L/R audio output
 //========================================================================
    UINT8 uFormat;                      ///< format of alignment
    UINT8 uBits;                        ///< number of bits per sample
    UINT8 uCycle;                       ///< cycles per sample
    BOOL  fgIsSPDIFin;                  ///< TRUE is slave mode
    BOOL  fgLrckInv;                    ///< TRUE while LRCK Low is Left channel
}   AIN_CFG_T;

typedef enum
{
    AVC_COMP_VIDEO_0 = 0,               ///< Composite video (CVBS) # 0
    AVC_COMP_VIDEO_1,                   ///< Composite video (CVBS) # 1
    AVC_COMP_VIDEO_2,                   ///< Composite video (CVBS) # 2
    AVC_S_VIDEO_0,                      ///< S-Video
    AVC_Y_PB_PR_0,                      ///< YPbPr # 0
    AVC_Y_PB_PR_1,                      ///< YPbPr # 1
    AVC_VGA,                            ///< VGA, via headphone jet
    AVC_HDMI,                           ///< HDMI (DVI)  
    AVC_S_VIDEO_1,
    AVC_S_VIDEO_2
}   AV_COMPONENT_T;

/// Audio decoder information table.
typedef struct
{
    AUD_FMT_T   e_aud_fmt;
    AUD_TYPE_T  e_aud_type;
    UINT32      ui4_sample_rate;
    UINT32      ui4_data_rate;
    UINT8       ui1_bit_depth;
} AUD_DEC_INFO_T;

typedef enum
{
    AUD_COND_ERROR = -1,
    AUD_COND_CTRL_DONE,
    AUD_COND_AUD_CLIP_DONE,
    AUD_COND_MEM_BUFFER_DONE,
    AUD_COND_FLUSH_DONE,
    AUD_COND_FEED_ME_ON,
    AUD_COND_FEED_ME_OFF,
    AUD_COND_AUD_INFO_CHG,
    AUD_COND_TV_SYS_DETECTED,
    AUD_COND_AUD_DECODE_STATUS_CHG,
    AUD_COND_MM_EOS, 
    AUD_COND_FM_RADIO_DET_DONE
}   AUD_COND_NFY_T;

typedef enum
{
    AUD_DECODE_UNKNOWN = -1, ///< initial value; the decode status should be initialized to this value after connect/disconnect or AUD_DEC_CTRL_STOP
    AUD_DECODE_NO_DATA,      ///< audio decoder found no audio data in stream. there will be a timer threashold value for decoder to change the decode status from NORMAL/ERROR to NO_DATA
    AUD_DECODE_NORMAL,     ///< audio decoder decode the first audio frame data successfully after AUD_DEC_CTRL_PLAY
    AUD_DECODE_ERROR,       ///< audio decoder decode the audio data abnormally (e.g. unsupported format)
    AUD_DECODE_NOT_SUPPORT,	///< unsupported format for WMA decoder
    AUD_DECODE_FMT_CONFLICT
}   AUD_DECODE_STATUS_T;

typedef enum
{
    AUD_HP_COND_HEADPHONE_COND_UNPLUGED = 0,
    AUD_HP_COND_HEADPHONE_COND_PLUGED
} AUD_HP_COND_NFY_T;

typedef enum        //Andrew Wen : 2007/2/2 for STC differnece setting
{
    AUD_STC_LO = 0,
    AUD_STC_HI,
    AUD_STC_WS    
}   AUD_STC_DIFF_SET;

typedef enum
{
    AUD_NFY_STREAM = 0,
    AUD_NFY_HP
}   AUD_NFY_TAG_T;

/* Notify function */
typedef void (*AudDecNfyFct) (void *          pvNfyTag,
                              UINT8           u1DecId,
                              AUD_COND_NFY_T  eNfyCond,
                              UINT32          u4Data1,
                              UINT32          u4Data2);

/* Notify setting info. */
/// Audio Notify setting info table.
typedef struct
{
    void *          pvTag;

    AudDecNfyFct    pfAudDecNfy;
}   AUD_NFY_INFO_T;

/// Audio reverb mode table.
typedef struct
{
    UINT32      u4Gain;
    UINT8       u1ReverbBank0;
    UINT8       u1ReverbBank1;
    UINT8       u1ReverbBank2;
    UINT8       u1ReverbBank3;
} REVERB_MODE_T;

/// Audio PAL2 setting table.
typedef struct
{
    UINT16 u2Config;
    UINT16 u2Mode;
} PL2_SETTING_T;

/// Audio EQ flag table.
typedef struct
{
    UINT8       fgChEqOn       : 1;
    UINT8       fgBassBoostOn  : 1;
    UINT8       fgSuperBassOn  : 1;  
    UINT8       fgClearBoostOn : 1;  
    UINT8       Reserve        : 4;  
}   EQ_FLAG_T;

typedef union
{
    UINT8       u1Eq;
    EQ_FLAG_T   rField;
} EQ_FLAG_UNION_T;

/// Audio channel band table.
typedef struct
{
    UINT8       u1Channel;
    UINT8       u1Band;
} CH_BAND_T;

/// Audio EQ band table.
typedef struct
{
    UINT8       u1BandIdx;
    INT8        i1Level;
}   EQ_BAND_CFG;

/// Audio EQ band information table.
typedef struct
{
    UINT8       ui1_band_idx;
    INT8        i1_max_lvl;
    INT8        i1_min_lvl;
    UINT32      ui4_freq_lower_bound;
    UINT32      ui4_freq_upper_bound;
} AUD_EQ_BAND_INFO_T;

// Test tone freqence definition for table selection
typedef enum
{
    TEST_TONE_1K_0dB = 0,
    TEST_TONE_1K_20dB,
    TEST_TONE_1K_60dB,
    TEST_TONE_1K_120dB,
    TEST_TONE_20K_0dB,
    TEST_TONE_4K_0dB,    
    TEST_TONE_200_0dB,
    TEST_TONE_200_60dB,
    TEST_TONE_400_0dB,
    TEST_TONE_1K3K_0dB,
    TEST_TONE_1K_0dB_LCH_ONLY
} TEST_TONE_T;

/// Audio clip type table.
typedef struct
{
    BOOL        fgPlayTestTone;
    UINT32      u4OutputChlSelect;
    UINT32      u4RepeatCount;
    UINT32      u4ToneFreq;
    UINT32      u4AudCtrlTypeAddr;
    
    AUD_FMT_T           eAudFmt;
    MEM_BUFFER_INFO_T   eMemBuf; 
} AUD_CLIP_TYPE_T;

/* BBE mode */
typedef enum
{
    AUD_BBE_MODE_OFF = 0,
    AUD_BBE_MODE_BBE,
    AUD_BBE_MODE_BBE_VIVA
}   AUD_BBE_MODE_T;

// Buffer agent
typedef struct 
{
    UINT8*              pui1_buff;
    //SIZE_T              z_size;  //FIXME
    UINT32              u4Size;
    UINT32              ui4_req_id;
    UINT32              ui4_flags;
    UINT64              ui8_offset;
} AUD_BUF_AGT_DATA_BLK_T;

typedef enum
{
    AUD_2_BAGT_NFY_COND_UNKNOWN = 0,
    AUD_2_BAGT_NFY_COND_DATA_REQ,
    AUD_2_BAGT_NFY_COND_FLUSH_REQ,
    AUD_2_BAGT_NFY_COND_DATA_CONSUMED
} AUD_2_BAGT_NFY_COND_T;

typedef enum
{    
    AUD_FM_RADIO_NON_DETECTED = 0,
    AUD_FM_RADIO_DETECTED
} AUD_FM_RADIO_DET_T;

typedef enum
{
    AUD_VIDEO_IS_STABLE = 0,
    AUD_VIDEO_IS_UNSTABLE = 1
}   AUD_VIDEO_STATE_T;

//-----------------------------------------------------------------------------
// Begin of NVM and Profile Functions 
//   for customers use EEPROM and FLASH audio parameters
//-----------------------------------------------------------------------------
// NVM Data Releated
#ifdef CC_AUD_USE_NVM
#define AUD_NVM_LENGTH (0xFF)
#define AUD_NVM_OFFSET (0xEA0)
#define AUD_NVM_PROFILE_OFFSET1 (0x100)
#define AUD_NVM_PROFILE_OFFSET2 (0x880)
#define AUD_NVM_PROFILE_OFFSET3 (0xFE0)
#define AUD_NVM_PROFILE_START   (0x28000000 + 0x760000 + 0x27FF0)  //FIXME!! nvm
#define AUD_NVM_PROFILE_END     (0x28000000 + 0x760000 + 0x288CD)  //FIXME!! nvm
#endif

typedef enum
{
    AUD_NVM_READ = 0,
    AUD_NVM_WRITE,
    AUD_NVM_INIT,
    AUD_NVM_VOL_INIT,
    AUD_NVM_PROFILE_QRY,
    AUD_NVM_VS_INIT,
    AUD_NVM_PROFILE_OFFSET
} AUD_NVM_OP_T;

typedef enum
{
    AUD_NVM_MTS_OBSRV_TIME =0,
    AUD_NVM_MTS_STEREO_TIME,
    AUD_NVM_MTS_SAP_TIME,
    AUD_NVM_MTS_UP_THR_BTSC,
    AUD_NVM_MTS_LO_THR_BTSC,
    AUD_NVM_MTS_UP_THR_SAP,
    AUD_NVM_MTS_LO_THR_SAP,
    AUD_NVM_MTS_NMUTE_FM_UPTHR,
    AUD_NVM_MTS_NMUTE_FM_LOTHR,
    AUD_NVM_MTS_NMUTE_SAP_UPTHR,
    AUD_NVM_MTS_NMUTE_SAP_LOTHR,
    AUD_NVM_MTS_PRE_BTSC_STMONO,
    AUD_NVM_MTS_PRE_SAP,
    AUD_NVM_MTS_HIGH_DEV,
    AUD_NVM_MTS_CARRIER_SHIFT,
    AUD_NVM_MTS_FMCARR_MUTE,
    AUD_NVM_AVC_ADJUST_RATE,
    AUD_NVM_AVC_UI_ADJUST_RATE,
    AUD_NVM_AVC_LEVEL,
    AUD_NVM_AVC_GAIN_UP,
    AUD_NVM_LIPSYNC_A,
    AUD_NVM_LIPSYNC_B,
    AUD_NVM_LIPSYNC_C,
    AUD_NVM_FLAT,
    AUD_NVM_REVERB,
    AUD_NVM_VOL_MIN,
    AUD_NVM_VOL_MAX,
    AUD_NVM_VOL_STEP1,
    AUD_NVM_VOL_STEP2,
    AUD_NVM_VOL_STEP3,
    AUD_NVM_VOL_STEP4,
    AUD_NVM_VOL_STEP5,
    AUD_NVM_PREHDMI,
    AUD_NVM_PREAV,
    AUD_NVM_PREUSB,
    AUD_NVM_PREATV,
    AUD_NVM_PREDTV,
    AUD_NVM_BASS_SUBWOOFER,
    AUD_NVM_BASS_SPKSIZE,
    AUD_NVM_VS_CLARITY,
    AUD_NVM_VS_WIDTH,     
    AUD_NVM_VS_LRGAIN,    
    AUD_NVM_VS_XTALK_GIAN,
    AUD_NVM_VS_OPGAIN,    
    AUD_NVM_VS_SWGAIN,    
    AUD_NVM_VS_FO,        
    AUD_NVM_BBE_LEVEL,
    AUD_NVM_BBE_UPGAIN,
    AUD_NVM_BBE_LOGAIN,
    AUD_NVM_BBE_3DGAIN,
    AUD_NVM_PL2_ON,
    AUD_NVM_PL2_MODE,
    AUD_NVM_NOP
} AUD_NVM_TYPE_T;

typedef struct 
{
    UINT16              u2Offset;
    UINT8               u1Length;
    UINT32              u4DefaultValue;
    AUD_NVM_TYPE_T      eDataType;
} AUD_NVM_DATA_T;    

typedef struct
{
    INT16 i2NvmInputSrcHDMI;
    INT16 i2NvmInputSrcAV;
    INT16 i2NvmInputSrcUSB;
    INT16 i2NvmInputSrcATV;
    INT16 i2NvmInputSrcDTV;
}AUD_IPT_SRC_GAIN_T;

//==========  Flash AQ Related Definitions ===================================
#define AUD_FLASH_AQ_SIZE (512*1024) //Total Flash AQ size on NAND Flash partition.
#define AUD_FLASH_AQ_BUF_SIZE (5*1024)  //Flash AQ buffer size for temp usage.
#define FLASH_AQ_VER_LENGTH 8
#define FLASH_AQ_VER_OFFSET 0
#define FLASH_AQ_SCENE_NUM   5
#define AQ_LEVEL_ITEMS 3
#define AQ_BASS_ITEMS 2
#define AQ_EQ_ITEMS 7
#define AQ_DRC_ITEMS 8
#define AQ_AVC_ITEMS 5
#define AQ_SPH_ITEMS 1
#define AQ_MVS_ITEMS 7
#define AQ_POSTSCL_ITEMS 2
#define AQ_PEQ_ITEMS 45
#define AQ_EXT_PEQ_ITEMS 35
#ifndef CC_AUD_NEW_AQ_PARSER
#define FLASH_AQ_COMM_SRC_NUM 5
#define FLASH_AQ_COMM_CH_NUM  10
#define FLASH_AQ_SRC_NUM  4
#define FLASH_AQ_SM_NUM   4
#define AQ_LIMITER_ITEMS 4
#else
#define FLASH_AQ_COMM_SRC_NUM 4
#define FLASH_AQ_COMM_CH_NUM  4
#define FLASH_AQ_SRC_NUM  3
#define FLASH_AQ_SM_NUM   4
#define AQ_LIMITER_ITEMS 1
#endif

typedef enum
{
    AQ_TYPE_VERSION =0,
    AQ_TYPE_CH_VOLUME,
    AQ_TYPE_SRC_VOLUME,
    AQ_TYPE_VOL_TABLE,
    AQ_TYPE_LO_VOL_TABLE,
    AQ_TYPE_PEQ,
    AQ_TYPE_LEVEL,
    AQ_TYPE_BASS_TREBLE,
    AQ_TYPE_EQ,
    AQ_TYPE_DRC,
    AQ_TYPE_AVC,
    AQ_TYPE_LIMITER,   
    AQ_TYPE_SPKHEIGHT,
    AQ_TYPE_MVS,
    AQ_TYPE_POST,
    AQ_TYPE_SP_VOL,
    AQ_TYPE_LO_VOL,
    AQ_TYPE_EXT_PEQ,
    AQ_TYPE_NUM
}AUD_AQ_TYPE_T;

typedef struct 
{
    AUD_AQ_TYPE_T       eDataType;
    CHAR                pChar[8];
    UINT8               u1X;
    UINT8               u1Y;
    UINT8               u1Bytes;
} AUD_AQ_DSCRPT_T;    

typedef struct 
{
    UINT8               u1Cutoff;
    UINT8               u1LfeGain;
    UINT8               u1AttackHi;
    UINT8               u1ReleaseHi;
    UINT8               u1RateHi;    
    UINT8               u1AttackLo;
    UINT8               u1ReleaseLo;
    UINT8               u1RateLo;
} AUD_AQ_AMP_DRC_T;    

typedef struct 
{
    UINT8 u1Data[15];
}
AUD_AQ_AMP_RAM_DATA_T;

typedef enum
{
    AQ_OP_EXE = 0,
    AQ_OP_INIT
}AUD_AQ_OP_TYPE_T;

typedef enum
{
    AQ_EXT_PEQ_HP =0,
    AQ_EXT_PEQ_LP,
    AQ_EXT_PEQ_BP,
    AQ_EXT_PEQ_FLAT
}AUD_AQ_EXTPEQ_TYPE_T;

typedef struct 
{
    UINT32 u4Data[AQ_EXT_PEQ_ITEMS];
} AUD_AQ_EXTPEQ_DATA_T;    

typedef enum
{
    AQ_AMP_FS_32K = 0,
    AQ_AMP_FS_44K,
    AQ_AMP_FS_48K,
    AQ_AMP_FS_NUM
}AUD_AQ_AMP_FS_T;

//===================================================

//----------- End of NVM and Profile Data Types -------------------------------

typedef enum
{
    AUD_SUR_CLARITY = 0,
    AUD_SUR_WIDTH,
    AUD_SUR_LRGAIN,
    AUD_SUR_XTALK,
    AUD_SUR_OUTPUT_GAIN,  
    AUD_SUR_BASS_GAIN,
    AUD_SUR_FO,
    AUD_SUR_PRAR_NUM
}   AUD_VSURR_CFG_TYPE_T;


typedef struct
{
    UINT32 u4Clarity;
    UINT32 u4Width;
    UINT32 u4LRGain;
    UINT32 u4Xtalk;
    UINT32 u4OutputGain;
    UINT32 u4BassGain;
    UINT32 u4Fo;
}AUD_VSURR_CFG_T;

typedef struct
{
    UINT16 u2Pktno;        //W_WMA_PACKET_NO
    UINT16 u2HdrSrc;       //W_WMA_HEADER_SOURCE
    UINT32 u4Pktcnt;       //D_WMA_PACKET_COUNT
    UINT32 u4Pktsz;        //D_WMA_PACKET_SIZE
    UINT16 u2Wmaver;       //W_WMA_VERSION
    UINT16 u2Numch;        //W_WMA_NUM_CH
    UINT32 u4Sampersec;    //D_WMA_SAMPLE_PER_SEC
    UINT32 u4Bpersec;      //D_WMA_BYTE_PER_SEC
    UINT32 u4Blocksz;      //D_WMA_BLOCK_SIZE
    UINT16 u2Encoderopt;   //W_WMA_ENCODER_OPTION
}AUD_WMA_CFG_T;

typedef struct
{
    UINT8   u1SmartRev[16];
} AUD_SMART_AUD_INFO_T;

typedef enum
{
    PCM_24_BITS = 0,
    PCM_IMA_ADPCM_BIG_ENDIAN =1,
    PCM_16_BITS_BIG_ENDIAN  =2,
    PCM_MS_ADPCM_BIG_ENDIAN  = 3,
    PCM_IMA_ADPCM_LITTLE_ENDIAN =1 |(1<<15),
    PCM_16_BITS_LITTLE_ENDIAN =2|(1<<15),
    PCM_MS_ADPCM_LITTLE_ENDIAN = 3|(1<<15)
} PCM_INPUT_TYPE_T;

/// Audio PCM type
typedef struct
{
	SAMPLE_FREQ_T      eSampleFreq;  ///< PCM Sampling Rate.
	PCM_INPUT_TYPE_T   ePcmDataInfo; ///< PCM Sampling Rate. 
} AUD_PCM_SETTING_T;

//Dynamic volume curve setting  //Andrew Wen 07/8/20
#define VOL_TABLE_CHK_PNT 7

typedef void (*AudBagtDataReqFct)(    UINT16                  ui2_buf_agt_id,
                                                        AUD_BUF_AGT_DATA_BLK_T*     pt_data_blk,
                                                        AUD_2_BAGT_NFY_COND_T eType);

/// Callbalck function if clip playback is done
typedef INT32 (*AUD_CLIP_NOTIFY)(UINT8 u1DecId, UINT32 u1Datat, UINT32 u4Datat2);

typedef void (*AUD_WMAErr_NOTIFY)(UINT32 u4WMAErrCode);
//static AUD_WMAErr_NOTIFY _hWMAErrNotifyFunc = NULL; 

//----- LPCM decoder -----
typedef enum
{
    LPCM_TYPE_NORAML = 0,
    LPCM_TYPE_MS_ADPCM = 1,
    LPCM_TYPE_IMA_ADPCM = 2
} LPCM_INPUT_TYPE_T;

typedef struct
{
    SAMPLE_FREQ_T      eSampleFreq; 
    LPCM_INPUT_TYPE_T  eLPcmType;
    UINT16             ui2_num_ch;
    UINT16             ui2_block_align;
    UINT16             ui2_bits_per_sample;
    BOOL               b_big_endian;
    BOOL               b_info_not_specific; /*pcm info from dmx*/
} AUD_LPCM_INFO_T;

typedef struct
{
    UINT8   u1LpcmChAssignment;
    UINT8   u1LpcmFreqGrp1;
    UINT8   u1LpcmFreqGrp2;
    UINT8   u1LpcmBitShift;    
    UINT8   u1LpcmDrcValue;    
    UINT8   u1LpcmBitResGrp1;    
    UINT8   u1LpcmBitResGrp2;        
    UINT8   u1LpcmDrcFlag;    
    UINT8   u1LpcmDecodingType;
    UINT8   u1ADPCMDecodingType;
    UINT16  u2ADPCMBlockAlign;    
} AUD_LPCM_SETTING_T;
//----- end of LPCM -----

/* Prologic 2 mode */
#define PROLOGIC_EMUATION       (UINT16)0
#define VIRUTAL_MODE            (UINT16)1
#define MUSIC_MODE              (UINT16)2
#define MOVIE_MODE              (UINT16)3
#define MATRIX_MODE             (UINT16)4
#define CUSTOM_MODE             (UINT16)7
    
/* Prologic 2 setting */
#define PL2_ENABLE              (UINT16)1       ///< Prologic 2 enable/disable
#define PL2_MODE_AUTO           (UINT16)(1<<1)  ///< Prologic 2 mode is decided from AC3 decoder
#define PL2_PCD_AUTO            (UINT16)(1<<2)  ///< Prologic 2 panorama, center width, dimention is decided from AC3 decoder
#define PL2_AUTO_ENABLE         (UINT16)(1<<3)  ///< Prologic 2 enable/disable from AC3 decoder
#define PL2_AUTO_BALANCE        (UINT16)(1<<4)  ///< auto balance
#define PL2_RS_POLAR_INV        (UINT16)(1<<5)  ///< invert polarity of RS's output
#define PL2_PANORAMA            (UINT16)(1<<6)  ///< default OFF
#define PL2_SURR_FILTER         (UINT16)(1<<7)  ///< surround shelf filter
/* Prologic 2 center width */
#define PL2_CWIDTH(a)           (UINT16)((0x0f & (a))<<8)   ///< center width spread value 0~7 default 3
#define PL2_DIM(a)              (UINT16)((0x0f & (a))<<12)  ///< dimention control -3~3 default 0

/* Prologic 2 control routine related*/
#define PL2CTRL_SWITCH          0
#define PL2CTRL_MODE            1
#define PL2CTRL_PANORAMA        2
#define PL2CTRL_DIMENSION       3   
#define PL2CTRL_C_WIDTH         4

/* Prologic 2 control related*/
#define PL2CTRL_ON              0
#define PL2CTRL_OFF             1
#define PL2CTRL_AUTO            2
/* Prologic 2 mode related*/
#define PL2CTRL_MODE_MANUAL     0
#define PL2CTRL_MODE_AUTO       1
/* Prologic 2 Panorama related*/
#define PL2CTRL_PAN_ON          0
#define PL2CTRL_PAN_OFF         1

#define AC3_KARA_DISABLE        ((UINT16)0x0001)    ///< disable karaoke mode
#define AC3_KARA_AWARE          ((UINT16)0x0002)    ///< karaoke aware mode
#define AC3_KARA_NONE           ((UINT16)0x0004)    ///< none (in capable mode)
#define AC3_KARA_V1             ((UINT16)0x0008)    ///< V1 only (in capable mode)
#define AC3_KARA_V2             ((UINT16)0x0010)    ///< V2 only (in capable mode)
#define AC3_KARA_BOTH           ((UINT16)0x0020)    ///< V1+V2   (in capable mode)
#define AC3_KARA_VOC_ASIST      ((UINT16)0x0040)    ///< vocal assist mode
#define AC3_KARA_NO_MELODY      ((UINT16)0x0080)    ///< melody off

#define AC3_DM_STEREO           0   ///< AC3 dual mono mode stereo
#define AC3_DM_L_MONO           1   ///< AC3 dual mono mode left mono
#define AC3_DM_R_MONO           2   ///< AC3 dual mono mode right mono
#define AC3_DM_M_MONO           3   ///< AC3 dual mono mode mix mono

#define AC3_COMP_CUSTOM_0       0   ///< AC3 compression mode custom 0
#define AC3_COMP_CUSTOM_1       1   ///< AC3 compression mode custom 1
#define AC3_COMP_LINE           2   ///< AC3 compression line mode P.S. default set as line mode
#define AC3_COMP_RF             3   ///< AC3 compression RF mode
#define AC3_COMP_WLINE          4   ///< AC3 compression wide range line mode

// *********************************************************************
// Constant definitions
// *********************************************************************
#define AUD_OK                  (INT32)(0)
#define AUD_FAIL                (INT32)(-1)

#define AUD_DEC_MAIN            ((UINT8)0)
#define AUD_DEC_AUX             ((UINT8)1)
//#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
#define AUD_DEC_THIRD          ((UINT8)2)
//#else
//#define AUD_SYSTEM              ((UINT8)2)
//#endif

#define INTERNAL_DAC_ID_MAIN    0
#define INTERNAL_DAC_ID_AUX     1
#define INTERNAL_DAC_ID_THIRD   2

#define APLL_ID_MAIN            0
#define APLL_ID_AUX             1

/* Move AUD_DEC_NUM to drv_cust/drvcust_if.h */

#define AUD_CMD_FLAG_PLAY       ((UINT32) (1<<(UINT32)AUD_CMD_PLAY))
#define AUD_CMD_FLAG_STOP       ((UINT32) (1<<(UINT32)AUD_CMD_STOP))
#define AUD_CMD_FLAG_RESET      ((UINT32) (1<<(UINT32)AUD_CMD_RESET))
#define AUD_CMD_FLAG_PAUSE      ((UINT32) (1<<(UINT32)AUD_CMD_PAUSE))
#define AUD_CMD_FLAG_AVSYNC     ((UINT32) (1<<(UINT32)AUD_CMD_AVSYNC))
#define AUD_CMD_FLAG_LOADCODE   ((UINT32) (1<<(UINT32)AUD_CMD_LOADCODE))
#define AUD_CMD_FLAG_RESUME     ((UINT32) (1<<(UINT32)AUD_CMD_RESUME))
#define AUD_CMD_FLAG_STOPSTREAM ((UINT32) (1<<(UINT32)AUD_CMD_STOPSTREAM))
#define AUD_CMD_FLAG_CHANGE_FORMAT     ((UINT32) (1<<(UINT32)AUD_CMD_CHANGE_FORMAT))

#define AUD_DUAL_DEC_CMD_FLAG_PLAY  ((UINT32) (1<<(UINT32)AUD_DUAL_DEC_CMD_PLAY))
#define AUD_DUAL_DEC_CMD_FLAG_STOP  ((UINT32) (1<<(UINT32)AUD_DUAL_DEC_CMD_STOP))

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define DEC_NUM_VALIDATE(NUM)   \
    if (NUM > AUD_DEC_NUM)      \
    {                           \
        ASSERT(0);              \
    }                           \

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// AUD_IsDecoderPlay
//
/** Brief of AUD_IsDecoderPlay.
 *  Details of AUD_IsDecoderPlay (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)   
 *
 *  @retval     TRUE	        
 *  @retval     FALSE	        
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_IsDecoderPlay(UINT8 u1DecId);

//-----------------------------------------------------------------------------
// AUD_IsDecoderPlay
//
/** Brief of AUD_IsDecoderRealPlay.
 *  Details of AUD_IsDecoderRealPlay (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)   
 *
 *  @retval     TRUE	        
 *  @retval     FALSE	        
 */
//-----------------------------------------------------------------------------
BOOL AUD_IsDecoderRealPlay(UINT8 u1DecId);

//-----------------------------------------------------------------------------
// AUD_CommandDone
//
/** Audio notify command check.
 *
 *  @param  ucDecId   		Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eAudDecCmd   	audio command.
 *  @param  u4Result   		audio status result.		
 *
 *  @retval void
 */
 extern void AUD_CommandDone(UINT8 u1DecId,  AUD_DEC_CMD_T eAudDecCmd, UINT32 u4Result);

//-----------------------------------------------------------------------------
// AUD_WaitCommandDone
//
/** Audio wait notify command check.
 *
 *  @param  ucDecId   		Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eAudDecCmd   	audio command.
 *
 *  @retval void
 */  
extern void AUD_WaitCommandDone(UINT8 u1DecId, AUD_DEC_CMD_T eAudDecCmd);

extern void AUD_DualDecWaitCommandDone(AUD_DUAL_DEC_CMD_T eDualDecCmd, BOOL fgTimeOut);
extern void AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_T eAudDecCmd, UINT32 u4Result);

//-----------------------------------------------------------------------------
// AUD_Init
//
/** Aud driver initialization. This function will create driver thread and let 
 *  DSP go on.
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
extern INT32 AUD_Init(void);

//-----------------------------------------------------------------------------
// AUD_GetDecType
//
/** Get audio decoder type.
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peStreamFrom   Stream source
 *  @param      prDecType      Stream format
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
extern INT32 AUD_GetDecType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T * peStreamFrom, AUD_FMT_T * prDecType);

//-----------------------------------------------------------------------------
// AUD_SetDecMemBuffer
//
/** Brief of AUD_SetDecMemBuffer.
 *  Details of AUD_SetDecMemBuffer (optional).
 *
 *  @param      u1DecId           Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prDecMemBuffer    Memory buffer
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_SetDecMemBuffer(UINT8 u1DecId, const MEM_BUFFER_INFO_T * prDecMemBuffer);


//-----------------------------------------------------------------------------
// AUD_SetDecType
//
/** Set audio decoder type.
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      eStreamFrom    Stream source
 *  @param      eDecFmt        Stream format
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
extern INT32 AUD_SetDecType(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt);


//-----------------------------------------------------------------------------
// AUD_DSPCmdPlay
//
/** Brief of AUD_DSPCmdPlay.
 *  Issue play command to audio decoder, note that this is a synchronous play
 *  function
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdPlay(UINT8 u1DecId);

//-----------------------------------------------------------------------------
// AUD_DSPCmdPlayAsyn
//
/** Brief of AUD_DSPCmdPlayAsyn.
 *  Issue play command to audio decoder, note that this is a asynchronous play
 *  function. There will be a audio decoder notification when the command was done
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdPlayAsyn(UINT8 u1DecId);


//-----------------------------------------------------------------------------
// AUD_DSPCmdResume
//
/** Brief of AUD_DSPCmdResume.
 *  Details of AUD_DSPCmdResume (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdResume(UINT8 u1DecId);



//-----------------------------------------------------------------------------
// AUD_DSPCmdPause
//
/** Brief of AUD_DSPCmdPause.
 *  Details of AUD_DSPCmdPause (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdPause(UINT8 u1DecId);


//-----------------------------------------------------------------------------
// AUD_DSPCmdPauseAsyn
//
/** Brief of AUD_DSPCmdPauseAsyn.
 *  Details of AUD_DSPCmdPauseAsyn (optional).
 *
 *  @param[in]        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdPauseAsyn(UINT8 u1DecId);


//-----------------------------------------------------------------------------
// AUD_DSPCmdStop
//
/** Brief of AUD_DSPCmdStop.
 *  Details of AUD_DSPCmdStop (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdStop(UINT8 u1DecId);


//-----------------------------------------------------------------------------
// AUD_DSPCmdStopAsyn
//
/** Brief of AUD_DSPCmdStopAsyn.
 *  Details of AUD_DSPCmdStopAsyn (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdStopAsyn(UINT8 u1DecId);


//-----------------------------------------------------------------------------
// AUD_DSPCmdReset
//
/** Brief of AUD_DSPCmdReset.
 *  Details of AUD_DSPCmdReset (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdReset(UINT8 u1DecId);


//-----------------------------------------------------------------------------
// AUD_DSPCmdResetAsyn
//
/** Brief of AUD_DSPCmdResetAsyn.
 *  Details of AUD_DSPCmdResetAsyn (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdResetAsyn(UINT8 u1DecId);


//-----------------------------------------------------------------------------
// AUD_DSPCmdDualDecPlay
//
/** Brief of AUD_DSPCmdDualDecPlay.
 *  ONLY used in EURO mode
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdDualDecPlay(BOOL fgBlock);



//-----------------------------------------------------------------------------
// AUD_DSPCmdDualDecStop
//
/** Brief of AUD_DSPCmdDualDecStop.
 *  ONLY used in EURO mode
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdDualDecStop(BOOL fgBlock);


//-----------------------------------------------------------------------------
// AUD_DSPCmdDualDecSetDecType
//
/** Brief of AUD_DSPCmdDualDecSetDecType.
 *  ONLY used in EURO mode
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdDualDecSetDecType(AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt);



//-----------------------------------------------------------------------------
// AUD_RegNotifyFunc
//
/** Register audio notify function.
 *
 *  @param     pu4Handler   Notify function handler
 *  @param     pfNfyFunc    Notify function pointer
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
extern BOOL AUD_RegNotifyFunc(UINT32 * pu4Handler, AudDecNfyFct pfNfyFunc);



//-----------------------------------------------------------------------------
// AUD_DeRegNotifyFunc
//
/** Deregister audio notify function.
 *
 *  @param  u4Handler   Handler
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
extern BOOL AUD_DeRegNotifyFunc(UINT32 u4Handler);

//-----------------------------------------------------------------------------
// AUD_GetDecodeType
//
/** Brief of AUD_GetDecodeType.
 *  Details of AUD_GetDecodeType (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peStreamFrom     Stream source
 *  @param      peDecType        Stream format    
 */
//-----------------------------------------------------------------------------
extern void AUD_GetDecodeType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T * peStreamFrom, AUD_FMT_T * peDecType);


//-----------------------------------------------------------------------------
// AUD_SetAudClip
//
/** Brief of AUD_SetAudClip.
 *  Details of AUD_SetAudClip (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peAudClip        
 */
//-----------------------------------------------------------------------------
extern void AUD_SetAudClip(UINT8 u1DecId, AUD_CLIP_TYPE_T * peAudClip);



//-----------------------------------------------------------------------------
// AUD_PlayAudClip
//
/** Brief of AUD_PlayAudClip.
 *  Details of AUD_PlayAudClip (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      u4Channel        
 *  @param      fgPlayTestTone   
 */
//-----------------------------------------------------------------------------
extern void AUD_PlayAudClip(UINT8 u1DecId, UINT32 u4Channel, BOOL fgPlayTestTone);


//-----------------------------------------------------------------------------
// AUD_StopAudClip
//
/** Brief of AUD_StopAudClip.
 *  Details of AUD_StopAudClip (optional).
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 */
//-----------------------------------------------------------------------------
extern void AUD_StopAudClip(UINT8 u1DecId);
extern void Aud_SetAudClipVolume(UINT8 u1DecId, UINT32 u4Channel, UINT8 u1Volume);

//-----------------------------------------------------------------------------
// AUD_AvInputMuxSel
//
/** Brief of AUD_AvInputMuxSel.
 *  Details of AUD_AvInputMuxSel (optional).
 *
 *  @param  eInputId   AV component id
 *
 */
//-----------------------------------------------------------------------------
extern void AUD_AvInputMuxSel(AUD_INPUT_ID_T eInputId);


//-----------------------------------------------------------------------------
// AUD_SetCtrlType
//
/** Brief of AUD_SetCtrlType.
 *  Details of AUD_SetCtrlType (optional).
 *
 *  @param  u4MWSetCtrlType 
 *
 */
//-----------------------------------------------------------------------------
extern void AUD_SetCtrlType(UINT32 u4MWSetCtrlType);

//-----------------------------------------------------------------------------
// AUD_Mute
//
/** Brief of AUD_Mute.
 *  Details of AUD_Mute (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_Mute(UINT8 u1DecId);

#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0821
//-----------------------------------------------------------------------------
// AUD_MuteByPassCh()
//
/** Brief of AUD_MuteByPassCh().
 *  Details of AUD_MuteByPassCh ().
 *  
  *  @param      fgMute          1: mute, o:un-mute  
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_MuteByPassCh(BOOL fgMute);
#endif

//-----------------------------------------------------------------------------
// AUD_DtvLock
//
/** DTV audio lock check
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)  
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DtvLock(UINT8 u1DecId);

//-----------------------------------------------------------------------------
// AUD_SetAvSynMode
//
/** DTV audio sync check
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)  
 *  @param      eSynMode       Sync mode, 0:Freerun 1: Slave, 2:Master
 */
//-----------------------------------------------------------------------------
extern void AUD_SetAvSynMode(UINT8 u1DecId, AV_SYNC_MODE_T eSynMode);

//-----------------------------------------------------------------------------
// AUD_UnMute
//
/** Brief of AUD_UnMute.
 *  Details of AUD_UnMute (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_UnMute(UINT8 u1DecId);

//-----------------------------------------------------------------------------
// AUD_SendAudioPes
//
/** Brief of AUD_SendAudioPes.
 *  This is the callback function for dmx inform audio driver when retrieving 
 *  audio PES
 *
 *  @param      prPes          PES information     
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_SendAudioPes(const DMX_AUDIO_PES_T* prPes);

//-----------------------------------------------------------------------------
// AUD_PcmSetting
//
/** PCM format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prPcmSetting   
 */
//-----------------------------------------------------------------------------
extern void AUD_PcmSetting(UINT8 u1DecId, const AUD_PCM_SETTING_T * prPcmSetting);

//-----------------------------------------------------------------------------
/** AUD_DspFineTuneVolume
 *  Fine tune master volume/trim, and set it to DSP
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  eChannel  : 0: front left channel. 
 *                               1: front right channel.  
 *                               2: rear left channel. 
 *                               3: rear right channel. 
 *                               4: center channel.
 *                               5: sub woofer channel. 
 *                               6: bypass left channel. 
 *                               7: bypass right channel. 
 *                               8: downmix left channel. 
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ). 
 *  @param  u4ShmValue       Volume (0x20000 is equal to 0 dBFs, max value: 0x7fffff)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspFineTuneVolume(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4ShmValue);

//-----------------------------------------------------------------------------
/** AUD_DspVolumeInit
 *  Initialize all variables for volume control
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder). 
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspVolumeInit(UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspEqCfgInit
 *  Initialize variables for equalizer preset configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspEqCfgInit(void);

#ifdef CC_AUD_SOUND_MODE
extern void AUD_DspPSMCfgInit(void);
extern void AUD_DspPostSoundModeChange(UINT8 u1DecId,UINT8 u1ModeIdx);
extern void AUD_DspVSMCfgInit(void);
#endif
#ifdef CC_AUD_VOLUME_OFFSET
extern void AUD_DspVolumeOffset(UINT8 u1DecId,UINT8 u1Index);
#endif
extern void AUD_DspPostSoundMode(UINT8 u1DecId,UINT8 u1ModeIdx);
//-----------------------------------------------------------------------------
/** AUD_DspChDelayInit
 *  Initialize table for channel delay configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChDelayInit(void);


//-----------------------------------------------------------------------------
/** AUD_DspSpdifInit
 *  Initialize SPDIF configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSpdifInit(void);


//-----------------------------------------------------------------------------
// AUD_DspReverbEnable
//
/** turn on / off reverb control.
 *
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  fgEnable          "TRUE" for enable.
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
 extern void AUD_DspReverbEnable(UINT8 u1DecId, BOOL fgEnable);


//-----------------------------------------------------------------------------
/** AUD_DspReverbMode
 *  load the predefined reverberation parameters sets.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  bMode From mode 0 to mode 7. Mode 0 will turn the reverberation effect off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspReverbMode(UINT8 u1DecId, UINT8 u1Mode);


//-----------------------------------------------------------------------------
// AUD_DspChannelEqEnable
//
/** turn on /off the bass boost control
 *
 *  @param  u1DecId 		Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  fgEnable          "TRUE" for enable.
 *  @retval void
 */
 //-----------------------------------------------------------------------------
extern void AUD_DspChannelEqEnable(UINT8 u1DecId, BOOL fgEnable);



//-----------------------------------------------------------------------------
// AUD_DspSpkSizeCfg
//
/** Set the bass management cut-off frequency
 *
 *  @param  u1DecId 		Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  u4SpkSize         u2SpkSize in xx Hz.
 *  @retval void
 */
extern void AUD_DspSpkSizeCfg(UINT8 u1DecId, UINT32 u4SpkSize);

//-----------------------------------------------------------------------------
/** AUD_DspBassBoostEnable
 *  turn on /off the bass boost control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  fgEnable ADSP_ON / ADSP_OFF.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspBassBoostEnable(UINT8 u1DecId, BOOL fgEnable) ;


//-----------------------------------------------------------------------------
/** AUD_DspBassBoostGain
 *   set the bass boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  u1Level the setting value is from 0 to 28, level 14 means 0db. A large 
 *                         number will boost the bass and a smaller number will attenuate the bass.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspBassBoostGain(UINT8 u1DecId, UINT8 u1Level);


//-----------------------------------------------------------------------------
/** AUD_DspGetBassBoostGain
 *   get the bass boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *
 *  @retval bass gain value
 */
//-----------------------------------------------------------------------------
extern UINT32 AUD_DspGetBassBoostGain(UINT8 u1DecId);


//-----------------------------------------------------------------------------
/** AUD_DspTrebleBoostCtrl
 *   turn on / off treble boost control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  fgEnable  ADSP_ON / ADSP_OFF.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSuperBassEnable(UINT8 u1DecId, BOOL fgEnable);


//-----------------------------------------------------------------------------
// AUD_DspClearBoostEnable
//
/** turn on / off treble boost control
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  fgEnable  ADSP_ON / ADSP_OFF.
 *  @retval void
 */
extern void AUD_DspClearBoostEnable(UINT8 u1DecId, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspClearBoostGain
 *   set the clear boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  u1Level  range from 0 to 28, level 14 means 0db.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspClearBoostGain(UINT8 u1DecId, UINT8 u1Level);


//-----------------------------------------------------------------------------
/** AUD_DspGetClearBoostGain
 *   get the treble boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *
 *  @retval treble gain value
 */
//-----------------------------------------------------------------------------
extern UINT32 AUD_DspGetClearBoostGain(UINT8 u1DecId);


//-----------------------------------------------------------------------------
/** AUD_DspEqEnable
 *  turn on / off equalizer control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  fgEnable  TRUE / FALSE.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspEqEnable(UINT8 u1DecId, BOOL fgEnable);


//-----------------------------------------------------------------------------
// AUD_DspChEqBand
//
/** used to adjusting the equalizer's parameter for each channel
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  u1BandIdx  EQ band index
 *  @param  u1Level range is from 0 to 28
 *  @param  fgNotify  to indicate changing the parameter immediately or not
 *
 *  @retval void
 */
extern void AUD_DspChEqBand(UINT8 u1DecId, UINT8 u1BandIdx, UINT8 u1Level, BOOL fgNotify);



//-----------------------------------------------------------------------------
/** AUD_DspChEqLoadPreset
 *   load the predefined channel equalizer setting.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChEqLoadPreset(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType) ;


//-----------------------------------------------------------------------------
// AUD_DspChEqBandMode
//
/** used to adjusting the equalizer's parameter for each channel
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  u1BandIdx  EQ band index
 *  @param  u1Level range is from 0 to 28
 *  @param  fgNotify  to indicate changing the parameter immediately or not
 *
 *  @retval void
 */
extern void AUD_DspChEqBandMode(UINT8 u1DecId, UINT8 u1BandIdx, UINT8 u1Level, BOOL fgNotify);



//-----------------------------------------------------------------------------
/** AUD_DspChEqLoadPresetChgMode
 *   load the predefined channel equalizer setting for mode changing.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChEqLoadPresetChgMode(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType) ;


//-----------------------------------------------------------------------------
/** AUD_DspChSetEqTable
 *   set the band gain of a specific equalizer.
 *
 *  @param  u1DecId  Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @param  eqValue  band gain, valid value from -20 ~ 20
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChSetEqTable(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 eqValue[]);


//-----------------------------------------------------------------------------
/** AUD_DspChQryEqTable
 *   query the band gain of a specific equalizer.
 *
 *  @param  u1DecId  Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @param  eqValue  band gain, valid value from -20 ~ 20
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChQryEqTable(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 eqValue[]);

//-----------------------------------------------------------------------------
/** AUD_DspGetEqType
 *   get the type of current equalizer.
 *
 *  @param  u1DecId  Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  eEqType  
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspGetEqType(UINT8 u1DecId, AUD_EQ_TYPE_T* eEqType);

//-----------------------------------------------------------------------------
/** AUD_DspSetLimiterMode
 *   set limiter mode
 *
 *  @param  u1Mode  limiter mode (0: off, 1: adaptive mode, 2: fixed mode). 
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetLimiterMode(UINT8 u1Mode);

//-----------------------------------------------------------------------------
/** AUD_DspSetLimiterFlag
 *   set limiter flag
 *
 *  @param  u1Flag  limiter flag (0: off, 1: on)
 *                         bit 0: vol
 *                         bit 1: vsurr
 *                         bit 2: sbass
 *                         bit 3: EQ
 *                         bit 4: reverb
 *                         bit 5: speaker
 *                         bit 6: voice
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetLimiterFlag(UINT8 u1Flag);

//-----------------------------------------------------------------------------
/** AUD_DspSetLimiterThreshold
 *   set limiter threshold
 *
 *  @param  u4Thre  limiter threshold (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetLimiterThreshold(UINT32 u4Thre);

#ifdef CC_RATIO_LIMITER
//-----------------------------------------------------------------------------
/** AUD_DspSetLimiterGainRatio
 *   set limiter gain ratio
 *
 *  @param  u4Ratio  limiter gain ratio (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetLimiterGainRatio(UINT32 u4Ratio);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspGetLimiterConfig
 *   get limiter configuration
 *
 *  @param  *pu1Mode  mode
 *  @param  *pu2Flag  flag
 *  @param  *pu4Thre  threshold
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspGetLimiterConfig(UINT8 *pu1Mode, UINT16 *pu2Flag, UINT32 *pu4Thre);

//-----------------------------------------------------------------------------
/** AUD_DspPL2Config
 *  main processing routine for Prologic 2.
 *
 *  @param  uCtrl control
                         PL2CTRL_SWITCH          0.
                         PL2CTRL_MODE            1.
                         PL2CTRL_PANORAMA        2.
                         PL2CTRL_DIMENSION       3.  
                         PL2CTRL_C_WIDTH         4.
 *  @param  uMode corresponding parameters for PL2 control (uCtrl).
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspPL2Config(UINT8 uCtrl, UINT8 uMode);


//-----------------------------------------------------------------------------
/** AUD_DspPL2ConfigInit
 *  initialization for PL2 configuration.
 *
 *  @param  u2PL2cfg W_PLIICONFIG
 *  @param  uMode W_PLIIMODE.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspPL2ConfigInit(UINT16 u2PL2cfg,UINT16 u2PL2Mode) ;


//-----------------------------------------------------------------------------
/** AUD_DspAC3KaraMode
 *  setup for AC3 karaoke mode.
 *
 *  @param  u2KaraMode Karaoke mode
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAC3KaraMode(UINT16 u2KaraMode,UINT8 uDecIndx) ;



//-----------------------------------------------------------------------------
/** AUD_DspAC3DualMono
 *  setup for AC3 dual mono mode.
 *
 *  @param  u2DMMode dual mono mode
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAC3DualMono(UINT16 u2DMMode,UINT8 uDecIndx) ;


//-----------------------------------------------------------------------------
/** AUD_DspAC3CompMode
 *  setup for compression mode.
 *
 *  @param  u2CompMode compression mode.
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAC3CompMode(UINT16 u2CompMode,UINT8 uDecIndx) ;


//-----------------------------------------------------------------------------
/** AUD_DspGetAC3CompMode
 *  setup for compression mode.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval u2CompMode compression mode.
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_DspGetAC3CompMode(UINT8 uDecIndx) ;


//-----------------------------------------------------------------------------
/** AUD_DspAC3DRCRange
 *  setup range for dynamix range compression.
 *
 *  @param  uDRCLevel  DRC range.
 *                               0x00000000-->0.0
 *                               0x007FFFFF-->1.0
 *                               0.0 ~ 1.0 step 0.125
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAC3DRCRange(UINT8 uDRCLevel,UINT8 u1DecId);


//-----------------------------------------------------------------------------
/** AUD_DspGetAC3DRCRange
 *  setup range for dynamix range compression.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval uDRCLevel  DRC range.
 *                               0x00000000-->0.0
 *                               0x007FFFFF-->1.0
 *                               0.0 ~ 1.0 step 0.125
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetAC3DRCRange(UINT8 u1DecId);


//-----------------------------------------------------------------------------
/** AUD_DspSpeakerConfig
 *  speaker configuration setup.
 *
 *  @param  u4SpkCfg  Speaker config
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSpeakerConfig(UINT32 u4SpkCfg,UINT8 uDecIndex);


//-----------------------------------------------------------------------------
/** AUD_DspSpeakerSizeConfig
 *  speaker configuration setup.
 *
 *  @param  uDecIndex  Speaker config
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSpeakerSizeConfig(UINT8 uDecIndex, BOOL fgFrontLarge, BOOL fgSurroundLarge, BOOL fgCenterLarge);


//-----------------------------------------------------------------------------
/** AUD_DspGetSpeakerConfig
 *  Get speaker configuration
 *  @param  u1DecIndex          Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  pu4SpkCfg           Speaker configuration.
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspGetSpeakerConfig(UINT8 u1DecIndex, UINT32 *pu4SpkCfg);

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerLargeSmallConfig
 *  Speaker size configuration of each channel pair.
 *  @param  u1DecIndex          Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  u1FrontPair          Speaker size configuration for front left and right channels. (1: large, 0: small)
 *  @param  u2Center             Speaker size configuration for center channel. (1: large, 0: small)
 *  @param  u1Surround          Speaker size configuration for left and right surround channels. (1: large, 0: small)
 *  @param  u1CH910              Speaker size configuration for CH9/10 channels. (1: large, 0: small)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
// large(1)/small(0)
extern void AUD_DspSpeakerLargeSmallConfig(UINT8 u1DecIndex, UINT8 u1FrontPair, UINT8 u2Center, UINT8 u1Surround, UINT8 u1CH910);

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerLsConfig
 *  setup speaker size of each channel.
 *
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @param  u1L  large(1)/small(0).
 *  @param  u1R  large(1)/small(0).                       
 *  @param  u1Ls   large(1)/small(0).                      
 *  @param  u1Rs   large(1)/small(0).                      
 *  @param  u1C  large(1)/small(0).
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSpeakerLsConfig(UINT8 u1DecIndex, UINT8 u1L, UINT8 u1R, UINT8 u1Ls, UINT8 u1Rs, UINT8 u1C);


//-----------------------------------------------------------------------------
/** AUD_DspSpeakerOutputConfig
 *  set speaker output config.
 *
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @param  u1SpkCfg.  
 *                              bit 0 ~ 2:.
 *                              b000: 2/0 (LT/RT downmix: prologic compatible). 
 *                              b001: 1/0.
 *                              b010: 2/0 (LO/RO).
 *                              b011: 3/0.
 *                              b100: 2/1.
 *                              b101: 3/1.
 *                              b110: 2/2.
 *                              b111: 3/2.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSpeakerOutputConfig(UINT8 u1DecIndex, UINT8 u1SpkCfg);


//-----------------------------------------------------------------------------
/** AUD_DspSpeakerSubwooferEnable
 *  Turn On/Off subwoofer
 *  @param  u1DecIndex          Audio decoder id (0: first decoder, 1: second decoder). 
 *  @param  fgEnable              Subwoofer channel is turned on or off. (1: on, 0: off)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
// bit 5: Subwoofer Channel present(1)/absent(0)
extern void AUD_DspSpeakerSubwooferEnable(UINT8 u1DecIndex, BOOL fgEnable);


//-----------------------------------------------------------------------------
/** AUD_DspChannelDelay
 *  setup channel delay for individual channel.
 *
 *  @param  u2Delay 0.05 m/unit.
*  @param  eChIndex 0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)
 *                   8(Downmix L), 9(Downmix R), 10(AUD L), 11(AUD R), 13(All), 14(LFE)
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChannelDelay(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex);
#ifdef CC_ENABLE_AV_SYNC
extern void AUD_DspChannelDelay_initial(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex);
#endif



//-----------------------------------------------------------------------------
/** AUD_DspInputChannelDelay
 *  setup channel delay by input source.
 *
 *  @param  eStreamFrom
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspInputChannelDelay(AUD_DEC_STREAM_FROM_T eStreamFrom);


//-----------------------------------------------------------------------------
/** AUD_DspChannelVolume
 *  Get channel volume of individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel. 
 *                               1: front right channel.  
 *                               2: rear left channel. 
 *                               3: rear right channel. 
 *                               4: center channel.
 *                               5: sub woofer channel. 
 *                               6: bypass left channel. 
 *                               7: bypass right channel. 
 *                               8: downmix left channel. 
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ). 
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetChannelVolume(UINT8 u1DecId, AUD_CH_T eChannel);

//-----------------------------------------------------------------------------
/** _AUD_DspChannelVolShmRawValue
 *  Set master volume use share memory raw data .
 *
 *  Note that AUD_DspChannelVolume & _AUD_DspChannelVolShmRawValue can only select
 *  one to use
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4VolShm : 0 ~ 0x20000
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChannelVolShmRawValue(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4VolShm);

//-----------------------------------------------------------------------------
/** AUD_DspChannelVolume
 *  setup channel volume for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel. 
 *                               1: front right channel.  
 *                               2: rear left channel. 
 *                               3: rear right channel. 
 *                               4: center channel.
 *                               5: sub woofer channel. 
 *                               6: bypass left channel. 
 *                               7: bypass right channel. 
 *                               8: downmix left channel. 
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ). 
 *  @param  u1Value 0~100.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChannelVolume(UINT8 u1DecId, AUD_CH_T eChannel, UINT8 u1Value);

#ifdef CC_AUD_LINEOUT_VOL_OFFSET
//-----------------------------------------------------------------------------
/** AUD_DspLineOutVolOffset
 *  setup bypass channel volume offset 
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Value 0~100.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspLineOutVolOffset(UINT8 u1DecId,UINT8 u1Value);
#endif
//-----------------------------------------------------------------------------
/** AUD_DspWMAHeader
 *  setup WMA information
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  prWmaInfo  : pointer of WMA header information
 *  @retval void
 */
//-----------------------------------------------------------------------------
#ifdef DSP_WMA_MWIF
extern void AUD_DspWMAHeader(UINT8 u1DecId, AUD_WMA_CFG_T* prWmaInfo);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspChannelMute
 *  setup mute for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel. 
 *                               1: front right channel.  
 *                               2: rear left channel. 
 *                               3: rear right channel. 
 *                               4: center channel.
 *                               5: sub woofer channel. 
 *                               6: bypass left channel. 
 *                               7: bypass right channel. 
 *                               8: downmix left channel. 
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ). 
 *  @param  fgMute TRUE, FALSE
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChannelMute(UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgMute);

//-----------------------------------------------------------------------------
/** AUD_DspChannelVolGain
 *  setup channel volume extra gain for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel. 
 *                               1: front right channel.  
 *                               2: rear left channel. 
 *                               3: rear right channel. 
 *                               4: center channel.
 *                               5: sub woofer channel. 
 *                               6: bypass left channel. 
 *                               7: bypass right channel. 
 *                               8: downmix left channel. 
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ). 
 *  @param  i4Value     -256~96.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChannelVolGain(UINT8 u1DecId, AUD_CH_T eChannel, INT32 i4Value);

//-----------------------------------------------------------------------------
/** AUD_DspChannelOutPort
 *  setup output port for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel. 
 *                               1: front right channel.  
 *                               2: rear left channel. 
 *                               3: rear right channel. 
 *                               4: center channel.
 *                               5: sub woofer channel. 
 *                               6: bypass left channel. 
 *                               7: bypass right channel. 
 *                               8: downmix left channel. 
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ). 
 *  @param  fgEnable TRUE, FALSE
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChannelOutPort(UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspBalance
 *  setup balance.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Balance  : balance value (0~100).
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspBalance(UINT8 u1DecId, UINT8 u1Balance);

//-----------------------------------------------------------------------------
/** AUD_DspMasterVolumeFineTune
 *  setup master volume to DSP directly.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4Volume  : volume value (0~0x7fffff).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMasterVolumeFineTune(UINT8 u1DecId, UINT32 u4Volume);

//-----------------------------------------------------------------------------
/** AUD_DspMasterVolShmRawValue
 *  Set master volume use share memory raw data .
 *
 *  Note that AUD_DspMasterVolume & _AUD_DspMasterVolShmRawValue can only select
 *  one to use
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4VolShm : 0 ~ 0x20000
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMasterVolShmRawValue(UINT8 u1DecId, UINT32 u4VolShm);

//-----------------------------------------------------------------------------
/** AUD_DspMasterVolume
 *  set master volume.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Volume  : volume value (0~100).
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMasterVolume(UINT8 u1DecId, UINT8 u1Volume);


//-----------------------------------------------------------------------------
/** AUD_DspAVCEnable
 *  enable AVC.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : on/off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAVCEnable(UINT8 u1DecId, BOOL fgEnable);


//-----------------------------------------------------------------------------
/** AUD_DspChangeAVCPara
 *  setup automatic volume control parameters.
 *
 *  @param  u2Type     parameter selection
 *                             0: AVC on/off (u2Value = 0: AVC off, 1: AVC on)
 *                             1: AVC target level (dB) (u2Value = -6 ~ -24)
 *                             3: Max gain up (dB) (u2Value = 0 ~ 8)
 *                             5: Adjust rate (dB/128ms)
 *                             6: Adjust rate when AVC is turned on in UI (dB/128ms)
 *  @param  u2Value    parameter value
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChangeAVCPara(UINT16 u2Type, INT16 u2Value, UINT8 u1DecId);
extern void AUD_DspGetAVCPara(UINT16 u2Type, UINT16* u2Value, UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspInputChangeAVCPara
 *  setup automatic volume control parameters by input source.
 *
 *  @param  eStreamFrom
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspInputChangeAVCPara(AUD_DEC_STREAM_FROM_T eStreamFrom);

//-----------------------------------------------------------------------------
/** AUD_DspChange2BandAVCPara
 *  setup automatic volume control parameters.
 *
 *  @param  u2Type     parameter selection
 *                             0: AVC on/off (u2Value = 0: AVC off, 1: AVC on)
 *                             1: AVC target level (dB) (u2Value = -6 ~ -24)
 *                             3: Max gain up (dB) (u2Value = 0 ~ 8)
 *                             5: Adjust rate (dB/128ms)
 *                             6: Adjust rate when AVC is turned on in UI (dB/128ms)
 *  @param  u2Value    parameter value
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChange2BandAVCPara(UINT8 u1Band, UINT16 u2Type, 
                                                UINT16 u2Value, UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspMuteEnable
 *  audio mute control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMuteEnable(UINT8 u1DecId, BOOL fgEnable);


//-----------------------------------------------------------------------------
/** AUD_DspHdmiModeMuteEnable
 *  audio mute control in HDMI mode.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspHdmiModeMuteEnable(UINT8 u1DecId, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspBassMngEnalbe
 *  Bass management control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Bass management enable. (1: enable, 0: disable)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspBassMngEnalbe(UINT8 u1DecId, BOOL fgEnable);


//-----------------------------------------------------------------------------
/** AUD_DspProcMode
 *  setup for compression mode.
 *
 *  @param  u2CompMode compression mode.
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspProcMode(UINT8 u1DecId, UINT16 u2Mode);


//-----------------------------------------------------------------------------
/** AUD_DspVirtualSurroundEnalbe
 *  enable virtual surround.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : on/off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
#ifndef CC_NEW_POST_MULTI_VS_SUPPORT
extern void AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, BOOL fgEnable);
#else
extern void AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, UINT8 u1ModeIdx);
extern void AUD_DspVSModeChange(UINT8 u1DecId,UINT8 u1ModeIdx);
#endif
/** AUD_DspBbeMode
 *  Set BBE mode.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1BbeMode  : 0: OFF, 1: BBE, 2: BBE ViVA4.
 *  @retval void
 */
extern void AUD_DspBbeMode(UINT8 u1DecId, UINT8 u1BbeMode);
#ifdef CC_SET_VOLUME
extern void AUD_DspSetVolTable(UINT8 u1Idx, UINT32  u4Volume);
#endif
#ifdef CC_AUD_NEW_CV_TABLE
extern void AUD_DspSetCVTable(UINT8 u1Idx, UINT32  u4Volume);
extern UINT32 AUD_DspGetCVTable(UINT8 u1VolTabIdx);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspMonoDownMix
 *  enable mono downmix.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : on/off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMonoDownMix(UINT8 u1DecId, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspSetKaraokeMixRatio
 *  Set karaoke mix ratio
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4MixRatio  : 0x0 ~ 0x800000 (for R channel)
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetKaraokeMixRatio(UINT8 u1DecId, UINT32 u4MixRatio);


//-----------------------------------------------------------------------------
/** AUD_DspSetMatrixOutMode
 *  setup for matrix output mode.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eMode 0: stereo (disable), 1: L mono, 2: R mono, 3: mix mono
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetMatrixOutMode(UINT8 u1DecId, AUD_MATRIX_OUT_T eMode) ;


//-----------------------------------------------------------------------------
/** AUD_DspAutoDmx
 *  Setup auto downmix mode for AC3
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : 0: disable AC3 auto downmix, 1: enable AC3 auto downmix
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAutoDmx(UINT8 u1DecId, BOOL fgEnable);


//-----------------------------------------------------------------------------
/** AUD_DspBypassPostProcess
 *  Bypass audio post-processing.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgBypass  : 1: bypass post-processing.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspBypassPostProcess(UINT8 u1DecId, BOOL fgBypass);

//-----------------------------------------------------------------------------
/** AUD_DspSoundEffectFlag
 *  Enable all audio sound effect. (EQ, Bass/Treble, Reverb, Surround, AVC)
 *
 *  @param  u1DecId  0: first decoder 1: seconder decoder.
 *  @param  fgFlag     0: disable, 1: enable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSoundEffectFlag(UINT8 u1DecId, BOOL fgFlag);



//-----------------------------------------------------------------------------
/** AUD_DspIECConfig
 *  Routine handling IEC Configuration.
 *
 *  @param  eIecCfg  0: PCM; 1:RAW.
 *  @param  fgEnable  on/off.
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspIECConfig(AUD_IEC_T eIecCfg, BOOL fgEnable);


//-----------------------------------------------------------------------------
/** AUD_DspIecChannel
 *  Routine handling IEC Configuration.
 *
 *  @param  eIecChannel  select audio channel,ex L/R.
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspIecChannel(AUD_IEC_CH_T eIecChannel);

//-----------------------------------------------------------------------------
/** AUD_DspIecCopyProtection
 *  Routine handling IEC Configuration.
 *
 *  @param  u1Enable 0: disable 1: enable
 *  @param  u1CategoryCode
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspIecCopyright(UINT8 u1Enable, UINT8 u1CategoryCode);


//-----------------------------------------------------------------------------
/** AUD_DspGetIecCopyright
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetIecCopyright(AUD_DEC_STREAM_FROM_T eStreamFrom);


//-----------------------------------------------------------------------------
/** AUD_DspGetIecCategoryCode
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  category code value
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetIecCategoryCode(AUD_DEC_STREAM_FROM_T eStreamFrom);


//-----------------------------------------------------------------------------
/** AUD_DspMixsoundControl
 *  Mix sound control.
 *
 *  @param  u1Mode play/stop.
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMixsoundControl(UINT16 u1Mode);



//-----------------------------------------------------------------------------
/** AUD_DspSetDemodOutputMode
 *  Set MTS output mode.
 *
 *  @param  u1Output    0: Output (Mono,Mono)
 *                               1: If stereo exists, Output (L,R) otherwise (Mono,Mono)
 *                               2: If SAP exists, Output (S,S) otherwise (L,R) otherwise (Mono,Mono)
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetDemodOutputMode(UINT8 u1Output);


//-----------------------------------------------------------------------------
// AUD_DspSetSRSWOW
//
/** Set SRS WOW mode
 *
 *  @param[in]     u1Mode   SV_OFF|SV_ON
 *
 *  @retval 	none
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSRSWOW(UINT8 u1Mode);


//-----------------------------------------------------------------------------
// AUD_DspSetSRSWOWSpkSz
//
/** Set SRS WOW Speaker size
 *
 *  @param[in]     u1Mode   Speaker size table: 0: 40Hz, 1: 60Hz, 2: 100Hz, 3: 150Hz, 4: 200Hz, 5: 250Hz, 6: 300Hz, 7: 400Hz
 *
 *  @retval 	none
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSRSWOWSpkSz(UINT8 u1Mode);


//-----------------------------------------------------------------------------
// AUD_DspSetSRSWOWBright
//
/** Set SRS WOW Brightness
 *
 *  @param[in]     u1Mode   Brightness: 0:0%, 1:20%, 2:40%, 3:100%
 *
 *  @retval 	none
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSRSWOWBright(UINT8 u1Mode);

//-----------------------------------------------------------------------------
// AUD_DspSetSRSWOWTBLvl
//
/** Set SRS WOW TrueBass Value
 *
 *  @param[in]     u1Mode   TrueBass value: 0:0%, 1:50%, 2:60%, 3:100%
 *
 *  @retval 	none
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSRSWOWTBLvl(UINT8 u1Mode);

//-----------------------------------------------------------------------------
// AUD_DspSetSRSWOWWidth
//
/** Set SRS WOW Width
 *
 *  @param[in]     u1Mode   TrueBass value: 0:0%, 1:30%, 2:80%, 3:100%
 *
 *  @retval 	none
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSRSWOWWidth(UINT8 u1Mode);

//-----------------------------------------------------------------------------
// AUD_DspSetSRSTSXTSpkSz
//
/** Set SRS TSXT Speaker Size
 *
 *  @param[in]     u1Mode   Speaker size table: 0: 40Hz, 1: 60Hz, 2: 100Hz, 3: 150Hz, 4: 200Hz, 5: 250Hz, 6: 300Hz, 7: 400Hz
 *
 *  @retval 	none
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSRSTSXTSpkSz(UINT8 u1Mode);


//-----------------------------------------------------------------------------
// AUD_DspSetSRSTSXTSetMode
//
/** Set SRS TSXT Mode
 *
 *  @param[in]     u1Mode   Mode: 0:off, 1:mono -> stereo, 2:stereo
 *
 *  @retval 	none
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSRSTSXTSetMode(UINT8 u1Mode);


//-----------------------------------------------------------------------------
// AUD_DspSetSRSTSXTElev
//
/** Set SRS TSXT Elevation Level
 *
 *  @param[in]     u1Mode   Elevation Level: 0:0%, 1:10%, 2:50%, 3:100%
 *
 *  @retval 	none
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSRSTSXTElev(UINT8 u1Mode);


//-----------------------------------------------------------------------------
// AUD_DspSetSRSTSXTTBLvl
//
/** Set SRS TSXT TrueBass Level
 *
 *  @param[in]     u1Mode   TrueBass Level: 0:0%, 1:38%, 2:60%, 3:100%
 *
 *  @retval 	none
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSRSTSXTTBLvl(UINT8 u1Mode);


//-----------------------------------------------------------------------------
/** AUD_DspSetDetectDefaultMode
 *   call this function before issue detection command.
 *
 *  @param  u1Mode 0: PAL_DK, 4: SECAM-L.
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetDetectDefaultMode(UINT8 u1Mode);


//-----------------------------------------------------------------------------
/** AUD_DspSetUserDefinedEqCfg
 *   setup user difined EQ config.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eEqType  0: off   others: 1~12.
 *  @param  u1BandIdx  0~4.
 *  @param  i1Value -100 ~ +100
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetUserDefinedEqCfg(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, UINT8 u1BandIdx, INT8 i1Value);


//-----------------------------------------------------------------------------
/** _AUD_DspSetUserDefinedSuitEqCfg
 *   setup user difined EQ config.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eEqType  0: off   others: 1~12.
 *  @param  i1Band1Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band2Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band3Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band4Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band5Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetUserDefinedSuitEqCfg(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 i1Band1Value, 
                                 INT8 i1Band2Value, INT8 i1Band3Value, INT8 i1Band4Value, 
                                 INT8 i1Band5Value);

//-----------------------------------------------------------------------------
/** _AUD_DspSetUserDefinedSuitEqCfg
 *   setup user difined EQ config.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eEqType  0: off   others: 1~12.
 *  @param  i1Band1Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band2Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band3Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band4Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band5Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band6Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @param  i1Band7Value -20 ~ +20  (range -10 ~ +10 dB, 0.5 dB/step).
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetUserDefinedSuitEqCfg7Bands(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 i1Band1Value, 
                                 INT8 i1Band2Value, INT8 i1Band3Value, INT8 i1Band4Value, 
                                 INT8 i1Band5Value, INT8 i1Band6Value, INT8 i1Band7Value);


//-----------------------------------------------------------------------------
/** AUD_DspSetUserDefinedEqCfg
 *   Get user difined EQ config.
 *
 *  @param  eEqType  0: off   others: 1~12.
 *  @param  u1BandIdx  0~4.
 *  @param  i1Value (-20 ~ +20)
 *
 *  @retval  TRUE: success
 *              FALSE: failed
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetUserDefineEqCfg(AUD_EQ_TYPE_T eEqType, UINT8 u1BandIdx, INT8* pi1Value);


//-----------------------------------------------------------------------------
/** AUD_DspGetUserEqBandNum
 *   Get user defined band number.
 *   
 *  @retval  CH_EQ_BAND_NO = 5.
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetUserEqBandNum(void);


//-----------------------------------------------------------------------------
/** AUD_DspGetEqBandInfo
 *   Get user defined band info.
 *   
 *  @param  prEqBandInfo  structure of eq band info (AUD_EQ_BAND_INFO_T).
 *  @retval  void 
 */
//-----------------------------------------------------------------------------
extern void AUD_DspGetEqBandInfo(AUD_EQ_BAND_INFO_T* prEqBandInfo);


//-----------------------------------------------------------------------------
/** AUD_DspGetSrcVol
 *   get source volume.
 *   
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner. 
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner. 
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_BUF_AGT,            //Buffer Agent.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media
                                   
 *  @retval  input source volume 
 */
//-----------------------------------------------------------------------------
extern INT16 AUD_DspGetSrcVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc);
//-----------------------------------------------------------------------------
/** AUD_DspSetSrcVol
 *   set source volume.
 *   
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner.
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner.
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_BUF_AGT,            //Buffer Agent.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media
                                   
 *  @param  i2Vol  -128 ~ 96                          
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSrcVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol);

//-----------------------------------------------------------------------------
/** AUD_DspVolumeChange
 *   Setup master volume, or input source gain, or channel trim to DSP.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  eAudChl  : 0: front left channel. 
 *                              1: front right channel.  
 *                              2: rear left channel. 
 *                              3: rear right channel. 
 *                              4: center channel.
 *                              5: sub woofer channel. 
 *                              6: bypass left channel. 
 *                              7: bypass right channel. 
 *                              8: downmix left channel. 
 *                              9: downmix right channel.
 *                            10: aux front left channel.
 *                            11: aux front right channel.
 *                            12: input source gain.
 *                            13: all channels    ( For master volume control ). 
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspVolumeChange(UINT8 u1DecId, AUD_CH_T eAudChl);

//-----------------------------------------------------------------------------
/** AUD_DspSetAvSrcVol
 *   set AV source volume.
 *   
 *  @param  eInputSrcId         AUD_INPUT_ID_COMP_VID_0 = 0,
                                          AUD_INPUT_ID_COMP_VID_1,
                                          AUD_INPUT_ID_COMP_VID_2,
                                          AUD_INPUT_ID_COMP_VID_3,
                                          AUD_INPUT_ID_S_VID_0,
                                          AUD_INPUT_ID_S_VID_1,
                                          AUD_INPUT_ID_S_VID_2,
                                          AUD_INPUT_ID_YPBPR_0,
                                          AUD_INPUT_ID_YPBPR_1,
                                          AUD_INPUT_ID_YPBPR_2,
                                          AUD_INPUT_ID_VGA_0,
                                          AUD_INPUT_ID_VGA_1,
                                          AUD_INPUT_ID_HDMI_0,
                                          AUD_INPUT_ID_HDMI_1,
                                          AUD_INPUT_ID_HDMI_2,
                                          AUD_INPUT_ID_HDMI_3,
                                          AUD_INPUT_ID_DVI_0,
                                          AUD_INPUT_ID_DVI_1,
                                          AUD_INPUT_ID_DVI_2,
                                          AUD_INPUT_ID_DVI_3,
                                          AUD_INPUT_ID_SCART_0,
                                          AUD_INPUT_ID_SCART_1,
                                          AUD_INPUT_ID_SCART_2, 
                                          AUD_INPUT_ID_SCART_3,
                                          AUD_INPUT_ID_AUXIN_0,
                                          AUD_INPUT_ID_AUXIN_1,
 *  @param  i2Vol  -256 ~ 256.                          
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetAvSrcVol(AUD_INPUT_ID_T eInputSrcId , INT16 i2Vol);

extern AUD_INPUT_ID_T AUD_DspGetAvInputSrcId(void);

//-----------------------------------------------------------------------------
/** AUD_DspGetAvInputSrcDvdId
 *
 *                 
 *  @retval  
 */
//-----------------------------------------------------------------------------
extern AUD_INPUT_ID_T AUD_DspGetAvInputSrcDvdId(void);

//-----------------------------------------------------------------------------
/** AUD_DspGetAtvTvSys
 *
 *  @param  prTvAudSys  tv audio system structure.               
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspGetAtvTvSys(MW_TV_AUD_SYS_T * prTvAudSys);

//-----------------------------------------------------------------------------
/** AUD_DspGetAtvTvSystem
 *
 *  @param void
 *  @retval  TV_AUD_SYS_T
 */
//-----------------------------------------------------------------------------
extern TV_AUD_SYS_T AUD_DspGetAtvTvSystem(void);


//-----------------------------------------------------------------------------
/** AUD_DspSetTvSrcMode
 *   set tv source mode.
 *   
 *  @param  eAudSys      SV_NONE_DETECTED = 0x0,  
                                   SV_MTS           = 0x1,    
                                   SV_FM_FM         = 0x2,     
                                   SV_NTSC_M        = 0x3,   
                                   SV_A2_BG         = 0x4,    
                                   SV_A2_DK1        = 0x5,     
                                   SV_A2_DK2        = 0x6,   
                                   SV_A2_DK3        = 0x7,     
                                   SV_PAL_I         = 0x8,     
                                   SV_PAL_BG        = 0x9,    
                                   SV_PAL_DK        = 0xa,     
                                   SV_SECAM_L       = 0xb,    
                                   SV_SECAM_L_PLUM  = 0xc,    
                                   SV_SECAM_BG      = 0xd,    
                                   SV_SECAM_DK      = 0xe    
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetTvSrcMode(TV_AUD_SYS_T eAudSys);
extern TV_AUD_SYS_T AUD_DspGetTvSrcMode(void);
#ifdef CC_AUD_TV_SYS_FINE_TUNE_VOL
extern	void AUD_SetTVSysFineTuneVol(TV_AUD_SYS_T eAudSys);
#endif
//-----------------------------------------------------------------------------
/** AUD_DspSetAtvOutputMode
 *   set tv source mode.
 *   
 *  @param  u1DecId 0:first decoder 1: seconder decoder.
 *  @param  eSoundMode         UNKNOWN = 0,
                                           MONO,
                                           STEREO,
                                           SUB_LANG,
                                           DUAL1,
                                           DUAL2,
                                           NICAM_MONO,
                                           NICAM_STEREO,
                                           NICAM_DUAL1,
                                           NICAM_DUAL2,
                                           A2_DUAL1,
                                           A2_DUAL2,
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetAtvOutputMode(UINT8 u1DecId, AUD_SOUND_MODE_T eSoundMode);


//-----------------------------------------------------------------------------
/** AUD_DspHdmiSpdifOutEnable
 *   SPDIF output control in HDMI mode.
 *   
 *  @param  fgEnable    0: disable SPDIF output, 1: enable SPDIF output
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspHdmiSpdifOutEnable(BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspGetSoundMode
 *   Get ATV sound mode
 *   
 *  @param  u1DecId 0:first decoder 1: seconder decoder.
 *
 *  @retval AUD_SOUND_MODE_T
 */
//-----------------------------------------------------------------------------
extern AUD_SOUND_MODE_T AUD_DspGetSoundMode(UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspGetSpectrumInfo
 *   Get audio spectrum information.
 *   
 *  @param  u1DecId 0:first decoder 1: seconder decoder.
 *  @param  u1BandNum      1~15 .  
 *  @retval  u4Spectrum
 */
//-----------------------------------------------------------------------------
extern UINT32 AUD_DspGetSpectrumInfo(UINT8 u1DecId, UINT8 u1BandNum);


//-----------------------------------------------------------------------------
/** AUD_DspSpectrumEnable
 *   Enable audio spectrum calculation.
 *   
 *  @param  u1DecId    0:first decoder 1: seconder decoder.
 *  @param  fgEnable    0: disable calculation, 1: enable calculation.
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSpectrumEnable(UINT8 u1DecId, BOOL fgEnable);


//-----------------------------------------------------------------------------
/** AUD_AudioSetStcDiffBound
 *   Set STC PTS difference bound. Bound would be (x MS * (1+uStcDiff)).
 *   
 *  @param  u1DecId      0:first decoder 1: seconder decoder.
 *  @param  uStcDiffLo    for low boundary of stc diff (0~255) . 
 *  @param  uStcDiffHi    for high boundary of stc diff (0~255) . 
 *  @param  uStcDiffWs   for worst boundary of stc diff (0~255) . 
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_AudioSetStcDiffBound(UINT8 u1DecId, UINT8 uStcDiffLo, UINT8 uStcDiffHi, UINT8 uStcDiffWs);


//-----------------------------------------------------------------------------
/** AUD_DDBannerEnable
 *   Turn on this funtion to close some effect and post processing,  for DD test only.
 *   
 *  @param  u1DecId      0:first decoder 1: seconder decoder.
 *  @param  fgEnable     0: disable, 1: enable .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DDBannerEnable(UINT8 u1DecId, BOOL fgEnable);


//-----------------------------------------------------------------------------
/** AUD_DspSetPalDetection
 *   demodulation related configuration.
 *   
 *  @param  eChangedItem    
 *  @param  u2Value   
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetPalDetection ( AUD_FACTORY_PAL_DETECTION_ITEM_T eChangedItem, UINT16 u2Value);



//-----------------------------------------------------------------------------
/** AUD_DspSetPalDetection
 *   demodulation related configuration.
 *   
 *  @param  eItem   
 *
 *  @retval  
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_DspGetPalDetection ( AUD_FACTORY_PAL_DETECTION_ITEM_T eItem);


//-----------------------------------------------------------------------------
/** AUD_DspSetA2Detection
 *   demodulation related configuration.
 *   
 *  @param  eItem   
 *
 *  @retval  
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetA2Detection ( AUD_FACTORY_A2_DETECTION_ITEM_T eItem, UINT16 u2Value);



//-----------------------------------------------------------------------------
/** AUD_DspGetA2Detection
 *   demodulation related configuration.
 *   
 *  @param  eItem   
 *
 *  @retval  
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_DspGetA2Detection ( AUD_FACTORY_A2_DETECTION_ITEM_T eItem);



//-----------------------------------------------------------------------------
/** AUD_DspSetA2Threshold
 *   demodulation related configuration.
 *   
 *  @param  eItem   
 *
 *  @retval  
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetA2Threshold ( AUD_FACTORY_A2_THRESHOLD_ITEM_T eItem, UINT16 u2Value);



//-----------------------------------------------------------------------------
/** AUD_DspGetA2Threshold
 *   demodulation related configuration.
 *   
 *  @param  eItem   
 *
 *  @retval  
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_DspGetA2Threshold ( AUD_FACTORY_A2_THRESHOLD_ITEM_T  eItem);



//-----------------------------------------------------------------------------
/** AUD_DspSetMtsDetection
 *   Set NPTV MTS Detection Configurations.
 *   
 *  @param  eChangedItem		   
 *  @param  u2Value			   
 *
 *  @retval  
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetMtsDetection ( AUD_FACTORY_MTS_DETECTION_ITEM_T eChangedItem,UINT16 u2Value)  ;



//-----------------------------------------------------------------------------
/** AUD_DspGetMtsDetection
 *   Set NPTV MTS Detection Configurations.
 *   
 *  @param  eItem		   
 *
 *  @retval  
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_DspGetMtsDetection ( AUD_FACTORY_MTS_DETECTION_ITEM_T eItem);


//-----------------------------------------------------------------------------
/** AUD_DspSetFmfmDetection
 *   demodulation related configuration.
 *   
 *  @param  eChangedItem		   
 *  @param  u2Value		   
 *
 *  @retval  
 */
//-----------------------------------------------------------------------------
void AUD_DspSetFmfmDetection ( AUD_FACTORY_FMFM_DETECTION_ITEM_T eChangedItem, UINT16 u2Value);


//-----------------------------------------------------------------------------
/** AUD_DspGetFmfmDetection
 *   demodulation related configuration.
 *   
 *  @param  eItem		   
 *
 *  @retval  
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_DspGetFmfmDetection ( AUD_FACTORY_FMFM_DETECTION_ITEM_T eItem) ;

//-----------------------------------------------------------------------------
/** AUD_DspSetHdevMode
 *   set high deveation mode on/off.
 *   
 *  @param  eDecType	
 *			bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *  @param  fgEnable		   
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetHdevMode ( AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspGetHdevMode
 *   set high deveation mode on/off.
 *   
 *  @param  eDecType	
 *			bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *  @param  fgEnable		   
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetHdevMode ( AUD_ATV_DECODER_TYPE_T eDecType);


//-----------------------------------------------------------------------------
/** AUD_DspSetAmMute
 *   set AM mute mode on/off and thresholds.
 *   
 *  @param  eChangedItem	
 *  @param  u1Value		   
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetAmMute( AUD_FACTORY_PAL_AM_MUTE_ITEM_T eChangedItem, UINT8 u1Value);


//-----------------------------------------------------------------------------
/** AUD_DspGetAmMute
 *   set AM mute mode on/off and thresholds.
 *   
 *  @param  eItem	
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetAmMute( AUD_FACTORY_PAL_AM_MUTE_ITEM_T eItem);

//-----------------------------------------------------------------------------
/** AUD_DspSetFmMute
 *   set FM mute mode on/off and thresholds.
 *   
 *  @param  eDecType	
 *			bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *  @param  eChangedItem
 *  @param  u1Value		   
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetFmMute ( AUD_ATV_DECODER_TYPE_T eDecType, 
                                                            AUD_FACTORY_FM_MUTE_ITEM_T eChangedItem, 
                                                                  UINT8 u1Value) ;

//-----------------------------------------------------------------------------
/** AUD_DspGetFmMute
 *   set FM mute mode on/off and thresholds.
 *   
 *  @param  eDecType	
 *			bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *  @param  eChangedItem
 *  @param  u1Value		   
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetFmMute ( AUD_ATV_DECODER_TYPE_T eDecType, 
                                                            AUD_FACTORY_FM_MUTE_ITEM_T eChangedItem) ;

//-----------------------------------------------------------------------------
/** AUD_DspSetCarrierShiftMode
 *   set carrier shift mode on/off .
 *   
 *  @param  eDecType	
 *			bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *  @param  fgEnable		   
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetCarrierShiftMode ( AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable);


//-----------------------------------------------------------------------------
/** AUD_DspGetCarrierShiftMode
 *   set carrier shift mode on/off .
 *   
 *  @param  eDecType	
 *			bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *
 *  @retval  TRUE/FAUSE
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetCarrierShiftMode (AUD_ATV_DECODER_TYPE_T eDecType);

//-----------------------------------------------------------------------------
/** AUD_DspSetNonEUMode
 *   set Non-EU mode on/off.
 *   
 *  @param  fgEnable	TRUE/FALSE
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetNonEUMode (BOOL fgEnable) ;


//-----------------------------------------------------------------------------
/** AUD_DspGetNonEUMode
 *   Get Non-EU mode on/off.
 *   
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetNonEUMode (void);


//-----------------------------------------------------------------------------
/** AUD_DspSetFMSatuMuteMode
 *   demodulation related configuration.
 *  @param  fgEnable	TRUE/FALSE
 *  @param  eDecType	
 *			bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetFMSatuMuteMode (AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable) ;



//-----------------------------------------------------------------------------
/** AUD_DspGetFMSatuMuteMode
 *   demodulation related configuration.   
 *  @param  eDecType	
 *			bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetFMSatuMuteMode (AUD_ATV_DECODER_TYPE_T eDecType) ;

//-----------------------------------------------------------------------------
/** AUD_DspSetPALFineVolume
 *   Set fine tune volume for PAL.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetPALFineVolume (UINT8 u1Index) ;


//-----------------------------------------------------------------------------
/** AUD_DspGetPALFineVolume
 *   Get fine tune volume for PAL.
 *     
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetPALFineVolume (void) ;


//-----------------------------------------------------------------------------
/** AUD_DspSetNicamFineVolume
 *   Set fine tune volume for NICAM.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetNicamFineVolume (UINT8 u1Index) ;


//-----------------------------------------------------------------------------
/** AUD_DspGetNicamFineVolume
 *   Get fine tune volume for NICAM.
 *     
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetNicamFineVolume (void) ;


//-----------------------------------------------------------------------------
/** AUD_DspSetAMFineVolume
 *   Set fine tune volume for AM.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetAMFineVolume (UINT8 u1Index);

//-----------------------------------------------------------------------------
/** AUD_DspGetAMFineVolume
 *   Get fine tune volume for AM.
 *     
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetAMFineVolume (void) ;


//-----------------------------------------------------------------------------
/** AUD_DspSetA2FineVolume
 *   Set fine tune volume for A2.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetA2FineVolume (UINT8 u1Index) ;


//-----------------------------------------------------------------------------
/** AUD_DspGetA2FineVolume
 *   Get fine tune volume for A2.
 *     
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetA2FineVolume (void) ;

//-----------------------------------------------------------------------------
/** AUD_DspSetMtsMonoFineVolume
 *   Set fine tune volume for MTS mono.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetMtsMonoFineVolume (UINT8 u1Index) ;

//-----------------------------------------------------------------------------
/** AUD_DspGetMtsMonoFineVolume
 *   Get fine tune volume for MTS mono.
 *     
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetMtsMonoFineVolume (void) ;


//-----------------------------------------------------------------------------
/** AUD_DspSetSAPFineVolume
 *   Set fine tune volume for SAP.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSAPFineVolume(UINT8 u1Index);


//-----------------------------------------------------------------------------
/** AUD_DspGetSapFineVolume
 *   Get fine tune volume for SAP.
 *     
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetSapFineVolume (void) ;

//-----------------------------------------------------------------------------
/** AUD_DspSetFmfmMonoFineVolume
 *   Set fine tune volume for FMFM mono.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetFmfmMonoFineVolume (UINT8 u1Index) ;

//-----------------------------------------------------------------------------
/** AUD_DspGetFmfmMonoFineVolume
 *   Get fine tune volume for FMFM mono.
 *     
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetFmfmMonoFineVolume (void) ;

//-----------------------------------------------------------------------------
/** AUD_DspSetFmfmDualFineVolume
 *   Set fine tune volume for FMFM dual.
 *   
 *  @param  u1Index      0~40 .  
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetFmfmDualFineVolume(UINT8 u1Index);

//-----------------------------------------------------------------------------
/** AUD_DspGetFmfmDualFineVolume
 *   Get fine tune volume for FMFM dual.
 *   
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetFmfmDualFineVolume (void) ;


//-----------------------------------------------------------------------------
/** AUD_DspSetMtsPilotDetection
 *   set MTS pilot offset detection on/off.
 *  @param  fgEnable	TRUE/FALSE
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetMtsPilotDetection (BOOL fgEnable) ;

//-----------------------------------------------------------------------------
/** AUD_DspGetMtsPilotDetection
 *   get MTS pilot offset detection on/off.
 *  @param  void	
 *  @retval  BOOL 
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetMtsPilotDetection (void) ;

//-----------------------------------------------------------------------------
/** AUD_DspSetSapMute
 *   set SAP noise mute thresholds
 *  @param  eChangedItem	lower threshold or higher threshold
 *  @param  u1Value
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSapMute( AUD_FACTORY_SAP_MUTE_ITEM_T eChangedItem, UINT8 u1Value); 

//-----------------------------------------------------------------------------
/** AUD_DspGetSapMute
 *   get SAP noise mute thresholds
 *  @param  eItem lower threshold or higher threshold
 *  @retval  UINT8
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetSapMute( AUD_FACTORY_SAP_MUTE_ITEM_T eItem);       

//-----------------------------------------------------------------------------
/** AUD_DspSetFMSatuMuteMode
 *   Set saturation mute thresholds
 *  @param  eItem lower threshold or higher threshold
 *  @retval  UINT8
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetFMSatuMuteTh ( AUD_FACTORY_SATU_MUTE_ITEM_T eChangedItem, 
                                                                                        UINT8 u1Value) ;

//-----------------------------------------------------------------------------
/** AUD_DspGetFMSatuMuteTh
 *   get saturation mute thresholds
 *  @param  eItem lower threshold or higher threshold
 *  @retval  UINT8
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetFMSatuMuteTh( AUD_FACTORY_SATU_MUTE_ITEM_T eItem);      

//-----------------------------------------------------------------------------
// ADAC_Init
//
/** Initial audio codec and digital amp.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_Init(void);

//-----------------------------------------------------------------------------
// ADAC_Enable
//
/** Audio codec and amp function table init.
 *
 *  @param[in]     fgEnable   TRUE for initial.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_Enable(BOOL fgEnable);

//-----------------------------------------------------------------------------
// ADAC_Mute
//
/** Audio codec and speaker mute.
 *
 *  @param[in]     fgEnable   "TRUE" for mute,"FALSE" for unmute.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_Mute(BOOL fgEnable);


//-----------------------------------------------------------------------------
// ADAC_DacFormatCfg
//
/** Setting DAC Data Format.
 *
 *  @param[in]     u1DecId   Decoder ID.
 *  @param[in]     eFormat   Audio data format,ex:LJ,RJ,I2S.
 *  @param[in]     eMclk   MCLK frequency.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_DacFormatCfg(UINT8 u1DecId, DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)  ;


//-----------------------------------------------------------------------------
// ADAC_AdcFormatCfg
//
/** Setting Codec Adc Data Format.
 *
 *  @param[in]     u1DecId   Decoder ID.
 *  @param[in]     eFormat   Audio data format,ex:LJ,RJ,I2S.
 *  @param[in]     eMclk   MCLK frequency.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_AdcFormatCfg(UINT8 u1DecId, DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)  ;

//-----------------------------------------------------------------------------
// ADAC_HeadphoneVolCtl
//
/** Setting Codec Adc Data Format.
 *
 *  @param[in]     i2VolInHalfDb   volume value.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_HeadphoneVolCtl(INT16 i2VolInHalfDb);

//-----------------------------------------------------------------------------
// ADAC_GetHeadphoneVolMaxMin
/**  Setting the max and min value of headphone volume.
*
*  @param[in]     pi2Max   volume max value.
*  @param[in]     pi2Min   volume min value.
*
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_GetHeadphoneVolMaxMin(INT16* pi2Max,INT16* pi2Min);

//-----------------------------------------------------------------------------
// ADAC_VolumeMuteSpeakerEnable
//
/** Mute speaker when volume value equal to zero.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_VolumeMuteSpeakerEnable(BOOL fgFlag);

//-----------------------------------------------------------------------------
// ADAC_DcDetectSpkEnable
//
/** Detect the DC and mute speaker.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_DcDetectSpkEnable(BOOL fgFlag);

//-----------------------------------------------------------------------------
// ADAC_HpDetectSpkEnable
//
/** Detect the headphone plug in and mute speaker.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_HpDetectSpkEnable(BOOL fgFlag);

//-----------------------------------------------------------------------------
// ADAC_HpDetectCodecMute
//
/** Detect the headphone plug in and mute codec.
 *
 *  @param[in]     fgMute   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_HpDetectCodecMute(BOOL fgMute);


//-----------------------------------------------------------------------------
// ADAC_HpDetectDacMute
//
/** Detect the headphone plug in and mute internaldac.
 *
 *  @param[in]     fgMute   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_HpDetectDacMute(BOOL fgMute);

//-----------------------------------------------------------------------------
// ADAC_ClkChangeCodecMute
//
/** Some codec have to mute before clock source change 
 *
 *  @param[in]     fgMute   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_ClkChangeCodecMute(BOOL fgMute);

//-----------------------------------------------------------------------------
// ADAC_SpeakerEnable
//
/** Speaker enable routine.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute speaker.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_SpeakerEnable(BOOL fgEnable);


//-----------------------------------------------------------------------------
// ADAC_CodecMute
//
/** Codec enable routine.
 *
 *  @param[in]     fgMute   "TRUE" for enable, "FALSE" for mute codec.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_CodecMute(BOOL fgMute);


//-----------------------------------------------------------------------------
// ADAC_GpioCodecMute
//
/** Codec GPIO control for mute/unmute.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute codec from GPIO.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_GpioCodecMute(BOOL fgEnable);


//-----------------------------------------------------------------------------
// ADAC_GpioAmpEnable
//
/** Speaker GPIO control for mute/unmute.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute speaker from GPIO.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_GpioAmpEnable(BOOL fgEnable);


//-----------------------------------------------------------------------------
// ADAC_CodecAdChlSel
//
/** Codec ad channel select.
 *
 *  @param[in]     u1Chl   Input data channel.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_CodecAdChlSel(UINT8 u1Chl);

//-----------------------------------------------------------------------------
// ADAC_CodecRegRead
//
/** Read register of external codec.
 *
 *  @param[in]     u1Offset   Address.
 *
 *  @retval 	UINT8	Data.
 */
//-----------------------------------------------------------------------------
extern UINT8 ADAC_CodecRegRead(UINT8 u1Offset);

//-----------------------------------------------------------------------------
// ADAC_CodecRegWrite
//
/** Write register of external codec.
 *
 *  @param[in]     u1Offset   	Address.
 *  @param[in]     u1Data   	Data.
 *
 *  @retval 	void.
 */
//-----------------------------------------------------------------------------
extern void ADAC_CodecRegWrite(UINT8 u1Offset, UINT8 u1Data);


//-----------------------------------------------------------------------------
// ADAC_AmpRegRead
//
/** Read register of external digital amp.
 *
 *  @param[in]     u1Offset   Address.
 *
 *  @retval 	UINT8	Data.
 */
//-----------------------------------------------------------------------------
extern UINT8 ADAC_AmpRegRead(UINT8 u1Offset);

//-----------------------------------------------------------------------------
// ADAC_AmpRegWrite
//
/** Write register of external digital amplify.
 *
 *  @param[in]     u1Offset   	Address.
 *  @param[in]     u1Data   	Data.
 *
 *  @retval 	void.
 */
//-----------------------------------------------------------------------------
extern void ADAC_AmpRegWrite(UINT8 u1Offset, UINT8 u1Data);

//-----------------------------------------------------------------------------
// AUD_BagtNfyFct
//
/** Brief of AUD_BagtNfyFct.
 *  Details of AUD_BagtNfyFct (optional).
 *
 *  @param      
 *
 *  @retval 
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_BagtNfyFct (
    const void *                     pv_tag,
    AUD_BUF_AGT_NFY_COND_T     e_nfy_cond,
    UINT32                    ui4_data_1,
    UINT32                    ui4_data_2);

//-----------------------------------------------------------------------------
// AUD_BagtSetSrcInfo
//
/** Brief of AUD_BagtSetSrcInfo.
 *  Details of AUD_BagtSetSrcInfo (optional).
 *
 *  @param      
 *
 *  @retval 
 */
//-----------------------------------------------------------------------------
extern void AUD_BagtSetSrcInfo (UINT16 ui2_buf_agt_id, 
                         const AUD_BUF_AGT_SRC_ALIGN_INFO_T*  t_data_blk, BOOL fgType);

//-----------------------------------------------------------------------------
// AUD_RegBagtReqestFunc
//
/** Register buffer agnet notify function.
 *
 *  @param     pu4Handler   Notify function handler
 *  @param     pfNfyFunc    Notify function pointer
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
 //-----------------------------------------------------------------------------
extern BOOL AUD_RegBagtReqestFunc(UINT32 * pu4Handler, AudBagtDataReqFct pfBagtRqstFct);

//-----------------------------------------------------------------------------
// AUD_BagtGetRqstId
//
/**Get current request ID of Buffer agent function.
 *
 *  @retval UINT32  current ID
 */
 //-----------------------------------------------------------------------------
extern UINT32  AUD_BagtGetRqstId (void);

//-----------------------------------------------------------------------------
// AUD_BagtGetCurrOffset
//
/**Get current request ID of Buffer agent function.
 *
 *  @retval UINT32  current ID
 */
 //-----------------------------------------------------------------------------
extern UINT32  AUD_BagtGetCurrOffset (void);

extern void AUD_BagtDataReqTest(    UINT16                  ui2_buf_agt_id,
                                                        const AUD_BUF_AGT_DATA_BLK_T*     pt_data_blk,
                                                        AUD_2_BAGT_NFY_COND_T eType);
extern void  AUD_BagtSetSrcInfoTest (void);

extern UINT8 AUD_DspGetBbeMode(void);
extern void AUD_DspSetBbeLevel (INT8 i1Level);
extern void AUD_DspSetBbeProcess (UINT8 u1BoostDb); 
extern void AUD_DspSetBbeLoContour(UINT8 u1BoostDb); 
extern void AUD_DspSetBbe3dGain(UINT8 u13dGain);
extern INT8 AUD_DspGetBbeLevel(void);     
extern UINT8 AUD_DspGetBbeProcess(void);
extern UINT8 AUD_DspGetBbeLoContour(void);
extern UINT8 AUD_DspGetBbe3dGain(void);


//-----------------------------------------------------------------------------
// AUD_FeederSetRequestFunc
//
/** For feeder to set request function.
 *
 *  @param     prTransmit   Notify function handler
 *
 *  @retval    TRUE   ok
 *  @retval    FALSE  fail
 */
 //-----------------------------------------------------------------------------
extern BOOL AUD_FeederSetRequestInfo(FEEDER_TRANSMIT *prTransmit);

extern void AUD_GetStreamInfo(UINT8 u1DecId, UINT8 *pu1Acmode, UINT8 *pu1SmpRate, UINT32 * pu4DataRate);
extern void AUD_GetStreamInfo_AC3(UINT8 u1DecId, UINT8 *pu1Acmode, UINT8 *pu1SmpRate, UINT32 * pu4DataRate);
extern void AUD_StrSelect(UINT8 u1DecId, UINT8 u1Str, MEM_BUFFER_INFO_T *prMemBuf);
extern void AUD_DspCfgSetDecType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt);
extern void _AUD_OutPadEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_SpdifOutPadEnable(UINT8 u1DecId, BOOL fgEnable);

extern BOOL AUD_SetStartPts(UINT8 u1DecId, UINT32 u4Pts);
extern void AUD_DrvClkCfgDualDecMode(BOOL fgEnable);

extern void AudShowDspStatus(void);
extern void AudShowMtsStatus(UINT8 u1DecId);
extern void AudShowStatus(UINT8 u1DecId);
extern void AUD_ChlOutDump(UINT8 u1Chl);

//-----------------------------------------------------------------------------
/** AUD_DspSetVolumeTable
 *   Set volume table.
 *   
 *  @param  u4VolTabChkPnt[]
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetVolumeTable(UINT32* u4VolTabChkPnt);
#ifdef CC_AUD_LINEOUT_VOL_MODE
extern void AUD_DspSetLineOutVolumeMode(UINT8 u1DecId, UINT8 fgMode);
extern UINT8 AUD_DspGetLineOutFixVol(void);
#endif
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
extern void AUD_DspSrcLinoutVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspGetVolumeTable
 *   Query volume table.
 *   
 *  @param  u1VolTabIdx
 *
 *  @retval  volume gain
 */
//-----------------------------------------------------------------------------
extern UINT32 AUD_DspGetVolumeTable(UINT8 u1VolTabIdx);
extern UINT32 AUD_DspSpkSizeQry(UINT8 u1DecId);

//-----------------------------------------------------------------------------
// AUD_DrvSetDDBanner
//
/** Turn on DDBanner.
 *
 *  @param     fgEnable     TRUE/FALSE
 *
 *  @retval    void
 */
//-----------------------------------------------------------------------------
extern void AUD_DrvSetDDBanner(BOOL fgEnable);


//-----------------------------------------------------------------------------
/** AUD_DspGetVolumeBaseLevel
 *   Get volume base level (0dB level).
 *   
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetVolumeBaseLevel(void);

extern void Aud_SetTvSystemMask(UINT8 u1DecId, UINT32 u4Mask);
extern UINT32 Aud_GetTvSystemMask(UINT8 u1DecId);
extern void AUD_DspADModeSet(UINT8 u1DecId, UINT16 u2Value);
extern void AUD_DspADEnable(UINT8 u1DecId, BOOL fgEnable);
extern void AUD_DspSetPcmPreBuffer (UINT8 u1DecId, UINT16 u2Length);
extern UINT16 AUD_DspGetPcmPreBuffer(UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_Surround_Get_Config
 *   Set new mtk vsurr config
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_Surround_Get_Config(AUD_VSURR_CFG_T* prVsurrCfg);

//-----------------------------------------------------------------------------
/** AUD_Surround_Set_Config
 *   Set new mtk vsurr config
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_Surround_Set_Config(AUD_VSURR_CFG_TYPE_T eVSCfgType, UINT32 u4VSParameter);

//-----------------------------------------------------------------------------
/** AUD_SendApllAdjustCmd
 *  Send APLL MODIN adjust command
 *
 *  @param  u4Level    video calculated level (0 ~ 255. 128 as center)
 *                     bit 8: 0 (slower), 1 (faster)
 *                            
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_SendApllAdjustCmd(UINT32 u4Level);

//-----------------------------------------------------------------------------
/** AUD_GetNicamStatus
 *  Get Nicam exists or not
 *
 *  @param  void
 *                            
 *  @retval TRUE/FALSE
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_GetNicamStatus(void);

//-----------------------------------------------------------------------------
/** AUD_GetDtvAudInfo
 *  Get DTV info
 *
 *  @param  u1DecId
 *  @param  prAudInfo
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_GetDtvAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo); 
	

//-----------------------------------------------------------------------------
/** AUD_DspSpeed
 *  Set dsp speed (non-fractional)
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Speed  : 0:normal play
 *                            : 2:FF 2-times
 *                            : 4:FF 4-times
 *                            : 8:FF 8-times
 *                            : 16:FF 16-times
 *                            : 32:FF 32-times
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSpeed(UINT8 u1DecId, UINT16 u2Speed);

//-----------------------------------------------------------------------------
/** AUD_DspDownmixPosition
 *  Set Downmix Channel (CH9/10) Position
 *
 *  @param  u1DecId 0       : first decoder 1: seconder decoder.
 *  @param  u1DMPosition   : 0: Downmix channel(CH9/10) do ALL post processing
 *                                   : 1: doing downmix after input source gain and before AVC
 *                                   : 2: Reserved
 *                                   : 3: Downmix channel(CH9/10) do not do post processing

 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspDownmixPosition(UINT8 u1DecId, UINT16 u1DMPosition);

//-----------------------------------------------------------------------------
/** AUD_DspMultiPairOutput
 *  Copy CH9/10 to other channel(LS/RS/C/SUB)
 *
 *  @param  u1DecId 0       : first decoder 1: seconder decoder.
 *  @param  u1MultiPair   : 0: OFF
 *                                   : 1: ON

 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMultiPairOutput(UINT8 u1DecId, UINT16 u1MultiPair); 

//-----------------------------------------------------------------------------
/** _AUD_DspLRDownmix
 *  Downmix to L/R channel after Reverbation and before BassManagement
 *
 *  @param  u1DecId 0       : first decoder 1: seconder decoder.
 *  @param  u1MultiPair   : 0: OFF
 *                                   : 1: ON

 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspLRDownmix(UINT8 u1DecId, BOOL fgEnable);
	
//-----------------------------------------------------------------------------
/** AUD_SetLpcmTable
 *  Set Lpcm decoder parameters
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  AUD_LPCM_SETTING_T 
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_SetLpcmTable(UINT8 u1DecId, AUD_LPCM_SETTING_T rLpcmSetting);

//-----------------------------------------------------------------------------
/** AUD_DspSetDualDecMode
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetDualDecMode(BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspSetHPFEnable
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetHPFEnable(BOOL fgEnable);

//-----------------------------------------------------------------------------
/** _AUD_DspSetHPFFc
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetHPFFc(UINT16 u2Fc);

//-----------------------------------------------------------------------------
/** _AUD_DspSetClipperEnable
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetClipperEnable(BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspSetClipperValue
 *   
 *  @param  vaeChannel
 *  @param  u4Value
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetClipperValue(AUD_CH_T eChannel, UINT32 u4Value);

//-----------------------------------------------------------------------------
/** _AUD_DspSetPEQEnable
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetPEQEnable(BOOL fgEnable);
#ifdef CC_AUD_SPEAKER_HEIGHT
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
extern void AUD_DspSetSPHEnable(BOOL fgEnable);
#else
extern void AUD_DspSetSPHMode(UINT8 u1Mode);
#endif
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetPEQCfg2
 *   
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param u1Set                      PEQ set (0 ~ 7)
 *  @param u4Frequency                Frequency (20 ~ 20K)
 *  @param eQValue                    Q
 *  @param i2Gain                     Gain (+-12dB) with 0.5dB/step
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetPEQCfg2(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            AUD_PEQ_Q_TYPE_T eQValue, INT16 i2Gain);

//-----------------------------------------------------------------------------
/** AUD_DspSetPEQCfg
 *   
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param u1Set                      PEQ set (0 ~ 7)
 *  @param u4Frequency                Frequency (20 ~ 20K)
 *  @param u2BW                       Bandwidth (xx Hz)
 *  @param i2Gain                     Gain (+-12dB) with 0.5dB/step
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetPEQCfg(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT16 u2BW, INT16 i2Gain);

//-----------------------------------------------------------------------------
/** AUD_DspSetVirBassEnable
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetVirBassEnable(BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspSetPEQCfg
 *   
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param u4Frequency                Frequency (20 ~ xx)
 *  @param i2Gain                     Gain (+-12dB) with 0.5dB/step
 *  @param i2Gain2                    Gain2 (+-12dB) with 0.5dB/step
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetVirBassCfg(UINT8 u1DecId, UINT32 u4Frequency, UINT32 u4Gain, 
                        UINT32 u4Gain2);

#ifdef CC_MT5391_AUD_SUPPORT	  	  
extern void _MT8297_ADC_PowerDown(BOOL fgEnable);
#elif defined (CC_DYNAMIC_POWER_ONOFF)
extern void _MT536xInternalAdcPowerDown(BOOL fgEnable);
#endif

extern void AUD_SetHeadphoneCond(UINT8 u1DecId, UINT8 u1HPCond);

extern UINT8 AUD_GetHeadphoneCond(UINT8 u1DecId);

extern UINT32 AUD_NVM_Get_Profile(UINT32* pAudProfileStart, UINT32* pAudProfileEnd, UINT8* u1Profile);

extern UINT32 AUD_NVM_Read(void);

extern void AUD_DspDDCOEnable(BOOL fgEnable);

extern void AUD_DspDDCOAgcEnable(BOOL fgEnable);

extern void AUD_DspDDCOLFEEnable(BOOL fgEnable);



//-----------------------------------------------------------------------------
// AUD_ReadDspPROM
//
/** Read PROM 16-bits instruction.
 *  NOTICE: Do not use this function with Rom booting!
 *
 *  @param[in]        u4Addr           read PROM address  
 *
 *  @retval PROM value
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_ReadPROM (UINT32 u4Addr);

//-----------------------------------------------------------------------------
// AUD_ReadDspDROM
//
/** Read DROM 24-bits data.
 *
 *  @param[in]        u4Addr           read DROM offset  
 *
 *  @retval DROM value
 */
//-----------------------------------------------------------------------------
UINT16 AUD_ReadDROM (UINT32 u4Addr);

//-----------------------------------------------------------------------------
// AUD_Read32ByteID
//
/** Read ROM 32 bytes ID.
 *
 *  @param[in]        u1ID_p           ROM ID pointer
 *                    u4Num            ROM ID byte number    
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_Read32ByteID (UINT8 *u1ID_p, UINT32 u4Num);
extern void AUD_MWCtrl_Set(UINT8 u1DecId,BOOL fgPlay);
extern BOOL AUD_MWCtrl_Get(UINT8 u1DecId);

extern void AUD_GetDspPtsInfo (UINT8 u1DecId, UINT32* pu4DspPts, UINT32* pu4PtsPrsPnt);

#ifdef CC_AUD_SETUP_MELODY

void Aud_GetSetupMelodyInfo(AUD_CLIP_TYPE_T *prAudClip);
#endif
//-----------------------------------------------------------------------------
// AUD_DrvDmxIntThreshold
//
/** Set DMX interrupt threshold.
 *
 *  @param[in]          u1DecId         Decoder ID  
 *                      u1Factor        threshold = 188 * u1Factor
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DrvDmxIntThreshold(UINT8 u1DecId, UINT8 u1Factor);

//-----------------------------------------------------------------------------
// AUD_SetLPCMFmt
//
/** Set LPCM parameter
 *
 *  @param[in]          u4PriData         private data about lpcm format in VOB  
 *                      
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_SetLPCMFmt(UINT32 u4PriData);
//-----------------------------------------------------------------------------
// AUD_IsLpcmFmtPS
//
/** Query LPCM from AVI or Program Stream.
 *
 *  @param[in]          void
 *                      
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_IsLpcmFmtPS(void);
//-----------------------------------------------------------------------------
// AUD_LPcmSetting
//
/** Set LPCM parameter
 *
 *  @param[in]          u1DecId         
 *                      prLPcmInfo
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_LPcmSetting(UINT8 u1DecId, AUD_LPCM_INFO_T * prLPcmInfo);   

//-----------------------------------------------------------------------------
// AUD_MM_Set_Dec_Fmt_Conflict
//
/** For SWDMX callback AUD_DRV to notify that audio fomat is conflict.
 *
 *  @param[in]        u1DecId          Decoder ID
 *                    fgFlag           Enable Flag
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_MM_Set_Dec_Fmt_Conflict(UINT8 u1DecId, BOOL fgFlag);
extern BOOL AUD_Is_MMDecFmtConflict(UINT8 u1DecId);
extern void AUD_MM_Set_Dec_Fmt_Scramble(UINT8 u1DecId, BOOL fgFlag);
extern void AUD_MM_Set_Avsync_event(UINT8 u1DecId);

extern BOOL AUD_Is_MMDecFmtScramble(UINT8 u1DecId);
extern INT32 AUD_MTS_CFO_IDX(void);
extern INT32 AudCmdAtvHdevDetQuery(void);

//-----------------------------------------------------------------------------
/** AUD_DspGetMpegAudInfo
 *   
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param *prAudInfo                audio info structure pointer
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspGetMpegAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo);

//-----------------------------------------------------------------------------
/** AUD_DspSetMp3PreparsingMode
 *   
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetMp3PreparsingMode(BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspIsMp3Vbr
 *   
 *  @param  void
 *  @retval true (VBR)  false(CBR)
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspIsMp3Vbr(void);
//sunman_aout
extern UINT32 AudGetAoutStatus(void);
extern void AudResetAoutStatus(void);

//sunman cfo
extern INT32 AUD_MTS_CFO_IDX(void);

extern UINT32 AUD_GetTargetCountry(void);

#ifdef CC_AUD_SETUP_MELODY
extern void AUD_PlaySetupMelody(UINT8 u1DecId, UINT32 u4Channel, UINT32 u4RepeatCount);
extern void AUD_SetSetupPlayStatue(BOOL fgIsFinish);
extern BOOL AUD_IsSetupMelodyFinish(void);
extern void AUD_StopSetupMelody(UINT8 u1DecId);
#endif

#ifdef CC_AUD_USE_FLASH_AQ
extern INT32 Aud_FlashAQRead(UINT16 u2Offset, UINT8 *u1Data, UINT32 u4Length);
extern void Aud_FlashAQWrite(UINT16 u2Offset, UINT8 *u1Data, UINT32 u4size);
extern BOOL Aud_FlashAQCompVersion(CHAR *szCurVer, CHAR *szUpgVer);
extern void Aud_FlashAQ_SetDefault(void);
extern void Aud_FlashAQ_Fill_Data(BOOL fgDelayMode);
extern BOOL Aud_FlashAQ_GetDscrpt(AUD_AQ_DSCRPT_T** pAudFlashAQDscrpt);
extern void AUD_FlashAQ_Init(void);
extern BOOL Aud_FlashAQQuery(AUD_AQ_TYPE_T eAQDataType, VOID* *pData);
extern void _AUD_FlashAQOperation(UINT8 u1DecId, AUD_AQ_TYPE_T eAQ_Type, AUD_AQ_OP_TYPE_T eAQ_OPType, UINT16 u2Data);
#endif /* CC_AUD_USE_FLASH_AQ */
extern BOOL ADAC_DigiAmp_ReadRam(AUD_AMP_ID_T u1AmpID, UINT8 u1Bank, UINT8 u1Addr, UINT8* u1Data, BOOL fgAllSet);
extern BOOL ADAC_DigiAmp_WriteRam(AUD_AMP_ID_T u1AmpID, UINT8 u1Bank, UINT8 u1Addr, AUD_AQ_AMP_RAM_DATA_T rAmpRamData, BOOL fgAllSet);
extern void ADAC_DigiAmp_SetChannelVol(AUD_AMP_ID_T u1AmpID, UINT8 u1ChId, UINT8 u1Value);
extern void ADAC_DigiAmp_SetDualDRC(AUD_AMP_ID_T u1AmpID, AUD_AQ_AMP_DRC_T eDrcData);
extern void ADAC_DigiAmp_SetPostScale(AUD_AMP_ID_T u1AmpID, UINT32 u4PostSclCh1, UINT32 u4PostSclCh2);
extern void ADAC_DigiAmp_SetPEQ(AUD_AMP_ID_T u1AmpID, AUD_AQ_EXTPEQ_DATA_T rAQExtPeqData);


#endif /* _AUD_IF_H_ */
