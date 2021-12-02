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

#ifndef _XOSA_SIGNAL_H_
#define _XOSA_SIGNAL_H_

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
#include "xlibc_api.h"
/*------------------------------------------------------------------------
*	constant definitions
*-----------------------------------------------------------------------*/


#define	SIGCMD_BASE					(0x80000000)
#define	SIGCMD_DEFINE(x)			((x)+SIGCMD_BASE)

#define	SIGCMD_MAX					32
#define SIGCMD_TRACE				SIGCMD_DEFINE(1)
#define	SIGCMD_LGSYSRQ				SIGCMD_DEFINE(2)
#define SIGCMD_THREAD_PRINTON		SIGCMD_DEFINE(3)
#define SIGCMD_THREAD_PRINTOFF		SIGCMD_DEFINE(4)
#define SIGCMD_MUX_WAKEUP			SIGCMD_DEFINE(5)

#define SIGCMD_END					SIGCMD_DEFINE(32)

/*------------------------------------------------------------------------
*	macro function definitions
*-----------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/
typedef void (*osa_sigcmd_t) 		(int sigNo, siginfo_t *pSigInfo, void *pFrame);
typedef void (*osa_sig_setup_t)  (void);
/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/


/* 	IMCHANG 아래 sigcmd및 exclog 함수들은 모두 thread safty 하지 않다.
	cmd함수가 연달아 오면 이전것을 덮는다.
	exclog도 동기화 되지 않는다.

	이 exclog는 현재 exception이 발생한 thread만이 callback 내에서 호출 되어야만 한다.
*/

extern int 	osa_signal_raised(void);
extern int 	osa_signal_register_after_exclog( bool (* callback)(LOGBUF * logbuf));
extern int 	osa_signal_register_before_exclog( bool (* callback)(LOGBUF * logbuf));

extern void osa_signal_exclog_save(LOGBUF * log);
extern void osa_signal_exclog_dump(FILE * fp, pid_t pid, char * p_name);
extern void osa_signal_last_exclog_dump(void);

extern int 	osa_sigcmd(int cmd_no, osa_sigcmd_t sigcmd, osa_sigcmd_t * ocmd);
extern int 	osa_sigcmd_kill(osa_thread_t tid, int cmd_no);

extern int 	osa_signal_register_installer(void (* signal_install)(void));




/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  /* _XOSA_SIGNAL_H_ */
