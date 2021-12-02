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

static const AUD_INPUT_SW_GPIO_T _arMtkInputSwGpio = 
{
    UNUSED_GPIO,                         // AUD_GPIO_SWITCH_0       17           
    UNUSED_GPIO,                         // AUD_GPIO_SWITCH_0	15
    UNUSED_GPIO,
    UNUSED_GPIO
};

static const AUD_INPUT_MUX_SEL_T _arMtkAudInputMux[] = 
{   // Input ID             DacMuxSel,    SwGpio1,    SwGpio2,    SwGpio3,    SwGpio4
    {AUD_INPUT_ID_COMP_VID_0,    0,    		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_COMP_VID_1,    1,    		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_COMP_VID_2,    2,    		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_COMP_VID_3, NO_USED, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_S_VID_0,       0,    		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},  
    {AUD_INPUT_ID_S_VID_1,       NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_S_VID_2,       NO_USED,    SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_YPBPR_0,       4,            	SW_NO_USED,SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_YPBPR_1,       NO_USED,            	SW_NO_USED, SW_NO_USED,SW_NO_USED, SW_NO_USED},            
    {AUD_INPUT_ID_YPBPR_2,    	NO_USED, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_VGA_0,         6,            	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},    
    {AUD_INPUT_ID_VGA_1,      	NO_USED, 	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_0,        3,            	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},  
    {AUD_INPUT_ID_HDMI_1,     NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_2,     NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_HDMI_3,     NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_DVI_0,         3,            	SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},    
    {AUD_INPUT_ID_DVI_1,      	3, 			SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_DVI_2,      NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},   
    {AUD_INPUT_ID_DVI_3,      NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_0,      NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_1,      NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_2,      5, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_SCART_3,      7, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_AUXIN_0,      NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED},
    {AUD_INPUT_ID_AUXIN_1,      NO_USED, 		SW_NO_USED, SW_NO_USED, SW_NO_USED, SW_NO_USED}
};

static const AUD_INPUT_MUX_SEL_TABLE_T _rMtkAudInputMuxTable = 
{
    (sizeof(_arMtkAudInputMux)/sizeof(AUD_INPUT_MUX_SEL_T)),
    _arMtkAudInputMux   
};

//-----------------------------------------------------------------------------
// Audio adac input/output format, volume control
//-----------------------------------------------------------------------------

static const AOUT_CFG_T _arMtkAudOutFmtTable = 
{
    FORMAT_RJ,              // format of alignment
    DAC_24_BIT,             // number of bits per sample
    LRCK_CYC_32,            // cycles per sample
    MCLK_256FS,             // DAC sampling frequence
    FALSE,                  // Invert audio output for OP phase
    FALSE                    // Invert L/R audio output
};

static const AOUT_CFG_T _arMtkAudInFmtTable = 
{
    FORMAT_RJ,             // format of alignment
    DAC_24_BIT,             // number of bits per sample
    LRCK_CYC_32,            // cycles per sample
    MCLK_256FS,             // DAC sampling frequence
    FALSE,                  // Invert audio output for OP phase
    TRUE                    // Invert L/R audio output 
};

static const  AUD_ADAC_VOL_CFG_T _arMtkDacVolCfg =
{
    0,                      //u4LineOutVol;
    0,                      //u4HeadphoneVol;
    0                       //u4SpeakerVol;
};

//-----------------------------------------------------------------------------
// Audio adjustable volume
//-----------------------------------------------------------------------------

#define VOL_SHM_0_DB        (0x20000)
#define VOL_SHM_NEG_6_DB    (0x20000/2)
#define VOL_SHM_NEG_12_DB   (0x20000/4)

static const INT16 _aai2MtkAudInputSrcVol[AUD_DEC_NUM][AUD_STREAM_FROM_NUM] = 
{   // OTHERS DIGITAL_TUNER ANALOG_TUNER  SPDIF   LINE_IN      HDMI    MEMORY
    {VOL_0_DB, VOL_0_DB, VOL_POS_6_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB},
    {VOL_0_DB, VOL_NEG_6_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB}
};

static UINT32 _aau4MtkAudInputSrcAgcVol[AUD_DEC_NUM][AUD_STREAM_FROM_NUM] = {
    //     OTHERS DIGITAL_TUNER  ANALOG_TUNER         SPDIF       LINE_IN          HDMI        MEMORY
    {VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB},
    {VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB}
};

