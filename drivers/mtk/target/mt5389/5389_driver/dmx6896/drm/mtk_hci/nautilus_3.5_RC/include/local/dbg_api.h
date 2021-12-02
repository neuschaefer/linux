/** @addtogroup base base
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2004,2005,2006,2007,2008,2009,2010 Sony Corporation, All Rights Reserved.
 */

#ifndef DBG_API_H
#define DBG_API_H

NP_BEGIN_EXTERN_C

/*===========================================================================*/
/** @file
 * @brief Debug Support Definition and API
 *
 * dependency: base
 *
 * @author Yoshizumi Tanaka <y-tanaka@sm.sony.co.jp>
 * @date   29 Nov, 2004 by y-tanaka
 */
/*===========================================================================*/
/*---------------------------------------------------------------------------*
 * type definition
 *---------------------------------------------------------------------------*/

#if defined(NS_DEBUG)

#include <assert.h>
#include "log_api.h"
/* A max. logging level in the system - if we use this value, we will
 * print a message every time. Should be set to INT_MAX, but I am not
 * sure if Nautilus supports limits.h, so it is safer to set it to a
 * number. */
#define L_LOG_ALWAYS (32767)

/*---------------------------------------------------------------------------*/
/**
 * @brief macros for debugging messages
 *
 * @verbatim
 * usage: ({n} is message level)
 *   MSG{n}((format, args...)); // used when displaying message
 *   MSGI{n}((format, args...)); // used when displaying message with indent
 *   DUMP{n}(len, buf); // used at hexadecimal dump
 *   PRINT{n}(len, buf); // used at character dump 
 * message level:
 *   0: display information always (used only for sample code)
 *   1: display error information
 *   2: display serious debug information
 *   3: display verbose debug information
 * The format and args follow the same manner as printf about MSG and MSGI.
 * Note: format and args should be surrounded by duplicated `('s and ')'s.
 * @endverbatim
 */
/*---------------------------------------------------------------------------*/

#define MSG0(x) LOG_REQ(dbg_l_Msg, L_LOG_ALWAYS, x)
#define MSG1(x) LOG_REQ(dbg_l_Msg, LOG_ERROR, x)
#define MSG2(x) LOG_REQ(dbg_l_Msg, LOG_WARN, x)
#define MSG3(x) LOG_REQ(dbg_l_Msg, LOG_INFO, x)

#define MSGI0(x) LOG_REQ(dbg_l_MsgWithIndent, L_LOG_ALWAYS, x)
#define MSGI1(x) LOG_REQ(dbg_l_MsgWithIndent, LOG_ERROR, x)
#define MSGI2(x) LOG_REQ(dbg_l_MsgWithIndent, LOG_WARN, x)
#define MSGI3(x) LOG_REQ(dbg_l_MsgWithIndent, LOG_INFO, x)

#define PRINT0(x) LOG_REQ(dbg_l_Print, L_LOG_ALWAYS, x)
#define PRINT1(x) LOG_REQ(dbg_l_Print, LOG_ERROR, x)
#define PRINT2(x) LOG_REQ(dbg_l_Print, LOG_WARN, x)
#define PRINT3(x) LOG_REQ(dbg_l_Print, LOG_INFO, x)

#define DUMP0(x) LOG_REQ(dbg_l_Dump, L_LOG_ALWAYS, x)
#define DUMP1(x) LOG_REQ(dbg_l_Dump, LOG_ERROR, x)
#define DUMP2(x) LOG_REQ(dbg_l_Dump, LOG_WARN, x)
#define DUMP3(x) LOG_REQ(dbg_l_Dump, LOG_INFO, x)

/*---------------------------------------------------------------------------*/
/**
 * @brief macros for debugging messages at function I/O
 *
 * @verbatim
 * usage:
 *   MSGENT(f); // used when announcing the bigging of functions
 *   MSGEXT(f); // used when announcing the end of functions
 * Output function name and increment/decrement indent of message.
 * NOTE: If f is NULL, entring/exiting messages are not displayed, but
 *       an indent is changed.
 * @endverbatim
 */
/*---------------------------------------------------------------------------*/
#if defined(ENABLE_PROFILING_TIME)
#define MSGENT(x)
#define MSGEXT(x)
#define MSGEXT_V(x)
#elif defined(ENABLE_PROFILING_SPACE)
#include "prof_api.h"
#define MSGENT(x) PROF_SPACESTACK()
#define MSGEXT(x)
#define MSGEXT_V(x)
#else
#define MSGENT(x) LOG_REQ(dbg_l_MsgEnter, LOG_DEBUG, x)
#define MSGEXT(x) LOG_REQ(dbg_l_MsgExit, LOG_DEBUG, (x,TRUE,ret))
#define MSGEXT_V(x) LOG_REQ(dbg_l_MsgExit, LOG_DEBUG,(x,FALSE))
#endif

