/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @file u_common.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains type definitions common to the whole
 *         Middleware.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW Middleware
 *
 *  @brief This module provides APIs and Definations which used by application and driver layer.
 *
 *  %MW All commands application send to driver and messages driver send to application
 *   through middleware layer. 
 *
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_COMMON_H_
#define _U_COMMON_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include <stdarg.h>
#include <stddef.h>


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#if !defined (_NO_TYPEDEF_UCHAR_) && !defined (_TYPEDEF_UCHAR_)
typedef unsigned char  UCHAR;    /**<        */

#define _TYPEDEF_UCHAR_    /**<        */
#endif

#if !defined (_NO_TYPEDEF_UINT8_) && !defined (_TYPEDEF_UINT8_)
typedef unsigned char  UINT8;    /**<        */

#define _TYPEDEF_UINT8_    /**<        */
#endif

#if !defined (_NO_TYPEDEF_UINT16_) && !defined (_TYPEDEF_UINT16_)
typedef unsigned short  UINT16;    /**<        */

#define _TYPEDEF_UINT16_    /**<        */
#endif

#if !defined (_NO_TYPEDEF_UINT32_) && !defined (_TYPEDEF_UINT32_)

#ifndef EXT_UINT32_TYPE
typedef unsigned long    UINT32;    /**<        */
#else
typedef EXT_UINT32_TYPE  UINT32;    /**<        */
#endif

#define _TYPEDEF_UINT32_    /**<        */
#endif

#if !defined (_NO_TYPEDEF_UINT64_) && !defined (_TYPEDEF_UINT64_)

#ifndef EXT_UINT64_TYPE
typedef unsigned long long  UINT64;    /**<        */
#else
typedef EXT_UINT64_TYPE     UINT64;    /**<        */
#endif

#define _TYPEDEF_UINT64_    /**<        */
#endif

#if !defined (_NO_TYPEDEF_CHAR_) && !defined (_TYPEDEF_CHAR_)
typedef char  CHAR;    /**<        */

#define _TYPEDEF_CHAR_    /**<        */
#endif

#if !defined (_NO_TYPEDEF_INT8_) && !defined (_TYPEDEF_INT8_)
typedef signed char  INT8;    /**<        */

#define _TYPEDEF_INT8_    /**<        */
#endif

#if !defined (_NO_TYPEDEF_INT16_) && !defined (_TYPEDEF_INT16_)
typedef signed short  INT16;    /**<        */

#define _TYPEDEF_INT16_    /**<        */
#endif

#if !defined (_NO_TYPEDEF_INT32_) && !defined (_TYPEDEF_INT32_)

#ifndef EXT_INT32_TYPE
typedef signed long     INT32;    /**<        */
#else
typedef EXT_INT32_TYPE  INT32;    /**<        */
#endif

#define _TYPEDEF_INT32_    /**<        */
#endif

#if !defined (_NO_TYPEDEF_INT64_) && !defined (_TYPEDEF_INT64_)

#ifndef EXT_INT64_TYPE
typedef signed long long  INT64;    /**<        */
#else
typedef EXT_INT64_TYPE    INT64;    /**<        */
#endif

#define _TYPEDEF_INT64_    /**<        */
#endif

#if !defined (_NO_TYPEDEF_SIZE_T_) && !defined (_TYPEDEF_SIZE_T_)

#ifndef EXT_SIZE_T_TYPE
typedef size_t           SIZE_T;    /**<        */
#else
typedef EXT_SIZE_T_TYPE  SIZE_T;    /**<        */
#endif

#define _TYPEDEF_SIZE_T_    /**<        */
#endif

#if !defined (_NO_TYPEDEF_UTF16_T_) && !defined (_TYPEDEF_UTF16_T_)
typedef unsigned short  UTF16_T;    /**<        */

#define _TYPEDEF_UTF16_T_    /**<        */
#endif

#if !defined (_NO_TYPEDEF_UTF32_T_) && !defined (_TYPEDEF_UTF32_T_)
typedef unsigned long  UTF32_T;    /**<        */

#define _TYPEDEF_UTF32_T_    /**<        */
#endif

#if !defined (_NO_TYPEDEF_FLOAT_) && !defined (_TYPEDEF_FLOAT_)
typedef float  FLOAT;    /**<        */

#define _TYPEDEF_FLOAT_    /**<        */
#endif

#if !defined (_NO_TYPEDEF_DOUBLE_)  && !defined (_TYPEDEF_DOUBLE_)
typedef double  DOUBLE;    /**<        */

#define _TYPEDEF_DOUBLE_    /**<        */
#endif

/* Do not typedef VOID but use define. Will keep some */
/* compilers very happy.                              */
#if !defined (_NO_TYPEDEF_VOID_)  && !defined (_TYPEDEF_VOID_)
#undef VOID
#define VOID  void    /**<        */

#define _TYPEDEF_VOID_    /**<        */
#endif

/* Define a boolean as 8 bits. */
#if !defined (_NO_TYPEDEF_BOOL_) && !defined (_TYPEDEF_BOOL_)

#ifndef EXT_BOOL_TYPE
typedef UINT8  BOOL;    /**<        */
#else
typedef signed EXT_BOOL_TYPE  BOOL;    /**<        */
#endif

#define _TYPEDEF_BOOL_    /**<        */

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define TRUE ((BOOL) 1)    /**<        */
#define FALSE ((BOOL) 0)    /**<        */
#endif

/* Variable argument macros. The ones named "va_..." are defined */
/* in the header file "stdarg.h".                                */
#ifdef VA_LIST
#undef VA_LIST
#endif

#ifdef VA_START
#undef VA_START
#endif

#ifdef VA_END
#undef VA_END
#endif

#ifdef VA_ARG
#undef VA_ARG
#endif

#ifdef VA_COPY
#undef VA_COPY
#endif

#define VA_LIST  va_list    /**<        */
#define VA_START va_start    /**<        */
#define VA_END   va_end    /**<        */
#define VA_ARG   va_arg    /**<        */
#define VA_COPY  va_copy    /**<        */


/* Min and max macros. Watch for side effects! */
#define X_MIN(_x, _y)  (((_x) < (_y)) ? (_x) : (_y))  /**< Get the minimal value of x,y*/
#define X_MAX(_x, _y)  (((_x) > (_y)) ? (_x) : (_y))  /**< Get the maximal value of x,y*/

/* The following macros are useful to create bit masks. */
#define MAKE_BIT_MASK_8(_val)  (((UINT8)  1) << (_val))    /**<        */
#define MAKE_BIT_MASK_16(_val) (((UINT16) 1) << (_val))    /**<        */
#define MAKE_BIT_MASK_32(_val) (((UINT32) 1) << (_val))    /**<        */
#define MAKE_BIT_MASK_64(_val) (((UINT64) 1) << (_val))    /**<        */

/* The following macros can be used to convert to and from standard    */
/* endian. Standard endian is defined as BIG ENDIAN in the middleware. */
/* Note that one, and only one, of the definitions _CPU_BIG_ENDIAN_ or */
/* _CPU_LITTLE_ENDIAN_ must be set.                                    */
#if (defined (_CPU_LITTLE_ENDIAN_) && defined (_CPU_BIG_ENDIAN_))
#error "Not allowed to define _CPU_LITTLE_ENDIAN_ and _CPU_BIG_ENDIAN_ concurrently."
#endif

