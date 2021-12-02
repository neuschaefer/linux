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
 * @file SdTTX.h
 * @brief TTX header file
 * @author S/W Platform Lab
 * @date 2008/04/17
 */

#ifndef	_SDTTX_H_
#define	_SDTTX_H_

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
	SD_TTX_STATUS_AVAILABLE	= 0x01, 
	SD_TTX_STATUS_MAX	
} SdTTX_StatusMask_k;

typedef enum
{
	SD_TTX_KEY_MIX  = 0, 
	SD_TTX_KEY_CANCEL,
	SD_TTX_KEY_HOLD,  
	SD_TTX_KEY_SIZE, 
	SD_TTX_KEY_REVEAL,
	SD_TTX_KEY_SUBPAGE,
	SD_TTX_KEY_INDEX,
	SD_TTX_KEY_LISTFLOF,
	SD_TTX_KEY_STORE,
	SD_TTX_KEY_DIGIT1,
	SD_TTX_KEY_DIGIT2,
	SD_TTX_KEY_DIGIT3,
	SD_TTX_KEY_DIGIT4,
	SD_TTX_KEY_DIGIT5,
	SD_TTX_KEY_DIGIT6,
	SD_TTX_KEY_DIGIT7,
	SD_TTX_KEY_DIGIT8,
	SD_TTX_KEY_DIGIT9,
	SD_TTX_KEY_DIGIT0,
	SD_TTX_KEY_CH_UP,
	SD_TTX_KEY_CH_DOWN,
	SD_TTX_KEY_RED,
	SD_TTX_KEY_GREEN,
	SD_TTX_KEY_YELLOW,
	SD_TTX_KEY_BLUE,
} SdTTX_Key_k;

typedef enum
{
	SD_TTX_FLOF_MODE = 0,
	SD_TTX_LIST_MODE,
} SdTTX_Mode_k;

typedef enum
{
	SD_TTX_LANG_WESTEUROPE = 0,
	SD_TTX_LANG_EASTEUROPE,
	SD_TTX_LANG_RUSSIA,
	SD_TTX_LANG_GREEK,
	SD_TTX_LANG_TURKEY,
	SD_TTX_LANG_ARAB,
	SD_TTX_LANG_FARSI,
	SD_TTX_LANG_HEBREW,
} SdTTX_LanguageGroup_k;

typedef struct
{
	uint8_t R_Mag_Num;
	uint8_t R_Page_Num;
	uint8_t G_Mag_Num;
	uint8_t G_Page_Num;
	uint8_t B_Mag_Num;
	uint8_t B_Page_Num;	
	uint8_t Y_Mag_Num;
	uint8_t Y_Page_Num;	
} SdTTX_ColorKeyIndex_t;

typedef struct
{
	SdTTX_Mode_k eMode;		///< TTX Mode
	SdTTX_ColorKeyIndex_t sColorKeyIndex;  ///< Color Key Index
	SdBool_t bShow; 	///< TTX Show On/Off
	SdTTX_LanguageGroup_k eLangGroup; ///< TTX Language Group
} SdTTX_Settings_t;

typedef struct
{
	SdBool_t bAvailable; ///< TRUE : TTX is Available, FALSE : TTX is not Available
} SdTTX_Status_t;

/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/

