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

#ifndef _XOSA_H_
#define _XOSA_H_

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

/******************************************************************************
	매크로 함수 정의 (Macro Definitions)
******************************************************************************/
#define	OSA_TASK_RESOURCE_MAX						(256)		/** 생성할 수 있는 Task의 최대 개수 */
#define	OSA_MSGQ_RESOURCE_MAX		  		  		(64*4)		/** 생성할 수 있는 Message Queue의 최대 개수 */
#define	OSA_MEM_BLK_RESOURCE_MAX					(64)		/** 생성할 수 있는 Memory Block의 최대 개수 */
#define OSA_MEMREG_RESOURCE_MAX						(16)		/** 생성할 수 있는 Memory Region의 최대 개수 */
#define	OSA_SEMA_RESOURCE_MAX						(256)		/** 생성할 수 있는 세마포어의 최대 개수 */
#define	OSA_MUTEX_RESOURCE_MAX						(256)		/** 생성할 수 있는 Mutex의 최대 개수 */
#define	OSA_TIMER_CONTEXT_MAX						(64)		/**
																		 * 생성할 수 있는 Timer Context의 최대 개수.
																		 * 각 Timer Context별로 사용할 수 있는 Timer ID는 UINT32 범위에서
																		 * 자유롭게 설정 가능
																		 */
#define	OSA_REALTIME_TASK_PRIORITY_MAX				(99)
#define	OSA_REALTIME_TASK_PRIORITY_MIN				(1)
#define OSA_SCHEDULE_LOCK_PRIORITY					(94)
#define OSA_SYS_TIMER_TASK_PRIORITY					(93)
#define OSA_REPEAT_CHECK_TASK_PRIORITY				(92)
#define	OSA_ROOT_TASK_PRIORITY						(91)

#define	OSA_USER_TASK_PRIORITY_MAX					(90)		/*OSA_CreateTask()로 생성 가능한 Task의 최대 Priority*/
#define	OSA_USER_TASK_PRIORITY_MIN					(10)		/*OSA_CreateTask()로 생성 가능한 Task의 최소 Priority*/
#define	OSA_USER_TASK_DEFAULT_PRIORITY				(40)		/*OSA_CreateTask()로 Task 생성할 때 Default로 사용하는 Priority*/


#define OSA_USER_TASK_STACK_SIZE_MIN				(16*1024)	/**
																		 *	OSA_CreateTask()로 Task 생성할 때 가능한 최소 Stack Size.
																		 *	<p>
																		 *	- Linux-Only: Linux thread의 경우 최소 16 KB가 필요한데 그 이유는 다음과 같다.
																		 *	  sys 6KB + Guard 4KB + User 6KB = 16KB <p>
																		 *	  이 값은 PTHREAD_STACK_MIN이라는 값으로 정의되어 있다.
																		 */

#define	OSA_USER_TASK_STACK_SIZE_DEFAULT			(24*1024)	/*OSA_CreateTask()로 Task 생성할 때 사용하는 Default Stack Size.*/

/*--------------------------------------------------------------------------+
| Constant Define
+---------------------------------------------------------------------------*/
#define OSA_NM_MAX				 OBJ_NM_MAX
#define TASK_COMM_LEN			 16
#define LG_KERN_SZ			0x2000

#define	ID_MASK_SEMA		0x1000	/* Task is waiting for semaphore/mutex	*/
#define ID_MASK_MSGQ		0x2000	/* Task is waiting for message queue	*/
#define	ID_MASK_EVNT		0x4001	/* Task is waiting for task event		*/
#define	ID_MASK_CHAR		0x4003	/* Task is waiting for key input		*/
#define	ID_MASK_TIME		0x400f	/* Task is sleeping 					*/


/*--------------------------------------------------------------------------+
| Macros for task memory check flag mask
+---------------------------------------------------------------------------*/
#define OSA_MEM_CHECK_MASK		( 0x30 )

/*--------------------------------------------------------------------------+
| Constant for task creation
+---------------------------------------------------------------------------*/
#define DEFAULT_STACK_SIZE      (4*1024)		/* Default 4k byte stack	*/
#define DEFAULT_TASK_PRIORITY   (     8)		/* Default Task Priority	*/
#define CREATE_TASK_LEVEL(_x)	(_x&0x3)		/* 0 : Normal Task Create   */
												/* 1 : System Task Create   */
												/* 2 : Already Created Task
												       but regist to Task
													   Pool                 */

