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
 * $RCSfile: mem_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/


#include "os/inc/os.h"
#include "os/memory/mem_lib.h"
#include "os/thread/thread_lib.h"
#include "os/semaphore/sema_lib.h"
#include "os/timer/timer_lib.h"
#include "os/msgQ/msgq_lib.h"

#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
#include <stdlib.h>
#endif

#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
#undef x_mem_alloc
#undef x_mem_calloc
#undef x_mem_realloc
#endif

#ifdef SYS_MEM_CHK
#undef x_mem_alloc
#undef x_mem_calloc
#undef x_mem_realloc
#undef x_mem_free

extern VOID* x_mem_alloc (SIZE_T  z_size);

extern VOID* x_mem_calloc (UINT32  ui4_num_element,
                           SIZE_T  z_size_element);

extern VOID* x_mem_realloc (VOID*        pv_mem_block,
                            SIZE_T       z_new_size);

extern VOID x_mem_free (VOID*  pv_mem_block);
#endif

#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
extern BOOL os_linux_get_fbm_info(UINT32* pui4_phy_addr, UINT32* pui4_virt_addr, UINT32* pui4_size);
extern VOID* os_linux_share_mem_alloc(SIZE_T z_size);
extern VOID os_linux_share_mem_free(VOID* pv_phy_addr, SIZE_T z_size);
extern VOID* os_linux_mmap_share_mem(VOID* pv_phy_addr, SIZE_T z_size);
extern VOID* os_linux_mmap_share_mem_rmmgr(VOID* pv_phy_addr, SIZE_T z_size);
extern VOID os_linux_munmap_share_mem(VOID* pv_virt_addr, SIZE_T z_size);
#endif

MEM_GLOBAL_T *pt_mem_global = NULL;     /* carved from default partition */
MEM_PART_T *pt_def_part = NULL;         /* default partition */
HANDLE_T h_def_part_hdl = NULL_HANDLE;            /* default partition handle */


/*----------------------------------------------------------------------------
 * Function: x_mem_alloc()
 *
 * Description:
 *      this API allocates memory from default partition. it performs
 *      1. check if default partition exists (created via os_main()).
 *      2. call partition allocate funtiocn pf_alloc().
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
VOID *
x_mem_alloc(SIZE_T z_size)
{
    VOID *pv_mem = NULL;

    /* if z_size is 0, get the min. block */
    if (pt_def_part)
    {
        pv_mem = (pt_def_part->pf_alloc)(pt_def_part, z_size);
#ifdef CLI_LVL_ALL
#if defined(OSAI_FOR_NUCLEUS) || !defined(LINUX_TURNKEY_SOLUTION)
        if (pv_mem)
        {
            MEM_OBJ_T *pt_obj;
            VOID *pv_th_id;

            pt_obj = (MEM_OBJ_T *) (((UINT32) pv_mem) - sizeof(MEM_OBJ_T));

            if (x_os_drv_thread_self(&pv_th_id, ((VOID**) &(pt_obj->pv_thread)))!= OSR_OK)
            {
                pt_obj->pv_thread = NULL;
            }
        }
#endif
#endif  /* CLI_LVL_ALL */
    }

    return(pv_mem);
}


/*----------------------------------------------------------------------------
 * Function: x_mem_calloc()
 *
 * Description:
 *      this API allocates memory from default partition. it performs
 *      1. adjust to have at least one unit if needed.
 *      2. check if default partition exists (created via os_main()).
 *      3. call partition allocate funtiocn pf_alloc().
 *      4. zero out the allocated memory space.
 *
 * Inputs:
 *      ui4_num_unit: number of memory units
 *      z_unit_size: size of each unit
 *
 * Outputs: -
 *
 * Returns:
 *      non NULL: routine was successful.
 *      NULL: routine failed.
 *---------------------------------------------------------------------------*/
