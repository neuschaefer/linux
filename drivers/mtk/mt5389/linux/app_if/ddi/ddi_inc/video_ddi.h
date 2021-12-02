/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file video_ddi.h
 *
 *  It is the header file for controling VIDEO function.
 *  The header of VIDEO DDI is controlled by this file.
 *
 *  @author		Lee, Jong-Hun(drmyung@lge.com)
 *  @author		Lee, illo(b215@lge.com)
 *  @version		0.2
 *  @date		2006.03.16
 *  @date		2008.03.06
 *  @note
 *  @see		video_ddi.c
 */

/*-----------------------------------------------------------------------------
	Header File Guarder
------------------------------------------------------------------------------*/
#ifndef	_VIDEO_DDI_H_
#define	_VIDEO_DDI_H_

/*-----------------------------------------------------------------------------
	Control Constants
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    File Inclusions
------------------------------------------------------------------------------*/
//#include "branches.h"
//#include "common.h"
#ifndef WIN32_SIM
#include "common.h"
#else
#include "common_win32.h"
#endif
//#include "global_configurations.h"
//#include "osa_api.h"
//#include "nvm_ddi.h"


#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
	Constant Definitions
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
------------------------------------------------------------------------------*/
#define VIDEO_SUCCESS(e)      ((e) == API_OK)
#define VIDEO_ERROR(e)        ((e) != API_OK)

/*----------------------------------------------------------------------------
    Type Definitions
------------------------------------------------------------------------------*/
/** Video Window Id.
 * @ingroup ka_video_types
 */
typedef enum {
	VIDEO_WID_MIN = 0,
	VIDEO_WID_0 	= VIDEO_WID_MIN,
	VIDEO_WID_1,
	VIDEO_WID_MAX = VIDEO_WID_1,
	VIDEO_WID_MAXN,
} VIDEO_WID_T;

/**
 * Video Input signal source definition
 *
 */
typedef enum VIDEO_SRC_TYPE
{
	VIDEO_SRC_DTV		=	0,	// TV Input
	VIDEO_SRC_ATV,				// AV Input
	VIDEO_SRC_SCART,			// Scart Input
	VIDEO_SRC_AV,				// Scart Input
	VIDEO_SRC_AUTOAV,			// Auto AV(SCART) Input
	VIDEO_SRC_COMP,				// Component(YPbPr) Input
	VIDEO_SRC_RGB,				// RGB Input
	VIDEO_SRC_HDMI,				// HDMI Input
	VIDEO_SRC_PLAYHDD,          // PVR Input- added by ikarus :20080604
	VIDEO_SRC_MAX,				// MAX Input
	VIDEO_SRC_INVALID	= 0xff	// Invalid Source Input

} VIDEO_SRC_TYPE_T;

/**
 * Video Input source Information
 *
 */
typedef struct VIDEO_SRC_INFO
{
	UINT32
	type	: 16,
	index	: 16;
} VIDEO_SRC_INFO_T;

/**
 * DTVCC Service type
*/
typedef enum
{
	DTVCC_SERVICE1		= 0x1,
	DTVCC_SERVICE2		= 0x2,
	DTVCC_SERVICE3		= 0x3,
	DTVCC_SERVICE4		= 0x4,
	DTVCC_SERVICE5		= 0x5,
	DTVCC_SERVICE6		= 0x6,
	DTVCC_SERVICE_INVALID	= 0x7
} VDEC_DCC_SERVICE_NUM_T;

/**
 * Video Timing Information
 *
 */
typedef struct
{
	SINT32 format;	// video Resoultion
	UINT16 hFreq;	// for debugging
	UINT16 vFreq;
	UINT16 hTotal;	// for debugging
	UINT16 vTotal;	// for debugging
	UINT16 hStart;
	UINT16 vStart;
	UINT16 hActive;
	UINT16 vActive;
	UINT8  scanType;
	UINT8  phase;	// for debugging
	UINT8  mode;	// for debugging
} VIDEO_TIMING_INFO_T;

/* VBI Type.
 * @ingroup ka_video_types
 */
