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
 * $RCSfile: sort_search.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains sorting and searching functions (qsort,
 *         binary, etc)  implementations.
 *        
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  include files
  --------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "util/x_sort_search.h"


/*-----------------------------------------------------------------------------
  static function prototypes
  ----------------------------------------------------------------------------*/
static
VOID s_quicksort
(
    VOID*               pv_base,
    INT32               i4_P,
    INT32               i4_R,
    SIZE_T              z_size_of_elem,
    x_sort_cmp_fct      pf_compare,
    UINT8*              pui1_pivot,
    UINT8*              pui1_buffer,
    VOID*               pv_base_object_array
);

static
INT32  s_partition
(
    UINT8*              pui1_base,
    INT32               i4_P,
    INT32               i4_R,
    SIZE_T              z_size_of_elem,
    x_sort_cmp_fct      pf_compare,
    UINT8*              pui1_pivot,
    UINT8*              pui1_temp_buf,
    VOID*               pv_base_object_array
);


static INT32 s_equal_partition
(
    UINT8*              pui1_base,
    INT32               i4_P,
    INT32               i4_R,
    SIZE_T              z_size_of_elem,
    x_sort_cmp_fct      pf_cmp_fct,
    UINT8*              pui1_pivot,
    UINT8*              pui1_temp_buf,
    VOID*               pv_opt_param
);


static
INT32 s_binary_find
(
    const UINT8*        pui1_base,
    INT32               i4_P,
    INT32               i4_R,
    SIZE_T              z_num_elem,
    SIZE_T              z_size_of_elem,
    x_search_cmp_fct    pf_compare,
    const VOID*         pv_search_key,
    VOID*               pv_object_array,
    INT32*              pi4_result,
    INT32*              pi4_lower_idx,
    INT32*              pi4_upper_idx
);

#define _SWAP_ELEM_(_a_base_,_a_pos_,_b_pos_,_sz_of_elem_,_tmp_buf_) \
    do { x_memcpy((VOID*)(_tmp_buf_),(VOID*)(_a_base_+(_a_pos_*_sz_of_elem_)),_sz_of_elem_); \
        x_memcpy((VOID*)(_a_base_+(_a_pos_*_sz_of_elem_)),(VOID*)(_a_base_+(_b_pos_*_sz_of_elem_)),_sz_of_elem_); \
        x_memcpy((VOID*)(_a_base_+(_b_pos_*_sz_of_elem_)),(VOID*)(_tmp_buf_),_sz_of_elem_); \
  } while (0)

/*
  size of the local memory buffer size, if greater than this size we
  will do heap-memory allocation.
*/
#define _MEM_BUF_LOCAL_STACK_SIZE_     32

#ifdef DEBUG
/*
  variable to keep track the number of recursion.
*/
static INT32   i4_nb_recursion = 0 ;

static INT32   i4_nb_max_recur = 0 ;

INT32  x_get_qsort_recur(VOID)
{
    return i4_nb_max_recur;
}
#endif  /* DEBUG : determine the depth of recursion */

/*-----------------------------------------------------------------------------
 * Name: x_qsort 
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
 *   SSR__OUT_OF_MEM  Can allocate internal memory for the sorting function.
 ----------------------------------------------------------------------------*/
