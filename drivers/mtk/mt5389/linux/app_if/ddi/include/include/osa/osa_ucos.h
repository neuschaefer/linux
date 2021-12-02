/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*
 *	Operating System Adpatation Layer for vxWorks
 */

#ifndef _OSA_UCOS_H_
#define _OSA_UCOS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------+
| Include essential header file for VxWorks
+----------------------------------------------------------------------------*/
#include	<ucos_ii.h>

/*----------------------------------------------------------------------------+
| Include essential header file for O/S adaptation and debug
+----------------------------------------------------------------------------*/
#include	"osa/osa_types.h"
#include	"osa/dbgprint.h"

#define 	FAST register
/*----------------------------------------------------------------------------+
| Return codes
+----------------------------------------------------------------------------*/
#define OSA_OK		(0)
#define OSA_NOT_OK	(1)
#define ERROR		NOT_OK

/*----------------------------------------------------------------------------+
| Semaphore/Mutex related defines and macros.
+----------------------------------------------------------------------------*/
#define	SMT_ACC			0				/* Semaphore for access control 	*/
#define	SMT_MTX			1				/* Special Binary Sema4 for Mutex	*/
#define	SMT_SER			2				/* Semaphore for serialization		*/

#define	SMF_NONE		0				/* No flag are set				*/

#define	SMF_BOUNDED		0				/* No meaning in vxworks		*/
#define SMF_RECURSIVE	0x00000001		/* Recursive, default for semM	*/
#define SMF_PI_CONTROL	0				/* Priority Inversion Control	*/


/*----------------------------------------------------------------------------+
| Defines comes from vxworks message/semaphore/events
+----------------------------------------------------------------------------*/
#define MSG_Q_FIFO 0
#define SEM_Q_FIFO 0
#define SEM_EMPTY 0
#define SEM_DELETE_SAFE 0
#define EVENTS_SEND_IF_FREE 0
#define EVENTS_WAIT_ALL		0
#define EVENTS_WAIT_ANY		0


/*----------------------------------------------------------------------------+
| Defines for tty control flags
| Copied from /usr/include/sys/termios.h
+----------------------------------------------------------------------------*/
/* Flag bis for uartLFlags */
#define ISIG	0x0001
#define ICANON	0x0002
#define ECHO	0x0004
#define ECHOE	0x0008
#define ECHOK	0x0010
#define ECHONL	0x0020
#define NOFLSH	0x0040
#define TOSTOP	0x0080
#define IEXTEN	0x0100
#define FLUSHO	0x0200
#define ECHOKE	0x0400
#define ECHOCTL	0x0800

/* Flag bis for uartOFlags */
#define OPOST	0x00001
#define OLCUC	0x00002
#define OCRNL	0x00004
#define ONLCR	0x00008
#define ONOCR	0x00010
#define ONLRET	0x00020
#define OFILL	0x00040
#define CRDLY	0x00180


/*----------------------------------------------------------------------------+
| Tick related macros.
+----------------------------------------------------------------------------*/
#define	KC_MS_PER_TICK	 (1000 / sysClkRateGet())				// 100Hz
#define MS2TICKS(time)	 ((time+KC_MS_PER_TICK-1) / KC_MS_PER_TICK)

/*----------------------------------------------------------------------------+
| Other miscellaneous macros.
+----------------------------------------------------------------------------*/
#define NELEMENTS(array)	(sizeof(array) / sizeof((array)[0]))

/*----------------------------------------------------------------------------+
| Global Type declaration wrapper for Task,Queue,Semaphore
+----------------------------------------------------------------------------*/
typedef	OS_EVENT *		SID_TYPE;		/* Type for id of semaphore			*/
typedef SID_TYPE		SEM_ID;
typedef	unsigned char	TID_TYPE;		/* Type for id of task(process)		*/
typedef	OS_EVENT * 		QID_TYPE;		/* Type for id of message quue		*/
typedef unsigned long	MSG_TYPE[4];	/* Type for fixed size of msg		*/
typedef const unsigned	int	FILE;

