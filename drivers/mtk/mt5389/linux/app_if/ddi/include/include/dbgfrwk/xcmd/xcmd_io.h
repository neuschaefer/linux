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

#ifndef _XCMD_IO_H_
#define _XCMD_IO_H_

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
/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
extern int cmd_io_read_int( const char * comment, signed int * pValue );
extern int cmd_io_read_dec( const char * comment, unsigned int * pValue );
extern unsigned int cmd_io_read_hex( const char * comment, unsigned int * pValue );
extern unsigned int cmd_io_read_hex_withsize( char * comment, unsigned short *strSize );
extern unsigned int	cmd_io_read_double( const char * comment, double * pDValue );
extern unsigned int	cmd_io_read_float( const char * comment, float * pFValue );
extern unsigned int	cmd_io_read_string(const char *prompt, char *cmdStr, size_t cmdLen);
extern unsigned int cmd_io_read_number(const char *prompt, int digits, unsigned int val);
extern unsigned int	cmd_io_read_bin_file(const char *prompt, char *fileBuf, size_t fileSize);
extern void  		cmd_io_pause( const char * comment);

/*------------------------------------------------------------------------
*	Private Function Declaration
*-----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  /* _XCMD_IO_H_ */
