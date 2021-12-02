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
#ifndef MTK_AUD_H
#define MTK_AUD_H

#include "aud_if.h"

//-----------------------------------------------------------------------------
// Audio input SW
//-----------------------------------------------------------------------------

#ifdef AUDIO_INPUT_SW_GPIO_SEL_DEFAULT
static const AUD_INPUT_SW_GPIO_T _arMtkInputSwGpio =
{
    UNUSED_GPIO,                         // AUD_GPIO_SWITCH_0       17
    UNUSED_GPIO,                         // AUD_GPIO_SWITCH_0 15
    UNUSED_GPIO,
    UNUSED_GPIO
};
#endif

#ifdef AUDIO_INPUT_MUX_SEL_TABLE_DEFAULT
static const AUD_INPUT_MUX_SEL_T _arMtkAudInputMux[] =
{
    // Input ID             AdcMuxSel,    SwGpio1,    SwGpio2,    SwGpio3,    SwGpio4
    {AUD_INPUT_ID_COMP_VID_0,    3,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_COMP_VID_1,    4,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_COMP_VID_2,    NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_COMP_VID_3, NO_USED,  SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_S_VID_0,          6,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_S_VID_1,       NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_S_VID_2,       NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_YPBPR_0,          5,  SW_NO_USED,SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_YPBPR_1,          NO_USED,             SW_NO_USED, SW_NO_USED,SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_YPBPR_2,     NO_USED,  SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_VGA_0,             2,             SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_VGA_1,       NO_USED,  SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_HDMI_0,           NO_USED,  SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_HDMI_1,     NO_USED,   SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_HDMI_2,     NO_USED,   SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_HDMI_3,     NO_USED,   SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_HDMI_4,     NO_USED,   SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_DVI_0,             NO_USED,  SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_DVI_1,           NO_USED,  SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_DVI_2,      NO_USED,   SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_DVI_3,      NO_USED,   SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_DVI_4,      NO_USED,   SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_0,         0,   SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_1,         1,   SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_2,         NO_USED,  SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_3,         NO_USED,  SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_AUXIN_0,      2,   SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_AUXIN_1,      NO_USED,   SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED}
};

static const AUD_INPUT_MUX_SEL_TABLE_T _rMtkAudInputMuxTable =
{
    (sizeof(_arMtkAudInputMux)/sizeof(AUD_INPUT_MUX_SEL_T)),
    _arMtkAudInputMux
};
#endif

//-----------------------------------------------------------------------------
// Audio adac input/output format, volume control
//-----------------------------------------------------------------------------
#ifdef AUDIO_OUT_FMT_TABLE_DEFAULT
static const AOUT_CFG_T _arMtkAudOutFmtTable =
{
#ifdef CC_FPGA
    FORMAT_RJ,              // format of alignment
#else
    FORMAT_I2S,              // format of alignment
#endif    
    DAC_24_BIT,             // number of bits per sample
    LRCK_CYC_32,            // cycles per sample
    MCLK_256FS,             // DAC sampling frequence
    FALSE,                  // Invert audio output for OP phase
    FALSE                    // Invert L/R audio output
};
#endif

#ifdef AUDIO_OUT_FMT_TABLE2_DEFAULT
static const AOUT_CFG_T _arMtkAudOutFmtTable2 =
{
#ifdef CC_FPGA
    FORMAT_RJ,              // format of alignment
#else
    FORMAT_I2S,              // format of alignment
#endif    
    DAC_24_BIT,             // number of bits per sample
    LRCK_CYC_32,            // cycles per sample
    MCLK_256FS,             // DAC sampling frequence
    FALSE,                  // Invert audio output for OP phase
    FALSE                    // Invert L/R audio output
};
#endif

#ifdef AUDIO_IN_FMT_TABLE_DEFAULT
#ifndef __MODEL_slt__
static const AOUT_CFG_T _arMtkAudInFmtTable =
{
    FORMAT_I2S,             // format of alignment
    DAC_24_BIT,             // number of bits per sample
    LRCK_CYC_32,            // cycles per sample
    MCLK_256FS,             // DAC sampling frequence
    FALSE,                  // Invert audio output for OP phase
    TRUE                    // Invert L/R audio output
};
#else
static const AOUT_CFG_T _arMtkAudInFmtTable =
{
    FORMAT_I2S,             // format of alignment
    DAC_24_BIT,             // number of bits per sample
    LRCK_CYC_32,            // cycles per sample
    MCLK_256FS,             // DAC sampling frequence
    FALSE,                  // Invert audio output for OP phase
    TRUE                    // Invert L/R audio output
};
#endif  // __MODEL_slt__
#endif

//-----------------------------------------------------------------------------
// Audio adjustable volume
//-----------------------------------------------------------------------------

#define VOL_SHM_0_DB        (0x20000)
#define VOL_SHM_NEG_6_DB    (0x20000/2)
#define VOL_SHM_NEG_12_DB   (0x20000/4)

#ifdef AUDIO_INPUT_SRC_VOL_DEFAULT
static const INT16 _aai2MtkAudInputSrcVol[AUD_DEC_NUM][AUD_STREAM_FROM_NUM] =
{
    //Others   DTV       ATV       SPDIF     LINE_IN   HDMI      MEMORY    BUFF_AGT  FEEDER    MM
    {VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB},
    {VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB}
};
#endif

#ifdef AUDIO_INPUT_SRC_LINE_OUT_VOL_DEFAULT
static const INT16 _aai2MtkAudInputSrcLineOutVol[AUD_DEC_NUM][AUD_STREAM_FROM_NUM] =
{
    //Others   DTV       ATV       SPDIF     LINE_IN   HDMI      MEMORY    BUFF_AGT  FEEDER    MM
    {VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB},
    {VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB}
};
#endif

#ifdef AUDIO_INPUT_SRC_AGC_VOL_DEFAULT
static const UINT32 _aau4MtkAudInputSrcAgcVol[AUD_DEC_NUM][AUD_STREAM_FROM_NUM] =
{
    //Others       DTV           ATV           SPDIF         LINE_IN       HDMI          MEMORY        BUFF_AGT      FEEDER        MM
    {VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB},
    {VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB}
};
#endif

