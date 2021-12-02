/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2006 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file emp_ddi.h
 *
 *  Header file of EMP DDI for GNSS platform
 *
 *  @author	Oh kyongjoo (kjoh@lge.com)
 *  @version	0.0
 *  @date		2008.02.12
 */


/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef _EMP_DDI_H_
#define _EMP_DDI_H_

#include "common.h"

/******************************************************************************
    전역 제어 상수(Control Constants)
******************************************************************************/

/******************************************************************************
	#include 파일들 (File Inclusions)
******************************************************************************/

/******************************************************************************
 	상수 정의(Constant Definitions)
******************************************************************************/
#define _LG_SCALER_ENV_	1

#define EMP_HORIZONTAL_ACTIVE_PXL		1920
#define EMP_VERTICAL_ACTIVE_LINES		1080

#define EMP_FRAME_BUF_LUMA_WIDTH		2048
#define EMP_FRAME_BUF_LUMA_HEIGHT		1088

#if (PLATFORM_TYPE == MSTAR_PLATFORM)
	#define JPEG_REGISTER_CALLBACK
//	#define AUTO_FLUSH_FRAMEBUFFER
#elif (PLATFORM_TYPE == BCM_PLATFORM)
	#define JPEG_REGISTER_CALLBACK
//	#define AUTO_FLUSH_FRAMEBUFFER
#endif

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/

/**
 * Multimedia type; audio, video, image
 * see alse : UI_EMF_TYPE_T in 'ui_emf_main.h' ( number should be same each other )
 */
typedef enum
{
	EMP_MEDIA_TYPE_IMAGE = 0,
	EMP_MEDIA_TYPE_AUDIO = 1,
	EMP_MEDIA_TYPE_VIDEO = 2,
	EMP_MEDIA_TYPE_UNKNOWN,
} EMP_MEDIA_TYPE_T;

/**
 * Pixel format
 */
typedef enum
{
    EMP_PIXELFMT_RGB888,
    EMP_PIXELFMT_RGB555,
    EMP_PIXELFMT_YUV420,
    EMP_PIXELFMT_YUV422,
    EMP_PIXELFMT_YUV444
} EMP_PIXEL_FORMAT_T;

/**
 * Rotation angle
 */
typedef enum
{
    EMP_ROTATION_0,
    EMP_ROTATION_90,
    EMP_ROTATION_180,
    EMP_ROTATION_270
}  EMP_ROTATION_T;

/**
 * Multimedia file information
 */
typedef enum
{
    EMP_METADATA_TITLE,
    EMP_METADATA_ALBUM,
    EMP_METADATA_ARTIST,
    EMP_METADATA_GENRE,
    EMP_METADATA_SAMPL_FREQ,
    EMP_METADATA_BITRATE,
    EMP_METADATA_FRAMESIZE,
    EMP_METADATA_PLAYTIME,
    EMP_METADATA_FRAMERATE,
    EMP_METADATA_CAM_MAKER,
    EMP_METADATA_CAM_MODEL,
    EMP_METADATA_CREATION_DATE,
    EMP_METADATA_HRES,
    EMP_METADATA_VRES,
    EMP_METADATA_BPP,
    EMP_METADATA_IMAGETYPE,
    EMP_METADATA_PAR_WIDTH,
    EMP_METADATA_PAR_HEIGHT,
    EMP_METADATA_AUDIOCODEC
} EMP_METADATA_T;

/**
 * Event message; play done message or error message
 */
typedef enum
{
   EMP_MSG_PLAYDONE = 1,
   EMP_MSG_PLAYERR,
   EMP_MSG_ELAPSEDTIME
} EMP_MSG_T;

typedef void* EMP_MEDIA_ITEM_T;

typedef void (*pfnEMPCallback)(EMP_MEDIA_ITEM_T item, EMP_MSG_T msg, UINT32 data);

/**
 * Trick play direction
 */
typedef enum
{
	EMP_TRICK_DIRECTION_FORWARD,
	EMP_TRICK_DIRECTION_BACKWARD
} EMP_TRICK_DIRECTION_T;

/**
 * Trick play speed
 */