INT32 x_qsort
(
    VOID*            pv_array,
    SIZE_T           z_num_elem,
    SIZE_T           z_size_of_elem,
    x_sort_cmp_fct   pf_cmp_fct,
    VOID*            pv_opt_param
)
{
    UINT8*   pui1_buffer;
    UINT8*   pui1_pivot;
    UINT8    a_buffer[_MEM_BUF_LOCAL_STACK_SIZE_];
    UINT8    a_pivot[_MEM_BUF_LOCAL_STACK_SIZE_];

#ifdef  DEBUG    
    /* Initialize the number of recursion count to 0 */
    i4_nb_recursion=0;
    i4_nb_max_recur=0;
#endif  /* DEBUG for determine the level of recursion. */    
    
    if ( (pv_array != NULL) && (z_num_elem > 0) && (z_size_of_elem > 0) &&
         (pf_cmp_fct != (x_sort_cmp_fct) NULL) )
    {
        if ( z_size_of_elem <= _MEM_BUF_LOCAL_STACK_SIZE_ )
        {
            /*
              Optimization:

              If the size of the element is less than or equal to
              _MEM_BUF_LOCAL_STACK_SIZE_ bytes, then use memory on the
              stack, avoid doing memory allocation.
            */
            pui1_buffer = a_buffer;
            pui1_pivot  = a_pivot;
        }
        else
        {
            /*
               Allocate temporary buffers to hold a single element of the input
               base_array.  This is needed for exchange of two elements in the
               array and for saving pivot value.
            */
            pui1_buffer = x_mem_alloc(z_size_of_elem);
            pui1_pivot  = x_mem_alloc(z_size_of_elem);
        }
        
        if ( pui1_buffer != NULL && pui1_pivot != NULL )
        {
            /*
              For array less than 2, we don't really need to do qsort.
            */
            switch(z_num_elem)
            {
                case 1 :
                    break;

                case 2:
                {                    
                    if ( pf_cmp_fct(pv_array,
                                    ((UINT8*)pv_array+z_size_of_elem),
                                    pv_opt_param) == RC_GREATER )
                    {
                        /* swap elem 0 and elem 1. */
                        _SWAP_ELEM_((UINT8*)pv_array,0,1,z_size_of_elem,pui1_buffer);
                    }
                }
                break;
                    
                default:
                {
                    s_quicksort(pv_array,
                                (INT32) 0,
                                (INT32) (z_num_elem-1),
                                z_size_of_elem,
                                pf_cmp_fct,
                                pui1_pivot,
                                pui1_buffer,pv_opt_param
                        );
                }
                break;
            }

            /*
              Don't forget to free the memory if they were allocated.
            */
            if ( z_size_of_elem > _MEM_BUF_LOCAL_STACK_SIZE_ )
            {
                x_mem_free(pui1_buffer);
                x_mem_free(pui1_pivot);
            }
        }
        else
        {
            if(pui1_buffer)
            {
                x_mem_free(pui1_buffer);
                pui1_buffer = NULL;
            }
            if(pui1_pivot)
            {
                x_mem_free(pui1_pivot);
                pui1_pivot = NULL;
            }
            return SSR_OUT_OF_MEM;
        }
    }
    else
    {
        return SSR_INV_ARG ;
    }
    return SSR_OK;
} 


/*-----------------------------------------------------------------------------
 * Name: s_quicksort
 *
 * Description: Internal API to perform quicksort algorithm for numeric array.
 *              or array contain references (indices) to a data records.
 * Inputs: 
 *   pv_array:        Input array to be sorted.
 *   i4_P:            Starting index of 'pv_array' to be sorted.
 *   i4_R:            Ending index of 'pv_array' to be sorted.
 *   z_size_of_elem:  The size of individual element in the 'pv_array'.
 *   pf_cmp_fct:      Compare function to compare two elements in 'pv_array'.
 *   pui1_pivot:      Pointer to the memory buffer to store the pivot value.
 *   pui1_buffer:     Pointer to an memory buffer to swapping 2 elements.
 *   pv_opt_param:    Optional parameters pass to the compare function.
 *
 * Outputs:
 *   pv_array         Array with elements partitioned.
 *
 * Returns: VOID 
 *            
 ----------------------------------------------------------------------------*/
