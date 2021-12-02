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
 * $RCSfile: dsm_avc.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains DSM function interfacing with the
 *         AVC (audio-video connect) drivers.
 *
 *---------------------------------------------------------------------------*/

#include "dsm/_dsm.h"
#include "dsm/dsm_avc_combi.h"
#include "dsm/dsm_avc_scart.h"

/*--------------------------------------------------------

static internal array variables holding the avc information.
  
----------------------------------------------------------*/
#define   MAX_NB_AVC_DRV_TYPE     10

static DRV_TYPE_T   at_avc_drv[MAX_NB_AVC_DRV_TYPE] =
{  
    DRVT_AVC_COMP_VIDEO,  
    DRVT_AVC_S_VIDEO, 
    DRVT_AVC_Y_PB_PR,  
    DRVT_AVC_VGA,    
    DRVT_AVC_SCART,
    DRVT_AVC_DVI,        
    DRVT_AVC_HDMI,
    DRVT_AVC_AUDIO_INP,
    DRVT_AVC_SPDIF,  
    DRVT_AVC_COMBI
};

static CHAR*       ac_avc_name[MAX_NB_AVC_DRV_TYPE] =
{    
    "DRVT_AVC_COMP_VIDEO",  
    "DRVT_AVC_S_VIDEO", 
    "DRVT_AVC_Y_PB_PR",  
    "DRVT_AVC_VGA",    
    "DRVT_AVC_SCART",
    "DRVT_AVC_DVI",        
    "DRVT_AVC_HDMI",
    "DRVT_AVC_AUDIO_INP",
    "DRVT_AVC_SPDIF",  
    "DRVT_AVC_COMBI"   
};

/*
  DSM device name corresponding to the avc driver name.
*/
static DEVICE_TYPE_T    at_dsm_avc_device[MAX_NB_AVC_DRV_TYPE] =
{ 
    DEV_AVC_COMP_VIDEO,
    DEV_AVC_S_VIDEO,
    DEV_AVC_Y_PB_PR,
    DEV_AVC_VGA,
    DEV_AVC_SCART,
    DEV_AVC_DVI,
    DEV_AVC_HDMI,
    DEV_AVC_AUDIO_INP,
    DEV_AVC_SPDIF,   
    DEV_AVC_COMBI
};

/*
  The structure for storing dsm component is a similar to a hash table.
  It has an static array of n element.  For each element, it store an
  DSM avc driver component. If more then one avc driver component of
  the same type existed, then then the next DSM component of that
  driver is store in the overflow bucket. The overflow bucket is
  dynamically allocated, and 'pt_next' point the the overflow bucket.


     [AVC_S_VIDEO] -->   NULL
     [AVC_DVI] --> [AVC_DVI] --> NULL
     [AVC_HDMI] -->   NULL

  Here, the driver type AVC_S_VIDEO and AVC_HDMI has only a single
  instance of the driver compoent, but the AVC_DVI has two instances.
*/
static  DSM_COMP_T*  pat_dsm_avcs[MAX_NB_AVC_DRV_TYPE] ;

/*--------------------------------------------------------
 * Name: dsm_avc_cmp_drv_data
 *
 * Description - Compare Driver component data with component select
 *    data to determine if this driver component should be selected.
 *
 *    For AVC SCART component, we need to select the component that
 *    has AVC_FLAG_SCART_MONITOR flag set.
 *
 * Input  -
 *    t_dev:         device type.
 *    pv_drv_data:   pointer to the driver data.
 *    pv_client_data specifies the client data.
 *
 * Output - None
 *
 * Return -
 *    TRUE:      Driver component data matches selection criteria.
 *    FALSE:     Driver component data do not match selection
 *               criteria.
----------------------------------------------------------*/
static BOOL dsm_avc_cmp_drv_data
(
    DEVICE_TYPE_T       t_dev,
    const VOID*         pv_drv_data,
    VOID*               pv_client_data
)
{
    UINT32       ui4_drv_data = 0;
    UINT32       ui4_flag     = 0;
    
    BOOL         b_res;

    b_res = TRUE;
    ui4_drv_data = (UINT32) pv_drv_data;
    ui4_flag     = (UINT32) pv_client_data;
    
    if ( t_dev == DEV_AVC_SCART )
    {
        if ( ui4_drv_data & ui4_flag )
        {
            DBG_INFO(("{DSM} AVC SCART driver selected, drv data: %d\n",(UINT32)pv_drv_data));
            b_res = TRUE;
        }
        else
        {
            b_res = FALSE;
        }
    }
    return b_res;
}

