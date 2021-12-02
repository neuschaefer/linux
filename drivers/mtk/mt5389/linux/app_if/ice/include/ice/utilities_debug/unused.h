/**
 * Copyright 2004 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Prevent unused parameter warnings
 */

#ifndef CABOT_UTILITIES_DEBUG_UNUSED_H_INCLUDED
#define CABOT_UTILITIES_DEBUG_UNUSED_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Generic macro to apply to unused parameters to remove warnings
 */
#define CABOT_UNUSED_PARAM(p) ((void)p)

#ifdef CABOT_TRACE
    #define TRACE_ONLY_PARAM(e)
#else
    /**
     * Macro to apply to parameters only needed by TRACE calls, to help eradicate
     * warnings about unused parameters
     */
    #define TRACE_ONLY_PARAM(e)        CABOT_UNUSED_PARAM(e)
#endif /* CABOT_TRACE */

#ifdef CABOT_ASSERTIONS
    #define CABOT_ASSERT_ONLY(e)
#else
    /**
     * Macro to apply to parameters only needed by CABOT_ASSERT calls, to help
     * eradicate warnings about unused parameters
     */
    #define CABOT_ASSERT_ONLY(e)        CABOT_UNUSED_PARAM(e)
#endif /* CABOT_ASSERTIONS */


#ifdef __cplusplus
}
#endif

#endif /* CABOT_UTILITIES_DEBUG_UNUSED_H_INCLUDED*/