static VOID s_quicksort
(
    VOID*               pv_array,
    INT32               i4_P,
    INT32               i4_R,
    SIZE_T              z_size_of_elem,
    x_sort_cmp_fct      pf_cmp_fct,
    UINT8*              pui1_pivot,
    UINT8*              pui1_buffer,
    VOID*               pv_opt_param
)
{
    INT32 i4_Q;

#ifdef DEBUG    
    /*
      Increment the number of recursion each time this function
      is called.
    */
    i4_nb_recursion++;
#endif
    
    /*
      improve version of qsort, remove the tail recursion.
      worst cast depth is ln(N).
    */
    while ( i4_P < i4_R )
    {
        i4_Q=s_equal_partition(pv_array, i4_P, i4_R,
                         z_size_of_elem, pf_cmp_fct,
                         pui1_pivot, pui1_buffer,
                         pv_opt_param);
        if ( (i4_Q - i4_P + 1 ) <= (i4_R - i4_Q) )
        {            
            if ( i4_P < i4_Q )
            {
                s_quicksort(pv_array, i4_P, i4_Q, z_size_of_elem,
                            pf_cmp_fct,pui1_pivot,pui1_buffer,
                            pv_opt_param);
            }
            i4_P = i4_Q + 1 ;            
        }
        else
        {
            if ( i4_Q+1 < i4_R )
            {
                s_quicksort(pv_array, i4_Q+1, i4_R, z_size_of_elem,
                        pf_cmp_fct,pui1_pivot, pui1_buffer,
                        pv_opt_param);
            }
            i4_R = i4_Q ;
        }      
    }    
    
#if 0
    /*
      recursive version of qsort.
    */
    if ( i4_P < i4_R )
    {
        i4_Q=s_equal_partition(pv_array, i4_P, i4_R,
                         z_size_of_elem, pf_cmp_fct,
                         pui1_pivot, pui1_buffer,
                         pv_opt_param);

        s_quicksort(pv_array, i4_P, i4_Q, z_size_of_elem,
                        pf_cmp_fct,pui1_pivot,pui1_buffer,
                        pv_opt_param);

        s_quicksort(pv_array, i4_Q+1, i4_R, z_size_of_elem,
                        pf_cmp_fct,pui1_pivot, pui1_buffer,
                        pv_opt_param);
        
    }
#endif

#ifdef DEBUG    
    if ( i4_nb_recursion > i4_nb_max_recur )
    {
        i4_nb_max_recur = i4_nb_recursion ;
    }
    i4_nb_recursion--;
#endif    
}

/*-----------------------------------------------------------------------------
 * Name: s_equal_partition
 *
 * Description: Internal API to use a middle point for
 *              partition the input array.
 *
 * Inputs:  -
 *   pui1_base  Array containing elements to be sorted.
 *   i4_P    start index in the pui1_base array.
 *   i4_R    end index in the pui1_base array.
 *   z_size_of_elem:  The size of individual element in the 'pv_array'.
 *   pf_cmp_fct:      Compare function to compare two elements in 'pv_array'.
 *   pui1_pivot:      Pointer to the memory buffer to store the pivot value.
 *   pui1_temp_buffer Pointer to an memory buffer to swapping 2 elements.
 *   pv_opt_param:    Optional parameters pass to the compare function.
 *
 * Outputs: 
 *
 * Returns: index to split the 'pui1_base' array. 
 *           
 ----------------------------------------------------------------------------*/
static INT32 s_equal_partition
(
    UINT8*              pui1_base,
    INT32               i4_P,
    INT32               i4_R,
    SIZE_T              z_size_of_elem,
    x_sort_cmp_fct      pf_cmp_fct,
    UINT8*              pui1_pivot,
    UINT8*              pui1_temp_buf,
    VOID*               pv_opt_param
)
{
    INT32  i4_mid;
    i4_mid = i4_R - i4_P;
    if ( i4_mid > 3 )
    {
        i4_mid = i4_mid/2+i4_P;
         _SWAP_ELEM_(pui1_base,i4_P,i4_mid,z_size_of_elem,pui1_temp_buf);
    }
    return s_partition(pui1_base,i4_P,i4_R, z_size_of_elem,
                       pf_cmp_fct, pui1_pivot, pui1_temp_buf,
                       pv_opt_param);
}

    
    