/*--------------------------------------------------------------------------
 * Name: dsm_print_obj
 *
 * Description - Print out information for the AVC component.
 *
 * Input  -
 *   pt_obj:      pointer to an componet object
 *   ui4_idx:     index into the driver resource array.
 *   ui4_sub_idx  index into the overflow bucket location.  
 *   pv_tag:      private tag provided to the iteration function.
 *
 * Output - None
 *
 * Return - Void
 *  
------------------------------------------------------------------------- */
static INT32 dsm_print_obj
(
    DSM_COMP_T* pt_obj,
    UINT32      ui4_idx,
    UINT32      ui4_sub_idx,
    VOID*       pv_tag
)
{

    if ( pt_obj->h_rm_hdle != NULL_HANDLE )
    {
        x_dbg_stmt("{DSM} dsm comp handle: [%d] "
                   "AVC name: [%s] "
                   "AVC driver type: [%d] "
                   "AVC drv component id: [%d] "
                   "AVC device type id:   [%d]\n",
                   pt_obj->h_rm_hdle,
                   ac_avc_name[ui4_idx],
                   pt_obj->t_drv_type,
                   pt_obj->ui2_comp_id,
                   pt_obj->t_dev_type_name);
    }

    /*
      always return OK so the next element will be visited.
    */
    return  DSMR_OK ;
}

/*--------------------------------------------------------------------------
 * Name: dsm_avc_delink_comp_obj
 *
 * Description - Delink the AVC component from the internal array.
 *
 * Input  -
 *   e_drv_type:    Driver device type.
 *
 * Output -
 *   pe_dev_type:   Pointer to a device type.
 *
 * Return -
 *   DSMR_OK
 *   DSMR_DEV_TYPE_NOT_SUPPORTED
------------------------------------------------------------------------- */
INT32 dsm_avc_delink_comp_obj
(
    DSM_COMP_T*     pt_obj_to_delink
)
{
    DRV_TYPE_T      e_drv_type;
    DEVICE_TYPE_T   e_dev_type;
    DSM_COMP_T*     pt_comp;
    DSM_COMP_T*     pt_prev;
    INT32           i4_rc;
    INT32           i4_i;
    UINT16          ui2_comp_id;
    
    i4_rc = DSMR_DEV_TYPE_NOT_SUPPORTED;

    e_drv_type = pt_obj_to_delink->t_drv_type;
    e_dev_type = pt_obj_to_delink->t_dev_type_name;
    ui2_comp_id = pt_obj_to_delink->ui2_comp_id;

    pt_comp=NULL;
    pt_prev=NULL;
    for ( i4_i = 0 ; i4_i < MAX_NB_AVC_DRV_TYPE ; i4_i++ )
    {
        if ( at_avc_drv[i4_i] == e_drv_type )
        {
            pt_comp=pat_dsm_avcs[i4_i];
            break;
        }
    }

    while( pt_comp != NULL)
    {
        /* iterate through the list find the object to delink. */
        if ( pt_comp->t_drv_type      ==  e_drv_type     &&
             pt_comp->t_dev_type_name ==  e_dev_type     &&
             pt_comp->ui2_comp_id     ==  ui2_comp_id
            )
        {
            /* object found: 
               memory check, the object found must be the same as
               the object to delink.
            */
            if ( pt_comp == pt_obj_to_delink )
            {   
                /* found the object at the head of list. */
                if ( pt_prev == NULL )
                {
                    /* remove the object from the head. */
                    pat_dsm_avcs[i4_i]=pt_comp->pt_next;
                }
                else
                {
                    /* delink the object. */
                    pt_prev->pt_next=pt_comp->pt_next;
                }
                DBG_INFO(("{DSM} AVC component object delinked [%d]\n",
                          pt_comp->h_rm_hdle));
                i4_rc=DSMR_OK;
            }
            else
            {
                x_dbg_stmt("{DSM} ERROR internal memory corruption\n");
                i4_rc = DSMR_FAIL;
            }
            break;
        }
        else
        {
            /*
              goto next object.
            */
            pt_prev=pt_comp;
            pt_comp=pt_comp->pt_next;
        }
    }
    return  i4_rc ;
}

