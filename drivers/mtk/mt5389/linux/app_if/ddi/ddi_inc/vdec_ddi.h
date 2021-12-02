/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2006 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file 		vdec_ddi.h
 *
 *  header of Mpeg2 Video Decoder DDI Fuction.
 *
 *  @author		Oh, Kyong-Joo (kjoh@lge.com)
 *  @version	1.0
 *  @date		2007.8.1     porting on GNSS platform
 *  @date		2007.12.27   Y2DTV
 *  @note		Additional information.
 *  @see
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef _VDEC_DDI_H_
#define _VDEC_DDI_H_

/******************************************************************************
    전역 제어 상수(Control Constants)
******************************************************************************/

/******************************************************************************
	#include 파일들 (File Inclusions)
******************************************************************************/
#include <osa_api.h>
#ifndef WIN32
#include "common.h"
#else
#include "common_win32.h"
#endif
#include "video_ddi.h"

/******************************************************************************
 	상수 정의(Constant Definitions)
******************************************************************************/

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

#define HD1920   0
#define HD1280   1
#define SD720    2

#define VDEC_NORMAL_PLAY_SPEED 1000

//callback status mask
#define VDEC_DATA_ERROR  (0x1 << 0)
#define VDEC_PIC_DECODING_ERROR  (0x1 << 1)
#define VDEC_BUF_OVERFLOW  (0x1 << 2)
#define VDEC_BUF_UNDERFLOW  (0x1 << 3)
#define VDEC_FRM_DROP  (0x1 << 4)
#define VDEC_FRM_REPEAT  (0x1 << 5)

/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/

/**
 * VDEC status
 */
typedef enum
{
	VDEC_NOT_INIT	= 0,
	VDEC_INIT ,
	VDEC_PLAYING ,
	VDEC_STOPPED ,
	VDEC_FREEZED
} VDEC_STATE_T;

/**
 * codec type
 */
typedef enum
{
	VDEC_CODEC_NONE	= 0,
	VDEC_CODEC_MPEG1  ,
	VDEC_CODEC_MPEG2  ,
	VDEC_CODEC_MPEG4_PART2  ,
	VDEC_CODEC_H264_MPEG4AVC    ,
	VDEC_CODEC_VC1   ,
	VDEC_CODEC_DIVX
} VDEC_CODEC_T;

/**
 * Drip type ; I frame
 */
typedef enum
{
	VDEC_DRIP_I_FRAME	= 0,
	VDEC_DRIP_NORMAL	= 1
} VDEC_DRIP_TYPE_T;

/**
 * constant about Good video
 */
typedef enum
{
	VDEC_BAD_VIDEO		= 0,
	VDEC_GOOD_VIDEO		= 1
} VDEC_VIDEO_STATUS_T;

/**
 * video의 size, bitrate등을 얻고자 할 경우에 쓰이는데 주로 메모리 MAP관련시에 쓰인다.
 */
typedef enum {
	VDEC_HDR_V_SIZE                    = 0,	/**< vertical size */
	VDEC_HDR_H_SIZE                    = 1,	/**< horizontal size */
	VDEC_HDR_VIDEO_FORMAT              = 2,	/**< video format */
	VDEC_HDR_FRAME_RATE                = 3,	/**< frame rate */
	VDEC_HDR_ASPECT_RATIO              = 4,	/**< aspect ratio */
	VDEC_HDR_BIT_RATE                  = 5,	/**< bit rate*/
	VDEC_HDR_PROGRESSIVE_SEQUENCE      = 6,	/**< progressive sequence */
	VDEC_HDR_PICTURE_TYPE              = 7,	/**< picture type */
	VDEC_HDR_TEMP_REF                  = 8,	/**< temporal reference */
	VDEC_HDR_PICTURE_STRUCT            = 9,	/**< picture reference */
	VDEC_HDR_TOP_FF                    = 10,	/**< top field first */
	VDEC_HDR_REPT_FF                   = 11,	/**< repeat field first */
	VDEC_HDR_PROGRESSIVE_FRAME         = 12,	/**< progressive frame */
	VDEC_HDR_INVALID                   = 13
} VDEC_HDR_T;


