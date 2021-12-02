/******************************************************************************

    LGE. DTV RESEARCH LABORATORY
    COPYRIGHT(c) LGE CO.,LTD. 2002. SEOUL, KOREA.
    All rights are reserved.
    No part of this work covered by the copyright hereon may be
    reproduced, stored in a retrieval system, in any form or
    by any means, electronic, mechanical, photocopying, recording
    or otherwise, without the prior permission of LG Electronics.

    FILE NAME   : osadap.h
    VERSION     :
    AUTHOR      : Jaegil Lee(jackee@lge.com), Seokjoo Lee( contigo@lge.com )
    DATE        : 2002.07.19
    DESCRIPTION : Operating System Adpatation Layer for pSOS and vxWorks
	              Includes various redefined error codes from system call
*******************************************************************************/

#ifndef _OSA_H_
#define _OSA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "xosa_api.h"
#include "common.h"

#define _DEBUG_OSADAP /* Undef this if you don't need to debug SCAPI */



/*--------------------------------------------------------------------------+
| System Boot up mode
+---------------------------------------------------------------------------*/
#define SYS_DBG_MODE			0				/* Debug  Mode */
#define SYS_EVT_MODE			1				/* Event  Mode */
#define SYS_REL_MODE			2				/* Relese Mode */

/*----------------------------------------------------------------------------+
| Task Create Status
+-----------------------------------------------------------------------------*/
#define FLAG_TASK_C_START		0				/* task center start flag              */
#define FLAG_TASK_L_START		1				/* task local(indivisuaal ) start flag */
#define FLAG_TASK_STARTED		2				/* task already started flag           */
#define FLAG_TASK_DELETED		3				/* task deleted flag                   */


#define FLAG_CPU_0				0
#define FLAG_CPU_1				1
#define FLAG_CPU_ALL			0xffffffff

#ifndef _EMUL_WIN
#define ERROR		NOT_OK
#endif

#ifndef CONST_FUNC
#define	CONST_FUNC				__INIT__
#endif

/*----------------------------------------------------------------------------+
| linux Adaptation Layer API prototypes
+-----------------------------------------------------------------------------*/
typedef osa_thread_t	 	TID_TYPE;
typedef osa_lock_t			SID_TYPE;
typedef osa_queue_t			QID_TYPE;
typedef unsigned long		SG_TYPE[4];

typedef	 QID_TYPE			OSA_MSGQ_T;
typedef	 TID_TYPE			OSA_TASK_T;
typedef	 osa_lock_t			OSA_MUTEX_T;
typedef	 osa_lock_t			OSA_SEMA_T;
typedef osa_timer_proc_t 	TIMER_CALLBACK_T;
typedef TIMER_CALLBACK_T	OSA_TIMER_CONTEXT_T;

typedef osa_thread_attr_t	TASK_CONF_T;

typedef UINT32				OSA_MBLK_T;
typedef UINT32				OSA_MRGN_T;


typedef osa_timer_t			OSA_TIMER_T;


typedef region_t			REGION_T;
typedef osa_thread_attr_t	TASK_INIT_T;
typedef	 int				kernel_state;


typedef osa_thread_proc_t	TASK_PROC_T;

#ifndef TIME_T
#define TIME_T xlibc_time_t
#endif
typedef print_proc_t 		PRT_FUNCPTR;
typedef int_proc_t			INT_FUNCPTR;				/* ptr to function returning int   */
typedef void_proc_t			VOIDFUNCPTR;				/* ptr to function returning void  */
typedef double_proc_t		DBL_FUNCPTR;				/* ptr to function returning double*/


typedef struct {
    pthread_cond_t cond;
    pthread_mutex_t mutex;
} OSA_COND_VAL;


