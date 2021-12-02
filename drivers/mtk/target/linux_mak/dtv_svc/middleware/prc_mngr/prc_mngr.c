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
 * $RCSfile: prc_mngr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/5 $
 * $SWAuthor: Felisa Hsiao $
 * $MD5HEX: c771cd48aae953cc560437b06fe5ea09 $
 *
 * Description: 
 *         This header file contains Process Manager public APIs.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "prc_mngr/prc_mngr.h"
#include "prc_mngr/prc_common.h"
#include "prc_mngr/prc_db.h"
#include "prc_mngr/prc_scdb.h"
#include "prc_mngr/prc_filter.h"
#include "prc_mngr/prc_thread.h"
#include "prc_mngr/prc_lock.h"
#include "prc_mngr/prc_util.h"
#include "prc_mngr/prc_ca.h"
#include "prc_mngr/prc_desc.h"
#include "prc_mngr/prc_cli.h"
#include "prc_mngr/prc_debug.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/ 

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static BOOL b_pm_init = FALSE;  

#if (PM_ENABLE_MULTIMEDIA)
/* Detail for Set operation */
PM_SET_TYPE_T         e_dmx_ctrl_set_type; 
VOID*                 pt_dmx_ctrl_setinfo = NULL;
INT32                 i4_dmx_ctrl_set_result;

/* Detail for Get operation */
PM_GET_TYPE_T         e_dmx_ctrl_get_type; 
VOID*                 pt_dmx_ctrl_get_info = NULL;
SIZE_T                t_dmx_ctrl_get_size;
INT32                 i4_dmx_ctrl_get_result;
#endif

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static BOOL pm_es_handle_free_nfy_fct (HANDLE_T       h_handle,
                                       HANDLE_TYPE_T  e_type,
                                       void*          pv_obj,
                                       void*          pv_tag,   /* reference to ESNODE */
                                       BOOL           b_req_handle); 
static BOOL pm_service_handle_free_nfy_fct (HANDLE_T       h_handle,
                                            HANDLE_TYPE_T  e_type,
                                            VOID*          pv_obj,
                                            VOID*          pv_tag, /* reference to SERVICENODE */
                                            BOOL           b_req_handle);
HP_TYPE_T pm_es_filter_feedback_fct (UINT16         ui2_count,
                                     UINT16         ui2_max_count,
                                     HANDLE_T       h_handle,
                                     HANDLE_TYPE_T  e_type,
                                     void*          pv_obj,
                                     void*          pv_tag,
                                     void*          pv_parse_data);
HP_TYPE_T pm_es_ca_feedback_fct (UINT16         ui2_count,
                                 UINT16         ui2_max_count,
                                 HANDLE_T       h_handle,
                                 HANDLE_TYPE_T  e_type,
                                 void*          pv_obj,
                                 void*          pv_tag,
                                 void*          pv_parse_data);                                     
HP_TYPE_T pm_es_cp_feedback_fct (UINT16         ui2_count,
                                 UINT16         ui2_max_count,
                                 HANDLE_T       h_handle,
                                 HANDLE_TYPE_T  e_type,
                                 void*          pv_obj,
                                 void*          pv_tag,
                                 void*          pv_parse_data); 
static VOID pm_db_parser_play_fct(PM_COMPONENT_INFO_T* pt_es_node);      
static VOID pm_db_parser_stop_fct(PM_COMPONENT_INFO_T* pt_es_node);              
static VOID pm_db_parser_close_fct(PM_COMPONENT_INFO_T* pt_es_node);  

#if (PM_ENABLE_MULTIMEDIA)
static VOID pm_db_parser_set_control_fct(PM_COMPONENT_INFO_T* pt_es_node);  
static VOID pm_db_parser_play_ctrl_fct(PM_COMPONENT_INFO_T* pt_es_node);      
static VOID pm_db_parser_stop_ctrl_fct(PM_COMPONENT_INFO_T* pt_es_node);                                       
#endif
                                                                                                     
/*-----------------------------------------------------------------------------
 * Name: pm_es_handle_free_nfy_fct
 *
 * Description: This API is responsible to handle the notification from RM for 
 *              ES handle free.
 *
 * Inputs:  h_handle         Contains the handle which shalled be freed.
 *          e_type           Contains the handle type
 *          pv_obj           References the data object.
 *          pv_tag           Contains the handles private tag.
 *          b_req_handle     TRUE: This free operation is specially requested 
 *                                 on this handle.
 *                           FALSE:This free operation is caused due some 
 *                                 other effect.
 * Outputs: -
 *
 * Returns: TRUE             Free handle is allowed.
 *          FALSE            Free handle is rejected.
 ----------------------------------------------------------------------------*/        
BOOL pm_es_handle_free_nfy_fct (HANDLE_T       h_handle,
                                HANDLE_TYPE_T  e_type,
                                VOID*          pv_obj,
                                VOID*          pv_tag,   /* reference to ESNODE */
                                BOOL           b_req_handle)
{      
    PM_COMPONENT_INFO_T* pt_esdb_node;       
    
    if ((e_type == PM_GROUP_PMAPI_ES) && (pv_obj != NULL) && (pv_obj == pv_tag))
    {    
        pt_esdb_node = pv_obj;
    
        if (ESNODE_FILTER_STATUS(pt_esdb_node) == PM_COND_CLOSED)
        {
            handle_delink(&(ESNODE_LINK(pt_esdb_node)), h_handle);
            return TRUE;
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: pm_service_handle_free_nfy_fct
 *
 * Description: This API is responsible to handle the notification from RM for 
 *              SERVICE handle free.
 *
 * Inputs:  h_handle         Contains the handle which shalled be freed.
 *          e_type           Contains the handle type
 *          pv_obj           References the data object.
 *          pv_tag           Contains the handles private tag.
 *          b_req_handle     TRUE: This free operation is specially requested 
 *                                 on this handle.
 *                           FALSE:This free operation is caused due some 
 *                                 other effect.
 * Outputs: -
 *
 * Returns: TRUE             Free handle is allowed.
 *          FALSE            Free handle is rejected.
 ----------------------------------------------------------------------------*/        
BOOL pm_service_handle_free_nfy_fct (HANDLE_T       h_handle,
                                     HANDLE_TYPE_T  e_type,
                                     VOID*          pv_obj,
                                     VOID*          pv_tag, /* reference to SERVICENODE */
                                     BOOL           b_req_handle)
{    
    PM_SERVICE_INFO_T* pt_svdb_node;
    
    if ((e_type == PM_GROUP_PMAPI_SV) && (pv_obj != NULL) && (pv_obj == pv_tag))
    {
    
        pt_svdb_node = pv_obj;
    
        if (SVNODE_COND(pt_svdb_node) == PM_COND_CLOSE)
        {
            handle_delink(&(SVNODE_LINK(pt_svdb_node)), h_handle);
            return TRUE;
        }        
    }
    return FALSE;
    
}

/*-----------------------------------------------------------------------------
 * Name: pm_es_filter_feedback_fct
 *
 * Description: This API is responsible to feedback the status to Caller (SM).
 *
 * Inputs:  ui2_count        Ignored. 
 *          ui2_max_count    Contains the number of iterations. 
 *          h_handle         Contains the handle.
 *          e_type           Contains the handles type.
 *          pv_obj           References the data object. 
 *          pv_tag           Contains the handle private tag.
 *          pv_parse_data    Ignored. 
 * Outputs: -
 *
 * Returns: HP_BREAK         Break the parse loop and return from handle_parse.
 ----------------------------------------------------------------------------*/ 
HP_TYPE_T pm_es_filter_feedback_fct (UINT16         ui2_count,
                              UINT16         ui2_max_count,
                              HANDLE_T       h_handle,
                              HANDLE_TYPE_T  e_type,
                              VOID*          pv_obj,
                              VOID*          pv_tag,
                              VOID*          pv_parse_data)
{
    PM_COMPONENT_INFO_T* pt_es_node;
    
    pt_es_node = pv_obj;
    if ((ui2_max_count == 1) && (e_type == PM_GROUP_PMAPI_ES) && (pt_es_node != NULL) && (pv_obj == pv_tag))
    {
        if ((*((PM_COND_T *)pv_parse_data)) == ESNODE_FILTER_STATUS(pt_es_node))        
            ESNODE_FILTER_NFY(pt_es_node)(h_handle, ESNODE_FILTER_STATUS(pt_es_node), 0, ESNODE_FILTER_TAG(pt_es_node));                            
        return HP_BREAK;
    }
    else
        return HP_BREAK;
    
}

/*-----------------------------------------------------------------------------
 * Name: pm_es_ca_feedback_fct
 *
 * Description: This API is responsible to feedback the CA status to Caller (SM).
 *
 * Inputs:  ui2_count        Ignored. 
 *          ui2_max_count    Contains the number of iterations. 
 *          h_handle         Contains the handle.
 *          e_type           Contains the handles type.
 *          pv_obj           References the data object. 
 *          pv_tag           Contains the handle private tag.
 *          pv_parse_data    Ignored. 
 * Outputs: -
 *
 * Returns: HP_BREAK         Break the parse loop and return from handle_parse.
 ----------------------------------------------------------------------------*/ 
HP_TYPE_T pm_es_ca_feedback_fct (UINT16         ui2_count,
                                 UINT16         ui2_max_count,
                                 HANDLE_T       h_handle,
                                 HANDLE_TYPE_T  e_type,
                                 VOID*          pv_obj,
                                 VOID*          pv_tag,
                                 VOID*          pv_parse_data)
{
    
    PM_COMPONENT_INFO_T* pt_es_node;
    PM_SERVICE_INFO_T* pt_svdb_node;  
        
    pt_es_node = pv_obj;    
    if ((ui2_max_count == 1) && (e_type == PM_GROUP_PMAPI_ES) && (pt_es_node != NULL) && (pv_obj == pv_tag))
    {
        pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_es_node));
        if (pt_svdb_node == NULL)
        {
            return HP_BREAK;
        }

        switch (SVNODE_PROC_TYPE(pt_svdb_node))
        {
            case PM_PRESENT:         
                if (((*((PM_CA_COND_T *)pv_parse_data)) == ESNODE_CA_COND(pt_es_node))&&
                    (ESNODE_CA_NFY(pt_es_node) != NULL))        
                {
                    ESNODE_CA_NFY(pt_es_node)(h_handle, ESNODE_CA_COND(pt_es_node), ESNODE_CA_DETAIL(pt_es_node), ESNODE_CA_TAG(pt_es_node));      
                }
                return HP_BREAK;
                
        #if (PM_ENABLE_MULTIMEDIA)
            case PM_MULTIMEDIA:
        #if (PM_ENABLE_RECORD)
            case PM_RECORDING:
        #endif
        #endif /* (PM_ENABLE_MULTIMEDIA) */
            case PM_DETECTION:
                if (((*((PM_CA_COND_T *)pv_parse_data)) == ESNODE_CA_COND(pt_es_node))&&
                    (ESNODE_CA_NFY(pt_es_node) != NULL))       
                {                    
                    ESNODE_CA_NFY(pt_es_node)(h_handle, ESNODE_CA_COND(pt_es_node), ESNODE_CA_DETAIL(pt_es_node), ESNODE_CA_TAG(pt_es_node));                                                                                
                }
                return HP_BREAK;
                
            default:
                return HP_BREAK;
        }
    }
    else
        return HP_BREAK;  
}


HP_TYPE_T pm_es_cp_feedback_fct (UINT16         ui2_count,
                                 UINT16         ui2_max_count,
                                 HANDLE_T       h_handle,
                                 HANDLE_TYPE_T  e_type,
                                 VOID*          pv_obj,
                                 VOID*          pv_tag,
                                 VOID*          pv_parse_data)
{
    
    PM_COMPONENT_INFO_T* pt_es_node;
    PM_SERVICE_INFO_T* pt_svdb_node;  
        
    pt_es_node = pv_obj;    
    if ((ui2_max_count == 1) && (e_type == PM_GROUP_PMAPI_ES) && (pt_es_node != NULL) && (pv_obj == pv_tag))
    {
        pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_es_node));
        if (pt_svdb_node == NULL)
        {
            return HP_BREAK;
        }
     
        switch (SVNODE_PROC_TYPE(pt_svdb_node))
        {
            case PM_PRESENT:         
            case PM_RECORDING:         
                if (ESNODE_CP_NFY(pt_es_node) != NULL)
                {
                    ESNODE_CP_NFY(pt_es_node)(h_handle, ESNODE_CP_INFO(pt_es_node), ESNODE_CP_TAG(pt_es_node));                                                        
                }
                return HP_BREAK;
    
            default:
                return HP_BREAK;
        }
    }
    else
        return HP_BREAK;  
}

/*-----------------------------------------------------------------------------
 * Name: pm_db_parser_play_fct
 *
 * Description: This API is responsible to play the ES. 
 *
 * Inputs:  pt_es_node       Contains the reference to ESDB node. 
 *
 * Outputs: pt_es_node       Contains the reference to ESDB node for update 
 *                           status. 
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
VOID pm_db_parser_play_fct(PM_COMPONENT_INFO_T* pt_es_node)
{
    PM_SERVICE_INFO_T* pt_svdb_node;  
    PM_COND_T          t_cond;
       
    pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_es_node));
    if (pt_svdb_node != NULL)
    {
        switch (SVNODE_PROC_TYPE(pt_svdb_node))
        {
            case PM_PRESENT:
                if ((ESNODE_REQUEST_TYPE(pt_es_node) == PM_PRESENT_ES_WI_SETTING) ||
                    (ESNODE_REQUEST_TYPE(pt_es_node) == PM_PRESENT_ES_WO_SETTING))
                {
                    if ((pmi_valid_request(pt_es_node, PM_ES_STATE_WAIT_PLAY) == TRUE) &&
                        (pmi_filter_enable(pt_es_node) == PMR_OK))
                    {               
                        ESNODE_FILTER_STATUS(pt_es_node) = PM_COND_PLAYED;
                        t_cond = PM_COND_PLAYED;
                        
                        /* Call Notify Function */
                        handle_parse(&(ESNODE_LINK(pt_es_node)), pm_es_filter_feedback_fct, &t_cond);                
                    }
                }
                break;            
                
            case PM_DETECTION:             
                if ((pmi_valid_request(pt_es_node, PM_ES_STATE_WAIT_PLAY) == TRUE) &&
                    (pmi_filter_enable(pt_es_node) == PMR_OK))
                {               
                    ESNODE_FILTER_STATUS(pt_es_node) = PM_COND_PLAYED;
                    t_cond = PM_COND_PLAYED;
                    
                    /* Call Notify Function */
                    handle_parse(&(ESNODE_LINK(pt_es_node)), pm_es_filter_feedback_fct, &t_cond);                
                }
                break;

        #if (PM_ENABLE_MULTIMEDIA)                
            #if (PM_ENABLE_RECORD)
            case PM_RECORDING:
                if ((ESNODE_REC_REQUEST_TYPE(pt_es_node) == PM_RECORDING_ES_WI_SETTING) ||
                    (ESNODE_REC_REQUEST_TYPE(pt_es_node) == PM_RECORDING_ES_WO_SETTING))
                {
                    if ((pmi_valid_request(pt_es_node, PM_ES_STATE_WAIT_PLAY) == TRUE) &&
                        (pmi_filter_enable(pt_es_node) == PMR_OK))
                    {               
                        ESNODE_FILTER_STATUS(pt_es_node) = PM_COND_PLAYED;
                        t_cond = PM_COND_PLAYED;
                        
                        /* Call Notify Function */
                        handle_parse(&(ESNODE_LINK(pt_es_node)), pm_es_filter_feedback_fct, &t_cond);                
                    }
                }
                break;
            #endif
                
            case PM_MULTIMEDIA:
                if ((ESNODE_FILTER_TYPE(pt_es_node) == DRVT_DEMUX_CONTAINER_ES) ||
                    (ESNODE_FILTER_TYPE(pt_es_node) == DRVT_DEMUX_CONTAINER_ES_MEMORY))
                {
                    if ((pmi_valid_request(pt_es_node, PM_ES_STATE_WAIT_PLAY) == TRUE) &&
                        (pmi_filter_enable(pt_es_node) == PMR_OK))
                    {               
                        ESNODE_FILTER_STATUS(pt_es_node) = PM_COND_PLAYED;
                        t_cond = PM_COND_PLAYED;
                        
                        /* Call Notify Function */
                        handle_parse(&(ESNODE_LINK(pt_es_node)), pm_es_filter_feedback_fct, &t_cond);                
                    }                    
                }
                break;
        #endif
            default:
                /* ToDo: Other mode */
                break;                                
        }
    }
}  
#if (PM_ENABLE_MULTIMEDIA) 
/*-----------------------------------------------------------------------------
 * Name: pm_db_parser_play_ctrl_fct
 *
 * Description: This API is responsible to play the demux control ES. 
 *
 * Inputs:  pt_es_node       Contains the reference to ESDB node. 
 *
 * Outputs: pt_es_node       Contains the reference to ESDB node for update 
 *                           status. 
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
VOID pm_db_parser_play_ctrl_fct(PM_COMPONENT_INFO_T* pt_es_node)
{
    PM_SERVICE_INFO_T* pt_svdb_node;  
       
    pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_es_node));
    if (pt_svdb_node != NULL)
    {
        switch (SVNODE_PROC_TYPE(pt_svdb_node))
        {
        #if (PM_ENABLE_MULTIMEDIA)                
            case PM_MULTIMEDIA:
                if (ESNODE_FILTER_TYPE(pt_es_node) == DRVT_DEMUX_CONTROL)
                {
                    if ((pmi_valid_request(pt_es_node, PM_ES_STATE_WAIT_PLAY) == TRUE) &&
                        (pmi_filter_enable(pt_es_node) == PMR_OK))
                    {               
                        ESNODE_FILTER_STATUS(pt_es_node) = PM_COND_PLAYED;
                    }                    
                }
                break;
        #endif
            default:
                /* ToDo: Other mode */
                break;                                
        }
    }
}  
#endif
/*-----------------------------------------------------------------------------
 * Name: pm_db_parser_stop_fct
 *
 * Description: This API is responsible to stop the ES. 
 *
 * Inputs:  pt_es_node       Contains the reference to ESDB node. 
 *           
 * Outputs: pt_es_node       Contains the reference to ESDB node for update 
 *                           status. 
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
VOID pm_db_parser_stop_fct(PM_COMPONENT_INFO_T* pt_es_node)
{        
    PM_SERVICE_INFO_T* pt_svdb_node;  
    PM_COND_T          t_cond;
       
    pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_es_node));
    if (pt_svdb_node != NULL)
    {
        switch (SVNODE_PROC_TYPE(pt_svdb_node))
        {
            case PM_PRESENT:
                if ((ESNODE_REQUEST_TYPE(pt_es_node) == PM_PRESENT_ES_WI_SETTING) ||
                    (ESNODE_REQUEST_TYPE(pt_es_node) == PM_PRESENT_ES_WO_SETTING))
                {
                    if ((pmi_valid_request(pt_es_node, PM_ES_STATE_WAIT_STOP) == TRUE) &&
                        (pmi_filter_disable(pt_es_node) == PMR_OK))
                    {
                        ESNODE_FILTER_STATUS(pt_es_node) = PM_COND_STOPPED;                    
                        t_cond = PM_COND_STOPPED; 
                                    
                        /* Call Notify Function */
                        handle_parse(&(ESNODE_LINK(pt_es_node)), pm_es_filter_feedback_fct, &t_cond);
                    } 
                }
                break;            
            case PM_DETECTION:    
                if ((pmi_valid_request(pt_es_node, PM_ES_STATE_WAIT_STOP) == TRUE) &&
                    (pmi_filter_disable(pt_es_node) == PMR_OK))
                {
                    ESNODE_FILTER_STATUS(pt_es_node) = PM_COND_STOPPED;                    
                    t_cond = PM_COND_STOPPED; 
                                
                    /* Call Notify Function */
                    handle_parse(&(ESNODE_LINK(pt_es_node)), pm_es_filter_feedback_fct, &t_cond);
                }         
               
                break;
                
        #if (PM_ENABLE_MULTIMEDIA)                
            #if (PM_ENABLE_RECORD)
            case PM_RECORDING:
                if ((ESNODE_REC_REQUEST_TYPE(pt_es_node) == PM_RECORDING_ES_WI_SETTING) ||
                    (ESNODE_REC_REQUEST_TYPE(pt_es_node) == PM_RECORDING_ES_WO_SETTING))
                {
                    if ((pmi_valid_request(pt_es_node, PM_ES_STATE_WAIT_STOP) == TRUE) &&
                        (pmi_filter_disable(pt_es_node) == PMR_OK))
                    {
                        ESNODE_FILTER_STATUS(pt_es_node) = PM_COND_STOPPED;                    
                        t_cond = PM_COND_STOPPED; 
                                    
                        /* Call Notify Function */
                        handle_parse(&(ESNODE_LINK(pt_es_node)), pm_es_filter_feedback_fct, &t_cond);
                    } 
                }
                break;
            #endif
                
            case PM_MULTIMEDIA:  
                if ((ESNODE_FILTER_TYPE(pt_es_node) == DRVT_DEMUX_CONTAINER_ES) ||
                    (ESNODE_FILTER_TYPE(pt_es_node) == DRVT_DEMUX_CONTAINER_ES_MEMORY))
                {    
                    if ((pmi_valid_request(pt_es_node, PM_ES_STATE_WAIT_STOP) == TRUE) &&
                        (pmi_filter_disable(pt_es_node) == PMR_OK))
                    {
                        ESNODE_FILTER_STATUS(pt_es_node) = PM_COND_STOPPED;                    
                        t_cond = PM_COND_STOPPED; 
                                    
                        /* Call Notify Function */
                        handle_parse(&(ESNODE_LINK(pt_es_node)), pm_es_filter_feedback_fct, &t_cond);
                    }
                }  
                break;
        #endif
        
            default:
                /* ToDo: Other mode */
                break;                                
        }
    }        

}        
#if (PM_ENABLE_MULTIMEDIA) 
/*-----------------------------------------------------------------------------
 * Name: pm_db_parser_stop_ctrl_fct
 *
 * Description: This API is responsible to stop the demux control ES. 
 *
 * Inputs:  pt_es_node       Contains the reference to ESDB node. 
 *           
 * Outputs: pt_es_node       Contains the reference to ESDB node for update 
 *                           status. 
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
VOID pm_db_parser_stop_ctrl_fct(PM_COMPONENT_INFO_T* pt_es_node)
{        
    PM_SERVICE_INFO_T* pt_svdb_node;      
       
    pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_es_node));
    if (pt_svdb_node != NULL)
    {
        switch (SVNODE_PROC_TYPE(pt_svdb_node))
        {                      
           case PM_MULTIMEDIA:              
                if (ESNODE_FILTER_TYPE(pt_es_node) == DRVT_DEMUX_CONTROL)
                {    
                    if ((pmi_valid_request(pt_es_node, PM_ES_STATE_WAIT_STOP) == TRUE) &&
                        (pmi_filter_disable(pt_es_node) == PMR_OK))
                    {
                        ESNODE_FILTER_STATUS(pt_es_node) = PM_COND_STOPPED;         
                    }
                }  
                break;                
                
            default:
                /* ToDo: Other mode */
                break;                                
        }
    }        

}        
#endif
/*-----------------------------------------------------------------------------
 * Name: pm_db_parser_close_fct
 *
 * Description: This API is responsible to close the ES.
 *
 * Inputs:  pt_es_node       Contains the reference to ESDB node. 
 *          
 * Outputs: pt_es_node       Contains the reference to ESDB node for update 
 *                           status. 
 *
 * Returns: -
 ----------------------------------------------------------------------------*/             
