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
 * $RCSfile: dsm_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file contains implementation of the exportted Device Status
 *    Manager (DSM) API's.         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "dsm/_dsm.h"

/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/

static BOOL dsm_is_valid_device(DEVICE_TYPE_T ui4_dsm_id)
{
    BOOL   b_rc;
    
    switch (DEV_GET_GROUP_ID(ui4_dsm_id))
    {
        case DEV_1394:
        {
            b_rc = (DEV_GET_DEV_ID(ui4_dsm_id) > DEV_UNKNOWN) &&
                (DEV_GET_DEV_ID(ui4_dsm_id) < DEV_1394_RESERVED )
                ? TRUE : FALSE ;
        }
        break;

        case DEV_TUNER:
        {
            b_rc = (DEV_GET_DEV_ID(ui4_dsm_id) > DEV_UNKNOWN) &&
                (DEV_GET_DEV_ID(ui4_dsm_id) < DEV_TUNER_RESERVED )
                ? TRUE : FALSE ;
        }
        break;

        case DEV_AVC:
        {
            b_rc = (DEV_GET_DEV_ID(ui4_dsm_id) > DEV_UNKNOWN) &&
                (DEV_GET_DEV_ID(ui4_dsm_id) < DEV_AVC_RESERVED )
                ? TRUE: FALSE ;
        }
        break;

        default:
        {                                
            b_rc = FALSE;
        }
        break;
    }
    return b_rc;
}
/*-----------------------------------------------------------------------------
 * Name: dsm_grp_handle_free_cb 
 *
 * Description: This API is called whenever handle_free() function 
 *              for a handle is called.  It free the resource for a
 *              dsm_group handle.
 *
 * Inputs:  h_hdl         Contains the handle to dsm client.
 *          e_type        Contains the core db client handle type and must be
 *                        set to  DSMT_GROUP_T.
 *          pv_obj        References the client object structure.
 *          pv_tag        Ignored.
 *          b_req_handle  Is set to TRUE if the handle free was initiated with
 *                        this handle else FALSE.
 *
 * Outputs: None
 *
 * Returns: TRUE:   handle control block resource is freed. The handle
 *                  library should continue with freeing of this handle.
 *          FALSE:  handle control block resource is not free. The
 *                  handle library should not delete this handle.
 ---------------------------------------------------------------------------*/
static BOOL dsm_grp_handle_free_cb
(
    HANDLE_T       h_hdl,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle
)
{
    BOOL                 b_rc;
    DSM_GROUP_BLK_T*     pt_dsm_ctrl;
    
    b_rc=FALSE;
    
    /* Abort if this is not the correct type. */
    if (e_type ==  DSMT_GROUP_T )
    {   
        pt_dsm_ctrl = (DSM_GROUP_BLK_T*) pv_obj;

        /*
           De-link the client handle to the handle for this
           device component.
        */
        if ( pt_dsm_ctrl->h_client != h_hdl )
        {
            /*
              Something is wrong
            */
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE);
        }

        /*
          Free all the child handles.
        */
        dsm_grp_del_all_dev_hdle(h_hdl);
        
        /*
          delete the semaphore.
        */
        x_sema_delete(pt_dsm_ctrl->h_grp_sema);
                    
        /*
          free the handle control block for the DSM handle.
        */
        DBG_API(("{DSM} free control block structure for the DSM group handle.\n"));
        x_mem_free(pt_dsm_ctrl);

        /*
          Set the return value to TRUE so the handle library
          will free up this handle.
        */
        b_rc = TRUE;
    }
    else
    {
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
    }
    return b_rc;
}

/*-----------------------------------------------------------------------------
 * Name: dsm_handle_free_cb 
 *
 * Description: This API is called whenever handle_free() function 
 *              is called.  It free the resource for a dsm_dev handle.
 *
 * Inputs:  h_hdl         Contains the handle to dsm client.
 *          e_type        Contains the core db client handle type and must be
 *                        set to  DSMT_DEVICE_T.
 *          pv_obj        References the client object structure.
 *          pv_tag        Ignored.
 *          b_req_handle  Is set to TRUE if the handle free was initiated with
 *                        this handle else FALSE.
 *
 * Outputs: None
 *
 * Returns: TRUE:   handle control block resource is freed. The handle
 *                  library should continue with freeing of this handle.
 *          FALSE:  handle control block resource is not free. The
 *                  handle library should not delete this handle.
 ---------------------------------------------------------------------------*/
