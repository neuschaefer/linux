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
 *  @author Oh kyongjoo (kjoh@lge.com)
 *  @version    0.0
 *  @date       2008.02.12
 */


/******************************************************************************
    Header File Guarder
******************************************************************************/
#ifndef _EMP_DDI_H_
#define _EMP_DDI_H_

#define _LG_ENV_            1

#if _LG_ENV_
#include "common.h"
#endif



//#include "../saurn6/inc/general.h"
//#include "mdrv_types.h"
#if (!_LG_ENV_)

#ifndef MSTAR_PLATFORM
#define MSTAR_PLATFORM      0
#endif

#ifndef BCM_PLATFORM
#define BCM_PLATFORM        1
#endif

#ifndef PLATFORM_TYPE
#define PLATFORM_TYPE       MSTAR_PLATFORM
#endif

#if (PLATFORM_TYPE != MSTAR_PLATFORM)
#undef  PLATFORM_TYPE
#define PLATFORM_TYPE   MSTAR_PLATFORM
#endif

#endif

#ifdef _EMUL_WIN
#define off_t long
#endif

#if (PLATFORM_TYPE==MSTAR_PLATFORM)
//#undef  USE_STREAMBUFFERHANDLER_THREAD
#define  USE_STREAMBUFFERHANDLER_THREAD
#endif
/******************************************************************************
    전역 제어 상수(Control Constants)
******************************************************************************/
//#define INCLUDE_FLASH_BR

/******************************************************************************
    #include 파일들 (File Inclusions)
******************************************************************************/

/******************************************************************************
    상수 정의(Constant Definitions)
******************************************************************************/
#define     _LG_SCALER_ENV_             1
#define     _FOR_TEST_                  0

#define     EMP_ASSERT                  DISABLE
#define     FEATURE                     DISABLE

#if (PLATFORM_TYPE==MSTAR_PLATFORM)
#define     DIVXTHUMBNAIL_GETFRAME_INFO TRUE //2009/12/21
#endif

#define EMP_HORIZONTAL_ACTIVE_PXL       1920
#define EMP_VERTICAL_ACTIVE_LINES       1080

#define EMP_FRAME_BUF_LUMA_WIDTH        2048
#define EMP_FRAME_BUF_LUMA_HEIGHT       1088
#define EMP_FEEDSTREAM_SIZE             (0x100000*6) // 1M * 6
#define EMP_REMAINFEEDBUF_THRESHOLD_FOR_PLAY    (EMP_FEEDSTREAM_SIZE - 0x400000)
#define MAX_PHOTO_ITEM_NUM                      3
#define MAX_STREAM_FILE_READ                    0x19000 // 100k
#define MAX_AUDIO_THRESHOLD_FOR_PLAY	        (0x400*128) //128k
#define MAX_STREAM_NO_FILESIZE                   0x3E800000;//1000M

//These definitions describe a stream error code.
#define STREAM_STATUS_NORMAL				  0
#define STREAM_STATUS_ERROR					 -1
#define STREAM_STATUS_EMPTY					 -4
#define STREAM_STATUS_END					 -5
#define STREAM_STATUS_TIMEOUT				-16
#define STREAM_STATUS_MUTEX_FAILED			-15
#define STREAM_STATUS_ABORTED				-17
#if (PLATFORM_TYPE==MSTAR_PLATFORM)
#define STREAM_READ_EOF                 (0x00000001<<0)
#define STREAM_READ_TIMEOUT             (0x00000001<<1)
#define STREAM_READ_MUTEX_FAILED             (0x00000001<<2)
#endif
#define JPEG_REGISTER_CALLBACK
//#define AUTO_FLUSH_FRAMEBUFFER

//#define BROWSER_LG_HTTP
/**
 * parameter and return array length
 */
#define EMP_MAX_MSG_PARAM 4
#define EMP_MAX_MSG_RET	4

#define	STREAM_OBJECT_URL_LENGTH	1024
#define	NO_OF_STREAM_HANDLE			4

//thumbnail resolution
#define	DIVX_THUMBNAIL_WIDTH			100
#define DIVX_THUMBNAIL_HEIGHT			100
#define	DIVX_ATUOTHUMBNAIL_WIDTH		180//240 //320
#define DIVX_AUTOTHUMBNAIL_HEIGHT		136//180 //240

#if (MAIN_CHIP_TYPE == CHIP_BCM35230) || (MAIN_CHIP_TYPE == CHIP_SATURN8)	//#if (BCHP_CHIP == 35230 )
#define SUPPORT_DIVX_HD_PLUS
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
#ifdef INCLUDE_YVIDEO
	EMP_MEDIA_TYPE_YVIDEO,
#endif
#ifdef INCLUDE_YAUDIO
	EMP_MEDIA_TYPE_YAUDIO,
#endif
#ifdef INCLUDE_FLASH_BR
	/* 100903 mk.heo
	상위 type 추가 시 MEDIA_GetEMEType( Addon_Hoa_Mediaplay2.c)를 수정해주어야 합니다.
	가능한한 하위에 type 추가 부탁드립니다. flash video playback에 영향을 줄 수잇습니다..*/
	EMP_MEDIA_TYPE_FLASHES = 5,
#endif
#ifdef INCLUDE_BROWSER
	EMP_MEDIA_TYPE_BROWSER,
#endif
#ifdef INCLUDE_VCS
	EMP_MEDIA_TYPE_VCS,
#endif
    EMP_MEDIA_TYPE_UNKNOWN,
} EMP_MEDIA_TYPE_T;

/**
 * Preview Mode
*/
typedef enum
{
    E_EMP_PREVIEW_NONE              = 0, // give you audio , video,  duration, resolution
    E_EMP_PREVIEW_AUDIO             = 1, // give you audio
    E_EMP_PREVIEW_VIDEO             = 2, // give you vidoe
    E_EMP_PREVIEW_INFO_DURATION     = 3, // give you resolution
    E_EMP_PREVIEW_INFO_RESOLUTION   = 4,
    E_EMP_PREVIEW_INFO_INDEXTABLE   = 5,
    E_EMP_PREVIEW_INFO_THUMBNAIL    = 6, // give you thumbnail ,and resolution

} EN_EMP_PREVIDE_MODE;

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
    EMP_METADATA_VIDEO_CODEC,
    EMP_METADATA_AUDIO_CODEC,
    EMP_METADATA_ERR_CODEC_INFO,
    EMP_METADATA_INDEX_EXISTED,
	EMP_METADATA_LIVEFLAG,
    EMP_METADATA_VBR,       // added by geoffrey 20090113
    EMP_METADATA_EMP_FIFODEPTH,
	EMP_METADATA_EMP_FIFOSIZE,
    EMP_METADATA_VDEC_FIFODEPTH,
    EMP_METADATA_VDEC_FIFOSIZE,
    EMP_METADATA_JPEG_DISPLAY_W,
    EMP_METADATA_JPEG_DISPLAY_H,
    EMP_METADATA_ISDIVXPLUSHD,
#ifdef INCLUDE_MSDL_PROCESS
	EMP_METADATA_ASF_HAEDER_INFO,
#endif
    EMP_METADATA_VIDEO_CSC_FORMAT,  // TODO: to be modified
} EMP_METADATA_T;

/**
 * Transport Type
*/
typedef enum
{
	EMP_TRANSPORT_UNKNOWN,
	EMP_TRANSPORT_ES,
	EMP_TRANSPORT_TS,
	EMP_TRANSPORT_MPEG2PES,
	EMP_TRANSPORT_VOB,
	EMP_TRANSPORT_MPEG1PS,
	EMP_TRANSPORT_DSSES,
	EMP_TRANSPORT_DSSPES,
	EMP_TRANSPORT_ASF,
	EMP_TRANSPORT_AVI,
	EMP_TRANSPORT_MP4,
	EMP_TRANSPORT_FLV,
	EMP_TRANSPORT_MKV,
	EMP_TRANSPORT_WAV,
	EMP_TRANSPORT_MAX
} EMP_TRANSPORT_TYPE_T;

/**
* Subtitle Type
*/
typedef enum
{
    EMP_SUBTITLE_UNKNOWN,
    EMP_SUBTITLE_SRT, //Text
    EMP_SUBTITLE_UTF8, //Text
    EMP_SUBTITLE_ASS,//Text
    EMP_SUBTITLE_SSA,//Text
    EMP_SUBTITLE_3GPP,//Text
    EMP_SUBTITLE_USF,//Text
    EMP_SUBTITLE_BMP, //bitmap
    EMP_SUBTITLE_VOBSUB, //bitmap
    EMP_SUBTITLE_BMP_TS, //bitmap
    EMP_SUBTITLE_MAX
} EMP_SUBTITLE_TYPE_T;

/**
 * If this changed, "eme_api.h" should be changed also
 * kjoh 20081009.
 */
typedef enum
{
    EMP_VIDEO_UNKNOW,
    EMP_VIDEO_MPEG1,
    EMP_VIDEO_MPEG2,
    EMP_VIDEO_MPEG4,
    EMP_VIDEO_MJPEG,
    EMP_VIDEO_H264,
    EMP_VIDEO_REALVIDEO,
    EMP_VIDEO_VC1,
    EMP_VIDEO_H263,
    EMP_VIDEO_AVS,
    EMP_VIDEO_WMV,
    EMP_VIDEO_DIVX3,
    EMP_VIDEO_DIVX4,
    EMP_VIDEO_DIVX5,
    EMP_VIDEO_MAX
} EMP_VIDEO_CODE_T;

/**
 * If this changed, "eme_api.h" should be changed also
 * kjoh 20081009.
 */
typedef enum
{
    EMP_AUDIO_UNKNOW,
    EMP_AUDIO_MP3,
    EMP_AUDIO_AC3,
    EMP_AUDIO_MPEG,
    EMP_AUDIO_AAC_HAAC,
    EMP_AUDIO_CDDA,
    EMP_AUDIO_LPCM,
    EMP_AUDIO_LBR,
    EMP_AUDIO_WMA,
    EMP_AUDIO_DTS,
    #if (PLATFORM_TYPE == BCM_PLATFORM)
    EMP_AUDIO_AC3PLUS,
    #endif
    EMP_AUDIO_NONE,
    EMP_AUDIO_MAX
} EMP_AUDIO_CODE_T;

