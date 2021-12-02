/******************************************************************************
*	DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA.
*	Copyright(c) 1999 by LG Electronics Inc.
*
*	 All rights reserved. No part of this work may be reproduced, stored in a
*	 retrieval system, or transmitted by any means without prior written
*	 permission of LG Electronics Inc.
*****************************************************************************/

/** @file sp_ddi.h
*
*	SP Interface header file.
*
*	@author		cho sang hyun(cshsjy@lge.com)
*	@version	1.0
*	@date	  	2008.10.08
*	@note
*/


#ifndef _SP_DDI_H_
#define _SP_DDI_H_


//=========================================================================

/** UI Messaging System에 사용되어지는 Callback 함수를 위한 정의.
 *	API_UTIL_InitUIMessagingSystem 함수 호출 시 사용되어진다.
 */
#define  MAX_SP2UI_MSG_PARAM 		4

//handle repeat key process for SP device
#define SP_REPEAT_KEY	0

#define ENABLE_6AXIS 0
#define ENABLE_WHEEL 0

extern	DTV_STATUS_T	DDI_SP_Initialize(void);


typedef void (*pfnSPSendMouseToUI) (float posx, float posy, UINT32 button_val, UINT32 gesture_ptr);
typedef void (*pfnSPSendKeyToUI) (UINT32 key, UINT32 keyType );
// hyunsun
typedef void (*pfnSPGetMouseParam)( char req_type, UINT32 *nParam1, UINT32 *nParam2, UINT32 *nParam3, UINT32 *nParam4);

/**
 * SP state id.
*/
typedef enum tagSP_STATE_T
{
	SP_STATE_IDLE=0,		/**< SP idle			 	*/
	SP_STATE_ACTIVE,		/**< SP active 			*/
	SP_STATE_IDLE2ACTIVE,	/**< SP idle to active 	*/
	SP_STATE_SLEEP,			/**< SP sleep 			*/
	SP_STATE_PAIRING,		/**< SP pairing 			*/
	SP_STATE_IR_ON,			/**< SP IR key input 	*/
	SP_STATE_BATTERY_OFF,	/**< SP Battery off 		*/
	SP_STATE_BATTERY_OUT,	/**< SP Battery out 		*/
	SP_STATE_SLEEP_BUTTON,	/**< SP sleep button push*/
	SP_STATE_NONE,			/**< SP none 			*/
} SP_STATE_T;

/*=======================================================
	MMR Interface Function 선언 (sp_task.c)
=======================================================*/
extern	void sp_task(void);

void DDI_SP_RegisterSendMouseToUICallback( pfnSPSendMouseToUI mouseCbFn, pfnSPGetMouseParam mouseParamCbFn );


void DDI_SP_GetGyroRef(float *pfGyrRefX, float *pfGyroRefY);
void DDI_SP_SetGyroRef(float fGyroRefX, float fGyroRefY );


DTV_STATUS_T DDI_SP_SetFeedback( BOOLEAN bFeedback );
DTV_STATUS_T DDI_SP_SetFeedbackLevel( UINT32 nFBLevel );
DTV_STATUS_T DDI_SP_GetFeedbackLevel( UINT32 *pnFBLevel );

DTV_STATUS_T DDI_SP_EnableIR( UINT32 nIRKey, BOOLEAN bEnabled );
DTV_STATUS_T DDI_SP_ClosePort( void );
DTV_STATUS_T DDI_SP_OpenPort( void );
DTV_STATUS_T DDI_SP_SetPointerSpeed( UINT32 nPointSpeed );

void		 DDI_SP_SetSPState( UINT32 nState );
UINT32		 DDI_SP_GetSPState( void );
UINT32 		 DDI_SP_GetSPStatePrev( void );

DTV_STATUS_T DDI_SP_SaveSensorParams( void );
DTV_STATUS_T DDI_SP_OpenSensorParams( void );

BOOLEAN 	 DDI_SP_GetPairingInfo( void );
DTV_STATUS_T DDI_SP_SetPairingInfo( BOOLEAN bPairing );
DTV_STATUS_T DDI_SP_StartPairing( void );
DTV_STATUS_T DDI_SP_StopPairing( void );

BOOLEAN 	DDI_SP_CheckRFRcvLink( void );
void 		DDI_SP_StartPairingLine( void );
void 		DDI_SP_StopPairingLine( void );


DTV_STATUS_T DDI_SP_Initialize(void);
DTV_STATUS_T DDI_SP_Destroy(void);

DTV_STATUS_T DDI_SP_ReadFwVersion(UINT8 *pString);
DTV_STATUS_T DDI_SP_ReadNetId(UINT8 *pString);
void		DDI_SP_FW_Download(void);
DTV_STATUS_T DDI_SP_3DEmitterTest(BOOLEAN bEnable);
int DDI_SP_CheckNetId(void);

int DDI_SP_3DStop(void);
int DDI_SP_3DStart(void);




void 		DDI_SP_SetLongLiveMouse(BOOLEAN bLongLiveMouse);
BOOLEAN 	DDI_SP_GetLongLiveMouse(void);
void 		DDI_SP_SetLoggingSensorParams( BOOLEAN bEnable );

void		DDI_SP_SetDebugprintSensor ( BOOLEAN bEnable );
void		DDI_SP_SetDebugprintSPState( BOOLEAN bEnable );


/* local functions for sp_task.c */
DTV_STATUS_T DDI_SP_SendMouseToUI( SINT32 nPosX, SINT32 nPosY, UINT32 nbutton, UINT32 nGesturePtr);
DTV_STATUS 	 DDI_SP_ResetRFModule(void);

// hyunsun
DTV_STATUS_T DDI_SP_GetMouseParam( char req_type, UINT32 *pnParam1, UINT32 *pnParam2, UINT32 *pnParam3, UINT32 *pnParam4 );

SINT32 	DDI_SP_GetCursorPosX(void);
SINT32	DDI_SP_GetCursorPosY(void);
void 	DDI_SP_SetCursorPosX(SINT32	posX);
void 	DDI_SP_SetCursorPosY(SINT32	posY);

void 	DDI_SP_SetMouseMode(UINT16	clickMode, UINT16 dragMode);
void 	DDI_SP_SetForceLoc(void);
SINT32 	DDI_SP_GetDesiredPosX(void);
SINT32 	DDI_SP_GetDesiredPosY(void);

void DDI_SP_SetAutoAlignment(BOOLEAN bOnOff);
void DDI_SP_SetCursorBounce(SINT32 x0, SINT32 y0, SINT32 x1, SINT32 y1);

#endif /* end of _SP_DDI_H_ */
