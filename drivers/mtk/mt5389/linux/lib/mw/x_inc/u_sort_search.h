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
/*! @file u_sort_search.h
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * @par Description: 
 *         This header file contains shared typedef and return codes for
 *         sorting and searching function API.
 *         
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_UTIL
  *  @{  
  */
/*----------------------------------------------------------------------------*/ 

#ifndef _U_SORT_SEARCH_H_
#define _U_SORT_SEARCH_H_

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "u_common.h"

/*--------------------------------------------------------------------------
 Constants, enumerations, and macros
---------------------------------------------------------------------------*/
    
#define SSR_OK           ((INT32)   0)      /**<Routine successful*/
#define SSR_INV_ARG      ((INT32)  -1)      /**<One or more of the arguments contain invalid data.*/
#define SSR_OUT_OF_MEM   ((INT32)  -2)      /**<There is no enough memory*/
#define SSR_NOT_FOUND    ((INT32)  -3)      /**<The specified record can not be found.*/
#define SSR_FAIL         ((INT32)  -4)      /**<Routine fail*/


/*
  return code for the sorting comparison function and for searching
  comparison function.
*/
#define  RC_CMP_FCT_T  INT32                /**<Definition of the type of return code for the sorting comparision function*/

#define   RC_SMALLER  ((INT32)-1)           /**< return code of smaller*/
#define   RC_EQUAL    ((INT32)0)            /**<return code of equal*/
#define   RC_GREATER  ((INT32)1)            /**<return code of greater*/
#define   RC_ERROR    ((INT32)-100)         /**<return code of error*/

/*---------------------------------------------------------------------------
 Type definition
----------------------------------------------------------------------------*/

/* 
  Typedef for the sort compare function. This function is passed
  to the x_qsort function.
   
*/
/*------------------------------------------------------------------*/
/*! @brief  This type defines for the sort compare function. This function is passed to the x_qsort function.
*  @param [in] pv_elem_a        Indicates the first element to be compared.
*  @param [in] pv_elem_b        Indicates the second element to be compared.
*  @param [in] pv_opt_param   Reference the optional parameter.
*/
/*------------------------------------------------------------------*/
typedef INT32 (*x_sort_cmp_fct)
(
    const VOID*  pv_elem_a,
    const VOID*  pv_elem_b,
    VOID*        pv_opt_param
);

/* Typedef for the search compare function. This function is to be passed
   to the binary search function to compare the search parameters and search
   field in the element in the sorted array.
*/ 
/*------------------------------------------------------------------*/
/*! @brief  This type defines for the search compare function. This function is to be passed
*   to the binary search function to compare the search parameters and search
*   field in the element in the sorted array.
*  @param [in] pv_elem_a            Indicates the first element to be compared.
*  @param [in] pv_search_param  Indicates the parameter used to search the object array.
*  @param [in] pv_opt_param       Reference the optional parameter.
*/
/*------------------------------------------------------------------*/
typedef INT32 (*x_search_cmp_fct) 
(
    const VOID*  pv_elem_a,
    const VOID*  pv_search_param,
    VOID*        pv_opt_param
);

#endif /* _U_SORT_SEARCH_H_ */


/*----------------------------------------------------------------------------*/
/*! @} */                                               /* end of groupMW_UTIL*/
/*----------------------------------------------------------------------------*/


