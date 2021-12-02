#ifndef	_XCMD_DEBUG_H_
#define	_XCMD_DEBUG_H_

/*-----------------------------------------------------------------------------
    #include 파일들
    (File Inclusions)
------------------------------------------------------------------------------*/
#include	<stdio.h>

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

#define CMD_FILTER_MAX			10
#define CMD_LINELEN 	   		256
#define CMD_ARG_NUM				16

/*------------------------------------------------------------------------------
	매크로 함수 정의
	(Macro Definitions)
------------------------------------------------------------------------------*/
#define	TAG_USAGE_LABEL(expr)						\
	if (expr)										\
	{												\
PrintHelpAndReturn:									\
		cmd_dbg_helpmenu(NULL, 0);					\
		return;										\
	}

#define	GOTO_USAGE_LABEL()	goto PrintHelpAndReturn

#define DMT_HELP_TAG		"^"
#define DMT_VOPT_END		"\0"

#define	GET_EXTM(_p)		((cmd_menu_t *)_p->dm_next - 1)

/*------------------------------------------------------------------------------
	형 정의
	(Type Definitions)
------------------------------------------------------------------------------*/

/*
**	Debug Menu 를 위한 구조체 초기값
**
**	  - 첫번째 Entry는 Prompt를 위해 Description 만 사용한다.
**	  - 마지막 Entry는 Termination 을 위해 dm_type 이 반드시
**			DMT_TERM(Terminal node 인 경우) 또는
**			DMT_EXTM(Extended menu 가 있는 경우)
**		이어야 한다. DMT_TERM일 때는 dm_name 도 같이 NULL이어야 한다.
**
**	  - dm_type 정의 방법.
**		DMT_NONE: 아무런 Arguement도 전달하지 않는다.
**			f(void)
**
**		DMT_ARG1: 명령행에서 입력한 argument중 첫번째것만 String으로 전달
**			f(char *arg1)
**
**		DMT_ARG2: 명령행에서 입력한 argument를 처음 두개만 String으로 전달.
**			f(char *arg1, char *arg2) 형태의 함수
**
**		DMT_OPTS: dm_opts 에 정의된 값을 argument로 전달한다.
**			f(int)
**
**		DMT_INDX: dm_help에 정의된 option string과 입력된 arguement를 비교하여
**                일치하는 option의 번호를 arguement 로 전달한다.
**			f(int)
**			[1..15]		 : 1부터 15까지의 숫자. 입력하지 않으면 -1
**			1..15		 : 1부터 15까지의 숫자. 입력하지 않으면 오류
**			[off|on|str] : "on" = 0, "off" = 1, "str" = 2, 입력하지 않으면 -1
**			off|on|str	 : "on" = 0, "off" = 1, "str" = 2, 입력하지 않으면 오류
**          1,kor|2,eng  : "kor" 또는 "1" = 1, "eng" 또는 "2" = 2, 입력하지 않으면 오류
**
**		DMT_ARGV: 명령행에서 입력한 argument를 argvlist 로 만들어서 전달한다.
**			f(char **argv)
**
**		DMT_ARGCV: 명령행에서 입력한 argument를 argvlist 로 만들어서 전달한다.
**			f(int argc, char **argv)
**
**		DMT_LINK: 다른 메뉴 구조체의 명령을 실행한다. 실행할 명령은
**			      dm_help 에 기록한다.
**
**	 	DMT_MENU: dm_args 에 정의된 Menu 구조체와 argvlist를 함께 전달한다.
**			f(cmd_menu_t *, char **argv)
**
**		DMT_DESC: Menu list중 제일 처음 것으로 Menu의 내용애 대한 설명을 가진다.
**
**		DMT_EXTM: Menu list의 제일 마지막 것으로 dm_next가 가리키는 곳에 확장
**			menu(사용자가 추가 등록한 것들)가 있음을 알린다.
**
**		DMT_TERM: Menu list의 제일 마지막 것으로 list의 끝을 알린다.
**			dm_name도 같이 NULL이여야 한다.
*/

typedef struct {
	char		*dm_name;		/* Command Name of debug menu			*/
	void		*dm_next;		/* Call Context for Next Level			*/
								/*  1. Function pointer to be called	*/
								/*	2. Menu Structure to be passed		*/
	int			dm_opts;		/* Optional argument to be passed		*/
	int			dm_type;		/* Call type of sub function or Menu	*/
	char		*dm_desc;		/* Description of debug menu			*/
	char		*dm_help;		/* Detail help message of usage 		*/
								/*  must start with option strings, it	*/
								/*  will be parsed while command line	*/
								/*  interpreting 						*/
} cmd_menu_t;

#define	DMT_NONE		 0		/* Pass no arguement       				*/
#define	DMT_ARG1		11		/* Pass only first one arguement as str */
#define	DMT_ARG2		12		/* Pass only first two arguement as str */
#define	DMT_ARGV		18		/* Pass argc and argv list  			*/
#define	DMT_ARGCV		19		/* Pass argv list        				*/
#define	DMT_OPTS		21		/* Pass optional arguement 				*/
#define DMT_INDX		22		/* Pass Index of matched options string	*/
#define DMT_LINK		23		/* link to command in other menu 		*/
#define	DMT_DESC		80		/* Item describing current menu			*/
#define	DMT_EXTM		81		/* Link to user added command menus		*/
#define	DMT_TERM		82		/* End mark of list of menu items		*/
#define	DMT_MENU		90		/* Pass Menu Struct & Argv List 		*/
								/* This should be last item				*/

#define	DM_SHOW_HELP	 1		/* Show help on entering new menu		*/
#define	DM_EXIT_ON_CR	 2		/* Exit on enter command				*/
#define DM_BUF_ALLOC	 4		/* cmd_menu_t stroage is allocated on	*/

/*------------------------------------------------------------------------------
	Public Extern 전역변수와 함수 prototype 선언
	(Extern Variables & Function Prototype Declarations)
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
/* Function to add new command into menu, in cmd_debug.c	*/
extern void cmd_dbg_main(void);
extern int 	cmd_dbg_main_menu(cmd_menu_t *pMenu, char **argv);
extern int 	cmd_dbg_register_cmd(cmd_menu_t *pParent, char *name, void *next, int opts, int type, char *desc, char *help);
extern int 	cmd_dbg_register_cmdstruct(cmd_menu_t *pParent, cmd_menu_t *pCmd);
extern int 	cmd_dbg_register_to_global(cmd_menu_t *pCmd);
extern int 	cmd_dbg_register_to_main(cmd_menu_t *pCmd);
extern int 	cmd_dbg_register_to_test(cmd_menu_t *pCmd);
extern void cmd_dbg_not_coded_yet(void);
extern void cmd_dbg_helpmenu(cmd_menu_t *pMenu, int help_mode);

extern int cmd_dbg_register_filter_out(const char * cmd);
extern int cmd_dbg_register_filter_in(const char * cmd);
extern char ** cmd_dbg_get_filter_in(int * size);
/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/*	_XCMD_DEBUG_H_ */
