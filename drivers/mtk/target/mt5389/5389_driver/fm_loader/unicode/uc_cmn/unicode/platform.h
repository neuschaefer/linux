/*
******************************************************************************
*
*   Copyright (C) 1997-2004, International Business Machines
*   Corporation and others.  All Rights Reserved.
*
******************************************************************************
*
*  FILE NAME : platform.h
*
*   Date        Name        Description
*   05/13/98    nos         Creation (content moved here from ptypes.h).
*   03/02/99    stephen     Added AS400 support.
*   03/30/99    stephen     Added Linux support.
*   04/13/99    stephen     Reworked for autoconf.
******************************************************************************
*/

/* Define the platform we're on. */
#ifndef U_CYGWIN
#define U_CYGWIN
#endif

/* Define whether inttypes.h is available */
#ifndef U_HAVE_INTTYPES_H
#define U_HAVE_INTTYPES_H 1
#endif

/*
 * Define what support for C++ streams is available.
 *     If U_IOSTREAM_SOURCE is set to 199711, then <iostream> is available
 * (1997711 is the date the ISO/IEC C++ FDIS was published), and then
 * one should qualify streams using the std namespace in ICU header
 * files.
 *     If U_IOSTREAM_SOURCE is set to 198506, then <iostream.h> is
 * available instead (198506 is the date when Stroustrup published
 * "An Extensible I/O Facility for C++" at the summer USENIX conference).
 *     If U_IOSTREAM_SOURCE is 0, then C++ streams are not available and
 * support for them will be silently suppressed in ICU.
 *
 */

#ifndef U_IOSTREAM_SOURCE
#define U_IOSTREAM_SOURCE 0
#endif

/* Determines whether specific types are available */
#ifndef U_HAVE_INT8_T
#define U_HAVE_INT8_T 1
#endif

#ifndef U_HAVE_UINT8_T
#define U_HAVE_UINT8_T 1
#endif

#ifndef U_HAVE_INT16_T
#define U_HAVE_INT16_T 1
#endif

#ifndef U_HAVE_UINT16_T
#define U_HAVE_UINT16_T 1
#endif

#ifndef U_HAVE_INT32_T
#define U_HAVE_INT32_T 1
#endif

#ifndef U_HAVE_UINT32_T
#define U_HAVE_UINT32_T 1
#endif

#ifndef U_HAVE_INT64_T
#define U_HAVE_INT64_T 1
#endif

#ifndef U_HAVE_UINT64_T
#define U_HAVE_UINT64_T 1
#endif

/*===========================================================================*/
/* Generic data types                                                        */
/*===========================================================================*/
/*
#include "sys/types.h"
*/

/*
#if ! U_HAVE_INT8_T
typedef signed char int8_t;
#endif

#if ! U_HAVE_UINT8_T
typedef unsigned char uint8_t;
#endif

#if ! U_HAVE_INT16_T
typedef signed short int16_t;
#endif

#if ! U_HAVE_UINT16_T
typedef unsigned short uint16_t;
#endif

#if ! U_HAVE_INT32_T
typedef signed int int32_t;
#endif

#if ! U_HAVE_UINT32_T
typedef unsigned int uint32_t;
#endif

#if ! U_HAVE_INT64_T
    typedef signed long long int64_t;
#endif

#if ! U_HAVE_UINT64_T
    typedef unsigned long long uint64_t;
#endif
*/

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;

#ifndef EXT_INT64_TYPE
typedef signed long long    int64_t;
#else
typedef EXT_INT64_TYPE      int64_t;
#endif

#ifndef EXT_UINT64_TYPE
typedef unsigned long long  uint64_t;
#else
typedef EXT_UINT64_TYPE     uint64_t;
#endif


/*===========================================================================*/
/* Compiler and environment features                                         */
/*===========================================================================*/

/* Define whether namespace is supported */
#ifndef U_HAVE_NAMESPACE
#define U_HAVE_NAMESPACE 1
#endif

/* Determines the endianness of the platform
   It's done this way in case multiple architectures are being built at once.
   For example, Darwin supports fat binaries, which can be both PPC and x86 based. */
#if defined(BYTE_ORDER) && defined(BIG_ENDIAN)
#define U_IS_BIG_ENDIAN (BYTE_ORDER == BIG_ENDIAN)
#else
#define U_IS_BIG_ENDIAN 0
#endif

/* 1 or 0 to enable or disable threads.  If undefined, default is: enable threads. */
#define ICU_USE_THREADS 1

#ifndef U_DEBUG
#define U_DEBUG 0
#endif

#ifndef U_RELEASE
#define U_RELEASE 1
#endif

#define U_DISABLE_RENAMING 1

/* Determine whether to override new and delete. */
#ifndef U_OVERRIDE_CXX_ALLOCATION
#define U_OVERRIDE_CXX_ALLOCATION 1
#endif
/* Determine whether to override placement new and delete for STL. */
#ifndef U_HAVE_PLACEMENT_NEW
#define U_HAVE_PLACEMENT_NEW 1
#endif

/* Determine whether to enable tracing. */
#ifndef U_ENABLE_TRACING
#define U_ENABLE_TRACING 0
#endif

/* Define the library suffix in a C syntax. */
#define U_HAVE_LIB_SUFFIX 0
#define U_LIB_SUFFIX_C_NAME
#define U_LIB_SUFFIX_C_NAME_STRING ""

/*===========================================================================*/
/* Character data types                                                      */
/*===========================================================================*/

#if defined(OS390) || defined(OS400)
#   define U_CHARSET_FAMILY 1
#endif

/*===========================================================================*/
/* Information about POSIX support                                           */
/*===========================================================================*/

#define U_HAVE_NL_LANGINFO          1
#define U_HAVE_NL_LANGINFO_CODESET  1
#define U_NL_LANGINFO_CODESET       CODESET

#if 1
#define U_TZSET         tzset
#endif
#if 1
#define U_TIMEZONE      _timezone
#endif
#if 1
#define U_TZNAME        tzname
#endif

#define U_HAVE_MMAP     1
#define U_HAVE_POPEN    1

/*===========================================================================*/
/* Symbol import-export control                                              */
/*===========================================================================*/

#define U_EXPORT
/* U_CALLCONV is releated to U_EXPORT2 */
#define U_EXPORT2

/* cygwin needs to export/import data */
#ifdef U_CYGWIN
#define U_IMPORT __declspec(dllimport)
#else
#define U_IMPORT
#endif

/*===========================================================================*/
/* Code alignment and C function inlining                                    */
/*===========================================================================*/

#define U_ALIGN_CODE(n)

/*===========================================================================*/
/* Programs used by ICU code                                                 */
/*===========================================================================*/

#define U_MAKE  "make"
