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
 * @file SdDisplay.h
 * @brief Display header file
 * @author S/W Platform Lab
 * @date 2006/07/07
 */

#ifndef	_SDDISPLAY_H_
#define	_SDDISPLAY_H_

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
typedef enum
{
	SD_DNIE_MODE_OFF,		///<dnie off
	SD_DNIE_MODE_ON,		///<dnie on
	SD_DNIE_MODE_DEMO,		///<dnie demo mode	
	SD_DNIE_MODE_HTPC_OFF,  ///<dnie Off when HTPC is On
	SD_DNIE_MODE_HTPC_ON,	///<dnie On when HTPC is On
	SD_DNIE_MODE_MAX	
} SdDisplay_DNIeMode_k;

typedef enum
{
	SD_COLORSPACE_AUTO,
	SD_COLORSPACE_WIDE,
	SD_COLORSPACE_XVYCC,
	SD_COLORSPACE_NATIVE,
	SD_COLORSPACE_CUSTOM,
	SD_COLORSPACE_MAX
} SdDisplay_ColorSpace_k;

typedef enum
{
	SD_COLOR_TONE_COOL1,
	SD_COLOR_TONE_COOL2,
	SD_COLOR_TONE_NORMAL,
	SD_COLOR_TONE_WARM1,
	SD_COLOR_TONE_WARM2,
	SD_COLOR_TONE_MAX	
} SdDisplay_ColorTone_k;

typedef enum
{
	SD_CUSTOM_COLOR_RED,
	SD_CUSTOM_COLOR_GREEN,
	SD_CUSTOM_COLOR_BLUE,
	SD_CUSTOM_COLOR_YELLOW,
	SD_CUSTOM_COLOR_CYAN,
	SD_CUSTOM_COLOR_MAGENTA,
	SD_CUSTOM_COLOR_MAX
} SdDisplay_CustomColor_k;

typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;	
} SdDisplay_CustomRGB_t;

typedef struct
{
	SdDisplay_ColorSpace_k 	eColorSpace;
	SdDisplay_CustomRGB_t 	sCustomColorSet[SD_CUSTOM_COLOR_MAX];
} SdDisplay_ColorSettings_t;

typedef struct
{
	uint32_t 				contrast;		///< 명암(contrast), Range :0~100
	uint32_t 				brightness;		///< 밝기(brightness), Range :0~100
	uint32_t 				sharpness;		///< 선명도 (sharpness), Range :0~100
	uint32_t 				color;			///< 색농도	(color saturation), Range :0~100
	uint32_t 				tint;			///< 색조(color tint), Range :0~100(center:50)
	SdDisplay_ColorTone_k 	colortone;		///< 색온도	(color temperature)
} SdDisplay_PictureMode_t;

typedef struct 
{
 	uint32_t red;		///< Range :0~30
	uint32_t green;		///< Range :0~30
	uint32_t blue;		///< Range :0~30
	uint32_t pink;		///< Range :0~30
	uint32_t yellow;	///< Range :0~30
	uint32_t white;		///< Range :0~30
} SdDisplay_MCCMode_t;

typedef struct
{
	SdBool_t onFlag;
	uint32_t red;		///< Range:0~9
	uint32_t green;		///< Range:0~9
	uint32_t blue;		///< Range:0~9
} SdDisplay_CVDMode_t;

typedef struct
{
	uint32_t rGain;		///< Range :0~50
	uint32_t gGain;		///< Range :0~50
	uint32_t bGain;		///< Range :0~50
	uint32_t rOffset;	///< Range :0~50
	uint32_t gOffset;	///< Range :0~50
	uint32_t bOffset;	///< Range :0~50
} SdDisplay_WhiteBalance_t;

typedef struct
{
    uint64_t ScreenSize;    ///< Capture Screen Size(byte)
    uint32_t Height;        ///< Capture Screen Height 
    uint32_t Width;         ///< Capture Screen Width
    SdColorFormat_k eFormat;///< Capture Screen Video Format 
} SdDisplay_CaptureInfo_t;

