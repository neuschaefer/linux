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
/*! @file
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *
 *  @par Description:
 *       This header file contains the Bluetooth Manager specific definitions
 *       that are exported.
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_INET_BLUETOOTH Bluetooth Manager
 *  @ingroup  groupMW_INET
 *  @brief    Provides a unified interface for Bluetooth Manager
 *  @see      groupMW_INET
 *
 *  @{
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_BLUETOOTH_H_
#define _U_BLUETOOTH_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define BLUETOOTHR_ASYNC_NFY                 ((INT32)   1)  /**< Async notification */
#define BLUETOOTHR_OK                        ((INT32)   0)  /**< Successful */
#define BLUETOOTHR_NOT_READY                 ((INT32)  -1)  /**< Not ready */
#define BLUETOOTHR_FAIL                      ((INT32)  -2)  /**< Fail */
#define BLUETOOTHR_INV_ARG                   ((INT32)  -3)  /**< Invalid arguments */
#define BLUETOOTHR_NOT_ENOUGH_MEM            ((INT32)  -4)  /**< Not enough memory*/

#define BLUETOOTH_PROFILE_CAP_HID                           MAKE_BIT_MASK_32(0)
#define BLUETOOTH_PROFILE_CAP_GENERAL_AUDIO                 MAKE_BIT_MASK_32(1)
#define BLUETOOTH_PROFILE_CAP_AVRCP                         MAKE_BIT_MASK_32(2)

#define BLUETOOTH_MAX_STR_LEN                                             (128)
#define BLUETOOTH_MAX_PINCODE_LEN                                          (16)

typedef UINT32 BLUETOOTH_NFY_REASON_T;

/* scan related */
#define BLUETOOTH_NFY_REASON_SCAN_DEVICE_FOUND         ((BLUETOOTH_NFY_REASON_T)  0)
#define BLUETOOTH_NFY_REASON_SCAN_COMPLETED            ((BLUETOOTH_NFY_REASON_T)  1)
#define BLUETOOTH_NFY_REASON_SCAN_FAILED               ((BLUETOOTH_NFY_REASON_T)  2)
#define BLUETOOTH_NFY_REASON_SCAN_ABORTED              ((BLUETOOTH_NFY_REASON_T)  3)

/* pairing related */
#define BLUETOOTH_NFY_REASON_PAIR_COMPLETED            ((BLUETOOTH_NFY_REASON_T)  4)  /**< Pair is completed */
#define BLUETOOTH_NFY_REASON_PAIR_AUTH_TIMEOUT         ((BLUETOOTH_NFY_REASON_T)  5)  /**< Authenication is timeout */
#define BLUETOOTH_NFY_REASON_PAIR_AUTH_REJECTED        ((BLUETOOTH_NFY_REASON_T)  6)  /**< Authenication is rejected by remote device */
#define BLUETOOTH_NFY_REASON_PAIR_AUTH_CANCELED        ((BLUETOOTH_NFY_REASON_T)  7)  /**< Authenication is canceled by ourself */
#define BLUETOOTH_NFY_REASON_PAIR_AUTH_FAILED          ((BLUETOOTH_NFY_REASON_T)  8)  /**< Authenication is failed, ex. pincode is incorrect */
#define BLUETOOTH_NFY_REASON_PAIR_ALREADY_PAIRED       ((BLUETOOTH_NFY_REASON_T)  9)  /**< This device is already paired */
#define BLUETOOTH_NFY_REASON_PAIR_CONN_FAILED          ((BLUETOOTH_NFY_REASON_T) 10)  /**< Connection is failed, ex. the remote device is shutdown */
#define BLUETOOTH_NFY_REASON_PAIR_FAILED_UNKNOWN       ((BLUETOOTH_NFY_REASON_T) 11)  /**< Unknown reason */

typedef struct _BLUETOOTH_DEVICE_PROPERTY_T
{
    CHAR           s_name[BLUETOOTH_MAX_STR_LEN];
    CHAR           s_address[BLUETOOTH_MAX_STR_LEN];
    UINT32         ui4_profile_cap;
    BOOL           b_paired;
    BOOL           b_connected;
} BLUETOOTH_DEVICE_PROPERTY_T;


typedef struct _BLUETOOTH_PINCODE_T
{
    CHAR           s_pincode[BLUETOOTH_MAX_PINCODE_LEN];
} BLUETOOTH_PINCODE_T;

typedef VOID (*x_bluetooth_scan_nfy_fct)(BLUETOOTH_NFY_REASON_T    t_nfy_reason,
                                               VOID*                     pv_nfy_data,
                                               VOID*                     pv_nfy_tag);

typedef VOID (*x_bluetooth_pair_nfy_fct)(BLUETOOTH_NFY_REASON_T    t_nfy_reason,
                                               VOID*                     pv_nfy_data,
                                               VOID*                     pv_nfy_tag);

/*! @} */

#endif /*  _U_BLUETOOTH_H_ */

