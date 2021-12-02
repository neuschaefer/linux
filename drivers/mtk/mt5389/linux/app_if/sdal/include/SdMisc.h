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
/**
 * @file SdMisc.h
 * @brief Platform Specific API header file
 This file contains the definitions of miscillaneous functions which are used only for 
 specific platforms or products.
 This file is a sample code of SdMisc.h and should not be included in any other header or source file.
 The actual definitions should be defined in SdMisc.h in a Product directory and it is the header to be included.
  SDAL.h has an option whether SdMisc.h is used or not.
 The corresponding preprocessor line in SDAL.h is 
 #ifndef NO_SD_MISC   
 * @author S/W Platform Lab
 * @date 2006/08/09
 */

#ifndef	_SDMISC_H_
#define	_SDMISC_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "SdType.h"
#include "SdUtil.h"

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
	TYPE DEFINITION
********************************************************************/

/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
/**
 * @brief SdMisc API의 Manual Debug 함수. 
 Manual Debug function for SdMisc API.
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdMisc_Debug(void);

typedef enum {
             SIP_ADEC, // Audio Decoder
             SIP_FEEDER,
             SIP_SIF,
             SIP_HDMI,
             SIP_AIN0,
             SIP_AIN1,
             SIP_AIN2,
             SIP_AIN3,
             SIP_AIN4,
             SIP_AIN5,
             SIP_AIN6,
} SoundInputPort_k;

//mtk01140 20071016 SDAL sharpness interface
//===== Picture enhance =====
typedef struct
{
	//Attention!!!! this struct must be same with SDAL/SdMisc.h
	uint32_t		lowGain;	// 0x23A10 [7:0]
	uint32_t		middleGain;	// 0x23A10 [15:8]
	uint32_t		highGain;	// 0x23A10 [23:16]
	uint32_t		localLow;	// 0x23A10 [31:24]
	uint32_t		localMiddle;	// 0x23A14 [7:0]
	uint32_t		localHigh;	// 0x23A14 [15:8]
	uint32_t		Clip_th;		// 0x23A1C [31:24]
	uint32_t		gain1;		// 0x25100 [31:24]
	uint32_t		gain2;		// 0x25108 [31:24]
	uint32_t		gain3;		// 0x25110 [31:24]
	uint32_t		gain4;		// 0x25120 [31:24]
	uint32_t		gain5;		// 0x2512C [31:24]
	uint32_t		gain6;		// 0x25134 [31:24]
	uint32_t		gain7;		// 0x2513C [31:24]
	uint32_t		gain8;		// 0x25144 [31:24]
	uint32_t		limit_pos_all;		// 0x2514C [23:16]
	uint32_t		limit_neg_all;		// 0x2514C [15:8]	
	uint32_t		LTIGain;	// 0x2D324 [27:20]
	uint32_t		ECTIGain;	// 0x23954 [6:4]
	uint32_t		SCTICgain;	// 0x2D28C [2:0]
	uint32_t		SCTIFgain;	// 0x2D28C [10:4]
	uint32_t		Udelay;		// 0x2D260 [2:0]
	uint32_t		Vdelay;		// 0x2D264 [2:0]
	uint32_t		MainColor;	// 0x2D270 [7:0]
	uint32_t		SubColor;	// 0x2D2D4 [7:0]
}SdMisc_PictureEnhance_t;

//===== LNA+ =====
typedef struct
{
	uint32_t	Y_Gain0;	// 0x238A0	[7:0]
	uint32_t	Y_Gain1;	// 0x238A0	[15:8]
	uint32_t	Y_Gain2;	// 0x238A0	[23:16]
	uint32_t	Y_Gain3;	// 0x238A0	[31:24]
	uint32_t	Y_Gain4;	// 0x238A4	[7:0]
	uint32_t	Y_Gain5;	// 0x238A4	[15:8]
	uint32_t	Y_Gain6;	// 0x238A4	[23:16]
	uint32_t	C_Gain0;	// 0x238C0	[7:0]
	uint32_t	C_Gain1;	// 0x238C0	[15:8]
	uint32_t	C_Gain2;	// 0x238C0	[23:16]
	uint32_t	C_Gain3;	// 0x238C0	[31:24]
	uint32_t	C_Gain4;	// 0x238C4	[7:0]
	uint32_t	C_Gain5;	// 0x238C4	[15:8]
	uint32_t	C_Gain6;	// 0x238C4	[23:16]
}SdMisc_TNRValue_t;

typedef struct
{
	uint32_t		Y1;	// 0x23880	[27]
	uint32_t		Y2;	// 0x23888	[22:20]
	uint32_t		C;	// 0x23888	[18:16]
	uint32_t		OnOff; // 0x250D4	[25]
}SdMisc_SNRValue_t;

typedef struct
{
	uint32_t		LOW_Coring;	// 0x23A18	[7:0]
	uint32_t		MID_Coring;	// 0x23A18	[15:8]
	uint32_t		HIGH_Coring;	// 0x23A18	[23:16]
}SdMisc_SharpnessGlobal_t;

typedef struct
{
	uint32_t		LOW_Coring;	// 0x23A18	[31:24]
	uint32_t		MID_Coring;	// 0x23A1C	[7:0]
	uint32_t		HIGH_Coring;	// 0x23A1C	[15:8]
}SdMisc_SharpnessLocal_t;

typedef struct
{
	uint32_t		Coring1;	// 0x25100	[7:0]
	uint32_t		Coring2;	// 0x25108	[7:0]
	uint32_t		Coring3;	// 0x25110	[7:0]
	uint32_t		Coring4;	// 0x25120	[7:0]
	uint32_t		Coring5;	// 0x2512C	[7:0]
	uint32_t		Coring6;	// 0x25134	[7:0]
	uint32_t		Coring7;	// 0x2513C	[7:0]
	uint32_t		Coring8;	// 0x25144	[7:0]
}SdMisc_Sharpness2D_t;

typedef struct
{
	SdMisc_TNRValue_t		tnr;
	SdMisc_SNRValue_t		snr;
	SdMisc_SharpnessGlobal_t	sharpG;
	SdMisc_SharpnessLocal_t	sharpL;
	SdMisc_Sharpness2D_t		sharp2D;
}SdMisc_LNAPlusSettings_t;

SdResult SdMisc_GetLNAPlusStatus(SdMisc_LNAPlusSettings_t *pSattings);
SdResult SdMisc_SetLNAPlusStatus(SdMisc_LNAPlusSettings_t *pSattings);

//mtk01140 20071015 set gamma interface
SdResult SdMisc_SetGammaData(uint8_t bMode, uint8_t **ppbTable);

//mtk01140 20071016 SDAL sharpness interface
SdResult SdMisc_GetPictureEnhanceStatus(SdMisc_PictureEnhance_t *pStatus);
SdResult SdMisc_SetPictureEnhanceStatus(SdMisc_PictureEnhance_t *pStatus);

SdResult SdMisc_Sound_Connect(SdSoundID_k eSoundId, SoundInputPort_k eInPort, SdBackEndInID_k eInputId);

SdResult SdMisc_ExtIn_Start(SdExtInID_k eExtInId);
SdResult SdMisc_ExtIn_Stop(SdExtInID_k eExtInId);

SdResult SdMisc_LoadPictureData(SdVideoID_k eVideoId, uint8_t *pData, uint32_t size);

SdResult SdMisc_AFTDone(SdBool_t bAFTDone);
SdResult SdMisc_ExtIn_LockFast(void);
SdResult SdMisc_ExtIn_LockMid(void);

#if 0 // move calibration functions to SdExtIn
//fixed type name conflict
#include "SdExtIn.h"
typedef SdExtIn_OutFormat_k SdMisc_OutFormat_k;
typedef struct
{
    uint32_t SD_CAPTURE_POS_X;
    uint32_t SD_CAPTURE_POS_Y;
} SdMisc_CapturePosition_k;
typedef SdExtIn_ControlValue_k SdMisc_ControlValue_k;
#endif

typedef struct
{
    SdBool_t bmanualAGC;
    uint8_t minHWidth;
    uint8_t maxHWidth;
    uint8_t thHigh;
    uint8_t thSuper;
} SdMisc_AGCInfo_t;

typedef enum
{
    SD_FAC_MODE_BEGIN    =    0,
             
    /* MTS FACTORY MODE */
    SD_FAC_MODE_MTS_NUM_CHECK,          ///< numbers of check
    SD_FAC_MODE_MTS_NUM_PILOT,          ///< numbers of pilot 
    SD_FAC_MODE_MTS_PILOT_HIGH,         ///< pilot higher threshold 
    SD_FAC_MODE_MTS_PILOT_LOW,          ///< pilot lower threshold 
    SD_FAC_MODE_MTS_NUM_SAP,            ///< numbers of sap 
    SD_FAC_MODE_MTS_SAP_HIGH,           ///< sap higher threshold 
    SD_FAC_MODE_MTS_SAP_LOW,            ///< sap lower threshold 
    SD_FAC_MODE_MTS_HDEV,               ///< high deviation mode on/off 
    SD_FAC_MODE_MTS_CARRIER_SHIFT,      ///< carrier shift mode on/off 
    SD_FAC_MODE_MTS_FM_SATU,            ///< fm saturation mute mode on/off 
    SD_FAC_MODE_MTS_FM_SATU_HIGH,       ///< fm saturation mute mode on/off 
    SD_FAC_MODE_MTS_FM_SATU_LOW,        ///< fm saturation mute mode on/off 
    SD_FAC_MODE_MTS_FM_MUTE,            ///< carrier sense mute mode on/off 
    SD_FAC_MODE_MTS_FM_MUTE_HIGH,       ///< higher carrier sense mute threshold 
    SD_FAC_MODE_MTS_FM_MUTE_LOW,        ///< lower carrier sense mute threshold 
    SD_FAC_MODE_MTS_FINE_VOLUME,        ///< fine tune volume for mono and stereo 
    SD_FAC_MODE_SAP_FINE_VOLUME,        ///< fine tune volume for sap 
    SD_FAC_MODE_MTS_PILOT_OFFSET,       ///< pilot shift calibration mode on/off 
    SD_FAC_MODE_SAP_MUTE_LOW,           ///< sap mute lower threshold 
    SD_FAC_MODE_SAP_MUTE_HIGH,          ///< sap mute higher threshold 
    /* A2 FACTORY MODE */
    SD_FAC_MODE_A2_NUM_CHECK,           ///< numbers of check 
    SD_FAC_MODE_A2_NUM_DOUBLE,          ///< numbers of double check 
    SD_FAC_MODE_A2_MONO_WEIGHT,         ///< mono wieght 
    SD_FAC_MODE_A2_STEREO_WEIGHT,       ///< stereo wieght 
    SD_FAC_MODE_A2_DUAL_WEIGHT,         ///< dual wieght 
    SD_FAC_MODE_A2_HDEV,                ///< high deviation mode on/off 
    SD_FAC_MODE_A2_CARRIER_SHIFT,       ///< carrier shift mode on/off 
    SD_FAC_MODE_A2_FM_MUTE,             ///< carrier sense mute mode on/off 
    SD_FAC_MODE_A2_FM_MUTE_HIGH,        ///< higher carrier sense mute threshold 
    SD_FAC_MODE_A2_FM_MUTE_LOW,         ///< lower carrier sense mute threshold 
    SD_FAC_MODE_A2_FINE_VOLUME,         ///< fine tune volume 
    SD_FAC_MODE_A2_MONO2STEREO,         ///< mono to stereo threshold
    SD_FAC_MODE_A2_STEREO2MONO,         ///< stereo to mono threshold
    /* PAL FACTORY MODE */
    SD_FAC_MODE_PAL_CORRECT,            ///< correct threshold 
    SD_FAC_MODE_PAL_SYNC_LOOP,          ///< total sync loop 
    SD_FAC_MODE_PAL_ERROR,              ///< error threshold 
    SD_FAC_MODE_PAL_PARITY_ERROR,       ///< parity error threshold 
    SD_FAC_MODE_PAL_NUM_FRAMES,         ///< every number frames 
    SD_FAC_MODE_PAL_HDEV,               ///< high deviation mode on/off 
    SD_FAC_MODE_PAL_AM_MUTE,            ///< AM carrier sense mute mode on/off
    SD_FAC_MODE_PAL_AM_MUTE_HIGH,       ///< AM higher carrier sense mute threshold 
    SD_FAC_MODE_PAL_AM_MUTE_LOW,        ///< AM lower carrier sense mute threshold 
    SD_FAC_MODE_PAL_CARRIER_SHIFT,      ///< carrier shift mode on/off 
    SD_FAC_MODE_PAL_FM_MUTE,            ///< FM carrier sense mute mode on/off
    SD_FAC_MODE_PAL_FM_MUTE_HIGH,       ///< FM higher carrier sense mute threshold 
    SD_FAC_MODE_PAL_FM_MUTE_LOW,        ///< FM lower carrier sense mute threshold 
    SD_FAC_MODE_PAL_FINE_VOLUME,        ///< fine tune volume 
    SD_FAC_MODE_AM_FINE_VOLUME,         ///< fine tune volume 
    SD_FAC_MODE_NICAM_FINE_VOLUME,      ///< fine tune volume 
    SD_FAC_MODE_FM_SATU_MUTE,           ///< fm saturation mute mode on/off  

    SD_FAC_MODE_END

}SdMisc_FacMode_k;

