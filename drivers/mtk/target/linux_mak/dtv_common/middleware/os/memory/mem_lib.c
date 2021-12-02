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
 * $RCSfile: mem_lib.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

#include "os/inc/os.h"
#include "os/semaphore/sema_lib.h"
#include "os/thread/thread_lib.h"
#include "os/memory/mem_lib.h"
#include "os/timer/timer_lib.h"
#include "os/msgQ/msgq_lib.h"
#include "os/event_group/ev_grp_lib.h"
#include "dbg/x_dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "cli/x_cli.h"

#define OS_MEM_MIN_OBJ_SIZE ((sizeof(MEM_FREEOBJ_T) + 7) & ~0x7)
#define fix_bucket          t_bucket[OS_MEM_FIXED_INDEX]

static UINT16 ui2_os_dbg_level = DBG_LEVEL_NONE;

/* declared in mem_api.c only */
extern MEM_GLOBAL_T *pt_mem_global;  /* carved from default partition */
extern MEM_PART_T *pt_def_part;      /* default partition */
extern HANDLE_T h_def_part_hdl;      /* default partition handle */

#ifdef CLI_LVL_GUEST
#ifndef ANDROID
extern CLI_EXEC_T at_os_root_cmd_tbl[];
#endif
#endif  /* CLI_LVL_ALL */

/*-------------------------------------------------------------------------
  OS module handles autofree functions implementation
 --------------------------------------------------------------------------*/
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
VOID os_init_def_part_ops (os_mem_alloc_fct      pf_alloc,
                           os_mem_realloc_fct    pf_realloc,
                           os_mem_calloc_fct     pf_calloc,
                           os_mem_free_fct       pf_free)
{
    if(pt_def_part == NULL)
    {
        pt_def_part = pf_alloc(NULL, sizeof(MEM_PART_T));
    }

    if(pt_def_part != NULL)
    {
        pt_def_part->pf_alloc   = pf_alloc;
        pt_def_part->pf_realloc = pf_realloc;
        pt_def_part->pf_calloc  = pf_calloc;
        pt_def_part->pf_free    = pf_free;
    }

    /* Initialize remainder of default partition. */
}
#endif

/*
  This autofree function is used to delete binary_semaphore,
  mutex_semaphore, and counting_semaphore.
*/
static INT32 sema_handle_autofree
(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type
)
{
    INT32  i4_rc;

    i4_rc = x_sema_delete(h_handle) ;
    switch (i4_rc)
    {
        case OSR_OK:
        {
            i4_rc = HR_OK;
        }
        break;

        case OSR_INV_HANDLE:
        {
            i4_rc = HR_INV_HANDLE;
        }
        break;

        default:
        {
            i4_rc = HR_NOT_ALLOWED;
        }
    }
    return i4_rc;
}

static INT32 timer_handle_autofree
(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type
)
{
    INT32  i4_rc;

    i4_rc = x_timer_delete(h_handle) ;
    switch (i4_rc)
    {
        case OSR_OK:
        {
            i4_rc = HR_OK;
        }
        break;

        case OSR_INV_HANDLE:
        {
            i4_rc = HR_INV_HANDLE;
        }
        break;

        default:
        {
            i4_rc = HR_NOT_ALLOWED;
        }
    }
    return i4_rc;
}

/*
  Autofree function tables.  Note: for the OS handles that is not
  exported to application, we need to register "NULL" in the
  autofree function table.
*/
static handle_autofree_fct   apf_autofree_fct[]=
{
    (handle_autofree_fct) NULL,
    sema_handle_autofree,
    sema_handle_autofree,
    sema_handle_autofree,
    (handle_autofree_fct) NULL,
    timer_handle_autofree,
    (handle_autofree_fct) NULL
};


/*-----------------------------------------------------------------------------
 * Name: os_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
UINT16
os_get_dbg_level(VOID)
{
    if (b_os_mem_inited)
    {
        return(ui2_os_dbg_level);
    }
    else
    {
        return(DBG_LEVEL_NONE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: os_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 *          FALSE           Routine failed.
 ----------------------------------------------------------------------------*/
