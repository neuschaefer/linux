/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file sdec_ddi.h
 *
 *  System Decoder DDI function header file.
 *
 *	@author		Kim, Soon-Tae(iamsoontae@lge.com)
 *  @version	1.0
 *  @date		2007.04.16
 *  @see		sdec_ddi.c
 */

#ifndef _SDEC_DDI_H_
#define _SDEC_DDI_H_

/******************************************************************************
 #include 파일들 (File Inclusions)
******************************************************************************/
#include "common.h"
#include "osa_api.h"
#include "osa_debug.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
 	상수 정의(Constant Definitions)
******************************************************************************/

#define	INVALID_PID	0x1FFF
#define	SECTION_FIELD_MASK		0
/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/

/**
 * SDEC PID Type For Filtering.
*/
typedef enum {
	SDEC_PID_VIDEO = 0,
	SDEC_PID_AUDIO,
	SDEC_PID_AUDIO_DESCRIPTION = SDEC_PID_AUDIO,
	SDEC_PID_PCR,
	SDEC_PID_TELETEXT,
	SDEC_PID_SUBTITLE ,
	SDEC_PID_DCC = SDEC_PID_SUBTITLE,
	SDEC_PID_GP,
	SDEC_PID_TYPE_NUM
} SDEC_PID_TYPE_T;

/**
 * Destination of PES Filter.
*/
typedef enum {
	PES_DEST_NONE = 0,
	PES_DEST_ADEC0,
	PES_DEST_ADEC1,
	PES_DEST_VDEC0,
	PES_DEST_VDEC1,
	PES_DEST_STC0,
	PES_DEST_STC1,
	PES_DEST_BUFFER,
    PES_DEST_RECORD_A,
    PES_DEST_RECORD_B
} SDEC_PES_DEST_T;

/**
 * Input Channel.
*/
typedef enum {
	SDEC_CH_MIN	= 0,
	SDEC_CH_A	= SDEC_CH_MIN,
	SDEC_CH_B,
	SDEC_CH_C,
	SDEC_CH_MAX	= SDEC_CH_C,
	SDEC_CH_MAXN,

	SDEC_CH_PLAYER_MIN = 10,
	SDEC_CH_PLAYER_A = SDEC_CH_PLAYER_MIN,	/* Playpump 0  */
	SDEC_CH_PLAYER_B,	/* Playpump 1  */
	SDEC_CH_PLAYER_C,	/* Divx Player */
	SDEC_CH_PLAYER_MAX = SDEC_CH_PLAYER_C,

	SDEC_CH_VOD_MIN = 20,
	SDEC_CH_VOD_A	= SDEC_CH_VOD_MIN,	/* Playpump 0: VOD Play */
	SDEC_CH_VOD_B,					/* Playpump 1: VOD Record */
	SDEC_CH_VOD_MAX = SDEC_CH_VOD_B,
} SDEC_CHANNEL_T;

/**
 * Input Type
*/
typedef enum {
	SDEC_INPUT_SERIAL = 0,
	SDEC_INPUT_PARALLEL,
	SDEC_INPUT_ENABLE,
	SDEC_INPUT_DISABLE,
	SDEC_INPUT_MEM,
	SDEC_INPUT_NUM,

	SDEC_INPUT_NULL //temp
} SDEC_INPUT_TYPE_T;

/**
 * CAS Type
*/
typedef enum {
	SDEC_WITHOUT_CAS = 0,
	SDEC_WITH_CAS
} SDEC_CAS_TYPE_T;

/**
 * Input Port Type
*/
typedef enum {
	SDEC_PORT_NONE = 0,				/* No Port */
	SDEC_PORT_IN_DEMOD0,			/* Input From Internal Demodulator */
	SDEC_PORT_IN_DEMOD1,			/* Input From Internal Demodulator */
	SDEC_PORT_EXT_INPUT0,			/* SoC EXT Input ( such as External Demodulator or Channel Browser Chip */
	SDEC_PORT_EXT_INPUT1,			/* SoC EXT Input ( such as External Demodulator or Channel Browser Chip */
	SDEC_PORT_EXT_INPUT2,			/* SoC EXT Input ( such as External Demodulator or Channel Browser Chip */
	SDEC_PORT_FROM_MEM,				/* TS Input From Memory */
	SDEC_PORT_NUM,

	SDEC_PORT_NULL //temp
} SDEC_INPUT_PORT_T;

/**
 * PID filter cleaning scope.
*/
typedef enum
{
	CLEAN_ALL = 0,
	CLEAN_AV,
	CLEAN_VIDEO,
	CLEAN_AUDIO,
	CLEAN_PCR,
	CLEAN_WO_AV,
	CLEAN_SECTION,
	CLEAN_PES,
	CLEAN_RECORD_A,
	CLEAN_RECORD_B,
	CLEAN_PVR
} SDEC_CLEANING_SCOPE_T;


