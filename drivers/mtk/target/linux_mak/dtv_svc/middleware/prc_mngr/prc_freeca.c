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
 * $RCSfile: prc_freeca.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Felisa Hsiao $
 * $MD5HEX: 84e80ee42d19802b939a6fd42b50bafc $
 *
 * Description: 
 *         This file is the implementation of PM_CA interface for Free CA.
 *
 * History:
 *           
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_desc.h"    
#include "prc_mngr.h"
#include "prc_ca.h"
#include "prc_freeca_db.h"
#include "prc_freeca.h"

#ifdef ENABLE_CA_SUPPORT  


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define FREECA_IGNORE_NOTIFY_FOR_CLOSE        0

typedef struct _FREECA_REQ_OBJ_T
{  
    FREECA_ES_T* pt_freeca_list;
    UINT8 ui1_freeca_list_size;   
} FREECA_REQ_OBJ_T ;
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/ 
static BOOL b_ca_core_init = FALSE;
static HANDLE_T  h_ca_lock;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
void pmi_freeca_lock_init(VOID);  
void pmi_freeca_lock(VOID);
void pmi_freeca_unlock(VOID); 
static BOOL pmi_freeca_es_handle_free_nfy_fct (HANDLE_T       h_handle,
                                       HANDLE_TYPE_T  e_type,
                                       void*          pv_obj,
                                       void*          pv_tag,   /* reference to ESNODE */
                                       BOOL           b_req_handle); 
static BOOL pmi_freeca_service_handle_free_nfy_fct (HANDLE_T       h_handle,
                                               HANDLE_TYPE_T  e_type,
                                               VOID*          pv_obj,
                                               VOID*          pv_tag, /* reference to SERVICENODE */
                                               BOOL           b_req_handle);     
                                               
static VOID pmi_freeca_ca_feedback_handler(HANDLE_T          h_ca,
                                           CA_DETAIL_COND_T  e_nfy_cond,
                                           CP_INFO_LIST_T    e_cpinfo,
                                           VOID*             pv_tag);                                                                      

INT32 pmi_freeca_pi_start_service (HANDLE_T             h_connection,    
                             PM_PROCESSING_TYPE_T e_processing_type,                                                                       
                             UINT32               ui4_list_size,                   
                             PM_CA_REQUEST_T      pt_component_list[],
                             HANDLE_T*            ph_ca_service);
                             
INT32 pmi_freeca_pi_stop_service (HANDLE_T     h_ca_service);

INT32 pmi_freeca_pi_add_component (HANDLE_T            h_ca_service, 
                             PM_CA_REQUEST_T*    pt_component);
                             
INT32 pmi_freeca_pi_del_component (HANDLE_T    h_ca);  

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_lock_init
 *
 * Description: This API is called to initialize lock for global data access.
 *
 * Inputs:  - 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/                      
