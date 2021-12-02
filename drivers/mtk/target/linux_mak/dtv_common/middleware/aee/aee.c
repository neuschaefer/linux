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
 * $RCSfile: aee.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/1 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 0772fbcbfc330a3368874e31e29a6d08 $
 *
 * Description: 
 *         This file contains the implementation of the APIs shared with other
 *         middleware components.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "aee/_aee.h"
#include "aee/aee.h"
#include "os/inc/os.h"
#include "os/thread/thread_lib.h"
#include "inc/pvt_key.h"
#include "dbg/def_dbg_level_mw.h"
#include "dbg/u_dbg.h"

/*-----------------------------------------------------------------------------
                    defines
-----------------------------------------------------------------------------*/

#define AEE_RESOURCE_OK(_res)  \
((_res == 0) || (pt_aee->t_res._res >= _res))

#ifdef DEBUG
#define GRAB_RESOURCE(_res)                            \
do {                                                   \
    pt_aee->t_res._res -= _res;                        \
    if (pt_aee->t_res._res < pt_aee->t_peak_res._res)  \
    {                                                  \
        pt_aee->t_peak_res._res = pt_aee->t_res._res;  \
    }                                                  \
} while (0)
#else
#define GRAB_RESOURCE(_res) pt_aee->t_res._res -= _res
#endif

#define RELEASE_RESOURCE(_res, _max_res)                             \
    ui4_overflow <<= 1;                                              \
    if ((_res) <= (pt_aee->t_max_res._max_res - pt_aee->t_res._res)) \
    {                                                                \
        pt_aee->t_res._res += _res;                                  \
    }                                                                \
    else                                                             \
    {                                                                \
        pt_aee->t_res._res = pt_aee->t_max_res._max_res;             \
        ui4_overflow++;                                              \
    }


/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

static AEE_T*    pt_aees    = NULL;
static HANDLE_T  h_aee_sem  = NULL_HANDLE;

static UINT16  ui2_aee_dbg_level = DBG_INIT_LEVEL_MW_AEE;


/*-----------------------------------------------------------------------------
 * Name: aee_get_dbg_level
 *
 * Description: This API gets the debug level of the AEE component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: the AEE debug level.
 ----------------------------------------------------------------------------*/
UINT16 aee_get_dbg_level (VOID)
{
    return ui2_aee_dbg_level;
}

/*-----------------------------------------------------------------------------
 * Name: aee_set_dbg_level
 *
 * Description: This API sets the debug level of the AEE component.
 *
 * Inputs:  ui2_dbg_level  Contains the new debug level.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The debug level was set successfully.
 *          CMR_FAIL  The debug level could not be set.
 ----------------------------------------------------------------------------*/
INT32 aee_set_dbg_level (UINT16  ui2_dbg_level)
{
    ui2_aee_dbg_level = ui2_dbg_level;
    
    return AEER_OK;
}

/*-----------------------------------------------------------------------------
 * Name: aee_primary_handle_autofree
 *
 * Description: This API does not do anything (besides returning OK) since
 *              such a handle cannot be deleted (it will be deleted when the
 *              application terminates).
 *
 * Inputs:  h_handle  Contains the handle to delete
 *          e_type    Contains the type of the handle.
 *
 * Outputs: -
 *
 * Returns: HR_OK  Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 aee_primary_handle_autofree (HANDLE_T       h_handle,
                                          HANDLE_TYPE_T  e_type)
{
    /*
      nothing to do, the handle will be deleted automatically when the
      application terminates.
    */
    
    return HR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: aee_secondary_handle_autofree
 *
 * Description: This API deletes the secondary handle (simply calling
 *              handle_free).
 *
 * Inputs:  h_handle  Contains the handle to delete
 *          e_type    Contains the type of the handle.
 *
 * Outputs: -
 *
 * Returns: HR_OK  Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 aee_secondary_handle_autofree (HANDLE_T       h_handle,
                                            HANDLE_TYPE_T  e_type)
{
    return handle_free(h_handle, FALSE);
}

/*-----------------------------------------------------------------------------
 * Name: aee_init
 *
 * Description: This API initializes the AEE middleware component. The system
 *              will abort if the initialization failed.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: AEER_OK  Routine successful.
 ----------------------------------------------------------------------------*/