typedef enum
{
    SD_ATV_SOUND_TYPE_BEGIN = 0,
    SD_ATV_SOUND_TYPE_MTS,    
    SD_ATV_SOUND_TYPE_A2_KOREA,    
    SD_ATV_SOUND_TYPE_END
}  SdMisc_AtvSoundType_k;

typedef enum {
    HDMI_SOURCE_SIGNAL_UNSTABLE = 0,
    HDMI_SOURCE_SIGNAL_STABLE,
    HDMI_SOURCE_MAX
} SdMiscSoundCallbackType_k;

typedef enum {
	PatternType_S,    // test pattern at scaler output
	PatternType_M,    // test pattern at MLC output
	PatternType_L,    // test pattern at LVDS output
} SdTestPatternType_k;

typedef void (*SdMisc_SoundCallBack)(SdMiscSoundCallbackType_k eStatus);

SdResult SdMisc_GetAGCStatus(SdMisc_AGCInfo_t *pStatus);
SdResult SdMisc_SetAGCStatus(SdMisc_AGCInfo_t *pStatus);
SdResult SdMisc_ReadNoiseLevel(uint32_t *val);

//mtk01140 20071101 SS want to control PE setting, instead of always set it in modechg done.
SdResult SdMisc_SetPEDefaultValue(uint8_t bPath);