VOID *
x_mem_calloc(UINT32 ui4_num_unit, SIZE_T z_unit_size)
{
    VOID *pv_mem = NULL;
    UINT32  *pui4_mem;
    SIZE_T z_size;

    UNUSED(z_size);
    UNUSED(pui4_mem);

    if (ui4_num_unit == 0)
    {
        ui4_num_unit = 1;   /* allocate 1 unit of size z_unit_size */
    }

    if (pt_def_part)
    {
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
        pv_mem = (pt_def_part->pf_calloc)(pt_def_part, ui4_num_unit, z_unit_size);
#else
        z_size = ((ui4_num_unit * z_unit_size) + 3) & ~0x3;
        pv_mem = (pt_def_part->pf_alloc)(pt_def_part, z_size);

        if (pv_mem != NULL)
        {
            pui4_mem = (UINT32 *) pv_mem;
            while (z_size != 0)
            {
                *pui4_mem++ = (UINT32) 0;
                z_size -= sizeof(UINT32);
            }
        }
#endif

#ifdef CLI_LVL_ALL
#if defined(OSAI_FOR_NUCLEUS) || !defined(LINUX_TURNKEY_SOLUTION)
        if (pv_mem)
        {
            MEM_OBJ_T *pt_obj;
            VOID *pv_th_id;

            pt_obj = (MEM_OBJ_T *) (((UINT32) pv_mem) - sizeof(MEM_OBJ_T));

            if (x_os_drv_thread_self(&pv_th_id, ((VOID**) &(pt_obj->pv_thread)))!= OSR_OK)
            {
                pt_obj->pv_thread = NULL;
            }
        }
#endif
#endif  /* CLI_LVL_ALL */
    }

    return(pv_mem);
}


/*----------------------------------------------------------------------------
 * Function: x_mem_realloc()
 *
 * Description:
 *      this API allocates memory block of "z_new_size" from default partition
 *      to replace the existing memory blcok "pv_mem". it performs
 *      1. check validity of pv_mem.
 *      2. check if default partition exists (created via os_main()).
 *      3. if pv_mem is NULL, it is equivalent to x_mem_alloc().
 *      4. if z_new_size is 0, it is equivalent to x_mem_free().
 *      5. call os_mem_realloc() to allocate memory.
 *
 * Inputs:
 *      pv_mem: existing, allocated memory block.
 *      z_new_size: size of new memory block.
 *
 * Outputs: -
 *
 * Returns:
 *      non NULL: routine was successful.
 *      NULL: routine failed.
 *---------------------------------------------------------------------------*/
VOID *
x_mem_realloc(VOID *pv_mem, SIZE_T z_new_size)
{
    VOID *pv_m;

#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
    if (pv_mem)
    {
        MEM_OBJ_T*  pt_mem;
    
        pt_mem = ((MEM_OBJ_T*) pv_mem) - 1;
        
        if (pt_mem->ui2_padding3 != 0xFFFF)
        {
            return realloc(pv_mem, z_new_size);
        }
    }
#endif

    if ((pv_mem != NULL) && (((UINT32) pv_mem) & 0x3))
    {
        OS_DBG_ABORT(DBG_CAT_MEMORY, DBG_MEM_ADDR_NOT_ALIGNED);
    }

    /* check
     *   if ((pt_mem_global == NULL) || (b_os_mem_inited != TRUE))
     * is OK. But the check of the following is convenient.
     */
    if (pt_def_part != NULL)
    {
        /* If pv_mem is NULL, the call is equivalent to x_mem_alloc(size) */
        if (pv_mem == NULL)
        {
            return(x_mem_alloc(z_new_size));
        }

        /* If z_new_size is 0, the call is equivalent to x_mem_free(pv_new) */
        if (z_new_size == 0)
        {
            x_mem_free(pv_mem);
            return(NULL);
        }

#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
        pv_m = (pt_def_part->pf_realloc)(pt_def_part, pv_mem, z_new_size);
#else
        pv_m = os_mem_realloc(pt_def_part, pv_mem, z_new_size);
#endif

#ifdef CLI_LVL_ALL
#if defined(OSAI_FOR_NUCLEUS) || !defined(LINUX_TURNKEY_SOLUTION)
        if (pv_m)
        {
            MEM_OBJ_T *pt_obj;
            VOID *pv_th_id;

            pt_obj = (MEM_OBJ_T *) (((UINT32) pv_m) - sizeof(MEM_OBJ_T));

            if (x_os_drv_thread_self(&pv_th_id, ((VOID**) &(pt_obj->pv_thread)))!= OSR_OK)
            {
                pt_obj->pv_thread = NULL;
            }
        }
#endif
#endif  /* CLI_LVL_ALL */

        return(pv_m);
    }
    else
    {
        return(NULL);
    }
}