INT32 aee_init (VOID)
{
    static BOOL   b_aee_init = FALSE;
    CRIT_STATE_T  t_state;
    INT32         i4_res;
    BOOL          b_aee_already_init;
    
    t_state = x_crit_start();

    b_aee_already_init = b_aee_init;

    x_crit_end(t_state);

    if (! b_aee_already_init)
    {
        static handle_autofree_fct  apf_autofree_fcts[2] =
            {
                aee_primary_handle_autofree,
                aee_secondary_handle_autofree
            };
        
        i4_res = x_sema_create(& h_aee_sem,
                               X_SEMA_TYPE_MUTEX, 1);
        
        if (i4_res != OSR_OK)
        {
            ABORT(DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_CREATE_SEMAPHORE);
        }
        
        i4_res = handle_set_autofree_tbl(HT_GROUP_AEE,
                                         apf_autofree_fcts);
        
        if (i4_res != HR_OK)
        {
            ABORT(DBG_CAT_INV_OP, DBG_ABRT_CANNOT_SET_AUTOFREE);
        }
        
        aee_cli_init();
    }
    
    i4_res = AEER_OK;
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: aee_add
 *
 * Description: This API adds an AEE structure to the list of AEEs.
 *
 * Inputs:  pt_aee  References the AEE to add to the list.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID aee_add (AEE_T*  pt_aee)
{
    x_sema_lock(h_aee_sem, X_SEMA_OPTION_WAIT);
    
    pt_aee->pt_next = pt_aees;
    pt_aees         = pt_aee;
    
    x_sema_unlock(h_aee_sem);
}

/*-----------------------------------------------------------------------------
 * Name: aee_remove
 *
 * Description: This API removes an AEE structure from the list of AEEs. It
 *              assumes the structure is in the list.
 *
 * Inputs:  pt_aee  References the AEE to remove from the list.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID aee_remove (AEE_T*  pt_aee)
{
    AEE_T**  ppt_aee;
    
    x_sema_lock(h_aee_sem, X_SEMA_OPTION_WAIT);
    
    ppt_aee = & pt_aees;
    
    while (*ppt_aee != NULL)
    {
        if (*ppt_aee == pt_aee)
        {
            *ppt_aee = pt_aee->pt_next;
            break;
        }
        
        ppt_aee = & ((*ppt_aee)->pt_next);
    }
    
    x_sema_unlock(h_aee_sem);
}

/*-----------------------------------------------------------------------------
 * Name: aee_find_by_name
 *
 * Description: This API finds an AEE given a name.
 *
 * Inputs:  ps_name  References the name of the application.
 *
 * Outputs: -
 *
 * Returns: A pointer to the AEE structure if it exists, or NULL.
 ----------------------------------------------------------------------------*/
AEE_T* aee_find_by_name (const CHAR*  ps_name)
{
    AEE_T*  pt_aee;
    
    x_sema_lock(h_aee_sem, X_SEMA_OPTION_WAIT);
    
    pt_aee = pt_aees;

    while (pt_aee != NULL)
    {
        if (x_strcmp(pt_aee->s_name, ps_name) == 0)
        {
            break;
        }
        
        pt_aee = pt_aee->pt_next;
    }
    
    x_sema_unlock(h_aee_sem);

    return pt_aee;
}

#if defined(DEBUG) || defined(CLI_LVL_ALL)
/*-----------------------------------------------------------------------------
 * Name: aee_list_aee_info
 *
 * Description: This API displays information about a specific AEE.
 *
 * Inputs:  pt_aee  References the AEE structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID aee_list_aee_info (const AEE_T*  pt_aee)
{
    UINT16  ui2_current;
    UINT16  ui2_peak;
    
    x_dbg_stmt(DBG_PREFIX" %s\n", pt_aee->s_name);
    
    ui2_current =
        pt_aee->t_max_res.ui2_max_handles - pt_aee->t_res.ui2_handles;
    ui2_peak =
        pt_aee->t_max_res.ui2_max_handles - pt_aee->t_peak_res.ui2_handles;
    
    x_dbg_stmt(DBG_PREFIX"   Handles: %d (peak:%d, max allowed:%d)\n",
               ui2_current, ui2_peak,
               pt_aee->t_max_res.ui2_max_handles);
    
    x_dbg_stmt(DBG_PREFIX"   Memory: %d bytes (peak:%d, max allowed:%d)\n",
               pt_aee->t_max_res.z_max_memory - pt_aee->t_res.z_memory,
               pt_aee->t_max_res.z_max_memory - pt_aee->t_peak_res.z_memory,
               pt_aee->t_max_res.z_max_memory);
}

/*-----------------------------------------------------------------------------
 * Name: aee_list_all_aees_info
 *
 * Description: This API displays information about all the AEEs.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID aee_list_all_aees_info (VOID)
{
    AEE_T*  pt_aee;
    
    x_dbg_stmt(DBG_PREFIX"AEEs:\n");
    
    x_sema_lock(h_aee_sem, X_SEMA_OPTION_WAIT);
    
    pt_aee = pt_aees;
    
    while (pt_aee != NULL)
    {
        aee_list_aee_info(pt_aee);
        
        pt_aee = pt_aee->pt_next;
    }
    
    x_sema_unlock(h_aee_sem);
}
#endif /* DEBUG || CLI_LVL_ALL*/

/*-----------------------------------------------------------------------------
 * Name: aee_clean_up
 *
 * Description: This API checks whether the AEE is terminated and will perform
 *              clean-up (i.e. deletion of all handles allocated by the app) if
 *              required. The system will abort if clean-up could not be
 *              performed completely for whatever reason.
 *
 * Inputs:  h_app   Contains the application handle.
 *          pt_aee  References the AEE structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID aee_clean_up (HANDLE_T  h_app,
                          AEE_T*    pt_aee)
{
    CRIT_STATE_T  t_state;
    BOOL          b_free;
    
    t_state = x_crit_start();
    
    pt_aee->t_res.ui2_threads++; /* release one thread resource */
    
    b_free =
        (pt_aee->t_res.ui2_threads == pt_aee->t_max_res.ui2_max_threads);
    
    x_crit_end(t_state);
    
    if (b_free)
    {
        aee_remove(pt_aee);
        
        DBG_INFO((DBG_PREFIX"\"%s\" exits\n", pt_aee->s_name));
        
        handle_free(h_app, FALSE);
    }
}

/*-----------------------------------------------------------------------------
 * Name: aee_clean_relationships
 *
 * Description: This API cleans up the relationships between AEEs. It also
 *              release the handle back into the parent's resource pool.
 *
 * Inputs:  h_app           Contains the application handle.
 *          pt_exiting_aee  References the AEE structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID aee_clean_relationships (HANDLE_T      h_app,
                                     const AEE_T*  pt_exiting_aee)
{
    HANDLE_T  h_parent_app;

    h_parent_app = pt_exiting_aee->h_parent_app;
    
    if (h_parent_app != NULL_HANDLE)
    {
        AEE_T*  pt_aee;
        
        x_sema_lock(h_aee_sem, X_SEMA_OPTION_WAIT);

        pt_aee = pt_aees;

        while (pt_aee != NULL)
        {
            /* the parent of the exiting AEE gets a handle back in the pool */
            if (pt_aee->h_app == h_parent_app)
            {
                pt_aee->t_res.ui2_handles++;
            }

            /* children of the exiting AEE reset parent to NULL_HANDLE */
            if (pt_aee->h_parent_app == h_app)
            {
                pt_aee->h_parent_app = NULL_HANDLE;
            }
            
            pt_aee = pt_aee->pt_next;
        }
        
        x_sema_unlock(h_aee_sem);
    }
}

