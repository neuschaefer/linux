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

#ifndef _XOSA_TIMER_H_
#define _XOSA_TIMER_H_

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

/*------------------------------------------------------------------------
*	constant definitions
*-----------------------------------------------------------------------*/
#define OSA_TIMER_CONTEXT_EVENT			0			/* Send event on timeout	*/
#define OSA_TIMER_CONTEXT_FUNCTION		(1<<0)		/* Call function on timeout	*/
#define OSA_TIMER_CONTEXT_NEW_TASK		(1<<1)		/* Create task for context	*/
#define OSA_TIMER_CONTEXT_CALLBACK		(OSA_TIMER_CONTEXT_FUNCTION | OSA_TIMER_CONTEXT_NEW_TASK)

#define OSA_TIMER_ONCE					0
#define OSA_TIMER_PERIODIC				(1<<1)

/*------------------------------------------------------------------------
*	macro function definitions
*-----------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/

typedef	unsigned int							osa_timer_t;
typedef unsigned int							osa_timer_context_t;
typedef void	(*osa_timer_proc_t)(osa_timer_t timerId, unsigned int param);


/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
extern int osa_timer_setcallback(osa_timer_t timerId, unsigned int flags, osa_timer_proc_t pfCallback, unsigned int arg, unsigned int delay);
extern int osa_timer_setevent(osa_timer_t timerId, unsigned int flags, osa_thread_t taskId, unsigned int event, unsigned int delay);
extern int osa_timer_cancel(osa_timer_t timerId);

/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif  /* _XOSA_TIMER_H_ */