typedef enum VIDEO_DDI_VBI_TYPE
{
	VIDEO_DDI_VBI_PAL_TELETEXT = 0,
	VIDEO_DDI_VBI_PAL_WSS,
	VIDEO_DDI_VBI_PAL_VPS,
	VIDEO_DDI_VBI_NTSC_CC608,
//	VIDEO_DDI_VBI_DVB_TELETEXT,
//	VIDEO_DDI_VBI_DVB_SUBTITLE,
//	VIDEO_DDI_VBI_ATSC_CC608,
//	VIDEO_DDI_VBI_ATSC_CC708,
	VIDEO_DDI_VBI_MAX_SERVICE,
} VIDEO_DDI_VBI_TYPE_T;

//#if (SYS_DVB)
#if (0)
#else
/*
 * Analog Closed Caption VBI Field
 */
typedef enum {
	VBI_NO_FIELD	= 0,
	VBI_ODD_FIELD,
	VBI_EVEN_FIELD,
	VBI_ALL_FIELD,
	VBI_FIELD_MAXN,
} VBI_FIELD_T;

// not used, 20081117
//DTV_STATUS_T DDI_VIDEO_EnableVbiSlicer(VIDEO_WID_T wId);
//DTV_STATUS_T DDI_VIDEO_DisableVbiSlicer(VIDEO_WID_T wId);

DTV_STATUS_T DDI_VIDEO_SetVbiSlicerOnOff(VIDEO_WID_T wId, BOOLEAN onOff);
DTV_STATUS_T DDI_VIDEO_SetVbiField(VIDEO_WID_T wId, VBI_FIELD_T field);
#endif

/***********************************************************************/
/* Common structures for every layer,                        		*/
/* But the name which includes DDI is different from video api's name	*/
/**********************************************************************/
/**
 * Video Window 설정하는데 사용.
 * @ingroup ka_video_types
 */
typedef struct {
	UINT16		w;	/**< horizontal Size(Width)    in pixel [0,].*/
	UINT16		h;	/**< vertical   Size(Height)   in pixel [0,].*/
	UINT16		x;	/**< horizontal Start Position in pixel [0,].*/
	UINT16		y;	/**< vertical   Start Position in pixel [0,].*/
} VIDEO_DDI_RECT_T;

typedef enum {
	VIDEO_DDI_WIN_COLOR_BLACK,
	VIDEO_DDI_WIN_COLOR_BLUE
} VIDEO_DDI_WIN_COLOR_T;

/**
 * Each component of adjustment RGB-PC
 *
 */
typedef struct
{
	SINT16		s16pcHDelay;		/**< Horizontal Sync Delay 값 */
	SINT16		s16pcVDelay;		/**< Vertical Sync Delay 값 */
	SINT8		s8pcClock;		/**< Clock 값 */
	UINT8		u8pcPhase;		/**< Phase 값 */
} VIDEO_DDI_ADJ_PC_T;

/**
 * PC SCREEN adjust mode
 *
 */
typedef enum
{
	VIDEO_DDI_ADJ_PC_AUTO = 0,
	VIDEO_DDI_ADJ_PC_H_POS,
	VIDEO_DDI_ADJ_PC_V_POS,
	VIDEO_DDI_ADJ_PC_CLOCK,
	VIDEO_DDI_ADJ_PC_PHASE,
} VIDEO_DDI_ADJ_PC_MODE_T;

#if 1//(SYS_DVB)
/* PAL WSS Aspect Ratio.
 * @ingroup ka_video_types
 */
typedef enum VIDEO_DDI_WSS_MODE
{
	VIDEO_DDI_WSS_FULLFORMAT_4_3			= 0x8,
	VIDEO_DDI_WSS_LETTERBOX_14_9_CENT		= 0x1,
	VIDEO_DDI_WSS_LETTERBOX_14_9_TOP		= 0x2,
	VIDEO_DDI_WSS_LETTERBOX_16_9_CENT		= 0xb,
	VIDEO_DDI_WSS_LETTERBOX_16_9_TOP		= 0x4,
	VIDEO_DDI_WSS_LETTERBOX_16_9_ZOOM		= 0xd,
	VIDEO_DDI_WSS_FULLFORMAT_14_9			= 0xe,
	VIDEO_DDI_WSS_FULLFORMAT_16_9			= 0x7,
	VIDEO_DDI_WSS_INVALID					= 0x0
} VIDEO_DDI_WSS_ARC_MODE_T;
#endif

