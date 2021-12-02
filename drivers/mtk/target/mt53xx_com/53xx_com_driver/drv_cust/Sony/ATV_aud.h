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
#ifndef SONY_AUD_H
#define SONY_AUD_H

#include "aud_if.h"

//-----------------------------------------------------------------------------
// Audio input SW
//-----------------------------------------------------------------------------

static const AUD_INPUT_MUX_SEL_T _arSony2K10AudInputMux[] = 
{   // Input ID                  DacMuxSel,    SwGpio1,    SwGpio2,    SwGpio3,    SwGpio4
    {AUD_INPUT_ID_COMP_VID_0,    0,            SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_COMP_VID_1,    1,            SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_COMP_VID_2,    2,            SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_COMP_VID_3,    NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_S_VID_0,       NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},  
    {AUD_INPUT_ID_S_VID_1,       NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_S_VID_2,       NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_YPBPR_0,       2,            SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_YPBPR_1,       NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},            
    {AUD_INPUT_ID_YPBPR_2,       NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_VGA_0,         3,            SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},    
    {AUD_INPUT_ID_VGA_1,         NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_0,        3,            SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},  
    {AUD_INPUT_ID_HDMI_1,        3,            SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_2,        3,            SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_3,        3,            SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_4,        NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_DVI_0,         3,            SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},    
    {AUD_INPUT_ID_DVI_1,         3,            SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_DVI_2,         3,            SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_DVI_3,         3,            SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_DVI_4,         NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_0,       NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_1,       NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_2,       NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_3,       NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_AUXIN_0,       NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_AUXIN_1,       NO_USED,      SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED}
};

static const AUD_INPUT_MUX_SEL_TABLE_T _rSony2K10AudInputMuxTable = 
{
    (sizeof(_arSony2K10AudInputMux)/sizeof(AUD_INPUT_MUX_SEL_T)),
    _arSony2K10AudInputMux   
};

static const INT16 _aai2SonyAudInputSrcVol[AUD_DEC_NUM][AUD_STREAM_FROM_NUM] = 
{   // OTHERS DIGITAL_TUNER ANALOG_TUNER  SPDIF   LINE_IN      HDMI    MEMORY
    {VOL_0_DB, VOL_0_DB, VOL_POS_6_DB, VOL_0_DB, VOL_POS_14_DB, VOL_0_DB, VOL_0_DB},
    {VOL_0_DB, VOL_NEG_6_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB}
};
//-----------------------------------------------------------------------------
// DAC config
//-----------------------------------------------------------------------------
static const AUD_ADAC_USAGE_T _arSony2K10AdacUsage = 
{
    AUD_ADAC_NULL,
    AUD_ADAC_NULL,
    AUD_ADAC_NULL,
    AUD_ADAC_NULL,
    AUD_CODEC_MT5387,
    AUD_CODEC_NULL,
    AUD_AMP_NULL,
    AUD_AMP_NULL
};

//-----------------------------------------------------------------------------
// Internal DAC channel config
//-----------------------------------------------------------------------------
static const AUD_INTER_DAC_CFG_T _arSony2K10InterDacUsage = 
{
    AUD_CHL_L_R,                  // interDAC 0	
    AUD_CHL_BYPASS,               // interDAC 1	
    AUD_CHL_AUX                   // interDAC 2	
};

//-----------------------------------------------------------------------------
// ATV FActory Mode
//-----------------------------------------------------------------------------
static const AUD_PAL_FAC_T _arSony2K10PalFactory = 
{
    //******  detection  ****************************//
    6,                                 // correction threshold
    63,                                // total sync loop
    8,                                 // error threshold
    1056,                                // parity error threshold
    512,                               // every number frames
    //****** high deviation mode  *******************//
    FALSE,                              // enable/disable of high deviation mode
    //****** AM mute mode  ******************//
    FALSE,                             // enable/disable of AM mute mode  
    53,                                // higher threshold of AM mute     
    37,                                // lower threshold of AM mute     
    //****** carrier shift mode  ********************//
    TRUE,                             // enable/disable of carrier shift mode
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
    TRUE                                // enable/disable of non-EU mode 
};