VOID pm_db_parser_close_fct(PM_COMPONENT_INFO_T* pt_es_node)
{    
    /* Here */
    PM_SERVICE_INFO_T* pt_svdb_node;
    RM_COND_T          t_cond;
    
    if (pt_es_node == NULL)
        return;

    if (ESNODE_SERVICE_INDEX(pt_es_node) != PM_NULL_IDX)
    {
        
        pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_es_node));
        if (pt_svdb_node != NULL)
        {                     
            /* Release SCDB query */
            pmi_scdb_release(pt_es_node);
            
            /* free all HW resource */ 
            switch (SVNODE_PROC_TYPE(pt_svdb_node))
            {
                case PM_PRESENT:
                case PM_DETECTION:
                #if PM_ENABLE_CA                 
                    if (ESNODE_CRYPTO_HANDLE(pt_es_node) != NULL_HANDLE)
                    {
                        pmi_descrambler_free(pt_es_node);
                    }
                #endif               
                
                    if ((ESNODE_FILTER_HANDLE(pt_es_node)!= NULL_HANDLE)&&(ESNODE_REQUEST_TYPE(pt_es_node)!=PM_PRESENT_ES_FILTER_ALLOCATED))
                    {
                        if (pmi_valid_request(pt_es_node, PM_ES_STATE_WAIT_CLOSE) == TRUE)        
                            pmi_filter_free(pt_es_node);    
                    }
                    break;
                    
            #if (PM_ENABLE_MULTIMEDIA)            
                #if (PM_ENABLE_RECORD)
                case PM_RECORDING:
                #if (PM_ENABLE_CA)
                    if (ESNODE_CRYPTO_HANDLE(pt_es_node) != NULL_HANDLE)
                    {
                        pmi_descrambler_free(pt_es_node);
                    }
                #endif
                    if ((ESNODE_FILTER_HANDLE(pt_es_node)!= NULL_HANDLE)&&(ESNODE_REC_REQUEST_TYPE(pt_es_node)!=PM_RECORDING_ES_FILTER_ALLOCATED))
                    {
                        if (pmi_valid_request(pt_es_node, PM_ES_STATE_WAIT_CLOSE) == TRUE)        
                            pmi_filter_free(pt_es_node);    
                    }
                    break;
                #endif
                    
                case PM_MULTIMEDIA:     
                    if ((ESNODE_FILTER_HANDLE(pt_es_node) != NULL_HANDLE) &&
                        (ESNODE_MMP_REQUEST_TYPE(pt_es_node) != PM_MM_ES_FILTER_ALLOCATED))
                    {
                        if (pmi_valid_request(pt_es_node, PM_ES_STATE_WAIT_CLOSE) == TRUE)        
                            pmi_filter_free(pt_es_node);    
                    }  
                    break;
            #endif
                         
                default:
                    break;
            }        

            
            /* Update status */
            ESNODE_FILTER_STATUS(pt_es_node) = PM_COND_CLOSED;                    
            t_cond = PM_COND_CLOSED; 
                            
            /* Call Notify Function */
            if (ESNODE_FILTER_NFY(pt_es_node) != NULL)
                handle_parse(&(ESNODE_LINK(pt_es_node)), pm_es_filter_feedback_fct, &t_cond);
                
            /* Remove from Service */
            pmi_db_remove_from_service(pt_es_node);    
            
            /* Delink and Free Handle */    
            handle_free_all(&(ESNODE_LINK(pt_es_node))); 
            
            /* Free ES node */
            pmi_esdb_free(pt_es_node);          
        }
        else
        {
            /* ToDo */
        }
    }    
}  

#if (PM_ENABLE_MULTIMEDIA)
/*-----------------------------------------------------------------------------
 * Name: pm_db_parser_set_control_fct
 *
 * Description: This API is responsible to set operation for DRVT_DEMUX_CONTROL.
 *
 * Inputs:  pt_es_node       Contains the reference to ESDB node. 
 *          
 * Outputs: pt_es_node       Contains the reference to ESDB node for update 
 *                           status. 
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
static VOID pm_db_parser_set_control_fct(PM_COMPONENT_INFO_T* pt_es_node)
{
    
    PM_SERVICE_INFO_T* pt_svdb_node;      
       
    pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_es_node));
    if (pt_svdb_node != NULL)
    {
        switch (SVNODE_PROC_TYPE(pt_svdb_node))
        {
            case PM_MULTIMEDIA:
                if (ESNODE_FILTER_TYPE(pt_es_node) == DRVT_DEMUX_CONTROL)
                {
                    i4_dmx_ctrl_set_result = pmi_filter_set(pt_es_node, e_dmx_ctrl_set_type, pt_dmx_ctrl_setinfo);
                }                
                break; 
                
            default:
                /* ToDo: Other mode */
                PM_DBG_ERROR( "the command type is not MULTIMEDIA\n" );
                break;                                
        }
    }    
}

/*-----------------------------------------------------------------------------
 * Name: pm_db_parser_get_control_fct
 *
 * Description: This API is responsible to get operation for DRVT_DEMUX_CONTROL.
 *
 * Inputs:  pt_es_node       Contains the reference to ESDB node. 
 *          
 * Outputs: pt_es_node       Contains the reference to ESDB node for update 
 *                           status. 
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
static VOID pm_db_parser_get_control_fct(PM_COMPONENT_INFO_T* pt_es_node)
{
    
    PM_SERVICE_INFO_T* pt_svdb_node;      
       
    pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_es_node));
    if (pt_svdb_node != NULL)
    {
        switch (SVNODE_PROC_TYPE(pt_svdb_node))
        {
            case PM_MULTIMEDIA:
                if (ESNODE_FILTER_TYPE(pt_es_node) == DRVT_DEMUX_CONTROL)
                {
                    i4_dmx_ctrl_get_result = pmi_filter_get(pt_es_node,
                                                            e_dmx_ctrl_get_type,
                                                            pt_dmx_ctrl_get_info,
                                                            &t_dmx_ctrl_get_size);
                }                
                break; 
                
            default:
                /* ToDo: Other mode */
                PM_DBG_ERROR( "the command type is not MULTIMEDIA\n" );
                break;                                
        }
    }    
} 
#endif

/*-----------------------------------------------------------------------------
 * Name: pm_init
 *
 * Description: This API is responsible to initialize PM module.
 *
 * Inputs:  ui2_num_ca_systems Contains the number of CA system. 
 *          ui2_num_services   Contains the number of service. 
 *          ui2_num_comps      Contains the number of ES. 
 *          pt_thread_descr    Contains the thread descriptor for notification 
 *                             from other modules.
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *          PMR_ALREADY_INIT PM is already initialized.
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/             
INT32 pm_init (UINT16           ui2_num_ca_systems,
               UINT16           ui2_num_services,
               UINT16           ui2_num_comps,
               THREAD_DESCR_T*  pt_thread_descr)
{
    UINT8  ui1_priority;
    INT32  i4_return;
    UINT16 ui2_num_msgs;
    SIZE_T z_stack_size;
        
    i4_return = PMR_ALREADY_INIT;
    
#if PM_ENABLE_RECORD || PM_ENABLE_TIME_SHIFT
    if (ui2_num_comps < 0x20)
    {
        ui2_num_comps = 0x20;
    }
#endif

    /* DVBC, DVBT, DVBS need 3 nodes */
    if (3 > ui2_num_ca_systems)
    {
        ui2_num_ca_systems = 3;
    }
    
    if (! (b_pm_init))
    {
        b_pm_init = TRUE;
        i4_return = PMR_FAILED;
        
        pmi_dbg_init();
        
        if (pmi_init_lock_module() != PMR_OK)
        {
            PM_ERR_CODE(i4_return);
            return i4_return;
        }
        
        if (pmi_db_init(ui2_num_services, ui2_num_comps) != PMR_OK)
        {
            PM_ERR_CODE(i4_return);
            return i4_return;
        }
        
        if (pmi_filter_init(ui2_num_comps) != PMR_OK) 
        {
             return i4_return;
        }
       
    #if PM_ENABLE_CA
        if (pmi_descrambler_init() != PMR_OK)
        {
            PM_ERR_CODE(i4_return);
            return i4_return;
        }
  
        if (pmi_ca_init(ui2_num_ca_systems) != PMR_OK)
        {
            PM_ERR_CODE(i4_return);
            return i4_return;
        }
                  
    #endif /* PM_ENABLE_CA */
    
        /* Extract the thread values. */
        ui1_priority = DEFAULT_PRIORITY;
        z_stack_size = DEFAULT_STACK_SIZE;
        ui2_num_msgs = DEFAULT_NUM_MSGS;
        
        if (pt_thread_descr != NULL)
        {
            ui1_priority = pt_thread_descr->ui1_priority;
            z_stack_size = pt_thread_descr->z_stack_size;
            ui2_num_msgs = pt_thread_descr->ui2_num_msgs;
        }
    
        pmi_thread_init (z_stack_size, ui1_priority, ui2_num_msgs);
     
#ifdef CLI_LVL_ALL 
        if (pmi_cli_init() != PMR_OK)
        {
            PM_ERR_CODE(i4_return);
            return i4_return;    
        }
#endif /* CLI_LVL_ALL */        
        i4_return = PMR_OK;      
    }  

    PM_ERR_CODE(i4_return);
    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: pm_open_service
 *
 * Description: This API is responsible to open service which may contain 
 *              multiple ESs.
 *
 * Inputs:  h_connection      Contains the connection handle assigned by CM.
 *          e_prc_type        Contains the type of process. 
 *          pt_info           Contains the reference to the related arguments 
 *                            by process type.
 * Outputs: pt_info           Contains the reference to the related arguments 
 *                            by process type and the output.
 *          ph_service        Contain the reference to the returned service 
 *                            handle.
 *
 * Returns: PMR_OK              Success
 *          PMR_NOT_INIT        PM module is not initialized.
 *          PMR_INV_ARG         Invalid arguments. 
 *          PMR_OUT_OF_HANDLES  No avaialabe handle.
 *          PMR_INV_HANDLE      Invalid connection handle
 *          PMR_FAILED          Failed due to other reason.
 ----------------------------------------------------------------------------*/                               