SdResult SdMisc_ReadReg(uint32_t addr, uint32_t *val);

//mtk00820 20071013 SDAL PCR/STC/PTS
unsigned long long SdMisc_GetPcr(void);
unsigned long long SdMisc_GetStc(void);
long long SdMisc_GetStcPcrDelta(void);
SdBool_t SdMisc_IsVideoFreeRun(void);
void SdMisc_VideoSynchronize(SdBool_t bOnOff);
unsigned long long SdMisc_GetVideoPts(void);
long long SdMisc_GetVideoPtsStcDelta(void);
SdBool_t SdMisc_IsAudioFreeRun(void);
void SdMisc_AudioSynchronize(SdBool_t bOnOff);
unsigned long long SdMisc_GetAudioPts(void);
long long SdMisc_GetAudioPtsStcDelta(void);

SdResult SdMisc_FacModeGetMaxMin(SdMisc_FacMode_k eFacMode, uint16_t *pMax, uint16_t *pMin);
SdResult SdMisc_FacModeGet(SdMisc_FacMode_k eFacMode, uint16_t *pValue);
SdResult SdMisc_FacModeSet(SdMisc_FacMode_k eFacMode, uint16_t Value);
SdResult SdMisc_EnableDeblocking(SdBool_t bDeblock); 
SdResult SdMisc_HDMIForce422(SdBool_t bForce422OnOff);
SdResult SdMisc_SetOSDPosition(uint32_t hOSDStart, uint32_t vOSDStart, uint32_t OSDWidth, uint32_t OSDHeight);
SdResult SdMisc_SetADCPhase(uint32_t phase);
SdResult SdMisc_SetAtvSoundType(SdMisc_AtvSoundType_k eAtvSoundType);
SdBool_t SdMisc_GetADSPOutputEnable(void);
SdResult SdMisc_InstallSoundCallback(SdMisc_SoundCallBack callback);
SdResult SdMisc_UnInstallSoundCallback(SdMisc_SoundCallBack callback);
SdResult SdMisc_StopFeedingForAUI(void);
SdResult SdMisc_SetChromaCorning(SdBool_t on);
SdResult SdMisc_SetHdmiZero(uint32_t value);
SdResult SdMisc_SetHdmiEqBoost(uint32_t value);
SdResult SdMisc_SetHdmiEqBias(uint32_t value);
SdResult SdMisc_ShowTestPattern(SdTestPatternType_k ePatternType, SdBool_t bOnOff);

#endif