/**< hykimfnd 20080906	added */
#if (PLATFORM_TYPE == BCM_PLATFORM)
typedef enum {
    VDEC_FRAMERATE_eUnknown = 0,
    VDEC_FRAMERATE_e23_976,
    VDEC_FRAMERATE_e24,
    VDEC_FRAMERATE_e25,
    VDEC_FRAMERATE_e29_97,
    VDEC_FRAMERATE_e30,
    VDEC_FRAMERATE_e50,
    VDEC_FRAMERATE_e59_94,
    VDEC_FRAMERATE_e60,
    VDEC_FRAMERATE_eMax
} VDEC_FRAMERATE_T;
#endif


/**< hykimfnd 20080816	The vdec isr does not provide the picture type of a frame just decoded for the current nexus version.
                                        When it's avail. Re-define it as void		DDI_VDEC_RegisterDVRCallback( pfnVDECInfoToDVR fnCbFn ); */
#if (PLATFORM_TYPE == BCM_PLATFORM)
typedef void (*pfnVDECInfoToDVR)(UINT32 vdec_ch, UINT32 pic_type);
#else
typedef void (*pfnVDECInfoToDVR) (UINT8 pic_type, UINT8 vdec_ch);
#endif

typedef void (*pfnVDECStatusCB) (UINT32 u4vdec_status, UINT8 vdec_ch);

#if 0
/**
 * userdata의 DCC type.
 * video_api.h에도 정의 됨.
 */
typedef enum
{
	DCC_UNKNOWN       	= 0x0,
	DCC_EIA708        	= 0x1,	/**< EIA-708 Format */
	DCC_DVS157        	= 0x2,	/**< DVS-157 Format */
	DCC_EIA708nDVS157 	= 0x3,	/**< EIA-708 + DVS-157  Format */
	DCC_TEST1         	= 0x6,
	DCC_TEST2         	= 0xa
} DCC_TYPE_STATE;
#endif

/**
 * userdata의 DCC type.
 * VIDEO_API.h에도 정의 됨.
 *
 * @ingroup ka_vdec_types
 */
typedef enum
{
	DCC_UNKNOWN       	= 0x0,
	DCC_EIA708        	= 0x1,	/**< EIA-708 Format */
	DCC_DVS157        	= 0x2,	/**< DVS-157 Format */
	DCC_EIA708_DVS157 	= 0x3,	/**< EIA-708 + DVS-157  Format */
	DCC_TEST1         	= 0x6,
	DCC_TEST2         	= 0xa,
#ifdef REF_ONLY_PACKET
	DCC_EIA608		= 0x8,
#endif
} VDEC_DCC_TYPE_T;



/**
 * VDEC input video DTV format.
 *
 * @see
*/
typedef enum VDEC_DTV_FORMAT
{
	DTV1920X1080		= 0,
	DTV1280X720 		= 1,
	DTV1440X1080		= 2,
	DTV1280X1080		= 3,
#if 0// ATSC에 DVB format을 엎어쓸것인가 추가할 것인가? 검토해야 함.
	DTV720X480			= 4,
	DTV704X480			= 5,
	DTV640X480			= 6,
	DTV544X480			= 7,
	DTV480X480			= 8,
	DTV352X480			= 9,
	DTV352X240			= 10,
	DTV768X480			= 11,
	DTV528X480			= 12,
	NT704X480			= 13,
	DTV_FORMAT_INVALID	= 14
#else
	DTV720X576			= 4,
	DTV704X576			= 5,
	DTV640X576			= 6,
	DTV544X576			= 7,
	DTV480X576			= 8,
	DTV352X576			= 9,
	DTV352X288			= 10,
	DTV768X576			= 11,
	DTV528X576			= 12,
	DTV720X480			= 14,
	DTV704X480			= 15,
	DTV_FORMAT_INVALID	= 16
#endif
} VDEC_DTV_FORMAT_T;

/**
 * AFD Mode Information
 *
 */
