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
/*! @file u_time_msrt.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_TIMEMEASUREMENT Time Measurement
 *
 *  @ingroup groupMW
 *  @brief Provides constant,type definitions and function of the  time measurement
 *   ..  Time measurement is a middleware component that is responsible for measure time
 *
 *  %MW Application API would be used by TV Application, lot of these APIs 
 *  would
 *   be wrapped from Exported API
 *
 *  @see groupMW
 *  @{
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_TIME_MSRT_H_
#define _U_TIME_MSRT_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#ifdef LINUX_TURNKEY_SOLUTION
#include "u_common.h"
#else
#include "u_common.h"
#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

#define TMS_LEVEL_NONE          ((UINT16) 0)    /**<default real time measurement         */
#define TMS_LEVEL_REAL_TIME     MAKE_BIT_MASK_16(0)    /**<real time measurement        */
#define TMS_LEVEL_OFF_LINE      MAKE_BIT_MASK_16(1)    /**<off line time measurement        */

#define TMS_NAME_MAX_LENGTH     ((SIZE_T) 31)    /**<max name length        */
#define TMS_COMMENT_MAX_LENGTH  ((SIZE_T) 15)    /**<max comment length        */
#define TMS_REC_CNT_MAX         ((UINT16) 256)    /**<max record number        */
#define TMS_SPOT_REC_CNT_MAX    ((UINT16) 32)    /**<max spot record count        */

#define TMS_FLAG_NONE           ((UINT32) 0)    /**<default flag        */
#define TMS_FLAG_BOOT           MAKE_BIT_MASK_32(0)    /**<        */
#define TMS_FLAG_CHG_CHL        MAKE_BIT_MASK_32(1)    /**<        */
#define TMS_FLAG_AUTO_SCN       MAKE_BIT_MASK_32(2)    /**<        */
#define TMS_FLAG_INP_CHG        MAKE_BIT_MASK_32(3)    /**<        */

/* Common macros to perform CLI controlled time measurement statements. */
/* Note that an individual SW Module MUST set the macro                 */
/* TMS_MODULE_LEVEL else a compile error will occur.                    */
#undef TMS_SPOT
#undef TMS_BEGIN
#undef TMS_END


#define TMS_SPOT( _name )                                                     \
    TIME_MSRT_SPOT(                                                           \
        TMS_MODULE_LEVEL,                                                     \
        _name)    /**<  TMS_SPOT macro records a spot of time-stamp      */

/* TMS_DIFF macro records the differences of time-stamps */
#define TMS_DIFF( _name )                                                     \
    TIME_MSRT_DIFF(                                                           \
        TMS_MODULE_LEVEL,                                                     \
        _name)    /**< TMS_DIFF macro records the differences of time-stamps        */

/* TMS_BEGIN macro record the begin time-stamp */
#define TMS_BEGIN( _name )                                                    \
    TIME_MSRT_BEGIN(                                                          \
        TMS_MODULE_LEVEL,                                                     \
        _name)    /**<  TMS_BEGIN macro record the begin time-stamp      */

/* TMS_END macro record the end time-stamp */
#define TMS_END( _name )                                                      \
    TIME_MSRT_END(                                                            \
        TMS_MODULE_LEVEL,                                                     \
        _name)    /**<  TMS_END macro record the end time-stamp      */

/* TMS_SPOT macro records a spot of time-stamp */
#define TMS_SPOT_EX( _flag, _name, _comment )                                 \
    TIME_MSRT_SPOT_EX(                                                        \
        TMS_MODULE_LEVEL,                                                     \
        _flag,                                                                \
        _name,                                                                \
        _comment)    /**< TMS_SPOT macro records a spot of time-stamp        */

/* TMS_DIFF macro records the differences of time-stamps */
#define TMS_DIFF_EX( _flag, _name, _comment )                                 \
    TIME_MSRT_DIFF_EX(                                                        \
        TMS_MODULE_LEVEL,                                                     \
        _flag,                                                                \
        _name,                                                                \
        _comment)    /**< TMS_DIFF macro records the differences of time-stamps        */

/* TMS_BEGIN macro record the begin time-stamp */
#define TMS_BEGIN_EX( _flag, _name, _comment )                                \
    TIME_MSRT_BEGIN_EX(                                                       \
        TMS_MODULE_LEVEL,                                                     \
        _flag,                                                                \
        _name,                                                                \
        _comment)    /**< TMS_BEGIN macro record the begin time-stamp       */

/* TMS_END macro record the end time-stamp */
#define TMS_END_EX( _flag, _name, _comment )                                  \
    TIME_MSRT_END_EX(                                                         \
        TMS_MODULE_LEVEL,                                                     \
        _flag,                                                                \
        _name,                                                                \
        _comment)    /**< TMS_END macro record the end time-stamp       */

#define TMSR_OK                         ((INT32)   0)    /**<  return value ok      */
#define TMSR_FAIL                       ((INT32)   -1)    /**< return failed       */
#define TMSR_NOT_INIT                   ((INT32)   -2)    /**< not init       */
#define TMSR_INSUFFICIENT_MEMORY        ((INT32)   -3)    /**< insufficient memory       */



/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/


#endif /* _U_TIME_MSRT_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_TIMEMEASUREMENT*/
/*----------------------------------------------------------------------------*/