/*--------------------------------------------------------------------------
 * Name: dsm_map_drv_type_to_dev_type
 *
 * Description - Convert DRV_TYPE_T returned by the RM to the DEVICE_TYPE_T.
 *
 * Input  -
 *   e_drv_type:    Driver device type.
 *
 * Output -
 *   pe_dev_type:   Pointer to a device type.
 *
 * Return -
 *   DSMR_OK
 *   DSMR_DEV_TYPE_NOT_SUPPORTED
------------------------------------------------------------------------- */
INT32 dsm_avc_map_drv_type_to_dev_type
(
    DRV_TYPE_T      e_drv_type,
    DEVICE_TYPE_T*  pe_dev_type
)
{
    INT32    i4_rc;
    INT32    i4_i;

    i4_rc = DSMR_DEV_TYPE_NOT_SUPPORTED;
    *pe_dev_type = DEV_UNKNOWN;
    
    for ( i4_i = 0 ; i4_i < MAX_NB_AVC_DRV_TYPE ; i4_i++ )
    {
        if ( at_avc_drv[i4_i] == e_drv_type )
        {
            *pe_dev_type = at_dsm_avc_device[i4_i];
            i4_rc = DSMR_OK;
            break;
        }
    }
    return  i4_rc ;
}


/*--------------------------------------------------------------------------
 * Name: dsm_map_dev_type_to_drv_type
 *
 * Description - Convert DEVICE_TYPE_T to the DRV_TYPE_T used by RM.
 *
 * Input  -
 *   e_dev_type:    DSM device type.
 *
 * Output -
 *   pe_drv_type:   Pointer to a driver type.
 *
 * Return -
 *   DSMR_OK
 *   DSMR_DEV_TYPE_NOT_SUPPORTED
------------------------------------------------------------------------- */
INT32 dsm_avc_map_dev_type_to_drv_type
(
    DEVICE_TYPE_T   e_dev_type,
    DRV_TYPE_T*     pe_drv_type
)
{
    INT32    i4_rc;
    INT32    i4_i;

    i4_rc = DSMR_DEV_TYPE_NOT_SUPPORTED;
    *pe_drv_type = DRVT_UNKNOWN  ;
    
    for ( i4_i = 0 ; i4_i < MAX_NB_AVC_DRV_TYPE ; i4_i++ )
    {
        if ( at_dsm_avc_device[i4_i] == e_dev_type )
        {
            *pe_drv_type = at_avc_drv[i4_i];
            i4_rc = DSMR_OK;
            break;
        }
    }
    return  i4_rc ;
}

/*--------------------------------------------------------------------------
 * Name: dsm_avc_close_rm_hdle
 *
 * Description - Close the AVC component handle.
 *
 * Input  -
 *   pt_obj:      pointer to an componet object
 *   ui4_idx:     index into the driver resource array.
 *   ui4_sub_idx  index into the overflow bucket location.  
 *   pv_tag:      private tag provided to the iteration function.
 *
 * Output - None
 *
 * Return - Void
 *  
------------------------------------------------------------------------- */
static INT32 dsm_avc_close_rm_hdle
(
    DSM_COMP_T* pt_obj,
    UINT32      ui4_idx,
    UINT32      ui4_sub_idx,
    VOID*       pv_tag
)
{
    if ( pt_obj->h_rm_hdle != NULL_HANDLE )
    {
        if ( dsm_close_avc_comp(pt_obj)
             == DSMR_OK )
        {
            pt_obj->h_rm_hdle=NULL_HANDLE;
        }
    }

    /*
      always return OK so the next element will be visited.
    */
    return  DSMR_OK ;
}

/*--------------------------------------------------------------------------
 * Name: dsm_avc_info
 *
 * Description - API to print out information about all the
 *               AVC components found  by the DSM.
 *
 * Input  -
 *   None.
 *
 * Output -
 *   None
 *
 * Return - Void
 *  
------------------------------------------------------------------------- */
VOID dsm_avc_info(VOID)
{
    /*
       Need to protect access/operation on the static
       AVC component data arrays.
    */
    DSM_GRAB_SEMA();
        
    dsm_iterate_comp_data(pat_dsm_avcs,
                          MAX_NB_AVC_DRV_TYPE,
                          dsm_print_obj, NULL);
    
    DSM_UNGRAB_SEMA();
    
    return;
}

