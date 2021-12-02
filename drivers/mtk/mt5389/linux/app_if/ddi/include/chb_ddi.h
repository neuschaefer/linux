/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/


/** @file chb_ddi.h
 *
 *  This header file declares the data types and DDI functions to control the gpio ports
 *
 *  @author		Ahn, Kyu-Tae (cpplover@lge.com)
 *  @version	0.1
 *  @date		2008.10.22
 *  @note		Additional information.
 *  @see
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef _CHB_DDI_H_
#define _CHB_DDI_H_

/******************************************************************************
	#include 파일들 (File Inclusions)
******************************************************************************/
#include "common.h"

/******************************************************************************
 	상수 정의(Constant Definitions)
******************************************************************************/



/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/

enum {
		CHB_VSB = 0,
		CHB_NTSC = 1,
		CHB_QAM64 = 2,
		CHB_QAM256 = 3,
		CHB_PAL = 4,
		CHB_DVBT = 5,
		CHB_DTMBT = 6,
		CHB_DVBC = 7
	};

#if  (PLATFORM_TYPE != LG_PLATFORM)
/* [L8] minho.hwang (20100913) - Not used on L8 BEGIN */
/**	CHB CALLBACK FUNCTION POINTER TYPE */
typedef DTV_STATUS_T	(*CHB_CALLBACKFUNC_PTR_T)(void *Obj, UINT32, UINT8* );

extern OSA_MSGQ_T			chbFilterSem;
/* [L8] minho.hwang (20100913) END */
#endif


#if 0
/**
 * channel scan parameters .
 */
typedef	struct CHB_SCANPARAMS {
	unsigned short 	ChMajorNum;
	unsigned short 	ChMinorNum;
	unsigned int 	SendCount;
	unsigned char	*pRetBuffer;
	unsigned int	RetBufferSize;
} CHB_SCANPARAMS_T;
#else
/**
 * channel scan parameters .
 */
typedef	struct CHB_SCANPARAMS {
	UINT16			chMajorNum;	/**< major number of scan channel */
	UINT16			chMinorNum;	/**< minor number of scan channel */
	UINT32			sendCount;		/**< send count */
	UINT8			*pRetBuffer;		/**< return thumnail buffer */
	UINT32			retBufferSize;	/**< thumnail buffer size */
	UINT32			feSpecialData;	/**< special data of current channel */
} CHB_SCANPARAMS_T;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if  (PLATFORM_TYPE != LG_PLATFORM)
/* [L8] minho.hwang (20100913) - Not used on L8 BEGIN */
//UINT32		ChNumInfo;
extern CHB_SCANPARAMS_T	gCHB_ChNumInfo;
/* [L8] minho.hwang (20100720) END */
#endif

#define MSG_CHB_BASE					(0x00FF0000)
#define MSG_CHB_SIGNAL_ERROR			(MSG_CHB_BASE + 1)
#define MSG_CHB_VDP_ERROR				(MSG_CHB_BASE + 2)
#define MSG_CHB_THUMBNAIL_TIMEOUT		(MSG_CHB_BASE + 3)
#define MSG_CHB_THUMBNAIL_OK			(MSG_CHB_BASE + 4)
#define MSG_CHB_THUMBNAIL_ERROR			(MSG_CHB_BASE + 5)


/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/
DTV_STATUS_T	DDI_CHB_Init(void);
#if  (PLATFORM_TYPE != LG_PLATFORM)
/* [L8] minho.hwang (20100913) - Not used on L8 BEGIN */
void			DDI_CHB_Debug(void);
/* [L8] minho.hwang (20100720) END */
#endif
DTV_STATUS_T	DDI_CHB_StartScan(UINT32 demodType, UINT32	freq, UINT16 vidpid, CHB_SCANPARAMS_T *pOptCB);
DTV_STATUS_T	DDI_CHB_StopScan(void);
void 		  	DDI_CHB_SuspendTask(BOOLEAN bCmd);
void			DDI_CHB_ToggleRawTSFilter(int bEnable);

#if  (PLATFORM_TYPE != LG_PLATFORM)
/* [L8] minho.hwang (20100913) - Not used on L8 BEGIN */
void			CHB_DEBUG_ToggleRawTSFilter(int bEnable);
void			CHB_DEBUG_ToggleDumpJpeg(void);
void			CHB_RequestRawTS(int Channel, UINT16 PID);
void			CHB_CancelRawTS(int Channel, UINT8 filter_index );
/* [L8] minho.hwang (20100720) END */
#endif

void 			DDI_CHB_SetCHBMsgQ(OSA_MSGQ_T msgQueueID);
#if  (PLATFORM_TYPE != LG_PLATFORM)
/* [L8] minho.hwang (20100913) - Not used on L8 BEGIN */
const char 		*CHB_demodType2String(UINT32 demodType);
/* [L8] minho.hwang (20100720) END */
#endif

#if  (PLATFORM_TYPE == LG_PLATFORM)
/* [L8] minho.hwang (20100913) - New functions on L8 BEGIN */
BOOLEAN CHB_GetSaveScaledImageFlag(void);
void CHB_SetSaveScaledImageFlag(BOOLEAN isEnable);
/* [L8] minho.hwang (20100913) END */
#endif

#ifdef INCLUDE_CHB
DTV_STATUS_T DDI_CHB_ResetAllCHB(void);
#endif

#ifdef _EMUL_WIN
void	DDI_CHB_SetMCBMsgQ(OSA_MSGQ_T queue);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* _GPIO_DDI_H_ */

