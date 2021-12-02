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

#ifndef	_PVR_DOOWNLOAD_DDI_H_
#define	_PVR_DOOWNLOAD_DDI_H_

#if (PLATFORM_TYPE == BCM_PLATFORM)
#include "pvr/pvr_types.h"
#else
#include "common.h"
#ifndef    INCLUDE_DVR_READY
#include "sc_api.h"
#endif

#if (SYS_DVB)
#define CHIP_FRAME_FORWARD
#else
#undef CHIP_FRAME_FORWARD
#endif

#define	PVR_USE_CYPER		FALSE
#define NORMAL_UPLOAD_SPEED	(100)

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

typedef struct _PVR_STREAM_INFO_T
{
	BOOLEAN				bFoundSeqSPS;
	UINT32				bitRate;
	PVR_FRAMERATE_T		frRate;
} PVR_STREAM_INFO_T;

typedef enum _PVR_PID_TYPE_T
{
#define	PVR_PID_CLASS_SHIFT	(4)
	PVR_VIDEO_PID = 0x10,
	PVR_MPEG2_VIDEO_PID,
	PVR_MPEG4_VIDEO_PID,
	PVR_H264_VIDEO_PID,
	PVR_AUDIO_PID = 0x20,
	PVR_MPEG_AUDIO_PID,
	PVR_AAC_AUDIO_PID,
	PVR_AACP_ADTS_AUDIO_PID,
	PVR_AACP_LOAS_AUDIO_PID,
	PVR_AC3_AUDIO_PID,
	PVR_AC3P_AUDIO_PID,
	PVR_PCR_PID = 0x30,
	PVR_SI_PID,
	PVR_DATA_PID
} PVR_PID_TYPE_T;

typedef struct _PVR_PID_INFO_T
{
	UINT16			pid;
	PVR_PID_TYPE_T	pidType;
} PVR_PID_INFO_T;

typedef enum _PVR_DOWN_CH_T
{
	PVR_DOWN_CH_A = 0,
	PVR_DOWN_CH_MAX = PVR_DOWN_CH_A
#define	PVR_DOWN_CH_NR	(PVR_DOWN_CH_MAX+1)
} PVR_DOWN_CH_T;

typedef enum _PVR_DOWN_SRC_T
{
	PVR_DOWN_FROM_TPA = 0,
	PVR_DOWN_FROM_TPB,
	PVR_DOWN_FROM_TPC,
	PVR_DOWN_FROM_TPD,
} PVR_DOWN_SRC_T ;

typedef enum _PVR_UP_CH_T
{
	PVR_UP_CH_A = 0,
	PVR_UP_CH_MAX = PVR_UP_CH_A
#define	PVR_UP_CH_NR	(PVR_UP_CH_MAX+1)
} PVR_UP_CH_T;

typedef enum _PVR_INDEX_TYPE_T
{
	PVR_INDEX_TYPE_SEQ = 0x01,
	PVR_INDEX_TYPE_I_PIC = 0x02,
	PVR_INDEX_TYPE_SEQ_I_PIC = 0x03,
 	PVR_INDEX_TYPE_P_PIC = 0x04,
	PVR_INDEX_TYPE_B_PIC = 0x08,
	PVR_INDEX_TYPE_NONE = 0xFF
} PVR_INDEX_TYPE_T;

typedef struct _PVR_INDEX_T
{
	PVR_INDEX_TYPE_T	indexType;
	UINT32				byteOffset;
}PVR_INDEX_T;
#endif

#define	PVR_RAW_TP_SIZE		188
#define	PVR_TIMESTAMP_SIZE	4

#define	PVR_TP_SIZE			(PVR_RAW_TP_SIZE + PVR_TIMESTAMP_SIZE)

#define PVR_DOWN_READY_TIMEOUT	20
#define PVR_DOWN_TIMEOUT		(PVR_DOWN_READY_TIMEOUT * 50)

#define PVR_UP_READY_TIMEOUT	150
#define PVR_UP_TIMEOUT			(PVR_UP_READY_TIMEOUT * 10)
#define	PVR_UP_BUFF_TIMEOUT		(PVR_UP_READY_TIMEOUT/3)
#if (PLATFORM_TYPE == BCM_PLATFORM)/* TODO : sometimes long delay happens when getting upload buffer is tried in BCM3556 */
#define PVR_UP_GET_BUFF_TIMEOUT	(PVR_UP_BUFF_TIMEOUT * 30)
#else
#define PVR_UP_GET_BUFF_TIMEOUT	(PVR_UP_BUFF_TIMEOUT * 30)
#endif

DTV_STATUS_T DDI_PVR_InitializeModule(void);

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

#ifdef USE_CHIP_TRICK_MODE
DTV_STATUS_T DDI_PVR_SetUploadSpeed(PVR_UP_CH_T ch, SINT16 speed);
DTV_STATUS_T DDI_PVR_BCM_SetUploadLiveSpeed(void);
#endif

#if (PLATFORM_TYPE == BCM_PLATFORM)
BOOLEAN DDI_PVR_BCM_IsSTCTrickMode(void);
#endif

DTV_STATUS_T DDI_PVR_StopUpload(PVR_UP_CH_T ch);

DTV_STATUS_T DDI_PVR_StopThumbnailDownload(PVR_DOWN_CH_T ch);
DTV_STATUS_T DDI_PVR_GetThumbnail(PVR_DOWN_CH_T ch, UINT8 *pBuffer, UINT8 *pXsize, UINT8 *pYsize);

/* The following functions are only for direct access to upload buffer */
DTV_STATUS_T DDI_PVR_GetUploadBuffer(PVR_UP_CH_T ch, UINT8 **ppBuffer, UINT32 *pSize);


/* The following functions are only for BRCM */
#if (PLATFORM_TYPE == BCM_PLATFORM)
void *DDI_PVR_BCM_SetUploadPid(PVR_PID_INFO_T pid);
DTV_STATUS_T DDI_PVR_BCM_ClearUpoloadPid(PVR_PID_INFO_T pid);
DTV_STATUS_T DDI_PVR_BCM_ClearAllUpoloadPid(void);
#endif

#endif