#ifdef CC_SECURE_WORLD
#define ASSERT(x) assert(x)
#endif

#define RUN0(x) RUN_CODE(L_LOG_ALWAYS, x)
#define RUN1(x) RUN_CODE(LOG_ERROR, x)
#define RUN2(x) RUN_CODE(LOG_WARN, x)
#define RUN3(x) RUN_CODE(LOG_INFO, x)

#define LOGOUT() do{dbg_PrintErrStack();CLEARLOG();}while(0)

#else
/**
 * @brief Logs a message (always).  
 *
 * @param[in] x bracketted list of the type (format, ..)
 *
 * @return none 
*/
#define MSG0(x)
/**
 * @brief Logs a message (if module has ERROR logging).  
 *
 * @param[in] x bracketted list of the type (format, ..)
 *
 * @return none 
*/
#define MSG1(x)
/**
 * @brief Logs a message (if module has WARN logging).  
 *
 * @param[in] x bracketted list of the type (format, ..)
 *
 * @return none 
*/
#define MSG2(x)
/**
 * @brief Logs a message (if module has INFO logging).  
 *
 * @param[in] x bracketted list of the type (format, ..)
 *
 * @return none 
*/
#define MSG3(x)

/**
 * @brief Logs a message with indentation (always).  
 *
 * @param[in] x bracketted list of the type (format, ..)
 *
 * @return none 
*/
#define MSGI0(x)
/**
 * @brief Logs a message with indentation (if module has ERROR logging).  
 *
 * @param[in] x bracketted list of the type (format, ..)
 *
 * @return none 
*/
#define MSGI1(x)
/**
 * @brief Logs a message with indentation (if module has WARN logging).  
 *
 * @param[in] x bracketted list of the type (format, ..)
 *
 * @return none 
*/
#define MSGI2(x)
/**
 * @brief Logs a message with indentation (if module has INFO logging).  
 *
 * @param[in] x bracketted list of the type (format, ..)
 *
 * @return none 
*/
#define MSGI3(x)

/**
 * @brief Prints a string (always), with non-printable characters made readable.  
 *
 * @param[in] x bracketted list of the type (len, buff)
 *
 * @return none
 */
#define PRINT0(x)
/**
 * @brief Prints a string (if module has ERROR logging), with non-printable characters made readable.  
 *
 * @param[in] x bracketted list of the type (len, buff)
 *
 * @return none
 */
#define PRINT1(x)
/**
 * @brief Prints a string (if module has WARN logging), with non-printable characters made readable.  
 *
 * @param[in] x bracketted list of the type (len, buff)
 *
 * @return none
 */
#define PRINT2(x)
/**
 * @brief Prints a string (if module has INFO logging), with non-printable characters made readable.  
 *
 * @param[in] x bracketted list of the type (len, buff)
 *
 * @return none
 */
#define PRINT3(x)

/**
 * @brief Dumps data in hex (always).  
 *
 * @param[in] x bracketted list of the type (len, buff)
 *
 * @return none
 */
#define DUMP0(x)
/**
 * @brief Dumps data in hex (if module has ERROR logging).  
 *
 * @param[in] x bracketted list of the type (len, buff)
 *
 * @return none
 */
#define DUMP1(x)
/**
 * @brief Dumps data in hex (if module has WARN logging).  
 *
 * @param[in] x bracketted list of the type (len, buff)
 *
 * @return none
 */
#define DUMP2(x)
/**
 * @brief Dumps data in hex (if module has INFO logging).  
 *
 * @param[in] x bracketted list of the type (len, buff)
 *
 * @return none
 */
#define DUMP3(x)
/**
 * @brief Logs entrance in function and increment indentation, if module has DEBUG logging.    
 *
 * Used at the begining of function
 *
 * @return none
 */
#define MSGENT(x)
/**
 * @brief Logs exit of function and its return code in hex and decrement indentation, if module has DEBUG logging.    
 *
 * Used at the end of function
 *
 * @return none
 */
#define MSGEXT(x)

/**
 * @brief Logs exit of function and decrement indentation, if module has DEBUG logging.  
 *
 * Used at the end of function
 *
 * @return none
 */

#define MSGEXT_V(x)

/**
 * @brief Executes system's assert()    
 *
 * @return none
 */
#ifdef CC_SECURE_WORLD 
#define ASSERT(x)
#endif

/**
 * @brief Executes parameter (always)   
 *
 * @return none
 */
#define RUN0(x) 
/**
 * @brief Executes parameter (if module has ERROR logging)   
 *
 * @return none
 */
#define RUN1(x)
/**
 * @brief Executes parameter (if module has WARN logging)   
 *
 * @return none
 */
#define RUN2(x)
/**
 * @brief Executes parameter (if module has INFO logging)   
 *
 * @return none
 */
#define RUN3(x)

