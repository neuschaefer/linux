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
 * $RCSfile: prc_filter.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Felisa Hsiao $
 * $MD5HEX: f2a778fe5ea7154d439315adcd6e25f4 $
 *
 * Description: 
 *         PM internal filter handler 
 * History:
 *                
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "prc_mngr/prc_mngr.h"
#include "prc_mngr/prc_common.h"
#include "prc_mngr/prc_filter.h"
#include "prc_mngr/prc_lock.h"
#include "prc_mngr/prc_util.h"
#include "prc_mngr/prc_thread.h"
#include "prc_mngr/prc_debug.h"

#if (PM_ENABLE_FILTER_CTRL)
#include "prc_mngr/prc_filter_db.h"
#endif

#include "conn_mngr/x_cm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
 
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/* Filter Handler */
static BOOL b_filter_init = FALSE;    

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
void pmi_filter_nfy_fct(HANDLE_T   h_obj,
                       UINT8      ui1_port,
                       RM_COND_T  e_nfy_cond,
                       VOID*      pv_tag,
                       UINT32     ui4_data);
                       
SELECT_T pm_comp_sel_first_unused_one(DRV_TYPE_T   e_type,
                                      UINT16       ui2_id,
                                      VOID*        pv_comp_sel_data,
                                      const VOID*  pv_comp_data,
                                      SIZE_T       z_comp_data_len,
                                      UINT16       ui2_num_active,
                                      UINT16       ui2_num_wait,
                                      CTRL_TYPE_T  e_ctrl,
                                      BOOL         b_would_conflict);
                                      
SELECT_T pm_comp_sel_specific_used_one(DRV_TYPE_T   e_type,
                                       UINT16       ui2_id,
                                       VOID*        pv_comp_sel_data,
                                       const VOID*  pv_comp_data,
                                       SIZE_T       z_comp_data_len,
                                       UINT16       ui2_num_active,
                                       UINT16       ui2_num_wait,
                                       CTRL_TYPE_T  e_ctrl,
                                       BOOL         b_would_conflict);                                      
                                      
INT32 pmi_filter_alloc_present(PM_SERVICE_INFO_T*   pt_svdb_node, 
                               DRV_TYPE_T           t_filter_type,                     
                               PM_COMPONENT_INFO_T* pt_esdb_node);
                               
INT32 pmi_filter_free_present(PM_COMPONENT_INFO_T* pt_esdb_node);                                    
                                  
#if (PM_ENABLE_MULTIMEDIA)  
INT32 pmi_filter_alloc_multimedia(PM_SERVICE_INFO_T*   pt_svdb_node, 
                                  DRV_TYPE_T           t_filter_type,                     
                                  PM_COMPONENT_INFO_T* pt_esdb_node); 
                                  
INT32 pmi_filter_free_multimedia(PM_COMPONENT_INFO_T* pt_esdb_node);                                      
#endif                                                                      
                                  

                                                      

#if PM_ENABLE_SCRAMBLE_DETECTION                                      
BOOL pmi_scramble_status_nfy_fct(VOID*         pv_nfy_tag,
                                 DEMUX_COND_T  e_nfy_cond,
                                 UINT32        ui4_data_1,
                                 UINT32        ui4_data_2
                            #ifndef MW_CLASSIC_DMX_NFY
                                 ,
                                 UINT64        ui8_ext_data_1,
                                 UINT64        ui8_ext_data_2,
                                 UINT32        ui4_ext_data_3
                            #endif
                                 );
                                 
#if (PM_ENABLE_FILTER_CTRL)
BOOL pmi_scramble_status_nfy_central_ctrl_fct(VOID*         pv_nfy_tag,
                                              DEMUX_COND_T  e_nfy_cond,
                                              UINT32        ui4_data_1,
                                              UINT32        ui4_data_2
                                    #ifndef MW_CLASSIC_DMX_NFY
                                              ,
                                              UINT64        ui8_ext_data_1,
                                              UINT64        ui8_ext_data_2,
                                              UINT32        ui4_ext_data_3
                                    #endif
                                              );   
#endif
                                                                    
#endif                                 
                                 
/*-----------------------------------------------------------------------------
 * Name: pmi_filter_nfy_fct
 *
 * Description: This API is responsible to handle the notification from RM for 
 *              HW filter component.
 *
 * Inputs:  h_obj            Contains a handle referencing a filter component.
 *          ui1_port         Ignores this argument.
 *          e_nfy_cond       Contains the notify conditions.
 *          pv_tag           Contains the tag value which was specified in the
 *                           API rm_open_comp.
 *          ui4_data         Ignores this argument.
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/                      
VOID pmi_filter_nfy_fct(HANDLE_T   h_obj,
                       UINT8      ui1_port,
                       RM_COND_T  e_nfy_cond,
                       VOID*      pv_tag,
                       UINT32     ui4_data)
{
    PM_COMPONENT_INFO_T* pt_esdb_node;
    RM_COND_T            e_last_cond;
    RM_COND_T            t_cond;
            
    pt_esdb_node = pv_tag;
    
    if (pt_esdb_node != NULL)
    {
        if ((ESNODE_FILTER_HANDLE(pt_esdb_node) == h_obj)&&(e_nfy_cond != RM_COND_NO_CHG))
        {
            e_last_cond = ESNODE_FILTER_COND(pt_esdb_node);
            ESNODE_FILTER_COND(pt_esdb_node) = e_nfy_cond;   
            
            /* check object state */
            t_cond = rm_cond_chg (e_nfy_cond, e_last_cond, COND_GRP_OBJ_STATE); 
            switch (t_cond)
            {            
                case RM_COND_CLOSED:
                    PM_DBG_INFO("F[%5d] Closed\n", ESNODE_PID(pt_esdb_node));
                    pmi_thread_filter_closed(h_obj, pt_esdb_node);
                    break;
                case RM_COND_OPENED:
                    PM_DBG_INFO("F[%5d] Opened\n", ESNODE_PID(pt_esdb_node));                    
                    pmi_thread_filter_opened(h_obj, pt_esdb_node);
                    break;                
                default:
                    /* ToDo:
                       Other state update */
                    PM_DBG_INFO("F[%5d] Cond:%d\n", t_cond, ESNODE_PID(pt_esdb_node));                    
                    break;
            }
        }
        else
        {
            PM_DBG_ERROR( "filter callback is exceptional\n" );
        }
    } 
    else
    {
        PM_DBG_ERROR("Invalid input for filter notification\n");
    }       
}  

/*-----------------------------------------------------------------------------
 * Name: pm_comp_sel_first_unused_one
 *
 * Description: This API is responsible for RM to select a specific component 
 *              and it always select the first unused one.
 *
 * Inputs:  e_type           Contains the component type.
 *          ui2_id           Ignores this argument.
 *          pv_comp_sel_data Ignores this argument.
 *          z_comp_data_len  Ignores this argument.
 *          ui2_num_active   If the component is already in use this argument 
 *                           lists the number of users (handles). If the 
 *                           component is not in use, this argument will be 
 *                           set to '0'.
 *          ui2_num_wait     In case argument "ui2_num_active > 0" then other 
 *                           users (handles) may be waiting for the component 
 *                           to become available. This argument contains the 
 *                           number of waiting users (handles).
 *          e_ctrl           In case argument "ui2_num_active > 0" this 
 *                           argument contains the current control mode of 
 *                           the component.
 *          b_would_conflict this argument indicates if the component would 
 *                           have a conflict with another, alreday opened 
 *                           component, if this one would be opened.
 * Outputs: -
 *
 * Returns: SEL_COMP_BREAK   Selects ths current one and stop.
 *          SEL_NEXT         Go ahead ro next one.
 ----------------------------------------------------------------------------*/  
SELECT_T pm_comp_sel_first_unused_one(DRV_TYPE_T   e_type,
                                      UINT16       ui2_id,
                                      VOID*        pv_comp_sel_data,
                                      const VOID*  pv_comp_data,
                                      SIZE_T       z_comp_data_len,
                                      UINT16       ui2_num_active,
                                      UINT16       ui2_num_wait,
                                      CTRL_TYPE_T  e_ctrl,
                                      BOOL         b_would_conflict)
{
    if ((ui2_num_active == 0)&&(b_would_conflict == FALSE))
        return SEL_COMP_BREAK;
    else
        return SEL_NEXT;
} 

/*-----------------------------------------------------------------------------
 * Name: pm_comp_sel_specific_used_one
 *
 * Description: This API is responsible for RM to select a specific component 
 *              and it always select the specifict used one.
 *
 * Inputs:  e_type           Contains the component type.
 *          ui2_id           Ignores this argument.
 *          pv_comp_sel_data Ignores this argument.
 *          z_comp_data_len  Ignores this argument.
 *          ui2_num_active   If the component is already in use this argument 
 *                           lists the number of users (handles). If the 
 *                           component is not in use, this argument will be 
 *                           set to '0'.
 *          ui2_num_wait     In case argument "ui2_num_active > 0" then other 
 *                           users (handles) may be waiting for the component 
 *                           to become available. This argument contains the 
 *                           number of waiting users (handles).
 *          e_ctrl           In case argument "ui2_num_active > 0" this 
 *                           argument contains the current control mode of 
 *                           the component.
 *          b_would_conflict this argument indicates if the component would 
 *                           have a conflict with another, alreday opened 
 *                           component, if this one would be opened.
 * Outputs: -
 *
 * Returns: SEL_COMP_BREAK   Selects ths current one and stop.
 *          SEL_NEXT         Go ahead ro next one.
 ----------------------------------------------------------------------------*/  
SELECT_T pm_comp_sel_specific_used_one(DRV_TYPE_T   e_type,
                                       UINT16       ui2_id,
                                       VOID*        pv_comp_sel_data,
                                       const VOID*  pv_comp_data,
                                       SIZE_T       z_comp_data_len,
                                       UINT16       ui2_num_active,
                                       UINT16       ui2_num_wait,
                                       CTRL_TYPE_T  e_ctrl,
                                       BOOL         b_would_conflict)
{
    if ((ui2_num_active >= 1)&&(b_would_conflict == FALSE))
        return SEL_COMP_BREAK;
    else
        return SEL_NEXT;
} 

#if PM_ENABLE_SCRAMBLE_DETECTION        
/*-----------------------------------------------------------------------------
 * Name: pmi_scramble_status_nfy_fct
 *
 * Description: This API is responsible handle the demux scramble status 
 *              notification. 
 *
 * Inputs:  pv_nfy_tag       References the tag for notification.
 *          e_nfy_cond       Contains the notify type.
 *          ui4_data_1       Contains the scramble status.
 *          ui4_data_1       Not used.
 *
 * Outputs: -
 *
 * Returns: FALSE
 ----------------------------------------------------------------------------*/                                
BOOL pmi_scramble_status_nfy_fct(VOID*         pv_nfy_tag,
                                 DEMUX_COND_T  e_nfy_cond,
                                 UINT32        ui4_data_1,
                                 UINT32        ui4_data_2
                            #ifndef MW_CLASSIC_DMX_NFY
                                 ,
                                 UINT64        ui8_ext_data_1,
                                 UINT64        ui8_ext_data_2,
                                 UINT32        ui4_ext_data_3
                            #endif
                                 )
{
    PM_COMPONENT_INFO_T* pt_esdb_node;
    DEMUX_SCRS_T         t_scramble_status;
    
    pt_esdb_node = pv_nfy_tag;
    t_scramble_status = (DEMUX_SCRS_T) ui4_data_1;
    
    if (e_nfy_cond == DEMUX_COND_SCRS)
    {
        pmi_thread_scramble_updated(ESNODE_FILTER_HANDLE(pt_esdb_node), pt_esdb_node, t_scramble_status);
    }
    else
    {
        PM_DBG_ERROR("Receive other notify :d from demux\r\n", e_nfy_cond);
    }
    
    return FALSE;
}                             


