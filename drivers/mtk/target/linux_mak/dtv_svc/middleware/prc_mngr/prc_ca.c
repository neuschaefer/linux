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
 * $RCSfile: prc_ca.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Felisa Hsiao $
 * $MD5HEX: 7f9f49cf1c18acf08c2a3a5579c8c60d $
 *
 * Description: 
 *         This file is the implementation of CA_PI modules.
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
#include "prc_mngr/prc_db.h"
#include "prc_mngr/prc_filter.h"
#include "prc_mngr/prc_lock.h"
#include "prc_mngr/prc_util.h"
#include "prc_mngr/prc_thread.h"
#include "prc_mngr/prc_desc.h"
#include "prc_mngr/prc_ca.h"
#include "prc_mngr/prc_freeca.h"
#include "prc_mngr/prc_debug.h"

#include "conn_mngr/x_cm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define SUPPORT_MULTIPLE_SOURCE    1

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/ 
static UINT16 ui2_cadb_num_total;
static PMCA_SYSNODE_T* pt_pm_cadb_arr;   
static BOOL b_ca_init;

static PM_CA_REQUEST_T* pt_careq_list;
static PM_DESC_SETTING_T* pt_g_desc_setting_list;
static UINT16 ui2_careq_count;

static PMCA_SYSNODE_T t_freeca_ctrl;
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern HP_TYPE_T pm_es_ca_feedback_fct (UINT16         ui2_count,
                         UINT16         ui2_max_count,
                         HANDLE_T       h_handle,
                         HANDLE_TYPE_T  e_type,
                         VOID*          pv_obj,
                         VOID*          pv_tag,
                         VOID*          pv_parse_data);                    
                          
static VOID pm_ca_pi_nfy_fct(
                HANDLE_T                h_ca,
                CP_INFO_LIST_T          t_cp_info,
                VOID*                   pv_tag);

static VOID pm_ca_key_nfy_fct(HANDLE_T           h_ca, 
                              PM_DESC_SETTING_T  t_desc_setting,
                              VOID*              pv_tag);                                           

static VOID pm_ca_key_init_nfy_fct(HANDLE_T           h_ca, 
                              PM_DESC_SETTING_T  t_desc_setting,
                              VOID*              pv_tag);

static VOID pm_db_parser_ca_request_fct(PM_COMPONENT_INFO_T* pt_es_node);   
static VOID pm_db_parser_ca_request_ret_fct(PM_COMPONENT_INFO_T* pt_es_node);  
static VOID pm_db_parser_ca_close_fct(PM_COMPONENT_INFO_T* pt_es_node);  
/*-----------------------------------------------------------------------------
 * Name: pm_db_parser_ca_request_fct
 *
 * Description: This API is responsible to initialize CA request by ES. 
 *
 * Inputs:  pt_es_node       Contains the reference to ESDB node. 
 *
 * Outputs: pt_es_node       Contains the reference to ESDB node for update 
 *                           status. 
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
VOID pm_db_parser_ca_request_fct(PM_COMPONENT_INFO_T* pt_es_node)
{
    if (ESNODE_CA_HANDLE(pt_es_node)== NULL_HANDLE && PM_CA_REQUIRE_MANDATORY == ESNODE_CA_REQ_LVL(pt_es_node))
    {
        PM_DBG_INFO("C[%5d]prepare CA component\n", ESNODE_PID(pt_es_node));
        pt_careq_list[ui2_careq_count].t_stream_comp_id = ESNODE_DESCR(pt_es_node);
        pt_careq_list[ui2_careq_count].t_component_pid = ESNODE_PID(pt_es_node);
        pt_careq_list[ui2_careq_count].pf_nfy = pm_ca_pi_nfy_fct;
        pt_careq_list[ui2_careq_count].pf_desc_fct = pm_ca_key_nfy_fct;
        pt_careq_list[ui2_careq_count].pv_tag = pt_es_node;
        pt_careq_list[ui2_careq_count].pf_desc_init_fct = pm_ca_key_init_nfy_fct;
        pt_careq_list[ui2_careq_count].pv_desc_init_tag = &pt_g_desc_setting_list[ui2_careq_count];
        ui2_careq_count ++; 
    }
}  

/*-----------------------------------------------------------------------------
 * Name: pm_db_parser_ca_request_ret_fct
 *
 * Description: This API is responsible to initialize CA request by ES. 
 *
 * Inputs:  pt_es_node       Contains the reference to ESDB node. 
 *
 * Outputs: pt_es_node       Contains the reference to ESDB node for update 
 *                           status. 
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
VOID pm_db_parser_ca_request_ret_fct(PM_COMPONENT_INFO_T* pt_es_node)
{    
    DESC_KEY_INFO_T*        pt_dst;
    
    ESNODE_CA_HANDLE(pt_es_node) = pt_careq_list[ui2_careq_count].h_ca;
    ESNODE_CP_INFO(pt_es_node) = pt_careq_list[ui2_careq_count].t_cp_info;
    /*set cp info*/
    pmi_thread_cp_updated(ESNODE_CA_HANDLE(pt_es_node),pt_es_node);
    
    if ((ESNODE_CRYPTO_HANDLE(pt_es_node)) != NULL_HANDLE)
    {
        pmi_descrambler_set( pt_es_node, pt_g_desc_setting_list+ui2_careq_count ); 

        pt_dst = &pt_g_desc_setting_list[ui2_careq_count].u_desc_cmd_info.t_key_info.t_desc_key;

        if (CA_SET_KEY == pt_g_desc_setting_list[ui2_careq_count].t_desc_set_cmd)
        {
            switch (pt_dst->e_descrambler_alg)
            {
                case DESC_ALG_DES_ECB:
                    PM_DBG_INFO("descrambler alg is DESC_ALG_DES_ECB\n");
                    if (0 != pt_dst->u_key_info.t_des_ecb_key.z_key_len)
                    {
                        x_mem_free( (VOID*)pt_dst->u_key_info.t_des_ecb_key.pui1_key );
                        pt_dst->u_key_info.t_des_ecb_key.pui1_key = NULL;
                        pt_dst->u_key_info.t_des_ecb_key.z_key_len = 0;
                    }
                    break;
                    
                case DESC_ALG_AES_128_CBC:
                    PM_DBG_INFO("descrambler alg is DESC_ALG_AES_128_CBC\n");
                    if (0 != pt_dst->u_key_info.t_aes_128_cbc_key.z_key_len)
                    {
                        x_mem_free( (VOID*)pt_dst->u_key_info.t_aes_128_cbc_key.pui1_key );
                        pt_dst->u_key_info.t_aes_128_cbc_key.pui1_key = NULL;
                        pt_dst->u_key_info.t_aes_128_cbc_key.z_key_len = 0;
                    }

                    if (0 != pt_dst->u_key_info.t_aes_128_cbc_key.z_iv_len)
                    {
                        x_mem_free( (VOID*)pt_dst->u_key_info.t_aes_128_cbc_key.pui1_iv );
                        pt_dst->u_key_info.t_aes_128_cbc_key.pui1_iv = NULL;
                        pt_dst->u_key_info.t_aes_128_cbc_key.z_iv_len = 0;
                    }
                    break;

                default:
                    PM_DBG_INFO("descrambler alg is UNKNOWN\n");
                    break;
            }
        }
    }                        
    ui2_careq_count ++;    
}  

