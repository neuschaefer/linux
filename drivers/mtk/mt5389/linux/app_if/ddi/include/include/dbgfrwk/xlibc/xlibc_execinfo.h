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

#ifndef _XLIBC_EXEC_INFO_H_
#define _XLIBC_EXEC_INFO_H_

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
#include <signal.h>

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

/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
/*
	xlibc_sym.c
*/
extern void 		xlibc_sym_load(void);
extern char *		xlibc_sym_getname(unsigned int addr);
extern unsigned int xlibc_sym_getaddr(char *symName);
extern char *		xlibc_getexepath(void);
extern char *  		xlibc_getexenm(void);
extern void 		xlibc_setexenm(char * p_name);

/*
	xlibc_dyn_sym.c
*/
extern int			xlibc_dynsym_load(void);
extern char	*		xlibc_dynsym_getname (unsigned int addr);
extern unsigned int	xlibc_dynsym_getaddr(char *pSymName, char **ppLibName);
/*
	xlibc_addr2line.c
*/
extern int 			xlibc_addr2line(unsigned int addr, char **ppFileName);
extern void 		xlibc_sym_backtrace(void);
extern void			xlibc_backtrace(void);
extern void 		xlibc_sym_backtracebypid(pid_t pid);
extern void 		xlibc_backtracebypid(pid_t pid);
extern void 		xlibc_sym_backtrace_ex(unsigned int size, unsigned int count, unsigned int *res, unsigned int *pStack, int trMode);


/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/

/*
	xlibc_sym.c
*/



/*
	xlibc_dyn_sym.c
*/

/*
	xlibc_demangle.c
*/




#ifdef __cplusplus
}
#endif

#endif  /* _XLIBC_EXEC_INFO_H_ */