#define	OSA_Syscall               		syscall
#define	OSA_Exit                  		exit
#define	OSA_Fork                  		fork
#define	OSA_Read                  		read
#define	OSA_Write                 		write
#define	OSA_Open                  		open
#define OSA_Open64				  		pen64
#define	OSA_Close                 		close
#define	OSA_Waitpid               		waitpid
#define	OSA_Creat                 		creat
#define	OSA_Link                  		link
#define	OSA_Unlink                		unlink
#define	OSA_Execve                		execve
#define	OSA_Chdir                 		chdir
#define	OSA_Time                  		time
#define	OSA_Mknod                 		mknod
#define	OSA_Chmod                 		chmod
#define	OSA_Lchown                		lchown
#define	OSA_Break                 		break
#define	OSA_Unused18              		unused18
#define	OSA_Lseek                 		lseek
#define OSA_Lseek64				  		lseek64
#define	OSA_Getpid                		getpid
#define	OSA_Mount                 		mount
#define	OSA_Umount                		umount
#define	OSA_Setuid                		setuid
#define	OSA_Getuid                		getuid
#define	OSA_Stime                 		stime
#define	OSA_Ptrace                		ptrace
#define	OSA_Alarm                 		alarm
#define	OSA_Unused28              		unused28
#define	OSA_Pause                 		pause
#define	OSA_Utime                 		utime
#define	OSA_Stty                  		stty
#define	OSA_Gtty                  		gtty
#define	OSA_Access                		access
#define	OSA_Nice                  		nice
#define	OSA_Ftime                 		ftime
#ifndef _EMUL_WIN
#define	OSA_Sync                  		sync
#else
#define OSA_Sync						_flushall
#endif
#define	OSA_Kill                  		kill
#define	OSA_Rename                		rename
#define	OSA_Mkdir                 		mkdir
#define	OSA_Rmdir                 		rmdir
#define	OSA_Dup                   		dup
#define	OSA_Pipe                  		pipe
#define	OSA_Times                 		times
#define	OSA_Prof                  		prof
#define	OSA_Brk                   		brk
#define	OSA_Setgid                		setgid
#define	OSA_Getgid                		getgid
#define	OSA_Signal                		signal
#define	OSA_Geteuid               		geteuid
#define	OSA_Getegid               		getegid
#define	OSA_Acct                  		acct
#define	OSA_Umount2               		umount2
#define	OSA_Lock                  		lock
#define	OSA_Ioctl                 		ioctl
#define	OSA_Fcntl                 		fcntl
#define	OSA_Mpx                   		mpx
#define	OSA_Setpgid               		setpgid
#define	OSA_Ulimit                		ulimit
#define	OSA_Unused59              		unused59
#define	OSA_Umask                 		umask
#define	OSA_Chroot                		chroot
#define	OSA_Ustat                 		ustat
#define	OSA_Dup2                  		dup2
#define	OSA_Getppid               		getppid
#define	OSA_Getpgrp               		getpgrp
#define	OSA_Setsid                		setsid
#define	OSA_Sigaction             		sigaction
#define	OSA_Sgetmask              		sgetmask
#define	OSA_Ssetmask              		ssetmask
#define	OSA_Setreuid              		setreuid
#define	OSA_Setregid              		setregid
#define	OSA_Sigsuspend            		sigsuspend
#define	OSA_Sigpending            		sigpending
#define	OSA_Sethostname           		sethostname
#define	OSA_Setrlimit             		setrlimit
#define	OSA_Getrlimit             		getrlimit
#define	OSA_Getrusage             		getrusage
#define	OSA_Gettimeofday          		gettimeofday
#define	OSA_Settimeofday          		settimeofday
#define	OSA_Getgroups             		getgroups
#define	OSA_Setgroups             		setgroups
#define	OSA_Reserved82            		reserved82
#define	OSA_Symlink               		symlink
#define	OSA_Unused84              		unused84
#define	OSA_Readlink              		readlink
#define	OSA_Uselib                		uselib
#define	OSA_Swapon                		swapon
#define	OSA_Reboot                		reboot
#define	OSA_Readdir               		readdir
#define	OSA_Mmap                  		mmap
#define	OSA_Munmap                		munmap
#define	OSA_Truncate              		truncate
#define	OSA_Ftruncate             		ftruncate
#define	OSA_Fchmod                		fchmod
#define	OSA_Fchown                		fchown
#define	OSA_Getpriority           		getpriority
#define	OSA_Setpriority           		setpriority
#define	OSA_Profil                		profil
#define	OSA_Statfs                		statfs
#define	OSA_Fstatfs               		fstatfs
#define	OSA_Ioperm                		ioperm
#define	OSA_Socketcall            		socketcall
#define	OSA_Syslog                		syslog
#define	OSA_Setitimer             		setitimer
#define	OSA_Getitimer             		getitimer
#define	OSA_Stat                  		stat
#define	OSA_Stat64                		stat64
#define	OSA_Lstat                 		lstat
#define	OSA_Fstat                 		fstat
#define	OSA_Unused109             		unused109
#define	OSA_Iopl                  		iopl
#define	OSA_Vhangup               		vhangup
#define	OSA_Idle                  		idle
#define	OSA_Vm86                  		vm86
#define	OSA_Wait4                 		wait4
#define	OSA_Swapoff               		swapoff
#define	OSA_Sysinfo               		sysinfo
#define	OSA_Ipc                   		ipc
#ifndef _EMUL_WIN
#define	OSA_Fsync                 		fsync
#else
#define	OSA_Fsync()					NULL
#endif
#define	OSA_Sigreturn             		sigreturn
#define	OSA_Clone                 		clone
#define	OSA_Setdomainname         		setdomainname
#define	OSA_Uname                 		uname
#define	OSA_Modify_Ldt            		modify_ldt
#define	OSA_Adjtimex              		adjtimex
#define	OSA_Mprotect              		mprotect
#define	OSA_Sigprocmask           		sigprocmask
#define	OSA_Create_Module         		create_module
#define	OSA_Init_Module           		init_module
#define	OSA_Delete_Module         		delete_module
#define	OSA_Get_Kernel_Syms       		get_kernel_syms
#define	OSA_Quotactl              		quotactl
#define	OSA_Getpgid               		getpgid
#define	OSA_Fchdir                		fchdir
#define	OSA_Bdflush               		bdflush
#define	OSA_Sysfs                 		sysfs
#define	OSA_Personality           		personality
#define	OSA_Afs_Syscall           		afs_syscall
#define	OSA_Setfsuid              		setfsuid
#define	OSA_Setfsgid              		setfsgid
#define	OSA__Llseek               		_llseek
#define	OSA_Getdents              		getdents
#define	OSA__Newselect            		_newselect
#define	OSA_Flock                 		flock
#define	OSA_Msync                 		msync
#define	OSA_Readv                 		readv
#define	OSA_Writev                		writev
#define	OSA_Cacheflush            		cacheflush
#define	OSA_Cachectl              		cachectl
#define	OSA_Sysmips               		sysmips
#define	OSA_Unused150             		unused150
#define	OSA_Getsid                		getsid
#define	OSA_Fdatasync             		fdatasync
#define	OSA__Sysctl               		_sysctl
#define	OSA_Mlock                 		mlock
#define	OSA_Munlock               		munlock
#define	OSA_Mlockall              		mlockall
#define	OSA_Munlockall            		munlockall
#define	OSA_Sched_Setparam        		sched_setparam
#define	OSA_Sched_Getparam        		sched_getparam
#define	OSA_Sched_Setscheduler    		sched_setscheduler
#define	OSA_Sched_Getscheduler    		sched_getscheduler
#define	OSA_Sched_Yield           		sched_yield
#define	OSA_Sched_Get_Priority_Max		sched_get_priority_max
#define	OSA_Sched_Get_Priority_Min		sched_get_priority_min
#define	OSA_Sched_Rr_Get_Interval 		sched_rr_get_interval
#define	OSA_Nanosleep             		nanosleep
#define	OSA_Mremap                		mremap
#define	OSA_Accept                		accept
#define	OSA_Bind                  		bind
#define	OSA_Connect               		connect
#define	OSA_Getpeername           		getpeername
#define	OSA_Getsockname           		getsockname
#define	OSA_Getsockopt            		getsockopt
#define	OSA_Listen                		listen
#define	OSA_Recv                  		recv
#define	OSA_Recvfrom              		recvfrom
#define	OSA_Recvmsg               		recvmsg
#define	OSA_Send                  		send
#define	OSA_Sendmsg               		sendmsg
#define	OSA_Sendto                		sendto
#define	OSA_Setsockopt            		setsockopt
#define	OSA_Shutdown              		shutdown
#define	OSA_Socket                		socket
#define	OSA_Socketpair            		socketpair
#define	OSA_Setresuid             		setresuid
#define	OSA_Getresuid             		getresuid
#define	OSA_Query_Module          		query_module
#define	OSA_Poll                  		poll
#define	OSA_Nfsservctl            		nfsservctl
#define	OSA_Setresgid             		setresgid
#define	OSA_Getresgid             		getresgid
#define	OSA_Prctl                 		prctl
#define	OSA_Rt_Sigreturn          		rt_sigreturn
#define	OSA_Rt_Sigaction          		rt_sigaction
#define	OSA_Rt_Sigprocmask        		rt_sigprocmask
#define	OSA_Rt_Sigpending         		rt_sigpending
#define	OSA_Rt_Sigtimedwait       		rt_sigtimedwait
#define	OSA_Rt_Sigqueueinfo       		rt_sigqueueinfo
#define	OSA_Rt_Sigsuspend         		rt_sigsuspend
#define	OSA_Pread64               		pread64
#define	OSA_Pwrite64              		pwrite64
#define	OSA_Chown                 		chown
#define	OSA_Getcwd                		getcwd
#define	OSA_Capget                		capget
#define	OSA_Capset                		capset
#define	OSA_Sigaltstack           		sigaltstack
#define	OSA_Sendfile              		sendfile
#define	OSA_Getpmsg               		getpmsg
#define	OSA_Putpmsg               		putpmsg
#define	OSA_Mmap2                 		mmap2
#define	OSA_Truncate64            		truncate64
#define	OSA_Ftruncate64           		ftruncate64
#define	OSA_Lstat64               		lstat64
#define	OSA_Fstat64               		fstat64
#define	OSA_Pivot_root            		pivot_root
#define	OSA_Mincore               		mincore
#define	OSA_Madvise               		madvise
#define	OSA_Getdents64            		getdents64
#define	OSA_Fcntl64               		fcntl64
#define	OSA_Reserved221           		reserved221
#define	OSA_Gettid                		gettid
#define	OSA_Readahead             		readahead
#define	OSA_Setxattr              		setxattr
#define	OSA_Lsetxattr             		lsetxattr
#define	OSA_Fsetxattr             		fsetxattr
#define	OSA_Getxattr              		getxattr
#define	OSA_Lgetxattr             		lgetxattr
#define	OSA_Fgetxattr             		fgetxattr
#define	OSA_Listxattr             		listxattr
#define	OSA_Llistxattr            		llistxattr
#define	OSA_Flistxattr            		flistxattr
#define	OSA_Removexattr           		removexattr
#define	OSA_Lremovexattr          		lremovexattr
#define	OSA_Fremovexattr          		fremovexattr
#define	OSA_Tkill                 		tkill
#define	OSA_Sendfile64            		sendfile64
#define	OSA_Futex                 		futex
#define	OSA_Sched_Setaffinity     		sched_setaffinity
#define	OSA_Sched_Getaffinity     		sched_getaffinity
#define	OSA_Io_Setup              		io_setup
#define	OSA_Io_Destroy            		io_destroy
#define	OSA_Io_Getevents          		io_getevents
#define	OSA_Io_Submit             		io_submit
#define	OSA_Io_Cancel             		io_cancel
#define	OSA_Exit_Group            		exit_group
#define	OSA_Lookup_Dcookie        		lookup_dcookie
#define	OSA_Epoll_Create          		epoll_create
#define	OSA_Epoll_Ctl             		epoll_ctl
#define	OSA_Epoll_Wait            		epoll_wait
#define	OSA_Remap_File_Pages      		remap_file_pages
#define	OSA_Set_Tid_Address       		set_tid_address
#define	OSA_Restart_Syscall       		restart_syscall
#define	OSA_Fadvise64             		fadvise64
#define	OSA_Statfs64              		statfs64
#define	OSA_Fstatfs64             		fstatfs64
#define	OSA_Timer_Create          		timer_create
#define	OSA_Timer_Settime         		timer_settime
#define	OSA_Timer_Gettime         		timer_gettime
#define	OSA_Timer_Getoverrun      		timer_getoverrun
#define	OSA_Timer_Delete          		timer_delete
#define	OSA_Clock_Settime         		clock_settime
#define	OSA_Clock_Gettime         		clock_gettime
#define	OSA_Clock_Getres          		clock_getres
#define	OSA_Clock_Nanosleep       		clock_nanosleep
#define	OSA_Tgkill                		tgkill
#define	OSA_Utimes                		utimes
#define	OSA_Mbind                 		mbind
#define	OSA_Get_Mempolicy         		get_mempolicy
#define	OSA_Set_Mempolicy         		set_mempolicy
#define	OSA_Mq_Open               		mq_open
#define	OSA_Mq_Unlink             		mq_unlink
#define	OSA_Mq_Timedsend          		mq_timedsend
#define	OSA_Mq_Timedreceive       		mq_timedreceive
#define	OSA_Mq_Notify             		mq_notify
#define	OSA_Mq_Getsetattr         		mq_getsetattr
#define	OSA_Vserver               		vserver
#define	OSA_Waitid                		waitid
#define	OSA_Add_Key               		add_key
#define	OSA_Request_Key           		request_key
#define	OSA_Keyctl			      		keyctl

