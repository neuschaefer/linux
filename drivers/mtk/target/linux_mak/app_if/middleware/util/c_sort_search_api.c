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
/*-----------------------------------------------------------------------------
 * $RCSfile: c_sort_search_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains implementation of sorting and searching
 *         functions (qsort, binary, etc) for applications.
 *        
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  include files
  --------------------------------------------------------------------------*/

#include "util/x_sort_search.h"


/*-----------------------------------------------------------------------------
 * Name: c_qsort 
 *
 * Description: This API perform quicksort operation.
 *
 * Inputs:
 *   pv_array:        Input array to be sorted.
 *   z_num_elem:      Number of element in the 'pv_array'.
 *   z_size_of_elem:  The size of individual element in the 'pv_array'.
 *   pf_cmp_fct:      Compare function to compare two elements in 'pv_array'.
 *   pv_opt_param:    Optional parameters pass to the compare function.
 *
 * Outputs:
 *   pv_array:        Array with elements sorted.
 *
 * Returns:
 *   SSR_OK        Success.
 *   SSR_INV_ARG   pv_array is NULL, or z_num_elem is 0, or z_size_of_elem
 *                 is 0, or pf_cmp_fct is NULL.
 *   SSR_OUT_OF_MEM  Can not allocate internal memory for the sorting function.
 ----------------------------------------------------------------------------*/
INT32 c_qsort
(
    VOID*            pv_array,
    SIZE_T           z_num_elem,
    SIZE_T           z_size_of_elem,
    x_sort_cmp_fct   pf_cmp_fct,
    VOID*            pv_opt_param
)
{
    return x_qsort(pv_array, z_num_elem, z_size_of_elem,
                   pf_cmp_fct, pv_opt_param);
}


/*-----------------------------------------------------------------------------
 * Name: c_binary_search
 *
 * Description: This API perform perform a recursive search on sorted array.
 *              If the input list, 'pv_array' is not sorted or is not sorted
 *              using the same key parameter as for searching, then this API
 *              will failed.
 *
 * Inputs: - pv_array:         Address of the array containing sorted elements.
 *                            The sorted elements can be an array of sorted
 *                            indices into a array object.
 *           z_num_elem:      Count of element in the 'pv_array' array.
 *           z_size_of_elem:  Size of element in the 'pv_array' array.
 *           pf_cmp_fct:      A function that compare the element of
 *                            'pv_array' array and the search parameter
 *                            value. Note: if the 'pv_array' array
 *                            contains indices to the object array, the
 *                            comparison is performed between the 'actual'
 *                            object and the search key parameter..
 *           pv_search_param: Parameter used to search the object array.
  *           i4_idx          Specifies the index into element in 
 *                            the 'pv_array' with the same 'pv_search_key' 
 *                            value to be returned from the search operation.
 *           pv_opt_param:    Pointer to the optional parameter passed to
 *                            the compare function.
 *
 * Outputs: - pi4_result:     Variable to store the index in element
 *                            in 'pv_array' array that match the search
 *                            parameter.
 *
 * Returns: -
 *   SSR_OK        Success.
 *   SSR_INV_ARG   pv_array is NULL, or z_num_elem is 0, or z_size_of_elem
 *                 is 0, or pf_cmp_fct is NULL.
 *   SSR_OUT_OF_MEM  Can not allocate internal memory for the sorting function.
 ----------------------------------------------------------------------------*/
INT32 c_binary_search
(
    const VOID*      pv_array,
    SIZE_T           z_num_elem,
    SIZE_T           z_size_of_elem,
    x_search_cmp_fct pf_cmp_fct,
    const VOID*      pv_search_param,
    INT32            i4_idx,
    VOID*            pv_opt_param,
    INT32*           pi4_result
)
{
    return x_binary_search(pv_array, z_num_elem, z_size_of_elem,
                           pf_cmp_fct, pv_search_param, i4_idx, pv_opt_param,
                           pi4_result);
} 