/**
 * @brief TTX 모듈에 대한 최초의 초기화 과정을 수행한다. 
 It carries out the first initialization prodecure for the Vendor TTX module.
 * @remarks 
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTTX_Init(void);

/**
 * @brief TTX 관련 설정값을 설정한다. 
 It sets the TTX-related setting values. 
 * @remarks
 * @param pSettings
 * @code
	typedef struct
	{
		SdTTX_Mode_k eMode;		///< TTX Mode
		SdTTX_ColorKeyIndex_t sColorKeyIndex;  ///< Color Key Index
		SdBool_t bShow;
	} SdTTX_Settings_t;
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTTX_Set(SdTTX_Settings_t* pSettings);


/**
 * @brief TTX 관련 설정값을 읽어온다. 
 It reads in the TTX-related setting values.
 * @remarks
 * @param pSettings
 * @code
	typedef struct
	{
		SdTTX_Mode_k eMode;		///< TTX Mode
		SdTTX_ColorKeyIndex_t sColorKeyIndex;  ///< Color Key Index
		SdBool_t bShow;
	} SdTTX_Settings_t;
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTTX_Get(SdTTX_Settings_t* pSettings);

/**
 * @brief TTX 입력 path를 설정하는 함수. ATV TTX 또는 DTV TTX와 연결한다.
 ExtIn component를 다른 component들과 연결한다. 
ATV TTX의 경우 VBI 구간을 읽어들여 TTX data를 받아들이고, 
DTV TTX의 경우 Application 에서 PES parsing을 통해 얻은 TTX raw data(42byte packet)들을 
SdAuxOut_SendVbiData() 함수를 통해서 TTX 모듈에 전달한다.
 It connects the TTX module to ATV or DTV. In case of ATV, TTX module can get TTX data from VBI area.
 In case of DTV, Application will send TTX raw data(42byte packet) by using SdAuxOut_SendVbiData() after parsing PES packets.
 * @remarks 
 * @param eInputSrc path 설정을 통하여 연결하고자 하는 소스의 ID. Input source ID ATV or DTV
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTTX_Connect(SdSource_k eInputSrc);

/**
 * @brief TTX 입력 path의 설정을 해제하는 함수
 The function to release the settings of TTX input path.
 * @remarks Connect를 통해 연결된 TTX 모듈의 연결을 해제한다.
 It releases the connection of TTX input Components which are connected through the Connect.
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTTX_Disconnect(void);

/**
 * @brief TTX 관련 Status를 읽어온다.
 It reads in the TTX-related Satus.
 * @remarks 
 * @param mask
 * @code
	typedef enum
	{
		SD_TTX_STATUS_AVAILABLE	= 0x01, 
		SD_TTX_STATUS_MAX	
	}
 * @endcode
 * @param pState
 * @code
	typedef struct
	{
		SdBool_t bAvailable; ///< TRUE : TTX is Available, FALSE : TTX is not Available
	} SdTTX_Status_t;
 * @endcode 
 * @param timeout
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTTX_GetStatus(SdMask_t mask, SdTTX_Status_t *pState, uint32_t timeout);

/**
 * @brief TTX 모듈로 user의 리모콘 키 값을 전달 하며 TTX 모듈은 그에 맞는 기능을 수행한다. 
 It send key value of user remote control to TTX module.
 TTX module should execute proper operation.
 * @remarks
 * @param eKeyCmd
  * @code
	typedef enum
	{
		SD_TTX_KEY_MIX  = 0, 
		SD_TTX_KEY_CANCEL,
		SD_TTX_KEY_HOLD,  
		SD_TTX_KEY_SIZE, 
		SD_TTX_KEY_REVEAL,
		SD_TTX_KEY_SUBPAGE,
		SD_TTX_KEY_INDEX,
		SD_TTX_KEY_LISTFLOF,
		SD_TTX_KEY_STORE,
		SD_TTX_KEY_DIGIT1,
		SD_TTX_KEY_DIGIT2,
		SD_TTX_KEY_DIGIT3,
		SD_TTX_KEY_DIGIT4,
		SD_TTX_KEY_DIGIT5,
		SD_TTX_KEY_DIGIT6,
		SD_TTX_KEY_DIGIT7,
		SD_TTX_KEY_DIGIT8,
		SD_TTX_KEY_DIGIT9,
		SD_TTX_KEY_DIGIT0,
		SD_TTX_KEY_CH_UP,
		SD_TTX_KEY_CH_DOWN,
		SD_TTX_KEY_RED,
		SD_TTX_KEY_GREEN,
		SD_TTX_KEY_YELLOW,
		SD_TTX_KEY_BLUE,
	} SdTTX_Key_k;
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTTX_Key_Command(SdTTX_Key_k eKeyCmd);

/**
 * @brief TTX 모듈을 시작한다. 
 It starts TTX module.
 * @remarks
 * @param InitialPageNum Initial page number only for Digital. other case InitialPageNum has invalid value 0xffffffff.
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTTX_Start(uint32_t InitialPageNum);

/**
 * @brief TTX 모듈을 중지한다. 
 It stops TTX module.
 * @remarks
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTTX_Stop(void);

/**
 * @brief TTX Subtitle 모듈을 시작한다. 
 It starts TTX Subtitle module.
 * @remarks
 * @param InitialPageNum
 * @param InitialMagNum
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTTX_SBT_Start(uint32_t InitialPageNum, uint32_t InitialMagNum);

/**
 * @brief TTX Subtitle 모듈을 중지한다. 
 It stops TTX Subtitle module.
 * @remarks
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTTX_SBT_Stop(void);

/**
 * @brief SdTTX API의 Manual Debug 함수
 Manual Debug function of SdTTX API
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdTTX_Debug(void);

#endif //_SDTTX_H_