/**
 * If this changed, "eme_api.h" should be changed also
 * sdcho 20081127.
 */
typedef enum
{
    EMP_SECURITY_NONE,
    EMP_SECURITY_AES,
    EMP_SECURITY_WMDRM,
    EMP_SECURITY_MAX
} EMP_SECURITY_ALGORITHM_T;

/**
 * Video Codec information
 */
typedef enum
{
    EMP_VIDEOCODEC_UNKNOWN,
    EMP_VIDEOCODEC_KNOWN
} EMP_VIDEOCODEC_T;

/**
 * Event message; play done message or error message
 */
typedef enum
{
   EMP_MSG_PLAYDONE = 1,
   EMP_MSG_PLAYERR,
   EMP_MSG_PLAYSTART,   // #ifdef INCLUDE_YOUTUBE
   EMP_MSG_PLAYPAUSE,	// #ifdef INCLUDE_YOUTUBE
   EMP_MSG_PLAYRESUME,
   EMP_MSG_PLAYSEEK,
   EMP_MSG_ELAPSEDTIME,
   EMP_MSG_EOF,			// #ifdef INCLUDE_YOUTUBE
   EMP_MSG_DATAEXHAUSTED,//for Netflix only
   EMP_MSG_UNKNOWN,
   EMP_MSG_DRM_CHECK_DONE,
   EMP_MSG_FAILDRAG,
   EMP_MSG_SEEKDONE,
   EMP_MSG_SEEKFAIL,
   EMP_MSG_INTERNAL_SUBTITLE,

   EMP_MSG_END
} EMP_MSG_T;

typedef void* EMP_MEDIA_ITEM_T;

typedef void (*pfnEMPCallback_T)(EMP_MEDIA_ITEM_T item, EMP_MSG_T msg, UINT32 data);


/**
 * Action message; send action msg to sw decoder
 */
typedef enum
{
   EMP_MSG_SWDEC_PLAY          = 1,
   EMP_MSG_SWDEC_EXIF_PLAY     = 2,
   EMP_MSG_SWDEC_SHOW_MP3_JPEG = 3,
   EMP_MSG_SWDEC_GET_JPEGINFO  = 4
} EMP_MSG_SWDEC_T;

/**
 * Software Decoder Callback Function
 *   @param
 *      it's decieded by msg type
 *   @ret
 *      it's decieded by msg type
*/
typedef UINT32(*pfnEMPSwDecCallback_T)(EMP_MSG_SWDEC_T msg,
                                     const UINT32 param[EMP_MAX_MSG_PARAM],
                                     UINT32 ret[EMP_MAX_MSG_RET]);


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
    EMP_TRICK_SPEED_FAST_32X,
    EMP_TRICK_SPEED_USERDEFINED
} EMP_TRICK_SPEED_T;

/**
 * Trick mode parameters
 */
typedef struct
{
    EMP_TRICK_DIRECTION_T direction;
    EMP_TRICK_SPEED_T speed;
	SINT32 userdefinedrate;
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
    EMP_INT_SUBTITLE_LANGUAGE_COUNT,   // internal subtitle language count, max number of language
    EMP_INT_SUBTITLE_MKV_SUBT,		   // MKV internal subtitle setting
    EMP_INT_SUBTITLE_TYPE              // internal subtitle type

} EMP_INT_SUBTITLE_SETTINGS_T;

/**
 * Subtitle Text Informaion
 */
typedef struct  EMP_INT_SUBTITLE_TEXT_INFO
{
    UINT32 u32TimeStart;
    UINT32 u32TimeEnd;
    UINT32 u32SubtitlePos;

} EMP_INT_SUBTITLE_TEXT_INFO_T;
/**
 * Language groups
 */
typedef enum
{
    EMP_LANG_LATIN1    = 1,
    EMP_LANG_LATIN2    = 1 << 1,
    EMP_LANG_LATIN3    = 1 << 2,
    EMP_LANG_LATIN4    = 1 << 3,
    EMP_LANG_CYRILLIC  = 1 << 4,
    EMP_LANG_ARABIC    = 1 << 5,
    EMP_LANG_GREEK     = 1 << 6,
    EMP_LANG_HEBREW    = 1 << 7,
    EMP_SUBT_TURKISH = 1 << 8,
    EMP_SUBT_THAI = 1 << 9,
    EMP_SUBT_SIMPLIFIED_CHINESE = 1 << 10
} EMP_LANG_GROUP_T;

/**
 * DRM Activation Status
*/
typedef enum {
	EMP_DRM_IDLE = 0,
	EMP_DRM_ACTIVATION,
	EMP_DRM_DEACTIVATION,
} EMP_DRM_ACTIVATION_STATUS_T;

/**
 * view frame for EMP output
 */
typedef enum
{
    EMP_VIEWFRAME_OSD1,
    EMP_VIEWFRAME_OSD2,
    EMP_VIEWFRAME_VIDEO
} EMP_VIEWFRAME_T;

/**
 * DRM status
 */
typedef enum
{
    EMP_DRM_AUTHORIZED_SUCCESS = 0,
    EMP_DRM_AUTHORIZED_ERROR,
    EMP_DRM_REGISTERED_ERROR,
    EMP_DRM_RENTAL_EXPIRED
} EMP_DRM_STATUS_T;

/**
 * Video Information
*/
typedef struct EMP_VIDEO_INFO{
	UINT32 videoPID;
	UINT32 videoCodec;
} EMP_VIDEO_INFO_T;

/**
 * Audio Information
*/
typedef struct EMP_AUDIO_INFO{
	UINT32 audioPID;
	UINT32 audioCodec;
} EMP_AUDIO_INFO_T;

/**
* This enumeration describes the video type in streaming mode.
*/
typedef enum
{
    EMP_STREAM_VIDEO_TYPE_UNKNOWN = 0x00,
    EMP_STREAM_VIDEO_TYPE_MPEG1 = 0x01,
    EMP_STREAM_VIDEO_TYPE_MPEG2 = 0x02,
 #ifdef INCLUDE_FLASH_BR
	EMP_STREAM_VIDEO_TYPE_H264 = 0x03,
#endif
} EMP_STREAM_VIDEO_TYPE_T;

/**
* This enumeration describes the audio type in streaming mode.
*/
typedef enum
{
    EMP_STREAM_AUDIO_TYPE_UNKNOWN = 0x00,
    EMP_STREAM_AUDIO_TYPE_MP3 = 0x01,
    EMP_STREAM_AUDIO_TYPE_PCM = 0x02,
	 EMP_STREAM_AUDIO_TYPE_WAV = 0x03,
#ifdef INCLUDE_FLASH_BR
	EMP_STREAM_AUDIO_TYPE_AAC = 0x04,
#endif
} EMP_STREAM_AUDIO_TYPE_T;


#if (PLATFORM_TYPE == MSTAR_PLATFORM)
/**
* This enumeration describes the streaming mode for EMP module.
*/
typedef enum {
    EMP_STREAM_NONE_MODE = 0x00,
	EMP_STREAM_DLNA_MODE          = 0x01,
	EMP_STREAM_YOUTUBE_IMAGE_MODE = 0x02,
	EMP_STREAM_YOUTUBE_VIDEO_MODE = 0x03,
	EMP_STREAM_FLASH_MODE         = 0x04,
	EMP_STREAM_YAHOO_VIDEO_MODE   = 0x05,
	EMP_STREAM_PUSH_VIDEO_MODE    = 0x06,
	EMP_STREAM_PUSH_SOUND_MODE    = 0x07,
	EMP_STREAM_BROWSER_VIDEO_MODE = 0x08,
	EMP_STREAM_DLNA_DMR_MODE      = 0x09,
//#ifdef INCLUDE_FLASH_BR
	EMP_STREAM_FLASHES_MODE = 0x0A,
//#endif

} EMP_STREAM_MODE_T;

#elif (PLATFORM_TYPE == BCM_PLATFORM)
/**
* This enumeration describes the streaming mode for EMP module.
*/
typedef enum
{
	/*
	 * comment by bk1472 - 2009/10/17
	 * 기존의 compile option 삭제.
	 * 이유 : compile option 이 분리 되어잇다고 하더라도 겹치는 number 가 없음.
	 *        enumeration에서는 가급적 compile option 삭제 바람.
	 */
	EMP_STREAM_NONE_MODE          = 0x00,
	EMP_STREAM_DLNA_MODE          = 0x01,
	EMP_STREAM_PULL_BB_MODE			= 0x02,
	EMP_STREAM_FLASH_MODE         = 0x04,
	EMP_STREAM_PUSH_VIDEO_MODE = 0x06,
	EMP_STREAM_PUSH_SOUND_MODE = 0x07,
	EMP_STREAM_BROWSER_VIDEO_MODE = 0x08,
	EMP_STREAM_DLNA_DMR_MODE      = 0x09,
#ifdef INCLUDE_FLASH_BR
	EMP_STREAM_FLASHES_MODE = 0x0A,
#endif
} EMP_STREAM_MODE_T;

#ifdef INCLUDE_FLASH_BR
/**
* Flash Channel
*/
typedef enum
{
	EMP_FLASHES_NONE_CH				= 0x00,
	EMP_FLASHES_VIDEO_CH,
	EMP_FLASHES_AUDIO_CH,
	EMP_FLASHES_CH_MAX = EMP_FLASHES_AUDIO_CH
} EMP_FLASHES_CHANNEL_T;

/**
* Flash Buffer Status
*/
typedef enum
{
	EMP_FLASHES_BUFFER_LOW,
	EMP_FLASHES_BUFFER_FULL,
	EMP_FLASHES_H264_EOF,
} EMP_FLASHES_CBMSG_T;

/**
 * Typedef of callback function to get notice about playback end.
 */
typedef void (*FLASHES_BUFFER_CB_T)(EMP_FLASHES_CHANNEL_T ch, EMP_FLASHES_CBMSG_T msg);
#endif

