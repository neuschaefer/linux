/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2008,2009,2010 Sony Corporation, All Rights Reserved.
 *
 */

#ifndef BASIC_TYPES__H
#define BASIC_TYPES__H

/*===========================================================================*/
/** @file
 * Basic type and macro definition (NP internal, platform depended)
 *
 * @reqire none
 * @author Norifumi Goto <n-goto@sm.sony.co.jp>
 */
/*===========================================================================*/

NP_BEGIN_EXTERN_C

#ifndef NULL
# define NULL  (void*)0
#endif

/*
 * def: NAUTILUS_MODULE
 */

#define NAUTILUS_MODULE(modulename) \
    char *l_modulename = #modulename

/*
 * def: INLINE
 */
#if defined(__STRICT_ANSI__)
#define INLINE
#else
#define INLINE
#endif

/*
 * type: bool_t
 */
typedef int				bool_t;

/*
 * type: uintptr_t
 */

/*
 * type: fio_size_t, fio_offset_t
 *
 *   There is no necessity which is fio_size_t == np_size_t.
 *   fio_size_t may change to handle the file which exceeds 4G Byte
 *   in the future.
 */
typedef unsigned long		fio_size_t;
typedef long				fio_offset_t;

/*
 * type: time_unix_t
 */
typedef u_int32_t			time_unix_t;

/*
 * type: mutex_t
 */
typedef struct {
    //pthread_mutex_t  mutex;
    int32_t          init;
}                           mutex_t;

NP_END_EXTERN_C

#endif