/*----------------------------------------------------------------------------
 * Function: x_mem_part_alloc()
 *
 * Description:
 *      this API allocates memory from specified partition. it performs
 *      1. check if memory was initialized.
 *      2. check validity of partition handle.
 *      3. check if partition exists (created via x_mem_part_create()).
 *      4. call partition allocate funtiocn pf_alloc().
 *
 * Inputs:
 *      h_part_hdl: memory partition handle.
 *      z_size: requested memory size.
 *
 * Outputs: -
 *
 * Returns:
 *      non NULL: routine was successful.
 *      NULL: routine failed.
 *---------------------------------------------------------------------------*/
VOID *
x_mem_part_alloc(HANDLE_T h_part_hdl, SIZE_T z_size)
{
    MEM_PART_T *pt_part;
    VOID *pv_mem = NULL;
    INT32 i4_ret;


    if ((pt_mem_global == NULL) || (b_os_mem_inited != TRUE))
    {
        return(NULL);
    }

    if (h_part_hdl == NULL_HANDLE)
    {
        pt_part = pt_def_part;
    }
    else
    {
        i4_ret = os_check_handle(h_part_hdl,
                                 HT_GROUP_OS_MEMORY,
                                 (VOID **)&pt_part);
        if (i4_ret != OSR_OK)
        {
            return(NULL);
        }

        if ((pt_part == NULL) ||
            ((pt_part->ui1_flags != OS_MEM_PART_FLAG_ACTIVE) &&
             (pt_part->ui1_flags != OS_MEM_PART_FLAG_EXTERNAL) &&
             (pt_part->ui1_flags != OS_MEM_PART_FLAG_PERMANENT)))
        {
            return(NULL);
        }
    }

    pv_mem = (pt_part->pf_alloc)(pt_part, z_size);

    return(pv_mem);
}


/*----------------------------------------------------------------------------
 * Function: x_mem_part_calloc()
 *
 * Description:
 *      this API allocates memory from specified partition. it performs
 *      1. check if memory was initialized.
 *      2. check validity of partition handle.
 *      3. check if partition exists (created via x_mem_part_create()).
 *      4. adjust to have at least one unit if needed.
 *      5. call partition allocate funtiocn pf_alloc().
 *      6. zero out the allocated memory space.
 *
 * Inputs:
 *      h_part_hdl: memory partition handle.
 *      ui4_num_unit: number of memory units
 *      z_unit_size: size of each unit
 *
 * Outputs: -
 *
 * Returns:
 *      non NULL: routine was successful.
 *      NULL: routine failed.
 *---------------------------------------------------------------------------*/
VOID *
x_mem_part_calloc(HANDLE_T h_part_hdl, UINT32 ui4_num_unit, SIZE_T z_unit_size)
{
    MEM_PART_T *pt_part;
    VOID *pv_mem = NULL;
    UINT32  *pui4_mem;
    SIZE_T z_size;
    INT32 i4_ret;

    UNUSED(z_size);
    UNUSED(pui4_mem);

    if ((pt_mem_global == NULL) || (b_os_mem_inited != TRUE))
    {
        return(NULL);
    }

    if (ui4_num_unit == 0)
    {
        ui4_num_unit = 1;   /* allocate 1 unit of size z_unit_size */
    }

    if (h_part_hdl == NULL_HANDLE)
    {
        pt_part = pt_def_part;
    }
    else
    {
        i4_ret = os_check_handle(h_part_hdl,
                                 HT_GROUP_OS_MEMORY,
                                 (VOID **)&pt_part);
        if (i4_ret != OSR_OK)
        {
            return(NULL);
        }

        if ((pt_part == NULL) ||
            ((pt_part->ui1_flags != OS_MEM_PART_FLAG_ACTIVE) &&
             (pt_part->ui1_flags != OS_MEM_PART_FLAG_EXTERNAL) &&
             (pt_part->ui1_flags != OS_MEM_PART_FLAG_PERMANENT)))
        {
            return(NULL);
        }
    }

#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
    pv_mem = (pt_part->pf_calloc)(pt_part, ui4_num_unit, z_unit_size);
#else
    z_size = ((ui4_num_unit * z_unit_size) + 3) & ~0x3;
    pv_mem = (pt_part->pf_alloc)(pt_part, z_size);

    if (pv_mem != NULL)
    {
        pui4_mem = (UINT32 *) pv_mem;
        while (z_size != 0)
        {
            *pui4_mem++ = (UINT32) 0;
            z_size -= sizeof(UINT32);
        }
    }
#endif

    return(pv_mem);
}