#elif (PLATFORM_TYPE == MTK_PLATFORM)
/**
* This enumeration describes the streaming mode for EMP module.
*/
typedef enum {
    EMP_STREAM_NONE_MODE = 0x00,
	EMP_STREAM_DLNA_MODE          = 0x01,
	EMP_STREAM_YOUTUBE_IMAGE_MODE = 0x02,
	EMP_STREAM_YOUTUBE_VIDEO_MODE = 0x03,
	EMP_STREAM_FLASH_MODE         = 0x04,
	EMP_STREAM_YAHOO_VIDEO_MODE   = 0x05,
	EMP_STREAM_PUSH_VIDEO_MODE    = 0x06,
	EMP_STREAM_PUSH_SOUND_MODE    = 0x07,
	EMP_STREAM_BROWSER_VIDEO_MODE = 0x08,
	EMP_STREAM_DLNA_DMR_MODE      = 0x09,
//#ifdef INCLUDE_FLASH_BR
	EMP_STREAM_FLASHES_MODE = 0x0A,
//#endif

} EMP_STREAM_MODE_T;
#endif

/**
* This enum describes the streaming flow mode.
*/
typedef enum
{
    EMP_STREAM_FLOW_NONE,
    EMP_STREAM_FLOW_PUSH,
    EMP_STREAM_FLOW_PULL
} EMP_STREAM_FLOW_MODE_T;

/**
* Stream Object Information
*/
typedef enum
{
    EMP_STREAM_OBJECT_INFO_AUDIOTYPE        = 0x00000100,
    EMP_STREAM_OBJECT_INFO_VIDEOTYPE        = 0x00000200,
    EMP_STREAM_OBJECT_INFO_CONTENTLENGTH    = 0x00000400,
    EMP_STREAM_OBJECT_INFO_DURATION         = 0x00000800,
    EMP_STREAM_OBJECT_INFO_PCM              = 0x00001000,
    EMP_STREAM_OBJECT_INFO_CONTENTBUFFER    = 0x00002000,
}EMP_STREAM_OBJECT_INFO_FLAG_T;

/**
* Internal Subtitle Parameter
*/
typedef struct {
	int startTime;
	int endTime;
	UINT32 nLen;
	char *szBuf;
} EMP_INTERNAL_SUBTITLE_PARAM_T;


#if (PLATFORM_TYPE == MSTAR_PLATFORM)
/**
* This struct describes the stream object type for Streaming mode.
* This enumeration describes the streaming mode for EMP module.
*/
typedef struct EMP_STREAM_OBJECT_INFO{
	EMP_MEDIA_TYPE_T		mediaType;
	EMP_STREAM_AUDIO_TYPE_T	audioType;	/* DLNA */
	EMP_STREAM_VIDEO_TYPE_T	videoType;	/* DLNA */
	UINT64					contentLength;
	UINT32					duration;
	UINT32 					sampleFrequency;
	UINT16					pcmSampleRate;
	UINT16					pcmChannelMode;
	UINT16 					pcmBitsPerSample;
	UINT8					thumbnailURL[STREAM_OBJECT_URL_LENGTH];
	UINT8					*pContentBuffer;
	UINT64 					bitRate;	/* DLNA */
	UINT8 					ch;
    UINT8 					Ismsdlmode;
    EMP_STREAM_OBJECT_INFO_FLAG_T settingflags;
	EMP_STREAM_FLOW_MODE_T	flowMode;
#ifdef INCLUDE_FLASH_BR
	UINT32					bufferMinLevel;
	UINT32					bufferMaxLevel;
	SINT64					ptsToDecode;
	BOOLEAN					pauseAtDecodeTime;
#endif
}EMP_STREAM_OBJECT_INFO_T;

typedef enum
{
	EMP_FLASHES_NONE_CH				= 0x00,
	EMP_FLASHES_VIDEO_CH,
	EMP_FLASHES_AUDIO_CH,
	EMP_FLASHES_CH_MAX = EMP_FLASHES_AUDIO_CH
} EMP_FLASHES_CHANNEL_T;

typedef enum
{
    EMP_FLASHES_BUFFER_LOW,
    EMP_FLASHES_BUFFER_FULL,
    EMP_FLASHES_H264_EOF,
} EMP_FLASHES_CBMSG_T;
typedef void (*FLASHES_BUFFER_CB_T)(EMP_FLASHES_CHANNEL_T ch, EMP_FLASHES_CBMSG_T msg);

typedef int (*pfnStreamClientDescriptor)(int sd);
typedef int (*pfnStreamRead_T)(int streamHandle,void * pBuf,UINT32 size,int timeoutMs);
typedef long long (*pfnStreamSkip_T)(int streamHandle,SINT64 offset);
typedef int (*pfnStreamStop_T)(int streamHandle);
typedef int (*pfnStreamClose_T)(int streamHandle);
typedef int (*pfnStreamRestart)(char *uri, UINT8 type);


/**
 * This struct describes the streaming callback function that will be used in EMP DD.
*/
typedef struct EMP_STREAM_PFNHANDLER{

    pfnStreamRead_T read;
    pfnStreamSkip_T skip;
    pfnStreamStop_T stop;
    pfnStreamClose_T close;
    int streamHandle;
    pfnStreamClientDescriptor getSD;
    pfnStreamRestart restart;
    int handle[NO_OF_STREAM_HANDLE];
}EMP_STREAM_PFNHANDLER_T;

#elif (PLATFORM_TYPE == BCM_PLATFORM)

#define	DRM_TEST	DISABLE

/**
 * Stream Object Information Structure
*/
typedef struct	EMP_STREAM_OBJECT_INFO{

	EMP_MEDIA_TYPE_T		mediaType;
	EMP_STREAM_AUDIO_TYPE_T	audioType;	/* DLNA */
	EMP_STREAM_VIDEO_TYPE_T	videoType;	/* DLNA */
	UINT64					contentLength;
	UINT32					duration;
	UINT32 					sampleFrequency;
	UINT16					pcmSampleRate;
	UINT16					pcmChannelMode;
	UINT16 					pcmBitsPerSample;
	UINT64 					bitRate;	/* DLNA */
	UINT8					thumbnailURL[STREAM_OBJECT_URL_LENGTH];
	UINT8					*pContentBuffer;
	UINT8 					ch;
    EMP_STREAM_OBJECT_INFO_FLAG_T settingflags;
	EMP_STREAM_FLOW_MODE_T	flowMode;
#ifdef INCLUDE_FLASH_BR
	UINT32					bufferMinLevel;
	UINT32					bufferMaxLevel;
	SINT64					ptsToDecode;
	BOOLEAN					pauseAtDecodeTime;
#endif
}EMP_STREAM_OBJECT_INFO_T;

typedef int (*pfnStreamClientDescriptor)(int sd);
typedef ssize_t (*pfnStreamRead_T)(int sd, void * pBuf, size_t size, int timeoutMs);
typedef off_t (*pfnStreamSkip_T)(int sd, off_t offset);
typedef int	 (*pfnStreamStop_T)(int sd);
typedef int (*pfnStreamClose_T)(int sd);
typedef int (*pfnStreamRestart)(char *uri, UINT8 type);
typedef int (*pfnStreamStart_T)(int sd, long long startOffset, long long endOffset); //100204 sungeol.baek
typedef void (*pfnStreamStat_T)(UINT64 *fileSize, UINT32 *bufReadable);


/**
 * This struct describes the streaming callback function that will be used in EMP DD.
*/
typedef struct EMP_STREAM_PFNHANDLER{
	pfnStreamClientDescriptor	getSD;
	pfnStreamRead_T				read;
	pfnStreamSkip_T				skip;
	pfnStreamStop_T				stop;
	pfnStreamClose_T			close;
	pfnStreamRestart			restart;
	pfnStreamStart_T			start;			//100204 sungeol.baek
	pfnStreamStat_T				stat;
	int							handle[NO_OF_STREAM_HANDLE];
}EMP_STREAM_PFNHANDLER_T;

#elif (PLATFORM_TYPE == MTK_PLATFORM)

#define	DRM_TEST	DISABLE

/**
 * Stream Object Information Structure
*/
typedef struct	EMP_STREAM_OBJECT_INFO{

	EMP_MEDIA_TYPE_T		mediaType;
	EMP_STREAM_AUDIO_TYPE_T	audioType;	/* DLNA */
	EMP_STREAM_VIDEO_TYPE_T	videoType;	/* DLNA */
	UINT64					contentLength;
	UINT32					duration;
	UINT32 					sampleFrequency;
	UINT16					pcmSampleRate;
	UINT16					pcmChannelMode;
	UINT16 					pcmBitsPerSample;
	UINT64 					bitRate;	/* DLNA */
	UINT8					thumbnailURL[STREAM_OBJECT_URL_LENGTH];
	UINT8					*pContentBuffer;
	UINT8 					ch;
    EMP_STREAM_OBJECT_INFO_FLAG_T settingflags;
	EMP_STREAM_FLOW_MODE_T	flowMode;
#ifdef INCLUDE_FLASH_BR
	UINT32					bufferMinLevel;
	UINT32					bufferMaxLevel;
	SINT64					ptsToDecode;
	BOOLEAN					pauseAtDecodeTime;
#endif
}EMP_STREAM_OBJECT_INFO_T;

typedef int (*pfnStreamClientDescriptor)(int sd);
typedef ssize_t (*pfnStreamRead_T)(int sd, void * pBuf, size_t size, int timeoutMs);
typedef off_t (*pfnStreamSkip_T)(int sd, off_t offset);
typedef int	 (*pfnStreamStop_T)(int sd);
typedef int (*pfnStreamClose_T)(int sd);
typedef int (*pfnStreamRestart)(char *uri, UINT8 type);
typedef int (*pfnStreamStart_T)(int sd, long long startOffset, long long endOffset); //100204 sungeol.baek
typedef void (*pfnStreamStat_T)(UINT64 *fileSize, UINT32 *bufReadable);


/**
 * This struct describes the streaming callback function that will be used in EMP DD.
*/
typedef struct EMP_STREAM_PFNHANDLER{
	pfnStreamClientDescriptor	getSD;
	pfnStreamRead_T				read;
	pfnStreamSkip_T				skip;
	pfnStreamStop_T				stop;
	pfnStreamClose_T			close;
	pfnStreamRestart			restart;
	pfnStreamStart_T			start;			//100204 sungeol.baek
	pfnStreamStat_T				stat;
	int							handle[NO_OF_STREAM_HANDLE];
}EMP_STREAM_PFNHANDLER_T;


