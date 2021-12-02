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
#ifndef IRIS_AUD_H
#define IRIS_AUD_H

#include "aud_if.h"

//-----------------------------------------------------------------------------
// Audio input SW
//-----------------------------------------------------------------------------

static const AUD_INPUT_MUX_SEL_T _arIrisAudInputMux[] = 
{   // Input ID             DacMuxSel,    SwGpio1,    SwGpio2,    SwGpio3,    SwGpio4
    {AUD_INPUT_ID_COMP_VID_0,    0,    		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_COMP_VID_1,    0,    		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_COMP_VID_2,    6,    		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_COMP_VID_3, NO_USED, 	  SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_S_VID_0,    NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},  
    {AUD_INPUT_ID_S_VID_1,    NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_S_VID_2,    NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_YPBPR_0,       0,       SW_NO_USED,SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_YPBPR_1,       3,       SW_NO_USED, SW_NO_USED,SW_NO_USED, SW_NO_USED},            
    {AUD_INPUT_ID_YPBPR_2,    	NO_USED, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_VGA_0,         2,       SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},    
    {AUD_INPUT_ID_VGA_1,      	NO_USED, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_0,        2,       SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},  
    {AUD_INPUT_ID_HDMI_1,     2, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_2,     2, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_3,     2, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_4,     2, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_DVI_0,            2,  SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},    
    {AUD_INPUT_ID_DVI_1,      	    2, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_DVI_2,      NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_DVI_3,      NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_DVI_4,      NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_0,    NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_1,    NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_2,      NO_USED, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_3,      NO_USED, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_AUXIN_0,      NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_AUXIN_1,      NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED}
};

static const AUD_INPUT_MUX_SEL_TABLE_T _rIrisAudInputMuxTable = 
{
    (sizeof(_arIrisAudInputMux)/sizeof(AUD_INPUT_MUX_SEL_T)),
    _arIrisAudInputMux   
};

//-----------------------------------------------------------------------------
// DAC config
//-----------------------------------------------------------------------------
static const AUD_ADAC_USAGE_T _arIrisAdacUsage = 
{
    AUD_ADAC_NULL,
    AUD_ADAC_NULL,
    AUD_ADAC_NULL,
    AUD_ADAC_NULL,
    AUD_CODEC_MT5387,
    AUD_CODEC_NULL,
    AUD_AMP_APOGEE_DDX_2051,
    AUD_AMP_NULL
};

static const AUD_INTER_DAC_CFG_T _arIrisInterDacUsage = 
{
    AUD_CHL_BYPASS,
    AUD_CHL_L_R,
    AUD_CHL_AUX
};

// parson Iris
static const AUD_AOUT_PAD_CFG_T _arIrisAoutPadTable = 
{
    AUD_CH_FRONT_LEFT,
#if 0       
    AUD_CH_FRONT_LEFT,
    AUD_CH_FRONT_LEFT,    
#else
    AUD_CH_CENTER,
    AUD_CH_REAR_LEFT,
#endif
    AUD_CH_DMX_LEFT,
    AUD_CH_FRONT_LEFT,
    AUD_CH_AUX_FRONT_LEFT,

};
static const AUD_DIGITAL_DATA_T _afgIrisAudDigitalDataUsage =
{
    TRUE,			//MCLK,BCK,LRCK
    TRUE,			//AOSDATA0
    TRUE,			//AOSDATA1
    TRUE,			//AOSDATA2
    FALSE,			//AOSDATA3
    FALSE			//AOSDATA4
};
static const AOUT_CFG_T _arIrisAudOutFmtTable =
{
    FORMAT_I2S,              // format of alignment
    DAC_24_BIT,             // number of bits per sample
    LRCK_CYC_32,            // cycles per sample
    MCLK_256FS,             // DAC sampling frequence
    FALSE,                  // Invert audio output for OP phase
    TRUE                    // Invert L/R audio output
};


//-----------------------------------------------------------------------------
// Flash AQ
//-----------------------------------------------------------------------------
/* For adding new items:
    1.Add Data Description 
    2.Modify Default data (size and content)
    3.Check Size definition in aud_if.h
    4.Check definition and parser code in aud_dsp_cfg.c
*/
static const AUD_AQ_DSCRPT_T _arIrisAudNvmDscrpt[] =
{
    {AQ_TYPE_VERSION,    "Ver",     1, 8,  1},
    {AQ_TYPE_CH_VOLUME,  "Ch Vol",  5, 10, 1},
    {AQ_TYPE_SRC_VOLUME, "Src Vol", 1,  7, 1},        
    {AQ_TYPE_VOL_TABLE,  "Vol Tab",     1, 101, 3},
    {AQ_TYPE_LEVEL,      "Level",       16,  3, 1},
    {AQ_TYPE_BASS_TREBLE,"Bass/Tre",    16,  2, 1},
    {AQ_TYPE_EQ,         "EQ",          16,  7, 1},
    {AQ_TYPE_DRC,        "DRC",         16,  8, 1},
    {AQ_TYPE_AVC,        "AVC",          4,  5, 1},
    {AQ_TYPE_LIMITER,    "Limiter",      4,  4, 3},
    {AQ_TYPE_SPKHEIGHT,  "SpkHght",      4,  1, 1},
    {AQ_TYPE_MVS,        "VSurr",        5,  7, 3},
    {AQ_TYPE_POST,       "PostScl",      1,  2, 3}    
};