static const AUD_MTS_FAC_T _arSony2K10MtsFactory = 
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
    90,//180,                               // higher threshold of FM carrier mute     
    60,//120,                                // lower threshold of FM carrier mute     
    //****** prescale  ******************************//
    20,                                //mono and stereo prescale
    20,                                //SAP prescale  
    //****** pilot offset calibration mode  *********//
    FALSE,                             // enable/disable of pilot calibration mode      
    //****** SAP noise mute  ************************//   
    4,                               //  (higher threshold - lower threshold )of SAP noise mute
    12,                                 // lower threshold of SAP noise mute
    0x1f08                        // output level gain
};
static const UINT32 _au4SonyVolumeTable[100 + 1] = {
0x0, 0x19, 0x58, 0xBF, 0x132, 0x198, 0x1FE, 0x264, 
0x2CA, 0x331, 0x396, 0x3FD, 0x463, 0x4C9, 0x52F, 0x595, 
0x5FB, 0x661, 0x6C8, 0x72D, 0x761, 0x7D8, 0x824, 0x8A3, 
0x903, 0xA1A, 0xA64, 0xBE9, 0xC8F, 0xD80, 0xEFE, 0xFE2, 
0x1154, 0x12F7, 0x1455, 0x152C, 0x1718, 0x1904, 0x1B01, 0x1CE0, 
0x1F86, 0x21F6, 0x2525, 0x2788, 0x2920, 0x2C4F, 0x2F7D, 0x32D6, 
0x350F, 0x383E, 0x3C38, 0x3E5B, 0x41EE, 0x45BA, 0x48A2, 0x4C54, 
0x5007, 0x52EF, 0x56A2, 0x5A8D, 0x5E6D, 0x6219, 0x6678, 0x6A54, 
0x6F39, 0x741E, 0x7904, 0x7DE9, 0x82CE, 0x87B3, 0x8D64, 0x93CB, 
0x9968, 0xA09A, 0xA637, 0xAC9E, 0xB306, 0xBA38, 0xC03A, 0xC695, 
0xCE21, 0xD5AD, 0xDC07, 0xE3F9, 0xEC69, 0xF310, 0xFB35, 0x102F3, 
0x10AE5, 0x1146D, 0x11E5B, 0x12849, 0x134FE, 0x1401D, 0x14C07, 0x15726, 
0x163DC, 0x17091, 0x17D47, 0x189FD, 0x196B2, 
};

static const UINT32 _au4SonyChannelVolumeTable[100 + 1] = {
0x0, 0x51E, 0xA3D, 0xF5C, 0x147A, 0x1999, 0x1EB8, 0x23D7, 
0x28F5, 0x2E14, 0x3333, 0x3851, 0x3D70, 0x428F, 0x47AE, 0x4CCC, 
0x51EB, 0x570A, 0x5C28, 0x6147, 0x6666, 0x6B85, 0x70A3, 0x75C2, 
0x7AE1, 0x8000, 0x851E, 0x8A3D, 0x8F5C, 0x947A, 0x9999, 0x9EB8, 
0xA3D7, 0xA8F5, 0xAE14, 0xB333, 0xB851, 0xBD70, 0xC28F, 0xC7AE, 
0xCCCC, 0xD1EB, 0xD70A, 0xDC28, 0xE147, 0xE666, 0xEB85, 0xF0A3, 
0xF5C2, 0xFAE1, 0x10000, 0x1051E, 0x10A3D, 0x10F5C, 0x1147A, 0x11999, 
0x11EB8, 0x123D7, 0x128F5, 0x12E14, 0x13333, 0x13851, 0x13D70, 0x1428F, 
0x147AE, 0x14CCC, 0x151EB, 0x1570A, 0x15C28, 0x16147, 0x16666, 0x16B85, 
0x170A3, 0x175C2, 0x17AE1, 0x18000, 0x1851E, 0x18A3D, 0x18F5C, 0x1947A, 
0x19999, 0x19EB8, 0x1A3D7, 0x1A8F5, 0x1AE14, 0x1B333, 0x1B851, 0x1BD70, 
0x1C28F, 0x1C7AE, 0x1CCCC, 0x1D1EB, 0x1D70A, 0x1DC28, 0x1E147, 0x1E666, 
0x1EB85, 0x1F0A3, 0x1F5C2, 0x1FAE1, 0x20000, 
};
//0 ~ 224,  every 0.5dB per step -64db->
static const INT32 _aai4SonyAudVolOffsetMap[AUD_VOLUME_OFFSET_IDX_NUM] = 
{
    110,
    113,
    116,
    119,
    122,
    125,
    128,
    131,
    134,
    137,
    140,
    143,
    146
};