#if ((! defined (_CPU_LITTLE_ENDIAN_)) && (! defined (_CPU_BIG_ENDIAN_)))
#error "Must specify the CPU's endian by setting either _CPU_LITTLE_ENDIAN_ or _CPU_BIG_ENDIAN_."
#endif

/* The following macros swap 16, 32 or 64 bit values from big */
/* to little or from little to big endian.                    */
#define SWAP_END_16(_val)                             \
    (((((UINT16) _val) & ((UINT16) 0x00ff)) << 8)  |  \
     ((((UINT16) _val) & ((UINT16) 0xff00)) >> 8))    /**<        */

#define SWAP_END_32(_val)                                  \
    (((((UINT32) _val) & ((UINT32) 0x000000ff)) << 24)  |  \
     ((((UINT32) _val) & ((UINT32) 0x0000ff00)) <<  8)  |  \
     ((((UINT32) _val) & ((UINT32) 0x00ff0000)) >>  8)  |  \
     ((((UINT32) _val) & ((UINT32) 0xff000000)) >> 24))    /**<        */

#define SWAP_END_64(_val)                                             \
    (((((UINT64) _val) & ((UINT64) 0x00000000000000ffLL)) << 56)   |  \
     ((((UINT64) _val) & ((UINT64) 0x000000000000ff00LL)) << 40)   |  \
     ((((UINT64) _val) & ((UINT64) 0x0000000000ff0000LL)) << 24)   |  \
     ((((UINT64) _val) & ((UINT64) 0x00000000ff000000LL)) <<  8)   |  \
     ((((UINT64) _val) & ((UINT64) 0x000000ff00000000LL)) >>  8)   |  \
     ((((UINT64) _val) & ((UINT64) 0x0000ff0000000000LL)) >> 24)   |  \
     ((((UINT64) _val) & ((UINT64) 0x00ff000000000000LL)) >> 40)   |  \
     ((((UINT64) _val) & ((UINT64) 0xff00000000000000LL)) >> 56))    /**<        */


/* The following macros return a 16, 32 or 64 bit value given a reference */
/* to a memory location and the endian of the data representation.        */
#define GET_UINT16_FROM_PTR_BIG_END(_ptr)      \
    ((((UINT16) (((UINT8*) _ptr) [0])) << 8) | \
     ((UINT16)  (((UINT8*) _ptr) [1])))    /**<        */

#define GET_UINT32_FROM_PTR_BIG_END(_ptr)       \
    ((((UINT32) (((UINT8*) _ptr) [0])) << 24) | \
     (((UINT32) (((UINT8*) _ptr) [1])) << 16) | \
     (((UINT32) (((UINT8*) _ptr) [2])) <<  8) | \
     ((UINT32)  (((UINT8*) _ptr) [3])))    /**<        */

#define GET_UINT64_FROM_PTR_BIG_END(_ptr)       \
    ((((UINT64) (((UINT8*) _ptr) [0])) << 56) | \
     (((UINT64) (((UINT8*) _ptr) [1])) << 48) | \
     (((UINT64) (((UINT8*) _ptr) [2])) << 40) | \
     (((UINT64) (((UINT8*) _ptr) [3])) << 32) | \
     (((UINT64) (((UINT8*) _ptr) [4])) << 24) | \
     (((UINT64) (((UINT8*) _ptr) [5])) << 16) | \
     (((UINT64) (((UINT8*) _ptr) [6])) <<  8) | \
     ((UINT64)  (((UINT8*) _ptr) [7])))    /**<        */

#define GET_INT16_FROM_PTR_BIG_END(_ptr)  ((INT16) GET_UINT16_FROM_PTR_BIG_END (_ptr))    /**<        */
#define GET_INT32_FROM_PTR_BIG_END(_ptr)  ((INT32) GET_UINT32_FROM_PTR_BIG_END (_ptr))    /**<        */
#define GET_INT64_FROM_PTR_BIG_END(_ptr)  ((INT64) GET_UINT64_FROM_PTR_BIG_END (_ptr))    /**<        */

#define GET_UINT16_FROM_PTR_LITTLE_END(_ptr)   \
    ((((UINT16) (((UINT8*) _ptr) [1])) << 8) | \
     ((UINT16)  (((UINT8*) _ptr) [0])))    /**<        */

#define GET_UINT32_FROM_PTR_LITTLE_END(_ptr)    \
    ((((UINT32) (((UINT8*) _ptr) [3])) << 24) | \
     (((UINT32) (((UINT8*) _ptr) [2])) << 16) | \
     (((UINT32) (((UINT8*) _ptr) [1])) <<  8) | \
     ((UINT32)  (((UINT8*) _ptr) [0])))    /**<        */

#define GET_UINT64_FROM_PTR_LITTLE_END(_ptr)    \
    ((((UINT64) (((UINT8*) _ptr) [7])) << 56) | \
     (((UINT64) (((UINT8*) _ptr) [6])) << 48) | \
     (((UINT64) (((UINT8*) _ptr) [5])) << 40) | \
     (((UINT64) (((UINT8*) _ptr) [4])) << 32) | \
     (((UINT64) (((UINT8*) _ptr) [3])) << 24) | \
     (((UINT64) (((UINT8*) _ptr) [2])) << 16) | \
     (((UINT64) (((UINT8*) _ptr) [1])) <<  8) | \
     ((UINT64)  (((UINT8*) _ptr) [0])))    /**<        */

#define GET_INT16_FROM_PTR_LITTLE_END(_ptr)  ((INT16) GET_UINT16_FROM_PTR_LITTLE_END (_ptr))    /**<        */
#define GET_INT32_FROM_PTR_LITTLE_END(_ptr)  ((INT32) GET_UINT32_FROM_PTR_LITTLE_END (_ptr))    /**<        */
#define GET_INT64_FROM_PTR_LITTLE_END(_ptr)  ((INT64) GET_UINT64_FROM_PTR_LITTLE_END (_ptr))    /**<        */

#define GET_UINT16_FROM_PTR_STD_END(_ptr)  GET_UINT16_FROM_PTR_BIG_END (_ptr)    /**<        */
#define GET_UINT32_FROM_PTR_STD_END(_ptr)  GET_UINT32_FROM_PTR_BIG_END (_ptr)    /**<        */
#define GET_UINT64_FROM_PTR_STD_END(_ptr)  GET_UINT64_FROM_PTR_BIG_END (_ptr)    /**<        */
#define GET_INT16_FROM_PTR_STD_END(_ptr)   ((INT16) GET_UINT16_FROM_PTR_BIG_END (_ptr))    /**<        */
#define GET_INT32_FROM_PTR_STD_END(_ptr)   ((INT32) GET_UINT32_FROM_PTR_BIG_END (_ptr))    /**<        */
#define GET_INT64_FROM_PTR_STD_END(_ptr)   ((INT64) GET_UINT64_FROM_PTR_BIG_END (_ptr))    /**<        */