#ifdef INCLUDE_FLASH_BR
/**
* Flash Channel
*/
typedef enum
{
	EMP_FLASHES_NONE_CH				= 0x00,
	EMP_FLASHES_VIDEO_CH,
	EMP_FLASHES_AUDIO_CH,
	EMP_FLASHES_CH_MAX = EMP_FLASHES_AUDIO_CH
} EMP_FLASHES_CHANNEL_T;

/**
* Flash Buffer Status
*/
typedef enum
{
	EMP_FLASHES_BUFFER_LOW,
	EMP_FLASHES_BUFFER_FULL,
	EMP_FLASHES_H264_EOF,
} EMP_FLASHES_CBMSG_T;

/**
 * Typedef of callback function to get notice about playback end.
 */
typedef void (*FLASHES_BUFFER_CB_T)(EMP_FLASHES_CHANNEL_T ch, EMP_FLASHES_CBMSG_T msg);

#endif  // INCLUDE_FLASH_BR

#endif

/**
 * Image Move Direction
*/
typedef enum
{
    EMP_PICTURE_MOVE_DIRECTION_UP = 0,
    EMP_PICTURE_MOVE_DIRECTION_DOWN,
    EMP_PICTURE_MOVE_DIRECTION_LEFT,
    EMP_PICTURE_MOVE_DIRECTION_RIGHT,
    EMP_PICTURE_MOVE_DIRECTION_NOT_SUPPORT,
} EMP_PICTURE_MOVE_DIRECTION_T; // Move direction.

#if (MAIN_CHIP_TYPE == CHIP_SATURN7) || (MAIN_CHIP_TYPE == CHIP_SATURN8)
/**
 * Image Enlargement
*/
typedef enum
{
    EMP_PICTURE_ENLARGEMENT_1X = 0,
    EMP_PICTURE_ENLARGEMENT_2X,
    EMP_PICTURE_ENLARGEMENT_4X,
    EMP_PICTURE_ENLARGEMENT_8X,
    EMP_PICTURE_ENLARGEMENT_MAX_NUM,
} EMP_PICTURE_ENLARGEMENT_RATIO_T; // Enlargement Ratio
#else
/**
 * Image Enlargement
*/
typedef enum
{
    EMP_PICTURE_ENLARGEMENT_1X = 0,
    EMP_PICTURE_ENLARGEMENT_2X,
    EMP_PICTURE_ENLARGEMENT_4X,
//    EMP_PICTURE_ENLARGEMENT_8X,
    EMP_PICTURE_ENLARGEMENT_MAX_NUM,
} EMP_PICTURE_ENLARGEMENT_RATIO_T; // Enlargement Ratio
#endif

/**
 * Audio Type
*/
#if (PLATFORM_TYPE==MSTAR_PLATFORM)
typedef enum
{
    EMP_MEDIA_TYPE_AUDIO_MP3,
    EMP_MEDIA_TYPE_AUDIO_WMA,
    EMP_MEDIA_TYPE_AUDIO_AAC,
    EMP_MEDIA_TYPE_AUDIO_PCM,
    EMP_MEDIA_TYPE_AUDIO_UNKNOW,
} EMP_MEDIA_TYPE_AUDIO_T;
#else
/**
* Audio Type
*/
typedef enum
{
    EMP_MEDIA_TYPE_AUDIO_MP3,
    EMP_MEDIA_TYPE_AUDIO_WMA,
    EMP_MEDIA_TYPE_AUDIO_AAC,
    EMP_MEDIA_TYPE_AUDIO_PCM
} EMP_MEDIA_TYPE_AUDIO_T;
#endif
/**
 * Image Type
*/
typedef enum
{
    EMP_MEDIA_TYPE_IMAGE_JPG,
    EMP_MEDIA_TYPE_IMAGE_MOTION_JPG,
    EMP_MEDIA_TYPE_IMAGE_BMP,
    EMP_MEDIA_TYPE_IMAGE_PNG
} EMP_MEDIA_TYPE_IMAGE_T;

/**
 * Video Player Type
*/
typedef enum
{
    VIDEO_PLAYER_PLAYING_TYPE_MPEG4 = 0,
    VIDEO_PLAYER_PLAYING_TYPE_MPEG2,                // mpeg2
    VIDEO_PLAYER_PLAYING_TYPE_TS,                   // TS file
    VIDEO_PLAYER_PLAYING_TYPE_RM,                   // RM file
    VIDEO_PLAYER_PLAYING_TYPE_NUM,
    VIDEO_PLAYER_PLAYING_TYPE_INVALID,
} VIDEO_PLAYER_PLAYING_TYPE_T;

/**
 * Media Channel
*/
typedef enum
{
	EMP_MEDIA_CH_ALL = -1,	/**< All channel */
	EMP_MEDIA_CH_A = 0,		/**< Channel A */
	EMP_MEDIA_CH_B,			/**< Channel B */
	EMP_MEDIA_CH_NUM		/**< Maximum channel number */
} EMP_MEDIA_CHANNEL_T;

// 누군가가 adec에서 정의되어 있던 ADEC_BT_STEREO를 가져다 쓰고 있어, 이곳으로 옮겨놓음.
// 2010.06.18 YW Jung.
/**
 * ADEC Channel Mode for Bluetooth
 *
 */
typedef enum
{
	ADEC_BT_MONO			= 0,
	ADEC_BT_DUAL_CHANNEL	= 1,
	ADEC_BT_STEREO			= 2,
	ADEC_BT_JOINT_STEREO	= 3,
} ADEC_BT_CHANNEL_MODE_T;

/**
 * stream type when feeding stream
 */
typedef enum
{
	EMP_STREAM_TYPE_NONE = 0,
	EMP_STREAM_TYPE_MULTIPLEXED,
	EMP_STREAM_TYPE_AUDIO,
	EMP_STREAM_TYPE_VIDEO
} EMP_STREAM_TYPE_T;

/**
 * Movie Information
*/
typedef struct EMP_MOVIE_INFO
{
	UINT32 						width;
	UINT32 						height;
	EMP_VIDEO_CODE_T			eVideoCodec;
	EMP_AUDIO_CODE_T			eAudioCodec;
	UINT32						uVideoId;
	UINT32						uAudioId;
	EMP_TRANSPORT_TYPE_T		eTransportType;
	EMP_SECURITY_ALGORITHM_T	eSecurityType;
	BOOLEAN						bSeperatedAV;
	BOOLEAN						bAdaptiveStreamChangingResolution;// adaptive stream for changing Resolution.
} EMP_MOVIE_INFO_T;

/**
 * feed Option
 */
typedef struct EMP_FEED_OPT
{
	EMP_STREAM_TYPE_T	eStreamType;	/**< stream type(Multi/Audio/Video) */
	BOOLEAN				bHeaderData;	/**< header or data packet */
	BOOLEAN				bSendEOS;		/**< whether send EOS */
} EMP_FEED_OPT_T;

#if (PLATFORM_TYPE==MSTAR_PLATFORM)

#ifdef USE_STREAMBUFFERHANDLER_THREAD
/**
 * Stream Message
*/
typedef enum {

	EMP_STREAM_MSG_PLAY_START =0,
	EMP_STREAM_MSG_PLAY_STOP,
	EMP_STREAM_MSG_PLAY_RESUME,
	EMP_STREAM_MSG_PLAY_PAUSE,
	EMP_STREAM_MSG_PLAY_JUMP,
	EMP_STREAM_MSG_PLAY_UNKNOWN,
	EMP_STREAM_MSG_PLAY_DRM_CHECK,
	EMP_STREAM_MSG_MAX,
}EMP_STREAM_MSG_T;

/**
 * Stream Status
*/
typedef enum{
	EMP_STREAM_PB_STATUS_INIT = 0,
	EMP_STREAM_PB_STATUS_PROVEN,
	EMP_STREAM_PB_STATUS_PLAY,
	EMP_STREAM_PB_STATUS_STOP,
	EMP_STREAM_PB_STATUS_UNKNOWN

}EMP_STREAM_PB_STATUS_T;

/**
 * Audio Clip Status
*/
typedef enum{
	EMP_AUDIO_CLIP_STATUS_NONE = 0,
	EMP_AUDIO_CLIP_STATUS_PLAY,
	EMP_AUDIO_CLIP_STATUS_PAUSE,
	EMP_AUDIO_CLIP_STATUS_DONE
}EMP_AUDIO_CLIP_STATUS_T;

/**
 * Stream Message Type
*/
typedef struct {

	EMP_STREAM_MSG_T	msgID;
	EMP_MEDIA_TYPE_T	mediaType;
	EMP_MEDIA_ITEM_T	pItem;
} EMP_STREAM_MSG_TYPE_T;

#endif
#endif
/**
 * Media Item Information Structure
*/
typedef struct MEDIA_ITEM
{
    UINT32      fd;
    UINT64      size;
    UINT8       type;
#if (MAIN_CHIP_TYPE == CHIP_SATURN7) || (MAIN_CHIP_TYPE == CHIP_SATURN8)
    UINT8       u8FileName[512];
	UINT64      u64fileCurPos;
#else
    UINT8       u8FileName[256];
	UINT32      u32fileCurPos;
#endif
	UINT32 u32EsFlashPts;
    EMP_MEDIA_TYPE_AUDIO_T      enAudioSubtype;
    EMP_MEDIA_TYPE_IMAGE_T      enImageSubtype;

    //Keep Stream mode information
    EMP_STREAM_FLOW_MODE_T      enflowMode;
    EMP_STREAM_MODE_T      		enStreamMode; //2009/05/06

    BOOLEAN                     bStreamMode;
#if (PLATFORM_TYPE==MSTAR_PLATFORM)
    BOOLEAN                     bUnknowAudioStream;
#endif
    EMP_STREAM_OBJECT_INFO_T    StreamInfo;
    EMP_STREAM_OBJECT_INFO_T    *pStreamInfo;
    EMP_STREAM_PFNHANDLER_T     Streamfnhandler;
    int 						getSD;
    UINT32                      u32StreamReadStatus;
#if (PLATFORM_TYPE==MSTAR_PLATFORM)
#ifdef USE_STREAMBUFFERHANDLER_THREAD
	UINT8					Ismsdltype;
	EMP_STREAM_PB_STATUS_T	PBstatus;
	EMP_STREAM_PB_STATUS_T	PBstopped;
#endif
#endif
    //Keep Audio information
    void *pAudioInfo;

    //Keep Video information
    void* pVideoInfo;
    VIDEO_PLAYER_PLAYING_TYPE_T eVideoType;

    //Keep photo information
    void* pPhotoInfo;
} MEDIA_ITEM_T;
/******************************************************************************
    JPEG Slide Show(Function Declaration)
******************************************************************************/
#define PHOTO_SLIDE_SHOW_EFFECT                         1
#define ENABLE_PHOTO_TRANS_EFFECT_BLOCKS                1
#define ENABLE_PHOTO_TRANS_EFFECT_DIAGONAL_BLOCKS       1
#define ENABLE_PHOTO_TRANS_EFFECT_WIPE                  1
#define ENABLE_PHOTO_TRANS_EFFECT_SLIDE                 1
#define ENABLE_PHOTO_TRANS_EFFECT_FADE_IN               1


