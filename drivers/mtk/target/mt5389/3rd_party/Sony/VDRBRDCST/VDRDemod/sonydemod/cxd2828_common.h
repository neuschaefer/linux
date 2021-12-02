/*------------------------------------------------------------------------------

 Copyright 2009, 2010, 2011 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2011/01/28
 Revision: 1.0.3.0

------------------------------------------------------------------------------*/

#ifndef CXD2828_COMMON_H
#define CXD2828_COMMON_H

/* Type definitions. */
/* <PORTING> Please comment out if conflicted */
/* typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed long int32_t; */

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif

/* <PORTING> Sleep function define (Please modify as you like) */
#ifdef _WINDOWS
#include <windows.h>
#define CXD2828_SLEEP(n) Sleep(n)
#endif

#include "x_os.h"
#include "x_typedef.h"
#define CXD2828_SLEEP(n) x_thread_delay(n)

#ifndef CXD2828_SLEEP
#error CXD2828_SLEEP(n) is not defined.
#endif

/* <PORTING> Floating point enable */
//#define CXD2828_FP_ENABLE

/* <PORTING> Trace function enable */
#define CXD2828_TRACE_ENABLE
/* <PORTING> Enable if I2C related function trace is necessary */
/* #define CXD2828_TRACE_I2C_ENABLE */

/* <PORTING> Macro to specify unused argument and suppress compiler warnings. */
#define CXD2828_ARG_UNUSED(arg) ((arg) = (arg))

/*------------------------------------------------------------------------------
  Enums
------------------------------------------------------------------------------*/
/* Return codes */
typedef enum {
    CXD2828_RESULT_OK,
    CXD2828_RESULT_ERROR_ARG,      /* Invalid argument (maybe software bug) */
    CXD2828_RESULT_ERROR_I2C,      /* I2C communication error */
    CXD2828_RESULT_ERROR_SW_STATE, /* Invalid software state */
    CXD2828_RESULT_ERROR_HW_STATE, /* Invalid hardware state */
    CXD2828_RESULT_ERROR_TIMEOUT,  /* Timeout occured */
    CXD2828_RESULT_ERROR_UNLOCK,   /* Failed to lock */
    CXD2828_RESULT_ERROR_RANGE,    /* Out of range */
    CXD2828_RESULT_ERROR_NOSUPPORT,/* Not supported for current device */
    CXD2828_RESULT_ERROR_CANCEL,   /* The operation is canceled */
    CXD2828_RESULT_ERROR_OTHER
} cxd2828_result_t;

/*------------------------------------------------------------------------------
  Common functions
------------------------------------------------------------------------------*/
INT32  cxd2828_Convert2SComplement(UINT32 value, UINT32 bitlen);
UINT32  cxd2828_BitSplitFromByteArray(UINT8 *pArray, UINT32 startBit, UINT32 bitNum);

/*------------------------------------------------------------------------------
  Trace
------------------------------------------------------------------------------*/
#ifdef CXD2828_TRACE_ENABLE
/* <PORTING> This is only a sample of trace macro. Please modify is necessary. */
/* In sample application, these functions are implemented in main.c */
void cxd2828_trace_log_enter(const char* funcname, const char* filename, unsigned int linenum);
void cxd2828_trace_log_return(cxd2828_result_t result, const char* filename, unsigned int linenum);
#define CXD2828_TRACE_ENTER(func) cxd2828_trace_log_enter((func), __FILE__, __LINE__)
#define CXD2828_TRACE_RETURN(result) do{cxd2828_trace_log_return((result), __FILE__, __LINE__); return (result);}while(0)
#else /* CXD2828_TRACE_ENABLE */
#define CXD2828_TRACE_ENTER(func)
#define CXD2828_TRACE_RETURN(result) return(result)
#define CXD2828_TRACE_I2C_ENTER(func)
#define CXD2828_TRACE_I2C_RETURN(result) return(result)
#endif /* CXD2828_TRACE_ENABLE */


#ifdef CXD2828_TRACE_I2C_ENABLE
/* <PORTING> This is only a sample of trace macro. Please modify is necessary. */
void cxd2828_trace_i2c_log_enter(const char* funcname, const char* filename, unsigned int linenum);
void cxd2828_trace_i2c_log_return(cxd2828_result_t result, const char* filename, unsigned int linenum);
#define CXD2828_TRACE_I2C_ENTER(func) cxd2828_trace_i2c_log_enter((func), __FILE__, __LINE__)
#define CXD2828_TRACE_I2C_RETURN(result) do{cxd2828_trace_i2c_log_return((result), __FILE__, __LINE__); return (result);}while(0)
#else /* CXD2828_TRACE_I2C_ENABLE */
#define CXD2828_TRACE_I2C_ENTER(func)
#define CXD2828_TRACE_I2C_RETURN(result) return(result)
#endif /* CXD2828_TRACE_I2C_ENABLE */

#endif /* CXD2828_COMMON_H */
