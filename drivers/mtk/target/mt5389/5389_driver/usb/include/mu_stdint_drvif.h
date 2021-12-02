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

/*
 * MUSBStack-S Keil/8051 C'99 definitions.
 * $Revision: #1 $
 */

#ifndef __C8051_STDINT_H__
#define __C8051_STDINT_H__

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#ifdef MUSB_8226
    #include "usb_debug.h"
    #include "x_hal_5371.h"
    #include "x_hal_926.h"
    #include "bim_8225.h"
#else /* 53xx */
    #include "x_hal_5381.h"
    #include "x_hal_arm.h"
    #include "x_os.h"
#endif /* #ifdef MUSB_8226 */
#include "x_typedef.h"
#include "x_assert.h"
#include "x_serial.h"
#include "x_bim.h"
#include "x_printf.h"

LINT_EXT_HEADER_END

/* 7.18.1.1 */

#ifndef __linux__
/* exact-width signed integer types */
#ifdef MUSB_8226
    typedef INT8			int8_t;
#else /* 53xx */
    typedef CHAR			int8_t;
#endif /* #ifdef MUSB_8226 */

typedef INT16			int16_t;
typedef INT32			int32_t;

/* exact-width unsigned integer types */
typedef UINT8		        uint8_t;
typedef UINT16		uint16_t;
typedef UINT32		uint32_t;
//  MTK Notice: Max Liao, 2006/11/20.
//  support disk size > 4 Gbytes.
typedef UINT64		uint64_t;
typedef INT64		        int64_t;
#endif

/* 7.18.1.2 */

/* smallest type of at least n bits */
/* minimum-width signed integer types */
typedef   int8_t		int_least8_t;
typedef   int16_t		int_least16_t;
typedef   int32_t		int_least32_t;

/* minimum-width unsigned integer types */
typedef uint8_t			uint_least8_t;
typedef uint16_t		uint_least16_t;
typedef uint32_t		uint_least32_t;

/* 7.18.1.3 */

/* fastest minimum-width signed integer types */
typedef   int8_t		int_fast8_t;
typedef   int16_t		int_fast16_t;
typedef   int32_t		int_fast32_t;

    /* fastest minimum-width unsigned integer types */
typedef uint8_t			uint_fast8_t;
typedef uint16_t		uint_fast16_t;
typedef uint32_t		uint_fast32_t;

/* 7.18.1.4 integer types capable of holding object pointers */
typedef int32_t			intptr_t;
//typedef uint32_t		uintptr_t;

/* 7.18.1.5 greatest-width integer types */
typedef int32_t			intmax_t;
typedef uint32_t		uintmax_t;

/* 7.18.2.1 */
#ifdef MUSB_8226

    typedef UINT32  HANDLE_T;
    typedef VOID (*x_os_timer_cb_fct) (HANDLE_T  pt_tm_handle,
                                       VOID*     pv_tag);

    typedef VOID (*x_os_isr_fct) (UINT16  ui2_vector_id);

    #define OSR_OK                    ((INT32)   0)

    typedef enum
    {
        X_TIMER_FLAG_ONCE   = 1,
        X_TIMER_FLAG_REPEAT
    }   TIMER_FLAG_T;

#endif /* #ifdef MUSB_8226 */

/* minimum values of exact-width signed integer types */

#define INT8_MIN                   -128
#define INT16_MIN                -32768
#define INT32_MIN          (~0x7fffffff)   /* -2147483648 is unsigned */

/* maximum values of exact-width signed integer types */
#define INT8_MAX                    127
#define INT16_MAX                 32767
#define INT32_MAX            2147483647

/* maximum values of exact-width unsigned integer types */
#define UINT8_MAX                   255
#define UINT16_MAX                65535
#define UINT32_MAX           4294967295u

/* 7.18.2.2 */

/* minimum values of minimum-width signed integer types */
#define INT_LEAST8_MIN                   -128
#define INT_LEAST16_MIN                -32768
#define INT_LEAST32_MIN          (~0x7fffffff)

/* maximum values of minimum-width signed integer types */
#define INT_LEAST8_MAX                    127
#define INT_LEAST16_MAX                 32767
#define INT_LEAST32_MAX            2147483647

/* maximum values of minimum-width unsigned integer types */
#define UINT_LEAST8_MAX                   255
#define UINT_LEAST16_MAX                65535
#define UINT_LEAST32_MAX           4294967295u

/* 7.18.2.3 */

/* minimum values of fastest minimum-width signed integer types */
#define INT_FAST8_MIN           (~0x7fffffff)
#define INT_FAST16_MIN          (~0x7fffffff)
#define INT_FAST32_MIN          (~0x7fffffff)

/* maximum values of fastest minimum-width signed integer types */
#define INT_FAST8_MAX             2147483647
#define INT_FAST16_MAX            2147483647
#define INT_FAST32_MAX            2147483647

/* maximum values of fastest minimum-width unsigned integer types */
#define UINT_FAST8_MAX            4294967295u
#define UINT_FAST16_MAX           4294967295u
#define UINT_FAST32_MAX           4294967295u

/* 7.18.2.4 */

/* minimum value of pointer-holding signed integer type */
#define INTPTR_MIN (~0x7fffffff)

/* maximum value of pointer-holding signed integer type */
#define INTPTR_MAX   2147483647

/* maximum value of pointer-holding unsigned integer type */
#define UINTPTR_MAX  4294967295u

/* 7.18.2.5 */

/* minimum value of greatest-width signed integer type */
#define INTMAX_MIN  ((int64_t)~0x7fffffffffffffff)

/* maximum value of greatest-width signed integer type */
#define INTMAX_MAX   ((int64_t)9223372036854775807)

/* maximum value of greatest-width unsigned integer type */
#define UINTMAX_MAX ((uint64_t)18446744073709551615)

#endif	/* multiple inclusion protection */
