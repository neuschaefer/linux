/******************************************************************************
/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file	pvr_ddi.h
 *
 *  Header file for PVR DDI.
 *
 *  @author   Min Hyoung Kim (a9a9@lge.com)
 *  @version  0.5
 *  @date     2008.02.26
 *
 *  @note      none
 *  @see
 */

#ifndef	_PVR_DDI_H_
#define	_PVR_DDI_H_

#include "common.h"
#include "osa_api.h"

#include "sdec_ddi.h"
#include "vdec_ddi.h"
#include "adec_ddi.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define NORMAL_UPLOAD_SPEED		(100)
#define UPLOAD_SPEED_INCREMENT	(20)
#define DOWNLOAD_SLEEP_PREIOD	10 // 10 ms

/**
 * Frame rate
 *
*/
typedef enum _PVR_FRAMERATE_T
{
	PVR_FR_FORBIDDEN = 0,
	PVR_FR_23_976HZ = 1,
	PVR_FR_24HZ,
	PVR_FR_25HZ,
	PVR_FR_29_97HZ,
	PVR_FR_30HZ,
	PVR_FR_50HZ,
	PVR_FR_59_94HZ,
	PVR_FR_60HZ,
#define	MAX_SUPPORTED_FR_NR (PVR_FR_60HZ)
	PVR_FR_UNKNOWN
} PVR_FRAMERATE_T;

/**
 * Stream information
 *
*/
typedef struct _PVR_STREAM_INFO_T
{
	BOOLEAN				bFoundSeqSPS;
	UINT32				bitRate;
	PVR_FRAMERATE_T		frRate;
} PVR_STREAM_INFO_T;

/**
 * PID information
 *
*/
typedef struct _PVR_PID_INFO_T
{
	UINT16			pid;
	SDEC_PID_TYPE_T	pidType;
	VDEC_CODEC_T	videoCodecType;
	ADEC_SRC_TYPE_T audioCodecType;
} PVR_PID_INFO_T;

/**
 * Download channel
 *
*/
typedef enum _PVR_DOWN_CH_T
{
	PVR_DOWN_CH_A = 0,
	PVR_DOWN_CH_MAX = PVR_DOWN_CH_A
#define	PVR_DOWN_CH_NR	(PVR_DOWN_CH_MAX+1)
} PVR_DOWN_CH_T;

/**
 * Download source
 *
*/
typedef enum _PVR_DOWN_SRC_T
{
	PVR_DOWN_SRC_DTV = 0x00,
	PVR_DOWN_SRC_ATV = 0x20,
	PVR_DOWN_SRC_AV_1 = 0x40,
	PVR_DOWN_SRC_AV_2 = 0x41,
} PVR_DOWN_SRC_T ;

/**
 * Upload channel
 *
*/
typedef enum _PVR_UP_CH_T
{
	PVR_UP_CH_A = 0,
	PVR_UP_CH_MAX = PVR_UP_CH_A
#define	PVR_UP_CH_NR	(PVR_UP_CH_MAX+1)
} PVR_UP_CH_T;

/**
 * Index type
 *
*/
typedef enum _PVR_INDEX_TYPE_T
{
	PVR_INDEX_TYPE_SEQ = 0x01,
	PVR_INDEX_TYPE_I_PIC = 0x02,
	PVR_INDEX_TYPE_SEQ_I_PIC = 0x03,
 	PVR_INDEX_TYPE_P_PIC = 0x04,
	PVR_INDEX_TYPE_B_PIC = 0x08,
	PVR_INDEX_TYPE_NONE = 0xFF
} PVR_INDEX_TYPE_T;

/**
 * Timestamp type
 *
*/
typedef enum _PVR_TIMESTAMP_T
{
	PVR_TIMESTAMP_TYPE_NONE = 0,
	PVR_TIMESTAMP_TYPE_4BYTE
} PVR_TIMESTAMP_T;

/**
 * Encryption type
 *
*/
typedef enum _PVR_CIPHER_T
{
	PVR_CIPHER_NONE = 0,
	PVR_CIPHER_AES_ECB,
	PVR_CIPHER_AES_CBC
} PVR_CIPHER_T;

/**
* PVR Cipher Key Type
*
* @see
*/
typedef enum _PVR_CIPHER_KEY_T
{
    PVR_CIPHER_KEY_EVEN = 0,
    PVR_CIPHER_KEY_ODD
} PVR_CIPHER_KEY_T;

/**
* PVR Cipher Operation Type
*
* @see
*/
typedef enum _PVR_CIPHER_OPERATION_T
{
    PVR_CIPHER_OPERATION_ENCRYPT = 0,
    PVR_CIPHER_OPERATION_DECRYPT,
	PVR_CIPHER_OPERATION_NUM
} PVR_CIPHER_OPERATION_T;

/**
 * Cryptography Information Type
 *
*/
typedef struct _PVR_CRYPTO_TYPE_T
{
	PVR_CIPHER_T			cipherAlgorithm;
	PVR_CIPHER_KEY_T		cipherKeyType;
	PVR_CIPHER_OPERATION_T	cipherOperation;
	UINT32					keyBitLen;		/**<  Length of the key  */
	UINT8					*pIV;			/**< A possible Initialization Vector for ciphering */
	UINT8					*pCipherKey;
} PVR_CRYPTO_TYPE_T;


/**
 * Index
 *
*/
typedef struct _PVR_INDEX_T
{
	PVR_INDEX_TYPE_T	indexType;
	UINT32				byteOffset;
}PVR_INDEX_T;

