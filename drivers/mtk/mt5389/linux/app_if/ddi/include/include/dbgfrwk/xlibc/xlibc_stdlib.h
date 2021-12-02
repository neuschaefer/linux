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

#ifndef _XLIBC_STDLIB_H_
#define _XLIBC_STDLIB_H_

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
#define BITMASK_MEMSIZE(last_pos) ((((last_pos) + 7) >> 3 ) + 4)
/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/

/*	xlibc_map.c	*/
typedef enum
{
	METHOD_AUTO,
	METHOD_PROCFS,
	METHOD_MAX
}MAP_METHOD_T ;

/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
/*
	xlibc_map.c
*/
extern void 				xlibc_map_dump(void) ;
extern void 				xlibc_map_reload(void);

/*
	xlibc_utils.c
*/
extern struct timespec		xlibc_ms2abstime(int ms);
extern uint64_t 			xlibc_read_usticks(void);
extern uint64_t 			xlibc_read_msticks(void);
extern void 				xlibc_delay_ms(unsigned int ms);
extern void 				xlibc_delay_us(unsigned int us);
extern unsigned long		xlibc_suspend(unsigned int ms);
extern unsigned long 		xlibc_calccrc32(unsigned char *buf, int len);
extern unsigned long 		xlibc_rand(void);
extern void 				xlibc_quicksort(void *base, size_t nmemb, size_t size,
						   int (*compar)(const void *, const void *));

extern void *				bitmask_setup (size_t last_pos, unsigned char * mem, unsigned char init_val);
extern void *				bitmask_attach(size_t last_pos, unsigned char * mem);

extern void *				bitmask_alloc(size_t last_pos);
extern void					bitmask_free(void *arg);
extern void					bitmask_set(void *arg, unsigned char set_pos);
extern void					bitmask_clear(void *arg, unsigned char set_pos);
extern int					bitmask_isset(void *arg, unsigned char test_pos);

extern int 					max(int a, int b);
extern int 					min(int a, int b);
extern int 					xlibc_system(const char *command);

/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/

/*
	xlibc_map.c
*/





#ifdef __cplusplus
}
#endif

#endif  /* _XLIBC_STDLIB_H_ */