/* The following macros place a 16, 32 or 64 bit value in big or */
/* little endian format into a memory location.                  */
#define PUT_UINT16_TO_PTR_BIG_END(_val, _ptr)                        \
    ((UINT8*) _ptr) [0] = (UINT8) ((((UINT16) _val) & 0xff00) >> 8); \
    ((UINT8*) _ptr) [1] = (UINT8) (((UINT16)  _val) & 0x00ff);    /**<        */

#define PUT_UINT32_TO_PTR_BIG_END(_val, _ptr)                             \
    ((UINT8*) _ptr) [0] = (UINT8) ((((UINT32) _val) & 0xff000000) >> 24); \
    ((UINT8*) _ptr) [1] = (UINT8) ((((UINT32) _val) & 0x00ff0000) >> 16); \
    ((UINT8*) _ptr) [2] = (UINT8) ((((UINT32) _val) & 0x0000ff00)>>   8); \
    ((UINT8*) _ptr) [3] = (UINT8) (((UINT32)  _val) & 0x000000ff);    /**<        */

#define PUT_UINT64_TO_PTR_BIG_END(_val, _ptr)                                       \
    ((UINT8*) _ptr) [0] = (UINT8) ((((UINT64) _val) & 0xff00000000000000LL) >> 56); \
    ((UINT8*) _ptr) [1] = (UINT8) ((((UINT64) _val) & 0x00ff000000000000LL) >> 48); \
    ((UINT8*) _ptr) [2] = (UINT8) ((((UINT64) _val) & 0x0000ff0000000000LL) >> 40); \
    ((UINT8*) _ptr) [3] = (UINT8) ((((UINT64) _val) & 0x000000ff00000000LL) >> 32); \
    ((UINT8*) _ptr) [4] = (UINT8) ((((UINT64) _val) & 0x00000000ff000000LL) >> 24); \
    ((UINT8*) _ptr) [5] = (UINT8) ((((UINT64) _val) & 0x0000000000ff0000LL) >> 16); \
    ((UINT8*) _ptr) [6] = (UINT8) ((((UINT64) _val) & 0x000000000000ff00LL) >>  8); \
    ((UINT8*) _ptr) [7] = (UINT8) (((UINT64)  _val) & 0x00000000000000ffLL);    /**<        */

#define PUT_INT16_TO_PTR_BIG_END(_val, _ptr)  PUT_UINT16_TO_PTR_BIG_END (_val, _ptr)    /**<        */
#define PUT_INT32_TO_PTR_BIG_END(_val, _ptr)  PUT_UINT32_TO_PTR_BIG_END (_val, _ptr)    /**<        */
#define PUT_INT64_TO_PTR_BIG_END(_val, _ptr)  PUT_UINT64_TO_PTR_BIG_END (_val, _ptr)    /**<        */

#define PUT_UINT16_TO_PTR_LITTLE_END(_val, _ptr)                     \
    ((UINT8*) _ptr) [1] = (UINT8) ((((UINT16) _val) & 0xff00) >> 8); \
    ((UINT8*) _ptr) [0] = (UINT8) (((UINT16)  _val) & 0x00ff);    /**<        */

#define PUT_UINT32_TO_PTR_LITTLE_END(_val, _ptr)                          \
    ((UINT8*) _ptr) [3] = (UINT8) ((((UINT32) _val) & 0xff000000) >> 24); \
    ((UINT8*) _ptr) [2] = (UINT8) ((((UINT32) _val) & 0x00ff0000) >> 16); \
    ((UINT8*) _ptr) [1] = (UINT8) ((((UINT32) _val) & 0x0000ff00)>>   8); \
    ((UINT8*) _ptr) [0] = (UINT8) (((UINT32)  _val) & 0x000000ff);    /**<        */

#define PUT_UINT64_TO_PTR_LITTLE_END(_val, _ptr)                                    \
    ((UINT8*) _ptr) [7] = (UINT8) ((((UINT64) _val) & 0xff00000000000000LL) >> 56); \
    ((UINT8*) _ptr) [6] = (UINT8) ((((UINT64) _val) & 0x00ff000000000000LL) >> 48); \
    ((UINT8*) _ptr) [5] = (UINT8) ((((UINT64) _val) & 0x0000ff0000000000LL) >> 40); \
    ((UINT8*) _ptr) [4] = (UINT8) ((((UINT64) _val) & 0x000000ff00000000LL) >> 32); \
    ((UINT8*) _ptr) [3] = (UINT8) ((((UINT64) _val) & 0x00000000ff000000LL) >> 24); \
    ((UINT8*) _ptr) [2] = (UINT8) ((((UINT64) _val) & 0x0000000000ff0000LL) >> 16); \
    ((UINT8*) _ptr) [1] = (UINT8) ((((UINT64) _val) & 0x000000000000ff00LL) >>  8); \
    ((UINT8*) _ptr) [0] = (UINT8) (((UINT64)  _val) & 0x00000000000000ffLL);    /**<        */

#define PUT_INT16_TO_PTR_LITTLE_END(_val, _ptr)  PUT_UINT16_TO_PTR_LITTLE_END (_val, _ptr)    /**<        */
#define PUT_INT32_TO_PTR_LITTLE_END(_val, _ptr)  PUT_UINT32_TO_PTR_LITTLE_END (_val, _ptr)    /**<        */
#define PUT_INT64_TO_PTR_LITTLE_END(_val, _ptr)  PUT_UINT64_TO_PTR_LITTLE_END (_val, _ptr)    /**<        */

#define PUT_UINT16_TO_PTR_STD_END(_val, _ptr)  PUT_UINT16_TO_PTR_BIG_END (_val, _ptr)    /**<        */
#define PUT_UINT32_TO_PTR_STD_END(_val, _ptr)  PUT_UINT32_TO_PTR_BIG_END (_val, _ptr)    /**<        */
#define PUT_UINT64_TO_PTR_STD_END(_val, _ptr)  PUT_UINT64_TO_PTR_BIG_END (_val, _ptr)    /**<        */
#define PUT_INT16_TO_PTR_STD_END(_val, _ptr)   PUT_UINT16_TO_PTR_BIG_END (_val, _ptr)    /**<        */
#define PUT_INT32_TO_PTR_STD_END(_val, _ptr)   PUT_UINT32_TO_PTR_BIG_END (_val, _ptr)    /**<        */
#define PUT_INT64_TO_PTR_STD_END(_val, _ptr)   PUT_UINT64_TO_PTR_BIG_END (_val, _ptr)    /**<        */


#ifdef _CPU_BIG_ENDIAN_
#define CONV_BIG_END_TO_UINT16(_val)  ((UINT16) _val)    /**<        */
#define CONV_BIG_END_TO_UINT32(_val)  ((UINT32) _val)    /**<        */
#define CONV_BIG_END_TO_UINT64(_val)  ((UINT64) _val)    /**<        */
#define CONV_BIG_END_TO_INT16(_val)   ((INT16)  _val)    /**<        */
#define CONV_BIG_END_TO_INT32(_val)   ((INT32)  _val)    /**<        */
#define CONV_BIG_END_TO_INT64(_val)   ((INT64)  _val)    /**<        */