/*-----------------------------------------------------------------------------
 * Name: s_partition
 *
 * Description: Internal API to partition the input array.
 *
 * Inputs:  -
 *   pui1_base  Input array to be sorted and partitioned.
 *   i4_P    start index in the pui1_base array.
 *   i4_R    end index in the pui1_base array.
 *   z_size_of_elem:  The size of individual element in the 'pv_array'.
 *   pf_cmp_fct:      Compare function to compare two elements in 'pv_array'.
 *   pui1_pivot:      Pointer to the memory buffer to store the pivot value.
 *   pui1_temp_buffer Pointer to an memory buffer to swapping 2 elements.
 *   pv_opt_param:    Optional parameters pass to the compare function.
 *
 * Outputs: -
 *   pui1_base  Array with elements partitioned.
 *
 * Returns: -  
 *           
 ----------------------------------------------------------------------------*/
static INT32  s_partition
(
    UINT8*              pui1_base,
    INT32               i4_P,
    INT32               i4_R,
    SIZE_T              z_size_of_elem,
    x_sort_cmp_fct      pf_cmp_fct,
    UINT8*              pui1_pivot,
    UINT8*              pui1_temp_buf,
    VOID*               pv_opt_param
)
{
    INT32  i4_lower;   /* Lower half index. */
    INT32  i4_upper;   /* Upper half index. */
    
    x_memcpy(pui1_pivot, pui1_base+(i4_P*z_size_of_elem), z_size_of_elem);
    i4_lower = i4_P - 1 ;
    i4_upper = i4_R + 1 ;
 
    for ( ; ; )
    {
        /* Upper portion */

        while(1)
        {
            i4_upper = i4_upper - 1;
            
            if ( i4_upper == i4_P        ||
                 i4_upper <= i4_lower    ||
                 pf_cmp_fct( (VOID*) pui1_pivot,
                             (VOID*) (pui1_base+i4_upper*z_size_of_elem),
                             pv_opt_param ) >= 0 )
            {
                break;
            }
        }

        while(1)
        {
            i4_lower = i4_lower + 1;
                        
            if ( i4_lower == i4_P         ||
                 i4_lower >= i4_upper     ||
                 pf_cmp_fct( (VOID*) (pui1_base+i4_lower*z_size_of_elem),
                             (VOID*) pui1_pivot,
                             pv_opt_param) >=  0 )
            {
                break;
            }
        }
        
        /* exchange the element from lower portion to upper portion. */
        if ( i4_lower < i4_upper )
        {
            _SWAP_ELEM_(pui1_base,i4_lower,i4_upper,z_size_of_elem,pui1_temp_buf);
        }
        else
        {
            return i4_upper;
        }
       
        /* optimization: end check to avoid additional comparison. */
        if ( (i4_upper-1) == i4_lower )
        {
            return (i4_upper-1);
        }        
    }
}
/*-----------------------------------------------------------------------------
 * Name: x_binary_search
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
 *           i4_idx           Specifies the index into element in 
 *                            the 'pv_array' with the same 'pv_search_key' 
 *                            value to be returned from the search operation.
 *           pv_opt_param:    Pointer to the optional parameter passed to
 *                            the compare function.
 *
 * Outputs: - pi4_result:     Variable to store the index in element
 *                            in 'pv_array' array that match the search
 *                            parameter.
 *
 * Returns: - 0     Success.
 *            -1    Sort failed.
 ----------------------------------------------------------------------------*/
INT32 x_binary_search
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
    return x_bin_search(
        pv_array,
        z_num_elem,
        z_size_of_elem,
        pf_cmp_fct,
        pv_search_param,
        i4_idx,
        pv_opt_param,
        pi4_result,
        NULL,
        NULL);
}
        