#if (PM_ENABLE_MULTIMEDIA)
#if (PM_ENABLE_RECORD)
BOOL pmi_recdemux_scramble_status_nfy_fct(
            VOID*                   pv_nfy_tag,
            RECDEMUX_COND_T         e_nfy_cond,
            UINT32                  ui4_data_1 )
{
    PM_COMPONENT_INFO_T*    pt_esdb_node;
    RECDEMUX_SCRS_T         t_recdemux_scramble_status;
    DEMUX_SCRS_T            t_dmx_scramble_status = DEMUX_SCRS_UNKNOWN;
    
    pt_esdb_node                = pv_nfy_tag;
    t_recdemux_scramble_status  = (RECDEMUX_SCRS_T) ui4_data_1;

    switch (t_recdemux_scramble_status)
    {
        case RECDEMUX_SCRS_SCRAMBLED:
            t_dmx_scramble_status = DEMUX_SCRS_SCRAMBLED;
            break;
        case RECDEMUX_SCRS_CLEAR:
            t_dmx_scramble_status = DEMUX_SCRS_CLEAR;
            break;
        case RECDEMUX_SCRS_UNKNOWN:
            t_dmx_scramble_status = DEMUX_SCRS_UNKNOWN;
            break;
    }
    
    if (e_nfy_cond == RECDEMUX_COND_SCRS)
    {
        pmi_thread_scramble_updated(ESNODE_FILTER_HANDLE(pt_esdb_node), pt_esdb_node, t_dmx_scramble_status);
    }
    else
    {
        PM_DBG_ERROR("Receive other notify :d from demux\r\n", e_nfy_cond);
    }
    
    return FALSE;
}
#endif
#endif


#if (PM_ENABLE_FILTER_CTRL)
/*-----------------------------------------------------------------------------
 * Name: pmi_scramble_status_nfy_central_ctrl_fct
 *
 * Description: This API is responsible to handle the demux scramble status 
 *              notification while PID central control is enabled. 
 *
 * Inputs:  pv_nfy_tag       References the tag for notification.
 *          e_nfy_cond       Contains the notify type.
 *          ui4_data_1       Contains the scramble status.
 *          ui4_data_1       Not used.
 *
 * Outputs: -
 *
 * Returns: FALSE
 ----------------------------------------------------------------------------*/                                
BOOL pmi_scramble_status_nfy_central_ctrl_fct(VOID*         pv_nfy_tag,
                                              DEMUX_COND_T  e_nfy_cond,
                                              UINT32        ui4_data_1,
                                              UINT32        ui4_data_2
                                    #ifndef MW_CLASSIC_DMX_NFY
                                              ,
                                              UINT64        ui8_ext_data_1,
                                              UINT64        ui8_ext_data_2,
                                              UINT32        ui4_ext_data_3
                                    #endif
                                              )
{
    PM_FILTER_CTRL_OBJ_T* pt_filter_ctrl = NULL; 
    DEMUX_SCRS_T         t_scramble_status;
    
    pt_filter_ctrl = pv_nfy_tag;
    t_scramble_status = (DEMUX_SCRS_T) ui4_data_1;
    
    if (e_nfy_cond == DEMUX_COND_SCRS)
    {
        pmi_thread_scramble_updated_for_central_ctrl(NULL_HANDLE, pt_filter_ctrl, t_scramble_status);
    }
    else
    {
        PM_DBG_ERROR("Receive other notify :d from demux\r\n", e_nfy_cond);
    }
    
    return FALSE;
}                           
#endif /* #if (PM_ENABLE_FILTER_CTRL) */            
                                              
#endif     

/*-----------------------------------------------------------------------------
 * Name: pmi_filter_init
 *
 * Description: This API initialize filter handler module 
 *
 * Inputs:  ui2_max_filter_ctrl_size  The max. size for filter control element
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *          PMR_ALREADY_INIT This filter handler is already initialized.
 ----------------------------------------------------------------------------*/       
INT32 pmi_filter_init(UINT16 ui2_max_filter_ctrl_size)
{    
    /* Do Nothing */
    if (!b_filter_init)
    {
        b_filter_init = TRUE;
        
#if (PM_ENABLE_FILTER_CTRL)
        if (pmi_filter_db_init(ui2_max_filter_ctrl_size)==PMR_OK)
            return PMR_OK;
        else
            return PMR_FAILED;
#else               
        return PMR_OK;      
#endif                 
    }
    else
    {
        return PMR_ALREADY_INIT;
    }    
}

/*-----------------------------------------------------------------------------
 * Name: pmi_filter_alloc_present
 *
 * Description: This API allocates filter based on the filter type for 
 *              present mode. 
 *
 * Inputs:  pt_svdb_node     Contains the reference to SVDB node.
 *          t_filter_type    Contains teh filter type
 *          pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     The referenced ESDB node will contains the handle 
 *                           of allocated filter.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     Filter module is not initialized.
 *          PMR_INV_ARG      Invalid arguments.
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/   
INT32 pmi_filter_alloc_present(PM_SERVICE_INFO_T*   pt_svdb_node, 
                               DRV_TYPE_T           t_filter_type,                     
                               PM_COMPONENT_INFO_T* pt_esdb_node)
{
    CM_CTRL_RM_SRC_COMP_T  t_ctrl_rm_src_comp; 
    const VOID*  pv_set_info = NULL;
    SIZE_T       z_set_info_len = 0;    
    INT32        i4_return;
    INT32        i4_rt_val;
    RM_COND_T    t_cond;
    BOOL         b_execute_allocate_detail = FALSE;    
    
#if (PM_ENABLE_FILTER_CTRL)    
    BOOL         b_filter_check_failed = TRUE;
    UINT16       ui2_filter_id = ANY_COMP_ID;
    DEMUX_NFY_INFO_T t_ctrl_scramble_nfy;
    PM_FILTER_CTRL_OBJ_T* pt_filter_ctrl = NULL; 
#endif
                    
    i4_return = PMR_FAILED;   
    ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_WAIT_OPEN;
    
    /* 1. Get Source component handle */
    i4_rt_val = x_cm_get(SVNODE_CONNECTION_HANDLE(pt_svdb_node), 
                       CM_CTRL_RM_SRC_COMP,
                       (VOID*) & t_ctrl_rm_src_comp);

    if (i4_rt_val == CMR_OK)
    {

#if (PM_ENABLE_FILTER_CTRL) 
        /* 2. Open Filter */      
        b_execute_allocate_detail = FALSE;
        
        if (pmi_filter_db_check(t_ctrl_rm_src_comp.h_rm_comp, t_filter_type, *pt_esdb_node, &ui2_filter_id) == PMR_OK)
        {
            b_filter_check_failed = FALSE;
            
            if (ui2_filter_id == ANY_COMP_ID)
            {
                i4_rt_val = rm_open_comp(t_ctrl_rm_src_comp.h_rm_comp, OBJ_COMP_REL_SRC, t_filter_type, ui2_filter_id, 
                                         NULL, CTRL_TYPE_SHARED, NULL, pm_comp_sel_first_unused_one, pt_esdb_node, pmi_filter_nfy_fct, 
                                         &(ESNODE_FILTER_HANDLE(pt_esdb_node)), &(ESNODE_FILTER_COND(pt_esdb_node)));
            }
            else
            {
                i4_rt_val = rm_open_comp(t_ctrl_rm_src_comp.h_rm_comp, OBJ_COMP_REL_SRC, t_filter_type, ui2_filter_id, 
                                         NULL, CTRL_TYPE_SHARED, NULL, pm_comp_sel_specific_used_one, pt_esdb_node, pmi_filter_nfy_fct, 
                                         &(ESNODE_FILTER_HANDLE(pt_esdb_node)), &(ESNODE_FILTER_COND(pt_esdb_node)));
            }
                                                 
            if ((i4_rt_val == RMR_OK) || (i4_rt_val == RMR_ARBITRATION))
            {
                pmi_filter_db_add(t_ctrl_rm_src_comp.h_rm_comp, t_filter_type, pt_esdb_node);
               
                pt_filter_ctrl = ESNODE_FILTER_OBJ_CTRL(pt_esdb_node);
                ESNODE_CA_COND(pt_esdb_node) = pt_filter_ctrl->t_cond;
               
                if (ui2_filter_id == ANY_COMP_ID)
                {
                    PM_DBG_INFO("F[%5d] need to connect, \n", ESNODE_PID(pt_esdb_node));
                    b_execute_allocate_detail = TRUE;               
                }
                else
                {
                    PM_DBG_INFO("F[%5d] has same one, do NOT connect, \n", ESNODE_PID(pt_esdb_node));
                    b_execute_allocate_detail = FALSE;
                }
            }
            else
            {
               /* ToDo */
               PM_DBG_ERROR("F[%5d] Failed to open filter \n", ESNODE_PID(pt_esdb_node));
            }
        } 
        else
        {
            /* ToDo */
            b_filter_check_failed = TRUE;
        }              
#else
        /* 2. Open Filter */
        i4_rt_val = rm_open_comp(t_ctrl_rm_src_comp.h_rm_comp, OBJ_COMP_REL_SRC, t_filter_type, ANY_COMP_ID, 
                         NULL, CTRL_TYPE_SINGLE, NULL, pm_comp_sel_first_unused_one, pt_esdb_node, pmi_filter_nfy_fct, 
                         &(ESNODE_FILTER_HANDLE(pt_esdb_node)), &(ESNODE_FILTER_COND(pt_esdb_node)));                
                                 
        if ((i4_rt_val == RMR_OK) || (i4_rt_val == RMR_ARBITRATION))
        {
            b_execute_allocate_detail = TRUE;
        }
        else
        {
            PM_DBG_ERROR("F[%5d] Failed to open filter \n", ESNODE_PID(pt_esdb_node));
            /* ToDo */
        }
#endif        

        if (b_execute_allocate_detail == TRUE)
        {   
            t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
            
            switch (t_cond)
            {
                case RM_COND_OPENING:
                    i4_return = PMR_OK;                              
                    break;
                    
                case RM_COND_OPENED:                            
                    /* 3. Connect Filter */
            #if (!PM_ENABLE_FILTER_CTRL) 
                #if (!PM_AUTO_CONNECT_BY_RM) 
                    if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_CONNECT) == TRUE) &&
                        (pmi_filter_connect(pt_svdb_node, pt_esdb_node) == PMR_OK))
                    {                                        
                        t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                        switch (t_cond)
                        {
                            case RM_COND_CONNECTED:  
                                ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                break;                                                
                            case RM_COND_CONNECTING:
                                ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                break;
                            default:
                                PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                break;
                        }
                    }
                    else
                    {
                        PM_DBG_ERROR("F[%5d] connect FAIL \n", ESNODE_PID(pt_esdb_node));
                    }
                #endif /* (!PM_AUTO_CONNECT_BY_RM) */
            #else
                    ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;    
            #endif
                    /* 4. Set PID/Stream ID and destination ID by filter type */
                    switch (t_filter_type)
                    {
                        case DRVT_DEMUX_TS_PES_PACKET:
                            
                            if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0 , DEMUX_SET_TYPE_PID, 
                                   (const void *)((UINT32)(ESNODE_PID(pt_esdb_node))), sizeof(MPEG_2_PID_T), &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK)
                            {
                                BOOL    fg_need_set_dest = TRUE;

                                switch (SVNODE_PROC_TYPE(pt_svdb_node))
                                {
                                    case PM_PRESENT:
                                        pv_set_info = &(ESNODE_SINK_INFO(pt_esdb_node));
                                        z_set_info_len = sizeof(DEMUX_DEST_INFO_T); 
                                        fg_need_set_dest = TRUE;                                        
                                        PM_DBG_INFO("F[%5d] destination info [type:%d] [ID:%d] \n", ESNODE_SINK_INFO(pt_esdb_node).e_type, ESNODE_SINK_INFO(pt_esdb_node).ui2_id, ESNODE_PID(pt_esdb_node));
                                        break;
                                    case PM_DETECTION:
                                        pv_set_info = NULL;
                                        z_set_info_len = 0; 
                                        fg_need_set_dest = FALSE;
                                        break;
                                    default:
                                        PM_DBG_ERROR("F[%5d] Failed handle filter allocation for unsupported type\n", ESNODE_PID(pt_esdb_node));
                                        /* ToDo */
                                        break;
                                }
                            
                                if ((FALSE == fg_need_set_dest)||
                                    (TRUE == fg_need_set_dest && 
                                     rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0 , DEMUX_SET_TYPE_DEST_ID, 
                                           pv_set_info, z_set_info_len, &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK))
                                {
                                    ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED;
                                
                                #if PM_ENABLE_SCRAMBLE_DETECTION
                                    
                                    ESNODE_DMX_SCRAMBLE_TAG(pt_esdb_node) = pt_esdb_node;
                                    ESNODE_DMX_SCRAMBLE_NFY(pt_esdb_node) = pmi_scramble_status_nfy_fct;
                                     
                                #if (PM_ENABLE_FILTER_CTRL)      
                                
                                    t_ctrl_scramble_nfy.pv_tag = ESNODE_FILTER_OBJ_CTRL(pt_esdb_node);
                                    t_ctrl_scramble_nfy.pf_demux_nfy = pmi_scramble_status_nfy_central_ctrl_fct;
                                    
                                    if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0 , DEMUX_SET_TYPE_SCRS_NFY_FCT, 
                                               &t_ctrl_scramble_nfy, sizeof(DEMUX_NFY_INFO_T), &(ESNODE_FILTER_COND(pt_esdb_node))) != RMR_OK)                             
                                #else     
                                
                                    
                                    if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0 , DEMUX_SET_TYPE_SCRS_NFY_FCT, 
                                               &(ESNODE_DMX_SCRAMBLE_INFO(pt_esdb_node)), sizeof(DEMUX_NFY_INFO_T), &(ESNODE_FILTER_COND(pt_esdb_node))) != RMR_OK)
                                #endif               
                                    {
                                        PM_DBG_ERROR("F[%5d] Failed to set scramble status notify \n", ESNODE_PID(pt_esdb_node));
                                        /* ToDo */
                                    }
                                    else
                                    {
                                        i4_return = PMR_OK;  
                                    }
                                #else
                                    i4_return = PMR_OK;                                                 
                                #endif            
                                }
                                else
                                {
                                    PM_DBG_ERROR("F[%5d] Failed to set sink \n", ESNODE_PID(pt_esdb_node));
                                    /* ToDo */
                                }                                          
                            }
                            else
                            {
                                PM_DBG_ERROR("F[%5d] Failed to set PID \n", ESNODE_PID(pt_esdb_node));
                                /* ToDo */
                            }                                    

                            break;
                    #if (PM_ALLOCATE_PCR)
                        case DRVT_DEMUX_TS_PCR:
                            
                            if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0 , DEMUX_SET_TYPE_PID, 
                                   ESNODE_PID(pt_esdb_node), sizeof(MPEG_2_PID_T), &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK)
                            {
                                ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED;
                                i4_return = PMR_OK;                                              
                            }
                            else
                            {
                                PM_DBG_ERROR("F[%5d] Failed to set PID \n", ESNODE_PID(pt_esdb_node));
                            }
                            break;
                    #endif /* PM_ALLOCATE_PCR */
                    
                        default:
                            PM_DBG_ERROR("F[%5d] unsupported type:%d  for filter allocation \n", t_filter_type, ESNODE_PID(pt_esdb_node)); 
                            break;
                    }
                    
                #if (PM_ENABLE_FILTER_CTRL) 
                    pmi_filter_db_ctrl_info_update(pt_esdb_node, 
                                                   (PM_FILTER_INFO_UPDATE_PID | PM_FILTER_INFO_UPDATE_DEST_ID | PM_FILTER_INFO_UPDATE_STATUS));                                            
                #endif                    
                    
                    break;
                default:
                    break;
            }                        

        }
        else
        {                    
        #if (PM_ENABLE_FILTER_CTRL)     
             
            if (b_filter_check_failed == FALSE)
            {                   
                /* Update filter related data directly */  
                ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED;
                
            #if PM_ENABLE_SCRAMBLE_DETECTION                
                ESNODE_DMX_SCRAMBLE_TAG(pt_esdb_node) = pt_esdb_node;
                ESNODE_DMX_SCRAMBLE_NFY(pt_esdb_node) = pmi_scramble_status_nfy_fct;                                                          
            #endif                            
                i4_return = PMR_OK;                  
                
            }
        #endif              
            
        }
    }                    
    else
    {
         PM_DBG_ERROR("F[%5d] Failed to get source  return value:%d\n", ESNODE_PID(pt_esdb_node), i4_rt_val);
        /* ToDo */
    }     
    
    if (i4_return == PMR_OK)
    {
         PM_DBG_INFO("F[%5d] Allocated\n", ESNODE_PID(pt_esdb_node));
        /* ToDo */                           
    }
    
    return (i4_return);
} 

