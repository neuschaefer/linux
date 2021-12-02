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
 * $RCSfile: prc_desc.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/1 $
 * $SWAuthor: Felisa Hsiao $
 * $MD5HEX: a32bc9ba0937014a76796df58bfce8fd $
 *
 * Description: 
 *         This file is the implementation of  CRYPTO_CTRL module.
 *
 * History:
 *           
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "prc_mngr/prc_mngr.h"
#include "prc_mngr/prc_common.h"

#if PM_ENABLE_CA
#include "prc_mngr/prc_filter.h"
#include "prc_mngr/prc_lock.h"
#include "prc_mngr/prc_util.h"
#include "prc_mngr/prc_debug.h"
#include "prc_mngr/prc_thread.h"
#include "prc_mngr/prc_ca.h"
#include "prc_mngr/prc_desc.h"

#include "conn_mngr/cm_conn_api.h"
#include "conn_mngr/cm_handler_api.h"

#if (PM_ENABLE_FILTER_CTRL)
#include "prc_mngr/prc_filter_db.h"
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
 
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/ 
/* Descrambler Handler */
static BOOL b_descrambler_init = FALSE;    
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static void pmi_descrambler_nfy_fct(HANDLE_T   h_obj,
                       UINT8      ui1_port,
                       RM_COND_T  e_nfy_cond,
                       VOID*      pv_tag,
                       UINT32     ui4_data);
                       
SELECT_T pmi_descrambler_comp_sel_first_unused_one(DRV_TYPE_T   e_type,
                                            UINT16       ui2_id,
                                            VOID*        pv_comp_sel_data,
                                            const VOID*  pv_comp_data,
                                            SIZE_T       z_comp_data_len,
                                            UINT16       ui2_num_active,
                                            UINT16       ui2_num_wait,
                                            CTRL_TYPE_T  e_ctrl,
                                            BOOL         b_would_conflict);

static INT32 pmi_descrambler_open(PM_COMPONENT_INFO_T* pt_esdb_node, CTRL_TYPE_T t_ctrl_type);
static INT32 pmi_descrambler_connect(PM_COMPONENT_INFO_T* pt_esdb_node);
static INT32 pmi_descrambler_disconnect(PM_COMPONENT_INFO_T* pt_esdb_node);
/*-----------------------------------------------------------------------------
 * Name: pmi_descrambler_nfy_fct
 *
 * Description: This API is responsible to handle the notification from RM for 
 *              HW descrambler component.
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
VOID pmi_descrambler_nfy_fct(HANDLE_T   h_obj,
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
        if ((ESNODE_CRYPTO_HANDLE(pt_esdb_node) == h_obj)&&(e_nfy_cond != RM_COND_NO_CHG))
        {
            e_last_cond = ESNODE_CRYPTO_COND(pt_esdb_node);
            ESNODE_CRYPTO_COND(pt_esdb_node) = e_nfy_cond;   
            
            /* check object state */
            t_cond = rm_cond_chg (e_nfy_cond, e_last_cond, COND_GRP_OBJ_STATE); 
            switch (t_cond)
            {            
                case RM_COND_CLOSED:
                    PM_DBG_INFO("D[%5d] Closed\n", ESNODE_PID(pt_esdb_node));
                    pmi_thread_desc_closed(h_obj, pt_esdb_node);
                    break;
                case RM_COND_OPENED:
                    PM_DBG_INFO("D[%5d] Opened\n", ESNODE_PID(pt_esdb_node));                    
                    pmi_thread_desc_opened(h_obj, pt_esdb_node);
                    break;                
                default:
                    /* ToDo:
                       Other state update */
                    PM_DBG_INFO("D[%5d] Cond:%d\n", t_cond, ESNODE_PID(pt_esdb_node));                    
                    break;
            }
        }
    } 
    else
    {
        PM_DBG_ERROR("Invalid input for descrambler notification\n");
    }       
}  

/*-----------------------------------------------------------------------------
 * Name: pmi_descrambler_comp_sel_first_unused_one
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
SELECT_T pmi_descrambler_comp_sel_first_unused_one(DRV_TYPE_T   e_type,
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
SELECT_T pmi_descrambler_comp_sel_specific_used_one(DRV_TYPE_T   e_type,
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


/*-----------------------------------------------------------------------------
 * Name: pmi_descrambler_init
 *
 * Description: This API is called to initialize the PM_CA module.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *          PMR_ALREADY_INIT This module is already initialized.
 *          PMR_FAILED       Failed due to other reason. 
 ----------------------------------------------------------------------------*/                      
