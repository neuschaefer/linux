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

#ifndef _LIBM_H_
#define _LIBM_H_

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	(Control Constants)
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------
*	include files
*-----------------------------------------------------------------------*/
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>
#include <pthread.h>
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

/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/
/*=================================================================
	IMCHANG 이하 내용은 나중에 자동으로 생성되어야 함.
===================================================================*/

typedef int (*pthread_create_proc_t)(pthread_t *restrict thread,
		   const pthread_attr_t *restrict attr,
		   void *(*start_routine)(void*), void *restrict arg);

typedef pid_t 	(*fork_proc_t)(void);
typedef int		(*system_proc_t)(const char * cmd);
typedef void *	(*malloc_proc_t)(size_t size) ;
typedef void 	(*free_proc_t)	(void *ptr);
typedef void* 	(*calloc_proc_t)(size_t nmemb, size_t size);
typedef void*	(*realloc_proc_t)(void *ptr, size_t size);
typedef void *  (*memalign_proc_t)(size_t alignment, size_t bytes);
typedef int 	(*posix_memalign_proc_t)(void **memptr, size_t alignment, size_t size);


/*------------------------------------------------------------------------
*	Function Declaration
*-----------------------------------------------------------------------*/
extern void * _WEAK_ 	libm_setup_hook(char * func_nm, void * 	hook_ptr);
extern void  _WEAK_ 	libm_reset_hook(char * func_nm);
extern void _WEAK_  	libm_system_init(void);
extern void * _WEAK_	libm_get_hook(char * func_nm);

#ifdef __cplusplus
}
#endif

#endif  /* _XCMD_API_H_ */