#define OSA_Opendir						opendir
#define OSA_Rewinddir					rewinddir
#define OSA_Closedir					closedir
#define OSA_Seekdir						seekdir
#define OSA_Telldir						telldir
#define OSA_Scandir						scandir
#define OSA_Dirfd 						dirfd


#define OSA_Fopen						fopen
#define OSA_Feof						feof
#define OSA_FClose						fclose
#define OSA_FFlush						fflush
#define OSA_FSeek						fseek
#define OSA_FTell						ftell
#define OSA_FRewind						rewind
#define OSA_FGetC						fgetc
#define OSA_FGetS						fgets
#define OSA_FPutC						fputc
#define OSA_FPutS						fputs
#define OSA_FWrite						fwrite
#define OSA_FCloseAll					fcloseall
#define OSA_Fread						fread
#define OSA_mkdir						mkdir
#define	OSA_DeleteTask					osa_thread_cancel
#define	OSA_AddTaskRes					osa_thread_attrres_register

#define OSA_WaitSem						osa_lock_wait
#define OSA_PostSem						osa_lock_post
#define OSA_CreateSerSem 				osa_lock_ssema_init
#define OSA_CreateMtxSem				osa_lock_mutex_init
#define OSA_CreateBinSem				osa_lock_bsema_init
#define OSA_DeleteSem					osa_lock_destroy