/*-----------------------------------------------------------------------------
 * Name: s_binary_find 
 *
 * Description: This API perform perform a binary search on sorted array.
 *
 * Inputs: - pv_array:         Address of the array containing sorted elements.
 *                            The sorted elements can be an array of sorted
 *                            indices into a array object.
 *           i4_P:            Starting index of the pv_array array.
 *           i4_R:            Ending index of the pv_array array.
 *           z_num_elem:      Count of element in the 'pv_array' array.
 *           z_size_of_elem:  Size of element in the 'pv_array' array.
 *           pf_cmp_fct:      A function that compare the element of
 *                            'pv_array' array and the search parameter
 *                            value. Note: if the 'pv_array' array
 *                            contains indices to the object array, the
 *                            comparison is performed between the 'actual'
 *                            object and the search key parameter..
 *           pv_search_param: Parameter value of the element to search.
 *           pv_opt_param:    Pointer to the optional parameter passed to
 *                            the compare function.
 *
 * Outputs: - pi4_result:     Variable to store the index to the element
 *                            in 'pv_array' array that match the searching
 *                            parameter.
 *
 *            pi4_upper_idx:  If the object is found, these two indices
 *            pi4_lower_idx:  return the positions of the neighboring
 *                            elements bracking the founded object.
 *                            
 *                            If the object is not found, then these two
 *                            variables return indices to elements in sorted
 *                            array which the object will be located if it
 *                            exist.
 *
 *                            for example, if sorted array contain following
 *                            values:
 *
 *                                 101, 103, 204, 208, 310
 *
 *                            Searching for element with value of 102, the
 *                            API returns SSR_NOT_FOUND and i4_upper_idx = 1
 *                            (for value 103), and i4_lower_idx = 0 (for
 *                            value 101).
 *
 *                            Searching for element with value of 311, the
 *                            API will return SSR_NOT_FOUND and i4_upper_idx
 *                            = -1 and i4_lower_idx (4) (for value 310)
 *
 *                            Searching for element with value of 91, the API
 *                            will return SSR_NOT_FOUND  and i4_upper_idx = 0
 *                            (for value 101) and i4_lower_idx = -1.
 *
 *                            
 * Returns: -
 *    SSR_OK          specified item found.
 *    SSR_NOT_FOUND   specified item not found.
 ----------------------------------------------------------------------------*/
static INT32 s_binary_find
(
    const UINT8*          pui1_base,
    INT32                 i4_P,
    INT32                 i4_R,
    SIZE_T                z_num_elem,
    SIZE_T                z_size_of_elem,
    x_search_cmp_fct      pf_cmp_fct,
    const VOID*           pv_search_param,
    VOID*                 pv_opt_param,
    INT32*                pi4_result,
    INT32*                pi4_lower_idx,
    INT32*                pi4_upper_idx
)
{
    INT32    i4_Q;
    INT32    i4_rc;    /* return status code */
    INT32    i4_cmp;   /* return code for compare function. */
    

    if (  pi4_lower_idx != NULL      &&
          pi4_upper_idx != NULL
        )
    {
        *pi4_lower_idx = -1 ;
        *pi4_upper_idx = -1 ;
    }
    
    /*
       Note:
       
       i4_P is lower bound, i4_R is the upper bound of
       search element within each sub-group of elements
       to search.
    */
    while (1)
    {
        
        /* compute the pivot point. */
        i4_Q=(INT32)((i4_R-i4_P)/2) + i4_P;
    
        if ( (i4_cmp = pf_cmp_fct((VOID*) (pui1_base+i4_Q*z_size_of_elem),
                                  (VOID*) pv_search_param,
                                  pv_opt_param))
             == RC_EQUAL )
        {
            /* element in the array matching the search parameter
               is found. */
            *pi4_result=(i4_Q);
            i4_rc=SSR_OK;

            /*
              Object found, copy the location to the upper and lower
              indices.
            */
            if ( pi4_lower_idx != NULL      &&
                 pi4_upper_idx != NULL
                )
            {
                *pi4_upper_idx = *pi4_result + 1 ;
                /*
                  check upper boundary.
                */
                if ( *pi4_upper_idx >= ((INT32)z_num_elem) )
                {
                    *pi4_upper_idx = -1 ;
                }
                
                *pi4_lower_idx = *pi4_result - 1 ;
            }
            break;
        }
        else
        {
            if ( i4_cmp == RC_ERROR )
            {
                /*
                  compare function failed, so teminate the search
                  operation.
                */
                return SSR_FAIL;
            }
        }

        if ( i4_P >= i4_R )
        {
            /* No element in the array match search parameter. */
            *pi4_result= -1 ;
            i4_rc = SSR_NOT_FOUND ;

            if ( pi4_lower_idx != NULL      &&
                 pi4_upper_idx != NULL
                )
            {
                /* determine the indices of the neighboring elements. */
                if ( pf_cmp_fct((VOID*)(pui1_base+i4_R*z_size_of_elem),
                                (VOID*) pv_search_param,
                                pv_opt_param) == RC_GREATER  )
                {
                    *pi4_upper_idx = i4_R ;

                    *pi4_lower_idx = i4_R - 1 ;
                }
                else
                {
                    *pi4_upper_idx = i4_R + 1 ;

                    if ( *pi4_upper_idx >=  ((INT32)z_num_elem) )
                    {
                        *pi4_upper_idx = -1 ;
                    }
                
                    *pi4_lower_idx = i4_R ;
                }
            }
            break;
        }

        /* If the pivot is greater than object */
        if ( pf_cmp_fct((VOID*) (pui1_base+i4_Q*z_size_of_elem),
                        (VOID*) pv_search_param,
                        pv_opt_param)
             == RC_GREATER )
        {
            /* Search the lower half. */
            i4_R = i4_Q ;
        }
        else
        {
            /* else, the piovt is less than the object,
               search the upper half */
            i4_P = i4_Q+1;
            
        }
    }
    return i4_rc;
} 

