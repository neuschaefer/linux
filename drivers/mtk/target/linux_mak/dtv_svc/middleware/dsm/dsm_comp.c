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
 * $RCSfile: dsm_comp.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains DSM function interfacing with the
 *         tuner drivers.
 *         The interface to the tuner contains tuner specific types,
 *         component id, get and set commands recognized by the specific
 *         tuner component.
 *
 *---------------------------------------------------------------------------*/

#include "handle/x_handle.h"
#include "dsm/_dsm.h"
#include "dsm/dsm_comp.h"

/*--------------------------------------------------------
 * Name: dsm_init_comp_elem
 *
 * Description - Initialize data for the component structure.
 *               A dsm component structure contain information
 *               to access a specific device via the RM interface.
 *
 * Input  - 
 *    pt_data:   pointer to the component data structure.
 *
 * Output - None
 *
 * Return - Void
 *
----------------------------------------------------------*/
static VOID dsm_init_comp_elem(DSM_COMP_T* pt_data)
{
    if ( pt_data != NULL )
    {
        pt_data->h_rm_hdle=NULL_HANDLE;
        pt_data->t_drv_type=DRVT_UNKNOWN;
        pt_data->t_dev_type_name=DEV_UNKNOWN;
        pt_data->pt_next=NULL;
        pt_data->ui2_comp_id=0;
        pt_data->pv_private=NULL;
        
        /* Initialize the handle link structure. */
        if ( handle_link_init( &(pt_data->t_link)) != HR_OK )
        {
            ABORT(DBG_CAT_HANDLE, DBG_ABRT_OPS_FAIL);
        }
    }
    return;
}


/*--------------------------------------------------------------------------
 * Name: dsm_free_comp_elem
 *
 * Description - API that frees all component element(s) in a linked list. 
 *
 * Input  - 
 *    pt_data:   pointer to the component data structure.
 *
 * Output - None
 *
 * Return - Void
 *
 --------------------------------------------------------------------------*/
static VOID dsm_free_comp_elem
(
    DSM_COMP_T* pt_data
)
{
    DSM_COMP_T*     pt_comp;
    
    /*
      Free the overflow bucket, delete the next element in the list.
    */
    while ( pt_data != NULL         &&
            pt_data->pt_next != NULL    )
    {
        pt_comp = pt_data->pt_next ;
        pt_data->pt_next=pt_data->pt_next->pt_next;

        /*
          Notify the client of this DSM component that the
          device is closed.
        */
        
        x_mem_free(pt_comp);
    }

    /*
      Now free the head element
    */
    if ( pt_data != NULL )
    {
        x_mem_free(pt_data);
    }
    return;
}

/*--------------------------------------------------------------------------
 * Name: dsm_iterate_comp_elem
 *
 * Description - API that applies a client provided function on each
 *               element of the component selected.
 *
 * Input  - 
 *    pt_data:   pointer to the component data structure.
 *    ui4_idx:   index in the component array for this component.  
 *    pf_action: client provided function.
 *    pv_tag:  Tag value to pass to client provided action function.
 *
 * Output - None
 *
 * Return
 *    DSMR_OK:   all element are visited and client action is apply to it.
 *
 --------------------------------------------------------------------------*/
static INT32 dsm_iterate_comp_elem
(
    DSM_COMP_T*           pt_data,
    UINT32                ui4_idx,
    dsm_comp_action_fct   pf_action,
    VOID*                 pv_tag
)
{
    DSM_COMP_T*   pt_comp;
    UINT32        ui4_sub_idx;
    INT32         i4_rc;

    i4_rc = DSMR_OK;
    
    /*
      Start with the head object and then the next object in the
      list, until next object pointer is null.
    */
    pt_comp = pt_data ;
    ui4_sub_idx = 0 ;
    
    while ( pt_comp != NULL )
    {
        i4_rc = pf_action(pt_comp, ui4_idx, ui4_sub_idx, pv_tag);
        if ( i4_rc == DSMR_OK )
        {
            pt_comp = pt_comp->pt_next ;
            ui4_sub_idx++;
        }
        else
        {
            break;
        }
    }
    return i4_rc;
}
/*-----------------------------------------------------------------------------
 * Name: dsm_comp_handle_free_cb 
 *
 * Description: This API is called whenever handle_free() function 
 *              is called.  It free the resource for a dsm_comp_t handle.
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
static BOOL dsm_comp_handle_free_cb
(
    HANDLE_T       h_hdl,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle
)
{
    BOOL                b_rc = FALSE;
    DSM_COMP_T*         pt_dsm_comp;
    
    /* Abort if this is not the correct type. */
    if (e_type ==  DSMT_DSM_COMP_T )
    {   
        pt_dsm_comp = (DSM_COMP_T*) pv_obj;
        
        if ( pt_dsm_comp->h_this_dsm_comp == h_hdl )
        {
            /*
              Set the return value to TRUE so the handle library
              will free up this handle.

              Note: we don't free pt_dsm_comp yet, it will
              be free in dsm_avc_close_combi_comp()  after other
              resouces in DSM_COMP_T structure are released.
            */
            b_rc = TRUE;
        }
        else
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE);
        }
    }
    else
    {
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
    }
    return b_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_alloc_and_init_comp
 *
 * Description - Allocate and initialze the dsm component and handle
 *    that associate with this component.
 *
 * Input  - 
 *    pv_tag:  Tag value to pass to client provided action function.
 *
 * Output - None
 *
 * Return
 *    NULL:   can not allocate and initialize the dsm_comp structure.
 *    ptr to DSM_COMP_T structure.
 --------------------------------------------------------------------------*/