#if (PM_ENABLE_MULTIMEDIA)  
#if (PM_ENABLE_RECORD)
/*-----------------------------------------------------------------------------
 * Name: pmi_filter_alloc_recording
 *
 * Description: This API allocates filter based on the filter type for 
 *              recording mode. 
 *
 * Inputs:  pt_svdb_node     Contains the reference to SVDB node.
 *          t_filter_type    Contains teh filter type
 *          pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     The referenced ESDB node will contains the handle 
 *                           of allocated filter.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     Filter module is not initialized.
 *          PMR_INV_ARG      Invalid arguments.
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/   
INT32 pmi_filter_alloc_recording(PM_SERVICE_INFO_T*   pt_svdb_node, 
                                 DRV_TYPE_T           t_filter_type,                     
                                 PM_COMPONENT_INFO_T* pt_esdb_node)
{
    CM_CTRL_RM_SRC_COMP_T  t_ctrl_rm_src_comp; 
    const VOID*  pv_set_info = NULL;
    SIZE_T       z_set_info_len = 0;    
    INT32        i4_return;
    INT32        i4_rt_val;
    RM_COND_T    t_cond;
    
    i4_return = PMR_FAILED;   
    ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_WAIT_OPEN;
    
    /* 1. Get Source component handle */
    i4_rt_val = x_cm_get( SVNODE_CONNECTION_HANDLE(pt_svdb_node), 
                           CM_CTRL_RM_SRC_COMP,
                           (VOID*) & t_ctrl_rm_src_comp);

    if (i4_rt_val == CMR_OK)
    {
        /* 2. Open Filter */
        i4_rt_val = rm_open_comp( t_ctrl_rm_src_comp.h_rm_comp, 
                                  OBJ_COMP_REL_SRC, 
                                  t_filter_type, 
                                  ANY_COMP_ID,
                                  NULL, 
                                  CTRL_TYPE_SINGLE, 
                                  NULL, 
                                  pm_comp_sel_first_unused_one, 
                                  pt_esdb_node, 
                                  pmi_filter_nfy_fct,
                                  &(ESNODE_FILTER_HANDLE(pt_esdb_node)),
                                  &(ESNODE_FILTER_COND(pt_esdb_node)));                
                                 
        if ((i4_rt_val == RMR_OK) || (i4_rt_val == RMR_ARBITRATION))
        {   
            t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
            
            switch (t_cond)
            {
                case RM_COND_OPENING:
                    i4_return = PMR_OK;                              
                    break;
                    
                case RM_COND_OPENED:                            
                    /* 3. Connect Filter */
                #if (!PM_AUTO_CONNECT_BY_RM) 
                    if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_CONNECT) == TRUE) &&
                        (pmi_filter_connect(pt_svdb_node, pt_esdb_node) == PMR_OK))
                    {                                        
                        t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                        switch (t_cond)
                        {
                            case RM_COND_CONNECTED:  
                                ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                break;                                                
                            case RM_COND_CONNECTING:
                                ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                break;
                            default:
                                PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                break;
                        }
                    }
                    else
                    {
                        PM_DBG_ERROR("F[%5d] connect FAIL \n", ESNODE_PID(pt_esdb_node));
                    }
                #endif /* (!PM_AUTO_CONNECT_BY_RM) */
                
                    /* 4. Set PID/Stream ID and destination ID by filter type */
                    switch (t_filter_type)
                    {
                        case DRVT_DEMUX_REC:
                            if (rm_set( ESNODE_FILTER_HANDLE(pt_esdb_node), 
                                        FALSE, 
                                        0,
                                        RECDEMUX_SET_TYPE_PID,
                                        (const void *)((UINT32)(ESNODE_PID(pt_esdb_node))),
                                        sizeof(MPEG_2_PID_T),
                                        &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK)
                            {

                                /* set stream type mask to record demux */
                                if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), 
                                        FALSE, 
                                        0, 
                                        RECDEMUX_SET_TYPE_STREAM_TYPE_MASK, 
                                        (const void *)((UINT32)(ESNODE_REC_STREAM_TYPE_MASK(pt_esdb_node))), 
                                        sizeof(UINT32), 
                                        &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK)
                                {
                                    i4_return = PMR_OK;
                                }
                                else
                                {
                                    PM_DBG_ERROR("F[%5d] Set stream type\n", ESNODE_PID(pt_esdb_node));
                                }
                             
                                pv_set_info    = &(ESNODE_REC_SINK_INFO(pt_esdb_node));
                                z_set_info_len = sizeof(RECDEMUX_DEST_INFO_T);
        
                                if (rm_set( ESNODE_FILTER_HANDLE(pt_esdb_node), 
                                            FALSE, 
                                            0, 
                                            RECDEMUX_SET_TYPE_DEST_ID,
                                            pv_set_info, 
                                            z_set_info_len, 
                                            &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK)
                                {
                                    #if PM_ENABLE_SCRAMBLE_DETECTION
                                        RECDEMUX_NFY_INFO_T     t_recdmx_scramble_info;

                                        t_recdmx_scramble_info.pf_recdemux_nfy = pmi_recdemux_scramble_status_nfy_fct;
                                        t_recdmx_scramble_info.pv_tag          = pt_esdb_node;
                                        
                                        if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0 , RECDEMUX_SET_TYPE_SCRS_NFY_FCT, 
                                                   &t_recdmx_scramble_info, sizeof(RECDEMUX_NFY_INFO_T), &(ESNODE_FILTER_COND(pt_esdb_node))) != RMR_OK)
                                        {
                                            PM_DBG_ERROR("F[%5d] Failed to set scramble status notify \n", ESNODE_PID(pt_esdb_node));    
                                            /* ToDo */
                                        }
                                        else
                                        {
                                            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED;
                                            i4_return = PMR_OK;
                                        }
                                    #else
                                        ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED;
                                        i4_return = PMR_OK;
                                    #endif
                                }
                                else
                                {
                                    PM_DBG_ERROR("F[%5d] Failed to set sink \n", ESNODE_PID(pt_esdb_node));
                                    /* ToDo */
                                }
                            }
                            else
                            {
                                PM_DBG_ERROR("F[%5d] Failed to set PID \n", ESNODE_PID(pt_esdb_node));
                                /* ToDo */
                            }
                        	break;
                        	
                        case DRVT_DEMUX_REC_PCR:
                            if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), 
                            	  FALSE, 
                            	  0, 
                            	  RECDEMUX_SET_TYPE_PID, 
                                  (const void *)((UINT32)(ESNODE_PID(pt_esdb_node))), 
                                  sizeof(MPEG_2_PID_T), 
                                  &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK)
                            {
                                ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED;
                                i4_return = PMR_OK;                                              
                               
                            }
                            else
                            {
                                PM_DBG_ERROR("F[%5d] Failed to set PID \n", ESNODE_PID(pt_esdb_node));
                            }
                            break;
                            
                        default:
                            PM_DBG_ERROR("F[%5d] unsupported type:%d  for filter allocation \n", t_filter_type, ESNODE_PID(pt_esdb_node)); 
                            break;
                    }
                    break;
                    
                default:
                    PM_DBG_ERROR( "opened filter condition is exceptional\n" );
                    break;
            }                        
        }
        else
        {
            PM_DBG_ERROR( "open filter FAIL\n" );
        }
    }
    else
    {
        PM_DBG_ERROR( "get source component handle FAIL\n" );
    }                    
    
    return (i4_return);
} 
#endif


