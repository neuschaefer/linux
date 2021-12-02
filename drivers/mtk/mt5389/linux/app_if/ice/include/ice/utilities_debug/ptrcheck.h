/**
 * Copyright 2004 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Portable pointer check
 */

#ifndef CABOT_UTILITIES_DEBUG_PTRCHECK_H_INCLUDED
#define CABOT_UTILITIES_DEBUG_PTRCHECK_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Generic pointer check which should work on any platform
 */
#ifdef CABOT_VERIFY
extern void CabotPointerCheck(unsigned char* ptr);
#define CABOT_POINTER_CHECK(e) CabotPointerCheck(e)
#else
#define CABOT_POINTER_CHECK(ignore) ((void)0)
#endif /* CABOT_VERIFY */

#ifdef __cplusplus
}
#endif

#endif /* CABOT_UTILITIES_DEBUG_PTRCHECK_H_INCLUDED*/
