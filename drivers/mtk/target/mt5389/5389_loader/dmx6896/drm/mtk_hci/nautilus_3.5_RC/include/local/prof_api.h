/** @addtogroup base base
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2004,2005,2006,2008,2009,2010 Sony Corporation, All Rights Reserved.
 */

#ifndef PROF_API_H
#define PROF_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================*/
/** @file
 * @brief Profiling(Time Performance measurement) Library
 *
 * dependency: basic_types.h
 *
 * @author Norifumi Goto <n-goto@sm.sony.co.jp>
 * @date   20 Jan, 2005 by n-goto
 */
/*===========================================================================*/

#if defined(ENABLE_PROFILING_TIME)

extern void prof_l_timeinit(void);
extern void prof_l_timefin(void);
extern void prof_l_timestart(const char *in_title);
extern void prof_l_timestop(void);
extern void prof_l_timediff(void);
extern void prof_l_timeprint(void);
extern void prof_l_timegetsec(u_int32_t *out_sec);
extern void prof_l_timegetusec(u_int32_t *out_usec);
#define PROF_L_TIMEINIT() prof_l_timeinit()
#define PROF_L_TIMEFIN() prof_l_timefin()
#define PROF_L_TIMESTART(t) prof_l_timestart(t)
#define PROF_L_TIMESTOP() prof_l_timestop()
#define PROF_L_TIMEDIFF() prof_l_timediff()
#define PROF_L_TIMEPRINT() prof_l_timeprint()
#define PROF_L_TIMEGETSEC(x) prof_l_timegetsec(&x)
#define PROF_L_TIMEGETUSEC(x) prof_l_timegetusec(&x)

#endif /* ENABLE_PROFILING_TIME */

#if defined(ENABLE_PROFILING_SPACE)
#define PROF_L_INDEX_MAX 2
typedef struct {
    int32_t size;
    int32_t count;
    int32_t esize; /* estimated size */
} prof_l_memstat_t;
typedef struct {
    const char *title;
    int32_t sz_stack;
    prof_l_memstat_t sz_max[PROF_L_INDEX_MAX];
    prof_l_memstat_t sz_min[PROF_L_INDEX_MAX];
    prof_l_memstat_t bk_max[PROF_L_INDEX_MAX];
    prof_l_memstat_t bk_min[PROF_L_INDEX_MAX];
} prof_l_context_t;
extern retcode_t prof_l_SpaceStart(prof_l_context_t *io_context);
extern retcode_t prof_l_SpaceStop(prof_l_context_t *io_context);
extern retcode_t prof_l_SpacePrint(prof_l_context_t *io_context);
extern retcode_t prof_l_SpaceStack(void);
extern retcode_t prof_l_SpaceAlloc(int32_t idx, int32_t in_sz);
extern retcode_t prof_l_SpaceFree(int32_t idx, int32_t in_sz);
extern void prof_l_SpaceCurrentStatus(int32_t idx);
#define PROF_L_SPACEINIT(c) prof_l_context_t c
#define PROF_L_SPACESTART(c,t) do{(c).title=(t);prof_l_SpaceStart(&c);}while(0)

#endif

#if defined(ENABLE_PROFILING_TIME)
#define PROF_INIT() PROF_L_TIMEINIT()
#define PROF_TIMEINIT() PROF_L_TIMEINIT()
#define PROF_TIMESTART(t) PROF_L_TIMESTART(t)
#define PROF_TIMESTOP() PROF_L_TIMESTOP()
#define PROF_TIMEPRINT() PROF_L_TIMEPRINT()
#define PROF_TIMEGETSEC(x) PROF_L_TIMEGETSEC(x)
#define PROF_TIMEGETUSEC(x) PROF_L_TIMEGETUSEC(x)
#define PROF_TIMEFIN() PROF_L_TIMEFIN()
#define PROF_SPACEALLOC(idx,sz)
#define PROF_SPACEFREE(idx,sz)
#define PROF_SPACEPRINT()
#define PROF_SPACESTACK()
#define PROF_SPACESTATUS(idx)
#define PROF_START(t) PROF_TIMESTART(t)
#define PROF_STOP() PROF_TIMESTOP()
#define PROF_PRINT() PROF_TIMEPRINT()
#define PROF_FIN() PROF_L_TIMEFIN()
#elif defined(ENABLE_PROFILING_SPACE)
#define PROF_INIT() PROF_L_SPACEINIT(prof_l_context)
#define PROF_TIMEINIT(t)
#define PROF_TIMESTART(t)
#define PROF_TIMESTOP()
#define PROF_TIMEPRINT()
#define PROF_TIMEGETSEC(x)
#define PROF_TIMEGETUSEC(x)
#define PROF_TIMEFIN()
#define PROF_SPACEALLOC(idx,sz) prof_l_SpaceAlloc(idx,sz)
#define PROF_SPACEFREE(idx,sz) prof_l_SpaceFree(idx,sz)
#define PROF_SPACEPRINT() prof_l_SpacePrint(&prof_l_context)
#define PROF_SPACESTACK() prof_l_SpaceStack()
#define PROF_SPACESTATUS(idx) prof_l_SpaceCurrentStatus(idx)
#define PROF_START(t) PROF_L_SPACESTART(prof_l_context,t)
#define PROF_STOP() prof_l_SpaceStop(&prof_l_context)
#define PROF_PRINT() PROF_SPACEPRINT()
#define PROF_FIN()
#else
/**
 * @brief Clears the profiling data and label.
 *
 * @return none
 */
#define PROF_INIT() 
/**
 * @brief Retrieves the current value of the high-resolution performance counter to get the start time and set the label.
 *
 * @param[in] t label used in reporting
 * 
 * @return none
 */
#define PROF_TIMESTART(t)
/**
 * @brief  Retrieves the current value of the high-resolution performance counter and get the end time.
 *
 * @return none
 */
#define PROF_TIMESTOP()
 /**
 * @brief  Retrieves the time difference and logs the elapsed time
 *
 * @return none
 */
#define PROF_TIMEINIT()
#define PROF_TIMEPRINT()
#define PROF_TIMEGETSEC(x)
#define PROF_TIMEGETUSEC(x)
#define PROF_TIMEFIN()
#define PROF_SPACEALLOC(idx,sz)
#define PROF_SPACEFREE(idx,sz)
#define PROF_SPACEPRINT()
#define PROF_SPACESTACK()
#define PROF_SPACESTATUS(idx)
 /**
 * @brief Starts profiling (space or time)
 *
 * @param[in] t label for the profiling data
 *
 * @return none
 */
#define PROF_START(t)
/**
 * @brief Stops profiling 
 *
 * @return none
 */
#define PROF_STOP()
/**
 * @brief  Logs profiling data
 *
 * @return none
 */
#define PROF_PRINT()
/**
 * @brief  Finishes profiling.  
 *
 * @return none
 */
#define PROF_FIN()
#endif

#ifdef __cplusplus
}
#endif

#endif /* PROF_API_H */
/** @} */
