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

#ifndef _CMD_H_
#define _CMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "osa_printex.h"


typedef cmd_menu_t CMD_DBG_MENU_T;



#define CMD_DBG_RegisterCmd 	cmd_dbg_register_cmd
#define CMD_DBG_HelpMenu		cmd_dbg_helpmenu
/*
 * Debug Menu Table.
 */
typedef struct DEBUG_MENU {

	UINT8	number;							/**< debug number */
	UINT8	string[30];						/**< debug menu string */
	void	(*function)(void);				/**< debug function pointer */
	UINT32	moduleMask;						/**< debug module print mask */

} DEBUG_MENU_T;


extern 	void CMD_Init(void);
extern	void CMD_System(char * cmd);

#define UartGetChar2 	xlibc_rgetc

#ifdef __cplusplus
}
#endif

#endif  /* _CMD_H_ */