#define CONV_LITTLE_END_TO_UINT16(_val)  ((UINT16) SWAP_END_16 (_val))    /**<        */
#define CONV_LITTLE_END_TO_UINT32(_val)  ((UINT32) SWAP_END_32 (_val))    /**<        */
#define CONV_LITTLE_END_TO_UINT64(_val)  ((UINT64) SWAP_END_64 (_val))    /**<        */
#define CONV_LITTLE_END_TO_INT16(_val)   ((INT16)  SWAP_END_16 (_val))    /**<        */
#define CONV_LITTLE_END_TO_INT32(_val)   ((INT32)  SWAP_END_32 (_val))    /**<        */
#define CONV_LITTLE_END_TO_INT64(_val)   ((INT64)  SWAP_END_64 (_val))    /**<        */

#define CONV_STD_END_TO_UINT16(_val)  ((UINT16) _val)    /**<        */
#define CONV_STD_END_TO_UINT32(_val)  ((UINT32) _val)    /**<        */
#define CONV_STD_END_TO_UINT64(_val)  ((UINT64) _val)    /**<        */
#define CONV_STD_END_TO_INT16(_val)   ((INT16)  _val)    /**<        */
#define CONV_STD_END_TO_INT32(_val)   ((INT32)  _val)    /**<        */
#define CONV_STD_END_TO_INT64(_val)   ((INT64)  _val)    /**<        */

#endif

#ifdef _CPU_LITTLE_ENDIAN_
#define CONV_STD_END_TO_UINT16(_val)  ((UINT16) SWAP_END_16 (_val))    /**<        */
#define CONV_STD_END_TO_UINT32(_val)  ((UINT32) SWAP_END_32 (_val))    /**<        */
#define CONV_STD_END_TO_UINT64(_val)  ((UINT64) SWAP_END_64 (_val))    /**<        */
#define CONV_STD_END_TO_INT16(_val)   ((INT16)  SWAP_END_16 (_val))    /**<        */
#define CONV_STD_END_TO_INT32(_val)   ((INT32)  SWAP_END_32 (_val))    /**<        */
#define CONV_STD_END_TO_INT64(_val)   ((INT64)  SWAP_END_64 (_val))    /**<        */

#define CONV_BIG_END_TO_UINT16(_val)  ((UINT16) SWAP_END_16 (_val))    /**<        */
#define CONV_BIG_END_TO_UINT32(_val)  ((UINT32) SWAP_END_32 (_val))    /**<        */
#define CONV_BIG_END_TO_UINT64(_val)  ((UINT64) SWAP_END_64 (_val))    /**<        */
#define CONV_BIG_END_TO_INT16(_val)   ((INT16)  SWAP_END_16 (_val))    /**<        */
#define CONV_BIG_END_TO_INT32(_val)   ((INT32)  SWAP_END_32 (_val))    /**<        */
#define CONV_BIG_END_TO_INT64(_val)   ((INT64)  SWAP_END_64 (_val))    /**<        */

#define CONV_LITTLE_END_TO_UINT16(_val)  ((UINT16) _val)    /**<        */
#define CONV_LITTLE_END_TO_UINT32(_val)  ((UINT32) _val)    /**<        */
#define CONV_LITTLE_END_TO_UINT64(_val)  ((UINT64) _val)    /**<        */
#define CONV_LITTLE_END_TO_INT16(_val)   ((INT16)  _val)    /**<        */
#define CONV_LITTLE_END_TO_INT32(_val)   ((INT32)  _val)    /**<        */
#define CONV_LITTLE_END_TO_INT64(_val)   ((INT64)  _val)    /**<        */

#endif

/* The following type definition is used to store a STC / PTS value, which is */
/* in multiples of 90 kHz. Note that such a value is 33 bit in size. Hence,   */
/* the bits [32:0] will contain the actual STC / PTS value and the bits       */
/* [63:33] must be set '0'.                                                   */
typedef UINT64  STC_T;    /**<        */
typedef UINT64  PTS_T;    /**<        */

/* The following type is used to carry system or local time and date information. */
typedef INT64  TIME_T;    /**<        */

/* The following define the value of Null Time. */
#define NULL_TIME  ((TIME_T) 0)    /**<        */

/* The following definitions are used to define a MPEG-2 PID variable. */
typedef UINT16  MPEG_2_PID_T;    /**<        */

#define MPEG_2_NULL_PID  ((MPEG_2_PID_T) 0x1fff)    /**<        */


/* The following definitions are used to define a MPEG-2 stream id variable. */
typedef UINT8  MPEG_2_STREAM_ID_T;    /**<        */

#define MPEG_2_NULL_STREAM_ID  ((MPEG_2_STREAM_ID_T) 0x00)    /**<        */


/* The following definition is used for ISO-639 language definitions. Note that */
/* such a language string MUST always be zero terminated so that comparisons or */
/* copying will become simpler. Note that the minimum value of ISO_639_LANG_LEN */
/* is '4'. It MUST NOT be set to a lower value.                                 */
#define ISO_639_LANG_LEN  4    /**<        */

typedef CHAR ISO_639_LANG_T [ISO_639_LANG_LEN];    /**<        */

#define ISO_639_DEFAULT  ""    /**<        */


/* The following definition is used for ISO-3166 coutry code definitions. Note that  */
/* such a language string MUST always be zero terminated so that comparisons or      */
/* copying will become simpler. Note that the minimum value of ISO_3166_COUNT_LEN is */
/* '4'. It MUST NOT be set to a lower value.                                         */
#define ISO_3166_COUNT_LEN  4    /**<        */

typedef CHAR ISO_3166_COUNT_T [ISO_3166_COUNT_LEN];    /**<        */

#define ISO_3166_DEFAULT  ""    /**<        */


/* The following structures and definitions are used to convey text style info.  */
/* Structure STYLE_T contains a single style which may be applicable to a single */
/* character or a complete string. Structure STYLE_STR_T contains a style which  */
/* may be applicable only to a few characters within a string. It is pretty      */
/* common to convey an array of STYLE_STR_T elements. However, that is not a     */
/* requirement.                                                                  */
#define STYLE_EFFECT_BOLD          ((UINT16) 0x0001)    /**<        */
#define STYLE_EFFECT_UNDERLINE     ((UINT16) 0x0002)    /**<        */
#define STYLE_EFFECT_SUB_SCRIPT    ((UINT16) 0x0004)    /**<        */
#define STYLE_EFFECT_SUPER_SCRIPT  ((UINT16) 0x0008)    /**<        */

/*----------------------------------------------------------------------------*/
/*! @struct STYLE_T 
 *  @brief  Text style structure
 *  @code
 *  typedef struct _STYLE_T
 * {
 *     UINT16  ui2_style;
 *  } STYLE_T;  
 *  @endcode
 *
 *  @li@c ui2_style        - Text style
 */
/*----------------------------------------------------------------------------*/
typedef struct _STYLE_T
{
    UINT16  ui2_style;
}   STYLE_T;

