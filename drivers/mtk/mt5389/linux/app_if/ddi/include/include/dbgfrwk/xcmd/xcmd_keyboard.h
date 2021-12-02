#ifndef	_XCMD_KEYBOARD_H_
#define	_XCMD_KEYBOARD_H_

/*-----------------------------------------------------------------------------
    #include 파일들
    (File Inclusions)
------------------------------------------------------------------------------*/
#include	<stdio.h>

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */


/*------------------------------------------------------------------------------
	매크로 함수 정의
	(Macro Definitions)
------------------------------------------------------------------------------*/


#define CMD_ACT_NONE		0
#define CMD_ACT_FUNC		1
#define CMD_ACT_CMD		2
#define CMD_ACT_MENU		3


#define CMD_ACTION_OBJ(paction)	((void*)(&((paction)->action)))
#define CMD_ACTION_FUNC(paction) 	((cmd_action_func_t*)(&((paction)->action)))
#define CMD_ACTION_CMD(paction) 	((cmd_action_cmd_t*)(&((paction)->action)))
#define CMD_ACTION_MENU(paction) 	((cmd_action_menu_t*)(&((paction)->action)))
#define CMD_TBL_NUM(tbl)	(sizeof(tbl)/sizeof(tbl[0]))

#define CMD_ACTION_PRESSED		((void*)true)
#define CMD_ACTION_UNPRESSED	((void*)false)

/*------------------------------------------------------------------------------
	형 정의
	(Type Definitions)
------------------------------------------------------------------------------*/

typedef struct _cmd_action_func_t
{
	bool (* func)(int * in_key);
	char * help;
}cmd_action_func_t;

typedef struct _cmd_action_cmd_t
{
	char * 			cmd;
	unsigned int	processed;
}cmd_action_cmd_t;

typedef struct _cmd_action_menu_t
{
	cmd_menu_t * 	menu;
	unsigned int	processed;
}cmd_action_menu_t;

typedef	struct _cmd_action_t
{
	int 	key;
	int		type;
	void * 	action;
	void * 	option;
}cmd_action_t;

/*------------------------------------------------------------------------------
	Public Extern 전역변수와 함수 prototype 선언
	(Extern Variables & Function Prototype Declarations)
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
extern int 	cmd_kb_register_spkey_page (char * pagename, char * desc, cmd_action_t table [],int table_num);
extern int 	cmd_kb_register_char_action(char chkey, int type, void * action_obj);
extern int 	cmd_kb_register_char_tbl(cmd_action_t table [], int table_num);

extern void * 	cmd_kb_task (void * arg);
extern void	 	cmd_kb_system(char * cmd);

extern int		cmd_kb_set_spkey_page (char * p_pagename);
extern void 	cmd_kb_ctrl_specialkey(int enb) ;

extern int 		cmd_kb_get_status_specialkey(void);
extern void 	cmd_kb_ctrl_char(int enb);
extern int 		cmd_kb_get_status_char(void);
extern int		cmd_kb_ctrl_print(int print);

/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/
//extern bool cmd_kb_check_specialkey(int *pInkey);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/*	_XCMD_KEYBOARD_H_ */