void pmi_freeca_lock_init(VOID)
{

    /* Create the database object semaphores. Simply */
    /* abort if semahore cannot be created.            */
    if (x_sema_create( &(h_ca_lock), X_SEMA_TYPE_MUTEX,
                       X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        FREECA_ABORT (DBG_CAT_SEMAPHORE, FREECA_DBG_ABRT_CANNOT_CREATE_SEMAPHORE);
    }   
    
    return; 
}

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_lock
 *
 * Description: This API is called to lock the global data access.
 *
 * Inputs:  - 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/                      
void pmi_freeca_lock(VOID)
{
    /* lock database object semaphores. Simply */
    /* abort if semaphore unlock fails.            */
    if (x_sema_lock( h_ca_lock, X_SEMA_OPTION_WAIT) != OSR_OK)
    {
        FREECA_ABORT (DBG_CAT_SEMAPHORE, FREECA_DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
    }
    return;
}

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_unlock
 *
 * Description: This API is called to unlock the global data access.
 *
 * Inputs:  - 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/  
void pmi_freeca_unlock(VOID)
{
    /* un-lock the database object semaphores. Simply */
    /* abort if semaphore unlock fails.            */
    if (x_sema_unlock( h_ca_lock ) != OSR_OK)
    {
        FREECA_ABORT (DBG_CAT_SEMAPHORE, FREECA_DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
    }
    return;
}

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_es_handle_free_nfy_fct
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
BOOL pmi_freeca_es_handle_free_nfy_fct (HANDLE_T       h_handle,
                                HANDLE_TYPE_T  e_type,
                                VOID*          pv_obj,
                                VOID*          pv_tag,   /* reference to ESNODE */
                                BOOL           b_req_handle)
{  
    if ((e_type == CA_GROUP_FREE_CA_ES) && (pv_obj != NULL) && (pv_obj == pv_tag))
        return TRUE;
    else
        return FALSE;
}


/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_service_handle_free_nfy_fct
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
BOOL pmi_freeca_service_handle_free_nfy_fct (HANDLE_T       h_handle,
                                        HANDLE_TYPE_T  e_type,
                                        void*          pv_obj,
                                        VOID*          pv_tag, /* reference to SERVICENODE */
                                        BOOL           b_req_handle)
{    
    if ((e_type == CA_GROUP_FREE_CA_SV) && (pv_obj != NULL) && (pv_obj == pv_tag))
        return TRUE;              
    else    
        return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_esdb_close_fct
 *
 * Description: This API is responsible to close the ES.
 *
 * Inputs:  pt_es_node       Contains the reference to ESDB node. 
 *          b_forced         TRUE: closed by POD-CA  FALSE: closed by Client
 *          
 * Outputs: pt_es_node       Contains the reference to ESDB node for update 
 *                           status. 
 *
 * Returns: -
 ----------------------------------------------------------------------------*/             
VOID pmi_freeca_esdb_close_fct(CADB_ES_OBJ_T* pt_es_node, void *pb_forced)
{ 
    if (pt_es_node == NULL)
        return;
    
    /* Remove from Service */
    pmi_freeca_db_remove_from_service(pt_es_node);
    
    /* Update status */
    CA_ESNODE_CPINFO_SIZE(pt_es_node) = 0;   
    

#if (!FREECA_IGNORE_NOTIFY_FOR_CLOSE)  
    /* Call Notify Function */
    if (*((BOOL *)pb_forced) == FALSE)
    {
        if (CA_ESNODE_NFY(pt_es_node) != NULL)
            (CA_ESNODE_NFY(pt_es_node))(CA_ESNODE_HANDLE(pt_es_node),
                                        CA_ESNODE_CPINFO(pt_es_node),
                                        CA_ESNODE_TAG(pt_es_node));
    }
#endif    
    
    /* Delink and Free Handle */    
    handle_free(CA_ESNODE_HANDLE(pt_es_node), TRUE); 
    
    /* Free ES node */
    pmi_freeca_esdb_free(pt_es_node);
}  

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_ca_feedback_handler
 *
 * Description: This API is responsible to handle CA feedback from CableCard.
 *
 * Inputs:  h_ca             Contains the component CA handle. 
 *          e_nfy_cond       Contains the notification condition.
 *          e_cpinfo         Contains the CP information.
 *          pv_tag           Contains the tag for notification.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/    
VOID pmi_freeca_ca_feedback_handler(HANDLE_T          h_ca,
                               CA_DETAIL_COND_T  e_nfy_cond,
                               CP_INFO_LIST_T    e_cpinfo,
                               VOID*             pv_tag)
                      
{
    #if 0
    CADB_ES_OBJ_T* pt_es_node;
    PM_CA_COND_T   e_pmca_nfy_cond;
    
    pt_es_node = pv_tag;
    
    if (pt_es_node != NULL)
    {    
        if (pt_es_node->h_ca == h_ca)
        {   
            switch (e_nfy_cond)
            {
                case CA_DETAIL_UNKNOW:
                    e_pmca_nfy_cond = PM_CA_COND_UNKNOWN;              
                    break;                    
                default:
                    e_pmca_nfy_cond = PM_CA_COND_NOT_AUTHORIZED; 
                    break;
            }
            
            if (CA_ESNODE_NFY(pt_es_node) != NULL)
            {           
                CA_ESNODE_NFY(pt_es_node)(CA_ESNODE_HANDLE(pt_es_node), 
                                          e_cpinfo,
                                          CA_ESNODE_TAG(pt_es_node));
            }
        }
    }
    #endif
}                          
                                   

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_esdb_to_eslist_fct
 *
 * Description: This API is responsible to generate the related ES info for 
 *              PODCA.                    
 *
 * Inputs:  pt_es_node       Contains the reference to ESDB node. 
 *          
 * Outputs: pv_tag           Contains the reference to returned list.
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID pmi_freeca_esdb_to_eslist_fct(CADB_ES_OBJ_T* pt_es_node, void *pv_tag)
{
    FREECA_REQ_OBJ_T* pt_freeca_req;
    
    pt_freeca_req = pv_tag;
    
    (pt_freeca_req->pt_freeca_list)[pt_freeca_req->ui1_freeca_list_size].h_ca = CA_ESNODE_HANDLE(pt_es_node);    
    (pt_freeca_req->pt_freeca_list)[pt_freeca_req->ui1_freeca_list_size].t_es_pid = CA_ESNODE_PID(pt_es_node);  
    (pt_freeca_req->pt_freeca_list)[pt_freeca_req->ui1_freeca_list_size].pf_nfy = pmi_freeca_ca_feedback_handler;
    (pt_freeca_req->pt_freeca_list)[pt_freeca_req->ui1_freeca_list_size].pf_cpkey_nfy = NULL;
    (pt_freeca_req->pt_freeca_list)[pt_freeca_req->ui1_freeca_list_size].pv_tag = pt_es_node;    
    pt_freeca_req->ui1_freeca_list_size++;
    
}

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_init
 *
 * Description: This API is called to initialize the PM_CA module.
 *
 * Inputs:  ui2_num_services     Number of CA_SVDB node.
 *          ui2_num_comps        Number of CA_ESDB node. 
 *          pt_thread_descr      Contains the thread descriptor for notification 
 *                               from other modules. 
 *
 * Outputs: -
 *
 * Returns: PMR_OK               Success
 *          PMR_ALREADY_INIT     This module is already initialized.
 *          PMR_INV_ARG          ui2_num_services or ui2_num_comps is zero.
 *          PMR_NOT_ENOUGH_MEM   Not enough memory.
 ----------------------------------------------------------------------------*/                      
INT32 pmi_freeca_init(UINT16  ui2_num_services, UINT16  ui2_num_comps, THREAD_DESCR_T*  pt_thread_descr)
{
    INT32 i4_return;
    PM_CA_OPERATION_T t_ca_core_capi; 
    
    if ((ui2_num_services == 0) || (ui2_num_comps == 0))
    {
        return PMR_INV_ARG;
    }
    
    t_ca_core_capi.pf_capi_start_service = pmi_freeca_pi_start_service;
    t_ca_core_capi.pf_capi_stop_service = pmi_freeca_pi_stop_service;      
    t_ca_core_capi.pf_capi_add_component = pmi_freeca_pi_add_component;
    t_ca_core_capi.pf_capi_del_component = pmi_freeca_pi_del_component;
    
    if (pmi_freeca_db_init(ui2_num_services, ui2_num_comps) != PMR_OK)
        FREECA_ABORT(DBG_CAT_MEMORY, FREECA_DBG_ABRT_OUT_OF_MEMORY);
    
    pmi_freeca_lock_init();
    
    i4_return = pmi_freeca_register(t_ca_core_capi); 
    if (i4_return == PMR_OK)
    {        
        b_ca_core_init = TRUE;
    }  

    return i4_return;    
}

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_pi_start_service
 *
 * Description: This API is called to issue a service request to CA core.
 *
 * Inputs:  h_connection   	    Contains a handle referencing to the selected 
 *                              sevice. 
 *          e_processing_type	Contains the processing type for this service. 
 *          ui4_list_size 	    Contains the size of list referenced by 
 *                              pt_component_list .
 *          pt_component_list 	Contains the pointer to the list of stream 
 *                              components for this service. 
 *
 * Outputs: pt_component_list 	Contains the pointer to the list of stream 
 *                              components for this service. 
 *          ph_ca_service       Contains the pointer to the returned CA service 
 *                              handle.
 *
 * Returns: PMR_OK	            The routine was successful.
 *          PMR_INV_ARG	        The arguments of ui4_list_size, pt_component_list
 *                              or ph_ca_service is invalid.
 *          PMR_INV_HANDLE	    The argument h_connection contains an invalid or 
 *                              unused handle. 
 *          PMR_OUT_OF_HANDLES	No free handles are available.
 *          PMR_NOT_INIT	    No CA core is registered.
 *          PMR_FAILED	        Failure caused by other reason.
 ----------------------------------------------------------------------------*/  

INT32 pmi_freeca_pi_start_service (HANDLE_T             h_connection,    
                              PM_PROCESSING_TYPE_T e_processing_type,                                                                       
                              UINT32               ui4_list_size,                   
                              PM_CA_REQUEST_T      pt_component_list[],
                              HANDLE_T*            ph_ca_service)
{
    CADB_SV_OBJ_T*  pt_svdb_node = NULL;
    CADB_ES_OBJ_T*  pt_esdb_node = NULL;    
    INT32           i4_return = PMR_INV_ARG;
    UINT32          i,j;
    BOOL            b_es_ok, b_forced;

    if (b_ca_core_init == FALSE)
    {
        return (PMR_NOT_INIT);
    }
    
    if (x_handle_valid (h_connection) == FALSE)
    {
        return (PMR_INV_HANDLE);
    }
            
    if ((ph_ca_service == NULL)||((ui4_list_size > 0)&&(pt_component_list == NULL)))
    {
        return (PMR_INV_ARG);                
    } 
    
    /* Check invalid ES argument */           
    for (i=0; i< ui4_list_size; i++)
    {  
        if (pt_component_list[i].pf_nfy == NULL)
        {
            return (PMR_INV_ARG);       
        }
    }
    
    /* Lock */
    pmi_freeca_lock();
    
    if (pmi_freeca_svdb_alloc(&pt_svdb_node) != CA_OK)
    {
        i4_return = PMR_FAILED;
        goto EXIT;                     
       
    }
        
    /* Bind Handle and Object */
    if (handle_alloc(CA_GROUP_FREE_CA_SV, pt_svdb_node, pt_svdb_node, pmi_freeca_service_handle_free_nfy_fct, ph_ca_service) != HR_OK)
    {
        pmi_freeca_svdb_free(pt_svdb_node);
        i4_return = PMR_OUT_OF_HANDLES;
        goto EXIT;        
    }               
                            
    /* Fill Service info */
    CA_SVNODE_CONNECTION_HANDLE(pt_svdb_node) = h_connection;
    CA_SVNODE_CA_HANDLE(pt_svdb_node) = *ph_ca_service;
    CA_SVNODE_PROC_TYPE(pt_svdb_node) = e_processing_type;               
    CA_SVNODE_IS_USED(pt_svdb_node) = TRUE;   
    
    b_es_ok = TRUE;

    /* Handle es list */
    switch (e_processing_type)
    {
        case PM_PRESENT:
        case PM_RECORDING:
            
            for (i=0; i< ui4_list_size; i++)
            {                  
                /* reset output */
                pt_component_list[i].h_ca = NULL_HANDLE;
                pt_component_list[i].t_cp_info.ui1_ctrl_size =0;
            }
            
            /* allocate es node */                        
            for (i=0; i< ui4_list_size; i++)
            {  
                b_es_ok = FALSE;    
                
                /* Bind Handle and Object */
                if (pmi_freeca_esdb_alloc(&pt_esdb_node) == CA_OK)                                
                {  
                    if (handle_alloc(CA_GROUP_FREE_CA_ES, pt_esdb_node, pt_esdb_node, pmi_freeca_es_handle_free_nfy_fct, &(pt_component_list[i].h_ca)) == HR_OK)
                    { 
                        /* Fill es info */
                        CA_ESNODE_HANDLE(pt_esdb_node) = pt_component_list[i].h_ca;   
                        CA_ESNODE_DESCR(pt_esdb_node) = pt_component_list[i].t_stream_comp_id;
                        CA_ESNODE_PID(pt_esdb_node) = pt_component_list[i].t_component_pid;
                        CA_ESNODE_NFY(pt_esdb_node) = pt_component_list[i].pf_nfy;
                        CA_ESNODE_DESC_SET(pt_esdb_node) = pt_component_list[i].pf_desc_fct;
                        CA_ESNODE_TAG(pt_esdb_node) = pt_component_list[i].pv_tag;
                        CA_ESNODE_CPINFO_SIZE(pt_esdb_node) = pt_component_list[i].t_cp_info.ui1_ctrl_size;   
                                              
                        /* Attach ES to Service */
                        if (pmi_freeca_db_insert_to_service(pt_esdb_node, pt_svdb_node) == CA_OK)
                        {             
                            b_es_ok = TRUE;
                        }                                                                             
                    }        
                    else
                    {
                        i4_return = PMR_OUT_OF_HANDLES;
                    }
                }              
                else
                {
                    i4_return = PMR_FAILED;
                }
                
                if (b_es_ok == FALSE)
                {                   
                    
                    /* Free all allocated ES */
                    b_forced = TRUE;
                    pmi_freeca_db_parse_service(pt_svdb_node, (void *)(&b_forced), pmi_freeca_esdb_close_fct);
                    
                    /* ToDo: Free SV */
                    CA_SVNODE_IS_USED(pt_svdb_node) = FALSE;  
    
                    /* Free Handle */
                    handle_free(CA_SVNODE_CA_HANDLE(pt_svdb_node), TRUE);           
    
                    /* Free SV node */
                    pmi_freeca_svdb_free(pt_svdb_node);    
                    
                    
                    for (j=0; j<=i; j++)
                    {                  
                        /* reset output */
                        pt_component_list[j].h_ca = NULL_HANDLE;
                        pt_component_list[j].t_cp_info.ui1_ctrl_size =0;
                        
                    }
        
                    *ph_ca_service = NULL_HANDLE;     
                                
                    break;                    
                }  
            } 
            
            if (b_es_ok == TRUE)
            {
                i4_return = PMR_OK;  
            }
                
            break;
        default:
            break;
    }     
    
EXIT:
    /* Unlock */
    pmi_freeca_unlock();    
    
    return i4_return;       
   
}                                

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_pi_stop_service
 *
 * Description: This API is called to initialize the PM_CA module.
 *
 * Inputs:  h_ca_service   	     Contains a handle referencing to the existed 
 *                               sevice. 
 *
 * Outputs: -
 *
 * Returns: PMR_OK	             The routine was successful.
 *          PMR_INV_HANDLE	     The argument h_service contains an invalid or 
 *                               unused handle. 
 *          PMR_FAILED           Failure caused by other reason.
 *          PMR_NOT_INIT	     No CA core is registered.
 ----------------------------------------------------------------------------*/                                       
INT32 pmi_freeca_pi_stop_service (HANDLE_T     h_ca_service)
{
    CADB_SV_OBJ_T*  pt_svdb_node;
    INT32           i4_return, i4_res;
    BOOL            b_forced;
    HANDLE_TYPE_T   e_type;

    if (b_ca_core_init == FALSE)
    {
        return (PMR_NOT_INIT);
    }
        
    /* Lock */
    pmi_freeca_lock();
        
    i4_res = handle_get_type_obj(h_ca_service, &e_type, (VOID *)(&pt_svdb_node));
       
    if ((i4_res == HR_OK)&&(e_type == CA_GROUP_FREE_CA_SV)&&(CA_SVNODE_IS_USED(pt_svdb_node) == TRUE))     
    {
        /* Free all allocated ES */
        b_forced = FALSE; 
        pmi_freeca_db_parse_service(pt_svdb_node, (void *)(&b_forced), pmi_freeca_esdb_close_fct);
        
        /* Free SV */
        CA_SVNODE_IS_USED(pt_svdb_node) = FALSE;  
    
        /* Free Handle */
        handle_free(CA_SVNODE_CA_HANDLE(pt_svdb_node), TRUE);           
    
        /* Free SV node */
        pmi_freeca_svdb_free(pt_svdb_node); 
        
        i4_return = PMR_OK;          
    }
    else
    {
        i4_return = PMR_INV_HANDLE;
        goto EXIT;        
    }        

EXIT:
    /* Unlock */
    pmi_freeca_unlock();    
    
    return i4_return;           
} 

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_pi_add_component
 *
 * Description: This function is called by the Process Manager to add a stream 
 *              component to CA core.
 *
 * Inputs:  h_ca_service 	     Contains a handle referencing a existed ca 
 *                               service entry, which is the returned value 
 *                               ph_ca_service of API (*pm_capi_start_service).
 *          t_component  	     Contains the ES's CA request. 
 * Outputs: t_component  	     Contains the returned output for ES's CA 
 *                               request. 
 *
 * Returns: PMR_OK	             The routine was successful.
 *          PMR_INV_ARG	         The arguments of t_component is invalid.
 *          PMR_INV_HANDLE	     The argument h_service contains an invalid or 
 *                               unused handle. 
 *          PMR_OUT_OF_HANDLES	 No free handles are available.
 *          PMR_NOT_INIT	     CA core is not initialized.
 *          PMR_FAILED  	     Fail due to other reason.         
 ----------------------------------------------------------------------------*/                                                                 
INT32 pmi_freeca_pi_add_component (HANDLE_T            h_ca_service, 
                             PM_CA_REQUEST_T*    pt_component)
{
    CADB_SV_OBJ_T* pt_svdb_node = NULL;
    CADB_ES_OBJ_T* pt_esdb_node = NULL;  
    INT32 i4_return = PMR_INV_ARG;
    BOOL  b_forced;
    HANDLE_TYPE_T  e_type;

    if (b_ca_core_init == FALSE)
    {
        return (PMR_NOT_INIT);   
    }
 
    if ((h_ca_service == NULL_HANDLE)||(pt_component == NULL))
    {
        return (PMR_INV_ARG);                
    } 
    
    if (!((x_handle_valid (h_ca_service) == TRUE)&& (handle_get_type_obj(h_ca_service, &e_type, (VOID *)(&pt_svdb_node)) == HR_OK)
            &&(e_type == CA_GROUP_FREE_CA_SV)))
    {
        return (PMR_INV_HANDLE);                 
    }
    
    /* Check invalid ES argument */           
    if (pt_component->pf_nfy == NULL)
    {
        return (PMR_INV_ARG);           
    }
    
    /* Lock */
    pmi_freeca_lock();

    /* Handle es list */
    switch (CA_SVNODE_PROC_TYPE(pt_svdb_node))
    {
        case PM_PRESENT:       
        case PM_RECORDING:
                                             
            /* reset output */
            pt_component->h_ca = NULL_HANDLE;
            pt_component->t_cp_info.ui1_ctrl_size =0;                       
            
            /* allocate es node */                        
            
            /* Bind Handle and Object */            
            if (pmi_freeca_esdb_alloc(&pt_esdb_node) == CA_OK)                                
            {  
                if (handle_alloc(CA_GROUP_FREE_CA_ES, pt_esdb_node, pt_esdb_node, pmi_freeca_es_handle_free_nfy_fct, &(pt_component->h_ca)) == HR_OK)
                { 
                    /* Fill es info */
                    CA_ESNODE_HANDLE(pt_esdb_node) = pt_component->h_ca;   
                    CA_ESNODE_DESCR(pt_esdb_node) = pt_component->t_stream_comp_id;
                    CA_ESNODE_PID(pt_esdb_node) = pt_component->t_component_pid;
                    CA_ESNODE_NFY(pt_esdb_node) = pt_component->pf_nfy;
                    CA_ESNODE_DESC_SET(pt_esdb_node) = pt_component->pf_desc_fct;
                    CA_ESNODE_TAG(pt_esdb_node) = pt_component->pv_tag;
                    CA_ESNODE_CPINFO_SIZE(pt_esdb_node) = pt_component->t_cp_info.ui1_ctrl_size;
                                          
                    /* Attach ES to Service */
                    if (pmi_freeca_db_insert_to_service(pt_esdb_node, pt_svdb_node) == CA_OK)
                    {   
                        i4_return = PMR_OK;  
                    }                                                                             
                }        
                else
                {
                    i4_return = PMR_OUT_OF_HANDLES;                                                              
                }
            }                         
            else
            {
                i4_return = PMR_FAILED;       
            }
                
            if (i4_return != PMR_OK)
            {                       
                /* Free all allocated ES */
                b_forced = TRUE;
                if (pt_esdb_node != NULL)
                {
                    pmi_freeca_esdb_close_fct(pt_esdb_node, &b_forced); 
                }
                    
                /* reset output */
                pt_component->h_ca = NULL_HANDLE;
                pt_component->t_cp_info.ui1_ctrl_size = 0; 
            }
    
            break;
        default:
            break;
    }         

    /* Unlock */
    pmi_freeca_unlock();    
    
    return i4_return;       
        
}                            

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_pi_del_component
 *
 * Description: This function is called by the Process Manager to ask CA core 
 *              remove the existed stream component.
 *
 * Inputs:  h_ca            Contains a handle referencing a existed ca entry.  
 *
 * Outputs: -
 *
 * Returns: PMR_OK	        The routine was successful.
 *          PMR_INV_HANDLE	The argument h_ca contains an invalid or unused handle. 
 *          PMR_NOT_INIT	No CA core is registered.
 *          PMR_FAILED	    Failure caused by other reason.

 ----------------------------------------------------------------------------*/                                
INT32 pmi_freeca_pi_del_component (HANDLE_T    h_ca)
{
    CADB_ES_OBJ_T*  pt_esdb_node = NULL; 
    CADB_SV_OBJ_T*  pt_svdb_node = NULL;
    INT32           i4_return, i4_res;
    BOOL            b_forced;
    HANDLE_TYPE_T   e_type;

    if (b_ca_core_init == FALSE)   
        return (PMR_NOT_INIT);
    
    /* Lock */
    pmi_freeca_lock();    
    
    i4_res = handle_get_type_obj(h_ca, &e_type, (VOID *)(&pt_esdb_node));
       
    if ((i4_res == HR_OK)&&(e_type == CA_GROUP_FREE_CA_ES))     
    {
        /* Get the service node before free esnode */
        pmi_freeca_db_get_service_from_es(&pt_svdb_node, pt_esdb_node);
        
        /* Free allocated ES */
        b_forced = FALSE;  
        pmi_freeca_esdb_close_fct(pt_esdb_node, (void *)(&b_forced));   
        i4_return = PMR_OK;  
    }
    else
    {
        i4_return = PMR_INV_HANDLE;             
    }    

    /* Unlock */
    pmi_freeca_unlock(); 
    return i4_return;    
}
#endif