typedef enum VDEC_AFD_MODE
{
	VDEC_AFD_4_3_1000		= 0x08,// AFD_43_SAME
	VDEC_AFD_4_3_1001		= 0x09,// AFD_43_TO_43
	VDEC_AFD_4_3_1010		= 0x0a,// AFD_43_TO_169
	VDEC_AFD_4_3_1011		= 0x0b,// AFD_43_TO_149
	VDEC_AFD_4_3_1100		= 0x0c,// AFD_43_TO_RSVD
	VDEC_AFD_4_3_1101		= 0x0d,// AFD_43_TO_43SP149
	VDEC_AFD_4_3_1110		= 0x0e,// AFD_43_TO_169SP149
	VDEC_AFD_4_3_1111		= 0x0f,// AFD_43_TO_SP43
	VDEC_AFD_4_3_INVALID	= 0x00,// AFD_43_INVALID
	VDEC_AFD_16_9_1000		= 0x18,// AFD_169_SAME
	VDEC_AFD_16_9_1001		= 0x19,// AFD_169_TO_43
	VDEC_AFD_16_9_1010		= 0x1a,// AFD_169_TO_169
	VDEC_AFD_16_9_1011		= 0x1b,// AFD_169_TO_149
	VDEC_AFD_16_9_1100		= 0x1c,// AFD_169_TO_RSVD
	VDEC_AFD_16_9_1101		= 0x1d,// AFD_169_TO_43SP149
	VDEC_AFD_16_9_1110		= 0x1e,// AFD_169_TO_169SP149
	VDEC_AFD_16_9_1111		= 0x1f,// AFD_169_TO_SP43
	VDEC_AFD_16_9_INVALID	= 0x10,// AFD_169_INVALID
} VDEC_AFD_MODE_T;

/**< hykimfnd 20080806	Used for DVR. Need to rearrage it */
#if (PLATFORM_TYPE == BCM_PLATFORM)
/**
 * BCM decoder extended status
 *
 * @see
*/
typedef struct tag_VDEC_BCM_EXTENDED_STATUS
{
	BOOLEAN		bMute;
	UINT32		DataReadyCounter;
}VDEC_BCM_EXTENDED_STATUS;

#endif


/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/
/**
 * Initialize VDEC.
 * Create semaphore and thread (process message, process AFD)
 * @return API_OK, API_ERROR when error
 */
DTV_STATUS_T DDI_VDEC_Initialize( void );

/**
 * start VDEC.
 * call GNSS gmPE_Start().
 * @param port [in] deprecated, set 0
 * @param codec [in] video codec to set
 * @return API_OK, API_ERROR when error
 */
DTV_STATUS_T DDI_VDEC_Start( UINT8 port, VDEC_CODEC_T codec);

/**
 * stop VDEC.
 * @param port [in] deprecated, set 0
 * @return API_OK, API_ERROR when error
 * @see gmPE_Stop()
 */
DTV_STATUS_T DDI_VDEC_Stop( UINT8 vPort );

/**
 * Pause VDEC.
 * just do not pass to display, decoding is going on.
 *
 * @param port [in] deprecated, set 0
 * @param frz [in] freeze: 1, melt: 0
 * @return API_OK, API_ERROR when error
 */
DTV_STATUS_T DDI_VDEC_Freeze(	UINT8 port, UINT8 frz );

/**
 * Start still-picture(I-Frame) decoding.
 * MHEG I-frame function use this function.
 *
 * @param port [in] deprecated, set 0
 * @param pDripData [in] ptr in system memory containing I-frame data
 * @param dripSize [in] byte size of I-frame
 * @return API_OK, API_ERROR when error
 */
DTV_STATUS_T DDI_VDEC_StartDripDec( UINT8 vPort, UINT8* dripData, UINT32 dripSize, VDEC_DRIP_TYPE_T dripType );

/**
 * Stop still-picture(I-Frame) decoding.
 * MHEG I-frame function use this function.
 *
 * @param port [in] deprecated, set 0
 * @return API_OK, API_ERROR when error
 */
DTV_STATUS_T DDI_VDEC_StopDripDec( UINT8 vPort );

