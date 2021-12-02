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
 * @file SdVideo.h
 * @brief Video header file
 * @author S/W Platform Lab
 * @date 2006/07/10
 */

#ifndef	_SDVIDEO_H_
#define	_SDVIDEO_H_

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
	
typedef struct
{
	uint32_t hResolution;	///< Horizontal Size for Scaling
	uint32_t vResolution;	///< Vertical Size for Scaling
	uint32_t hStart;	///< horizontal active resolution start point
	uint32_t vStart;	///< vertical active resolution start point
	uint32_t hTotal;	///< total horizontal resolution
	uint32_t vTotal;	///< total vertical resolution
	SdBool_t bProgressScan;	///< progress / interace identifier
	uint32_t frameRate;	///< Frame rate (Hz * 1000)
} SdVideo_VideoFormat_t;

typedef struct
{
	SdVideo_VideoFormat_t sCaptureSize;		///<scaler output capture size
	SdGeometry_t sGeometry;		///<geometry values(16:9,4:3,...)
	SdColor_t sBGColor;	///< BackGround Color(각 Plane에서 Video Mute시 보여지는 Color, Color when the plane is mute)
	SdBool_t bVideoOn; 			///< Pip on/off
	SdBool_t bEnable32PullDown; 	///< 480i 신호에서 Film mode를 검출하고, 3:2 Pull Down상태를 복원하여 출력하는 기능 On/Off(보통 IPC 모듈에서 처리됨). Film mode on/off in 480i signal. If On, it detects film mode and enables 3:2 Pull Down mode (Generally processed in IPC module)
	SdBool_t bGameModeOn;		///< Game Mode Function On/Off.
	SdBool_t b3DGameModeOn;		///< 3D Game Mode Function On/Off.		
	uint32_t VideoDelay;		///< video output delay(ms)	
	SdColorFormat_k eColorFormat; 	///< scaler Input Color Format.
} SdVideo_Settings_t;

typedef struct
{
	uint64_t FrameSize; 	///<Capture Frame Size(byte)
	uint32_t Height;		///<Capture Frame Height 
	uint32_t Width;			///<Capture Frame Width
	SdColorFormat_k eFormat;///<Capture Frame Video Format 
	uint32_t Pts;			///<Capture Frame PTS
} SdVideo_CaptureInfo_t;

/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/