typedef long int	__time_t;
typedef __time_t	time_t;
struct	timespec
{
	__time_t	tv_sec;
	long int	tv_nsec;
};

struct tm
{
  int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
  int tm_min;			/* Minutes.	[0-59] */
  int tm_hour;			/* Hours.	[0-23] */
  int tm_mday;			/* Day.		[1-31] */
  int tm_mon;			/* Month.	[0-11] */
  int tm_year;			/* Year	- 1900.  */
  int tm_wday;			/* Day of week.	[0-6] */
  int tm_yday;			/* Days in year.[0-365]	*/
  int tm_isdst;			/* DST.		[-1/0/1]*/

#ifdef	__USE_BSD
  long int tm_gmtoff;		/* Seconds east of UTC.  */
  __const char *tm_zone;	/* Timezone abbreviation.  */
#else
  long int __tm_gmtoff;		/* Seconds east of UTC.  */
  __const char *__tm_zone;	/* Timezone abbreviation.  */
#endif
};

typedef  void (*PFNCT)(int);

/*----------------------------------------------------------------------------+
| Global variables.
+----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
| Object Specific Variables
|  Keep track of general stats of calls to these APIs.
|  1. Keep full name objects(task, semaphore, message queue)
|  2. Log address of caller of binary or mutex semaphore
|  3. Log success/fail count of messsage queue
+----------------------------------------------------------------------------*/
#define NUM_OF_HASH_ROWS		128
#define NUM_OF_HASH_COLS		  4

#define	OSV_A_SM_T		 0x00	// Object type is Binary Semaphore(Access Control)
#define	OSV_M_SM_T		 0x01	// Object type is Mutex Semaphore(Mutual Exclusion)
#define	OSV_S_SM_T		 0x02	// Object type is Count Semaphore(Serialization)
#define	OSV_MSGQ_T		 0x04	// Object type is Message Queue
#define	OSV_TASK_T		 0x05	// Object type is Task

typedef struct {				// Total 0x0c bytes
	UINT16	ov_wait;			// 0x14: Semaphore/Message id waiting for
	UINT16	ov_rsv1;			// 0x16: _reserved for future use.
	UINT32	ov_mUse;			// 0x18: Memory usage of task.
	UINT32	ov_addr;			// 0x1c: Where has OSA_WaitSem() been called
} osv_tt;

typedef struct {				// Total 0x0c bytes
	UINT16	ov_iVal;			// 0x14: Initial Lock Counter
	UINT16	ov_recu;			// 0x16: Recursive Counter
	UINT32	ov_task;			// 0x18:
	UINT32	ov_addr;			// 0x1c: Where has OSA_WaitSem() been called
} osv_st;

typedef struct {				// Total 0x0c bytes
	UINT16	ov_nErr;			// 0x14: Number of failures in sendMsg()
	UINT16	ov_nSnd;			// 0x16: Number of messages sent
	UINT16	ov_nRcv;			// 0x18: Number of messages received
	UINT16	ov_nMax;			// 0x1a: Maximum length of message
	UINT32	ov_ptr;				// 0x1c: Pointer to MSG buffer
} osv_qt;

typedef	struct {				// Total 0x20 bytes.
	char	ov_name[8];			// 0x00: Name of object
	void	*ov_pObj;			// 0x08: Pointer to object struct
	UINT8	ov_type;			// 0x0c: Type of this object.
	UINT8	ov_grId;			// 0x0d: Group id for task.
	UINT16	ov_myId;			// 0x0e: Object ID of task/semaphore.
	UINT32	ov_errno;			// 0x10: Last error code for this object.
	union
	{
		osv_tt	t;				// 0x14: TSV Task    Specific Variable
		osv_st	s;				// 0x14: TSV Sema4   Specific Variable
		osv_qt	q;				// 0x14: TSV Message Specific Variable
	} u;
} osv_t;

extern osv_t  osvHash[NUM_OF_HASH_ROWS][NUM_OF_HASH_COLS];