BOOL
os_set_dbg_level(UINT16 ui2_level)
{
    if (b_os_mem_inited)
    {
        ui2_os_dbg_level = ui2_level;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*-----------------------------------------------------------------------------
 * Name:  os_reg_cli
 *
 * Description: This API registers the SO library with the CLI module. The
 *              registration can only be done once the CLI module has been
 *              initialized, which does not happen before "os_init". Hence
 *              this additional CLI registration function.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: OSR_OK                   Routine successful.
 *          OSR_NOT_INIT             The handle library is not initialized yet.
 *          OSR_CANNOT_REG_WITH_CLI  registration with CLI failed.
 ----------------------------------------------------------------------------*/
INT32
os_reg_cli(VOID)
{
    INT32 i4_return;


    i4_return = OSR_NOT_INIT;

    if (b_os_mem_inited)
    {
        ui2_os_dbg_level = DBG_INIT_LEVEL_MW_OS;

#ifdef CLI_LVL_GUEST
#ifndef ANDROID
        i4_return = OSR_CANNOT_REG_WITH_CLI;

        /* And register the Debug library with the CLI module. */
        if (x_cli_attach_cmd_tbl (&(at_os_root_cmd_tbl[0]), CLI_CAT_MW, CLI_GRP_NONE) == CLIR_OK)
        {
            i4_return = OSR_OK;
        }
#else
        i4_return = OSR_OK;
#endif
#else
        i4_return = OSR_OK;
#endif  /* CLI_LVL_ALL */
    }

    return (i4_return);
}


/*----------------------------------------------------------------------------
 * Function: os_init()
 *
 * Description:
 *      This routine is called as part of system start up. it creates the
 *      default memory partition. it should be called once. it performs
 *      1. check arguments.
 *      2. create mem global struct.
 *      3. create default memory partition.
 *      4. call init routine of each OS module.
 *
 * Inputs:
 *      pv_addr: pointer to a memory block
 *      z_size: size of the memory block
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_INV_ARG: invalid argument
 *      OSR_NO_RESOURCE: no resource (memory).
 *      OSR_EXIST: this routine was called previously.
 *---------------------------------------------------------------------------*/
INT32
os_init(const VOID *pv_addr, SIZE_T z_size)
{
    VOID *pv_mem = NULL;
    MEM_PART_T *pt_part;
    INT32 i4_i;

    if (z_size == 0)
    {
        return(OSR_INV_ARG);
    }

    /* this routine shall only be called once */
    /* to create the default memory partition */
    if (pt_mem_global == NULL)
    {
        if (((sizeof(MEM_OBJ_T) % 4) != 0) || ((sizeof(MEM_OBJ_T) % 8) == 0))
        {
            /*
              The current algorithms assume:
              - sizeof(MEM_OBJ_T) is a multiple of 4
              - sizeof(MEM_OBJ_T) is not a multiple of 8
            */
            return OSR_INVALID;
        }

        if (pv_addr != NULL)
        {
            /* memory global is carved from the default memory partition */
            pv_mem = (VOID *) ((((UINT32) pv_addr) + 3) & ~0x3);     /* alignment */
            i4_i = (INT32) SIZE_MEM_GLOBAL_T + (INT32) ((UINT32) pv_mem - (UINT32) pv_addr);
            if (z_size <= (SIZE_T) i4_i)     /* not enough memory space */
            {
                return(OSR_NO_RESOURCE);
            }

            z_size -= (SIZE_T) i4_i;                 /* remaining memory */
            pt_mem_global = (MEM_GLOBAL_T *) pv_mem;
        }
        else
        {
            pt_mem_global = (MEM_GLOBAL_T*) x_mem_alloc (SIZE_MEM_GLOBAL_T);

            if (pt_mem_global == NULL)
            {
                return(OSR_NO_RESOURCE);
            }
        }

        b_os_mem_inited = FALSE;
        h_os_mem_gsema_hdl = NULL_HANDLE;

        /* memory after memory global */
        if (pv_addr != NULL)
        {
            pv_mem = (VOID *) (((UINT32) pv_mem) + SIZE_MEM_GLOBAL_T);

            /* if pv_mem is aligned on 8 bytes, add 4 bytes so the address is aligned on 4 bytes but not 8 */
            if ((((UINT32) pv_mem) % 8) == 0)
            {
                pv_mem = (VOID*) (((CHAR*) pv_mem) + 4);
                z_size -= 4;
            }
        }

        /* link inactive partition table entries */
        pt_part = (MEM_PART_T *) &(pt_mem_global->t_part_tbl[0]);
        pt_mem_global->pt_free = pt_part;
        pt_mem_global->pt_active = NULL;

        for (i4_i = 0; i4_i < OS_MEM_PART_NUM; i4_i++)
        {
            pt_part->pt_next = (pt_part + 1);
            pt_part->pt_sub_part = NULL;
            pt_part->h_part_hdl = NULL_HANDLE;
            pt_part->ui1_part_index = (UINT8) i4_i;
            pt_part->ui1_flags = OS_MEM_PART_FLAG_IDLE;
            ++pt_part;
        }

        /* pt_part_tbl is out bound */
        --pt_part;
        pt_part->pt_next = NULL;

        /* create the default memory partition */
        if (pv_addr != NULL)
        {
            i4_i = os_mem_part_create(&h_def_part_hdl,
                                      pv_mem,
                                      z_size,
                                      OS_MEM_DYNAMIC_ALLOC,
                                      DEF_PART_NAME);
        }
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
        else
        {
            /* initialize semaphore MHF Linux - Bin Zhang */
            if (sema_init() != OSR_OK)
            {
                return(OSR_NO_RESOURCE);
            }
            i4_i = OSR_OK;
        }
#endif

        if (i4_i == OSR_OK)
        {
            /* create memory global semaphore */
            i4_i = x_sema_create(&(h_os_mem_gsema_hdl),
                                 X_SEMA_TYPE_BINARY,
                                 X_SEMA_STATE_UNLOCK);

            if (i4_i == OSR_OK)
            {
                /* now initialize other modules */
                if (thread_init() != OSR_OK)
                {
                    i4_i = OSR_NO_RESOURCE;
                }
                if (timer_init() != OSR_OK)
                {
                    i4_i = OSR_NO_RESOURCE;
                }
                if (msg_q_init() != OSR_OK)
                {
                    i4_i = OSR_NO_RESOURCE;
                }

                if (ev_grp_init() != OSR_OK)
                {
                    i4_i = OSR_NO_RESOURCE;
                }
            }
            else
            {
                i4_i = OSR_NO_RESOURCE;
            }

            if (i4_i == OSR_OK)
            {
                /* register autofree functions for the OS module handle. */
                if ( handle_set_autofree_tbl(HT_GROUP_OS,
                                             apf_autofree_fct)
                     != HR_OK )
                {
                    OS_DBG_ABORT(DBG_CAT_INV_OP,DBG_CANNOT_SET_AUTOFREE);
                }
                b_os_mem_inited = TRUE;
            }
            else
            {
                pt_mem_global = NULL;
            }

            return(i4_i);
        }
        else
        {
            pt_mem_global = NULL;
            return(OSR_NO_RESOURCE);
        }
    }
    else
    {
        /* default memory partition established */
        /* i.e. os_init() called before */
        return(OSR_EXIST);
    }
}


/*----------------------------------------------------------------------------
 * Function: os_mem_part_check()
 *
 * Description:
 *      This routine is called to check if a partition can be created.
 *      it performs
 *      1. check if there is a name collision.
 *      2. check if start address and its size do not overlap with an
 *         existing partition.
 *      3. return not support if partition is to be created from an
 *         existing partition of fixed allocation scheme.
 *      4. check start address and its size are legal that was allocated
 *         from an existing partition.
 *
 * Inputs:
 *      pv_start: starting address for a partition.
 *      z_size: size of the memory block
 *      ps_name: partition name, null ('\0') terminated, up to 16
 *               characters excluding '\0' will be used.
 *
 * Outputs: -
 *      ppt_part: pointer to hold returned mem_part struct.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_EXIST: partition exists.
 *      OSR_OUT_BOUND: memory overlap with other partitions.
 *      OSR_NOT_SUPPORT: this type of creation not support.
 *---------------------------------------------------------------------------*/
INT32
os_mem_part_check(MEM_PART_T **ppt_part,
                  VOID       *pv_start,
                  SIZE_T     z_size,
                  const CHAR *ps_name)
{
    VOID *pv_end1, *pv_end2;
    MEM_PART_T *pt_part, *pt_want;
    INT32 i4_ret;

    pt_want = NULL;
    *ppt_part = NULL;
    pt_part = pt_mem_global->pt_active;

    /* end of to-be created partition */
    pv_end2 = (VOID *) (((UINT32) pv_start) + z_size);
    while (pt_part != NULL)
    {
        if (x_strncmp(pt_part->s_name, ps_name, PART_NAME_LEN) == 0) /* exist */
        {
            *ppt_part = pt_part;
            return(OSR_EXIST);
        }

        /* check memory range either it is outside all existing partitions */
        /* or it is within the range of the closest existing partition */
        /* if (pv_start != NULL) */
        {
            /* end of pt_part */
            pv_end1 = (VOID *) (((UINT32) pt_part->pv_start) + pt_part->z_size);
            if (((UINT32) pv_start >= (UINT32) pv_end1) ||
                ((UINT32) pv_end2 <= (UINT32) (pt_part->pv_start)))
            {
                /* the to-be-created partition is not overlaping */
                /* with this partition */
                pt_part = pt_part->pt_next;
            }
            else if (((UINT32) pv_start >= (UINT32) (pt_part->pv_start)) &&
                     ((UINT32) pv_end2 < (UINT32) pv_end1))
            {
                /* the to-be-created partition falls into */
                /* the range of this partition */
                pt_want = pt_part;
                pt_part = pt_part->pt_sub_part;
            }
            else
            {
                /* the to-be-created partition is partially overlapped */
                /* with this partition, but it goes beyond the end */
                return(OSR_OUT_BOUND);
            }
        }
    }

    if (pt_want != NULL)
    {
        if (pt_want->z_alloc_size == (SIZE_T) OS_MEM_DYNAMIC_ALLOC)
        {
            MEM_FREEOBJ_T *pt_obj;

            /* this is the closest partition that the new partition is */
            /* to be created from within */
            i4_ret = x_sema_lock(pt_want->h_sema_hdl, X_SEMA_OPTION_WAIT);
            if (i4_ret != OSR_OK)
            {
                return(OSR_FAIL);
            }

            pt_obj = pt_want->pt_first->pt_bkend;
            i4_ret = OSR_OUT_BOUND;
            while (pt_obj != pt_want->pt_last)
            {
                if ((((UINT32) pt_obj->pt_ftend) & 0x3) ||
                    (((UINT32) pt_obj->pt_bkend) & 0x3) ||
                    (pt_obj->ui1_index >= (UINT8) OS_MEM_BUCKET_SIZE) ||
                    (pt_obj->ui1_part_index != pt_want->ui1_part_index) ||
                    ((pt_obj->ui2_flags != OS_MEM_FLAG_FREE) &&
                     (pt_obj->ui2_flags != OS_MEM_FLAG_ALLOCATED)))
                {
                    /* the memory object is probably corrupted */
                    break;
                }

                if (((UINT32) pv_start >= ((UINT32) &(pt_obj->pt_next))) &&
                    ((UINT32) pv_end2 <= (UINT32) (pt_obj->pt_bkend)))
                {
                    /* pv_start and z_size are both valid */
                    i4_ret = OSR_OK;
                    *ppt_part = pt_want;
                    break;
                }

                pt_obj = pt_obj->pt_bkend;
            }

            (VOID) x_sema_unlock(pt_want->h_sema_hdl);

            return(i4_ret);
        }
        else
        {
            /* not support creating a memory partition within */
            /* an existing partition of fixed allocation */
            return(OSR_NOT_SUPPORT);
        }
    }

    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: os_mem_part_create()
 *
 * Description:
 *      This routine is called to check if a partition can be created.
 *      it performs
 *      1. call os_mem_part_check() for pre-qualification check.
 *      2. allocate a handle.
 *      3. create 2 dummy mem objs: one at the beginning, one at the end.
 *         they are marked as allocated. this helps memory coalescing.
 *      4. create and initialize partition bucket array.
 *      5. assign pf_alloc and pf_free based on z_alloc_size value.
 *      6. for fixed allocation, divide partition memory into blcoks of
 *         size "z_alloc_size" and link them in a list.
 *      7. for dynamic allocation, set up proper pt_ftend and pt_bkend, and
 *         put the memory obj (containing all remaining memory as free memory)
 *         into its bucket based on the obj size.
 *      8. create a partition semaphore.
 *
 * Inputs:
 *      pv_start: starting address for a partition.
 *      z_size: size of the memory block
 *      z_alloc_size: allocation size. 0: dynamic size.
 *      ps_name: partition name, null ('\0') terminated, up to 16
 *               characters excluding '\0' will be used.
 *
 * Outputs:
 *      ph_part_hdl: pointer to hold returnedpartition handle.
 *                   the handle is valid only if this API returns OSR_OK.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_EXIST: partition exists.
 *      OSR_OUT_BOUND: memory overlap with other partitions.
 *      OSR_NOT_SUPPORT: this type of creation not support.
 *      OSR_NO_RESOURCE: no resource (memory).
 *      OSR_TOO_MANY: too many partitions created.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: invalid handle.
 *      OSR_OUT_OF_HANDLES: no handle.
 *---------------------------------------------------------------------------*/
INT32
os_mem_part_create(HANDLE_T    *ph_part_hdl,
                   const VOID  *pv_start,
                   SIZE_T      z_size,
                   SIZE_T      z_alloc_size,
                   const CHAR  *ps_name)
{
    VOID *pv_tmp;
    MEM_FREEOBJ_T *pt_obj;
    MEM_PART_T *pt_part, *pt_parent;
    MEM_BUCKET_T *pt_bucket;
    INT32 i4_ret, i4_i;


    i4_ret = os_mem_part_check(&pt_parent, (VOID *) pv_start, z_size, ps_name);
    if (i4_ret != OSR_OK)
    {
        return(i4_ret);
    }

    if ((pt_part = pt_mem_global->pt_free) != NULL)
    {
        pt_mem_global->pt_free = pt_part->pt_next;
        pt_part->pt_next = NULL;
        pt_part->pt_sub_part = NULL;

        pv_tmp = (VOID *) ((((UINT32) pv_start) + 3) & ~0x3);
        z_alloc_size = (z_alloc_size + 3) & ~0x3;   /* multiple of INT32 */

        /* each memory partition has the following layout */
        /* partition struct + partition space */
        {
            /* create a handle for this partition */
            i4_ret = handle_alloc(HT_GROUP_OS_MEMORY,
                                  pt_part,
                                  pt_part,
                                  mem_free_cb,
                                  &(pt_part->h_part_hdl));

            if (i4_ret != HR_OK)
            {
                pt_part->pt_next = pt_mem_global->pt_free;
                pt_mem_global->pt_free = pt_part;
                return(HR_TO_OSR(i4_ret));
            }

            if (pt_parent != NULL)
            {
                /* link this partition to the sub_part list */
                pt_part->ui1_flags = OS_MEM_PART_FLAG_ACTIVE;
                pt_part->pt_next = pt_parent->pt_sub_part;
                pt_parent->pt_sub_part = pt_part;
            }
            else
            {
                /* link this partition to the unrelated part list */
                pt_part->ui1_flags = OS_MEM_PART_FLAG_EXTERNAL;
                pt_part->pt_next = pt_mem_global->pt_active;
                pt_mem_global->pt_active = pt_part;
            }

            *ph_part_hdl = pt_part->h_part_hdl;
            x_strncpy(pt_part->s_name, ps_name, PART_NAME_LEN);
            pt_part->s_name[PART_NAME_LEN] = '\0';
            pt_part->b_sema_inited = FALSE;
            pt_part->pv_start = (VOID *) pv_start;
            pt_part->pv_mem = pv_tmp;       /* start of free, allocable mem */

            /* Bug-48  ffr  2005-06-10. Ensure that the memory chunk size is always a multiple of 4 Bytes. */
            pt_part->z_size = (z_size - (SIZE_T) ((UINT32) pv_tmp - (UINT32) pv_start)) & (~((SIZE_T) 3));
            pt_part->z_alloc_size = z_alloc_size;
            pt_part->ui4_alloc_obj = 0;     /* statistics */
            pt_part->ui4_free_obj = 0;      /* statistics */

            /* reset the free buckets */
            for (i4_i = OS_MEM_FIXED_INDEX,
                 pt_bucket = &(pt_part->t_bucket[i4_i]);
                    i4_i < OS_MEM_BUCKET_SIZE; i4_i++, pt_bucket++)
            {
                SET_LINK_HEAD(pt_bucket->pt_head, NULL);
                SET_LINK_TAIL(pt_bucket->pt_tail, NULL);
            }

            if (z_alloc_size == OS_MEM_DYNAMIC_ALLOC)
            {
                /* dynamic memory allocation */
                MEM_FREEOBJ_T *pt_front, *pt_last;

                pt_part->pf_alloc = (os_mem_alloc_fct) os_mem_alloc;
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
                pt_part->pf_realloc = (os_mem_realloc_fct) os_mem_realloc;
                pt_part->pf_calloc = (os_mem_calloc_fct) os_mem_calloc;
#endif
                pt_part->pf_free = (os_mem_free_fct) os_mem_merge_obj;

                /* create two dummy blocks and never free */
                /* so that coalescing (de-frag) will not be performed */
                /* on these two blocks */
                /* dummy block at the end */
                pv_tmp = (VOID *) (((UINT32) (pt_part->pv_mem)) +
                                                pt_part->z_size);
                pt_last = (MEM_FREEOBJ_T *) (((UINT32) pv_tmp) -
                                                    sizeof(MEM_OBJ_T));
                pt_part->pt_last = pt_last;
                pt_last->pt_ftend = pt_last->pt_bkend = NULL;
                pt_last->ui1_part_index = pt_part->ui1_part_index;
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
                pt_last->ui2_padding3 = 0xFFFF;
#endif
                pt_last->ui2_flags = OS_MEM_FLAG_ALLOCATED;
                pt_part->z_size -= sizeof(MEM_OBJ_T); /* decrement total size */

                /* dummy block in the beginning */
                pt_front = (MEM_FREEOBJ_T *) pt_part->pv_mem;
                pt_part->pt_first = pt_front;
                pt_front->pt_ftend = pt_front->pt_bkend = NULL;
                pt_front->ui1_part_index = pt_part->ui1_part_index;
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
                pt_front->ui2_padding3 = 0xFFFF;
#endif
                pt_front->ui2_flags = OS_MEM_FLAG_ALLOCATED;
                pt_part->z_size -= (sizeof(MEM_OBJ_T) + 7) & ~0x7;
                pt_part->ui4_alloc_obj = 2;     /* statistics */

                /* put the entire free memory space into its bucket */
                /* construct a memory object first */
                /* de-frag info */
                pt_obj = (MEM_FREEOBJ_T *) (((UINT32) (pt_part->pv_mem)) +
                                                        ((sizeof(MEM_OBJ_T) + 7) & ~0x7));
                pt_front->pt_bkend = pt_obj;
                pt_obj->pt_ftend = pt_front;
                pt_obj->pt_bkend = pt_last;
                pt_obj->ui1_part_index = pt_part->ui1_part_index;
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
                pt_obj->ui2_padding3 = 0xFFFF;
#endif
                pt_obj->ui2_flags = OS_MEM_FLAG_FREE;

                /* free memory info */
                pt_obj->z_size = pt_part->z_size;
                pt_obj->ui1_index = os_mem_bucket_index(pt_obj->z_size);
                pt_part->ui1_min_index = pt_obj->ui1_index;
                pt_part->ui1_max_index = pt_obj->ui1_index;

                os_mem_put_to_bucket(pt_part, pt_obj);
                pt_part->z_size = z_size;
            }
            else
            {
                /* fixed memory allocation */
                pt_part->pf_alloc = (os_mem_alloc_fct) os_mem_fix_alloc;
                pt_part->pf_free = (os_mem_free_fct) os_mem_fix_free;
                pt_part->ui1_min_index = pt_part->ui1_max_index = OS_MEM_FIXED_INDEX;
                pt_part->pt_first = pt_part->pt_last = NULL;

                /* chop the partition memory into fixed blocks */
                /* and links these memory blocks */
                z_alloc_size += OS_MEM_STATUS_SIZE;
                if (pt_part->z_size >= z_alloc_size)
                {
                    pv_tmp = pt_part->pv_mem;
                    while (pt_part->z_size >= z_alloc_size)
                    {
                        ((MEM_BLK_T *) pv_tmp)->ui1_part_index =
                                                        pt_part->ui1_part_index;
                        ((MEM_BLK_T *) pv_tmp)->ui1_index = OS_MEM_FIXED_INDEX;
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
                        ((MEM_BLK_T *) pv_tmp)->ui2_padding3 = 0xFFFF;
#endif
                        ((MEM_BLK_T *) pv_tmp)->ui2_flags = OS_MEM_FLAG_FREE;
                        ((MEM_BLK_T *) pv_tmp)->pt_next = (MEM_BLK_T *)
                                            (((UINT32) pv_tmp) + z_alloc_size);

                        pv_tmp = (VOID *) (((UINT32) pv_tmp) + z_alloc_size);
                        pt_part->z_size -= z_alloc_size;
                        pt_part->ui4_free_obj++;        /* statistics */
                    }

                    /* pv_tmp is out of range, back off one block */
                    pv_tmp = (VOID *) (((UINT32) pv_tmp) - z_alloc_size);
                    ((MEM_BLK_T *) pv_tmp)->pt_next = NULL;
#if 0
                    SET_LINK_HEAD(pt_part->fix_bucket.pt_head, pt_part->pv_mem);
                    SET_LINK_TAIL(pt_part->fix_bucket.pt_tail, pv_tmp);
                    pt_part->fix_bucket.pt_head = (MEM_FREEOBJ_T *) pt_part->pv_mem;
#endif
                    SET_LINK_HEAD(pt_part->fix_bucket.pt_head, (MEM_FREEOBJ_T *) pt_part->pv_mem);
                    SET_LINK_TAIL(pt_part->fix_bucket.pt_tail, (MEM_FREEOBJ_T *) pv_tmp);
                }
                pt_part->z_size = z_size;
            }

            if (pt_def_part == NULL)
            {
                pt_def_part = pt_part;
                pt_part->ui1_flags = OS_MEM_PART_FLAG_PERMANENT;

                /* initialize semaphore */
                if (sema_init() != OSR_OK)
                {
                    pt_mem_global = NULL;
                    pt_def_part = NULL;
                    return(OSR_NO_RESOURCE);
                }
            }

            /* create a memory semaphore for this partition */
            /* used to be MUTEX seamphore */
            /* but the code is structured so that BINARY is fine */
            i4_ret = x_sema_create(&(pt_part->h_sema_hdl),
                                    X_SEMA_TYPE_BINARY,     /* was MUTEX */
                                    X_SEMA_STATE_UNLOCK);

            if (i4_ret == OSR_OK)
            {
                pt_part->b_sema_inited = TRUE;
                return(OSR_OK);
            }
            else
            {
                pt_part->b_sema_inited = FALSE;
                return(OSR_FAIL);
            }
        }
    }
    else
    {
        return(OSR_TOO_MANY);   /* too many memory partitions */
    }
}


/*----------------------------------------------------------------------------
 * Function: os_mem_part_delete()
 *
 * Description:
 *      This routine is called to delete a partition. Note that the memory
 *      for the partition will not be freed and should be freed by a caller.
 *      it performs
 *      1. check if the partition exists.
 *      2. find its originating partition.
 *      3. isolate this partition by linking other partitions.
 *      4. delete its sub partitions.
 *      5. free this partition's handle.
 *      6. link this partition to free partition list.
 *
 * Inputs:
 *      pt_part: pointer to a mem_part struct.
 *
 * Outputs:
 *      None
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_EXIST: partition does not exist.
 *      OSR_MEM_NOT_FREE: partition is deleted but its memory has not freed.
 *---------------------------------------------------------------------------*/
INT32
os_mem_part_delete(MEM_PART_T *pt_part)
{
    MEM_PART_T *pt_tmp, *pt_prev;
    INT32 i4_i;

    if ((pt_part->ui1_flags != OS_MEM_PART_FLAG_ACTIVE) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_EXTERNAL) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_PERMANENT))
    {
        return(OSR_NOT_EXIST);
    }

    pt_tmp = NULL;
    if (pt_part->ui1_flags == OS_MEM_PART_FLAG_EXTERNAL)
    {
        pt_prev = NULL;
        pt_tmp = pt_mem_global->pt_active;
        while (pt_tmp != NULL)
        {
            if (pt_tmp == pt_part)
            {
                if (pt_prev == NULL)
                {
                    pt_mem_global->pt_active = pt_tmp->pt_next;
                }
                else
                {
                    pt_prev->pt_next = pt_tmp->pt_next;
                }

                break;
            }
            else
            {
                pt_prev = pt_tmp;
                pt_tmp = pt_tmp->pt_next;
            }
        }
    }

    if (pt_tmp == NULL)
    {
        i4_i = 0;
        pt_tmp = (MEM_PART_T *) &(pt_mem_global->t_part_tbl[i4_i]);
        while (i4_i < OS_MEM_PART_NUM)
        {
            if (/* (pt_tmp == pt_part) || */
                (pt_tmp->pt_next == pt_part) ||
                (pt_tmp->pt_sub_part == pt_part))
            {
                break;
            }
            else
            {
                ++pt_tmp;
                ++i4_i;
            }
        }

        if (i4_i >= OS_MEM_PART_NUM)
        {
            return(OSR_NOT_EXIST);
        }

        /* isolate this partition */
        if (pt_tmp->pt_sub_part == pt_part)
        {
            pt_tmp->pt_sub_part = pt_part->pt_next;
        }

        if (pt_tmp->pt_next == pt_part)
        {
            pt_tmp->pt_next = pt_part->pt_next;
        }
    }

    /* delete all sub partitions */
    pt_tmp = pt_part->pt_sub_part;
    if (pt_tmp != NULL)
    {
        (VOID) os_mem_sub_part_delete(pt_tmp);
        pt_part->pt_sub_part = NULL;
    }

    pt_part->ui1_flags = OS_MEM_PART_FLAG_DELETE;
    handle_free(pt_part->h_part_hdl, TRUE);

    pt_part->pt_next = pt_mem_global->pt_free;
    pt_mem_global->pt_free = pt_part;

    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: os_mem_sub_part_delete()
 *
 * Description:
 *      This routine is called to delete a partition. it performs
 *      1. chase down all sub partitions.
 *      2. mark at each level of sub partition as deleted.
 *      3. free each partition's handle.
 *      4. link this partition (list)  to free partition list.
 *
 * Inputs:
 *      pt_part: pointer to a mem_part struct.
 *
 * Outputs:
 *      None
 *
 * Returns:
 *      None
 *---------------------------------------------------------------------------*/
VOID
os_mem_sub_part_delete(MEM_PART_T *pt_part)
{
    MEM_PART_T *pt_tmp, *pt_prev;


    /* chase down all children partitions */
    pt_tmp = pt_part->pt_sub_part;
    if (pt_tmp != NULL)
    {
        os_mem_sub_part_delete(pt_tmp);

        pt_part->pt_sub_part = NULL;
    }

    /* mark all clildren partitions as deleted */
    pt_tmp = pt_part;
    while (pt_tmp != NULL)
    {
        pt_tmp->ui1_flags = OS_MEM_PART_FLAG_DELETE;

        pt_tmp = pt_tmp->pt_next;
    }

    pt_prev = NULL;
    pt_tmp = pt_part;
    while (pt_tmp != NULL)
    {
        handle_free(pt_tmp->h_part_hdl, TRUE);

        pt_prev = pt_tmp;
        pt_tmp = pt_tmp->pt_next;
    }

    if (pt_prev != NULL)
    {
        pt_prev->pt_next = pt_mem_global->pt_free;
        pt_mem_global->pt_free = pt_part;
    }
}

/*----------------------------------------------------------------------------
 * Function: os_mem_get_mem_ptr_size
 *
 * Description:
 *      This routine returns the actual size of a memory chunk.
 *
 * Inputs:
 *      pv_mem: pointer to a memory chunk allocated using x_mem_alloc for
 *              instance.
 *
 * Outputs: -
 *
 * Returns:
 *      actual size allocated.
 *---------------------------------------------------------------------------*/
SIZE_T  os_mem_get_mem_ptr_size (const VOID*  pv_mem)
{
    const MEM_OBJ_T*  pt_mem_obj;
    const CHAR*       pc_next_ptr;
    SIZE_T            z_size;

    pt_mem_obj = ((const MEM_OBJ_T*) pv_mem) - 1;

    pc_next_ptr = (const CHAR*) (pt_mem_obj->pt_bkend);

    z_size = (SIZE_T) (pc_next_ptr - (const CHAR*)pv_mem);

    return z_size;
}

/*----------------------------------------------------------------------------
 * Function: os_mem_alloc()
 *
 * Description:
 *      This routine is called to actually allocaate a memory block of
 *      dynamic size. it performs
 *      1. add mem_obj struct overhead and rounded to be multiple of INT32.
 *      2. locate bucket index based on size.
 *      3. adjust bucket index to min. bucket index if needed.
 *      4. start from adjusted bucket index, go throught bucket list to
 *         find the first memory obj that satisfies requested size.
 *      5. delink that memory obj, if the remaining memory sizee after
 *         allocation is greater than 32 bytes, construct a mem_obj header
 *         at the beginning of remaining memory and set up proper pt_ftend
 *         and pt_bkend pointers, put the remaining memory as a memory obj
 *         back into its bucket entry.
 *
 * Inputs:
 *      pt_part: pointer to a mem_part struct.
 *      z_size: size of the memory block request.
 *
 * Outputs: -
 *
 * Returns:
 *      non NULL: routine was successful.
 *      NULL: routine failed.
 *---------------------------------------------------------------------------*/
VOID *
os_mem_alloc(MEM_PART_T *pt_part, SIZE_T z_size)
{
    MEM_FREEOBJ_T *pt_obj, *pt_obj2;
    MEM_BUCKET_T *pt_bucket;
    VOID *pv_mem;
    INT32 i4_tmp, i4_index, i4_ret;
    SIZE_T z_left;

    if ((pt_part->ui1_flags != OS_MEM_PART_FLAG_ACTIVE) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_EXTERNAL) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_PERMANENT))
    {
        return(NULL);
    }

    /* MEM_OBJ_T contains the permant object info and is located */
    /* ahead of memory allocated */
    /* MEM_FREEOBJ_T has 2 portions: the first portion is identical */
    /* to MEM_OBJ_T, the second portion has dynamic info for free memory */
    /* used in free bucket */
    /* make the size of multiple of INT64 */
    z_size = (z_size + sizeof(MEM_OBJ_T) + 7) & ~0x7;
    if (z_size < OS_MEM_MIN_OBJ_SIZE)      /* min. allocated mem */
    {
        z_size = OS_MEM_MIN_OBJ_SIZE;
    }

    /* locate the bucket for the requested size */
    i4_tmp = i4_index = (INT32) os_mem_bucket_index(z_size);

    if (pt_part->b_sema_inited == TRUE)
    {
        i4_ret = x_sema_lock(pt_part->h_sema_hdl, X_SEMA_OPTION_WAIT);
        if (i4_ret != OSR_OK)
        {
            return(NULL);
        }
    }

    /* absolutely cannot satisfy */
    if (i4_tmp > pt_part->ui1_max_index)
    {
        x_sema_unlock(pt_part->h_sema_hdl);
        return(NULL);
    }

    /* start from the min. bucket */
    if (i4_tmp < pt_part->ui1_min_index)
    {
        i4_tmp = pt_part->ui1_min_index;
    }

    /* work upward to max index */
    pv_mem = (VOID *) NULL;
    pt_bucket = &(pt_part->t_bucket[i4_tmp]);
    while (i4_tmp <= pt_part->ui1_max_index)
    {
        /* quick check if this bucket can satisfy the request */
        pt_obj =  NULL;
        if ((LINK_NOT_EMPTY(pt_bucket->pt_tail, NULL)) &&
            ((i4_tmp == pt_part->ui1_max_index) || (z_size <= pt_bucket->pt_tail->z_size)))    /* can satisfy */
        {
            GET_LINK_HEAD(pt_obj, pt_bucket->pt_head);
            do
            {
                if (z_size <= pt_obj->z_size)           /* get this one */
                {
                    pv_mem = (VOID *) pt_obj;
                    break;
                }
                GET_LINK_NEXT(pt_obj, pt_obj);
            } while (pt_obj != pt_bucket->pt_head);
        }

        if (pv_mem == NULL)
        {
            ++i4_tmp;
            ++pt_bucket;        /* not found yet, go onto next bucket */
        }
        else
        {
            /*
             * if size left is more than the min. size
             * get a free memory obj and put the left memory in its bucket
             */
            os_mem_remove_from_bucket(pt_part, pt_obj);
            pt_part->ui4_alloc_obj++;
            z_left = pt_obj->z_size - z_size;
            if (z_left >= OS_MEM_MIN_OBJ_SIZE)
            {
                /* construct a new MEM_FREEOBJ_T (pt_obj2) for the left space */
                /* pt_obj holds memory to be allocated */
                /* pt_obj2 holds free memory */
                pt_obj2 = (MEM_FREEOBJ_T *) (((UINT32) pv_mem) + z_size);

                pt_obj2->ui1_part_index = pt_part->ui1_part_index;
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
                pt_obj2->ui2_padding3 = 0xFFFF;
#endif
                pt_obj2->ui2_flags = OS_MEM_FLAG_FREE;

                /* for coalescing (de-frag): */
                /* pt_obj is the obj before (front end) pt_obj2 */
                /* back end is the obj behinds pt_obj2 */
                pt_obj2->pt_ftend = pt_obj;
                pt_obj2->pt_bkend = pt_obj->pt_bkend;
                pt_obj->pt_bkend->pt_ftend = pt_obj2;
                pt_obj->pt_bkend = pt_obj2;
                pt_obj->ui1_index = (UINT8) i4_index;
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
                pt_obj->ui2_padding3 = 0xFFFF;
#endif
                pt_obj2->z_size = z_left;
                pt_obj2->ui1_index = os_mem_bucket_index(z_left);
                os_mem_put_to_bucket(pt_part, pt_obj2);
            }
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
            pt_obj->ui2_padding3 = 0xFFFF;
#endif
            pt_obj->ui2_flags = OS_MEM_FLAG_ALLOCATED;
            pv_mem = (VOID *) (((UINT32) pv_mem) + sizeof(MEM_OBJ_T));

            break;
        }
    }

    if (pt_part->b_sema_inited == TRUE)
    {
        (VOID) x_sema_unlock(pt_part->h_sema_hdl);
    }

    return(pv_mem);
}