/*-----------------------------------------------------------------------------
 * Name: pmi_filter_alloc_multimedia
 *
 * Description: This API allocates filter based on the filter type for multimedia. 
 *
 * Inputs:  pt_svdb_node     Contains the reference to SVDB node.
 *          t_filter_type      Contains teh filter type
 *          pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     The referenced ESDB node will contains the handle 
 *                           of allocated filter.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     Filter module is not initialized.
 *          PMR_INV_ARG      Invalid arguments.
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/   
INT32 pmi_filter_alloc_multimedia(PM_SERVICE_INFO_T*   pt_svdb_node, 
                                  DRV_TYPE_T           t_filter_type,                     
                                  PM_COMPONENT_INFO_T* pt_esdb_node)
{
    CM_CTRL_RM_SRC_COMP_T  t_ctrl_rm_src_comp; 
    INT32        i4_return;
    INT32        i4_rt_val;
    RM_COND_T    t_cond;  
    PM_SERVICE_INFO_T*  pt_svdb1_node = NULL;  
    const VOID*  pv_set_info = NULL; 
    SIZE_T       z_set_info_len = 0;    
            
    i4_return = PMR_FAILED;   
    ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_WAIT_OPEN;
    
    /* 1. Get Source component handle */
    i4_rt_val = x_cm_get(SVNODE_CONNECTION_HANDLE(pt_svdb_node), 
                       CM_CTRL_RM_SRC_COMP,
                       (VOID*) & t_ctrl_rm_src_comp);

    if (i4_rt_val == CMR_OK)
    {
        /* 2. Open Filter */
        i4_rt_val = rm_open_comp(t_ctrl_rm_src_comp.h_rm_comp, OBJ_COMP_REL_SRC, t_filter_type, ANY_COMP_ID, 
                         NULL, CTRL_TYPE_SHARED, NULL,  pm_comp_sel_first_unused_one, pt_esdb_node, pmi_filter_nfy_fct, 
                         &(ESNODE_FILTER_HANDLE(pt_esdb_node)), &(ESNODE_FILTER_COND(pt_esdb_node)));                
        if ((i4_rt_val == RMR_OK) || (i4_rt_val == RMR_ARBITRATION))
        {   
            t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
            
            switch (t_cond)
            {
                case RM_COND_OPENING:
                    i4_return = PMR_OK;                              
                    break;
                    
                case RM_COND_OPENED:                            
                    /* 3. Connect Filter */
                #if (!PM_AUTO_CONNECT_BY_RM) 
                    if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_CONNECT) == TRUE) &&
                        (pmi_filter_connect(pt_svdb_node, pt_esdb_node) == PMR_OK))
                    {                                        
                        t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                        switch (t_cond)
                        {
                            case RM_COND_CONNECTED:  
                                ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                break;                                                
                            case RM_COND_CONNECTING:
                                ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                break;
                            default:
                                PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                break;
                        }
                    }
                    else
                    {
                        PM_DBG_ERROR("F[%5d] connect FAIL \n", ESNODE_PID(pt_esdb_node));
                    }
                #endif /* (!PM_AUTO_CONNECT_BY_RM) */
                    /* 4. Set PID/Stream ID and destination ID by filter type */
                    switch (t_filter_type)
                    {
                        case DRVT_DEMUX_PS_PES_PACKET:
                        case DRVT_DEMUX_CONTAINER_ES:
                        case DRVT_DEMUX_CONTAINER_ES_MEMORY:    
                                                               
                            if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0 , DEMUX_SET_TYPE_STREAM_ID, 
                                       (const void *)(&(ESNODE_MMP_DESCR_SETTING(pt_esdb_node))), sizeof(STREAM_ID_T), &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK)
                            {
                                if (t_filter_type != DRVT_DEMUX_CONTAINER_ES_MEMORY)
                                {
                                    
                                    switch (SVNODE_PROC_TYPE(pt_svdb_node))
                                    {
                                        case PM_MULTIMEDIA:
                                            pv_set_info = &(ESNODE_SINK_INFO(pt_esdb_node));
                                            z_set_info_len = sizeof(DEMUX_DEST_INFO_T); 
                                            break;                                                
                                        default:
                                            PM_DBG_ERROR("Failed handle filter allocation for unsupported type\n", ESNODE_PID(pt_esdb_node));
                                            /* ToDo */
                                            break;
                                    }                                                                             
                                
                                    if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0 , DEMUX_SET_TYPE_DEST_ID, 
                                               pv_set_info, z_set_info_len, &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK)
                                    {
                                        
                                        #if PM_ENABLE_SCRAMBLE_DETECTION
                                            DEMUX_NFY_INFO_T        t_dmx_scramble_info;

                                            t_dmx_scramble_info.pf_demux_nfy = pmi_scramble_status_nfy_fct;
                                            t_dmx_scramble_info.pv_tag       = pt_esdb_node;
                                            
                                            if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0 , DEMUX_SET_TYPE_SCRS_NFY_FCT, 
                                                       &t_dmx_scramble_info, sizeof(DEMUX_NFY_INFO_T), &(ESNODE_FILTER_COND(pt_esdb_node))) != RMR_OK)
                                            {
                                                PM_DBG_ERROR("F[%5d] Failed to set scramble status notify \n", ESNODE_PID(pt_esdb_node));    
                                                /* ToDo */
                                            }
                                            else
                                            {
                                                ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED;
                                                i4_return = PMR_OK;
                                            }
                                        #else
                                            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED;                                                                                
                                            i4_return = PMR_OK;  
                                        #endif 
                                        
                                    }
                                    else
                                    {
                                        PM_DBG_ERROR("F[%5d] Failed to set sink \n", ESNODE_PID(pt_esdb_node));
                                        /* ToDo */
                                    }

                                    if ((t_filter_type == DRVT_DEMUX_CONTAINER_ES)&&
                                        (((MM_CRYPT_TYPE_PVR == ESNODE_MMP_CRYPT_TYPE(pt_esdb_node)) && 
                                          (MM_CRYPT_MODE_INVALID != ESNODE_MMP_CRYPT_PVR_MODE(pt_esdb_node))) ||
                                         ((MM_CRYPT_TYPE_MLN == ESNODE_MMP_CRYPT_TYPE(pt_esdb_node)) && 
                                          (0 != ESNODE_MMP_CRYPT_ML_CNT(pt_esdb_node)))))
                                    {
                                        switch (SVNODE_PROC_TYPE(pt_svdb_node))
                                        {
                                            case PM_MULTIMEDIA:
                                                pv_set_info = &(ESNODE_MMP_DESC_CRYPTO_INFO(pt_esdb_node));
                                                z_set_info_len = sizeof(MM_DESC_CRYPT_INFO_T); 
                                                break;                                                
                                            default:
                                                PM_DBG_ERROR("Failed handle filter allocation for unsupported type\n", ESNODE_PID(pt_esdb_node));
                                                /* ToDo */
                                                break;
                                        } 
                                        
                                        if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0 , DEMUX_SET_TYPE_DECRYPT_INFO, 
                                                   pv_set_info, z_set_info_len, &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK)
                                        {
                                            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED;                                                                                
                                            i4_return = PMR_OK;  
                                        }
                                        else
                                        {
                                            PM_DBG_ERROR("F[%5d] Failed to set decrypt info\n", ESNODE_PID(pt_esdb_node));
                                            /* ToDo */
                                        }
                                    }
                                }
                                else
                                {
                                    i4_return = PMR_OK;
                                }
                            }
                            else
                            {
                                PM_DBG_ERROR("F[%5d] Failed to set stream ID for ES stream_id \n", ESNODE_PID(pt_esdb_node));
                                /* ToDo */
                            }   
                            break;
                            
                        case DRVT_DEMUX_CONTROL: 
                            /* Set media format */                                   
                            pmi_db_get_service_from_index(&pt_svdb1_node, ESNODE_SERVICE_INDEX(pt_esdb_node));
                            if (SVNODE_MMP_MEDIA_ES_WITH_PTS(pt_svdb_node))
                            {
                                if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0 , DEMUX_SET_TYPE_ES_WITH_PTS_IN, 
                                           (const void *)(&(SVNODE_MMP_MEDIA_ES_WITH_PTS(pt_svdb_node))), sizeof(BOOL), &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK)
                                {
                                    ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED;
                                    i4_return = PMR_OK;                                                                                                                                             
                                }
                            }
                            else
                            {
                                if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0 , DEMUX_SET_TYPE_PRIORITY, 
                                           (const void *)(&(SVNODE_MMP_PRIORITY(pt_svdb_node))), sizeof(MM_PRIORITY_TYPE_T), &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK)
                                {
                                    i4_return = PMR_OK;                                                                                                                                             
                                }
                                else
                                {
                                }

                                if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0 , DEMUX_SET_TYPE_MEDIA_FORMAT, 
                                           (const void *)(&(SVNODE_MMP_MEDIA_FORMAT(pt_svdb_node))), sizeof(MEDIA_FORMAT_T), &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK)
                                {
                                    ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED;
                                    i4_return = PMR_OK;                                                                                                                                             
                                }
                                else
                                {
                                    PM_DBG_ERROR("F[%5d] Failed to set stream ID for ES stream_id \n", ESNODE_PID(pt_esdb_node));
                                    /* ToDo */
                                }
                            }
                            break;
                    
                        default:
                            PM_DBG_ERROR("F[%5d] unsupported type:%d  for filter allocation \n", t_filter_type, ESNODE_PID(pt_esdb_node)); 
                            break;
                    }
                    break;
                default:
                    break;
            }                        
        }
        else
        {                    
            PM_DBG_ERROR("F[%5d] Failed to open resource  return value:%d\n", ESNODE_PID(pt_esdb_node), i4_rt_val);
            /* ToDo */
        }
    }
    else
    {
        PM_DBG_ERROR("F[%5d] Failed to get source  return value:%d\n", ESNODE_PID(pt_esdb_node), i4_rt_val);
        /* ToDo */
    }
    
    if (i4_return == PMR_OK)
    {
        PM_DBG_INFO("F[%5d] Allocated\n", ESNODE_PID(pt_esdb_node));
        /* ToDo */                           
    }
    
    return (i4_return);
} 

