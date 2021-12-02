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

#ifndef _XOSA_QUEUE_H_
#define _XOSA_QUEUE_H_

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

/*------------------------------------------------------------------------
*	macro function definitions
*-----------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/
typedef unsigned long	osa_queue_t;		/* Type for id of message queue 	*/

/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
extern osa_queue_t 	osa_queue_init(char *name, int maxCnt, int msgLen, int prio);
extern int 			osa_queue_destroy(osa_queue_t qid);
extern int 			osa_queue_sendmsg(osa_queue_t qid, void *msg, unsigned long msz);
extern int 			osa_queue_sendmsg_urgent(osa_queue_t qid, void *msg, unsigned long msz);
extern int 			osa_queue_recvmsg(osa_queue_t qid, void *msg, unsigned long msz, int waitMs);
extern int 			osa_queue_flushmsg(osa_queue_t qid);
extern unsigned int osa_queue_getcount(osa_queue_t qid);
extern unsigned int osa_queue_getcount_remained(osa_queue_t qid);


/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/





#ifdef __cplusplus
}
#endif

#endif  /* _XOSA_QUEUE_H_ */
