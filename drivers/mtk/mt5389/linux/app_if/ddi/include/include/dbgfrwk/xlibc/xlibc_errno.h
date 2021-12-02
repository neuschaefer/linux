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

#ifndef _XLIBC_ERRNO_H_
#define _XLIBC_ERRNO_H_

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
#include <errno.h>
/*------------------------------------------------------------------------
*	constant definitions
*-----------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	macro function definitions
*-----------------------------------------------------------------------*/

#define xlibc_abort(format, args...)				xlibc_abort_msg(__FILE__, __FUNCTION__, __LINE__, format, ##args)
#define xlibc_assert(condition, format, args...) 	(condition) ? (void) 0 : \
													xlibc_assert_msg(__FILE__, __FUNCTION__, __LINE__, format,##args)
#define xlibc_critical(format, args...)				xlibc_critical_msg(__FILE__, __FUNCTION__, __LINE__, format, ##args)
#define xlibc_warning(format, args...)			xlibc_warning_msg(__FILE__, __FUNCTION__, __LINE__, format, ##args)

/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/


/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
extern void xlibc_abort_msg(const char *szFile, const char *szFunction, const int nLine, const char *format, ...);
extern void xlibc_critical_msg(const char *szFile, const char *szFunction, const int nLine, const char *format, ...);
extern void xlibc_assert_msg (const char *szFile, const char *szFunction, const int nLine
					 , const char *str, ...);
extern void xlibc_warning_msg (const char *szFile, const char *szFunction, const int nLine, const char *format, ...);


extern void xlibc_err_sys(const char *fmt, ...);
extern void xlibc_err_sys_abort(const char *fmt, ...);
extern void xlibc_err_user(int error, const char *fmt, ...);
extern void xlibc_err_user_abort(int error, const char *fmt, ...);
extern void xlibc_err_msg(const char *fmt, ...);
extern void xlibc_err_msg_abort(const char *fmt, ...);

/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/



#ifdef __cplusplus
}
#endif

#endif  /* _XLIBC_ERRNO_H_ */