/*-----------------------------------------------------------------------------
 * Name: pm_db_parser_ca_close_fct
 *
 * Description: This API is responsible to notify that CAES is closed. 
 *
 * Inputs:  pt_es_node       Contains the reference to ESDB node. 
 *
 * Outputs: pt_es_node       Contains the reference to ESDB node for update 
 *                           status. 
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
VOID pm_db_parser_ca_close_fct(PM_COMPONENT_INFO_T* pt_es_node)
{    
    PM_CA_COND_T         t_cond;
      
    if (pt_es_node == NULL)
        return;
    
    if (ESNODE_CA_HANDLE(pt_es_node) == NULL_HANDLE)
        return;
        
    ESNODE_CA_COND(pt_es_node) = PM_CA_COND_CLOSED;  
    ESNODE_CA_HANDLE(pt_es_node) = NULL_HANDLE;
    t_cond = ESNODE_CA_COND(pt_es_node); 
                  
    /* Call Notify Function */
    handle_parse(&(ESNODE_LINK(pt_es_node)), pm_es_ca_feedback_fct, &t_cond);  
}  

/*-----------------------------------------------------------------------------
 * Name: pm_ca_pi_nfy_fct
 *
 * Description: This API is responsible to handlle notification from CA. 
 *
 * Inputs:  h_ca             Contains ES's CA hanlde. 
 *          e_nfy_cond       Contains the condition.
 *          ui4_detail_code  Contains the detail code.
 *          t_cp_info        Contains the CP information
 *          pv_tag           Contains the tag specified by ES's PM_CA_REQUEST_T
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
VOID pm_ca_pi_nfy_fct(
                HANDLE_T                h_ca,
                CP_INFO_LIST_T          t_cp_info,
                VOID*                   pv_tag)
{
    
    PM_COMPONENT_INFO_T* pt_esdb_node;   
            
    pt_esdb_node = pv_tag;
    
    /* Lock */
    pmi_lock_db_wr();    
    
    if (pmi_esdb_is_valid(pt_esdb_node) == TRUE)
    {
        if ((ESNODE_CA_HANDLE(pt_esdb_node) == h_ca)&&
            (x_memcmp(&t_cp_info, &(ESNODE_CP_INFO(pt_esdb_node)), sizeof(CP_INFO_LIST_T)) != 0))
        {
            ESNODE_CP_INFO(pt_esdb_node) = t_cp_info;
            pmi_thread_cp_updated(h_ca, pt_esdb_node);
        }
        
    }           
    
    /* Unlock */
    pmi_unlock_db();    
}                     