/**
 * @brief Video 모듈에 대한 최초의 초기화 과정을 수행한다. 
 It carries out the first initialization prodecure for the Video module.
 * @remarks
 * @param ePowerState power state
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdVideo_Init(SdPowerState_k ePowerState);

/**
 * @brief Video(Scaler)의 Setting 함수. Resolution, VideoSize 및 Video와 관련된 정보들을 Setting한다. 
 It is the function to set the Video(Scaler). It sets the Resolution, VideoSize and Video-related information.
 * @remarks 설정하려는 값과 SdVideo에 이미 설정되어 있는 값을 비교하여 값이 다른 것만 설정한다.
 따라서 원하는 것만 설정하기 위해서는 SdExtIn_Get을 이용하여 현재 저장된 값을 얻어서 재설정하려는 것만 값을 변경하여 호출한다.
 By comparing the values to be set and the values which have been set in SdVideo, only different value will be set. 
 Therefore, in order to set the desired ones, by using the SdExtIn_Get, get the currently-saved value and then cange and call up only the values which you want to re-set.
 * @param eVideoId Video Component ID
 * @code
		 typedef enum
		{
			SD_VIDEO0, 
			SD_VIDEO1,
			SD_VIDEO_MAX
		} SdVideoID_k;
 * @endcode
 * @param pSettings setting data
 * @code
 		typedef struct
		{
			uint32_t hResolution;	///< Horizontal Size for Scaling
			uint32_t vResolution;	///< Vertical Size for Scaling
			uint32_t hStart;	///< horizontal active resolution start point
			uint32_t vStart;	///< vertical active resolution start point
			uint32_t hTotal;	///< total horizontal resolution
			uint32_t vTotal;	///< total vertical resolution
			SdBool_t bProgressScan;	///< progress / interace identifier
			uint32_t frameRate;	///< Frame rate (Hz * 1000)
		} SdVideo_VideoFormat_t;
		
		typedef struct
		{
			SdVideo_VideoFormat_t sCaptureSize;		///<scaler output capture size
			SdGeometry_t sGeometry;		///<geometry values(16:9,4:3,...)
			SdColor_t sBGColor;			///< BackGround Color (letter box color)
			SdBool_t bVideoOn; 			///< scaler on/off
			SdBool_t bEnable32PullDown; 	///< film mode
		} SdVideo_Settings_t;
 * @endcode 
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdVideo_Set(SdVideoID_k eVideoId, SdVideo_Settings_t* pSettings);

/**
 * @brief Video 모듈의 정보를 가져온다. Resolution, VideoSize 및 Video와 관련하여 저장된 정보들을 가져온다.
 It takes the information of Video module. It takes the information which is saved in regard with the Resolution, VideoSize and Video.
 * @remarks
 * @param eVideoId Video Component ID
 * @param pSettings [out]setting data
 * @code
		typedef struct
		{
			uint32_t hResolution;	///< Horizontal Size for Scaling
			uint32_t vResolution;	///< Vertical Size for Scaling
			uint32_t hStart;	///< horizontal active resolution start point
			uint32_t vStart;	///< vertical active resolution start point
			uint32_t hTotal;	///< total horizontal resolution
			uint32_t vTotal;	///< total vertical resolution
			SdBool_t bProgressScan;	///< progress / interace identifier
			uint32_t frameRate;	///< Frame rate (Hz * 1000)
		} SdVideo_VideoFormat_t;
 
		typedef struct
		{
			SdVideo_VideoFormat_t sCaptureSize;		///<scaler output capture size
			SdGeometry_t sGeometry;		///<geometry values(16:9,4:3,...)
			SdColor_t sBGColor;			///< BackGround Color (letter box color)
			SdBool_t bVideoOn; 			///< scaler on/off
			SdBool_t bEnable32PullDown; 	///< film mode
		} SdVideo_Settings_t;
 * @endcode 
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdVideo_Get(SdVideoID_k eVideoId, SdVideo_Settings_t* pSettings);

/**
 * @brief Video Component를 AVDec 또는 ExtIn과 연결시키는 함수 
 The function to connect the Video Component to AVDec or ExtIn.
 * @remarks TS 입력의 경우 AVDec을 거쳐 Video와 연결되고 나머지는 ExtIn을 거쳐서 Video로 들어온다.
 For the TS input, it is connected to Video through the AVDec and the rest will come in to Video through the ExtIn.
 * @code
		SdVideo_Settings_t sVideoSettings;
		SdExtIn_Status_t sExtInStatus;
		...
		SdExtIn_Connect(SD_EXTIN0, SOURCE_COMP1, SD_VIDEO0);
		SdVideo_Connect(SD_VIDEO0, SD_BE_IN_EXTIN0, SD_WINDOW_MAIN);
 * @endcode
 * @param eVideoId 연결하고자 하는 Video Component ID. Video Component ID to be connected
 * @code
		 typedef enum
		{
			SD_VIDEO0, 
			SD_VIDEO1,
			SD_VIDEO_MAX
		} SdVideoID_k;
 * @endcode
 * @param eInputId Video Component의 입력과 연결되는 Component의 ID. Input Component ID to be connected to Video Component
 * @code
 		typedef enum
		{
			SD_VIDEO_IN_AVDEC0,	///< Decoder0
			SD_VIDEO_IN_AVDEC1,	///< Decoder1
			SD_VIDEO_IN_EXTIN0,	///< External Input0
			SD_VIDEO_IN_EXTIN1	///< External input1
		} SdVideoInID_k; 
 * @endcode
 * @param eOutputId 연결된 Video가 Display될 Window ID. Window ID to be displayed by the Video Component
 * @code
		typedef enum
		{
			SD_WINDOW_MAIN,
			SD_WINDOW_SUB0,
			SD_STREAM_TSOUT,
			SD_STREAM_RECORD0,
			SD_STREAM_RECORD1,	
			SD_WINDOW_MAX,
		} SdWindowID_k;
 * @endcode
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdVideo_Connect(SdVideoID_k eVideoId, SdBackEndInID_k eInputId, SdWindowID_k eOutputId);

/**
 * @brief Video Component와 AVDec 또는 ExtIn과의 연결을 해제하는 함수 
 The function to release the connection between Video Component and either AVDec or ExtIn.
 * @remarks
 * @param eVideoId 연결을 해제 하고자 하는 Video Component ID. Video Component ID to be disconnected
 * @code
		 typedef enum
		{
			SD_VIDEO0, 
			SD_VIDEO1,
			SD_VIDEO_MAX
		} SdVideoID_k;
 * @endcode
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdVideo_Disconnect(SdVideoID_k eVideoId);

/**
 * @brief Video을 Mute시킨다.
 It makes Video muted. 
 * @remarks 화면 Mute시 보여지는 Background Color는 SdVideo_Set을 통해 미리 결정한다.
            Background Color(sBGColor) should be set before SdVideo_Mute is called. 
 * @param eVideoId Video Component ID
 * @code
		 typedef enum
		{
			SD_VIDEO0, 
			SD_VIDEO1,
			SD_VIDEO_MAX
		} SdVideoID_k;
		
		typedef struct
		{
			SdVideo_VideoFormat_t sCaptureSize;		///<scaler output capture size
			SdGeometry_t sGeometry;		///<geometry values(16:9,4:3,...)
			SdColor_t sBGColor;	///< BackGround Color(각 Plane에서 Video Mute시 보여지는 Color, Color when the plane is mute)
			SdBool_t bVideoOn; 			///< Pip on/off
			SdBool_t bEnable32PullDown; 	///< 480i 신호에서 Film mode를 검출하고, 3:2 Pull Down상태를 복원하여 출력하는 기능 On/Off(보통 IPC 모듈에서 처리됨). Film mode on/off in 480i signal. If On, it detects film mode and enables 3:2 Pull Down mode (Generally processed in IPC module)
			SdBool_t bIpcOn;		///< IPC(Interaced신호를 Progressive신호로 변환시켜주는 기능) On/Off. Interaced to Progressive Conversion(IPC) On/Off
			uint32_t VideoDelay;		///< video output delay(ms)	
		} SdVideo_Settings_t;
 * @endcode
 * @param bMuteOn mute On/Off. 
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdVideo_Mute(SdVideoID_k eVideoId, SdBool_t bMuteOn);

/**
 * @brief PC AutoAdjust동작시 Frequency(Coarse)를 Detect한다.
 It detect coarse, while doing autoadjustment.
 * @remarks 
 * @param eVideoId Video Component ID
 * @param uPll Horizontal Total size
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdVideo_DetectAutoCoarse(SdVideoID_k eVideoId, uint32_t *uPll);

/**
 * @brief PC AutoAdjust동작시 Phase(Fine)를 Detect한다.
 It detect Fine, while doing autoadjustment.
 * @remarks
 * @param eVideoId Video Component ID
 * @param uPhase Phase
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdVideo_DetectAutoFine(SdVideoID_k eVideoId, uint32_t *uPhase);

/**
 * @brief PC AutoAdjust동작시 x, y Postion을 구한다.
 It get the position of x,y, while dong autoadjustment.
 * @remarks
 * @param eVideoId Video Component ID
 * @param pPostition
 * @return SD_OK/SD_NOT_OK
 * @see 
 */SdResult SdVideo_DetectAutoPostion(SdVideoID_k eVideoId, SdGeometry_t *pPostition);