/*-----------------------------------------------------------------------------
 * Name: x_bin_search
 *
 * Description: This API perform perform a binary search on sorted array.
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
 *           pv_search_key:   Parameter used to search the object array.
 *           i4_idx           Specifies the index into element in 
 *                            the 'pv_array' with the same 'pv_search_key' 
 *                            value to be returned from the search operation.
 *           pv_opt_param:    Pointer to the optional parameter passed to
 *                            the compare function.
 *
 * Outputs: - pi4_result:     Variable to store the index in element
 *                            in 'pv_array' array that match the search
 *                            parameter.
 *
 *            pi4_upper_idx:  If the object is found, these two indices
 *            pi4_lower_idx:  return the location of the element (e.g., same
 *                            as the pi4_result.)
 *                            
 *                            If the object is not found, then these two
 *                            variables return indices to elements in sorted
 *                            array bracking the object.
 *
 *                            for example, if sorted array contain following
 *                            values:
 *
 *                                 101, 103, 204, 208, 310
 *
 *                            Searching for element with value of 102, the
 *                            API returns SSR_NOT_FOUND and i4_upper_idx = 1
 *                            (for value 103), and i4_lower_idx = 0 (for
 *                            value 101).
 *
 *                            Searching for element with value of 311, the
 *                            API will return SSR_NOT_FOUND and i4_upper_idx
 *                            = -1 and i4_lower_idx (4) (for value 310)
 *
 *                            Searching for element with value of 91, the API
 *                            will return SSR_NOT_FOUND  and i4_upper_idx = 0
 *                            (for value 101) and i4_lower_idx = -1.
 *
 *
 * Returns: - SSR_OK    Success.
 *            SSR_NOT_FOUND   Specified item is not found.
 *            SSR_INV_ARG  Sort failed due to pv_array=NULL, or
 *                         size of array or size of element in the array
 *                         is 0, or search compare function is NULL.
 ----------------------------------------------------------------------------*/
