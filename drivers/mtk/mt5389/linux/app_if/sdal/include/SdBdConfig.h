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
 * @file SdBdConfig.h
 * @brief Board specific configuration header file
 * @author S/W Platform Lab
 * @date 2006/07/07
 */

#ifndef	_SDBDCONFIG_H_
#define	_SDBDCONFIG_H_

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

typedef enum {
	SD_SERIAL_PORT_1,
	SD_SERIAL_PORT_2,
	SD_SERIAL_PORT_MAX
} SdBdConfig_SerialPort_k; ///< Serial port number

typedef enum {
	SD_SERIAL_FUNC_ANYNET,
	SD_SERIAL_FUNC_DEBUG,
	SD_SERIAL_FUNC_GEMSTAR,
	SD_SERIAL_FUNC_GEMSTARIR,
	SD_SERIAL_FUNC_NONE
} SdBdConfig_SerialFunc_k; ///< Define the function of Serial port

typedef enum {	
	///< External Source plug 
	SD_BDCONFIG_STATUS_PLUG_SOURCE_ALL,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_AV1,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_AV2,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_AV3,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_AV4,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_SVIDEO1,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_SVIDEO2,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_SVIDEO3,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_SVIDEO4,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_COMP1,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_COMP2,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_COMP3,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_COMP4,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_PC1,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_PC2,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_PC3,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_PC4,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_DVI1,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_DVI2,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_DVI3,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_DVI4,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_HDMI1,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_HDMI2,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_HDMI3,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_HDMI4,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_OLINK,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_DVR,
	SD_BDCONFIG_STATUS_PLUG_SOURCE_DLINK,	
	
	///< Headphone plug 
	SD_BDCONFIG_STATUS_PLUG_HEADPHONE_ALL,
	SD_BDCONFIG_STATUS_PLUG_HEADPHONE1,	
	SD_BDCONFIG_STATUS_PLUG_HEADPHONE2,	
	
	///< Pod Fan Error 
	SD_BDCONFIG_STATUS_POD_PAN_ERROR,	
} SdBdconfig_StatusMask_k; ///< Board의 특정 상태값을 얻어오기 위해 사용하는 Mask. Mask for the plug in status of the board

typedef enum {
	SD_BDCONFIG_TARGETCOUNTRY_USA,
	SD_BDCONFIG_TARGETCOUNTRY_KOR,
	SD_BDCONFIG_TARGETCOUNTRY_EU,
	SD_BDCONFIG_TARGETCOUNTRY_MAX
} SdBdConfig_TargetCountry_k; ///< Target Country 

enum
{
    SD_SOURCE_PLUGGED_NONE  = 0x00000000,

    SD_SOURCE_PLUGGED_AV1   = 0x00000001,
    SD_SOURCE_PLUGGED_AV2   = 0x00000002,
    SD_SOURCE_PLUGGED_AV3   = 0x00000004,
    SD_SOURCE_PLUGGED_AV4   = 0x00000008,

    SD_SOURCE_PLUGGED_SV1   = 0x00000010,
    SD_SOURCE_PLUGGED_SV2   = 0x00000020,
    SD_SOURCE_PLUGGED_SV3   = 0x00000040,
    SD_SOURCE_PLUGGED_SV4   = 0x00000080,

    SD_SOURCE_PLUGGED_COMP1 = 0x00000100,
    SD_SOURCE_PLUGGED_COMP2 = 0x00000200,
    SD_SOURCE_PLUGGED_COMP3 = 0x00000400,
    SD_SOURCE_PLUGGED_COMP4 = 0x00000800,

    SD_SOURCE_PLUGGED_PC1   = 0x00001000,
    SD_SOURCE_PLUGGED_PC2   = 0x00002000,
    SD_SOURCE_PLUGGED_PC3   = 0x00004000,
    SD_SOURCE_PLUGGED_PC4   = 0x00008000,

    SD_SOURCE_PLUGGED_DVI1  = 0x00010000,
    SD_SOURCE_PLUGGED_DVI2  = 0x00020000,
    SD_SOURCE_PLUGGED_DVI3  = 0x00040000,
    SD_SOURCE_PLUGGED_DVI4  = 0x00080000,