INT32 pm_open_service(HANDLE_T             h_connection,   
                      PM_PROCESSING_TYPE_T e_prc_type,     
                      VOID*                pt_info,                        
                      HANDLE_T*            ph_service)
{
    PM_SERVICE_INFO_T*       pt_svdb_node = NULL;
    PM_COMPONENT_INFO_T*     pt_esdb_node = NULL;   
    INT32                    i4_return, i4_es_open_status = PMR_FAILED;
    BOOL                     b_valid, b_continue;   
    UINT32                   i;
    RM_COND_T                t_cond, t_conn;  
    PM_BROADCAST_INFO_T*     pt_broadcast_info = NULL;   
    PM_DETECTION_INFO_T*     pt_detect_info = NULL; 
    
#if (PM_ENABLE_MULTIMEDIA)
    PM_MULTIMEDIA_INFO_T*    pt_multimedia_info = NULL;     
    #if (PM_ENABLE_RECORD)
    PM_RECORDING_INFO_T*     pt_recording_info = NULL;
    #endif
#endif    
 
    
    i4_return = PMR_NOT_INIT;
    
    if (b_pm_init)
    {
        i4_return = PMR_INV_HANDLE;      
        
        if (x_handle_valid (h_connection) == TRUE)
        {        
            i4_return = PMR_INV_ARG;    
            
            /* Check parameters */ 
            b_valid = TRUE;
            
            switch (e_prc_type)
            {
                case PM_PRESENT:
                    pt_broadcast_info = (PM_BROADCAST_INFO_T* )pt_info;
                    if (pt_broadcast_info != NULL)
                    {
                        if ( pt_broadcast_info->ui4_list_size > 0)         
                        {
                            if (pt_broadcast_info->pt_component_list != NULL)
                            {
                                for (i=0; i< pt_broadcast_info->ui4_list_size; i++)
                                {                                     
                                    switch (pt_broadcast_info->pt_component_list[i].e_es_info_type)
                                    {
                                        case PM_PRESENT_ES_WO_SETTING:
                                        #if (!PM_ENABLE_CA)    
                                            if  ((pmi_streamtype_valid(pt_broadcast_info->pt_component_list[i].u_es_info.t_stream_comp_id.e_type) == FALSE) ||
                                                 (pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.pf_nfy == NULL))
                                        #else
                                            if  ((pmi_streamtype_valid(pt_broadcast_info->pt_component_list[i].u_es_info.t_stream_comp_id.e_type) == FALSE) ||
                                                 (pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.pf_nfy == NULL)||
                                                 (pt_broadcast_info->pt_component_list[i].t_ca_nfy_info.pf_nfy == NULL) ||
                                                 (pt_broadcast_info->pt_component_list[i].t_cp_nfy_info.pf_nfy == NULL))                                         
                                        #endif   
                                            {
                                                b_valid = FALSE;
                                            }                                        
                                            break;
                                        case PM_PRESENT_ES_WI_SETTING:
                                        #if (!PM_ENABLE_CA)    
                                            if  ((pmi_streamtype_valid(pt_broadcast_info->pt_component_list[i].u_es_info.t_stream_setting.e_type) == FALSE) ||
                                                 (pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.pf_nfy == NULL))
                                        #else
                                            if  ((pmi_streamtype_valid(pt_broadcast_info->pt_component_list[i].u_es_info.t_stream_setting.e_type) == FALSE) ||
                                                 (pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.pf_nfy == NULL)||
                                                 (pt_broadcast_info->pt_component_list[i].t_ca_nfy_info.pf_nfy == NULL) ||
                                                 (pt_broadcast_info->pt_component_list[i].t_cp_nfy_info.pf_nfy == NULL))                                         
                                        #endif   
                                            {
                                                b_valid = FALSE;
                                            }                                                        
                                            break;
                                        
                                        case PM_PRESENT_ES_FILTER_ALLOCATED:
                                        #if (PM_ENABLE_CA)  
                                            if  ((pt_broadcast_info->pt_component_list[i].u_es_info.t_filter_setting.t_filter_handle == NULL_HANDLE) ||
                                                 (pt_broadcast_info->pt_component_list[i].t_ca_nfy_info.pf_nfy == NULL) ||
                                                 (pt_broadcast_info->pt_component_list[i].t_cp_nfy_info.pf_nfy == NULL)) 
                                            {
                                                b_valid = FALSE;
                                            }                                         
                                        #endif   
                                             
                                            break;
                                            
                                        default:
                                            b_valid = FALSE;
                                            break;
                                    }
                                    
                                    if (b_valid == FALSE)
                                        break;
                                }    
                            }
                            else
                                b_valid = FALSE;
                        }
                    }                    
                    break;
                    
                case PM_DETECTION:
                    pt_detect_info = (PM_DETECTION_INFO_T*)pt_info;
                    if (pt_detect_info != NULL)
                    {
                        if ( pt_detect_info->ui4_list_size > 0)         
                        {
                            if (pt_detect_info->pt_component_list != NULL)
                            {
                                 for (i=0; i< pt_detect_info->ui4_list_size; i++)
                                 { 
                                 #if (!PM_ENABLE_CA)    
                                    if  ((pmi_streamtype_valid(pt_detect_info->pt_component_list[i].e_type) == FALSE) ||
                                         (pt_detect_info->pt_component_list[i].t_filter_nfy_info.pf_nfy == NULL))
                                 #else
                                    if  ((pmi_streamtype_valid(pt_detect_info->pt_component_list[i].e_type) == FALSE) ||
                                         (pt_detect_info->pt_component_list[i].t_filter_nfy_info.pf_nfy == NULL)||
                                         (pt_detect_info->pt_component_list[i].t_ca_nfy_info.pf_nfy == NULL))                                         
                                 #endif   
                                    {
                                        b_valid = FALSE;
                                        break;
                                    }
                                 }    
                            }
                            else
                                b_valid = FALSE;
                        }
                    }                    
                    break;

            #if (PM_ENABLE_MULTIMEDIA)
                #if (PM_ENABLE_RECORD)
                case PM_RECORDING:
                    pt_recording_info = (PM_RECORDING_INFO_T* )pt_info;
                    if (pt_recording_info != NULL)
                    {
                        if ( pt_recording_info->ui4_list_size > 0)         
                        {
                            if (pt_recording_info->pt_component_list != NULL)
                            {
                                for (i = 0; i < pt_recording_info->ui4_list_size; i++)
                                {                                     
                                    switch (pt_recording_info->pt_component_list[i].e_es_info_type)
                                    {
                                        case PM_RECORDING_ES_WO_SETTING:
                                        #if (!PM_ENABLE_CA)    
                                            if  ((pmi_streamtype_valid(pt_recording_info->pt_component_list[i].u_es_info.t_stream_comp_id.e_type) == FALSE) ||
                                                 (pt_recording_info->pt_component_list[i].t_filter_nfy_info.pf_nfy == NULL))
                                        #else
                                            if  ((pmi_streamtype_valid(pt_recording_info->pt_component_list[i].u_es_info.t_stream_comp_id.e_type) == FALSE) ||
                                                 (pt_recording_info->pt_component_list[i].t_filter_nfy_info.pf_nfy == NULL)||
                                                 (pt_recording_info->pt_component_list[i].t_ca_nfy_info.pf_nfy == NULL) ||
                                                 (pt_recording_info->pt_component_list[i].t_cp_nfy_info.pf_nfy == NULL))                                         
                                        #endif   
                                            {
                                                PM_DBG_ERROR( "WO_SETTING component is invaild\n" );
                                                b_valid = FALSE;
                                            }                                        
                                            break;
                                        case PM_RECORDING_ES_WI_SETTING:
                                        #if (!PM_ENABLE_CA)    
                                            if  (pt_recording_info->pt_component_list[i].t_filter_nfy_info.pf_nfy == NULL)
                                        #else
                                            if  ((pt_recording_info->pt_component_list[i].t_filter_nfy_info.pf_nfy == NULL)||
                                                 (pt_recording_info->pt_component_list[i].t_ca_nfy_info.pf_nfy == NULL) ||
                                                 (pt_recording_info->pt_component_list[i].t_cp_nfy_info.pf_nfy == NULL))                                         
                                        #endif   
                                            {
                                                PM_DBG_ERROR( "WI_SETTING component is invaild\n" );
                                                b_valid = FALSE;
                                            }                                                        
                                            break;
                    
                                        case PM_RECORDING_ES_FILTER_ALLOCATED:
                                        #if (PM_ENABLE_CA)  
                                            if  ((pt_recording_info->pt_component_list[i].u_es_info.t_filter_setting.t_filter_handle == NULL_HANDLE) ||
                                                 (pt_recording_info->pt_component_list[i].t_ca_nfy_info.pf_nfy == NULL) ||
                                                 (pt_recording_info->pt_component_list[i].t_cp_nfy_info.pf_nfy == NULL)) 
                                            {
                                                b_valid = FALSE;
                                            }                                         
                                        #endif   
                                             
                                            break;
                                            
                                        default:
                                            PM_DBG_ERROR( "component type is invaild\n" );
                                            b_valid = FALSE;
                                            break;
                                    }
                                    
                                    if (b_valid == FALSE)
                                    {
                                        break;
                                    }
                                }    
                            }
                            else
                            {
                                PM_DBG_ERROR( "pt_component_list is NULL\n" );
                                b_valid = FALSE;
                            }
                        }
                    }                    
                    break;
                #endif
                    
                case PM_MULTIMEDIA:
                    pt_multimedia_info = (PM_MULTIMEDIA_INFO_T* )pt_info;
                    if (pt_multimedia_info != NULL)
                    {
                        if ( pt_multimedia_info->ui4_list_size > 0) 
                        {
                            if (pt_multimedia_info->pt_component_list != NULL)
                            {
                                for (i=0; i< pt_multimedia_info->ui4_list_size; i++)
                                {                                     
                                    switch (pt_multimedia_info->pt_component_list[i].e_es_info_type)
                                    {                                        
                                        case PM_MM_ES_WO_SETTING:                                                                                       
                                            if  ((pmi_streamtype_valid(pt_multimedia_info->pt_component_list[i].u_es_info.t_stream_comp_id.e_type) == FALSE) ||
                                                 (pt_multimedia_info->pt_component_list[i].t_filter_nfy_info.pf_nfy == NULL))                                        
                                            {
                                                b_valid = FALSE;
                                            }                                        
                                            break;
                                            
                                        case PM_MM_ES_WI_SETTING:                                            
                                            if  ((pmi_streamidtype_valid(pt_multimedia_info->pt_component_list[i].u_es_info.t_stream_setting.e_type) == FALSE) ||
                                                 (pt_multimedia_info->pt_component_list[i].t_filter_nfy_info.pf_nfy == NULL))                                        
                                            {
                                                b_valid = FALSE;
                                            }                                                        
                                            break; 
                                                                                        
                                        case PM_MM_ES_FILTER_ALLOCATED:
                                            if((pt_multimedia_info->pt_component_list[i].u_es_info.t_filter_setting.t_filter_handle == NULL_HANDLE))
                                            {
                                                b_valid = FALSE;
                                            }                                            
                                            break;
                                                                                        
                                        default:
                                            b_valid = FALSE;
                                            break;
                                    }
                                    
                                    if (b_valid == FALSE)
                                        break;
                                }    
                            }
                            else
                                b_valid = FALSE;
                        }
                    }
                    else    /* NULL == pt_multimedia_info */
                    {
                        b_valid = FALSE;
                    }
                    break;
            #endif
                default:
                    PM_DBG_ERROR( "service type is invaild\n" );
                    b_valid = FALSE;
                    break;
            } 
            
            if ( b_valid == TRUE )
            {
                if (ph_service != NULL)
                {
                    b_valid = TRUE;
                }
                else
                {
                    PM_DBG_ERROR( "ph_service is NULL\n" );
                    b_valid = FALSE; 
            }         
            }         
            
            if ((b_valid == TRUE) && (NULL != ph_service))
            {
                i4_return = PMR_FAILED;
                
                /* Lock */
                pmi_lock_db_wr();
                
                if ((pmi_svdb_alloc(&pt_svdb_node) == PMR_OK) && (NULL != pt_svdb_node))
                {
                    i4_return = PMR_OUT_OF_HANDLES;                    
                    
                    /* Bind Handle and Object */
                    if ((handle_alloc(PM_GROUP_PMAPI_SV, pt_svdb_node, pt_svdb_node, pm_service_handle_free_nfy_fct, ph_service) == HR_OK)&&
                        (handle_link(&SVNODE_LINK(pt_svdb_node), *ph_service ) == HR_OK))
                    {    
                        i4_return = PMR_FAILED;
                                            
                        /* Handle service layer */                        
                        SVNODE_CONNECTION_HANDLE(pt_svdb_node) = h_connection;
                        SVNODE_PROC_TYPE(pt_svdb_node) = e_prc_type; 
                        switch (e_prc_type)
                        {
                            case PM_PRESENT:
                            case PM_DETECTION:
                            #if (PM_ENABLE_CA)                            
                                pmi_ca_select_engine(SVNODE_CONNECTION_HANDLE(pt_svdb_node),
                                                     0, 
                                                     (PMCA_SYSNODE_T**)&(SVNODE_CA_CTRL(pt_svdb_node)));                                  
              
                            #endif 
                                break;
                                
                        #if (PM_ENABLE_MULTIMEDIA)
                            #if (PM_ENABLE_RECORD)
                            case PM_RECORDING:
                            #if (PM_ENABLE_CA)                            
                                pmi_ca_select_engine(SVNODE_CONNECTION_HANDLE(pt_svdb_node),
                                                     0, 
                                                     (PMCA_SYSNODE_T**)&(SVNODE_CA_CTRL(pt_svdb_node)));                                  
              
                            #endif 
                                break;
                            #endif
                                
                            case PM_MULTIMEDIA:
                                if (pt_multimedia_info != NULL)
                                {
                                    SVNODE_MMP_MEDIA_FORMAT(pt_svdb_node) = pt_multimedia_info->t_media_fmt;
                                    SVNODE_MMP_PRIORITY(pt_svdb_node) = pt_multimedia_info->e_priority;
                                    SVNODE_MMP_MEDIA_ES_WITH_PTS(pt_svdb_node)= pt_multimedia_info->b_es_with_pts_in;
                                }
                                break;
                        #endif
                            default:
                                break;
                        }  
                                   
                    
                        /* Handle ES list */
                        switch (e_prc_type)
                        {
                            case PM_PRESENT:
                            #if (PM_ALLOCATE_PCR)
                                /* Add PCR first */
                                i4_es_open_status = PMR_FAILED;    
                                
                                if ((pmi_esdb_alloc(&pt_esdb_node, PM_PRESENT) == PMR_OK) && 
                                   (pmi_db_insert_to_service(pt_esdb_node, pt_svdb_node)==PMR_OK))                                       
                                {
                                    ESNODE_DESCR_TYPE(pt_esdb_node) = ST_PCR;
                                    ESNODE_DESCR_TAG(pt_esdb_node) = 0;
                                    ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_UNKNOWN;
                                                                 
                                     /* query SCDB */
                                    if (pmi_scdb_query(pt_svdb_node, pt_esdb_node) == PMR_OK)
                                    {
                                    #if (!PM_ENABLE_CA)
                                        /* allocate demux */
                                        if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&        
                                            (pmi_filter_alloc(pt_svdb_node, DRVT_DEMUX_TS_PCR, pt_esdb_node) == PMR_OK))
                                        {   
                                            ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_DEMUX_TS_PCR;
                                            t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
                                            switch (t_cond)
                                            {
                                                case RM_COND_OPENED:
                                                    t_conn = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                                                    switch (t_conn)
                                                    {
                                                        case RM_COND_CONNECTED:  
                                                            ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                                            i4_es_open_status = PMR_OK;
                                                            break;                                                
                                                        case RM_COND_CONNECTING:
                                                            ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                            i4_es_open_status = PMR_OK;
                                                            break;
                                                        default:
                                                            PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                            break;
                                                    }
                                                    break;                                        
                                                case RM_COND_OPENING:
                                                    ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                    i4_es_open_status = PMR_OK;
                                                    break;
                                                default:
                                                    PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_OPEN_COND);
                                                    break;
                                            }  
                                        }
                                    #else
                                       /* allocate demux */
                                        if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&        
                                            (pmi_filter_alloc(pt_svdb_node, DRVT_DEMUX_TS_PCR, pt_esdb_node) == PMR_OK) &&
                                            (pmi_descrambler_alloc(pt_esdb_node) == PMR_OK))
                                        {   
                                            ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_DEMUX_TS_PCR;
                                            t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
                                            switch (t_cond)
                                            {
                                                case RM_COND_OPENED:
                                                    t_conn = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                                                    switch (t_conn)
                                                    {
                                                        case RM_COND_CONNECTED:  
                                                            ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                                            i4_es_open_status = PMR_OK;
                                                            break;                                                
                                                        case RM_COND_CONNECTING:
                                                            ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                            i4_es_open_status = PMR_OK;
                                                            break;
                                                        default:
                                                            PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                            break;
                                                    }
                                                    break;                                        
                                                case RM_COND_OPENING:
                                                    ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                    i4_es_open_status = PMR_OK;
                                                    break;
                                                default:
                                                    PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_OPEN_COND);
                                                    break;
                                            }  
                                        }                                    
                                    
                                    #endif
                                    }                                    
                                }
                                
                                if (i4_es_open_status != PMR_OK)
                                {
                                    if (pt_esdb_node != NULL)
                                        pm_db_parser_close_fct(pt_esdb_node); 
                                    break;
                                }
                                
                            #endif /* PM_ALLOCATE_PCR */
                                
                                /* allocate es node first */    
                                if (pt_broadcast_info != NULL)
                                {                    
                                    for (i=0; i< pt_broadcast_info->ui4_list_size; i++)
                                    {
                                        i4_es_open_status = PMR_FAILED;
                                        
                                        /* Bind Handle and Object */
                                        if ((pmi_esdb_alloc(&pt_esdb_node, PM_PRESENT) == PMR_OK) && (NULL != pt_esdb_node))
                                        {    
                                            i4_es_open_status = PMR_OUT_OF_HANDLES;    
                                            
                                            if ((handle_alloc(PM_GROUP_PMAPI_ES, pt_esdb_node, pt_esdb_node, pm_es_handle_free_nfy_fct, &(pt_broadcast_info->pt_component_list[i].h_component)) == HR_OK)&&
                                                (handle_link(&ESNODE_LINK(pt_esdb_node), pt_broadcast_info->pt_component_list[i].h_component ) == HR_OK))
                                            {
                                                i4_es_open_status = PMR_FAILED;
                                                
                                                /* Set default output */                                
                                                pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.e_cond = PM_COND_CLOSED;
                                                pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.ui4_detail_code = 0; 
                                                pt_broadcast_info->pt_component_list[i].t_ca_nfy_info.e_cond = PM_CA_COND_UNKNOWN;
                                                pt_broadcast_info->pt_component_list[i].t_ca_nfy_info.ui4_detail_code = 0; 
                                                x_memset(&(pt_broadcast_info->pt_component_list[i].t_cp_nfy_info.t_cp_info), 0, sizeof(CP_INFO_LIST_T));
                                                ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_UNKNOWN;
                                               
                                                /* Fill es info */
                                                ESNODE_REQUEST_TYPE(pt_esdb_node) = pt_broadcast_info->pt_component_list[i].e_es_info_type;
                                                switch (pt_broadcast_info->pt_component_list[i].e_es_info_type)
                                                {   
                                                    case PM_PRESENT_ES_WI_SETTING:
                                                        ESNODE_DESCR_TYPE(pt_esdb_node) = pt_broadcast_info->pt_component_list[i].u_es_info.t_stream_setting.e_type;       
                                                        ESNODE_PID(pt_esdb_node) = pt_broadcast_info->pt_component_list[i].u_es_info.t_stream_setting.t_pid; 
                                                        x_memcpy(&(ESNODE_SINK_INFO(pt_esdb_node)), &(pt_broadcast_info->pt_component_list[i].t_sink_comp_info), sizeof(DEMUX_DEST_INFO_T));
                                                        ESNODE_FILTER_NFY(pt_esdb_node) = pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.pf_nfy;
                                                        ESNODE_FILTER_TAG(pt_esdb_node) = pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.pv_tag;                                                        
                                                        break;
                                                        
                                                    case PM_PRESENT_ES_WO_SETTING:
                                                        x_memcpy(&(ESNODE_DESCR(pt_esdb_node)), &(pt_broadcast_info->pt_component_list[i].u_es_info.t_stream_comp_id), sizeof(STREAM_COMP_ID_T));                                                                                                                                                                                                        
                                                        x_memcpy(&(ESNODE_SINK_INFO(pt_esdb_node)), &(pt_broadcast_info->pt_component_list[i].t_sink_comp_info), sizeof(DEMUX_DEST_INFO_T));
                                                        ESNODE_FILTER_NFY(pt_esdb_node) = pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.pf_nfy;
                                                        ESNODE_FILTER_TAG(pt_esdb_node) = pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.pv_tag;
                                                        break;
                                                        
                                                    case PM_PRESENT_ES_FILTER_ALLOCATED:                                                        
                                                        ESNODE_PID(pt_esdb_node) = pt_broadcast_info->pt_component_list[i].u_es_info.t_filter_setting.t_pid; 
                                                        ESNODE_FILTER_HANDLE(pt_esdb_node) = pt_broadcast_info->pt_component_list[i].u_es_info.t_filter_setting.t_filter_handle;
                                                        break;
                                                    
                                                    default:
                                                        /* ToDo */
                                                        break;
                                                }                                                
                                                ESNODE_CA_NFY(pt_esdb_node) = pt_broadcast_info->pt_component_list[i].t_ca_nfy_info.pf_nfy;
                                                ESNODE_CA_TAG(pt_esdb_node) = pt_broadcast_info->pt_component_list[i].t_ca_nfy_info.pv_tag;
                                                ESNODE_CP_NFY(pt_esdb_node) = pt_broadcast_info->pt_component_list[i].t_cp_nfy_info.pf_nfy;
                                                ESNODE_CP_TAG(pt_esdb_node) = pt_broadcast_info->pt_component_list[i].t_cp_nfy_info.pv_tag;
                                                ESNODE_CA_REQ_LVL(pt_esdb_node) = pt_broadcast_info->pt_component_list[i].e_ca_require_lvl;
                                                
                                                /* Attach ES to Service */
                                                if (pmi_db_insert_to_service(pt_esdb_node, pt_svdb_node)==PMR_OK)
                                                {   
                                                    switch (pt_broadcast_info->pt_component_list[i].e_es_info_type)
                                                    {
                                                        case PM_PRESENT_ES_WI_SETTING:
                                                        case PM_PRESENT_ES_WO_SETTING:                                                                   
                                                            b_continue = TRUE;
                                                            if (pt_broadcast_info->pt_component_list[i].e_es_info_type == PM_PRESENT_ES_WO_SETTING)
                                                            {
                                                                if (pmi_scdb_query(pt_svdb_node, pt_esdb_node) != PMR_OK)
                                                                    b_continue = FALSE; 
                                                            }                             
                                                            /* query SCDB */
                                                            if (b_continue == TRUE)
                                                            {
                                                            #if (!PM_ENABLE_CA)
                                                                /* allocate demux */
                                                                if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&
                                                                    (pmi_filter_alloc(pt_svdb_node, DRVT_DEMUX_TS_PES_PACKET, pt_esdb_node) == PMR_OK))
                                                                {  
                                                                    ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_DEMUX_TS_PES_PACKET;
                                                                    t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
                                                                    switch (t_cond)
                                                                    {
                                                                        case RM_COND_OPENED:
                                                                            t_conn = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                                                                            switch (t_conn)
                                                                            {
                                                                                case RM_COND_CONNECTED:  
                                                                                    pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                                                                    #if (PM_ENABLE_FILTER_CTRL)
                                                                                    pt_broadcast_info->pt_component_list[i].t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                                    #endif
                                                                                    i4_es_open_status = PMR_OK;
                                                                                    break;                                                
                                                                                case RM_COND_CONNECTING:
                                                                                    pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                                    #if (PM_ENABLE_FILTER_CTRL)
                                                                                    pt_broadcast_info->pt_component_list[i].t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                                    #endif
                                                                                    i4_es_open_status = PMR_OK;
                                                                                    break;
                                                                                default:
                                                                                    PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                                                    break;
                                                                            }
                                                                            break;                                                    
                                                                        case RM_COND_OPENING:
                                                                            pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                            #if (PM_ENABLE_FILTER_CTRL)
                                                                            pt_broadcast_info->pt_component_list[i].t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                            #endif
                                                                            i4_es_open_status = PMR_OK;
                                                                            break;
                                                                        default:
                                                                            PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_OPEN_COND);
                                                                            break;
                                                                    }                                                     
                                                                }
                                                            #else
                                                                /* allocate demux */
                                                                if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&
                                                                    (pmi_filter_alloc(pt_svdb_node, DRVT_DEMUX_TS_PES_PACKET, pt_esdb_node) == PMR_OK)&&
                                                                    (pmi_descrambler_alloc(pt_esdb_node) == PMR_OK))
                                                                {  
                                                                    ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_DEMUX_TS_PES_PACKET;
                                                                    t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
                                                                    switch (t_cond)
                                                                    {
                                                                        case RM_COND_OPENED:
                                                                            t_conn = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                                                                            switch (t_conn)
                                                                            {
                                                                                case RM_COND_CONNECTED:  
                                                                                    pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                                                                    #if (PM_ENABLE_FILTER_CTRL)
                                                                                    pt_broadcast_info->pt_component_list[i].t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                                    #endif
                                                                                    i4_es_open_status = PMR_OK;
                                                                                    break;                                                
                                                                                case RM_COND_CONNECTING:
                                                                                    pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                                    #if (PM_ENABLE_FILTER_CTRL)
                                                                                    pt_broadcast_info->pt_component_list[i].t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                                    #endif
                                                                                    i4_es_open_status = PMR_OK;
                                                                                    break;
                                                                                default:
                                                                                    PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                                                    break;
                                                                            }
                                                                            break;                                                    
                                                                        case RM_COND_OPENING:
                                                                            pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                            #if (PM_ENABLE_FILTER_CTRL)
                                                                            pt_broadcast_info->pt_component_list[i].t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                            #endif
                                                                            i4_es_open_status = PMR_OK;
                                                                            break;
                                                                        default:
                                                                            PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_OPEN_COND);
                                                                            break;
                                                                    }                                                     
                                                                }                                                
                                                            
                                                            #endif
                                                            }                                                          
                                                            break;
                                                            
                                                        case PM_PRESENT_ES_FILTER_ALLOCATED:
                                                        #if (!PM_ENABLE_CA)
                                                            i4_es_open_status = PMR_OK; 
                                                        #else                                                            
                                                            /* allocate descrambler */
                                                            if (pmi_descrambler_alloc(pt_esdb_node) == PMR_OK)
                                                                i4_es_open_status = PMR_OK;    
                                                        #endif                                                        
                                                            break;
                                                        default:
                                                            break;
                                                    }   
                                                }                                                                             
                                            }                                                                       
                                        }                                         
                                        
                                        /* Free all ES related resource for open component failure */
                                        if (i4_es_open_status != PMR_OK)
                                        {
                                            if (pt_esdb_node != NULL)
                                                pm_db_parser_close_fct(pt_esdb_node); 
                                                
                                            pt_broadcast_info->pt_component_list[i].h_component = NULL_HANDLE;                                                              
                                            pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.e_cond = PM_COND_CLOSED;
                                            pt_broadcast_info->pt_component_list[i].t_filter_nfy_info.ui4_detail_code = 0; 
                                        }
                                    }
                                }                                    
                                
                                /* Always return PMR_OK if service node is generated successfully */
                                SVNODE_COND(pt_svdb_node) = PM_COND_OPEN;
                                i4_return = PMR_OK;
                                
                                break;
                                
                            case PM_DETECTION:
                                                            
                                /* allocate es node first */    
                                if (pt_detect_info != NULL)
                                {                    
                                    for (i=0; i< pt_detect_info->ui4_list_size; i++)
                                    {
                                        i4_es_open_status = PMR_FAILED;
                                        
                                        /* Bind Handle and Object */
                                        if (pmi_esdb_alloc(&pt_esdb_node, PM_DETECTION) == PMR_OK)                                
                                        {    
                                            i4_es_open_status = PMR_OUT_OF_HANDLES;    
                                            
                                            if ((handle_alloc(PM_GROUP_PMAPI_ES, pt_esdb_node, pt_esdb_node, pm_es_handle_free_nfy_fct, &(pt_detect_info->pt_component_list[i].h_component)) == HR_OK)&&
                                                (handle_link(&ESNODE_LINK(pt_esdb_node), pt_detect_info->pt_component_list[i].h_component ) == HR_OK))
                                            {
                                                i4_es_open_status = PMR_FAILED;
                                                
                                                /* Set default output */                                
                                                pt_detect_info->pt_component_list[i].t_filter_nfy_info.e_cond = PM_COND_CLOSED;
                                                pt_detect_info->pt_component_list[i].t_filter_nfy_info.ui4_detail_code = 0; 
                                                pt_detect_info->pt_component_list[i].t_ca_nfy_info.e_cond = PM_CA_COND_UNKNOWN;
                                                pt_detect_info->pt_component_list[i].t_ca_nfy_info.ui4_detail_code = 0;
                                                ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_UNKNOWN;
                                               
                                                /* Fill es info */
                                                ESNODE_DESCR_TYPE(pt_esdb_node) = pt_detect_info->pt_component_list[i].e_type;       
                                                ESNODE_PID(pt_esdb_node) = pt_detect_info->pt_component_list[i].t_pid;                                         
                                                ESNODE_FILTER_NFY(pt_esdb_node) = pt_detect_info->pt_component_list[i].t_filter_nfy_info.pf_nfy;
                                                ESNODE_FILTER_TAG(pt_esdb_node) = pt_detect_info->pt_component_list[i].t_filter_nfy_info.pv_tag;
                                                ESNODE_CA_NFY(pt_esdb_node) = pt_detect_info->pt_component_list[i].t_ca_nfy_info.pf_nfy;
                                                ESNODE_CA_TAG(pt_esdb_node) = pt_detect_info->pt_component_list[i].t_ca_nfy_info.pv_tag;
                                                
                                                /* Attach ES to Service */
                                                if (pmi_db_insert_to_service(pt_esdb_node, pt_svdb_node)==PMR_OK)
                                                {      
                                                #if (!PM_ENABLE_CA)
                                                    /* allocate demux */
                                                    if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&
                                                        (pmi_filter_alloc(pt_svdb_node, DRVT_DEMUX_TS_PES_PACKET, pt_esdb_node) == PMR_OK))
                                                    {  
                                                        ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_DEMUX_TS_PES_PACKET;
                                                        t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
                                                        switch (t_cond)
                                                        {
                                                            case RM_COND_OPENED:
                                                                t_conn = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                                                                switch (t_conn)
                                                                {
                                                                    case RM_COND_CONNECTED:  
                                                                        pt_detect_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                                                        #if (PM_ENABLE_FILTER_CTRL)
                                                                        pt_detect_info->pt_component_list[i].t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                        #endif
                                                                        i4_es_open_status = PMR_OK;
                                                                        break;                                                
                                                                    case RM_COND_CONNECTING:
                                                                        pt_detect_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                        #if (PM_ENABLE_FILTER_CTRL)
                                                                        pt_detect_info->pt_component_list[i].t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                        #endif
                                                                        i4_es_open_status = PMR_OK;
                                                                        break;
                                                                    default:
                                                                        PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                                        break;
                                                                }
                                                                break;                                                    
                                                            case RM_COND_OPENING:
                                                                pt_detect_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                #if (PM_ENABLE_FILTER_CTRL)
                                                                pt_detect_info->pt_component_list[i].t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                #endif
                                                                i4_es_open_status = PMR_OK;
                                                                break;
                                                            default:
                                                                PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_OPEN_COND);
                                                                break;
                                                        }                                                     
                                                    }
                                                #else
                                                    /* allocate demux */
                                                    if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&
                                                        (pmi_filter_alloc(pt_svdb_node, DRVT_DEMUX_TS_PES_PACKET, pt_esdb_node) == PMR_OK)&&
                                                        (pmi_descrambler_alloc(pt_esdb_node) == PMR_OK))
                                                    {  
                                                        ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_DEMUX_TS_PES_PACKET;
                                                        t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
                                                        switch (t_cond)
                                                        {
                                                            case RM_COND_OPENED:
                                                                t_conn = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                                                                switch (t_conn)
                                                                {
                                                                    case RM_COND_CONNECTED:  
                                                                        pt_detect_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                                                        #if (PM_ENABLE_FILTER_CTRL)
                                                                        pt_detect_info->pt_component_list[i].t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                        #endif
                                                                        i4_es_open_status = PMR_OK;
                                                                        break;                                                
                                                                    case RM_COND_CONNECTING:
                                                                        pt_detect_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                        #if (PM_ENABLE_FILTER_CTRL)
                                                                        pt_detect_info->pt_component_list[i].t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                        #endif
                                                                        i4_es_open_status = PMR_OK;
                                                                        break;
                                                                    default:
                                                                        PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                                        break;
                                                                }
                                                                break;                                                    
                                                            case RM_COND_OPENING:
                                                                pt_detect_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                #if (PM_ENABLE_FILTER_CTRL)
                                                                pt_detect_info->pt_component_list[i].t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                #endif                                                                
                                                                i4_es_open_status = PMR_OK;
                                                                break;
                                                            default:
                                                                PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_OPEN_COND);
                                                                break;
                                                        }                                                     
                                                    }                                                
                                                
                                                #endif                                      
                                                }                                                                             
                                            }                                                                       
                                        }                                         
                                        
                                        /* Free all ES related resource for open component failure */
                                        if (i4_es_open_status != PMR_OK)
                                        {
                                            if (pt_esdb_node != NULL)
                                                pm_db_parser_close_fct(pt_esdb_node); 
                                                
                                            pt_detect_info->pt_component_list[i].h_component = NULL_HANDLE;                                                              
                                            pt_detect_info->pt_component_list[i].t_filter_nfy_info.e_cond = PM_COND_CLOSED;
                                            pt_detect_info->pt_component_list[i].t_filter_nfy_info.ui4_detail_code = 0; 
                                        }
                                    }
                                }                                    
                                
                                if (pt_detect_info != NULL)
                                {
                                    if ((pt_detect_info->ui4_list_size > 0)&&(i4_es_open_status != PMR_OK))
                                        i4_return = PMR_FAILED;                                        
                                    else
                                    {
                                        SVNODE_COND(pt_svdb_node) = PM_COND_OPEN;
                                        i4_return = PMR_OK;                                        
                                    }    
                                }
                                else
                                {                                    
                                    SVNODE_COND(pt_svdb_node) = PM_COND_OPEN;
                                    i4_return = PMR_OK;
                                }
                                
                                break;

                        #if (PM_ENABLE_MULTIMEDIA)
                            #if (PM_ENABLE_RECORD)
                            case PM_RECORDING:
                                /* allocate es node first */    
                                if (pt_recording_info != NULL)
                                {                    
                                    for (i=0; i< pt_recording_info->ui4_list_size; i++)
                                    {
                                        i4_es_open_status = PMR_FAILED;
                                        
                                        /* Bind Handle and Object */
                                        if (pmi_esdb_alloc(&pt_esdb_node, PM_RECORDING) == PMR_OK)                                
                                        {    
                                            i4_es_open_status = PMR_OUT_OF_HANDLES;    
                                            
                                            if ((handle_alloc( PM_GROUP_PMAPI_ES, 
                                                               pt_esdb_node, 
                                                               pt_esdb_node, 
                                                               pm_es_handle_free_nfy_fct, 
                                                               &(pt_recording_info->pt_component_list[i].h_component)) == HR_OK) &&
                                                (handle_link(  &ESNODE_LINK(pt_esdb_node), 
                                                               pt_recording_info->pt_component_list[i].h_component ) == HR_OK))
                                            {
                                                i4_es_open_status = PMR_FAILED;
                                                
                                                /* Set default output */                                
                                                pt_recording_info->pt_component_list[i].t_filter_nfy_info.e_cond = PM_COND_CLOSED;
                                                pt_recording_info->pt_component_list[i].t_filter_nfy_info.ui4_detail_code = 0;
                                                pt_recording_info->pt_component_list[i].t_ca_nfy_info.e_cond = PM_CA_COND_UNKNOWN;
                                                pt_recording_info->pt_component_list[i].t_ca_nfy_info.ui4_detail_code = 0;
                                                x_memset(&(pt_recording_info->pt_component_list[i].t_cp_nfy_info.t_cp_info), 0, sizeof(CP_INFO_LIST_T));
                                                ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_UNKNOWN;
                                               
                                                /* Fill es info */
                                                ESNODE_REC_REQUEST_TYPE(pt_esdb_node) = pt_recording_info->pt_component_list[i].e_es_info_type;
                                                switch (pt_recording_info->pt_component_list[i].e_es_info_type)
                                                {   
                                                    case PM_RECORDING_ES_WI_SETTING:
                                                        ESNODE_REC_SETTING_INFO(pt_esdb_node) = pt_recording_info->pt_component_list[i].u_es_info.t_stream_setting.e_setting_info;
                                                        ESNODE_PID(pt_esdb_node)           = pt_recording_info->pt_component_list[i].u_es_info.t_stream_setting.t_pid; 
                                                        ESNODE_REC_STREAM_TYPE_MASK(pt_esdb_node) = pt_recording_info->pt_component_list[i].u_es_info.t_stream_setting.ui4_stream_type_mask;
                                                        x_memcpy( &(ESNODE_REC_SINK_INFO(pt_esdb_node)), &(pt_recording_info->pt_component_list[i].t_sink_comp_info), sizeof(RECDEMUX_DEST_INFO_T));
                                                        ESNODE_FILTER_NFY(pt_esdb_node)    = pt_recording_info->pt_component_list[i].t_filter_nfy_info.pf_nfy;
                                                        ESNODE_FILTER_TAG(pt_esdb_node)    = pt_recording_info->pt_component_list[i].t_filter_nfy_info.pv_tag;                                                        
                                                        break;
                                                        
                                                    case PM_RECORDING_ES_WO_SETTING:
                                                        x_memcpy( &(ESNODE_DESCR(pt_esdb_node)), &(pt_recording_info->pt_component_list[i].u_es_info.t_stream_comp_id), sizeof(STREAM_COMP_ID_T));                                                                                                                                                                                                        
                                                        x_memcpy( &(ESNODE_REC_SINK_INFO(pt_esdb_node)), &(pt_recording_info->pt_component_list[i].t_sink_comp_info), sizeof(RECDEMUX_DEST_INFO_T));
                                                        ESNODE_FILTER_NFY(pt_esdb_node)    = pt_recording_info->pt_component_list[i].t_filter_nfy_info.pf_nfy;
                                                        ESNODE_FILTER_TAG(pt_esdb_node)    = pt_recording_info->pt_component_list[i].t_filter_nfy_info.pv_tag;
                                                        break;
                                                        
                                                    case PM_RECORDING_ES_FILTER_ALLOCATED:                                                        
                                                        ESNODE_PID(pt_esdb_node)           = pt_recording_info->pt_component_list[i].u_es_info.t_filter_setting.t_pid; 
                                                        ESNODE_FILTER_HANDLE(pt_esdb_node) = pt_recording_info->pt_component_list[i].u_es_info.t_filter_setting.t_filter_handle;
                                                        break;
                                                    
                                                    default:
                                /* ToDo */
                                break;
                                                }                                                
                                                
                                                ESNODE_CA_NFY(pt_esdb_node) = pt_recording_info->pt_component_list[i].t_ca_nfy_info.pf_nfy;
                                                ESNODE_CA_TAG(pt_esdb_node) = pt_recording_info->pt_component_list[i].t_ca_nfy_info.pv_tag;
                                                ESNODE_CP_NFY(pt_esdb_node) = pt_recording_info->pt_component_list[i].t_cp_nfy_info.pf_nfy;
                                                ESNODE_CP_TAG(pt_esdb_node) = pt_recording_info->pt_component_list[i].t_cp_nfy_info.pv_tag;
                                                ESNODE_CA_REQ_LVL(pt_esdb_node) = pt_recording_info->pt_component_list[i].e_ca_require_lvl;
                                                
                                                /* Attach ES to Service */
                                                if (pmi_db_insert_to_service(pt_esdb_node, pt_svdb_node)==PMR_OK)
                                                {   
                                                    switch (pt_recording_info->pt_component_list[i].e_es_info_type)
                                                    {
                                                        case PM_RECORDING_ES_WI_SETTING:
                                                        case PM_RECORDING_ES_WO_SETTING:                                                                   
                                                            b_continue = TRUE;
                                                            if (pt_recording_info->pt_component_list[i].e_es_info_type == PM_RECORDING_ES_WO_SETTING)
                                                            {
                                                                if (pmi_scdb_query(pt_svdb_node, pt_esdb_node) != PMR_OK)
                                                                {
                                                                    b_continue = FALSE;
                                                                }
                                                            }                             
                                                            /* query SCDB */
                                                            if (b_continue == TRUE)
                                                            {
                                                                DRV_TYPE_T           t_filter_type = DRVT_DEMUX_REC;
                                                                
                                                                if (pt_recording_info->pt_component_list[i].e_es_info_type == PM_RECORDING_ES_WI_SETTING)
                                                                {
                                                                    if (PM_RECORDING_SETTING_PCR == ESNODE_REC_SETTING_INFO(pt_esdb_node))
                                                                    {
                                                                        t_filter_type = DRVT_DEMUX_REC_PCR;
                                                                    }
                                                                }

                                                            #if (!PM_ENABLE_CA)
                                                                /* allocate demux */
                                                                if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&
                                                                    (pmi_filter_alloc(pt_svdb_node, t_filter_type, pt_esdb_node) == PMR_OK))
                                                            #else
                                                                    /* allocate demux */
                                                                if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&
                                                                    (pmi_filter_alloc(pt_svdb_node, t_filter_type, pt_esdb_node) == PMR_OK)&&
                                                                    (pmi_descrambler_alloc(pt_esdb_node) == PMR_OK))
                                                            #endif
                                                                {  
                                                                    ESNODE_FILTER_TYPE(pt_esdb_node) = t_filter_type;
                                                                    t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
                                                                    switch (t_cond)
                                                                    {
                                                                        case RM_COND_OPENED:
                                                                            t_conn = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                                                                            switch (t_conn)
                                                                            {
                                                                                case RM_COND_CONNECTED:  
                                                                                    pt_recording_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                                                                    i4_es_open_status = PMR_OK;
                                                                                    break;                                                
                                                                                case RM_COND_CONNECTING:
                                                                                    pt_recording_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                                    i4_es_open_status = PMR_OK;
                                                                                    break;
                                                                                default:
                                                                                    PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                                                    break;
                                                                            }
                                                                            break;                                                    
                                                                        case RM_COND_OPENING:
                                                                            pt_recording_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                            i4_es_open_status = PMR_OK;
                                                                            break;
                                                                        default:
                                                                            PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_OPEN_COND);
                                                                            break;
                                                                    }                                                     
                                                                }
                                                                else
                                                                {
                                                                    PM_DBG_ERROR( "allocate filter for component FAIL(0x%x)\n", t_filter_type );
                                                                }
                                                            }                                                          
                                                            break;
                                                            
                                                        case PM_RECORDING_ES_FILTER_ALLOCATED:
                                                            i4_es_open_status = PMR_OK; 
                                                            break;
                                                        default:
                                                            break;
                                                    }   
                                                }                                                                             
                                            }                                                                       
                                        }                                         
                                        
                                        /* Free all ES related resource for open component failure */
                                        if (i4_es_open_status != PMR_OK)
                                        {
                                            if (pt_esdb_node != NULL)
                                            {
                                                pm_db_parser_close_fct(pt_esdb_node); 
                                            }
                                                
                                            pt_recording_info->pt_component_list[i].h_component                       = NULL_HANDLE;                                                              
                                            pt_recording_info->pt_component_list[i].t_filter_nfy_info.e_cond          = PM_COND_CLOSED;
                                            pt_recording_info->pt_component_list[i].t_filter_nfy_info.ui4_detail_code = 0; 
                                        }
                                    }
                                }                                    
                                
                                /* Always return PMR_OK if service node is generated successfully */
                                SVNODE_COND(pt_svdb_node) = PM_COND_OPEN;
                                i4_return = PMR_OK;
                                
                                break;
                            #endif
                            
                            case PM_MULTIMEDIA:
                                /* Add Demux control for service */
                                i4_es_open_status = PMR_FAILED;    
                                
                                if ((pmi_esdb_alloc(&pt_esdb_node, PM_MULTIMEDIA) == PMR_OK) && 
                                   (pmi_db_insert_to_service(pt_esdb_node, pt_svdb_node)==PMR_OK))                                       
                                {
                                    ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_UNKNOWN;
                                
                                    /* allocate demux */
                                    if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&        
                                        (pmi_filter_alloc(pt_svdb_node, DRVT_DEMUX_CONTROL, pt_esdb_node) == PMR_OK))
                                    {   
                                        ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_DEMUX_CONTROL;
                                        t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
                                        switch (t_cond)
                                        {
                                            case RM_COND_OPENED:
                                                t_conn = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                                                switch (t_conn)
                                                {
                                                    case RM_COND_CONNECTED:  
                                                        ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                                        i4_es_open_status = PMR_OK;
                                                        break;                                                
                                                    case RM_COND_CONNECTING:
                                                        ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                        i4_es_open_status = PMR_OK;
                                                        break;
                                                    default:
                                                        PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                        break;
                                                }
                                                break;                                        
                                            case RM_COND_OPENING:
                                                ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                i4_es_open_status = PMR_OK;
                                                break;
                                            default:
                                                PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_OPEN_COND);
                                                break;
                                        }  
                                    }                                                                        
                                }
                                
                                if (i4_es_open_status != PMR_OK)
                                {
                                    if (pt_esdb_node != NULL)
                                        pm_db_parser_close_fct(pt_esdb_node); 
                                    break;
                                }
                                
                                /* allocate es node first */    
                                if (pt_multimedia_info != NULL)
                                {                    
                                    for (i=0; i< pt_multimedia_info->ui4_list_size; i++)
                                    {
                                        i4_es_open_status = PMR_FAILED;
                                        
                                        /* Bind Handle and Object */
                                        if (pmi_esdb_alloc(&pt_esdb_node, PM_MULTIMEDIA) == PMR_OK)                                
                                        {    
                                            i4_es_open_status = PMR_OUT_OF_HANDLES;    
                                            
                                            if ((handle_alloc(PM_GROUP_PMAPI_ES, pt_esdb_node, pt_esdb_node, pm_es_handle_free_nfy_fct, &(pt_multimedia_info->pt_component_list[i].h_component)) == HR_OK)&&
                                                (handle_link(&ESNODE_LINK(pt_esdb_node), pt_multimedia_info->pt_component_list[i].h_component ) == HR_OK))
                                            {
                                                i4_es_open_status = PMR_FAILED;
                                                
                                                /* Set default output */                                
                                                pt_multimedia_info->pt_component_list[i].t_filter_nfy_info.e_cond = PM_COND_CLOSED;
                                                pt_multimedia_info->pt_component_list[i].t_filter_nfy_info.ui4_detail_code = 0;
                                                pt_multimedia_info->pt_component_list[i].t_ca_nfy_info.e_cond = PM_CA_COND_UNKNOWN;
                                                pt_multimedia_info->pt_component_list[i].t_ca_nfy_info.ui4_detail_code = 0;
                                                ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_UNKNOWN;
                                               
                                                /* Fill es info */
                                                ESNODE_MMP_REQUEST_TYPE(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].e_es_info_type;
                                                switch (pt_multimedia_info->pt_component_list[i].e_es_info_type)
                                                {                                                      
                                                    case PM_MM_ES_WI_SETTING:  
                                                        x_memcpy(&(ESNODE_SINK_INFO(pt_esdb_node)), &(pt_multimedia_info->pt_component_list[i].t_sink_comp_info), sizeof(DEMUX_DEST_INFO_T));
                                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].u_es_info.t_stream_setting.e_type;                                                        
                                                        x_memcpy(&(ESNODE_MMP_CRYPT_INFO(pt_esdb_node)), &(pt_multimedia_info->pt_component_list[i].t_crypt_info), sizeof(MM_CRYPT_INFO_T));
                                                        x_memcpy(&(ESNODE_MMP_CRYPT_STREAM(pt_esdb_node)), &(pt_multimedia_info->pt_component_list[i].u_es_info.t_stream_setting), sizeof(STREAM_ID_T));
                                                        switch(ESNODE_MMP_STREAM_TYPE(pt_esdb_node))
                                                        {
                                                            case STREAM_ID_TYPE_AVI:
                                                                ESNODE_MMP_STREAM_AVI_ID(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].u_es_info.t_stream_setting.u.t_stm_avi_id;
                                                                break;
                                                            case STREAM_ID_TYPE_ASF:
                                                                ESNODE_MMP_STREAM_ASF_ID(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].u_es_info.t_stream_setting.u.t_stm_asf_id;
                                                                break;
                                                            case STREAM_ID_TYPE_MP4:
                                                                ESNODE_MMP_STREAM_MP4_ID(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].u_es_info.t_stream_setting.u.t_stm_mp4_id;
                                                                break;
                                                            case STREAM_ID_TYPE_MKV:
                                                                ESNODE_MMP_STREAM_MKV_ID(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].u_es_info.t_stream_setting.u.t_stm_mkv_id;
                                                                break;
                                                            case STREAM_ID_TYPE_PS:
                                                                ESNODE_MMP_STREAM_PS_ID(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].u_es_info.t_stream_setting.u.t_stm_ps_id.ui2_stm_id;
                                                                break;
                                                            case STREAM_ID_TYPE_TS:
                                                                ESNODE_MMP_STREAM_TS_ID(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].u_es_info.t_stream_setting.u.t_stm_ts_id;
                                                                break;
                                                            case STREAM_ID_TYPE_RM:
                                                                ESNODE_MMP_STREAM_RM_ID(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].u_es_info.t_stream_setting.u.t_stm_rm_id;
                                                                break;
                                                            case STREAM_ID_TYPE_ES:
                                                                ESNODE_MMP_STREAM_ES_ID(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].u_es_info.t_stream_setting.u.t_stm_es_id;
                                                                break;
                                                            case STREAM_ID_TYPE_OGG:
                                                                ESNODE_MMP_STREAM_OGG_ID(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].u_es_info.t_stream_setting.u.t_stm_ogg_id;
                                                                break;
                                                            default:
                                                                /* ToDo */
                                                                break;                                                           
                                                        }
                                                        ESNODE_FILTER_NFY(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].t_filter_nfy_info.pf_nfy;
                                                        ESNODE_FILTER_TAG(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].t_filter_nfy_info.pv_tag;                                                        
                                                        break;
                                                        
                                                    case PM_MM_ES_WO_SETTING:
                                                        x_memcpy(&(ESNODE_MMP_DESCR(pt_esdb_node)), &(pt_multimedia_info->pt_component_list[i].u_es_info.t_stream_comp_id), sizeof(STREAM_COMP_ID_T));                                         
                                                        x_memcpy(&(ESNODE_SINK_INFO(pt_esdb_node)), &(pt_multimedia_info->pt_component_list[i].t_sink_comp_info), sizeof(DEMUX_DEST_INFO_T));
                                                        ESNODE_FILTER_NFY(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].t_filter_nfy_info.pf_nfy;
                                                        ESNODE_FILTER_TAG(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].t_filter_nfy_info.pv_tag;
                                                        ESNODE_MMP_CRYPT_TYPE(pt_esdb_node)     = MM_CRYPT_TYPE_PVR;
                                                        ESNODE_MMP_CRYPT_PVR_MODE(pt_esdb_node) = MM_CRYPT_MODE_INVALID;
                                                        break;       

                                                    case PM_MM_ES_FILTER_ALLOCATED:                                                        
                                                        x_memcpy(&(ESNODE_MMP_DESCR_SETTING(pt_esdb_node)), &(pt_multimedia_info->pt_component_list[i].u_es_info.t_filter_setting.t_stream_setting), sizeof(STREAM_ID_T));
                                                        ESNODE_FILTER_HANDLE(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].u_es_info.t_filter_setting.t_filter_handle;
                                                        x_memcpy(&(ESNODE_MMP_CRYPT_INFO(pt_esdb_node)), &(pt_multimedia_info->pt_component_list[i].t_crypt_info), sizeof(MM_CRYPT_INFO_T));
                                                        x_memcpy(&(ESNODE_MMP_CRYPT_STREAM(pt_esdb_node)), &(pt_multimedia_info->pt_component_list[i].u_es_info.t_stream_setting), sizeof(STREAM_ID_T));
                                                        break;       
                                                    
                                                    default:
                                                        /* ToDo */
                                                        break;
                                                }                                                                                                
                                                
                                                ESNODE_CA_NFY(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].t_ca_nfy_info.pf_nfy;
                                                ESNODE_CA_TAG(pt_esdb_node) = pt_multimedia_info->pt_component_list[i].t_ca_nfy_info.pv_tag;
                                                
                                                /* Attach ES to Service */
                                                if (pmi_db_insert_to_service(pt_esdb_node, pt_svdb_node)==PMR_OK)
                                                {   
                                                    switch (pt_multimedia_info->pt_component_list[i].e_es_info_type)
                                                    {
                                                        case PM_MM_ES_WI_SETTING:
                                                        case PM_MM_ES_WO_SETTING:                                                                   
                                                            b_continue = TRUE;
                                                            
                                                            if (pt_multimedia_info->pt_component_list[i].e_es_info_type == PM_MM_ES_WO_SETTING)
                                                            {
                                                                /* query SCDB */
                                                                if (pmi_scdb_query(pt_svdb_node, pt_esdb_node) != PMR_OK)
                                                                    b_continue = FALSE; 
                                                            }                             
                                                            
                                                            if (b_continue == TRUE)
                                                            {
                                                                /* allocate demux */
                                                                if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&
                                                                    (pmi_filter_alloc(pt_svdb_node, DRVT_DEMUX_CONTAINER_ES, pt_esdb_node) == PMR_OK))
                                                                {  
                                                                    ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_DEMUX_CONTAINER_ES;
                                                                    t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
                                                                    switch (t_cond)
                                                                    {
                                                                        case RM_COND_OPENED:
                                                                            t_conn = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                                                                            switch (t_conn)
                                                                            {
                                                                                case RM_COND_CONNECTED:  
                                                                                    pt_multimedia_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                                                                    i4_es_open_status = PMR_OK;
                                                                                    break;                                                
                                                                                case RM_COND_CONNECTING:
                                                                                    pt_multimedia_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                                    i4_es_open_status = PMR_OK;
                                                                                    break;
                                                                                default:
                                                                                    PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                                                    break;
                                                                            }
                                                                            break;                                                    
                                                                        case RM_COND_OPENING:
                                                                            pt_multimedia_info->pt_component_list[i].t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                            i4_es_open_status = PMR_OK;
                                                                            break;
                                                                        default:
                                                                            PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_OPEN_COND);
                                                                            break;
                                                                    }                                                     
                                                                }
                                                            
                                                            }                                                          
                                                            break;
                                                            
                                                        case PM_MM_ES_FILTER_ALLOCATED:
                                                            /* To Do. Add descryption function? */
                                                            i4_es_open_status = PMR_OK;
                                                            break;
                                                            
                                                        default:                                            
                                                            break;
                                                    }   
                                                }                                                                             
                                            }                                                                       
                                        }                                         
                                        
                                        /* Free all ES related resource for open component failure */
                                        if (i4_es_open_status != PMR_OK)
                                        {
                                            if (pt_esdb_node != NULL)
                                                pm_db_parser_close_fct(pt_esdb_node); 
                                                
                                            pt_multimedia_info->pt_component_list[i].h_component = NULL_HANDLE;                                                              
                                            pt_multimedia_info->pt_component_list[i].t_filter_nfy_info.e_cond = PM_COND_CLOSED;
                                            pt_multimedia_info->pt_component_list[i].t_filter_nfy_info.ui4_detail_code = 0; 
                                        }
                                    }
                                }                                    
                                
                                /* Always return PMR_OK if service node is generated successfully */
                                SVNODE_COND(pt_svdb_node) = PM_COND_OPEN;
                                i4_return = PMR_OK; 
                                break;
                        #endif
                            default:
                                break;
                        }        
                        
                    }
                    else
                    {
                        PM_DBG_ERROR( "alloc handle FAIL\n" );
                    }
                }
                else
                {
                    PM_DBG_ERROR( "alloc PM service node FAIL\n" );
                }
                
                if ((i4_return != PMR_OK)&&(pt_svdb_node != NULL))
                {
                    SVNODE_COND(pt_svdb_node) = PM_COND_CLOSE;  
                
                    /* Delink and Free Handle */
                    handle_free_all(&(SVNODE_LINK(pt_svdb_node)));              
                
                    /* Free SV node */
                    pmi_svdb_free(pt_svdb_node);    
                    
                    *ph_service = NULL_HANDLE;                  
                }
                
                /* Unlock */
                pmi_unlock_db();            
            } 
        }
        else
        {
            PM_DBG_ERROR( "CM handle is invaild\n" );
        }
    }
    else
    {
        PM_DBG_ERROR( "PM need to be initialized\n" );
    }
        
    PM_ERR_CODE(i4_return);
    return i4_return;       
}                      