/*----------------------------------------------------------------------------
 * Function: os_mem_fix_alloc()
 *
 * Description:
 *      This routine is called to actually allocaate a memory block of
 *      fixed size. it performs
 *      1. guard the memory obj list.
 *      2. delink a memory obj from the beginning of list.
 *
 * Inputs:
 *      pt_part: pointer to a mem_part struct.
 *      z_size: size of the memory block request.
 *
 * Outputs: -
 *
 * Returns:
 *      non NULL: routine was successful.
 *      NULL: routine failed.
 *---------------------------------------------------------------------------*/
VOID *
os_mem_fix_alloc(MEM_PART_T *pt_part, SIZE_T z_size)
{
    MEM_BLK_T *pt_blk = NULL;
    INT32 i4_ret;

    if ((pt_part->ui1_flags != OS_MEM_PART_FLAG_ACTIVE) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_EXTERNAL) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_PERMANENT))
    {
        return(NULL);
    }

    if (z_size > pt_part->z_alloc_size)
    {
        return(NULL);
    }

    i4_ret = x_sema_lock(pt_part->h_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return(NULL);
    }

    /* get from the free head */
    GET_LINK_HEAD(pt_blk, (MEM_BLK_T *) (pt_part->fix_bucket.pt_head));
    if (LINK_NOT_EMPTY(pt_blk, NULL))
    {
        pt_part->ui4_alloc_obj++;
        pt_part->ui4_free_obj--;
        SET_LINK_HEAD(pt_part->fix_bucket.pt_head,
                      (MEM_FREEOBJ_T *) (pt_blk->pt_next));

        /* adjust tail if necessary */
        if (LINK_IS_EMPTY(pt_part->fix_bucket.pt_head, NULL))
        {
            SET_LINK_TAIL(pt_part->fix_bucket.pt_tail, NULL);
        }
    }

    (VOID) x_sema_unlock(pt_part->h_sema_hdl);

    if (pt_blk)
    {
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
        pt_blk->ui2_padding3 = 0xFFFF;
#endif
        pt_blk->ui2_flags = OS_MEM_FLAG_ALLOCATED;
        return((VOID *) &(pt_blk->pt_next));
    }
    else
    {
        return(NULL);
    }
}


