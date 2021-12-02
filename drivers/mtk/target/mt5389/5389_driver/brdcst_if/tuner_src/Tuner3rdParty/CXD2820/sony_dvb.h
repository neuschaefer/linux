/*------------------------------------------------------------------------------

<dev:header>
    Copyright(c) 2009 Sony Corporation.

    $Revision: #1 $
    $Author: dtvbm11 $

</dev:header>

------------------------------------------------------------------------------*/
/**

 @file    sony_dvb.h

          This file provides low level type definitions and trace definitions
          used across the driver.
*/
/*----------------------------------------------------------------------------*/

#ifndef SONY_DVB_H
#define SONY_DVB_H

/* Type definitions. */
/* PORTING. Please comment out if conflicted */
typedef unsigned char uint8;      /**< 8-bit unsigned int. */
typedef unsigned short uint16;    /**< 16-bit unsigned int. */   
typedef unsigned long uint32;     /**< 32-bit unsigned int. */
typedef signed char int8;         /**< 8-bit signed int. */
typedef signed short int16;       /**< 16-bit signed int. */
typedef signed long int32;        /**< 32-bit signed int. */

#ifndef NULL
#	define NULL ((void*)0)          /**< NULL definition. */
#endif

/* PORTING. Sleep function define (Please modify as you like) */
#ifdef _WINDOWS
#   include <windows.h>
#   define SONY_DVB_SLEEP(n) Sleep(n)
#else
#   define SONY_DVB_SLEEP(n)        /**< PORTING. OS abstraction sleep(ms). */
#endif

#define SONY_DVB_TRACE_ENABLE       /**< PORTING. Trace function enable. */

#if 1
#define SONY_DVB_TRACE_I2C_ENABLE   /**< PORTING. Enable if I2C related function trace is necessary. */
#endif

/**
 @brief Gets the array size of statically defined array.
*/
#define SONY_DVB_ARRAY_SIZE(x)  (sizeof((x))/sizeof((x)[0]))

/*------------------------------------------------------------------------------
  Enums
------------------------------------------------------------------------------*/

/**
 @brief Result codes from demodulator operations.
*/
typedef enum {
    SONY_DVB_OK,                /**< Successful. */
    SONY_DVB_ERROR_ARG,         /**< Invalid argument (maybe software bug). */
    SONY_DVB_ERROR_I2C,         /**< I2C communication error. */
    SONY_DVB_ERROR_SW_STATE,    /**< Invalid software state. */
    SONY_DVB_ERROR_HW_STATE,    /**< Invalid hardware state. */
    SONY_DVB_ERROR_TIMEOUT,     /**< Timeout occured. */
    SONY_DVB_ERROR_UNLOCK,      /**< Failed to lock. */
    SONY_DVB_ERROR_RANGE,       /**< Out of range. */
    SONY_DVB_ERROR_NOSUPPORT,   /**< Not supported for current device. */
    SONY_DVB_ERROR_CANCEL,      /**< The operation is canceled. */
    SONY_DVB_ERROR_OTHER        /**< The operation failed. Enable tracing to determine details of the error. */
} sony_dvb_result_t;

/**
 @brief System(DVB-T/C/T2) 
*/
typedef enum {
    SONY_DVB_SYSTEM_UNKNOWN,    /**< Unknown. */
    SONY_DVB_SYSTEM_DVBC,       /**< DVB-C. */
    SONY_DVB_SYSTEM_DVBT,       /**< DVB-T. */
    SONY_DVB_SYSTEM_DVBT2       /**< DVB-T2. */
} sony_dvb_system_t;

/*------------------------------------------------------------------------------
  Structs
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  Common functions
------------------------------------------------------------------------------*/

/**
 @brief Convert a value to a 2s complement representation.

 @param value The unsigned value to convert.
 @param bitlen The length of the field.

 @return Converted value.
*/
//int32 dvb_Convert2SComplement (uint32 value, uint32 bitlen);

/*------------------------------------------------------------------------------
  Trace
------------------------------------------------------------------------------*/

#ifdef SONY_DVB_TRACE_ENABLE