/**
 * Section filter options.
*/
typedef struct SECTION_FILTER
{
 	UINT32	  transaction_id;					/**< Transaction ID */
	UINT16    pid;								/**< PID filter value */
	UINT8	  table_id;							/**< Table ID value */
 	UINT8	  section_syntax_indicator;			/**< Section Syntax Indicator value */
	UINT16	  table_id_extension;				/**< Table ID Extension value */
	UINT8	  version_number;					/**< Version Number value */
	UINT8	  current_next_indicator;			/**< Current Next Indicator value */
	UINT8	  last_section_number;				/**< Last section number value */
	UINT8	  protocol_version;					/**< Protocol Version value */

	UINT8	  table_id_filter_mask;				/**< Table ID filter bit mask (8 bits) */
	UINT16	  table_id_extension_filter_mask;	/**< Table ID extension filter bit mask (16 bits) */
 	UINT8
 		section_syntax_indicator_filter	: 1,	/**< Section syntax indicator filter ON/OFF */
 		version_number_filter			: 1,	/**< Version number filter ON/OFF */
 		not_version_number_filter		: 1,	/**< Not version number filter ON/OFF */
		current_next_indicator_filter	: 1,	/**< Current next indicator filter ON/OFF */
 		last_section_number_filter		: 1,	/**< Last section number filter ON/OFF */
 		protocol_version_filter			: 1,	/**< Protocol version filter ON/OFF */
 		reserved_filter					: 2;	/**< reserved */

 	UINT8
 		once_flag						: 1,	/**< Once flag ON/OFF */
 		crc_chksum						: 2, 	/**< 0 : No CRC & Checksum, 1 : CRC, 2 : CheckSum
												     <= regardless of section_syntax_indicator*/
 		reserved						: 5;	/**< reserved */

} SDEC_SECTION_FILTER_T, *P_SDEC_SECTION_FILTER_T;


/**
 * To Send Section Data To PSIP/SI
 */
typedef struct SDEC_MSG_TYPE
{
	UINT32	msgId;			///< 0x00: message id
	UINT32	channel;		///< 0x04: port id
	UINT8	filterId;		///< 0x05: filter id
	UINT16	pid;			///< 0x06: pid
	UINT8	*pData;			///< 0x08: pointer to section data
	UINT16	tableType;		///< 0x0C: table type
	UINT32	dataLen;		///< 0x0E: section data length
	UINT32	transactionId;	///< 0x10: transaction id
	SINT32	entryId;		///< 0x14: entry id
} SDEC_MSG_TYPE_T;

#ifndef INCLUDED_BY_FILTER
typedef DTV_STATUS_T (*pfnSDECDataHandlingCB)(SDEC_MSG_TYPE_T *pMsg);
#endif /* INCLUDED_BY_FILTER */

/******************************************************************************
	Extern 전역변수와 함수 prototype 선언
	(Extern Variables & Function Prototype Declarations)
*******************************************************************************/
DTV_STATUS_T	DDI_SDEC_InitializeModule( void );
DTV_STATUS_T	DDI_SDEC_FinalizeModule(void);
DTV_STATUS_T 	DDI_SDEC_SetInputConfig(SDEC_CHANNEL_T ch, SDEC_INPUT_PORT_T portType, SDEC_INPUT_TYPE_T inputType, SDEC_CAS_TYPE_T casType);
DTV_STATUS_T	DDI_SDEC_GetPID( SDEC_CHANNEL_T ch, SDEC_PID_TYPE_T pidType, UINT16 *pPid, SDEC_PES_DEST_T dest );
DTV_STATUS_T	DDI_SDEC_SetPID( SDEC_CHANNEL_T ch, SDEC_PID_TYPE_T pidType, UINT16 pid, SDEC_PES_DEST_T dest );
DTV_STATUS_T	DDI_SDEC_GetCurSTC( SDEC_CHANNEL_T ch, UINT32 *pStc_hi, UINT32 *pStc_lo );
DTV_STATUS_T	DDI_SDEC_ResetFilters( SDEC_CHANNEL_T ch, SDEC_CLEANING_SCOPE_T scope );

DTV_STATUS_T	DDI_SDEC_RequestSection( SDEC_CHANNEL_T ch,
									 SDEC_SECTION_FILTER_T *pSectionFilter,
									 UINT8 *pSecfIndex,
									 UINT32 gpbSize,
									 pfnSDECDataHandlingCB pfnCallBack );