#define	OSA_GetNameOfTask				osa_thread_getname
#define OSA_GetTaskId					osa_thread_byname
#define OSA_SetCurTaskCpu				osa_thread_set_cpu
#define OSA_SetTaskPriority				osa_thread_set_priority
#define OSA_GetTaskPriority				osa_thread_get_priority
#define OSA_SetSelfPriority				osa_thread_set_selfpriority

#define OSA_GET_PID						osa_thread_getpid
#define OSA_LoadSymTab					xlibc_sym_load

#define OSA_ReadMsTicks					xlibc_read_msticks
#define OSA_ReadUsTicks					xlibc_read_usticks
#define OSA_DelayMsTask					xlibc_delay_ms
#define OSA_DelayUsTask					xlibc_delay_us
#define OSA_ResumeTask					osa_thread_resume
#define	OSA_GetThIdx					osa_thread_self
#define OSA_Abort						osa_abort

#define	OSA_CalcCRC32					xlibc_calccrc32
#define	OSA_TaskSleep					osa_thread_sleep

#define OSA_GetNameOfObject				osa_thread_find_osvname_byid

#define OSA_DBG_PrintMenu				rprint0n
#define OSA_DBG_GetIntegerInput 		cmd_io_read_int
#define OSA_DBG_GetDecimalInput 		cmd_io_read_dec
#define OSA_ReadKeyInputWithSize 		cmd_io_read_hex_withsize
#define OSA_DBG_GetDoubleInput 			cmd_io_read_double
#define OSA_DBG_GetFloatInput 			cmd_io_read_float
#define OSA_ReadCmdString				cmd_io_read_string
#define OSA_ReadNumber 					cmd_io_read_number
#define OSA_GetBinaryFileInput 			cmd_io_read_bin_file
#define OSA_DBG_PrintColor				rprint0n
#define	OSA_DBG_InitPrintMask			cmd_mask_init
#define OSA_DBG_Pause					cmd_io_pause