/**
 * Thumbnail
 *
*/
typedef struct _PVR_THUMBNAIL_T
{
	UINT8	*pBuffer;
	UINT8	xSize;
	UINT8	ySize;
	UINT16	consumedTime;
} PVR_THUMBNAIL_T;

/**
 * Encoding rate
 *
*/
typedef enum
{
      MAX_BITRATE=1,
      HIGH_BITRATE,
      NORMAL_BITRATE,
      LOW_BITRATE,
      LOWEST_BITRATE
} PVR_ENCODING_BITRATE_T;

#define	PVR_RAW_TP_SIZE		188
#define	PVR_TIMESTAMP_SIZE	4

#define	PVR_TP_SIZE			(PVR_RAW_TP_SIZE + PVR_TIMESTAMP_SIZE)

#define PVR_DOWN_READY_TIMEOUT	30
#define PVR_DOWN_TIMEOUT		(PVR_DOWN_READY_TIMEOUT * 50)

#define PVR_UP_READY_TIMEOUT	150
#define PVR_UP_TIMEOUT			(PVR_UP_READY_TIMEOUT * 10)

/* It is an application's charge how to handle the situation, failure of getting upload buffer. */
#if 1
#define	PVR_UP_GET_BUFF_TIMEOUT	(1)
#define	PVR_UP_BUFF_TIMEOUT		(1)
#else
#define	PVR_UP_GET_BUFF_TIMEOUT	((PVR_UP_READY_TIMEOUT/3)*30)
#define	PVR_UP_BUFF_TIMEOUT		(PVR_UP_READY_TIMEOUT/3)
#endif

DTV_STATUS_T DDI_PVR_InitializeModule(void);

DTV_STATUS_T DDI_PVR_SetLowBitrateFlag(PVR_DOWN_CH_T ch, BOOLEAN bLowBitrate);
DTV_STATUS_T DDI_PVR_SetDownloadSource(PVR_DOWN_CH_T ch, PVR_DOWN_SRC_T downSrc);
DTV_STATUS_T DDI_PVR_SetDownloadPID(PVR_DOWN_CH_T ch, PVR_PID_INFO_T pidInfo);
DTV_STATUS_T DDI_PVR_StartDownload(PVR_DOWN_CH_T ch);
DTV_STATUS_T DDI_PVR_GetDownloadStream(PVR_DOWN_CH_T ch, UINT8 **ppStreamBuffer, UINT32 *pStreamSize,
										PVR_INDEX_T **ppIndexArray, UINT32 *pIndexNum, UINT32 timeout);
DTV_STATUS_T DDI_PVR_ReturnDownloadBuffer(PVR_DOWN_CH_T ch, UINT8 *pBuffer);
DTV_STATUS_T DDI_PVR_StopDownload(PVR_DOWN_CH_T ch);
DTV_STATUS_T DDI_PVR_StopDownloadStream(PVR_DOWN_CH_T ch);

DTV_STATUS_T DDI_PVR_GetDownloadStreamInfo(PVR_DOWN_CH_T ch, PVR_STREAM_INFO_T *pInfo);

DTV_STATUS_T DDI_PVR_ResetUpload(PVR_UP_CH_T	ch);
DTV_STATUS_T DDI_PVR_StartUpload(PVR_UP_CH_T	ch);
DTV_STATUS_T DDI_PVR_UploadStream(PVR_UP_CH_T ch, UINT8 *pUploadBuffer, UINT32 streamSize);

DTV_STATUS_T DDI_PVR_SetUploadSpeed(PVR_UP_CH_T ch, SINT16 speed);

DTV_STATUS_T DDI_PVR_StopUpload(PVR_UP_CH_T ch);

DTV_STATUS_T DDI_PVR_StopThumbnailDownload(PVR_DOWN_CH_T ch);
DTV_STATUS_T DDI_PVR_GetThumbnail(PVR_DOWN_CH_T ch, PVR_THUMBNAIL_T *pThumbnail);

/* The following functions are only for direct access to upload buffer */
DTV_STATUS_T DDI_PVR_GetUploadBuffer(PVR_UP_CH_T ch, UINT8 **ppBuffer, UINT32 *pSize);

DTV_STATUS_T DDI_PVR_SetEncodingBitRate(PVR_ENCODING_BITRATE_T bitRate);

#if (PLATFORM_TYPE == BCM_PLATFORM)
/* The following funcitons are used only with BCM platform. */
DTV_STATUS_T DDI_PVR_BCM_ClearDownloadPID(PVR_DOWN_CH_T ch, PVR_PID_INFO_T pidInfo);

BOOLEAN DDI_PVR_BCM_IsSTCTrickMode(void);

DTV_STATUS_T	DDI_PVR_BCM_SetUploadLiveSpeed(void);

void *DDI_PVR_BCM_SetUploadPid(PVR_PID_INFO_T pid);

DTV_STATUS_T DDI_PVR_BCM_ClearUploadPid(PVR_PID_INFO_T pid);

DTV_STATUS_T DDI_PVR_BCM_ClearAllUploadPid(void);
#endif

DTV_STATUS_T DDI_PVR_SetConfig(PVR_TIMESTAMP_T timeStampMode, PVR_CIPHER_T cipherAlgorithm);

DTV_STATUS_T DDI_PVR_SetCipherKeys(PVR_CRYPTO_TYPE_T cryptoInfo);

#ifdef __cplusplus
}
#endif

#endif