/*-----------------------------------------------------------------------------
 * Name: pm_play_service
 *
 * Description: This API is responsible to play service.
 *
 * Inputs:  h_service        Contains the service handle.
 *          i4_speed         Contains the speed of play.
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     PM module is not initialized.
 *          PMR_INV_ARG      Invalid arguments. 
 *          PMR_INV_HANDLE   Invalid service handle.
 ----------------------------------------------------------------------------*/                                           
INT32 pm_play_service(HANDLE_T     h_service, 
                      INT32        i4_speed)
{
    PM_SERVICE_INFO_T*     pt_svdb_node;  
    INT32                  i4_return, i4_res;   
    HANDLE_TYPE_T          e_type;
    
    i4_return = PMR_NOT_INIT;
    
    if (b_pm_init)
    {
        i4_return = PMR_INV_HANDLE;
        
        /* Lock */
        pmi_lock_db_wr();
        
        i4_res = handle_get_type_obj(h_service, &e_type, (VOID *)(&pt_svdb_node));              
        
        if ((i4_res == HR_OK)&& (e_type == PM_GROUP_PMAPI_SV) && (SVNODE_COND(pt_svdb_node) != PM_COND_CLOSE ))        
        {
            i4_return = PMR_INV_ARG;
            
            if (pmi_speed_valid(pt_svdb_node, i4_speed) == TRUE)
            {
                SVNODE_SPEED(pt_svdb_node) = i4_speed;
                
                switch (SVNODE_PROC_TYPE(pt_svdb_node))
                {
                    case PM_PRESENT:
                    case PM_DETECTION:
                        if (pmi_db_parse_service(pt_svdb_node, pm_db_parser_play_fct) == PMR_OK)
                        {
                            SVNODE_COND(pt_svdb_node) = PM_COND_PLAY;   
                            
                         #if (!PM_ENABLE_CA)
                            i4_return = PMR_OK;
                         #else                    
                            i4_return = pmi_ca_start_service(pt_svdb_node);
                         #endif                                    
                        }
                        break;
                        
                #if (PM_ENABLE_MULTIMEDIA)     
                    #if (PM_ENABLE_RECORD)
                    case PM_RECORDING:
                        if (pmi_db_parse_service(pt_svdb_node, pm_db_parser_play_fct) == PMR_OK)
                        {
                            SVNODE_COND(pt_svdb_node) = PM_COND_PLAY;   
                        #if (!PM_ENABLE_CA)
                            i4_return = PMR_OK;
                        #else                    
                            i4_return = pmi_ca_start_service(pt_svdb_node);
                        #endif 
                        }
                        else
                        {
                            PM_DBG_ERROR( "parse service node FAIL\n" );
                        }
                        break;
                    #endif
                        
                    case PM_MULTIMEDIA:
                        if (pmi_db_parse_service(pt_svdb_node, pm_db_parser_play_fct) == PMR_OK)
                        {
                            if (pmi_db_parse_service(pt_svdb_node, pm_db_parser_play_ctrl_fct) == PMR_OK)
                            {
                                SVNODE_COND(pt_svdb_node) = PM_COND_PLAY;                                                       
                                i4_return = PMR_OK;
                            }                                                            
                        }
                        break;
                #endif  
                        
                    default:
                        break;    
                }            
            }
        }
        else
        {
            PM_DBG_ERROR( "handle is invaild\n" );
        }
        
        /* Unlock */
        pmi_unlock_db();    
    }     
    
    PM_ERR_CODE(i4_return);
    return i4_return;       
}      

