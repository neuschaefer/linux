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

#ifndef _XCMD_H
#define _XCMD_H

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
#include "xcmd_mask.h"



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
typedef struct _cmd_attr_t
{
	int 		ctrl_standalone ;
	bool		(*enter_menu_callback)(cmd_menu_t *pmenu, char **argv);
	bool		(*exit_menu_callback)(cmd_menu_t *pmenu, char **argv);
	bool		(*kb_task_user_callback)(void);


	int			(*load_mask_proc)(cmd_mask_t * p_mask);
	int			(*save_mask_proc)(cmd_mask_t * p_mask);

	cmd_menu_t	*	psdm_global;
	cmd_menu_t	*	psdm_main;
}cmd_attr_t ;

/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
extern int cmd_attr_ctrl_standalone(cmd_attr_t * attr, bool ctrl);
extern int cmd_set_attr_enter_menu(cmd_attr_t * attr, bool (*callback)(cmd_menu_t *pmenu, char **argv));
extern int cmd_set_attr_exit_menu(cmd_attr_t * attr, bool (*callback)(cmd_menu_t *pmenu, char **argv));
extern int cmd_attr_set_global_menu(cmd_attr_t * attr, cmd_menu_t * menu);
extern int cmd_attr_set_main_menu(cmd_attr_t * attr,cmd_menu_t * menu);
extern int cmd_set_keyboard_usermode(cmd_attr_t * attr, bool (*callback)(void));

extern int cmd_set_attr_mask_proc (cmd_attr_t * attr,
							int (*load_mask_proc)(cmd_mask_t * mask),
							int (*save_mask_proc)(cmd_mask_t * mask));

extern int cmd_attr_init(cmd_attr_t * attr);
extern int cmd_attr_get(cmd_attr_t * attr);
extern int cmd_init( cmd_attr_t * attr);


extern void cmd_keyboard_task (void);

/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  /* _XCMD_H */
