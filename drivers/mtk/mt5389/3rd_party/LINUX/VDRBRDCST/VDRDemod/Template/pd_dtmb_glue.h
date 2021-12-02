#ifndef _PD_DTMB_GLUE_H_
#define _PD_DTMB_GLUE_H_

#include "u_handle.h"
#include "x_typedef.h"

typedef struct _DTMB_PD_CTX_T
{
//    void*           pDemodCtx;
    x_break_fct     isBreak;
    HANDLE_T        hHalLock;
    HANDLE_T        t_escape_mon_suspend;
    HANDLE_T        t_monitor_thread;
    BOOL            fgBreakMonitor;
    BOOL            fgPIMoniStatus; //1: doing monitor 0:not doing monitor
    BOOL            fgMonitorThreadRunning;
    INT32           Frequency;
} DTMB_PD_CTX_T, *PDTMB_PD_CTX_T;

#endif