#endif

/*-----------------------------------------------------------------------------
 * Name: pmi_filter_alloc
 *
 * Description: This API allocates filter based on the filter type. 
 *
 * Inputs:  pt_svdb_node     Contains the reference to SVDB node.
 *          t_filter_type      Contains teh filter type
 *          pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     The referenced ESDB node will contains the handle 
 *                           of allocated filter.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     Filter module is not initialized.
 *          PMR_INV_ARG      Invalid arguments.
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/   
INT32 pmi_filter_alloc(PM_SERVICE_INFO_T*   pt_svdb_node, 
                       DRV_TYPE_T           t_filter_type,                     
                       PM_COMPONENT_INFO_T* pt_esdb_node)
{
    
    INT32        i4_return;
            
    i4_return = PMR_NOT_INIT;    
    
    if (b_filter_init)
    {
        i4_return = PMR_INV_ARG;
                        
        if ((pt_esdb_node != NULL)&&(pt_svdb_node != NULL))
        {  

            switch (t_filter_type)
            {
                case DRVT_DEMUX_TS_PES_PACKET:
            #if (PM_ALLOCATE_PCR)
                case DRVT_DEMUX_TS_PCR:                    
            #endif /* PM_ALLOCATE_PCR */
                    return pmi_filter_alloc_present(pt_svdb_node, t_filter_type, pt_esdb_node);                    

            #if (PM_ENABLE_MULTIMEDIA)
            #if (PM_ENABLE_RECORD)
                case DRVT_DEMUX_REC_PCR:
                case DRVT_DEMUX_REC:
                    return pmi_filter_alloc_recording(pt_svdb_node, t_filter_type, pt_esdb_node);                    
            #endif
            
                case DRVT_DEMUX_PS_PES_PACKET:
                case DRVT_DEMUX_CONTAINER_ES:
                case DRVT_DEMUX_CONTAINER_ES_MEMORY:    
                case DRVT_DEMUX_CONTROL:
                    return pmi_filter_alloc_multimedia(pt_svdb_node, t_filter_type, pt_esdb_node);
            #endif /* (PM_ENABLE_MULTIMEDIA) */
                default:
                    /* ToDo */
                    PM_DBG_ERROR( "it is a UNKNOWN type filter\n" );
                    break;
            }                        
        }   
        else
        {
            PM_DBG_ERROR( "pt_esdb_node is NULL or pt_svdb_node is NULL\n" );
        }
    }       
    
    if (i4_return == PMR_OK)
    {
        /* ToDo */                           
        PM_DBG_ERROR( "allocate filter FAIL\n" );
    }    
    return (i4_return);
} 

/*-----------------------------------------------------------------------------
 * Name: pmi_filter_connect
 *
 * Description: This API requests RM to connect the filter to the tuner. 
 *
 * Inputs:  pt_svdb_node     Contains the reference to SVDB node.
 *          pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     The referenced ESDB node will contains the updated
 *                           status for this issue.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     Filter module is not initialized.
 *          PMR_INV_ARG      Invalid arguments.
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/ 
INT32 pmi_filter_connect(PM_SERVICE_INFO_T* pt_svdb_node,PM_COMPONENT_INFO_T* pt_esdb_node)
{
    CM_CTRL_RM_SRC_COMP_T  t_ctrl_rm_src_comp; 
    INT32       i4_return;
    INT32       i4_rt_val;
    RM_COND_T   t_cond;
              
    i4_return = PMR_NOT_INIT;    
    
    if (b_filter_init)
    {
        i4_return = PMR_INV_ARG;
        if ((pt_esdb_node != NULL)&&(pt_svdb_node != NULL))
        {  
            i4_return = PMR_FAILED;   
            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_WAIT_CONNECT;
                        
            if (pmi_execute_request_now(pt_esdb_node, PM_ES_STATE_WAIT_CONNECT) == TRUE)
            {                 
                i4_rt_val = x_cm_get(SVNODE_CONNECTION_HANDLE(pt_svdb_node), 
                                       CM_CTRL_RM_SRC_COMP,
                                       (VOID*) & t_ctrl_rm_src_comp);
 
                if (i4_rt_val == CMR_OK)
                {                     
                    
                    i4_rt_val = rm_connect(ESNODE_FILTER_HANDLE(pt_esdb_node), RM_CONN_TYPE_HANDLE, 
                                           (const void *)(t_ctrl_rm_src_comp.h_rm_comp), sizeof(HANDLE_T), 
                                           pt_esdb_node,
                                           &(ESNODE_FILTER_COND(pt_esdb_node)));
                                           
                    if ((i4_rt_val == RMR_OK) || (i4_rt_val == RMR_ASYNC_NFY))
                    {   
                        t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);
                        switch (t_cond)
                        {
                            case RM_COND_CONNECTED:
                                ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_CONNECTED;    
                                i4_return = PMR_OK;                     
                                break;
                            case RM_COND_CONNECTING:
                                i4_return = PMR_OK;     
                                break;
                            default:
                                PM_DBG_ERROR("F[%5d] Invalid connection status:%d \n", t_cond, ESNODE_PID(pt_esdb_node));
                                PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                break;
                        }                          
                    } 
                    else
                    {
                        PM_DBG_ERROR("F[%5d] Failed to connect to source \n", ESNODE_PID(pt_esdb_node));
                    }        
                }                 
                else
                {
                    PM_DBG_ERROR( "get filter source component FAIL\n" );
                }   
            }
            else
            {
                PM_DBG_ERROR( "can NOT excute because of the filter condition\n" );
            }
        }   
        else
        {
            PM_DBG_ERROR( "pt_esdb_node is NULL or pt_svdb_node is NULL\n" );
        }
    }    
    return (i4_return);    
}    

/*-----------------------------------------------------------------------------
 * Name: pmi_filter_disconnect
 *
 * Description: This API requests RM to disconnect the filter to the tuner. 
 *
 * Inputs:  pt_svdb_node     Contains the reference to SVDB node.
 *          pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     The referenced ESDB node will contains the updated
 *                           status for this issue.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     Filter module is not initialized.
 *          PMR_INV_ARG      Invalid arguments.
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/ 
INT32 pmi_filter_disconnect(PM_SERVICE_INFO_T* pt_svdb_node,PM_COMPONENT_INFO_T* pt_esdb_node)
{
    INT32       i4_return;
    INT32       i4_rm_ret;    
    RM_COND_T   t_cond; 
        
    i4_return = PMR_NOT_INIT;    
    
    if (b_filter_init)
    {
        i4_return = PMR_INV_ARG;
        if (pt_esdb_node != NULL)
        {              
            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_WAIT_DISCONNECT;
                        
            if (pmi_execute_request_now(pt_esdb_node, PM_ES_STATE_WAIT_DISCONNECT) == TRUE)
            {                
                i4_return = PMR_FAILED;   
                
                i4_rm_ret = rm_disconnect(ESNODE_FILTER_HANDLE(pt_esdb_node), RM_DISC_TYPE_IGNORE, NULL, 0, 
                                          &(ESNODE_FILTER_COND(pt_esdb_node)));      
                               
                if (i4_rm_ret == RMR_OK)
                {   
                    t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);
                    switch (t_cond)
                    {
                        case RM_COND_DISCONNECTED:
                            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_DISCONNECTED;    
                            i4_return = PMR_OK;                                 
                            break;               
                                         
                        default:
                            PM_DBG_ERROR("F[%5d] Invalid disconnection status:%d \n", t_cond, ESNODE_PID(pt_esdb_node));
                            PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_DISCONNECTION_COND);
                            break;
                    }  
                }                         
                else
                {
                    PM_DBG_ERROR("F[%5d] Failed to disconnect to source  ret:%d\n", ESNODE_PID(pt_esdb_node), i4_rm_ret);
                }
            }   
            else
            {
                PM_DBG_ERROR( "can NOT excute because of the filter condition\n" );
            }
        }
        else
        {
            PM_DBG_ERROR( "pt_esdb_node is NULL\n" );
        }
    }    
    return (i4_return);    
}    

/*-----------------------------------------------------------------------------
 * Name: pmi_filter_disable
 *
 * Description: This API requests RM to disable the filter. 
 *
 * Inputs:  pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     The referenced ESDB node will contains the updated
 *                           status for this issue.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     Filter module is not initialized.
 *          PMR_INV_ARG      Invalid arguments.
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/
INT32 pmi_filter_disable(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    BOOL        b_temp=FALSE;
    INT32       i4_rm_ret;    
    INT32       i4_return;    
#if (PM_ENABLE_FILTER_CTRL)    
    BOOL         b_execute_detail = FALSE;    
#endif    
#if (PM_ENABLE_MULTIMEDIA)
    MM_CRYPT_CMD_T t_cypty_cmd;
#endif

    i4_return = PMR_NOT_INIT;
    
    if (b_filter_init)
    {
        i4_return = PMR_INV_ARG;        
                        
        if (pt_esdb_node != NULL)
        {  
            i4_return = PMR_FAILED; 
            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_WAIT_STOP;
            
            if (pmi_execute_request_now(pt_esdb_node, PM_ES_STATE_WAIT_STOP) == TRUE)
            {                   
                PM_DBG_INFO("F[%5d] [type:%d] begin to disable ...\n", ESNODE_PID(pt_esdb_node), ESNODE_FILTER_TYPE(pt_esdb_node));
                switch (ESNODE_FILTER_TYPE(pt_esdb_node))
                {
                    case DRVT_DEMUX_TS_PES_PACKET:
                #if (PM_ALLOCATE_PCR)
                    case DRVT_DEMUX_TS_PCR:                    
                #endif /* PM_ALLOCATE_PCR */
                    #if (PM_ENABLE_FILTER_CTRL) 
                        pmi_filter_db_execute_stop(pt_esdb_node, &b_execute_detail);
                        if (b_execute_detail == TRUE)
                        {
                            i4_rm_ret = rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0, DEMUX_SET_TYPE_CTRL, 
                                           (const void *)((UINT32)b_temp), sizeof(BOOL), &(ESNODE_FILTER_COND(pt_esdb_node))) ;        
                            if (i4_rm_ret == RMR_OK)
                            {                    
                                ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_STOPPED;   
                                pmi_filter_db_ctrl_info_update(pt_esdb_node, PM_FILTER_INFO_UPDATE_STATUS);         
                                i4_return = PMR_OK;     
                                
                                PM_DBG_INFO("F[%5d] Disabled\n", ESNODE_PID(pt_esdb_node));
                            }        
                            else
                            {
                                PM_DBG_ERROR("F[%5d] Failed to disable filter  ret:%d\n", ESNODE_PID(pt_esdb_node), i4_rm_ret);
                            }      
                                                     
                        }
                        else
                        {
                            /* Update the status directly */
                            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_STOPPED;   
                            i4_return = PMR_OK; 
                        }
                    #else   
                        i4_rm_ret = rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0, DEMUX_SET_TYPE_CTRL, 
                                           (const void *)((UINT32)b_temp), sizeof(BOOL), &(ESNODE_FILTER_COND(pt_esdb_node))) ;        
                        if (i4_rm_ret == RMR_OK)
                        {                    
                            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_STOPPED;            
                            i4_return = PMR_OK;     
                            
                            PM_DBG_INFO("F[%5d] Disabled\n", ESNODE_PID(pt_esdb_node));            
                        }        
                        else
                        {
                            PM_DBG_ERROR("F[%5d] Failed to disable filter  ret:%d\n", ESNODE_PID(pt_esdb_node), i4_rm_ret);
                        }      
                    #endif
                                            
                        break;
                        
                #if (PM_ENABLE_MULTIMEDIA)
                    case DRVT_DEMUX_REC:
                    case DRVT_DEMUX_REC_PCR:
                        i4_rm_ret = rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0, RECDEMUX_SET_TYPE_CTRL, 
                                           (const void *)((UINT32)b_temp), sizeof(BOOL), &(ESNODE_FILTER_COND(pt_esdb_node))) ;        
                        if (i4_rm_ret == RMR_OK)
                        {                    
                            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_STOPPED;            
                            i4_return = PMR_OK;     
                            
                            PM_DBG_INFO("F[%5d] Disabled\n", ESNODE_PID(pt_esdb_node));            
                        }        
                        else
                        {
                            PM_DBG_ERROR("F[%5d] Failed to disable filter  ret:%d\n", ESNODE_PID(pt_esdb_node), i4_rm_ret);
                        }            
                        break;
                        
                    case DRVT_DEMUX_PS_PES_PACKET:
                    case DRVT_DEMUX_CONTAINER_ES:
                    case DRVT_DEMUX_CONTAINER_ES_MEMORY: 
                        i4_rm_ret = rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0, DEMUX_SET_TYPE_CTRL, 
                                           (const void *)((UINT32)b_temp), sizeof(BOOL), &(ESNODE_FILTER_COND(pt_esdb_node))) ;        
                        if (i4_rm_ret == RMR_OK)
                        {                    
                            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_STOPPED;            
                            i4_return = PMR_OK;     
                            
                            PM_DBG_INFO("F[%5d] Disabled\n", ESNODE_PID(pt_esdb_node));            
                        }        
                        else
                        {
                            PM_DBG_ERROR("F[%5d] Failed to disable filter  ret:%d\n", ESNODE_PID(pt_esdb_node), i4_rm_ret);
                        }            
                        break;
                        
                    case DRVT_DEMUX_CONTROL:
                        i4_rm_ret = rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0, DEMUX_SET_TYPE_CTRL, 
                                           (const void *)((UINT32)b_temp), sizeof(BOOL), &(ESNODE_FILTER_COND(pt_esdb_node))) ;        
                        if (i4_rm_ret == RMR_OK)
                        {                    
                            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_STOPPED;            
                            i4_return = PMR_OK;     
                            
                            PM_DBG_INFO("F[%5d] Disabled\n", ESNODE_PID(pt_esdb_node));            
                        }        
                        else
                        {
                            PM_DBG_ERROR("F[%5d] Failed to disable filter  ret:%d\n", ESNODE_PID(pt_esdb_node), i4_rm_ret);
                        }            

                        /* Reset the crypto information in demux */                        
                        {
                            t_cypty_cmd.t_crypt_ctrl_cmd = MM_CRYPT_CTRL_CMD_RESET_ALL;
                            t_cypty_cmd.pt_info = NULL;
                            i4_rm_ret = rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0, DEMUX_SET_TYPE_CRYPT_CMD, 
                                               &t_cypty_cmd, sizeof(MM_CRYPT_CMD_T), &(ESNODE_FILTER_COND(pt_esdb_node)));
                            if (i4_rm_ret == RMR_OK)
                            {
                                ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_STOPPED;
                                i4_return = PMR_OK;
                                
                                PM_DBG_INFO( "Reset crypto info\n" );
                            }        
                            else
                            {
                                PM_DBG_ERROR( "Failed to reset crypto info\n" );
                            }
                        }                   
                        break;
                        
                #endif /* (PM_ENABLE_MULTIMEDIA) */
                    default:
                    /* ToDo */
                        break;
                }
            }
            else
            {
                PM_DBG_ERROR( "can NOT excute because of the filter condition\n" );
            }
        }
        else
        {
            PM_DBG_ERROR( "pt_esdb_node is NULL\n" );
        }
    }    
    return (i4_return);    
} 