typedef struct
{
	SdDisplay_PictureMode_t 	sPictureMode; 		///< contrast, brightness, sharpness, tint etc.
	SdDisplay_CVDMode_t			sCVDMode; 			///< Color weakness mode	
	SdDisplay_WhiteBalance_t	sWhiteBalance;      ///< White Balance
	SdDisplay_MCCMode_t			sMCCMode;			///< My color control mode	
	SdColor_t 					sBackColor; 		///< Background color
	
	SdDisplay_DNIeMode_k 		eDNIeMode;			///< DNIe Mode	
	SdDisplay_ColorSettings_t	sColorSettings;		///< Color Space 및 Custom RGB군 설정.
	uint32_t					DigitalNR;     		///< Noise Reduction 레벨 설정. Noise Reduction level setting. Range:0~4
	uint32_t 					BlackAdjust;		///< Black tilt slope gain 설정. Black tilt slope gain setting. Range:0~3
	uint32_t					EnergySaving;		///< Panel의 밝기를 조정하여 소비전력을 줄이는 기능. Saving 레벨 설정. Energy saving by controlling panel brightness. Range:0~3
	uint32_t		 			DynamicContrast;	///< 영상의 명암 분포를 자동으로 검출하여 Contrast를 최적으로 자동 조정. Auto contrast adjustment by analysing screen contrast. Range:0~3
	uint32_t					EdgeEnhancement;	///< Edge Enhance 레벨 설정. Edge Enhance level setting. Range:0~1
		
	SdBool_t       				bActiveColorOn;		///< Dynamic Color 동작 on/off(FBE2 Chip 연계기능. Blue,Green효과 강화). Dynamic Color on/off(Releated to FBE2 Chip. Enhance Blue, Green Color)
	SdBool_t					bxvYCCOn;			///< Detail Setting for DLP, available values : TRUE/FALSE
	
	uint32_t 					gamma;				///< 7단계의 Gamma table index를 설정. 7 levels of gamma table index. Range:0~7
	uint32_t 					backlight;			///< Pannel backlight의 밝기 설정. Panel backlight level setting. Range:0~10
	uint32_t 					panelSize;			///< Pannel Size.
	
	SdBool_t					bPixelShiftOn;		///< Pixel Shift Function On/Off.
	int32_t						HPixelShift;		///< Horizontal Pixel Shift.
	int32_t						VPixelShift;		///< Vertical Pixel Shift.
	
	SdBool_t					bSpreadSpecOn;		///< Spread Spectrum Function On/Off.
	uint32_t					period;				///< (Spread Spectrum) display clk change period(ns)
	uint32_t					range;				///< (Spread Spectrum) display clk change range(ppm)
	
	SdBool_t					bBlueOnlyModeOn;	///< Blue Only Mode On/Off.
	int32_t						fleshTone;			///< Flesh Tone.(MCC Pink) Range: -15 ~ +15
} SdDisplay_Settings_t;


/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
/**
 * @brief Initializes the SdDisplay module
 * @remarks
 * @param ePowerState
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdDisplay_Init(SdPowerState_k ePowerState);

/**
 * @brief Apply new settings to the diplay property
 * @remarks
 * @param pSettings Desired the display property settings \n 
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdDisplay_Set(SdDisplay_Settings_t *pSettings);

/**
 * @brief Get the current settings of the display property
 * @remarks
 * @param pSettings [out]Current settings of the display property \n 
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdDisplay_Get(SdDisplay_Settings_t *pSettings);

/**
 * @brief Mute a video signal
 * @remarks DNIe Video Mute
 * @param bMuteOn video mute on/off
 * @param bOsdMuteOn osd on/off
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdDisplay_Mute(SdBool_t bMuteOn, SdBool_t bOsdMuteOn);

/**
 * @brief Display되는 화면을 Capture한다. OSD와 PIP 화면을 포함하여 최종 출력되는 화면을 Capture한다.
 * @remarks 
 * @param pDestination Capture된 Screen을 저장할 메모리 주소. address for storing the captured Screen
 * @param pCaptureInfo Capture된 Screen에 대한 정보. information of the captured Screen
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdDisplay_CaptureScreen(uint8_t* pDestination, SdDisplay_CaptureInfo_t *pCaptureInfo);

/**
 * @brief SdDisplay API의 Manual Debug 함수
 Manual Debug function of SdDisplay API
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdDisplay_Debug(void);

#endif //_SDDISPLAY_H_

