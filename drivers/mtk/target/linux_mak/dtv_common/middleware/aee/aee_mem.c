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
 * $RCSfile: aee_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/1 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: ed41ed3dcc5552c4ee8b06801ef05511 $
 *
 * Description: 
 *         This file contains the implementation of the AEE memory APIs.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "aee/x_aee.h"
#include "aee/_aee.h"
#include "aee/aee_mem.h"
#include "inc/pvt_key.h"
#include "os/inc/os.h"


/*-----------------------------------------------------------------------------
 * Name: aee_mem_delink_and_release
 *
 * Description: This API removes a pointer from the linked list of pointers
 *              attached to a specific AEE. It also releases the memory
 *              resource.
 *
 * Inputs:  pt_aee  References the AEE structure.
 *          pv_mem  Contains the pointer to remove.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID aee_mem_delink_and_release (AEE_T*  pt_aee,
                                        VOID*   pv_mem)
{
    VOID**  ppv_current;
    
    ppv_current = & pt_aee->pv_allocated_pointers;
    
    while (*ppv_current != NULL)
    {
        SIZE_T  z_size;
        
        z_size = os_mem_get_mem_ptr_size(*ppv_current);
        
        if (*ppv_current == pv_mem)
        {
            *ppv_current = *((VOID**)((CHAR*)*ppv_current + z_size) - 2);
            
            aee_release_memory_resource(pt_aee, z_size - (sizeof(VOID*) * 2));
            
            break;
        }
        
        ppv_current = (VOID**)((CHAR*)*ppv_current + z_size) - 2;
    }
}

/*-----------------------------------------------------------------------------
 * Name: aee_mem_grab_and_link
 *
 * Description: This API tries to grab memory resources from the memory pool
 *              and if successful, links the pointer to the AEE's list of
 *              allocated pointers.
 *
 * Inputs:  pt_aee  References the AEE structure.
 *          pv_mem  Contains the pointer to link.
 *
 * Outputs: -
 *
 * Returns: AEER_OK         Routine was successful
 *          negative value  Routine was not successful.
 ----------------------------------------------------------------------------*/