/*----------------------------------------------------------------------------
 * Function: os_mem_fix_free()
 *
 * Description:
 *      This routine is called to free a memory block of fixed size. it performs
 *      1. guard the memory obj list.
 *      2. link the  memory obj at the beginning of list.
 *
 * Inputs:
 *      pt_part: pointer to a mem_part struct.
 *      pv_mem: memory address to be freed.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
os_mem_fix_free(MEM_PART_T *pt_part, VOID *pv_mem)
{
    MEM_BLK_T *pt_blk;
    INT32 i4_ret;

    if ((pt_part->ui1_flags != OS_MEM_PART_FLAG_ACTIVE) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_EXTERNAL) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_PERMANENT))
    {
        return;
    }

    pt_blk = (MEM_BLK_T *) ((UINT32) pv_mem - (UINT32) OS_MEM_STATUS_SIZE);
    if (pt_blk->ui1_index != OS_MEM_FIXED_INDEX)
    {
        OS_DBG_ABORT(DBG_CAT_MEMORY, DBG_MEM_BUCKET_INDEX_CORRUPT);
    }
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
    pt_blk->ui2_padding3 = 0xFFFF;
#endif
    pt_blk->ui2_flags = OS_MEM_FLAG_FREE;

    i4_ret = x_sema_lock(pt_part->h_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return;
    }

    pt_part->ui4_alloc_obj--;
    pt_part->ui4_free_obj++;

    SET_LINK_NEXT(pt_blk, (MEM_BLK_T *) (pt_part->fix_bucket.pt_head));
    SET_LINK_HEAD(pt_part->fix_bucket.pt_head, (MEM_FREEOBJ_T *) pt_blk);

    if (LINK_IS_EMPTY(pt_part->fix_bucket.pt_tail, NULL))
    {
        SET_LINK_TAIL(pt_part->fix_bucket.pt_tail, (MEM_FREEOBJ_T *) pt_blk);
    }

    (VOID) x_sema_unlock(pt_part->h_sema_hdl);
}


/*----------------------------------------------------------------------------
 * Function: os_mem_remove_from_bucket()
 *
 * Description:
 *      This routine is called to delink a memory obj from its bucket.
 *      it performs
 *      1. loacte bucket.
 *      2. delink the memory obj from the bucket list according to its
 *         position in the list.
 *      3. adjust partition min and max bucket index.
 *
 * Inputs:
 *      pt_part: pointer to a mem_part struct.
 *      pv_obj: pointer to a memory obj.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
os_mem_remove_from_bucket(MEM_PART_T *pt_part, MEM_FREEOBJ_T *pt_obj)
{
    MEM_BUCKET_T *pt_bucket;

    if ((pt_part->ui1_flags != OS_MEM_PART_FLAG_ACTIVE) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_EXTERNAL) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_PERMANENT))
    {
        return;
    }

    /* pt_obj has a bucket index set at memory allocation time */
    pt_part->ui4_free_obj--;
    pt_bucket = &(pt_part->t_bucket[pt_obj->ui1_index]);

    if (pt_bucket->pt_head == pt_bucket->pt_tail)
    {
        /* the only obj in this bucket */
        SET_LINK_HEAD(pt_bucket->pt_head, NULL);
        SET_LINK_TAIL(pt_bucket->pt_tail, NULL);

        if (pt_part->ui1_min_index == pt_obj->ui1_index)
        {
            pt_part->ui1_min_index = pt_obj->ui1_index + (UINT8) 1;
        }
        else if (pt_part->ui1_max_index == pt_obj->ui1_index)
        {
            pt_part->ui1_max_index = pt_obj->ui1_index - (UINT8) 1;
        }

        if (pt_part->ui1_max_index < pt_part->ui1_min_index)
        {
            pt_part->ui1_min_index = pt_part->ui1_max_index;
        }
    }
    else
    {
        SET_LINK_NEXT(pt_obj->pt_prev, pt_obj->pt_next);
        SET_LINK_PREV(pt_obj->pt_next, pt_obj->pt_prev);

        if (pt_obj == pt_bucket->pt_head)
        {
            SET_LINK_HEAD(pt_bucket->pt_head, pt_obj->pt_next);
        }
        else if (pt_obj == pt_bucket->pt_tail)
        {
            SET_LINK_TAIL(pt_bucket->pt_tail, pt_obj->pt_prev);
        }
    }
}