/**
 @brief Logging trace called on entry into function.

 @param funcname The name of the function being entered.
 @param filename The name of the file that the function resides in.
                 C compiler defined __FILE__.
 @param linenum The line number that this was invoked from.
                C compiler defined __LINE__.
*/
void dvb_trace_log_enter (const char *funcname, const char *filename, unsigned int linenum);

void dvb_trace_i2c_log_enter (const char *funcname, const char *filename, unsigned int linenum);

/**
 @brief Logging trace called on exit of function.

 @param result The return result of the function.
 @param filename The name of the file that the function resides in.
                C compiler defined __FILE__.
 @param linenum The line number that this was invoked from.
                C compiler defined __LINE__.
*/
void dvb_trace_log_return (sony_dvb_result_t result, const char *filename, unsigned int linenum);

void dvb_trace_i2c_log_return (sony_dvb_result_t result, const char *filename, unsigned int linenum);


#   define SONY_DVB_TRACE_ENTER(func) dvb_trace_log_enter((func), __FILE__, __LINE__)
#   define SONY_DVB_I2C_TRACE_ENTER(func) dvb_trace_i2c_log_enter((func), __FILE__, __LINE__)

/**
 Disables MS compiler warning (__pragma(warning(disable:4127))
 with do { } while (0);
*/
#define SONY_DVB_MACRO_MULTILINE_BEGIN  do {
#   if ((defined _MSC_VER) && (_MSC_VER >= 1300))
    #define SONY_DVB_MACRO_MULTILINE_END \
	    __pragma(warning(push)) \
	    __pragma(warning(disable:4127)) \
	    } while(0) \
	    __pragma(warning(pop))
#else    
#   define SONY_DVB_MACRO_MULTILINE_END } while(0) 
#endif

#   define SONY_DVB_TRACE_RETURN(result) \
    SONY_DVB_MACRO_MULTILINE_BEGIN \
        dvb_trace_log_return((result), __FILE__, __LINE__); \
        return (result); \
    SONY_DVB_MACRO_MULTILINE_END

#   define SONY_DVB_I2C_TRACE_RETURN(result) \
    SONY_DVB_MACRO_MULTILINE_BEGIN \
        dvb_trace_i2c_log_return((result), __FILE__, __LINE__); \
        return (result); \
    SONY_DVB_MACRO_MULTILINE_END


#   ifdef SONY_DVB_TRACE_I2C_ENABLE
/* I2C related functions trace will be disabled because log output is too much... */
#       define SONY_DVB_TRACE_I2C_ENTER(func) SONY_DVB_I2C_TRACE_ENTER(func)
#       define SONY_DVB_TRACE_I2C_RETURN(result) SONY_DVB_I2C_TRACE_RETURN(result)
#   else
/* SONY_DVB_TRACE_I2C_ENABLE */
#       define SONY_DVB_TRACE_I2C_ENTER(func)
#       define SONY_DVB_TRACE_I2C_RETURN(result) return(result)
#   endif
/* SONY_DVB_TRACE_I2C_ENABLE */

#else
/* SONY_DVB_TRACE_ENABLE */

#   define SONY_DVB_TRACE_ENTER(func)
#   define SONY_DVB_TRACE_RETURN(result) return(result)
#   define SONY_DVB_TRACE_I2C_ENTER(func)
#   define SONY_DVB_TRACE_I2C_RETURN(result) return(result)

#endif
/* SONY_DVB_TRACE_ENABLE */


/*------------------------------------------------------------------------------
  Multi-threaded defines
 ------------------------------------------------------------------------------*/
/**
 @brief Defines for basic atomic operations for cancellation. 
        PORTING.
 */
typedef struct sony_dvb_atomic_t {
    /**
     @brief Underlying counter.
    */
    volatile int counter ;  
} sony_dvb_atomic_t ;
#define sony_dvb_atomic_set(a,i) ((a)->counter = i)                 /**< Set counter atomically. */
#define sony_dvb_atomic_read(a) ((a)->counter)                      /**< Get counter atomically. */

#endif /* SONY_DVB_H */