INT32 pmi_descrambler_init(void)
{
    /* Do Nothing */
    if (!b_descrambler_init)
    {
        b_descrambler_init = TRUE;
        return PMR_OK;  
    }
    else
    {
        return PMR_ALREADY_INIT;
    }   
}

/*-----------------------------------------------------------------------------
 * Name: pmi_descrambler_connect
 *
 * Description: This API requests RM to connect the descrambler to the filter. 
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
INT32 pmi_descrambler_connect(PM_COMPONENT_INFO_T* pt_esdb_node)
{    
    INT32       i4_return;
    INT32       i4_rt_val;
    RM_COND_T   t_cond;
              
    i4_return = PMR_NOT_INIT;    
    
    if (b_descrambler_init)
    {
        i4_return = PMR_INV_ARG;
        if (pt_esdb_node != NULL)
        {  
            i4_return = PMR_FAILED;               
#if 0  /* Skip the following first, ToDo */
            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_WAIT_CONNECT; 
                        
            if (pmi_execute_request_now(pt_esdb_node, PM_ES_STATE_WAIT_CONNECT) == TRUE)
            {                
              
#endif                            
                i4_rt_val = rm_connect(ESNODE_CRYPTO_HANDLE(pt_esdb_node), 
                                       RM_CONN_TYPE_IGNORE, 
                                       NULL, 
                                       0, 
                                       pt_esdb_node,
                                       &(ESNODE_CRYPTO_COND(pt_esdb_node)));
                               
                if ((i4_rt_val == RMR_OK) || (i4_rt_val == RMR_ASYNC_NFY) || (i4_rt_val == RMR_HARD_WIRED))
                {   
                    t_cond = rm_cond_chg(ESNODE_CRYPTO_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);
                    switch (t_cond)
                    {
                        case RM_COND_CONNECTED:
                        case RM_COND_HARD_WIRED:
                            /* ToDo ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_CONNECTED;    */
                            i4_return = PMR_OK;                     
                            break;
                        case RM_COND_CONNECTING:
                            i4_return = PMR_OK;     
                            break;
                        default:
                            PM_DBG_ERROR("Invalid connection status:%d for ES pid descrambler :%d\n", t_cond, ESNODE_PID(pt_esdb_node));
                            PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                            break;
                    }                          
                } 
                else
                {
                    PM_DBG_ERROR("Failed to connect to source for ES pid descrambler:0x%x\n", ESNODE_PID(pt_esdb_node));
                }        
#if 0       /* Skip the following first, ToDo */              
                          
            }   
#endif            
        }
    }    
    return (i4_return);    
}    

/*-----------------------------------------------------------------------------
 * Name: pmi_descrambler_disconnect
 *
 * Description: This API requests RM to disconnect the descrambler to the filter. 
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
INT32 pmi_descrambler_disconnect(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    INT32       i4_return;
    INT32       i4_rm_ret;    
    RM_COND_T   t_cond; 
        
    i4_return = PMR_NOT_INIT;    
    
    if (b_descrambler_init)
    {
        i4_return = PMR_INV_ARG;
        if (pt_esdb_node != NULL)
        {              
            ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_WAIT_DISCONNECT;

#if 0  /* Skip the following first, ToDo */                        
            if (pmi_execute_request_now(pt_esdb_node, PM_ES_STATE_WAIT_DISCONNECT) == TRUE)
            {         
#endif                       
                i4_return = PMR_FAILED;  
                
                i4_rm_ret = rm_disconnect(ESNODE_CRYPTO_HANDLE(pt_esdb_node), 
                                          RM_DISC_TYPE_IGNORE, NULL, 0, 
                                          &(ESNODE_CRYPTO_COND(pt_esdb_node)));       
                               
                if ((i4_rm_ret == RMR_OK)||(i4_rm_ret == RMR_HARD_WIRED))
                {   
                    t_cond = rm_cond_chg(ESNODE_CRYPTO_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);
                    switch (t_cond)
                    {
                        case RM_COND_DISCONNECTED:
                        case RM_COND_HARD_WIRED:
                            /* ToDo ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_DISCONNECTED; */
                            i4_return = PMR_OK;                                 
                            break;               
                                         
                        default:
                            PM_DBG_ERROR("Invalid disconnection status:%d for ES pid descrambler :%d\n", t_cond, ESNODE_PID(pt_esdb_node));
                            PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_DISCONNECTION_COND);
                            break;
                    }  
                }                         
                else
                {
                    PM_DBG_ERROR("Failed to disconnect to source for ES pid descrambler :%d ret:%d\n", ESNODE_PID(pt_esdb_node), i4_rm_ret);
                }