/*----------------------------------------------------------------------------*/
/*! @struct STYLE_STR_T 
 *  @brief  Text string style
 *  @code
 *  typedef struct _STYLE_STR_T
 * {
 *     UINT32  ui4_first_char;
 *     UINT32  ui4_num_char;
 *
 *     STYLE_T  t_style;
 *  } STYLE_STR_T;  
 *  @endcode
 *
 *  @li@c ui4_first_char          - Index of first character
 *  @li@c ui4_num_char         - Number of character.
 *  @li@c t_style                    - Style of this string.
 */
/*----------------------------------------------------------------------------*/
typedef struct _STYLE_STR_T
{
    UINT32  ui4_first_char;
    UINT32  ui4_num_char;

    STYLE_T  t_style;
}   STYLE_STR_T;


/* The following stream type enumeration and structure is used to identify an     */
/* individual stream component within the whole middleware. Note that the number  */
/* of stream types may never exceed 32 else we have an overflow in the stream     */
/* type bitmask. The values 24 to 31 (inclusive) are reserved for internal usage. */
/*----------------------------------------------------------------------------*/
/*! @enum   STREAM_TYPE_T 
 *  @brief  Stream type
 *  @code
 *  typedef enum
 *  {
 *      ST_UNKNOWN,
 *      ST_AUDIO,
 *      ST_VIDEO,
 *      ST_CLOSED_CAPTION,
 *      ST_ISDB_CAPTION,
 *      ST_ISDB_TEXT,
 *      ST_TELETEXT,
 *      ST_SUBTITLE,
 *      ST_DATA,
 *      ST_BYPASS,
 *      ST_MM_SUBTITLE,
 *      ST_RCRD_STRM
 *  }   STREAM_TYPE_T; 
 *  @endcode
 *   
 *  @li@c ST_UNKNOWN               - Unknown stream type
 *  @li@c ST_AUDIO                     - audio stream
 *  @li@c ST_VIDEO                     - video stream
 *  @li@c ST_CLOSED_CAPTION    - closed caption 
 *  @li@c ST_ISDB_CAPTION        - ISDB caption 
 *  @li@c ST_ISDB_TEXT              - ISDB text
 *  @li@c ST_TELETEXT                 - Teltext
 *  @li@c ST_SUBTITLE                 - Subtitle 
 *  @li@c ST_DATA                       - Data
 *  @li@c ST_BYPASS                    - Bypass
 *  @li@c ST_MM_SUBTITLE            - MM subtitle
 *  @li@c ST_RCRD_STRM             - RCRD stream
 *              
 */
/*----------------------------------------------------------------------------*/
typedef enum
{
    ST_UNKNOWN = -1,  /* Must be set to '-1' else I loose an entry in the bit mask. */
    ST_AUDIO,
    ST_VIDEO,
    ST_CLOSED_CAPTION,
    ST_ISDB_CAPTION,
    ST_ISDB_TEXT,
    ST_TELETEXT,
    ST_SUBTITLE,
    ST_DATA,
    ST_BYPASS,
    ST_MM_SUBTITLE,
    ST_RCRD_STRM
}   STREAM_TYPE_T;

#define ST_MASK_AUDIO           MAKE_BIT_MASK_32 (ST_AUDIO)    /**<        */
#define ST_MASK_VIDEO           MAKE_BIT_MASK_32 (ST_VIDEO)    /**<        */
#define ST_MASK_CLOSED_CAPTION  MAKE_BIT_MASK_32 (ST_CLOSED_CAPTION)    /**<        */
#define ST_MASK_ISDB_CAPTION    MAKE_BIT_MASK_32 (ST_ISDB_CAPTION)    /**<        */
#define ST_MASK_ISDB_TEXT       MAKE_BIT_MASK_32 (ST_ISDB_TEXT)    /**<        */
#define ST_MASK_TELETEXT        MAKE_BIT_MASK_32 (ST_TELETEXT)    /**<        */
#define ST_MASK_SUBTITLE        MAKE_BIT_MASK_32 (ST_SUBTITLE)    /**<        */
#define ST_MASK_DATA            MAKE_BIT_MASK_32 (ST_DATA)    /**<        */
#define ST_MASK_BYPASS          MAKE_BIT_MASK_32 (ST_BYPASS)    /**<        */
#define ST_MASK_MM_SUBTITLE     MAKE_BIT_MASK_32 (ST_MM_SUBTITLE)    /**<        */
#define ST_MASK_RCRD_STRM       MAKE_BIT_MASK_32 (ST_RCRD_STRM)    /**<        */

/*----------------------------------------------------------------------------*/
/*! @struct STREAM_COMP_ID_T 
 *  @brief  Stream component ID 
 *  @code
 *  typedef struct  _STREAM_COMP_ID_T
 * {
 *     STREAM_TYPE_T  e_type;    
 *
 *     VOID*  pv_stream_tag;
 *  } STREAM_COMP_ID_T;  
 *  @endcode
 *
 *  @li@c e_type                    - Specifies audio, video etc. 
 *  @li@c pv_stream_tag        - tag info of stream
 */
/*----------------------------------------------------------------------------*/
typedef struct _STREAM_COMP_ID_T
{
    STREAM_TYPE_T  e_type;     /* Specifies audio, video etc. */

    VOID*  pv_stream_tag;
}   STREAM_COMP_ID_T;


/* The folowing structure is used during initialization to specify */
/* a components thread priority and stack size.                    */
#define DEFAULT_PRIORITY    ((UINT8)  0)    /**<        */
#define DEFAULT_STACK_SIZE  ((SIZE_T) 0)    /**<        */
#define DEFAULT_NUM_MSGS    ((UINT16) 0)    /**<        */

/*----------------------------------------------------------------------------*/
/*! @struct THREAD_DESCR_T 
 *  @brief  Stream description structure
 *  @code
 *  typedef struct _THREAD_DESCR_T
 * {
 *     SIZE_T  z_stack_size;
 *
 *     UINT8  ui1_priority;
 *
 *     UINT16  ui2_num_msgs;
 *  } THREAD_DESCR_T;  
 *  @endcode
 *
 *  @li@c z_stack_size        - Size of stack
 *  @li@c ui1_priority          - Priority of stack
 *  @li@c ui2_num_msgs     - Num of messages
 */
/*----------------------------------------------------------------------------*/
typedef struct _THREAD_DESCR_T
{
    SIZE_T  z_stack_size;

    UINT8  ui1_priority;

    UINT16  ui2_num_msgs;
}   THREAD_DESCR_T;

/* The following structure contains a generic transponder description. */
typedef UINT8  BRDCST_TYPE_T;    /**<        */

#define BRDCST_TYPE_UNKNOWN  ((BRDCST_TYPE_T) 0)    /**<        */
#define BRDCST_TYPE_ANALOG   ((BRDCST_TYPE_T) 1)    /**<        */
#define BRDCST_TYPE_DVB      ((BRDCST_TYPE_T) 2)    /**<        */
#define BRDCST_TYPE_ATSC     ((BRDCST_TYPE_T) 3)    /**<        */
#define BRDCST_TYPE_SCTE     ((BRDCST_TYPE_T) 4)    /**<        */
#define BRDCST_TYPE_ISDB     ((BRDCST_TYPE_T) 5)    /**<        */
#define BRDCST_TYPE_FMRDO    ((BRDCST_TYPE_T) 6)    /**<        */
#define BRDCST_TYPE_DTMB     ((BRDCST_TYPE_T) 7)    /**<        */
#define BRDCST_TYPE_MHP      ((BRDCST_TYPE_T) 8)    /**<        */