/*-----------------------------------------------------------------------------
 * Name: pm_stop_service
 *
 * Description: This API is responsible to stop service.
 *
 * Inputs:  h_service        Contains the service handle.
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     PM module is not initialized.
 *          PMR_INV_HANDLE   Invalid service handle. 
 ----------------------------------------------------------------------------*/                     
INT32 pm_stop_service(HANDLE_T     h_service)
{
    PM_SERVICE_INFO_T*     pt_svdb_node;  
    INT32                  i4_return, i4_res;    
    HANDLE_TYPE_T          e_type;
    
    i4_return = PMR_NOT_INIT;
    
    if (b_pm_init)
    {
        i4_return = PMR_INV_HANDLE;
        
        /* Lock */
        pmi_lock_db_wr();
        
        i4_res = handle_get_type_obj(h_service, &e_type, (VOID *)(&pt_svdb_node));
        
        if ((i4_res == HR_OK)&&(e_type == PM_GROUP_PMAPI_SV) && (SVNODE_COND(pt_svdb_node) != PM_COND_CLOSE ))             
        {
            switch (SVNODE_PROC_TYPE(pt_svdb_node))
            {
                case PM_PRESENT:
                case PM_DETECTION:
                    i4_return = PMR_FAILED;
                    if (pmi_db_parse_service(pt_svdb_node, pm_db_parser_stop_fct) == PMR_OK)
                    {
                        SVNODE_COND(pt_svdb_node) = PM_COND_STOP;
                        i4_return = PMR_OK;
                    }                
                    break;

            #if (PM_ENABLE_MULTIMEDIA)        
                #if (PM_ENABLE_RECORD)
                case PM_RECORDING:
                    i4_return = PMR_FAILED;
                    if (pmi_db_parse_service(pt_svdb_node, pm_db_parser_stop_fct) == PMR_OK)
                    {
                        SVNODE_COND(pt_svdb_node) = PM_COND_STOP;
                        i4_return = PMR_OK;
                    }                
                    else
                    {
                        PM_DBG_ERROR( "parse service node FAIL\n" );
                    }
                    break;
                #endif
                    
                case PM_MULTIMEDIA:
                    i4_return = PMR_FAILED;
                    if (pmi_db_parse_service(pt_svdb_node, pm_db_parser_stop_ctrl_fct) == PMR_OK)
                    {
                        if (pmi_db_parse_service(pt_svdb_node, pm_db_parser_stop_fct) == PMR_OK)
                        {
                            SVNODE_COND(pt_svdb_node) = PM_COND_STOP;
                        
                            i4_return = PMR_OK;
                        }                
                    }                 
                    break;
            #endif
                    
                default:
                    break;
            }
        }
        else
        {
            PM_DBG_ERROR( "handle is invaild\n" );
        }
        
        /* Unlock */
        pmi_unlock_db();  
    }  
       
    PM_ERR_CODE(i4_return);
    return i4_return;       
}   

/*-----------------------------------------------------------------------------
 * Name: pm_close_service
 *
 * Description: This API is responsible to close service.
 *
 * Inputs:  h_service        Contains the service handle.
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     PM module is not initialized.
 *          PMR_INV_HANDLE   Invalid service handle. 
 ----------------------------------------------------------------------------*/  
INT32 pm_close_service(HANDLE_T     h_service)
{
    PM_SERVICE_INFO_T*     pt_svdb_node;
    INT32                  i4_return, i4_res;       
    HANDLE_TYPE_T          e_type;
    
    i4_return = PMR_NOT_INIT;
    
    if (b_pm_init)
    {
        i4_return = PMR_INV_HANDLE;
        
        /* Lock */
        pmi_lock_db_wr();
        
        i4_res = handle_get_type_obj(h_service, &e_type, (VOID *)(&pt_svdb_node));
        
        if ((i4_res == HR_OK)&&(e_type == PM_GROUP_PMAPI_SV) && (SVNODE_COND(pt_svdb_node) != PM_COND_CLOSE ))           
        {
            switch (SVNODE_PROC_TYPE(pt_svdb_node))
            {
                case PM_PRESENT:
                case PM_DETECTION:
                    i4_return = PMR_FAILED;
            
                #if (!PM_ENABLE_CA)
                    if (pmi_db_parse_service(pt_svdb_node, pm_db_parser_close_fct) == PMR_OK) 
                    {
                        SVNODE_COND(pt_svdb_node) = PM_COND_CLOSE;  
                        
                        /* Delink and Free Handle */
                        handle_free_all(&(SVNODE_LINK(pt_svdb_node)));              
                        
                        /* Free SV node */
                        pmi_svdb_free(pt_svdb_node);  
                        i4_return = PMR_OK;
                    }     
                        
                #else
                    if ((i4_return = pmi_ca_stop_service(pt_svdb_node)) == PMR_OK)
                    {
                        if (pmi_db_parse_service(pt_svdb_node, pm_db_parser_close_fct) == PMR_OK) 
                        {
                            SVNODE_COND(pt_svdb_node) = PM_COND_CLOSE;  
                            
                            /* Delink and Free Handle */
                            handle_free_all(&(SVNODE_LINK(pt_svdb_node)));              
                            
                            /* Free SV node */
                            pmi_svdb_free(pt_svdb_node);  
                            i4_return = PMR_OK;
                        }                     
                    }   
                #endif
                    break;

            #if (PM_ENABLE_MULTIMEDIA)                     
                #if (PM_ENABLE_RECORD)
                case PM_RECORDING:
                    i4_return = PMR_FAILED;
                    #if (PM_ENABLE_CA)
                    if ((i4_return = pmi_ca_stop_service(pt_svdb_node)) == PMR_OK)
                    #endif
                    {
                        if (pmi_db_parse_service(pt_svdb_node, pm_db_parser_close_fct) == PMR_OK) 
                        {
                            SVNODE_COND(pt_svdb_node) = PM_COND_CLOSE;  
                            
                            /* Delink and Free Handle */
                            handle_free_all(&(SVNODE_LINK(pt_svdb_node)));              
                            
                            /* Free SV node */
                            pmi_svdb_free(pt_svdb_node);  
                            i4_return = PMR_OK;
                        } 
                        else
                        {
                            PM_DBG_ERROR( "parse service node FAIL\n" );
                        }
                    }
                    
                    break;
                #endif

                case PM_MULTIMEDIA:
                    i4_return = PMR_FAILED;            
                
                    if (pmi_db_parse_service(pt_svdb_node, pm_db_parser_close_fct) == PMR_OK) 
                    {
                        SVNODE_COND(pt_svdb_node) = PM_COND_CLOSE;  
                        
                        /* Delink and Free Handle */
                        handle_free_all(&(SVNODE_LINK(pt_svdb_node)));              
                        
                        /* Free SV node */
                        pmi_svdb_free(pt_svdb_node);  
                        i4_return = PMR_OK;
                    }     
                    break;
            #endif
                    
                default:
                    break;                
            }
        }
        else
        {
            PM_DBG_ERROR( "handle is invaild\n" );
        }
        
        /* Unlock */
        pmi_unlock_db();   
    }    
      
    PM_ERR_CODE(i4_return);
    return i4_return;  
}   

/*-----------------------------------------------------------------------------
 * Name: pm_get_service_cond
 *
 * Description: This API is responsible to get the current service condition.
 *
 * Inputs:  h_service        Contains the service handle.
 * Outputs: pe_cond          Contains the reference to the returned service
 *                           condition.
 *          i4_speed         Contains the reference to the returned speed.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     PM module is not initialized.
 *          PMR_INV_HANDLE   h_service is invalid handle.
 *          PMR_INV_ARG      Invalid arguments. 
 ----------------------------------------------------------------------------*/  
INT32 pm_get_service_cond(HANDLE_T           h_service,
                          PM_SERVICE_COND_T* pe_cond,
                          INT32*             pi4_speed)
{   
    PM_SERVICE_INFO_T*     pt_svdb_node;
    INT32                  i4_return, i4_res;
    HANDLE_TYPE_T          e_type;
    
    i4_return = PMR_NOT_INIT;
    
    if (b_pm_init)
    {
        i4_return = PMR_INV_HANDLE;
        
        if (x_handle_valid(h_service) == TRUE)
        {
                
            i4_return = PMR_INV_ARG;
                     
            /* Lock */
            pmi_lock_db_wr();
                    
            if ((pe_cond != NULL) &&(pi4_speed != NULL))
	    {           
                i4_res = handle_get_type_obj(h_service, &e_type, (VOID *)(&pt_svdb_node));
                    
                if ((i4_res == HR_OK)&&(e_type == PM_GROUP_PMAPI_SV) && (SVNODE_COND(pt_svdb_node) != PM_COND_CLOSE ))   
                {      
                    *pe_cond = SVNODE_COND(pt_svdb_node);   
                    *pi4_speed = SVNODE_SPEED(pt_svdb_node);  
                    i4_return = PMR_OK;            
                }
            }
            else
            {
                PM_DBG_ERROR( "parameter is invaild\n" );
            }

            /* Unlock */
            pmi_unlock_db();
        }
        else
        {
            PM_DBG_ERROR( "handle is invaild\n" );
        }        
    }       
    
    PM_ERR_CODE(i4_return);
    return i4_return;    
} 
/*-----------------------------------------------------------------------------
 * Name: pm_add_component
 *
 * Description: This API is responsible to attach ES to the service.
 *
 * Inputs:  h_service        Contains the service handle.
 *          pt_componen      Contains the reference to ES request setting.
 * Outputs: pt_componen      Contains the reference to ES request setting and 
 *                           the returned updated status.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     PM module is not initialized.
 *          PMR_INV_HANDLE   Invalid service handle.
 *          PMR_INV_ARG      Invalid arguments. 
 *          PMR_FAILED       Failed due to other reason. 
 ----------------------------------------------------------------------------*/                                  
