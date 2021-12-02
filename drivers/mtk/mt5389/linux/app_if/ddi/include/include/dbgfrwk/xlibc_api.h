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

#ifndef _XLIBC_API_H_
#define _XLIBC_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	(Control Constants)
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	include files
*-----------------------------------------------------------------------*/
#include <ctype.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>

#include "dbgfrwk_types.h"
#include "xlibc/xlibc_init.h"
#include "xlibc/xlibc_stdlib.h"
#include "xlibc/xlibc_stdio.h"
#include "xlibc/xlibc_execinfo.h"
#include "xlibc/xlibc_termios.h"
#include "xlibc/xlibc_string.h"
#include "xlibc/xlibc_errno.h"
#include "xlibc/xlibc_time.h"
#include "arch/xlibc_arch.h"

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
*	Function Declaration
*-----------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif  /* _XLIBC_API_H_ */
