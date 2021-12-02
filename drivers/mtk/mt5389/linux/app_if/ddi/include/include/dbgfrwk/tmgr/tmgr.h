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

#ifndef _TMGR_H_
#define _TMGR_H_

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	(Control Constants)
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	include files
*-----------------------------------------------------------------------*/
#include 	"dbgfrwk_types.h"
#include	<termios.h>

/*------------------------------------------------------------------------
*	constant definitions
*-----------------------------------------------------------------------*/
#define TMGR_ROOT		0

/*------------------------------------------------------------------------
*	macro function definitions
*-----------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
extern int 		tmgr_focuson_me(void);
extern int 		tmgr_system(int idx, char * fmt, ...);
extern int 		tmgr_system_ex(int idx, char * cmd, char * ttyname);
extern void 	tmgr_broadcast(char * cmd);
extern int 		tmgr_attach(char * ttyname);
extern int 		tmgr_dettach(void);
extern char * 	tmgr_root_termname(void);
extern char * 	tmgr_org_termname(void);
extern void 	tmgr_stdout_lock(void);
extern void 	tmgr_stdout_unlock(void);

extern int 		tmgr_register_process(pid_t pid);
extern int 		tmgr_unregister(pid_t pid);
/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/





#ifdef __cplusplus
}
#endif

#endif  /* _INIT_H_ */
