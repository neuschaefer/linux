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

#ifndef _XCMD_MASK_H_
#define _XCMD_MASK_H_

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




/*------------------------------------------------------------------------
*	constant definitions
*-----------------------------------------------------------------------*/
#define CMD_GROUP_NAME_LEN				16
#define CMD_BIT_NAME_LEN				CMD_GROUP_NAME_LEN


#define CMD_MASK_BIT_MAX				32
#define	CMD_MASK_BIT(num)			(((num)>=0 && (num) < CMD_MASK_BIT_MAX)?(num):-1)


#define CMD_MASK_GROUP_MAX				9
#define	CMD_MASK_GROUP(num)		(((num)>=0 && (num) < CMD_MASK_GROUP_MAX)?(num):-1)

#define CMD_MASK_DEFAULT		{					\
		0,											\
		{ {{0,},0}, {{0,},0}, {{0,},0}, {{0,},0},	\
		  {{0,},0}, {{0,},0}, {{0,},0}, {{0,},0} 	\
		},											\
}

/*------------------------------------------------------------------------
*	macro function definitions
*-----------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/

typedef struct _mask_bit_attr_t
{
	char    * 			p_bitname;
	int					bit_num;
    unsigned long     	nm_flag;
    int     			color;

} cmd_mask_bit_attr_t;


typedef struct _mask_group_attr_t
{
	char	group_name[CMD_GROUP_NAME_LEN];
	int		group_num;
	cmd_mask_bit_attr_t	bits[CMD_MASK_BIT_MAX];
} cmd_mask_group_attr_t;


typedef struct {
	unsigned char	color[CMD_MASK_BIT_MAX];
	unsigned int	bitmask;
} cmd_mask_alloc_t;

typedef struct
{
	unsigned long				msk;
	cmd_mask_alloc_t			group[CMD_MASK_GROUP_MAX];
} cmd_mask_t;


/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
extern	int 	cmd_mask_register_group(cmd_mask_group_attr_t * p_attr);
extern  void 	cmd_mask_print(int group_num, int bit_num, const char *format, ...);
/*------------------------------------------------*/
extern 	int 		cmd_mask_init 		(cmd_mask_t *printMask);
extern	int 		cmd_mask_load 	(void);
extern 	int 		cmd_mask_save 	(void);
extern 	cmd_mask_t 	cmd_mask_get		(void);
extern	void 		cmd_mask_set		(cmd_mask_t mask);
/*------------------------------------------------*/
extern void 	cmd_mask_enable(void);
extern void 	cmd_mask_disable(void);

extern	void 	cmd_mask_bit_enable(int group_num/* -1 : all group*/, int bit_num /* -1 : all bit*/);
extern 	void 	cmd_mask_bit_disable(int group_num /* -1 : all group*/, int bit_num /* -1 : all bit*/);

extern void 	cmd_mask_group_enable(int group_num /* -1 : all group */);
extern void 	cmd_mask_group_disable(int group_num /* -1 : all group */);

extern void 	cmd_mask_set_color(int group_num, int bit_num , char color );
/*------------------------------------------------*/
extern  int 	cmd_mask_status(void);
extern	int 	cmd_mask_group_status (int group_num);
extern	int 	cmd_mask_bit_status (int group_num, int bit_num);
extern 	char * 	cmd_mask_group_name(int group_num);
extern 	char * 	cmd_mask_bit_name(int group_num, int bit_num);
/*------------------------------------------------*/
extern void 	cmd_mask_dump(void);
extern void 	cmd_mask_group_dump (int group_num /* -1 : all */, int mode /* 0: all, 1: on, 2: off*/);
extern void 	cmd_mask_bit_dump		(int group_num /*-1 : all*/, int bit_num/*-1:all*/, int mode/* 0: all, 1: on, 2: off*/);
/*------------------------------------------------*/
extern void 	cmd_builtin_mask_interpreter_menu (void);

/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  /* _XCMD_MASK_H_ */
