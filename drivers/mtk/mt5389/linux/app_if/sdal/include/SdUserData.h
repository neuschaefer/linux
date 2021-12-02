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
 * @file SdUserData.h
 * @brief Analog/Digital User Data header file
 Caption은 기본적으로 디지털 채널의 경우 EIA-708, 아날로그 채널(line 21)의 경우 EIA 608B의 스팩을 따른다.
 SdUserData에서는 아날로그와 디지털 Caption을 통합하여 모두 처리가능토록 한다. 
 
 Caption spec follows EIA-708 for Digital TV and EIA-608B for Analog TV (line 21).
 SdUserData integrates both analog caption and digital caption.
 * @author S/W Platform Lab
 * @date 2007/03/01
 */

#ifndef	_SDUSERDATA_H_
#define	_SDUSERDATA_H_

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
	SD_USER_DATA_TYPE_DIGITAL,	///< UserData Digital
	SD_USER_DATA_TYPE_ANALOG,	///< UserData Analog	
} SdUserData_Type_k;		///< SdUserData의 Digital, Analog를 식별하기 위한 enum. Enum for Digital/Analog identifier

typedef enum
{
	SD_USER_DATA_SLICE_EXTRA_INFO	= 0x01,
	SD_USER_DATA_PICTURE_EXTRA_INFO	= 0x02,
	SD_USER_DATA_PICTURE_USER_DATA	= 0x04,
	SD_USER_DATA_GOP_USER_DATA		= 0x08,
	SD_USER_DATA_SEQUENCE_USER_DATA	= 0x10,
	SD_USER_DATA_AFD				= 0x20,			///< Digital Active Format Descriptor	
} SdUserData_DataType_k;	///< UserData Digital인 경우 CallBack을 수행시 올라오는 Data Type 정보. In case of Digital UserData, this Data Type comes with Callback.

typedef enum
{
	SD_USER_DATA_CAPTION			= 0x01,
	SD_USER_DATA_RATING				= 0x02,		
	SD_USER_DATA_GEMSTAR_2X			= 0x04,		///< Analog SdUserData is Gemstar 2X
	SD_USER_DATA_GEMSTAR_FIELD_INFO	= 0x08,
	SD_USER_DATA_TTX				= 0x10,		///< Analog SdUserData is Teletext
	SD_USER_DATA_WSS				= 0x20,		///< Analog SdUserData is WSS
	SD_USER_DATA_VPS				= 0x40,		///< Analog SdUserData is VPS - included in NTSC line 21 XDS	
	SD_USER_DATA_PDC				= 0x80, 	///< Analog SdUserData is PDC
} SdUserData_AnalogDataType_k;	///< SdUserData가 Analog 인 경우 UserData Analog 종류를 식별하기 위한 enum. SdUserData_EnableVbiLine()호출시 전달받고자 하는 Data Type을 입력한다. enum for Analog UserData type identifier. Set by user when calling SdUserData_EnableVbiLine()

typedef struct
{
	SdUserData_DataType_k eDigital;	
	uint8_t picture_structure;		///<Reserved when 0, Top Field when 1, Bottom Field when 2, and Frame picture when 3.		
	uint8_t top_field_first;		///<Whether Top Field First method is used.	
	uint16_t temporal_reference;	///<value that increases by 1 for each frame (like time stamp)
	uint32_t pts;					///<Intended time of PTS Window and Sound presentation.	
} SdUserData_Digtal_t;

typedef struct
{
	SdUserData_AnalogDataType_k eAnalog;	
	uint16_t gemstar_temporal_reference;	///<value that increases by 1 for each frame (like time stamp) ==> In case of GemStar, have to declare whenever a frame starts. Only works for GemStar.	
	uint32_t vbi_line;   			///<Source VBI Line Number, OCAP enables even and odd at same time.
} SdUserData_Analog_t;

typedef struct
{
	SdUserData_Type_k eType;			///<  SdUserData's Digital, Analog Identifier
	union {		
		SdUserData_Digtal_t sDigital;
		SdUserData_Analog_t sAnalog;
	} uData;								///< Use one structure depending on eType	
} SdUserData_DataInfo_t;	