/**
 * Get MPEG decoding information.
 * V size, H size, frame rate, aspect ratio, progressive.
 *
 * @param port [in] deprecated, set 0
 * @param infonum [in] info. number to get
 * @return information value
 */
SINT32 DDI_VDEC_GetVideoInfo( UINT8 port, VDEC_HDR_T infonum );

/**
 * Get VDEC state.
 * @param port [in] deprecated, set 0
 * @param state [out] VDEC state
 * @return API_OK
 */
DTV_STATUS_T DDI_VDEC_GetState( UINT8 port , VDEC_STATE_T *pState);

/**
 * Synchronize AV.
 * @param port [in] deprecated, set 0
 * @param onOff [in] on off
 * @return API_OK, API_ERROR when error
 */
DTV_STATUS_T DDI_VDEC_SyncAV(UINT8 port , UINT8 onOff);

/**
 * Gets the status of the PTS-STC sync for video and audio.
 *
 * @param port [in] specifies a video port
 * @return  1 if AV sync is on,  else 0
 */
UINT8 DDI_VDEC_GetSyncAVStatus(UINT8 port);

/**
 * return AFD value
 * @param port [in] deprecated, set 0
 * @return AFD value
 * @see VDEC_DecodeAFD()
 */
VDEC_AFD_MODE_T DDI_VDEC_GetAFD(UINT8 port);

/**
 * Is MPEG decoding running or not.
 * @param port [in] deprecated, set 0
 * @return TRUE if running, or FALSE
 */
BOOLEAN DDI_VDEC_IsRunning(UINT8 port);

/**
 * function telling if DTV signal is good or not.
 * It must be called periodically by CM.
 * @param port [in] deprecated, set 0
 * @return TRUE if good, or FALSE
 * @see DDI_VDEC_IsRunning()
 */
BOOLEAN DDI_VDEC_IsDTVSigStatusGood( UINT8 vPort );

/**
 * set good video forcely regardless of decoding state.
 * It is called when EPF mode.
 * @param port [in] deprecated, set 0
 * @param GoodorBad [in] Good/Bad
 * @see DDI_VDEC_IsDTVSigStatusGood()
 */
void DDI_VDEC_SetLatestDTVSigStatus( UINT8 vPort, VDEC_VIDEO_STATUS_T GoodorBad );

/**
 * DDI fuction for starting to gether user data.
 *
 * @param port [in] deprecated, set 0
 * @return DTV_STATUS_T
 * @see DDI_VDEC_StopUserData()
 */
DTV_STATUS_T DDI_VDEC_StartUserData( UINT8 port );

/**
 * DDI fuction for stopping to gether user data.
 *
 * @param port [in] deprecated, set 0
 * @return DTV_STATUS_T
 * @see DDI_VDEC_StartUserData()
 */
DTV_STATUS_T DDI_VDEC_StopUserData( UINT8 port );

void DDI_VDEC_InitCntCheck(void);

/**
 * Sets the decoding speed of the VDEC.
 * The normal decoding speed value for input parameter is 1000.
 * Please use the following value VDEC_MORMAL_PLAY_SPEED when you input the speed value.
 *
 * @param vPort [IN] specifies a video port.
 * @param speed [IN] decoding speed(#define VDEC_NORMAL_PLAY_SPEED 1000)
 * @return  API_OK if success, else API_ERROR
 **/
DTV_STATUS_T DDI_VDEC_SetDecodingSpeed(UINT8 vPort, SINT32 speed);

/**
 * Registers callback function for DVR middleware.
 * DVR middleware need to know the picture type information for current decoding video.
 * Picture_type is as follows.
 * - sequence header
 * - I picture type
 * - P picture type
 * - B picture type
 * After VDEC decodes a frame, notify DVR middleware the picture type(I, P, B) and vdec_ch.
 * After the sequence header is detected, notify DVR middleware the picture type(sequence header) and vdec_ch.
 *
 * @param fnCbFn [IN] callback function
 * @return void
 */
//void DDI_VDEC_RegisterDVRCallback(void* pfnCbFn);
EXTERN void DDI_VDEC_RegisterDVRCallback(pfnVDECInfoToDVR pfnCbFn);