typedef UINT8  BRDCST_MEDIUM_T;    /**<        */

#define BRDCST_MEDIUM_UNKNOWN          ((BRDCST_MEDIUM_T) 0)    /**<        */
#define BRDCST_MEDIUM_DIG_TERRESTRIAL  ((BRDCST_MEDIUM_T) 1)    /**<        */
#define BRDCST_MEDIUM_DIG_CABLE        ((BRDCST_MEDIUM_T) 2)    /**<        */
#define BRDCST_MEDIUM_DIG_SATELLITE    ((BRDCST_MEDIUM_T) 3)    /**<        */
#define BRDCST_MEDIUM_ANA_TERRESTRIAL  ((BRDCST_MEDIUM_T) 4)    /**<        */
#define BRDCST_MEDIUM_ANA_CABLE        ((BRDCST_MEDIUM_T) 5)    /**<        */
#define BRDCST_MEDIUM_ANA_SATELLITE    ((BRDCST_MEDIUM_T) 6)    /**<        */
#define BRDCST_MEDIUM_1394             ((BRDCST_MEDIUM_T) 7)    /**<        */


#define TS_DESCR_FLAG_NW_ID_SOFT_DEFINED  ((UINT8) 0x01)    /**<        */
#define TS_DESCR_FLAG_ON_ID_SOFT_DEFINED  ((UINT8) 0x02)    /**<        */
#define TS_DESCR_FLAG_TS_ID_SOFT_DEFINED  ((UINT8) 0x04)    /**<        */

#define TS_DESCR_FLAG_IDS_SOFT_DEFINED  (TS_DESCR_FLAG_NW_ID_SOFT_DEFINED | \
                                         TS_DESCR_FLAG_ON_ID_SOFT_DEFINED | \
                                         TS_DESCR_FLAG_TS_ID_SOFT_DEFINED)    /**<        */

/*! @struct MEDIA_SUBTYPE_T 
 *  @brief  Media subtype structure
 *  @code
 *  typedef struct 
 * {
 *     union
 *    {  
 *        MEDIA_AUD_SUBTYPE_T         e_aud_subtype;
 *        MEDIA_VID_SUBTYPE_T         e_vid_subtype;
 *        MEDIA_IMG_SUBTYPE_T         e_img_subtype;
 *        MEDIA_CONTNR_SUBTYPE_T      e_contnr_subtype;
 *        MEDIA_PS_SUBTYPE_T          e_ps_subtype;
 *        MEDIA_TS_SUBTYPE_T          e_ts_subtype;
 *    }u; 
 *  } MEDIA_SUBTYPE_T;  
 *  @endcode
 *
 *  @li@c e_aud_subtype        - audio subtype
 *  @li@c e_vid_subtype         - video subtype
 *  @li@c e_img_subtype        - image subtype
 *  @li@c e_contnr_subtype    - container subtype
 *  @li@c e_ps_subtype          - PS subtype
 *  @li@c e_ts_subtype           - TS subtype
 */
/*----------------------------------------------------------------------------*/
typedef struct _TS_DESCR_T
{
    BRDCST_TYPE_T    e_bcst_type;
    BRDCST_MEDIUM_T  e_bcst_medium;

    UINT16  ui2_nw_id;     /* Some form of network identifier. */

    UINT16  ui2_on_id;     /* Original network id. */
    UINT16  ui2_ts_id;     /* Transport stream id. */

    UINT16  ui2_res_1;     /* NWL record id value.  See macro below. */

    UINT8   ui1_res_2;     /* NWL value.  See macro below. */

    UINT8  ui1_flags;      /* Some flags. */

}   TS_DESCR_T;

/* macros to set/get Network List and Network List record id value */
/* in the TS_DESCR_T structure.  */
/* Note: Network List value is restricted to between 1 and 127 */
#define TS_DESCR_GET_NWL_ID(_ts_descr_t_) ((UINT16)_ts_descr_t_.ui1_res_2)    /**<        */
#define TS_DESCR_SET_NWL_ID(_ts_descr_t_,_nwl_id_) (_ts_descr_t_.ui1_res_2=(UINT8)(_nwl_id_))    /**<        */
#define TS_DESCR_GET_NWL_REC_ID(_ts_descr_t_) ((UINT16)_ts_descr_t_.ui2_res_1)    /**<        */
#define TS_DESCR_SET_NWL_REC_ID(_ts_descr_t_,_nwl_rec_id_) (_ts_descr_t_.ui2_res_1=(UINT16)(_nwl_rec_id_))    /**<        */

#define TS_DESCR_PTR_GET_NWL_ID(_p_ts_descr_t_) ((UINT16)_p_ts_descr_t_->ui1_res_2)    /**<        */
#define TS_DESCR_PTR_GET_NWL_REC_ID(_p_ts_descr_t_) ((UINT16)_p_ts_descr_t_->ui2_res_1)    /**<        */



/* The following enumeration and bitmasks are used to convey */
/* TV-System type information.                               */

/*----------------------------------------------------------------------------*/
/*! @enum   TV_SYS_T 
 *  @brief  TV system type
 *  @code
 *  typedef enum
 *  {
 *      TV_SYS_UNKNOWN,
 *      TV_SYS_A,
 *      TV_SYS_B,
 *      TV_SYS_C,
 *      TV_SYS_D,
 *      TV_SYS_E,
 *      TV_SYS_F,
 *      TV_SYS_G,
 *      TV_SYS_H,
 *      TV_SYS_I,
 *      TV_SYS_J,
 *      TV_SYS_K,
 *      TV_SYS_K_PRIME,
 *      TV_SYS_L,
 *      TV_SYS_L_PRIME,
 *      TV_SYS_M,
 *      TV_SYS_N
 *  }   TV_SYS_T; 
 *  @endcode
 *   
 *  @li@c TV_SYS_UNKNOWN        - Unknown TV system
 *  @li@c TV_SYS_A                     - System A
 *  @li@c TV_SYS_B                     - System B
 *  @li@c TV_SYS_C                     - System C
 *  @li@c TV_SYS_D                     - System D
 *  @li@c TV_SYS_E                      - System E
 *  @li@c TV_SYS_F                      - System F
 *  @li@c TV_SYS_G                     - System G
 *  @li@c TV_SYS_H                     - System H  
 *  @li@c TV_SYS_I                      - System I   
 *  @li@c TV_SYS_J                      - System J   
 *  @li@c TV_SYS_K                     - System K   
 *  @li@c TV_SYS_K_PRIME          - System K PRIME 
 *  @li@c TV_SYS_L                     - System L  
 *  @li@c TV_SYS_L_PRIME           - System L PRIME  
 *  @li@c TV_SYS_M                     - System M 
 *  @li@c TV_SYS_N                     - System N 
 *              
 */
/*----------------------------------------------------------------------------*/
typedef enum

