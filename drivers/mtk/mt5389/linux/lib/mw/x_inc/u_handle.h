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
/*! @file u_handle.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains handle specific definitions, which are
 *         exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_HANDLE Handle Module
 *
 *  @ingroup groupMW
 *  @brief Provides external function to alloc/free/get handles
 *
 *  %MW Application and middleware all will use this module to alloc and free and get handles.
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


#ifndef _U_HANDLE_H_
#define _U_HANDLE_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Specify handle data types */
typedef UINT32  HANDLE_T;    /**<  handle defination      */
typedef UINT16  HANDLE_TYPE_T;    /**<  type of handle     */

#define NULL_HANDLE  ((HANDLE_T) 0)           /**< Value of empty handle */

#define INV_HANDLE_TYPE  ((HANDLE_TYPE_T) 0)  /**< Value of invalid handle type */

/* Handle API return values */
#define HR_OK                   ((INT32)   0) /**< Call handle function ok */
#define HR_INV_ARG              ((INT32)  -1) /**< Input argment is invalid */
#define HR_INV_HANDLE           ((INT32)  -2) /**< Input handle is invalid */
#define HR_OUT_OF_HANDLES       ((INT32)  -3) /**< Input handle is outof handle array */
#define HR_NOT_ENOUGH_MEM       ((INT32)  -4) /**< Memory is not enough */
#define HR_ALREADY_INIT         ((INT32)  -5) /**< Handle is already inited */
#define HR_NOT_INIT             ((INT32)  -6) /**< Handle is not inited */
#define HR_RECURSION_ERROR      ((INT32)  -7) /**< Handle is already in freeing */
#define HR_NOT_ALLOWED          ((INT32)  -8) /**< Handle operation is not allowed */
#define HR_ALREADY_LINKED       ((INT32)  -9) /**< Handle is already linked in list */
#define HR_NOT_LINKED           ((INT32) -10) /**< Handle is not linked in list */
#define HR_FREE_NOT_ALLOWED     ((INT32) -11) /**< Handle is not allowed to free */
#define HR_INV_AUX_HEAD         ((INT32) -12) /**< Handle auxiliary head is invalid */
#define HR_INV_HANDLE_TYPE      ((INT32) -13) /**< Handle type is invalid */
#define HR_CANNOT_REG_WITH_CLI  ((INT32) -14) /**< Register handle debug module failed */


#endif /* _U_HANDLE_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                              /* end of groupMW_HANDLE */
/*----------------------------------------------------------------------------*/

