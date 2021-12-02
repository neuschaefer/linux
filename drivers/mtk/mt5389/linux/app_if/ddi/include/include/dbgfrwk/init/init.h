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

#ifndef _INIT_H_
#define _INIT_H_

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

typedef enum
{
	LOW_SLEVEL,
	MID_SLEVEL,
	HIGH_SLEVEL,

}DBGFRWK_SLEVEL;

/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/

extern void 			dbgfrwk_init(int daemon);
extern void 			dbgfrwk_setsecuritylev(DBGFRWK_SLEVEL level);
extern DBGFRWK_SLEVEL 	dbgfrwk_getsecuritylev(void);
extern int 	_USER_  	dbgfrwk_server_self;

/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif  /* _INIT_H_ */
