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
#ifndef MT5389LM3V1_AUD_H
#define MT5389LM3V1_AUD_H

#include "aud_if.h"

//5389m3v1 do not need audio mux gpio
static const AUD_INPUT_SW_GPIO_T _armt5389m3v1InputSwGpio = 
{
    UNUSED_GPIO,                         
    UNUSED_GPIO,                         
    UNUSED_GPIO,
    UNUSED_GPIO
};

//AdcMuxSel = 6 is for microphone in BGA 
static const AUD_INPUT_MUX_SEL_T _armt5389m3v1AudInputMux[] = 
{   // Input ID                 AdcMuxSel,  SwGpio1,    SwGpio2,    SwGpio3,    SwGpio4
    {AUD_INPUT_ID_COMP_VID_0,   2,    	    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_COMP_VID_1,   NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_COMP_VID_2,   NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_COMP_VID_3,   NO_USED, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_S_VID_0,      NO_USED,	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},  
    {AUD_INPUT_ID_S_VID_1,      NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_S_VID_2,      NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_YPBPR_0,      2,          SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_YPBPR_1,      NO_USED, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_YPBPR_2,    	NO_USED, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_VGA_0,        2,          SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_VGA_1,        NO_USED, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_0,       2,          SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_1,       2,          SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_2,       NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_3,       NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_4,       NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_DVI_0,        2,          SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_DVI_1,        NO_USED, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_DVI_2,        NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_DVI_3,        NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_DVI_4,        NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_0,      1,          SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_SCART_1,      6, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_2,      NO_USED, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_3,      NO_USED, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_AUXIN_0,      NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_AUXIN_1,      NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED}
};

static const AUD_INPUT_MUX_SEL_TABLE_T _rmt5389m3v1AudInputMuxTable = 
{
    (sizeof(_armt5389m3v1AudInputMux)/sizeof(AUD_INPUT_MUX_SEL_T)),
    _armt5389m3v1AudInputMux   
};

//-----------------------------------------------------------------------------
// Internal DAC channel config
//-----------------------------------------------------------------------------
static const AUD_INTER_DAC_CFG_T _armt5389m3v1InterDacUsage = 
{
    AUD_CHL_BYPASS, //AR0, AL0 ==> 1st SCART1
    AUD_CHL_L_R, //AR1, AL1 ==> 2nd Headphone
    AUD_CHL_L_R, //NA
    AUD_CHL_L_R, //AR1, AL1 ==> Same with 2nd Headphone setting. (0xf00051d4 = 0x00987610)
};

static const AUD_ADAC_USAGE_T _armt5389m3v1AdacUsage = 
{
    AUD_ADAC_NULL,
    AUD_ADAC_NULL,
    AUD_ADAC_NULL,
    AUD_ADAC_NULL,
    AUD_CODEC_MT5365,
    AUD_CODEC_NULL,
    AUD_AMP_TAS5721,
    AUD_AMP_NULL
};

// Define the usage of audio I2S data
static const AUD_DIGITAL_DATA_T _afgmt5389m3v1AudDigitalDataUsage =
{
    TRUE,			//MCLK,BCK,LRCK
    TRUE,			//AOSDATA0
    TRUE,			//AOSDATA1
    FALSE,			//AOSDATA2
    FALSE,			//AOSDATA3
    FALSE			//AOSDATA4
};

static const AUD_AOUT_PAD_CFG_T _armt5389m3v1AoutPadTable =
{
    AUD_CH_FRONT_LEFT,  //AOSDATA0 ==> Speaker
    AUD_CH_BYPASS_LEFT, //AOSDATA1 ==> SCART2
    AUD_CH_FRONT_LEFT,  //NA
    AUD_CH_FRONT_LEFT,  //NA
    AUD_CH_FRONT_LEFT,  //NA
    AUD_CH_FRONT_LEFT,  //NA
};

static const AUD_PAL_FAC_T _armt5389m3v1PalFactory =
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
    TRUE,                             // enable/disable of AM mute mode
    53,                                // higher threshold of AM mute
    37,                                // lower threshold of AM mute
    //****** carrier shift mode  ********************//
    FALSE,                             // enable/disable of carrier shift mode
    //****** FM carrier mute mode  ******************//
    TRUE,                             // enable/disable of FM carrier mute mode
    64,                                // higher threshold of FM carrier mute
    16,                                // lower threshold of FM carrier mute
    //****** prescale  ******************************//
    20,                                //PAL prescale
    24,                                //AM prescale
    20,                                //NICAM prescale
    //****** FM saturation mute mode  ***************//
    FALSE,                             // enable/disable of saturation mute mode
    //****** non-EU mode  ***************************//
    FALSE,                       // enable/disable of non-EU mode
     //****** not to boost NICAM I ? ********************//
    FALSE                              // False :to boost nicam I by 4dB, True: not to boost NICAM I for 4dB
};


static const AUD_MTS_FAC_T _armt5389m3v1MtsFactory =
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
    TRUE,                             // enable/disable of FM carrier mute mode
    180,                               // higher threshold of FM carrier mute
    120,                                // lower threshold of FM carrier mute
    //****** prescale  ******************************//
    20,                                //mono and stereo prescale
    20,                                //SAP prescale
    //****** pilot offset calibration mode  *********//
    FALSE,                             // enable/disable of pilot calibration mode
    //****** SAP noise mute  ************************//
    4,                               //  (higher threshold - lower threshold )of SAP noise mute
    12,                              // lower threshold of SAP noise mute
    //****** SAP Filter  [0:auto/1:manual] [0:big/1:small] ********//
    0,
    0,
};

static const AUD_A2_FAC_T _armt5389m3v1A2Factory =
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
    TRUE,                             // enable/disable of FM carrier mute mode
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


static const UINT8 _au1mt5389m3v1AudIecCopyright[AUD_STREAM_FROM_NUM] =
{
    IEC_COPY_NEVER,                     // DVI
    IEC_COPY_NEVER,            // DIGITAL_TUNER
    IEC_COPY_NEVER,                    // ANALOG_TUNER
    IEC_COPY_NEVER,                    // SPDIF
    IEC_COPY_NEVER,                    // LINE_IN
    IEC_COPY_NEVER,                     // HDMI
    IEC_COPY_NEVER,                    // MEMORY
    IEC_COPY_NEVER,                    // BUF_AGT
    IEC_COPY_NEVER,                    // FEEDER
    IEC_COPY_NEVER,                    // MULTI_MEDIA
};

#endif /* MT5389LM3V1_AUD_H */