{
    TV_SYS_UNKNOWN = -1, /* Must be set to '-1' else I loose an entry in the bit mask. */
    TV_SYS_A,
    TV_SYS_B,
    TV_SYS_C,
    TV_SYS_D,
    TV_SYS_E,
    TV_SYS_F,
    TV_SYS_G,
    TV_SYS_H,
    TV_SYS_I,
    TV_SYS_J,
    TV_SYS_K,
    TV_SYS_K_PRIME,
    TV_SYS_L,
    TV_SYS_L_PRIME,
    TV_SYS_M,
    TV_SYS_N,
/*  Entries below are defined in AUD_TV_SYS_T, and used by audio driver
    AUD_TV_SYS_A2 = 16,
    AUD_TV_SYS_PAL = 17,
    AUD_TV_SYS_NICAM = 18,
    AUD_TV_SYS_SECAM = 19 */
    TV_SYS_AUTO = 31
}   TV_SYS_T;

#define TV_SYS_MASK_A        MAKE_BIT_MASK_32 (TV_SYS_A)    /**<        */
#define TV_SYS_MASK_B        MAKE_BIT_MASK_32 (TV_SYS_B)    /**<        */
#define TV_SYS_MASK_C        MAKE_BIT_MASK_32 (TV_SYS_C)    /**<        */
#define TV_SYS_MASK_D        MAKE_BIT_MASK_32 (TV_SYS_D)    /**<        */
#define TV_SYS_MASK_E        MAKE_BIT_MASK_32 (TV_SYS_E)    /**<        */
#define TV_SYS_MASK_F        MAKE_BIT_MASK_32 (TV_SYS_F)    /**<        */
#define TV_SYS_MASK_G        MAKE_BIT_MASK_32 (TV_SYS_G)    /**<        */
#define TV_SYS_MASK_H        MAKE_BIT_MASK_32 (TV_SYS_H)    /**<        */
#define TV_SYS_MASK_I        MAKE_BIT_MASK_32 (TV_SYS_I)    /**<        */
#define TV_SYS_MASK_J        MAKE_BIT_MASK_32 (TV_SYS_J)    /**<        */
#define TV_SYS_MASK_K        MAKE_BIT_MASK_32 (TV_SYS_K)    /**<        */
#define TV_SYS_MASK_K_PRIME  MAKE_BIT_MASK_32 (TV_SYS_K_PRIME)    /**<        */
#define TV_SYS_MASK_L        MAKE_BIT_MASK_32 (TV_SYS_L)    /**<        */
#define TV_SYS_MASK_L_PRIME  MAKE_BIT_MASK_32 (TV_SYS_L_PRIME)    /**<        */
#define TV_SYS_MASK_M        MAKE_BIT_MASK_32 (TV_SYS_M)    /**<        */
#define TV_SYS_MASK_N        MAKE_BIT_MASK_32 (TV_SYS_N)    /**<        */
#define TV_SYS_MASK_AUTO     MAKE_BIT_MASK_32 (TV_SYS_AUTO)    /**<        */

#define TV_SYS_MASK_NONE     ((UINT32) 0)    /**<        */


/* The following enumeration and bitmasks are used to convey */
/* Color-System type information.                            */
/*----------------------------------------------------------------------------*/
/*! @enum   COLOR_SYS_T 
 *  @brief  Color system type
 *  @code
 *  typedef enum
 *  {
 *      COLOR_SYS_UNKNOWN,
 *      COLOR_SYS_NTSC,
 *      COLOR_SYS_PAL,
 *      COLOR_SYS_SECAM,
 *      COLOR_SYS_NTSC_443,
 *      COLOR_SYS_PAL_M,
 *      COLOR_SYS_PAL_N,
 *      COLOR_SYS_PAL_60
 *  }   COLOR_SYS_T; 
 *  @endcode
 *   
 *  @li@c COLOR_SYS_UNKNOWN        - Unknown color system
 *  @li@c COLOR_SYS_NTSC               - NTSC
 *  @li@c COLOR_SYS_PAL                  - PAL
 *  @li@c COLOR_SYS_SECAM             - SECAM
 *  @li@c COLOR_SYS_NTSC_443        - NTSC 443
 *  @li@c COLOR_SYS_PAL_M              - PAL M
 *  @li@c COLOR_SYS_PAL_N              - PAL N
 *  @li@c COLOR_SYS_PAL_60             - PAL 60
 *              
 */
/*----------------------------------------------------------------------------*/
typedef enum
{
    COLOR_SYS_UNKNOWN = -1, /* Must be set to '-1' else I loose an entry in the bit mask. */
    COLOR_SYS_NTSC,
    COLOR_SYS_PAL,
    COLOR_SYS_SECAM,
    COLOR_SYS_NTSC_443,
    COLOR_SYS_PAL_M,
    COLOR_SYS_PAL_N,
    COLOR_SYS_PAL_60
}   COLOR_SYS_T;

#define COLOR_SYS_MASK_NTSC   MAKE_BIT_MASK_16 (COLOR_SYS_NTSC)    /**<        */
#define COLOR_SYS_MASK_PAL    MAKE_BIT_MASK_16 (COLOR_SYS_PAL)    /**<        */
#define COLOR_SYS_MASK_SECAM  MAKE_BIT_MASK_16 (COLOR_SYS_SECAM)    /**<        */

#define COLOR_SYS_MASK_NONE   ((UINT16) 0)    /**<        */


/* The following enumeration and bitmasks are used to convey */
/* Audio-System type information.                            */
/*----------------------------------------------------------------------------*/
/*! @enum   AUDIO_SYS_T 
 *  @brief  Audio system type
 *  @code
 *  typedef enum
 *  {
 *      AUDIO_SYS_UNKNOWN,
 *      AUDIO_SYS_AM,
 *      AUDIO_SYS_FM_MONO,
 *      AUDIO_SYS_FM_EIA_J,
 *      AUDIO_SYS_FM_A2,
 *      AUDIO_SYS_FM_A2_DK1,
 *      AUDIO_SYS_FM_A2_DK2,
 *      AUDIO_SYS_FM_RADIO,
 *      AUDIO_SYS_NICAM,
 *      AUDIO_SYS_BTSC
 *  }   AUDIO_SYS_T; 
 *  @endcode
 *   
 *  @li@c AUDIO_SYS_UNKNOWN        - Unknown audio system
 *  @li@c AUDIO_SYS_AM                  - AM audio
 *  @li@c AUDIO_SYS_FM_MONO        - FM mono
 *  @li@c AUDIO_SYS_FM_EIA_J         - FM EIA_J
 *  @li@c AUDIO_SYS_FM_A2              - FM A2
 *  @li@c AUDIO_SYS_FM_A2_DK1      - FM A2 DK1
 *  @li@c AUDIO_SYS_FM_A2_DK2      - FM A2 DK2
 *  @li@c AUDIO_SYS_FM_RADIO        - FM radio
 *  @li@c AUDIO_SYS_NICAM              - Nicam 
 *  @li@c AUDIO_SYS_BTSC                - BTSC
 *              
 */