static DSM_COMP_T*  dsm_alloc_and_init_comp
(
    VOID*           pv_tag
)
{
    DSM_COMP_T*   pt_new_dsm_comp = NULL;
    HANDLE_T      h_this_dsm_comp;

    pt_new_dsm_comp = (DSM_COMP_T*) x_mem_alloc(sizeof(DSM_COMP_T)); 
    
    if ( pt_new_dsm_comp != NULL )
    {
        if ( handle_alloc(DSMT_DSM_COMP_T,
                          (VOID*)(pt_new_dsm_comp),
                          pv_tag,
                          dsm_comp_handle_free_cb,
                          &(h_this_dsm_comp))
             == HR_OK
            )
        {
            dsm_init_comp_elem(pt_new_dsm_comp);
            pt_new_dsm_comp->h_this_dsm_comp = h_this_dsm_comp ;
        }
        else
        {
            x_mem_free(pt_new_dsm_comp);
            pt_new_dsm_comp = NULL ;
        }
    }
    return pt_new_dsm_comp;
}

/*--------------------------------------------------------------------------
 * Name: dsm_get_ref_to_comp
 *
 * Description - Get the reference to the DSM_COMP_T structure from the
 *    internal DSM_COMP_T handle. 
 *
 * Input  - 
 *    pv_tag:  Tag value to pass to client provided action function.
 *
 * Output - None
 *
 * Return
 *    NULL:   can not allocate and initialize the dsm_comp structure.
 *    ptr to DSM_COMP_T structure.
 --------------------------------------------------------------------------*/
DSM_COMP_T*  dsm_get_ref_to_comp
(
    HANDLE_T        h_this_dsm_comp
)
{
    DSM_COMP_T*     pt_new_dsm_comp = NULL;
    HANDLE_TYPE_T   e_type;
    
    if ( (h_this_dsm_comp != NULL_HANDLE)           &&
         (x_handle_valid(h_this_dsm_comp) == TRUE)
       )
    {
        if ( handle_get_type_obj(h_this_dsm_comp,
                                 &e_type,
                                 ((VOID**) &pt_new_dsm_comp))
             == HR_OK
            )
        {
            if ( (e_type == DSMT_DSM_COMP_T)       &&
                 (pt_new_dsm_comp != NULL )
               )
            {
                if ( pt_new_dsm_comp->h_this_dsm_comp !=
                     h_this_dsm_comp )
                {
                    pt_new_dsm_comp = NULL;
                }
            }
        }
    }
    return pt_new_dsm_comp;
}

/*--------------------------------------------------------------------------
 * Name: dsm_iterate_comp_data
 *
 * Description - Apply an user provide function to operate on the
 *               indivdual component element in caller provided
 *               input array. 
 *
 * Input  - 
 *    pv_data:    Array of DSM components.
 *    i4_nb_comp: Number of components
 *    pf_action:  client provided function to apply to each element of
 *                component.
 *    pv_tag:     client's private tag 
 *
 * Output - None
 *
 * Return - 
 *    DSMR_OK
 --------------------------------------------------------------------------*/
