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

#ifndef _XLIBC_STRING_H_
#define _XLIBC_STRING_H_

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

/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
/*
	xlibc_utils.c
*/
extern unsigned long		xlibc_strtoul(const char *s, char **ptr, int base);
extern char*				xlibc_strtok(char *srcStr, const char *del, char **ptr);
extern char *				xlibc_strtrim(char *src);
extern int					xlibc_str2argv(char *line, int limit, char **argv);
extern int					xlibc_str2index_inopts(char **argv, const char *opts, int *output);
extern char *				xlibc_strdup(const char *s);
extern char * 				xlibc_strupr( const char *s_str );
extern char *				xlibc_strlwr( const char *s_str );

/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/



#ifdef __cplusplus
}
#endif

#endif  /* _XLIBC_STDLIB_H_ */