static const INT16 _aai2SonyAudInputSrcLineOutVol[AUD_DEC_NUM][AUD_STREAM_FROM_NUM] = 
{   // OTHERS DIGITAL_TUNER ANALOG_TUNER  SPDIF   LINE_IN      HDMI    MEMORY
    {VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_POS_5_DB, VOL_0_DB, VOL_0_DB},
    {VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB}
};
static const AUD_POST_SM_T _arSonySoundModeCase[AUD_SOUND_MODE_NUM][AUD_STREAM_FROM_NUM] = 
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
//-----------------------------------------------------------------------------
// Bass and Trebel Configuration
//-----------------------------------------------------------------------------
static const UINT8 _aai1SonyAudBassTrebleTable[AUD_SM_NUM][2] = {
//bass ,treble
    {18,26},//0
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
    {18,26},
    {18,26},
    {18,26},
    {18,26}
    };

//-----------------------------------------------------------------------------
// PEQ Configuration
//-----------------------------------------------------------------------------
static const UINT16 _aai1SonyAudPeqFq[AUD_SM_NUM][PEQ_SETTING_MAX] = {
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
    {100,  200,  500,  1000,  2000,  5000,	10000,	305,  3000},//case 12
    {100,  200,  500,  1000,  2000,  5000,	10000,	305,  3000},//case 13
    {100,  200,  500,  1000,  2000,  5000,	10000,	305,  3000},//case 14
    {100,  200,  500,  1000,  2000,  5000,	10000,	305,  3000}//case 15
};
static const UINT16 _aai1SonyAudPeqBw[AUD_SM_NUM][PEQ_SETTING_MAX] = {
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
    {70,  90,  300,  1000,	1500,  2000,  5000,  150,  1600},
    {70,  90,  300,  1000,	1500,  2000,  5000,  150,  1600},
    {70,  90,  300,  1000,	1500,  2000,  5000,  150,  1600},
    {70,  90,  300,  1000,	1500,  2000,  5000,  150,  1600}//15
};
static const INT8 _aai1SonyAudPeqGain[AUD_SM_NUM][PEQ_SETTING_MAX] = {
    {9,  1,  -5,  -0,  -2,  -1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2,	-1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2,	-1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2,	-1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2,	-1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2,	-1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2,	-1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2,	-1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2,	-1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2,	-1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2,	-1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2,	-1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2,	-1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2,	-1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2,	-1,  15,  18,  5},
    {9,  1,  -5,  -0,  -2,	-1,  15,  18,  5}//15
};

//-----------------------------------------------------------------------------
//Surround Configuration
//-----------------------------------------------------------------------------
static const UINT32 _aai1SonyAudVSTable[AUD_VS_NUM][AUD_SUR_PRAR_NUM] = {

    {0x100000, 0x1a0000, 0x200000, 0x90000, 0x50000, 0x10000, 0x7f4300},
	{0x100000, 0x1a0000, 0x200000, 0x90000, 0x50000, 0x10000, 0x7f4300},
	{0x100001, 0x1a0001, 0x200000, 0x90000, 0x50000, 0x10000, 0x7f4301},
	{0x100002, 0x1a0002, 0x200000, 0x90000, 0x50000, 0x10000, 0x7f4302},
    {0x100003, 0x1a0003, 0x200000, 0x90000, 0x50000, 0x10000, 0x7f4303}
};