#ifdef AUDIO_INPUT_SRC_AGC_FLAG_DEFAULT
static const BOOL _aafgMtkAudInputSrcAgcFlag[AUD_DEC_NUM][AUD_STREAM_FROM_NUM] =
{
    {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
    {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE}
};
#endif

#ifdef AUDIO_AV_INPUT_SRC_VOL_DEFAULT
static const INT16 _ai2MtkAudAvInputSrcVol[AUD_INPUT_ID_MAX] =
    {   VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB,
        VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB
    };
#endif

//-----------------------------------------------------------------------------
// Channel volume extra gain
//-----------------------------------------------------------------------------
#ifdef AUDIO_CHL_VOL_EXTRA_GAIN_DEFAULT
// Table value: dB value for 0.5dB/step
// Range: -64dB (-64*2) ~ 47.5dB (47.5*2)
// Ex: +5dB => table value = 5*2 = 10
//     : -4dB => table value = -4*2 = -8
// Note: data may saturate if extra gain is larger than 0dB
static const INT8 _aai1MtkAudChVolExtraGain[AUD_DEC_NUM][AUD_CH_NUM] =
{
    //AUD_DEC_MAIN
      //L      R         SL        SR        Center    Subwoofer BypassL   BypassR   DownmixL  DownmixR
    {VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB},
    //AUD_DEX_AUX
      //L      R
    {VOL_0_DB, VOL_0_DB}
};
#endif

//-----------------------------------------------------------------------------
// AVC
//-----------------------------------------------------------------------------
#ifdef AUDIO_AVC_TARGET_LEV_DEFAULT
static const INT16 _ai2MtkAudAvcTarget[AUD_STREAM_FROM_NUM] =
{
    -20,        // OTHERS
    -20,        // DIGITAL_TUNER
    -20,        // ANALOG_TUNER
    -20,        // SPDIF
    -20,        // LINE_IN
    -20,        // HDMI
    -20,        // MEMORY
    -20,        // BUF_AGT
    -20,        // FEEDER
    -20,        // MM
};
#endif

#ifdef AUDIO_AVC_GAIN_UP_DEFAULT
static const INT16 _ai2MtkAudAvcGainUp[AUD_STREAM_FROM_NUM] =
{
    6,        // OTHERS
    6,        // DIGITAL_TUNER
    6,        // ANALOG_TUNER
    6,        // SPDIF
    6,        // LINE_IN
    6,        // HDMI
    6,        // MEMORY
    6,        // BUF_AGT
    6,        // FEEDER
    6,        // MM
};
#endif

#ifdef AUDIO_AVC_ADJUST_RATE_DEFAULT
static const INT16 _ai2MtkAudAvcAdjRate[AUD_STREAM_FROM_NUM] =
{
    0x80,        // OTHERS
    0x80,        // DIGITAL_TUNER
    0x80,        // ANALOG_TUNER
    0x80,        // SPDIF
    0x80,        // LINE_IN
    0x80,        // HDMI
    0x80,        // MEMORY
    0x80,        // BUF_AGT
    0x80,        // FEEDER
    0x80,        // MM
};
#endif

#ifdef AUDIO_AVC_UI_ADJUST_RATE_DEFAULT
static const INT16 _ai2MtkAudAvcUiAdjRate[AUD_STREAM_FROM_NUM] =
{
    0x80,        // OTHERS
    0x80,        // DIGITAL_TUNER
    0x80,        // ANALOG_TUNER
    0x80,        // SPDIF
    0x80,        // LINE_IN
    0x80,        // HDMI
    0x80,        // MEMORY
    0x80,        // BUF_AGT
    0x80,        // FEEDER
    0x80,        // MM
};
#endif

//-----------------------------------------------------------------------------
// Volume table
//-----------------------------------------------------------------------------
#ifdef AUDIO_VOL_TABLE_DEFAULT
#if 1
static const UINT32 _au4VolumeTable[100 + 1] =
{

//  1~9
    0x0000000, 0x0000020, 0x0000030, 0x0000050,0x0000070, 0x00000C0, 0x0000120, 0x00001A0,
    0x0000250, 0x0000340,

//10~14,2db,-41db~-33db
    0x0000490, 0x00005C0, 0x0000740, 0x0000920, 0x0000B70,

//15~26,1db,-31db~-20db
    0x0000E70, 0x0001030, 0x0001230, 0x0001460,
    0x00016E0, 0x00019B0, 0x0001CD0, 0x0002050, 0x0002440, 0x00028B0, 0x0002DA0, 0x0003330,

//27~45,0.5db,-19db~-10db
    0x0003970, 0x0003CE0, 0x0004070,
    0x0004440, 0x0004850, 0x0004CA0, 0x0005120, 0x00055F0, 0x0005B10, 0x0006070, 0x0006630,
    0x0006C30, 0x00072A0, 0x0007970, 0x00080A0, 0x0008840, 0x0009050, 0x00098E0, 0x000A1F0,

//46~67,0.25db,-9.5db~-4.25
    0x000AB80, 0x000B080, 0x000B5B0,
    0x000BB00, 0x000C070, 0x000C610, 0x000CBD0, 0x000D1D0, 0x000D7F0, 0x000DE30, 0x000E4B0,
    0x000EB60, 0x000F240, 0x000F950, 0x00100A0, 0x0010820, 0x0010FD0, 0x00117C0, 0x0011FF0,
    0x0012850, 0x0013100, 0x00139E0,

//68~100,0.125dB,-4db~0db
    0x0014310, 0x00147C0, 0x0014C80, 0x0015150, 0x0015630,
    0x0015B20, 0x0016030, 0x0016550, 0x0016A70, 0x0016FC0, 0x0017510, 0x0017A70, 0x0017FF0,
    0x0018580, 0x0018B30, 0x00190E0, 0x00196B0, 0x0019C90, 0x001A290, 0x001A8A0, 0x001AED0,
    0x001B510, 0x001BB60, 0x001C1D0, 0x001C850, 0x001CEF0, 0x001D5A0, 0x001DC70, 0x001E360,
    0x001EA60, 0x001F180, 0x001F8B0, 0x0020000,
};

static const UINT32 _au4ChannelVolumeTable[100 + 1] =
{

//  1~9
    0x0000000, 0x0000020, 0x0000030, 0x0000050,0x0000070, 0x00000C0, 0x0000120, 0x00001A0,
    0x0000250, 0x0000340,

//10~14,2db,-41db~-33db
    0x0000490, 0x00005C0, 0x0000740, 0x0000920, 0x0000B70,

//15~26,1db,-31db~-20db
    0x0000E70, 0x0001030, 0x0001230, 0x0001460,
    0x00016E0, 0x00019B0, 0x0001CD0, 0x0002050, 0x0002440, 0x00028B0, 0x0002DA0, 0x0003330,

//27~45,0.5db,-19db~-10db
    0x0003970, 0x0003CE0, 0x0004070,
    0x0004440, 0x0004850, 0x0004CA0, 0x0005120, 0x00055F0, 0x0005B10, 0x0006070, 0x0006630,
    0x0006C30, 0x00072A0, 0x0007970, 0x00080A0, 0x0008840, 0x0009050, 0x00098E0, 0x000A1F0,

//46~67,0.25db,-9.5db~-4.25
    0x000AB80, 0x000B080, 0x000B5B0,
    0x000BB00, 0x000C070, 0x000C610, 0x000CBD0, 0x000D1D0, 0x000D7F0, 0x000DE30, 0x000E4B0,
    0x000EB60, 0x000F240, 0x000F950, 0x00100A0, 0x0010820, 0x0010FD0, 0x00117C0, 0x0011FF0,
    0x0012850, 0x0013100, 0x00139E0,

//68~100,0.125dB,-4db~0db
    0x0014310, 0x00147C0, 0x0014C80, 0x0015150, 0x0015630,
    0x0015B20, 0x0016030, 0x0016550, 0x0016A70, 0x0016FC0, 0x0017510, 0x0017A70, 0x0017FF0,
    0x0018580, 0x0018B30, 0x00190E0, 0x00196B0, 0x0019C90, 0x001A290, 0x001A8A0, 0x001AED0,
    0x001B510, 0x001BB60, 0x001C1D0, 0x001C850, 0x001CEF0, 0x001D5A0, 0x001DC70, 0x001E360,
    0x001EA60, 0x001F180, 0x001F8B0, 0x0020000,
};

#else
//20050510,kevin,large mode
WORD code VOLUME[VOL_LEVEL_MAX + 1] =
{

//  0~10
    0x000000, 0x000003, 0x000006,
    0x00000C, 0x000015, 0x000025, 0x000041, 0x00005C, 0x000082,
    0x0000B7, 0x000103,

// 11~20,1db -27db~-18db
    0x00016E, 0x00019B, 0x0001CD, 0x000205, 0x000244, 0x00028B, 0x0002DA, 0x000333,
    0x000397, 0x000407,

// 21~33,0.5db -17db~-11db
    0x000485, 0x0004CA, 0x000512, 0x00055F, 0x0005B1, 0x000607, 0x000663,
    0x0006C3, 0x00072A, 0x000797, 0x00080A, 0x000884,0x000905,

//34~51,0.25db -11db~-6.25db
    0x00098E, 0x0009D5,
    0x000A1F, 0x000A6A, 0x000AB8, 0x000B08, 0x000B5B,
    0x000BB0, 0x000C07, 0x000C61, 0x000CBD, 0x000D1D, 0x000D7F, 0x000DE3, 0x000E4B,
    0x000EB6, 0x000F24, 0x000F95,

//52~67  ,0.125db -6db~-4.125
    0x00100A, 0x001045, 0x001082, 0x0010BF, 0x0010FD,
    0x00113C, 0x00117C, 0x0011BD, 0x0011FF, 0x001241, 0x001285, 0x0012CA, 0x001310,
    0x001356, 0x00139E, 0x0013E7,

//68~100,0.125dB,-4db~0db
    0x001431, 0x00147C, 0x0014C8, 0x001515, 0x001563,
    0x0015B2, 0x001603, 0x001655, 0x0016A7, 0x0016FC, 0x001751, 0x0017A7, 0x0017FF,
    0x001858, 0x0018B3, 0x00190E, 0x00196B, 0x0019C9, 0x001A29, 0x001A8A, 0x001AED,
    0x001B51, 0x001BB6, 0x001C1D, 0x001C85, 0x001CEF, 0x001D5A, 0x001DC7, 0x001E36,
    0x001EA6, 0x001F18, 0x001F8B, 0x002000,
};
#endif
#endif

#ifdef AUDIO_SRS_PEQ_COEF_DEFAULT
///MT5395 use 48K    DSP_SUPPORT_SRS_SSHD
static const UINT32 _arSRSPEQHPFCoef_DEF[3][40+10+10] =
{
        //32k
        {
                //--0--
                0x3bf14b, 0xff867061, 0xffc40eb5, 0x798f9f, 0x400000, 
                //--1--
                0x3bf14b, 0xff867061, 0xffc40eb5, 0x798f9f, 0x400000, 
                //--2--
                0x3d07cd, 0xff843c85, 0xffc2f833, 0x7bc37b, 0x400000, 
                //--3--
                0x3d07cd, 0xff843c85, 0xffc2f833, 0x7bc37b, 0x400000, 
                //--4--
                0x3bf14b, 0xff867061, 0xffc40eb5, 0x798f9f, 0x400000, 
                //--5--
                0x3bf14b, 0xff867061, 0xffc40eb5, 0x798f9f, 0x400000, 
                //--6--
                0x3d07cd, 0xff843c85, 0xffc2f833, 0x7bc37b, 0x400000, 
                //--7--
                0x3d07cd, 0xff843c85, 0xffc2f833, 0x7bc37b, 0x400000, 
                //HPF1
                0x000ff4a8, 0xffe016b0, 0xffc05aa1, 0x7fa51e, 0x000ff4a8, 
                //HPF2
                0x000ff4a8, 0xffe016b0, 0xffc05aa1, 0x7fa51e, 0x000ff4a8, 
        },
        //44k
        {
                //--0--
                0x3d07cd, 0xff843c85, 0xffc2f833, 0x7bc37b, 0x400000, 
                //--1--
                0x3d07cd, 0xff843c85, 0xffc2f833, 0x7bc37b, 0x400000, 
                //--2--
                0x3f9aea, 0xff8087db, 0xffc0765f, 0x7f7825, 0x3feeb7, 
                //--3--
                0x3fa063, 0xff80a480, 0xffc0842f, 0x7f5b80, 0x3fdb6e, 
                //--4--
                0x3d07cd, 0xff843c85, 0xffc2f833, 0x7bc37b, 0x400000, 
                //--5--
                0x3d07cd, 0xff843c85, 0xffc2f833, 0x7bc37b, 0x400000, 
                //--6--
                0x3f9aea, 0xff8087db, 0xffc0765f, 0x7f7825, 0x3feeb7, 
                //--7--
                0x3fa063, 0xff80a480, 0xffc0842f, 0x7f5b80, 0x3fdb6e, 
                //HPF1
                0x000ff7c4, 0xffe01078, 0xffc041d0, 0x7fbe0e, 0x000ff7c4, 
                0x000ff7c4, 0xffe01078, 0xffc041d0, 0x7fbe0e, 0x000ff7c4,
                //HPF2
                0x000ff7c4, 0xffe01078, 0xffc041d0, 0x7fbe0e, 0x000ff7c4, 
                0x000ff7c4, 0xffe01078, 0xffc041d0, 0x7fbe0e, 0x000ff7c4
        },
        //48k
        {
                //--0--
               0x3f7fdc,0xff805998,0xffc05590,0x7fa668,0x402a94,
                //--1--
               0x3db3d2,0xff824329,0xffc1a76a,0x7dbcd7,0x40a4c5,
                //--2--
                0x3ed565,0xff817faf,0xffc15dab,0x7e8051,0x3fccf1,
                //--3--
                0x2558d3,0xffc997e6,0xffd32fcd,0x36681a,0x477761,
                //--4--
                0x2deaff,0xff9a1116,0xffca4c31,0x65eeea,0x47c8d0,
                //--5--
                0x346d1e,0xffa094f3,0xffc7ba61,0x5f6b0d,0x43d881,
                //--6--
                0x3c5107,0xff8464dd,0xffc30dba,0x7b9b23,0x40a13f,
                //--7--
                0x23fb3c,0xffe4e205,0xffd73a3e,0x1b1dfb,0x44ca86,
                //HPF1
                0x0feb6c,0xffe02928,0xffc0a33a,0x7f59fc,0x0feb6c,
                0x3f3ba8,0xff8188b1,0xffc1874e,0x7e75ed,0x3f3ba8,
                //HPF2
                0x0feb6c,0xffe02928,0xffc0a33a,0x7f59fc,0x0feb6c,
                0x3f3ba8,0xff8188b1,0xffc1874e,0x7e75ed,0x3f3ba8
        }
};
#endif

#ifdef AUDIO_CHL_VOLUME_OFFSET_MAP_DEFAULT
static const INT32 _aai4MtkAudVolOffsetMap[AUD_VOLUME_OFFSET_IDX_NUM] =
{
    -18,
    -15,
    -12,
    -9,
    -6,
    -3,
    0,
    3,
    6,
    9,
    12,
    15,
    18
};
#endif
#ifdef AUD_CHL_SOUND_MODE_CFG_DEFAULT
static const AUD_POST_SM_T _arMtkSoundModeCase[AUD_SOUND_MODE_NUM][AUD_STREAM_FROM_NUM] =
{
    {//AUD_SOUND_MODE_DEFAULT
        AUD_SM_CASE0,//AUD_STREAM_FROM_OTHERS
        AUD_SM_CASE0,//AUD_STREAM_FROM_DIGITAL_TUNER
        AUD_SM_CASE0,//AUD_STREAM_FROM_ANALOG_TUNER
        AUD_SM_CASE0,//AUD_STREAM_FROM_SPDIF
        AUD_SM_CASE0,//AUD_STREAM_FROM_LINE_IN
        AUD_SM_CASE0,//AUD_STREAM_FROM_HDMI
        AUD_SM_CASE0,//AUD_STREAM_FROM_MEMORY
        AUD_SM_CASE0,//AUD_STREAM_FROM_BUF_AGT
        AUD_SM_CASE0,//AUD_STREAM_FROM_FEEDER
        AUD_SM_CASE0//AUD_STREAM_FROM_MULTI_MEDIA
    },
    {//AUD_SOUND_MODE_STANDARD
        AUD_SM_CASE0,//AUD_STREAM_FROM_OTHERS
        AUD_SM_CASE0,//AUD_STREAM_FROM_DIGITAL_TUNER
        AUD_SM_CASE7,//AUD_STREAM_FROM_ANALOG_TUNER
        AUD_SM_CASE0,//AUD_STREAM_FROM_SPDIF
        AUD_SM_CASE1,//AUD_STREAM_FROM_LINE_IN
        AUD_SM_CASE4,//AUD_STREAM_FROM_HDMI
        AUD_SM_CASE4,//AUD_STREAM_FROM_MEMORY
        AUD_SM_CASE0,//AUD_STREAM_FROM_BUF_AGT
        AUD_SM_CASE0,//AUD_STREAM_FROM_FEEDER
        AUD_SM_CASE4//AUD_STREAM_FROM_MULTI_MEDIA
    },
    {//AUD_SOUND_MODE_DYNAMIC
        AUD_SM_CASE0,//AUD_STREAM_FROM_OTHERS
        AUD_SM_CASE0,//AUD_STREAM_FROM_DIGITAL_TUNER
        AUD_SM_CASE6,//AUD_STREAM_FROM_ANALOG_TUNER
        AUD_SM_CASE0,//AUD_STREAM_FROM_SPDIF
        AUD_SM_CASE0,//AUD_STREAM_FROM_LINE_IN
        AUD_SM_CASE3,//AUD_STREAM_FROM_HDMI
        AUD_SM_CASE3,//AUD_STREAM_FROM_MEMORY
        AUD_SM_CASE0,//AUD_STREAM_FROM_BUF_AGT
        AUD_SM_CASE0,//AUD_STREAM_FROM_FEEDER
        AUD_SM_CASE3//AUD_STREAM_FROM_MULTI_MEDIA
    },
    {//AUD_SOUND_MODE_CLEAR_VOICE
        AUD_SM_CASE0,//AUD_STREAM_FROM_OTHERS
        AUD_SM_CASE0,//AUD_STREAM_FROM_DIGITAL_TUNER
        AUD_SM_CASE8,//AUD_STREAM_FROM_ANALOG_TUNER
        AUD_SM_CASE0,//AUD_STREAM_FROM_SPDIF
        AUD_SM_CASE2,//AUD_STREAM_FROM_LINE_IN
        AUD_SM_CASE5,//AUD_STREAM_FROM_HDMI
        AUD_SM_CASE5,//AUD_STREAM_FROM_MEMORY
        AUD_SM_CASE0,//AUD_STREAM_FROM_BUF_AGT
        AUD_SM_CASE0,//AUD_STREAM_FROM_FEEDER
        AUD_SM_CASE5//AUD_STREAM_FROM_MULTI_MEDIA
    }
};
#endif

//-----------------------------------------------------------------------------
// Bass and Trebel Configuration
//-----------------------------------------------------------------------------
#ifdef AUDIO_CHL_BASS_TREBLE_CFG_DEFAULT
static const UINT8 _aai1MtkAudBassTrebleTable[AUD_SM_NUM][2] =
{
//bass ,treble
    {18,26},
    {18,26},
    {18,26},
    {18,26},
    {18,26},
    {18,26},
    {18,26},
    {18,26},
    {18,26},
    {18,26},
    {18,26},
    {18,26}
};
#endif

//-----------------------------------------------------------------------------
// PEQ Configuration
//-----------------------------------------------------------------------------
#ifdef AUDIO_CHL_PEQ_FQ_CFG_DEFAULT
static const UINT16 _aai1MtkAudPeqFq[AUD_SM_NUM][PEQ_SETTING_MAX] =
{
    {100,  200,  500,  1000,  2000,  5000,  10000,  305,  3000},//case 0
    {100,  200,  500,  1000,  2000,  5000,  10000,  305,  3000},//case 1
    {100,  200,  500,  1000,  2000,  5000,  10000,  305,  3000},//case 2
    {100,  200,  500,  1000,  2000,  5000,  10000,  305,  3000},//case 3
    {100,  200,  500,  1000,  2000,  5000,  10000,  305,  3000},//case 4
    {100,  200,  500,  1000,  2000,  5000,  10000,  305,  3000},//case 5
    {100,  200,  500,  1000,  2000,  5000,  10000,  305,  3000},//case 6
    {100,  200,  500,  1000,  2000,  5000,  10000,  305,  3000},//case 7
    {100,  200,  500,  1000,  2000,  5000,  10000,  305,  3000},//case 8
    {100,  200,  500,  1000,  2000,  5000,  10000,  305,  3000},//case 9
    {100,  200,  500,  1000,  2000,  5000,  10000,  305,  3000},//case 10
    {100,  200,  500,  1000,  2000,  5000,  10000,  305,  3000},//case 11
};
#endif
#ifdef AUDIO_CHL_PEQ_BW_CFG_DEFAULT
static const UINT16 _aai1MtkAudPeqBw[AUD_SM_NUM][PEQ_SETTING_MAX] =
{
    {70,  90,  300,  1000,  1500,  2000,  5000,  150,  1600},
    {70,  90,  300,  1000,  1500,  2000,  5000,  150,  1600},
    {70,  90,  300,  1000,  1500,  2000,  5000,  150,  1600},
    {70,  90,  300,  1000,  1500,  2000,  5000,  150,  1600},
    {70,  90,  300,  1000,  1500,  2000,  5000,  150,  1600},
    {70,  90,  300,  1000,  1500,  2000,  5000,  150,  1600},
    {70,  90,  300,  1000,  1500,  2000,  5000,  150,  1600},
    {70,  90,  300,  1000,  1500,  2000,  5000,  150,  1600},
    {70,  90,  300,  1000,  1500,  2000,  5000,  150,  1600},
    {70,  90,  300,  1000,  1500,  2000,  5000,  150,  1600},
    {70,  90,  300,  1000,  1500,  2000,  5000,  150,  1600},
    {70,  90,  300,  1000,  1500,  2000,  5000,  150,  1600},
};
#endif
#ifdef AUDIO_CHL_PEQ_GAIN_CFG_DEFAULT
static const INT8 _aai1MtkAudPeqGain[AUD_SM_NUM][PEQ_SETTING_MAX] =
{
    {9,  1,  -5,  -0,  -2,  -1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2, -1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2, -1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2, -1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2, -1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2, -1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2, -1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2, -1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2, -1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2, -1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2, -1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2, -1,  15,  18,  5},
};
#endif

//-----------------------------------------------------------------------------
//Surround Configuration
//-----------------------------------------------------------------------------
#ifdef AUDIO_CHL_VS_MODE_CFG_DEFAULT
static const UINT32 _aai1MtkAudVSTable[AUD_VS_NUM][AUD_SUR_PRAR_NUM] =
{

    {0x100000, 0x1a0000, 0x200000, 0x90000, 0x50000, 0x10000, 0x7f4300},
    {0x100000, 0x1a0000, 0x200000, 0x90000, 0x50000, 0x10000, 0x7f4300},
    {0x100001, 0x1a0001, 0x200000, 0x90000, 0x50000, 0x10000, 0x7f4301},
    {0x100002, 0x1a0002, 0x200000, 0x90000, 0x50000, 0x10000, 0x7f4302},
    {0x100003, 0x1a0003, 0x200000, 0x90000, 0x50000, 0x10000, 0x7f4303}
};
#endif

//-----------------------------------------------------------------------------
// EQ Configuration
//-----------------------------------------------------------------------------
#ifdef AUDIO_CHL_EQ_CFG_DEFAULT
#ifdef CC_AUD_7BAND_EQUALIZER
static INT8 _aai1MtkAudChlEqCfg[AUD_EQ_NUM][CH_EQ_BAND_NO+1] = {
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 00 NONE   */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 01 ROCK   */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 02 POP    */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 03 LIVE   */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 04 DANCE  */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 05 TECHNO */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 06 Classic */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 07 Soft */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 08 User 1 */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 09 User 2 */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 10 User 3 */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 11 User 4 */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 12 User 5 */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 13 User 6 */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 14 User 7 */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 15 User 8 */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 16 BBE TV */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 17 BBE AV */
    { 0,   0,   0,   0,   0,   0,    0,   0},    /* 18 SBASS */
};
#else
static INT8 _aai1MtkAudChlEqCfg[AUD_EQ_NUM][CH_EQ_BAND_NO+1] = {
    { 0,   0 ,   0 ,   0 ,   0 ,   0  },    /* 00 NONE   */
    { 0,  18 ,   5 , (-2),   6 ,  18  },    /* 01 ROCK   */
    { 0, (-2),  12 ,  18 ,   6 , (-6) },    /* 02 POP    */
    { 0,  20 ,  10 , (-2),  16 ,  12  },    /* 03 LIVE   */
    { 0,  19 ,  11 , (-3), (-8), (-1) },    /* 04 DANCE  */
    { 0, (-6), (-1),   4 ,  14 ,  19  },    /* 05 TECHNO */
    { 0,   0 ,   0 ,   0 , (-3), (-8) },    /* 06 Classic */
    { 0,   9 ,   5 ,   0 , (-8),(-15) },    /* 07 Soft */
    { 0,   0,    0,    0,    0,    0  },    /* 08 User 1 */
    { 0,   0,    0,    0,    0,    0  },    /* 09 User 2 */
    { 0,   0,    0,    0,    0,    0  },    /* 10 User 3 */
    { 0,   0,    0,    0,    0,    0  },    /* 11 User 4 */
    { 0,   0,    0,    0,    0,    0  },    /* 12 User 5 */
    { 0,   0,    0,    0,    0,    0  },    /* 13 User 6 */
    { 0,   0,    0,    0,    0,    0  },    /* 14 User 7 */
    { 0,   0,    0,    0,    0,    0  },    /* 15 User 8 */
    { 0,   0,    0,    0,    0,    0  },    /* 16 BBE TV */
    { 0,   0,    0,    0,    0,    0  },    /* 17 BBE AV */
    { 0,   0,    0,    0,    0,    0  },    /* 18 SBASS */
};
#endif
#endif

#ifdef AUDIO_CHL_EQ_COEF_TBL_DEFAULT //20100524
#ifdef CC_AUD_7BAND_EQUALIZER
//MT5388 : 100, 200, 500, 1k, 2k, 5k, 10kHz
static const UINT32 _aau4MtkAudChlEqCoefTbl[(57+57+57+57+57+57+57)*5] =
{
    // 48000.0 Hz
    // Band 1, -14.0 dB ~ +14.0 dB (57 sets)
    0x003f34bc, 0xff8155b1, 0xffc152e9, 0x007eaa4f, 0x003f785b, //-14.0
    0x003f387f, 0xff814c2c, 0xffc14964, 0x007eb3d4, 0x003f7e1d, //-13.5
    0x003f3c19, 0xff8142eb, 0xffc14023, 0x007ebd15, 0x003f83c4, //-13.0
    0x003f3f8a, 0xff8139ec, 0xffc13724, 0x007ec614, 0x003f8952, //-12.5
    0x003f42d3, 0xff81312e, 0xffc12e65, 0x007eced2, 0x003f8ec8, //-12.0
    0x003f45f5, 0xff8128ad, 0xffc125e4, 0x007ed753, 0x003f9427, //-11.5
    0x003f48f0, 0xff812069, 0xffc11da0, 0x007edf97, 0x003f9970, //-11.0
    0x003f4bc5, 0xff811861, 0xffc11597, 0x007ee79f, 0x003f9ea4, //-10.5
    0x003f4e74, 0xff811091, 0xffc10dc7, 0x007eef6f, 0x003fa3c4, //-10.0
    0x003f50ff, 0xff8108fa, 0xffc10630, 0x007ef706, 0x003fa8d2, //-9.5
    0x003f5365, 0xff810198, 0xffc0fece, 0x007efe68, 0x003fadcd, //-9.0
    0x003f55a7, 0xff80fa6c, 0xffc0f7a1, 0x007f0594, 0x003fb2b8, //-8.5
    0x003f57c5, 0xff80f372, 0xffc0f0a8, 0x007f0c8e, 0x003fb793, //-8.0
    0x003f59c0, 0xff80ecab, 0xffc0e9e0, 0x007f1355, 0x003fbc60, //-7.5
    0x003f5b98, 0xff80e614, 0xffc0e34a, 0x007f19ec, 0x003fc11e, //-7.0
    0x003f5d4e, 0xff80dfad, 0xffc0dce2, 0x007f2053, 0x003fc5d0, //-6.5
    0x003f5ee1, 0xff80d973, 0xffc0d6a8, 0x007f268d, 0x003fca77, //-6.0
    0x003f6053, 0xff80d366, 0xffc0d09b, 0x007f2c9a, 0x003fcf12, //-5.5
    0x003f61a4, 0xff80cd84, 0xffc0cab9, 0x007f327c, 0x003fd3a3, //-5.0
    0x003f62d3, 0xff80c7cd, 0xffc0c502, 0x007f3833, 0x003fd82c, //-4.5
    0x003f63e1, 0xff80c23e, 0xffc0bf73, 0x007f3dc2, 0x003fdcac, //-4.0
    0x003f64ce, 0xff80bcd8, 0xffc0ba0c, 0x007f4328, 0x003fe126, //-3.5
    0x003f659a, 0xff80b798, 0xffc0b4cc, 0x007f4868, 0x003fe599, //-3.0
    0x003f6646, 0xff80b27e, 0xffc0afb2, 0x007f4d82, 0x003fea07, //-2.5
    0x003f66d2, 0xff80ad89, 0xffc0aabd, 0x007f5277, 0x003fee71, //-2.0
    0x003f673d, 0xff80a8b7, 0xffc0a5eb, 0x007f5749, 0x003ff2d8, //-1.5
    0x003f6789, 0xff80a408, 0xffc0a13c, 0x007f5bf8, 0x003ff73c, //-1.0
    0x003f67b4, 0xff809f7b, 0xffc09cae, 0x007f6085, 0x003ffb9e, //-0.5
    0x003f67be, 0xff809b0e, 0xffc09842, 0x007f64f2, 0x00400000, //0.0
    0x003f67a9, 0xff8096c1, 0xffc093f5, 0x007f693f, 0x00400462, //0.5
    0x003f6774, 0xff809293, 0xffc08fc7, 0x007f6d6d, 0x004008c6, //1.0
    0x003f671e, 0xff808e84, 0xffc08bb7, 0x007f717c, 0x00400d2b, //1.5
    0x003f66a8, 0xff808a91, 0xffc087c4, 0x007f756f, 0x00401194, //2.0
    0x003f6612, 0xff8086bb, 0xffc083ee, 0x007f7945, 0x00401600, //2.5
    0x003f655b, 0xff808301, 0xffc08034, 0x007f7cff, 0x00401a72, //3.0
    0x003f6483, 0xff807f61, 0xffc07c94, 0x007f809f, 0x00401ee9, //3.5
    0x003f638a, 0xff807bdc, 0xffc0790f, 0x007f8424, 0x00402367, //4.0
    0x003f6271, 0xff807870, 0xffc075a3, 0x007f8790, 0x004027ed, //4.5
    0x003f6135, 0xff80751c, 0xffc0724f, 0x007f8ae4, 0x00402c7b, //5.0
    0x003f5fd9, 0xff8071e1, 0xffc06f14, 0x007f8e1f, 0x00403114, //5.5
    0x003f5e5a, 0xff806ebd, 0xffc06bf0, 0x007f9143, 0x004035b6, //6.0
    0x003f5cb9, 0xff806baf, 0xffc068e2, 0x007f9451, 0x00403a65, //6.5
    0x003f5af6, 0xff8068b8, 0xffc065eb, 0x007f9748, 0x00403f20, //7.0
    0x003f590f, 0xff8065d6, 0xffc06309, 0x007f9a2a, 0x004043e8, //7.5
    0x003f5706, 0xff806309, 0xffc0603b, 0x007f9cf7, 0x004048bf, //8.0
    0x003f54d8, 0xff806050, 0xffc05d83, 0x007f9fb0, 0x00404da6, //8.5
    0x003f5286, 0xff805dab, 0xffc05add, 0x007fa255, 0x0040529d, //9.0
    0x003f500f, 0xff805b19, 0xffc0584b, 0x007fa4e7, 0x004057a6, //9.5
    0x003f4d73, 0xff805899, 0xffc055cb, 0x007fa767, 0x00405cc1, //10.0
    0x003f4ab1, 0xff80562c, 0xffc0535e, 0x007fa9d4, 0x004061f1, //10.5
    0x003f47c9, 0xff8053d0, 0xffc05102, 0x007fac30, 0x00406735, //11.0
    0x003f44b9, 0xff805185, 0xffc04eb7, 0x007fae7b, 0x00406c90, //11.5
    0x003f4182, 0xff804f4a, 0xffc04c7c, 0x007fb0b6, 0x00407201, //12.0
    0x003f3e22, 0xff804d20, 0xffc04a52, 0x007fb2e0, 0x0040778b, //12.5
    0x003f3a9a, 0xff804b06, 0xffc04838, 0x007fb4fa, 0x00407d2f, //13.0
    0x003f36e7, 0xff8048fa, 0xffc0462c, 0x007fb706, 0x004082ed, //13.5
    0x003f330a, 0xff8046fe, 0xffc04430, 0x007fb902, 0x004088c7, //14.0
    // Band 2, -14.0 dB ~ +14.0 dB (57 sets)
    0x000f9ba6, 0xffe0aa0f, 0xfff0a751, 0x001f55f1, 0x000fbd09, //-14.0
    0x000f9d7b, 0xffe0a568, 0xfff0a2aa, 0x001f5a98, 0x000fbfdc, //-13.5
    0x000f9f3b, 0xffe0a0e2, 0xfff09e23, 0x001f5f1e, 0x000fc2a2, //-13.0
    0x000fa0e7, 0xffe09c7b, 0xfff099bc, 0x001f6385, 0x000fc55c, //-12.5
    0x000fa281, 0xffe09833, 0xfff09574, 0x001f67cd, 0x000fc80b, //-12.0
    0x000fa407, 0xffe09409, 0xfff0914a, 0x001f6bf7, 0x000fcaaf, //-11.5
    0x000fa57b, 0xffe08ffd, 0xfff08d3d, 0x001f7003, 0x000fcd49, //-11.0
    0x000fa6db, 0xffe08c0c, 0xfff0894c, 0x001f73f4, 0x000fcfd9, //-10.5
    0x000fa82a, 0xffe08838, 0xfff08577, 0x001f77c8, 0x000fd25f, //-10.0
    0x000fa967, 0xffe0847e, 0xfff081bd, 0x001f7b82, 0x000fd4dc, //-9.5
    0x000faa92, 0xffe080de, 0xfff07e1d, 0x001f7f22, 0x000fd751, //-9.0
    0x000fabab, 0xffe07d58, 0xfff07a97, 0x001f82a8, 0x000fd9be, //-8.5
    0x000facb3, 0xffe079eb, 0xfff07729, 0x001f8615, 0x000fdc23, //-8.0
    0x000fadaa, 0xffe07696, 0xfff073d4, 0x001f896a, 0x000fde82, //-7.5
    0x000fae90, 0xffe07359, 0xfff07096, 0x001f8ca7, 0x000fe0da, //-7.0
    0x000faf64, 0xffe07032, 0xfff06d70, 0x001f8fce, 0x000fe32c, //-6.5
    0x000fb029, 0xffe06d22, 0xfff06a5f, 0x001f92de, 0x000fe578, //-6.0
    0x000fb0dc, 0xffe06a27, 0xfff06764, 0x001f95d9, 0x000fe7c0, //-5.5
    0x000fb17f, 0xffe06742, 0xfff0647e, 0x001f98be, 0x000fea02, //-5.0
    0x000fb212, 0xffe06471, 0xfff061ad, 0x001f9b8f, 0x000fec41, //-4.5
    0x000fb295, 0xffe061b4, 0xfff05ef0, 0x001f9e4c, 0x000fee7b, //-4.0
    0x000fb307, 0xffe05f0a, 0xfff05c46, 0x001fa0f6, 0x000ff0b3, //-3.5
    0x000fb369, 0xffe05c74, 0xfff059af, 0x001fa38c, 0x000ff2e7, //-3.0
    0x000fb3bc, 0xffe059f0, 0xfff0572b, 0x001fa610, 0x000ff519, //-2.5
    0x000fb3fe, 0xffe0577d, 0xfff054b8, 0x001fa883, 0x000ff74a, //-2.0
    0x000fb430, 0xffe0551c, 0xfff05257, 0x001faae4, 0x000ff978, //-1.5
    0x000fb453, 0xffe052cc, 0xfff05007, 0x001fad34, 0x000ffba6, //-1.0
    0x000fb466, 0xffe0508d, 0xfff04dc7, 0x001faf73, 0x000ffdd3, //-0.5
    0x000fb468, 0xffe04e5d, 0xfff04b98, 0x001fb1a3, 0x00100000, //0.0
    0x000fb45b, 0xffe04c3d, 0xfff04977, 0x001fb3c3, 0x0010022d, //0.5
    0x000fb43e, 0xffe04a2c, 0xfff04766, 0x001fb5d4, 0x0010045b, //1.0
    0x000fb411, 0xffe0482a, 0xfff04564, 0x001fb7d6, 0x0010068a, //1.5
    0x000fb3d5, 0xffe04637, 0xfff04370, 0x001fb9c9, 0x001008bb, //2.0
    0x000fb388, 0xffe04451, 0xfff0418a, 0x001fbbaf, 0x00100aee, //2.5
    0x000fb32a, 0xffe04279, 0xfff03fb2, 0x001fbd87, 0x00100d23, //3.0
    0x000fb2bd, 0xffe040ae, 0xfff03de7, 0x001fbf52, 0x00100f5c, //3.5
    0x000fb23f, 0xffe03ef0, 0xfff03c29, 0x001fc110, 0x00101198, //4.0
    0x000fb1b1, 0xffe03d3e, 0xfff03a77, 0x001fc2c2, 0x001013d8, //4.5
    0x000fb113, 0xffe03b98, 0xfff038d1, 0x001fc468, 0x0010161c, //5.0
    0x000fb063, 0xffe039ff, 0xfff03737, 0x001fc601, 0x00101865, //5.5
    0x000fafa3, 0xffe03870, 0xfff035a9, 0x001fc790, 0x00101ab4, //6.0
    0x000faed2, 0xffe036ed, 0xfff03426, 0x001fc913, 0x00101d08, //6.5
    0x000fadf0, 0xffe03575, 0xfff032ad, 0x001fca8b, 0x00101f63, //7.0
    0x000facfc, 0xffe03407, 0xfff0313f, 0x001fcbf9, 0x001021c5, //7.5
    0x000fabf7, 0xffe032a4, 0xfff02fdc, 0x001fcd5c, 0x0010242e, //8.0
    0x000faae0, 0xffe0314a, 0xfff02e82, 0x001fceb6, 0x0010269e, //8.5
    0x000fa9b6, 0xffe02ffa, 0xfff02d32, 0x001fd006, 0x00102917, //9.0
    0x000fa87b, 0xffe02eb4, 0xfff02bec, 0x001fd14c, 0x00102b99, //9.5
    0x000fa72d, 0xffe02d77, 0xfff02aae, 0x001fd289, 0x00102e25, //10.0
    0x000fa5cc, 0xffe02c42, 0xfff0297a, 0x001fd3be, 0x001030ba, //10.5
    0x000fa458, 0xffe02b17, 0xfff0284e, 0x001fd4e9, 0x0010335a, //11.0
    0x000fa2d1, 0xffe029f4, 0xfff0272b, 0x001fd60c, 0x00103605, //11.5
    0x000fa135, 0xffe028d9, 0xfff02610, 0x001fd727, 0x001038bb, //12.0
    0x000f9f86, 0xffe027c5, 0xfff024fc, 0x001fd83b, 0x00103b7e, //12.5
    0x000f9dc2, 0xffe026ba, 0xfff023f1, 0x001fd946, 0x00103e4d, //13.0
    0x000f9be9, 0xffe025b6, 0xfff022ed, 0x001fda4a, 0x0010412a, //13.5
    0x000f99fb, 0xffe024ba, 0xfff021f0, 0x001fdb46, 0x00104414, //14.0
    // Band 3, -14.0 dB ~ +14.0 dB (57 sets)
    0x000f0b9f, 0xffe1a835, 0xfff19775, 0x001e57cb, 0x000f5cec, //-14.0
    0x000f0fe0, 0xffe19d3b, 0xfff18c75, 0x001e62c5, 0x000f63ab, //-13.5
    0x000f13f4, 0xffe19289, 0xfff181bd, 0x001e6d77, 0x000f6a4f, //-13.0
    0x000f17db, 0xffe1881d, 0xfff1774c, 0x001e77e3, 0x000f70da, //-12.5
    0x000f1b95, 0xffe17df7, 0xfff16d1f, 0x001e8209, 0x000f774c, //-12.0
    0x000f1f23, 0xffe17413, 0xfff16336, 0x001e8bed, 0x000f7da6, //-11.5
    0x000f2286, 0xffe16a71, 0xfff1598f, 0x001e958f, 0x000f83eb, //-11.0
    0x000f25be, 0xffe16110, 0xfff15028, 0x001e9ef0, 0x000f8a19, //-10.5
    0x000f28cc, 0xffe157ec, 0xfff14700, 0x001ea814, 0x000f9034, //-10.0
    0x000f2baf, 0xffe14f06, 0xfff13e15, 0x001eb0fa, 0x000f963b, //-9.5
    0x000f2e69, 0xffe1465c, 0xfff13566, 0x001eb9a4, 0x000f9c31, //-9.0
    0x000f30fa, 0xffe13dec, 0xfff12cf1, 0x001ec214, 0x000fa215, //-8.5
    0x000f3361, 0xffe135b5, 0xfff124b6, 0x001eca4b, 0x000fa7e9, //-8.0
    0x000f35a0, 0xffe12db5, 0xfff11cb2, 0x001ed24b, 0x000fadae, //-7.5
    0x000f37b7, 0xffe125ec, 0xfff114e4, 0x001eda14, 0x000fb365, //-7.0
    0x000f39a5, 0xffe11e57, 0xfff10d4b, 0x001ee1a9, 0x000fb910, //-6.5
    0x000f3b6c, 0xffe116f6, 0xfff105e5, 0x001ee90a, 0x000fbeae, //-6.0
    0x000f3d0c, 0xffe10fc6, 0xfff0feb2, 0x001ef03a, 0x000fc442, //-5.5
    0x000f3e83, 0xffe108c8, 0xfff0f7b0, 0x001ef738, 0x000fc9cc, //-5.0
    0x000f3fd4, 0xffe101fa, 0xfff0f0de, 0x001efe06, 0x000fcf4e, //-4.5
    0x000f40fe, 0xffe0fb5a, 0xfff0ea3b, 0x001f04a6, 0x000fd4c8, //-4.0
    0x000f4201, 0xffe0f4e8, 0xfff0e3c5, 0x001f0b18, 0x000fda3b, //-3.5
    0x000f42dd, 0xffe0eea2, 0xfff0dd7b, 0x001f115e, 0x000fdfa8, //-3.0
    0x000f4392, 0xffe0e887, 0xfff0d75d, 0x001f1779, 0x000fe512, //-2.5
    0x000f4420, 0xffe0e296, 0xfff0d168, 0x001f1d6a, 0x000fea77, //-2.0
    0x000f4488, 0xffe0dcce, 0xfff0cb9d, 0x001f2332, 0x000fefda, //-1.5
    0x000f44ca, 0xffe0d72e, 0xfff0c5fa, 0x001f28d2, 0x000ff53c, //-1.0
    0x000f44e4, 0xffe0d1b4, 0xfff0c07e, 0x001f2e4c, 0x000ffa9e, //-0.5
    0x000f44d8, 0xffe0cc61, 0xfff0bb28, 0x001f339f, 0x00100000, //0.0
    0x000f44a5, 0xffe0c733, 0xfff0b5f7, 0x001f38cd, 0x00100564, //0.5
    0x000f444b, 0xffe0c229, 0xfff0b0e9, 0x001f3dd7, 0x00100acb, //1.0
    0x000f43ca, 0xffe0bd41, 0xfff0abff, 0x001f42bf, 0x00101036, //1.5
    0x000f4322, 0xffe0b87c, 0xfff0a738, 0x001f4784, 0x001015a6, //2.0
    0x000f4253, 0xffe0b3d8, 0xfff0a291, 0x001f4c28, 0x00101b1c, //2.5
    0x000f415b, 0xffe0af55, 0xfff09e0b, 0x001f50ab, 0x00102099, //3.0
    0x000f403c, 0xffe0aaf1, 0xfff099a5, 0x001f550f, 0x0010261f, //3.5
    0x000f3ef4, 0xffe0a6ac, 0xfff0955d, 0x001f5954, 0x00102bae, //4.0
    0x000f3d84, 0xffe0a284, 0xfff09134, 0x001f5d7c, 0x00103148, //4.5
    0x000f3beb, 0xffe09e7a, 0xfff08d27, 0x001f6186, 0x001036ee, //5.0
    0x000f3a29, 0xffe09a8c, 0xfff08937, 0x001f6574, 0x00103ca0, //5.5
    0x000f383d, 0xffe096ba, 0xfff08563, 0x001f6946, 0x00104260, //6.0
    0x000f3627, 0xffe09302, 0xfff081a9, 0x001f6cfe, 0x00104830, //6.5
    0x000f33e6, 0xffe08f65, 0xfff07e0a, 0x001f709b, 0x00104e10, //7.0
    0x000f317a, 0xffe08be1, 0xfff07a84, 0x001f741f, 0x00105402, //7.5
    0x000f2ee2, 0xffe08876, 0xfff07717, 0x001f778a, 0x00105a07, //8.0
    0x000f2c1e, 0xffe08523, 0xfff073c3, 0x001f7add, 0x0010601f, //8.5
    0x000f292d, 0xffe081e8, 0xfff07085, 0x001f7e18, 0x0010664e, //9.0
    0x000f260f, 0xffe07ec3, 0xfff06d5f, 0x001f813d, 0x00106c92, //9.5
    0x000f22c2, 0xffe07bb5, 0xfff06a4f, 0x001f844b, 0x001072ef, //10.0
    0x000f1f46, 0xffe078bc, 0xfff06754, 0x001f8744, 0x00107965, //10.5
    0x000f1b9b, 0xffe075d8, 0xfff0646f, 0x001f8a28, 0x00107ff6, //11.0
    0x000f17bf, 0xffe07309, 0xfff0619e, 0x001f8cf7, 0x001086a2, //11.5
    0x000f13b2, 0xffe0704e, 0xfff05ee2, 0x001f8fb2, 0x00108d6c, //12.0
    0x000f0f73, 0xffe06da6, 0xfff05c38, 0x001f925a, 0x00109455, //12.5
    0x000f0b00, 0xffe06b11, 0xfff059a2, 0x001f94ef, 0x00109b5e, //13.0
    0x000f0659, 0xffe0688e, 0xfff0571e, 0x001f9772, 0x0010a289, //13.5
    0x000f017d, 0xffe0661d, 0xfff054ab, 0x001f99e3, 0x0010a9d7, //14.0
    // Band 4, -14.0 dB ~ +14.0 dB (57 sets)
    0x000e304d, 0xffe3449c, 0xfff30523, 0x001cbb64, 0x000eca90, //-14.0
    0x000e37cb, 0xffe330dd, 0xfff2f138, 0x001ccf23, 0x000ed6fd, //-13.5
    0x000e3efc, 0xffe31d93, 0xfff2ddc3, 0x001ce26d, 0x000ee341, //-13.0
    0x000e45df, 0xffe30abc, 0xfff2cac3, 0x001cf544, 0x000eef5e, //-12.5
    0x000e4c75, 0xffe2f856, 0xfff2b835, 0x001d07aa, 0x000efb56, //-12.0
    0x000e52bf, 0xffe2e660, 0xfff2a616, 0x001d19a0, 0x000f072b, //-11.5
    0x000e58bd, 0xffe2d4d6, 0xfff29466, 0x001d2b2a, 0x000f12dd, //-11.0
    0x000e5e6f, 0xffe2c3b8, 0xfff28322, 0x001d3c48, 0x000f1e6f, //-10.5
    0x000e63d6, 0xffe2b303, 0xfff27248, 0x001d4cfd, 0x000f29e2, //-10.0
    0x000e68f2, 0xffe2a2b5, 0xfff261d6, 0x001d5d4b, 0x000f3538, //-9.5
    0x000e6dc4, 0xffe292cc, 0xfff251ca, 0x001d6d34, 0x000f4073, //-9.0
    0x000e724b, 0xffe28345, 0xfff24221, 0x001d7cbb, 0x000f4b94, //-8.5
    0x000e7689, 0xffe27420, 0xfff232db, 0x001d8be0, 0x000f569c, //-8.0
    0x000e7a7d, 0xffe2655a, 0xfff223f4, 0x001d9aa6, 0x000f618f, //-7.5
    0x000e7e28, 0xffe256f1, 0xfff2156b, 0x001da90f, 0x000f6c6d, //-7.0
    0x000e818a, 0xffe248e4, 0xfff2073e, 0x001db71c, 0x000f7738, //-6.5
    0x000e84a2, 0xffe23b2f, 0xfff1f96c, 0x001dc4d1, 0x000f81f2, //-6.0
    0x000e8772, 0xffe22dd2, 0xfff1ebf1, 0x001dd22e, 0x000f8c9c, //-5.5
    0x000e89fa, 0xffe220cb, 0xfff1decd, 0x001ddf35, 0x000f9739, //-5.0
    0x000e8c38, 0xffe21417, 0xfff1d1fd, 0x001debe9, 0x000fa1cb, //-4.5
    0x000e8e2e, 0xffe207b5, 0xfff1c580, 0x001df84b, 0x000fac52, //-4.0
    0x000e8fdc, 0xffe1fba3, 0xfff1b953, 0x001e045d, 0x000fb6d1, //-3.5
    0x000e9141, 0xffe1efe0, 0xfff1ad76, 0x001e1020, 0x000fc149, //-3.0
    0x000e925d, 0xffe1e469, 0xfff1a1e6, 0x001e1b97, 0x000fcbbe, //-2.5
    0x000e9330, 0xffe1d93d, 0xfff196a1, 0x001e26c3, 0x000fd62f, //-2.0
    0x000e93ba, 0xffe1ce5a, 0xfff18ba6, 0x001e31a6, 0x000fe0a0, //-1.5
    0x000e93fb, 0xffe1c3bf, 0xfff180f4, 0x001e3c41, 0x000feb12, //-1.0
    0x000e93f2, 0xffe1b96a, 0xfff17688, 0x001e4696, 0x000ff586, //-0.5
    0x000e93a0, 0xffe1af59, 0xfff16c60, 0x001e50a7, 0x00100000, //0.0
    0x000e9303, 0xffe1a58a, 0xfff1627c, 0x001e5a76, 0x00100a81, //0.5
    0x000e921c, 0xffe19bfd, 0xfff158da, 0x001e6403, 0x0010150a, //1.0
    0x000e90ea, 0xffe192af, 0xfff14f78, 0x001e6d51, 0x00101f9e, //1.5
    0x000e8f6c, 0xffe189a0, 0xfff14654, 0x001e7660, 0x00102a3f, //2.0
    0x000e8da3, 0xffe180cd, 0xfff13d6e, 0x001e7f33, 0x001034ef, //2.5
    0x000e8b8d, 0xffe17835, 0xfff134c3, 0x001e87cb, 0x00103fb0, //3.0
    0x000e8929, 0xffe16fd7, 0xfff12c53, 0x001e9029, 0x00104a84, //3.5
    0x000e8678, 0xffe167b2, 0xfff1241b, 0x001e984e, 0x0010556d, //4.0
    0x000e8378, 0xffe15fc3, 0xfff11c1b, 0x001ea03d, 0x0010606d, //4.5
    0x000e8028, 0xffe1580a, 0xfff11451, 0x001ea7f6, 0x00106b87, //5.0
    0x000e7c88, 0xffe15086, 0xfff10cbc, 0x001eaf7a, 0x001076bc, //5.5
    0x000e7897, 0xffe14935, 0xfff1055a, 0x001eb6cb, 0x0010820f, //6.0
    0x000e7453, 0xffe14215, 0xfff0fe2b, 0x001ebdeb, 0x00108d82, //6.5
    0x000e6fbc, 0xffe13b26, 0xfff0f72d, 0x001ec4da, 0x00109917, //7.0
    0x000e6ad1, 0xffe13466, 0xfff0f05e, 0x001ecb9a, 0x0010a4d1, //7.5
    0x000e6590, 0xffe12dd5, 0xfff0e9be, 0x001ed22b, 0x0010b0b2, //8.0
    0x000e5ff8, 0xffe12770, 0xfff0e34b, 0x001ed890, 0x0010bcbd, //8.5
    0x000e5a08, 0xffe12137, 0xfff0dd05, 0x001edec9, 0x0010c8f3, //9.0
    0x000e53be, 0xffe11b2a, 0xfff0d6ea, 0x001ee4d6, 0x0010d558, //9.5
    0x000e4d1a, 0xffe11545, 0xfff0d0f9, 0x001eeabb, 0x0010e1ed, //10.0
    0x000e4619, 0xffe10f8a, 0xfff0cb31, 0x001ef076, 0x0010eeb6, //10.5
    0x000e3eba, 0xffe109f6, 0xfff0c590, 0x001ef60a, 0x0010fbb6, //11.0
    0x000e36fb, 0xffe10489, 0xfff0c017, 0x001efb77, 0x001108ee, //11.5
    0x000e2edb, 0xffe0ff41, 0xfff0bac4, 0x001f00bf, 0x00111661, //12.0
    0x000e2658, 0xffe0fa1e, 0xfff0b595, 0x001f05e2, 0x00112413, //12.5
    0x000e1d70, 0xffe0f51f, 0xfff0b08b, 0x001f0ae1, 0x00113205, //13.0
    0x000e1421, 0xffe0f042, 0xfff0aba3, 0x001f0fbe, 0x0011403c, //13.5
    0x000e0a68, 0xffe0eb87, 0xfff0a6de, 0x001f1479, 0x00114eba, //14.0
    // Band 5, -14.0 dB ~ +14.0 dB (57 sets)
    0x000cb833, 0xffe65e8f, 0xfff57838, 0x0019a171, 0x000dcf95, //-14.0
    0x000cc3f8, 0xffe63e95, 0xfff5571f, 0x0019c16b, 0x000de4e9, //-13.5
    0x000ccf47, 0xffe61f37, 0xfff536a7, 0x0019e0c9, 0x000dfa12, //-13.0
    0x000cda20, 0xffe60073, 0xfff516cf, 0x0019ff8d, 0x000e0f12, //-12.5
    0x000ce482, 0xffe5e249, 0xfff4f795, 0x001a1db7, 0x000e23ea, //-12.0
    0x000cee6c, 0xffe5c4b6, 0xfff4d8f8, 0x001a3b4a, 0x000e389c, //-11.5
    0x000cf7df, 0xffe5a7b9, 0xfff4baf7, 0x001a5847, 0x000e4d2a, //-11.0
    0x000d00d9, 0xffe58b51, 0xfff49d90, 0x001a74af, 0x000e6197, //-10.5
    0x000d095b, 0xffe56f7d, 0xfff480c2, 0x001a9083, 0x000e75e3, //-10.0
    0x000d1163, 0xffe5543b, 0xfff4648a, 0x001aabc5, 0x000e8a12, //-9.5
    0x000d18f3, 0xffe53988, 0xfff448e8, 0x001ac678, 0x000e9e25, //-9.0
    0x000d2008, 0xffe51f65, 0xfff42dda, 0x001ae09b, 0x000eb21f, //-8.5
    0x000d26a2, 0xffe505ce, 0xfff4135d, 0x001afa32, 0x000ec601, //-8.0
    0x000d2cc2, 0xffe4ecc2, 0xfff3f970, 0x001b133e, 0x000ed9ce, //-7.5
    0x000d3266, 0xffe4d43f, 0xfff3e011, 0x001b2bc1, 0x000eed89, //-7.0
    0x000d378e, 0xffe4bc44, 0xfff3c73e, 0x001b43bc, 0x000f0134, //-6.5
    0x000d3c39, 0xffe4a4ce, 0xfff3aef5, 0x001b5b32, 0x000f14d2, //-6.0
    0x000d4067, 0xffe48ddc, 0xfff39735, 0x001b7224, 0x000f2865, //-5.5
    0x000d4416, 0xffe4776b, 0xfff37ffb, 0x001b8895, 0x000f3bef, //-5.0
    0x000d4748, 0xffe4617a, 0xfff36945, 0x001b9e86, 0x000f4f74, //-4.5
    0x000d49f9, 0xffe44c07, 0xfff35311, 0x001bb3f9, 0x000f62f6, //-4.0
    0x000d4c2b, 0xffe43710, 0xfff33d5d, 0x001bc8f0, 0x000f7678, //-3.5
    0x000d4ddc, 0xffe42293, 0xfff32828, 0x001bdd6d, 0x000f89fd, //-3.0
    0x000d4f0a, 0xffe40e8d, 0xfff3136e, 0x001bf173, 0x000f9d88, //-2.5
    0x000d4fb7, 0xffe3fafd, 0xfff2ff2e, 0x001c0503, 0x000fb11b, //-2.0
    0x000d4fdf, 0xffe3e7e1, 0xfff2eb66, 0x001c181f, 0x000fc4bb, //-1.5
    0x000d4f82, 0xffe3d537, 0xfff2d814, 0x001c2ac9, 0x000fd869, //-1.0
    0x000d4ea0, 0xffe3c2fc, 0xfff2c536, 0x001c3d04, 0x000fec2a, //-0.5
    0x000d4d37, 0xffe3b12f, 0xfff2b2c9, 0x001c4ed1, 0x00100000, //0.0
    0x000d4b45, 0xffe39fce, 0xfff2a0cc, 0x001c6032, 0x001013ef, //0.5
    0x000d48ca, 0xffe38ed7, 0xfff28f3c, 0x001c7129, 0x001027fa, //1.0
    0x000d45c4, 0xffe37e47, 0xfff27e18, 0x001c81b9, 0x00103c24, //1.5
    0x000d4232, 0xffe36e1d, 0xfff26d5d, 0x001c91e3, 0x00105071, //2.0
    0x000d3e12, 0xffe35e57, 0xfff25d09, 0x001ca1a9, 0x001064e5, //2.5
    0x000d3962, 0xffe34ef3, 0xfff24d1a, 0x001cb10d, 0x00107983, //3.0
    0x000d3422, 0xffe33fef, 0xfff23d8f, 0x001cc011, 0x00108e4f, //3.5
    0x000d2e4e, 0xffe33149, 0xfff22e65, 0x001cceb7, 0x0010a34d, //4.0
    0x000d27e5, 0xffe322fe, 0xfff21f9a, 0x001cdd02, 0x0010b880, //4.5
    0x000d20e6, 0xffe3150e, 0xfff2112d, 0x001ceaf2, 0x0010cdec, //5.0
    0x000d194e, 0xffe30777, 0xfff2031b, 0x001cf889, 0x0010e396, //5.5
    0x000d111c, 0xffe2fa35, 0xfff1f563, 0x001d05cb, 0x0010f981, //6.0
    0x000d084c, 0xffe2ed49, 0xfff1e802, 0x001d12b7, 0x00110fb2, //6.5
    0x000cfedc, 0xffe2e0af, 0xfff1daf7, 0x001d1f51, 0x0011262d, //7.0
    0x000cf4ca, 0xffe2d466, 0xfff1ce40, 0x001d2b9a, 0x00113cf5, //7.5
    0x000cea14, 0xffe2c86d, 0xfff1c1db, 0x001d3793, 0x00115411, //8.0
    0x000cdeb7, 0xffe2bcc1, 0xfff1b5c6, 0x001d433f, 0x00116b83, //8.5
    0x000cd2af, 0xffe2b161, 0xfff1aa00, 0x001d4e9f, 0x00118351, //9.0
    0x000cc5fb, 0xffe2a64b, 0xfff19e86, 0x001d59b5, 0x00119b7f, //9.5
    0x000cb896, 0xffe29b7d, 0xfff19358, 0x001d6483, 0x0011b412, //10.0
    0x000caa7f, 0xffe290f7, 0xfff18873, 0x001d6f09, 0x0011cd0f, //10.5
    0x000c9bb0, 0xffe286b5, 0xfff17dd5, 0x001d794b, 0x0011e67b, //11.0
    0x000c8c28, 0xffe27cb8, 0xfff1737d, 0x001d8348, 0x0012005b, //11.5
    0x000c7be2, 0xffe272fc, 0xfff1696a, 0x001d8d04, 0x00121ab4, //12.0
    0x000c6adb, 0xffe26981, 0xfff15f9a, 0x001d967f, 0x0012358b, //12.5
    0x000c590f, 0xffe26045, 0xfff1560b, 0x001d9fbb, 0x001250e7, //13.0
    0x000c4679, 0xffe25746, 0xfff14cbb, 0x001da8ba, 0x00126ccc, //13.5
    0x000c3316, 0xffe24e84, 0xfff143aa, 0x001db17c, 0x00128940, //14.0
    // Band 6, -14.0 dB ~ +14.0 dB (57 sets)
    0x0009b6d6, 0xffeef2a3, 0xfffa7aff, 0x00110d5d, 0x000bce2a, //-14.0
    0x0009c646, 0xffeec9b1, 0xfffa4754, 0x0011364f, 0x000bf267, //-13.5
    0x0009d518, 0xffeea12a, 0xfffa142f, 0x00115ed6, 0x000c16b9, //-13.0
    0x0009e347, 0xffee7912, 0xfff9e195, 0x001186ee, 0x000c3b23, //-12.5
    0x0009f0d1, 0xffee516a, 0xfff9af8a, 0x0011ae96, 0x000c5fa6, //-12.0
    0x0009fdb0, 0xffee2a34, 0xfff97e0e, 0x0011d5cc, 0x000c8442, //-11.5
    0x000a09e0, 0xffee0373, 0xfff94d27, 0x0011fc8d, 0x000ca8f9, //-11.0
    0x000a155d, 0xffeddd29, 0xfff91cd5, 0x001222d7, 0x000ccdce, //-10.5
    0x000a2024, 0xffedb758, 0xfff8ed1b, 0x001248a8, 0x000cf2c1, //-10.0
    0x000a2a2f, 0xffed9201, 0xfff8bdfc, 0x00126dff, 0x000d17d5, //-9.5
    0x000a337b, 0xffed6d26, 0xfff88f7a, 0x001292da, 0x000d3d0b, //-9.0
    0x000a3c03, 0xffed48c9, 0xfff86195, 0x0012b737, 0x000d6267, //-8.5
    0x000a43c4, 0xffed24ea, 0xfff83451, 0x0012db16, 0x000d87eb, //-8.0
    0x000a4ab9, 0xffed018b, 0xfff807ae, 0x0012fe75, 0x000dad99, //-7.5
    0x000a50df, 0xffecdead, 0xfff7dbae, 0x00132153, 0x000dd374, //-7.0
    0x000a5630, 0xffecbc51, 0xfff7b051, 0x001343af, 0x000df97f, //-6.5
    0x000a5aa9, 0xffec9a78, 0xfff78599, 0x00136588, 0x000e1fbe, //-6.0
    0x000a5e45, 0xffec7921, 0xfff75b87, 0x001386df, 0x000e4634, //-5.5
    0x000a6101, 0xffec584e, 0xfff7321a, 0x0013a7b2, 0x000e6ce5, //-5.0
    0x000a62d8, 0xffec37ff, 0xfff70955, 0x0013c801, 0x000e93d4, //-4.5
    0x000a63c5, 0xffec1835, 0xfff6e136, 0x0013e7cb, 0x000ebb05, //-4.0
    0x000a63c5, 0xffebf8ef, 0xfff6b9bf, 0x00140711, 0x000ee27d, //-3.5
    0x000a62d3, 0xffebda2d, 0xfff692ee, 0x001425d3, 0x000f0a3f, //-3.0
    0x000a60ea, 0xffebbbf0, 0xfff66cc5, 0x00144410, 0x000f3251, //-2.5
    0x000a5e06, 0xffeb9e37, 0xfff64742, 0x001461c9, 0x000f5ab7, //-2.0
    0x000a5a23, 0xffeb8102, 0xfff62266, 0x00147efe, 0x000f8377, //-1.5
    0x000a553b, 0xffeb6450, 0xfff5fe30, 0x00149bb0, 0x000fac94, //-1.0
    0x000a4f4b, 0xffeb4821, 0xfff5daa0, 0x0014b7df, 0x000fd616, //-0.5
    0x000a484c, 0xffeb2c75, 0xfff5b7b4, 0x0014d38b, 0x00100000, //0.0
    0x000a403b, 0xffeb114b, 0xfff5956b, 0x0014eeb5, 0x00102a59, //0.5
    0x000a3712, 0xffeaf6a2, 0xfff573c6, 0x0015095e, 0x00105528, //1.0
    0x000a2ccc, 0xffeadc79, 0xfff552c3, 0x00152387, 0x00108071, //1.5
    0x000a2164, 0xffeac2cf, 0xfff53260, 0x00153d31, 0x0010ac3c, //2.0
    0x000a14d4, 0xffeaa9a4, 0xfff5129d, 0x0015565c, 0x0010d88f, //2.5
    0x000a0717, 0xffea90f6, 0xfff4f378, 0x00156f0a, 0x00110570, //3.0
    0x0009f828, 0xffea78c5, 0xfff4d4f0, 0x0015873b, 0x001132e8, //3.5
    0x0009e7ff, 0xffea610f, 0xfff4b704, 0x00159ef1, 0x001160fd, //4.0
    0x0009d699, 0xffea49d3, 0xfff499b1, 0x0015b62d, 0x00118fb6, //4.5
    0x0009c3ed, 0xffea330f, 0xfff47cf7, 0x0015ccf1, 0x0011bf1c, //5.0
    0x0009aff7, 0xffea1cc3, 0xfff460d4, 0x0015e33d, 0x0011ef36, //5.5
    0x00099aaf, 0xffea06ed, 0xfff44545, 0x0015f913, 0x0012200c, //6.0
    0x0009840f, 0xffe9f18c, 0xfff42a4a, 0x00160e74, 0x001251a7, //6.5
    0x00096c10, 0xffe9dc9e, 0xfff40fe1, 0x00162362, 0x0012840f, //7.0
    0x000952ab, 0xffe9c822, 0xfff3f607, 0x001637de, 0x0012b74e, //7.5
    0x000937d9, 0xffe9b416, 0xfff3dcbb, 0x00164bea, 0x0012eb6c, //8.0
    0x00091b92, 0xffe9a079, 0xfff3c3fa, 0x00165f87, 0x00132073, //8.5
    0x0008fdcf, 0xffe98d49, 0xfff3abc4, 0x001672b7, 0x0013566d, //9.0
    0x0008de87, 0xffe97a86, 0xfff39415, 0x0016857a, 0x00138d63, //9.5
    0x0008bdb3, 0xffe9682c, 0xfff37ced, 0x001697d4, 0x0013c560, //10.0
    0x00089b49, 0xffe9563b, 0xfff36648, 0x0016a9c5, 0x0013fe6e, //10.5
    0x00087742, 0xffe944b0, 0xfff35026, 0x0016bb50, 0x00143899, //11.0
    0x00085193, 0xffe9338b, 0xfff33a83, 0x0016cc75, 0x001473ea, //11.5
    0x00082a34, 0xffe922ca, 0xfff3255e, 0x0016dd36, 0x0014b06e, //12.0
    0x0008011c, 0xffe9126b, 0xfff310b5, 0x0016ed95, 0x0014ee30, //12.5
    0x0007d63f, 0xffe9026c, 0xfff2fc85, 0x0016fd94, 0x00152d3c, //13.0
    0x0007a995, 0xffe8f2cc, 0xfff2e8cd, 0x00170d34, 0x00156d9e, //13.5
    0x00077b12, 0xffe8e389, 0xfff2d58b, 0x00171c77, 0x0015af64, //14.0
    // Band 7, -14.0 dB ~ +14.0 dB (57 sets)
    0x0007a4b7, 0xfffb52ef, 0xfffdeee2, 0x0004ad11, 0x000a6c67, //-14.0
    0x0007b2ac, 0xfffb43f6, 0xfffdb508, 0x0004bc0a, 0x000a984d, //-13.5
    0x0007bff4, 0xfffb350d, 0xfffd7b6b, 0x0004caf3, 0x000ac4a1, //-13.0
    0x0007cc8a, 0xfffb2635, 0xfffd4211, 0x0004d9cb, 0x000af166, //-12.5
    0x0007d865, 0xfffb1770, 0xfffd08ff, 0x0004e890, 0x000b1e9c, //-12.0
    0x0007e37f, 0xfffb08be, 0xfffcd03a, 0x0004f742, 0x000b4c46, //-11.5
    0x0007edd0, 0xfffafa23, 0xfffc97c9, 0x000505dd, 0x000b7a67, //-11.0
    0x0007f751, 0xfffaeb9e, 0xfffc5fb0, 0x00051462, 0x000ba8fe, //-10.5
    0x0007fffb, 0xfffadd31, 0xfffc27f5, 0x000522cf, 0x000bd811, //-10.0
    0x000807c5, 0xfffacede, 0xfffbf09b, 0x00053122, 0x000c07a0, //-9.5
    0x00080ea8, 0xfffac0a5, 0xfffbb9a9, 0x00053f5b, 0x000c37af, //-9.0
    0x0008149c, 0xfffab288, 0xfffb8323, 0x00054d78, 0x000c6841, //-8.5
    0x0008199b, 0xfffaa489, 0xfffb4d0c, 0x00055b77, 0x000c9959, //-8.0
    0x00081d9b, 0xfffa96a7, 0xfffb176b, 0x00056959, 0x000ccafa, //-7.5
    0x00082095, 0xfffa88e5, 0xfffae242, 0x0005771b, 0x000cfd29, //-7.0
    0x00082282, 0xfffa7b43, 0xfffaad95, 0x000584bd, 0x000d2fe8, //-6.5
    0x00082359, 0xfffa6dc2, 0xfffa796a, 0x0005923e, 0x000d633d, //-6.0
    0x00082313, 0xfffa6064, 0xfffa45c2, 0x00059f9c, 0x000d972c, //-5.5
    0x000821a6, 0xfffa5328, 0xfffa12a2, 0x0005acd8, 0x000dcbb8, //-5.0
    0x00081f0b, 0xfffa4611, 0xfff9e00c, 0x0005b9ef, 0x000e00e8, //-4.5
    0x00081b3b, 0xfffa391e, 0xfff9ae05, 0x0005c6e2, 0x000e36c1, //-4.0
    0x0008162b, 0xfffa2c51, 0xfff97c8e, 0x0005d3af, 0x000e6d47, //-3.5
    0x00080fd4, 0xfffa1fa9, 0xfff94bab, 0x0005e057, 0x000ea481, //-3.0
    0x0008082e, 0xfffa1329, 0xfff91b5e, 0x0005ecd7, 0x000edc75, //-2.5
    0x0007ff2f, 0xfffa06d0, 0xfff8eba9, 0x0005f930, 0x000f1528, //-2.0
    0x0007f4cf, 0xfff9fa9f, 0xfff8bc8e, 0x00060561, 0x000f4ea3, //-1.5
    0x0007e905, 0xfff9ee97, 0xfff88e11, 0x00061169, 0x000f88eb, //-1.0
    0x0007dbc8, 0xfff9e2b7, 0xfff86031, 0x00061d49, 0x000fc407, //-0.5
    0x0007cd0e, 0xfff9d701, 0xfff832f2, 0x000628ff, 0x00100000, //0.0
    0x0007bccf, 0xfff9cb75, 0xfff80654, 0x0006348b, 0x00103cdd, //0.5
    0x0007ab01, 0xfff9c013, 0xfff7da58, 0x00063fed, 0x00107aa6, //1.0
    0x0007979b, 0xfff9b4db, 0xfff7af01, 0x00064b25, 0x0010b964, //1.5
    0x00078292, 0xfff9a9ce, 0xfff7844e, 0x00065632, 0x0010f920, //2.0
    0x00076bdd, 0xfff99eec, 0xfff75a41, 0x00066114, 0x001139e3, //2.5
    0x00075371, 0xfff99435, 0xfff730d9, 0x00066bcb, 0x00117bb6, //3.0
    0x00073944, 0xfff989a8, 0xfff70819, 0x00067658, 0x0011bea3, //3.5
    0x00071d4c, 0xfff97f47, 0xfff6dfff, 0x000680b9, 0x001202b5, //4.0
    0x0006ff7e, 0xfff97512, 0xfff6b88d, 0x00068aee, 0x001247f5, //4.5
    0x0006dfce, 0xfff96b07, 0xfff691c2, 0x000694f9, 0x00128e70, //5.0
    0x0006be32, 0xfff96128, 0xfff66b9d, 0x00069ed8, 0x0012d630, //5.5
    0x00069a9e, 0xfff95774, 0xfff64620, 0x0006a88c, 0x00131f42, //6.0
    0x00067506, 0xfff94deb, 0xfff62149, 0x0006b215, 0x001369b1, //6.5
    0x00064d5e, 0xfff9448d, 0xfff5fd18, 0x0006bb73, 0x0013b58a, //7.0
    0x0006239a, 0xfff93b5a, 0xfff5d98c, 0x0006c4a6, 0x001402d9, //7.5
    0x0005f7ad, 0xfff93252, 0xfff5b6a5, 0x0006cdae, 0x001451ad, //8.0
    0x0005c98a, 0xfff92973, 0xfff59462, 0x0006d68d, 0x0014a213, //8.5
    0x00059924, 0xfff920bf, 0xfff572c2, 0x0006df41, 0x0014f41a, //9.0
    0x0005666d, 0xfff91835, 0xfff551c3, 0x0006e7cb, 0x001547cf, //9.5
    0x00053157, 0xfff90fd5, 0xfff53166, 0x0006f02b, 0x00159d43, //10.0
    0x0004f9d4, 0xfff9079e, 0xfff511a7, 0x0006f862, 0x0015f485, //10.5
    0x0004bfd4, 0xfff8ff8f, 0xfff4f287, 0x00070071, 0x00164da4, //11.0
    0x00048349, 0xfff8f7aa, 0xfff4d404, 0x00070856, 0x0016a8b3, //11.5
    0x00044423, 0xfff8efec, 0xfff4b61d, 0x00071014, 0x001705c1, //12.0
    0x00040252, 0xfff8e857, 0xfff498cf, 0x000717a9, 0x001764e0, //12.5
    0x0003bdc5, 0xfff8e0e8, 0xfff47c19, 0x00071f18, 0x0017c622, //13.0
    0x0003766b, 0xfff8d9a1, 0xfff45ffa, 0x0007265f, 0x0018299b, //13.5
    0x00032c33, 0xfff8d281, 0xfff44470, 0x00072d7f, 0x00188f5d, //14.0
};
#else // 100Hz, 300Hz, 1KHz, 3KHz, 10KHz
static const UINT32 _aau4MtkAudChlEqCoefTbl[(57+57*2+57*2+57*2+57)*5] =
{
    // 48000.0 Hz
    // Band 1, -14.0 dB ~ +14.0 dB (57 sets)
    0x003e4973, 0xff826497, 0xffc258e3, 0x007d939a, 0x003f55db, //-14.0
    0x003e4bd9, 0xff825bf3, 0xffc25076, 0x007d9c94, 0x003f5c37, //-13.5
    0x003e4e27, 0xff82536e, 0xffc24827, 0x007da56b, 0x003f628c, //-13.0
    0x003e505f, 0xff824b0a, 0xffc23ff5, 0x007dae22, 0x003f68d8, //-12.5
    0x003e5280, 0xff8242c4, 0xffc237e0, 0x007db6b8, 0x003f6f1c, //-12.0
    0x003e548a, 0xff823a9d, 0xffc22fe7, 0x007dbf2e, 0x003f755a, //-11.5
    0x003e567e, 0xff823295, 0xffc2280b, 0x007dc784, 0x003f7b90, //-11.0
    0x003e585b, 0xff822aab, 0xffc2204a, 0x007dcfbb, 0x003f81c0, //-10.5
    0x003e5a23, 0xff8222de, 0xffc218a5, 0x007dd7d3, 0x003f87ea, //-10.0
    0x003e5bd4, 0xff821b2e, 0xffc2111b, 0x007ddfce, 0x003f8e0d, //-9.5
    0x003e5d6f, 0xff82139b, 0xffc209ab, 0x007de7aa, 0x003f942b, //-9.0
    0x003e5ef4, 0xff820c24, 0xffc20256, 0x007def69, 0x003f9a44, //-8.5
    0x003e6063, 0xff8204ca, 0xffc1fb1a, 0x007df70b, 0x003fa058, //-8.0
    0x003e61bc, 0xff81fd8b, 0xffc1f3f8, 0x007dfe91, 0x003fa668, //-7.5
    0x003e62ff, 0xff81f667, 0xffc1ecf0, 0x007e05fa, 0x003fac73, //-7.0
    0x003e642d, 0xff81ef5f, 0xffc1e600, 0x007e0d48, 0x003fb27a, //-6.5
    0x003e6545, 0xff81e870, 0xffc1df29, 0x007e147b, 0x003fb87d, //-6.0
    0x003e6647, 0xff81e19c, 0xffc1d86b, 0x007e1b93, 0x003fbe7d, //-5.5
    0x003e6734, 0xff81dae2, 0xffc1d1c4, 0x007e2290, 0x003fc47a, //-5.0
    0x003e680c, 0xff81d442, 0xffc1cb35, 0x007e2973, 0x003fca75, //-4.5
    0x003e68ce, 0xff81cdba, 0xffc1c4bd, 0x007e303d, 0x003fd06d, //-4.0
    0x003e697b, 0xff81c74c, 0xffc1be5c, 0x007e36ed, 0x003fd663, //-3.5
    0x003e6a12, 0xff81c0f6, 0xffc1b812, 0x007e3d85, 0x003fdc57, //-3.0
    0x003e6a94, 0xff81bab8, 0xffc1b1de, 0x007e4404, 0x003fe24a, //-2.5
    0x003e6b01, 0xff81b493, 0xffc1abc0, 0x007e4a6b, 0x003fe83c, //-2.0
    0x003e6b59, 0xff81ae85, 0xffc1a5b8, 0x007e50b9, 0x003fee2e, //-1.5
    0x003e6b9b, 0xff81a88e, 0xffc19fc5, 0x007e56f1, 0x003ff41e, //-1.0
    0x003e6bc8, 0xff81a2ae, 0xffc199e8, 0x007e5d11, 0x003ffa0f, //-0.5
    0x003e6be0, 0xff819ce5, 0xffc19420, 0x007e631b, 0x00400000, //0.0
    0x003e6be3, 0xff819733, 0xffc18e6c, 0x007e690e, 0x004005f1, //0.5
    0x003e6bd1, 0xff819196, 0xffc188cc, 0x007e6eeb, 0x00400be4, //1.0
    0x003e6baa, 0xff818c10, 0xffc18341, 0x007e74b2, 0x004011d7, //1.5
    0x003e6b6d, 0xff81869f, 0xffc17dc9, 0x007e7a64, 0x004017cc, //2.0
    0x003e6b1c, 0xff818144, 0xffc17865, 0x007e8000, 0x00401dc3, //2.5
    0x003e6ab5, 0xff817bfe, 0xffc17314, 0x007e8588, 0x004023bc, //3.0
    0x003e6a39, 0xff8176cc, 0xffc16dd6, 0x007e8afb, 0x004029b8, //3.5
    0x003e69a8, 0xff8171b0, 0xffc168ab, 0x007e905a, 0x00402fb7, //4.0
    0x003e6902, 0xff816ca7, 0xffc16393, 0x007e95a5, 0x004035b8, //4.5
    0x003e6847, 0xff8167b3, 0xffc15e8c, 0x007e9add, 0x00403bbd, //5.0
    0x003e6776, 0xff8162d3, 0xffc15998, 0x007ea001, 0x004041c6, //5.5
    0x003e6690, 0xff815e07, 0xffc154b5, 0x007ea512, 0x004047d3, //6.0
    0x003e6595, 0xff81594e, 0xffc14fe4, 0x007eaa10, 0x00404de5, //6.5
    0x003e6484, 0xff8154a8, 0xffc14b24, 0x007eaefc, 0x004053fb, //7.0
    0x003e635e, 0xff815015, 0xffc14675, 0x007eb3d5, 0x00405a17, //7.5
    0x003e6223, 0xff814b95, 0xffc141d7, 0x007eb89c, 0x00406038, //8.0
    0x003e60d2, 0xff814728, 0xffc13d49, 0x007ebd52, 0x0040665e, //8.5
    0x003e5f6c, 0xff8142cd, 0xffc138cc, 0x007ec1f6, 0x00406c8b, //9.0
    0x003e5df0, 0xff813e84, 0xffc1345f, 0x007ec689, 0x004072bf, //9.5
    0x003e5c5e, 0xff813a4d, 0xffc13001, 0x007ecb0b, 0x004078f9, //10.0
    0x003e5ab7, 0xff813629, 0xffc12bb4, 0x007ecf7d, 0x00407f3b, //10.5
    0x003e58fa, 0xff813216, 0xffc12776, 0x007ed3de, 0x00408584, //11.0
    0x003e5727, 0xff812e14, 0xffc12347, 0x007ed82f, 0x00408bd5, //11.5
    0x003e553e, 0xff812a24, 0xffc11f27, 0x007edc6f, 0x0040922f, //12.0
    0x003e533f, 0xff812645, 0xffc11b16, 0x007ee0a0, 0x00409891, //12.5
    0x003e512a, 0xff812276, 0xffc11713, 0x007ee4c2, 0x00409efc, //13.0
    0x003e4efe, 0xff811eb9, 0xffc1131f, 0x007ee8d4, 0x0040a570, //13.5
    0x003e4cbd, 0xff811b0c, 0xffc10f39, 0x007eecd7, 0x0040abee, //14.0
    // Band 2, -14.0 dB ~ +14.0 dB (57*2 sets)
    0x0006f502, 0xfff1d461, 0xfff043ce, 0x001fbb35, 0x000738d1, //-14.0
    0x00072947, 0xfff16ae0, 0xfff044c8, 0x001fba34, 0x00076e0e, //-13.5
    0x00075f11, 0xfff0fe4e, 0xfff045c5, 0x001fb930, 0x0007a4d5, //-13.0
    0x0007966c, 0xfff08e97, 0xfff046c5, 0x001fb827, 0x0007dd31, //-12.5
    0x0007cf64, 0xfff01ba2, 0xfff047ca, 0x001fb71b, 0x0008172e, //-12.0
    0x00080a05, 0xffefa558, 0xfff048d2, 0x001fb60b, 0x000852d7, //-11.5
    0x0008465c, 0xffef2b9f, 0xfff049dd, 0x001fb4f7, 0x00089039, //-11.0
    0x00088474, 0xffeeae5f, 0xfff04aed, 0x001fb3de, 0x0008cf61, //-10.5
    0x0008c45b, 0xffee2d7d, 0xfff04c00, 0x001fb2c2, 0x0009105b, //-10.0
    0x0009061f, 0xffeda8df, 0xfff04d18, 0x001fb1a2, 0x00095336, //-9.5
    0x000949cc, 0xffed2069, 0xfff04e33, 0x001fb07d, 0x000997ff, //-9.0
    0x00098f72, 0xffec93fe, 0xfff04f52, 0x001faf54, 0x0009dec4, //-8.5
    0x0009d71e, 0xffec0381, 0xfff05076, 0x001fae26, 0x000a2794, //-8.0
    0x000a20e1, 0xffeb6ed5, 0xfff0519d, 0x001facf4, 0x000a727e, //-7.5
    0x000a6cc9, 0xffead5d9, 0xfff052c9, 0x001fabbe, 0x000abf92, //-7.0
    0x000abae6, 0xffea386f, 0xfff053f9, 0x001faa83, 0x000b0ede, //-6.5
    0x000b0b48, 0xffe99676, 0xfff0552d, 0x001fa944, 0x000b6075, //-6.0
    0x000b5e01, 0xffe8efcc, 0xfff05666, 0x001fa7ff, 0x000bb467, //-5.5
    0x000bb321, 0xffe8444e, 0xfff057a3, 0x001fa6b6, 0x000c0ac4, //-5.0
    0x000c0abb, 0xffe793d9, 0xfff058e5, 0x001fa569, 0x000c63a0, //-4.5
    0x000c64e0, 0xffe6de48, 0xfff05a2b, 0x001fa416, 0x000cbf0b, //-4.0
    0x000cc1a4, 0xffe62375, 0xfff05b76, 0x001fa2be, 0x000d1d1a, //-3.5
    0x000d211a, 0xffe5633a, 0xfff05cc5, 0x001fa161, 0x000d7ddf, //-3.0
    0x000d8356, 0xffe49d6e, 0xfff05e19, 0x001f9fff, 0x000de16f, //-2.5
    0x000de86c, 0xffe3d1e9, 0xfff05f72, 0x001f9e98, 0x000e47de, //-2.0
    0x000e5071, 0xffe30081, 0xfff060d0, 0x001f9d2c, 0x000eb141, //-1.5
    0x000ebb7b, 0xffe22909, 0xfff06233, 0x001f9bba, 0x000f1dae, //-1.0
    0x000f29a0, 0xffe14b57, 0xfff0639b, 0x001f9a43, 0x000f8d3b, //-0.5
    0x000f9af8, 0xffe0673a, 0xfff06508, 0x001f98c6, 0x00100000, //0.0
    0x00100f9a, 0xffdf7c85, 0xfff0667a, 0x001f9744, 0x00107614, //0.5
    0x0010879d, 0xffde8b06, 0xfff067f1, 0x001f95bb, 0x0010ef8f, //1.0
    0x0011031c, 0xffdd928c, 0xfff0696e, 0x001f942e, 0x00116c8a, //1.5
    0x0011822f, 0xffdc92e3, 0xfff06af0, 0x001f929a, 0x0011ed1f, //2.0
    0x001204f2, 0xffdb8bd7, 0xfff06c77, 0x001f9100, 0x00127169, //2.5
    0x00128b7e, 0xffda7d31, 0xfff06e04, 0x001f8f60, 0x0012f983, //3.0
    0x001315f1, 0xffd966b8, 0xfff06f97, 0x001f8dbb, 0x00138588, //3.5
    0x0013a467, 0xffd84835, 0xfff0712f, 0x001f8c0e, 0x00141596, //4.0
    0x001436fd, 0xffd7216a, 0xfff072cd, 0x001f8a5c, 0x0014a9ca, //4.5
    0x0014cdd2, 0xffd5f21d, 0xfff07471, 0x001f88a3, 0x00154243, //5.0
    0x00156905, 0xffd4ba0d, 0xfff0761b, 0x001f86e4, 0x0015df1f, //5.5
    0x001608b6, 0xffd378fb, 0xfff077ca, 0x001f851e, 0x00168080, //6.0
    0x0016ad06, 0xffd22ea4, 0xfff07980, 0x001f8351, 0x00172686, //6.5
    0x00175618, 0xffd0dac5, 0xfff07b3c, 0x001f817e, 0x0017d154, //7.0
    0x0018040e, 0xffcf7d17, 0xfff07cfe, 0x001f7fa3, 0x0018810c, //7.5
    0x0018b70c, 0xffce1552, 0xfff07ec7, 0x001f7dc2, 0x001935d2, //8.0
    0x00196f37, 0xffcca32d, 0xfff08095, 0x001f7bd9, 0x0019efcc, //8.5
    0x001a2cb5, 0xffcb265b, 0xfff0826b, 0x001f79ea, 0x001aaf20, //9.0
    0x001aefae, 0xffc99e8e, 0xfff08447, 0x001f77f3, 0x001b73f4, //9.5
    0x001bb849, 0xffc80b75, 0xfff08629, 0x001f75f4, 0x001c3e72, //10.0
    0x001c86b0, 0xffc66cbe, 0xfff08813, 0x001f73ee, 0x001d0ec2, //10.5
    0x001d5b0d, 0xffc4c213, 0xfff08a03, 0x001f71e0, 0x001de50f, //11.0
    0x001e358c, 0xffc30b1e, 0xfff08bfa, 0x001f6fcb, 0x001ec186, //11.5
    0x001f165a, 0xffc14783, 0xfff08df8, 0x001f6dad, 0x001fa452, //12.0
    0x001ffda6, 0xffbf76e6, 0xfff08ffd, 0x001f6b88, 0x00208da3, //12.5
    0x0020eb9e, 0xffbd98e9, 0xfff0920a, 0x001f695a, 0x00217da8, //13.0
    0x0021e074, 0xffbbad28, 0xfff0941e, 0x001f6724, 0x00227492, //13.5
    0x0022dc5b, 0xffb9b341, 0xfff09639, 0x001f64e6, 0x00237293, //14.0
    0x0020da85, 0xffbc97f5, 0xfff1c7ed, 0x001e0940, 0x0022a272, //-14.0
    0x001fefff, 0xffbe7323, 0xfff1c1ce, 0x001e10a9, 0x0021b1cd, //-13.5
    0x001f0bdf, 0xffc04174, 0xfff1bbc3, 0x001e17f5, 0x0020c7a2, //-13.0
    0x001e2df8, 0xffc2033e, 0xfff1b5cb, 0x001e1f25, 0x001fe3c3, //-12.5
    0x001d5621, 0xffc3b8d6, 0xfff1afe6, 0x001e2639, 0x001f0607, //-12.0
    0x001c8430, 0xffc5628e, 0xfff1aa15, 0x001e2d32, 0x001e2e44, //-11.5
    0x001bb7fd, 0xffc700b7, 0xfff1a456, 0x001e3411, 0x001d5c52, //-11.0
    0x001af161, 0xffc893a0, 0xfff19ea9, 0x001e3ad5, 0x001c900a, //-10.5
    0x001a3036, 0xffca1b94, 0xfff1990f, 0x001e417f, 0x001bc945, //-10.0
    0x00197457, 0xffcb98de, 0xfff19387, 0x001e4810, 0x001b07de, //-9.5
    0x0018bd9f, 0xffcd0bc8, 0xfff18e12, 0x001e4e87, 0x001a4bb1, //-9.0
    0x00180bec, 0xffce7496, 0xfff188ae, 0x001e54e5, 0x00199499, //-8.5
    0x00175f1b, 0xffcfd38f, 0xfff1835b, 0x001e5b2c, 0x0018e276, //-8.0
    0x0016b70a, 0xffd128f5, 0xfff17e1a, 0x001e615a, 0x00183524, //-7.5
    0x00161399, 0xffd2750b, 0xfff178ea, 0x001e6770, 0x00178c83, //-7.0
    0x001574a9, 0xffd3b80f, 0xfff173cb, 0x001e6d6f, 0x0016e874, //-6.5
    0x0014da19, 0xffd4f23f, 0xfff16ebd, 0x001e7357, 0x001648d6, //-6.0
    0x001443cd, 0xffd623d9, 0xfff169c0, 0x001e7929, 0x0015ad8d, //-5.5
    0x0013b1a6, 0xffd74d17, 0xfff164d3, 0x001e7ee4, 0x00151679, //-5.0
    0x00132389, 0xffd86e32, 0xfff15ff6, 0x001e848a, 0x0014837f, //-4.5
    0x00129959, 0xffd98763, 0xfff15b29, 0x001e8a19, 0x0013f482, //-4.0
    0x001212fb, 0xffda98df, 0xfff1566d, 0x001e8f94, 0x00136967, //-3.5
    0x00119054, 0xffdba2dc, 0xfff151c0, 0x001e94f9, 0x0012e214, //-3.0
    0x0011114c, 0xffdca58e, 0xfff14d22, 0x001e9a4a, 0x00125e6e, //-2.5
    0x001095c9, 0xffdda126, 0xfff14894, 0x001e9f87, 0x0011de5d, //-2.0
    0x00101db2, 0xffde95d6, 0xfff14415, 0x001ea4b0, 0x001161c7, //-1.5
    0x000fa8f0, 0xffdf83cd, 0xfff13fa6, 0x001ea9c5, 0x0010e895, //-1.0
    0x000f376b, 0xffe06b3a, 0xfff13b45, 0x001eaec7, 0x001072b0, //-0.5
    0x000ec90e, 0xffe14c4b, 0xfff136f2, 0x001eb3b5, 0x00100000, //0.0
    0x000e5dc2, 0xffe2272a, 0xfff132ae, 0x001eb891, 0x000f9070, //0.5
    0x000df571, 0xffe2fc03, 0xfff12e79, 0x001ebd5a, 0x000f23ea, //1.0
    0x000d9008, 0xffe3cb01, 0xfff12a51, 0x001ec212, 0x000eba59, //1.5
    0x000d2d72, 0xffe4944a, 0xfff12638, 0x001ec6b7, 0x000e53a9, //2.0
    0x000ccd9a, 0xffe55807, 0xfff1222c, 0x001ecb4a, 0x000defc6, //2.5
    0x000c706f, 0xffe6165f, 0xfff11e2e, 0x001ecfcc, 0x000d8e9d, //3.0
    0x000c15dc, 0xffe6cf77, 0xfff11a3e, 0x001ed43d, 0x000d301a, //3.5
    0x000bbdd1, 0xffe78374, 0xfff1165b, 0x001ed89d, 0x000cd42c, //4.0
    0x000b683b, 0xffe83278, 0xfff11285, 0x001edced, 0x000c7ac0, //4.5
    0x000b1509, 0xffe8dca8, 0xfff10ebc, 0x001ee12c, 0x000c23c5, //5.0
    0x000ac42b, 0xffe98223, 0xfff10b00, 0x001ee55b, 0x000bcf2b, //5.5
    0x000a7590, 0xffea230c, 0xfff10750, 0x001ee97a, 0x000b7ce0, //6.0
    0x000a2928, 0xffeabf82, 0xfff103ad, 0x001eed89, 0x000b2cd5, //6.5
    0x0009dee3, 0xffeb57a3, 0xfff10017, 0x001ef189, 0x000adefa, //7.0
    0x000996b4, 0xffebeb90, 0xfff0fc8d, 0x001ef57a, 0x000a9340, //7.5
    0x0009508a, 0xffec7b64, 0xfff0f90f, 0x001ef95c, 0x000a4999, //8.0
    0x00090c58, 0xffed073c, 0xfff0f59c, 0x001efd30, 0x000a01f4, //8.5
    0x0008ca10, 0xffed8f35, 0xfff0f236, 0x001f00f4, 0x0009bc46, //9.0
    0x000889a5, 0xffee1369, 0xfff0eedb, 0x001f04ab, 0x00097880, //9.5
    0x00084b08, 0xffee93f3, 0xfff0eb8c, 0x001f0853, 0x00093694, //10.0
    0x00080e2f, 0xffef10ed, 0xfff0e848, 0x001f0bee, 0x0008f677, //10.5
    0x0007d30b, 0xffef8a6f, 0xfff0e50f, 0x001f0f7b, 0x0008b81b, //11.0
    0x00079992, 0xfff00092, 0xfff0e1e2, 0x001f12fb, 0x00087b74, //11.5
    0x000761b7, 0xfff0736d, 0xfff0debf, 0x001f166d, 0x00084076, //12.0
    0x00072b6e, 0xfff0e318, 0xfff0dba7, 0x001f19d2, 0x00080715, //12.5
    0x0006f6ad, 0xfff14fa9, 0xfff0d89a, 0x001f1d2b, 0x0007cf48, //13.0
    0x0006c369, 0xfff1b936, 0xfff0d597, 0x001f2077, 0x00079901, //13.5
    0x00069198, 0xfff21fd3, 0xfff0d29f, 0x001f23b6, 0x00076437, //14.0
    // Band 3, -14.0 dB ~ +14.0 dB (57*2 sets)
    0x00069198, 0xfff21fd3, 0xfff0d29f, 0x001f23b6, 0x00076437, //-14.0
    0x0006c369, 0xfff1b936, 0xfff0d597, 0x001f2077, 0x00079901, //-13.5
    0x0006f6ad, 0xfff14fa9, 0xfff0d89a, 0x001f1d2b, 0x0007cf48, //-13.0
    0x00072b6e, 0xfff0e318, 0xfff0dba7, 0x001f19d2, 0x00080715, //-12.5
    0x000761b7, 0xfff0736d, 0xfff0debf, 0x001f166d, 0x00084076, //-12.0
    0x00079992, 0xfff00092, 0xfff0e1e2, 0x001f12fb, 0x00087b74, //-11.5
    0x0007d30b, 0xffef8a6f, 0xfff0e50f, 0x001f0f7b, 0x0008b81b, //-11.0
    0x00080e2f, 0xffef10ed, 0xfff0e848, 0x001f0bee, 0x0008f677, //-10.5
    0x00084b08, 0xffee93f3, 0xfff0eb8c, 0x001f0853, 0x00093694, //-10.0
    0x000889a5, 0xffee1369, 0xfff0eedb, 0x001f04ab, 0x00097880, //-9.5
    0x0008ca10, 0xffed8f35, 0xfff0f236, 0x001f00f4, 0x0009bc46, //-9.0
    0x00090c58, 0xffed073c, 0xfff0f59c, 0x001efd30, 0x000a01f4, //-8.5
    0x0009508a, 0xffec7b64, 0xfff0f90f, 0x001ef95c, 0x000a4999, //-8.0
    0x000996b4, 0xffebeb90, 0xfff0fc8d, 0x001ef57a, 0x000a9340, //-7.5
    0x0009dee3, 0xffeb57a3, 0xfff10017, 0x001ef189, 0x000adefa, //-7.0
    0x000a2928, 0xffeabf82, 0xfff103ad, 0x001eed89, 0x000b2cd5, //-6.5
    0x000a7590, 0xffea230c, 0xfff10750, 0x001ee97a, 0x000b7ce0, //-6.0
    0x000ac42b, 0xffe98223, 0xfff10b00, 0x001ee55b, 0x000bcf2b, //-5.5
    0x000b1509, 0xffe8dca8, 0xfff10ebc, 0x001ee12c, 0x000c23c5, //-5.0
    0x000b683b, 0xffe83278, 0xfff11285, 0x001edced, 0x000c7ac0, //-4.5
    0x000bbdd1, 0xffe78374, 0xfff1165b, 0x001ed89d, 0x000cd42c, //-4.0
    0x000c15dc, 0xffe6cf77, 0xfff11a3e, 0x001ed43d, 0x000d301a, //-3.5
    0x000c706f, 0xffe6165f, 0xfff11e2e, 0x001ecfcc, 0x000d8e9d, //-3.0
    0x000ccd9a, 0xffe55807, 0xfff1222c, 0x001ecb4a, 0x000defc6, //-2.5
    0x000d2d72, 0xffe4944a, 0xfff12638, 0x001ec6b7, 0x000e53a9, //-2.0
    0x000d9008, 0xffe3cb01, 0xfff12a51, 0x001ec212, 0x000eba59, //-1.5
    0x000df571, 0xffe2fc03, 0xfff12e79, 0x001ebd5a, 0x000f23ea, //-1.0
    0x000e5dc2, 0xffe2272a, 0xfff132ae, 0x001eb891, 0x000f9070, //-0.5
    0x000ec90e, 0xffe14c4b, 0xfff136f2, 0x001eb3b5, 0x00100000, //0.0
    0x000f376b, 0xffe06b3a, 0xfff13b45, 0x001eaec7, 0x001072b0, //0.5
    0x000fa8f0, 0xffdf83cd, 0xfff13fa6, 0x001ea9c5, 0x0010e895, //1.0
    0x00101db2, 0xffde95d6, 0xfff14415, 0x001ea4b0, 0x001161c7, //1.5
    0x001095c9, 0xffdda126, 0xfff14894, 0x001e9f87, 0x0011de5d, //2.0
    0x0011114c, 0xffdca58e, 0xfff14d22, 0x001e9a4a, 0x00125e6e, //2.5
    0x00119054, 0xffdba2dc, 0xfff151c0, 0x001e94f9, 0x0012e214, //3.0
    0x001212fb, 0xffda98df, 0xfff1566d, 0x001e8f94, 0x00136967, //3.5
    0x00129959, 0xffd98763, 0xfff15b29, 0x001e8a19, 0x0013f482, //4.0
    0x00132389, 0xffd86e32, 0xfff15ff6, 0x001e848a, 0x0014837f, //4.5
    0x0013b1a6, 0xffd74d17, 0xfff164d3, 0x001e7ee4, 0x00151679, //5.0
    0x001443cd, 0xffd623d9, 0xfff169c0, 0x001e7929, 0x0015ad8d, //5.5
    0x0014da19, 0xffd4f23f, 0xfff16ebd, 0x001e7357, 0x001648d6, //6.0
    0x001574a9, 0xffd3b80f, 0xfff173cb, 0x001e6d6f, 0x0016e874, //6.5
    0x00161399, 0xffd2750b, 0xfff178ea, 0x001e6770, 0x00178c83, //7.0
    0x0016b70a, 0xffd128f5, 0xfff17e1a, 0x001e615a, 0x00183524, //7.5
    0x00175f1b, 0xffcfd38f, 0xfff1835b, 0x001e5b2c, 0x0018e276, //8.0
    0x00180bec, 0xffce7496, 0xfff188ae, 0x001e54e5, 0x00199499, //8.5
    0x0018bd9f, 0xffcd0bc8, 0xfff18e12, 0x001e4e87, 0x001a4bb1, //9.0
    0x00197457, 0xffcb98de, 0xfff19387, 0x001e4810, 0x001b07de, //9.5
    0x001a3036, 0xffca1b94, 0xfff1990f, 0x001e417f, 0x001bc945, //10.0
    0x001af161, 0xffc893a0, 0xfff19ea9, 0x001e3ad5, 0x001c900a, //10.5
    0x001bb7fd, 0xffc700b7, 0xfff1a456, 0x001e3411, 0x001d5c52, //11.0
    0x001c8430, 0xffc5628e, 0xfff1aa15, 0x001e2d32, 0x001e2e44, //11.5
    0x001d5621, 0xffc3b8d6, 0xfff1afe6, 0x001e2639, 0x001f0607, //12.0
    0x001e2df8, 0xffc2033e, 0xfff1b5cb, 0x001e1f25, 0x001fe3c3, //12.5
    0x001f0bdf, 0xffc04174, 0xfff1bbc3, 0x001e17f5, 0x0020c7a2, //13.0
    0x001fefff, 0xffbe7323, 0xfff1c1ce, 0x001e10a9, 0x0021b1cd, //13.5
    0x0020da85, 0xffbc97f5, 0xfff1c7ed, 0x001e0940, 0x0022a272, //14.0
    0x001b1d9f, 0xffc58e7c, 0xfff4ee1b, 0x00197659, 0x00200bba, //-14.0
    0x001a64c8, 0xffc70f1b, 0xfff4dfb5, 0x00198f40, 0x001f447d, //-13.5
    0x0019b07a, 0xffc88687, 0xfff4d16c, 0x0019a7cb, 0x001e81e6, //-13.0
    0x0019009d, 0xffc9f4f4, 0xfff4c33f, 0x0019bffb, 0x001dc3dc, //-12.5
    0x00185519, 0xffcb5a90, 0xfff4b52f, 0x0019d7d0, 0x001d0a48, //-12.0
    0x0017add6, 0xffccb78e, 0xfff4a73c, 0x0019ef4c, 0x001c5511, //-11.5
    0x00170abb, 0xffce0c1b, 0xfff49965, 0x001a0670, 0x001ba420, //-11.0
    0x00166bb2, 0xffcf5867, 0xfff48bac, 0x001a1d3d, 0x001af75e, //-10.5
    0x0015d0a2, 0xffd09c9f, 0xfff47e10, 0x001a33b5, 0x001a4eb2, //-10.0
    0x00153977, 0xffd1d8f1, 0xfff47091, 0x001a49d7, 0x0019aa09, //-9.5
    0x0014a619, 0xffd30d89, 0xfff46330, 0x001a5fa7, 0x0019094a, //-9.0
    0x00141673, 0xffd43a93, 0xfff455ed, 0x001a7523, 0x00186c60, //-8.5
    0x00138a70, 0xffd56038, 0xfff448c7, 0x001a8a4f, 0x0017d336, //-8.0
    0x001301f9, 0xffd67ea4, 0xfff43bbe, 0x001a9f2a, 0x00173db7, //-7.5
    0x00127cfc, 0xffd795ff, 0xfff42ed3, 0x001ab3b7, 0x0016abcf, //-7.0
    0x0011fb62, 0xffd8a672, 0xfff42206, 0x001ac7f5, 0x00161d69, //-6.5
    0x00117d1a, 0xffd9b025, 0xfff41557, 0x001adbe6, 0x00159271, //-6.0
    0x0011020e, 0xffdab33f, 0xfff408c5, 0x001aef8c, 0x00150ad3, //-5.5
    0x00108a2c, 0xffdbafe6, 0xfff3fc51, 0x001b02e6, 0x0014867d, //-5.0
    0x00101561, 0xffdca641, 0xfff3effb, 0x001b15f7, 0x0014055c, //-4.5
    0x000fa39a, 0xffdd9673, 0xfff3e3c3, 0x001b28bf, 0x0013875d, //-4.0
    0x000f34c6, 0xffde80a1, 0xfff3d7a8, 0x001b3b3f, 0x00130c6e, //-3.5
    0x000ec8d3, 0xffdf64ee, 0xfff3cbab, 0x001b4d79, 0x0012947e, //-3.0
    0x000e5faf, 0xffe0437e, 0xfff3bfcc, 0x001b5f6d, 0x00121f7a, //-2.5
    0x000df949, 0xffe11c73, 0xfff3b40a, 0x001b711c, 0x0011ad53, //-2.0
    0x000d9591, 0xffe1efed, 0xfff3a865, 0x001b8287, 0x00113df6, //-1.5
    0x000d3476, 0xffe2be0e, 0xfff39cde, 0x001b93af, 0x0010d154, //-1.0
    0x000cd5e8, 0xffe386f6, 0xfff39174, 0x001ba496, 0x0010675d, //-0.5
    0x000c79d8, 0xffe44ac5, 0xfff38628, 0x001bb53b, 0x00100000, //0.0
    0x000c2036, 0xffe50999, 0xfff37af9, 0x001bc5a1, 0x000f9b2f, //0.5
    0x000bc8f3, 0xffe5c390, 0xfff36fe6, 0x001bd5c7, 0x000f38d9, //1.0
    0x000b7400, 0xffe678c9, 0xfff364f1, 0x001be5af, 0x000ed8f2, //1.5
    0x000b2150, 0xffe7295f, 0xfff35a19, 0x001bf55b, 0x000e7b68, //2.0
    0x000ad0d3, 0xffe7d571, 0xfff34f5d, 0x001c04c9, 0x000e2030, //2.5
    0x000a827c, 0xffe87d18, 0xfff344be, 0x001c13fc, 0x000dc73a, //3.0
    0x000a363d, 0xffe92071, 0xfff33a3b, 0x001c22f5, 0x000d7078, //3.5
    0x0009ec0a, 0xffe9bf96, 0xfff32fd5, 0x001c31b4, 0x000d1bdf, //4.0
    0x0009a3d5, 0xffea5aa1, 0xfff3258b, 0x001c403a, 0x000cc960, //4.5
    0x00095d91, 0xffeaf1ac, 0xfff31b5d, 0x001c4e88, 0x000c78ee, //5.0
    0x00091933, 0xffeb84cf, 0xfff3114b, 0x001c5c9f, 0x000c2a7e, //5.5
    0x0008d6ae, 0xffec1423, 0xfff30754, 0x001c6a7f, 0x000bde02, //6.0
    0x000895f6, 0xffec9fc0, 0xfff2fd7a, 0x001c782a, 0x000b936f, //6.5
    0x000856ff, 0xffed27be, 0xfff2f3ba, 0x001c85a0, 0x000b4aba, //7.0
    0x000819bf, 0xffedac32, 0xfff2ea16, 0x001c92e2, 0x000b03d6, //7.5
    0x0007de2a, 0xffee2d33, 0xfff2e08e, 0x001c9ff0, 0x000abeb8, //8.0
    0x0007a436, 0xffeeaad8, 0xfff2d720, 0x001caccc, 0x000a7b56, //8.5
    0x00076bd8, 0xffef2535, 0xfff2cdcd, 0x001cb977, 0x000a39a4, //9.0
    0x00073505, 0xffef9c5f, 0xfff2c494, 0x001cc5f0, 0x0009f999, //9.5
    0x0006ffb3, 0xfff0106b, 0xfff2bb76, 0x001cd23a, 0x0009bb29, //10.0
    0x0006cbd9, 0xfff0816c, 0xfff2b272, 0x001cde53, 0x00097e4c, //10.5
    0x0006996e, 0xfff0ef77, 0xfff2a988, 0x001cea3e, 0x000942f6, //11.0
    0x00066866, 0xfff15a9d, 0xfff2a0b8, 0x001cf5fb, 0x0009091f, //11.5
    0x000638ba, 0xfff1c2f2, 0xfff29802, 0x001d018b, 0x0008d0bd, //12.0
    0x00060a61, 0xfff22888, 0xfff28f66, 0x001d0ced, 0x000899c6, //12.5
    0x0005dd51, 0xfff28b70, 0xfff286e2, 0x001d1824, 0x00086433, //13.0
    0x0005b182, 0xfff2ebbc, 0xfff27e78, 0x001d232f, 0x00082ffa, //13.5
    0x000586ec, 0xfff3497d, 0xfff27627, 0x001d2e10, 0x0007fd13, //14.0
    // Band 4, -14.0 dB ~ +14.0 dB (57*2 sets)
    0x000586ec, 0xfff3497d, 0xfff27627, 0x001d2e10, 0x0007fd13, //-14.0
    0x0005b182, 0xfff2ebbc, 0xfff27e78, 0x001d232f, 0x00082ffa, //-13.5
    0x0005dd51, 0xfff28b70, 0xfff286e2, 0x001d1824, 0x00086433, //-13.0
    0x00060a61, 0xfff22888, 0xfff28f66, 0x001d0ced, 0x000899c6, //-12.5
    0x000638ba, 0xfff1c2f2, 0xfff29802, 0x001d018b, 0x0008d0bd, //-12.0
    0x00066866, 0xfff15a9d, 0xfff2a0b8, 0x001cf5fb, 0x0009091f, //-11.5
    0x0006996e, 0xfff0ef77, 0xfff2a988, 0x001cea3e, 0x000942f6, //-11.0
    0x0006cbd9, 0xfff0816c, 0xfff2b272, 0x001cde53, 0x00097e4c, //-10.5
    0x0006ffb3, 0xfff0106b, 0xfff2bb76, 0x001cd23a, 0x0009bb29, //-10.0
    0x00073505, 0xffef9c5f, 0xfff2c494, 0x001cc5f0, 0x0009f999, //-9.5
    0x00076bd8, 0xffef2535, 0xfff2cdcd, 0x001cb977, 0x000a39a4, //-9.0
    0x0007a436, 0xffeeaad8, 0xfff2d720, 0x001caccc, 0x000a7b56, //-8.5
    0x0007de2a, 0xffee2d33, 0xfff2e08e, 0x001c9ff0, 0x000abeb8, //-8.0
    0x000819bf, 0xffedac32, 0xfff2ea16, 0x001c92e2, 0x000b03d6, //-7.5
    0x000856ff, 0xffed27be, 0xfff2f3ba, 0x001c85a0, 0x000b4aba, //-7.0
    0x000895f6, 0xffec9fc0, 0xfff2fd7a, 0x001c782a, 0x000b936f, //-6.5
    0x0008d6ae, 0xffec1423, 0xfff30754, 0x001c6a7f, 0x000bde02, //-6.0
    0x00091933, 0xffeb84cf, 0xfff3114b, 0x001c5c9f, 0x000c2a7e, //-5.5
    0x00095d91, 0xffeaf1ac, 0xfff31b5d, 0x001c4e88, 0x000c78ee, //-5.0
    0x0009a3d5, 0xffea5aa1, 0xfff3258b, 0x001c403a, 0x000cc960, //-4.5
    0x0009ec0a, 0xffe9bf96, 0xfff32fd5, 0x001c31b4, 0x000d1bdf, //-4.0
    0x000a363d, 0xffe92071, 0xfff33a3b, 0x001c22f5, 0x000d7078, //-3.5
    0x000a827c, 0xffe87d18, 0xfff344be, 0x001c13fc, 0x000dc73a, //-3.0
    0x000ad0d3, 0xffe7d571, 0xfff34f5d, 0x001c04c9, 0x000e2030, //-2.5
    0x000b2150, 0xffe7295f, 0xfff35a19, 0x001bf55b, 0x000e7b68, //-2.0
    0x000b7400, 0xffe678c9, 0xfff364f1, 0x001be5af, 0x000ed8f2, //-1.5
    0x000bc8f3, 0xffe5c390, 0xfff36fe6, 0x001bd5c7, 0x000f38d9, //-1.0
    0x000c2036, 0xffe50999, 0xfff37af9, 0x001bc5a1, 0x000f9b2f, //-0.5
    0x000c79d8, 0xffe44ac5, 0xfff38628, 0x001bb53b, 0x00100000, //0.0
    0x000cd5e8, 0xffe386f6, 0xfff39174, 0x001ba496, 0x0010675d, //0.5
    0x000d3476, 0xffe2be0e, 0xfff39cde, 0x001b93af, 0x0010d154, //1.0
    0x000d9591, 0xffe1efed, 0xfff3a865, 0x001b8287, 0x00113df6, //1.5
    0x000df949, 0xffe11c73, 0xfff3b40a, 0x001b711c, 0x0011ad53, //2.0
    0x000e5faf, 0xffe0437e, 0xfff3bfcc, 0x001b5f6d, 0x00121f7a, //2.5
    0x000ec8d3, 0xffdf64ee, 0xfff3cbab, 0x001b4d79, 0x0012947e, //3.0
    0x000f34c6, 0xffde80a1, 0xfff3d7a8, 0x001b3b3f, 0x00130c6e, //3.5
    0x000fa39a, 0xffdd9673, 0xfff3e3c3, 0x001b28bf, 0x0013875d, //4.0
    0x00101561, 0xffdca641, 0xfff3effb, 0x001b15f7, 0x0014055c, //4.5
    0x00108a2c, 0xffdbafe6, 0xfff3fc51, 0x001b02e6, 0x0014867d, //5.0
    0x0011020e, 0xffdab33f, 0xfff408c5, 0x001aef8c, 0x00150ad3, //5.5
    0x00117d1a, 0xffd9b025, 0xfff41557, 0x001adbe6, 0x00159271, //6.0
    0x0011fb62, 0xffd8a672, 0xfff42206, 0x001ac7f5, 0x00161d69, //6.5
    0x00127cfc, 0xffd795ff, 0xfff42ed3, 0x001ab3b7, 0x0016abcf, //7.0
    0x001301f9, 0xffd67ea4, 0xfff43bbe, 0x001a9f2a, 0x00173db7, //7.5
    0x00138a70, 0xffd56038, 0xfff448c7, 0x001a8a4f, 0x0017d336, //8.0
    0x00141673, 0xffd43a93, 0xfff455ed, 0x001a7523, 0x00186c60, //8.5
    0x0014a619, 0xffd30d89, 0xfff46330, 0x001a5fa7, 0x0019094a, //9.0
    0x00153977, 0xffd1d8f1, 0xfff47091, 0x001a49d7, 0x0019aa09, //9.5
    0x0015d0a2, 0xffd09c9f, 0xfff47e10, 0x001a33b5, 0x001a4eb2, //10.0
    0x00166bb2, 0xffcf5867, 0xfff48bac, 0x001a1d3d, 0x001af75e, //10.5
    0x00170abb, 0xffce0c1b, 0xfff49965, 0x001a0670, 0x001ba420, //11.0
    0x0017add6, 0xffccb78e, 0xfff4a73c, 0x0019ef4c, 0x001c5511, //11.5
    0x00185519, 0xffcb5a90, 0xfff4b52f, 0x0019d7d0, 0x001d0a48, //12.0
    0x0019009d, 0xffc9f4f4, 0xfff4c33f, 0x0019bffb, 0x001dc3dc, //12.5
    0x0019b07a, 0xffc88687, 0xfff4d16c, 0x0019a7cb, 0x001e81e6, //13.0
    0x001a64c8, 0xffc70f1b, 0xfff4dfb5, 0x00198f40, 0x001f447d, //13.5
    0x001b1d9f, 0xffc58e7c, 0xfff4ee1b, 0x00197659, 0x00200bba, //14.0
    0x000e7ea5, 0xffddbcbf, 0xfffa261f, 0x000aef9a, 0x0018a4c4, //-14.0
    0x000e2df0, 0xffde76de, 0xfffa1939, 0x000b3219, 0x00184729, //-13.5
    0x000dde8a, 0xffdf2e90, 0xfffa0c17, 0x000b741d, 0x0017eaa2, //-13.0
    0x000d9074, 0xffdfe3d5, 0xfff9febb, 0x000bb5a5, 0x00178f2f, //-12.5
    0x000d43ab, 0xffe096ad, 0xfff9f125, 0x000bf6ae, 0x001734d0, //-12.0
    0x000cf830, 0xffe14717, 0xfff9e358, 0x000c3737, 0x0016db88, //-11.5
    0x000cae01, 0xffe1f513, 0xfff9d555, 0x000c773e, 0x00168356, //-11.0
    0x000c651c, 0xffe2a0a2, 0xfff9c71f, 0x000cb6c3, 0x00162c3b, //-10.5
    0x000c1d81, 0xffe349c4, 0xfff9b8b6, 0x000cf5c3, 0x0015d637, //-10.0
    0x000bd72d, 0xffe3f07b, 0xfff9aa1d, 0x000d343d, 0x0015814a, //-9.5
    0x000b921f, 0xffe494c8, 0xfff99b55, 0x000d7230, 0x00152d74, //-9.0
    0x000b4e55, 0xffe536ac, 0xfff98c5f, 0x000daf9b, 0x0014dab5, //-8.5
    0x000b0bce, 0xffe5d62a, 0xfff97d3e, 0x000dec7d, 0x0014890c, //-8.0
    0x000aca87, 0xffe67342, 0xfff96df3, 0x000e28d4, 0x0014387a, //-7.5
    0x000a8a7e, 0xffe70df8, 0xfff95e80, 0x000e64a1, 0x0013e8fe, //-7.0
    0x000a4bb0, 0xffe7a64e, 0xfff94ee7, 0x000e9fe1, 0x00139a97, //-6.5
    0x000a0e1b, 0xffe83c48, 0xfff93f29, 0x000eda94, 0x00134d44, //-6.0
    0x0009d1be, 0xffe8cfe7, 0xfff92f47, 0x000f14ba, 0x00130105, //-5.5
    0x00099694, 0xffe9612f, 0xfff91f44, 0x000f4e52, 0x0012b5d9, //-5.0
    0x00095c9c, 0xffe9f023, 0xfff90f22, 0x000f875a, 0x00126bbe, //-4.5
    0x000923d2, 0xffea7cc8, 0xfff8fee1, 0x000fbfd4, 0x001222b3, //-4.0
    0x0008ec34, 0xffeb0721, 0xfff8ee83, 0x000ff7be, 0x0011dab7, //-3.5
    0x0008b5bf, 0xffeb8f32, 0xfff8de0a, 0x00102f18, 0x001193ca, //-3.0
    0x00088070, 0xffec1500, 0xfff8cd78, 0x001065e1, 0x00114de8, //-2.5
    0x00084c43, 0xffec988e, 0xfff8bcce, 0x00109c1a, 0x00110911, //-2.0
    0x00081936, 0xffed19e1, 0xfff8ac0e, 0x0010d1c2, 0x0010c544, //-1.5
    0x0007e745, 0xffed98fe, 0xfff89b39, 0x001106da, 0x0010827d, //-1.0
    0x0007b66c, 0xffee15eb, 0xfff88a51, 0x00113b60, 0x001040bd, //-0.5
    0x000786aa, 0xffee90aa, 0xfff87956, 0x00116f56, 0x00100000, //0.0
    0x000757f9, 0xffef0943, 0xfff8684c, 0x0011a2ba, 0x000fc045, //0.5
    0x00072a57, 0xffef7fba, 0xfff85733, 0x0011d58f, 0x000f818a, //1.0
    0x0006fdc0, 0xffeff414, 0xfff8460c, 0x001207d2, 0x000f43cd, //1.5
    0x0006d231, 0xfff06658, 0xfff834da, 0x00123986, 0x000f070b, //2.0
    0x0006a7a6, 0xfff0d68a, 0xfff8239d, 0x00126aa9, 0x000ecb43, //2.5
    0x00067e1c, 0xfff144b0, 0xfff81257, 0x00129b3d, 0x000e9072, //3.0
    0x0006558e, 0xfff1b0d0, 0xfff80109, 0x0012cb42, 0x000e5697, //3.5
    0x00062df9, 0xfff21af0, 0xfff7efb4, 0x0012fab8, 0x000e1dad, //4.0
    0x0006075b, 0xfff28316, 0xfff7de5a, 0x0013299f, 0x000de5b5, //4.5
    0x0005e1ad, 0xfff2e948, 0xfff7ccfc, 0x001357f9, 0x000daeaa, //5.0
    0x0005bcee, 0xfff34d8b, 0xfff7bb9c, 0x001385c6, 0x000d788a, //5.5
    0x0005991a, 0xfff3afe7, 0xfff7aa3a, 0x0013b306, 0x000d4354, //6.0
    0x0005762c, 0xfff41060, 0xfff798d8, 0x0013dfba, 0x000d0f04, //6.5
    0x00055421, 0xfff46efe, 0xfff78776, 0x00140be3, 0x000cdb97, //7.0
    0x000532f5, 0xfff4cbc7, 0xfff77617, 0x00143781, 0x000ca90d, //7.5
    0x000512a5, 0xfff526c0, 0xfff764bb, 0x00146295, 0x000c7760, //8.0
    0x0004f32d, 0xfff57ff0, 0xfff75363, 0x00148d21, 0x000c4690, //8.5
    0x0004d488, 0xfff5d75e, 0xfff74211, 0x0014b724, 0x000c1699, //9.0
    0x0004b6b4, 0xfff62d10, 0xfff730c5, 0x0014e09f, 0x000be779, //9.5
    0x000499ad, 0xfff6810c, 0xfff71f80, 0x00150995, 0x000bb92d, //10.0
    0x00047d6e, 0xfff6d359, 0xfff70e44, 0x00153205, 0x000b8bb2, //10.5
    0x000461f5, 0xfff723fc, 0xfff6fd11, 0x001559f0, 0x000b5f06, //11.0
    0x0004473d, 0xfff772fd, 0xfff6ebe9, 0x00158157, 0x000b3326, //11.5
    0x00042d44, 0xfff7c062, 0xfff6dacc, 0x0015a83c, 0x000b080f, //12.0
    0x00041405, 0xfff80c31, 0xfff6c9ba, 0x0015ce9f, 0x000addbf, //12.5
    0x0003fb7c, 0xfff85670, 0xfff6b8b6, 0x0015f481, 0x000ab432, //13.0
    0x0003e3a7, 0xfff89f26, 0xfff6a7c0, 0x001619e4, 0x000a8b67, //13.5
    0x0003cc81, 0xfff8e658, 0xfff696d8, 0x00163ec8, 0x000a6359, //14.0
    // Band 5, -14.0 dB ~ +14.0 dB (57 sets)
    0x0001b267, 0xfffcd416, 0xfff696d8, 0x00163ec8, 0x0004a3e3, //-14.0
    0x0001c9ba, 0xfffc9ba5, 0xfff6a7c0, 0x001619e4, 0x0004d8fd, //-13.5
    0x0001e25f, 0xfffc5fe3, 0xfff6b8b6, 0x0015f481, 0x00051087, //-13.0
    0x0001fc67, 0xfffc20a1, 0xfff6c9ba, 0x0015ce9f, 0x00054a9f, //-12.5
    0x000217e7, 0xfffbddb0, 0xfff6dacc, 0x0015a83c, 0x00058762, //-12.0
    0x000234f3, 0xfffb96dd, 0xfff6ebe9, 0x00158157, 0x0005c6f0, //-11.5
    0x000253a1, 0xfffb4bf3, 0xfff6fd11, 0x001559f0, 0x0006096b, //-11.0
    0x00027409, 0xfffafcbb, 0xfff70e44, 0x00153205, 0x00064ef4, //-10.5
    0x00029641, 0xfffaa8fa, 0xfff71f80, 0x00150995, 0x000697af, //-10.0
    0x0002ba65, 0xfffa5074, 0xfff730c5, 0x0014e09f, 0x0006e3c3, //-9.5
    0x0002e08e, 0xfff9f2e7, 0xfff74211, 0x0014b724, 0x00073356, //-9.0
    0x000308d9, 0xfff99012, 0xfff75363, 0x00148d21, 0x00078691, //-8.5
    0x00033364, 0xfff927ad, 0xfff764bb, 0x00146295, 0x0007dd9f, //-8.0
    0x0003604d, 0xfff8b96e, 0xfff77617, 0x00143781, 0x000838ad, //-7.5
    0x00038fb5, 0xfff84508, 0xfff78776, 0x00140be3, 0x000897ea, //-7.0
    0x0003c1bf, 0xfff7ca29, 0xfff798d8, 0x0013dfba, 0x0008fb87, //-6.5
    0x0003f68f, 0xfff7487c, 0xfff7aa3a, 0x0013b306, 0x000963b5, //-6.0
    0x00042e4a, 0xfff6bfa9, 0xfff7bb9c, 0x001385c6, 0x0009d0ab, //-5.5
    0x00046919, 0xfff62f51, 0xfff7ccfc, 0x001357f9, 0x000a42a0, //-5.0
    0x0004a726, 0xfff59712, 0xfff7de5a, 0x0013299f, 0x000ab9cf, //-4.5
    0x0004e89b, 0xfff4f687, 0xfff7efb4, 0x0012fab8, 0x000b3672, //-4.0
    0x00052da8, 0xfff44d43, 0xfff80109, 0x0012cb42, 0x000bb8ca, //-3.5
    0x0005767d, 0xfff39ad6, 0xfff81257, 0x00129b3d, 0x000c4119, //-3.0
    0x0005c34d, 0xfff2decb, 0xfff8239d, 0x00126aa9, 0x000ccfa2, //-2.5
    0x0006144c, 0xfff218a6, 0xfff834da, 0x00123986, 0x000d64ae, //-2.0
    0x000669b2, 0xfff147e7, 0xfff8460c, 0x001207d2, 0x000e0088, //-1.5
    0x0006c3ba, 0xfff06c07, 0xfff85733, 0x0011d58f, 0x000ea37d, //-1.0
    0x000722a2, 0xffef847a, 0xfff8684c, 0x0011a2ba, 0x000f4dde, //-0.5
    0x000786aa, 0xffee90aa, 0xfff87956, 0x00116f56, 0x00100000, //0.0
    0x0007f014, 0xffed9000, 0xfff88a51, 0x00113b60, 0x0010ba3c, //0.5
    0x00085f27, 0xffec81d9, 0xfff89b39, 0x001106da, 0x00117ced, //1.0
    0x0008d42e, 0xffeb658e, 0xfff8ac0e, 0x0010d1c2, 0x00124873, //1.5
    0x00094f76, 0xffea3a6f, 0xfff8bcce, 0x00109c1a, 0x00131d33, //2.0
    0x0009d14f, 0xffe8ffc4, 0xfff8cd78, 0x001065e1, 0x0013fb93, //2.5
    0x000a5a0f, 0xffe7b4cd, 0xfff8de0a, 0x00102f18, 0x0014e402, //3.0
    0x000aea0e, 0xffe658c2, 0xfff8ee83, 0x000ff7be, 0x0015d6ef, //3.5
    0x000b81a9, 0xffe4ead3, 0xfff8fee1, 0x000fbfd4, 0x0016d4d0, //4.0
    0x000c2141, 0xffe36a24, 0xfff90f22, 0x000f875a, 0x0017de1f, //4.5
    0x000cc93c, 0xffe1d5d2, 0xfff91f44, 0x000f4e52, 0x0018f35d, //5.0
    0x000d7a03, 0xffe02cef, 0xfff92f47, 0x000f14ba, 0x001a150d, //5.5
    0x000e3406, 0xffde6e84, 0xfff93f29, 0x000eda94, 0x001b43b9, //6.0
    0x000ef7b9, 0xffdc998d, 0xfff94ee7, 0x000e9fe1, 0x001c7ff2, //6.5
    0x000fc594, 0xffdaacfe, 0xfff95e80, 0x000e64a1, 0x001dca4d, //7.0
    0x00109e17, 0xffd8a7bd, 0xfff96df3, 0x000e28d4, 0x001f2365, //7.5
    0x001181c5, 0xffd688a4, 0xfff97d3e, 0x000dec7d, 0x00208bdc, //8.0
    0x00127129, 0xffd44e83, 0xfff98c5f, 0x000daf9b, 0x0022045a, //8.5
    0x00136cd2, 0xffd1f81b, 0xfff99b55, 0x000d7230, 0x00238d8e, //9.0
    0x00147558, 0xffcf8421, 0xfff9aa1d, 0x000d343d, 0x0025282e, //9.5
    0x00158b56, 0xffccf13a, 0xfff9b8b6, 0x000cf5c3, 0x0026d4f6, //10.0
    0x0016af72, 0xffca3e01, 0xfff9c71f, 0x000cb6c3, 0x002894ac, //10.5
    0x0017e254, 0xffc768fd, 0xfff9d555, 0x000c773e, 0x002a681b, //11.0
    0x001924b1, 0xffc470aa, 0xfff9e358, 0x000c3737, 0x002c5016, //11.5
    0x001a7740, 0xffc15371, 0xfff9f125, 0x000bf6ae, 0x002e4d7c, //12.0
    0x001bdac5, 0xffbe0fad, 0xfff9febb, 0x000bb5a5, 0x0030612f, //12.5
    0x001d5008, 0xffbaa3a6, 0xfffa0c17, 0x000b741d, 0x00328c1e, //13.0
    0x001ed7db, 0xffb70d94, 0xfffa1939, 0x000b3219, 0x0034cf3f, //13.5
    0x00207319, 0xffb34b9b, 0xfffa261f, 0x000aef9a, 0x00372b92, //14.0
};
#endif
#endif

#ifdef AUDIO_CHL_BASS_TREBLE_COEF_TBL_DEFAULT //20100524
#ifdef CC_AUD_3_5K_TREBLE
// 200Hz/3.5kHz
// -14dB ~ +14dB, 1dB/step
static const UINT32 _aau4MtkAudChlBassTrebleCoefTbl[(57*2)*5] =
{
    // 48000.0 Hz
    // Bass Band
    0x003c8374, 0xff84d9b4, 0xffc4cb42, 0x007b1ca9, 0x003ea7a7, //-14.0
    0x003c87f9, 0xff84c890, 0xffc4ba62, 0x007b2e35, 0x003eb46b, //-13.5
    0x003c8c50, 0xff84b7a9, 0xffc4a9bb, 0x007b3f83, 0x003ec121, //-13.0
    0x003c907a, 0xff84a6fc, 0xffc4994d, 0x007b5093, 0x003ecdc9, //-12.5
    0x003c9479, 0xff84968b, 0xffc48916, 0x007b6168, 0x003eda64, //-12.0
    0x003c984b, 0xff848653, 0xffc47916, 0x007b7201, 0x003ee6f4, //-11.5
    0x003c9bf0, 0xff847654, 0xffc4694c, 0x007b8260, 0x003ef377, //-11.0
    0x003c9f6a, 0xff84668d, 0xffc459b9, 0x007b9285, 0x003efff0, //-10.5
    0x003ca2b8, 0xff8456fe, 0xffc44a5a, 0x007ba271, 0x003f0c5e, //-10.0
    0x003ca5da, 0xff8447a7, 0xffc43b2f, 0x007bb224, 0x003f18c2, //-9.5
    0x003ca8d0, 0xff843885, 0xffc42c39, 0x007bc1a0, 0x003f251d, //-9.0
    0x003cab9b, 0xff842999, 0xffc41d75, 0x007bd0e6, 0x003f316f, //-8.5
    0x003cae3a, 0xff841ae2, 0xffc40ee4, 0x007bdff5, 0x003f3db9, //-8.0
    0x003cb0ae, 0xff840c5f, 0xffc40085, 0x007beecf, 0x003f49fb, //-7.5
    0x003cb2f6, 0xff83fe11, 0xffc3f257, 0x007bfd74, 0x003f5637, //-7.0
    0x003cb513, 0xff83eff5, 0xffc3e459, 0x007c0be5, 0x003f626d, //-6.5
    0x003cb706, 0xff83e20b, 0xffc3d68c, 0x007c1a23, 0x003f6e9d, //-6.0
    0x003cb8cd, 0xff83d454, 0xffc3c8ee, 0x007c282f, 0x003f7ac8, //-5.5
    0x003cba69, 0xff83c6cd, 0xffc3bb7f, 0x007c3608, 0x003f86ee, //-5.0
    0x003cbbda, 0xff83b977, 0xffc3ae3e, 0x007c43b1, 0x003f9311, //-4.5
    0x003cbd20, 0xff83ac51, 0xffc3a12b, 0x007c5129, 0x003f9f30, //-4.0
    0x003cbe3b, 0xff839f5b, 0xffc39444, 0x007c5e71, 0x003fab4d, //-3.5
    0x003cbf2b, 0xff839293, 0xffc3878b, 0x007c6b8a, 0x003fb767, //-3.0
    0x003cbff0, 0xff8385fa, 0xffc37afd, 0x007c7874, 0x003fc381, //-2.5
    0x003cc08b, 0xff83798e, 0xffc36e9a, 0x007c8531, 0x003fcf99, //-2.0
    0x003cc0fb, 0xff836d50, 0xffc36263, 0x007c91c0, 0x003fdbb2, //-1.5
    0x003cc13f, 0xff83613e, 0xffc35656, 0x007c9e22, 0x003fe7cb, //-1.0
    0x003cc159, 0xff835558, 0xffc34a72, 0x007caa58, 0x003ff3e4, //-0.5
    0x003cc148, 0xff83499d, 0xffc33eb8, 0x007cb663, 0x00400000, //0.0
    0x003cc10c, 0xff833e0e, 0xffc33326, 0x007cc243, 0x00400c1e, //0.5
    0x003cc0a5, 0xff8332a9, 0xffc327bd, 0x007ccdf8, 0x0040183f, //1.0
    0x003cc013, 0xff83276e, 0xffc31c7b, 0x007cd984, 0x00402463, //1.5
    0x003cbf57, 0xff831c5c, 0xffc31160, 0x007ce4e6, 0x0040308b, //2.0
    0x003cbe6e, 0xff831174, 0xffc3066c, 0x007cf020, 0x00403cb9, //2.5
    0x003cbd5b, 0xff8306b4, 0xffc2fb9e, 0x007cfb31, 0x004048eb, //3.0
    0x003cbc1d, 0xff82fc1c, 0xffc2f0f6, 0x007d061b, 0x00405524, //3.5
    0x003cbab3, 0xff82f1ab, 0xffc2e673, 0x007d10df, 0x00406163, //4.0
    0x003cb91e, 0xff82e761, 0xffc2dc15, 0x007d1b7b, 0x00406daa, //4.5
    0x003cb75d, 0xff82dd3f, 0xffc2d1db, 0x007d25f2, 0x004079f9, //5.0
    0x003cb571, 0xff82d342, 0xffc2c7c4, 0x007d3043, 0x00408650, //5.5
    0x003cb358, 0xff82c96b, 0xffc2bdd1, 0x007d3a6f, 0x004092b1, //6.0
    0x003cb115, 0xff82bfb9, 0xffc2b401, 0x007d4477, 0x00409f1b, //6.5
    0x003caea5, 0xff82b62c, 0xffc2aa53, 0x007d4e5b, 0x0040ab90, //7.0
    0x003cac09, 0xff82acc4, 0xffc2a0c7, 0x007d581c, 0x0040b810, //7.5
    0x003ca940, 0xff82a37f, 0xffc2975d, 0x007d61ba, 0x0040c49c, //8.0
    0x003ca64c, 0xff829a5f, 0xffc28e13, 0x007d6b35, 0x0040d135, //8.5
    0x003ca32a, 0xff829161, 0xffc284ea, 0x007d748e, 0x0040ddda, //9.0
    0x003c9fdd, 0xff828886, 0xffc27be2, 0x007d7dc6, 0x0040ea8e, //9.5
    0x003c9c62, 0xff827fce, 0xffc272f9, 0x007d86dc, 0x0040f750, //10.0
    0x003c98ba, 0xff827738, 0xffc26a2f, 0x007d8fd2, 0x00410421, //10.5
    0x003c94e5, 0xff826ec3, 0xffc26184, 0x007d98a8, 0x00411102, //11.0
    0x003c90e2, 0xff826670, 0xffc258f8, 0x007da15e, 0x00411df4, //11.5
    0x003c8cb2, 0xff825e3e, 0xffc2508a, 0x007da9f5, 0x00412af7, //12.0
    0x003c8853, 0xff82562c, 0xffc2483a, 0x007db26d, 0x0041380c, //12.5
    0x003c83c7, 0xff824e3b, 0xffc24007, 0x007dbac6, 0x00414534, //13.0
    0x003c7f0c, 0xff82466a, 0xffc237f1, 0x007dc302, 0x0041526f, //13.5
    0x003c7a23, 0xff823eb8, 0xffc22ff7, 0x007dcb20, 0x00415fbe, //14.0
    // Treble Band
    0x0001861d, 0xfffb5073, 0xfff5bc95, 0x001956bf, 0x0004161b, //-14.0
    0x00019f2a, 0xfffb09a7, 0xfff5cd62, 0x00193fc4, 0x00044a09, //-13.5
    0x0001b9c1, 0xfffabed9, 0xfff5de51, 0x00192882, 0x00048093, //-13.0
    0x0001d5fa, 0xfffa6fd1, 0xfff5ef61, 0x001910f6, 0x0004b9de, //-12.5
    0x0001f3ef, 0xfffa1c54, 0xfff60092, 0x0018f921, 0x0004f60b, //-12.0
    0x000213b8, 0xfff9c423, 0xfff611e4, 0x0018e101, 0x00053540, //-11.5
    0x00023571, 0xfff966fc, 0xfff62358, 0x0018c896, 0x000577a5, //-11.0
    0x00025938, 0xfff9049b, 0xfff634eb, 0x0018afe0, 0x0005bd62, //-10.5
    0x00027f2a, 0xfff89cb6, 0xfff646a0, 0x001896de, 0x000606a3, //-10.0
    0x0002a769, 0xfff82f01, 0xfff65874, 0x00187d8e, 0x00065394, //-9.5
    0x0002d215, 0xfff7bb2b, 0xfff66a68, 0x001863f1, 0x0006a467, //-9.0
    0x0002ff54, 0xfff740df, 0xfff67c7c, 0x00184a05, 0x0006f94c, //-8.5
    0x00032f4b, 0xfff6bfc4, 0xfff68eaf, 0x00182fcb, 0x00075278, //-8.0
    0x00036222, 0xfff6377c, 0xfff6a100, 0x00181540, 0x0007b022, //-7.5
    0x00039803, 0xfff5a7a2, 0xfff6b371, 0x0017fa66, 0x00081284, //-7.0
    0x0003d11b, 0xfff50fd1, 0xfff6c600, 0x0017df3a, 0x000879db, //-6.5
    0x00040d99, 0xfff46f98, 0xfff6d8ac, 0x0017c3bd, 0x0008e665, //-6.0
    0x00044daf, 0xfff3c686, 0xfff6eb76, 0x0017a7ee, 0x00095867, //-5.5
    0x00049190, 0xfff31422, 0xfff6fe5d, 0x00178bcb, 0x0009d026, //-5.0
    0x0004d975, 0xfff257ea, 0xfff71161, 0x00176f55, 0x000a4deb, //-4.5
    0x00052596, 0xfff1915a, 0xfff72481, 0x0017528b, 0x000ad204, //-4.0
    0x00057632, 0xfff0bfe4, 0xfff737bc, 0x0017356c, 0x000b5cc2, //-3.5
    0x0005cb88, 0xffefe2f4, 0xfff74b13, 0x001717f8, 0x000bee7a, //-3.0
    0x000625dd, 0xffeef9ed, 0xfff75e84, 0x0016fa2d, 0x000c8785, //-2.5
    0x00068578, 0xffee042d, 0xfff7720f, 0x0016dc0c, 0x000d2840, //-2.0
    0x0006eaa6, 0xffed0106, 0xfff785b5, 0x0016bd93, 0x000dd10d, //-1.5
    0x000755b4, 0xffebefc3, 0xfff79973, 0x00169ec3, 0x000e8253, //-1.0
    0x0007c6f8, 0xffeacfa7, 0xfff7ad49, 0x00167f9a, 0x000f3c7f, //-0.5
    0x00083ec8, 0xffe99fe8, 0xfff7c138, 0x00166018, 0x00100000, //0.0
    0x0008bd83, 0xffe85fb5, 0xfff7d53d, 0x0016403c, 0x0010cd4e, //0.5
    0x0009438a, 0xffe70e32, 0xfff7e959, 0x00162006, 0x0011a4e5, //1.0
    0x0009d143, 0xffe5aa75, 0xfff7fd8b, 0x0015ff76, 0x00128747, //1.5
    0x000a671a, 0xffe4338c, 0xfff811d2, 0x0015de8a, 0x001374fe, //2.0
    0x000b0583, 0xffe2a876, 0xfff8262d, 0x0015bd42, 0x00146e97, //2.5
    0x000bacf4, 0xffe10828, 0xfff83a9c, 0x00159b9e, 0x001574aa, //3.0
    0x000c5deb, 0xffdf5186, 0xfff84f1e, 0x0015799d, 0x001687d3, //3.5
    0x000d18ee, 0xffdd8369, 0xfff863b2, 0x0015573f, 0x0017a8b8, //4.0
    0x000dde87, 0xffdb9c99, 0xfff87858, 0x00153483, 0x0018d805, //4.5
    0x000eaf4a, 0xffd99bd1, 0xfff88d0d, 0x00151169, 0x001a1670, //5.0
    0x000f8bd0, 0xffd77fb9, 0xfff8a1d3, 0x0014edef, 0x001b64b5, //5.5
    0x001074bb, 0xffd546eb, 0xfff8b6a7, 0x0014ca17, 0x001cc39c, //6.0
    0x00116ab7, 0xffd2efed, 0xfff8cb88, 0x0014a5df, 0x001e33f5, //6.5
    0x00126e76, 0xffd07933, 0xfff8e077, 0x00148147, 0x001fb69a, //7.0
    0x001380b3, 0xffcde11e, 0xfff8f571, 0x00145c4e, 0x00214c6f, //7.5
    0x0014a236, 0xffcb25fc, 0xfff90a77, 0x001436f5, 0x0022f663, //8.0
    0x0015d3cd, 0xffc84602, 0xfff91f86, 0x0014113a, 0x0024b571, //8.5
    0x00171652, 0xffc53f53, 0xfff9349e, 0x0013eb1e, 0x00268a9f, //9.0
    0x00186aac, 0xffc20ff8, 0xfff949be, 0x0013c4a0, 0x002876ff, //9.5
    0x0019d1c9, 0xffbeb5e3, 0xfff95ee5, 0x00139dc0, 0x002a7bb0, //10.0
    0x001b4ca7, 0xffbb2eeb, 0xfff97411, 0x0013767d, 0x002c99e0, //10.5
    0x001cdc50, 0xffb778cf, 0xfff98942, 0x00134ed8, 0x002ed2c8, //11.0
    0x001e81d8, 0xffb39130, 0xfff99e76, 0x001326cf, 0x003127b3, //11.5
    0x00203e64, 0xffaf7594, 0xfff9b3ac, 0x0012fe63, 0x003399f8, //12.0
    0x00221327, 0xffab2360, 0xfff9c8e4, 0x0012d594, 0x00362b01, //12.5
    0x00240162, 0xffa697db, 0xfff9de1b, 0x0012ac61, 0x0038dc47, //13.0
    0x00260a66, 0xffa1d02b, 0xfff9f351, 0x001282ca, 0x003baf54, //13.5
    0x00282f94, 0xff9cc954, 0xfffa0884, 0x001258d0, 0x003ea5c5, //14.0
};
#else
static const UINT32 _aau4MtkAudChlBassTrebleCoefTbl[(29*2)*5] =
{
    // 48000.0 Hz
    // Bass Band
    0x003c8374, 0xff84d9b4, 0xffc4cb42, 0x007b1ca9, 0x003ea7a7, //-14.0
    0x003c8c50, 0xff84b7a9, 0xffc4a9bb, 0x007b3f83, 0x003ec121, //-13.0
    0x003c9479, 0xff84968b, 0xffc48916, 0x007b6168, 0x003eda64, //-12.0
    0x003c9bf0, 0xff847654, 0xffc4694c, 0x007b8260, 0x003ef377, //-11.0
    0x003ca2b8, 0xff8456fe, 0xffc44a5a, 0x007ba271, 0x003f0c5e, //-10.0
    0x003ca8d0, 0xff843885, 0xffc42c39, 0x007bc1a0, 0x003f251d, //-9.0
    0x003cae3a, 0xff841ae2, 0xffc40ee4, 0x007bdff5, 0x003f3db9, //-8.0
    0x003cb2f6, 0xff83fe11, 0xffc3f257, 0x007bfd74, 0x003f5637, //-7.0
    0x003cb706, 0xff83e20b, 0xffc3d68c, 0x007c1a23, 0x003f6e9d, //-6.0
    0x003cba69, 0xff83c6cd, 0xffc3bb7f, 0x007c3608, 0x003f86ee, //-5.0
    0x003cbd20, 0xff83ac51, 0xffc3a12b, 0x007c5129, 0x003f9f30, //-4.0
    0x003cbf2b, 0xff839293, 0xffc3878b, 0x007c6b8a, 0x003fb767, //-3.0
    0x003cc08b, 0xff83798e, 0xffc36e9a, 0x007c8531, 0x003fcf99, //-2.0
    0x003cc13f, 0xff83613e, 0xffc35656, 0x007c9e22, 0x003fe7cb, //-1.0
    0x003cc148, 0xff83499d, 0xffc33eb8, 0x007cb663, 0x00400000, //0.0
    0x003cc0a5, 0xff8332a9, 0xffc327bd, 0x007ccdf8, 0x0040183f, //1.0
    0x003cbf57, 0xff831c5c, 0xffc31160, 0x007ce4e6, 0x0040308b, //2.0
    0x003cbd5b, 0xff8306b4, 0xffc2fb9e, 0x007cfb31, 0x004048eb, //3.0
    0x003cbab3, 0xff82f1ab, 0xffc2e673, 0x007d10df, 0x00406163, //4.0
    0x003cb75d, 0xff82dd3f, 0xffc2d1db, 0x007d25f2, 0x004079f9, //5.0
    0x003cb358, 0xff82c96b, 0xffc2bdd1, 0x007d3a6f, 0x004092b1, //6.0
    0x003caea5, 0xff82b62c, 0xffc2aa53, 0x007d4e5b, 0x0040ab90, //7.0
    0x003ca940, 0xff82a37f, 0xffc2975d, 0x007d61ba, 0x0040c49c, //8.0
    0x003ca32a, 0xff829161, 0xffc284ea, 0x007d748e, 0x0040ddda, //9.0
    0x003c9c62, 0xff827fce, 0xffc272f9, 0x007d86dc, 0x0040f750, //10.0
    0x003c94e5, 0xff826ec3, 0xffc26184, 0x007d98a8, 0x00411102, //11.0
    0x003c8cb2, 0xff825e3e, 0xffc2508a, 0x007da9f5, 0x00412af7, //12.0
    0x003c83c7, 0xff824e3b, 0xffc24007, 0x007dbac6, 0x00414534, //13.0
    0x003c7a23, 0xff823eb8, 0xffc22ff7, 0x007dcb20, 0x00415fbe, //14.0
    // Treble Band
    0x0001861d, 0xfffb5073, 0xfff5bc95, 0x001956bf, 0x0004161b, //-14.0
    0x0001b9c1, 0xfffabed9, 0xfff5de51, 0x00192882, 0x00048093, //-13.0
    0x0001f3ef, 0xfffa1c54, 0xfff60092, 0x0018f921, 0x0004f60b, //-12.0
    0x00023571, 0xfff966fc, 0xfff62358, 0x0018c896, 0x000577a5, //-11.0
    0x00027f2a, 0xfff89cb6, 0xfff646a0, 0x001896de, 0x000606a3, //-10.0
    0x0002d215, 0xfff7bb2b, 0xfff66a68, 0x001863f1, 0x0006a467, //-9.0
    0x00032f4b, 0xfff6bfc4, 0xfff68eaf, 0x00182fcb, 0x00075278, //-8.0
    0x00039803, 0xfff5a7a2, 0xfff6b371, 0x0017fa66, 0x00081284, //-7.0
    0x00040d99, 0xfff46f98, 0xfff6d8ac, 0x0017c3bd, 0x0008e665, //-6.0
    0x00049190, 0xfff31422, 0xfff6fe5d, 0x00178bcb, 0x0009d026, //-5.0
    0x00052596, 0xfff1915a, 0xfff72481, 0x0017528b, 0x000ad204, //-4.0
    0x0005cb88, 0xffefe2f4, 0xfff74b13, 0x001717f8, 0x000bee7a, //-3.0
    0x00068578, 0xffee042d, 0xfff7720f, 0x0016dc0c, 0x000d2840, //-2.0
    0x000755b4, 0xffebefc3, 0xfff79973, 0x00169ec3, 0x000e8253, //-1.0
    0x00083ec8, 0xffe99fe8, 0xfff7c138, 0x00166018, 0x00100000, //0.0
    0x0009438a, 0xffe70e32, 0xfff7e959, 0x00162006, 0x0011a4e5, //1.0
    0x000a671a, 0xffe4338c, 0xfff811d2, 0x0015de8a, 0x001374fe, //2.0
    0x000bacf4, 0xffe10828, 0xfff83a9c, 0x00159b9e, 0x001574aa, //3.0
    0x000d18ee, 0xffdd8369, 0xfff863b2, 0x0015573f, 0x0017a8b8, //4.0
    0x000eaf4a, 0xffd99bd1, 0xfff88d0d, 0x00151169, 0x001a1670, //5.0
    0x001074bb, 0xffd546eb, 0xfff8b6a7, 0x0014ca17, 0x001cc39c, //6.0
    0x00126e76, 0xffd07933, 0xfff8e077, 0x00148147, 0x001fb69a, //7.0
    0x0014a236, 0xffcb25fc, 0xfff90a77, 0x001436f5, 0x0022f663, //8.0
    0x00171652, 0xffc53f53, 0xfff9349e, 0x0013eb1e, 0x00268a9f, //9.0
    0x0019d1c9, 0xffbeb5e3, 0xfff95ee5, 0x00139dc0, 0x002a7bb0, //10.0
    0x001cdc50, 0xffb778cf, 0xfff98942, 0x00134ed8, 0x002ed2c8, //11.0
    0x00203e64, 0xffaf7594, 0xfff9b3ac, 0x0012fe63, 0x003399f8, //12.0
    0x00240162, 0xffa697db, 0xfff9de1b, 0x0012ac61, 0x0038dc47, //13.0
    0x00282f94, 0xff9cc954, 0xfffa0884, 0x001258d0, 0x003ea5c5, //14.0
};
#endif
#endif

//-----------------------------------------------------------------------------
// Audio channel delay
//-----------------------------------------------------------------------------
#ifdef AUDIO_CHANNEL_DELAY_DEFAULT
static const UINT8 _au1MtkAudChDelay[AUD_STREAM_FROM_NUM] =
{
    0x0,        // OTHERS
    0x0,        // DIGITAL_TUNER
    0x0,        // ANALOG_TUNER
    0x0,        // SPDIF
    0x0,        // LINE_IN
    0x0,        // HDMI
    0x0,        // MEMORY
    0x0,        // BUF_AGT
    0x0,        // FEEDER
    0x0,        // MULTI_MEDIA
};
#endif

//-----------------------------------------------------------------------------
// IEC Configuration
//-----------------------------------------------------------------------------
#ifdef AUDIO_IEC_COPYRIGHT_DEFAULT
static const UINT8 _au1MtkAudIecCopyright[AUD_STREAM_FROM_NUM] =
{
    IEC_COPY_NEVER,                     // DVI
    IEC_COPY_FREELY,            // DIGITAL_TUNER
    IEC_COPY_FREELY,                    // ANALOG_TUNER
    IEC_COPY_FREELY,                    // SPDIF
    IEC_COPY_FREELY,                    // LINE_IN
    IEC_COPY_NEVER,                     // HDMI
    IEC_COPY_FREELY,                    // MEMORY
    IEC_COPY_FREELY,                    // BUF_AGT
    IEC_COPY_FREELY,                    // FEEDER
    IEC_COPY_FREELY,                    // MULTI_MEDIA
};
#endif
#ifdef AUDIO_IEC_CATEGORY_DEFAULT
static const UINT8 _au1MtkAudIecCategory[AUD_STREAM_FROM_NUM] = 
{
    IEC_NON_ORIGINAL_BROAD_CAST,        // DVI
    IEC_NON_ORIGINAL_BROAD_CAST,        // DIGITAL_TUNER 
    IEC_ORIGINAL_GENERAL,               // ANALOG_TUNER
    IEC_ORIGINAL_GENERAL,               // SPDIF
    IEC_ORIGINAL_GENERAL,               // LINE_IN
    IEC_NON_ORIGINAL_BROAD_CAST,        // HDMI
    IEC_ORIGINAL_GENERAL,               // MEMORY
    IEC_ORIGINAL_GENERAL,               // BUF_AGT
    IEC_ORIGINAL_GENERAL,               // FEEDER
    IEC_ORIGINAL_GENERAL,               // MULTI_MEDIA    
};
#endif
#ifdef AUDIO_IEC_WORD_LENGTH_DEFAULT
static const UINT8 _au1MtkAudIecWordLength[AUD_STREAM_FROM_NUM] =
{
    IEC_WORD_LENGTH_16BIT,              // DVI
    IEC_WORD_LENGTH_16BIT,              // DIGITAL_TUNER
    IEC_WORD_LENGTH_16BIT,              // ANALOG_TUNER
    IEC_WORD_LENGTH_16BIT,              // SPDIF
    IEC_WORD_LENGTH_16BIT,              // LINE_IN
    IEC_WORD_LENGTH_16BIT,              // HDMI
    IEC_WORD_LENGTH_16BIT,              // MEMORY
    IEC_WORD_LENGTH_16BIT,              // BUF_AGT
    IEC_WORD_LENGTH_16BIT,              // FEEDER
    IEC_WORD_LENGTH_16BIT,              // MULTI_MEDIA
};
#endif

#ifdef AUDIO_IEC_CHANNEL_STATUS_BYSOURCE_DEFAULT
static const UINT8 _au1MtkAudIecChannelStatusBySource[AUD_STREAM_FROM_NUM] =
{
    IEC_CHANNEL_STATUS_BY_SETUP,                     // DVI
    IEC_CHANNEL_STATUS_BY_SOURCE,                    // DIGITAL_TUNER
    IEC_CHANNEL_STATUS_BY_SETUP,                    // ANALOG_TUNER
    IEC_CHANNEL_STATUS_BY_SETUP,                    // SPDIF
    IEC_CHANNEL_STATUS_BY_SETUP,                    // LINE_IN
    IEC_CHANNEL_STATUS_BY_SETUP,                     // HDMI
    IEC_CHANNEL_STATUS_BY_SETUP,                    // MEMORY
    IEC_CHANNEL_STATUS_BY_SETUP,                    // BUF_AGT
    IEC_CHANNEL_STATUS_BY_SETUP,                    // FEEDER
    IEC_CHANNEL_STATUS_BY_SETUP,                    // MULTI_MEDIA
};
#endif

//-----------------------------------------------------------------------------
// Unused table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Audio DSP table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DAC config
//-----------------------------------------------------------------------------
#ifdef AUDIO_ADAC_USAGE_DEFAULT
static const AUD_ADAC_USAGE_T _arMtkAdacUsage =
{
    AUD_ADAC_NULL,
    AUD_ADAC_NULL,
    AUD_ADAC_NULL,
    AUD_ADAC_NULL,
    AUD_CODEC_MT5365,
    AUD_CODEC_NULL,
    AUD_AMP_NULL,
    AUD_AMP_NULL
};
#endif

// Define the usage of audio I2S data
#ifdef AUDIO_Digital_Data_TABLE_DEFAULT
static const AUD_DIGITAL_DATA_T _afgMtkAudDigitalDataUsage =
{
    TRUE,   //MCLK,BCK,LRCK
    TRUE,   //AOSDATA0
    TRUE,   //AOSDATA1
    FALSE,   //AOSDATA2
    FALSE,   //AOSDATA3
    FALSE   //AOSDATA4
};
#endif

//-----------------------------------------------------------------------------
// AOUT PAD config
//-----------------------------------------------------------------------------
#ifdef AUDIO_AOUT_PAD_CFG_DEFAULT
static const AUD_AOUT_PAD_CFG_T _arMtkAoutPadTable =
{
    AUD_CH_FRONT_LEFT,
    AUD_CH_REAR_LEFT,
    AUD_CH_CENTER,
    AUD_CH_DMX_LEFT,
    AUD_CH_FRONT_LEFT,
    AUD_CH_AUX_FRONT_LEFT,
};
#endif

//-----------------------------------------------------------------------------
// Internal DAC channel config
//-----------------------------------------------------------------------------
#ifdef AUDIO_INTER_DAC_USAGE_DEFAULT
static const AUD_INTER_DAC_CFG_T _arMtkInterDacUsage =
{
#ifndef CC_AUDIO_EU_PWM_OUTPUT_AUXDEC
    AUD_CHL_L_R,
    AUD_CHL_AUX,
    AUD_CHL_L_R,
    AUD_CHL_LS_RS           //monitor out use Ls/Rs channel
#else
    AUD_CHL_L_R,            ///AR0, AL0
    AUD_CHL_L_R,            ///AR2, AL2
    AUD_CHL_LS_RS,         ///AR3, AL3
    AUD_CHL_AUX,            ///AR1, AL1 AUDIO_AUX_OUT_AT_AL2AR2 = TRUE at Drv_default.h
#endif
};
#endif

//-----------------------------------------------------------------------------
// ATV FActory Mode
//-----------------------------------------------------------------------------
#ifdef AUDIO_TV_SYS_FINE_TUNE_VOL_DEFAULT
static const UINT8 _au1MtkTVFineTuneVol[SV_AUD_SYS_NUM] =
{
    0,//SV_NONE_DETECTED
    20,//SV_MTS
    20,//SV_FM_FM
    20,//SV_NTSC_M
    20,//SV_A2_BG
    20,//SV_A2_DK1
    20,//SV_A2_DK2
    20,//SV_A2_DK3
    20,//SV_PAL_I
    20,//SV_PAL_BG
    20,//SV_PAL_DK
    20,//SV_SECAM_L
    20,//SV_SECAM_L_PLUM
    20,//SV_SECAM_BG
    20//SV_SECAM_DK
};
#endif

#ifdef AUDIO_MTS_FACTORY_MODE_DEFAULT
static const AUD_MTS_FAC_T _arMtsFactory =
{
    50,                                // numers of check
    //****** stereo detection  **********************//
    35,                                // numers of pilot
    155,                               // higher threshold of pilot detection
    112,                               // lower threshold of pilot detection
    //****** SAP detection  *************************//
    30,                                // numers of SAP
    144,                               // higher threshold of SAP detection
    106,                               // lower threshold of SAP detection
    //****** high deviation mode  *******************//
    FALSE,                             // enable/disable of high deviation mode
    //****** carrier shift mode  ********************//
    TRUE,                             // enable/disable of carrier shift mode
    //****** FM saturation mute mode  ***************//
    FALSE,                             // enable/disable of saturation mute mode
    69,                                // higher threshold of saturation mute
    16,                                // lower threshold of saturation mute
    //****** FM carrier mute mode  ******************//
    FALSE,                             // enable/disable of FM carrier mute mode
    180,                               // higher threshold of FM carrier mute
    120,                                // lower threshold of FM carrier mute
    //****** prescale  ******************************//
    20,                                //mono and stereo prescale
    20,                                //SAP prescale
    //****** pilot offset calibration mode  *********//
    FALSE,                             // enable/disable of pilot calibration mode
    //****** SAP noise mute  ************************//
    4,                               //  (higher threshold - lower threshold )of SAP noise mute
    12,                                 // lower threshold of SAP noise mute
    //****** SAP Filter  [0:auto/1:manual] [0:big/1:small] ********//
    0,
    0,
};
#endif

static const AUD_A2_FAC_T _arA2Factory =
{
    //******  detection  ****************************//
    10,                                // numers of check
    10,                                // numers of double check
    1,                                 // mono weight
    1,                                 // stereo weight
    1,                                 // dual weight
    //****** high deviation mode  *******************//
    FALSE,                             // enable/disable of high deviation mode
    //****** carrier shift mode  ********************//
    FALSE,                             // enable/disable of carrier shift mode
    //****** FM carrier mute mode  ******************//
    FALSE,                             // enable/disable of FM carrier mute mode
    34,                                // higher threshold of FM carrier mute
    32,                                // lower threshold of FM carrier mute
    //****** prescale  ******************************//
    20,                                //A2 prescale
    //****** FM saturation mute mode  ***************//
    FALSE,                             // enable/disable of saturation mute mode
    //****** non-EU mode  ***************************//
    FALSE,                               // enable/disable of non-EU mode
    //****** stereo/dual detection threshold  *******************//
    128,                             // mono 2 stereo threshold for EU model
    64,                             // stereo 2 mono threshold for EU model
    112,                             // mono 2 stereo threshold for Korea model
    80,                             // stereo 2 mono threshold for Korea model
    656,                             // mono 2 stereo threshold for EU model
    896                             // stereo 2 mono threshold for EU model
};

#ifdef AUDIO_PAL_FACTORY_MODE_DEFAULT
static const AUD_PAL_FAC_T _arPalFactory =
{
    //******  detection  ****************************//
    6,                                 // correction threshold
    63,                                // total sync loop
    8,                                 // error threshold
    1056,                                // parity error threshold
    512,                               // every number frames
    //****** high deviation mode  *******************//
    TRUE,                              // enable/disable of high deviation mode
    //****** AM mute mode  ******************//
    FALSE,                             // enable/disable of AM mute mode
    53,                                // higher threshold of AM mute
    37,                                // lower threshold of AM mute
    //****** carrier shift mode  ********************//
    FALSE,                             // enable/disable of carrier shift mode
    //****** FM carrier mute mode  ******************//
    FALSE,                             // enable/disable of FM carrier mute mode
    32,                                // higher threshold of FM carrier mute
    16,                                // lower threshold of FM carrier mute
    //****** prescale  ******************************//
    20,                                //PAL prescale
    20,                                //AM prescale
    20,                                //NICAM prescale
    //****** FM saturation mute mode  ***************//
    FALSE,                             // enable/disable of saturation mute mode
    //****** non-EU mode  ***************************//
    FALSE,                       // enable/disable of non-EU mode
     //****** not to boost NICAM I ? ********************//
    FALSE                              // False :to boost nicam I by 4dB, True: not to boost NICAM I for 4dB
};
#endif

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
#ifdef AUDIO_AEQ_5BAND_TABLE_DEFAULT
//Only 48KHz is available
//B2,B1,A2,A1,B0 (A1,A2 need to apply minus), range need to 1~-1
static const UINT32 _aau4MtkAeq5BandTable[(1+1+1+1+1)*5] =
{
    //48000.0 Hz
    //Band 1
    0xFFD7AE97, 0xFFF946B1, 0x0006055D, 0x0006B94F, 0x00424C0B,
    //Band 2
    0x001FBFAB, 0xFFC061E0, 0xFFE05FDE, 0x003F9E20, 0x001FE077,
    //Band 3
    0x0019742B, 0xFFC8AC7F, 0xFFE79704, 0x00375381, 0x001EF4D1,
    //Band 4
    0x001E4E47, 0xFFC22E0E, 0xFFE1F816, 0x003DD1F2, 0x001FB9A2,
    //Band 5
    0x00009E93, 0xFFE890EC, 0xFFFD7A3E, 0x00176F14, 0x0021E72F,
};
#endif
#endif

#if 0 //CC_AUD_QUERY_VDO_INFO
static INT16 arMTKAudVdoCustTblDtv[AUD_SRM_VDO_HEIGHT_NUM][AUD_SRM_VDO_IP_NUM][AUD_SRM_VDO_RATE_NUM]=
{
    { // 480
     {15,15}, // P 50, 60
     {15,15}, // I 50, 60
    },
    { // 576
     {0,0}, // P 50, 60
     {0,0}, // I 50, 60
    },     
    { // 720
     {15,15}, // P 50, 60
     {0,0}, // I 50, 60
    },     
    { // 1080
     {20,20}, // P 50, 60
     {30,30}, // I 50, 60
    },         
};
#endif

#endif /* MTK_AUD_H */