/**
 * @brief UserData 처리를 위한 Callback함수 Type. Function prototype of Callback function for handling UesrData
 * @remarks SdUserData_InstallCallback()을 통해 등록하여 사용하고, Picture Header를 통해 UserData가 들어올때마다 호출된다.
 UserData의 Endian은 Big Endian, 순서는 Display order를 따른다. Digital Type의 UserData에서는 Picture User Data Syntax에서 user_data_start_code(0x0000 01B2)이후의 
 모든 Data가 pData를 통해 전달된다. Endian을 SDAL에서 변경해야 될 경우 SdMisc의 SdUserdata_ChangeEndian()를 사용한다.
 
 UserData callback function is registered through SdUserData_InstallCallback().
 It assumes UserData Endian is Big-Endian and the order is Display-order.
 For Digital UserData all the data following user_data_start_code(0x0000 01B2, explained in Picture User Data Syntax) is stored in pData.
 In case of changing Little-Endian to Big-Endian, Use the Sample function, SdUserdata_ChangeEndian(), implemented in SdMisc.h
 
 * @param pInfo 수집된 UserData에 대한 정보. Information for the collected UserData
 * @param pData 수집된 UserData가 저장된 버퍼의 주소. The buffer address of UserData
 * @param length 수집된 UserData의 크기(< 128byte). sizer of the buffer
 * @param uwParam Callback 등록시 입력한 user parameter. the user parameter of the callback
 * @see 
 */
typedef void (*SdUserData_CallBack)(SdUserData_DataInfo_t *pInfo, uint8_t *pData, uint8_t length, uint32_t uwParam);


/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/

/**
 * @brief User Data를 상위 모듈에게 전달하는 Callback 함수를 등록한다.
 It registers the Callback function which delivers the User Data to the upper module.
 * @remarks SdUserData_InstallCallback을 사용하기 위해, SdUserData_CallBack Type과 동일하게 상위 Layer에서 CallBack 함수를 정의하여 사용 한다. 
  In order to use the SdUserData_InstallCallback, it should be pre-defined for the SdUserData_CallBack function in the upper Layer.
  * @param callback 등록시키고자 하는 SdUserData_CallBack타입의 함수포인터로서 하기와 같이 정의 된다. 
  pInfo:UserData 정보, pData : 실제 UserData 내용, length : UserData 길이 
 Function pointer of SdUserData callback. Its parameters includes pInfo: UserData Information, pData: contents of UesrData, length: length of UserData
 * @code  
 	typedef void (*SdUserData_CallBack)(SdUserData_DataInfo_t *pInfo, uint8_t *pData, uint8_t length, uint32_t uwParam);
 * @endcode	
 * @code
 //< 상위 Layer에서 SdUserData_CallBack 함수에 대해 미리정의한 부분. SdUserData_CallBack defined in an upper layer
 	 void SsTvInfo::CaptionCallback(SdUserData_DataInfo_t* pInfo, unsigned char* pData, unsigned char length, unsigned int  callerParam)
	{
		m_CaptionMutex.Lock();   							 ///<  m_CaptionMutex : PCMutex class provides functions used to create and manage mutexes.
		m_CaptionCallback(pInfo, pData, length, callerParam); 	 ///<  Callbck 함수를 처리하는 SsTvInfo의 Static 함수. static function in SsTvInfo which handles callback
		m_CaptionMutex.Unlock();
	}

 //< 상위 Layer에서 SdUserData_InstallCallback을 호출하는 예. Example of calling SdUserData_InstallCallback in an upper layer
	if(SdUserData_InstallCallback(SsTvInfo::m_CaptionCallback, (unsigned int)this != SD_OK)) ///< Example usage of SdUserData_InstallCallback
	{
		SSERR_PRINT(" SdUserData_Install error !!\n");
		return SS_NOT_OK;
	}
 * @endcode
 * @param uwParam 
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdUserData_InstallCallback(SdUserData_CallBack callback, uint32_t uwParam);

/**
 * @brief User Data 처리 모듈을 초기화 한다.
 It initializes the module of processing the User Data.
 * @remarks SdUserData_InstallCallback를 사용하기 전에 호출해야한다.
 This must be called before SdUserData_InstallCallback()
 * @code
		SdUserData_Init();
		if(SdUserData_InstallCallback(SsTvInfo::m_CaptionCallback, (unsigned int)this != SD_OK))
		{
			SSERR_PRINT(" SdUserData_Install error !!\n");
			return SS_NOT_OK;			
		}
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdUserData_Init(void);

/**
 * @brief Analog UserData의 경우, SdUserData_Start 수행후, 원하는 VBI Data 수신을 위해서 해당 VBI Line을 설정한다.
 그리고, Data를 받게 되는 Line 별로 Gemstar, Caption 등의 DataType이 지정된다.
 For the Analog UserData, set the VBI Line to which the VBI Data are to be received and the type of information to get received.  
 * Closed Caption : Line 21, NTSC, 2Byte data
 * CGMS : Line 21에서 데이터를 받을수 있으며, CGMS는 필요한 경우 Misc에서 구현함. It may collected in Line 21 and is implemented in Misc.
 * Gemstar 1x :Line 21, 2Byte data.  Offline에서 Data를 받게 됨. Process the data in Offline
 * Gemstar 2x :Line 10~25, 4Byte data. Offline에서 Data를 받게 됨. Process the data in Offline
 * Teletex : Line 10~21 , NTSC
 * WSS : Line 20, NTSC
 * @remarks Caption의 경우 NTSC 기준으로 21번째 Line 값은 odd field, 284 번째 Line은 even field 이다. 
 On the basis of NTSC, the 21st line value is odd field and the 284th line is even field.
 * @param eInput
 * @param Startline 수신설정하고자 하는 시작 VBI Line 번호.
 * @param Endline 수신설정하고자 하는 마지막 VBI Line 번호. 설정하고자 하는 VBI Line의 수가 하나인 경우 Startline과 Endline의 값은 같다.
 * @param AnalogDataTypes : 상위 Layer에서 SdUserData를 사용시 Analog 종류에 대한 정보를 넘겨 줌. Analog type passing to the upper layer
 * @code 
		 if(Line >=  VBI_EVEN_START && Line <=  VBI_EVEN_END)
		{
			Line -= VBI_EVEN_START;		///< even field			
		}
		else
		{
			if(Line >=  VBI_ODD_START && Line <=  VBI_ODD_END)
			{
				Line -= VBI_ODD_START;	///< odd field
			}
		}
 * @endcode			
 * @return SdResult SD_OK/SD_NOT_OK
 * @see SdUserData_DisableVbiLine(), SdUserData_Start()
 */
