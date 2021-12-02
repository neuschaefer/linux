#ifndef _PD_ISDBT_GLUE_H_
#define _PD_ISDBT_GLUE_H_

#include "u_handle.h"
#include "x_typedef.h"

typedef struct _ISDBT_PD_CTX_T
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
} ISDBT_PD_CTX_T, *PISDBT_PD_CTX_T;

/* temporary type definition */
typedef enum
{
    ISDBT_LAYER_UNKNOWN = 0,
    ISDBT_LAYER_A,
    ISDBT_LAYER_B,
    ISDBT_LAYER_C,
} TUNER_ISDBT_LAYER_T;

#endif