DTV_STATUS_T	DDI_SDEC_CancelSection(	SDEC_CHANNEL_T ch, UINT8 secfIndex );
DTV_STATUS_T	DDI_SDEC_CopySectionData( SDEC_CHANNEL_T ch,
									 UINT8 secfIndex,
									 UINT16 pid,
									 UINT8 *pDest,
									 UINT8 *pBufAddr,
									 UINT32 dataSize );
DTV_STATUS_T	DDI_SDEC_ReturnSectionBuffer( SDEC_CHANNEL_T ch,
									 	  UINT8 secfIndex,
										  UINT16 pid,
										  UINT8 *pBufAddr,
										  UINT32 dataSize );
#if 1
DTV_STATUS_T	DDI_SDEC_StartPCRRecovery( SDEC_CHANNEL_T ch);
DTV_STATUS_T	DDI_SDEC_StopPCRRecovery( SDEC_CHANNEL_T ch);
#endif 

DTV_STATUS_T	DDI_SDEC_RequestPES( SDEC_CHANNEL_T ch,
								 SDEC_PID_TYPE_T pidType,
								 UINT16 pesPid,
								 UINT8 *pPesfIndex,
								 UINT32 gpbSize,
								 pfnSDECDataHandlingCB pfnCallBack );
DTV_STATUS_T	DDI_SDEC_CancelPES( SDEC_CHANNEL_T ch, UINT8 pesfIndex );
DTV_STATUS_T	DDI_SDEC_CopyPESData( SDEC_CHANNEL_T ch,
								  UINT8 pesfIndex,
								  UINT16 pid,
								  UINT8 *pDest,
								  UINT8 *pBufAddr,
								  UINT32 dataSize );
DTV_STATUS_T	DDI_SDEC_ReturnPESBuffer( SDEC_CHANNEL_T ch,
									  UINT8 pesfIndex,
									  UINT16 pid,
									  UINT8 *pBufAddr,
									  UINT32 dataSize );

DTV_STATUS_T	DDI_SDEC_StartScrambleCheck( SDEC_CHANNEL_T ch, SDEC_PID_TYPE_T pidType, UINT16 pid );
DTV_STATUS_T	DDI_SDEC_StopScrambleCheck( SDEC_CHANNEL_T ch, SDEC_PID_TYPE_T pidType, UINT16 pid );
DTV_STATUS_T	DDI_SDEC_CheckScrambledStatus( SDEC_CHANNEL_T ch, SDEC_PID_TYPE_T pidType, UINT16 pid, UINT8 *bScrambled );
#if (PLATFORM_TYPE == BCM_PLATFORM)
/* added by ikarus */
DTV_STATUS DDI_SDEC_BCM_GetPIDHandle( SDEC_CHANNEL_T ch, UINT16 PID, SDEC_PID_TYPE_T pidType, SDEC_PES_DEST_T dest, void **hPID );
DTV_STATUS DDI_SDEC_BCM_GetRemuxPIDHandle( SDEC_CHANNEL_T ch, UINT16 PID, SDEC_PID_TYPE_T pidType, SDEC_PES_DEST_T dest, void **hPID );
DTV_STATUS DDI_SDEC_BCM_SetSTCRate( UINT32 rate, BOOLEAN isLive );
DTV_STATUS DDI_SDEC_BCM_ChangeStcMode(BOOLEAN bAuto );
#endif

DTV_STATUS_T DDI_SDEC_RequestTS( SDEC_CHANNEL_T ch, UINT16 rawPid, UINT32 bufferSz, pfnSDECDataHandlingCB pfnCallBack );
DTV_STATUS_T DDI_SDEC_CancelTS( SDEC_CHANNEL_T ch, UINT8 filterIndex );

#ifdef INCLUDE_ACAP
/**
 * Section filter information for ACAP.
*/
typedef struct ACAP_SECF_INFO
{
	UINT8	acapOwn;	/**< Section filter used for ACAP */
	UINT8	psipOwn;	/**< Section filter used for PSIP */
	UINT8	psipNotver;	/**< Section filter for PSIP is setted in Not version */
	UINT8	NotverbySW;	/**< Not version is done by SW(TRUE) or HW(FALSE) */
	SINT16	fgID;		/**< Section filter Group IDP */
	UINT8	maxFNum;	/**< Maximum Section filter number in Group */
	SINT16	fID;		/**< Section filter ID in Group */
	UINT8	reserve;	/**< Section filter reserved for ACAP */
	UINT16	pId;		/**< PID value used for ACAP */
	SINT8	tId;		/**< Section filter value used for ACAP */
	UINT8	enable;		/**< Section filter Enable */
} ACAP_SECF_INFO_T;
#endif	/* INCLUDE_ACAP */

#ifdef __cplusplus
}
#endif

#endif  /* _SDEC_DDI_H_ */