/*--------------------------------------------------------------------------
 * Name: dsm_avc_close
 *
 * Description - API to close the AVC component handle allocated by the DSM,
 *               and free other resource.
 *
 * Input  -
 *   None.
 *
 * Output -
 *   None
 *
 * Return - Void
 *  
------------------------------------------------------------------------- */
VOID dsm_avc_close(VOID)
{

    /*
       Need to protect access/operation on the static
       AVC component data arrays.
    */
    DSM_GRAB_SEMA();
    
    /*
      iterate over each AVC component element and free the RM handle
      associated with the AVC component.
    */
    dsm_iterate_comp_data(pat_dsm_avcs,
                          MAX_NB_AVC_DRV_TYPE,
                          dsm_avc_close_rm_hdle, NULL);

    /*
      Free the AVC component element.
    */
    dsm_free_comp_data(pat_dsm_avcs,MAX_NB_AVC_DRV_TYPE);

    DSM_UNGRAB_SEMA();
    
    return;
}

/*--------------------------------------------------------------------------
 * Name: dsm_rm_avc_nfy_fct
 *
 * Description - Handle notify function for the AVC driver. This
 *               function is called by the Resource Manager when the AVC 
 *               component's state or condition changed.
 *
 * Input  - h_obj:  handle to the AVC component
 *          ui1_port:  Not used by the AVC component. 
 *          e_nfy_cond:  Condition of AVC component 
 *          pv_tag:    Tag pass into Resource Manager when opening
 *                     this AVC component.
 *          ui4_data:  Private data, not used by the AVC component.
 *
 * Output - None
 *
 * Return - Void
 *
 --------------------------------------------------------------------------*/
static  VOID dsm_rm_avc_nfy_fct 
(
    HANDLE_T   h_obj,
    UINT8      ui1_port,
    RM_COND_T  e_nfy_cond,
    VOID*      pv_tag,
    UINT32     ui4_data
)
{
    INT32              i4_n;
    DSM_MSG_T          t_msg;
    DSM_COMP_T*        pt_dsm_comp = NULL;
    HANDLE_T           h_dsm_comp  = NULL_HANDLE;

    i4_n = 0 ;
    
    if ( pv_tag != NULL )
    {
        /*
          The pv_tag to the RM handle for DSM device contains
          the pointer to the DSM component structure. If
          this structure is not set, then we will
          not process the notificaiton to the client.
        */
        h_dsm_comp = (HANDLE_T) pv_tag;

        pt_dsm_comp = dsm_get_ref_to_comp(h_dsm_comp);
        if ( pt_dsm_comp != NULL )
        {
            /*
               index into the internal avc array.
            */
            i4_n = (INT32)(pt_dsm_comp->ui2_idx);
        }
        else
        {
            return;
        }
    }
    else
    {
        return ;
    }

    switch (e_nfy_cond)
    {
        case RM_COND_OPENED:
        {
            
        }
        break;
        
        case RM_COND_OPENING:
        {
            DBG_INFO(("{DSM}: component %s open\n",ac_avc_name[i4_n]));
            /*
              Send notification to the DSM thread.
            */
            
        }
        break;

        case RM_COND_CONNECTED:
        {
            
        }
        break;

        case RM_COND_DISCONNECTED:
        {
            
        }
        break;
        
        case RM_COND_CLOSING:
        {
            /* Set the Device handle to NULL */
            DBG_INFO(("{DSM}: component %s closing\n",ac_avc_name[i4_n]));
        }
        break;

        case RM_COND_CLOSED:
        {
            /* Set the Device handle to NULL */
            DBG_INFO(("{DSM}: component %s closed\n",ac_avc_name[i4_n]));

            t_msg.h_dsm_comp  = h_dsm_comp;
            t_msg.e_msg_code   = DSM_MSG_RM_HANDLE_CLOSED;
            t_msg.e_cond       = DSM_COND_DEV_HANDLE_CLOSED;

            dsm_send_msg(&t_msg);
        }
        break;

        default:
        {
            i4_n++;   /* use this variable to remove warning. */
        }
        break;
    }
    return ;
}

/*--------------------------------------------------------------------------
 * Name: dsm_avc_init
 *
 * Description: This API initialize the avc interface. It will open
 *              an handle to all the avc components that has been
 *              registered with the Resource Manager.
 *
 * Input  -
 *    None
 *
 * Return -
 *   DSMR_OK:   avc handles are  registered with DSM.
 *   DSMR_FAIL: avc component initialization failed.
 *          
 --------------------------------------------------------------------------*/

