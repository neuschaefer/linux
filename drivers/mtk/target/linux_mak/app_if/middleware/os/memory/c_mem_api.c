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
 * $RCSfile: c_mem_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains implementation of the application memory
 *         allocation/free API's
 *         .
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "aee/aee_mem.h"

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Function: c_mem_alloc()
 *
 * Description:
 *      this API allocates memory from AEE default memory partition.
 *
 * Inputs:
 *      z_size: requested memory size
 *
 * Outputs: -
 *
 * Returns:
 *      non NULL: routine was successful.
 *      NULL: routine failed.
 *---------------------------------------------------------------------------*/
#ifdef SYS_MEM_CHK
VOID* c_mem_dbg_alloc (SIZE_T z_size, const CHAR* s_file, UINT32 ui4_line)
{
#if defined(OSAI_FOR_NUCLEUS) || !defined(LINUX_TURNKEY_SOLUTION)
    VOID*  pv_mem;
    
    pv_mem = aee_mem_dbg_alloc_for_aee(z_size, s_file, ui4_line);
    
    return pv_mem;
#else
    return x_mem_dbg_alloc(z_size, s_file, ui4_line);
#endif
}
#else
VOID* c_mem_alloc
(
     SIZE_T  z_size
)
{
#if defined(OSAI_FOR_NUCLEUS) || !defined(LINUX_TURNKEY_SOLUTION)
    VOID*  pv_mem;
    
    pv_mem = aee_mem_alloc_for_aee(z_size);
    
    return pv_mem;
#else
    return x_mem_alloc(z_size);
#endif


}
#endif    /* #endif SYS_MEM_CHK */

/*----------------------------------------------------------------------------
 * Function: c_mem_calloc()
 *
 * Description:
 *      this API allocates memory from the AEE default memory partition. This 
 *      API zeros out the allocated memory space.
 *
 * Inputs:
 *      ui4_num_elem: number of memory units
 *      z_unit_elem: size of each element.
 *
 * Outputs: -
 *
 * Returns:
 *      non NULL: routine was successful.
 *      NULL: routine failed.
 *---------------------------------------------------------------------------*/
#ifdef SYS_MEM_CHK
VOID* c_mem_dbg_calloc(UINT32      ui4_num_elem,
                       SIZE_T      z_size_elem, 
                       const CHAR* s_file, 
                       UINT32      ui4_line)
{
#if defined(OSAI_FOR_NUCLEUS) || !defined(LINUX_TURNKEY_SOLUTION)
    VOID*   pv_mem;
    SIZE_T  z_size;
    
    z_size = (ui4_num_elem * z_size_elem);
    
    pv_mem = c_mem_dbg_alloc(z_size, s_file, ui4_line);
    
    if (pv_mem != NULL)
    {
        x_memset(pv_mem, 0, z_size);
    }
    
    return pv_mem;
#else
    return x_mem_dbg_calloc(ui4_num_elem, z_size_elem, s_file, ui4_line);
#endif  
}
#else
VOID* c_mem_calloc
(
    UINT32  ui4_num_elem,
    SIZE_T  z_size_elem
)
{
#if defined(OSAI_FOR_NUCLEUS) || !defined(LINUX_TURNKEY_SOLUTION)
    VOID*   pv_mem;
    SIZE_T  z_size;
    
    z_size = (ui4_num_elem * z_size_elem);
    
    pv_mem = c_mem_alloc(z_size);
    
    if (pv_mem != NULL)
    {
        x_memset(pv_mem, 0, z_size);
    }
    
    return pv_mem;
#else
    return x_mem_calloc(ui4_num_elem, z_size_elem);
#endif  
}
#endif

/*------------------------------------------------------------------------
 * Function: c_mem_realloc()
 *
 * Description:
 *      this API changes the size of memory block point to by the
 *      "pv_mem_block" to the 'z_new_size' bytes.
 *
 *      If 'pv_mem_block' is NULL, this call is equivalent to x_mem_alloc().
 *      If 'z_new_size' is 0, this call is equivalent to x_mem_free().
 *
 * Inputs:
 *      pv_mem_block: existing, allocated memory block. 
 *      z_new_size:   size of new memory block.
 *
 * Outputs:
 *      pv_mem_block: the memory block with the 'z_new_size' bytes
 *
 * Returns:
 *      non NULL: routine was successful.
 *      NULL: routine failed.
 *-----------------------------------------------------------------------*/