/*----------------------------------------------------------------------------
 * Function: os_mem_merge_obj()
 *
 * Description:
 *      This routine is called to perform memory coalescing. it performs
 *      1. get memory obj struct from pv_mem.
 *      2. use pt_ftend to merger to memory obj before this memory obj
 *         if front memory obj is not allocated, put merged memory obj
 *         back to its bucket.
 *      3. use pt_bkend to merger to memory obj behind this memory obj
 *         if back memory obj is not allocated, put merged memory obj
 *         back to its bucket.
 *
 * Inputs:
 *      pt_part: pointer to a mem_part struct.
 *      pv_mem: memory address to be freed.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
os_mem_merge_obj(MEM_PART_T *pt_part, VOID *pv_mem)
{
    MEM_FREEOBJ_T *pt_obj, *pt_tmp;
    INT32 i4_ret;

    if ((pt_part->ui1_flags != OS_MEM_PART_FLAG_ACTIVE) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_EXTERNAL) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_PERMANENT))
    {
        return;
    }

    pt_obj = (MEM_FREEOBJ_T *) (((UINT32) pv_mem) - sizeof(MEM_OBJ_T));

    if ((((UINT32) (pt_obj->pt_bkend)) & 0x3) ||
        (((UINT32) (pt_obj->pt_bkend)) > ((UINT32) (pt_part->pt_last))) ||
        (((UINT32) (pt_obj->pt_bkend)) < ((UINT32) (pt_part->pt_first))))
    {
        OS_DBG_ABORT(DBG_CAT_MEMORY, DBG_MEM_BACK_END_CORRUPT);
    }

    if ((((UINT32) (pt_obj->pt_ftend)) & 0x3) ||
        (((UINT32) (pt_obj->pt_ftend)) > ((UINT32) (pt_part->pt_last))) ||
        (((UINT32) (pt_obj->pt_ftend)) < ((UINT32) (pt_part->pt_first))))
    {
        OS_DBG_ABORT(DBG_CAT_MEMORY, DBG_MEM_FRONT_END_CORRUPT);
    }

    if (pt_part->b_sema_inited == TRUE)
    {
        i4_ret = x_sema_lock(pt_part->h_sema_hdl, X_SEMA_OPTION_WAIT);
        if (i4_ret != OSR_OK)
        {
            return;
        }
    }

    if (pt_obj->ui2_flags == OS_MEM_FLAG_ALLOCATED)
    {
        pt_part->ui4_alloc_obj--;
        pt_obj->z_size = ((UINT32) (pt_obj->pt_bkend)) - ((UINT32) pt_obj);

        /* pt_obj was allocated and is to be freed now */
        /* check if memory object before pt_obj is already freed */
        /* such that coalescing can be performed */

        pt_tmp = pt_obj->pt_ftend;      /* obj before pt_obj */
        if (pt_tmp->ui2_flags == OS_MEM_FLAG_FREE)
        {
            os_mem_remove_from_bucket(pt_part, pt_tmp);

            /* merge pt_obj to pt_tmp */
            pt_tmp->z_size += pt_obj->z_size;
            pt_tmp->ui1_index = os_mem_bucket_index(pt_tmp->z_size);
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
            pt_tmp->ui2_padding3 = 0xFFFF;
#endif
            pt_tmp->pt_bkend = pt_obj->pt_bkend;
            pt_obj->pt_bkend->pt_ftend = pt_tmp;

            pt_obj->pt_ftend = (MEM_FREEOBJ_T *) 0xffffffff;
            pt_obj->pt_ftend = (MEM_FREEOBJ_T *) 0xffffffff;
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
            pt_obj->ui2_padding3 = 0xFFFF;
#endif
            pt_obj->ui2_flags = OS_MEM_FLAG_FREE;

            pt_obj = pt_tmp;
        }

        /* check if memory object behind pt_obj is already freed */
        /* such that coalescing can be perfprmed */
        /* pt_obj is not the last obj of this partition */

        pt_tmp = pt_obj->pt_bkend;      /* memory behind pt_obj */
        if (pt_tmp->ui2_flags == OS_MEM_FLAG_FREE)
        {
            os_mem_remove_from_bucket(pt_part, pt_tmp);

            /* merge pt_tmp to pt_obj */
            pt_obj->z_size += pt_tmp->z_size;
            pt_obj->ui1_index = os_mem_bucket_index(pt_obj->z_size);
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
            pt_obj->ui2_padding3 = 0xFFFF;
#endif
            pt_obj->pt_bkend = pt_tmp->pt_bkend;
            pt_tmp->pt_bkend->pt_ftend = pt_obj;

            pt_tmp->pt_ftend = (MEM_FREEOBJ_T *) 0xffffffff;
            pt_tmp->pt_ftend = (MEM_FREEOBJ_T *) 0xffffffff;
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
            pt_tmp->ui2_padding3 = 0xFFFF;
#endif
            pt_tmp->ui2_flags = OS_MEM_FLAG_FREE;
        }
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
        pt_obj->ui2_padding3 = 0xFFFF;