/**
 * Registers callback function of decoding status including error/fifo overflow/underflow/repeat/drop etc.
 *
 * @param fnCbFn [IN] callback function
 * @return void
 */
void DDI_VDEC_RegisterStatusCallback(pfnVDECStatusCB pfnCbFn);

/**
 * DDI fuction to clean bit stream buffer.
 *
 * @param port [in] deprecated, set 0
 * @return DTV_STATUS_T
 */
DTV_STATUS_T DDI_VDEC_CleanBitStreamBuff(void);

/**
 * DDI fuction to get decoder skip / repeat count
 *
 * @param port [in] deprecated, set 0
 * @return DTV_STATUS_T
 */
DTV_STATUS_T DDI_VDEC_GetNotSyncCount(UINT32 *countSkip, UINT32 *countRepeat);

/**
 * DDI fuction to get current PTS value.
 *
 * @param port [in] deprecated, set 0
 * @return DTV_STATUS_T
 */
DTV_STATUS_T DDI_VDEC_GetCurPTS(UINT8 u1Port, UINT32 *pPts_hi, UINT32 *pPts_lo);

/**
 * VDEC debug menu.
 *
 * @param void
 * @return void
 */
void DDI_VDEC_DebugMenu(void);

/**
 * Existence of the DVS157 data.
 *
 * @author
 * @param	void		[IN]
 * @return	BOOLEAN : TRUE if there are DVS157 data
 */
BOOLEAN DDI_VDEC_IsDVS157ExistCnt(void);

/**
 * Existence of the EIA708 data.
 *
 * @author
 * @param	serviceNum	[IN] VDEC_DCC_SERVICE_NUM_T
 * @return	BOOLEAN : TRUE if there are EIA708 data
 */
BOOLEAN DDI_VDEC_IsDCCExistCnt(VDEC_DCC_SERVICE_NUM_T serviceNum);

/**
 * Set 0 on the EIA708 data Existence flag of All Services and DVS157 data Existence flag.
 *
 * @author
 * @param	void		[IN]
 * @return	void
 */
void DDI_VDEC_ResetExistCnt(void);

/**
 * decrease EIA708 data & DVS157 data Existence flag of all services.
 *
 * @author
 * @param	void		[IN]
 * @return	void
 */
void DDI_VDEC_MonitoringDCC(void);

/**
 * 채널 체인지 이후에 dcc type, field에 대한 값을 초기화
 * Vdec_Start 되면서 초기화 하는데도 불구하고 이번 채널의 data가 영향을 줌
 *
 * @author
 * @param	void		[IN]
 * @return	void
 */
 /* skystone_FT_NewJersey */
void DDI_VDEC_ResetAllDCCType( void );


/**
 * reset count value for CC stability
 *
 * @author
 * @param	void
 * @return	void
 */
void DDI_VDEC_InitCntCheck( void );

/**
 * DCC Type Setting.(Unknown, SCTE21(EIA608 or EIA708), SCTE20(DVS157), UI에서 컨트롤.
 *
 * @author
 * @param 	_bDCCType	[IN]	dcc type
 * @return 	void
 */
void DDI_VDEC_SetDCCType( VDEC_DCC_TYPE_T dccType );

/**
 * enable userdata rx
 *
 * 1. prevent the repeat call of this function.
 * 3. If this is the first call, then conintue
 * 4. If not, return.
 * 5. Send USRDATA_EN event to UsrDataRxTask.
 * 6. Initialize all userdata queues variables.
 * 7. clear usrdata temperary buffer.
 * 8. initialize all userdata related hdall registers.
 * 9. start ACC encoder.
 * 10. enable userdata interrupt.
 *
 * @author
 * @param	void	[IN]
 * @return	DTV_STATUS_T
 */