static const UINT8 _au1SonyTVFineTuneVol[SV_AUD_SYS_NUM] = 
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
/*=====================================AQ Start============================*/

static const AUD_AQ_DSCRPT_T _arSonyAudNvmDscrpt[] =
{
    {AQ_TYPE_VERSION,      "Ver",         1, 8, 1},
    {AQ_TYPE_CH_VOLUME,    "Ch Vol",      4, 4, 1},
    {AQ_TYPE_VOL_TABLE,    "Vol Tab",    1, 101, 3},
    {AQ_TYPE_LO_VOL_TABLE, "LOV Tab",    1, 101, 3},
    {AQ_TYPE_PEQ,          "PEQ ",        12,  45, 1},
    {AQ_TYPE_BASS_TREBLE,  "Bass/Tre",    12,  2, 1},
    {AQ_TYPE_AVC,          "AVC",         3,  5, 1},
    {AQ_TYPE_LIMITER,      "Pos Limt",    3,  1, 1},
    {AQ_TYPE_SP_VOL,       "SP Vol",      3,  1, 1},
    {AQ_TYPE_LO_VOL,       "LO Vol",      3,  1, 1},
    {AQ_TYPE_SPKHEIGHT,    "SpkHght",     3,  1, 1},
    {AQ_TYPE_MVS,          "VSurr",       5,  7, 3},
};
static const UINT8 u1SonyAudAQDscrptSize = sizeof(_arSonyAudNvmDscrpt)/sizeof(AUD_AQ_DSCRPT_T);