#if PHOTO_SLIDE_SHOW_EFFECT
/**
 * Slide show effect enum
*/
typedef enum
{
    EMP_SLIDE_SHOW_EFFECT_NORMAL,//0
#if (ENABLE_PHOTO_TRANS_EFFECT_BLOCKS)
    EMP_SLIDE_SHOW_EFFECT_HORZ_BLOCKS_TOPLEFT, //01
    EMP_SLIDE_SHOW_EFFECT_HORZ_BLOCKS_TOPRIGHT,
    EMP_SLIDE_SHOW_EFFECT_HORZ_BLOCKS_BOTTOMLEFT,
    EMP_SLIDE_SHOW_EFFECT_HORZ_BLOCKS_BOTTOMRIGHT,
    EMP_SLIDE_SHOW_EFFECT_VERT_BLOCKS_TOPLEFT,
    EMP_SLIDE_SHOW_EFFECT_VERT_BLOCKS_TOPRIGHT,
    EMP_SLIDE_SHOW_EFFECT_VERT_BLOCKS_BOTTOMLEFT,
    EMP_SLIDE_SHOW_EFFECT_VERT_BLOCKS_BOTTOMRIGHT,
    EMP_SLIDE_SHOW_EFFECT_HORZ_ZIGZAG_BLOCKS_TOPLEFT,
    EMP_SLIDE_SHOW_EFFECT_HORZ_ZIGZAG_BLOCKS_TOPRIGHT,//10
    EMP_SLIDE_SHOW_EFFECT_HORZ_ZIGZAG_BLOCKS_BOTTOMLEFT,
    EMP_SLIDE_SHOW_EFFECT_HORZ_ZIGZAG_BLOCKS_BOTTOMRIGHT,
    EMP_SLIDE_SHOW_EFFECT_VERT_ZIGZAG_BLOCKS_TOPLEFT,
    EMP_SLIDE_SHOW_EFFECT_VERT_ZIGZAG_BLOCKS_TOPRIGHT,
    EMP_SLIDE_SHOW_EFFECT_VERT_ZIGZAG_BLOCKS_BOTTOMLEFT,
    EMP_SLIDE_SHOW_EFFECT_VERT_ZIGZAG_BLOCKS_BOTTOMRIGHT,
    EMP_SLIDE_SHOW_EFFECT_BLOCKS_RANDOM,//17
#endif
#if (ENABLE_PHOTO_TRANS_EFFECT_DIAGONAL_BLOCKS)
    EMP_SLIDE_SHOW_EFFECT_DIAGONAL_BLOCKS_TOPLEFT,
    EMP_SLIDE_SHOW_EFFECT_DIAGONAL_BLOCKS_TOPRIGHT,
    EMP_SLIDE_SHOW_EFFECT_DIAGONAL_BLOCKS_BOTTOMLEFT, //20
    EMP_SLIDE_SHOW_EFFECT_DIAGONAL_BLOCKS_BOTTOMRIGHT,
#endif
#if (ENABLE_PHOTO_TRANS_EFFECT_WIPE)
    EMP_SLIDE_SHOW_EFFECT_BARWIPE_LEFT2RIGHT,//22
    EMP_SLIDE_SHOW_EFFECT_BARWIPE_RIGHT2LEFT,//23
    EMP_SLIDE_SHOW_EFFECT_BARWIPE_TOP2BOTTOM,//24
    EMP_SLIDE_SHOW_EFFECT_BARWIPE_BOTTOM2TOP,//25
    EMP_SLIDE_SHOW_EFFECT_BOXWIPE_TOPLEFT,//26
    EMP_SLIDE_SHOW_EFFECT_BOXWIPE_TOPRIGHT,//27
    EMP_SLIDE_SHOW_EFFECT_BOXWIPE_BOTTOMLEFT,//28
    EMP_SLIDE_SHOW_EFFECT_BOXWIPE_BOTTOMRIGHT,//29
    EMP_SLIDE_SHOW_EFFECT_BARNDOORWIPE_HORIZONTAL_OPEN,//30
    EMP_SLIDE_SHOW_EFFECT_BARNDOORWIPE_HORIZONTAL_CLOSE,//31
    EMP_SLIDE_SHOW_EFFECT_BARNDOORWIPE_VERTICAL_OPEN,//32
    EMP_SLIDE_SHOW_EFFECT_BARNDOORWIPE_VERTICAL_CLOSE,//33
    EMP_SLIDE_SHOW_EFFECT_BOXWIPE_TOPCENTER,//34
    EMP_SLIDE_SHOW_EFFECT_BOXWIPE_RIGHTCENTER,//35
    EMP_SLIDE_SHOW_EFFECT_BOXWIPE_BOTTOMCENTER,//36
    EMP_SLIDE_SHOW_EFFECT_BOXWIPE_LEFTCENTER,//37
    EMP_SLIDE_SHOW_EFFECT_IRISWIPE_CENTER,//38
    EMP_SLIDE_SHOW_EFFECT_IRISWIPE_OUTER,//39
    EMP_SLIDE_SHOW_EFFECT_4BARWIPE_HORIZONTAL_FROM_TOP,//40
    EMP_SLIDE_SHOW_EFFECT_4BARWIPE_HORIZONTAL_FROM_BOTTOM,
    EMP_SLIDE_SHOW_EFFECT_4BARWIPE_VERTICAL_FROM_LEFT,
    EMP_SLIDE_SHOW_EFFECT_4BARWIPE_VERTICAL_FROM_RIGHT,//43
#endif
#if (ENABLE_PHOTO_TRANS_EFFECT_SLIDE)
    EMP_SLIDE_SHOW_EFFECT_BARSLIDE_LEFT2RIGHT,//44
    EMP_SLIDE_SHOW_EFFECT_BARSLIDE_RIGHT2LEFT,
    EMP_SLIDE_SHOW_EFFECT_BARSLIDE_TOP2BOTTOM,
    EMP_SLIDE_SHOW_EFFECT_BARSLIDE_BOTTOM2TOP,
    EMP_SLIDE_SHOW_EFFECT_BOXSLIDE_TOPLEFT,
    EMP_SLIDE_SHOW_EFFECT_BOXSLIDE_TOPRIGHT,
    EMP_SLIDE_SHOW_EFFECT_BOXSLIDE_BOTTOMLEFT,//50
    EMP_SLIDE_SHOW_EFFECT_BOXSLIDE_BOTTOMRIGHT,
    EMP_SLIDE_SHOW_EFFECT_BARNDOORSLIDE_HORIZONTAL_OPEN,
    EMP_SLIDE_SHOW_EFFECT_BARNDOORSLIDE_HORIZONTAL_CLOSE,
    EMP_SLIDE_SHOW_EFFECT_BARNDOORSLIDE_VERTICAL_OPEN,
    EMP_SLIDE_SHOW_EFFECT_BARNDOORSLIDE_VERTICAL_CLOSE,
    EMP_SLIDE_SHOW_EFFECT_4BARSLIDE_HORIZONTAL_FROM_TOP,
    EMP_SLIDE_SHOW_EFFECT_4BARSLIDE_HORIZONTAL_FROM_BOTTOM,
    EMP_SLIDE_SHOW_EFFECT_4BARSLIDE_VERTICAL_FROM_LEFT,
    EMP_SLIDE_SHOW_EFFECT_4BARSLIDE_VERTICAL_FROM_RIGHT,//59
#endif
#if (ENABLE_PHOTO_TRANS_EFFECT_FADE_IN)
    EMP_SLIDE_SHOW_EFFECT_FADE_IN,
#endif
    EMP_SLIDE_SHOW_EFFECT_RANDOM,
    EMP_SLIDE_SHOW_EFFECT_CLOSE,
} EMP_PHOTO_SLIDE_SHOW_EFFECT_T;
#endif //#if PHOTO_SLIDE_SHOW_EFFECT

#ifdef SUPPORT_DIVX_HD_PLUS

#if 0
typedef struct EMP_DIVXPLUSHD_TITLE
{
	UINT32 titleID;
	UINT8 titleStringBuffer;
	UINT8 *pTitleString;
}EMP_DIVXPLUSHD_TITLE_T;

typedef struct EMP_DIVXPLUSHD_CHAPTER
{
	UINT32 chapterID;
	UINT32 chapterTimeStart;
	UINT32 chapterTimeEnd;
	UINT8 chapStringBufferSize;
	UINT8* pChapString;
}EMP_DIVXPLUSHD_CHAPTER_T;


typedef struct EMP_DIVXPLUSHD_EDITION
{
	UINT32 editionID;
	UINT32 editionOrdered;
	UINT8 chapNum;
	EMP_DIVXPLUSHD_CHAPTER_T *pSubElement;
}EMP_DIVXPLUSHD_EDITION_T;


typedef struct EMP_DIVXPLUSHD_METADATA
{
	EMP_DIVXPLUSHD_TITLE_T title;
	EMP_DIVXPLUSHD_EDITION_T edition;
	EMP_DIVXPLUSHD_CHAPTER_T chapter;

}EMP_DIVXPLUSHD_METADATA_T;
#else

/**
 * struct for divxplus hd  chapter.
*/
typedef struct tagEMP_DIVXPLUSHD_CHAPTER
{
	UINT64	chapterID;
	UINT32	chapterTimeStart;
	UINT32	chapterTimeEnd;
	UINT8	chapStringLen;
	UINT8	*pChapString;
}EMP_DIVXPLUSHD_CHAPTER_T;