/*-----------------------------------------------------------------------------
 * Name: aee_main
 *
 * Description: This API is a wrapper to the application's main function that
 *              is specified when the application is created.
 *
 * Inputs:  pv_arg  References the arguments to the main function.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID aee_main (VOID*  pv_arg)
{
    AEE_MAIN_ARG_T*  pt_arg;
    INT32            i4_res;
    
    pt_arg = (AEE_MAIN_ARG_T*) pv_arg;
    
    aee_add(pt_arg->pt_aee);
    
    DBG_INFO((DBG_PREFIX"\"%s\" starts (%p)\n",
              pt_arg->pt_aee->s_name, pt_arg->pt_aee));
    
    i4_res = x_thread_set_pvt(PVT_KEY_AEE, NULL, pt_arg->pt_aee);
    
    if (i4_res == OSR_OK)
    {
        pt_arg->pf_main(pt_arg->h_app,
                        pt_arg->pv_arg);
    }

    x_thread_del_pvt(PVT_KEY_AEE);
    
    x_mem_free(pt_arg->pv_arg);
    
    aee_clean_up(pt_arg->h_app, pt_arg->pt_aee);
}

/*-----------------------------------------------------------------------------
 * Name: aee_thread_create_main
 *
 * Description: This API is a wrapper to the main function specified when an
 *              application spawns a new thread.
 *
 * Inputs:  pv_arg  References the arguments to the main function.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID aee_thread_create_main (VOID*  pv_arg)
{
    AEE_THREAD_CREATE_ARG_T*  pt_arg;
    INT32                     i4_res;
    
    pt_arg = (AEE_THREAD_CREATE_ARG_T*) pv_arg;
    
    i4_res = x_thread_set_pvt(PVT_KEY_AEE, NULL, pt_arg->pt_aee);
    
    if (i4_res == OSR_OK)
    {
        pt_arg->pf_main(pt_arg->pv_arg);
    }

    
    x_thread_del_pvt(PVT_KEY_AEE);

    x_mem_free(pt_arg->pv_arg);
    
    aee_clean_up(pt_arg->h_app, pt_arg->pt_aee);
}

/*-----------------------------------------------------------------------------
 * Name: aee_get_info
 *
 * Description: This API returns information about the application (group id,
 *              app id and AEE structure). It assumes the arguments are valid.
 *
 * Inputs:  -
 *
 * Outputs: pui4_app_group_id  Contains the application group id.
 *          pui4_app_id        Contains the application id.
 *          ppt_aee            Contains a pointer to the AEE structure.
 *
 * Returns: AEER_OK    Routine successful.
 *          AEER_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
static INT32 aee_get_info (UINT32*  pui4_app_group_id,
                           UINT32*  pui4_app_id,
                           AEE_T**  ppt_aee)
{
    INT32  i4_res;
    
    i4_res = x_thread_get_pvt(PVT_KEY_AEE, (VOID**) ppt_aee);
    
    if (i4_res == OSR_OK)
    {
        *pui4_app_group_id = (*ppt_aee)->ui4_app_group_id;
        *pui4_app_id       = (*ppt_aee)->ui4_app_id;
        
        return AEER_OK;
    }
    else
    {
        return AEER_FAIL;
    }
}

/*-----------------------------------------------------------------------------
 * Name: aee_thread_create
 *
 * Description: This API spawns a new application thread. It must be called
 *              from an application thread.
 *
 * Inputs:  ps_name      Contains the thread name.
 *          z_stacksize  Contains the stack size.
 *          ui1_pri      Contains the thread priority.
 *          pf_main      References the thread main function.
 *          z_arg_size   Contains the size of the pv_arg data.
 *          pv_arg       References the data passed to the main function.
 *
 * Outputs: ph_th_hdl  Contains the thread handle.
 *
 * Returns: AEER_OK             Routine successful.
 *          AEER_OUT_OF_MEMORY  Out of memory.
 *          AEER_FAIL           Routine failed.
 ----------------------------------------------------------------------------*/
