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
 * $RCSfile: dsm_tuner.c,v $
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

#include "dsm/_dsm.h"
#include "res_mngr/x_rm.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"

/*--------------------------------------------------------

static internal array variables holding the tuner information.
  
----------------------------------------------------------*/

#define   MAX_NB_TUNER_DRV_TYPE     8

static DRV_TYPE_T   at_tuner_drv[MAX_NB_TUNER_DRV_TYPE] =
{
    DRVT_TUNER_SAT_DIG,
    DRVT_TUNER_CAB_DIG,
    DRVT_TUNER_TER_DIG,
    DRVT_TUNER_SAT_ANA,
    DRVT_TUNER_CAB_ANA,
    DRVT_TUNER_TER_ANA,
    DRVT_TUNER_CAB_DIG_OOB_TX,
    DRVT_TUNER_CAB_DIG_OOB_RX   
};

static CHAR*       ac_tuner_name[MAX_NB_TUNER_DRV_TYPE] =
{
    "DRVT_TUNER_SAT_DIG",
    "DRVT_TUNER_CAB_DIG",
    "DRVT_TUNER_TER_DIG",
    "DRVT_TUNER_SAT_ANA",
    "DRVT_TUNER_CAB_ANA",
    "DRVT_TUNER_TER_ANA",
    "DRVT_TUNER_CAB_DIG_OOB_TX",
    "DRVT_TUNER_CAB_DIG_OOB_RX"   
};

/*
  DSM device name corresponding to the tuner driver name.
*/
static DEVICE_TYPE_T    at_dsm_tuner_device[MAX_NB_TUNER_DRV_TYPE] =
{
    DEV_TUNER_SAT_DIG,
    DEV_TUNER_CAB_DIG,
    DEV_TUNER_TER_DIG,
    DEV_TUNER_SAT_ANA,
    DEV_TUNER_CAB_ANA,
    DEV_TUNER_TER_ANA,
    DEV_TUNER_CAB_DIG_OOB_TX,
    DEV_TUNER_CAB_DIG_OOB_RX   
};

/*
  The structure for storing dsm component is a similar to a hash table.
  It has an static array of n element.  For each element, it store an
  DSM tuner driver component. If more then one tuner driver component of
  the same type existed, then then the next DSM component of that
  driver is store in the overflow bucket. The overflow bucket is
  dynamically allocated, and 'pt_next' point the the overflow bucket.


     [TUNER_SAT_DIG] -->   NULL
     [TUNER_TER_DIG] --> [TUNER_TER_DIG] --> NULL
     [TUNER_CAB_DIG] -->   NULL

  Here, the driver type SAT_DIG and CAB_DIG has only instance of the driver,
  but the TER_DIG has two tuners.
*/
static  DSM_COMP_T*  pat_dsm_tuners[MAX_NB_TUNER_DRV_TYPE] ;


/*--------------------------------------------------------------------------
 * Name: dsm_print_obj
 *
 * Description - Handle notify function for the RTC component handle. This
 *               function is called by the Resource Manager when the RTC 
 *               component's state or condition changed.
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
                   "tuner-name: [%s] "
                   "tuner drv type: [%d] "
                   "tuner drv component id: [%d] "
                   "tuner device type id:   [%d]\n",
                   pt_obj->h_rm_hdle,
                   ac_tuner_name[ui4_idx],
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
 * Name: dsm_tuner_close_rm_hdle
 *
 * Description - Close the tuner component handle.
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
static INT32 dsm_tuner_close_rm_hdle
(
    DSM_COMP_T* pt_obj,
    UINT32      ui4_idx,
    UINT32      ui4_sub_idx,
    VOID*       pv_tag
)
{

    if ( pt_obj->h_rm_hdle != NULL_HANDLE )
    {
        if ( rm_close(pt_obj->h_rm_hdle,FALSE,NULL) ==RMR_OK )
        {
            x_dbg_stmt("{DSM} %d %d Close tuner resource comp handle: [%d] "
                       "avc-name: [%s] "
                       "drv type: [%d] "
                       "drv component id: [%d] "
                       "device type id:   [%d]\n",
                       ui4_idx,
                       ui4_sub_idx,
                       pt_obj->h_rm_hdle,
                       ac_tuner_name[ui4_idx],
                       pt_obj->t_drv_type,
                       pt_obj->ui2_comp_id,
                       pt_obj->t_dev_type_name);
            
            pt_obj->h_rm_hdle=NULL_HANDLE;
        }
    }

    /*
      always return OK so the next element will be visited.
    */
    return  DSMR_OK ;
}

