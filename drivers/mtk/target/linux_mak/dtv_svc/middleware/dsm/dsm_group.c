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
 * $RCSfile: dsm_group.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file contains function for DSM group operation.
 *  
 *
 *---------------------------------------------------------------------------*/

#include "handle/x_handle.h"
#include "dsm/_dsm.h"
#include "dsm/dsm_comp.h"
#include "dsm/dsm_avc_group.h"

/*----------------------------------------------------------------------
 * Name: dsm_grab_sema
 *
 * Description: This function locks the SVL object.   The function blocks 
 *              until the lock is acquired.
 *
 * Inputs: 
 *   h_sema     Handle to the semaphore object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   None
 *
 -----------------------------------------------------------------------*/
static VOID dsm_grab_sema
(
    HANDLE_T           h_sema
) 
{
    if ( h_sema != NULL_HANDLE     &&
         x_sema_lock( h_sema, X_SEMA_OPTION_WAIT) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
    }
}

/*----------------------------------------------------------------------
 * Name: dsm_ungrab_sema
 *
 * Description: This function unlocks the semaphore. 
 *
 * Inputs: 
 *   h_sema     Handle to the semaphore object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK          SVL database is unlocked. 
 *   SVLR_INV_HANDLE  Bad SVL handle.
 *   
 -----------------------------------------------------------------------*/
static VOID dsm_ungrab_sema
(
    HANDLE_T           h_sema
) 
{
    if ( h_sema != NULL_HANDLE     &&
         x_sema_unlock( h_sema) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE);
    }
    return; 
}

/*--------------------------------------------------------
 * Name: dsm_grp_link_dev_hdle
 *
 * Description -
 *    Add the child handle that belongs to this group handle.
 *
 * Input  - 
 *    h_grp_hdle:  Handle to the group.
 *    h_dev_hdle:  Handle to the device belongs to this group.
 *
 * Output - None
 *
 * Return - 
 *    DSMR_OK
 *    DSMR_INV_ARG
 *    DSMR_FAIL
----------------------------------------------------------*/

