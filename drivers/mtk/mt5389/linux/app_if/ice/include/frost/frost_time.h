/**
 * Copyright 2002-4 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Cabot portable interface to time functionality.
 *
 * @note All time values are in units of milliseconds.  Time
 * values may wrap at any point beyond 25 hours (90,000,000 ms).
 */

#ifndef CABOT_FROST_FROST_TIME_H_INCLUDED
#define CABOT_FROST_FROST_TIME_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Obtain the current time in milliseconds.
 *
 * @return The current time in milliseconds.
 *
 * @note The value returned must be unique within any 25
 * hour period.  It may wrap at any point beyond 25 hours.
 */
frost_uint32 FROST_Time(void);

/**
 * @brief Add two times taking account of wrapping.
 *
 * @return The sum of the two times, taking account of wrapping 
 * if required.
 */
frost_uint32 FROST_TimeAdd(frost_uint32 timeB, frost_uint32 timeA);

/**
 * @brief Subtract timeA from timeB taking account of wrapping.
 *
 * Calculate the elapsed time between timeA and timeB, taking account
 * of wrapping if required. It is assumed that timeA is "before" timeB.
 * The behaviour of this function is best illustrated by examples:
 *                                                                      <pre>
 *                             timeA     timeB                          <br>
 * Example1                      |         |                            <br>
 *                               V         V                            <br>
 * +-------------------------------------------------+                  <br>
 * |         |         |         |         |         |                  <br>
 * 0       2000      4000      6000      8000      10000  seconds       <br>
 *                                                                      </pre>
 * Example1 is the trivial case where FROST_TimeSubtract(timeB, timeA)
 * returns 2000 seconds (i.e 2,000,000 mS).
 *                                                                      <pre>
 *         timeB                                   timeA                <br>
 * Example2  |                                       |                  <br>
 *           V                                       V                  <br>
 * +-------------------------------------------------+                  <br>
 * |         |         |         |         |         |                  <br>
 * 0       2000      4000      6000      8000      10000  seconds       </pre>
 *
 * In Example2, the period between timeA and timeB has wrapped past
 * the 10000 second wrap time.  FROST_TimeSubtract(timeB, timeA) in this case
 * also returns 2000 seconds (i.e 2,000,000 mS).
 *                                                                      <pre>
 *                   timeB     timeA                                    <br>
 * Example3            |         |                                      <br>
 *                     V         V                                      <br>
 * +-------------------------------------------------+                  <br>
 * |         |         |         |         |         |                  <br>
 * 0       2000      4000      6000      8000      10000  seconds       </pre>
 * 
 * In Example3 also, the period between timeA and timeB has wrapped past
 * the 10000 second wrap time.  This time FROST_TimeSubtract(timeB, timeA) returns
 * 8000 seconds (i.e. 8,000,000 mS).  This is (wrap_time - timeA) + timeB.
 *
 * @return The elapsed time between timeA and timeB, taking account
 * of wrapping if required.
 */
frost_uint32 FROST_TimeSubtract(frost_uint32 timeB, frost_uint32 timeA);

/**
 * @brief Cause the current task to sleep for the specified number of
 * system milliseconds.
 *
 * @param msecs The number of milliseconds to sleep for.
 */
void FROST_Sleep(frost_uint32 msecs);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_FROST_FROST_TIME_H_INCLUDED*/

