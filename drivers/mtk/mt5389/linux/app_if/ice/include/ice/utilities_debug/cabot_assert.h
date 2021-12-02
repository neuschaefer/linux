/**
 * Copyright 2004 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Portable assertions
 */

#ifndef CABOT_UTILITIES_DEBUG_CABOT_ASSERT_H_INCLUDED
#define CABOT_UTILITIES_DEBUG_CABOT_ASSERT_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Generic assert which should work on any platform
 */
#ifdef CABOT_ASSERTIONS
    extern void CabotAssertFailed(char const*, char const*, int);
    #define CABOT_ASSERT(e)      ((e) ? (void)0 : CabotAssertFailed(#e, __FILE__, __LINE__))
    /*
     * The following macro should be used in preference to CABOT_ASSERT(false)
     * because:
     * 1) it is Lint clean -- CABOT_ASSERT(false) fires Lint warning 506
     * 2) the code more clearly expresses its intent.
     */
    #define SHOULD_NOT_BE_HERE() CabotAssertFailed("Should not be here!", __FILE__, __LINE__)
#else
    #define CABOT_ASSERT(ignore) ((void)0)
    #define SHOULD_NOT_BE_HERE() ((void)0)
#endif /* CABOT_ASSERTIONS */

/**
 * Build-time assertion.  Will break the build if the specified expression is
 * false.  The expression must be a compile-time constant
 *
 * This works by evaluating sizeof(int[1]) if the expression is true,
 * or sizeof(int[-1]) if the expression is false.  sizeof(int[-1])
 * should be a compile error under most compilers.
 *
 * With an optimising compiler, this should generate zero code.
 *
 * For example:
 *
 * enum Baz { FOO = 5, BAR };
 *
 * void myfunc(Baz x)
 * {
 *    CABOT_BUILD_ASSERT(FOO == 5); // Evaluated at compile time
 *    CABOT_BUILD_ASSERT(BAR == 6); // Evaluated at compile time
 *    CABOT_ASSERT(x == FOO || x == BAR); // Evaluated at runtime
 *    ...
 * }
 */
#define CABOT_BUILD_ASSERT(e) ((void)sizeof(int[(e) ? 1 : -1]))

#ifdef __cplusplus
}
#endif

#endif /* CABOT_UTILITIES_DEBUG_CABOT_ASSERT_H_INCLUDED*/