/**
 * @brief 외부 입력 중 PC input이 Video에 연결된 경우 PC의 화면을 Video 화면에 자동으로 맞추도록 Auto Adjustment를 수행한다.
 frequency,phase,H/V position을 조절한다.
 It performs the Auto Adjustment so that the PC screen can be adjusted to the Video screen automatically 
 when a PC input is connected to Video while external input is going on.
 * @remarks 보통 AutoAdjustment는 ExtIn과 Video가 상호 작용을 하면서 수행되는데, 이 함수는 하나의 Video 모듈에서 모든 AutoAdjust를
 수행하는 경우 호출된다.
 Generally, AutoAdjust is performed by the interaction between ExtIn and Video components.
 This function is used when only Video Component performs AutoAdjust.
 * @code
		...
		SdVideo_DoAutoAdjust(sPathInfo.eVideoID, &sAutoAdjustInfo);
 * @endcode
 * @param eVideoId Video Component ID
 * @code
		 typedef enum
		{
			SD_VIDEO0, 
			SD_VIDEO1,
			SD_VIDEO_MAX
		} SdVideoID_k;
 * @endcode
 * @param pPcData [out] frequency, phase, geometry: the result of AutoAdjust
 * @code
 		typedef struct
		{
			uint32_t x;
			uint32_t y;
			uint32_t w;
			uint32_t h;
		} SdGeometry_t;
		
		typedef struct
		{
			uint32_t frequency;
			uint32_t phase;
			SdGeometry_t sPCPosition;
		} SdPCAdjustInfo_t;
 * @endcode 
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdVideo_DoAutoAdjust(SdVideoID_k eVideoId, SdPCAdjustInfo_t* pPcData);

/**
 * @brief Stop doing AutoAdjustment. AutoAdjust 작업을 중지한다.
 * @remarks
 * @param eVideoId Video Component ID
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdVideo_StopAutoAdjust(SdVideoID_k eVideoId);

/**
 * @brief Still(freeze) a video picture
 * @remarks
 * @param eVideoId Video Component ID
 * @code
		 typedef enum
		{
			SD_VIDEO0, 
			SD_VIDEO1,
			SD_VIDEO_MAX
		} SdVideoID_k;
 * @endcode
 * @param bStillOn still on/off
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdVideo_StillVideo(SdVideoID_k eVideoId, SdBool_t bStillOn);

/**
 * @brief 화면을 Capture한다.
 It captures a Frame Picture.
 * @remarks 
 * @param eVideoId Video Component ID
 * @param pDestination Capture된 Frame을 저장할 메모리 주소. address for storing the captured frame
 * @param pCaptureInfo Capture된 Frame에 대한 정보. information of the captured frame
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdVideo_CaptureFrame(SdVideoID_k eVideoId, uint8_t* pDestination, SdVideo_CaptureInfo_t *pCaptureInfo);

/**
 * @brief SdVideo API의 Manual Debug 함수
 Manual Debug function of SdVideo API
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdVideo_Debug(void);

#endif //_SDVIDEO_H_