#if 0  /* Skip the following first, ToDo */                   
            }   
#endif        
        }
    }    
    return (i4_return);    
}    


INT32 pmi_descrambler_open(PM_COMPONENT_INFO_T* pt_esdb_node, CTRL_TYPE_T t_ctrl_type)
{
    INT32               i4_rt_val;
    INT32               i4_return = PMR_FAILED;
    RM_COND_T           t_cond;    
    PM_DESC_SETTING_T   t_desc_setting;
    
    /* 1. Alloc descrambler */
    i4_rt_val = rm_open_comp(NULL_HANDLE,//ESNODE_FILTER_HANDLE(pt_esdb_node),
                             OBJ_COMP_REL_IGNORE, //OBJ_COMP_REL_SUPPORT,
                             DRVT_DESCRAMBLER, 
                             ANY_COMP_ID, 
                             NULL,
                             t_ctrl_type, 
                             NULL, 
                             pmi_descrambler_comp_sel_first_unused_one, 
                             pt_esdb_node, 
                             pmi_descrambler_nfy_fct, 
                             &(ESNODE_CRYPTO_HANDLE(pt_esdb_node)), 
                             &(ESNODE_CRYPTO_COND(pt_esdb_node)));
                             
    if ((i4_rt_val == RMR_OK) || (i4_rt_val == RMR_ARBITRATION))
    {   
        t_cond = rm_cond_chg(ESNODE_CRYPTO_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
        
        switch (t_cond)
        {
            case RM_COND_OPENING:
                i4_return = PMR_OK;                              
                break;
                
            case RM_COND_OPENED:   
                
                /* 2. Connect to PES filter */                 
                #if (!PM_AUTO_CONNECT_BY_RM) 
                #if 0 /* Skip the checking, ToDo */
                    if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_CONNECT) == TRUE) &&
                        (pmi_descrambler_connect(pt_esdb_node) == PMR_OK))
                #else
                    if (pmi_descrambler_connect(pt_esdb_node) == PMR_OK)
                #endif
                    {                                        
                        t_cond = rm_cond_chg(ESNODE_CRYPTO_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                        switch (t_cond)
                        {
                            case RM_COND_CONNECTED:  
                            case RM_COND_HARD_WIRED:
                                /* ToDo ESNODE_CRYPTO_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE; */
                                break;                                                
                            case RM_COND_CONNECTING:
                                /* ToDo ESNODE_CRYPTO_STATUS(pt_esdb_node) = PM_COND_OPENING; */
                                /* ToDo How to handle this case */
                                break;
                            default:
                                PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                break;
                        }
                    }                                    
                #endif /* (!PM_AUTO_CONNECT_BY_RM) */
                    
                /* Set algorithm */
                t_desc_setting.t_desc_set_cmd = CA_SET_ALG;
                t_desc_setting.u_desc_cmd_info.t_algorithm.t_desc_alg = CA_DESC_ALG_NONE;
                if (pmi_descrambler_set( pt_esdb_node, &t_desc_setting ) == PMR_OK)
                {   
                    /* 3. Set PID */
                    if (rm_set(ESNODE_CRYPTO_HANDLE(pt_esdb_node), 
                               FALSE, 
                               0, 
                               DESC_SET_TYPE_PID, 
                               (const void *)((UINT32)(ESNODE_PID(pt_esdb_node))),
                               sizeof(MPEG_2_PID_T), 
                               &(ESNODE_CRYPTO_COND(pt_esdb_node))) == RMR_OK)
                    {   
                        /* 4. Enable descrambler */
                        if (rm_set(ESNODE_CRYPTO_HANDLE(pt_esdb_node), 
                                   FALSE, 
                                   0, 
                                   DESC_SET_TYPE_CTRL, 
                                   (const void *)((UINT32)TRUE),
                                   sizeof(BOOL), 
                                   &(ESNODE_CRYPTO_COND(pt_esdb_node))) == RMR_OK)
                        {                                    
                            /* ToDo ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED; */
                            i4_return = PMR_OK;    
                            PM_DBG_INFO("D[%5d] Allocated\n", ESNODE_PID(pt_esdb_node));      
                        }
                        else
                        {
                            PM_DBG_ERROR("D[%5d] Failed to enable descrambler \n", ESNODE_PID(pt_esdb_node));
                            /* ToDo */
                        }                                          
                    }
                    else
                    {
                        PM_DBG_ERROR("D[%5d] Failed to enable descrambler \n", ESNODE_PID(pt_esdb_node));
                        /* ToDo */
                    }                                    
                }
                else
                {
                    PM_DBG_ERROR("D[%5d] Failed to set descrambler algorithm \n", ESNODE_PID(pt_esdb_node));
                    /* ToDo */
                }
                       
                break;
            default:
                break;
        }                        
    }
    else
    {                    
        PM_DBG_ERROR("D[%5d] Failed to open descrambler [ret:%d]\n", ESNODE_PID(pt_esdb_node), i4_rt_val);
    }

    return i4_return;
}