/*----------------------------------------------------------------------------+
| Message Queue  Related O/S Wrapper
+-----------------------------------------------------------------------------*/
#define OSA_CreateMsg					osa_queue_init
#define OSA_SendMsg						osa_queue_sendmsg
#define OSA_SendUrgentMsg				osa_queue_sendmsg_urgent
#define OSA_RecvMsg						osa_queue_recvmsg
#define OSA_DeleteMsg					osa_queue_destroy
#define OSA_FlushMsg					osa_queue_flushmsg
#define OSA_GetQueueCount				osa_queue_getcount
#define OSA_GetQueueRemainCount			osa_queue_getcount_remained

/*----------------------------------------------------------------------------+
| Event  Related O/S Wrapper
+-----------------------------------------------------------------------------*/
#define OSA_RecvEvent					osa_event_receive
#define OSA_SendEvent					osa_event_send
#define OSA_ClearEvent					osa_event_clear

#define OSA_RegisterEventNotification	osa_event_register
#define	OSA_UnregisterEventNotification	osa_event_unregister

/*----------------------------------------------------------------------------+
| Timer Control Functions Set
+-----------------------------------------------------------------------------*/
#define OSA_TIMER_InitModule			osa_timer_init
#define OSA_TIMER_SetEventTimer			osa_timer_setevent
#define OSA_TIMER_SetCallbackTimer		osa_timer_setcallback
#define OSA_TIMER_CancelTimer			osa_timer_cancel