/**
 * struct for divxplus hd  edition.
*/
typedef struct tagEMP_DIVXPLUSHD_EDITION
{
	UINT64	editionID;
	UINT32	editionOrdered;
	UINT8	chapNum;
	EMP_DIVXPLUSHD_CHAPTER_T *chapter;
}EMP_DIVXPLUSHD_EDITION_T;

/**
 * struct for divxplus hd  title item.
*/
typedef struct tagEMP_DIVXPLUSHD_TITLE_ITEM
{
	UINT64	titleID;
	UINT8	titleStringLen;
	UINT8	*pTitleString;
	UINT8	editionNum;
	EMP_DIVXPLUSHD_EDITION_T *edition;
}EMP_DIVXPLUSHD_TITLE_ITEM_T;

/**
 * struct for divxplus hd  title.
*/
typedef struct tagEMP_DIVXPLUSHD_TITLE
{
	UINT8	titleNum;
	EMP_DIVXPLUSHD_TITLE_ITEM_T *titleItem;
}EMP_DIVXPLUSHD_TITLE_T;



#endif

/**
 * divxplus hd  metadata enum.
*/
typedef enum
{
	EMP_DIVXPLUSHD_METADATA_TITLE =0,
	EMP_DIVXPLUSHD_METADATA_EDITION,
	EMP_DIVXPLUSHD_METADATA_CHAPTER,
}EMP_DIVXPLUSHD_METADATA_TYPE_T;
#endif //SUPPORT_DIVX_HD_PLUS



/******************************************************************************
    함수 선언 (Function Declaration)
******************************************************************************/
DTV_STATUS_T DDI_EMP_Initialize( EMP_MEDIA_TYPE_T mediaType, UINT32 useJpegDecoderBuf);

DTV_STATUS_T DDI_EMP_Finalize(EMP_MEDIA_TYPE_T mediaType);  // kjoh LGE 20081118

DTV_STATUS_T DDI_EMP_CreateMediaItem(UINT8 *pMediaPath, EMP_MEDIA_ITEM_T *pItem);

DTV_STATUS_T DDI_EMP_DeleteMediaItem(EMP_MEDIA_ITEM_T pItem);

#if (PLATFORM_TYPE == MSTAR_PLATFORM)

DTV_STATUS_T DDI_EMP_RegisterCallback(EMP_MEDIA_ITEM_T pItem, EMP_MEDIA_TYPE_T type, pfnEMPCallback_T pfnHandler);

#elif (PLATFORM_TYPE == BCM_PLATFORM)
DTV_STATUS_T DDI_EMP_RegisterCallback(EMP_MEDIA_ITEM_T pItem, EMP_MEDIA_TYPE_T type, pfnEMPCallback_T pfnHandler);
DTV_STATUS_T DDI_EMP_SetVideoPlayMode(BOOLEAN bCheckPlayMode);		//jm_kim 20091001; Temporary code to set a play mode. This code is for getting elapsed time in streaming mode

#ifdef SUPPORT_DIVX_HD_PLUS
DTV_STATUS_T DDI_EMP_GetDivXPlusHDMetaData(EMP_MEDIA_ITEM_T pItem,EMP_DIVXPLUSHD_METADATA_TYPE_T metaDataType, void *pMetaData);
DTV_STATUS_T DDI_EMP_SetChapter( EMP_MEDIA_ITEM_T pItem, UINT32 number);
DTV_STATUS_T DDI_EMP_SetEdition( EMP_MEDIA_ITEM_T pItem, UINT32 number);
DTV_STATUS_T DDI_EMP_CheckAutoChapter(EMP_MEDIA_ITEM_T pItem);
DTV_STATUS_T DDI_EMP_SetTitle( EMP_MEDIA_ITEM_T pItem, UINT32 number);
DTV_STATUS_T DDI_EMP_LoadFontdata(EMP_MEDIA_ITEM_T pItem, unsigned char **fontData, int *size);


#endif	//SUPPORT_DIVX_HD_PLUS

#endif




DTV_STATUS_T DDI_EMP_RegisterSwDecCallback(pfnEMPSwDecCallback_T pfnHandler);

DTV_STATUS_T DDI_EMP_Play(EMP_MEDIA_ITEM_T pItem);

DTV_STATUS_T DDI_EMP_PreviewVideo(EMP_MEDIA_ITEM_T pItem, UINT16 x, UINT16 y, UINT16 width, UINT16 height);

DTV_STATUS_T DDI_EMP_Stop(EMP_MEDIA_ITEM_T pItem);

DTV_STATUS_T DDI_EMP_Pause(EMP_MEDIA_ITEM_T pItem);

DTV_STATUS_T DDI_EMP_Resume(EMP_MEDIA_ITEM_T pItem);

DTV_STATUS_T DDI_EMP_Rotate(EMP_MEDIA_ITEM_T pItem, EMP_ROTATION_T rotation);

//DTV_STATUS_T DDI_EMP_GetThumbNail(EMP_MEDIA_ITEM_T pItem, UINT32 *pWidth, UINT32 *pHeight, EMP_PIXEL_FORMAT_T format, UINT32 *pBuf);
#if (MAIN_CHIP_TYPE == CHIP_SATURN7) || (MAIN_CHIP_TYPE == CHIP_SATURN8)
//DTV_STATUS_T DDI_EMP_GetThumbNail(EMP_MEDIA_ITEM_T pItem, UINT32 *pWidth, UINT32 *pHeight, EMP_PIXEL_FORMAT_T format, UINT32 *pBuf);	// geoffrey@lge.com for compile error

//DTV_STATUS_T DDI_EMP_GetThumbNail (EMP_MEDIA_ITEM_T item, UINT32* pJpgData, UINT32* pSize, UINT16* pWidth, UINT16* pHeight, EMP_PIXEL_FORMAT_T fpFormat, UINT32* buf);
DTV_STATUS_T DDI_EMP_GetThumbNail (EMP_MEDIA_ITEM_T pItem, UINT32* pJpgData, UINT32* pSize, UINT16* pWidth, UINT16* pHeight, EMP_PIXEL_FORMAT_T fpFormat);
#else
DTV_STATUS_T DDI_EMP_GetThumbNail ( EMP_MEDIA_ITEM_T pItem, UINT32* pJpgData, UINT32* pSize, UINT16* pWidth, UINT16* pHeight, EMP_PIXEL_FORMAT_T fpFormat);
#endif
DTV_STATUS_T DDI_EMP_PlayThumbNail(UINT32 x, UINT32 y, UINT32 width, UINT32 height, EMP_PIXEL_FORMAT_T format, UINT32* pBuf);
DTV_STATUS_T DDI_EMP_YUV2ARGB888(UINT32 u32SrcWidth, UINT32 u32SrcHeight, UINT32 u32SrcAddr,UINT32 u32DstWidth, UINT32 u32DstHeight, UINT32 u32DstAddr);
DTV_STATUS_T DDI_EMP_GetMetaData(EMP_MEDIA_ITEM_T pItem, EMP_METADATA_T meta, UINT8 *buf, UINT8 bufsize);

DTV_STATUS_T DDI_EMP_GetPlayPosition(EMP_MEDIA_ITEM_T pItem, UINT32 *pPosition);

DTV_STATUS_T DDI_EMP_SetPlayPosition(EMP_MEDIA_ITEM_T pItem, UINT32 position);

DTV_STATUS_T DDI_EMP_SetTrickPlayMode(EMP_TRICK_MODE_PARAM_T param);

DTV_STATUS_T DDI_EMP_GetTrickPlayMode(EMP_TRICK_MODE_PARAM_T *pParam);
BOOLEAN DDI_EMP_GetPlayStatus( void );

DTV_STATUS_T DDI_EMP_SetDisplayRectangle(UINT32 x, UINT32 y, UINT32 width, UINT32 height);
DTV_STATUS_T DDI_EMP_GetDisplayRectangle(UINT32 *x, UINT32 *y, UINT32 *width, UINT32 *height);


DTV_STATUS_T DDI_EMP_SetViewFrame(EMP_VIEWFRAME_T viewFrame);
#if 1//(PLATFORM_TYPE==BCM_PLATFORM) 100212 yunjoo.kim
DTV_STATUS_T DDI_EMP_SetExternalSubtitleSettings(EMP_EXT_SUBTITLE_SETTINGS_T settingType, UINT8 settingValue);
DTV_STATUS_T DDI_EMP_GetExternalSubtitleSettings(EMP_EXT_SUBTITLE_SETTINGS_T settingType, UINT8 *pSettingValue);
DTV_STATUS_T DDI_EMP_SetInternalSubtitleSettings(EMP_INT_SUBTITLE_SETTINGS_T settingType, UINT8 settingValue);
DTV_STATUS_T DDI_EMP_GetInternalSubtitleSettings(EMP_INT_SUBTITLE_SETTINGS_T settingType, UINT8 *pSettingValue);
#else
DTV_STATUS_T DDI_EMP_SetExternalSubtitleSettings(EMP_EXT_SUBTITLE_SETTINGS_T settingType, UINT8 settingValue);
DTV_STATUS_T DDI_EMP_GetExternalSubtitleSettings(EMP_EXT_SUBTITLE_SETTINGS_T settingType, UINT8 *pSettingValue);
DTV_STATUS_T DDI_EMP_SetInternalSubtitleSettings(EMP_INT_SUBTITLE_SETTINGS_T settingType, UINT8 settingValue);
DTV_STATUS_T DDI_EMP_GetInternalSubtitleSettings(EMP_INT_SUBTITLE_SETTINGS_T settingType, UINT8 *pSettingValue);

#endif
DTV_STATUS_T DDI_EMP_GetAudioLanguageCount(UINT8 *pLangCnt);

DTV_STATUS_T DDI_EMP_SetAudioLanguage(UINT8 langIndex);

DTV_STATUS_T DDI_EMP_GetAudioLanguage(UINT8 *pLangIndex);

DTV_STATUS_T DDI_EMP_GetMediaType(EMP_MEDIA_ITEM_T pItem, EMP_MEDIA_TYPE_T *pType);
#if (PLATFORM_TYPE==BCM_PLATFORM)
DTV_STATUS_T DDI_EMP_SetExtSubt(BOOLEAN bExist);
#endif

void DDI_EMP_ClearFrameBuffer(void);

UINT8 *DDI_EMP_GetFrameBuffer(void);
UINT8* DDI_EMP_GetRotateBuffer (void);