/*-----------------------------------------------------------------------------
 * Name: pmi_filter_enable
 *
 * Description: This API requests RM to enable the filter. 
 *
 * Inputs:  pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     The referenced ESDB node will contains the updated
 *                           status for this issue.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     Filter module is not initialized.
 *          PMR_INV_ARG      Invalid arguments.
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/
INT32 pmi_filter_enable(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    BOOL        b_temp=TRUE;
    INT32       i4_rm_ret;        
    INT32       i4_return;        
#if (PM_ENABLE_FILTER_CTRL)    
    BOOL         b_execute_detail = FALSE;    
#endif 
    
    i4_return = PMR_NOT_INIT;
    
    if (b_filter_init)
    {
        i4_return = PMR_INV_ARG;        
                        
        if (pt_esdb_node != NULL)
        {  
            i4_return = PMR_FAILED;   
            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_WAIT_PLAY;
           
            if (pmi_execute_request_now(pt_esdb_node, PM_ES_STATE_WAIT_PLAY) == TRUE)
            {
                PM_DBG_INFO("F[%5d] [type:%d] begin to enable ...\n", ESNODE_PID(pt_esdb_node), ESNODE_FILTER_TYPE(pt_esdb_node));
                switch (ESNODE_FILTER_TYPE(pt_esdb_node))
                {
                    case DRVT_DEMUX_TS_PES_PACKET:
                #if (PM_ALLOCATE_PCR)
                    case DRVT_DEMUX_TS_PCR:                    
                #endif /* PM_ALLOCATE_PCR */
                    #if (PM_ENABLE_FILTER_CTRL) 
                        pmi_filter_db_execute_play(pt_esdb_node, &b_execute_detail);
                        if (b_execute_detail == TRUE)
                        {
                            i4_rm_ret = rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0, DEMUX_SET_TYPE_CTRL, 
                                           (const void *)((UINT32)b_temp), sizeof(BOOL), &(ESNODE_FILTER_COND(pt_esdb_node)));
                        
                            if ( i4_rm_ret == RMR_OK)
                            {                    
                                ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_PLAYED;    
                                pmi_filter_db_ctrl_info_update(pt_esdb_node, PM_FILTER_INFO_UPDATE_STATUS);
                                        
                                i4_return = PMR_OK;     
                                
                                PM_DBG_INFO("F[%5d] Enabled\n", ESNODE_PID(pt_esdb_node));             
                            }                    
                            else
                            {                    
                                PM_DBG_ERROR("F[%5d] Failed to enable filter  ret:%d\n", ESNODE_PID(pt_esdb_node), i4_rm_ret);
                            }                                   
                        }
                        else
                        {
                            /* Update the status directly */
                            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_PLAYED;   
                            i4_return = PMR_OK; 
                        }
                    #else    
                        i4_rm_ret = rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0, DEMUX_SET_TYPE_CTRL, 
                                           (const void *)((UINT32)b_temp), sizeof(BOOL), &(ESNODE_FILTER_COND(pt_esdb_node)));
                        
                        if ( i4_rm_ret == RMR_OK)
                        {                    
                            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_PLAYED;            
                            i4_return = PMR_OK;     
                            
                            PM_DBG_INFO("F[%5d] Enabled\n", ESNODE_PID(pt_esdb_node));             
                        }                    
                        else
                        {                    
                            PM_DBG_ERROR("F[%5d] Failed to enable filter  ret:%d\n", ESNODE_PID(pt_esdb_node), i4_rm_ret);
                        }                    
                    #endif
                                            
                        break;
                        
                #if (PM_ENABLE_MULTIMEDIA)
                    case DRVT_DEMUX_REC:
                    case DRVT_DEMUX_REC_PCR:
                        i4_rm_ret = rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0, RECDEMUX_SET_TYPE_CTRL, 
                                           (const void *)((UINT32)b_temp), sizeof(BOOL), &(ESNODE_FILTER_COND(pt_esdb_node)));
                        
                        if ( i4_rm_ret == RMR_OK)
                        {                    
                            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_PLAYED;            
                            i4_return = PMR_OK;     
                            
                            PM_DBG_INFO("F[%5d] Enabled\n", ESNODE_PID(pt_esdb_node));             
                        }                    
                        else
                        {                    
                            PM_DBG_ERROR("F[%5d] Failed to enable filter  ret:%d\n", ESNODE_PID(pt_esdb_node), i4_rm_ret);
                        }                       
                        break;

                    case DRVT_DEMUX_PS_PES_PACKET:
                    case DRVT_DEMUX_CONTAINER_ES:
                    case DRVT_DEMUX_CONTAINER_ES_MEMORY: 
                    case DRVT_DEMUX_CONTROL:
                        i4_rm_ret = rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0, DEMUX_SET_TYPE_CTRL, 
                                           (const void *)((UINT32)b_temp), sizeof(BOOL), &(ESNODE_FILTER_COND(pt_esdb_node)));
                        
                        if ( i4_rm_ret == RMR_OK)
                        {                    
                            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_PLAYED;            
                            i4_return = PMR_OK;     
                            
                            PM_DBG_INFO("F[%5d] Enabled\n", ESNODE_PID(pt_esdb_node));             
                        }                    
                        else
                        {                    
                            PM_DBG_ERROR("F[%5d] Failed to enable filter  ret:%d\n", ESNODE_PID(pt_esdb_node), i4_rm_ret);
                        }                       
                        break;
                #endif /* (PM_ENABLE_MULTIMEDIA) */
                    default:
                    /* ToDo */
                        break;
                }
            }
            else
            {
                PM_DBG_ERROR( "can NOT excute because of the filter condition\n" );
            }
        }
        else
        {
            PM_DBG_ERROR( "pt_esdb_node is NULL\n" );
        }                
    }    
    return (i4_return);                
} 
/*-----------------------------------------------------------------------------
 * Name: pmi_filter_update
 *
 * Description: This API requests RM to update the filter. 
 *
 * Inputs:  pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     The referenced ESDB node will contains the updated
 *                           status for this issue.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     Filter module is not initialized.
 *          PMR_INV_ARG      Invalid arguments.
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/
INT32 pmi_filter_update(PM_COMPONENT_INFO_T* pt_esdb_node)
{    
    INT32              i4_return;    
    PM_ES_STATE_T      e_original_state;
    
#if (PM_ENABLE_FILTER_CTRL)        
    PM_SERVICE_INFO_T* pt_svdb_node;    
#endif    

    i4_return = PMR_NOT_INIT;
    
    if (b_filter_init)
    {
        i4_return = PMR_INV_ARG;        
                        
        if (pt_esdb_node != NULL)
        {  
            i4_return = PMR_FAILED; 
            
            e_original_state = ESNODE_STATE(pt_esdb_node);

            PM_DBG_INFO("F begin to update ...\n", ESNODE_PID(pt_esdb_node));
            
            if ((ESNODE_STATE(pt_esdb_node) == PM_ES_STATE_PLAYED) ||
                (ESNODE_STATE(pt_esdb_node) == PM_ES_STATE_STOPPED)) 
            {   
                if (e_original_state == PM_ES_STATE_PLAYED) 
                {
                    if (pmi_filter_disable(pt_esdb_node)!= PMR_OK)
                        return i4_return;                        
                        
                #if (PM_ENABLE_FILTER_CTRL)     
                    ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_STOPPED;  
                #endif
                }
                
                switch (ESNODE_PROC_TYPE(pt_esdb_node))
                {
                    case PM_PRESENT:
                    case PM_DETECTION:                    
                    #if (PM_ENABLE_FILTER_CTRL)    
                        /* Remove it */
                        pmi_filter_free(pt_esdb_node); 
                        ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_CLOSED;   
                        
                        /* Allocate a new one */                                        
                        pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_esdb_node));                            
                        pmi_filter_alloc(pt_svdb_node, DRVT_DEMUX_TS_PES_PACKET, pt_esdb_node);  
                       
                    #else                                                    
                        if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), 
                                   FALSE, 
                                   0, 
                                   DEMUX_SET_TYPE_PID,
                                   (const void *)((UINT32)(ESNODE_PID(pt_esdb_node))),
                                   sizeof(MPEG_2_PID_T), 
                                   &(ESNODE_FILTER_COND(pt_esdb_node))) != RMR_OK)
                        {                            
                            PM_DBG_ERROR("F[%5d] Failed to set filter PID \n", ESNODE_PID(pt_esdb_node));
                            return i4_return;
                        }
                        else
                        {                            
                            PM_DBG_INFO("F[%5d] Updated\n", ESNODE_PID(pt_esdb_node));                                 
                        }                    
                    #endif                        
                        break;

                #if (PM_ENABLE_MULTIMEDIA)                 
                    #if (PM_ENABLE_RECORD)
                    case PM_RECORDING:
                        if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), 
                                   FALSE, 
                                   0, 
                                   RECDEMUX_SET_TYPE_PID,
                                   (const void *)((UINT32)(ESNODE_PID(pt_esdb_node))),
                                   sizeof(MPEG_2_PID_T), 
                                   &(ESNODE_FILTER_COND(pt_esdb_node))) != RMR_OK)
                        {                            
                            PM_DBG_ERROR("F[%5d] Failed to set filter PID \n", ESNODE_PID(pt_esdb_node));
                            return i4_return;
                        }
                        else
                        {                            
                            PM_DBG_INFO("F[%5d] Updated\n", ESNODE_PID(pt_esdb_node));                                 
                        }
                        break;
                    #endif
                        
                    case PM_MULTIMEDIA:                        
                        if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), 
                                   FALSE, 
                                   0, 
                                   DEMUX_SET_TYPE_STREAM_ID, 
                                   (const void *)(&(ESNODE_MMP_DESCR_SETTING(pt_esdb_node))), 
                                   sizeof(STREAM_ID_T), 
                                   &(ESNODE_FILTER_COND(pt_esdb_node))) != RMR_OK)
                        {                            
                            PM_DBG_ERROR("F[%5d] Failed to set filter PID \n", ESNODE_PID(pt_esdb_node));
                            return i4_return;
                        }
                        else
                        {
                            /* PM_DBG_INFO("F[%5d] Updated\n", ESNODE_PID(pt_esdb_node)); */                                
                        }                    
                        break;
                #endif                        
                    default:
                        break;
                }
                
                
                    
                if (e_original_state == PM_ES_STATE_PLAYED)
                {
                    if (pmi_filter_enable(pt_esdb_node)!= PMR_OK)
                        return i4_return;
                        
                #if (PM_ENABLE_FILTER_CTRL)     
                    ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_PLAYED;  
                #endif                        
                
                }
                
                i4_return = PMR_OK; 
            }
            else
            {
                PM_DBG_ERROR( "can NOT excute because of the filter condition\n" );
            }
        }
        else
        {
            PM_DBG_ERROR( "pt_esdb_node is NULL\n" );
        }            
    }    
    return (i4_return);                
} 