/*--------------------------------------------------------------------------
 * Name: dsm_tuner_info
 *
 * Description - API to print out information about all the
 *               tuners found  by the DSM.
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
VOID dsm_tuner_info(VOID)
{
    /*
       Need to protect access/operation on the static
       Tuner component data arrays.
    */

    DSM_GRAB_SEMA();
    
    dsm_iterate_comp_data(pat_dsm_tuners,
                          MAX_NB_TUNER_DRV_TYPE,
                          dsm_print_obj, NULL);

    DSM_UNGRAB_SEMA();
    
    return;
}


/*--------------------------------------------------------------------------
 * Name: dsm_tuner_close
 *
 * Description - API to close the Tuner component handle allocated by the DSM,
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
VOID dsm_tuner_close(VOID)
{
    /*
       Need to protect access/operation on the static
       Tuner component data arrays.
    */

    DSM_GRAB_SEMA();
    
    /*
      iterate over each tuner component element and free the RM handle
      associated with the tuner component.
    */
    dsm_iterate_comp_data(pat_dsm_tuners,
                          MAX_NB_TUNER_DRV_TYPE,
                          dsm_tuner_close_rm_hdle, NULL);

    /*
      Free the Tuner component element.
    */
    dsm_free_comp_data(pat_dsm_tuners,MAX_NB_TUNER_DRV_TYPE);

    DSM_UNGRAB_SEMA();    
    
    return;
}

/*--------------------------------------------------------------------------
 * Name: dsm_rm_tuner_nfy_fct
 *
 * Description - Handle notify function for the tuner driver. This
 *               function is called by the Resource Manager when the RTC 
 *               component's state or condition changed.
 *
 * Input  - h_obj:  handle to the RTC component
 *          ui1_port:  Not used by the RTC component. 
 *          e_nfy_cond:  Condition of RTC component 
 *          pv_tag:    Tag pass into Resource Manager when opening
 *                     this RTC component.
 *          ui4_data:  Private data, not used by the RTC component.
 *
 * Output - None
 *
 * Return - Void
 *
 --------------------------------------------------------------------------*/
static  VOID dsm_rm_tuner_nfy_fct 
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
               index into the internal tuner array.
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
            DBG_INFO(("{DSM}: component %s open\n",ac_tuner_name[i4_n]));
            /*
              Send notification to the DSM thread.
            */
            
        }
        break;
        
        case RM_COND_CLOSING:
        {
            /* Resource Manager handle to the device object closing. */
            DBG_INFO(("{DSM}: component %s closing\n",ac_tuner_name[i4_n]));
        }
        break;

        case RM_COND_CLOSED:
        {
            /* Resource Manager handle to the device object has been closed. */
            DBG_INFO(("{DSM}: component %s closed\n",ac_tuner_name[i4_n]));
            
            /*
              Send notification to the DSM thread.
            */
            t_msg.h_dsm_comp  = h_dsm_comp;
            t_msg.e_msg_code   = DSM_MSG_DEV_NFY;
            dsm_send_msg(&t_msg);
        }
        break;

        default:
        {
            i4_n++ ;  /* use the variable to prvent compiler warning.  */
        }
        break;
    }
    return ;
}

/*--------------------------------------------------------------------------
 * Name: dsm_tuner_init
 *
 * Description: This API initialize the tuner interface. It will open
 *              an handle to all the tuner components that has been
 *              registered with the Resource Manager.
 *
 * Input  -
 *    None
 *
 * Return -
 *   DSMR_OK:   tuner handles are  registered with DSM.
 *   DSMR_FAIL: 
 *          
 --------------------------------------------------------------------------*/

INT32 dsm_tuner_init(VOID)
{
    INT32              i4_rc;
    INT32              i4_n;
    INT32              i4_nb_tuners;

    i4_rc = DSMR_OK;
    
    i4_nb_tuners = (INT32)(sizeof(at_tuner_drv)/sizeof(DRV_TYPE_T));

    /*
      check the number of tuner types with the array size.
    */
    if ( MAX_NB_TUNER_DRV_TYPE != i4_nb_tuners )
    {
        i4_rc =  DSMR_FAIL;
    }
    else
    {
        for ( i4_n = 0 ; i4_n < i4_nb_tuners ; i4_n++ )
        {
            pat_dsm_tuners[i4_n] = NULL ;
        }
    }
    return i4_rc;
}

/*
  structure for passing tuner search data to the DSM find component
  API.
*/
typedef struct _DSM_SELECT_DATA_T_
{
    DEVICE_TYPE_T    t_dev_type_name;
    UINT16			 ui2_idx;
    DSM_COMP_T*      pt_comp;
} DSM_SELECT_DATA_T;

