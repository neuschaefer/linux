/**
 * Copyright 2002-4 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Cabot portable interface to basic types
 * (fixed-sized integral types and booleans).
 *
 * This file provides some portable typedefs, in a manner comparable to
 * that of C99's <inttypes.h> but portable to C and C++ compilers.
 *
 * @note  This header file is currently only portable to platforms
 *        where CHAR_BIT==8, sizeof(short)==2, sizeof(int)==4 (and
 *        there are no pad bits in short or int).
 */

#ifndef CABOT_FROST_FROST_BASICTYPES_H_INCLUDED
#define CABOT_FROST_FROST_BASICTYPES_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * A portable typedef for an unsigned integral type of at least 16 bits.
 */
typedef unsigned int frost_uint;

/**
 * A portable typedef for an signed integral type of at least 16 bits.
 */
typedef int frost_int;

/**
 * A portable typedef for an unsigned integral type of at least 32 bits.
 */
typedef unsigned long frost_ulong;

/**
 * A portable typedef for an signed integral type of at least 32 bits.
 */
typedef long frost_long;

/**
 * A portable typedef for an unsigned integral type of exactly 32 bits.
 * FROST is not currently portable to platforms which do not have such a type.
 */
typedef unsigned int frost_uint32;

/**
 * A portable typedef for an unsigned integral type of exactly 64 bits,
 * represented as two 32-bit words.
 */
typedef struct frost_uint64s
{
    frost_uint32 high; /**< High 32 bits */
    frost_uint32 low;  /**< Low 32 bits */
} frost_uint64s;

/**
 * A portable typedef for an unsigned integral type of exactly 16 bits.
 * FROST is not currently portable to platforms which do not have such a type.
 */
typedef unsigned short frost_uint16;

/**
 * A portable typedef for an unsigned integral type of exactly 8 bits.
 * FROST is not currently portable to platforms which do not have such a type.
 */
typedef unsigned char frost_uint8;

/**
 * A portable typedef for a signed integral type of exactly 32 bits.
 * FROST is not currently portable to platforms which do not have such a type.
 */
typedef int frost_int32;

/**
 * A portable typedef for a signed integral type of exactly 16 bits.
 * FROST is not currently portable to platforms which do not have such a type.
 */
typedef short frost_int16;

/**
 * A portable typedef for a signed integral type of exactly 8 bits.
 * FROST is not currently portable to platforms which do not have such a type,
 * i.e., platforms on which (CHAR_BIT != 8).
 */
typedef char frost_int8;

/**
 * A portable typedef for a boolean type.
 */
typedef enum frost_bool 
{
    frost_false = 0,  /**< Boolean false */
    frost_true = 1    /**< Boolean true */

} frost_bool;

/**
 * @def FROST_INT64_SUPPORTED
 *
 * This is defined if and only if the platform has the 64-bit types
 * frost_int64 and frost_uint64.
 */
/**
 * @typedef frost_int64
 *
 * A mostly-portable typedef for a signed integral type of exactly 64 bits.
 *
 * Not all platforms have such a type.  Check the FROST_INT64_SUPPORTED
 * define to test if this type is supported.
 */
/**
 * @typedef frost_uint64
 *
 * A mostly-portable typedef for an unsigned integral type of exactly 64 bits.
 *
 * Not all platforms have such a type.  Check the FROST_INT64_SUPPORTED
 * define to test if this type is supported.
 */
#if defined(FROST_DISABLE_INT64_FOR_TESTING_PURPOSES) && defined(WIN32)
    /* Cabot will use this for debugging our (above-ICE) 64-bit emulation code
     * under Windows.
     * Customers should never define FROST_DISABLE_INT64_FOR_TESTING_PURPOSES.
     */
    #undef FROST_INT64_SUPPORTED
#elif defined(__GNUC__) || defined(__DOXYGEN__)
    /* GCC supports 64-bit integers on most 32-bit and 64-bit platforms;
     * Doxygen understands GNU's "long long" syntax.
     */
    #define FROST_INT64_SUPPORTED
    typedef long long frost_int64;
    typedef unsigned long long frost_uint64;
#elif defined(_MSC_VER)
    /* Visual C++ supports 64-bit integers on both 32-bit and 64-bit Windows */
    #define FROST_INT64_SUPPORTED
    typedef __int64 frost_int64;
    typedef unsigned __int64 frost_uint64;
#else
    /* Assume this platform doesn't support real 64-bit ints. */
    #undef FROST_INT64_SUPPORTED
#endif

/**
 * @typedef frost_intptr
 *
 * @brief A signed integer that is the same size as a pointer.
 *
 * This is used where the code casts between pointers and integers.
 * It is a 64-bit integer if the platform uses 64-bit pointers,
 * or a 32-bit integer if the platform uses 32-bit pointers
 *
 * This is equivalent to the C99 "intptr_t" type.  However, not all
 * compilers support C99 yet, so Cabot code should use this FROST type.
 */
/**
 * @typedef frost_uintptr
 *
 * @brief An unsigned integer that is the same size as a pointer.
 *
 * This is used where the code casts between pointers and integers.
 * It is a 64-bit integer if the platform uses 64-bit pointers,
 * or a 32-bit integer if the platform uses 32-bit pointers
 *
 * This is equivalent to the C99 "uintptr_t" type.  However, not all
 * compilers support C99 yet, so Cabot code should use this FROST type.
 */
#if defined(__GNUC__) && defined(__x86_64__)
    /* Linux on x86-64, a 64-bit platform. */
    typedef          long long frost_intptr;
    typedef unsigned long long frost_uintptr;
#elif defined(WIN64)
    /* Windows on x86-64, a 64-bit platform. */
    typedef          __int64 frost_intptr;
    typedef unsigned __int64 frost_uintptr;
#elif defined(_MSC_VER) && (_MSC_VER >= 1300)
    /* Windows on x86, a 32-bit platform
     *
     * The __w64 annotation helps the compiler issue the right warnings when
     * you use the /Wp64 "detect 64-bit portability issues" option.
     * It tells the compiler that frost_intptr will be 64-bit on 64-bit
     * systems.
     */
    typedef __w64          int frost_intptr;
    typedef __w64 unsigned int frost_uintptr;
#else
    /* All other platforms are 32-bit. */
    typedef frost_int32  frost_intptr;
    typedef frost_uint32 frost_uintptr;
#endif

typedef void *         frost_pointer;

typedef const void *   frost_constpointer;
#ifdef __cplusplus
}
#endif

#endif /* CABOT_FROST_FROST_BASICTYPES_H_INCLUDED*/
