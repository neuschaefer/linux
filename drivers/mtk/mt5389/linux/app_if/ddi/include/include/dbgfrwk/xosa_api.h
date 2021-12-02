/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 1999 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file osa_api.h
 *
 *	POSIX thread based System Call API.
 *
 *	@author 	cskim
 *	@version	1.0
 *	@date		2005. 6. 1
 *	@note
 *	@see
 */

/******************************************************************************
	Header File Guarder
******************************************************************************/

#ifndef _XOSA_API_H_
#define _XOSA_API_H_
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
	#include 파일들 (File Inclusions)
******************************************************************************/
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <sched.h>
#ifndef _EMUL_WIN
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <vfs.h>
#endif

#ifndef _EMUL_WIN
/* ------------------------ End of SYSTEM IPC 관련 API ---------------------- */
/******************************************************************************
	#include 파일들 (File Inclusions)
******************************************************************************/
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/vfs.h>
#include <sys/time.h>
#include <linux/reboot.h>
#if !defined(bool_t) && !defined(__GLIBC__)
#include <rpc/types.h>
#endif

#endif  /* _EMUL_WIN */


#include "xosa/xosa.h"
#include "xosa/xosa_thread.h"
#include "xosa/xosa_signal.h"
#include "xosa/xosa_lock.h"
#include "xosa/xosa_queue.h"
#include "xosa/xosa_event.h"
#include "xosa/xosa_timer.h"


#include "xosa/xosa_mem.h"


/******************************************************************************
	상수 정의(Constant Definitions)
******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif  /* _XOSA_API_H_ */