/*----------------------------------------------------------------------------
 * Function: x_mem_part_realloc()
 *
 * Description:
 *      this API allocates memory block of "z_new_size" from specified partition
 *      to replace the existing memory blcok "pv_mem". it performs
 *      1. check if memory was initialized.
 *      2. check validity of partition handle.
 *      3. check validity of pv_mem.
 *      4. check if default partition exists (created via os_main()).
 *      5. if pv_mem is NULL, it is equivalent to x_mem_alloc().
 *      6. if z_new_size is 0, it is equivalent to x_mem_free().
 *      7. call os_mem_realloc() to allocate memory.
 *
 * Inputs:
 *      h_part_hdl: memory partition handle.
 *      pv_mem: existing, allocated memory block.
 *      z_new_size: size of new memory block.
 *
 * Outputs: -
 *
 * Returns:
 *      non NULL: routine was successful.
 *      NULL: routine failed.
 *---------------------------------------------------------------------------*/
VOID *
x_mem_part_realloc(HANDLE_T h_part_hdl, VOID *pv_mem, SIZE_T z_new_size)
{
    MEM_PART_T *pt_part;
    INT32 i4_ret;

    if ((pv_mem != NULL) && (((UINT32) pv_mem) & 0x3))
    {
        OS_DBG_ABORT(DBG_CAT_MEMORY, DBG_MEM_ADDR_NOT_ALIGNED);
    }

    if ((pt_mem_global == NULL) || (b_os_mem_inited != TRUE))
    {
        return(NULL);
    }

    if (h_part_hdl == NULL_HANDLE)
    {
        pt_part = pt_def_part;
    }
    else
    {
        i4_ret = os_check_handle(h_part_hdl,
                                 HT_GROUP_OS_MEMORY,
                                 (VOID **)&pt_part);
        if (i4_ret != OSR_OK)
        {
            return(NULL);
        }

        if ((pt_part == NULL) ||
            ((pt_part->ui1_flags != OS_MEM_PART_FLAG_ACTIVE) &&
             (pt_part->ui1_flags != OS_MEM_PART_FLAG_EXTERNAL) &&
             (pt_part->ui1_flags != OS_MEM_PART_FLAG_PERMANENT)))
        {
            return(NULL);
        }
    }

    /* If pv_mem is NULL, the call is equivalent to x_mem_alloc(size) */
    if (pv_mem == NULL)
    {
        return(x_mem_part_alloc(h_part_hdl, z_new_size));
    }

    /* If z_new_size is 0, the call is equivalent to x_mem_free(pv_new) */
    if (z_new_size == 0)
    {
        x_mem_free(pv_mem);
        return(NULL);
    }

#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
    return (pt_part->pf_realloc)(pt_part, pv_mem, z_new_size);
#else
    return(os_mem_realloc(pt_part, pv_mem, z_new_size));
#endif
}


/*----------------------------------------------------------------------------
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
 *---------------------------------------------------------------------------*/