static BOOL dsm_handle_free_cb
(
    HANDLE_T       h_hdl,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle
)
{
    BOOL                b_rc;
    DSM_CTRL_BLK_T*     pt_dsm_ctrl;
    
    b_rc=FALSE;
    
    /* Abort if this is not the correct type. */
    if (e_type ==  DSMT_DEVICE_T )
    {   
        pt_dsm_ctrl = (DSM_CTRL_BLK_T*) pv_obj;

        
        if ( pt_dsm_ctrl->h_client == h_hdl )
        {
            /*
              De-link the client handle to the handle for this
              device component.
            */
            dsm_delink_client_hdle_with_comp(
                pt_dsm_ctrl->pt_dsm_comp, h_hdl );

            /*
              De-link from the parent's child list.
            */
            dsm_grp_delink_dev_hdle(pt_dsm_ctrl->h_grp_parent, h_hdl);
            
            /*
              free the handle control block for the DSM handle.
            */
            DBG_API(("{DSM} free dsm device handle [%d] and its control block structure.\n",h_hdl));
            x_mem_free(pt_dsm_ctrl);

            /*
              Set the return value to TRUE so the handle library
              will free up this handle.
            */
            b_rc = TRUE;
        }
        else
        {
            /*
              Something is wrong
            */
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE);
        }
    }
    else
    {
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
    }
    return b_rc;
}

/* --------------------------------------------------------------
  Function implementation.
---------------------------------------------------------------- */

/*----------------------------------------------------------------------
 * Name: x_dsm_open
 *
 * Description: This function create a handle to the a device interface
 *              group. 
 *
 * Inputs:
 *   t_dev_group    The interface specified is one of the following
 *                  interface:
 *                       DEV_1394, DEV_TUNER, DEV_AVC.
 *
 *   pv_tag         Private tag that will be passed to the client's
 *                  provided notification function. 
 *
 *   pf_nfy_fct     DSM device group notification function.  This function
 *                  is called when a new device is detected or removed on
 *                  that  DSM device group.
 *
 * Output:
 *   ph_dsm:	    Handle to an interface (e.g., a device group)
 *
 * Returns: 
 *   DSMR_OK               DSM handle is returned. 
 *   DSMR_OUT_OF_HANDLE    No more handle available.
 *   DSMR_OUT_OF_MEM       Not enough memory to create the DSM control 
 *                         structure.
 *   DSMR_INV_ARG          'ph_dsm' is NULL or t_dev_group is not a valid
 *                         groud.
 -----------------------------------------------------------------------*/
