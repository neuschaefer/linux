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

#ifndef _XLIBC_INIT_H_
#define _XLIBC_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	(Control Constants)
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	include files
*-----------------------------------------------------------------------*/
#include <stdio.h>
/*------------------------------------------------------------------------
*	constant definitions
*-----------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	macro function definitions
*-----------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/

typedef struct xlibc_attr_t
{
/* output */

	const char* (*print_name_callback)	(void);
	int 		(*print_osd_callback)	(char *,...);
/* input */
	bool		(*getc_before_callback)(int *ret_key, FILE *fp);
	bool		(*getc_after_callback)(int *ret_key,FILE *fp);

	bool		(*fgets_before_callback)(char *buf, int n, FILE *fp);
	bool		(*fgets_after_callback)(char *buf, int n, FILE *fp);
	bool		(*fgets_in_str_callback)(int ch,char *buf, int n);
	bool		(*special_key_callback)(int * key) ;
	void		(*kill_signal_trace_callback)(pid_t pid);

}xlibc_attr_t ;


/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/


/* output attr */
extern int			xlibc_attr_set_print_name	(xlibc_attr_t * attr,	const char * (*getname)(void));
extern int			xlibc_attr_set_print_osd		(xlibc_attr_t * attr,	int (*osdprint)(char*,...));
extern int			xlibc_set_print_osd			(int (*osdprintf)(char*,...));

/* input attr */
extern int 			xlibc_attr_set_getc_before(xlibc_attr_t * attr, bool (*callback)(int *ret_key, FILE *fp));
extern int			xlibc_attr_set_getc_after(xlibc_attr_t * attr,	 bool (*callback)(int *ret_key, FILE *fp));

extern int 			xlibc_attr_set_fgets_before(xlibc_attr_t * attr, bool (*callback)(char *buf, int n, FILE *fp));
extern int			xlibc_attr_set_fgets_after(xlibc_attr_t * attr,	 bool (*callback)(char *buf, int n, FILE *fp));
extern int			xlibc_attr_set_fgets_in_str(xlibc_attr_t * attr,	 bool (*callback)(int ch,char *buf, int n));

extern int 			xlibc_attr_set_special_key(xlibc_attr_t * attr,	 bool (*callback)(int * key));
extern int			xlibc_attr_set_kill_sigtrace(xlibc_attr_t * attr,	 void (*kill_signal_trace_callback)(pid_t pid));

/* input attr */
extern int 			xlibc_attr_init	(xlibc_attr_t * attr);
extern int 			xlibc_attr_get	(xlibc_attr_t * attr) ;
extern int 			xlibc_init(const xlibc_attr_t * attr);


/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/



#ifdef __cplusplus
}
#endif

#endif  /* _XLIBC_INIT_H_ */