#define LOGOUT()

#endif

/*---------------------------------------------------------------------------*/
/**
 * @brief macros for debugging messages at function I/O
 *
 * @verbatim
 * usage:
 *   ERRLOG(err); // Push err on error stack
 * @endverbatim
 */
/*---------------------------------------------------------------------------*/

#if defined(NS_DEBUG)
#  define ERRLOG(err) dbg_l_WriteLog((err),#err,__FILE__,__FUNCTION__,__LINE__)
#  define CLEARLOG()  dbg_l_ClearErrStack()
#  define DBG_PREPARESAVELOG() int32_t l_dbg_stackdepth = 0
#  define DBG_SAVELOG() l_dbg_stackdepth=dbg_l_SaveErrStack()
#  define DBG_RESTORELOG() dbg_l_RestoreErrStack(l_dbg_stackdepth)
#else
#  define ERRLOG(err) (err)
#  define CLEARLOG()
#  define DBG_PREPARESAVELOG()
#  define DBG_SAVELOG()
#  define DBG_RESTORELOG()
#endif

/*---------------------------------------------------------------------------*
 * APIs
 *---------------------------------------------------------------------------*/

/**
 * @brief Output message with indent
 *
 * @param[in] in_fmt message format
 * @return  none
 */
extern void
dbg_l_MsgWithIndent(const char *in_fmt, ...);

/**
 * @brief Output message without indent
 *
 * @param[in] in_fmt message format
 *
 * @return  none
 */
extern void
dbg_l_Msg(const char *in_fmt, ...);

/**
 * @brief Output function name and increment indent
 *
 * @param[in] in_fname function name
 *
 * @return none 
 */
extern void
dbg_l_MsgEnter(const char *in_fname);

/**
 * @brief Output function name and decrement indent
 *
 * @param[in] in_fname function name
 *
 * @return none 
*/
extern void
dbg_l_MsgExit(const char *in_fname, bool_t is_ret, ...);

/**
 * @brief Hexadecimal dump for binary data
 *
 * @param[in] in_len     buffer size
 * @param[in] in_buf     buffer address
 *
 * @return none 
 */
void
dbg_l_Dump(np_size_t         in_len,
           const u_int8_t    *in_buf);

/**
 * @brief Character dump for binary data
 *
 * @param[in] in_context memory context
 * @param[in] in_len buffer size
 * @param[in] in_buf buffer address
 *
 * @return none
 */
void
dbg_l_Print(np_size_t         in_len,
            const u_int8_t    *in_buf);

/**
 * @brief Write error information to error stack
 *
 * @param[in] in_err error code
 * @param[in] in_eanme error name
 * @param[in] in_fname file name
 * @param[in] in_func function name
 * @param[in] in_line line number
 *
 * @return in_err
 */
extern retcode_t
dbg_l_WriteLog(retcode_t   in_err,
               const char *in_ename,
               const char *in_file,
               const char *in_func,
               int32_t     in_line);

/**
 * @brief Clears error stack
 *
 * @return ::ERR_OK        
 */
extern retcode_t
dbg_l_ClearErrStack( void);

/**
 * @brief Saves the error stack
 *
 * @return Debug stack counter
 */
extern int32_t
dbg_l_SaveErrStack( void);

/**
 * @brief Restores the error stack counter 
 *
 * @param[in]  in_depth  Error stack counter value
 *
 * @return ::ERR_OK
 */
extern retcode_t
dbg_l_RestoreErrStack(int32_t in_depth);

#if !defined(NPOPT_PUBLISH_BUILD)

/**
 * @brief Initializes debug environment
 *
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_NOMEM                  <br>A resource failure has occurred.
 * @return ::ERR_ILLEGALARGS            <br>The input arguments are incorrect. Arguments cannot be NULL.
 * @return ::ERR_NG                     <br>If failed to get the context of intialized memory.
*/
extern retcode_t
dbg_Init( void);

/**
 * @brief Outputs error stack
 *
 * @return ::ERR_OK                     <br>Indicating success
 * @return ::ERR_INTERNALERROR          <br>An unrecoverable error has occurred in Nautilus.
 * @return ::ERR_NG                     <br>If failed to get the context of memory.
 */
extern retcode_t
dbg_PrintErrStack( void);

/**
 * @brief Finalize debug environment
 *
 * @return ::ERR_OK                     <br>Indicating succes
 * @return ::ERR_INTERNALERROR  	<br>An unrecoverable error has occurred in Nautilus.
 * @return ::ERR_NG                     <br>If failed to get the context of intialized memory.
 */
extern retcode_t
dbg_Fin( void);

#else

#define dbg_Init()
#define dbg_PrintErrStack()
#define dbg_Fin()

#endif /* NPOPT_PUBLISH_BUILD */

NP_END_EXTERN_C

#endif /* DBG_API_H */
/** @} */