/* descrambler is attached to filter, as a part of filter */
INT32 pmi_descrambler_alloc(PM_COMPONENT_INFO_T* pt_esdb_node )
{     
    INT32               i4_return;    
    PM_SERVICE_INFO_T*  pt_svdb_node;  
    PMCA_SYSNODE_T*     pt_ca_ctrl;   
    BOOL                b_ret;
                
    i4_return = PMR_NOT_INIT;    

    if (b_descrambler_init)
    {
        i4_return = PMR_INV_ARG;
                        
        if (pt_esdb_node != NULL)
        {
            if (PM_CA_REQUIRE_MANDATORY == ESNODE_CA_REQ_LVL(pt_esdb_node))
            {
                i4_return = PMR_FAILED;   
                /* ToDo ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_WAIT_OPEN; */
                
                pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_esdb_node));
                
                if (pt_svdb_node == NULL)
                {
                    return i4_return; 
                }
                
                b_ret = pmi_ca_select_engine(SVNODE_CONNECTION_HANDLE(pt_svdb_node),
                                             0, 
                                             &pt_ca_ctrl); 
                                     
                ESNODE_CA_CTRL(pt_esdb_node) = (VOID *)pt_ca_ctrl;      
                
                if (b_ret == TRUE)
                {                                 
                    if (pt_ca_ctrl->b_alloc_desc)
                    {
                        /* 1. filter type is DRVT_DEMUX_TS_PES_PACKET */
                        /* 2. PM_ENABLE_FILTER_CTRL has been enabled */
                        /* Both 1 and 2 are TRUE, fg_center_ctrl will be TRUE */
                        /* Because filter center control only support DRVT_DEMUX_TS_PES_PACKET right now */
                        BOOL                            fg_center_ctrl;

                        #if (PM_ENABLE_FILTER_CTRL)
                        DRV_TYPE_T                      e_desc_type;
                        UINT16                          ui2_desc_id;
                        UINT16                          ui2_desc_counter;
                        
                        i4_return = pmi_filter_db_get_desc_info( pt_esdb_node, &e_desc_type, &ui2_desc_id, &ui2_desc_counter );
                        if (PMR_OK == i4_return)
                        {
                            fg_center_ctrl = TRUE;
                        }
                        else
                        {
                            fg_center_ctrl = FALSE;
                        }
                        #else
                        fg_center_ctrl = FALSE;
                        #endif
                        
                        if (fg_center_ctrl)
                        {
                            #if (PM_ENABLE_FILTER_CTRL)
                            if (0 == ui2_desc_id && 0 == ui2_desc_counter)
                            {
                                i4_return = pmi_descrambler_open( pt_esdb_node, CTRL_TYPE_SHARED );
                                if (i4_return == 0)
                                {
                                    DRV_TYPE_T  e_type;
                                    UINT16      ui2_id;
                                    UINT8       ui1_num_inp_ports;
                                    UINT8       ui1_num_out_ports;
                                    UINT32      ui4_comp_flags;

                                    if ((i4_return = rm_get_comp_info_from_handle(
                                                        ESNODE_CRYPTO_HANDLE(pt_esdb_node), 
                                                        &e_type, 
                                                        &ui2_id, 
                                                        &ui1_num_inp_ports, 
                                                        &ui1_num_out_ports, 
                                                        &ui4_comp_flags)) == RMR_OK)
                                    {
                                        ui2_desc_counter++;
                                        ui2_desc_id = ui2_id;
                                        pmi_filter_db_set_desc_info( pt_esdb_node, e_desc_type, ui2_desc_id, ui2_desc_counter );
                                    }
                                }
                                else
                                {
                                    PM_DBG_ERROR("Failed to pmi_descrambler_open\n");
                                }
                            }
                            else
                            {
                                i4_return = rm_open_comp(
                                                    NULL_HANDLE,//ESNODE_FILTER_HANDLE(pt_esdb_node),
                                                    OBJ_COMP_REL_IGNORE, //OBJ_COMP_REL_SUPPORT,
                                                    DRVT_DESCRAMBLER, 
                                                    ui2_desc_id, 
                                                    NULL,
                                                    CTRL_TYPE_SHARED, 
                                                    NULL, 
                                                    pmi_descrambler_comp_sel_specific_used_one, 
                                                    pt_esdb_node, 
                                                    pmi_descrambler_nfy_fct, 
                                                    &(ESNODE_CRYPTO_HANDLE(pt_esdb_node)), 
                                                    &(ESNODE_CRYPTO_COND(pt_esdb_node)));

                                if (i4_return >= 0)
                                {
                                    ui2_desc_counter++;
                                    pmi_filter_db_set_desc_info( pt_esdb_node, e_desc_type, ui2_desc_id, ui2_desc_counter );
                                }
                                else
                                {
                                    PM_DBG_ERROR("Failed to pmi_filter_db_set_desc_info\n");
                                }
                            }
                            #endif
                        }
                        else
                        {
                            i4_return = pmi_descrambler_open( pt_esdb_node, CTRL_TYPE_SINGLE );
                            if (i4_return < 0)
                            {
                                PM_DBG_ERROR("Failed to pmi_descrambler_open\n");
                            }
                        }
                    }
                    else
                    {
                        ESNODE_CRYPTO_HANDLE(pt_esdb_node) = NULL_HANDLE;
                        i4_return = PMR_OK;    
                    }
                }   
                else
                {
                    i4_return = PMR_OK;                 
                }
            }
            else
            {
                i4_return = PMR_OK;
            }
        }   
    }

    if (i4_return != PMR_OK)
    {
        PM_DBG_ERROR("Failed\n");
    }
    
    return (i4_return);    
}
/*-----------------------------------------------------------------------------
 * Name: pmi_descrambler_free
 *
 * Description: This API requests RM to free the descrambler. 
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
INT32 pmi_descrambler_free(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    INT32       i4_return;
    INT32       i4_rt_val;
    BOOL        fg_need_discon = FALSE; /* need to disconnect the descrambler */
        
    i4_return = PMR_NOT_INIT;    
    
    if (b_descrambler_init)
    {
        i4_return = PMR_INV_ARG;        
                        
        if (pt_esdb_node != NULL)
        {  
            i4_return = PMR_FAILED;  
            
            if (ESNODE_CRYPTO_HANDLE(pt_esdb_node) != NULL_HANDLE )
            {
                #if (PM_ENABLE_FILTER_CTRL)
                DRV_TYPE_T                      e_desc_type;
                UINT16                          ui2_desc_id;
                UINT16                          ui2_desc_counter;

                i4_rt_val = pmi_filter_db_get_desc_info( pt_esdb_node, &e_desc_type, &ui2_desc_id, &ui2_desc_counter );
                if (PMR_OK == i4_rt_val)
                {
                    if (1 == ui2_desc_counter)/* This is the last one */
                    {
                        fg_need_discon = TRUE;
                    }
                    else
                    {
                        fg_need_discon = FALSE;
                    }
                }
                else
                {
                    fg_need_discon = TRUE;
                }
                #else
                fg_need_discon = TRUE;
                #endif

                if (fg_need_discon)
                {
                    /* 1. Disable descrambler */
                    if (rm_set(ESNODE_CRYPTO_HANDLE(pt_esdb_node), 
                               FALSE, 
                               0, 
                               DESC_SET_TYPE_CTRL, 
                               (const void *)((UINT32)FALSE),
                               sizeof(BOOL), 
                               &(ESNODE_CRYPTO_COND(pt_esdb_node))) == RMR_OK)
                    {               
                        /* ToDo ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED; */
                        
                        /* 2. Disconnect descrambler */
                        if (pmi_descrambler_disconnect(pt_esdb_node) == PMR_OK)
                        {
                            /* 3. Close descrambler */
                            if (rm_close(ESNODE_CRYPTO_HANDLE(pt_esdb_node), TRUE, &(ESNODE_CRYPTO_COND(pt_esdb_node))) == RMR_OK)
                            {   
                                switch (rm_cond_chg(ESNODE_CRYPTO_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE))
                                {
                                    case RM_COND_CLOSING:
                                        /* ToDo ...*/                        
                                        break;
                                    case RM_COND_CLOSED:
                                        ESNODE_CRYPTO_HANDLE(pt_esdb_node) = NULL_HANDLE;
                                        /* ToDo ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_CLOSED; */
                                        i4_return = PMR_OK;
                                        
                                        PM_DBG_INFO("D[%5d] Free\n", ESNODE_PID(pt_esdb_node));             
                                        break;
                                    default:
                                        break;
                                }                      
                            }
                            else
                            {                        
                                PM_DBG_ERROR("D[%5d] Failed to close descrambler \n", ESNODE_PID(pt_esdb_node));
                                /* ToDo */
                            }   
                        }
                        else
                        {                        
                            PM_DBG_ERROR("D[%5d] Failed to disconnect descrambler \n", ESNODE_PID(pt_esdb_node));
                            /* ToDo */
                        }                  
                    }
                    else
                    {
                        PM_DBG_ERROR("D[%5d] Failed to disable descrambler \n", ESNODE_PID(pt_esdb_node));
                        /* ToDo */
                    }
                }
                else
                {
                    /* only close descrambler */
                    if (rm_close(ESNODE_CRYPTO_HANDLE(pt_esdb_node), TRUE, &(ESNODE_CRYPTO_COND(pt_esdb_node))) == RMR_OK)
                    {   
                        switch (rm_cond_chg(ESNODE_CRYPTO_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE))
                        {
                            case RM_COND_CLOSING:
                                /* ToDo ...*/                        
                                break;
                            case RM_COND_CLOSED:
                                ESNODE_CRYPTO_HANDLE(pt_esdb_node) = NULL_HANDLE;
                                /* ToDo ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_CLOSED; */
                                i4_return = PMR_OK;
                                
                                PM_DBG_INFO("D[%5d] Free\n", ESNODE_PID(pt_esdb_node));             
                                break;
                            default:
                                break;
                        }                      
                    }
                }

                if (PMR_OK == i4_rt_val && FALSE == fg_need_discon)
                {
                    #if (PM_ENABLE_FILTER_CTRL)
                    ui2_desc_counter--;
                    pmi_filter_db_set_desc_info( pt_esdb_node, e_desc_type, ui2_desc_id, ui2_desc_counter );
                    #endif
                }
            }
            else
            {
                i4_return = PMR_OK;
            }
        }            
    }
    if (i4_return != PMR_OK)
    {
        PM_DBG_ERROR("Failed\n");
    }
    return (i4_return);       
}
/*-----------------------------------------------------------------------------
 * Name: pmi_descrambler_update
 *
 * Description: This API requests RM to update the descrambler's PID value. 
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
INT32 pmi_descrambler_update(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    INT32       i4_return;
        
    i4_return = PMR_NOT_INIT;    
    
    if (b_descrambler_init)
    {
        i4_return = PMR_INV_ARG;        
                        
        if (pt_esdb_node != NULL)
        {  
            i4_return = PMR_FAILED;   
            
            if (ESNODE_CRYPTO_HANDLE(pt_esdb_node) != NULL_HANDLE)
            {            
                /* 1. Disable descrambler */
                if (rm_set(ESNODE_CRYPTO_HANDLE(pt_esdb_node), 
                           FALSE, 
                           0, 
                           DESC_SET_TYPE_CTRL, 
                           (const void *)((UINT32)FALSE),
                           sizeof(BOOL), 
                           &(ESNODE_CRYPTO_COND(pt_esdb_node))) == RMR_OK)
                {              
                    /* ToDo ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED; */
                    
                    /* 2. Update descrambler PID */                                        
                    if (rm_set(ESNODE_CRYPTO_HANDLE(pt_esdb_node), 
                               FALSE, 
                               0, 
                               DESC_SET_TYPE_PID, 
                               (const void *)((UINT32)(ESNODE_PID(pt_esdb_node))),
                               sizeof(MPEG_2_PID_T), 
                               &(ESNODE_CRYPTO_COND(pt_esdb_node))) == RMR_OK)
                    {
                        /* 3. Enable descrambler */
                        if (rm_set(ESNODE_CRYPTO_HANDLE(pt_esdb_node), 
                                   FALSE, 
                                   0, 
                                   DESC_SET_TYPE_CTRL, 
                                   (const void *)((UINT32)FALSE),
                                   sizeof(BOOL), 
                                   &(ESNODE_CRYPTO_COND(pt_esdb_node))) == RMR_OK)
                         {
                            i4_return = PMR_OK;   
                            PM_DBG_INFO("D[%5d] Updated\n", ESNODE_PID(pt_esdb_node));  
                         }
                         else
                         {
                            PM_DBG_ERROR("D[%5d] Failed to enable descrambler \n", ESNODE_PID(pt_esdb_node));
                            /* ToDo */
                         }                    
                    }                
                    else
                    {                        
                        PM_DBG_ERROR("D[%5d] Failed to set descrambler PID \n", ESNODE_PID(pt_esdb_node));
                        /* ToDo */
                    }                  
                }
                else
                {
                    PM_DBG_ERROR("D[%5d] Failed to disable descrambler \n", ESNODE_PID(pt_esdb_node));
                    /* ToDo */
                }      
            }
        }            
    }    
    return (i4_return);    
}