/*-----------------------------------------------------------------------------
 * Name: pmi_filter_free_present
 *
 * Description: This API requests RM to free the filter for present mode. 
 *
 * Inputs:  pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     The referenced ESDB node will contains the updated
 *                           status for this issue.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     Filter module is not initialized.
 *          PMR_INV_ARG      Invalid arguments. 
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/
INT32 pmi_filter_free_present(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    PM_SERVICE_INFO_T* pt_svdb_node;
    RM_COND_T          t_cond;
    BOOL               b_do_disconnect = FALSE;
    BOOL               b_execute_free_detail = FALSE;   
    INT32              i4_return;
        
    i4_return = PMR_NOT_INIT;    
    
    if (b_filter_init)
    {
        i4_return = PMR_INV_ARG;        
                        
        if (pt_esdb_node != NULL)
        {  
            i4_return = PMR_FAILED;  
            
            /* Disconnect first */
            pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_esdb_node));
            if (pt_svdb_node != NULL)
            {            
            #if (!PM_AUTO_CONNECT_BY_RM) 
                switch (SVNODE_PROC_TYPE(pt_svdb_node))
                {
                    case PM_PRESENT:
                    case PM_DETECTION:
                        if ((ESNODE_FILTER_HANDLE(pt_esdb_node)!= NULL_HANDLE)&&(ESNODE_REQUEST_TYPE(pt_esdb_node)!=PM_PRESENT_ES_FILTER_ALLOCATED))
                            b_do_disconnect = TRUE;
                        break; 
                    default:
                        b_do_disconnect = FALSE;
                        break;
                }
                
                if (b_do_disconnect == TRUE)
                {   
                #if (PM_ENABLE_FILTER_CTRL) 
                    b_execute_free_detail = FALSE;
                #else
                    b_execute_free_detail = TRUE;
                #endif
                    if (b_execute_free_detail == TRUE)
                    {                         
                        if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_DISCONNECT) == TRUE) &&
                            (pmi_filter_disconnect(pt_svdb_node, pt_esdb_node) == PMR_OK))
                        {                                        
                            t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), RM_COND_IGNORE, COND_GRP_CONN_STATE);                                        
                            switch (t_cond)
                            {
                                case RM_COND_DISCONNECTED:  
                                    break;                                                
                                default:
                                    PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_DISCONNECTION_COND);
                                    return PMR_FAILED;                  
                            }
                        }
                    }
                    else
                    {
                    #if (PM_ENABLE_FILTER_CTRL)     
                        /* Update filter related data directly */  
                        ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_DISCONNECTED; 
                    #endif
                    }
                }
            #endif /* (!PM_AUTO_CONNECT_BY_RM) */             
            }     
            
            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_WAIT_CLOSE;
            if ((pmi_execute_request_now(pt_esdb_node, PM_ES_STATE_WAIT_CLOSE) == TRUE) &&
                (rm_close(ESNODE_FILTER_HANDLE(pt_esdb_node), TRUE, &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK))
            {   
                switch (rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE))
                {
                    case RM_COND_CLOSING:
                        /* ToDo ...*/                        
                        break;
                    case RM_COND_CLOSED:
                        ESNODE_FILTER_HANDLE(pt_esdb_node) = NULL_HANDLE;
                        ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_CLOSED;
                        
                        switch (ESNODE_FILTER_TYPE(pt_esdb_node))
                        {
                            case DRVT_DEMUX_TS_PES_PACKET:
                            case DRVT_UNKNOWN:
                            #if (PM_ENABLE_FILTER_CTRL)
                                pmi_filter_db_remove(pt_esdb_node);
                            #endif
                                break;
                            default:
                                break;
                        }                        
                        
                        i4_return = PMR_OK;
                        
                        PM_DBG_INFO("F[%5d] Free\n", ESNODE_PID(pt_esdb_node));             
                        break;
                    default:
                        break;
                }                      
            }                     
        }            
    }    
    return (i4_return);   
} 

#if (PM_ENABLE_MULTIMEDIA)  
#if (PM_ENABLE_RECORD)
/*-----------------------------------------------------------------------------
 * Name: pmi_filter_free_recording
 *
 * Description: This API requests RM to free the filter for recording mode. 
 *
 * Inputs:  pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     The referenced ESDB node will contains the updated
 *                           status for this issue.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     Filter module is not initialized.
 *          PMR_INV_ARG      Invalid arguments. 
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/
INT32 pmi_filter_free_recording(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    PM_SERVICE_INFO_T* pt_svdb_node;
    RM_COND_T          t_cond;
    INT32              i4_return;
        
    i4_return = PMR_NOT_INIT;    
    
    if (b_filter_init)
    {
        i4_return = PMR_INV_ARG;        
                        
        if (pt_esdb_node != NULL)
        {  
            i4_return = PMR_FAILED;  
            
            /* Disconnect first */
            pmi_db_get_service_from_index( &pt_svdb_node, ESNODE_SERVICE_INDEX(pt_esdb_node) );
            if (pt_svdb_node != NULL)
            {            
            #if (!PM_AUTO_CONNECT_BY_RM) 
                if ((ESNODE_FILTER_HANDLE(pt_esdb_node)   != NULL_HANDLE) && 
                    (ESNODE_REC_REQUEST_TYPE(pt_esdb_node)!= PM_RECORDING_ES_FILTER_ALLOCATED))
                {   
                    if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_DISCONNECT) == TRUE) &&
                        (pmi_filter_disconnect(pt_svdb_node, pt_esdb_node) == PMR_OK))
                    {                                        
                        t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), RM_COND_IGNORE, COND_GRP_CONN_STATE);
                        switch (t_cond)
                        {
                            case RM_COND_DISCONNECTED:  
                                break;                                                
                            default:
                                PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_DISCONNECTION_COND);
                                return PMR_FAILED;                  
                        }
                    }
                }
            #endif /* (!PM_AUTO_CONNECT_BY_RM) */             
            }     
            
            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_WAIT_CLOSE;
            if ((pmi_execute_request_now(pt_esdb_node, PM_ES_STATE_WAIT_CLOSE) == TRUE) &&
                (rm_close(ESNODE_FILTER_HANDLE(pt_esdb_node), TRUE, &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK))
            {   
                switch (rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE))
                {
                    case RM_COND_CLOSING:
                        /* ToDo ...*/                        
                        break;
                    case RM_COND_CLOSED:
                        ESNODE_FILTER_HANDLE(pt_esdb_node) = NULL_HANDLE;
                        ESNODE_STATE(pt_esdb_node)         = PM_ES_STATE_CLOSED;
                        i4_return = PMR_OK;
                        
                        PM_DBG_INFO("F[%5d] Free\n", ESNODE_PID(pt_esdb_node));             
                        break;
                    default:
                        break;
                }                      
            }                     
            else
            {
                PM_DBG_ERROR( "can NOT excute because of the filter condition\n" );
            }
        }
        else
        {
            PM_DBG_ERROR( "pt_esdb_node is NULL\n" );
        }            
    }    
    return (i4_return);   
}
#endif  /* (PM_ENABLE_RECORD) */


