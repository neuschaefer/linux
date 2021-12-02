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

#ifndef _XOSA_THREAD_H_
#define _XOSA_THREAD_H_

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
#include <pthread.h>

/*------------------------------------------------------------------------
*	constant definitions
*-----------------------------------------------------------------------*/
//#define OSA_THREAD_STACK_MIN	PTHREAD_STACK_MIN
#define OSA_THREAD_STACK_MIN	(2*1024*1024)

#define OSA_THREAD_DETACHED		0x00
#define OSA_THREAD_JAINABLE		0x01

/*------------------------------------------------------------------------
*	macro function definitions
*-----------------------------------------------------------------------*/
#define OSA_CPU(_x)	(_x & osa_cpu_mask)
#define OSA_CPU_ALL	(osa_cpu_mask)

/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/
typedef struct
{

	char	name [OSA_NM_MAX];
	void*	(*entry_point)(void *);
	int		priority;
	size_t	stack_size;
	int		sched_policy;
	int		create_flag;	/* no use */
	unsigned long	affnty;

	int		exitstat ;

	void * 	arg;
	pthread_attr_t	* pthread_attr;
	int 	init;
}osa_thread_attr_t;

typedef	unsigned long	osa_thread_t;		/* Type for id of task(process)		*/
typedef void*	(*osa_thread_proc_t)(void *pParam);

/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
extern int osa_thread_attrres_register(osa_thread_attr_t * p_attr);
extern int osa_thread_attrres_unregister(char * p_name);
extern osa_thread_attr_t * 	osa_thread_attrres_find(char *p_name);



extern int osa_thread_attr_init(osa_thread_attr_t * p_osaattr, const char * name);
extern int osa_thread_attr_set(osa_thread_attr_t * p_osaattr, const pthread_attr_t * p_attr);
extern int osa_thread_attr_setstacksize(osa_thread_attr_t * p_osaattr, size_t stack_size);
extern int osa_thread_attr_getstacksize(const osa_thread_attr_t * p_osaattr, size_t * p_stack_size);
extern int osa_thread_attr_setsched (osa_thread_attr_t * p_osaattr, int policy, int priority);
extern int osa_thread_attr_getsched (const osa_thread_attr_t * p_osaattr, int * p_policy, int * p_priority);
extern int osa_thread_attr_setaffnty (osa_thread_attr_t * p_osaattr, int affnty);
extern int osa_thread_attr_getaffnty(const osa_thread_attr_t * p_osaattr, int * p_affnty);
extern int osa_thread_attr_joinable(osa_thread_attr_t * p_osaattr);
extern int osa_thread_attr_detached(osa_thread_attr_t * p_osaattr);

extern pthread_t osa_thread_create(osa_thread_t * thread, osa_thread_attr_t * p_attr,
										void * (*start_routine)(void *),void * arg );



extern int 			osa_thread_cancel(osa_thread_t taskId);

extern osa_thread_t osa_thread_self(void);
extern osa_thread_t osa_thread_byname(char *name);
extern osa_thread_t osa_thread_bypid(pid_t pid);

extern char *		osa_thread_getname(osa_thread_t tId);

extern pid_t		osa_thread_getpid(osa_thread_t tId);
extern pthread_t	osa_thread_getpthread(osa_thread_t tId);


extern unsigned long 	osa_thread_suspend(unsigned int ms);
extern void 			osa_thread_resume(osa_thread_t tid);
extern unsigned long 	osa_thread_sleep(unsigned int ms);


/* IMCHANG 아래 함수들은 일반적인 attr를 설정하는  interface로 변경 필요. */

extern int 			osa_thread_set_cpu(unsigned int mask);
extern int 			osa_thread_set_cpu_raw(unsigned int mask);
extern unsigned int	osa_thread_get_cpumask(void);

extern int 			osa_thread_set_selfpriority(int priority);
extern void 		osa_thread_set_priority(osa_thread_t taskId, int priority);
extern int  		osa_thread_get_priority(osa_thread_t taskId);


extern void 		osa_thread_trace(FILE *fp, char * taskName);
extern void 		osa_thread_info	(FILE * fp, char * taskName);

extern osa_thread_t	osa_thread_register_self(void);
extern int 			osa_thread_find_osvname_byid(void *id, char *nameStr);
extern unsigned int                 	osa_cpu_mask;

/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/






#ifdef __cplusplus
}
#endif

#endif  /* _XOSA_THREAD_H_ */