static const BOOL _aafgMtkAudInputSrcAgcFlag[AUD_DEC_NUM][AUD_STREAM_FROM_NUM] = {
    {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
    {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE}
};

static const INT16 _ai2MtkAudAvInputSrcVol[AUD_INPUT_ID_MAX] = 
{   VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB,
    VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB, VOL_0_DB}; 

//-----------------------------------------------------------------------------
// AVC
//-----------------------------------------------------------------------------
static const INT16 _ai2MtkAudAvcTarget = -20;
static const INT16 _ai2MtkAudAvcGainUp =  6;
static const INT16 _ai2MtkAudAvcAttackThres =  0;
static const INT16 _ai2MtkAudAvcAdjustRate =  0x80;

//-----------------------------------------------------------------------------
// Volume table
//-----------------------------------------------------------------------------

#if 1

static const UINT32 _au4VolumeTable[100 + 1] = {

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
WORD code VOLUME[VOL_LEVEL_MAX + 1] = {

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

//-----------------------------------------------------------------------------
// EQ Configuration
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------
// Unused table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Audio DSP table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DAC config
//-----------------------------------------------------------------------------

static const AUD_ADAC_USAGE_T _arMtkAdacUsage = 
{
    AUD_ADAC_NULL,
    AUD_ADAC_NULL,
    AUD_ADAC_NULL,
    AUD_ADAC_NULL,
    AUD_CODEC_MT538xInternal,
    AUD_CODEC_NULL,
    AUD_AMP_NULL,
    AUD_AMP_NULL
};

//-----------------------------------------------------------------------------
// AOUT PAD config
//-----------------------------------------------------------------------------
static const AUD_AOUT_PAD_CFG_T _arMtkAoutPadTable = 
{
    AUD_CH_FRONT_LEFT,
    AUD_CH_REAR_LEFT,
    AUD_CH_CENTER,
    AUD_CH_DMX_LEFT,
    AUD_CH_FRONT_LEFT,
    AUD_CH_AUX_FRONT_LEFT,
};

static const AUD_AOUT_PAD_CFG_T _arMtkTermKeyAoutPadTable = 
{
    AUD_CH_FRONT_LEFT,
    AUD_CH_REAR_LEFT,
    AUD_CH_CENTER,
    AUD_CH_BYPASS_LEFT,
    AUD_CH_DMX_LEFT,
    AUD_CH_AUX_FRONT_LEFT,
};

//-----------------------------------------------------------------------------
// Internal DAC channel config
//-----------------------------------------------------------------------------
static const AUD_INTER_DAC_CFG_T _arMtkInterDacUsage = 
{
#ifndef CC_AUD_DVBT_SUPPORT
    AUD_CHL_L_R,
    AUD_CHL_L_R
#else
    AUD_CHL_L_R,
    AUD_CHL_AUX
#endif
};

//-----------------------------------------------------------------------------
// ATV FActory Mode
//-----------------------------------------------------------------------------
static const AUD_MTS_FAC_T _arMtsFactory = 
{
    50,                                // numers of check
    //****** stereo detection  **********************//
    35,                                // numers of pilot
    155,                               // higher threshold of pilot detection
    112,                               // lower threshold of pilot detection
    //****** SAP detection  *************************//
    25,                                // numers of SAP
    180,                               // higher threshold of SAP detection
    106,                               // lower threshold of SAP detection
    //****** high deviation mode  *******************//
    FALSE,                             // enable/disable of high deviation mode
    //****** carrier shift mode  ********************//
    FALSE,                             // enable/disable of carrier shift mode
    //****** FM saturation mute mode  ***************//
    FALSE,                             // enable/disable of saturation mute mode
    69,                                // higher threshold of saturation mute
    16,                                // lower threshold of saturation mute
    //****** FM carrier mute mode  ******************//
    FALSE,                             // enable/disable of FM carrier mute mode  
    144,                               // higher threshold of FM carrier mute     
    56,                                // lower threshold of FM carrier mute     
    //****** prescale  ******************************//
    20,                                //mono and stereo prescale
    20,                                //SAP prescale  
    //****** pilot offset calibration mode  *********//
    FALSE,                             // enable/disable of pilot calibration mode      
    //****** SAP noise mute  ************************//   
    120,                               // higher threshold of SAP noise mute
    98                                 // lower threshold of SAP noise mute
};

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
    //****** non-EU mute  ***************************//   
    TRUE                               // enable/disable of non-EU mute 
};

static const AUD_PAL_FAC_T _arPalFactory = 
{
    //******  detection  ****************************//
    6,                                 // correction threshold
    201,                               // total sync loop
    8,                                 // error threshold
    48,                                // parity error threshold
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
    69,                                // higher threshold of FM carrier mute     
    32,                                // lower threshold of FM carrier mute     
    //****** prescale  ******************************//
    20,                                //PAL prescale
    20,                                //AM prescale
    20,                                //NICAM prescale
    //****** FM saturation mute mode  ***************//
    FALSE,                             // enable/disable of saturation mute mode
    //****** non-EU mute  ***************************//   
    TRUE                               // enable/disable of non-EU mute 
};
#endif /* MTK_AUD_H */