/*--------------------------------------------------------------------------
 * Name: dsm_find_tuner_obj
 *
 * Description - API to get the the pointer to the DSM tuner component data 
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
static INT32 dsm_find_tuner_obj
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
        if ( (at_dsm_tuner_device[ui4_idx] == pt_data->t_dev_type_name)  &&
             (ui4_sub_idx                  == pt_data->ui2_idx)
            )
        {
            pt_data->pt_comp = pt_obj;

            DBG_INFO(("{DSM} Tuner component => "
                      "tuner-name: [%s] "
                      "handle: [%d]  "
                      "drv type: [%d] "
                      "drv component id: [%d] "
                      "dsm device id: [%d] \n",
                      ac_tuner_name[ui4_idx],
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
 * Name: dsm_get_tuner_comp
 *
 * Description: This API get handle to a specified tuner driver.
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
INT32 dsm_get_tuner_comp
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
    
    if ( DEV_GET_GROUP_ID(t_dev_type_name) == DEV_TUNER )
    {
        dsm_iterate_comp_data(pat_dsm_tuners,
                              MAX_NB_TUNER_DRV_TYPE,
                              dsm_find_tuner_obj,
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


/*--------------------------------------------------------------------------
 * Name: dsm_open_tuner_comp
 *
 * Description: This API get handle to a specified TUNER driver.
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
INT32 dsm_open_tuner_comp
(
    DEVICE_TYPE_T    t_dev_type,
    UINT16			 ui2_idx,
    const CHAR*      ps_name,
    DSM_COMP_T**     ppt_dsm_obj
)
{
    INT32            i4_rc;
    HANDLE_T         h_rm_pipe;
    
    i4_rc = DSMR_DEV_TYPE_NOT_SUPPORTED;
    h_rm_pipe = NULL_HANDLE;
    /*
       Need to protect access/operation on the static
       Tuner component data arrays.
    */

    DSM_GRAB_SEMA();
    
    if ( DEV_GET_GROUP_ID(t_dev_type) == DEV_TUNER )
    {
        i4_rc = dsm_open_rm_comp(h_rm_pipe,
                                 t_dev_type,
                                 ui2_idx,
                                 ps_name,
                                 NULL,
                                 NULL,
                                 at_tuner_drv,
                                 ac_tuner_name,
                                 at_dsm_tuner_device,
                                 pat_dsm_tuners,
                                 MAX_NB_TUNER_DRV_TYPE,
                                 dsm_rm_tuner_nfy_fct,
                                 ppt_dsm_obj);
        
    }

    DSM_UNGRAB_SEMA();
     
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_tuner_get_ops
 *
 * Description: This API executed 'GET' operation on DSM tuner device.
 *
 * Input -
 *    pt_dsm_obj:  Referencing the DSM component structure.
 *    e_get_type:  Specifies the type of get operation for which the
 *                 data is returned by the tuner driver.
 *    pz_size      Contains the length of buffer which is referenced
 *                 by pv_get_info.
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
INT32 dsm_tuner_get_ops
(
    DSM_COMP_T*      pt_dsm_obj,
    DSM_DEV_GET_T    e_get_type,
    VOID*            pv_get_info,
    SIZE_T*		     pz_size,
    VOID*            pv_data
)
{
    INT32            i4_rc;
    RM_COND_T        e_cond;
    

    i4_rc = rm_get(pt_dsm_obj->h_rm_hdle,
                   FALSE,
                   ANY_PORT_NUM,
                   (DRV_GET_TYPE_T)e_get_type,
                   pv_get_info,
                   pz_size,
                   &e_cond);
    
    return dsm_rm_to_dsm_ret_code(i4_rc);
}

/*--------------------------------------------------------------------------
 * Name: dsm_tuner_set_ops
 *
 * Description: This API executed 'SET' operation on DSM tuner device.
 *
 * Input -
 *    pt_dsm_obj:  Eeferencing the DSM component structure.
 *    e_get_type:  Specifies the type of get operation for which the
 *                 data is returned by the tuner driver.
 *    pz_size      Contains the length of buffer which is referenced
 *                 by pv_get_info.
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
INT32 dsm_tuner_set_ops
(
    DSM_COMP_T*      pt_dsm_obj,
    DSM_DEV_GET_T    e_set_type,
    VOID*            pv_set_info,
    SIZE_T           z_set_info_len
)
{
    INT32            i4_rc;
    RM_COND_T        e_cond;
    

    i4_rc = rm_set(pt_dsm_obj->h_rm_hdle,
                   FALSE,
                   ANY_PORT_NUM,
                   (DRV_SET_TYPE_T)e_set_type,
                   pv_set_info,
                   z_set_info_len,
                   &e_cond);
    
    return dsm_rm_to_dsm_ret_code(i4_rc);
}