/*----------------------------------------------------------------------------+
| File related globals declared in libucos.c
+----------------------------------------------------------------------------*/
extern FILE *stdin, *stdout, *stderr;

/*----------------------------------------------------------------------------+
| Function Prototypes.
+----------------------------------------------------------------------------*/

/*========================================================================
 *
 *                   System Call Definition ( uC/OS specific )
 *
 *=======================================================================*/

/*========================================================================
 * Defined in libucos.c
 *=======================================================================*/
extern int		printf (const char *fmt, ...);
extern int		pollPrint(char *fmt, ...);
extern int		fprintf(FILE *fp, char *fmt, ...);
extern int		intLock();
extern void		intUnlock(int status);
extern int		getc(FILE *fp);
extern int		fflush(FILE *fp);
extern char*	fgets(char *c, int n, FILE *fp);
extern void		putchar(UINT8 digit);
extern void		puts(char *c);
extern void		fputs(char *c, FILE *fp);
extern int		taskDelay(int ms);
extern int		taskSuspend(short tid);
extern float	atof(char *in);
extern int		max(int a, int b);
extern int		min(int a, int b);
extern int		abs(int a);
extern int		INUM_TO_IVEC(int in);
extern int		IVEC_TO_INUM(int in);
extern UINT64	sysClkReadNanoSec (void);

/*========================================================================
 * Defined in vsprintf.c
 *=======================================================================*/
extern int		sprintf(char * buf, const char *fmt, ...);
extern int		sscanf(const char * buf, const char * fmt, ...);
extern int		atoi(const char *s);
extern ULONG	strtoul(const char *cp,char **endp,unsigned int base);
extern long		strtol(const char *cp,char **endp,unsigned int base);

/*========================================================================
 * Defined in snprintf.c
 *=======================================================================*/
extern int snprintf (char *, size_t, const char *, ...);
extern int vsnprintf (char *, size_t, const char *, va_list);

/*========================================================================
 * Defined in bsp_c.c
 *=======================================================================*/
extern void		intConnect(int intNum, PFNCT func, int arg, char*name);
extern void		fastintConnect(int intVector, PFNCT isr, int arg, char*name);
extern int		intEnable(int intNum);
extern int		intDisable(int intNum);
extern int		intContext();
extern void		setIntNum(int num);
extern int		getIntNum(void);
extern int		isr_manager_init(void);

/*========================================================================
 * Defined in osadap.c
 *=======================================================================*/
extern int		msgIdListGet(int idList[], int maxMsgs);
extern int		semIdListGet(int idList[], int maxSems);

/*========================================================================
 * Defined in osamem.c
 *=======================================================================*/
extern void*	malloc(size_t);
extern void*	calloc(size_t, size_t);
extern void*	realloc(void *, size_t);
extern void		free(void *);

/*========================================================================
 * Defined in string.c
 *=======================================================================*/
extern char*	strcpy(char * dest,const char *src);
extern char*	strncpy(char * dest,const char *src,size_t count);
extern char*	strcat(char * dest, const char * src);
extern char*	strncat(char *dest, const char *src, size_t count);
extern int		strcmp(const char * cs,const char * ct);
extern int		strcasecmp(const char * cs,const char * ct);
extern int		strncmp(const char * cs,const char * ct,size_t count);
extern char*	strchr(const char * s, int c);
extern char*	strrchr(const char * s, int c);
extern size_t	strlen(const char * s);
extern size_t	strnlen(const char * s, size_t count);
extern char*	strdup(const char *s);
extern void*	memset(void * s,int c,size_t count);
extern char*	bcopy(const char * src, char * dest, int count);
extern void*	memcpy(void * dest,const void *src,size_t count);
extern void*	memmove(void * dest,const void *src,size_t count);
extern int		memcmp(const void * cs,const void * ct,size_t count);
extern char*	strstr(const char * s1,const char * s2);

#ifdef __cplusplus
}
#endif

#endif	/* _OSA_UCOS_H	*/