INT32 pm_add_component(HANDLE_T             h_service,
                       VOID*                pt_component_info)
{
    /* here */
    PM_SERVICE_INFO_T*      pt_svdb_node = NULL;
    PM_COMPONENT_INFO_T*     pt_esdb_node = NULL; 
    INT32                    i4_return;
    HANDLE_TYPE_T            e_type;
    RM_COND_T                t_cond, t_conn;
    BOOL                     b_valid, b_continue;   
    PM_ES_REQUEST_T*         pt_component = NULL;   
    PM_ES_WO_SINK_REQUEST_T* pt_component_wo_sink = NULL;  
       
#if (PM_ENABLE_MULTIMEDIA)   
    #if (PM_ENABLE_RECORD)
    PM_REC_ES_REQUEST_T*     pt_rec_component = NULL;
    #endif
    PM_MM_REQUEST_T*         pt_mm_component_info = NULL; 
#endif
   
    i4_return = PMR_NOT_INIT;
        
    if (b_pm_init)
    {
        i4_return = PMR_INV_ARG;
                     
        /* Check parameters */
        if (pt_component_info == NULL)
        {
            PM_DBG_ERROR("Invalid input for pm add component\n");
            PM_ERR_CODE(i4_return);
            return i4_return;
        }        
        
        i4_return = PMR_INV_HANDLE;    
        
        /* Lock */
        pmi_lock_db_wr();
        
        if ((x_handle_valid (h_service) == TRUE) &&
            (handle_get_type_obj(h_service, &e_type, (VOID *)(&pt_svdb_node)) == HR_OK) &&
            (pt_svdb_node != NULL) &&
            (e_type == PM_GROUP_PMAPI_SV))
        {        
            
            i4_return = PMR_INV_ARG;    
            
            /* Check parameters */ 
            b_valid = TRUE;
            switch (SVNODE_PROC_TYPE(pt_svdb_node))
            {
                case PM_PRESENT:
                    pt_component = (PM_ES_REQUEST_T*)pt_component_info;
                    
                    switch (pt_component->e_es_info_type)
                    {
                        case PM_PRESENT_ES_WO_SETTING:
                        #if (!PM_ENABLE_CA)    
                            if  ((pmi_streamtype_valid(pt_component->u_es_info.t_stream_comp_id.e_type) == FALSE) ||
                                 (pt_component->t_filter_nfy_info.pf_nfy == NULL))
                        #else
                            if  ((pmi_streamtype_valid(pt_component->u_es_info.t_stream_comp_id.e_type) == FALSE) ||
                                 (pt_component->t_filter_nfy_info.pf_nfy == NULL)||
                                 (pt_component->t_ca_nfy_info.pf_nfy == NULL) ||
                                 (pt_component->t_cp_nfy_info.pf_nfy == NULL))                                         
                        #endif   
                            {
                                b_valid = FALSE;
                            }                                        
                            break;
                            
                        case PM_PRESENT_ES_WI_SETTING:
                        #if (!PM_ENABLE_CA)    
                            if  ((pmi_streamtype_valid(pt_component->u_es_info.t_stream_setting.e_type) == FALSE) ||
                                 (pt_component->t_filter_nfy_info.pf_nfy == NULL))
                        #else
                            if  ((pmi_streamtype_valid(pt_component->u_es_info.t_stream_setting.e_type) == FALSE) ||
                                 (pt_component->t_filter_nfy_info.pf_nfy == NULL)||
                                 (pt_component->t_ca_nfy_info.pf_nfy == NULL) ||
                                 (pt_component->t_cp_nfy_info.pf_nfy == NULL))                                         
                        #endif   
                            {
                                b_valid = FALSE;
                            }                                                        
                            break;
                            
                        case PM_PRESENT_ES_FILTER_ALLOCATED:
                        #if (PM_ENABLE_CA)  
                            if  ((pt_component->u_es_info.t_filter_setting.t_filter_handle == NULL_HANDLE) ||
                                 (pt_component->t_ca_nfy_info.pf_nfy == NULL) ||
                                 (pt_component->t_cp_nfy_info.pf_nfy == NULL))                                         
                          
                            {
                                b_valid = FALSE;
                            }  
                        #endif 
                            break;                        
                       
                        default:
                            b_valid = FALSE;
                            break;
                    }   
                    break;
                    
                case PM_DETECTION:                
                    pt_component_wo_sink = (PM_ES_WO_SINK_REQUEST_T*)pt_component_info;                                  
                     
                #if (!PM_ENABLE_CA)
                    if ((pt_component_wo_sink != NULL)&& 
                        (pt_component_wo_sink->t_filter_nfy_info.pf_nfy != NULL) &&
                        (pmi_streamtype_valid(pt_component_wo_sink->e_type) == TRUE))
                #else
                    if ((pt_component_wo_sink != NULL)&& 
                        (pt_component_wo_sink->t_filter_nfy_info.pf_nfy != NULL) &&
                        (pmi_streamtype_valid(pt_component_wo_sink->e_type) == TRUE) &&
                        (pt_component_wo_sink->t_ca_nfy_info.pf_nfy != NULL)) 
                #endif
                        b_valid = TRUE;
                    else
                        b_valid = FALSE;
                    break;   

            #if (PM_ENABLE_MULTIMEDIA)        
                #if (PM_ENABLE_RECORD)
                case PM_RECORDING:
                    pt_rec_component = (PM_REC_ES_REQUEST_T*)pt_component_info;
                    
                    switch (pt_rec_component->e_es_info_type)
                    {
                        case PM_RECORDING_ES_WO_SETTING:
                        #if (!PM_ENABLE_CA)    
                            if ((pmi_streamtype_valid(pt_rec_component->u_es_info.t_stream_comp_id.e_type) == FALSE) ||
                                 (pt_rec_component->t_filter_nfy_info.pf_nfy == NULL))
                        #else
                            if  ((pmi_streamtype_valid(pt_rec_component->u_es_info.t_stream_comp_id.e_type) == FALSE) ||
                                 (pt_rec_component->t_filter_nfy_info.pf_nfy == NULL)||
                                 (pt_rec_component->t_ca_nfy_info.pf_nfy == NULL) ||
                                 (pt_rec_component->t_cp_nfy_info.pf_nfy == NULL))                                         
                        #endif   
                            {
                                PM_DBG_ERROR( "WO_SETTING component is invaild\n" );
                                b_valid = FALSE;
                            }                                        
                            break;
                            
                        case PM_RECORDING_ES_WI_SETTING:
                        #if (!PM_ENABLE_CA)    
                            if (pt_rec_component->t_filter_nfy_info.pf_nfy == NULL)
                        #else
                            if  ((pt_rec_component->t_filter_nfy_info.pf_nfy == NULL)||
                                 (pt_rec_component->t_ca_nfy_info.pf_nfy == NULL) ||
                                 (pt_rec_component->t_cp_nfy_info.pf_nfy == NULL))                                         
                        #endif   
                            {
                                PM_DBG_ERROR( "WI_SETTING component is invaild\n" );
                                b_valid = FALSE;
                            }                                                        
                            break;
                            
                        case PM_RECORDING_ES_FILTER_ALLOCATED:
                        #if (PM_ENABLE_CA)  
                            if  ((pt_rec_component->u_es_info.t_filter_setting.t_filter_handle == NULL_HANDLE) ||
                                 (pt_rec_component->t_ca_nfy_info.pf_nfy == NULL) ||
                                 (pt_rec_component->t_cp_nfy_info.pf_nfy == NULL))                                         
                          
                            {
                                b_valid = FALSE;
                            }  
                        #endif 
                            break;                        
                       
                        default:
                            PM_DBG_ERROR( "component type is invaild\n" );
                            b_valid = FALSE;
                            break;
                    }   
                    break;
                #endif
                    
                case PM_MULTIMEDIA:
                    pt_mm_component_info = (PM_MM_REQUEST_T* )pt_component_info;
                    
                    switch (pt_mm_component_info->e_es_info_type)
                    {                                        
                        case PM_MM_ES_WO_SETTING:                                                                                       
                            if  ((pmi_streamtype_valid(pt_mm_component_info->u_es_info.t_stream_comp_id.e_type) == FALSE) ||
                                 (pt_mm_component_info->t_filter_nfy_info.pf_nfy == NULL))                                        
                            {
                                b_valid = FALSE;
                            }                                        
                            break;
                            
                        case PM_MM_ES_WI_SETTING:                                            
                            if  ((pmi_streamidtype_valid(pt_mm_component_info->u_es_info.t_stream_setting.e_type) == FALSE) ||
                                 (pt_mm_component_info->t_filter_nfy_info.pf_nfy == NULL))                                        
                            {
                                b_valid = FALSE;
                            }                                                        
                            break; 
                                                                        
                        case PM_MM_ES_FILTER_ALLOCATED:
                            if((pt_mm_component_info->u_es_info.t_filter_setting.t_filter_handle == NULL_HANDLE))
                            {
                                b_valid = FALSE;
                            }
                            break;
                                                                        
                        default:
                            b_valid = FALSE;
                            break;
                    }
                    break;               
            #endif                
                default:
                    PM_DBG_ERROR( "service node type is invaild\n" );
                    b_valid = FALSE;
            } 
                     
            if (b_valid == TRUE)
            {      
                i4_return = PMR_FAILED;
                
                /* Bind Handle and Object */
                if ((pmi_esdb_alloc(&pt_esdb_node, SVNODE_PROC_TYPE(pt_svdb_node)) == PMR_OK) && (NULL != pt_esdb_node))
                {  
                    i4_return = PMR_OUT_OF_HANDLES;                    
                    
                    switch (SVNODE_PROC_TYPE(pt_svdb_node))
                    {
                        case PM_PRESENT:
                            if ((NULL != pt_component)&&
                                (handle_alloc(PM_GROUP_PMAPI_ES, pt_esdb_node, pt_esdb_node, pm_es_handle_free_nfy_fct, &(pt_component->h_component)) == HR_OK)&&
                                (handle_link(&ESNODE_LINK(pt_esdb_node), pt_component->h_component ) == HR_OK))
                            {
                                /* Set default output */                                
                                pt_component->t_filter_nfy_info.e_cond = PM_COND_CLOSED;
                                pt_component->t_filter_nfy_info.ui4_detail_code = 0; 
                                pt_component->t_ca_nfy_info.e_cond = PM_CA_COND_UNKNOWN;
                                pt_component->t_ca_nfy_info.ui4_detail_code = 0; 
                                x_memset(&(pt_component->t_cp_nfy_info.t_cp_info), 0, sizeof(CP_INFO_LIST_T));
                                ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_UNKNOWN;
                                
                                /* Fill es info */
                                ESNODE_REQUEST_TYPE(pt_esdb_node) = pt_component->e_es_info_type;
                                switch (pt_component->e_es_info_type)
                                {   
                                    case PM_PRESENT_ES_WI_SETTING:
                                        ESNODE_DESCR_TYPE(pt_esdb_node) = pt_component->u_es_info.t_stream_setting.e_type;       
                                        ESNODE_PID(pt_esdb_node) = pt_component->u_es_info.t_stream_setting.t_pid; 
                                        x_memcpy(&(ESNODE_SINK_INFO(pt_esdb_node)), &(pt_component->t_sink_comp_info), sizeof(DEMUX_DEST_INFO_T));
                                        ESNODE_FILTER_NFY(pt_esdb_node) = pt_component->t_filter_nfy_info.pf_nfy;
                                        ESNODE_FILTER_TAG(pt_esdb_node) = pt_component->t_filter_nfy_info.pv_tag;                                                        
                                        break;
                                        
                                    case PM_PRESENT_ES_WO_SETTING:
                                        x_memcpy(&(ESNODE_DESCR(pt_esdb_node)), &(pt_component->u_es_info.t_stream_comp_id), sizeof(STREAM_COMP_ID_T));                                                                                                                                                                                                        
                                        x_memcpy(&(ESNODE_SINK_INFO(pt_esdb_node)), &(pt_component->t_sink_comp_info), sizeof(DEMUX_DEST_INFO_T));
                                        ESNODE_FILTER_NFY(pt_esdb_node) = pt_component->t_filter_nfy_info.pf_nfy;
                                        ESNODE_FILTER_TAG(pt_esdb_node) = pt_component->t_filter_nfy_info.pv_tag;
                                        break;
                                        
                                    case PM_PRESENT_ES_FILTER_ALLOCATED:                                              
                                        ESNODE_PID(pt_esdb_node) = pt_component->u_es_info.t_filter_setting.t_pid; 
                                        ESNODE_FILTER_HANDLE(pt_esdb_node) = pt_component->u_es_info.t_filter_setting.t_filter_handle;
                                        break;
                                    
                                    default:
                                        /* ToDo */
                                        break;
                                }                                                   
                                
                                ESNODE_CA_NFY(pt_esdb_node) = pt_component->t_ca_nfy_info.pf_nfy;
                                ESNODE_CA_TAG(pt_esdb_node) = pt_component->t_ca_nfy_info.pv_tag;
                                ESNODE_CP_NFY(pt_esdb_node) = pt_component->t_cp_nfy_info.pf_nfy;
                                ESNODE_CP_TAG(pt_esdb_node) = pt_component->t_cp_nfy_info.pv_tag;
                                ESNODE_CA_REQ_LVL(pt_esdb_node) = pt_component->e_ca_require_lvl;
                                
                                i4_return = PMR_FAILED;
                            
                                /* Attach ES to Service */
                                if (pmi_db_insert_to_service(pt_esdb_node, pt_svdb_node)==PMR_OK)
                                {  
                                    switch (pt_component->e_es_info_type)
                                    {
                                        case PM_PRESENT_ES_WI_SETTING:
                                        case PM_PRESENT_ES_WO_SETTING:  
                                            b_continue = TRUE;
                                            if (pt_component->e_es_info_type == PM_PRESENT_ES_WO_SETTING)
                                            {
                                                /* query SCDB */
                                                if (pmi_scdb_query(pt_svdb_node, pt_esdb_node) != PMR_OK)
                                                    b_continue = FALSE; 
                                            }                             
                                            
                                            if (b_continue == TRUE)
                                            {
                                            #if (!PM_ENABLE_CA)                           
                                                /* allocate demux */
                                                if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&
                                                    (pmi_filter_alloc(pt_svdb_node, DRVT_DEMUX_TS_PES_PACKET, pt_esdb_node) == PMR_OK))
                                                {
                                                    ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_DEMUX_TS_PES_PACKET;
                                                    t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
                                                    switch (t_cond)
                                                    {
                                                        case RM_COND_OPENED:
                                                            t_conn = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                                                            switch (t_conn)
                                                            {
                                                                case RM_COND_CONNECTED:  
                                                                    pt_component->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                                                    #if (PM_ENABLE_FILTER_CTRL)
                                                                    pt_component->t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                    #endif
                                                                    i4_return = PMR_OK;   
                                                                    break;                                                
                                                                case RM_COND_CONNECTING:
                                                                    pt_component->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                    #if (PM_ENABLE_FILTER_CTRL)
                                                                    pt_component->t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                    #endif
                                                                    i4_return = PMR_OK;  
                                                                    break;
                                                                default:
                                                                    PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                                    break;
                                                            }
                                                            break;                                    
                                                        case RM_COND_OPENING:
                                                            pt_component->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                            #if (PM_ENABLE_FILTER_CTRL)
                                                            pt_component->t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                            #endif
                                                            i4_return = PMR_OK;  
                                                            break;
                                                        default:
                                                            x_dbg_stmt( "[PM]the condition(%d) is invalid\n", t_cond );
                                                            PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                            break;
                                                    }     
                                                }
                                            #else
                                                /* allocate demux */
                                                if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&
                                                    (pmi_filter_alloc(pt_svdb_node, DRVT_DEMUX_TS_PES_PACKET, pt_esdb_node) == PMR_OK) &&
                                                    (pmi_descrambler_alloc(pt_esdb_node) == PMR_OK))
                                                {
                                                    ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_DEMUX_TS_PES_PACKET;
                                                    t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
                                                    switch (t_cond)
                                                    {
                                                        case RM_COND_OPENED:
                                                            t_conn = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                                                            switch (t_conn)
                                                            {
                                                                case RM_COND_CONNECTED:  
                                                                    pt_component->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                                                    #if (PM_ENABLE_FILTER_CTRL)
                                                                    pt_component->t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                    #endif
                                                                    i4_return = PMR_OK;   
                                                                    break;                                                
                                                                case RM_COND_CONNECTING:
                                                                    pt_component->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                    #if (PM_ENABLE_FILTER_CTRL)
                                                                    pt_component->t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                                    #endif
                                                                    i4_return = PMR_OK;  
                                                                    break;
                                                                default:
                                                                    PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                                    break;
                                                            }
                                                            break;                                    
                                                        case RM_COND_OPENING:
                                                            pt_component->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                            #if (PM_ENABLE_FILTER_CTRL)
                                                            pt_component->t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                            #endif
                                                            i4_return = PMR_OK;  
                                                            break;
                                                        default:
                                                            PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                            break;
                                                    }     
                                                }                            
                                            #endif
                                            }                                      
                                            break;
                                            
                                        case PM_PRESENT_ES_FILTER_ALLOCATED:
                                        #if (!PM_ENABLE_CA)
                                            i4_return = PMR_OK;   
                                        #else                                                            
                                            /* allocate descrambler */
                                            if (pmi_descrambler_alloc(pt_esdb_node) == PMR_OK)
                                                i4_return = PMR_OK;      
                                        #endif                                                      
                                            break;
                                        default:
                                            break;
                                    }  
                                }
                            }                                
                            
                            break;
                            
                        case PM_DETECTION:          
                            if ((NULL != pt_component_wo_sink)&&
                                (handle_alloc(PM_GROUP_PMAPI_ES, pt_esdb_node, pt_esdb_node, pm_es_handle_free_nfy_fct, &(pt_component_wo_sink->h_component)) == HR_OK)&&
                                (handle_link(&ESNODE_LINK(pt_esdb_node), pt_component_wo_sink->h_component ) == HR_OK))
                            {
                                /* Set default output */                                
                                pt_component_wo_sink->t_filter_nfy_info.e_cond = PM_COND_CLOSED;
                                pt_component_wo_sink->t_filter_nfy_info.ui4_detail_code = 0; 
                                pt_component_wo_sink->t_ca_nfy_info.e_cond = PM_CA_COND_UNKNOWN;
                                pt_component_wo_sink->t_ca_nfy_info.ui4_detail_code = 0;
                                ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_UNKNOWN;
                               
                                /* Fill es info */
                                ESNODE_DESCR_TYPE(pt_esdb_node) = pt_component_wo_sink->e_type;
                                ESNODE_PID(pt_esdb_node) = pt_component_wo_sink->t_pid; 
                                ESNODE_FILTER_NFY(pt_esdb_node) = pt_component_wo_sink->t_filter_nfy_info.pf_nfy;
                                ESNODE_FILTER_TAG(pt_esdb_node) = pt_component_wo_sink->t_filter_nfy_info.pv_tag;
                                ESNODE_CA_NFY(pt_esdb_node) = pt_component_wo_sink->t_ca_nfy_info.pf_nfy;
                                ESNODE_CA_TAG(pt_esdb_node) = pt_component_wo_sink->t_ca_nfy_info.pv_tag;   
                                
                                i4_return = PMR_FAILED;
                            
                                /* Attach ES to Service */
                                if (pmi_db_insert_to_service(pt_esdb_node, pt_svdb_node)==PMR_OK)
                                {  
                                    
                                #if (!PM_ENABLE_CA)                           
                                    /* allocate demux */
                                    if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&
                                        (pmi_filter_alloc(pt_svdb_node, DRVT_DEMUX_TS_PES_PACKET, pt_esdb_node) == PMR_OK))
                                    {
                                        ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_DEMUX_TS_PES_PACKET;
                                        t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
                                        switch (t_cond)
                                        {
                                            case RM_COND_OPENED:
                                                t_conn = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                                                switch (t_conn)
                                                {
                                                    case RM_COND_CONNECTED:  
                                                        pt_component_wo_sink->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                                        #if (PM_ENABLE_FILTER_CTRL)
                                                        pt_component_wo_sink->t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                        #endif
                                                        i4_return = PMR_OK;   
                                                        break;                                                
                                                    case RM_COND_CONNECTING:
                                                        pt_component_wo_sink->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                        #if (PM_ENABLE_FILTER_CTRL)
                                                        pt_component_wo_sink->t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                        #endif
                                                        i4_return = PMR_OK;  
                                                        break;
                                                    default:
                                                        PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                        break;
                                                }
                                                break;                                    
                                            case RM_COND_OPENING:
                                                pt_component_wo_sink->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                #if (PM_ENABLE_FILTER_CTRL)
                                                pt_component_wo_sink->t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                #endif
                                                i4_return = PMR_OK;  
                                                break;
                                            default:
                                                PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                break;
                                        }     
                                    }
                                #else
                                    /* allocate demux */
                                    if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&
                                        (pmi_filter_alloc(pt_svdb_node, DRVT_DEMUX_TS_PES_PACKET, pt_esdb_node) == PMR_OK) &&
                                        (pmi_descrambler_alloc(pt_esdb_node) == PMR_OK))
                                    {
                                        ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_DEMUX_TS_PES_PACKET;
                                        t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
                                        switch (t_cond)
                                        {
                                            case RM_COND_OPENED:
                                                t_conn = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                                                switch (t_conn)
                                                {
                                                    case RM_COND_CONNECTED:  
                                                        pt_component_wo_sink->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                                        #if (PM_ENABLE_FILTER_CTRL)
                                                        pt_component_wo_sink->t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                        #endif
                                                        i4_return = PMR_OK;   
                                                        break;                                                
                                                    case RM_COND_CONNECTING:
                                                        pt_component_wo_sink->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                        #if (PM_ENABLE_FILTER_CTRL)
                                                        pt_component_wo_sink->t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                        #endif
                                                        i4_return = PMR_OK;  
                                                        break;
                                                    default:
                                                        PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                        break;
                                                }
                                                break;                                    
                                            case RM_COND_OPENING:
                                                pt_component_wo_sink->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                #if (PM_ENABLE_FILTER_CTRL)
                                                pt_component_wo_sink->t_ca_nfy_info.e_cond = ESNODE_CA_COND(pt_esdb_node);
                                                #endif
                                                i4_return = PMR_OK;  
                                                break;
                                            default:
                                                PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                break;
                                        }     
                                    }                            
                                #endif                 
                                }
                            }
                            break;

                    #if (PM_ENABLE_MULTIMEDIA)      
                        #if (PM_ENABLE_RECORD)
                        case PM_RECORDING:
                            if ((handle_alloc(PM_GROUP_PMAPI_ES, pt_esdb_node, pt_esdb_node, pm_es_handle_free_nfy_fct, &(pt_rec_component->h_component)) == HR_OK) &&
                                (handle_link(&ESNODE_LINK(pt_esdb_node), pt_rec_component->h_component ) == HR_OK))
                            {
                                /* Set default output */                                
                                pt_rec_component->t_filter_nfy_info.e_cond = PM_COND_CLOSED;
                                pt_rec_component->t_filter_nfy_info.ui4_detail_code = 0;
                                pt_rec_component->t_ca_nfy_info.e_cond = PM_CA_COND_UNKNOWN;
                                pt_rec_component->t_ca_nfy_info.ui4_detail_code = 0;
                                x_memset(&(pt_rec_component->t_cp_nfy_info.t_cp_info), 0, sizeof(CP_INFO_LIST_T));
                                ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_UNKNOWN;
                                
                                /* Fill es info */
                                ESNODE_REC_REQUEST_TYPE(pt_esdb_node) = pt_rec_component->e_es_info_type;
                                switch (pt_rec_component->e_es_info_type)
                                {   
                                    case PM_RECORDING_ES_WI_SETTING:
                                        ESNODE_REC_SETTING_INFO(pt_esdb_node) = pt_rec_component->u_es_info.t_stream_setting.e_setting_info; 
                                        ESNODE_PID(pt_esdb_node)        = pt_rec_component->u_es_info.t_stream_setting.t_pid; 
                                        ESNODE_REC_STREAM_TYPE_MASK(pt_esdb_node) = pt_rec_component->u_es_info.t_stream_setting.ui4_stream_type_mask;
                                        x_memcpy( &(ESNODE_REC_SINK_INFO(pt_esdb_node)), &(pt_rec_component->t_sink_comp_info), sizeof(RECDEMUX_DEST_INFO_T));
                                        ESNODE_FILTER_NFY(pt_esdb_node) = pt_rec_component->t_filter_nfy_info.pf_nfy;
                                        ESNODE_FILTER_TAG(pt_esdb_node) = pt_rec_component->t_filter_nfy_info.pv_tag;                                                        
                                        break;
                                        
                                    case PM_RECORDING_ES_WO_SETTING:
                                        x_memcpy(&(ESNODE_DESCR(pt_esdb_node)), &(pt_rec_component->u_es_info.t_stream_comp_id), sizeof(STREAM_COMP_ID_T));                                                                                                                                                                                                        
                                        x_memcpy( &(ESNODE_REC_SINK_INFO(pt_esdb_node)), &(pt_rec_component->t_sink_comp_info), sizeof(RECDEMUX_DEST_INFO_T));
                                        ESNODE_FILTER_NFY(pt_esdb_node) = pt_rec_component->t_filter_nfy_info.pf_nfy;
                                        ESNODE_FILTER_TAG(pt_esdb_node) = pt_rec_component->t_filter_nfy_info.pv_tag;
                                        break;
                                        
                                    case PM_RECORDING_ES_FILTER_ALLOCATED:                                              
                                        ESNODE_PID(pt_esdb_node)        = pt_rec_component->u_es_info.t_filter_setting.t_pid; 
                                        ESNODE_FILTER_HANDLE(pt_esdb_node) = pt_rec_component->u_es_info.t_filter_setting.t_filter_handle;
                                        break;
                                    
                                    default:
                                        /* ToDo */
                                        break;
                                }                                                   
                                
                                ESNODE_CA_NFY(pt_esdb_node) = pt_rec_component->t_ca_nfy_info.pf_nfy;
                                ESNODE_CA_TAG(pt_esdb_node) = pt_rec_component->t_ca_nfy_info.pv_tag;
                                ESNODE_CP_NFY(pt_esdb_node) = pt_rec_component->t_cp_nfy_info.pf_nfy;
                                ESNODE_CP_TAG(pt_esdb_node) = pt_rec_component->t_cp_nfy_info.pv_tag;
                                ESNODE_CA_REQ_LVL(pt_esdb_node) = pt_rec_component->e_ca_require_lvl;
                                i4_return = PMR_FAILED;
                            
                                /* Attach ES to Service */
                                if (pmi_db_insert_to_service(pt_esdb_node, pt_svdb_node)==PMR_OK)
                                {  
                                    switch (pt_rec_component->e_es_info_type)
                                    {
                                        case PM_RECORDING_ES_WI_SETTING:
                                        case PM_RECORDING_ES_WO_SETTING:  
                                            b_continue = TRUE;
                                            if (pt_rec_component->e_es_info_type == PM_RECORDING_ES_WO_SETTING)
                                            {
                                                /* query SCDB */
                                                if (pmi_scdb_query(pt_svdb_node, pt_esdb_node) != PMR_OK)
                                                {
                                                    b_continue = FALSE;
                                                }
                                                else
                                                {
                                                    PM_DBG_ERROR( "query SCDB FAIL, when component type is WO_SETTING\n" );
                                                }
                                            }                             
                                            
                                            if (b_continue == TRUE)
                                            {
                                                DRV_TYPE_T           t_filter_type = DRVT_DEMUX_REC;
                                                
                                                if(pt_rec_component->e_es_info_type == PM_RECORDING_ES_WI_SETTING)
                                                {
                                                    if (PM_RECORDING_SETTING_PCR == ESNODE_REC_SETTING_INFO(pt_esdb_node))
                                                    {
                                                        t_filter_type = DRVT_DEMUX_REC_PCR;
                                                    }
                                                }

                                                /* allocate demux */
                                                if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&
                                                    (pmi_filter_alloc(pt_svdb_node, t_filter_type, pt_esdb_node) == PMR_OK))
                                                {
                                                    BOOL            fg_is_ok = TRUE;

                                                    #if (PM_ENABLE_CA)
                                                    if (PM_CA_REQUIRE_MANDATORY == ESNODE_CA_REQ_LVL(pt_esdb_node))
                                                    {
                                                        if (pmi_descrambler_alloc(pt_esdb_node) < PMR_OK)
                                                        {
                                                            fg_is_ok = FALSE;
                                                        }
                                                    }
                                                    #endif
                                                    
                                                    if (fg_is_ok)
                                                    {
                                                        ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_DEMUX_REC;
                                                        t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
                                                        switch (t_cond)
                                                        {
                                                            case RM_COND_OPENED:
                                                                t_conn = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                                                                switch (t_conn)
                                                                {
                                                                    case RM_COND_CONNECTED:  
                                                                        pt_rec_component->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                                                        i4_return = PMR_OK;   
                                                                        break;                                                
                                                                    case RM_COND_CONNECTING:
                                                                        pt_rec_component->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                        i4_return = PMR_OK;  
                                                                        break;
                                                                    default:
                                                                        PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                                        break;
                                                                }
                                                                break;                                    
                                                            case RM_COND_OPENING:
                                                                pt_rec_component->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                i4_return = PMR_OK;  
                                                                break;
                                                            default:
                                                                PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                                break;
                                                        }     
                                                    }
                                                    else
                                                    {
                                                        PM_DBG_ERROR( "allocate filter for component FAIL\n" );
                                                    }
                                                }
                                            }
                                            break;
                                            
                                        case PM_RECORDING_ES_FILTER_ALLOCATED:
                                            i4_return = PMR_OK;
                                            
                                            #if (PM_ENABLE_CA)
                                            if (PM_CA_REQUIRE_MANDATORY == ESNODE_CA_REQ_LVL(pt_esdb_node))
                                            {
                                                if (pmi_descrambler_alloc(pt_esdb_node) < PMR_OK)
                                                {
                                                    i4_return = PMR_FAILED;
                                                }
                                            }
                                            #endif
                                            
                                            break;
                                        default:
                                            break;
                                    }  
                                }
                                else
                                {
                                    PM_DBG_ERROR( "insert component to service node FAIL\n" );
                                }
                            }
                            else
                            {
                                PM_DBG_ERROR( "allocate component handle FAIL\n" );
                            }                                
                            
                            break;
                        #endif
                            
                        case PM_MULTIMEDIA:
                            pt_mm_component_info = (PM_MM_REQUEST_T* )pt_component_info;                            

                            if ((handle_alloc(PM_GROUP_PMAPI_ES, pt_esdb_node, pt_esdb_node, pm_es_handle_free_nfy_fct, &(pt_mm_component_info->h_component)) == HR_OK)&&
                                (handle_link(&ESNODE_LINK(pt_esdb_node), pt_mm_component_info->h_component ) == HR_OK))
                            {   
                                /* Set default output */                                
                                pt_mm_component_info->t_filter_nfy_info.e_cond = PM_COND_CLOSED;
                                pt_mm_component_info->t_filter_nfy_info.ui4_detail_code = 0;
                                ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_UNKNOWN;
                               
                                /* Fill es info */
                                ESNODE_MMP_REQUEST_TYPE(pt_esdb_node) = pt_mm_component_info->e_es_info_type;
                                switch (pt_mm_component_info->e_es_info_type)
                                {                                                      
                                    case PM_MM_ES_WI_SETTING:  
                                        x_memcpy(&(ESNODE_SINK_INFO(pt_esdb_node)), &(pt_mm_component_info->t_sink_comp_info), sizeof(DEMUX_DEST_INFO_T));
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = pt_mm_component_info->u_es_info.t_stream_setting.e_type;                                                        
                                        x_memcpy(&(ESNODE_MMP_CRYPT_INFO(pt_esdb_node)), &(pt_mm_component_info->t_crypt_info), sizeof(MM_CRYPT_INFO_T));
                                        x_memcpy(&(ESNODE_MMP_CRYPT_STREAM(pt_esdb_node)), &(pt_mm_component_info->u_es_info.t_stream_setting), sizeof(STREAM_ID_T));
                                        switch(ESNODE_MMP_STREAM_TYPE(pt_esdb_node))
                                        {
                                            case STREAM_ID_TYPE_AVI:
                                                ESNODE_MMP_STREAM_AVI_ID(pt_esdb_node) = pt_mm_component_info->u_es_info.t_stream_setting.u.t_stm_avi_id;
                                                break;
                                            case STREAM_ID_TYPE_ASF:
                                                ESNODE_MMP_STREAM_ASF_ID(pt_esdb_node) = pt_mm_component_info->u_es_info.t_stream_setting.u.t_stm_asf_id;
                                                break;
                                            case STREAM_ID_TYPE_MP4:
                                                ESNODE_MMP_STREAM_MP4_ID(pt_esdb_node) = pt_mm_component_info->u_es_info.t_stream_setting.u.t_stm_mp4_id;
                                                break;
                                            case STREAM_ID_TYPE_MKV:
                                                ESNODE_MMP_STREAM_MKV_ID(pt_esdb_node) = pt_mm_component_info->u_es_info.t_stream_setting.u.t_stm_mkv_id;
                                                break;
                                            case STREAM_ID_TYPE_PS:
                                                ESNODE_MMP_STREAM_PS_ID(pt_esdb_node) = pt_mm_component_info->u_es_info.t_stream_setting.u.t_stm_ps_id.ui2_stm_id;
                                                break;
                                            case STREAM_ID_TYPE_TS:
                                                ESNODE_MMP_STREAM_TS_ID(pt_esdb_node) = pt_mm_component_info->u_es_info.t_stream_setting.u.t_stm_ts_id;
                                                break;
                                            case STREAM_ID_TYPE_RM:
                                                ESNODE_MMP_STREAM_RM_ID(pt_esdb_node) = pt_mm_component_info->u_es_info.t_stream_setting.u.t_stm_rm_id;
                                                break;
                                            case STREAM_ID_TYPE_ES:
                                                ESNODE_MMP_STREAM_ES_ID(pt_esdb_node) = pt_mm_component_info->u_es_info.t_stream_setting.u.t_stm_es_id;
                                                break;
                                            case STREAM_ID_TYPE_OGG:
                                                ESNODE_MMP_STREAM_OGG_ID(pt_esdb_node) = pt_mm_component_info->u_es_info.t_stream_setting.u.t_stm_ogg_id;
                                                break;
                                            default:
                                                /* ToDo */
                                                break;                                                           
                                        }
                                        ESNODE_FILTER_NFY(pt_esdb_node) = pt_mm_component_info->t_filter_nfy_info.pf_nfy;
                                        ESNODE_FILTER_TAG(pt_esdb_node) = pt_mm_component_info->t_filter_nfy_info.pv_tag;                                                        
                                        break;
                                        
                                    case PM_MM_ES_WO_SETTING:
                                        x_memcpy(&(ESNODE_MMP_DESCR(pt_esdb_node)), &(pt_mm_component_info->u_es_info.t_stream_comp_id), sizeof(STREAM_COMP_ID_T));                                         
                                        x_memcpy(&(ESNODE_SINK_INFO(pt_esdb_node)), &(pt_mm_component_info->t_sink_comp_info), sizeof(DEMUX_DEST_INFO_T));
                                        ESNODE_FILTER_NFY(pt_esdb_node) = pt_mm_component_info->t_filter_nfy_info.pf_nfy;
                                        ESNODE_FILTER_TAG(pt_esdb_node) = pt_mm_component_info->t_filter_nfy_info.pv_tag;
                                        ESNODE_MMP_CRYPT_TYPE(pt_esdb_node)     = MM_CRYPT_TYPE_PVR;
                                        ESNODE_MMP_CRYPT_PVR_MODE(pt_esdb_node) = MM_CRYPT_MODE_INVALID;
                                        break;

                                    case PM_MM_ES_FILTER_ALLOCATED:
                                        x_memcpy(&(ESNODE_MMP_DESCR_SETTING(pt_esdb_node)), &(pt_mm_component_info->u_es_info.t_filter_setting.t_stream_setting), sizeof(STREAM_ID_T));
                                        ESNODE_FILTER_HANDLE(pt_esdb_node) = pt_mm_component_info->u_es_info.t_filter_setting.t_filter_handle;
                                        x_memcpy(&(ESNODE_MMP_CRYPT_INFO(pt_esdb_node)), &(pt_mm_component_info->t_crypt_info), sizeof(MM_CRYPT_INFO_T));
                                        x_memcpy(&(ESNODE_MMP_CRYPT_STREAM(pt_esdb_node)), &(pt_mm_component_info->u_es_info.t_stream_setting), sizeof(STREAM_ID_T));
                                        break;       
                                    
                                    default:
                                        /* ToDo */
                                        break;
                                }         
                                
                                ESNODE_CA_NFY(pt_esdb_node) = pt_mm_component_info->t_ca_nfy_info.pf_nfy;
                                ESNODE_CA_TAG(pt_esdb_node) = pt_mm_component_info->t_ca_nfy_info.pv_tag;
                                i4_return = PMR_FAILED;                                                                                       
                                
                                /* Attach ES to Service */
                                if (pmi_db_insert_to_service(pt_esdb_node, pt_svdb_node)==PMR_OK)
                                {   
                                    switch (pt_mm_component_info->e_es_info_type)
                                    {
                                        case PM_MM_ES_WI_SETTING:
                                        case PM_MM_ES_WO_SETTING:                                                                   
                                            b_continue = TRUE;
                                            
                                            if (pt_mm_component_info->e_es_info_type == PM_MM_ES_WO_SETTING)
                                            {
                                                /* query SCDB */
                                                if (pmi_scdb_query(pt_svdb_node, pt_esdb_node) != PMR_OK)
                                                    b_continue = FALSE; 
                                            }                             
                                            
                                            if (b_continue == TRUE)
                                            {
                                                /* allocate demux */
                                                if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_OPEN) == TRUE) &&
                                                    (pmi_filter_alloc(pt_svdb_node, DRVT_DEMUX_CONTAINER_ES, pt_esdb_node) == PMR_OK))
                                                {  
                                                    ESNODE_FILTER_TYPE(pt_esdb_node) = DRVT_DEMUX_CONTAINER_ES;
                                                    t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_OBJ_STATE);
                                                    switch (t_cond)
                                                    {
                                                        case RM_COND_OPENED:
                                                            t_conn = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), 0, COND_GRP_CONN_STATE);                                        
                                                            switch (t_conn)
                                                            {
                                                                case RM_COND_CONNECTED:  
                                                                    pt_mm_component_info->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                                                                    i4_return = PMR_OK;   
                                                                    break;                                                
                                                                case RM_COND_CONNECTING:
                                                                    pt_mm_component_info->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                                    i4_return = PMR_OK;   
                                                                    break;
                                                                default:
                                                                    PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                                                                    break;
                                                            }
                                                            break;                                                    
                                                        case RM_COND_OPENING:
                                                            pt_mm_component_info->t_filter_nfy_info.e_cond = ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                                                            i4_return = PMR_OK;   
                                                            break;
                                                        default:
                                                            PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_OPEN_COND);
                                                            break;
                                                    }                                                     
                                                }
                                            
                                            }                                                          
                                            break;
                                            
                                        case PM_MM_ES_FILTER_ALLOCATED:
                                            /* To Do. Add descryption function? */
                                            i4_return = PMR_OK;
                                            break;
                                            
                                        default:                                            
                                            break;
                                    }   
                                }                                                                             
                            }                                         
                                                        
                            break;
                    #endif
                        default:
                            /* ToDo */   
                            break;                             
                    }
                }
            }

            if (i4_return != PMR_OK)
            {
                if (pt_esdb_node != NULL)
                    pm_db_parser_close_fct(pt_esdb_node); 
                    
                switch (SVNODE_PROC_TYPE(pt_svdb_node))
                {
                    case PM_PRESENT:            
                        if (pt_component != NULL)
                        {                                
                            pt_component->h_component = NULL_HANDLE;                                                              
                            pt_component->t_filter_nfy_info.e_cond = PM_COND_CLOSED;
                            pt_component->t_filter_nfy_info.ui4_detail_code = 0; 
                        }
                        break;  
                                   
                    case PM_DETECTION:
                        if (pt_component_wo_sink != NULL)
                        {                                
                            pt_component_wo_sink->h_component = NULL_HANDLE;                                                              
                            pt_component_wo_sink->t_filter_nfy_info.e_cond = PM_COND_CLOSED;
                            pt_component_wo_sink->t_filter_nfy_info.ui4_detail_code = 0; 
                        }
                        break;

                #if (PM_ENABLE_MULTIMEDIA)                          
                #if (PM_ENABLE_RECORD)
                case PM_RECORDING:            
                    if (pt_rec_component != NULL)
                    {                                
                        pt_rec_component->h_component = NULL_HANDLE;                                                              
                        pt_rec_component->t_filter_nfy_info.e_cond = PM_COND_CLOSED;
                        pt_rec_component->t_filter_nfy_info.ui4_detail_code = 0; 
                    }
                    break;
                #endif
                    
                    case PM_MULTIMEDIA:
                        if (pt_mm_component_info != NULL)
                        {                                
                            pt_mm_component_info->h_component = NULL_HANDLE;                                                              
                            pt_mm_component_info->t_filter_nfy_info.e_cond = PM_COND_CLOSED;
                            pt_mm_component_info->t_filter_nfy_info.ui4_detail_code = 0; 
                        }
                        
                        break;
                #endif
                    default:
                        /* ToDo */
                        break;
                }
            }       
       }
            
    /* Unlock */
    pmi_unlock_db();

    }
    
    PM_ERR_CODE(i4_return);
    return i4_return;      
}                        