typedef enum
{
	EMP_TRICK_SPEED_DEFAULT,
	EMP_TRICK_SPEED_SLOW_2X,
	EMP_TRICK_SPEED_SLOW_3X,
	EMP_TRICK_SPEED_SLOW_4X,
	EMP_TRICK_SPEED_SLOW_8X,
	EMP_TRICK_SPEED_SLOW_16X,
	EMP_TRICK_SPEED_SLOW_32X,
	EMP_TRICK_SPEED_FAST_2X,
	EMP_TRICK_SPEED_FAST_3X,
	EMP_TRICK_SPEED_FAST_4X,
	EMP_TRICK_SPEED_FAST_8X,
	EMP_TRICK_SPEED_FAST_16X,
	EMP_TRICK_SPEED_FAST_32X
} EMP_TRICK_SPEED_T;

/**
 * Trick mode parameters
 */
typedef struct
{
	EMP_TRICK_DIRECTION_T direction;
	EMP_TRICK_SPEED_T speed;
} EMP_TRICK_MODE_PARAM_T;

/**
 * Subtitle status
 */
typedef enum
{
	EMP_SUBTITLE_NOT_FOUND = -1,
	EMP_SUBTITLE_OFF = 0,
	EMP_SUBTITLE_ON = 1
} EMP_EXT_SUBTITLE_STATUS_T;

/**
 * External subtitle settings
 */
typedef enum
{
	EMP_EXT_SUBTITLE_ENABLE,      // external subtitle on/off setting
	EMP_EXT_SUBTITLE_POSITION,    // subtitle postion setting, Y coordinate offset in step (1step = 10pixels)
	EMP_EXT_SUBTITLE_SYNC,        // subtitle sync setting, sync offset in step (1 step = 0.5 sec)
	EMP_EXT_SUBTITLE_LANGUAGE     // subtitle language setting, language group (see EMP_LANG_GROUP_T)
} EMP_EXT_SUBTITLE_SETTINGS_T;

/**
 * Internal subtitle settings
 */
typedef enum
{
	EMP_INT_SUBTITLE_ENABLE ,         // internal subtitle on/off setting
	EMP_INT_SUBTITLE_LANGUAGE,        // internal subtitle language setting - language index
	EMP_INT_SUBTITLE_LANGUAGE_COUNT   // internal subtitle language count, max number of language
} EMP_INT_SUBTITLE_SETTINGS_T;

/**
 * Language groups
 */
typedef enum
{
	EMP_LANG_LATIN1    = 1,
	EMP_LANG_LATIN2    = 1<<1,
	EMP_LANG_LATIN3    = 1<<2,
	EMP_LANG_LATIN4    = 1<<3,
	EMP_LANG_CYRILLIC  = 1<<4,
	EMP_LANG_ARABIC    = 1<<5,
	EMP_LANG_GREEK     = 1<<6,
	EMP_LANG_HEBREW    = 1<<7,
	EMP_SUBT_TURKISH = 1 << 8,
	EMP_SUBT_THAI = 1 << 9,
	EMP_SUBT_SIMPLIFIED_CHINESE = 1 << 10
} EMP_LANG_GROUP_T;

/**
 * view frame for EMP output
 */
typedef enum
{
	EMP_VIEWFRAME_OSD1,
	EMP_VIEWFRAME_OSD2,
	EMP_VIEWFRAME_VIDEO
} EMP_VIEWFRAME_T;

/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/

#if (PLATFORM_TYPE==BCM_PLATFORM)

EXTERN DTV_STATUS_T DDI_EMP_Initialize( EMP_MEDIA_TYPE_T mediaType);
DTV_STATUS_T DDI_EMP_CheckFormat(void *, UINT8, UINT8);
DTV_STATUS_T DDI_EMP_DecodingSegment(UINT16, UINT16, UINT16, void**, int);
void 		 DDI_EMP_SetAdjustDispRect(UINT32 imageWidth,UINT32 imageHeight,UINT8 scale_denom);

#else
EXTERN DTV_STATUS_T DDI_EMP_Initialize( void );
#endif
DTV_STATUS_T DDI_EMP_Finalize( void );

DTV_STATUS_T DDI_EMP_CreateMediaItem( UINT8 *pMediaPath, EMP_MEDIA_ITEM_T *pItem );

DTV_STATUS_T DDI_EMP_CreateMediaItemFromBuffer( UINT8 *pMediaPath, UINT8 *pBuf, EMP_MEDIA_ITEM_T *pItem );

DTV_STATUS_T DDI_EMP_DeleteMediaItem( EMP_MEDIA_ITEM_T pItem );

DTV_STATUS_T DDI_EMP_RegisterCallback( EMP_MEDIA_TYPE_T type, pfnEMPCallback pfnHandler);

DTV_STATUS_T DDI_EMP_Play( EMP_MEDIA_ITEM_T pItem );

