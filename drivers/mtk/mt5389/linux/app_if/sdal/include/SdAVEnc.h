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
 * @file SdAVEnc.h
 * @brief Encoder Function Define Header File
 * @author S/W Platform Lab
 * @date 2007/03/16
 * @version		
 */
 
#ifndef _SDAVENC_H_
#define _SDAVENC_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "SdType.h"
#include "SdMisc.h"

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
	TYPE DEFINITION
********************************************************************/

typedef	enum  {
	SD_AVENC_STATE_POWERDOWN = 0,
	SD_AVENC_STATE_IDLE,
	SD_AVENC_STATE_STOP,
	SD_AVENC_STATE_PAUSE,
	SD_AVENC_STATE_ENCODE,
	SD_AVENC_STATE_MAX
} SdAVEnc_State_k;

typedef struct
{
	int32_t	videoQuality;		///< Video Quality for encoding
} SdAVEnc_Settings_t;

typedef struct
{	
	SdAVEnc_State_k	eEncoderState;	///< Encoder Current State	
} SdAVEnc_Status_t;

/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/

/**
 * @brief Encoder Device를 초기화하여 사용자가 사용할 수 있도록 한다. Initialize the Encoder Device.
 * @remarks
 * @param ePowerState Setting the power state: either power off->on, off->standby or standby->on. default is off->on.
 The following is the Power state list.\n
 * @code
		typedef enum
		{
			SD_PWROFF_TO_NORMAL,
			SD_PWROFF_TO_STANDBY,
			SD_STANDBY_TO_NORMAL
		} SdPowerState_k;
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult	SdAVEnc_Init(SdPowerState_k ePowerState);

/**
 * @brief	Encoder path를 설정하는 함수. Encoder path connection
 * @remarks	Encoder와 입력 소스를 연결한다. Connect between Encoder and Input Source
 * @remarks 이전의 Disconnect 상태에 따라 입력만 연결 또는 입/출력 모두 연결해야 한다. Connect Input or Input/Output depending on the previous disconnect status.
 * @param	eAVEncId	연결하려는 Encoder ID. Encoder ID to be connected
 * @param	eInputSrc	Encoder에 입력되는 소스명. Input Source
 * @return	SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult	SdAVEnc_Connect(SdAVEncID_k eAVEncId, SdSource_k eInputSrc);

/**
 * @brief	Encoder path의 설정을 해제하는 함수. Release the connection of the Encoder path
 * @remarks Connect를 통해 연결된 Encoder Component의 연결을 해제한다. Release the connection of the Encoder Component by the function SdAVEnc_Connect
 * @param eAVEncId	path 설정을 해제하고자 하는 Encoder ID. Encoder ID to be released
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult	SdAVEnc_Disconnect(SdAVEncID_k eAVEncId);

/**
 * @brief	Encoder가 실제 Encoding 작업을 하도록 한다. Start Encoding
 * @remarks
 * @param eAVEncId	Encoder ID
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult	SdAVEnc_Start(SdAVEncID_k eAVEncId);

/**
 * @brief	Encoder의 Encoding 작업을 중단한다. Stop Encoding
 * @remarks
 * @param eAVEncId	Encoder ID
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult	SdAVEnc_Stop(SdAVEncID_k eAVEncId);

/**
 * @brief	Encoder의 현재 설정 상태를 얻어낸다. Get the Encoder settings
 * @remarks
 * @param eAVEncId	Encoder ID
 * @param pSettings	[out]현재 읽어온 Encoder 관련 설정으로 다음과 같은 정보가 있다. 
 video encoding Quality 정보가 있다.
 [out]The saved Encoder settings. It contains the video encoding Quality information.
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult	SdAVEnc_Get(SdAVEncID_k eAVEncId, SdAVEnc_Settings_t * pSettings);

/**
 * @brief	Encoder 동작을 위해 필요한 설정값을 설정하도록 한다.
 It sets the setting for the Encoder
 * @remarks	설정하려는 값과 SdAVEnc 내부적으로 저장된 값을 비교하여 값이 다른 것만 설정한다.
 따라서 원하는 것만 설정하기 위해서는 SdAVEnc_Get을 이용하여 현재 저장된 값을 얻어서 재설정하려는 것만 값을 변경하여 호출한다.
 It compares pSettins and the saved settings, and sets only the changed valued.
 It is advised that call SdAVEnc_Get first, change only the required settings and then call SdAVEnc_Set.
 * @param eAVEncId	Encoder ID
 * @param pSettings	Encoder에 설정하려는 값으로 다음과 같은 정보가 있다.
 video encoding Quality 정보가 있다.
 The value of Encoder settings to be written. It contains the video encoding Quality information.
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult	SdAVEnc_Set(SdAVEncID_k eAVEncId, SdAVEnc_Settings_t * pSettings);

/**
 * @brief	Encoder의 상태를 얻어온다. Get the status of Encoder
 * @remarks	Encoder와 관련된 상태를 얻어오는 것으로 이 값들을 설정하지는 않는다.
 mask parameter를 이용하여 얻어내기를 원하는 것을 지정할 수 있다.
 mask는 or를 이용하여 여러 값을 구할 수 있다.
 It gets the status and does not set any values.
 Use mask or operation to get multiple status.
 * @param	eAVEncId	Encoder ID
 * @param	mask	상태 정보를 나타내는 구조체에서 구하려는 정보를 나타내는 정보. mask of status information
 * @param	pState	[out]현재 읽어온 Encoder의 상태로 다음과 같은 정보가 있다.
 	Encoder의 현재 State 정보가 있다.
 	[out]The Encoder status. It contains Encoder current status.
 * @param	timeout
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult	SdAVEnc_GetStatus(SdAVEncID_k eAVEncId, SdMask_t mask, SdAVEnc_Status_t * pState, uint32_t timeout);

/**
 * @brief SdAVEnc API의 Manual Debug 함수
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdAVEnc_Debug(void);

#endif //_SDAVENC_H_