/**
 * Frond-End intput source type
 *
 */
typedef enum
{
	VIDEO_DDI_FB_STATUS_CVBS,
	VIDEO_DDI_FB_STATUS_RGB,
} VIDEO_DDI_SCART_FB_STATUS_T;

/*
 * Scart Aspect Ratio
 */
typedef enum
{
    VIDEO_DDI_SCART_AR_16_9, //5 ~ 8V
    VIDEO_DDI_SCART_AR_4_3,  // 9.5 ~ 12V
    VIDEO_DDI_SCART_AR_INVALID
} VIDEO_DDI_SCART_AR_T;

typedef enum
{
//	ADC_SRC_TUNER,
//	ADC_SRC_COMPOSITE,
	ADC_COMPONENT_SD,
	ADC_COMPONENT_HD,
	ADC_RGB,
	ADC_MAX_INPUT
} VIDEO_DDI_ADC_SRC_T;


/*-----------------------------------------------------------------------------
	(Extern Variables & Function Prototype Declarations)
------------------------------------------------------------------------------*/
DTV_STATUS_T	DDI_VIDEO_Initialize(void);
DTV_STATUS_T 	DDI_VIDEO_SetBackgroundColor(UINT8 red, UINT8 green, UINT8 blue);
BOOLEAN			DDI_VIDEO_IsAVDECSyncExist(UINT32 *pVideoSyncInfo);
DTV_STATUS_T	DDI_VIDEO_SetFrameLockMode(UINT8 frameLockMode);
void			DDI_VIDEO_SetHsyncDetectionForTuning(BOOLEAN bEnable);

UINT8			DDI_VIDEO_GetHDMIAspectRatio(void);
UINT8			DDI_VIDEO_GetCOMPAspectRatio(void);

UINT8			DDI_VIDEO_GetHDMIMode(UINT8 port);
DTV_STATUS_T	DDI_VIDEO_AdjustPCAuto(VIDEO_DDI_ADJ_PC_T *pAdjResult);
DTV_STATUS_T	DDI_VIDEO_AdjustPC(VIDEO_DDI_ADJ_PC_MODE_T mode, SINT16 value);
//#if (SYS_DVB)
#if (1)
VIDEO_DDI_WSS_ARC_MODE_T	DDI_VIDEO_VBI_GetWSSData(void);
#endif

DTV_STATUS_T DDI_VIDEO_VBI_Enable(void);
DTV_STATUS_T DDI_VIDEO_VBI_Disable(void);
DTV_STATUS_T DDI_VIDEO_VBI_StartSlicing (VIDEO_WID_T wId, VIDEO_DDI_VBI_TYPE_T type, void *pfnVBICB);
DTV_STATUS_T DDI_VIDEO_VBI_StopSlicing (VIDEO_WID_T wId, VIDEO_DDI_VBI_TYPE_T type);

DTV_STATUS_T DDI_VIDEO_VBI_StartInsertion (VIDEO_DDI_VBI_TYPE_T type);
DTV_STATUS_T DDI_VIDEO_VBI_StopInsertion (VIDEO_DDI_VBI_TYPE_T type);
DTV_STATUS_T DDI_VIDEO_VBI_InsertData (VIDEO_DDI_VBI_TYPE_T type, UINT32 nrLine, UINT32 *pLines, UINT32 size, UINT8 *pData);