/*
static UINT8 arSonyAudNvmData[  (1*8*1) + (4*4*1) + (1*101*3) + (1*101*3)
                              + (12*45*1) + (12*2*1)+ (3*5*1)+ (3*1*3)
                              + (3*1*1) + (3*1*1) + (3*1*1) + (5*7*3)] = 
{
    //default version : AQ0.0001
    0x41,0x51,0x30,0x2E,0x30,0x30,0x2E,0x31,
    
    //Channel volume 
    100,100,100,100,
    100,100,100,100,
    100,100,100,100,
    100,100,100,100,

    //Speaker Volume Table
    0x00,   0x00,   0x0,    0x00,   0x00,   0x19,   0x00,   0x00,   0x58,   0x00,   0x00,   0xbf,   0x00,   0x1,    0x32,
    0x00,   0x1,    0x98,   0x00,   0x1,    0xfe,   0x00,   0x2,    0x64,   0x00,   0x2,    0xca,   0x00,   0x3,    0x31,
    0x00,   0x3,    0x96,   0x00,   0x3,    0xfd,   0x00,   0x4,    0x63,   0x00,   0x4,    0xc9,   0x00,   0x5,    0x2f,
    0x00,   0x5,    0x95,   0x00,   0x5,    0xfb,   0x00,   0x6,    0x61,   0x00,   0x6,    0xc8,   0x00,   0x7,    0x2d,
    0x00,   0x7,    0x61,   0x00,   0x7,    0xd8,   0x00,   0x8,    0x24,   0x00,   0x8,    0xa3,   0x00,   0x9,    0x03,
    0x00,   0xa,    0x1a,   0x00,   0xa,    0x64,   0x00,   0xb,    0xe9,   0x00,   0xc,    0x8f,   0x00,   0xd,    0x80,
    0x00,   0xe,    0xfe,   0x00,   0xf,    0xe2,   0x00,   0x11,   0x54,   0x00,   0x12,   0xf7,   0x00,   0x14,   0x55,
    0x00,   0x15,   0x2c,   0x00,   0x17,   0x18,   0x00,   0x19,   0x04,   0x00,   0x1b,   0x01,   0x00,   0x1c,   0xe0,
    0x00,   0x1f,   0x86,   0x00,   0x21,   0xf6,   0x00,   0x25,   0x25,   0x00,   0x27,   0x88,   0x00,   0x29,   0x20,
    0x00,   0x2c,   0x4f,   0x00,   0x2f,   0x7d,   0x00,   0x32,   0xd6,   0x00,   0x35,   0x0f,   0x00,   0x38,   0x3e,
    0x00,   0x3c,   0x38,   0x00,   0x3e,   0x5b,   0x00,   0x41,   0xee,   0x00,   0x45,   0xba,   0x00,   0x48,   0xa2,
    0x00,   0x4c,   0x54,   0x00,   0x50,   0x07,   0x00,   0x52,   0xef,   0x00,   0x56,   0xa2,   0x00,   0x5a,   0x8d,
    0x00,   0x5e,   0x6d,   0x00,   0x62,   0x19,   0x00,   0x66,   0x78,   0x00,   0x6a,   0x54,   0x00,   0x6f,   0x39,
    0x00,   0x74,   0x1e,   0x00,   0x79,   0x04,   0x00,   0x7d,   0xe9,   0x00,   0x82,   0xce,   0x00,   0x87,   0xb3,
    0x00,   0x8d,   0x64,   0x00,   0x93,   0xcb,   0x00,   0x99,   0x68,   0x00,   0xa0,   0x9a,   0x00,   0xa6,   0x37,
    0x00,   0xac,   0x9e,   0x00,   0xb3,   0x06,   0x00,   0xba,   0x38,   0x00,   0xc0,   0x3a,   0x00,   0xc6,   0x95,
    0x00,   0xce,   0x21,   0x00,   0xd5,   0xad,   0x00,   0xdc,   0x07,   0x00,   0xe3,   0xf9,   0x00,   0xec,   0x69,
    0x00,   0xf3,   0x10,   0x00,   0xfb,   0x35,   0x1,    0x02,   0xf3,   0x1,    0x0a,   0xe5,   0x1,    0x16,   0x4d,
    0x1,    0x1e,   0x5b,   0x1,    0x28,   0x49,   0x1,    0x34,   0xfe,   0x1,    0x40,   0x1d,   0x1,    0x4c,   0x07,
    0x1,    0x57,   0x26,   0x1,    0x63,   0xdc,   0x1,    0x70,   0x91,   0x1,    0x7d,   0x47,   0x1,    0x89,   0xfd,
    0x1,    0x96,   0xb2,
    
    //Line out Volume Table
    0x00,   0x00,   0x0,    0x00,   0x5,    0x1e,   0x00,   0xa,    0x3d,   0x00,   0xf,    0x5c,   0x00,   0x14,   0x7a,
    0x00,   0x19,   0x99,   0x00,   0x1e,   0xb8,   0x00,   0x23,   0xd7,   0x00,   0x28,   0xf5,   0x00,   0x2e,   0x14,
    0x00,   0x33,   0x33,   0x00,   0x38,   0x51,   0x00,   0x3d,   0x70,   0x00,   0x42,   0x8f,   0x00,   0x47,   0xae,
    0x00,   0x4c,   0xcc,   0x00,   0x51,   0xeb,   0x00,   0x57,   0x0a,   0x00,   0x5c,   0x28,   0x00,   0x61,   0x47,
    0x00,   0x66,   0x66,   0x00,   0x6b,   0x85,   0x00,   0x70,   0xa3,   0x00,   0x75,   0xc2,   0x00,   0x7a,   0xe1,
    0x00,   0x80,   0x00,   0x00,   0x85,   0x1e,   0x00,   0x8a,   0x3d,   0x00,   0x8f,   0x5c,   0x00,   0x94,   0x7a,
    0x00,   0x99,   0x99,   0x00,   0x9e,   0xb8,   0x00,   0xa3,   0xd7,   0x00,   0xa8,   0xf5,   0x00,   0xae,   0x14,
    0x00,   0xb3,   0x33,   0x00,   0xb8,   0x51,   0x00,   0xbd,   0x70,   0x00,   0xc2,   0x8f,   0x00,   0xc7,   0xae,
    0x00,   0xcc,   0xcc,   0x00,   0xd1,   0xeb,   0x00,   0xd7,   0x0a,   0x00,   0xdc,   0x28,   0x00,   0xe1,   0x47,
    0x00,   0xe6,   0x66,   0x00,   0xeb,   0x85,   0x00,   0xf0,   0xa3,   0x00,   0xf5,   0xc2,   0x00,   0xfa,   0xe1,
    0x1,    0x00,   0x00,   0x1,    0x05,   0x1e,   0x1,    0x0a,   0x3d,   0x1,    0x0f,   0x5c,   0x1,    0x14,   0x7a,
    0x1,    0x19,   0x99,   0x1,    0x1e,   0xb8,   0x1,    0x23,   0xd7,   0x1,    0x28,   0xf5,   0x1,    0x2e,   0x14,
    0x1,    0x33,   0x33,   0x1,    0x38,   0x51,   0x1,    0x3d,   0x70,   0x1,    0x42,   0x8f,   0x1,    0x47,   0xae,
    0x1,    0x4c,   0xcc,   0x1,    0x51,   0xeb,   0x1,    0x57,   0x0a,   0x1,    0x5c,   0x28,   0x1,    0x61,   0x47,
    0x1,    0x66,   0x66,   0x1,    0x6b,   0x85,   0x1,    0x70,   0xa3,   0x1,    0x75,   0xc2,   0x1,    0x7a,   0xe1,
    0x1,    0x80,   0x00,   0x1,    0x85,   0x1e,   0x1,    0x8a,   0x3d,   0x1,    0x8f,   0x5c,   0x1,    0x94,   0x7a,
    0x1,    0x99,   0x99,   0x1,    0x9e,   0xb8,   0x1,    0xa3,   0xd7,   0x1,    0xa8,   0xf5,   0x1,    0xae,   0x14,
    0x1,    0xb3,   0x33,   0x1,    0xb8,   0x51,   0x1,    0xbd,   0x70,   0x1,    0xc2,   0x8f,   0x1,    0xc7,   0xae,
    0x1,    0xcc,   0xcc,   0x1,    0xd1,   0xeb,   0x1,    0xd7,   0x0a,   0x1,    0xdc,   0x28,   0x1,    0xe1,   0x47,
    0x1,    0xe6,   0x66,   0x1,    0xeb,   0x85,   0x1,    0xf0,   0xa3,   0x1,    0xf5,   0xc2,   0x1,    0xfa,   0xe1,
    0x2,    0x00,   0x00,

//PEQ Table 12x45x1
//Line-in

    0x0,    0x64,   0x00,   0x46,   0x9,    0x00,   0xC8,   0x00,   0x5A,   0x1,    0x1,    0xF4,   0x1,    0x2C,   0xFB,
    0x3,    0xE8,   0x3,    0xE8,   0x0,    0x7,    0xD0,   0x5,    0xDC,   0xFE,   0x13,   0x88,   0x7,    0xD0,   0xFF,
    0x27,   0x10,   0x13,   0x88,   0xF,    0x1,    0x31,   0x00,   0x96,   0x12,   0xB,    0xB8,   0x6,    0x40,   0x5,

    0x0,    0x64,   0x00,   0x46,   0x9,    0x00,   0xC8,   0x00,   0x5A,   0x1,    0x1,    0xF4,   0x1,    0x2C,   0xFB,
    0x3,    0xE8,   0x3,    0xE8,   0x0,    0x7,    0xD0,   0x5,    0xDC,   0xFE,   0x13,   0x88,   0x7,    0xD0,   0xFF,
    0x27,   0x10,   0x13,   0x88,   0xF,    0x1,    0x31,   0x00,   0x96,   0x12,   0xB,    0xB8,   0x6,    0x40,   0x5,

    0x0,    0x64,   0x00,   0x46,   0x9,    0x00,   0xC8,   0x00,   0x5A,   0x1,    0x1,    0xF4,   0x1,    0x2C,   0xFB,
    0x3,    0xE8,   0x3,    0xE8,   0x0,    0x7,    0xD0,   0x5,    0xDC,   0xFE,   0x13,   0x88,   0x7,    0xD0,   0xFF,
    0x27,   0x10,   0x13,   0x88,   0xF,    0x1,    0x31,   0x00,   0x96,   0x12,   0xB,    0xB8,   0x6,    0x40,   0x5,

    0x0,    0x64,   0x00,   0x46,   0x9,    0x00,   0xC8,   0x00,   0x5A,   0x1,    0x1,    0xF4,   0x1,    0x2C,   0xFB,
    0x3,    0xE8,   0x3,    0xE8,   0x0,    0x7,    0xD0,   0x5,    0xDC,   0xFE,   0x13,   0x88,   0x7,    0xD0,   0xFF,
    0x27,   0x10,   0x13,   0x88,   0xF,    0x1,    0x31,   0x00,   0x96,   0x12,   0xB,    0xB8,   0x6,    0x40,   0x5,

//HDMI USB
    0x0,    0x64,   0x00,   0x46,   0x9,    0x00,   0xC8,   0x00,   0x5A,   0x1,    0x1,    0xF4,   0x1,    0x2C,   0xFB,
    0x3,    0xE8,   0x3,    0xE8,   0x0,    0x7,    0xD0,   0x5,    0xDC,   0xFE,   0x13,   0x88,   0x7,    0xD0,   0xFF,
    0x27,   0x10,   0x13,   0x88,   0xF,    0x1,    0x31,   0x00,   0x96,   0x12,   0xB,    0xB8,   0x6,    0x40,   0x5,

    0x0,    0x64,   0x00,   0x46,   0x9,    0x00,   0xC8,   0x00,   0x5A,   0x1,    0x1,    0xF4,   0x1,    0x2C,   0xFB,
    0x3,    0xE8,   0x3,    0xE8,   0x0,    0x7,    0xD0,   0x5,    0xDC,   0xFE,   0x13,   0x88,   0x7,    0xD0,   0xFF,
    0x27,   0x10,   0x13,   0x88,   0xF,    0x1,    0x31,   0x00,   0x96,   0x12,   0xB,    0xB8,   0x6,    0x40,   0x5,

    0x0,    0x64,   0x00,   0x46,   0x9,    0x00,   0xC8,   0x00,   0x5A,   0x1,    0x1,    0xF4,   0x1,    0x2C,   0xFB,
    0x3,    0xE8,   0x3,    0xE8,   0x0,    0x7,    0xD0,   0x5,    0xDC,   0xFE,   0x13,   0x88,   0x7,    0xD0,   0xFF,
    0x27,   0x10,   0x13,   0x88,   0xF,    0x1,    0x31,   0x00,   0x96,   0x12,   0xB,    0xB8,   0x6,    0x40,   0x5,

    0x0,    0x64,   0x00,   0x46,   0x9,    0x00,   0xC8,   0x00,   0x5A,   0x1,    0x1,    0xF4,   0x1,    0x2C,   0xFB,
    0x3,    0xE8,   0x3,    0xE8,   0x0,    0x7,    0xD0,   0x5,    0xDC,   0xFE,   0x13,   0x88,   0x7,    0xD0,   0xFF,
    0x27,   0x10,   0x13,   0x88,   0xF,    0x1,    0x31,   0x00,   0x96,   0x12,   0xB,    0xB8,   0x6,    0x40,   0x5,

//ATV
    0x0,    0x64,   0x00,   0x46,   0x9,    0x00,   0xC8,   0x00,   0x5A,   0x1,    0x1,    0xF4,   0x1,    0x2C,   0xFB,
    0x3,    0xE8,   0x3,    0xE8,   0x0,    0x7,    0xD0,   0x5,    0xDC,   0xFE,   0x13,   0x88,   0x7,    0xD0,   0xFF,
    0x27,   0x10,   0x13,   0x88,   0xF,    0x1,    0x31,   0x00,   0x96,   0x12,   0xB,    0xB8,   0x6,    0x40,   0x5,

    0x0,    0x64,   0x00,   0x46,   0x9,    0x00,   0xC8,   0x00,   0x5A,   0x1,    0x1,    0xF4,   0x1,    0x2C,   0xFB,
    0x3,    0xE8,   0x3,    0xE8,   0x0,    0x7,    0xD0,   0x5,    0xDC,   0xFE,   0x13,   0x88,   0x7,    0xD0,   0xFF,
    0x27,   0x10,   0x13,   0x88,   0xF,    0x1,    0x31,   0x00,   0x96,   0x12,   0xB,    0xB8,   0x6,    0x40,   0x5,

    0x0,    0x64,   0x00,   0x46,   0x9,    0x00,   0xC8,   0x00,   0x5A,   0x1,    0x1,    0xF4,   0x1,    0x2C,   0xFB,
    0x3,    0xE8,   0x3,    0xE8,   0x0,    0x7,    0xD0,   0x5,    0xDC,   0xFE,   0x13,   0x88,   0x7,    0xD0,   0xFF,
    0x27,   0x10,   0x13,   0x88,   0xF,    0x1,    0x31,   0x00,   0x96,   0x12,   0xB,    0xB8,   0x6,    0x40,   0x5,

    0x0,    0x64,   0x00,   0x46,   0x9,    0x00,   0xC8,   0x00,   0x5A,   0x1,    0x1,    0xF4,   0x1,    0x2C,   0xFB,
    0x3,    0xE8,   0x3,    0xE8,   0x0,    0x7,    0xD0,   0x5,    0xDC,   0xFE,   0x13,   0x88,   0x7,    0xD0,   0xFF,
    0x27,   0x10,   0x13,   0x88,   0xF,    0x1,    0x31,   0x00,   0x96,   0x12,   0xB,    0xB8,   0x6,    0x40,   0x5,


//Bass & Treble  12x2x1
    18,26,
    18,26,
    18,26,
    18,26,
    18,26,
    18,26,
    18,26,
    18,26,
    18,26,
    18,26,
    18,26,
    18,26,


//AVC 3x5x1
    0x01,0xE8,0x04,0x00,0x80,
    0x01,0xE8,0x04,0x00,0x80,
    0x01,0xE8,0x04,0x00,0x80,


//Post limiter 3x1x3
    0xb,0xd2,0x00,
    0xb,0xd2,0x00,
    0xb,0xd2,0x00,

//Source Volume Speaker Vol  3x1x1
    0x9C,
    0x9C,
    0x9C,

//Source Volume Line Out Vol 3x1x1
    0x7E,
    0x7E,
    0x7E,

//Speaker Shift 3x1x1
    0x0,
    0x0,
    0x0,

//MVS 5x7x3
    0x10,   0x00,   0x00,   0x1a,   0x00,   0x00,   0x20,
    0x00,   0x00,   0x9,    0x00,   0x00,   0x5,    0x00,
    0x00,   0x1,    0x00,   0x00,   0x7f,   0x43,   0x00,
    0x10,   0x00,   0x00,   0x1a,   0x00,   0x00,   0x20,
    0x00,   0x00,   0x9,    0x00,   0x00,   0x5,    0x00,
    0x00,   0x1,    0x00,   0x00,   0x7f,   0x43,   0x00,
    0x10,   0x00,   0x01,   0x1a,   0x00,   0x01,   0x20,
    0x00,   0x00,   0x9,    0x00,   0x00,   0x5,    0x00,
    0x00,   0x1,    0x00,   0x00,   0x7f,   0x43,   0x01,
    0x10,   0x00,   0x02,   0x1a,   0x00,   0x02,   0x20,
    0x00,   0x00,   0x9,    0x00,   0x00,   0x5,    0x00,
    0x00,   0x1,    0x00,   0x00,   0x7f,   0x43,   0x02,
    0x10,   0x00,   0x03,   0x1a,   0x00,   0x03,   0x20,
    0x00,   0x00,   0x9,    0x00,   0x00,   0x5,    0x00,
    0x00,   0x1,    0x00,   0x00,   0x7f,   0x43,   0x03
};
*/
/*=====================================AQ End=============================*/
#endif /* SONY_AUD_H */