#ifdef SYS_MEM_CHK
VOID* c_mem_dbg_realloc(VOID* pv_mem_block, SIZE_T z_new_size, const CHAR* s_file, UINT32 ui4_line)
{
#if defined(OSAI_FOR_NUCLEUS) || !defined(LINUX_TURNKEY_SOLUTION)
    return aee_mem_dbg_realloc_for_aee(pv_mem_block, z_new_size, s_file, ui4_line);
#else
    return x_mem_dbg_realloc(pv_mem_block, z_new_size, s_file, ui4_line);
#endif
}
#else
VOID* c_mem_realloc
(
    VOID*        pv_mem_block,
    SIZE_T       z_new_size
)
{
#if defined(OSAI_FOR_NUCLEUS) || !defined(LINUX_TURNKEY_SOLUTION)
    return aee_mem_realloc_for_aee(pv_mem_block, z_new_size);
#else
    return x_mem_realloc(pv_mem_block, z_new_size);
#endif
}
#endif

/*------------------------------------------------------------------------
 * Function: x_mem_free()
 *
 * Description:
 *      This API frees a memory block. it performs
 *      1. check validity of pv_mem.
 *      2. call os_mem_free() to actual free the memory block.
 *
 * Inputs:
 *      pv_mem: pointer to a memory block to be freed.
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------*/
#ifdef SYS_MEM_CHK
VOID c_mem_dbg_free(VOID* pv_mem, const CHAR* s_file, UINT32 ui4_line)
{
#if defined(OSAI_FOR_NUCLEUS) || !defined(LINUX_TURNKEY_SOLUTION)
    aee_mem_dbg_free_for_aee(pv_mem, s_file, ui4_line);
#else
    x_mem_dbg_free(pv_mem, s_file, ui4_line);
#endif
}
#else
VOID c_mem_free
(
    VOID*  pv_mem
)
{
#if defined(OSAI_FOR_NUCLEUS) || !defined(LINUX_TURNKEY_SOLUTION)
    aee_mem_free_for_aee(pv_mem);
#else
    x_mem_free(pv_mem);
#endif
}
#endif    /* #endif SYS_MEM_CHK */

/*----------------------------------------------------------------------------
 * Function: c_mem_part_create()
 *
 * Description:
 *      this API creates a memory partition.
 *
 * Inputs:
 *   ps_name: an unique partition name, null ('\0') terminated. Length can be
 *            up to 16 characters, excluding '\0'.
 *   pv_addr: starting memory address for this partition.
 *   z_size: size of this partition.
 *   z_alloc_size: 0 for dynamic size allocation. non-zero for fixed size
 *                 allocation (z_alloc_size is the size).
 *
 * Outputs:
 *   *ph_part_hdl: pointer to hold returned partition handle.
 *                 the handle is valid only if this API returns OSR_OK.
 *
 * Returns:
 *   OSR_OK : routine was successful. 
 *   OSR_NOT_INIT : memory module has not been initiated. 
 *   OSR_INV_ARG: an error in the arguments of this API.
 *   OSR_NO_RESOURCE: no resources (memory).
 *   OSR_FAIL: OS driver related errors.
 *   OSR_EXIST: partition exists (ps_name collision).
 *   OSR_AEE_OUT_OF_RESOURCES: can not allocate AEE resources for the memory
 *                             partition.
 *   OSR_AEE_NO_RIGHTS: No permission to create the memory partition.
 *---------------------------------------------------------------------------*/
INT32 c_mem_part_create
(
    HANDLE_T*    ph_part_hdl,
    const CHAR*  ps_name,
    VOID*        pv_addr,
    SIZE_T       z_size,
    SIZE_T       z_alloc_size
)
{
    return x_mem_part_create(ph_part_hdl,
                             ps_name,
                             pv_addr,
                             z_size,
                             z_alloc_size);
}

/*----------------------------------------------------------------------------
 * Function: c_mem_part_delete()
 *
 * Description:
 *      this API creates a memory partition. it performs
 *      1. check if (default) memory is initiated. 
 *      2. check argument.
 *      3. call sem lock to guard partition table.
 *      4. call os_mem_part_delete() to actually delete the partition.
 *
 * Inputs:
 *      h_part_hdl: memory partition handle.
 *
 * Outputs:
 *      None
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : memory module has not been initiated.
 *      OSR_INV_HANDLE: invalid handle.
 *      OSR_NOT_EXIST: partition does not exists.
 *      OSR_MEM_NOT_FREE: partition is deleted but its memory has not freed.
 *---------------------------------------------------------------------------*/