static INT32 aee_mem_grab_and_link (AEE_T*  pt_aee,
                                    VOID*   pv_mem)
{
    SIZE_T  z_size;
    INT32   i4_res;
    
    z_size = os_mem_get_mem_ptr_size(pv_mem);
    
    i4_res = aee_grab_memory_resource(pt_aee,
                                      z_size - (sizeof(VOID*) * 2));
    
    if (i4_res == AEER_OK)
    {
        CRIT_STATE_T  t_state;
        
        t_state = x_crit_start();
        
        *((VOID**)((CHAR*)pv_mem + z_size) - 1) =
            pt_aee;
        *((VOID**)((CHAR*)pv_mem + z_size) - 2) =
            pt_aee->pv_allocated_pointers;
        pt_aee->pv_allocated_pointers = pv_mem;
        
        x_crit_end(t_state);
    }
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: aee_mem_alloc_for_aee
 *
 * Description: This API allocates memory on behalf of an AEE.
 *
 * Inputs:  z_size  Contains the number of bytes to allocate. Actual size may
 *                  be bigger because of additional overhead.
 *
 * Outputs: -
 *
 * Returns: Pointer to allocated memory, or NULL if allocation failed.
 ----------------------------------------------------------------------------*/
#ifdef SYS_MEM_CHK
VOID* aee_mem_dbg_alloc_for_aee (SIZE_T z_size, const CHAR* s_file, UINT32 ui4_line)
#else
VOID* aee_mem_alloc_for_aee (SIZE_T  z_size)
#endif    /* #endif SYS_MEM_CHK */
{
    AEE_T*  pt_aee;
    VOID*   pv_mem;
    INT32   i4_res;
    
    pv_mem = NULL;
    
    i4_res = x_thread_get_pvt(PVT_KEY_AEE, (VOID**) & pt_aee);
    
    if (i4_res == OSR_OK)
    {
        SIZE_T  z_allocated_size;
        
        /* first, we try to allocate memory in the default partition */
        
        if (pt_aee->h_mem_partition != NULL_HANDLE)
        {
            pv_mem = x_mem_part_alloc(pt_aee->h_mem_partition,
                                      z_size + (sizeof(VOID*)));
        }
        
        if (pv_mem != NULL)
        {
            /*
              Allocation was successful. Chunk owner is set to NULL since
              the memory will automatically be deleted when the partition's
              handle is deleted.
            */
            
            z_allocated_size = os_mem_get_mem_ptr_size(pv_mem);
            
            *((VOID**)((CHAR*)pv_mem + z_allocated_size) - 1) = NULL;
        }
        else
        {
#ifdef SYS_MEM_CHK
            pv_mem = x_mem_dbg_alloc(z_size + (sizeof(VOID*) * 2),
                                     s_file,
                                     ui4_line);
#else
            pv_mem = x_mem_alloc(z_size + (sizeof(VOID*) * 2));
#endif    /* #endif SYS_MEK_CHK */
            
            if (pv_mem != NULL)
            {
                i4_res = aee_mem_grab_and_link(pt_aee, pv_mem);
                
                if (i4_res != AEER_OK)
                {
                    DBG_ERROR((DBG_PREFIX"Could not grab/link memory (%d)\n",
                               i4_res));
                    
                    x_mem_free(pv_mem);
                    
                    pv_mem = NULL;
                }
            }
        }
    }
    
    return pv_mem;
}

/*-----------------------------------------------------------------------------
 * Name: aee_mem_realloc_for_aee
 *
 * Description: This API reallocates memory on behalf of an AEE.
 *
 * Inputs:  pv_mem  References the memory to reallocate. Can be NULL.
 *          z_size  Contains the number of bytes to allocate. Actual size may
 *                  be bigger because of additional overhead.
 *
 * Outputs: -
 *
 * Returns: Pointer to allocated memory, or NULL if allocation failed.
 ----------------------------------------------------------------------------*/
#ifdef SYS_MEM_CHK
VOID* aee_mem_dbg_realloc_for_aee (VOID*       pv_mem,
                                   SIZE_T      z_new_size, 
                                   const CHAR* s_file, 
                                   UINT32      ui4_line)
#else
VOID* aee_mem_realloc_for_aee (VOID*   pv_mem,
                               SIZE_T  z_new_size)
#endif    /* #endif SYS_MEM_CHK */
{
    if (pv_mem != NULL)
    {
        AEE_T*  pt_aee;
        INT32   i4_res;
        
        i4_res = x_thread_get_pvt(PVT_KEY_AEE, (VOID**) & pt_aee);
        
        if (i4_res == OSR_OK)
        {
            AEE_T*  pt_owner;
            SIZE_T  z_allocated_size;
            
            z_allocated_size = os_mem_get_mem_ptr_size(pv_mem);
            
            pt_owner =
                (AEE_T*) *((VOID**)((CHAR*)pv_mem + z_allocated_size) - 1);
            
            if (pt_owner == NULL)
            {
                /* memory was allocated in the main partition */
                
                pv_mem = x_mem_part_realloc(pt_aee->h_mem_partition,
                                            pv_mem,
                                            z_new_size + sizeof(VOID*));
                
                if (pv_mem != NULL)
                {
                    z_allocated_size = os_mem_get_mem_ptr_size(pv_mem);
                    
                    *((VOID**)((CHAR*)pv_mem + z_allocated_size) - 1) = pt_aee;
                }
            }
            else
            {
                /* memory was allocated in the system pool */
                
                aee_mem_delink_and_release(pt_owner, pv_mem);
#ifdef SYS_MEM_CHK
                pv_mem = x_mem_dbg_realloc(pv_mem,
                                           z_new_size + (sizeof(VOID*) * 2),
                                           s_file,
                                           ui4_line);
#else
                pv_mem = x_mem_realloc(pv_mem,
                                       z_new_size + (sizeof(VOID*) * 2));
#endif    /* #endif SYS_MEM_CHK */
                
                if (pv_mem != NULL)
                {
                    i4_res = aee_mem_grab_and_link(pt_aee, pv_mem);
                    
                    if (i4_res != AEER_OK)
                    {
                        DBG_ERROR((DBG_PREFIX
                                   "Could not grab/link memory (%d)\n",
                                   i4_res));
                        
                        x_mem_free(pv_mem);
                        
                        pv_mem = NULL;
                    }
                }
            }
        }
        else
        {
            /* memory being reallocated outside AEE context */
#ifdef SYS_MEM_CHK
            aee_mem_dbg_free_for_aee(pv_mem, s_file, ui4_line);
#else
            aee_mem_free_for_aee(pv_mem);
#endif    /* #endif SYS_MEM_CHK */
            
            pv_mem = NULL;
        }
    }
    else
    {
        /* NULL pointer: equivalent to allocation */
#ifdef SYS_MEM_CHK
        pv_mem = aee_mem_dbg_alloc_for_aee(z_new_size, s_file, ui4_line);
#else
        pv_mem = aee_mem_alloc_for_aee(z_new_size);
#endif    /* #endif SYS_MEM_CHK */
    }
    
    return pv_mem;
}

/*-----------------------------------------------------------------------------
 * Name: aee_mem_free_for_aee
 *
 * Description: This API frees memory on behalf of an AEE.
 *
 * Inputs:  pv_mem  References the memory to free. Can be NULL.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
#ifdef SYS_MEM_CHK
VOID aee_mem_dbg_free_for_aee(VOID*       pv_mem,
                              const CHAR* s_file, 
                              UINT32      ui4_line)
#else
VOID aee_mem_free_for_aee (VOID*  pv_mem)
#endif    /* #endif SYS_MEM_CHK */
{
    if (pv_mem != NULL)
    {
        AEE_T*  pt_aee;
        SIZE_T  z_size;
        
        z_size = os_mem_get_mem_ptr_size(pv_mem);
        
        pt_aee = (AEE_T*) *((VOID**)((CHAR*)pv_mem + z_size) - 1);
        
        if (pt_aee != NULL)
        {
            aee_mem_delink_and_release(pt_aee, pv_mem);
        }
        
#ifdef SYS_MEM_CHK
        x_mem_dbg_free(pv_mem, s_file, ui4_line);
#else
        x_mem_free(pv_mem);
#endif    /* #endif SYS_MEM_CHK */
    }
}

/*-----------------------------------------------------------------------------
 * Name: aee_mem_free_all_allocated_pointers
 *
 * Description: This API frees all the pointers the AEE allocated in the
 *              system pool. Memory resources are not returned to the pool
 *              since this API is called when the AEE quits (so the pool
 *              values are not relevant anymore).
 *
 * Inputs:  pt_aee  References the AEE structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID aee_mem_free_all_allocated_pointers (AEE_T*  pt_aee)
{
#ifndef LINUX_TURNKEY_SOLUTION
    VOID*  pv_mem;
    
    pv_mem = pt_aee->pv_allocated_pointers;
    
    pt_aee->pv_allocated_pointers = NULL;
    
    while (pv_mem != NULL)
    {
        VOID*   pv_next;
        SIZE_T  z_size;
        
        z_size = os_mem_get_mem_ptr_size(pv_mem);
        
        pv_next = *((VOID**)((CHAR*)pv_mem + z_size) - 2);
        
        x_mem_free(pv_mem);
        
        pv_mem = pv_next;
    }
#else
#endif
}
