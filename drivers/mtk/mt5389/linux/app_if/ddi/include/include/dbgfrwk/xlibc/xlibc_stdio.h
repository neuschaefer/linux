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

#ifndef _XLIBC_STDIO_H_
#define _XLIBC_STDIO_H_

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
#include "pc_key.h"

/*------------------------------------------------------------------------
*	constant definitions
*-----------------------------------------------------------------------*/

/*------------------------------------------------------------------------
*	macro function definitions
*-----------------------------------------------------------------------*/
#define PRINT_FLAG_NULL 		(0x00000000)	/* by junorion */
#define PRINT_FLAG_TPRINT		(0x00000001)
#define PRINT_FLAG_RPRINT		(0x00000010)
#define PRINT_FLAG_EXCLOG		(0x00000100)
#define PRINT_FLAG_OSD			(0x00001000)

typedef enum
{
	PRINT_MODE_NULL 	= 0,
	PRINT_MODE_PRINT 	= 1,
	PRINT_MODE_LOGBUF	= 2,
	PRINT_MODE_SYSLOG	= 3,
	PRINT_MODE_OSD		= 4,
	PRINT_MODE_RAW 		= 5,

}PRINT_MODE_T ;

#define DEF_LOGBUF_SIZE			0x4000


/*------------------------------------------------------------------------
*	type definitions
*-----------------------------------------------------------------------*/

typedef const unsigned	int LOGBUF;


/*------------------------------------------------------------------------
*	Public Function Declaration
*-----------------------------------------------------------------------*/
/*	output	*/
extern void xlibc_print_ctrl(int ctrl) ;
extern void xlibc_print_ctrl_onself(int ctrl);

extern int	xlibc_print_status(void) ;
extern int 	xlibc_print_status_onself(void);
extern int 	xlibc_print_allstatus(void);

extern void xlibc_print_ctrl_syslog(int enable);
extern void xlibc_print_ctrl_ringbuf(int enable);

extern void xlibc_hexdump(const char *name, void *vcp, int size);
extern void xlibc_fhexdump(FILE * fp, const char *name, void *vcp, int size);

extern void hexdump(const char *name, void *vcp, int size);


extern int f_dbgprint(FILE * stream ,const char *format , ... );
extern int dbgprint(const char *format , ... );

extern int f_tprint0n(FILE * stream, const char *format , ... );
extern int tprint0n(const char *format , ... );

extern int f_tprint1n(FILE * stream ,const char *format , ... );
extern int tprint1n(const char *format , ... );

extern int f_rprint0n(FILE * stream, const char *format , ... );
extern int rprint0n(const char *format , ... );

extern int f_rprint1n(FILE * stream, const char *format , ... );
extern int rprint1n(const char *format , ... );

extern int f_aprint0n(FILE * stream, const char *format , ... );
extern int aprint0n(const char *format , ... );

extern int f_aprint1n(FILE * stream, const char *format , ... );
extern int aprint1n(const char *format , ... );

extern int vf_tprint0n(FILE * stream, const char *format , va_list ap );
extern int va_tprint0n(const char *format , va_list ap );

extern int vf_rprint0n(FILE * stream, const char *format , va_list ap );
extern int va_rprint0n(const char *format , va_list ap );

extern int vf_aprint0n(FILE * stream, const char *format , va_list ap );
extern int va_aprint0n(const char *format , va_list ap );

extern int 	xlibc_osdprintf(const char *format, ...);
extern int 	xlibc_syslog(const char *format , ... );
extern int 	rawprintf(const char *format, ... );


extern LOGBUF * xlibc_log_create(int size);
extern void 	xlibc_log_delete(LOGBUF * log);
extern char * 	xlibc_log_getbuf(LOGBUF * log);
extern int 		xlibc_log_dup2(LOGBUF * dst_log, LOGBUF * src_log);
extern LOGBUF * xlibc_log_dup (LOGBUF * src_log);

extern void 	xlibc_log_reset(void);
extern void 	xlibc_log_lreset(LOGBUF * log);
extern int 		xlibc_log_vprint(const char *format , va_list args);
extern int 		xlibc_log_vlprint(LOGBUF * log, const char *format , va_list args);
extern int 		xlibc_log_print(const char *format, ...);
extern int 		xlibc_log_lprint(LOGBUF * log, const char *format, ...);
extern void 	xlibc_log_dump(FILE * fp);
extern void 	xlibc_log_ldump(LOGBUF * log, FILE * fp);
extern void 	xlibc_log_console(void);
extern void 	xlibc_log_lconsole(LOGBUF * log);

extern int 		xlibc_log_lwrite(LOGBUF * log, char * dst, size_t size);
extern int 		xlibc_log_write(char * dst, size_t size);


/*	input	*/
extern int 		xlibc_getc(FILE *fp);
extern int 		xlibc_getc_e(FILE *fp);	/* with echo */
extern int 		xlibc_rgetc(FILE *fp);	/* raw getc */
extern int 		xlibc_rgetc_t(FILE *fp, int waitms); /* raw getc with time*/
extern char *	xlibc_fgets(char *buf, int n, FILE *fp);
extern char *	xlibc_read(char *buf, int n, FILE *fp);	/* binary read */

extern int		xlibc_print_value;
extern int		xlibc_print_process_nm;

extern FILE *	stdosd;
extern FILE * 	stdlog;

extern FILE *  	stdsyslog;
extern FILE * 	stdraw;
extern FILE *	stdnull;

extern LOGBUF * stdlogbuf;

extern FILE * 	xlibc_stdout;

/*------------------------------------------------------------------------
*	Priviate Function Declaration
*-----------------------------------------------------------------------*/

/*	output	*/



/*	input	*/





#ifdef __cplusplus
}
#endif

#endif  /* _XLIBC_STDIO_H_ */
