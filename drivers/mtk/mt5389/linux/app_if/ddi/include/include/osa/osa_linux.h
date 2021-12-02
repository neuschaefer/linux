/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 1999 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file osa_linux.h
 *
 *	System Start point at Linux system
 *
 *	@author 	changwook.im
 *	@version	2.0
 *	@date		2011. 07. 14
 *	@note
 *	@see
 */

#ifndef _OSA_LINUX_H_
#define _OSA_LINUX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include	"adap.h"
#include 	"xlibc.h"
#include 	<signal.h>

#define SIGLGSYSRQ					SIGUSR2//(SIGRTMIN + 1)

typedef struct _DUMP_ABN_POWEROFF_HIST_
{
	int inputIdx;
	int powerOffMode;
	int sourceIndex;
	int physicalNum;
	int majorNum;
	int minorNum;
} DUMP_HIST_T;

extern int 	OSA_DoKernelUserInitcall(void);
extern int 	OSA_CheckKernelUserInitcallDone(void);
extern char *OSA_ReadCmdLine(void);
extern int 	OSA_PrintkControl(int bctrl);
extern int 	OSA_KoLazyLoadCtrl(int bctrl);
extern int 	OSA_MarkUserInterrupt(unsigned long mark_time);
extern void OSA_SysKernPrintCtrl(int bEnb);
extern int 	OSA_SetPrintOnOff(int debugMode, int initMode);
extern void OSA_SetPanicValue(int newvalue);
extern int 	OSA_LGSysRqInit(void);
extern void OSA_InstallSigSysrqHndlr(void);
extern void OSA_SystemCacheFlush(void);
extern void OSA_SetOverCommitRatio(int ratio);
extern int 	OSA_SetPrintOnOff(int debugMode, int initMode);
extern int 	OSA_SVC_Printf(const char *format, ... );
extern char * OSA_GetKernMsgBuff( int * len );
extern char * OSA_DumpMemInfo(void);
extern int 	OSA_ExecCommand(const char *format , ... );
extern void OSA_CreateRWFSCheckPoint(void);
extern void OSA_CloseRWFS(void);
extern void OSA_AbnormalPowerOffHistory(DUMP_HIST_T dh);

extern void OSA_SetReadAheadValue(char *partname, int newvalue);
extern int 	OSA_GetSysKernPrintStat(void);

extern int 	OSA_GetPartSz(char *name);
extern int 	OSA_GetPartIdx(char *name);
extern void OSA_ShowPartmap(void);
extern int 	OSA_InitPartInfo(void);
extern void OSA_SetupRWFS(void);
extern void OSA_CloseRWFS(void);


extern void OSA_SetInit(void);
extern int 	OSA_GetInitStat(void);

#endif	/* _OSA_LINUX_H	*/