INT32 c_mem_part_delete(HANDLE_T h_part_hdl)
{
    return x_mem_part_delete(h_part_hdl);
}

VOID* c_mem_part_alloc
(
    HANDLE_T h_part_hdl, 
    SIZE_T z_size
)
{
    return x_mem_part_alloc(h_part_hdl, z_size);
}

#if 0
/*----------------------------------------------------------------------------
 * Function: c_mem_part_attach()
 *
 * Description:
 *    this API creates a handle to an existing memory partition.
 *
 * Inputs:
 *    ps_name: an unique partition name, null ('\0') terminated. Length
 *             can be up to 16 characters, excluding '\0'.
 *
 * Outputs:
 *    ph_part_hdl: pointer to hold returned partition handle.
 *                 the handle is valid only if this API returns OSR_OK.
 *
 * Returns:
 *    OSR_OK : routine was successful. 
 *    OSR_INV_ARG: an error in the arguments of this API.
 *    OSR_NO_RESOURCE:  No memory.
 *    OSR_NOT_EXIST: partition does not exist.
 *    OSR_AEE_OUT_OF_RESOURCES: can not allocate AEE resources for the memory
 *                               partition.
 *    OSR_AEE_NO_RIGHTS: No permission to create the memory partition.
 *---------------------------------------------------------------------------*/
INT32 c_mem_part_attach
(
    HANDLE_T*    ph_part_hdl,
    const CHAR*  ps_name
)
{
    HANDLE_T  h_aux;
    INT32     i4_rc;

    i4_rc = aee_grab_handle_resource(AEE_FLAG,
                                     1,
                                     &h_aux);
    switch (i4_rc)
    {
        case AEER_OK:
        {
            i4_rc = x_mem_part_attach(ph_part_hdl, ps_name);   
            
            if (i4_rc == OSR_OK)
            {
                handle_link_to_aux(h_aux, *ph_part_hdl);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_rc = OSR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_rc = OSR_AEE_NO_RIGHTS;
        }
        break;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------------
 * Function: x_mem_part_alloc()
 *
 * Description:
 *      this API allocates memory from specified partition.
 *
 * Inputs:
 *    h_part_hdl: memory partition handle.
 *    z_size: requested memory size.
 *
 * Outputs:
 *    None.
 *
 * Returns:
 *    non NULL: routine was successful.
 *    NULL: routine failed.
 *---------------------------------------------------------------------------*/
VOID* c_mem_part_alloc
(
    HANDLE_T  h_part_hdl,
    SIZE_T    z_size
)
{
    return x_mem_part_alloc(h_part_hdl, z_size);
}

/*----------------------------------------------------------------------------
 * Function: c_mem_part_calloc()
 *
 * Description:
 *      this API allocates memory from specified partition. The allocated
 *      memory space is zero out.
 *
 * Inputs:
 *      h_part_hdl: memory partition handle.
 *      ui4_num_elem: number of memory units
 *      z_elem_size: size of each unit
 *
 * Outputs: -
 *
 * Returns:
 *      non NULL: routine was successful.
 *      NULL: routine failed.
 *---------------------------------------------------------------------------*/
VOID* c_mem_part_calloc
(
    HANDLE_T  h_part_hdl,
    UINT32    ui4_num_elem,
    SIZE_T    z_elem_size
)
{
    return x_mem_part_calloc(h_part_hdl, ui4_num_elem, z_elem_size);
}

/*-------------------------------------------------------------------------
 * Function: c_mem_part_realloc()
 *
 * Description:
 *    this API allocates memory block of "z_new_size" from specified
 *    partition to replace the existing memory blcok "pv_mem".
 *
 *    If 'pv_mem' is NULL, this call is equivalent to x_mem_part_alloc().
 *    If 'z_new_size' is 0, this call is equivalent to x_mem_free().
 *
 * Inputs:
 *    h_part_hdl: memory partition handle.
 *    pv_mem: existing, allocated memory block. 
 *    z_new_size: size of new memory block.
 *
 * Outputs:
 *    pv_mem: the memory block with the 'z_new_size' bytes
 *
 * Returns:
 *    non NULL: routine was successful.
 *    NULL: routine failed.
 *------------------------------------------------------------------------*/
VOID* c_mem_part_realloc
(
    HANDLE_T     h_part_hdl,
    VOID*        pv_mem,
    SIZE_T       z_new_size
)
{
    return x_mem_part_realloc(h_part_hdl, pv_mem, z_new_size);
}
#endif