#endif
        pt_obj->ui2_flags = OS_MEM_FLAG_FREE;

        os_mem_put_to_bucket(pt_part, pt_obj);
    }
    else
    {
        OS_DBG_ABORT(DBG_CAT_MEMORY, DBG_MEM_FREED);
    }

    if (pt_part->b_sema_inited == TRUE)
    {
        (VOID) x_sema_unlock(pt_part->h_sema_hdl);
    }
}


#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
VOID *
os_mem_calloc(MEM_PART_T *pt_part, UINT32 ui4_count, SIZE_T z_size)
{
    VOID *pv_mem = NULL;
    UINT32  *pui4_mem;
    SIZE_T z_size_tmp;

    if (pt_def_part)
    {
        z_size_tmp = ((ui4_count * z_size) + 3) & ~0x3;
        pv_mem = (pt_def_part->pf_alloc)(pt_def_part, z_size_tmp);

        if (pv_mem != NULL)
        {
            pui4_mem = (UINT32 *) pv_mem;
            while (z_size_tmp != 0)
            {
                *pui4_mem++ = (UINT32) 0;
                z_size_tmp -= sizeof(UINT32);
            }
        }
    }

    return(pv_mem);
}
#endif


/*----------------------------------------------------------------------------
 * Function: os_mem_put_to_bucket()
 *
 * Description:
 *      This routine is called to link a memory obj into its bucket list.
 *      it performs
 *      1. locate bucket entry.
 *      2. if memory obj size is greater than any memory obj in the bucket
 *         list, link it at the end.
 *      3. otherwise, link it before the end memory obj of bucket list.
 *
 * Inputs:
 *      pt_part: pointer to a mem_part struct.
 *      pv_obj: pointer to a memory obj.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
os_mem_put_to_bucket(MEM_PART_T *pt_part, MEM_FREEOBJ_T *pt_obj)
{
    MEM_BUCKET_T *pt_bucket;
    MEM_FREEOBJ_T *pt_tmp;

    if ((pt_part->ui1_flags != OS_MEM_PART_FLAG_ACTIVE) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_EXTERNAL) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_PERMANENT))
    {
        return;
    }

    /* pt_obj has a bucket index set at memory allocation time */
    pt_part->ui4_free_obj++;
    pt_bucket = &(pt_part->t_bucket[pt_obj->ui1_index]);

    if (LINK_NOT_EMPTY(pt_bucket->pt_head, NULL))
    {
        /* this bucket is not empty */
        if (pt_obj->z_size >= pt_bucket->pt_tail->z_size)
        {
            /* put right after the tail */
            pt_tmp = pt_bucket->pt_tail;       /* put after pt_tmp */
            SET_LINK_TAIL(pt_bucket->pt_tail, pt_obj);  /* make pt_obj tail */
        }
        else
        {
            /* put right before the tail */
            pt_tmp = pt_bucket->pt_tail->pt_prev;   /* put after pt_tmp */
            if (pt_bucket->pt_tail == pt_bucket->pt_head)
            {
                SET_LINK_HEAD(pt_bucket->pt_head, pt_obj);
            }
        }

        SET_LINK_NEXT(pt_obj, pt_tmp->pt_next);
        SET_LINK_PREV(pt_obj, pt_tmp);
        SET_LINK_PREV(pt_tmp->pt_next, pt_obj);
        SET_LINK_NEXT(pt_tmp, pt_obj);
    }
    else
    {
        SET_LINK_NEXT(pt_obj, pt_obj);
        SET_LINK_PREV(pt_obj, pt_obj);
        SET_LINK_HEAD(pt_bucket->pt_head, pt_obj);
        SET_LINK_TAIL(pt_bucket->pt_tail, pt_obj);
    }

    if (pt_part->ui1_min_index > pt_obj->ui1_index)
    {
        pt_part->ui1_min_index = pt_obj->ui1_index;
    }
    else if (pt_part->ui1_max_index < pt_obj->ui1_index)
    {
        pt_part->ui1_max_index = pt_obj->ui1_index;
    }
}


