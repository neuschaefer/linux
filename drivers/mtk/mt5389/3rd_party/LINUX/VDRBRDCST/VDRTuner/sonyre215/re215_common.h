/*------------------------------------------------------------------------------

 Copyright 2009-2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/12/06
 Revision: 1.0.2.0

------------------------------------------------------------------------------*/

#ifndef RE215_COMMON_H
#define RE215_COMMON_H

/* Type definitions. */
/* <PORTING> Please comment out if conflicted */
#ifdef __linux__
//#include <stdint.h> /* int_x_t, uint_x_t are defined in this header */
#include <linux/types.h>
#else
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed long int32_t;
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif

/* <PORTING> Sleep function define (Please modify as you like) */
/*
#ifdef _WINDOWS
#include <windows.h>
#define RE215_SLEEP(n) Sleep(n)
#endif
*/

/* For MTK */
#include "x_os.h"

//extern void x_thread_delay (uint32_t ui4_delay);
#define RE215_SLEEP(n) x_thread_delay(n)

#ifndef RE215_SLEEP
#error RE215_SLEEP(n) is not defined.
#endif

/* <PORTING> Floating point enable */
/* #define RE215_FP_ENABLE */

/* <PORTING> Trace function enable */
#define RE215_TRACE_ENABLE
/* <PORTING> Enable if I2C related function trace is necessary */
/* #define RE215_TRACE_I2C_ENABLE */

/* <PORTING> Macro to specify unused argument and suppress compiler warnings. */
#define RE215_ARG_UNUSED(arg) ((arg) = (arg))

/*------------------------------------------------------------------------------
  Enums
------------------------------------------------------------------------------*/
/* Return codes */
typedef enum {
	RE215_RESULT_OK,
	RE215_RESULT_ERROR_ARG,      /* Invalid argument (maybe software bug) */
	RE215_RESULT_ERROR_I2C,      /* I2C communication error */
	RE215_RESULT_ERROR_SW_STATE, /* Invalid software state */
	RE215_RESULT_ERROR_HW_STATE, /* Invalid hardware state */
	RE215_RESULT_ERROR_TIMEOUT,  /* Timeout occured */
	RE215_RESULT_ERROR_UNLOCK,   /* Failed to lock */
	RE215_RESULT_ERROR_RANGE,    /* Out of range */
	RE215_RESULT_ERROR_NOSUPPORT,/* Not supported for current device */
	RE215_RESULT_ERROR_CANCEL,   /* The operation is canceled */
	RE215_RESULT_ERROR_OTHER
} re215_result_t;

/*------------------------------------------------------------------------------
  Common functions
------------------------------------------------------------------------------*/
int32_t re215_Convert2SComplement(uint32_t value, uint32_t bitlen);
uint32_t re215_BitSplitFromByteArray(uint8_t *pArray, uint32_t startBit, uint32_t bitNum);

/*------------------------------------------------------------------------------
  Trace
------------------------------------------------------------------------------*/
#ifdef RE215_TRACE_ENABLE
/* <PORTING> This is only a sample of trace macro. Please modify is necessary. */
/* In sample application, these functions are implemented in main.c */
void re215_trace_log_enter(const char* funcname, const char* filename, unsigned int linenum);
void re215_trace_log_return(re215_result_t result, const char* filename, unsigned int linenum);
#define RE215_TRACE_ENTER(func) re215_trace_log_enter((func), __FILE__, __LINE__)
#define RE215_TRACE_RETURN(result) do{re215_trace_log_return((result), __FILE__, __LINE__); return (result);}while(0)
#else /* RE215_TRACE_ENABLE */
#define RE215_TRACE_ENTER(func)
#define RE215_TRACE_RETURN(result) return(result)
#define RE215_TRACE_I2C_ENTER(func)
#define RE215_TRACE_I2C_RETURN(result) return(result)
#endif /* RE215_TRACE_ENABLE */


#ifdef RE215_TRACE_I2C_ENABLE
/* <PORTING> This is only a sample of trace macro. Please modify is necessary. */
void re215_trace_i2c_log_enter(const char* funcname, const char* filename, unsigned int linenum);
void re215_trace_i2c_log_return(re215_result_t result, const char* filename, unsigned int linenum);
#define RE215_TRACE_I2C_ENTER(func) re215_trace_i2c_log_enter((func), __FILE__, __LINE__)
#define RE215_TRACE_I2C_RETURN(result) do{re215_trace_i2c_log_return((result), __FILE__, __LINE__); return (result);}while(0)
#else /* RE215_TRACE_I2C_ENABLE */
#define RE215_TRACE_I2C_ENTER(func)
#define RE215_TRACE_I2C_RETURN(result) return(result)
#endif /* RE215_TRACE_I2C_ENABLE */

#endif /* RE215_COMMON_H */