INT32  x_dsm_open
(
    DEVICE_TYPE_T       t_dev_group,
    const CHAR*         ps_name,
    VOID*               pv_tag,
    dsm_dev_nfy_fct     pf_nfy_fct,
    HANDLE_T*           ph_dsm
) 
{ 
    INT32               i4_rc;
    DSM_GROUP_BLK_T*    pt_ctrl;

    if ( ph_dsm  != NULL                                &&
         DEV_IS_VALID_GROUP(t_dev_group) == TRUE
        )
    {
        *ph_dsm = NULL_HANDLE;
        pt_ctrl  = NULL;

        /*
          allocate control block object.
        */
        pt_ctrl = (DSM_GROUP_BLK_T*) x_mem_alloc(sizeof(DSM_GROUP_BLK_T));
        if ( pt_ctrl != NULL )
        {
            x_memset(pt_ctrl,0,sizeof(DSM_GROUP_BLK_T));
            
            pt_ctrl->t_dsm_name = t_dev_group ;
            pt_ctrl->u.pf_grp_nfy = pf_nfy_fct ;
            pt_ctrl->pt_child_hdle_list = NULL;

            if ( ps_name != NULL )
            {
                x_strncpy(pt_ctrl->ps_buf,ps_name,DSM_BUF_LEN-1);
            }
            
            /* Create semaphore for the group operation. */
            
            if ( x_sema_create(&(pt_ctrl->h_grp_sema), X_SEMA_TYPE_MUTEX,
                          X_SEMA_STATE_UNLOCK) == OSR_OK
                )
            {            
                if ( handle_alloc(DSMT_GROUP_T, (VOID*)(pt_ctrl), pv_tag,
                                  dsm_grp_handle_free_cb, &(pt_ctrl->h_client))
                     == HR_OK
                    )
                {
                    *ph_dsm = pt_ctrl->h_client;
                    i4_rc=DSMR_OK;
                }
                else
                {
                    /*
                      delete the semaphore.
                    */
                    x_sema_delete(pt_ctrl->h_grp_sema);

                    /*
                      delete the group handle control block structure.
                    */
                    x_mem_free(pt_ctrl);
                    i4_rc=DSMR_OUT_OF_HANDLE;
                }
            }
            else
            {
                /*
                   delete the group handle control block structure.
                */
                x_mem_free(pt_ctrl);
                i4_rc = DSMR_FAIL;
            }
        }
        else
        {
            i4_rc = DSMR_OUT_OF_MEM ;
        }        
    }
    else
    {
        i4_rc = DSMR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_dsm_dev_open
 *
 * Description: This function creates a handle to an instance of source 
 *              device attached to the system.
 *
 * Inputs: 
 *   h_dsm:	           Handle to the DSM interface which this device
 *                     type belongs to.
 *
 *   t_dev_type        Specifies the device name belonging to this
 *                     'h_dsm' device interface .
 *
 *   ps_name           string to pass to the Resource Manager 
 *
 *   ui2_idx           iterate for next instance of the device of the
 *                     specified device type name.
 *
 *   pv_tag            Private tag that will be passed to the client's
 *                     provided notification function. 
 *
 *   pf_dev_nfy_fct    DSM device notification function.  This function
 *                     will be called in the context of the DSM thread to
 *                     notify application of any change in the status of
 *                     the attached device.
 *
 * Outputs:
 *   ph_dsm_dev        handle to an instance of a device.
 *
 * Returns: 
 *   DSMR_OK             DSM handle is returned. 
 *   DSMR_OUT_OF_HANDLE  No more handle available.
 *   DSMR_OUT_OF_MEM     Not enough memory to create the DSM handle structure.
 *   DSMR_DEV_TYPE_NOT_SUPPORTED  The specified DSM name is not found.
 *   DSMR_INV_ARG        'h_dsm' is NULL_HANDLE, 'ph_dsm_dev' is NULL.
 *   DSMR_INV_HANDLE     'h_dsm' is wrong handle for the interface group
 *                       that this device belongs to.
 -----------------------------------------------------------------------*/
INT32  x_dsm_dev_open 
(
    HANDLE_T         h_dsm,
    DEVICE_TYPE_T    t_dev_type,
    const CHAR*      ps_name,
    UINT16			 ui2_idx,
    VOID*            pv_tag,
    dsm_dev_nfy_fct  pf_dev_nfy_fct,
    HANDLE_T*        ph_dsm_dev	
) 
{
    INT32           i4_rc;
    DSM_CTRL_BLK_T* pt_ctrl;
    HANDLE_TYPE_T   e_type;
    DSM_COMP_T*     pt_dsm_comp;
    

    if ( h_dsm           != NULL_HANDLE               &&
         ph_dsm_dev      != NULL                      &&
         DEV_IS_VALID_GROUP(t_dev_type) == TRUE
        )
    {
        
        *ph_dsm_dev = NULL_HANDLE;
        pt_ctrl  = NULL;
        
        if ( handle_get_type_obj(h_dsm, &e_type, ((VOID**) &pt_ctrl))
             == HR_OK                     &&
             (e_type == DSMT_GROUP_T)    &&
             (pt_ctrl != NULL )
            )
        {
            /* check the handle with the dev_type */
            if (  DEV_GET_GROUP_ID(pt_ctrl->t_dsm_name) ==
                  DEV_GET_GROUP_ID(t_dev_type)
                )
            {
                if ( dsm_is_valid_device(t_dev_type) == TRUE )
                {
                    i4_rc = dsm_open_comp(t_dev_type,
                                          ui2_idx,
                                          ps_name,
                                          &(pt_dsm_comp)) ;
                
                    if ( i4_rc == DSMR_OK )
                    {
                        /*
                          allocate control block object.
                        */
                        pt_ctrl = (DSM_CTRL_BLK_T*) x_mem_alloc(
                            sizeof(DSM_CTRL_BLK_T));
        
                        if ( pt_ctrl != NULL )
                        {
                            if ( handle_alloc(DSMT_DEVICE_T, (VOID*)(pt_ctrl),
                                              pv_tag,
                                              dsm_handle_free_cb,
                                              &(pt_ctrl->h_client))
                                 == HR_OK
                                )
                            {
                                pt_ctrl->t_dsm_name = t_dev_type ;
                                pt_ctrl->u.pf_dev_nfy = pf_dev_nfy_fct ;
                                pt_ctrl->pt_dsm_comp = pt_dsm_comp;
                                pt_ctrl->h_grp_parent = h_dsm;

                                DBG_INFO(("{DSM} rm drv handle: %d "
                                          "t_drv_type:  %d  "
                                          "comp id:     %d  "
                                          "device type id: %d \n",
                                          pt_dsm_comp->h_rm_hdle,
                                          pt_dsm_comp->t_drv_type,
                                          pt_dsm_comp->ui2_comp_id,
                                          pt_dsm_comp->t_dev_type_name));
                            
                                *ph_dsm_dev = pt_ctrl->h_client;

                                /*
                                  Store the handle in the device
                                  component structure, so we can later
                                  refer to its handle control structure.
                                */
                                pt_dsm_comp->h_dsm_dev = *ph_dsm_dev;
                                
                                /*
                                  Link the client handle to the handle for this
                                  device component .
                                */
                                i4_rc = dsm_link_client_hdle_with_comp(
                                    pt_dsm_comp, pt_ctrl->h_client);

                                /*
                                  link the device handle to the list of
                                  parent's child handles.
                                */
                                dsm_grp_link_dev_hdle(h_dsm, pt_ctrl->h_client);
                            }
                            else
                            {
                                x_mem_free(pt_ctrl);
                                i4_rc=DSMR_OUT_OF_HANDLE;
                            }
                        }
                        else
                        {
                            i4_rc = DSMR_OUT_OF_MEM ;
                        }
                    }
                    else
                    {
                        DBG_ERROR(("{DSM} can not open RM handle for "
                                   "dev type: %d, idx: %d, name: %s\n",
                                   t_dev_type, ui2_idx, ps_name));
                    }
                }
                else
                {
                    i4_rc = DSMR_DEV_TYPE_NOT_SUPPORTED;
                }
            }
            else
            {
                i4_rc = DSMR_INV_ARG;
            }
        }
        else
        {
            i4_rc = DSMR_INV_HANDLE;
        }
    }
    else
    {
        i4_rc = DSMR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_dsm_get
 *
 * Description: Get information on a device group.
 * 
 * Input:
 *
 *    h_dsm:        handle to device group.
 *
 *    t_get_type    specifies the get operation to be performed on
 *                  the device group.  The set type is device group
 *                  specific.  
 *
 *
 *    pv_get_info   optional parameters for the get operation.
 *
 *    pz_size       the size of buffer pointed to by the pv_data. 
 *
 * Output:
 *
 *    pz_size       the number of valid bytes in the buffer pointed
 *                  to by the pv_data. 
 *
 *    pv_data       the data returned by the get operation.
 *
 * Returns: 
 *   DSMR_OK           Information about this device is returned
 *   DSMR_INV_HANDLE   'h_dsm' is a invalid.
 *   
 -----------------------------------------------------------------------*/
INT32  x_dsm_get
(
    HANDLE_T         h_dsm,
    DSM_GET_T  		 t_get_type,
    VOID*            pv_get_info,
    SIZE_T*		     pz_size,
    VOID*            pv_data
)
{ 
    INT32            i4_rc;
    DSM_GROUP_BLK_T* pt_grp_ctrl;
    HANDLE_TYPE_T    e_type;
    
    if ( (handle_get_type_obj(h_dsm, &e_type,
                              ((VOID**)&pt_grp_ctrl))
          == HR_OK)                           &&
         (e_type      == DSMT_GROUP_T)       &&
         (pt_grp_ctrl  != NULL)
        )
    {
        i4_rc = dsm_group_get_ops(pt_grp_ctrl->t_dsm_name,
                                  pt_grp_ctrl->pt_dsm_comp,
                                  t_get_type,
                                  pv_get_info,
                                  pz_size,
                                  pv_data);
    }
    else
    {
        i4_rc = DSMR_INV_HANDLE;
    }
    return i4_rc;     
}

/*----------------------------------------------------------------------
 * Name: x_dsm_dev_get
 *
 * Description: Get information on an instance of a specific device.
 * 
 * Input:
 *
 *    h_dsm_dev     handle to an instance of device.
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
 *    pv_data       the data returned by the get operation.
 *
 * Returns: 
 *   DSMR_OK           Data from the specified device is returned. 
 *   DSMR_INV_HANDLE   ' h_dsm_dev' is invalid.
 *   
 -----------------------------------------------------------------------*/
INT32  x_dsm_dev_get 
(
    HANDLE_T         h_dsm_dev,
    DSM_DEV_GET_T    t_get_type,
    VOID*            pv_get_info,
    SIZE_T*		     pz_size,
    VOID*            pv_data
)
{
    INT32            i4_rc;
    DSM_CTRL_BLK_T*  pt_dsm_ctrl;
    HANDLE_TYPE_T    e_type;
    
    pt_dsm_ctrl = NULL ;
        
    if ( (handle_get_type_obj(h_dsm_dev, &e_type,
                              ((VOID**) &pt_dsm_ctrl))
          == HR_OK)                                  &&
         (e_type == DSMT_DEVICE_T)                  &&
         (pt_dsm_ctrl  != NULL)
        )
    {
        i4_rc = dsm_comp_get_ops(pt_dsm_ctrl->t_dsm_name,
                                 pt_dsm_ctrl->pt_dsm_comp,
                                 t_get_type,
                                 pv_get_info,
                                 pz_size,
                                 pv_data);
    }
    else
    {
        i4_rc = DSMR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_dsm_set
 *
 * Description: Perform set operation on a device group.
 *		
 * Input:
 *
 *    h_dsm_dev    handle to a device group.
 *
 *    t_set_type   specify the set command.	 The set type is 
 *                 device group specific.  
 *
 *    pv_set_info  Optional parameter for the set command.
 *                 If the 'h_dsm_dev' is a 1394 device group, then
 *                 'pv_set_info' is directly mapped to the 'pv_set_info'
 *                 information required for the FWM (Firewire) set API
 *                 function.
 *
 *                 If the 'h_dsm_dev' is an AVC device group, then this
 *                 argument corresponds to the 'pv_set_info' argument in
 *                 the *(x_rm_set_fct)() function.				 
 *
 *    z_size:      
 *
 * Returns: 
 *   DSMR_OK         Number of records found.
 *   DSMR_INV_HANDLE Bad 'h_dsm' handle.
 *   DSMR_INV_ARG    'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32  x_dsm_set
(
    HANDLE_T         h_dsm_dev,
    DSM_SET_T        t_set_type,
    VOID*            pv_set_info,
    SIZE_T           z_size
)
{
    INT32            i4_rc;
    DSM_GROUP_BLK_T* pt_grp_ctrl;
    HANDLE_TYPE_T    pe_type;
    
    pt_grp_ctrl = NULL ;
        
    if ( (handle_get_type_obj(h_dsm_dev, &pe_type,
                              ((VOID**) &pt_grp_ctrl))
          == HR_OK)                                     &&
         (pe_type == DSMT_GROUP_T)                 &&
         (pt_grp_ctrl  != NULL)
        )
    {
        i4_rc = DSMR_OK;
    }
    else
    {
        i4_rc = DSMR_INV_HANDLE;
    }
        
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_dsm_dev_set
 *
 * Description: Perform set operation on an instance of a specified device.
 *	
 * Input:
 *
 *    h_dsm_dev     handle to an instance of device.
 *
 *    t_set_type    specify the set command. The set type is
 *                  device specific.
 *    pv_set_info   Optional parameter for the set command.
 *                  If the 'h_dsm_dev' is a 1394 device, then
 *                  'pv_set_info' is directly mapped to the 
 *                  'pv_set_info' information required for the
 *                   FWM (Firewire) set function.
 *
 *                  If the 'h_dsm_dev' is an AVC device, then this
 *                  argument corresponds to the 'pv_set_info' argument
 *                  in the *(x_rm_set_fct)
 *
 *    pv_data       Optional data for the set operation.
 *                  If 'h_dsm_dev' is a 1394 device, then this
 *                  parameter contains addition data for the set o
 *                  peration.
 *                  If 'h_dsm_dev' is an AVC device or Tuner device, 
 *                  then this argument is the size (length in byte) of 
 *                  the buffer referenced by the argument 'pv_set_info'.  
 *
 * Returns: 
 *   DSMR_OK           
 *   DSMR_INV_HANDLE
 -----------------------------------------------------------------------*/
INT32  x_dsm_dev_set
(
    HANDLE_T         h_dsm_dev,
    DSM_DEV_SET_T    t_set_type,
    VOID*            pv_set_info,
    SIZE_T           z_size
)
{
    INT32               i4_rc;
    DSM_CTRL_BLK_T*     pt_dsm_ctrl;
    HANDLE_TYPE_T       e_type;
 
    if ( (handle_get_type_obj(h_dsm_dev, &e_type,
                              ((VOID**) &pt_dsm_ctrl))
                       == HR_OK)                       &&
         (e_type      == DSMT_DEVICE_T)           &&
         (pt_dsm_ctrl  != NULL)
        )
    {
        /*
          Set notify flag to true so the driver notification
          is pass up to the client.  This flag is stored on
          a per-client info structure.
        */
        if ( t_set_type == DSM_SET_AVC_START_MONITOR )
        {
            pt_dsm_ctrl->b_mon_enabled = TRUE ;

            /*
              After the data monitor has started, the TV decoder
              will send a notification to the client when
              it starts to 'play'.
              
              The boolean flag 'b_send_nfy_once' is used to
              prevent duplicate 'MONITOR_START' notification.
              This situation can arise because multiple
              DSM handles can shared the same single TV decoder.

              For example: DSM handle A issues 'DSM_SET_AVC_START_MONITOR'
              command, it receives a notification from the TV decoder
              that monitoring has started after TV decoder starts to
              'play'.
              
              Then DSM handle B issues 'DSM_SET_AVC_START_MONITOR', the TV
              decoder will send another notification (it is
              already playing, so it will send 'play' notification)
              to the DSM layer.

              Since the DSM module uses Handle Link facility to
              notify all the DSM handles that shared
              the same TV decoder.  The handle A will get a duplicate
              notification of 'MONITOR_START' when handle B issues
              'DSM_SET_AVC_START_MONITOR' command.

              The boolean flag 'b_send_nfy_once' (a per DSM handle
              variable) is set to TRUE when 'DSM_SET_AVC_START_MONITOR'
              command is issued. When handle parse function iterates
              over the list of handle and calling the handle's notification
              function the it is set to FALSE once the notification
              for that handle is called.  The next iteratation of handle
              parse will prevent the duplicate call to the same notification
              function.

              Note: For AVC COMBI device, issuing DSM_SET_AVC_START_MONITOR will
              result in an asynchronous notication of MONITOR_START to the
              DSM handle. This is because AVC COMBI will internally enable TV Decoder
              'play' in order to monitor the signal. TV decoder 'play' is an
              asynchronous command.

              For AVC SCART devce, it does not require TV decoder for signal monitoring,
              therefore, issuing of DSM_SET_AVC_START_MONITOR will be non-synchronous,
              and no notification will be sent.

              Issueing DSM_SET_AVC_STOP_MONITOR is an synchronous call. It
              set the 'b_mon_enable' flag to FALSE to stop the notification going
              to the DSM handle.

              MONITOR_STOP can come from the driver layer (TV decoder). For
              example, when another component take TV decoder away from the DSM
              module.  Similary, MONOTOR_START can also come from the driver
              layer, when the TV decoder become available again for use again
              by the DSM module to monitor the signal.
            */
            pt_dsm_ctrl->b_send_nfy_once = TRUE ;
        }

        /*
           Set the  notify flag to false so the
           driver notification is not pass up to
           the client.  This flag is stored on a per-client
           info structure.
        */
        if ( t_set_type == DSM_SET_AVC_STOP_MONITOR )
        {
            pt_dsm_ctrl->b_mon_enabled = FALSE ;
        }

        /*
          Pass the set flag down to individual DSM component to
          perform necessary set operation.
        */
        i4_rc = dsm_comp_set_ops(pt_dsm_ctrl->t_dsm_name,
                                 pt_dsm_ctrl->pt_dsm_comp,
                                 t_set_type,
                                 pv_set_info,
                                 z_size);       
    }
    else
    {
        i4_rc = DSMR_INV_HANDLE;
    }
    return i4_rc;
}