INT32 aee_thread_create (HANDLE_T*             ph_th_hdl,
                         CHAR*                 ps_name,
                         SIZE_T                z_stacksize,
                         UINT8                 ui1_pri,
                         x_os_thread_main_fct  pf_main,
                         SIZE_T                z_arg_size,
                         VOID*                 pv_arg)
{
    UINT32  ui4_app_group_id;
    UINT32  ui4_app_id;
    INT32   i4_res;
    AEE_T*  pt_aee;

    ui4_app_group_id = 0;
    ui4_app_id       = 0;
    pt_aee           = NULL;
    
    i4_res = aee_get_info (& ui4_app_group_id,
                           & ui4_app_id,
                           & pt_aee);

    if (i4_res == AEER_OK)
    {
        AEE_THREAD_CREATE_ARG_T  t_arg;

        if (pv_arg != NULL)
        {
            t_arg.pv_arg = x_mem_alloc(z_arg_size);
            
            if (t_arg.pv_arg != NULL)
            {
                x_memcpy(t_arg.pv_arg, pv_arg, z_arg_size);
            }
            else
            {
                i4_res = AEER_OUT_OF_MEMORY;
            }
        }
        else
        {
            t_arg.pv_arg = NULL;
        }
        
        if (i4_res == AEER_OK)
        {
            t_arg.pf_main = pf_main;
            t_arg.pt_aee  = pt_aee;
            t_arg.h_app   = pt_aee->h_app;
            
            i4_res = x_thread_create(ph_th_hdl,
                                     ps_name,
                                     z_stacksize,
                                     ui1_pri,
                                     aee_thread_create_main,
                                     sizeof(t_arg),
                                     & t_arg);
            
            if (i4_res != OSR_OK)
            {
                x_mem_free(t_arg.pv_arg);
                
                i4_res = AEER_FAIL;
            }
            else
            {
#ifdef LINUX_TURNKEY_SOLUTION
                pt_aee->t_res.ui2_threads    -= 1;
#endif
                i4_res = AEER_OK;
            }
        }
    }
    else
    {
        i4_res = AEER_FAIL;
    }
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: aee_free_handle
 *
 * Description: This API is called when an application's primary handle is
 *              deleted.
 *
 * Inputs:  h_app         Contains the handle to delete.
 *          e_type        Contains the handle type (should be AEET_PRIMARY_AEE)
 *          pv_obj        References the object attached to the handle.
 *          pv_tag        Contains the handle tag.
 *          b_req_handle  Is set to TRUE if the handle free was initiated with
 *                        this handle else FALSE.
 *
 * Outputs: -
 *
 * Returns: TRUE if the handle free is allowed, FALSE otherwise.
 ----------------------------------------------------------------------------*/
static BOOL aee_free_handle (HANDLE_T       h_app,
                             HANDLE_TYPE_T  e_type,
                             VOID*          pv_obj,
                             VOID*          pv_tag,
                             BOOL           b_req_handle)
{
    BOOL  b_free_allowed;

    b_free_allowed = ! b_req_handle;
    
    if (b_free_allowed)
    {
        AEE_T*  pt_aee;

        pt_aee = (AEE_T*) pv_obj;
        
        /* autofree handles */
        {
            HANDLE_T  h_handle = NULL_HANDLE;
            
            handle_next_aux_linked(pt_aee->h_aux, & h_handle);
            
            while (h_handle != NULL_HANDLE)
            {
                HANDLE_T  h_next_handle = NULL_HANDLE;
                
                handle_next_aux_linked(h_handle, & h_next_handle);
                
                handle_delink_from_aux(h_handle);
                
                handle_autofree(h_handle);
                
                h_handle = h_next_handle;
            }
        }
        
        /* free auxilary list "header" */
        handle_free(pt_aee->h_aux, FALSE);
        
        /* free main partition */
        if (pt_aee->h_mem_partition != NULL_HANDLE)
        {
            mem_part_delete(pt_aee->h_mem_partition);
        }
        
        /* free pointers that are not in main partition */
        aee_mem_free_all_allocated_pointers(pt_aee);
        
        aee_clean_relationships(h_app, pt_aee);
        
        x_mem_free(pv_obj);
    }
    
    return b_free_allowed;
}

/*-----------------------------------------------------------------------------
 * Name: aee_free_secondary_handle
 *
 * Description: This API is called when an application's secondary handle is
 *              deleted.
 *
 * Inputs:  h_app         Contains the handle to delete.
 *          e_type        Contains the handle type (AEET_SECONDARY_AEE)
 *          pv_obj        References the object attached to the handle.
 *          pv_tag        Contains the handle tag.
 *          b_req_handle  Is set to TRUE if the handle free was initiated with
 *                        this handle else FALSE.
 *
 * Outputs: -
 *
 * Returns: TRUE.
 ----------------------------------------------------------------------------*/
static BOOL aee_free_secondary_handle (HANDLE_T       h_app,
                                       HANDLE_TYPE_T  e_type,
                                       VOID*          pv_obj,
                                       VOID*          pv_tag,
                                       BOOL           b_req_handle)
{
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: aee_aux_free_release_handle
 *
 * Description: This API is called when a handle is removed from the auxilary
 *              list. This will increment the number of available handles by 1.
 *
 * Inputs:  pv_obj    References the AEE structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID aee_aux_free_release_handle (VOID*  pv_obj)
{
#ifndef LINUX_TURNKEY_SOLUTION
    CRIT_STATE_T  t_state;
#endif
    AEE_T*        pt_aee;
    UINT32        ui4_overflow;
    UINT16        ui2_handles;
    
    pt_aee       = (AEE_T*) pv_obj;
    ui4_overflow = 0;
    ui2_handles  = 1;
    
#ifdef LINUX_TURNKEY_SOLUTION
    RELEASE_RESOURCE(ui2_handles, ui2_max_handles);
#else
    t_state = x_crit_start();
    
    RELEASE_RESOURCE(ui2_handles, ui2_max_handles);
    
    x_crit_end(t_state);
#endif    
    if (ui4_overflow != 0)
    {
        DBG_ERROR((DBG_PREFIX"Handle resource release overflow\n"));
    }
}

/*-----------------------------------------------------------------------------
 * Name: aee_create_op
 *
 * Description: This API creates a new application execution environment.
 *
 * Inputs:  pt_desc  References the application's description (resources, etc).
 *          ps_name  References the application's name.
 *
 * Outputs: ph_app  Contains the application handle.
 *
 * Returns: AEER_OK              Routine successful.
 *          AEER_OUT_OF_HANDLES  Out of handles.
 *          AEER_OUT_OF_MEMORY   Out of memory.
 *          AEER_FAIL            Routine failed (undefined reason).
 ----------------------------------------------------------------------------*/
INT32 aee_create_op (AEE_APP_DESC_T*        pt_desc,
                     const CHAR*            ps_name,
                     HANDLE_T*              ph_app)
{
    SIZE_T  z_size;
    AEE_T*  pt_aee;
    INT32   i4_res;
	
#ifdef LINUX_TURNKEY_SOLUTION
    pt_desc->t_max_res.z_min_memory = 0;
#endif
    /* memory allocated for both AEE_T and main partition */
    z_size = sizeof(AEE_T) + pt_desc->t_max_res.z_min_memory;
    
    pt_aee = (AEE_T *) x_mem_alloc(z_size);
    
    if (pt_aee != NULL)
    {
        x_memset(pt_aee, 0, sizeof(AEE_T));
        
        pt_aee->t_max_res        = pt_desc->t_max_res;
        pt_aee->ui8_flags        = pt_desc->ui8_flags;
        pt_aee->pf_receive_data  = pt_desc->pf_receive_data;
        
        pt_aee->t_res.z_memory        = pt_desc->t_max_res.z_max_memory;
        pt_aee->t_res.z_memory       -= pt_desc->t_max_res.z_min_memory;
        pt_aee->t_res.ui8_files_size  = pt_desc->t_max_res.ui8_max_files_size;
        pt_aee->t_res.ui2_files       = pt_desc->t_max_res.ui2_max_files;
        pt_aee->t_res.ui2_handles     = pt_desc->t_max_res.ui2_max_handles;
        pt_aee->t_res.ui2_threads     = pt_desc->t_max_res.ui2_max_threads;
        pt_aee->t_res.ui2_threads    -= 1;
        /* - 1 because one thread will be automatically created */
        pt_aee->t_res.ui2_semaphores  = pt_desc->t_max_res.ui2_max_semaphores;
        pt_aee->t_res.ui2_msg_queues  = pt_desc->t_max_res.ui2_max_msg_queues;
        
#ifdef DEBUG
        pt_aee->t_peak_res = pt_aee->t_res;
#endif
        
        pt_aee->ui4_app_group_id = pt_desc->ui4_app_group_id;
        pt_aee->ui4_app_id       = pt_desc->ui4_app_id;
        
        x_strncpy(pt_aee->s_name, ps_name, AEE_NAME_MAX_LENGTH);
        
        pt_aee->pv_allocated_pointers = NULL;
        
        if (pt_desc->t_max_res.z_min_memory > 0)
        {
            /*
              memory for the partition was allocated at the same time as pt_aee
              (see above), to (pt_aee + 1) is the beginning of the partition's
              memory.
            */
            
            i4_res = x_mem_part_create(& pt_aee->h_mem_partition,
                                       ps_name,
                                       (VOID*) (pt_aee + 1),
                                       pt_desc->t_max_res.z_min_memory, 0);
        }
        else
        {
            pt_aee->h_mem_partition = NULL_HANDLE;
            
            i4_res = OSR_OK;
        }
        
        if (i4_res == OSR_OK)
        {
            i4_res = handle_alloc(AEET_PRIMARY_AEE,
                                  pt_aee,
                                  NULL, /* tag */
                                  aee_free_handle,
                                  ph_app);
            
            if (i4_res == HR_OK)
            {
                i4_res =
                    handle_alloc_aux_link_head(pt_aee,
                                               aee_aux_free_release_handle,
                                               & pt_aee->h_aux);
                
                if (i4_res == HR_OK)
                {
                    AEE_MAIN_ARG_T  t_arg;
                    
                    pt_aee->h_app = *ph_app;
                    
                    t_arg.pt_aee  = pt_aee;
                    t_arg.h_app   = *ph_app;
                    t_arg.pf_main = pt_desc->pf_main;
                    t_arg.pv_arg  = NULL;
                    
                    if (pt_desc->pv_arg != NULL)
                    {
                        t_arg.pv_arg = x_mem_alloc(pt_desc->z_arg_size);
                        
                        if (t_arg.pv_arg != NULL)
                        {
                            x_memcpy(t_arg.pv_arg,
                                     pt_desc->pv_arg,
                                     pt_desc->z_arg_size);
                        }
                    }
                    
                    if ((pt_desc->pv_arg == NULL) || (t_arg.pv_arg != NULL))
                    {
                        AEE_T*  pt_parent_aee;
                        SIZE_T  z_stack_size;
                        UINT8   ui1_priority;
                        
                        z_stack_size = pt_desc->t_thread_desc.z_stack_size;
                        ui1_priority = pt_desc->t_thread_desc.ui1_priority;
                        
                        i4_res =
                            x_thread_get_pvt(PVT_KEY_AEE,
                                             (VOID**) & pt_parent_aee);
                        
                        if ((i4_res == OSR_OK) && (pt_parent_aee != NULL))
                        {
                            pt_aee->h_parent_app = pt_parent_aee->h_app;
                        }
                        else
                        {
                            pt_aee->h_parent_app = NULL_HANDLE;
                        }
                        
                        /* create thread */
                        i4_res = x_thread_create(& pt_aee->h_thread,
                                                 ps_name,
                                                 z_stack_size,
                                                 ui1_priority,
                                                 aee_main,
                                                 sizeof(t_arg),
                                                 & t_arg);
                        
                        if (i4_res == OSR_OK)
                        {
                            i4_res = AEER_OK;
                        }
                        else
                        {
                            handle_free(*ph_app, FALSE);
                            
                            i4_res = AEER_FAIL;
                        }
                    }
                    else
                    {
                        handle_free(*ph_app, FALSE);
                        
                        i4_res = AEER_FAIL;
                    }
                }
                else
                {
                    handle_free(*ph_app, FALSE);
                    
                    i4_res = AEER_OUT_OF_HANDLES;
                }
            }
            else
            {
                if (pt_aee->h_mem_partition != NULL_HANDLE)
                {
                    mem_part_delete(pt_aee->h_mem_partition);
                }
                
                x_mem_free(pt_aee);
                
                i4_res = AEER_OUT_OF_HANDLES;
            }
        }
        else
        {
            x_mem_free(pt_aee);
            
            i4_res = AEER_FAIL;
        }
    }
    else
    {
        i4_res = AEER_OUT_OF_MEMORY;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: aee_get_handle_op
 *
 * Description: This API returns a handle to an application (different from the
 *              handle created when the AEE was created).
 *
 * Inputs:  ps_name  References the name of the AEE.
 *
 * Outputs: ph_app  Contains the application handle.
 *
 * Returns: AEER_OK              Routine successful.
 *          AEER_FAIL            No application with such name exists.
 *          AEER_OUT_OF_HANDLES  Out of handles.
 ----------------------------------------------------------------------------*/
INT32 aee_get_handle_op (const CHAR*  ps_name,
                         HANDLE_T*    ph_app)
{
    INT32  i4_res;
    
    i4_res = handle_alloc(AEET_SECONDARY_AEE,
                          NULL,
                          NULL,
                          aee_free_secondary_handle,
                          ph_app);
    
    if (i4_res == HR_OK)
    {
        AEE_T*  pt_aee;

        x_sema_lock(h_aee_sem, X_SEMA_OPTION_WAIT);

        pt_aee = aee_find_by_name(ps_name);

        if (pt_aee != NULL)
        {
            handle_set_obj(*ph_app, pt_aee);
            
            i4_res = AEER_OK;
        }
        else
        {
            x_handle_free(*ph_app);
            
            i4_res = AEER_FAIL;
        }
        
        x_sema_unlock(h_aee_sem);
    }
    else
    {
        i4_res = AEER_OUT_OF_HANDLES;
    }
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: aee_send_data_op
 *
 * Description: This API calls the application's receive_data function.
 *
 * Inputs:  h_app       Contains the application handle.
 *          ui4_type    Contains the type of data.
 *          pv_data     References the data to send.
 *          z_data_len  Contains the size (in bytes) of the pv_data buffer.
 *
 * Outputs: -
 *
 * Returns: AEER_OK          Routine successful.
 *          AEER_INV_HANDLE  Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 aee_send_data_op (HANDLE_T  h_app,
                        UINT32    ui4_data,
                        VOID*     pv_data,
                        SIZE_T    z_data_len)
{
    AEE_T*         pt_aee;
    HANDLE_TYPE_T  e_type;
    INT32          i4_res;
    
    i4_res = handle_get_type_obj(h_app, &e_type, (VOID**) &pt_aee);
    
    if ((i4_res == HR_OK) &&
        ((e_type == AEET_PRIMARY_AEE) || (e_type == AEET_SECONDARY_AEE)))
    {
        pt_aee->pf_receive_data(h_app, ui4_data, pv_data, z_data_len);
        
        i4_res = AEER_OK;
    }
    else
    {
        i4_res = AEER_INV_HANDLE;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: aee_grab_handle_resource
 *
 * Description: This API tries to grab (i.e. reserve) handle resources from the
 *              application pool, and checks the application's authorization
 *              flags. This API must be called from an application thread.
 *
 * Inputs:  ui8_flags  Contains the flags that specify the attempted
 *                     operation(s).
 *          ui2_handles  Contains the number of handles to reserve
 *
 * Outputs: ph_aux  Contains the application's auxilary handle.
 *
 * Returns: AEER_OK                Routine successful.
 *          AEER_OUT_OF_RESOURCES  Not enough resources available.
 *          AEER_FAIL              Routine failed.
 ----------------------------------------------------------------------------*/
INT32 aee_grab_handle_resource (UINT64     ui8_flags,
                                UINT16     ui2_handles,
                                HANDLE_T*  ph_aux)
{
#ifndef LINUX_TURNKEY_SOLUTION
    AEE_T*  pt_aee;
    INT32   i4_res;
    
    i4_res = x_thread_get_pvt(PVT_KEY_AEE, (VOID**) & pt_aee);
    
    if (i4_res == OSR_OK)
    {
        CRIT_STATE_T  t_state;
        
        t_state = x_crit_start();
        
        if (AEE_RESOURCE_OK(ui2_handles))
        {
            GRAB_RESOURCE(ui2_handles);
            
            *ph_aux = pt_aee->h_aux;
            
            i4_res = AEER_OK;
        }
        else
        {
            i4_res = AEER_OUT_OF_RESOURCES;
        }
        
        x_crit_end(t_state);
    }
    else
    {
        i4_res = AEER_FAIL;
    }

    return i4_res;
#else
    if (ph_aux != NULL)
    {
        *ph_aux = NULL_HANDLE;
    }
    return AEER_OK;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: aee_grab_memory_resource
 *
 * Description: This API tries to grab (i.e. reserve) memory resources from the
 *              application pool, and checks the application's authorization
 *              flags. This API must be called from an application thread.
 *
 * Inputs:  pt_aee  References the AEE structure.
 *          z_size  Contains the amount of memory to grab.
 *
 * Outputs: -
 *
 * Returns: AEER_OK                Routine successful.
 *          AEER_OUT_OF_RESOURCES  Not enough resources available.
 *          AEER_FAIL              Routine failed.
 ----------------------------------------------------------------------------*/
INT32 aee_grab_memory_resource (AEE_T*  pt_aee,
                                SIZE_T  z_memory)
{
#ifndef LINUX_TURNKEY_SOLUTION
    CRIT_STATE_T  t_state;
    INT32         i4_res;
    
    t_state = x_crit_start();
    
    if (AEE_RESOURCE_OK(z_memory))
    {
        GRAB_RESOURCE(z_memory);
        
        i4_res = AEER_OK;
    }
    else
    {
        i4_res = AEER_OUT_OF_RESOURCES;
    }
    
    x_crit_end(t_state);
    
    return i4_res;
#else
    return AEER_OK;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: aee_grab_resources
 *
 * Description: This API tries to grab (i.e. reserve) resources from the
 *              application pool, and checks the application's authorization
 *              flags. This API must be called from an application thread.
 *
 * Inputs:  ui8_flags       Contains the flags that specify the attempted
 *                          operation(s).
 *          z_memory        Contains the amount of memory to reserve
 *          ui8_files_size  Contains the files size to reserve
 *          ui2_files       Contains the number of files to reserve
 *          ui2_handles     Contains the number of handles to reserve
 *          ui2_threads     Contains the number of threads to reserve
 *          ui2_semaphores  Contains the number of semaphores to reserve
 *          ui2_msg_queues  Contains the number of message queues to reserve.
 *
 * Outputs: ph_aux  Contains the application's auxilary handle.
 *
 * Returns: AEER_OK                Routine successful.
 *          AEER_OUT_OF_RESOURCES  Not enough resources available.
 *          AEER_FAIL              Routine failed.
 ----------------------------------------------------------------------------*/
INT32 aee_grab_resources (UINT64     ui8_flags,
                          SIZE_T     z_memory,
                          UINT64     ui8_files_size,
                          UINT16     ui2_files,
                          UINT16     ui2_handles,
                          UINT16     ui2_threads,
                          UINT16     ui2_semaphores,
                          UINT16     ui2_msg_queues,
                          HANDLE_T*  ph_aux)
{
#ifndef LINUX_TURNKEY_SOLUTION
    AEE_T*  pt_aee;
    INT32   i4_res;
    
    i4_res = x_thread_get_pvt(PVT_KEY_AEE, (VOID**) & pt_aee);
    
    if (i4_res == OSR_OK)
    {
        CRIT_STATE_T  t_state;
        
        t_state = x_crit_start();

        if ((ui8_flags & pt_aee->ui8_flags) == ui8_flags)
        {
            if (AEE_RESOURCE_OK(z_memory)        &&
                AEE_RESOURCE_OK(ui8_files_size)  &&
                AEE_RESOURCE_OK(ui2_files)       &&
                AEE_RESOURCE_OK(ui2_handles)     &&
                AEE_RESOURCE_OK(ui2_threads)     &&
                AEE_RESOURCE_OK(ui2_semaphores)  &&
                AEE_RESOURCE_OK(ui2_msg_queues))
            {
                GRAB_RESOURCE(z_memory);
                GRAB_RESOURCE(ui8_files_size);
                GRAB_RESOURCE(ui2_files);
                GRAB_RESOURCE(ui2_handles);
                GRAB_RESOURCE(ui2_threads);
                GRAB_RESOURCE(ui2_semaphores);
                GRAB_RESOURCE(ui2_msg_queues);
                
                if (ph_aux != NULL)
                {
                    *ph_aux = pt_aee->h_aux;
                }
                
                i4_res = AEER_OK;
            }
            else
            {
                i4_res = AEER_OUT_OF_RESOURCES;
            }
        }
        else
        {
            i4_res = AEER_NOT_AUTHORIZED;
        }
        
        x_crit_end(t_state);
    }
    else
    {
        i4_res = AEER_FAIL;
    }

    return i4_res;
#else
    if (ph_aux != NULL)
    {
        *ph_aux = NULL_HANDLE;
    }
    return AEER_OK;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: aee_release_handle_resource
 *
 * Description: This API releases handle resources into the application pool.
 *              This API must be called from an application thread.
 *
 * Inputs:  ui2_handles  Contains the number of handles to release.
 *
 * Outputs: -
 *
 * Returns: AEER_OK    Routine successful.
 *          AEER_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
INT32 aee_release_handle_resource (UINT16  ui2_handles)
{
#ifndef LINUX_TURNKEY_SOLUTION
    AEE_T*  pt_aee;
    INT32   i4_res;
    
    i4_res = x_thread_get_pvt(PVT_KEY_AEE, (VOID**) & pt_aee);
    
    if (i4_res == OSR_OK)
    {
        CRIT_STATE_T  t_state;
        UINT32        ui4_overflow;
        
        ui4_overflow = 0;
        
        t_state = x_crit_start();
        
        RELEASE_RESOURCE(ui2_handles, ui2_max_handles);
        
        x_crit_end(t_state);
        
        if (ui4_overflow != 0)
        {
            DBG_ERROR((DBG_PREFIX"Handle resource release overflow\n"));
        }
        
        return AEER_OK;
    }
    else
    {
        return AEER_FAIL;
    }
#else
    return AEER_OK;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: aee_release_memory_resource
 *
 * Description: This API releases memory resources into the application pool.
 *              This API must be called from an application thread.
 *
 * Inputs:  pt_aee    References the AEE.
 *          z_memory  Contains the amount of memory (in bytes) to release.
 *
 * Outputs: -
 *
 * Returns: AEER_OK    Routine successful.
 *          AEER_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
INT32 aee_release_memory_resource (AEE_T*  pt_aee,
                                   SIZE_T  z_memory)
{
#ifndef LINUX_TURNKEY_SOLUTION
    CRIT_STATE_T  t_state;
    UINT32        ui4_overflow;
    
    ui4_overflow = 0;
    
    t_state = x_crit_start();
    
    RELEASE_RESOURCE(z_memory, z_max_memory);
    
    x_crit_end(t_state);
    
    if (ui4_overflow != 0)
    {
        DBG_ERROR((DBG_PREFIX"Memory resource release overflow\n"));
    }
    
    return AEER_OK;
#else
    return AEER_OK;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: aee_release_resources
 *
 * Description: This API releases resources into the application pool. This API
 *              must be called from an application thread.
 *
 * Inputs:  z_memory        Contains the amount of memory to release.
 *          ui8_files_size  Contains the files size to release.
 *          ui2_files       Contains the number of files to release.
 *          ui2_handles     Contains the number of handles to release.
 *          ui2_threads     Contains the number of threads to release.
 *          ui2_semaphores  Contains the number of semaphores to release.
 *          ui2_msg_queues  Contains the number of message queues to release.
 *
 * Outputs: -
 *
 * Returns: AEER_OK    Routine successful.
 *          AEER_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
INT32 aee_release_resources (SIZE_T  z_memory,
                             UINT64  ui8_files_size,
                             UINT16  ui2_files,
                             UINT16  ui2_handles,
                             UINT16  ui2_threads,
                             UINT16  ui2_semaphores,
                             UINT16  ui2_msg_queues)
{
#ifndef LINUX_TURNKEY_SOLUTION
    AEE_T*  pt_aee;
    INT32   i4_res;
    
    i4_res = x_thread_get_pvt(PVT_KEY_AEE, (VOID**) & pt_aee);
    
    if (i4_res == OSR_OK)
    {
        CRIT_STATE_T  t_state;
        UINT32        ui4_overflow;
        
        ui4_overflow = 0;
        
        t_state = x_crit_start();
        
        RELEASE_RESOURCE(z_memory, z_max_memory);
        RELEASE_RESOURCE(ui8_files_size, ui8_max_files_size);
        RELEASE_RESOURCE(ui2_files, ui2_max_files);
        RELEASE_RESOURCE(ui2_handles, ui2_max_handles);
        RELEASE_RESOURCE(ui2_threads, ui2_max_threads);
        RELEASE_RESOURCE(ui2_semaphores, ui2_max_semaphores);
        RELEASE_RESOURCE(ui2_msg_queues, ui2_max_msg_queues);
        
        x_crit_end(t_state);
        
        if (ui4_overflow != 0)
        {
            DBG_ERROR((DBG_PREFIX"Resource release overflow (0x%08x)\n",
                       ui4_overflow));
        }
        
        return AEER_OK;
    }
    else
    {
        return AEER_FAIL;
    }
#else
    return AEER_OK;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: aee_get_resource_info_op
 *
 * Description: This API returns information about the AEE (e.g. maximum number
 *              of handles the application can allocate, etc)
 *
 * Inputs:  h_app     Contains the application handle.
 *          e_res_id  Contains the id of the resource information to get.
 *
 * Outputs: pv_res    Contains the value of the resource information.
 *
 * Returns: AEER_OK       Routine successful.
 *          AEER_INV_ARG  Invalid argument.
 ----------------------------------------------------------------------------*/
INT32 aee_get_resource_info_op (HANDLE_T           h_app,
                                AEE_RESOURCE_ID_T  e_res_id,
                                VOID*              pv_res)
{
    AEE_T*         pt_aee;
    HANDLE_TYPE_T  e_type;
    INT32          i4_res;
    
    i4_res = handle_get_type_obj(h_app, & e_type, (VOID**) & pt_aee);
    
    if ((i4_res == HR_OK) &&
        ((e_type == AEET_PRIMARY_AEE) || (e_type == AEET_SECONDARY_AEE)))
    {
        AEE_MAX_RESOURCES_T  t_max_res;
        AEE_RESOURCES_T      t_res;
        CRIT_STATE_T         t_state;
        
        t_state = x_crit_start();
        
        t_max_res = pt_aee->t_max_res;
        t_res     = pt_aee->t_res;
        
        x_crit_end(t_state);
        
        switch (e_res_id)
        {
            case AEE_RES_MIN_MEMORY_ID:
                *((SIZE_T *) pv_res) = t_max_res.z_min_memory;
                break;
                
            case AEE_RES_MAX_MEMORY_ID:
                *((SIZE_T *) pv_res) = t_max_res.z_max_memory;
                break;
                
            case AEE_RES_MAX_FILES_SIZE_ID:
                *((UINT64 *) pv_res) = t_max_res.ui8_max_files_size;
                break;
                
            case AEE_RES_MAX_FILES_ID:
                *((UINT16 *) pv_res) = t_max_res.ui2_max_files;
                break;
                
            case AEE_RES_MAX_HANDLES_ID:
                *((UINT16 *) pv_res) = t_max_res.ui2_max_handles;
                break;
                
            case AEE_RES_MAX_THREADS_ID:
                *((UINT16 *) pv_res) = t_max_res.ui2_max_threads;
                break;
                
            case AEE_RES_MAX_SEMAPHORES_ID:
                *((UINT16 *) pv_res) = t_max_res.ui2_max_semaphores;
                break;
                
            case AEE_RES_MAX_MSG_QUEUES_ID:
                *((UINT16 *) pv_res) = t_max_res.ui2_max_msg_queues;
                break;
                
            case AEE_RES_MAX_NB_MSGS_ID:
                *((UINT16 *) pv_res) = t_max_res.ui2_max_nb_msgs;
                break;
                
            case AEE_RES_MAX_MSG_SIZE_ID:
                *((UINT16 *) pv_res) = t_max_res.ui2_max_msg_size;
                break;
                
            case AEE_RES_MEMORY_ID:
                *((SIZE_T *) pv_res) = t_res.z_memory;
                break;
                
            case AEE_RES_FILES_SIZE_ID:
                *((UINT64 *) pv_res) = t_res.ui8_files_size;
                break;
                
            case AEE_RES_FILES_ID:
                *((UINT16 *) pv_res) = t_res.ui2_files;
                break;
                
            case AEE_RES_HANDLES_ID:
                *((UINT16 *) pv_res) = t_res.ui2_handles;
                break;
                
            case AEE_RES_THREADS_ID:
                *((UINT16 *) pv_res) = t_res.ui2_threads;
                break;
                
            case AEE_RES_SEMAPHORES_ID:
                *((UINT16 *) pv_res) = t_res.ui2_semaphores;
                break;
                
            case AEE_RES_MSG_QUEUES_ID:
                *((UINT16 *) pv_res) = t_res.ui2_msg_queues;
                break;
                
            default:
                i4_res = AEER_INV_ARG;
        }
    }
    else
    {
        i4_res = AEER_INV_HANDLE;
    }
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: aee_get_msg_queue
 *
 * Description: This API returns the message queue handle associated with the
 *              application. This is only used by the application manager part.
 *
 * Inputs:  h_app           Contains the application handle.
 *
 * Outputs: ph_msg_queue    Contains the message queue handle.
 *
 * Returns: AEER_OK          Routine successful.
 *          AEER_INV_HANDLE  Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 aee_get_msg_queue (HANDLE_T   h_app,
                         HANDLE_T*  ph_msg_queue)
{
    AEE_T*         pt_aee;
    HANDLE_TYPE_T  e_type;
    INT32          i4_res;
    
    i4_res = handle_get_type_obj(h_app, &e_type, (VOID**) &pt_aee);
    
    if ((i4_res == HR_OK) &&
        ((e_type == AEET_PRIMARY_AEE) || (e_type == AEET_SECONDARY_AEE)))
    {
        *ph_msg_queue = pt_aee->h_msg_queue;
        
        i4_res = AEER_OK;
    }
    else
    {
        i4_res = AEER_INV_HANDLE;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: aee_set_msg_queue
 *
 * Description: This API sets the message queue handle of the application.
 *              This is only used by the application manager part.
 *
 * Inputs:  h_app           Contains the application handle.
 *          h_msg_queue     Contains the message queue handle.
 *
 * Outputs: -
 *
 * Returns: AEER_OK          Routine successful.
 *          AEER_INV_HANDLE  Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 aee_set_msg_queue (HANDLE_T  h_app,
                         HANDLE_T  h_msg_queue)
{
    AEE_T*         pt_aee;
    HANDLE_TYPE_T  e_type;
    INT32          i4_res;
    
    i4_res = handle_get_type_obj(h_app, &e_type, (VOID**) &pt_aee);
    
    if ((i4_res == HR_OK) &&
        ((e_type == AEET_PRIMARY_AEE) || (e_type == AEET_SECONDARY_AEE)))
    {
        pt_aee->h_msg_queue = h_msg_queue;
        
        i4_res = AEER_OK;
    }
    else
    {
        i4_res = AEER_INV_HANDLE;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: aee_get_thread
 *
 * Description: This API returns the thread handle associated with the
 *              application. The thread handle is the handle of the thread
 *              created automatically when the AEE was created.
 *
 * Inputs:  h_app           Contains the application handle.
 *
 * Outputs: ph_thread       Contains the thread handle.
 *
 * Returns: AEER_OK          Routine successful.
 *          AEER_INV_HANDLE  Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 aee_get_thread (HANDLE_T   h_app,
                      HANDLE_T*  ph_thread)
{
    AEE_T*         pt_aee;
    HANDLE_TYPE_T  e_type;
    INT32          i4_res;
    
    i4_res = handle_get_type_obj(h_app, &e_type, (VOID**) &pt_aee);
    
    if ((i4_res == HR_OK) &&
        ((e_type == AEET_PRIMARY_AEE) || (e_type == AEET_SECONDARY_AEE)))
    {
        *ph_thread = pt_aee->h_thread;
        
        i4_res = AEER_OK;
    }
    else
    {
        i4_res = AEER_INV_HANDLE;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: aee_get_name
 *
 * Description: This API returns the name of the AEE, or NULL. Note that the
 *              name is not copied: the API returns a pointer to the internal
 *              name.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: the name of the AEE, or NULL.
 ----------------------------------------------------------------------------*/
const CHAR* aee_get_name (VOID)
{
    const CHAR*  ps_name;
    AEE_T*       pt_aee;
    INT32        i4_res;
    
    i4_res = x_thread_get_pvt(PVT_KEY_AEE, (VOID**) & pt_aee);
    
    ps_name = (i4_res == OSR_OK) ? pt_aee->s_name : NULL;
    
    return ps_name;
}