#define OSA_TIME_GetElapsedMS 			xlibc_time_getelapsedms
#define OSA_TIME_ConvertSec2Time 		xlibc_time_convert_sec2time
#define OSA_TIME_ConvertTime2Sec 		xlibc_time_convert_time2sec
#define OSA_TIME_SetClock				xlibc_time_setclock
#define OSA_TIME_GetClock				xlibc_time_getclock

/*----------------------------------------------------------------------------+
| Terminal Control Functions Set
+-----------------------------------------------------------------------------*/
#define OSA_SetTermOneShot				xlibc_term_setoneshot
#define OSA_GetTermMode					xlibc_term_getmode
#define OSA_SetTermMode					xlibc_term_setmode
#define OSA_SetTermRawMode				xlibc_term_setrawmode


/*----------------------------------------------------------------------------+
| 아래 define은 지워 줘야함. IMCHANG
+-----------------------------------------------------------------------------*/
#define API_DEBUG_PrintMaskMenu			cmd_builtin_mask_interpreter_menu

extern void OSA_Init(void);

extern TID_TYPE	OSA_RegistAndCreateTask(
		   char		*name,
		   void	*	(*entry_point)(void *),
		   void		*arg,
		   void		*rsv,
		   size_t	stack_size,
		   int		prio,
		   char		group,
		   unsigned long	cpunum);

extern TID_TYPE 	OSA_CreateTask(
           char    *name,
           void    *arg,
           void    *rsv,                    /* must be 0 */
           char    group );


extern void OSA_SetExceptionRaised(int onoff);
extern int 	OSA_GetExceptionRaised(void);
extern int 	OSA_RemTaskRes(TID_TYPE tId);
extern int 	OSA_RemThidPool(pthread_t thread_id, int res_mode);


int		OSA_GetMemCheckFlag(TID_TYPE taskId);
UINT8	OSA_UpdateMemUsage(TID_TYPE taskId, int size);
void	OSA_UpdateWaitStatus(TID_TYPE taskId, int waitMs);
UINT32	OSA_GetTaskMemUse(TID_TYPE taskId);
SINT32	OSA_GetTaskMemPreference(TID_TYPE taskId, SINT32 size);
void 	OSA_SetBaudRate(speed_t speed);
void 	OSA_PrintStack(void);
void 	PrintStack(void);

/*----------------------------------------------------------------------------+
| IPC Control Functions Set
+-----------------------------------------------------------------------------*/
extern API_STATE_T	OSA_IPC_CreateProcess(const char *pPath, char *const pArgv[], SINT32 *pPid);
extern API_STATE_T	OSA_IPC_WaitProcess(SINT32 pid, SINT32 *status, SINT32 options);
extern API_STATE_T	OSA_IPC_KillProcess(SINT32 pid, SINT32 signal);
extern API_STATE_T	OSA_IPC_CreateKey(const CHAR *pPathName, SINT32 projId, SINT32 *pKey);
extern API_STATE_T	OSA_IPC_CreateMsg(SINT32 key, SINT32 msgFlag, SINT32 *pMsgQId);
extern API_STATE_T	OSA_IPC_SendMsg(SINT32 msgQId, const void *pMsgBuf, UINT32 msgSize, SINT32 msgFlag);
extern API_STATE_T	OSA_IPC_ReceiveMsg(SINT32 msgQId, void *pMsgBuf, UINT32 msgSize, SINT32 msgType, SINT32 msgFlag);
extern API_STATE_T	OSA_IPC_DeleteMsg(SINT32 msgQId);
extern API_STATE_T	OSA_IPC_InfoMsg(SINT32 msgQId, void *pMsgInfo);


/*----------------------------------------------------------------------------+
| Initializer of OS Adaptation Layer
+-----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
| Some utility functions for debugging
+----------------------------------------------------------------------------*/
extern UINT32	OSA_DBG_GetHexInput( const char * comment, UINT32 * pValue );
extern ULONG  	OSA_SuspendTask(unsigned int ms);

#ifdef PROFILE_TO_FILE
extern void     OSA_DbgPrntMsgQsToFile(int prof_num);
extern void 	Set_profile_to_file_number(int a);
extern int 		Get_profile_to_file_number(void);
#endif /* PROFILE_TO_FILE */

#ifdef __cplusplus
}
#endif

#endif	/* _OSA_H_	*/