    SD_SOURCE_PLUGGED_HDMI1 = 0x00100000,
    SD_SOURCE_PLUGGED_HDMI2 = 0x00200000,
    SD_SOURCE_PLUGGED_HDMI3 = 0x00400000,
    SD_SOURCE_PLUGGED_HDMI4 = 0x00800000,
    
    SD_SOURCE_PLUGGED_OLINK = 0x01000000,

    SD_SOURCE_PLUGGED_ALL   = 0x00FFFFFF,
};

enum
{
    SD_HEADPHONE_PLUGGED_NONE  = 0x00000000,

    SD_HEADPHONE_PLUGGED_AV1   = 0x00000001,
    SD_HEADPHONE_PLUGGED_AV2   = 0x00000002,
    
    SD_HEADPHONE_PLUGGED_ALL   = 0x00FFFFFF, 	
};

typedef struct
{
	SdBool_t bPODFanOn;				///< POD Fan에 대한 설정값. a setting for POD Fan.
	SdBool_t bFlashWriteProtectOn;  ///< Flash Write Protect에 대한 설정값. a setting for Flash Write Protect
	SdBdConfig_SerialFunc_k	eSerialFunc[SD_SERIAL_PORT_MAX]; ///< 각각의 Serial에 대한 기능 설정값. a setting for each serial port.
	SdBool_t bI2cStop;				///< I2C Bus Stop Mode flag. True - Stop
} SdBdConfig_Settings_t;


/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
/**
 * @brief 초기화되지 않은 Board Dependent 컴포넌트 및 관련 변수들을 초기화 한다.
 Initializes the SdBdConfig module 
 * @remarks BSP에 포함되지 않은 Board Dependent 모듈을 초기화한다.
 Initialize the board dependent module not supported in BSP
 * @param ePowerState
 * @param eTargetCountry
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdBdConfig_Init(SdPowerState_k ePowerState, SdBdConfig_TargetCountry_k eTargetCountry);

/**
 * @brief Board에 새로운 설정값을 적용시킨다.
 Apply new settings to the Board configuration
 * @remarks 
 * @param pSettings [in] Board에 적용할 새로운 설정값. New settings to the Board
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdBdConfig_Set(SdBdConfig_Settings_t *pSettings);

/**
 * @brief 현재의 Board 설정값을 얻어온다.
 Get the current settings of the Board configuration
 * @remarks 
 * @param pSettings [out] 현재의 Board 설정값. Current settings of the Board.
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdBdConfig_Get(SdBdConfig_Settings_t *pSettings);

/**
 * @brief Standby시에 init되어야 하는 DEVICE을 초기화하는 함수.
 The function which initializes the DEVICE which should be initialized upon standby
 * @remarks
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdBdConfig_InitDeviceForStandby(void);

/**
 * @brief Power On되어야 할 장치를 Reset시키는 함수.
 The function which resets the DEVICE during Power On
 * @remarks
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdBdConfig_ResetPowerOnDevice(void);

/**
 * @brief Power Off이전에 Open되어있는 Device를 모두 Close하는 함수.
 As the function of closing the devices which are opened before turning Power Off, 
 it closes all outputs toward the external.
 * @remarks 외부로 나가는 출력을 모두 차단시킨다.
 Close all the external output port
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdBdConfig_CloseForPowerOff(void);

/**
 * @brief 요청된 항목에 대한 상태값을 읽어오는 함수.
 The function to read in the status value pertaining to the requested item
 * @remarks 
 * @param eStatusRequest [in] 얻어오고자 하는 상태값 항목을 지정. the mask of the status value of the requested item.
 * @param pState	[out] 요청한 항목의 상태값. SdBdconfig_Status_k항목에서 ALL이 들어간 것을 제외하면 SdBool_t형과 동일하게 사용된다.
 [out] the status value. SdBool_t type except ALL in SdBdconfig_Status_k.
 * @param timeout	[in]
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdBdConfig_GetStatus(SdBdconfig_StatusMask_k eStatusRequest, uint32_t* pState, uint32_t timeout);

/**
 * @brief SdBdConfig API의 Manual Debug 함수
 The Manual Debug function of SdBdConfig API.
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdBdConfig_Debug(void);

#endif

