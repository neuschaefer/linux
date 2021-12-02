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

#ifndef _XOSA_LOCK_H_
#define _XOSA_LOCK_H_

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
//#define	OSA_MUTEX_TYPE_NORMAL			0
//#define	OSA_MUTEX_TYPE_RECURSIVE		(1<<1)

/*------------------------------------------------------------------------
*	macro function definitions
*-----------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/
typedef unsigned long	osa_lock_t; 	/* Type for id of semaphore 		*/

/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
extern osa_lock_t 	osa_lock_mutex_init(char *name, int sFlag);
extern osa_lock_t 	osa_lock_bsema_init(char *name, int sFlag, int iCount);
extern osa_lock_t 	osa_lock_ssema_init(char *name, int sFlag, int iCount);
extern void 		osa_lock_destroy( osa_lock_t which );
extern int 			osa_lock_post( osa_lock_t which );
extern int 			osa_lock_wait( osa_lock_t which, int wait_ms );

/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/





#ifdef __cplusplus
}
#endif

#endif  /* _XOSA_LOCK_H_ */