INT32 dsm_iterate_comp_data
(
    DSM_COMP_T*           pat_data[],
    INT32                 i4_nb_comp,
    dsm_comp_action_fct   pf_action,
    VOID*                 pv_tag
)
{
    UINT32   ui4_n;
    INT32    i4_rc;

    i4_rc = DSMR_OK;
    
    for ( ui4_n = 0 ; ui4_n < ((UINT32)i4_nb_comp) ; ui4_n++ )
    {
        i4_rc = dsm_iterate_comp_elem(pat_data[ui4_n],
                                      ui4_n,
                                      pf_action,
                                      pv_tag);
        if ( i4_rc != DSMR_OK )
        {
            break;
        }    
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_init_comp_data
 *
 * Description - Initialize the DSM component structure.
 *
 * Input  - 
 *    pat_data: Array of pointer to DSM component structure.
 *    i4_nb_comp:  number of components in pat_data.
 *
 * Output - None
 *
 * Return - Void
 *
 --------------------------------------------------------------------------*/
VOID  dsm_init_comp_data
(
    DSM_COMP_T* pat_data[],
    INT32  i4_nb_comp
)
{
    UINT32   ui4_n;

    for ( ui4_n = 0 ; ui4_n < ((UINT32)i4_nb_comp) ; ui4_n++ )
    {
        dsm_init_comp_elem(pat_data[ui4_n]);
    }
    return ;
}

/*--------------------------------------------------------------------------
 * Name: dsm_free_comp_data
 *
 * Description - API to remove all the DSM components specified.
 *
 * Input  - 
 *    pt_data:      array of DSM components.
 *    ui4_nb_comp:  number of components in the input array.
 *
 * Output - None
 *
 * Return - Void
 *
 --------------------------------------------------------------------------*/
VOID  dsm_free_comp_data
(
    DSM_COMP_T* pt_data[],
    INT32  i4_nb_comp
)
{
    UINT32   ui4_n;

    for ( ui4_n = 0 ; ui4_n < ((UINT32)i4_nb_comp) ; ui4_n++ )
    {
         dsm_free_comp_elem(pt_data[ui4_n]);
         pt_data[ui4_n]=NULL;
    }
    return ;
}

/*--------------------------------------------------------------------------
 * Name: dsm_rm_comp_sel_fct
 *
 * Description - Select the tuner component when multiple tuner of same type
 *               has been registered with resouce manager.
 *
 * Input  - 
 *    e_type:  Tuner driver type.
 *    ui2_comp_id:  component id for this driver.
 *    pv_comp_sel_data: client data specified in rm_comp_open() that is
 *                      passed to the select function .
 *    pv_comp_data:  Driver data for this component.
 *
 * Output - None
 *
 * Return - Void
 *
 --------------------------------------------------------------------------*/
SELECT_T dsm_rm_comp_sel_fct
(
    DRV_TYPE_T   e_type,
    UINT16       ui2_comp_id,
    VOID*        pv_comp_sel_data,
    const VOID*  pv_comp_data,
    SIZE_T       z_comp_data_len,
    UINT16       ui2_num_active,
    UINT16       ui2_num_wait,
    CTRL_TYPE_T  e_ctrl,
    BOOL         b_would_conflict
)
{
    SELECT_T                 t_select_rc;
    DSM_COMP_SEL_DATA_T*     pt_select_data;
    DSM_COMP_T*              pt_dsm_comp;
    DSM_COMP_T*              pt_new_dsm_comp;
    BOOL                     b_rc;

    t_select_rc = SEL_NEXT ;
    pt_select_data = NULL ;
    pt_dsm_comp = NULL;
    
    if ( pv_comp_sel_data != NULL )
    {
        pt_select_data = (DSM_COMP_SEL_DATA_T*) pv_comp_sel_data;
        pt_dsm_comp   = *(pt_select_data->ppt_comp_first_entry);
    }
    else
    {
        DBG_INFO(("{DSM} Error [dsm_rm_comp_sel_fct] component select data argument can not be NULL.\n"));
        /* Fatal error if this pointer is NULL. */
        ABORT(DBG_CAT_INV_OP,DBG_ABRT_INVALID_ARG);
    }

    /*
      Check the index to see if this one should be selected base on
      the index number.  
    */
    if ( pt_select_data->ui2_idx == pt_select_data->ui2_current_idx )
    {
        /*
          compare drv data with client specified data.
        */
        if ( pt_select_data->pf_sel_fct != NULL )
        {
            /*
              Get the component flag for this driver
            */
            UINT32      ui4_comp_flags;
            UINT8       ui1_num_inp_ports;
            UINT8       ui1_num_out_ports;
            
            if ( rm_get_comp_info_by_id(e_type,
                                        ui2_comp_id,
                                        &ui1_num_inp_ports,
                                        &ui1_num_out_ports,
                                        &ui4_comp_flags) == RMR_OK )
            {
                b_rc = pt_select_data->pf_sel_fct(pt_select_data->t_dev_type,
                                                  (VOID*) ui4_comp_flags,
                                                  pt_select_data->pv_select_data);
            }
            else
            {
                b_rc = FALSE;
            }
        }
        else
        {
            b_rc = TRUE;
        }                                     

        if ( b_rc == TRUE )
        {
            
            DBG_INFO(("{DSM} comp_select: drv_type %d ; component id: %d\n",
                      e_type, ui2_comp_id));

            /*
              We now know the driver type and component id, now look
              through the existing DSM component to see if this
              RM component has already been opened.
            */
            while ( pt_dsm_comp != NULL )
            {
                if ( pt_dsm_comp->h_rm_hdle != NULL_HANDLE   &&
                     x_handle_valid( pt_dsm_comp->h_rm_hdle ) &&
                     pt_dsm_comp->t_drv_type    == e_type        &&
                     pt_dsm_comp->ui2_comp_id   == ui2_comp_id
                    )
                {
                    /*
                      RM component has already been open.
                    */
                    pt_select_data->pt_comp_selected=pt_dsm_comp;
                    pt_select_data->ui1_comp_select_state = DSM_COMP_FOUND;

                    /*
                      Break out the selection loop without allocating a
                      new RM handle to this Driver component.
                    */
                    t_select_rc = SEL_BREAK;
                    break;
                }
                else
                {
                    /*
                      goto the next component and check for match.
                    */
                    pt_dsm_comp = pt_dsm_comp->pt_next;
                }
            }

            /*
              If a RM component matching the selection criteria is found,
              but assocated DSM component structure has not been found,
              then we will create a DSM component structure for
              this RM component.
            */
            if ( pt_select_data->ui1_comp_select_state != DSM_COMP_FOUND )
            {
                /*
                  Allocate the DSM component structure.
                */
                pt_new_dsm_comp = dsm_alloc_and_init_comp(NULL);
                
                if ( pt_new_dsm_comp != NULL )
                {
                    /*
                      Fill in the component info.
                    */
                    pt_new_dsm_comp->t_drv_type = e_type;
                    pt_new_dsm_comp->ui2_comp_id = ui2_comp_id;
                    /*
                      Select this RM component for the DSM component
                    */
                    pt_select_data->pt_comp_selected=pt_new_dsm_comp;
                    pt_select_data->ui1_comp_select_state=DSM_COMP_ALLOCATED;

                    /*
                      Attach the newly allocated DSM component structure to
                      the end of component list for this component type.
                    */
                    if ( *(pt_select_data->ppt_comp_first_entry) == NULL )
                    {
                        /* List is empty, so this allocated component
                           is stored at head of list.
                        */
                    
                        *(pt_select_data->ppt_comp_first_entry) = pt_new_dsm_comp;
                    }
                    else
                    {
                        /*
                          Find the end of list to attach this new component
                        */
                        pt_dsm_comp=*(pt_select_data->ppt_comp_first_entry);

                        for ( ; ; ) 
                        {
                            if ( pt_dsm_comp->pt_next == NULL )
                            {
                                /* Found the end of list. */
                                pt_dsm_comp->pt_next = pt_new_dsm_comp ;
                                break;
                            }
                            else
                            {
                                pt_dsm_comp=pt_dsm_comp->pt_next;
                            } 
                        }
                    }
                    t_select_rc = SEL_COMP_BREAK;
                }
                else
                {
                    /*
                      memory allocation fail.
                    */
                    pt_select_data->ui1_comp_select_state=DSM_COMP_OUT_OF_MEM;
                    t_select_rc = SEL_BREAK;
                }
            }
        }
        else
        {
            /*
              Driver data does not match the client's selection, try
              next component.
            */
            t_select_rc = SEL_NEXT;
        }
    }
    else
    {
        /* Not the right driver component (e.g., index does not
           match), try the next index. */
        pt_select_data->ui2_current_idx++;
        t_select_rc = SEL_NEXT ;
    }

    return t_select_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_open_comp
 *
 * Description: This API open a RM handle to the specified DSM device.
 *              Multiple client can shared the same RM handle. 
 *
 * Input  -
 *    t_dev_type:  Specifies the name of the device
 *
 *    ui2_idx:  specifies the index of the registered device (when more
 *              than one device of the same type is registered.  0 means
 *              the first device of that registered, etc.
 *
 *    ps_name:  A RM component group name.
 *
 * Output - 
 *    ppt_dsm_obj:  Pointer to a DSM component structure containing
 *                  information to access the RM handle.
 *
 * Return -
 *   DSMR_OK:   synchronization source is set.
 *   DSMR_FAIL: Can not set the source of the synchronization for
 *                   the current time. 
 *          
 --------------------------------------------------------------------------*/
INT32 dsm_open_comp
(
    DEVICE_TYPE_T        t_dev_type,
    UINT16			     ui2_idx,
    const CHAR*          ps_name,
    DSM_COMP_T**         ppt_dsm_obj
)
{
    INT32                i4_rc;
    DEVICE_TYPE_T        t_group;

    i4_rc = DSMR_DEV_NOT_FOUND;
    *ppt_dsm_obj = NULL ;
    
    t_group = DEV_GET_GROUP_ID(t_dev_type);

    switch (t_group)
    {
        case DEV_TUNER:
        {
            i4_rc = dsm_open_tuner_comp(t_dev_type,
                                        ui2_idx,
                                        ps_name,
                                        ppt_dsm_obj);
        }
        break;

        case DEV_1394:
        {
            
        }
        break;

        case DEV_AVC:
        {
            i4_rc = dsm_open_avc_comp(t_dev_type,
                                      ui2_idx,
                                      ps_name,
                                      ppt_dsm_obj);
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
 * Name: dsm_open_rm_comp
 *
 * Description: This API open RM handle and initialize component element.
 *              for a specified device.
 *
 * Input  -
 *
 *    h_rm_pipe:   Specify the pipe which this rm component should be
 *                 connected to. If no connection to pipe is needed, then
 *                 this parameter can be NULL_HANDLE.
 *
 *    t_dev_type:  Specifies the name of the DSM device to open with RM
 *
 *    ui2_idx:  specifies the index of the registered device (when more
 *              than one device of the same type is registered.  0 means
 *              the first device of that registered, then 
 *
 *    ps_name:  specifies the optional device group name used by the
 *              driver to register with RM.
 *
 *    at_drv_type: An array of device driver type ID's to select from.
 *
 *    at_device_name: An array of driver name (char string)
 *                    corresponds to the 'at_drv_type' array
 *                    (for debug print purpose).
 *  
 *    at_device_type: An array of device type ID corrsponds to the
 *                    array of driver type ID.
 *
 *    pat_dsm_comp:  An array containing pointer to the DSM component
 *                   corrsponds to each registered driver.  Note:
 *                   Initially pointer in this array is set to NULL.
 *                   Once the RM handle to the specified driver type
 *                   has been opened, and the appropriate initialization
 *                   has been performed.  A pointer to the DSM component
 *                   containing the RM handle to the specified device and
 *                   associated data will be store in this 'pat_dsm_comp'
 *                   array.
 *
 *    i4_nb_comp:    The number of elements in 'at_drv_type',
 *                   'ac_device_name', 'at_device_type', and
 *                   'pat_dsm_comp' arrays.  All array must have the
 *                   same number of elements.
 *
 *    dsm_rm_nfy_fct: notification function for the RM handle.
 *
 * Output -
 *
 *    pat_dsm_comp: Contains the pointers to the DSM component structure
 *                  for all the driver object that has been initialized.
 *
 *    ppt_dsm_obj:  Pointer to pointer variable storing the address of
 *                  DSM component structure associate with the spcified
 *                  driver object.  The 'DSM component structure' is also
 *                  store in the 'pat_dsm_comp' array.
 *
 * Return -
 *   DSMR_OK:   a RM handle to the specified driver object is created.
 *   DSMR_FAIL: Can not create a RM handle to the specified driver object.
 *          
 --------------------------------------------------------------------------*/
INT32 dsm_open_rm_comp
(
    HANDLE_T          h_rm_pipe,
    DEVICE_TYPE_T     t_dev_type,
    UINT16            ui2_idx,
    const CHAR*       ps_name,
    VOID*             pv_sel_data,
    dsm_sel_drv_data  pf_sel_fct,
    DRV_TYPE_T*       at_drv_type,
    CHAR**            ac_device_name,
    DEVICE_TYPE_T*    at_device_type,
    DSM_COMP_T*       pat_dsm_comp[],
    INT32             i4_nb_comp,
    rm_nfy_fct        dsm_rm_nfy_fct,
    DSM_COMP_T**      ppt_dsm_obj
)
{
    INT32              i4_rc;
    INT32              i4_n;
    RM_COND_T          e_rm_cond;
    DSM_COMP_T*        pt_dsm_comp;
    DRV_TYPE_T         t_drv_type;
    HANDLE_T           h_rm_hdle;
    DSM_COMP_SEL_DATA_T     t_select_data;
    
    t_drv_type = DRVT_UNKNOWN ;
    i4_rc = DSMR_DEV_TYPE_NOT_SUPPORTED;
    
    /* Find corresponding driver device for the specified
       DSM device type. */
    for ( i4_n = 0; i4_n < i4_nb_comp ; i4_n++ )
    {
        if ( t_dev_type == at_device_type[i4_n] )
        {
            t_drv_type = at_drv_type[i4_n];
            /*
              Initialize the select data parameter.
            */
            t_select_data.t_dev_type = t_dev_type;
            t_select_data.ui2_current_idx = 0;
            t_select_data.ui2_idx = ui2_idx ;
            t_select_data.pv_select_data = pv_sel_data;
            t_select_data.pf_sel_fct     = pf_sel_fct;
            t_select_data.ppt_comp_first_entry = &(pat_dsm_comp[i4_n]);
            t_select_data.pt_comp_selected = NULL;
            t_select_data.ui1_comp_select_state = DSM_COMP_NULL ;
            i4_rc=DSMR_OK;
            break;
        }
    }

    if ( i4_rc == DSMR_OK )
    {
        /*
          open the avc component.
        */
        if ( rm_open_comp(h_rm_pipe,
                          OBJ_COMP_REL_IGNORE,
                          t_drv_type,
                          ANY_COMP_ID,
                          ps_name,
                          CTRL_TYPE_SHARED,
                          &(t_select_data),
                          dsm_rm_comp_sel_fct, /* NULL, */
                          (VOID*) NULL,        /* index to the internal
                                                  driver array for
                                                  this component to open.
                                               */
                          dsm_rm_nfy_fct,
                          &(h_rm_hdle),
                          &e_rm_cond)
             == RMR_OK
            )
        {
            pt_dsm_comp=t_select_data.pt_comp_selected;
            
            if ( t_select_data.ui1_comp_select_state == DSM_COMP_ALLOCATED &&
                 pt_dsm_comp != NULL                   &&
                 pt_dsm_comp->t_drv_type == t_drv_type 
                )
            {
                pt_dsm_comp->ui1_comp_stat_flag = DSM_COMP_ALLOCATED ;
                
                pt_dsm_comp->h_rm_hdle = h_rm_hdle;
                
                /*
                   Save the component device type info
                   to the DSM comoponent structure. 
                */
                pt_dsm_comp->t_dev_type_name= t_dev_type;

                pt_dsm_comp->ui2_idx = (UINT16)i4_n;

                /*
                  Now we can store the DSM component structure
                  to the RM handle allocated.
                */
                if ( x_handle_set_tag(h_rm_hdle,
                                      (VOID*) pt_dsm_comp->h_this_dsm_comp)
                     != HR_OK )
                {
                    ABORT(DBG_CAT_HANDLE,DBG_ABRT_OPS_FAIL);
                }
                
                DBG_API(("{DSM} RM component allocated %s, "
                         "handle= %d\n",
                         ac_device_name[i4_n],
                         pt_dsm_comp->h_rm_hdle));

                /*
                   return the DSM component structrue for this
                   device back to the client.
                */
                *ppt_dsm_obj = pt_dsm_comp;
                
                i4_rc = DSMR_OK;
            }
            else
            {
                DBG_API(("{DSM} Internal error: can not open component: %s\n",
                         ac_device_name[i4_n]));
                
                ABORT(DBG_CAT_INV_OP,DBG_ABRT_OPS_FAIL);
            }
        }
        else
        {
            /*
              RM handle is not opened, now check to see if the RM component
              has already been opened and other error condition.
            */
            switch (t_select_data.ui1_comp_select_state)
            {
                case DSM_COMP_FOUND:
                {
                    pt_dsm_comp = t_select_data.pt_comp_selected;
                    if ( pt_dsm_comp->t_drv_type == t_drv_type        &&
                         pt_dsm_comp->h_rm_hdle != NULL_HANDLE    &&
                         pt_dsm_comp->t_dev_type_name == t_dev_type
                        )
                    {
                        DBG_API(("{DSM} RM component founded %s, "
                                 "rm handle= %d\n",
                                 ac_device_name[i4_n],
                                 pt_dsm_comp->h_rm_hdle));

                        pt_dsm_comp->ui1_comp_stat_flag = DSM_COMP_FOUND ;
                        
                        /*
                          return the DSM component structrue for this
                          device back to the client.
                        */
                        *ppt_dsm_obj = pt_dsm_comp;
                    
                        i4_rc = DSMR_OK;    
                    }
                    else
                    {
                        i4_rc = DSMR_DEV_NOT_FOUND;
                    }
                }
                break;
                
                /*
                  Check other error condition.
                */
                case DSM_COMP_OUT_OF_MEM:
                {
                    i4_rc = DSMR_OUT_OF_MEM;
                }
                break;

                default:
                {
                    i4_rc = DSMR_FAIL;
                }
                break;
            }
        }
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_link_client_hdle_with_comp
 *
 * Description: This API links the client handle to the component handle.
 *              The component handle is the handle return by RM that references
 *              a specific device object. The linked client handles is used
 *              for the notification purpose.
 *
 * Input  -
 *    pt_dsm_comp:  Pointer the component object.
 *
 *    h_client: Client handle to this component. 
 *
 * Return -
 *   DSMR_OK:   Client handle is linked to the RM component handle.
 *   DSMR_FAIL: Internal error: linking failed. 
 *          
 --------------------------------------------------------------------------*/
INT32 dsm_link_client_hdle_with_comp
(
    DSM_COMP_T*         pt_dsm_comp,
    HANDLE_T            h_client
)
{
    INT32               i4_rc ;

    i4_rc = DSMR_FAIL;
    
    if ( pt_dsm_comp                  != NULL            &&
         pt_dsm_comp->h_rm_hdle       != NULL_HANDLE     &&
         h_client                     != NULL_HANDLE
        )
    {
        DSM_GRAB_SEMA();
        
        if ( handle_link (&(pt_dsm_comp->t_link), h_client)
             == HR_OK
            )
        {
            i4_rc = DSMR_OK;
        }
        else
        {
            /* Something is wrong, abort. */
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_OPS_FAIL);
        }
        
        DSM_UNGRAB_SEMA();
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_delink_client_hdle_with_comp
 *
 * Description: This API de-links the client handle to the component handle.
 *              The component handle is the handle return by RM that references
 *              a specific device object. The linked client handles is used
 *              for the notification purpose.
 *
 * Input  -
 *    pt_dsm_comp:  Pointer the component object.
 *
 *    h_client: Client handle to this component. 
 *
 * Return -
 *   DSMR_OK:   Client handle is linked to the RM component handle.
 *   DSMR_FAIL: Internal error: linking failed. 
 *          
 --------------------------------------------------------------------------*/
INT32 dsm_delink_client_hdle_with_comp
(
    DSM_COMP_T*         pt_dsm_comp,
    HANDLE_T            h_client
)
{
    INT32               i4_rc ;

    i4_rc = DSMR_FAIL;
    
    if ( pt_dsm_comp                  != NULL            &&
         pt_dsm_comp->h_rm_hdle       != NULL_HANDLE     &&
         h_client                     != NULL_HANDLE
        )
    {
        DSM_GRAB_SEMA();
        
        if ( handle_delink (&(pt_dsm_comp->t_link), h_client)
             == HR_OK
            )
        {
            UINT16    ui2_num_links;
            
            ui2_num_links = handle_num_links (&(pt_dsm_comp->t_link));
            
            if ( ui2_num_links == 0 )
            {
                DBG_API(("{DSM} ref count = 0, free shared RM handle resource [%d]\n",
                         pt_dsm_comp->h_rm_hdle));
                /*
                  Free the resource for this component.
                */
                dsm_close_comp(pt_dsm_comp);
            }
            
            i4_rc = DSMR_OK;
        }
        else
        {
            /* Something is wrong, abort. */
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_OPS_FAIL);
        }
        
        DSM_UNGRAB_SEMA();
    }
    return i4_rc;
}


/*--------------------------------------------------------------------------
 * Name: dsm_close_comp
 *
 * Description: This API free the DSM component data structure. The component
 *              data structure is part of linked list belong to the device
 *              group.  Close the client handle to this DSM component.
 *              
 * Input  -
 *    t_dev_type:  Specifies the name of the device
 *
 *    ui2_idx:  specifies the index of the registered device (when more
 *              than one device of the same type is registered.  0 means
 *              the first device of that registered, etc.
 *
 *    ps_name:  A RM component group name.
 *
 *    ppt_dsm_obj:  Pointer to a DSM component structure containing
 *                  information to access the RM handle.
 *
 * Return -
 *   DSMR_OK:   synchronization source is set.
 *   DSMR_FAIL: Can not set the source of the synchronization for
 *                   the current time. 
 *          
 --------------------------------------------------------------------------*/
INT32 dsm_close_comp
(
    DSM_COMP_T*          pt_dsm_obj
)
{
    INT32                i4_rc;
    DEVICE_TYPE_T        t_group;

    i4_rc = DSMR_FAIL;
    
    if ( pt_dsm_obj != NULL )
    {
        t_group = DEV_GET_GROUP_ID(pt_dsm_obj->t_dev_type_name);

        switch (t_group)
        {
            case DEV_TUNER:
            {
            
            }
            break;

            case DEV_1394:
            {
            
            }
            break;

            case DEV_AVC:
            {
            
                i4_rc = dsm_close_avc_comp(pt_dsm_obj);
            }
            break;
        
            default:
            {
                i4_rc = DSMR_DEV_NOT_FOUND;
            }
            break;
        }
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_comp_get_ops
 *
 * Description: This API dispatch the get operation command to
 *              the specific driver group.
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
INT32 dsm_comp_get_ops
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
            i4_rc = dsm_tuner_get_ops(pt_dsm_obj,
                                      t_get_type,
                                      pv_get_info,
                                      pz_size,
                                      pv_data);
        }
        break;

        case DEV_1394:
        {
            i4_rc = DSMR_DEV_TYPE_NOT_SUPPORTED ;
        }
        break;

        case DEV_AVC:
        {
            i4_rc = dsm_avc_get_ops(pt_dsm_obj,
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
 * Name: dsm_comp_set_ops
 *
 * Description: This API dispatch the set operation command to
 *              the specific driver group.
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
INT32 dsm_comp_set_ops
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
            z_set_info_len = z_size;
            
            i4_rc = dsm_tuner_set_ops(pt_dsm_obj,
                                      t_set_type,
                                      pv_set_info,
                                      z_set_info_len);
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
            i4_rc = dsm_avc_set_ops(pt_dsm_obj,
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

/*--------------------------------------------------------------------------
 * Name:   dsm_comp_get_grp_ctrl_blk
 *
 * Description:
 *   If user has specified a TV decoder to used, then get the
 *   group control block.
 *
 * Input -
 *    pt_dsm_comp:  Ref to a AVC COMBI structure.
 *
 * Output -
 *    
 *
 * Return -
 *    DSMR_OK:    TV decoder is connected.
 *   
 --------------------------------------------------------------------------*/
INT32 dsm_comp_get_grp_ctrl_blk
(
    DSM_COMP_T*         pt_dsm_comp,
    DSM_GROUP_BLK_T**   ppt_grp_ctrl
)
{
    DSM_CTRL_BLK_T*     pt_dsm_ctrl;
    DSM_GROUP_BLK_T*    pt_group_ctrl;
    HANDLE_TYPE_T       e_type;
    INT32               i4_rc = DSMR_FAIL;

    /*
      Get the control structure of the device handle.
    */
    if ( (handle_get_type_obj(pt_dsm_comp->h_dsm_dev, &e_type,
                              ((VOID**) &pt_dsm_ctrl))
                       == HR_OK)                       &&
         (e_type      == DSMT_DEVICE_T)           &&
         (pt_dsm_ctrl  != NULL)
        )
    {
        /*
          Get the control structure of this device's parent
          handle.
        */
        if ( (handle_get_type_obj(pt_dsm_ctrl->h_grp_parent, &e_type,
                                  ((VOID**) &pt_group_ctrl))
              == HR_OK)                                     &&
             (e_type == DSMT_GROUP_T)                 &&
             (pt_group_ctrl  != NULL)
            )
        {
            *ppt_grp_ctrl = pt_group_ctrl;
            i4_rc = DSMR_OK;
        }
        else
        {
            DBG_ERROR(("{dsm_avc_combi} Error: no dev/grp ctrl blk structure.\n"));
        }   
    }
    else
    {
        DBG_ERROR(("{dsm_avc_combi} Error: no dev/grp ctrl blk structure.\n"));
    }
    return i4_rc;
}