/*--------------------------------------------------------------------------+
| Constant to give timeout value
+---------------------------------------------------------------------------*/
#define OSA_INF_WAIT	        (    -1)		/* Wait for ever			*/
#define OSA_NO_WAIT             (     0)		/* No wait					*/
#define OSA_WAIT_FOREVER	    OSA_INF_WAIT

/*--------------------------------------------------------------------------+
| Return code for timeout in waiting
+---------------------------------------------------------------------------*/
enum
{
	OSA_ERROR     				=	-1,		/* from API_ERROR				*/
	OSA_INVALID_PARAM			=	-2,		/* from PARAMETER ERROR			*/
	OSA_NOT_AVAILABLE			=	-3,		/* from API_NOT_ENOUGH_RESOURCE	*/
	OSA_NOT_CALLABLE			=	-4,		/* from API_NOT_SUPPORTED		*/
	OSA_ERR_LENGTH				=	-5,		/* from API_NOT_PERMITTED		*/
	OSA_ERR_TIMEOUT				=	-6,		/* from API_TIMEOUT				*/

	OSA_ERR_OBJ_DELETED			=	-101
};

/*--------------------------------------------------------------------------+
| Semaphore related OSA return codes
+---------------------------------------------------------------------------*/
#define OSA_SEM_SUCCESS         0x0
#define OSA_SEM_NOT_VALID       OSA_INVALID_PARAM			/*	-2			*/
#define OSA_SEM_NOT_AVAILABLE   OSA_NOT_AVAILABLE			/*	-3			*/
#define OSA_SEM_NOT_CALLABLE    OSA_NOT_CALLABLE			/*	-4			*/
#define OSA_SEM_TIMEOUT         OSA_ERR_TIMEOUT		    	/*	-6			*/
#define OSA_SEM_DELETED         OSA_ERR_OBJ_DELETED			/*	-101		*/

/*--------------------------------------------------------------------------+
| Message Queue related OSA return codes
+---------------------------------------------------------------------------*/
#define OSA_MSG_NOT_VALID       OSA_INVALID_PARAM			/*	-2			*/
#define OSA_MSG_NOT_AVAILABLE   OSA_NOT_AVAILABLE			/*	-3			*/
#define OSA_MSG_NOT_CALLABLE    OSA_NOT_CALLABLE			/*	-4			*/
#define OSA_MSG_LENGTH_ERROR    OSA_ERR_LENGTH				/*	-5			*/
#define OSA_MSG_TIMEOUT         OSA_ERR_TIMEOUT		    	/*	-6			*/
#define OSA_MSG_DELETED         OSA_ERR_OBJ_DELETED			/*	-101		*/

/*--------------------------------------------------------------------------+
| Message Queue related OSA return codes
+---------------------------------------------------------------------------*/
#define OSA_EVT_NOT_VALID       OSA_INVALID_PARAM			/*	-2			*/
#define OSA_EVT_NOT_AVAILABLE   OSA_NOT_AVAILABLE			/*	-3			*/
#define OSA_EVT_NOT_CALLABLE    OSA_NOT_CALLABLE			/*	-4			*/
#define OSA_EVT_TIMEOUT         OSA_ERR_TIMEOUT				/*	-6			*/
#define OSA_EVT_DELETED         OSA_ERR_OBJ_DELETED			/*	-101		*/

/*--------------------------------------------------------------------------+
| Other adaptation codes
+---------------------------------------------------------------------------*/
#define OSA_SLEEP_ON_LOCAL      0
#define OSA_SLEEP_ON_REMOTE     1
#define OS_ERROR                ERROR            /* ERROR = -1 for VxWorks */
#define OS_REMOTE_SUCCESS       OK               /* OK    =  0 for VxWorks */

#ifndef	SMF_NONE

#define	SMF_NONE		0				/* No flag are set				*/
#define	SMF_BOUNDED		0				/* No meaning in vxworks		*/
#define SMF_RECURSIVE	0x00000001		/* Recursive, default for semM	*/
#define SMF_PI_CONTROL	0				/* Priority Inversion Control	*/

#endif

/*------------------------------------------------------------------------
*	macro function definitions
*-----------------------------------------------------------------------*/
#define CURRENT_TASK	osa_thread_self()

/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/

typedef struct
{
	int reserved;
}osa_attr_t;

/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
extern int osa_attr_init( osa_attr_t * attr);
extern int osa_init ( osa_attr_t * attr);


/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/



#ifdef __cplusplus
}
#endif

#endif  /* _XOSA_H_ */