DTV_STATUS_T	DDI_VIDEO_VBI_StartSlice(VIDEO_DDI_VBI_TYPE_T type, OSA_MSGQ_T qId);
DTV_STATUS_T	DDI_VIDEO_VBI_StopSlice(VIDEO_DDI_VBI_TYPE_T type);
VIDEO_DDI_SCART_FB_STATUS_T	DDI_VIDEO_GetFBStatus(void);
VIDEO_DDI_SCART_AR_T		DDI_VIDEO_GetScartInputMode(UINT8 scartId);
DTV_STATUS_T	DDI_VIDEO_SetScartOverlay(UINT8 scartId, BOOLEAN bOnOff);
UINT8			DDI_VIDEO_GetOutputFrameRate(void);
void			DDI_VIDEO_SetPCResolution(SINT32 resolution);
DTV_STATUS_T	DDI_VIDEO_SetWinBlank(VIDEO_WID_T wId, BOOLEAN bOnOff, VIDEO_DDI_WIN_COLOR_T color);
//DTV_STATUS_T	DDI_VIDEO_SetVCRBlank(BOOLEAN bOnOff);
DTV_STATUS_T	DDI_VIDEO_SetVCROutput(VIDEO_SRC_TYPE_T vType, BOOLEAN bOnOff);
DTV_STATUS_T	DDI_VIDEO_GetWin(VIDEO_WID_T wId, VIDEO_DDI_RECT_T *pIn, VIDEO_DDI_RECT_T *pOut);
DTV_STATUS_T	DDI_VIDEO_SetWin(VIDEO_WID_T wId, VIDEO_DDI_RECT_T *pIn, VIDEO_DDI_RECT_T *pOut);
DTV_STATUS_T	DDI_VIDEO_SetWinFreeze(VIDEO_WID_T wId, BOOLEAN bOnOff);
DTV_STATUS_T	DDI_VIDEO_SetInput(VIDEO_WID_T wId, VIDEO_SRC_INFO_T src);
DTV_STATUS_T	DDI_VIDEO_GetTimingInfo(VIDEO_WID_T wId, VIDEO_TIMING_INFO_T *pInfo);
void			DDI_VIDEO_GNSS_SetZoomingStatus(BOOLEAN bIsZoomingMode);

DTV_STATUS_T	DDI_VIDEO_SetDisplayOutput(BOOLEAN bOnOff);
DTV_STATUS_T	DDI_VIDEO_SetDisplayFormat(SINT32 fmtID, UINT8 frameRate, UINT8 lvdsType, UINT8 bitNum);
void			DDI_VIDEO_SetFrameRate(BOOLEAN isForceFreeRun, UINT8 frameRates);

DTV_STATUS_T	DDI_VIDEO_SetMessageIDOfACC(OSA_MSGQ_T qid4ACCTx,
											UINT32 oddMsgID,
											UINT32 evenMsgID,
											UINT32 mainSrcID,
											UINT32 subSrcID);

void DDI_VIDEO_SelectTestPattern(UINT8 pattern_type, UINT8 pattern_param);
void DDI_VIDEO_SetWBTestPattern(BOOLEAN bOnOff, BOOLEAN bIs216);


BOOLEAN DDI_VIDEO_AdjustADCCalibration(BOOLEAN bInternal, UINT8 input);
void DDI_VIDEO_UpdateADCValue(VIDEO_DDI_ADC_SRC_T input, ADC_CALIB_DATA_T calibData);
void DDI_VIDEO_GetADCValue(ADC_CALIB_DATA_T *calibData);

void DDI_VIDEO_DEBUG_Test(void);
void DDI_VIDEO_DEBUG_SetWinInput(VIDEO_WID_T, VIDEO_DDI_RECT_T *);

extern BOOLEAN DDI_VBI_IsVPSDataReady(void);
extern BOOLEAN DDI_VBI_IsTTXDataReady(void);
extern void DDI_VBI_ResetDataReady(void);
extern void DDI_VBI_ResetTTXBuffer(void);   //swen 20081016

/* ddi video functions related to ADC adjustment on BCM platform  */
DTV_STATUS DDI_VIDEO_CaptureVideoBuffer(void);
DTV_STATUS DDI_VIDEO_ReleaseVideoBuffer(void);
DTV_STATUS DDI_VIDEO_ReadVideoBuffer(UINT8 win_id, UINT8 YCbCr, UINT32 *x, UINT32 *y, UINT8 block_x, UINT8 block_y, float *ret_mean);


//#if (PLATFORM_TYPE == MSTAR_PLATFORM)
#if (1)
void DDI_VIDEO_SetSpreadSpectrum(UINT16 u16Period, UINT16 u16Percent, BOOLEAN bEnable);
#else
void DDI_VIDEO_SetSpreadSpectrum(UINT8 step, UINT8 range, UINT8 hold, UINT8 scale);
#endif

//#if (PLATFORM_TYPE == MSTAR_PLATFORM)	//added by lanbird
#if (0)
void DDI_VIDEO_MWE_Enable(BOOLEAN bEnable);
void DDI_VIDEO_MWE_SetWinType(UINT8 u8MWEtype);
#endif	//end of added

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _VIDEO_DDI_H_ */