/*-----------------------------------------------------------------------------
 * Name: pm_ca_key_nfy_fct
 *
 * Description: This API is responsible to handlle descrambler key setting
 *              from CA. 
 *
 * Inputs:  h_ca             Contains ES's CA hanlde. 
 *          t_desc_setting   Contains the descrambler's setting.
 *          pv_tag           Contains the tag specified by ES's PM_CA_REQUEST_T
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
VOID pm_ca_key_nfy_fct(
                HANDLE_T                h_ca, 
                PM_DESC_SETTING_T       t_pm_desc_setting,
                VOID*                   pv_tag)
{
    
    PM_COMPONENT_INFO_T* pt_esdb_node;   
            
    pt_esdb_node = pv_tag;
    
    /* Lock */
    pmi_lock_db_wr();    
    
    if (pmi_esdb_is_valid(pt_esdb_node) == TRUE)
    {
        if (ESNODE_CA_HANDLE(pt_esdb_node) == h_ca)
        {
            /* Update descramble key */      
            pmi_descrambler_set( pt_esdb_node, &t_pm_desc_setting ); 
        }        
    }   
    
    /* Unlock */
    pmi_unlock_db();           
}         

VOID pm_ca_key_init_nfy_fct(
                HANDLE_T                h_ca, 
                PM_DESC_SETTING_T       t_desc_setting,
                VOID*                   pv_tag)
{
    PM_DESC_SETTING_T*      pt_local_setting = pv_tag;
    DESC_KEY_INFO_T*        pt_src;
    DESC_KEY_INFO_T*        pt_dst;
    UINT16                  ui2_desc_offset;

    ui2_desc_offset =  pt_local_setting - pt_g_desc_setting_list;
    
    x_memcpy(pt_g_desc_setting_list+ui2_desc_offset,&t_desc_setting,sizeof(PM_DESC_SETTING_T));
    
    pt_src = &t_desc_setting.u_desc_cmd_info.t_key_info.t_desc_key;
    pt_dst = &(pt_g_desc_setting_list+ui2_desc_offset)->u_desc_cmd_info.t_key_info.t_desc_key;

    if (CA_SET_KEY == t_desc_setting.t_desc_set_cmd)
    {
        switch (pt_src->e_descrambler_alg)
        {
            case DESC_ALG_DES_ECB:
                pt_dst->u_key_info.t_des_ecb_key.pui1_key = x_mem_alloc( pt_src->u_key_info.t_des_ecb_key.z_key_len*sizeof(UINT8) );
                x_memcpy(
                    (VOID*)pt_dst->u_key_info.t_des_ecb_key.pui1_key,
                    pt_src->u_key_info.t_des_ecb_key.pui1_key,
                    pt_src->u_key_info.t_des_ecb_key.z_key_len );
                break;
                
            case DESC_ALG_AES_128_CBC:
                pt_dst->u_key_info.t_aes_128_cbc_key.pui1_key = x_mem_alloc( pt_src->u_key_info.t_aes_128_cbc_key.z_key_len*sizeof(UINT8) );
                x_memcpy(
                    (VOID*)pt_dst->u_key_info.t_aes_128_cbc_key.pui1_key,
                    pt_src->u_key_info.t_aes_128_cbc_key.pui1_key,
                    pt_src->u_key_info.t_aes_128_cbc_key.z_key_len );
                pt_dst->u_key_info.t_aes_128_cbc_key.pui1_iv = x_mem_alloc( pt_src->u_key_info.t_aes_128_cbc_key.z_iv_len*sizeof(UINT8) );
                x_memcpy(
                    (VOID*)pt_dst->u_key_info.t_aes_128_cbc_key.pui1_iv,
                    pt_src->u_key_info.t_aes_128_cbc_key.pui1_iv,
                    pt_src->u_key_info.t_aes_128_cbc_key.z_iv_len );
                break;

            default:
                break;
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: pm_ca_select_engine
 *
 * Description: This API is called to select the appropriate CA engine.
 *
 * Inputs:  h_connection         Contains the CM connection handle.
 *          ui2_ca_sys_id        Contains the related CA system ID for current 
 *                               service.
 *          pt_ca_ctrl           Contains the reference to returned CA engine.
 *
 * Outputs: pt_ca_ctrl           Contains the reference to returned CA engine.
 * Returns: TRUE                 Success
 *          FALSE                Failed
 ----------------------------------------------------------------------------*/    
BOOL pmi_ca_select_engine(HANDLE_T        h_connection, 
                          UINT16          ui2_ca_sys_id, 
                          PMCA_SYSNODE_T** ppt_ca_ctrl)            
{
#if (SUPPORT_MULTIPLE_SOURCE)    
    CM_CTRL_RM_SRC_COMP_T  t_ctrl_rm_src_comp; 
    INT32                  i4_rt_val;    
    DRV_TYPE_T             e_source_type    = 0;
    UINT16                 ui2_source_id    = 0;    
    UINT8                  ui1_num_inp_ports;
    UINT8                  ui1_num_out_ports;
    UINT32                 ui4_comp_flags;    
    UINT32                 i;
#endif    
    
    if ((ppt_ca_ctrl == NULL) || (h_connection == NULL_HANDLE))
        return FALSE;
    
    *ppt_ca_ctrl = &t_freeca_ctrl;
        
#if (SUPPORT_MULTIPLE_SOURCE)
    
    i4_rt_val = x_cm_get(h_connection, 
                          CM_CTRL_RM_SRC_COMP,
                          (VOID*) & t_ctrl_rm_src_comp);
    if (i4_rt_val == CMR_OK)
    {
        rm_get_comp_info_from_handle (t_ctrl_rm_src_comp.h_rm_comp,
                                      &e_source_type,
                                      &ui2_source_id,
                                      &ui1_num_inp_ports,
                                      &ui1_num_out_ports,
                                      &ui4_comp_flags);
                                      
       
                                      
        for(i=0; i<ui2_cadb_num_total; i++)
        {
            if ((pt_pm_cadb_arr[i].b_is_used == TRUE) && 
               (pt_pm_cadb_arr[i].e_source_type == e_source_type) &&
               (pt_pm_cadb_arr[i].ui2_source_id == ui2_source_id))
            {
                *ppt_ca_ctrl = &(pt_pm_cadb_arr[i]);
                return TRUE;
            }
        }
        
        return FALSE;
    }                     
    else
        return FALSE;
     
#else  
    if (pt_pm_cadb_arr[0].b_is_used == TRUE)
    {
        *ppt_ca_ctrl = &(pt_pm_cadb_arr[0]);        
        return TRUE;
    }
    
    return FALSE;    
       
#endif    
    
}                                    
 

/*-----------------------------------------------------------------------------
 * Name: pmi_ca_init
 *
 * Description: This API is called to initialize the PM_CA module.
 *
 * Inputs:  ui2_num_ca_systems Contains the number of CA system. 
 *
 * Outputs: -
 *
 * Returns: PMR_OK               Success
 *          PMR_ALREADY_INIT     This module is already initialized.
 *          PMR_INV_ARG          ui2_num_ca_systems is zero.
 *          PMR_NOT_ENOUGH_MEM   Not enough memory.
 ----------------------------------------------------------------------------*/                      
INT32 pmi_ca_init(UINT16  ui2_num_ca_systems)
{
    INT32 i4_return;
    
    if (b_ca_init == TRUE)
    {
        i4_return = PMR_ALREADY_INIT;
        goto EXIT;
    }
    
    if (ui2_num_ca_systems == 0)
    {
        i4_return = PMR_INV_ARG;
        goto EXIT;
    }    
    
    if ((pt_pm_cadb_arr = (PMCA_SYSNODE_T*) x_mem_alloc (sizeof (PMCA_SYSNODE_T) * ui2_num_ca_systems)) == NULL)
    {
        i4_return = PMR_NOT_ENOUGH_MEM;
         goto EXIT;
    }
    
    x_memset(pt_pm_cadb_arr, 0, (sizeof (PMCA_SYSNODE_T) * ui2_num_ca_systems));
    x_memset(&t_freeca_ctrl, 0, sizeof(PMCA_SYSNODE_T));    
    ui2_cadb_num_total = ui2_num_ca_systems;
    pt_careq_list =  NULL;
    b_ca_init = TRUE;  
    
    pmi_freeca_init(16,16, NULL);
    
    i4_return = PMR_OK;

EXIT:
    PM_ERR_CODE(i4_return);
    return i4_return;    
}

/*-----------------------------------------------------------------------------
 * Name: pmi_ca_register
 *
 * Description: This API is called by CA engine to register its CA operation 
 *              table. 
 *
 * Inputs:  b_alloc_desc          TRUE: Need allocate descrambler 
 *                                FALSE: Don't need to allocate descrambler
 *          pt_desc_info          Contains the reference to descrambler 
 *                                information. 
 *          e_source_type         Contains the data source type (tuner,..)
 *          ui2_source_id         Contains the component id for data source. 
 *          ui2_ca_sys_id         Contains the CA system id. 
 *          t_ca_core_capi        Contains the CA operation table.
 *
 * Outputs: -
 *
 * Returns: PMR_OK                Success
 *          PMR_NOT_INIT          CA_PI is not initialized.
 *          PMR_INV_ARG           Part of the operation is NULL.  
 *          PMR_NOT_SURPPORT      The algorithm is not surpported. 
 *          PMR_ALREADY_REGISTER  This module is already registered.
 *          PMR_FAILED            No availabe resource
 ----------------------------------------------------------------------------*/ 
INT32 pmi_ca_register(BOOL              b_alloc_desc,
                      PM_DESC_INFO_T*   pt_desc_info,
                      DRV_TYPE_T        e_source_type,
                      UINT16            ui2_source_id,
                      UINT16            ui2_ca_sys_id,
                      PM_CA_OPERATION_T t_ca_core_capi)
{
    INT32   i4_return;    
    UINT16  i;
    
    if (b_ca_init == FALSE)
    {
        i4_return = PMR_NOT_INIT;
        goto EXIT;
    }
    
    /* Check operation */
    if ((t_ca_core_capi.pf_capi_start_service == NULL) ||
        (t_ca_core_capi.pf_capi_stop_service == NULL) ||
        (t_ca_core_capi.pf_capi_add_component == NULL) ||
        (t_ca_core_capi.pf_capi_del_component == NULL) ||
        ((b_alloc_desc == TRUE) && (pt_desc_info == NULL)))
    {
        i4_return = PMR_INV_ARG;
        goto EXIT;
        
    }    
    
    /* ToDo: Check the descrambler is available or not. */         
    

    
    /* Check the CA system is already registered or not */
    for(i=0; i<ui2_cadb_num_total; i++)
    {
        if (pt_pm_cadb_arr[i].b_is_used == TRUE)
        {
        #if 1
            if ((pt_pm_cadb_arr[i].e_source_type == e_source_type)&&
                (x_memcmp(&t_ca_core_capi, &(pt_pm_cadb_arr[i].t_operation), sizeof(PM_CA_OPERATION_T)) == 0) &&
                (pt_pm_cadb_arr[i].ui2_ca_sys_id == ui2_ca_sys_id))    
        #else                    
            if ((x_memcmp(&t_ca_core_capi, &(pt_pm_cadb_arr[i].t_operation), sizeof(PM_CA_OPERATION_T)) == 0) &&
                (pt_pm_cadb_arr[i].ui2_ca_sys_id == ui2_ca_sys_id) &&
                (x_strcmp(pt_pm_cadb_arr[i].ac_algorithm_name, ac_algorithm_name) == 0))
        #endif
            {
                i4_return = PMR_ALREADY_REGISTER;
                goto EXIT;
            } 
        }
    }
    
    /* Get a free one */
    for(i=0; i<ui2_cadb_num_total; i++)
    {
        if (pt_pm_cadb_arr[i].b_is_used == FALSE)
        {
            pt_pm_cadb_arr[i].b_is_used = TRUE;
            pt_pm_cadb_arr[i].e_source_type = e_source_type;
            pt_pm_cadb_arr[i].ui2_source_id = ui2_source_id;            
            pt_pm_cadb_arr[i].ui2_ca_sys_id = ui2_ca_sys_id;
            pt_pm_cadb_arr[i].b_alloc_desc = b_alloc_desc;
            if (b_alloc_desc == TRUE)
            {
                x_strcpy(pt_pm_cadb_arr[i].ac_algorithm_name, pt_desc_info->ac_algorithm_name);
                pt_pm_cadb_arr[i].e_desc_type = pt_desc_info->e_desc_type;
            }
            
            pt_pm_cadb_arr[i].t_operation = t_ca_core_capi;
            i4_return = PMR_OK;
            goto EXIT;           
        }
    }    
    
    i4_return = PMR_FAILED;

EXIT:
    PM_ERR_CODE(i4_return);
    return i4_return;        
} 

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_register
 *
 * Description: This API is called by free CA engine to register its CA operation 
 *              table. 
 *
 * Inputs:  t_ca_core_capi        Contains the CA operation table.
 *
 * Outputs: -
 *
 * Returns: PMR_OK                Success
 *          PMR_NOT_INIT          CA_PI is not initialized.
 *          PMR_INV_ARG           Part of the operation is NULL.  
 *          PMR_NOT_SURPPORT      The algorithm is not surpported. 
 *          PMR_ALREADY_REGISTER  This module is already registered.
 *          PMR_FAILED            No availabe resource
 ----------------------------------------------------------------------------*/ 
INT32 pmi_freeca_register(PM_CA_OPERATION_T t_ca_core_capi)
{
    INT32 i4_return;   
    
    if (b_ca_init == FALSE)
    {
        i4_return = PMR_NOT_INIT;
        goto EXIT;
    }
    
    if (t_freeca_ctrl.b_is_used == TRUE)
    {
        i4_return = PMR_ALREADY_REGISTER;
         goto EXIT;
    }
    
    /* Check operation */
    if ((t_ca_core_capi.pf_capi_start_service == NULL) ||
        (t_ca_core_capi.pf_capi_stop_service == NULL) ||
        (t_ca_core_capi.pf_capi_add_component == NULL) ||
        (t_ca_core_capi.pf_capi_del_component == NULL))
    {
        i4_return = PMR_INV_ARG;
        goto EXIT;
        
    }  
        
    t_freeca_ctrl.t_operation = t_ca_core_capi;
    i4_return = PMR_OK;

EXIT:
    PM_ERR_CODE(i4_return);
    return i4_return;        
} 

/*-----------------------------------------------------------------------------
 * Name: pmi_ca_start_service
 *
 * Description: This API is called to start CA processing for service.
 *
 * Inputs:  pt_svdb_node     Contains the reference to service node.
 *
 * Outputs: pt_svdb_node     Contains the new allocated CA service handle.
 *
 * Returns: PMR_OK               Success 
 *          PMR_NOT_INIT         CA_PI is not initialized. 
 *          PMR_INV_HANDLE       Handle h_connection is invalid. 
 *          PMR_INV_ARG          pt_svdb_node is NULL or the content of 
 *                               pt_svdb_node is invalid. 
 *          PMR_OUT_OF_HANDLES   No handle available. 
 *          PMR_FAILED           Failed due to other reason.
 ----------------------------------------------------------------------------*/ 
INT32 pmi_ca_start_service(PM_SERVICE_INFO_T* pt_svdb_node)
{
    INT32                i4_return;       
    UINT16               ui2_es_count;                    
    PM_CA_REQUEST_T      *pt_component_list = NULL;
    PM_DESC_SETTING_T*      pt_desc_setting_list = NULL;
    UINT16               ui2_curr_index;    
    PM_COMPONENT_INFO_T* pt_curr_es_node;
    PM_CA_REQUEST_T      t_component;
    PM_DESC_SETTING_T       t_desc_setting_list;
    PMCA_SYSNODE_T*      pt_ca_ctrl;
    
    if (b_ca_init == FALSE)
    {
        i4_return = PMR_NOT_INIT;
        goto EXIT;
    }
    
    if (pmi_svdb_is_valid(pt_svdb_node) == FALSE)
    {
        PM_DBG_ERROR("invalid service node\n");
        i4_return = PMR_INV_ARG;
        goto EXIT;
    }

    if (SVNODE_CA_HANDLE(pt_svdb_node) != NULL_HANDLE)
    {
        PM_DBG_INFO("service node has CA handle\n");
        i4_return = PMR_OK;
        goto EXIT;
    }
    
    /* Count ES */
    i4_return = pmi_db_count_es(pt_svdb_node, &ui2_es_count);
    if (i4_return != PMR_OK)
    {
        PM_DBG_ERROR("Failed at count es\n");
        i4_return = PMR_INV_ARG;
        goto EXIT;
    }
    
    if (0 != ui2_es_count)
    {
        pt_component_list = x_mem_alloc(sizeof(PM_CA_REQUEST_T) * ui2_es_count);
        if  (pt_component_list == NULL)
        {
            PM_DBG_ERROR("allocate memory FAIL\n");
            i4_return = PMR_FAILED;
            goto EXIT;        
        }

        pt_desc_setting_list = x_mem_alloc(sizeof(PM_DESC_SETTING_T) * ui2_es_count);
        if  (pt_desc_setting_list == NULL)
        {
            PM_DBG_ERROR("allocate memory FAIL\n");
            x_mem_free( pt_component_list );
            i4_return = PMR_FAILED;
            goto EXIT;        
        }
    }
    else
    {
        PM_DBG_INFO("component list is NULL\n");
        pt_component_list = NULL;
        pt_desc_setting_list = NULL;
    }

    /* Initialize CA request */     
    pt_careq_list = pt_component_list;
    pt_g_desc_setting_list = pt_desc_setting_list;
    ui2_careq_count = 0;
    pmi_db_parse_service(pt_svdb_node, pm_db_parser_ca_request_fct);
    
    pt_ca_ctrl = SVNODE_CA_CTRL(pt_svdb_node);
    
    if (pt_ca_ctrl != NULL)
    {    
        /* Never call capi_start_service for this service */
        if (ui2_careq_count == ui2_es_count)
        {
            PM_DBG_INFO("begin start CA service [es count:%d] ...\n", ui2_es_count);
            i4_return = pt_ca_ctrl->t_operation.pf_capi_start_service(
                                                SVNODE_CONNECTION_HANDLE(pt_svdb_node),    
                                                SVNODE_PROC_TYPE(pt_svdb_node),                                                                       
                                                (UINT32)ui2_es_count,                   
                                                pt_component_list,
                                                &(SVNODE_CA_HANDLE(pt_svdb_node)));     
            
            /* Copy the returned output */
            if (0 <= i4_return)
            {
                ui2_careq_count = 0;
                pmi_db_parse_service(pt_svdb_node, pm_db_parser_ca_request_ret_fct); 
            }
            else
            {
                PM_DBG_ERROR("start CA service FAIL[%d]\n", i4_return);
            }
        }
        else
        {
            /* ToDo: use a better way */
            PM_DBG_INFO("begin start CA service [es count:%d][careq_count:%d] ...\n", ui2_es_count, ui2_careq_count);
            ui2_curr_index = pt_svdb_node->ui2_node_head;                             
            i4_return = PMR_OK;
            
            while(ui2_curr_index != PM_NULL_IDX)
            {
                pt_curr_es_node = get_es_node_from_idx(ui2_curr_index);
                
                if (pt_curr_es_node != NULL)
                {
                    /* Retrive the next ES node's index first */                
                    ui2_curr_index = pt_curr_es_node->ui2_next;  
        
                    if (ESNODE_CA_HANDLE(pt_curr_es_node)== NULL_HANDLE && PM_CA_REQUIRE_MANDATORY == ESNODE_CA_REQ_LVL(pt_curr_es_node))
                    { 
                        pt_careq_list = &t_component;
                        pt_g_desc_setting_list = &t_desc_setting_list;
                        ui2_careq_count = 0;
                        pm_db_parser_ca_request_fct( pt_curr_es_node );

                        if (NULL_HANDLE == SVNODE_CA_HANDLE(pt_svdb_node))
                        {
                            i4_return = pt_ca_ctrl->t_operation.pf_capi_start_service(
                                                SVNODE_CONNECTION_HANDLE(pt_svdb_node),    
                                                SVNODE_PROC_TYPE(pt_svdb_node),                                                                       
                                                0,                   
                                                NULL,
                                                &(SVNODE_CA_HANDLE(pt_svdb_node))); 
                            if (i4_return < 0)
                            {
                                PM_DBG_ERROR("start CA service FAIL[%d]\n", i4_return);
                                break;
                            }
                        }
                        
                        if ((i4_return = pt_ca_ctrl->t_operation.pf_capi_add_component(SVNODE_CA_HANDLE(pt_svdb_node), &t_component))== PMR_OK)                     
                        {
                            ESNODE_CA_HANDLE(pt_curr_es_node) = t_component.h_ca;
                            ESNODE_CP_INFO(pt_curr_es_node) = t_component.t_cp_info;

                            ui2_careq_count = 0;
                            pm_db_parser_ca_request_ret_fct( pt_curr_es_node );
                        }        
                        else
                        {
                            PM_DBG_ERROR("add CA component FAIL[%d]\n", i4_return);
                            break;
                        }
                    }                
                }
                else
                {
                    i4_return = PMR_FAILED;
                    goto EXIT;
                }
            }    
            
            if (i4_return != PMR_OK)
            {
                PM_DBG_INFO("stop CA service if error happen\n");
                pmi_ca_stop_service(pt_svdb_node);            
            }
            
        }
    }
    else
    {
        PM_DBG_ERROR("have NOT select ca engine for service\n");
        i4_return = PMR_FAILED;
        goto EXIT;  
    }    
    
EXIT:
    PM_ERR_CODE(i4_return);
        /* Free CA_REQUEST list */
    if (NULL != pt_component_list)
    {
        x_mem_free(pt_component_list);
    }

    if (NULL != pt_desc_setting_list)
    {
        x_mem_free(pt_desc_setting_list);
    } 
    pt_careq_list = NULL;
    pt_g_desc_setting_list = NULL;
    ui2_careq_count = 0; 
    
    return i4_return;  
} 

/*-----------------------------------------------------------------------------
 * Name: pmi_ca_stop_service
 *
 * Description: This API is called to stop CA processing for service.
 *
 * Inputs:  pt_svdb_node     Contains the reference to service node.
 *
 * Outputs: pt_svdb_node     CA handle will be released.
 *
 * Returns: PMR_OK               Success 
 *          PMR_NOT_INIT         CA_PI is not initialized. 
 *          PMR_INV_ARG          pt_svdb_node is NULL or the content of 
 *                               pt_svdb_node is invalid. 
 *          PMR_FAILED           Failed due to other reason. 
 ----------------------------------------------------------------------------*/
INT32 pmi_ca_stop_service(PM_SERVICE_INFO_T* pt_svdb_node)
{
    INT32              i4_return;    
    PMCA_SYSNODE_T*    pt_ca_ctrl = NULL;
    
    if (b_ca_init == FALSE)
    {
        i4_return = PMR_NOT_INIT;
        goto EXIT;
    }
    
    if (pmi_svdb_is_valid(pt_svdb_node) == FALSE)
    {
        i4_return = PMR_INV_ARG;        
        goto EXIT;           
    } 
                         
    pt_ca_ctrl = SVNODE_CA_CTRL(pt_svdb_node);
    
    if (pt_ca_ctrl != NULL)
    {    
        if (SVNODE_CA_HANDLE(pt_svdb_node) != NULL_HANDLE)    
        {
            i4_return = pt_ca_ctrl->t_operation.pf_capi_stop_service(SVNODE_CA_HANDLE(pt_svdb_node));             
            if (i4_return == PMR_OK)
            {
                pmi_db_parse_service(pt_svdb_node, pm_db_parser_ca_close_fct);
                SVNODE_CA_HANDLE(pt_svdb_node) = NULL_HANDLE;            
            }
            else
            {
                PM_DBG_ERROR("stop CA service FAIL[%d]\n", i4_return);
            }
        }
        else
        {
            PM_DBG_ERROR("the service do NOT has ca service\n");
            i4_return = PMR_OK;
        }
    }
    else
    {
        i4_return = PMR_FAILED;        
        goto EXIT;          
    }
    
EXIT:
    PM_ERR_CODE(i4_return);
    return i4_return;    
} 

/*-----------------------------------------------------------------------------
 * Name: pmi_ca_add_component
 *
 * Description: This API is called to notify CA that a new component is added 
 *              to service.  
 *
 * Inputs:  pt_esdb_node     Contains the reference to ES node.
 *
 * Outputs: pt_esdb_node     Contains the the new allocated CA ES handle.
 *
 * Returns: PMR_OK               Success
 *          PMR_NOT_INIT         CA_PI is not initialized. 
 *          PMR_INV_ARG          Parameter pt_esdb_node is NULL. 
 *          PMR_NOT_REGISTER     NO CA engine is registered. 
 *          PMR_FAILED           Failed due to other reason. 
 ----------------------------------------------------------------------------*/ 
INT32 pmi_ca_add_component(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    INT32               i4_return;    
    PM_CA_REQUEST_T     t_component;
    PM_DESC_SETTING_T   t_desc_setting_list;
    PM_SERVICE_INFO_T*  pt_svdb_node;   
    PMCA_SYSNODE_T*     pt_ca_ctrl;    
    
    if (b_ca_init == FALSE)
    {
        i4_return = PMR_NOT_INIT;
        goto EXIT;
    }
    
    if (pmi_esdb_is_valid(pt_esdb_node) == FALSE)
    {
        i4_return = PMR_INV_ARG;        
        goto EXIT;           
    }

    if (ESNODE_CA_HANDLE(pt_esdb_node)== NULL_HANDLE && PM_CA_REQUIRE_MANDATORY == ESNODE_CA_REQ_LVL(pt_esdb_node))
    {
        pt_ca_ctrl = ESNODE_CA_CTRL(pt_esdb_node);
        
        /* Fill PM_CA_REQUEST_T */
        pt_careq_list = &t_component;
        pt_g_desc_setting_list = &t_desc_setting_list;
        ui2_careq_count = 0;
        pm_db_parser_ca_request_fct( pt_esdb_node );

        i4_return = PMR_FAILED;  
        pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_esdb_node));

        if (pt_svdb_node != NULL)
        {
            if (SVNODE_CA_HANDLE(pt_svdb_node) != NULL_HANDLE)
            {
                PM_DBG_INFO("C[%5d]add CA component ...\n", ESNODE_PID(pt_esdb_node));
                i4_return = pt_ca_ctrl->t_operation.pf_capi_add_component(SVNODE_CA_HANDLE(pt_svdb_node), &t_component);  
            }
            else
            {
                PM_DBG_INFO("C[%5d]begin start CA service because ...\n", ESNODE_PID(pt_esdb_node));
                i4_return = pt_ca_ctrl->t_operation.pf_capi_start_service(
                                                SVNODE_CONNECTION_HANDLE(pt_svdb_node),    
                                                SVNODE_PROC_TYPE(pt_svdb_node),                                                                       
                                                1,                   
                                                &t_component,
                                                &(SVNODE_CA_HANDLE(pt_svdb_node)));   
                
            }  
            
            if (i4_return < 0)
            {
                ESNODE_CA_HANDLE(pt_esdb_node) = NULL_HANDLE;
            }
            else
            {
                /* Copy the returned output */
                ESNODE_CA_HANDLE(pt_esdb_node) = t_component.h_ca;
                ESNODE_CP_INFO(pt_esdb_node) = t_component.t_cp_info;
            
                if ((ESNODE_CRYPTO_HANDLE(pt_esdb_node)) != NULL_HANDLE)
                {
                    ui2_careq_count = 0;
                    pm_db_parser_ca_request_ret_fct( pt_esdb_node );
                }
            }
        }
    }
    else
    {
        i4_return = PMR_OK;
    }
    
EXIT:
    PM_ERR_CODE(i4_return);
    
    pt_careq_list = NULL;
    pt_g_desc_setting_list = NULL;
    ui2_careq_count = 0;
    
    return i4_return;            
} 

/*-----------------------------------------------------------------------------
 * Name: pmi_ca_del_component
 *
 * Description: This API is called to notify CA that a component is deleted 
 *              from service.  
 *
 * Inputs:  pt_esdb_node     Contains the reference to ES node.
 *
 * Outputs: pt_esdb_node     CA handle will be released.
 *
 * Returns: PMR_OK               Success
 *          PMR_NOT_INIT         CA_PI is not initialized. 
 *          PMR_INV_ARG          Parameter pt_esdb_node is NULL. 
 *          PMR_NOT_REGISTER     NO CA engine is registered. 
 *          PMR_FAILED           Failed due to other reason. 
 ----------------------------------------------------------------------------*/ 
INT32 pmi_ca_del_component(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    INT32              i4_return;     
    PMCA_SYSNODE_T*    pt_ca_ctrl;        
    
    if (b_ca_init == FALSE)
    {
        i4_return = PMR_NOT_INIT;
        goto EXIT;
    }
    
    if (pmi_esdb_is_valid(pt_esdb_node) == FALSE)
    {
        i4_return = PMR_INV_ARG;        
        goto EXIT;           
    }
    
    pt_ca_ctrl = ESNODE_CA_CTRL(pt_esdb_node);
    
    if (ESNODE_CA_HANDLE(pt_esdb_node) != NULL_HANDLE)
    {    
        i4_return = pt_ca_ctrl->t_operation.pf_capi_del_component(ESNODE_CA_HANDLE(pt_esdb_node));   
        if (i4_return == PMR_OK)
        {
            pm_db_parser_ca_close_fct(pt_esdb_node);   
        }
    }
    else
    {
        i4_return = PMR_OK;      
    }
    
EXIT:
    PM_ERR_CODE(i4_return);
    return i4_return;  
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description:            
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 *          
 ----------------------------------------------------------------------------*/ 
BOOL pmi_is_ca_sys_id_match_pmt(VOID)
{
    return ca_is_ci_system_id_match_pmt();
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description:            
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 *          
 ----------------------------------------------------------------------------*/ 
CAM_STATUS_T pmi_get_cam_wait_status(VOID)
{
    return ca_get_cam_wait_status();
}

#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: pmi_ca_sys_dump_4cli
 *
 * Description: This API dump CA system registered into PM.
 *
 * Inputs:  -    
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *----------------------------------------------------------------------------*/
INT32 pmi_ca_sys_dump_4cli( VOID )
{
    UINT32      i;
    
    for(i=0; i<ui2_cadb_num_total; i++)
    {
        if (TRUE == pt_pm_cadb_arr[i].b_is_used)
        {
            x_dbg_stmt( 
                "CA CORE index:%d, e_source_type(%d), ui2_source_id(%d), ui2_ca_sys_id(%d), b_alloc_desc(%d)\n",
                i,
                pt_pm_cadb_arr[i].e_source_type,
                pt_pm_cadb_arr[i].ui2_source_id,
                pt_pm_cadb_arr[i].ui2_ca_sys_id,
                pt_pm_cadb_arr[i].b_alloc_desc );
        }
    }

    return PMR_OK;
}
#endif

#endif /* PM_ENABLE_CA */