DTV_STATUS_T DDI_EMP_FlushFrameBuffer(UINT8* pFrame);

void DDI_EMP_SetDoubleDraw(int temp);


BOOLEAN DDI_EMP_IsHwDecodingSupported(EMP_MEDIA_ITEM_T pItem);

#ifdef INCLUDE_NEW_WMDRMPD

#if 1
typedef unsigned char           DRM_BYTE_T;
typedef char                    DRM_CHAR_T;
typedef unsigned short          DRM_WORD_T, DRM_WCHAR_T;
typedef unsigned long           DRM_DWORD_T, DRM_ULONG_T;

#endif
#define PK_ENC_PRIVATE_KEY_LEN	20
#define __CB_DECL(x) (x)

/**
 * brief description.
 * detailed description. Private Key Get struct.
*/

typedef struct tagPRIVKEY
{
    DRM_BYTE_T x[ __CB_DECL(PK_ENC_PRIVATE_KEY_LEN) ];
} PRIVKEY_T;

typedef BOOLEAN(*pPFN_WMDRM_FEEDDATA_CB)(unsigned char *pData);
typedef BOOLEAN(*pPFN_WMDRM_CALLBACK_CB)(void);

typedef int (*pEMP_WMDRM_Payload_Callback_T)(void *pdata,size_t len);
extern void DDI_EMP_WMDRM_RegisterCallback(pEMP_WMDRM_Payload_Callback_T *gregistCallback);
extern void DDI_EMP_WMDRM_DestroyCallback(void);
extern void DDI_EMP_WMDRM_SetStatus(BOOLEAN wmdrmstatus);
extern BOOLEAN DDI_EMP_WMDRM_GetStatus(void);

extern void DDI_EMP_WMDRM_RegisterCallbackCall(pPFN_WMDRM_CALLBACK_CB *WMDRM_CB );

DTV_STATUS_T DDI_EMP_WMDRM_FeedDataRegisterCB(pPFN_WMDRM_FEEDDATA_CB *WMDRM_CB);
DTV_STATUS_T DDI_EMP_WMDRM_SetDmaHandle(UINT32 dmaHandle);
DTV_STATUS_T DDI_EMP_WMDRM_SetSecureStoreGlobalPasswordSeed(DRM_CHAR_T *pszPath);
DTV_STATUS_T DDI_EMP_WMDRM_GetSecureStoreGlobalPasswordSeed(DRM_CHAR_T *pszPath,DRM_BYTE_T *pbSecStorePassword, DRM_DWORD_T *cbSecStorePassword);

DTV_STATUS_T DDI_EMP_WMDRM_Get_DeviceCert (DRM_CHAR_T *pszPath, DRM_BYTE_T *pbDevCert,   DRM_DWORD_T cbDevCert);
DTV_STATUS_T DDI_EMP_WMDRM_Set_DeviceCert (DRM_CHAR_T *pszPath, DRM_BYTE_T *pbDevCert,   DRM_DWORD_T cbDevCert);
DTV_STATUS_T DDI_EMP_WMDRM_Get_PrviateKey(DRM_CHAR_T *pszPath,DRM_CHAR_T drmpath_len,PRIVKEY_T *f_pPrivkey);
DTV_STATUS_T DDI_EMP_WMDRM_FeedStreamData(UINT8 *pBuf);
//DTV_STATUS_T DDI_EMP_WMDRM_RegisterPayload_Callback(NEXUS_PlaypumpHandle   playpumpHandle);
//DTV_STATUS_T DDI_EMP_WMDRM_UnRegisterPayload_Callback(NEXUS_PlaypumpHandle   playpumpHandle);

#endif

DTV_STATUS_T DDI_EMP_SetDRMPath(const char *pPath);
DTV_STATUS_T DDI_EMP_IsDRMMediaItem(EMP_MEDIA_ITEM_T pItem, BOOLEAN *pIsDRMFile);
DTV_STATUS_T DDI_EMP_GetDRMRegistrationCode(UINT8 *pDRMCode);
DTV_STATUS_T DDI_EMP_DeactivationDRMDevice(UINT8 *pDRMCode);
DTV_STATUS_T DDI_EMP_GetDRMMediaItemStatus(EMP_MEDIA_ITEM_T pItem, EMP_DRM_STATUS_T  *pReturnStatus); //jm_kim 20081117
DTV_STATUS_T DDI_EMP_QueryDRMRentalStatus(EMP_MEDIA_ITEM_T pItem, BOOLEAN * pIsRental, UINT8 *pViewLimit, UINT8 *pViewCount);
void DDI_EMP_ClearDRMMemory(void); //zuel_081205
DTV_STATUS_T DDI_EMP_IsActivatedDevice(UINT8 *pIsActivation);


#if (PLATFORM_TYPE == BCM_PLATFORM)
DTV_STATUS_T DDI_EMP_CheckDRMAuthorization(EMP_MEDIA_ITEM_T pItem);
DTV_STATUS_T DDI_EMP_GetMp3ThumbData(UINT8 *pFullPath, UINT8 *pJpgData, UINT32 *pSize);
BOOLEAN DDI_EMP_CheckMemoryBandwidth(EMP_MEDIA_ITEM_T pItem);

#endif

DTV_STATUS_T DDI_EMP_ShowMp3Jpeg(EMP_MEDIA_ITEM_T pItem);    // geoffrey for warning
DTV_STATUS_T DDI_EMP_ReleaseMp3Jpeg(EMP_MEDIA_ITEM_T pItem); // geoffrey for warning
DTV_STATUS_T DDI_EMP_CheckMp3Jpeg(EMP_MEDIA_ITEM_T item);   // geoffrey 20081014
DTV_STATUS_T DDI_EMP_HeaderParserMode( EN_EMP_PREVIDE_MODE PrevideMode);

DTV_STATUS_T DDI_EMP_SetPlayRepeat(EMP_MEDIA_ITEM_T pItem, BOOLEAN bRepeat);	//jm_kin 20090831, This function should be implemented
DTV_STATUS_T DDI_EMP_FlushStream(EMP_MEDIA_ITEM_T pItem);	//jm_kin 20090901, This function should be implemented

void DDI_EMP_SetStreamResumePosition(UINT32 position);

#if PHOTO_SLIDE_SHOW_EFFECT
extern DTV_STATUS_T DDI_EMP_SetSlideShowEffect(EMP_PHOTO_SLIDE_SHOW_EFFECT_T eType);
EMP_PHOTO_SLIDE_SHOW_EFFECT_T DDI_EMP_GetSlideShowEffect(void);
extern DTV_STATUS_T DDI_EMP_SlideShow(void);
#endif //#if PHOTO_SLIDE_SHOW_EFFECT

extern DTV_STATUS_T DDI_EMP_ReadDRMInf(UINT32 offset, UINT32 nData, void *pBuf);
extern DTV_STATUS_T DDI_EMP_WriteDRMInf(UINT32 offset, UINT32 nData, void *pBuf);

// Samuel, bad HDD timeout, 090115
extern int g_timeout_state ;
extern int g_timeout_cnt ;


extern DTV_STATUS_T DDI_EMP_SetResumePlay(EMP_MEDIA_ITEM_T item, UINT64 u64SeekPos , UINT32 u32SeekPts);
extern DTV_STATUS_T DDI_EMP_GetResumePlay(EMP_MEDIA_ITEM_T item,UINT64 *u64SeekPos , UINT32 *u32SeekPts);
extern void DDI_EMP_SetStreamResumeEnable(BOOLEAN set);
extern BOOLEAN DDI_EMP_GetStreamResumeEnable(void);
extern EMP_MEDIA_ITEM_T DDI_EMP_GetStreamLastPlayItem(void);

extern DTV_STATUS_T DDI_EMP_FeedStream (EMP_MEDIA_ITEM_T item, UINT8 *pBuf, UINT32 bufLen, UINT32 *pNumOfBytesFed, EMP_FEED_OPT_T feedOpt);
#if (PLATFORM_TYPE == MSTAR_PLATFORM)
extern DTV_STATUS_T DDI_EMP_SetPictureMoveDirection ( EMP_MEDIA_ITEM_T pItem, EMP_PICTURE_MOVE_DIRECTION_T picDirect );
extern DTV_STATUS_T DDI_EMP_SetEnlargement ( EMP_MEDIA_ITEM_T pItem, EMP_PICTURE_ENLARGEMENT_RATIO_T EnlarageRatio );
extern DTV_STATUS_T DDI_EMP_RegisterStreamCallback (EMP_MEDIA_ITEM_T item, EMP_STREAM_PFNHANDLER_T *pfnhandler);
//extern DTV_STATUS_T DDI_EMP_RegisterStreamCallback (EMP_MEDIA_ITEM_T pItem, EMP_STREAM_PFNHANDLER_T *pfnhandler);
//extern DTV_STATUS_T DDI_EMP_CreateStreamMediaItem(EMP_MEDIA_ITEM_T *pItem, EMP_MEDIA_TYPE_T mediaType, EMP_STREAM_OBJECT_INFO_T *pObjInfo,EMP_STREAM_FLOW_MODE_T flowMode);
extern DTV_STATUS_T	DDI_EMP_CreateStreamMediaItem(UINT8 *pMediaPath, EMP_MEDIA_ITEM_T *pItem, EMP_STREAM_MODE_T mode, EMP_STREAM_OBJECT_INFO_T *pObjInfo);
extern DTV_STATUS_T DDI_EMP_GetMp3Thumbnail(EMP_MEDIA_ITEM_T item ,UINT32 *pJpgData, UINT32 *pSize,UINT16 *pWidth, UINT16 *pHeight);
#if (MAIN_CHIP_TYPE == CHIP_SATURN7) || (MAIN_CHIP_TYPE == CHIP_SATURN8)
//extern DTV_STATUS_T DDI_EMP_GetDivxThumbnail(EMP_MEDIA_ITEM_T pItem, UINT8 *pJpgData, UINT16 *pImgW, UINT16 *pImgH);	// geoffrey@lge.com // for compile error
extern DTV_STATUS_T DDI_EMP_GetDivxThumbnail (EMP_MEDIA_ITEM_T item, UINT32* pJpgData, UINT32* pSize, UINT16* pWidth, UINT16* pHeight);
//DTV_STATUS_T DDI_EMP_GetDivxThumbnail (EMP_MEDIA_ITEM_T item, UINT32* pJpgData, UINT32* pSize, UINT16* pWidth, UINT16* pHeight, int time);
UINT32				DDI_EMP_GetCurrent_MapVideoCodec4UI(void);
UINT32				DDI_EMP_GetCurrent_MapAudioCodec4UI(void);
extern DTV_STATUS_T	DDI_EMP_StreamUnknown(EMP_MEDIA_ITEM_T	pItem);