VOID
x_mem_free(VOID *pv_mem)
{
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
    MEM_OBJ_T*  pt_mem;

    pt_mem = ((MEM_OBJ_T*) pv_mem) - 1;

    if (pv_mem)
    {
        if (pt_mem->ui2_padding3 != 0xFFFF)
        {
            free(pv_mem);
            return;
        }
    }
#endif

    if ((pt_def_part != NULL) && (pv_mem != NULL))
    {
        if (((UINT32) pv_mem) & 0x3)
        {
            OS_DBG_ABORT(DBG_CAT_MEMORY, DBG_MEM_ADDR_NOT_ALIGNED);
        }

#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
        if (pt_mem->ui1_part_index == OS_MEM_PART_NUM)
        {
            pt_def_part->pf_free(pt_def_part, pv_mem);
        }
        else
        {
            os_mem_free(pv_mem);
        }
#else
        os_mem_free(pv_mem);
#endif
    }
}


/*----------------------------------------------------------------------------
 * Function: x_mem_part_create()
 *
 * Description:
 *      this API creates a memory partition. it performs
 *      1. check if (default) memory is initiated.
 *      2. parse argumets.
 *      3. call sem lock to guard partition table.
 *      4. call os_mem_part_create() to actually create a partition.
 *
 * Inputs:
 *      ps_name: an unique partition name, null ('\0') terminated. Length can
 *               be up to 16 characters, excluding '\0'.
 *      pv_addr: starting memory address for this partition.
 *      z_size: size of this partition.
 *      z_alloc_size: 0 for dynamic size allocation. non-zero for fixed size
 *                    allocation (z_alloc_size is the size).
 *
 * Outputs:
 *      *ph_part_hdl: pointer to hold returned partition handle.
 *                    the handle is valid only if this API returns OSR_OK.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : memory module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_NO_RESOURCE: no resources (memory).
 *      OSR_FAIL: OS driver related errors.
 *      OSR_EXIST: partition exists (ps_name collision).
 *---------------------------------------------------------------------------*/
INT32
x_mem_part_create(HANDLE_T      *ph_part_hdl,    /* partition handle */
                  const CHAR    *ps_name,
                  VOID          *pv_addr,
                  SIZE_T        z_size,
                  SIZE_T        z_alloc_size)
{
    INT32 i4_ret;

    if ((pt_mem_global == NULL) || (b_os_mem_inited != TRUE))
    {
        return(OSR_NOT_INIT);
    }

    /* check arguments */
    if ((ps_name == NULL) || (ps_name[0] == '\0') || (ph_part_hdl == NULL) ||
        (pv_addr == NULL) || (z_size == 0))
    {
        return(OSR_INV_ARG);
    }

    i4_ret = x_sema_lock(h_os_mem_gsema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return(i4_ret);
    }

    i4_ret = os_mem_part_create(ph_part_hdl,
                                pv_addr,
                                z_size,
                                z_alloc_size,
                                ps_name);

    if (x_sema_unlock(h_os_mem_gsema_hdl) != OSR_OK)
    {
        OS_DBG_ABORT(DBG_CAT_SEMAPHORE, DBG_SEMA_UNLOCK_FAIL);
    }

    return(i4_ret);
}


/*----------------------------------------------------------------------------
 * Function: x_mem_part_attach()
 *
 * Description:
 *      this API creates a memory partition. note that this API returns the
 *      same handle as the one returned by x_mem_part_create().
 *      it performs
 *      1. check if (default) memory is initiated. 
 *      2. parse argumets.
 *      3. call sem lock to guard partition table.
 *      4. call os_mem_part_check() to check existence of partition based on
 *         ps_name.
 *      5. return partition handle created in x_mem_part_create().
 *
 * Inputs:
 *      ps_name: an unique partition name, null ('\0') terminated. Length can
 *               be up to 16 characters, excluding '\0'.
 *
 * Outputs:
 *      *ph_part_hdl: pointer to hold returned partition handle.
 *                    the handle is valid only if this API returns OSR_OK.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : memory module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_NOT_EXIST: partition does not exist.
 *---------------------------------------------------------------------------*/
