/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 1999 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file common.h
 *
 *	Common Used Type Definitions.
 *
 *	@author 	Changwook Lim (changwook.im@lge.com)
 *	@version	1.0
 *	@date		2005. 6. 1
 *	@note
 *	@see
 */

/******************************************************************************
	Header File Guarder
******************************************************************************/

#ifndef _XOSA_EVENT_H_
#define _XOSA_EVENT_H_

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	(Control Constants)
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	include files
*-----------------------------------------------------------------------*/
#include "dbgfrwk_types.h"
#include "xosa/xosa_thread.h"
#include "xosa/xosa_queue.h"

/*------------------------------------------------------------------------
*	constant definitions
*-----------------------------------------------------------------------*/
#define	OSA_EVENT_RECEIVE_ANY			0
#define	OSA_EVENT_RECEIVE_ALL			(1<<0)




/*------------------------------------------------------------------------
*	macro function definitions
*-----------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
extern int osa_event_register(osa_queue_t qid, osa_thread_t tid, unsigned int event);
extern int osa_event_unregister(osa_queue_t qid);
extern int osa_event_send(osa_thread_t tid, unsigned int ev);
extern int osa_event_receive(unsigned int events, unsigned int *pRetEvents, int waitMs, unsigned int options);
extern int osa_event_clear(osa_thread_t tid);

/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/




#ifdef __cplusplus
}
#endif

#endif  /* _XOSA_EVENT_H_ */