/*----------------------------------------------------------------------------
 * Function: os_mem_realloc()
 *
 * Description:
 *      This routine is called to link a memory obj into its bucket list.
 *      it performs
 *      1. get memory obj struct from pv_mem.
 *      2. if current memory obj has enough space for z_new_size,
 *         return memory address of this memory obj.
 *      3. otherwise, call partition pf_alloc to get a proper memory obj,
 *         copy contents of current memory to new memory, call pf_free
 *         to free current memory obj.
 *
 * Inputs:
 *      pt_part: pointer to a mem_part struct.
 *      pv_mem: memory address of allocated memory.
 *      z_new_size: size of new memory block.
 *
 * Outputs: -
 *
 * Returns:
 *      non NULL: new memory address.
 *      NULL: routine failed.
 *---------------------------------------------------------------------------*/
VOID *
os_mem_realloc(MEM_PART_T *pt_part, const VOID *pv_mem, SIZE_T z_new_size)
{
    MEM_FREEOBJ_T *pt_obj;
    VOID *pv_new;
    SIZE_T z_size;

    if ((pt_part->ui1_flags != OS_MEM_PART_FLAG_ACTIVE) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_EXTERNAL) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_PERMANENT))
    {
        return((VOID *) NULL);
    }

    pt_obj = (MEM_FREEOBJ_T *) (((UINT32) pv_mem) - sizeof(MEM_OBJ_T));
    z_size = (SIZE_T) ((UINT32) (pt_obj->pt_bkend) - (UINT32) pv_mem);
    if (z_new_size <= z_size)
    {
        /* make use the existing pv_mem */
        return((VOID *) pv_mem);
    }
    else
    {
        pv_new = (pt_part->pf_alloc)(pt_part, z_new_size);
        if (pv_new == NULL)
        {
            return((VOID *) NULL);
        }

        /* copy data to new memory and put the memory handle to free bucket */
        x_memmove(pv_new, pv_mem, z_size);
        (pt_part->pf_free)(pt_part, (VOID *) pv_mem);
        return(pv_new);
    }
}