INT32
x_mem_part_attach(HANDLE_T    *ph_part_hdl,    /* partition handle */
                  const CHAR  *ps_name)
{
    MEM_PART_T *pt_part;
    INT32 i4_ret, i4_i;

    if ((pt_mem_global == NULL) || (b_os_mem_inited != TRUE))
    {
        return(OSR_NOT_INIT);
    }

    /* arguments check */
    if ((ps_name == NULL) || (ps_name[0] == '\0') || (ph_part_hdl == NULL))
    {
        return(OSR_INV_ARG);
    }

    i4_ret = x_sema_lock(h_os_mem_gsema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return(i4_ret);
    }

    /* i4_ret = os_mem_part_find(&pt_part, ps_name); */

    i4_ret = OSR_NOT_EXIST;
    i4_i = 0;
    pt_part = (MEM_PART_T *) &(pt_mem_global->t_part_tbl[i4_i]);
    while (i4_i < OS_MEM_PART_NUM)
    {
        if (x_strncmp(pt_part->s_name, ps_name, PART_NAME_LEN) == 0)
        {
            *ph_part_hdl = pt_part->h_part_hdl;
            i4_ret = OSR_OK;
            break;
        }
        else
        {
            ++pt_part;
            ++i4_i;
        }
    }

    if (x_sema_unlock(h_os_mem_gsema_hdl) != OSR_OK)
    {
        OS_DBG_ABORT(DBG_CAT_SEMAPHORE, DBG_SEMA_UNLOCK_FAIL);
    }

    return(i4_ret);
}


/*----------------------------------------------------------------------------
 * Function: x_mem_part_delete()
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
INT32
x_mem_part_delete(HANDLE_T h_part_hdl)    /* partition handle */
{
    MEM_PART_T *pt_part;
    INT32 i4_ret;

    if ((pt_mem_global == NULL) || (b_os_mem_inited != TRUE))
    {
        return(OSR_NOT_INIT);
    }

    i4_ret = OSR_INV_HANDLE;
    if (h_part_hdl != NULL_HANDLE)
    {
        i4_ret = os_check_handle(h_part_hdl,
                                 HT_GROUP_OS_MEMORY,
                                 (VOID **)&pt_part);
        if (i4_ret != OSR_OK)
        {
            return(OSR_INV_HANDLE);
        }

        if ((pt_part == NULL) ||
            ((pt_part->ui1_flags != OS_MEM_PART_FLAG_ACTIVE) &&
             (pt_part->ui1_flags != OS_MEM_PART_FLAG_EXTERNAL) &&
             (pt_part->ui1_flags != OS_MEM_PART_FLAG_PERMANENT)))
        {
            return(OSR_NOT_EXIST);
        }

        if (pt_part->ui1_flags == OS_MEM_PART_FLAG_PERMANENT)
        {
            return(OSR_INV_HANDLE);
        }

        i4_ret = x_sema_lock(h_os_mem_gsema_hdl, X_SEMA_OPTION_WAIT);
        if (i4_ret != OSR_OK)
        {
            return(i4_ret);
        }

        i4_ret = os_mem_part_delete(pt_part);

        if (x_sema_unlock(h_os_mem_gsema_hdl) != OSR_OK)
        {
            OS_DBG_ABORT(DBG_CAT_SEMAPHORE, DBG_SEMA_UNLOCK_FAIL);
        }
    }

    return(i4_ret);
}