DTV_STATUS_T DDI_VDEC_EnableUserDataRx(void);
/**
 * disable userdata rx
 * reverse order of EnableUserDataRx routine.
 *
 * 1. make pointer of userdata rx routine NULL.
 * 2. check is this the last possible disable call for userdata.
 * 3. if no, return immediately.
 * 4. if yes, continue.
 * 5. send EVENT_USRDATA_DIS to UsrDataRxTask to make ready to terminate
 *	   its usrdata rx operation.
 * 6. disable UserData Interrupt of hdall.
 * 7. stop ACC Encoder operation.
 * 8. initialize all variables for usrdata queues.
 *
 * @author
 * @param	void	[IN]
 * @return	DTV_STATUS_T
 */
DTV_STATUS_T DDI_VDEC_DisableUserDataRx(void);
/**
 * Start send userdata to DCC Engine
 * Obtain the callback for userdata transmission.
 *
 * @author
 * @param	pfnUserDataRx		[IN]	callback function is called whenever the userdata slot is received
 * @param	pfnUserDataOverflow	[IN]	callback function is called whenever the userdata buffer is overflow
 * @return	DTV_STATUS_T
 */
DTV_STATUS_T DDI_VDEC_Start708Rx(
	BOOLEAN	(*pfnUserDataRx)(void),			/* callback function is called whenever the userdata slot is received */
	BOOLEAN	(*pfnUserDataOverflow)(void) 	/* callback function is called whenever the userdata buffer is overflow */
);
/**
 * stop send userdata to DCC engine
 * Delete the callback function pointer for userdata transmission
 *
 * @author
 * @param	void	[IN]
 * @return	DTV_STATUS_T
 */
DTV_STATUS_T DDI_VDEC_Stop708Rx( void );
/**
 * start parsing the 608 in 708
 *
 * @author
 * @param	qid4ACCTx		[IN]	QID for 608Data Sending
 * @param	msgID4Field1	[IN]	Message ID for field 1
 * @param	msgID4Field2	[IN]	Message ID for field 2
 * @param	srcID4ACCTx		[IN]	Message Source ID for Main Picture
 * @return	DTV_STATUS_T
 */
DTV_STATUS_T DDI_VDEC_Start608Parsing(
//	OSA_MSGQ_T	qid4ACCTx, 			// QID for 608Data Sending
	UINT32		msgID4Field1, 		// Message ID for field 1
	UINT32		msgID4Field2,		// Message ID for field 2
	UINT32		srcID4ACCTx		// Message Source ID for  608Data Sending
);

/**
 * stop parsing the 608 in 708
 *
 * @author
 * @param	void	[IN]
 * @return	DTV_STATUS_T
 */
DTV_STATUS_T DDI_VDEC_Stop608Parsing( void );
/**
 * Obtain the stored userdata .
 * This function copy back the received userdata to DCC
 *
 * @author
 * @param	ret_Size	[OUT]	buffer Size
 * @param	buffer		[OUT]	buffer pointer to save data
 * @return	DTV_STATUS_T
 */
DTV_STATUS_T DDI_VDEC_GetUserData(
	UINT16	*pRetSize,
	void	*pBuffer
);

/**< hykimfnd 20080806	Used for DVR. Need to rename it */
#if (PLATFORM_TYPE == BCM_PLATFORM)
DTV_STATUS	DDI_VDEC_Reset( UINT8 vPort );
DTV_STATUS 	DDI_VDEC_GetExtendedStatus4DVR(VDEC_BCM_EXTENDED_STATUS *extendedStatus);
void		DDI_VDEC_RegisterDVRCallback( void *fnCbFn );
void 		DDI_VDEC_UnRegisterDVRCallback(void);

#endif


DTV_STATUS_T DDI_VDEC_GetFrameCount(UINT8 u1Port, UINT32 *pu4FrmCnt);

DTV_STATUS_T DDI_VDEC_GetSyncStatus(UINT8 u1Port, UINT32 *pu4SyncStatus);

/**
 * DDI fuction timer.
 *
 * @param u4Sec, sleep second
 * @param u4NanoSec, sleep nano-second
 * @return DTV_STATUS_T
 */
DTV_STATUS_T DDI_VDEC_NanoSleep(UINT32 u4Sec, UINT32 u4NanoSec);

#endif /*_VDEC_DDI_H_ */