extern DTV_STATUS_T      DD_EMP_IsDRMStream(EMP_MEDIA_ITEM_T pItem, BOOLEAN *pIsDrm);
extern DTV_STATUS_T      DDI_EMP_IsDRMStream(EMP_MEDIA_ITEM_T pItem);

#else
//extern DTV_STATUS_T DDI_EMP_GetDivxThumbnail ( EMP_MEDIA_ITEM_T item, UINT32* pJpgData, UINT32* pSize, UINT16* pWidth, UINT16* pHeight, int time);
extern DTV_STATUS_T DDI_EMP_GetDivxThumbnail(EMP_MEDIA_ITEM_T pItem, UINT8 **pJpgData, UINT32 *pImgW, UINT32 *pImgH, int time);

#endif
extern DTV_STATUS_T DDI_EMP_GetJpegXYWH(EMP_MEDIA_ITEM_T item, UINT16 *pImgX, UINT16 *pImgY, UINT16 *pImgW, UINT16 *pImgH);

#ifdef USE_STREAMBUFFERHANDLER_THREAD
extern DTV_STATUS_T	DD_EMP_StreamInitialize(EMP_MEDIA_TYPE_T emfType);
extern DTV_STATUS_T	DD_EMP_StreamResourceInit(UINT32 index);
extern DTV_STATUS_T DDI_EMP_StreamPlay(EMP_MEDIA_ITEM_T pItem);
#endif

#elif (PLATFORM_TYPE == BCM_PLATFORM)

extern DTV_STATUS_T DDI_EMP_GetDivxThumbnail(EMP_MEDIA_ITEM_T pItem, UINT8 **pJpgData, UINT32 *pImgW, UINT32 *pImgH, int *time);

DTV_STATUS_T DDI_EMP_StartSlideShow( EMP_MEDIA_ITEM_T pItem );
//DTV_STATUS_T DDI_EMP_StartSlideShow( char *pszFilePath );
DTV_STATUS_T DDI_EMP_StopSlideShow(void);






/**
 * Stream Message
*/
typedef enum {

	EMP_STREAM_MSG_PLAY_START =0,
	EMP_STREAM_MSG_PLAY_STOP,
	EMP_STREAM_MSG_PLAY_RESUME,
	EMP_STREAM_MSG_PLAY_PAUSE,
	EMP_STREAM_MSG_PLAY_JUMP,
	EMP_STREAM_MSG_PLAY_UNKNOWN,
	EMP_STREAM_MSG_PLAY_DRM_CHECK,
	EMP_STREAM_MSG_MAX,
}EMP_STREAM_MSG_T;

/**
 * Stream Status
*/
typedef enum{
	EMP_STREAM_PB_STATUS_INIT = 0,
	EMP_STREAM_PB_STATUS_PROVEN,
	EMP_STREAM_PB_STATUS_PLAY,
	EMP_STREAM_PB_STATUS_STOP,
	EMP_STREAM_PB_STATUS_UNKNOWN

}EMP_STREAM_PB_STATUS_T;

/**
 * Audio Clip Status
*/
typedef enum{
	EMP_AUDIO_CLIP_STATUS_NONE = 0,
	EMP_AUDIO_CLIP_STATUS_PLAY,
	EMP_AUDIO_CLIP_STATUS_PAUSE,
	EMP_AUDIO_CLIP_STATUS_DONE
}EMP_AUDIO_CLIP_STATUS_T;

/**
 * Stream Message Type
*/
typedef struct {

	EMP_STREAM_MSG_T	msgID;
	EMP_MEDIA_TYPE_T	mediaType;
	EMP_MEDIA_ITEM_T	pItem;
} EMP_STREAM_MSG_TYPE_T;

/**
 * Stream Protocol Type
*/
typedef enum {
	EMP_STREAM_PROTOCOL_UNKNOWN=0,
	EMP_STREAM_PROTOCOL_HTTP,
	EMP_STREAM_PROTOCOL_MMS,
	EMP_STREAM_PROTOCOL_RTSP
} EMP_STREAM_PROTOCOL_TYPE_T;

extern void DDI_EMP_SetStreamPlayProtocolType( EMP_STREAM_PROTOCOL_TYPE_T procotol);
extern EMP_STREAM_PROTOCOL_TYPE_T DDI_EMP_GetStreamPlayProtocolType( void);

extern	DTV_STATUS_T	DDI_EMP_StreamInitialize(EMP_MEDIA_TYPE_T	 emfType);
extern	DTV_STATUS_T	DDI_EMP_StreamFinalize(EMP_MEDIA_TYPE_T	 emfType);
extern	DTV_STATUS_T	DDI_EMP_StreamPlay(EMP_MEDIA_ITEM_T	pItem);
extern	DTV_STATUS_T	DDI_EMP_StreamStop(EMP_MEDIA_ITEM_T	pItem);
extern	DTV_STATUS_T	DDI_EMP_StreamResume(EMP_MEDIA_ITEM_T	pItem);
extern	DTV_STATUS_T	DDI_EMP_StreamPause(EMP_MEDIA_ITEM_T pItem);
extern	DTV_STATUS_T	DDI_EMP_CreateStreamMediaItem(UINT8 *pMediaPath, EMP_MEDIA_ITEM_T *pItem, UINT8 mode, EMP_STREAM_OBJECT_INFO_T *pObjInfo);
extern	DTV_STATUS_T	DDI_EMP_StreamDeleteMediaItem(EMP_MEDIA_ITEM_T pItem);
extern	DTV_STATUS_T	DDI_EMP_RegisterStreamCallback(EMP_STREAM_PFNHANDLER_T	*pfnhandler, EMP_MEDIA_TYPE_T emfType);
extern	DTV_STATUS_T	DDI_EMP_StreamGetMetaData( EMP_MEDIA_ITEM_T pItem, EMP_METADATA_T meta, UINT8 *pBuf, UINT8 bufsize );
extern	DTV_STATUS_T 	DDI_EMP_StreamSetPlayPosition( EMP_MEDIA_ITEM_T pItem, UINT32 position);
extern	DTV_STATUS_T	DDI_EMP_GetCapturedVideoImage(EMP_MEDIA_ITEM_T item, UINT8 **pBuf, UINT32 *len);
extern	DTV_STATUS_T	DDI_EMP_FreeCapturedVideoImage(EMP_MEDIA_ITEM_T item, UINT8 *pBuf);

#ifdef INCLUDE_FLASH_BR
extern DTV_STATUS_T		DDI_EMP_FlashESCreateStreamMediaItem(EMP_MEDIA_ITEM_T *pItem, EMP_STREAM_OBJECT_INFO_T *pObjInfo);
extern DTV_STATUS_T 	DDI_EMP_FlashESDeleteMediaItem(EMP_MEDIA_ITEM_T pItem);
extern DTV_STATUS_T		DDI_EMP_FlashESPlay(EMP_MEDIA_ITEM_T pItem);
extern DTV_STATUS_T		DDI_EMP_FlashESResume(EMP_MEDIA_ITEM_T	pItem);
extern DTV_STATUS_T		DDI_EMP_FlashESPause(EMP_MEDIA_ITEM_T pItem);
extern DTV_STATUS_T		DDI_EMP_FlashESFeedStream(EMP_MEDIA_ITEM_T item, UINT8 *pBuf, UINT32 bufLen, UINT32 *pNumOfBytesFed, UINT32 pts);
extern DTV_STATUS_T		DDI_EMP_FlashESStop(EMP_MEDIA_ITEM_T pItem);
extern DTV_STATUS_T		DDI_EMP_FlashESRegisterCB(EMP_MEDIA_ITEM_T pItem, FLASHES_BUFFER_CB_T cb);
extern DTV_STATUS_T 	DDI_EMP_FlashESGetCurrentPTSLocked(EMP_FLASHES_CHANNEL_T channel, SINT64 *lockedPTS, BOOLEAN *decoderTime);
extern DTV_STATUS_T 	DDI_EMP_FlashESGetBufferLevel(EMP_FLASHES_CHANNEL_T channel, UINT32 *bufferLevel);
#endif

extern DTV_STATUS_T	DDI_EMP_StreamUnknown(EMP_MEDIA_ITEM_T	pItem);
UINT32				DDI_EMP_GetCurrent_MapVideoCodec4UI(void);
UINT32				DDI_EMP_GetCurrent_MapAudioCodec4UI(void);

#if (PLATFORM_TYPE == BCM_PLATFORM)
extern DTV_STATUS_T      DD_EMP_IsDRMStream(EMP_MEDIA_ITEM_T pItem, BOOLEAN *pIsDrm);
extern DTV_STATUS_T      DDI_EMP_IsDRMStream(EMP_MEDIA_ITEM_T pItem);
#endif

extern	DTV_STATUS_T	DDI_EMP_BCM_OpenDmaHandle(void);
extern	DTV_STATUS_T	DDI_EMP_BCM_CloseDmaHandle(void);
extern	DTV_STATUS_T	DDI_EMP_BCM_SetDmaHandle(UINT32 dmaHandle);
extern	DTV_STATUS_T	DDI_EMP_BCM_GetDmaHandle(UINT32 *pDmaHandle);
extern	DTV_STATUS_T	DDI_EMP_SetPushStreamInfo(EMP_MEDIA_ITEM_T	pItem, EMP_MOVIE_INFO_T mediaInfo);

#endif // end of #if (PLATFORM_TYPE == MSTAR_PLATFORM) else part #elif (PLATFORM_TYPE == MSTAR_PLATFORM)


#if !(PLATFORM_TYPE == BCM_PLATFORM)
#ifdef MSTAR_GP3
#undef SYS_ATSC
#undef PLATFORM_TYPE
#undef BCM_PLATFORM
#endif
#endif

#endif /*_EMP_DDI_H_ */