INT32 x_bin_search
(
    const VOID*      pv_array,
    SIZE_T           z_num_elem,
    SIZE_T           z_size_of_elem,
    x_search_cmp_fct pf_cmp_fct,
    const VOID*      pv_search_param,
    INT32            i4_idx,
    VOID*            pv_opt_param,
    INT32*           pi4_result,
    INT32*           pi4_lower_idx,
    INT32*           pi4_upper_idx
)
{
    INT32  i4_rc;
    
    if ( (pv_array != NULL) && (z_num_elem > 0) && (z_size_of_elem > 0) &&
         (pf_cmp_fct != (x_search_cmp_fct) NULL) 
        )
    {
        /*
          Search the sorted list for the object matching the
          search parameter.
        */
        
        i4_rc=s_binary_find(
            pv_array,
            (INT32) 0,
            (INT32) (z_num_elem-1),
            z_num_elem,
            z_size_of_elem,
            pf_cmp_fct,
            pv_search_param,
            pv_opt_param,
            pi4_result,
            pi4_lower_idx,
            pi4_upper_idx
            );

        /*
          We found the object in the sorted list, now we need to find the
          element (of the same key) matching the specified indice.
        */
        if ( i4_rc == SSR_OK )
        {
            const UINT8*          pui1_base;
            INT32                 i4_pos;
            INT32                 i4_found_pos;
            INT32                 i4_this_pos;
            INT32                 i4_this_idx;

            i4_pos                = *pi4_result;
            i4_found_pos          = *pi4_result;
            i4_this_pos           = *pi4_result;
            
            pui1_base             = (UINT8*) pv_array;
            i4_this_idx           = 0 ;
            
            /* Found the specified object, now determine the index for the 
               element found. */
            while ( (--i4_pos) >= 0 )
            {
                if ( pf_cmp_fct((VOID*) (pui1_base+i4_pos*z_size_of_elem),
                                (VOID*) pv_search_param,
                                pv_opt_param) == RC_EQUAL 
                    )
                {
                    i4_this_idx++;
                }
                else 
                { 
                    break;
                }
            }

            /* If the index of this element found is equal to
               the specified index, then we are done.
            */
            if ( i4_this_idx == i4_idx )
            {
                *pi4_result = i4_found_pos;
                i4_rc = SSR_OK;
            }
            else
            {
                /*
                  compute the location of element in the array
                  with the specified index value.
                */
                i4_found_pos = i4_found_pos + (i4_idx - i4_this_idx) ;
                /*
                  check this element to see if it matches the
                  search parameter.
                */
                if ( pf_cmp_fct((VOID*) (pui1_base+i4_found_pos*
                                         z_size_of_elem),
                                (VOID*) pv_search_param,
                                pv_opt_param) == RC_EQUAL 
                    )
                {
                    *pi4_result = i4_found_pos;
                    i4_rc = SSR_OK;
                    /*
                      Found the object with specified index, update the
                      neighbor element indices.
                    */
                     if ( pi4_lower_idx != NULL      &&
                          pi4_upper_idx != NULL
                         )
                     {
                         *pi4_upper_idx = *pi4_result + 1 ;
                         /*
                            check upper boundary.
                          */
                         if ( *pi4_upper_idx >= ((INT32)z_num_elem) )
                         {
                             *pi4_upper_idx = -1 ;
                         }
                
                         *pi4_lower_idx = *pi4_result - 1 ;
                     }
                }
                else
                {
                    *pi4_result = -1 ;
                    i4_rc = SSR_NOT_FOUND;
                    /*
                      Object with specified index is not found,
                      We need compute the neighboring element position.
                    */
                     if ( pi4_lower_idx != NULL      &&
                          pi4_upper_idx != NULL
                         )
                     {
                         /* Find the position of last index for this
                            element group.
                         */
                         while ( i4_this_pos < ((INT32)z_num_elem) )
                         {
                             if ( pf_cmp_fct(
                                      (VOID*)(pui1_base+i4_this_pos*
                                              z_size_of_elem),
                                      (VOID*) pv_search_param,
                                      pv_opt_param) == RC_EQUAL 
                                 )
                             {
                                 i4_this_pos++;
                             }
                             else 
                             { 
                                 break;
                             }
                         }

                         *pi4_upper_idx = i4_this_pos ;
                         /*
                           check the upper boundary condition.
                         */
                         if ( *pi4_upper_idx >= ((INT32)z_num_elem)  )
                         {
                             *pi4_lower_idx = *pi4_upper_idx - 1 ;
                             *pi4_upper_idx = -1 ;
                         }
                         else
                         {
                             *pi4_lower_idx = *pi4_upper_idx - 1 ;    
                         }
                     }
                }
            }
        } 
    }
    else
    {
        i4_rc=SSR_INV_ARG;
    }
    return i4_rc;
} 