INT32 dsm_avc_init(VOID)
{
    INT32              i4_rc;
    INT32              i4_n;
    INT32              i4_nb_avcs;

    i4_rc = DSMR_OK;
    
    i4_nb_avcs = (INT32)(sizeof(at_avc_drv)/sizeof(DRV_TYPE_T));

    if ( MAX_NB_AVC_DRV_TYPE != i4_nb_avcs )
    {
        i4_rc = DSMR_FAIL;
    }
    else
    {
        for ( i4_n = 0 ; i4_n < i4_nb_avcs ; i4_n++ )
        {
            pat_dsm_avcs[i4_n] = NULL ;
        }
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_close_avc_comp
 *
 * Description - API to free the DSM avc component data and notify clients
 *               to this DSM component.
 *
 * Input  -
 *   pt_obj:      pointer to a componet object
 *   ui4_idx:     index into the driver resource array.
 *   ui4_sub_idx  index into the overflow bucket location.  
 *   pv_tag:      private tag provided to the iteration function. In this
 *                case: the pv_tag is a pointer to DSM_SELECT_DATA_T
 *                structure, which contains the dsm_device_name and index.
 *
 * Output -
 *   pv_tag: 
 *
 * Return - 
 *   DSMR_OK
 *
------------------------------------------------------------------------- */
INT32 dsm_close_avc_comp
(
    DSM_COMP_T*      pt_dsm_comp
)
{
    INT32            i4_rc;
    DEVICE_TYPE_T    t_dev_type;
    
    i4_rc = DSMR_DEV_NOT_FOUND;

    t_dev_type = pt_dsm_comp->t_dev_type_name ;
    
    /*
       Need to protect access/operation on the static
       AVC component data arrays.
    */
    DSM_GRAB_SEMA();
    
    if ( DEV_GET_GROUP_ID(t_dev_type) == DEV_AVC )
    {
        /*
          performed resource free operation to the
          AVC component.
        */
        switch (t_dev_type)
        {
            case DEV_AVC_COMBI:
            {
                DBG_INFO(("{DSM} close AVC component => "
                          "handle: [%d]  "
                          "drv type id: [%d] "
                          "drv component id: [%d] "
                          "dsm device type: [%d] \n",
                          pt_dsm_comp->h_rm_hdle,
                          pt_dsm_comp->t_drv_type,
                          pt_dsm_comp->ui2_comp_id,
                          pt_dsm_comp->t_dev_type_name));
                /*
                  Perform AVC_COMBI specific operation.
                */
                i4_rc = dsm_avc_close_combi_comp(pt_dsm_comp);
            }
            break;

            default:
            {
                /*
                  No close ops needed, just continue.
                */
                i4_rc = DSMR_OK;
            }
            break;
        }
    }
    
    DSM_UNGRAB_SEMA();

    
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_open_avc_comp
 *
 * Description: This API get handle to a specified AVC driver.
 *
 * Input  -
 *    t_dev_type:  Specifies the device type.
 *
 *    ui2_idx:  specifies the index of the registered device (when more
 *              than one device of the same type is registered.  0 means
 *              the first device of that registered, etc.
 *
 *    ps_name:  A RM group name used to group related RM compoent.
 *
 *    ppt_dsm_obj:  Pointer to a DSM component structure containing
 *                  information to access the RM handle for the specified
 *                  device. 
 * Return -
 *   DSMR_OK:   synchronization source is set.
 *   DSMR_FAIL: Can not set the source of the synchronization for
 *                   the current time. 
 *          
 --------------------------------------------------------------------------*/
INT32 dsm_open_avc_comp
(
    DEVICE_TYPE_T    t_dev_type,
    UINT16			 ui2_idx,
    const CHAR*      ps_name,
    DSM_COMP_T**     ppt_dsm_obj
)
{
    INT32            i4_rc;
    RM_COND_T        e_rm_cond;
    HANDLE_T         h_rm_pipe;
    VOID*            pv_sel_data;
    dsm_sel_drv_data pf_sel_fct;
    
    i4_rc = DSMR_DEV_TYPE_NOT_SUPPORTED;
    h_rm_pipe = NULL_HANDLE;
    pv_sel_data = NULL;
    pf_sel_fct  = NULL;

    /*
       Protect access/operation on the static
       AVC component data arrays.
    */

    DSM_GRAB_SEMA();
    
    if ( DEV_GET_GROUP_ID(t_dev_type) == DEV_AVC )
    {
        /*
          pre-action performed before opening RM handle to the
          AVC component.
        */
        switch (t_dev_type)
        {
            case DEV_AVC_COMBI:
            {
                /*
                  Perform AVC_COMBI specific initialization.
                  
                  For AVC Combi, create a pipeline so the
                  AVC combi can be used as source to the TV decoder
                  (which is a sink) to monitor the signal.
                */
                i4_rc = dsm_avc_combi_open_pipe(&h_rm_pipe);
            }
            break;

            case DEV_AVC_SCART:
            {
                pv_sel_data = (VOID*) AVC_FLAG_SCART_MONITOR;
                pf_sel_fct  = dsm_avc_cmp_drv_data;
                i4_rc = DSMR_OK;
            }
            break;
            
            default:
            {
                /*
                  No pre-initialization needed.
                  just continue.
                */
                i4_rc = DSMR_OK;
            }
            break;
        }

        if ( i4_rc == DSMR_OK )
        {
            i4_rc = dsm_open_rm_comp(h_rm_pipe,
                                     t_dev_type,
                                     ui2_idx,
                                     ps_name,
                                     pv_sel_data,
                                     pf_sel_fct,
                                     at_avc_drv,
                                     ac_avc_name,
                                     at_dsm_avc_device,
                                     pat_dsm_avcs,
                                     MAX_NB_AVC_DRV_TYPE,
                                     dsm_rm_avc_nfy_fct,
                                     ppt_dsm_obj);
        }

        /*
          Perform post action  after opening RM handle to the
          AVC component.
        */
        switch (t_dev_type)
        {
            case DEV_AVC_COMBI:
            {
                DSM_AVC_COMBI_T*   pt_avc_combi;

                if ( i4_rc == DSMR_OK )
                {   
                    /*
                      Perform AVC_COMBI specific post action after
                      RM handle is opened to the AVC COMBI component.
                    */
                    
                    i4_rc = dsm_avc_combi_post_open_action(*ppt_dsm_obj);

                    if ( i4_rc == DSMR_OK )
                    {
                        pt_avc_combi = (DSM_AVC_COMBI_T*)
                            ((*ppt_dsm_obj)->pv_private) ;
                        /*
                          Store the pipe handle in AVC COMBI structure.
                          This pipe-handle will be needed later when user
                          try to monitor the AVC COMBI component.

                          When the monitoring is started by the client on a
                          specific AVC combi component, we will then connect
                          the TV decoder and AVC combi component using this
                          pipeline handle.
                          
                          To prevent handle leakage (CR00035075) We need check
                          if the AVC COMBI component returned from dsm_open_rm_comp()
                          is a newly created AVC COMBI component or an existing AVC COMBI
                          component.  If an existing COMBI is returned, then we need to
                          delete the 'h_rm_pipe' handle that was created and passed to
                          dsm_open_rm_comp(). If a new AVC COMBI component is created, then
                          we save the 'h_rm_pipe' in AVC COMBI strucutre.  When the AVC
                          combi is closed, we will then free that 'h_rm_pipe'.
                          
                        */
                        if ( (*ppt_dsm_obj)->ui1_comp_stat_flag == DSM_COMP_FOUND )
                        {
                            if ( pt_avc_combi->h_rm_pipe != NULL_HANDLE )
                            {
                                DBG_INFO(("Use exisitng AVC combi component, close new pipe handle: %d\n", h_rm_pipe));
                                rm_close(h_rm_pipe, FALSE, &e_rm_cond);
                            }
                            else
                            {
                                ABORT(DBG_CAT_INV_OP,DBG_ABRT_OPS_FAIL);
                            }
                            
                        }
                        else if ( (*ppt_dsm_obj)->ui1_comp_stat_flag == DSM_COMP_ALLOCATED )
                        {
                            if ( pt_avc_combi->h_rm_pipe == NULL_HANDLE )
                            {
                                DBG_INFO(("New AVC combi component: save pipe handle: %d\n", h_rm_pipe));
                                pt_avc_combi->h_rm_pipe = h_rm_pipe ;
                            }
                            else
                            {
                                ABORT(DBG_CAT_INV_OP,DBG_ABRT_OPS_FAIL);
                            }
                        }

                        if ( x_handle_set_tag(pt_avc_combi->h_rm_pipe,*ppt_dsm_obj)
                             != HR_OK )
                        {
                            ABORT(DBG_CAT_HANDLE,DBG_ABRT_OPS_FAIL);
                        }
                    }
                    else
                    {
                        /*
                          close the RM pipe handle and AVC COMBI handle
                        */
                        rm_close(h_rm_pipe, FALSE, &e_rm_cond);
                        rm_close((*ppt_dsm_obj)->h_rm_hdle, FALSE, &e_rm_cond);
                    }
                }
                else
                {
                    /*
                      Because AVC COMBI was not opened, we
                      need to close pipe that was opened to connect
                      TV decoder and AVC COMBI.
                    */
                    rm_close(h_rm_pipe, FALSE, &e_rm_cond);
                }
            }
            break;

            case DEV_AVC_SCART:
            {
                if ( i4_rc == DSMR_OK )
                {   
                    /*
                      Perform AVC_SCART specific post action after
                      RM handle is opened to the AVC SCART component.
                    */       
                    i4_rc = dsm_avc_scart_post_open_action(*ppt_dsm_obj);

                    if ( i4_rc != DSMR_OK )
                    {
                        /*
                          close AVC SCART handle
                        */
                        rm_close((*ppt_dsm_obj)->h_rm_hdle, FALSE, &e_rm_cond);
                    }
                }
                else
                {
                    /*
                      AVC SCART was not opened
                    */
                    x_dbg_stmt("{DSM} Error: can not open RM handle to AVC_SCART device.\n");
                }
            }
            break;

            default:
                break;
        }
    }

    DSM_UNGRAB_SEMA();
    
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_avc_get_ops
 *
 * Description: This API executed 'GET' operation for a specific
 *              DSM AVC device.
 *
 * Input -
 *    pt_dsm_obj:     Specifies reference to a DSM component structure.
 *    e_get_type:     Specifies the type of get operation for which the
 *                    data is returned by the avc driver.
 *    pz_size         Contains the length of buffer which is referenced
 *                    by pv_get_info.
 *
 * Output -
 *    pv_get_info:    Pointer to a structure containing informatio
 *    pz_size         Contains the length of the valid data in the
 *                    buffer referenced by the 'pv_get_info' argument.
 *
 * Return -
 *   DSMR_OK:   get operation success. 
 *   
 --------------------------------------------------------------------------*/
INT32 dsm_avc_get_ops
(
    DSM_COMP_T*      pt_dsm_obj,
    DSM_DEV_GET_T    e_get_type,
    VOID*            pv_get_info,
    SIZE_T*		     pz_size,
    VOID*            pv_data
)
{
    INT32            i4_rc;
    
    switch ( pt_dsm_obj->t_drv_type )
    {
        case DRVT_AVC_COMBI :
        {
            i4_rc = dsm_avc_combi_get_ops(pt_dsm_obj,
                                          e_get_type,
                                          pv_get_info,
                                          pz_size,
                                          pv_data);
        }
        break;

        case DRVT_AVC_SCART :
        {
            i4_rc = dsm_avc_scart_get_ops(pt_dsm_obj,
                                          e_get_type,
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
 * Name: dsm_avc_set_ops
 *
 * Description: This API executed 'SET' operation on DSM AVC device.
 *
 * Input -
 *    pt_dsm_obj:     Specifies reference to a DSM component structure.
 *    e_get_type:     Specifies the type of get operation for which the
 *                    data is returned by the avc driver.
 *    pz_size         Contains the length of buffer which is referenced
 *                    by pv_get_info.
 *
 * Output -
 *    pv_get_info:    Pointer to a structure containing informatio
 *    pz_size         Contains the length of the valid data in the
 *                    buffer referenced by the 'pv_get_info' argument.
 *
 * Return -
 *   DSMR_OK:   get operation success. 
 *   
 --------------------------------------------------------------------------*/
INT32 dsm_avc_set_ops
(
    DSM_COMP_T*      pt_dsm_obj,
    DSM_DEV_GET_T    e_set_type,
    VOID*            pv_set_info,
    SIZE_T           z_set_info_len
)
{
    INT32            i4_rc;
    
    switch ( pt_dsm_obj->t_drv_type )
    {
        case DRVT_AVC_COMBI :
        {
            i4_rc = dsm_avc_combi_set_ops(pt_dsm_obj,
                                          e_set_type,
                                          pv_set_info,
                                          z_set_info_len);
        }
        break;

        case DRVT_AVC_SCART :
        {
            i4_rc = dsm_avc_scart_set_ops(pt_dsm_obj,
                                          e_set_type,
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

/*
  Internal structure for passing avc search data to the DSM find component
  API.
*/
typedef struct _DSM_SELECT_DATA_T_
{
    DEVICE_TYPE_T    t_dev_type_name;
    UINT16			 ui2_idx;
    DSM_COMP_T*      pt_comp;
} DSM_SELECT_DATA_T;




/*--------------------------------------------------------------------------
 * Name: dsm_find_avc_obj
 *
 * Description - API to get the the pointer to the DSM avc component data 
 *               structure for the specified DSM device name and index number.
 *
 * Input  -
 *   pt_obj:      pointer to a componet object
 *   ui4_idx:     index into the driver resource array.
 *   ui4_sub_idx  index into the overflow bucket location.  
 *   pv_tag:      private tag provided to the iteration function. In this
 *                case: the pv_tag is a pointer to DSM_SELECT_DATA_T
 *                structure, which contains the dsm_device_name and index.
 *
 * Output -
 *   pv_tag: 
 *
 * Return - Void
 *  
------------------------------------------------------------------------- */
static INT32 dsm_find_avc_obj
(
    DSM_COMP_T* pt_obj,
    UINT32      ui4_idx,
    UINT32      ui4_sub_idx,
    VOID*       pv_tag
)
{
    DSM_SELECT_DATA_T*    pt_data;
    INT32                 i4_rc;

    i4_rc = DSMR_OK;

    if ( pv_tag != NULL )
    {
        pt_data = (DSM_SELECT_DATA_T*) pv_tag ;
    }
    else
    {
        return DSMR_INV_ARG;
    }
    
    if ( pt_obj->h_rm_hdle != NULL_HANDLE )
    {
        if ( (at_dsm_avc_device[ui4_idx] == pt_data->t_dev_type_name)  &&
             (ui4_sub_idx                == pt_data->ui2_idx)
            )
        {
            pt_data->pt_comp = pt_obj;

            DBG_INFO(("{DSM} AVC component => "
                      "AVC-name: [%s] "
                      "handle: [%d]  "
                      "drv type: [%d] "
                      "drv component id: [%d] "
                      "dsm device id: [%d] \n",
                      ac_avc_name[ui4_idx],
                      pt_obj->h_rm_hdle,
                      pt_obj->t_drv_type,
                      pt_obj->ui2_comp_id,
                      pt_obj->t_dev_type_name));

            /*
              Found the object Return negative value, so the iteration
              loop will stop.
            */
            i4_rc =  DSMR_FAIL;
        }
        
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_get_avc_comp
 *
 * Description: This API get handle to a specified AVC driver.
 *
 * Input  -
 *    t_dev_type_name:  Specifies the name of the device
 *
 *    ui2_idx:  specifies the index of the registered device (when more
 *              than one device of the same type is registered.  0 means
 *              the first device of that registered, then 
 *
 *    pv_src_info:      Pointer to a structure containing information
 *                      about the synchronization source. 
 *
 * Return -
 *   DSMR_OK:   synchronization source is set.
 *   DSMR_FAIL: Can not set the source of the synchronization for
 *                   the current time. 
 *          
 --------------------------------------------------------------------------*/
INT32 dsm_get_avc_comp
(
    DEVICE_TYPE_T    t_dev_type_name,
    UINT16			 ui2_idx,
    DSM_COMP_T**     ppt_dsm_obj
)
{
    DSM_SELECT_DATA_T    t_data;
    INT32                i4_rc;

    t_data.t_dev_type_name = t_dev_type_name;
    t_data.ui2_idx         = ui2_idx;
    t_data.pt_comp         = NULL;
    
    if ( DEV_GET_GROUP_ID(t_dev_type_name) == DEV_AVC )
    {
        dsm_iterate_comp_data(pat_dsm_avcs,
                              MAX_NB_AVC_DRV_TYPE,
                              dsm_find_avc_obj,
                              &t_data);
    }

    /*
      check if the specified DSM component handle is found.
    */
    if ( t_data.pt_comp != NULL )
    {
        *ppt_dsm_obj = t_data.pt_comp;
        i4_rc = DSMR_OK ;
    }
    else
    {
        i4_rc = DSMR_DEV_NOT_FOUND;
    }  
    return i4_rc;
}