#if 0
/*----------------------------------------------------------------------------
 * Function: os_mem_stats()
 *
 * Description:
 *      this API returns statistics of specified partition. it performs
 *      1. check if memory was initialized.
 *      2. check handle validity (NULL_HANDLE for default partition).
 *      3. lock partion sema.
 *      3. go through partition free bucket.
 *
 * Inputs:
 *      h_part_hdl: memory partition handle.
 *
 * Outputs: -
 *      *z_totalfree: total size of all free memory block.
 *      *z_bigfree: size of biggest free memory block.
 *      *z_freeobj: number of free memory blocks.
 *      *z_allocatedobj: number of allocated memory blocks.
 *      *z_totalsize: total partition size.
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
os_mem_stats(HANDLE_T h_part_hdl,
             SIZE_T *z_totalfree,
             SIZE_T *z_bigfree,
             SIZE_T *z_freeobj,
             SIZE_T *z_allocatedobj,
             SIZE_T *z_totalsize)
{
    INT32 i4_n, i4_ret;
    MEM_BUCKET_T *pt_bucket;
    MEM_FREEOBJ_T *pt_obj;
    MEM_PART_T *pt_part;

    *z_totalfree = 0;
    *z_bigfree = 0;
    *z_freeobj = 0;
    *z_allocatedobj = 0;
    *z_totalsize = 0;

    if ((pt_mem_global == NULL) || (b_os_mem_inited != TRUE))
    {
        return;
    }

    if (h_part_hdl == NULL_HANDLE)
    {
        pt_part = pt_def_part;
    }
    else
    {
        i4_ret = os_check_handle(h_part_hdl,
                                 HT_GROUP_OS_MEMORY,
                                 (VOID **)&pt_part);
        if (i4_ret != OSR_OK)
        {
            return;
        }
    }

    i4_ret = x_sema_lock(pt_part->h_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return;
    }

    if (pt_part->z_alloc_size == OS_MEM_DYNAMIC_ALLOC)
    {
        /* free memory */
        for (i4_n = OS_MEM_FIXED_INDEX , pt_bucket = &(pt_part->t_bucket[i4_n]);
                i4_n < OS_MEM_BUCKET_SIZE; i4_n++, pt_bucket++)
        {
            GET_LINK_HEAD(pt_obj, pt_bucket->pt_head);

            if (LINK_NOT_EMPTY(pt_obj, NULL))
            {
                do
                {
                    *z_totalfree += pt_obj->z_size;     /* total free memory */

                    if (*z_bigfree < pt_obj->z_size)
                    {
                        *z_bigfree = pt_obj->z_size;
                    }

                    GET_LINK_NEXT(pt_obj, pt_obj);
                } while (pt_obj != pt_bucket->pt_head);
            }
        }

        if (*z_bigfree >= sizeof(MEM_OBJ_T))
        {
            *z_bigfree -= sizeof(MEM_OBJ_T);
        }
        else
        {
            *z_bigfree = 0;
        }
    }
    else
    {
        *z_bigfree = pt_part->z_alloc_size;
        *z_totalfree = pt_part->z_alloc_size * pt_part->ui4_free_obj;
    }

    *z_totalsize = pt_part->z_size +
        (SIZE_T) ((UINT32) (pt_part->pv_mem) - ((UINT32) (pt_part->pv_start)));

    *z_allocatedobj = pt_part->ui4_alloc_obj;   /* total # of allocated obj*/
    *z_freeobj = pt_part->ui4_free_obj;

    if (x_sema_unlock(pt_part->h_sema_hdl) != OSR_OK)
    {
        OS_DBG_ABORT(DBG_CAT_SEMAPHORE, DBG_SEMA_UNLOCK_FAIL);
    }
}
#endif

VOID* x_share_mem_alloc(SIZE_T z_size)
{
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
    return os_linux_share_mem_alloc(z_size);
#else
    return x_mem_alloc(z_size);
#endif

}

VOID x_share_mem_free(VOID* pv_phy_addr, SIZE_T z_size)
{
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
    os_linux_share_mem_free(pv_phy_addr, z_size);
    return;
#else
    x_mem_free(pv_phy_addr);
    return;
#endif

}

VOID* x_mmap_share_mem_rmmgr(VOID* pv_phy_addr, SIZE_T z_size)
{
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
    return os_linux_mmap_share_mem_rmmgr(pv_phy_addr, z_size);
#else
    return pv_phy_addr;
#endif

}

VOID* x_mmap_share_mem(VOID* pv_phy_addr, SIZE_T z_size)
{
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
    return os_linux_mmap_share_mem(pv_phy_addr, z_size);
#else
    return pv_phy_addr;
#endif

}

VOID x_munmap_share_mem(VOID* pv_virt_addr, SIZE_T z_size)
{
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
    os_linux_munmap_share_mem(pv_virt_addr, z_size);
    return;
#else
    return;
#endif

}

#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
BOOL x_get_fbm_info(UINT32* pui4_phy_addr, UINT32* pui4_virt_addr, UINT32* pui4_size)
{
    if ((NULL==pui4_phy_addr) ||
        (NULL==pui4_virt_addr) ||
        (NULL==pui4_size))
    {
        return FALSE;
    }

    return os_linux_get_fbm_info(pui4_phy_addr, pui4_virt_addr, pui4_size);
}
#endif