/*-----------------------------------------------------------------------------
 * Name: pmi_descrambler_set
 *
 * Description: This API is called to initialize the PM_CA module.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *          PMR_ALREADY_INIT This module is already initialized.
 *          PMR_FAILED       Failed due to other reason. 
 ----------------------------------------------------------------------------*/                      
INT32 pmi_descrambler_set( PM_COMPONENT_INFO_T* pt_esdb_node, PM_DESC_SETTING_T* pt_desc_setting )
{
    INT32            i4_return;
    DESC_ALG_T          t_desc_alg;
        
    i4_return = PMR_NOT_INIT;    
    
    if (b_descrambler_init)
    {
        i4_return = PMR_INV_ARG;        
                        
        if (pt_esdb_node != NULL)
        {
            if (NULL_HANDLE != ESNODE_CRYPTO_HANDLE(pt_esdb_node))
            {
                i4_return = PMR_FAILED;
                
                if ((CA_SET_ALG == pt_desc_setting->t_desc_set_cmd) ||
                    (CA_SET_KEY == pt_desc_setting->t_desc_set_cmd ))
                {
                    if(CA_SET_ALG == pt_desc_setting->t_desc_set_cmd)
                    {
                        t_desc_alg = (DESC_ALG_T)pt_desc_setting->u_desc_cmd_info.t_algorithm.t_desc_alg;
                    }
                    else
                    {
                        t_desc_alg = (DESC_ALG_T)pt_desc_setting->u_desc_cmd_info.t_key_info.t_desc_key.e_descrambler_alg;
                    }

                    /* If the command is algorithm change, 
                       always disable descrambler before set new algorithm to avoid descrambler stay previous status */
                    if (CA_SET_ALG == pt_desc_setting->t_desc_set_cmd)
                    {                        
                        if (rm_set(ESNODE_CRYPTO_HANDLE(pt_esdb_node), 
                              FALSE, 
                              0, 
                              DESC_SET_TYPE_CTRL, 
                              (const void *)((UINT32)FALSE),
                              sizeof(BOOL), 
                              &(ESNODE_CRYPTO_COND(pt_esdb_node))) == RMR_OK)
                        {                                    
                            /* ToDo ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED; */
                            i4_return = PMR_OK;    
                            PM_DBG_INFO("D[%5d] Disalbe Descrambler\n", ESNODE_PID(pt_esdb_node));      
                        }
                        else
                        {
                            PM_DBG_ERROR("D[%5d] Failed to disable descrambler \n", ESNODE_PID(pt_esdb_node));
                            /* ToDo */
                        }
                    }
                    
                    if (rm_set(ESNODE_CRYPTO_HANDLE(pt_esdb_node), 
                           FALSE, 
                           0, 
                           DESC_SET_TYPE_ALGORITHM, 
                           (const void *)(&t_desc_alg),
                           sizeof(DESC_ALG_T), 
                           &(ESNODE_CRYPTO_COND(pt_esdb_node))) == RMR_OK)
                    {
                        i4_return = PMR_OK;
                        PM_DBG_INFO("D[%5d] Set algorithm[%d]\n", ESNODE_PID(pt_esdb_node), t_desc_alg);  
                    }
                    else
                    {
                        PM_DBG_ERROR("Failed to set algorithm\n");
                        /* ToDo */
                    }
                }
                if (CA_SET_KEY == pt_desc_setting->t_desc_set_cmd)
                {
                    DRV_SET_TYPE_T  t_key_type;

                    do
                    {
                        if (CA_CRYPTO_KEY_EVEN == pt_desc_setting->u_desc_cmd_info.t_key_info.t_key_type)
                        {
                            t_key_type = DESC_SET_TYPE_EVEN_KEY;
                            PM_DBG_INFO( "DESC_SET_TYPE_EVEN_KEY\n" );
                        }
                        else if (CA_CRYPTO_KEY_ODD == pt_desc_setting->u_desc_cmd_info.t_key_info.t_key_type)
                        {
                            t_key_type = DESC_SET_TYPE_ODD_KEY;
                            PM_DBG_INFO( "DESC_SET_TYPE_ODD_KEY\n" );
                        }
                        else
                        {
                            PM_DBG_INFO( "key type is UNKNOWN\n" );
                            break;
                        }

                        if (DESC_ALG_DES_ECB     != pt_desc_setting->u_desc_cmd_info.t_key_info.t_desc_key.e_descrambler_alg &&
                            DESC_ALG_AES_128_CBC != pt_desc_setting->u_desc_cmd_info.t_key_info.t_desc_key.e_descrambler_alg)
                        {
                            PM_DBG_INFO( "key is not DESC_ALG_DES_ECB or DESC_ALG_AES_128_CBC, drop it\n" );
                            i4_return = PMR_OK;
                            break;
                        }
                        else if ( DESC_ALG_DES_ECB     == pt_desc_setting->u_desc_cmd_info.t_key_info.t_desc_key.e_descrambler_alg )
                        {
                            DES_ECB_KEY_INFO_T*     pt_des_ecb_key      = &pt_desc_setting->u_desc_cmd_info.t_key_info.t_desc_key.u_key_info.t_des_ecb_key;

                            {
                                UINT32          i;        

                                for (i = 0; i < pt_des_ecb_key->z_key_len; i++)
                                {
                                    if (0 == i%10)
                                    {
                                        PM_DBG( "\n" );
                                    }
                                    
                                    if (pt_des_ecb_key->pui1_key[i] < 0x10)
                                    {
                                        PM_DBG( " 0%x", pt_des_ecb_key->pui1_key[i] );
                                    }
                                    else
                                    {
                                        PM_DBG( " %x", pt_des_ecb_key->pui1_key[i] );
                                    }
                                }

                                PM_DBG( "\n" );
                            }
                        }
                        else if ( DESC_ALG_AES_128_CBC == pt_desc_setting->u_desc_cmd_info.t_key_info.t_desc_key.e_descrambler_alg )
                        {
                            AES_128_CBC_KEY_INFO_T* pt_aes_128_cbc_key  = &pt_desc_setting->u_desc_cmd_info.t_key_info.t_desc_key.u_key_info.t_aes_128_cbc_key;

                            {
                                UINT32          i;        

                                for (i = 0; i < pt_aes_128_cbc_key->z_iv_len; i++)
                                {
                                    if (0 == i%10)
                                    {
                                        PM_DBG( "\n" );
                                    }
                                    
                                    if (pt_aes_128_cbc_key->pui1_iv[i] < 0x10)
                                    {
                                        PM_DBG( " 0%x", pt_aes_128_cbc_key->pui1_iv[i] );
                                    }
                                    else
                                    {
                                        PM_DBG( " %x", pt_aes_128_cbc_key->pui1_iv[i] );
                                    }
                                }

                                PM_DBG( "\n" );

                                for (i = 0; i < pt_aes_128_cbc_key->z_key_len; i++)
                                {
                                    if (0 == i%10)
                                    {
                                        PM_DBG( "\n" );
                                    }
                                    
                                    if (pt_aes_128_cbc_key->pui1_key[i] < 0x10)
                                    {
                                        PM_DBG( " 0%x", pt_aes_128_cbc_key->pui1_key[i] );
                                    }
                                    else
                                    {
                                        PM_DBG( " %x", pt_aes_128_cbc_key->pui1_key[i] );
                                    }
                                }

                                PM_DBG( "\n" );
                            }
                        }
                
                        if (rm_set(ESNODE_CRYPTO_HANDLE(pt_esdb_node), 
                                   FALSE, 
                                   0, 
                                   t_key_type, 
                                   (const void *)(&pt_desc_setting->u_desc_cmd_info.t_key_info.t_desc_key),
                                   sizeof(DESC_KEY_INFO_T), 
                                   &(ESNODE_CRYPTO_COND(pt_esdb_node))) == RMR_OK)
                        {
                            i4_return = PMR_OK;   
                            PM_DBG_INFO("D[%5d] Set key\n", ESNODE_PID(pt_esdb_node));  
                        }
                        else
                        {
                            PM_DBG_ERROR("Failed to set key\n");
                            /* ToDo */
                        }        
                    } while( 0 );
                }

                /* Enable descrambler */
                if (rm_set(ESNODE_CRYPTO_HANDLE(pt_esdb_node), 
                      FALSE, 
                      0, 
                      DESC_SET_TYPE_CTRL, 
                      (const void *)((UINT32)TRUE),
                      sizeof(BOOL), 
                      &(ESNODE_CRYPTO_COND(pt_esdb_node))) == RMR_OK)
                {                                    
                    /* ToDo ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_OPENED; */
                    i4_return = PMR_OK;    
                    PM_DBG_INFO("D[%5d] Allocated\n", ESNODE_PID(pt_esdb_node));      
                }
                else
                {
                    PM_DBG_ERROR("D[%5d] Failed to enable descrambler \n", ESNODE_PID(pt_esdb_node));
                    /* ToDo */
                }      
            }
        }
    }            
    return (i4_return);    
}

#endif /* PM_ENABLE_CA  */