SdResult SdUserData_EnableVbiLine(SdBackEndInID_k eInput, uint16_t Startline, uint16_t Endline, SdMask_t AnalogDataTypes);

/**
 * @brief Analog UserData에서 VBI Data 수신을 중지하고자 하는 VBI Line을 설정한다.
 For Analog UserData, It sets the VBI Line to which the VBI Data are to be received. 
 * @remarks Gemstar Test 수행시에도 사용된다.
 It is used upon conducting the Gemstar Test.
 * @param eInput
 * @param Startline 수신중지하고자 하는 시작 VBI Line 번호.
 * @param Endline 수신중지하고자 하는 마지막 VBI Line 번호. 수신중지하고자 하는 VBI Line의 수가 하나인 경우 Startline과 Endline의 값은 같다.
 * @return SdResult SD_OK/SD_NOT_OK
 * @see SdUserData_EnableVbiLine
 */
SdResult SdUserData_DisableVbiLine(SdBackEndInID_k eInput, uint16_t Startline, uint16_t Endline);

/**
 * @brief  User Data 처리를 시작한다.
 It starts the User Data process.
 * @remarks
 * @code 
 // 상위 Layer에서 SdUserData_Start의 사용예. Usage of SdUserData_Start in an upper layer
 	if ((udType == TPCWindow::USER_DATA_MODE_ANALOG))							///< Analog Catption
	{
		SdUserData_Start((SdBackEndInID_k)eInputID);
		SdUserData_EnableVbiLine(SD_BE_IN_AVDEC0,line, SD_USER_DATA_CAPTION);   	
		bDigitalCaption = FALSE;	 
	}
	if ((udType == TPCWindow::USER_DATA_MODE_DIGITAL)&&(bDigitalCaption == FALSE))	///< Digital Catption 
	{
		SdUserData_Start((SdBackEndInID_k)eInputID);
		bDigitalCaption = TRUE;
	}
 * @endcode	
 * @param eInput
 * @return SdResult SD_OK/SD_NOT_OK
 * @see
 */
SdResult SdUserData_Start(SdBackEndInID_k eInput);

/**
 * @brief User Data 처리를 중지한다.
 It stops the User Data process.
 * @remarks
 * @param eInput
 * @return SdResult SD_OK/SD_NOT_OK
 * @see
 */
SdResult SdUserData_Stop(SdBackEndInID_k eInput);

/**
 * @brief Digital AFD의 수신을 허용한다. Set AFD to be received.
 * @remarks AFD를 제외한 나머지 Digital UserData들은 항상 수신 가능 상태이다. AFD는 초기값이 수신 불가이기 때문에 
 이를 수신하기 위해서는 SdUserData_EnableDigitalAFD()를 호출한다. Basically, all Digital User Data can be received except for AFD.
 In order to receive AFD, this Api should be called.
 * @param eInput
 * @return SdResult SD_OK/SD_NOT_OK
 * @see
 */
SdResult SdUserData_EnableDigitalAFD(SdBackEndInID_k eInput);

/**
 * @brief Digital AFD의 수신 허용을 중지한다. Set AFD not to be received.
 * @remarks AFD의 초기 수신 모드는 수신 불가이다.
 * @param eInput
 * @return SdResult SD_OK/SD_NOT_OK
 * @see
 */
SdResult SdUserData_DisableDigitalAFD(SdBackEndInID_k eInput);

/**
 * @brief SdUserData API의 Manual Debug 함수
 Manual Debug function of SdUserData API
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdUserData_Debug(void);

#endif
