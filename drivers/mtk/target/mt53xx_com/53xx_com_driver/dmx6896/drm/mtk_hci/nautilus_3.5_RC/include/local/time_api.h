/** @addtogroup base base
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2004,2005,2006,2007,2008,2009,2010 Sony Corporation, All Rights Reserved.
 *
 */

#ifndef TIME_API_H
#define TIME_API_H

/** @file
 * Time API
 *
 * @require none
 * @author Norifumi Goto <n-goto@sm.sony.co.jp>
 */

NP_BEGIN_EXTERN_C

/**
 * Error
 */
#define ERR_TIME_OUTOFRANGE     (ERR_TIME_BASE|1)

/*
 * The UNIX time defined in POSIX.
 * Seconds since 1970/1/1 0:0:0 UTC, ignoring leap seconds.
 */
/* defined in basic_types_.h
 * typedef u_int32_t    time_unix_t;
 */

/**
 * @brief   Time represented in Gregorian calendar.
 */
typedef struct _time_date {
    /** year in A.D. (1970 - 2038)     */
    int32_t    year;
    /** month        (01:Jan - 12:Dec) */
    int32_t    mon;
    /** day of month (01 - 31)         */
    int32_t    mday;
    /** hour         (00 - 23)         */
    int32_t    hour;
    /** minute       (00 - 59)         */
    int32_t    min;
    /** second       (00 - 60)         */
    int32_t    sec;
    /** milli-second (000 - 999)       */
    int32_t    msec;
    /** offset between UTC and local standard time (in seconds) */
    int32_t    offset;
    /** 0:Standard Time, otherwise:Daylight Saving Time */
    int32_t    isdst;
} time_date_t;

typedef u_int32_t time_epocmin_t;

/*---------------------------------------------------------------------------*
 * API
 *---------------------------------------------------------------------------*/

/**
 * @brief Gets the current UNIX time
 *
 * @param none
 * @return the localhost's current UNIX time
 */
extern time_unix_t
npi_time_GetTime(void);

/**
 * @brief Gets the difference between UTC and local standard time (in seconds)
 *
 * @param none
 * @return offset in seconds
 */
extern int32_t
npi_time_GetLocalTimeOffset(void);

/**
 * @brief Gets seed value using some time function
 *
 * @param [out] out_value       the buffer where the seed will be stored
 *
 * @return ::ERR_OK                     <br>Indicating Success
 */
extern retcode_t
npi_time_GetSeedValue(u_int32_t* out_value);

/**
 * @brief Converts time_date_t to time_unix_t.
 *
 * @param [in]  in_date         Source formatted time
 * @param [out] out_time        the buffer for calendar time
 *
 * @return ::ERR_OK                     <br>Indicating Success
 * @return ::ERR_TIME_OUTOFRANGE        <br>If the time is out of range. 
 * @return ::ERR_ILLEGALARGS            <br>The input arguments are incorrect. Arguments cannot be NULL. 
 */
extern retcode_t
npi_time_Date2Unix(
    const time_date_t*  in_date,
    time_unix_t*        out_time);

/**
 * @brief Converts time_unix_t to time_date_t.
 *
 * @param [in]  in_time         Source calendar time
 * @param [in]  in_offset       the difference between UTC and local standard time (in seconds)
 * @param [in]  in_isdst        Standard Time (0) or Daylight Saving Time (otherwise)
 * @param [out] out_date        the buffer for the formatted time.
 *
 * @return ::ERR_OK                     <br>Indicating Success
 * @return ::ERR_TIME_OUTOFRANGE        <br>If the time is out of range.
 * @return ::ERR_ILLEGALARGS            <br>The input arguments are incorrect. Arguments cannot be NULL.
 */
extern retcode_t
npi_time_Unix2Date(
    const time_unix_t* in_time,
    int32_t            in_offset,
    int32_t            in_isdst,
    time_date_t*       out_date);

/**
 * @brief Converts time_epocmin_t to time_date_t.
 *
 * @param [in]  in_time         Source calendar time(counted by minutes)
 * @param [out] out_date        the buffer for the formatted time.
 *
 * @return ::ERR_OK                     <br>Indicating Success.
 */
extern retcode_t
npi_time_EPOCMin2Date(
    const time_epocmin_t*   in_time,
    time_date_t*            out_date);

/**
 * convert time_date_t to time_epocmin_t
 *
 * @param [in]  in_date         the buffer for the formatted time.
 * @param [out] out_time        Source calendar time(counted by minutes)
 * @return ::ERR_OK               
 */
extern retcode_t
npi_time_Date2EPOCMin(
    const time_date_t*  in_date,
    time_epocmin_t*     out_time);

/**
 * compare time
 *
 * @param [in]  in_t1   time to be compared with in_t2
 * @param [in]  in_t2   time to be compared with in_t1
 * @return -1               
 * @return 0  
 * @return 1
 */
extern int32_t
npi_time_Cmp(
    const time_date_t* in_t1,
    const time_date_t* in_t2);

NP_END_EXTERN_C

#endif /* TIME_API_H */
/** @} */