/*-----------------------------------------------------------------------------
 * Name: pm_stop_component
 *
 * Description: This API is responsible to stop ES.
 *
 * Inputs:  h_component      Contains the ES handle.
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     PM module is not initialized.
 *          PMR_INV_HANDLE   Invalid component handle. 
 ----------------------------------------------------------------------------*/            
INT32 pm_stop_component(HANDLE_T   h_component)
{     
    PM_COMPONENT_INFO_T* pt_esdb_node;      
    PM_SERVICE_INFO_T*   pt_svdb_node;      
    INT32                i4_return, i4_res;
    HANDLE_TYPE_T        e_type;
    
    i4_return = PMR_NOT_INIT;
    
    if (b_pm_init)
    {
        i4_return = PMR_INV_HANDLE;
        
        /* Lock */
        pmi_lock_db_wr();
        
        i4_res = handle_get_type_obj(h_component, &e_type, (VOID *)(&pt_esdb_node));                 
        
        if ((i4_res == HR_OK)&&(e_type == PM_GROUP_PMAPI_ES))
        {              
            pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_esdb_node)); 
            if (pt_svdb_node == NULL)
            {
                /* Unlock */
                pmi_unlock_db();
                return PMR_FAILED;
            }
            switch (SVNODE_PROC_TYPE(pt_svdb_node))
            {
                case PM_PRESENT:
                    if ((((ESNODE_REQUEST_TYPE(pt_esdb_node)== PM_PRESENT_ES_WI_SETTING) || 
                          (ESNODE_REQUEST_TYPE(pt_esdb_node)== PM_PRESENT_ES_WO_SETTING)) && 
                         (ESNODE_FILTER_STATUS(pt_esdb_node) != PM_COND_CLOSED)) ||
                        (ESNODE_REQUEST_TYPE(pt_esdb_node)== PM_PRESENT_ES_FILTER_ALLOCATED)) 
                    {
                        pm_db_parser_stop_fct(pt_esdb_node);
                        
                        i4_return = PMR_OK;
                    }                     
                    break;
                    
                case PM_DETECTION:
                    if (ESNODE_FILTER_STATUS(pt_esdb_node) != PM_COND_CLOSED )
                    {
                        pm_db_parser_stop_fct(pt_esdb_node);
                        
                        i4_return = PMR_OK;
                    }                
                    break;
                    
            #if (PM_ENABLE_MULTIMEDIA)                          
                #if (PM_ENABLE_RECORD)
                case PM_RECORDING:
                    if ((((ESNODE_REC_REQUEST_TYPE(pt_esdb_node)== PM_RECORDING_ES_WI_SETTING) || 
                          (ESNODE_REC_REQUEST_TYPE(pt_esdb_node)== PM_RECORDING_ES_WO_SETTING)) && 
                         (ESNODE_FILTER_STATUS(pt_esdb_node) != PM_COND_CLOSED)) ||
                        (ESNODE_REC_REQUEST_TYPE(pt_esdb_node)== PM_RECORDING_ES_FILTER_ALLOCATED)) 
                    {
                        pm_db_parser_stop_fct(pt_esdb_node);
                        i4_return = PMR_OK;
                    }                     
                    else
                    {
                        PM_DBG_ERROR( "handle is invaild\n" );
                    }
                    break;
                #endif
                    
                case PM_MULTIMEDIA:
                    if ((((ESNODE_MMP_REQUEST_TYPE(pt_esdb_node) == PM_MM_ES_WI_SETTING) || 
                         (ESNODE_MMP_REQUEST_TYPE(pt_esdb_node)== PM_MM_ES_WO_SETTING)) && 
                         (ESNODE_FILTER_STATUS(pt_esdb_node) != PM_COND_CLOSED)) ||
                         (ESNODE_MMP_REQUEST_TYPE(pt_esdb_node) == PM_MM_ES_FILTER_ALLOCATED))                         
                    {
                        pm_db_parser_stop_fct(pt_esdb_node);
                        i4_return = PMR_OK;
                    }                                  
                    break;
            #endif
                default:
                    /* ToDo */
                    PM_DBG_ERROR( "handle is invaild\n" );
                    break;
            }                                      
        }
        else
        {
            PM_DBG_ERROR( "ESDB handle is invaild\n" );
        }
        
        /* Unlock */
        pmi_unlock_db(); 
    }       
       
    PM_ERR_CODE(i4_return);
    return i4_return;    
}   

/*-----------------------------------------------------------------------------
 * Name: pm_del_component
 *
 * Description: This API is responsible to detach the ES from the service.
 *
 * Inputs:  h_component      Contains the ES handle.
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     PM module is not initialized.
 *          PMR_INV_HANDLE   Invalid component handle. 
 ----------------------------------------------------------------------------*/ 
INT32 pm_del_component(HANDLE_T   h_component)
{        
    PM_COMPONENT_INFO_T* pt_esdb_node;   
    PM_SERVICE_INFO_T*   pt_svdb_node;     
    INT32                i4_return, i4_res;
    HANDLE_TYPE_T        e_type;
    
    i4_return = PMR_NOT_INIT;
    
    if (b_pm_init)
    {
        i4_return = PMR_INV_HANDLE;
        
        /* Lock */
        pmi_lock_db_wr();        
        
        i4_res = handle_get_type_obj(h_component, &e_type, (VOID *)(&pt_esdb_node));
        
        if ((i4_res == HR_OK)&&(e_type == PM_GROUP_PMAPI_ES))
        {              
            pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_esdb_node));
            if (pt_svdb_node == NULL)
            {
                /* Unlock */
                pmi_unlock_db();
                return PMR_FAILED;
            }
            switch (SVNODE_PROC_TYPE(pt_svdb_node))
            {
                case PM_PRESENT:
                    if ((((ESNODE_REQUEST_TYPE(pt_esdb_node)== PM_PRESENT_ES_WI_SETTING) || 
                          (ESNODE_REQUEST_TYPE(pt_esdb_node)== PM_PRESENT_ES_WO_SETTING)) && 
                         (ESNODE_FILTER_STATUS(pt_esdb_node) != PM_COND_CLOSED)) ||
                        (ESNODE_REQUEST_TYPE(pt_esdb_node)== PM_PRESENT_ES_FILTER_ALLOCATED)) 
                    {
                    #if (!PM_ENABLE_CA)
                        pm_db_parser_close_fct(pt_esdb_node);
                        i4_return = PMR_OK;            
                    #else   
                        if (ESNODE_CA_HANDLE(pt_esdb_node) == NULL_HANDLE )
                        {               
                            pm_db_parser_close_fct(pt_esdb_node);
                            i4_return = PMR_OK;   
                        }
                        else
                        {
                            if ((i4_return = pmi_ca_del_component(pt_esdb_node)) == PMR_OK)
                            {
                                pm_db_parser_close_fct(pt_esdb_node);
                                i4_return = PMR_OK;                
                            }
                        }
                    #endif     
                    }
                     
                    break;
                case PM_DETECTION:
                    if (ESNODE_FILTER_STATUS(pt_esdb_node) != PM_COND_CLOSED )
                    {
                    #if (!PM_ENABLE_CA)
                        pm_db_parser_close_fct(pt_esdb_node);
                        i4_return = PMR_OK;            
                    #else   
                        if (ESNODE_CA_HANDLE(pt_esdb_node) == NULL_HANDLE )
                        {               
                            pm_db_parser_close_fct(pt_esdb_node);
                            i4_return = PMR_OK;   
                        }
                        else
                        {
                            if ((i4_return = pmi_ca_del_component(pt_esdb_node)) == PMR_OK)
                            {
                                pm_db_parser_close_fct(pt_esdb_node);
                                i4_return = PMR_OK;                
                            }
                        }
                    #endif                             
                 
                    }
                
                    break;
                    
            #if (PM_ENABLE_MULTIMEDIA)                          
                #if (PM_ENABLE_RECORD)
                case PM_RECORDING:
                    if ((((ESNODE_REC_REQUEST_TYPE(pt_esdb_node)== PM_RECORDING_ES_WI_SETTING) || 
                          (ESNODE_REC_REQUEST_TYPE(pt_esdb_node)== PM_RECORDING_ES_WO_SETTING)) && 
                         (ESNODE_FILTER_STATUS(pt_esdb_node) != PM_COND_CLOSED)) ||
                        (ESNODE_REC_REQUEST_TYPE(pt_esdb_node)== PM_RECORDING_ES_FILTER_ALLOCATED)) 
                    {
                    #if (!PM_ENABLE_CA)
                        pm_db_parser_close_fct(pt_esdb_node);
                        i4_return = PMR_OK;            
                    #else   
                        if (ESNODE_CA_HANDLE(pt_esdb_node) == NULL_HANDLE )
                        {               
                            pm_db_parser_close_fct(pt_esdb_node);
                            i4_return = PMR_OK;   
                        }
                        else
                        {
                            if ((i4_return = pmi_ca_del_component(pt_esdb_node)) == PMR_OK)
                            {
                                pm_db_parser_close_fct(pt_esdb_node);
                                i4_return = PMR_OK;                
                            }
                        }
                    #endif     
                    }
                    else
                    {
                        PM_DBG_ERROR( "handle is invaild\n" );
                    }
                    break;
                #endif
                    
                case PM_MULTIMEDIA:
                    if ((((ESNODE_MMP_REQUEST_TYPE(pt_esdb_node) == PM_MM_ES_WI_SETTING) || 
                         (ESNODE_MMP_REQUEST_TYPE(pt_esdb_node)== PM_MM_ES_WO_SETTING)) && 
                         (ESNODE_FILTER_STATUS(pt_esdb_node) != PM_COND_CLOSED)) ||
                         (ESNODE_MMP_REQUEST_TYPE(pt_esdb_node) == PM_MM_ES_FILTER_ALLOCATED))                              
                    {                    
                        pm_db_parser_close_fct(pt_esdb_node);
                        i4_return = PMR_OK;  
                    }                
                    
                    break;
            #endif
            
                default:
                    /* ToDo */
                    break;
            }                                      
        }   
        else
        {
            PM_DBG_ERROR( "handle is invaild\n" );
        }
        
        /* Unlock */
        pmi_unlock_db();
    }   
    
    PM_ERR_CODE(i4_return);
    return i4_return;     
}   

