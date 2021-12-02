/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file 		voice_ddi.h
 *
 *  @author		Lee Minsup (minsup.lee@lge.com)
 *  @version	1.0
 *  @date		2010. 12. 11
 *  @note
 *  @see
 */

#ifndef _VOICE_H_
#define _VOICE_H_

#include "message.h"

#define VOICE_BUFFER_SIZE		64000//320000
#define VOICE_CHUNK_SIZE		1600


#define VOICE_MSG_TYPE_NONE		0
#define VOICE_MSG_TYPE_START		MSG_RCU2VOICE_SEND_START
#define VOICE_MSG_TYPE_DATA		MSG_RCU2VOICE_SEND_DATA
#define VOICE_MSG_TYPE_STOP		MSG_RCU2VOICE_SEND_END

/**
* Voice Message
*/
typedef struct _VOICE_MSG_TYPE
{
	UINT32		msgType;
	UINT32		size;
} VOICE_MSG_TYPE;

/**
* Voice Buffer
*/
typedef struct _VOICE_BUFFER
{
	unsigned short		data[VOICE_BUFFER_SIZE];
	unsigned int		rdp;						// read position
	unsigned int		wrp; 						// write position
	unsigned int		filled;
	OSA_MSGQ_T		msgQId;						// queue ID
	SID_TYPE		bufMtx;						// buffer mutex
} VOICE_BUFFER;

extern  VOICE_BUFFER *	DDI_VOICE_GetBuffer(void);
void DDI_SP_ParsingDelay( void );
void DDI_SP_VoiceMuteFlag( unsigned int MuteFlag );
void DDI_SP_VoiceMuteDetectionTimeOut( unsigned int MuteDetTimeOut );
void DDI_SP_VoiceMuteAdjustmentTimeOut( unsigned int MuteAdjTimeOut );
void DDI_SP_VoiceRecognitionTimeOut( unsigned int VoiceRegTimeOut );
void DDI_SP_VoiceModeSetting ( unsigned int VoiceModeset );

#endif /* _VOICE_H_ */