INT32 dsm_grp_link_dev_hdle
(
    HANDLE_T           h_grp_hdle,
    HANDLE_T           h_dev_hdle
)
{
    INT32              i4_rc;
    DSM_GROUP_BLK_T*   pt_ctrl;
    HANDLE_TYPE_T      e_type;
    DSM_DEV_LNK_OBJ_T* pt_obj = NULL ;

    if ( handle_get_type_obj(h_grp_hdle, &e_type, ((VOID**) &pt_ctrl))
         == HR_OK                     &&
         (e_type == DSMT_GROUP_T)     &&
         (pt_ctrl != NULL )
        )
    {
        dsm_grab_sema(pt_ctrl->h_grp_sema);
        
        /*
          allocate a link object to store the childe handle.
        */
        pt_obj = (DSM_DEV_LNK_OBJ_T*) x_mem_alloc(sizeof(DSM_DEV_LNK_OBJ_T));
        if ( pt_obj != NULL )
        {
            pt_obj->h_dev_hdle=h_dev_hdle;
            pt_obj->pt_next = NULL;
            
            if ( pt_ctrl->pt_child_hdle_list == NULL )
            {
                pt_ctrl->pt_child_hdle_list = pt_obj;
            }
            else
            {
                DSM_DEV_LNK_OBJ_T*     pt_curr_obj;

                pt_curr_obj = pt_ctrl->pt_child_hdle_list;
                
                /* Add to the end of list. */
                while ( pt_curr_obj != NULL )
                {
                    if ( pt_curr_obj->pt_next == NULL )
                    {
                        /* attach the new link object at the tail. */
                        pt_curr_obj->pt_next = pt_obj;

                        break;
                    }
                    else
                    {
                        /* goto next object and continue looking for
                           the end of list. */
                        pt_curr_obj = pt_curr_obj->pt_next;   
                    }
                }
            }
            i4_rc = DSMR_OK;
        }
        else
        {
            i4_rc = DSMR_OUT_OF_MEM;
        }
        
        dsm_ungrab_sema(pt_ctrl->h_grp_sema);
    }
    else
    {
        i4_rc = DSMR_INV_ARG;
    }
    return  i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_grp_delink_dev_hdle
 *
 * Description -
 *    Delink the device handle from the list for the specified group. 
 *
 * Input  - 
 *    h_grp_hdle:  Handle to the group.
 *    h_dev_hdle:  Handle to the device belongs to this group.
 *
 * Output - None
 *
 * Return -
 *    DSMR_OK
 *    DSMR_FAIL
 --------------------------------------------------------------------------*/
INT32 dsm_grp_delink_dev_hdle
(
    HANDLE_T           h_grp_hdle,
    HANDLE_T           h_dev_hdle
)
{
    INT32              i4_rc;
    HANDLE_TYPE_T      e_type;
    DSM_GROUP_BLK_T*   pt_ctrl;
    
    if ( handle_get_type_obj(h_grp_hdle, &e_type, ((VOID**) &pt_ctrl))
         == HR_OK                     &&
         (e_type == DSMT_GROUP_T)     &&
         (pt_ctrl != NULL )
        )
    {
        dsm_grab_sema(pt_ctrl->h_grp_sema);
        
        if ( pt_ctrl->pt_child_hdle_list != NULL )
        {
            DSM_DEV_LNK_OBJ_T*     pt_curr_obj;
            DSM_DEV_LNK_OBJ_T*     pt_prev_obj;

            pt_curr_obj = pt_ctrl->pt_child_hdle_list ;
            pt_prev_obj = pt_ctrl->pt_child_hdle_list ;

            i4_rc = DSMR_DEV_NOT_FOUND ;
                
            /* Find the child handle in the list. */
            while ( pt_curr_obj != NULL )
            {
                if ( pt_curr_obj->h_dev_hdle == h_dev_hdle )
                {
                    /* de-link object, the object is the head of the list. */
                    if ( pt_curr_obj == pt_ctrl->pt_child_hdle_list )
                    {
                        pt_ctrl->pt_child_hdle_list = pt_curr_obj->pt_next;
                    }
                    else
                    {
                        pt_prev_obj->pt_next = pt_curr_obj->pt_next;
                    }
                    /*
                       free the memory associate with the link object.
                    */
                    x_mem_free(pt_curr_obj);
                    i4_rc = DSMR_OK;
                    DBG_API(("{DSM} delink dev hdle holder obj [%d] from the group handle's child list.\n",
                             h_dev_hdle));
                    break;
                }
                else
                {
                    /* goto next object and continue looking for
                       the end of list. */
                    pt_curr_obj = pt_curr_obj->pt_next;
                    pt_prev_obj = pt_curr_obj;
                }
            }
        }
        else
        {
            /* device handle link list is empty, just return 0k. */
            i4_rc = DSMR_OK;
        }
        dsm_ungrab_sema(pt_ctrl->h_grp_sema);
    }
    else
    {
        i4_rc = DSMR_INV_ARG;
    }
    return  i4_rc;
}


/*--------------------------------------------------------------------------
 * Name: dsm_grp_free_all_dev_hdle
 *
 * Description - API to free all the device handle belongs to this
 *               group handle and remove the link list of device handles.
 *
 * Input  - 
 *    h_grp_hdle:  Handle to the group.
 *    
 * Output - None
 *
 * Return
 *    DSMR_OK:   
 *    DSMR_FAIL
 --------------------------------------------------------------------------*/
INT32 dsm_grp_del_all_dev_hdle
(
    HANDLE_T           h_grp_hdle
)
{
    INT32              i4_rc;
    HANDLE_TYPE_T      e_type;
    DSM_GROUP_BLK_T*   pt_ctrl;

    if ( handle_get_type_obj(h_grp_hdle, &e_type, ((VOID**) &pt_ctrl))
         == HR_OK                     &&
         (e_type == DSMT_GROUP_T)     &&
         (pt_ctrl != NULL )
        )
    {
        dsm_grab_sema(pt_ctrl->h_grp_sema);
        
        if ( pt_ctrl->pt_child_hdle_list != NULL )
        {
            DSM_DEV_LNK_OBJ_T*     pt_curr_obj;
            DSM_DEV_LNK_OBJ_T*     pt_next_obj;

            pt_curr_obj = pt_ctrl->pt_child_hdle_list ;

            /*
              Iterate through the child handle list, and free all
              the child handle belonging to this group handle.
            */
            while ( pt_curr_obj != NULL )
            {
                /*
                  Save the next link object, we need to do this before
                  calling x_handle_free() on the current object.  Because
                  the current link object will be freed during by the
                  x_handle_free() operation.
                */
                pt_next_obj=pt_curr_obj->pt_next;
                                
                x_handle_free(pt_curr_obj->h_dev_hdle);

                /* goto next object 
                */
                pt_curr_obj = pt_next_obj;
            }   
        }

        dsm_ungrab_sema(pt_ctrl->h_grp_sema);
        
        i4_rc = DSMR_OK;
    }
    else
    {
        i4_rc = DSMR_INV_ARG;
    }
        
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_group_get_ops
 *
 * Description: This API dispatch the get operation command to
 *              the specific DSM device group.
 *
 * Input  -
 *    t_dev_type_name:  Specifies the device name.
 *
 *    pt_dsm_obj: Pointer to the internal DSM device object structure.
 *
 *    t_get_type    specifies the get operation to be performed
 *                  on the specified device.  This flag is defined by
 *                  specific device.
 *
 *    pv_get_info   optional parameters for the get operation.
 *
 *    pz_size       yhe size of buffer pointed to by the pv_data. 
 *
 * Output:
 *
 *    pz_size       the number of valid bytes in the buffer pointed
 *                  to by the pv_data. 
 *
 *    pv_data       the data returned by the get operation.*     
 *
 * Return -
 *   DSMR_OK:   Get operation is success.
 *   DSMR_FAIL: Internal error.
 *          
 --------------------------------------------------------------------------*/
INT32 dsm_group_get_ops
(
    DEVICE_TYPE_T    t_dev_type_name,
    DSM_COMP_T*      pt_dsm_obj,
    DSM_DEV_GET_T    t_get_type,
    VOID*            pv_get_info,
    SIZE_T*		     pz_size,
    VOID*            pv_data
)
{
    INT32                i4_rc;
    DEVICE_TYPE_T        t_group;
    
    t_group = DEV_GET_GROUP_ID(t_dev_type_name);

    switch (t_group)
    {
        case DEV_TUNER:
        {
            i4_rc = DSMR_DEV_TYPE_NOT_SUPPORTED ;
        }
        break;

        case DEV_1394:
        {
            i4_rc = DSMR_DEV_TYPE_NOT_SUPPORTED ;
        }
        break;

        case DEV_AVC:
        {
            i4_rc = dsm_avc_group_get_ops(pt_dsm_obj,
                                          t_get_type,
                                          pv_get_info,
                                          pz_size,
                                          pv_data);
        }
        break;
        
        default:
        {
            i4_rc = DSMR_DEV_NOT_FOUND;
        }
        break;
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_group_set_ops
 *
 * Description: This API dispatch the set operation command to
 *              the specific DSM driver group.
 *
 * Input  -
 *    t_dev_type_name:  Specifies the device name.
 *
 *    pt_dsm_obj: Pointer to the internal DSM device object structure.
 *
 *    t_set_type    specifies the set operation to be performed
 *                  on the specified device.  This flag is defined by
 *                  specific device.
 *
 *    pv_set_info   optional parameters for the set operation.
 *
 *    pv_data       Optional data for the set operation. For tuner and AVC
 *                  device, this argument is the size of the buffer
 *                  referenced by 'pv_set_info' argument.
 *
 * Output:
 *    None.
 *
 * Return -
 *   DSMR_OK:   Set operation is success.
 *   DSMR_FAIL: Internal error.
 *          
 --------------------------------------------------------------------------*/
INT32 dsm_group_set_ops
(
    DEVICE_TYPE_T    t_dev_type_name,
    DSM_COMP_T*      pt_dsm_obj,
    DSM_DEV_SET_T    t_set_type,
    VOID*            pv_set_info,
    SIZE_T           z_size
)
{
    INT32                i4_rc;
    DEVICE_TYPE_T        t_group;
    SIZE_T               z_set_info_len;
    
    t_group = DEV_GET_GROUP_ID(t_dev_type_name);

    switch (t_group)
    {
        case DEV_TUNER:
        {
            i4_rc = DSMR_DEV_TYPE_NOT_SUPPORTED ;
        }
        break;

        case DEV_1394:
        {
            i4_rc = DSMR_DEV_TYPE_NOT_SUPPORTED ;
        }
        break;

        case DEV_AVC:
        {
            z_set_info_len = z_size;
            i4_rc = dsm_avc_group_set_ops(pt_dsm_obj,
                                          t_set_type,
                                          pv_set_info,
                                          z_set_info_len);
        }
        break;
        
        default:
        {
            i4_rc = DSMR_DEV_NOT_FOUND;
        }
        break;
    }
    return i4_rc;
}