/*----------------------------------------------------------------------------*/
typedef enum
{
    AUDIO_SYS_UNKNOWN = -1, /* Must be set to '-1' else I loose an entry in the bit mask. */
    AUDIO_SYS_AM,
    AUDIO_SYS_FM_MONO,
    AUDIO_SYS_FM_EIA_J,
    AUDIO_SYS_FM_A2,
    AUDIO_SYS_FM_A2_DK1,
    AUDIO_SYS_FM_A2_DK2,
    AUDIO_SYS_FM_RADIO,
    AUDIO_SYS_NICAM,
    AUDIO_SYS_BTSC
}   AUDIO_SYS_T;

#define AUDIO_SYS_MASK_AM         MAKE_BIT_MASK_16 (AUDIO_SYS_AM)    /**<        */
#define AUDIO_SYS_MASK_FM_MONO    MAKE_BIT_MASK_16 (AUDIO_SYS_FM_MONO)    /**<        */
#define AUDIO_SYS_MASK_FM_EIA_J   MAKE_BIT_MASK_16 (AUDIO_SYS_FM_EIA_J)    /**<        */
#define AUDIO_SYS_MASK_FM_A2      MAKE_BIT_MASK_16 (AUDIO_SYS_FM_A2)    /**<        */
#define AUDIO_SYS_MASK_FM_A2_DK1  MAKE_BIT_MASK_16 (AUDIO_SYS_FM_A2_DK1)    /**<        */
#define AUDIO_SYS_MASK_FM_A2_DK2  MAKE_BIT_MASK_16 (AUDIO_SYS_FM_A2_DK2)    /**<        */
#define AUDIO_SYS_MASK_FM_RADIO   MAKE_BIT_MASK_16 (AUDIO_SYS_FM_RADIO)    /**<        */
#define AUDIO_SYS_MASK_NICAM      MAKE_BIT_MASK_16 (AUDIO_SYS_NICAM)    /**<        */
#define AUDIO_SYS_MASK_BTSC       MAKE_BIT_MASK_16 (AUDIO_SYS_BTSC)    /**<        */

#define AUDIO_SYS_MASK_NONE   ((UINT16) 0)    /**<        */

/* Data formats */
typedef UINT8  DATA_FMT_T;    /**<        */

#define DATA_FMT_UNKNOWN    ((DATA_FMT_T)  0) /**< Unknown format */
#define DATA_FMT_ANALOG     ((DATA_FMT_T)  1) /**< Analog format */
#define DATA_FMT_MPEG_2     ((DATA_FMT_T)  2) /**< MPEG 2 */
#define DATA_FMT_PES        ((DATA_FMT_T)  3) /**< PES format */
#define DATA_FMT_MP3        ((DATA_FMT_T)  4) /**< MPEG 1 Layer 3 */
#define DATA_FMT_MP2        ((DATA_FMT_T)  5) /**< MPEG 1 Layer 2 */
#define DATA_FMT_AAC        ((DATA_FMT_T)  6) /**< Advanced Audio Coding */
#define DATA_FMT_AUDIO_ES   ((DATA_FMT_T)  7) /**< Audio ES */
#define DATA_FMT_VIDEO_ES   ((DATA_FMT_T)  8) /**< Video ES */
#define DATA_FMT_IBC        ((DATA_FMT_T)  9) /**< In Band Command */

/* Checks if a 32-bit value has a byte equal to zero */
#define HAS_ZERO_BYTE_UINT32(_val) \
    ((((_val)-0x01010101UL) & ~(_val) & 0x80808080UL) != 0)    /**<        */
    
#ifdef DIVX_PLUS_CER
#define SUPPORT_MULTI_INFO    1    /**<        */
#else
#define SUPPORT_MULTI_INFO    0    /**<        */
#endif

#ifdef RPC_ENABLE

#define RPC_ADD_ARG_TYPE        rpc_add_arg_type
#define RPC_ADD_REF_DESC        rpc_add_ref_desc  
#define RPC_ADD_REF_BUFF        rpc_add_ref_buff
#define RPC_ADD_REF_DESC_ARR    rpc_add_ref_desc_arr
#define RPC_ADD_REF_STR         rpc_add_ref_str
#define RPC_ADD_REF_VAL         rpc_add_ref_val
#define RPC_DEL                 rpc_del

#else  /* RPC_ENABLE */

#define RPC_ADD_ARG_TYPE(t_rpc_id, ui4_idx, e_type)
#define RPC_ADD_REF_DESC(t_rpc_id, pv_obj,  pt_desc, ps_field_name)
#define RPC_ADD_REF_BUFF(t_rpc_id, pv_buff, z_size)
#define RPC_ADD_REF_DESC_ARR(t_rpc_id, ui4_num_entries, pv_obj, pt_desc, ps_field_name) 
#define RPC_ADD_REF_STR(t_rpc_id, ps_str)
#define RPC_ADD_REF_VAL(t_rpc_id, pv_ref)         
#define RPC_DEL(t_rpc_id)

#endif /* RPC_ENABLE */

#define MW_EEP_PATH                   (SN_DEV_PATH "/" SN_EEPROM_4)
#define MW_EEP_OFFSET                 ((UINT16)0)
#define MW_EEP_DRM_OFFSET             MW_EEP_OFFSET
#define MW_EEP_DRM_SIZE               ((UINT16)64)
#define MW_EEP_DTV_SRC_OFFSET         MW_EEP_DRM_OFFSET + MW_EEP_DRM_SIZE
#define MW_EEP_DTV_SRC_SIZE           ((UINT16)1)
#define MW_EEP_SVL_LST_OFFSET         MW_EEP_DTV_SRC_OFFSET + MW_EEP_DTV_SRC_SIZE
#define MW_EEP_SVL_LST_SIZE           ((UINT16)1)
#define MW_EEP_LCH_OFFSET             MW_EEP_SVL_LST_OFFSET + MW_EEP_SVL_LST_SIZE
#define MW_EEP_LCH_SIZE               ((UINT16)4)

#endif /* _U_COMMON_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                              /* end of groupMW */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    page definitions
 ---------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*! @page   page_1      Detail Description
 *  @subpage    pageMW_1    "Middleware"
 */
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
/*! @page       pageMW_1                             Middleware
 *  @subpage    pageMW_EVCTX                         "Event Context                "
 *  @subpage    pageMW_FM                            "File Manager                 "
 *  @subpage    pageMW_FONT                          "Font Engine                  "
 *  @subpage    pageMW_IMG                           "Image Lib                    "
 *  @subpage    pageMW_IO_MNGR                       "IO Manager                   "
 *  @subpage    pageMW_MUTIL                         "Multi Media                  "
 *  @subpage    pageMW_MUTIL_MFMTRECG                "Multi Media Format Recognizer"
 *  @subpage    pageMW_MUTIL_MIDXBULD                "Multi Media Index Build      "
 *  @subpage    pageMW_MUTIL_MINFO                   "Media Information            " 
 *  @subpage    pageMW_SCC                           "Stream Component Controller  "
 *  @subpage    pageMW_SVCTX                         "Service Context              "
 *  @subpage    pageMW_MM_HDLR                       "Multi Media Handler          "
 *  @subpage    pageMW_SVL                           "Service List                 "
 *  @subpage    pageMW_SVL_BLDR                      "SVL Builder                  "
 */
/*--------------------------------------------------------------------------------*/