static const UINT8 u1IrisAudAQDscrptSize = sizeof(_arIrisAudNvmDscrpt)/sizeof(AUD_AQ_DSCRPT_T);

static UINT8 arIrisAudNvmData[  (1*8*1) + (5*10*1) + (1*7*1) + (1*101*3) 
                              + (16*3*1) + (16*2*1)+ (16*7*1)+ (16*8*1)
                              + (4*5*1) + (4*4*3) + (4*1*1) + (5*7*3) + (1*2*3)] = 
{
    //default version : AQ0.0000
    0x41,0x51,0x30,0x2E,0x30,0x30,0x30,0x30,
    
    //Channel volume
    100,100,100,100,100,
    100,100,100,100,100,
    100,100,100,100,100,
    100,100,100,100,100,
    100,100,100,100,100,
    100,100,100,100,100,
    100,100,100,100,100,
    100,100,100,100,100,
    100,100,100,100,100,
    100,100,100,100,100,

    //Source Volume
    128,
    128,
    128,
    128,
    128,
    128,
    128,

    //Volume Table
    0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x47, 0x00, 0x00, 0x84, 0x00, 0x00, 0xD2,
    0x00, 0x01, 0x29, 0x00, 0x01, 0x8C, 0x00, 0x01, 0xFB, 0x00, 0x02, 0x75, 0x00, 0x02, 0xF6,
    0x00, 0x03, 0x85, 0x00, 0x04, 0x10, 0x00, 0x04, 0xB2, 0x00, 0x05, 0x58, 0x00, 0x05, 0xFF,
    0x00, 0x06, 0xBB, 0x00, 0x07, 0x71, 0x00, 0x08, 0x3B, 0x00, 0x08, 0xF9, 0x00, 0x09, 0xC8,
    0x00, 0x0A, 0xAC, 0x00, 0x0B, 0x46, 0x00, 0x0B, 0xEC, 0x00, 0x0C, 0x9C, 0x00, 0x0D, 0x57,
    0x00, 0x0E, 0x1C, 0x00, 0x0F, 0x76, 0x00, 0x10, 0x30, 0x00, 0x11, 0x26, 0x00, 0x12, 0x60,
    0x00, 0x13, 0x77, 0x00, 0x15, 0x11, 0x00, 0x16, 0x52, 0x00, 0x17, 0x7E, 0x00, 0x18, 0xE0,
    0x00, 0x1A, 0x46, 0x00, 0x1B, 0x7E, 0x00, 0x1C, 0xCD, 0x00, 0x1E, 0x26, 0x00, 0x1F, 0x96,
    0x00, 0x20, 0xDE, 0x00, 0x22, 0x3B, 0x00, 0x23, 0x6B, 0x00, 0x24, 0xAE, 0x00, 0x25, 0xF4,
    0x00, 0x27, 0x4D, 0x00, 0x28, 0xAB, 0x00, 0x2A, 0x1C, 0x00, 0x2B, 0x94, 0x00, 0x2D, 0x1D,
    0x00, 0x2E, 0xB1, 0x00, 0x30, 0x56, 0x00, 0x32, 0x08, 0x00, 0x33, 0xCC, 0x00, 0x35, 0x9C,
    0x00, 0x37, 0x82, 0x00, 0x39, 0x72, 0x00, 0x3B, 0x76, 0x00, 0x3D, 0x8E, 0x00, 0x3F, 0xB3,
    0x00, 0x41, 0x94, 0x00, 0x43, 0x79, 0x00, 0x45, 0x77, 0x00, 0x47, 0x75, 0x00, 0x49, 0x94,
    0x00, 0x4B, 0xB2, 0x00, 0x4D, 0xF1, 0x00, 0x50, 0x2C, 0x00, 0x52, 0x8F, 0x00, 0x54, 0xEF,
    0x00, 0x57, 0xF4, 0x00, 0x5B, 0x20, 0x00, 0x5E, 0xCA, 0x00, 0x62, 0xC8, 0x00, 0x66, 0xD9,
    0x00, 0x6B, 0x15, 0x00, 0x6F, 0x7B, 0x00, 0x74, 0x10, 0x00, 0x78, 0xD9, 0x00, 0x7D, 0xD4,
    0x00, 0x83, 0xC0, 0x00, 0x89, 0xF5, 0x00, 0x91, 0x4B, 0x00, 0x99, 0x05, 0x00, 0xA1, 0x2A,
    0x00, 0xA9, 0xBD, 0x00, 0xB2, 0xC4, 0x00, 0xBC, 0x46, 0x00, 0xC6, 0x41, 0x00, 0xD0, 0xC5,
    0x00, 0xDF, 0xC0, 0x00, 0xEF, 0xAA, 0x01, 0x03, 0x94, 0x01, 0x13, 0xE9, 0x01, 0x21, 0xED,
    0x01, 0x3B, 0x59, 0x01, 0x57, 0xF7, 0x01, 0x77, 0x2D, 0x01, 0x92, 0x0A, 0x01, 0xBE, 0x43,
    0x02, 0x00, 0x00,

    //Level   [Source][SM] Master, Ch1, Ch2 
    0x00,0x40,0x40, 0x00,0x40,0x40, 0x00,0x40,0x40, 0x00,0x40,0x40, //Line in
    0x00,0x40,0x40, 0x00,0x40,0x40, 0x00,0x40,0x40, 0x00,0x40,0x40, //HDMI/USB
    0x00,0x40,0x40, 0x00,0x40,0x40, 0x00,0x40,0x40, 0x00,0x40,0x40, //ATV
    0x00,0x40,0x40, 0x00,0x40,0x40, 0x00,0x40,0x40, 0x00,0x40,0x40, //DTV

    //Bass/Treble  [Source][SM] Bass, Treble offset.
    0,0, 6,6, 6,6, 6,6, //Line in
    6,6, 6,6, 6,6, 6,6, //HDMI/USB
    6,6, 6,6, 6,6, 6,6, //ATV    
    6,6, 6,6, 6,6, 6,6, //DTV

    //EQ    [Source][SM] EQ1 ~ EQ7
    28,28,28,28,28,28,28, 28,28,28,28,28,28,28, 28,28,28,28,28,28,28, 28,28,28,28,28,28,28, //Line in
    28,28,28,28,28,28,28, 28,28,28,28,28,28,28, 28,28,28,28,28,28,28, 28,28,28,28,28,28,28, //HDMI/USB
    28,28,28,28,28,28,28, 28,28,28,28,28,28,28, 28,28,28,28,28,28,28, 28,28,28,28,28,28,28, //ATV
    28,28,28,28,28,28,28, 28,28,28,28,28,28,28, 28,28,28,28,28,28,28, 28,28,28,28,28,28,28,  //DTV
    
    //DRC   [Source][SM] Cutoff, LFE gain, Attack_Hi, Rel_Hi, Rate_Hi, Attack_Lo, Rel_Lo, Rate_Lo 
    2,64,48,48,6,48,48,6, 2,64,48,48,6,48,48,6, 2,64,48,48,6,48,48,6, 2,64,48,48,6,48,48,6,
    2,64,48,48,6,48,48,6, 2,64,48,48,6,48,48,6, 2,64,48,48,6,48,48,6, 2,64,48,48,6,48,48,6,
    2,64,48,48,6,48,48,6, 2,64,48,48,6,48,48,6, 2,64,48,48,6,48,48,6, 2,64,48,48,6,48,48,6,
    2,64,48,48,6,48,48,6, 2,64,48,48,6,48,48,6, 2,64,48,48,6,48,48,6, 2,64,48,48,6,48,48,6,

    //AVC   [Source] Level, Gain_up, Adjust_Rate, UI_Adjust_Rate, Attack_Rate.
    0xF4, 0x6, 0x80, 0x80, 0x80,
    0xF4, 0x6, 0x80, 0x80, 0x80,
    0xF4, 0x6, 0x80, 0x80, 0x80,
    0xF4, 0x6, 0x80, 0x80, 0x80,

    //Limiter [Source] Volume, Surr, Bass, EQ.
    0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,
    0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,
    0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,
    0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,

    //Speaker Shift
    0,0,0,0,

    //MVS
    0x10,0x00,0x00, 0x1a,0x00,0x00,0x20,0x00,0x00,0x9,0x00,0x00,0x5,0x00,0x00,0x1,0x00,0x00,0x7f,0x43,0x00,
    0x10,0x00,0x00, 0x1a,0x00,0x00,0x20,0x00,0x00,0x9,0x00,0x00,0x5,0x00,0x00,0x1,0x00,0x00,0x7f,0x43,0x00,
    0x10,0x00,0x00, 0x1a,0x00,0x00,0x20,0x00,0x00,0x9,0x00,0x00,0x5,0x00,0x00,0x1,0x00,0x00,0x7f,0x43,0x00,
    0x10,0x00,0x00, 0x1a,0x00,0x00,0x20,0x00,0x00,0x9,0x00,0x00,0x5,0x00,0x00,0x1,0x00,0x00,0x7f,0x43,0x00,
    0x10,0x00,0x00, 0x1a,0x00,0x00,0x20,0x00,0x00,0x9,0x00,0x00,0x5,0x00,0x00,0x1,0x00,0x00,0x7f,0x43,0x00,    

    //Post Scale
    0x7f,0xff,0xff,
    0x7f,0xff,0xff
};


#endif /* IRIS_AUD_H */