/*----------------------------------------------------------------------------
 * Function: os_mem_put_to_bucket()
 *
 * Description:
 *      This routine is called to actually free a memory. it performs
 *      1. get mem_blk struc from pv_mem.
 *      2. sanity check of mem_blk info for validity.
 *      3. get part struct and call pf_free routine.
 *
 * Inputs:
 *      pv_mem: memory address to be freed.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
os_mem_free(const VOID *pv_mem)
{
    MEM_BLK_T *pt_blk;
    MEM_PART_T *pt_part;

    pt_blk = (MEM_BLK_T *) (((UINT32) pv_mem) - OS_MEM_STATUS_SIZE);

    if (pt_blk->ui1_index >= OS_MEM_BUCKET_SIZE)
    {
        OS_DBG_ABORT(DBG_CAT_MEMORY, DBG_MEM_BUCKET_INDEX_CORRUPT);
    }

    if (pt_blk->ui1_part_index >= OS_MEM_PART_NUM)
    {
        OS_DBG_ABORT(DBG_CAT_MEMORY, DBG_MEM_PART_INDEX_CORRUPT);
    }

    if (pt_blk->ui2_flags != OS_MEM_FLAG_ALLOCATED)
    {
        OS_DBG_ABORT(DBG_CAT_MEMORY, DBG_MEM_FREED);
    }

    pt_part = (MEM_PART_T *) &(pt_mem_global->t_part_tbl[pt_blk->ui1_part_index]);
    if ((pt_part->ui1_flags != OS_MEM_PART_FLAG_ACTIVE) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_EXTERNAL) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_PERMANENT))
    {
        return;
    }

    (pt_part->pf_free)(pt_part, (VOID *) pv_mem);
}


/*----------------------------------------------------------------------------
 * Function: os_mem_bucket_index()
 *
 * Description:
 *      This routine is called to get bucket link based on memory size.
 *
 * Inputs:
 *      z_s: memory size.
 *
 * Outputs: -
 *
 * Returns:
 *      positive number: bucket index.
 *---------------------------------------------------------------------------*/
UINT8
os_mem_bucket_index(SIZE_T z_s)
{
    if (z_s <= 64)
    {
        return ((z_s >= 24) ? (UINT8) ((z_s / 8) - 2) : (UINT8) 1);
    }
    else if (z_s <= 512)
    {
        return 7;
    }
    else
    {
        return 8;
    }
}


/*----------------------------------------------------------------------------
 * Function: mem_free_cb()
 *
 * Description:
 *      this routine is called via handle_free(). it simply returns FALSE
 *      so that a memory partition can not be deleted since it was created.
 *
 * Inputs:
 *      h_hdl : handle.
 *      e_type : handle type.
 *      pv_obj : handle object.
 *      pv_tag : handle private tag.
 *      b_req_hdl : TRUE or FALSE.
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE : routine was successful.
 *      FALSE : routine failed.
 *---------------------------------------------------------------------------*/
BOOL
mem_free_cb(HANDLE_T       h_hdl,
            HANDLE_TYPE_T  e_type,
            VOID *         pv_obj,
            VOID *         pv_tag,
            BOOL           b_req_hdl)
{

    MEM_PART_T *pt_part;

    if (e_type != HT_GROUP_OS_MEMORY)
    {
        return(FALSE);
    }

    pt_part = (MEM_PART_T *) pv_obj;
    if (pt_part->ui1_flags == OS_MEM_PART_FLAG_DELETE)
    {
        pt_part->ui1_flags = OS_MEM_PART_FLAG_IDLE;

        /* free semaphore if created */
        if (pt_part->h_sema_hdl != NULL_HANDLE)
        {
            (VOID) x_sema_delete(pt_part->h_sema_hdl);
            pt_part->h_sema_hdl = NULL_HANDLE;
        }

        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

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
            *z_bigfree -= (sizeof(MEM_OBJ_T) + 7) & ~0x7;
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

    *z_totalsize = pt_part->z_size /* +
        (SIZE_T) ((UINT32) (pt_part->pv_mem) - ((UINT32) (pt_part->pv_start)))*/;

    *z_allocatedobj = pt_part->ui4_alloc_obj;   /* total # of allocated obj*/
    *z_freeobj = pt_part->ui4_free_obj;

    if (x_sema_unlock(pt_part->h_sema_hdl) != OSR_OK)
    {
        OS_DBG_ABORT(DBG_CAT_SEMAPHORE, DBG_SEMA_UNLOCK_FAIL);
    }
}