/*-----------------------------------------------------------------------------
 * Name: pmi_filter_free_multimedia
 *
 * Description: This API requests RM to free the filter for multimedia mode. 
 *
 * Inputs:  pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     The referenced ESDB node will contains the updated
 *                           status for this issue.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     Filter module is not initialized.
 *          PMR_INV_ARG      Invalid arguments. 
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/
INT32 pmi_filter_free_multimedia(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    PM_SERVICE_INFO_T* pt_svdb_node;
    RM_COND_T          t_cond;
    BOOL               b_do_disconnect = FALSE;  
    INT32              i4_return;
        
    i4_return = PMR_NOT_INIT;    
    
    if (b_filter_init)
    {
        i4_return = PMR_INV_ARG;        
                        
        if (pt_esdb_node != NULL)
        {  
            i4_return = PMR_FAILED;  
            
            /* Disconnect first */
            pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_esdb_node));
            if (pt_svdb_node != NULL)
            {            
            #if (!PM_AUTO_CONNECT_BY_RM) 
                switch (SVNODE_PROC_TYPE(pt_svdb_node))
                {        
                    case PM_MULTIMEDIA:
                        if ((ESNODE_FILTER_HANDLE(pt_esdb_node) != NULL_HANDLE)&&
                            (ESNODE_MMP_REQUEST_TYPE(pt_esdb_node) != PM_MM_ES_FILTER_ALLOCATED))
                        {
                            b_do_disconnect = TRUE;
                        }
                        break;
                    default:
                        b_do_disconnect = FALSE;
                        break;
                }
                
                if (b_do_disconnect == TRUE)
                {   
                    if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_DISCONNECT) == TRUE) &&
                        (pmi_filter_disconnect(pt_svdb_node, pt_esdb_node) == PMR_OK))
                    {                                        
                        t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), RM_COND_IGNORE, COND_GRP_CONN_STATE);                                        
                        switch (t_cond)
                        {
                            case RM_COND_DISCONNECTED:  
                                break;                                                
                            default:
                                PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_DISCONNECTION_COND);
                                return PMR_FAILED;                  
                        }
                    }
                }
            #endif /* (!PM_AUTO_CONNECT_BY_RM) */             
            }     
            
            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_WAIT_CLOSE;
            if ((pmi_execute_request_now(pt_esdb_node, PM_ES_STATE_WAIT_CLOSE) == TRUE) &&
                (rm_close(ESNODE_FILTER_HANDLE(pt_esdb_node), TRUE, &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK))
            {   
                switch (rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE))
                {
                    case RM_COND_CLOSING:
                        /* ToDo ...*/                        
                        break;
                    case RM_COND_CLOSED:
                        ESNODE_FILTER_HANDLE(pt_esdb_node) = NULL_HANDLE;
                        ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_CLOSED;
                        
                        i4_return = PMR_OK;
                        
                        PM_DBG_INFO("F[%5d] Free\n", ESNODE_PID(pt_esdb_node));             
                        break;
                    default:
                        break;
                }                      
            }                     
        }            
    }    
    return (i4_return);   
} 

#endif
/*-----------------------------------------------------------------------------
 * Name: pmi_filter_free
 *
 * Description: This API requests RM to free the filter. 
 *
 * Inputs:  pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     The referenced ESDB node will contains the updated
 *                           status for this issue.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     Filter module is not initialized.
 *          PMR_INV_ARG      Invalid arguments. 
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/
INT32 pmi_filter_free(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    PM_SERVICE_INFO_T* pt_svdb_node;
    INT32        i4_return;
            
    i4_return = PMR_NOT_INIT;    
    
    if (b_filter_init)
    {
        i4_return = PMR_INV_ARG;
        
        if (pt_esdb_node == NULL)
        {
            return i4_return;
        }
        
        pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_esdb_node));
                        
        if ((pt_esdb_node != NULL)&&(pt_svdb_node != NULL))
        {  
            switch (SVNODE_PROC_TYPE(pt_svdb_node))
            {
                case PM_PRESENT:
                case PM_DETECTION:
                    return pmi_filter_free_present(pt_esdb_node);   
                    
            #if (PM_ENABLE_MULTIMEDIA)         
                #if (PM_ENABLE_RECORD)
                case PM_RECORDING:
                    return pmi_filter_free_recording(pt_esdb_node);                    
                #endif
                     
                case PM_MULTIMEDIA:
                    return pmi_filter_free_multimedia(pt_esdb_node);
            #endif        
            
                default:                   
                    break;
            }
        }   
        else
        {
            PM_DBG_ERROR( "pt_esdb_node is NULL or pt_svdb_node is NULL\n" );
        }
    }          
 
    return (i4_return);
} 

#if (PM_ENABLE_MULTIMEDIA)
/*-----------------------------------------------------------------------------
 * Name: pmi_filter_set
 *
 * Description: This API requests RM to set the operation for filter. 
 *
 * Inputs:  pt_esdb_node     Contains the reference to ESDB node.
 *          e_set_type       Contains the set type.
 *          pt_info          Contains the parameter for set operation. 
 *
 * Outputs: pt_esdb_node     The referenced ESDB node will contains the updated
 *                           status for this issue.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     Filter module is not initialized.
 *          PMR_INV_ARG      Invalid arguments. 
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/
INT32 pmi_filter_set(PM_COMPONENT_INFO_T* pt_esdb_node, PM_SET_TYPE_T e_set_type, VOID* pt_info)
{
    DRV_SET_TYPE_T t_drv_set_type;
    INT32          i4_rm_ret;        
    INT32          i4_return;   
    SIZE_T         t_info_size;  
    
    i4_return = PMR_NOT_INIT;
    
    if (b_filter_init)
    {
        i4_return = PMR_INV_ARG;        
                        
        if (pt_esdb_node != NULL)
        {  
            switch (e_set_type)
            {
                case PM_SET_ADD_RANGE:
                    t_drv_set_type = DEMUX_SET_TYPE_ADD_RANGE;
                    t_info_size = sizeof(MM_RANGE_INFO_T);
                    break;
                case PM_SET_DEL_RANGE:
                    t_drv_set_type = DEMUX_SET_TYPE_DEL_RANGE;                    
                    t_info_size = sizeof(UINT32);
                    break;
                case PM_SET_SPEED:
                    t_drv_set_type = DEMUX_SET_TYPE_SPEED;
                    t_info_size = sizeof(MM_PLAY_MODE_PL_T);
                    break;
                case PM_SET_AB_POS:
                    t_drv_set_type = DEMUX_SET_TYPE_AB_POSITION;
                    t_info_size = sizeof(MM_AB_POS_T);
                    break;
                case PM_SET_3D_PROPERTY:
                    t_drv_set_type = DEMUX_SET_TYPE_3D_PROPERTY;
                    t_info_size = sizeof(MM_3D_PROPERTY_T);
                    break;
                case PM_SET_SEEK:
                    t_drv_set_type = DEMUX_SET_TYPE_SEEK_POSTION;
                    t_info_size = sizeof(MM_RANGE_POS_INFO_T);
                    break;
                case PM_SET_BLOCK_PSI:
                    t_drv_set_type = DEMUX_SET_TYPE_BLOCK_PSI;
                    t_info_size = sizeof(BOOL);
                    break;
                case PM_SET_STC_ID:
                    t_drv_set_type = DEMUX_SET_TYPE_STC_ID;
                    t_info_size = sizeof(UINT32);
                    break;
                case PM_SET_FREE_RUN:
                    t_drv_set_type = DEMUX_SET_TYPE_FREE_RUN;
                    t_info_size = sizeof(BOOL);
                    break;
                    
                case PM_SET_REC_BUF_RANGE:
                    t_drv_set_type = DEMUX_SET_TYPE_REC_BUF_RANGE;                    
                    t_info_size = sizeof(MM_REC_BUF_RANGE_INFO_T);
                    break;
                 case PM_SET_HDMVC_INFO:
                    t_drv_set_type=DEMUX_SET_TYPE_HDMVC_INFO;
                    t_info_size=sizeof(UINT32);
                    break;
                #if (PM_ENABLE_TIME_SHIFT)
                case PM_SET_MONITOR_LBA:
                    t_drv_set_type = DEMUX_SET_TYPE_MONITOR_LBA;
                    t_info_size = sizeof(UINT32);                    
                    break;
                #endif
                #if PM_ENABLE_MM_AUTOTEST
                case PM_SET_AUTOTEST:
                    t_drv_set_type=DEMUX_SET_TYPE_MM_AUTOTEST;
                    t_info_size=sizeof(MM_AUTOTEST_INFO_T);
                    break;
                #endif
                default:
                    PM_DBG_ERROR( "this is an UNKNOWN command\n" );
                    return PMR_NOT_SURPPORT;        
            }
            
            i4_return = PMR_FAILED;                      
                
            i4_rm_ret = rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0, t_drv_set_type, 
                               (const void *)(pt_info), t_info_size, &(ESNODE_FILTER_COND(pt_esdb_node)));
            
            if ( i4_rm_ret == RMR_OK)
            {                         
                i4_return = PMR_OK;               
            }                                
            else
            {
                PM_DBG_ERROR( "set command to RM FAIL\n" );
            }
        }                
    }    
    return (i4_return); 
}

/*-----------------------------------------------------------------------------
 * Name: pmi_filter_get
 *
 * Description: This API requests RM to get the operation for filter. 
 *
 * Inputs:  pt_esdb_node     Contains the reference to ESDB node.
 *          e_get_type       Contains the get type.
 *          pt_info          Contains the parameter for get operation. 
 *          pz_get_info_len: Contains the length of get info 
 *
 * Outputs: pt_esdb_node     The referenced ESDB node will contains the updated
 *                           status for this issue.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     Filter module is not initialized.
 *          PMR_INV_ARG      Invalid arguments. 
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/
INT32 pmi_filter_get(PM_COMPONENT_INFO_T* pt_esdb_node, 
                     PM_GET_TYPE_T e_get_type, 
                     VOID* pt_info, 
                     SIZE_T* pz_get_info_len)
{
    DRV_GET_TYPE_T t_drv_get_type;
    INT32          i4_rm_ret;        
    INT32          i4_return;
    RM_COND_T      e_cond;
    
    i4_return = PMR_NOT_INIT;

    if ( !pt_info || !pz_get_info_len )
    {
        return PMR_INV_ARG;    
    }  
    
    if (b_filter_init)
    {
        i4_return = PMR_INV_ARG;        
                        
        if (pt_esdb_node != NULL)
        {  
            switch (e_get_type)
            {
                case PM_GET_BLOCK_PSI:
                    t_drv_get_type = DEMUX_GET_TYPE_BLOCK_PSI;
                    *pz_get_info_len = sizeof(UINT32);
                    break;
                    
                case PM_GET_UOP_CAP:
                    t_drv_get_type = DEMUX_GET_TYPE_UOP_CAPABILITY;
                    *pz_get_info_len = sizeof(DEMUX_UOP_CAP_INFO_T);
                    break;
                    
                case PM_GET_POST_INFO:
                    t_drv_get_type = DEMUX_GET_TYPE_POST_INFO;
                    *pz_get_info_len = sizeof(DEMUX_POST_INFO_T);
                    break;
                    
                case PM_GET_STC_OFFSET:
                    t_drv_get_type = DEMUX_GET_TYPE_STC_OFFSET;
                    *pz_get_info_len = sizeof(UINT32);
                    break;
                    
                #if (PM_ENABLE_TIME_SHIFT)
                case PM_GET_CURR_LBA:
                    t_drv_get_type = DEMUX_GET_TYPE_CURR_LBA;
                    *pz_get_info_len = sizeof(UINT32);
                    break;

                case PM_GET_CURR_TICK_NUM:
                    t_drv_get_type = DEMUX_GET_TYPE_CURR_TICK_NUM;
                    *pz_get_info_len = sizeof(UINT32);
                    break;
                #endif
                    
                default:
                    PM_DBG_ERROR( "this is an UNKNOWN command\n" );
                    return PMR_NOT_SURPPORT;        
            }
            
            i4_return = PMR_FAILED;                      
                
            i4_rm_ret = rm_get(ESNODE_FILTER_HANDLE(pt_esdb_node),
                               FALSE, 
                               0,
                               t_drv_get_type,
                               pt_info,
                               pz_get_info_len, 
                               &e_cond);
            
            if ( i4_rm_ret == RMR_OK)
            {   
                i4_return = PMR_OK;               
            }                                
            else
            {
                PM_DBG_ERROR( "set command to RM FAIL\n" );
            }
        }                
    }    
    return (i4_return); 
}

#endif