DTV_STATUS_T DDI_EMP_PreviewVideo(EMP_MEDIA_ITEM_T item, UINT16 x, UINT16 y,UINT16 width, UINT16 height);

DTV_STATUS_T DDI_EMP_Stop( EMP_MEDIA_ITEM_T pItem );

DTV_STATUS_T DDI_EMP_Pause( EMP_MEDIA_ITEM_T pItem );

DTV_STATUS_T DDI_EMP_Resume( EMP_MEDIA_ITEM_T pItem );

DTV_STATUS_T DDI_EMP_Rotate( EMP_MEDIA_ITEM_T pItem, EMP_ROTATION_T rotation );

DTV_STATUS_T DDI_EMP_GetThumbNail( EMP_MEDIA_ITEM_T pItem, UINT32 *pWidth, UINT32 *pHeight, EMP_PIXEL_FORMAT_T format, UINT32 *pBuf );

DTV_STATUS_T DDI_EMP_PlayThumbNail(UINT32 x, UINT32 y, UINT32 width, UINT32 height, EMP_PIXEL_FORMAT_T format, UINT32* buf);

DTV_STATUS_T DDI_EMP_GetMetaData( EMP_MEDIA_ITEM_T pItem, EMP_METADATA_T meta, UINT8 *buf, UINT8 bufsize );

DTV_STATUS_T DDI_EMP_GetPlayPosition( EMP_MEDIA_ITEM_T pItem, UINT32 *pPosition );

DTV_STATUS_T DDI_EMP_SetPlayPosition( EMP_MEDIA_ITEM_T pItem, UINT32 position);

DTV_STATUS_T DDI_EMP_SetTrickPlayMode( EMP_TRICK_MODE_PARAM_T param );

DTV_STATUS_T DDI_EMP_GetTrickPlayMode( EMP_TRICK_MODE_PARAM_T *pParam );

DTV_STATUS_T DDI_EMP_SetDisplayRectangle(UINT32 x, UINT32 y, UINT32 width, UINT32 height);

DTV_STATUS_T DDI_EMP_SetViewFrame( EMP_VIEWFRAME_T viewFrame );

DTV_STATUS_T DDI_EMP_SetExternalSubtitleSettings(EMP_EXT_SUBTITLE_SETTINGS_T settingType, SINT8 settingValue );

DTV_STATUS_T DDI_EMP_GetExternalSubtitleSettings(EMP_EXT_SUBTITLE_SETTINGS_T settingType, SINT8 *pSettingValue);

DTV_STATUS_T DDI_EMP_SetInternalSubtitleSettings(EMP_INT_SUBTITLE_SETTINGS_T settingType, SINT8 settingValue);

DTV_STATUS_T DDI_EMP_GetInternalSubtitleSettings(EMP_INT_SUBTITLE_SETTINGS_T settingType, SINT8 *pSettingValue);

DTV_STATUS_T DDI_EMP_GetAudioLanguageCount(UINT8 *pLangCnt);

DTV_STATUS_T DDI_EMP_SetAudioLanguage(UINT8 langIndex);

DTV_STATUS_T DDI_EMP_GetAudioLanguage(UINT8 *pLangIndex);

DTV_STATUS_T DDI_EMP_GetMediaType( EMP_MEDIA_ITEM_T pItem, EMP_MEDIA_TYPE_T *pType );

void DDI_EMP_ClearFrameBuffer( void );

UINT8 *DDI_EMP_GetFrameBuffer( void );

DTV_STATUS_T DDI_EMP_FlushFrameBuffer( UINT8* pFrame );

BOOLEAN DDI_EMP_IsHwDecodingSupported(EMP_MEDIA_ITEM_T item);

DTV_STATUS_T DDI_EMP_GetDRMRegistrationCode(UINT8 *pDRMCode);

DTV_STATUS_T DDI_EMP_CheckDRMAuthorization(EMP_MEDIA_ITEM_T item);

DTV_STATUS_T DDI_EMP_QueryDRMRentalStatus(EMP_MEDIA_ITEM_T item, BOOLEAN *pIsRental, UINT8 *pViewLimit, UINT8 *pViewCount);
DTV_STATUS_T DDI_EMP_ShowMp3Jpeg(char *pFilePath);

DTV_STATUS_T DDI_EMP_Mp3JpegShow(EMP_MEDIA_ITEM_T item);	// geoffrey for warning
DTV_STATUS_T DDI_EMP_Mp3JpegRelease(EMP_MEDIA_ITEM_T item);	// geoffrey for warning



#endif /*_EMP_DDI_H_ */

