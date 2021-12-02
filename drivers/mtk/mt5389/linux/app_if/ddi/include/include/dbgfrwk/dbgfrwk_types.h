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

#ifndef 	_DBGFRWK_TYPE_H_
#define 	_DBGFRWK_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif
/*-----------------------------------------------------------------------------
	(Control Constants)
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	include files
*-----------------------------------------------------------------------*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

/*------------------------------------------------------------------------
*	constant definitions
*-----------------------------------------------------------------------*/


#define INF_WAIT	        (    -1)		/* Wait for ever			*/
#define NO_WAIT           	(     0)		/* No wait			*/
#define WAIT_FOREVER	    INF_WAIT


#define CMN_BUF_LEN			256
#define	CMN_NM_MAX			16

#define OBJ_NM_MAX			CMN_BUF_LEN

/*------------------------------------------------------------------------
*	macro function definitions
*-----------------------------------------------------------------------*/
#ifndef _EMUL_WIN
#define EXPORT extern __attribute__((visibility("default")))
#define __INIT__ __attribute__((constructor))
#define __EXIT__ __attribute__((destructor))
#define _TLS_		__thread
#define TLS_VALID(tls)	((unsigned long *)(&tls) > (&_end) )
#define SYMBOL_ALIAS(_n, _an)   extern __typeof (_n) _an __attribute__ ((alias (#_n)));
#define _WEAK_	__attribute__((weak))
#define _USER_
#else
#define EXPORT
#define	__INIT__
#define __EXIT__
#define	_TLS_
#define _WEAK_
#define SYMBOL_ALIAS(_n, _an)
#define _USERDEFINE_

#endif

#ifdef __cplusplus
#define	VARG_PROTO	...
#else
#define	VARG_PROTO
#endif	/* __cplusplus */


#define	LS_ISEMPTY(listp)													\
		(((lst_t *)(listp))->ls_next == (lst_t *)(listp))

#define	LS_INIT(listp) {													\
		((lst_t *)(&(listp)[0]))->ls_next = 								\
		((lst_t *)(&(listp)[0]))->ls_prev = ((lst_t *)(&(listp)[0]));		\
}

#define	LS_INS_BEFORE(oldp, newp) {											\
		((lst_t *)(newp))->ls_prev = ((lst_t *)(oldp))->ls_prev;			\
		((lst_t *)(newp))->ls_next = ((lst_t *)(oldp));						\
		((lst_t *)(newp))->ls_prev->ls_next = ((lst_t *)(newp));			\
		((lst_t *)(newp))->ls_next->ls_prev = ((lst_t *)(newp));			\
}

#define	LS_INS_AFTER(oldp, newp) {											\
		((lst_t *)(newp))->ls_next = ((lst_t *)(oldp))->ls_next;			\
		((lst_t *)(newp))->ls_prev = ((lst_t *)(oldp));						\
		((lst_t *)(newp))->ls_next->ls_prev = ((lst_t *)(newp));			\
		((lst_t *)(newp))->ls_prev->ls_next = ((lst_t *)(newp));			\
}

#define	LS_REMQUE(remp) {													\
	if (!LS_ISEMPTY(remp)) {												\
		((lst_t *)(remp))->ls_prev->ls_next = ((lst_t *)(remp))->ls_next;	\
		((lst_t *)(remp))->ls_next->ls_prev = ((lst_t *)(remp))->ls_prev;	\
		LS_INIT(remp);														\
	}																		\
}

#define LS_BASE(type, basep) ((type *) &(basep)[0])


#define LS_HEAD_DECLARE		lst_t * ls_prev ; lst_t* ls_next ; char * container_type = "list";
/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/

#ifndef __cplusplus
#ifndef bool
typedef unsigned long bool;
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif
#endif


#ifndef mtx_t
typedef	pthread_mutex_t	mtx_t;
#endif

#ifndef uint64_t
#ifndef _EMUL_WIN
typedef	unsigned long long		__uint64_t;
#else
typedef	unsigned _int64			__uint64_t;
#endif
#define uint64_t __uint64_t
#endif

#ifndef sint64_t
#ifndef _EMUL_WIN
typedef	signed long long		__sint64_t;
#else
typedef	signed _int64			__sint64_t;
#endif
#define sint64_t __sint64_t
#endif

#ifndef size_t
typedef	unsigned int	size_t;
#endif

#ifndef ssize_t
typedef	signed int		ssize_t;
#endif

typedef	struct ls_elt {
	struct ls_elt	*ls_next;
	struct ls_elt	*ls_prev;
} lst_t;


typedef struct
{
	unsigned int	reg_beg;
	unsigned int	reg_end;
	unsigned int	reg_opt;
} region_t;

typedef 	int			(* print_proc_t)(const char *fmt, ...);
typedef 	int			(* fprint_proc_t)(FILE * stream, const char *fmt, ...);

typedef 	int 		(* int_proc_t)(void);				/* ptr to function returning int   */
typedef 	void 		(* void_proc_t)(void);				/* ptr to function returning void  */
typedef 	double 		(* double_proc_t)(void);				/* ptr to function returning double*/

typedef		void	vfp_proc (void);
typedef 	void	ufp_proc (unsigned long);



/*------------------------------------------------------------------------
*	Function Declaration
*-----------------------------------------------------------------------*/

//#if   defined (__UCLIBC__)
#if 0
extern void  DBGFRWK_SYSTEM_INIT(void) _WEAK_ ;
#else
extern void  __INIT__ DBGFRWK(void) _WEAK_ ;
#endif

#define CHECK_DO_SYSTEM_INIT()	(DBGFRWK==NULL)

#ifdef __cplusplus
}
#endif

#endif  /* _DBGFRWK_TYPE_H_ */