/*-----------------------------------------------------------------------------
 * Name: pm_play_component
 *
 * Description: This API is responsible to play the individual ES.
 *
 * Inputs:  h_component      Contains the ES handle.
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     PM module is not initialized.
 *          PMR_INV_HANDLE   Invalid component handle. 
 ----------------------------------------------------------------------------*/ 
INT32 pm_play_component(HANDLE_T   h_component)
{       
    PM_COMPONENT_INFO_T* pt_esdb_node;   
    PM_SERVICE_INFO_T*   pt_svdb_node;  
    INT32                i4_return, i4_res;
    HANDLE_TYPE_T        e_type;
    
    i4_return = PMR_NOT_INIT;
    
    if (b_pm_init)
    {
        i4_return = PMR_INV_HANDLE;
        
        /* Lock */
        pmi_lock_db_wr();    
        
        i4_res = handle_get_type_obj(h_component, &e_type, (VOID *)(&pt_esdb_node));        

        
        if ((i4_res == HR_OK)&&(e_type == PM_GROUP_PMAPI_ES))
        {    
            pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_esdb_node));
            if (pt_svdb_node == NULL)
            {
                /* Unlock */
                pmi_unlock_db(); 
                return PMR_FAILED;
            }
            switch (SVNODE_PROC_TYPE(pt_svdb_node))
            {
                case PM_PRESENT:
                    if ((((ESNODE_REQUEST_TYPE(pt_esdb_node)== PM_PRESENT_ES_WI_SETTING) || 
                          (ESNODE_REQUEST_TYPE(pt_esdb_node)== PM_PRESENT_ES_WO_SETTING)) && 
                         (ESNODE_FILTER_STATUS(pt_esdb_node) != PM_COND_CLOSED)) ||
                        (ESNODE_REQUEST_TYPE(pt_esdb_node)== PM_PRESENT_ES_FILTER_ALLOCATED)) 
                    {
                        pm_db_parser_play_fct(pt_esdb_node);
                        
                    #if (!PM_ENABLE_CA)
                        i4_return = PMR_OK;
                    #else
                        i4_return = pmi_ca_add_component(pt_esdb_node);
                    #endif   
                    }                     
                    break;
                    
                case PM_DETECTION:
                    if (ESNODE_FILTER_STATUS(pt_esdb_node) != PM_COND_CLOSED )
                    {
                        pm_db_parser_play_fct(pt_esdb_node);
                            
                    #if (!PM_ENABLE_CA)
                        i4_return = PMR_OK;
                    #else
                        i4_return = pmi_ca_add_component(pt_esdb_node);
                    #endif                          
                    }                
                    break;
                    
            #if (PM_ENABLE_MULTIMEDIA)                          
                #if (PM_ENABLE_RECORD)
                case PM_RECORDING:
                    if ((((ESNODE_REC_REQUEST_TYPE(pt_esdb_node)== PM_RECORDING_ES_WI_SETTING) || 
                          (ESNODE_REC_REQUEST_TYPE(pt_esdb_node)== PM_RECORDING_ES_WO_SETTING)) && 
                         (ESNODE_FILTER_STATUS(pt_esdb_node) != PM_COND_CLOSED)) ||
                        (ESNODE_REC_REQUEST_TYPE(pt_esdb_node)== PM_RECORDING_ES_FILTER_ALLOCATED)) 
                    {
                        pm_db_parser_play_fct(pt_esdb_node);
                    #if (!PM_ENABLE_CA)
                        i4_return = PMR_OK;
                    #else
                        i4_return = pmi_ca_add_component(pt_esdb_node);
                    #endif 
                    }                     
                    else
                    {
                        PM_DBG_ERROR( "handle is invaild\n" );
                    }
                    break;
                #endif
                    
                case PM_MULTIMEDIA:
                    if ((((ESNODE_MMP_REQUEST_TYPE(pt_esdb_node) == PM_MM_ES_WI_SETTING) || 
                         (ESNODE_MMP_REQUEST_TYPE(pt_esdb_node)== PM_MM_ES_WO_SETTING)) && 
                         (ESNODE_FILTER_STATUS(pt_esdb_node) != PM_COND_CLOSED)) ||
                         (ESNODE_MMP_REQUEST_TYPE(pt_esdb_node) == PM_MM_ES_FILTER_ALLOCATED))                              
                    {       
                        pm_db_parser_play_fct(pt_esdb_node);
                        i4_return = PMR_OK;
                    }                                   
                    break;
                    
            #endif
                    
                default:
                    /* ToDo */
                    break;
            }   
        }        
        else
        {
            PM_DBG_ERROR( "handle is invaild\n" );
        }
        
        /* Unlock */
        pmi_unlock_db(); 
    }   
       
    PM_ERR_CODE(i4_return);
    return i4_return;      
}   

/*-----------------------------------------------------------------------------
 * Name: pm_get_component_cond
 *
 * Description: This API is responsible to get the current condition of ES.
 *
 * Inputs:  h_component      Contains the ES handle.
 * Outputs: pe_cond          Contains the reference to the returned condition.
 *          pui4_data        Contains the reference to the detail of condition.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     PM module is not initialized.
 *          PMR_INV_HANDLE   h_component is invalid handle.
 *          PMR_INV_ARG      Invalid arguments. 
 ----------------------------------------------------------------------------*/ 
INT32 pm_get_component_cond(HANDLE_T   h_component,
                            PM_COND_T* pe_cond,
                            UINT32*    pui4_data)
{       
    PM_COMPONENT_INFO_T* pt_esdb_node;    
    INT32                i4_return, i4_res;
    HANDLE_TYPE_T        e_type;
    
    i4_return = PMR_NOT_INIT;
    
    if (b_pm_init)
    {
        i4_return = PMR_INV_HANDLE;
           
        if (x_handle_valid(h_component) == TRUE)
        {
                i4_return = PMR_INV_ARG;
                
                if ((pe_cond != NULL) && (pui4_data != NULL))
                {                 
                    /* Lock */
                    pmi_lock_db_wr();     
            
                    i4_res = handle_get_type_obj(h_component, &e_type, (VOID *)(&pt_esdb_node));   
            
                    if ((i4_res == HR_OK)&&(e_type == PM_GROUP_PMAPI_ES) && (ESNODE_FILTER_STATUS(pt_esdb_node) != PM_COND_CLOSED ))
                    {      
                        *pe_cond = ESNODE_FILTER_STATUS(pt_esdb_node);   
                        *pui4_data = ESNODE_DETAIL_CODE(pt_esdb_node);
                        i4_return = PMR_OK;            
                    } 
                    else
                    {
                        PM_DBG_ERROR( "handle is invaild\n" );
                    }
            
                    /* Unlock */
                    pmi_unlock_db(); 
                }
        }
        else
        {
            PM_DBG_ERROR( "handle is invaild\n" );
        }        
    }    
       
    PM_ERR_CODE(i4_return);
    return i4_return;      
}   
/*-----------------------------------------------------------------------------
 * Name: pm_get_component_info
 *
 * Description: This API is responsible to extract the real HW handle.
 *
 * Inputs:  h_component      Contains the ES handle.
 * Outputs: ph_real_obj      Contains the reference to the returned filter 
 *                           handle.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     PM module is not initialized.
 *          PMR_INV_ARG      Invalid arguments. 
 ----------------------------------------------------------------------------*/    
INT32 pm_get_component_info(HANDLE_T   h_component, HANDLE_T*  ph_real_obj)                                
{
    PM_COMPONENT_INFO_T* pt_esdb_node;    
    INT32                i4_return, i4_res;
    HANDLE_TYPE_T        e_type;
    
    i4_return = PMR_NOT_INIT;
    
    if (b_pm_init)
    {
        i4_return = PMR_INV_ARG;
        
        if (ph_real_obj != NULL)
        {        
            /* Lock */
            pmi_lock_db_wr();     
            
            i4_res = handle_get_type_obj(h_component, &e_type, (VOID *)(&pt_esdb_node));     
            
            if ((i4_res == HR_OK)&&(e_type == PM_GROUP_PMAPI_ES) && (ESNODE_FILTER_STATUS(pt_esdb_node) != PM_COND_CLOSED ))
            {      
                *ph_real_obj = ESNODE_FILTER_HANDLE(pt_esdb_node);  
                i4_return = PMR_OK;            
            }
            else
            {
                PM_DBG_ERROR( "handle is invaild\n" );
            }
            
            /* Unlock */
            pmi_unlock_db();   
        }        
        else
        {
            PM_DBG_ERROR( "ph_real_obj is NULL\n" );
        }
    }             
     
    PM_ERR_CODE(i4_return);
    return i4_return;      
    
}    

#if PM_ENABLE_CA                                                                                
/* For embedded CA or POD-CP */ 
/* CA_PI */
/*-----------------------------------------------------------------------------
 * Name: pm_register_capi
 *
 * Description: This API is called by CA engine to resiter its CA operation.
 *
 * Inputs:  b_alloc_desc           TRUE: Need allocate descrambler 
 *                                 FALSE: Don't need to allocate descrambler
 *          pt_desc_info           Contains the reference to descrambler 
 *                                 information.
 *          e_source_type          Contains the data source type (tuner,..)
 *          ui2_source_id          Contains the component id for data source.
 *          ui2_ca_sys_id          Contains the CA system id.
 *          t_ca_core_capi         Contains the CA operation table.
 *
 * Outputs: -
 *
 * Returns: PMR_OK                Success
 *          PMR_ALREADY_REGISTER  This module is already registered.
 *          PMR_NOT_SURPPORT      The algorithm is not surpported.
 *          PMR_INV_ARG           Part of the operation is NULL. 
 *          PMR_FAILED            Failed due to other reason. 
 ----------------------------------------------------------------------------*/ 
INT32 pm_register_capi(BOOL               b_alloc_desc,
                       PM_DESC_INFO_T*    pt_desc_info,
                       DRV_TYPE_T         e_source_type,
                       UINT16             ui2_source_id,
                       UINT16             ui2_ca_sys_id,
                       PM_CA_OPERATION_T  t_ca_core_capi)
{
    INT32   i4_return;

    i4_return = PMR_NOT_INIT;   
     
    if (b_pm_init)
    {   
        /* Lock */
        pmi_lock_db_wr();     
        
        i4_return = pmi_ca_register(b_alloc_desc,
                                    pt_desc_info, 
                                    e_source_type, 
                                    ui2_source_id,  
                                    ui2_ca_sys_id, 
                                    t_ca_core_capi);  
        
        /* Unlock */
        pmi_unlock_db();
    }
    PM_ERR_CODE(i4_return);
    return i4_return;       
}            

#endif /*  PM_ENABLE_CA    */

#if (PM_ENABLE_MULTIMEDIA)
/*-----------------------------------------------------------------------------
 * Name: pm_set
 *
 * Description: This API is called by caller to set related operation.
 *
 * Inputs:  h_instance       Contains the ES/Service handle.
 *          e_set_type       Contains the desired set operation.
 *          pt_info          Contains the reference to the parameter for set 
 *                           operation.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     PM module is not initialized.
 *          PMR_INV_ARG      Invalid arguments. 
 *          PMR_NOT_SURPPORT The operation is not surpported.
 *          PMR_FAILED       Failed due to other reason. 
 ----------------------------------------------------------------------------*/ 
INT32 pm_set(HANDLE_T              h_instance,  /* For both service/component */                 
             PM_SET_TYPE_T         e_set_type, 
             VOID*                 pt_info)
{
    VOID*                pv_node;
    PM_SERVICE_INFO_T*   pt_svdb_node = NULL;  
    INT32                i4_return, i4_res;
    HANDLE_TYPE_T        e_type;
    
    i4_return = PMR_NOT_INIT;
    
    if (b_pm_init)
    {
        i4_return = PMR_INV_ARG;
        
        if (h_instance != NULL_HANDLE)
        {        
            /* Lock */
            pmi_lock_db_wr();         
            
            i4_res = handle_get_type_obj(h_instance, &e_type, (VOID *)(&pv_node));           
                         
            if (i4_res == HR_OK)
            {
                switch (e_type)
                {
                    case PM_GROUP_PMAPI_ES:
                        PM_DBG_ERROR( "the handle is not supported right now\n" );
                        i4_return = PMR_NOT_SURPPORT;
                        break;
                        
                    case PM_GROUP_PMAPI_SV:
                        pt_svdb_node = pv_node;
                        switch (SVNODE_PROC_TYPE(pt_svdb_node))
                        {
                            case PM_MULTIMEDIA: 
                                i4_return = PMR_NOT_SURPPORT;
                                switch(e_set_type)
                                {
                                    case PM_SET_ADD_RANGE:
                                    case PM_SET_DEL_RANGE:
                                    case PM_SET_SPEED:
                                    case PM_SET_SEEK:
                                    case PM_SET_BLOCK_PSI:
                                    case PM_SET_AB_POS:
                                    case PM_SET_3D_PROPERTY:
                                    case PM_SET_STC_ID:
                                    case PM_SET_FREE_RUN:
                                    case PM_SET_REC_BUF_RANGE:
                                    case PM_SET_HDMVC_INFO:   
                                #if (PM_ENABLE_TIME_SHIFT)
                                    case PM_SET_MONITOR_LBA:
                                #endif
                                #ifdef MULTIMEDIA_AUTOTEST_SUPPORT
                                    case PM_SET_AUTOTEST:
                                #endif
                                    {
                                        i4_dmx_ctrl_set_result = PMR_FAILED;
                                        e_dmx_ctrl_set_type = e_set_type;
                                        pt_dmx_ctrl_setinfo = pt_info;  
                                        
                                        if (pmi_db_parse_service(pt_svdb_node, pm_db_parser_set_control_fct) == PMR_OK)                                                                      
                                            i4_return = i4_dmx_ctrl_set_result;
                                    }
                                        break;

                                    default:
                                        PM_DBG_ERROR( "set command is invaild\n" );
                                        break;
                                }
                                break;
                                
                            default:
                                i4_return = PMR_NOT_SURPPORT;
                                break;
                        }
                        break;
                        
                    default:
                        PM_DBG_ERROR( "parameter is invaild\n" );
                        i4_return = PMR_INV_ARG;
                        break;               
                }
            }                 
            else
            {
                PM_DBG_ERROR( "handle is invaild\n" );
            }
            
            /* Unlock */
            pmi_unlock_db(); 
        }        
        else
        {
            PM_DBG_ERROR( "h_instance is NULL_HANDLE\n" );
        }
    } 
    PM_ERR_CODE(i4_return);
    return i4_return;       
}                 

/*-----------------------------------------------------------------------------
 * Name: pm_get
 *
 * Description: This API is called by caller to get related operation.
 *
 * Inputs:  h_instance       Contains the ES/Service handle.
 *          e_get_type       Contains the desired get operation.
 *          pt_info          Contains the reference to get operation.
 *          pz_get_info_len  Contains the length of get info 
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     PM module is not initialized.
 *          PMR_INV_ARG      Invalid arguments. 
 *          PMR_NOT_SURPPORT The operation is not surpported.
 *          PMR_FAILED       Failed due to other reason. 
 ----------------------------------------------------------------------------*/ 
INT32 pm_get(HANDLE_T              h_instance,  /* For both service/component */                 
             PM_GET_TYPE_T         e_get_type, 
             VOID*                 pt_info,
             SIZE_T*               pz_get_info_len)
{
    VOID*                pv_node;
    PM_SERVICE_INFO_T*   pt_svdb_node = NULL;  
    INT32                i4_return, i4_res;
    HANDLE_TYPE_T        e_type;
    
    i4_return = PMR_NOT_INIT;
    
    if ( !pt_info || !pz_get_info_len )
    {
        return PMR_INV_ARG;    
    }    
    
    if (b_pm_init)
    {
        i4_return = PMR_INV_ARG;
        
        if (h_instance != NULL_HANDLE)
        {        
            /* Lock */
            pmi_lock_db_wr();         
            
            i4_res = handle_get_type_obj(h_instance, &e_type, (VOID *)(&pv_node));           
                         
            if (i4_res == HR_OK)
            {
                switch (e_type)
                {
                    case PM_GROUP_PMAPI_ES:
                        PM_DBG_ERROR( "the handle is not supported right now\n" );
                        i4_return = PMR_NOT_SURPPORT;
                        break;
                        
                    case PM_GROUP_PMAPI_SV:
                        pt_svdb_node = pv_node;
                        switch (SVNODE_PROC_TYPE(pt_svdb_node))
                        {                                
                            case PM_MULTIMEDIA: 
                                i4_return = PMR_NOT_SURPPORT;
                                switch (e_get_type)
                                {
                                    case PM_GET_BLOCK_PSI:
                                    case PM_GET_UOP_CAP:
                                    #if (PM_ENABLE_TIME_SHIFT)
                                    case PM_GET_CURR_LBA:
                                    case PM_GET_CURR_TICK_NUM:
                                    #endif
                                    case PM_GET_POST_INFO:
                                    case PM_GET_STC_OFFSET:
                                        i4_dmx_ctrl_get_result = PMR_FAILED;
                                        e_dmx_ctrl_get_type = e_get_type;
                                        pt_dmx_ctrl_get_info = pt_info;
                                        t_dmx_ctrl_get_size = *pz_get_info_len;
                                        if (pmi_db_parse_service(pt_svdb_node, pm_db_parser_get_control_fct) == PMR_OK)
                                        {
                                            i4_return = i4_dmx_ctrl_get_result;
                                        }                                        
                                        break;
                                        
                                    case PM_GET_CURR_SVNODE_PROC_TYPE:
                                        *((PM_PROCESSING_TYPE_T*)(pt_info)) = PM_MULTIMEDIA;
                                        *pz_get_info_len = sizeof(UINT32);
                                        i4_return = PMR_OK;
                                        break;

                                    default:
                                        PM_DBG_ERROR( "PM_MULTIMEDIA get command is invaild\n" );
                                        i4_return = PMR_NOT_SURPPORT;
                                        break;                                        
                                }
                                break;
                                
                            case PM_RECORDING:
                                i4_return = PMR_NOT_SURPPORT;
                                switch(e_get_type)
                                {
                                    case PM_GET_CURR_SVNODE_PROC_TYPE:
                                        *((PM_PROCESSING_TYPE_T*)(pt_info)) = PM_RECORDING;
                                        *pz_get_info_len = sizeof(UINT32);
                                        i4_return = PMR_OK;
                                        break;
                                        
                                    default:
                                        PM_DBG_ERROR( "PM_RECORDING get command is invaild\n" );
                                        i4_return = PMR_NOT_SURPPORT;
                                        break;                                       
                                }
                                break;

                            case PM_PRESENT:
                                i4_return = PMR_NOT_SURPPORT;
                                switch(e_get_type)
                                {
                                    case PM_GET_CURR_SVNODE_PROC_TYPE:
                                        *((PM_PROCESSING_TYPE_T*)(pt_info)) = PM_PRESENT;
                                        *pz_get_info_len = sizeof(UINT32);
                                        i4_return = PMR_OK;
                                        break;
                                        
                                    default:
                                        PM_DBG_ERROR( "PM_PRESENT get command is invaild\n" );
                                        i4_return = PMR_NOT_SURPPORT;
                                        break;                                       
                                }
                                break;

                            case PM_DETECTION:
                                i4_return = PMR_NOT_SURPPORT;
                                switch(e_get_type)
                                {
                                    case PM_GET_CURR_SVNODE_PROC_TYPE:
                                        *((PM_PROCESSING_TYPE_T*)(pt_info)) = PM_DETECTION;
                                        *pz_get_info_len = sizeof(UINT32);
                                        i4_return = PMR_OK;
                                        break;
                                        
                                    default:
                                        PM_DBG_ERROR( "PM_DETECTION get command is invaild\n" );
                                        i4_return = PMR_NOT_SURPPORT;
                                        break;                                       
                                }
                                break;
                                
                            default:
                                i4_return = PMR_NOT_SURPPORT;
                                break;
                        }
                        break;
                        
                    default:
                        PM_DBG_ERROR( "parameter is invaild\n" );
                        i4_return = PMR_NOT_SURPPORT;
                        break;               
                }
            }                 
            else
            {
                PM_DBG_ERROR( "handle is invaild\n" );
            }
            
            /* Unlock */
            pmi_unlock_db(); 
        }        
        else
        {
            PM_DBG_ERROR( "h_instance is NULL_HANDLE\n" );
        }
    } 
    PM_ERR_CODE(i4_return);
    return i4_return;       
}

#endif
                              
                                   
                  

