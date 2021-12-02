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
 * $RCSfile: prc_db.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/4 $
 * $SWAuthor: Felisa Hsiao $
 * $MD5HEX: e55c71c3de88fc204cc32c14a159e5e3 $
 *
 * Description: 
 *         This file contains PM_ESDB and PM_SVDB which are the control data for 
 *         whole PM module.
 * History:
 *                
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "prc_mngr/prc_mngr.h"
#include "prc_mngr/prc_common.h"
#include "prc_mngr/prc_db.h"
#include "prc_mngr/prc_lock.h"
#include "prc_mngr/prc_util.h"
#include "prc_mngr/prc_debug.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

 
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
 /* PM_ESDB */
static BOOL b_db_init = FALSE;                  /* Tracks if PM_ESDB has been initialized. */  
static PM_COMPONENT_INFO_T* pt_pm_esdb_arr = NULL;     /* ESDB Data store */
static UINT16 ui2_esdb_num_total = 0;               /* Total number of PM_ESDB node. */
static UINT16 ui2_esdb_num_free = 0;                /* Number of free PM_ESDB node. */
static UINT16 ui2_esdb_free_nodes = 0;              /* The head of free nodes */
static PM_SERVICE_INFO_T* pt_pm_svdb_arr = NULL;       /* SVDB Data store */
static UINT16 ui2_svdb_num_total = 0;               /* Total number of PM_SVDB node. */
static UINT16 ui2_svdb_num_free = 0;                /* Number of free PM_SVDB node. */
static UINT16 ui2_svdb_free_nodes = 0;               /* The head of free nodes */

#if PM_DEBUG_DB_AUTOCOMPARE
static BOOL b_dbg_db_init = FALSE;              /* Tracks if PM_ESDB has been initialized. */  
static PM_COMPONENT_INFO_T* pt_dbg_pm_esdb_arr = NULL; /* ESDB Data store */
static UINT16 ui2_dbg_esdb_num_total = 0;           /* Total number of PM_ESDB node. */
static UINT16 ui2_dbg_esdb_num_free = 0;            /* Number of free PM_ESDB node. */
static UINT16 ui2_dbg_esdb_free_nodes = 0;          /* The head of free nodes */
static PM_SERVICE_INFO_T* pt_dbg_pm_svdb_arr = NULL;   /* SVDB Data store */
static UINT16 ui2_dbg_svdb_num_total = 0;           /* Total number of PM_SVDB node. */
static UINT16 ui2_dbg_svdb_num_free = 0;            /* Number of free PM_SVDB node. */
static UINT16 ui2_dbg_svdb_free_nodes = 0;          /* The head of free nodes */
#endif /* PM_DEBUG_DB_AUTOCOMPARE */

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static PM_SERVICE_INFO_T* get_sv_node_from_idx (UINT16  ui2_h_idx);
static UINT16 get_index_from_sv_node (PM_SERVICE_INFO_T* pt_node);
static VOID pmi_db_init_svnode(PM_SERVICE_INFO_T* pt_svdb_node);
static VOID pmi_db_init_esnode(PM_COMPONENT_INFO_T* pt_esdb_node, PM_PROCESSING_TYPE_T  e_prc_type );

/*-----------------------------------------------------------------------------
 * Name: get_es_node_from_idx
 *
 * Description: This API returns the reference to the ESDB node by index of 
 *              ESDB array.
 *
 * Inputs:  ui2_h_idx        The index for ESDB array.
 *         
 * Outputs: -
 *
 * Returns: Reference to the ESDB node.
 ----------------------------------------------------------------------------*/
PM_COMPONENT_INFO_T* get_es_node_from_idx (UINT16  ui2_h_idx)
{
    PM_COMPONENT_INFO_T* pt_node;

    pt_node = NULL;
    
    if ((ui2_h_idx > PM_NULL_IDX)&&(ui2_h_idx < ui2_esdb_num_total))
    {
        pt_node  = &(pt_pm_esdb_arr[ui2_h_idx]);
    }    
    else
    {
        PM_DBG_ERROR( "ESDB node index is invaild\n" );
    }
    return (pt_node);
}

/*-----------------------------------------------------------------------------
 * Name: get_index_from_es_node
 *
 * Description: This API returns the index of ESDB array by reference of ESDB 
 *              node. 
 *
 * Inputs:  pt_node          Reference to ESDB node.
 *                   
 * Outputs: -
 *
 * Returns: The index of ESDB array.
 ----------------------------------------------------------------------------*/
UINT16 get_index_from_es_node (PM_COMPONENT_INFO_T* pt_node)
{
    UINT16    ui2_idx;
    
    ui2_idx = PM_NULL_IDX;
            
    if ((((UINT16)(pt_node - pt_pm_esdb_arr)) < ui2_esdb_num_total) && 
        (((UINT16)(pt_node - pt_pm_esdb_arr)) > PM_NULL_IDX))
    {
        ui2_idx = (UINT16)(pt_node - pt_pm_esdb_arr);
    }     
    else
    {
        PM_DBG_ERROR( "ESDB node point is invaild\n" );
    }
    return ui2_idx;
}

/*-----------------------------------------------------------------------------
 * Name: get_sv_node_from_idx
 *
 * Description: This API returns the index of ESDB array by reference of ESDB 
 *              node. 
 *
 * Inputs:  ui2_h_idx        The index for SVDB array.
 *                   
 * Outputs: -
 *
 * Returns: Reference to the SVDB node.
 ----------------------------------------------------------------------------*/
static PM_SERVICE_INFO_T* get_sv_node_from_idx (UINT16  ui2_h_idx)
{
    PM_SERVICE_INFO_T* pt_node;

    pt_node = NULL;
    
    if ((ui2_h_idx > PM_NULL_IDX)&&(ui2_h_idx < ui2_svdb_num_total))
    {
        pt_node  = &(pt_pm_svdb_arr[ui2_h_idx]);
    }
    else
    {
        PM_DBG_ERROR( "SVDB node index is invaild\n" );
    }
    
    return (pt_node);
}

/*-----------------------------------------------------------------------------
 * Name: get_index_from_sv_node
 *
 * Description: This API returns the index of SVDB array by reference of SVDB 
 *              node. 
 *
 * Inputs:  pt_node          Reference to SVDB node.
 *                   
 * Outputs: -
 *
 * Returns: The index of SVDB array.
 ----------------------------------------------------------------------------*/
static UINT16 get_index_from_sv_node (PM_SERVICE_INFO_T* pt_node)
{
    UINT16    ui2_idx;
    
    ui2_idx = PM_NULL_IDX;
            
    if ((((UINT16)(pt_node - pt_pm_svdb_arr)) < ui2_svdb_num_total) && 
        (((UINT16)(pt_node - pt_pm_svdb_arr)) > PM_NULL_IDX))
    {
        ui2_idx = (UINT16)(pt_node - pt_pm_svdb_arr);
    }
    else
    {
        PM_DBG_ERROR( "SVDB node point is invaild\n" );
    }
             
    return ui2_idx;
}

/*-----------------------------------------------------------------------------
 * Name: pmi_db_init
 *
 * Description: This API allocate DB and initialize the whole DB related data. 
 *
 * Inputs:  ui2_num_services Number of SVDB node.
 *          ui2_num_comps    Number of ESDB node.
 *                   
 * Outputs: -
 *
 * Returns: PMR_OK             Success.
 *          PMR_ALREADY_INIT   Already initialize.
 *          PMR_NOT_ENOUGH_MEM Failure due to memory allocation failed
 ----------------------------------------------------------------------------*/    
INT32 pmi_db_init(UINT16  ui2_num_services, UINT16  ui2_num_comps)
{
    INT32     i4_return;
    UINT16    ui2_i;
    
    i4_return = PMR_ALREADY_INIT;
    
    if (! (b_db_init))
    {
        i4_return = PMR_NOT_ENOUGH_MEM;

        ui2_esdb_num_total = ui2_num_comps + ((UINT16)1);
        ui2_svdb_num_total = ui2_num_services + ((UINT16)1);
        
        /* Allocate memory first */        
    #if (!PM_DEBUG_DB_AUTOCOMPARE)               
        if (((pt_pm_svdb_arr = (PM_SERVICE_INFO_T*) x_mem_alloc (sizeof (PM_SERVICE_INFO_T) * ui2_svdb_num_total)) != NULL) && 
            ((pt_pm_esdb_arr = (PM_COMPONENT_INFO_T*) x_mem_alloc (sizeof (PM_COMPONENT_INFO_T) * ui2_esdb_num_total)) != NULL))
    #else
        if (((pt_pm_svdb_arr = (PM_SERVICE_INFO_T*) x_mem_alloc (sizeof (PM_SERVICE_INFO_T) * ui2_svdb_num_total)) != NULL) && 
            ((pt_pm_esdb_arr = (PM_COMPONENT_INFO_T*) x_mem_alloc (sizeof (PM_COMPONENT_INFO_T) * ui2_esdb_num_total)) != NULL) &&
            ((pt_dbg_pm_svdb_arr = (PM_SERVICE_INFO_T*) x_mem_alloc (sizeof (PM_SERVICE_INFO_T) * ui2_svdb_num_total)) != NULL) && 
            ((pt_dbg_pm_esdb_arr = (PM_COMPONENT_INFO_T*) x_mem_alloc (sizeof (PM_COMPONENT_INFO_T) * ui2_esdb_num_total)) != NULL))    
    #endif /* (!PM_DEBUG_DB_AUTOCOMPARE) */          
        {
            i4_return = PMR_OK;
            b_db_init = TRUE;       
            
             /* Initialize ES DB */
            ui2_esdb_num_free = 0;
            ui2_esdb_free_nodes = PM_NULL_IDX;
        
            for (ui2_i=1; ui2_i<ui2_esdb_num_total; ui2_i++)
            {
                handle_link_init(&(pt_pm_esdb_arr[ui2_i].t_handle_link));   
                
                pt_pm_esdb_arr[ui2_i].ui2_service = PM_NULL_IDX;
                pt_pm_esdb_arr[ui2_i].ui2_next = ui2_esdb_free_nodes;   
                ui2_esdb_free_nodes = ui2_i;
                ui2_esdb_num_free++;
            }
            
            /* Initialize Service DB */
            ui2_svdb_num_free = 0;
            ui2_svdb_free_nodes = PM_NULL_IDX;            
            
            for(ui2_i=1; ui2_i<ui2_svdb_num_total; ui2_i++)
            {
                handle_link_init(&(pt_pm_svdb_arr[ui2_i].t_handle_link));                      
                
                pt_pm_svdb_arr[ui2_i].h_connection = NULL_HANDLE;
                pt_pm_svdb_arr[ui2_i].e_process_type = PM_PRESENT;
                pt_pm_svdb_arr[ui2_i].i4_speed = 0;
                pt_pm_svdb_arr[ui2_i].e_cond = PM_COND_CLOSE;
                pt_pm_svdb_arr[ui2_i].ui2_node_head = PM_NULL_IDX;
                pt_pm_svdb_arr[ui2_i].ui2_next = ui2_svdb_free_nodes;   
                pt_pm_svdb_arr[ui2_i].h_ca_service = NULL_HANDLE;
            #if (PM_ENABLE_CA)
                pt_pm_svdb_arr[ui2_i].pv_ca_ctrl = NULL;
            #endif
                ui2_svdb_free_nodes = ui2_i;
                ui2_svdb_num_free++;
            }      
        } 
        else
        {
            /* Free memory */
        #if (!PM_DEBUG_DB_AUTOCOMPARE)               
            if (pt_pm_svdb_arr != NULL)
                x_mem_free(pt_pm_svdb_arr); 
            if (pt_pm_esdb_arr != NULL)
                x_mem_free(pt_pm_esdb_arr);
        #else
            if (pt_pm_svdb_arr != NULL)
                x_mem_free(pt_pm_svdb_arr); 
            if (pt_pm_esdb_arr != NULL)
                x_mem_free(pt_pm_esdb_arr);
            if (pt_dbg_pm_svdb_arr != NULL)
                x_mem_free(pt_dbg_pm_svdb_arr); 
            if (pt_dbg_pm_esdb_arr != NULL)
                x_mem_free(pt_dbg_pm_esdb_arr);
        #endif /* (!PM_DEBUG_DB_AUTOCOMPARE)  */  
            
        }
    }
           
    return i4_return;  
}

/*-----------------------------------------------------------------------------
 * Name: pmi_db_init_esnode
 *
 * Description: This API initialize the ESDB node. 
 *
 * Inputs:  pt_esdb_node     Reference to ESDB node.
 *                   
 * Outputs: pt_esdb_node     Reference to ESDB node have been initialized.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID pmi_db_init_esnode(PM_COMPONENT_INFO_T* pt_esdb_node, PM_PROCESSING_TYPE_T  e_prc_type)
{
    x_memset( pt_esdb_node, 0, sizeof(PM_COMPONENT_INFO_T) );
    handle_link_init(&(pt_esdb_node->t_handle_link));  
    
    switch (e_prc_type)
    {
        case PM_PRESENT:
        case PM_DETECTION:
            ESNODE_REQUEST_TYPE(pt_esdb_node) = PM_PRESENT_ES_UNKNOWN;   
            break;

    #if (PM_ENABLE_MULTIMEDIA)
        #if (PM_ENABLE_RECORD)
        case PM_RECORDING:
            ESNODE_REC_REQUEST_TYPE(pt_esdb_node) = PM_RECORDING_ES_UNKNOWN;
            ESNODE_PID(pt_esdb_node)              = 0;
            break;
        #endif
            
        case PM_MULTIMEDIA:
            ESNODE_MMP_REQUEST_TYPE(pt_esdb_node) = PM_MM_ES_UNKNOWN;   
            x_memset(&(ESNODE_MMP_DESCR_SETTING(pt_esdb_node)), 0, sizeof(STREAM_ID_T));             
            
            break;
    #endif
        default:
            break;
    }    
    
    ESNODE_PROC_TYPE(pt_esdb_node) = e_prc_type;
    ESNODE_CA_NFY(pt_esdb_node) = NULL;
    ESNODE_CA_TAG(pt_esdb_node) = NULL;     
    ESNODE_PID(pt_esdb_node) = 0;             
    x_memset(&(ESNODE_CA_INFO(pt_esdb_node)), 0, sizeof(PM_CA_INFO_T));

#if PM_ENABLE_SCRAMBLE_DETECTION  
    ESNODE_DMX_SCRAMBLE_TAG(pt_esdb_node) = NULL;
    ESNODE_DMX_SCRAMBLE_NFY(pt_esdb_node) = NULL;
#endif

    ESNODE_CA_COND(pt_esdb_node) = ESNODE_CA_LAST_COND(pt_esdb_node) = PM_CA_COND_UNKNOWN;
    ESNODE_CA_LAST_STAMP(pt_esdb_node) = 0;
    ESNODE_CA_TIME_MARK(pt_esdb_node) = 0;
            
    x_memset(&(ESNODE_CP_INFO(pt_esdb_node)), 0, sizeof(CP_INFO_LIST_T));
    
    x_memset(&(ESNODE_DESCR(pt_esdb_node)), 0, sizeof(STREAM_COMP_ID_T)); 
#if (PM_ENABLE_MULTIMEDIA)
    x_memset(&(ESNODE_SINK_INFO(pt_esdb_node)), 0, sizeof(DEMUX_DEST_INFO_T));
    x_memset(&(ESNODE_REC_SINK_INFO(pt_esdb_node)), 0, sizeof(RECDEMUX_DEST_INFO_T));
#else
    x_memset(&(ESNODE_SINK_INFO(pt_esdb_node)), 0, sizeof(DEMUX_DEST_INFO_T));  
#endif
    ESNODE_FILTER_NFY(pt_esdb_node) = NULL;
    ESNODE_FILTER_TAG(pt_esdb_node) = NULL;   
    ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_IDLE;
    ESNODE_CURR_STATE(pt_esdb_node) = PM_ES_STATE_IDLE;
    ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_CLOSED;
    ESNODE_DETAIL_CODE(pt_esdb_node) = 0;
    ESNODE_SCDB_HANDLE(pt_esdb_node) = NULL_HANDLE;  
    ESNODE_FILTER_HANDLE(pt_esdb_node) = NULL_HANDLE;
    ESNODE_FILTER_COND(pt_esdb_node) = RM_COND_IGNORE;    
    
#if (PM_ENABLE_FILTER_CTRL)
    ESNODE_FILTER_OBJ_CTRL(pt_esdb_node) = NULL;
    ESNODE_FILTER_OBJ_NEXT(pt_esdb_node) = NULL;
#endif 
      
    ESNODE_CRYPTO_HANDLE(pt_esdb_node) = NULL_HANDLE;  
    ESNODE_CRYPTO_COND(pt_esdb_node) = RM_COND_IGNORE;    
    
    pt_esdb_node->ui2_service = PM_NULL_IDX;
    
}   

/*-----------------------------------------------------------------------------
 * Name: pmi_esdb_alloc
 *
 * Description: This API allocate a free ESDB node. 
 *
 * Inputs:  ppt_esdb_node    Contains reference to pointer of ESDB node
 *     
 * Outputs: ppt_esdb_node    Contains reference to pointer of ESDB node for new 
 *                           allocated one. 
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     DB is not initialized yet.
 *          PMR_INV_ARG      Invalid arguments.
 *          PMR_FAILED       No free ESDB node.
 ----------------------------------------------------------------------------*/
INT32 pmi_esdb_alloc(PM_COMPONENT_INFO_T** ppt_esdb_node, PM_PROCESSING_TYPE_T  e_prc_type)
{
    INT32        i4_return;

    if (ppt_esdb_node != NULL) 
    { 
        *ppt_esdb_node = NULL;
    }
    i4_return = PMR_NOT_INIT;
    
    if (b_db_init)
    {
        i4_return = PMR_INV_ARG;

        /* Validate arguments. */
        if (ppt_esdb_node != NULL)                  
        {
            i4_return = PMR_FAILED;

            /* Get next free handle control structure. */
            
            if ((*ppt_esdb_node = get_es_node_from_idx (ui2_esdb_free_nodes)) != NULL)
            {
                ui2_esdb_free_nodes = (*ppt_esdb_node)->ui2_next;
                ui2_esdb_num_free--;
                
                /* Reinitialize node */
                pmi_db_init_esnode(*ppt_esdb_node, e_prc_type);  
                
                (*ppt_esdb_node)->ui2_next = PM_NULL_IDX;

                i4_return = PMR_OK;
            }            
        }
        else
        {
            PM_DBG_ERROR( "ppt_esdb_node is NULL\n" );
        }
    }
    
    if (i4_return != PMR_OK)
    {
         PM_DBG_ERROR("Failed to allocate ES\n");        
    }    
    
    return (i4_return);
}    

/*-----------------------------------------------------------------------------
 * Name: pmi_esdb_free
 *
 * Description: This API free the specified ESDB node. 
 *
 * Inputs:  ppt_esdb_node    Contains reference to pointer of ESDB node to be 
 *                           free
 *     
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     DB is not initialized yet.
 *          PMR_INV_ARG      Invalid arguments.
 ----------------------------------------------------------------------------*/
INT32 pmi_esdb_free(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    INT32        i4_return;
    UINT16       ui2_index;    

    i4_return = PMR_NOT_INIT;

    if (b_db_init)
    {
        i4_return = PMR_INV_ARG;

        /* Validate handle. */
        if ((ui2_index = get_index_from_es_node(pt_esdb_node)) != PM_NULL_IDX)
        {
            i4_return = PMR_OK;            
            
            pt_esdb_node->ui2_next = ui2_esdb_free_nodes;
            ui2_esdb_free_nodes = ui2_index;
            ui2_esdb_num_free++;           
        }
    }
    
    if (i4_return != PMR_OK)
    {
         PM_DBG_ERROR("Failed to free ES\n");        
    }    
    
    return (i4_return);    
} 

/*-----------------------------------------------------------------------------
 * Name: pmi_esdb_is_valid
 *
 * Description: This API free the specified ESDB node. 
 *
 * Inputs:  pt_esdb_node     Contains reference to pointer of ESDB node to be 
 *                           checked
 *     
 * Outputs: -
 *
 * Returns: TRUE             Valid
 *          FALSE            Invalid
 ----------------------------------------------------------------------------*/
BOOL  pmi_esdb_is_valid(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    if ((b_db_init)&&(pt_esdb_node!= NULL))
    {
        /* Validate ESDB */
        if (get_index_from_es_node(pt_esdb_node) != PM_NULL_IDX)
            return TRUE;          
        else
        {
            PM_DBG_ERROR("ES is not valid\n");
            return FALSE;
        }
    }
    else
    {
        PM_DBG_ERROR("ES is not valid\n");
        return FALSE;    
    }
} 
#if PM_DEBUG_ENABLE
/*-----------------------------------------------------------------------------
 * Name: pmi_esdb_dump
 *
 * Description: This API dump whole ESDB related data. 
 *
 * Inputs:  -    
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *----------------------------------------------------------------------------*/
INT32 pmi_esdb_dump(VOID)
{
    UINT32 i;
    
    PM_DEBUG(("pm_esdb: dump begin===================================\n")); 
    
    PM_DEBUG(("pm_esdb: b_db_init= %d\n", b_db_init));
    PM_DEBUG(("pm_esdb: ui2_esdb_num_total = %d\n", ui2_esdb_num_total));
    PM_DEBUG(("pm_esdb: ui2_esdb_num_free = %d\n", ui2_esdb_num_free));  
    PM_DEBUG(("pm_esdb: ui2_esdb_free_nodes = %d\n", ui2_esdb_free_nodes));     

#if PM_DEBUG_DB_DETAIL     
    for (i=0; i<ui2_esdb_num_total; i++)
    {   
        PM_DEBUG(("pm_esdb[%lu]: t_handle_link.ui4_data[0] = %lu\n", i, pt_pm_esdb_arr[i].t_handle_link.ui4_data[0]));
        PM_DEBUG(("pm_esdb[%lu]: t_handle_link.ui4_data[1] = %lu\n", i, pt_pm_esdb_arr[i].t_handle_link.ui4_data[1]));    
          
        PM_DEBUG(("pm_esdb[%lu]: t_stream_comp_id.e_type = %d\n", i, pt_pm_esdb_arr[i].t_stream_comp_id.e_type));
        PM_DEBUG(("pm_esdb[%lu]: t_stream_comp_id.pv_stream_tag = %lu\n", i, (UINT32)(pt_pm_esdb_arr[i].t_stream_comp_id.pv_stream_tag)));
        PM_DEBUG(("pm_esdb[%lu]: t_sink_comp_info.e_type = %d\n", i, pt_pm_esdb_arr[i].t_sink_comp_info.e_type));
        PM_DEBUG(("pm_esdb[%lu]: t_sink_comp_info.ui2_id = %lu\n", i, (UINT32)(pt_pm_esdb_arr[i].t_sink_comp_info.ui2_id)));
        PM_DEBUG(("pm_esdb[%lu]: pf_filter_nfy = %lu\n", i, (UINT32)(pt_pm_esdb_arr[i].pf_filter_nfy)));       
        PM_DEBUG(("pm_esdb[%lu]: pv_filter_tag = %lu\n", i, (UINT32)(pt_pm_esdb_arr[i].pv_filter_tag)));
        PM_DEBUG(("pm_esdb[%lu]: pf_ca_nfy = %lu\n", i, (UINT32)(pt_pm_esdb_arr[i].pf_ca_nfy)));       
        PM_DEBUG(("pm_esdb[%lu]: pv_ca_tag = %lu\n", i, (UINT32)(pt_pm_esdb_arr[i].pv_ca_tag)));
        
        PM_DEBUG(("pm_esdb[%lu]: e_request_state = %d\n", i, pt_pm_esdb_arr[i].e_request_state));
        PM_DEBUG(("pm_esdb[%lu]: e_current_state = %d\n", i, pt_pm_esdb_arr[i].e_current_state));
        PM_DEBUG(("pm_esdb[%lu]: e_filter_status = %d\n", i, pt_pm_esdb_arr[i].e_filter_status));
        PM_DEBUG(("pm_esdb[%lu]: ui4_detail_code = %lu\n", i, pt_pm_esdb_arr[i].ui4_detail_code));
        
        PM_DEBUG(("pm_esdb[%lu]: t_scdb_info.h_scdb = %lu\n", i, pt_pm_esdb_arr[i].t_scdb_info.h_scdb));
        PM_DEBUG(("pm_esdb[%lu]: t_scdb_info.t_component_pid = %d\n", i, pt_pm_esdb_arr[i].t_scdb_info.t_component_pid));

        PM_DEBUG(("pm_esdb[%lu]: t_ca_info.h_ca = %lu\n", i, pt_pm_esdb_arr[i].t_ca_info.h_ca));
        PM_DEBUG(("pm_esdb[%lu]: t_ca_info.ui2_ca_sys_id = %d\n", i, pt_pm_esdb_arr[i].t_ca_info.ui2_ca_sys_id));
        PM_DEBUG(("pm_esdb[%lu]: t_ca_info.e_status = %d\n", i, pt_pm_esdb_arr[i].t_ca_info.e_status));
     
        PM_DEBUG(("pm_esdb[%lu]: t_cp_info.h_cp = %lu\n", i, pt_pm_esdb_arr[i].t_cp_info.h_cp));
        PM_DEBUG(("pm_esdb[%lu]: t_cp_info.ui4_cp_sys_bitmask = %lu\n", i, pt_pm_esdb_arr[i].t_cp_info.ui4_cp_sys_bitmask));
        PM_DEBUG(("pm_esdb[%lu]: t_cp_info.ui4_cp_setting = %lu\n", i, pt_pm_esdb_arr[i].t_cp_info.ui4_cp_setting));   
        
        PM_DEBUG(("pm_esdb[%lu]: t_filter_info.h_filter = %lu\n", i, pt_pm_esdb_arr[i].t_filter_info.h_filter));
        PM_DEBUG(("pm_esdb[%lu]: t_filter_info.t_filter_cond = %lu\n", i, (UINT32)(pt_pm_esdb_arr[i].t_filter_info.t_filter_cond)));        
        
        PM_DEBUG(("pm_esdb[%lu]: t_crypto_info.h_des = %lu\n", i, pt_pm_esdb_arr[i].t_crypto_info.h_des));
        PM_DEBUG(("pm_esdb[%lu]: t_crypto_info.t_des_cond = %lu\n", i, (UINT32)(pt_pm_esdb_arr[i].t_crypto_info.t_des_cond)));
        
        PM_DEBUG(("pm_esdb[%lu]: ui2_service = %d\n", i, pt_pm_esdb_arr[i].ui2_service));     
        PM_DEBUG(("pm_esdb[%lu]: ui2_next = %d\n\n", i, pt_pm_esdb_arr[i].ui2_next));                   
    }
#endif  /* PM_DEBUG_DB_DETAIL */    
    
    /* Show Free service node */
    i= ui2_esdb_free_nodes;
    PM_DEBUG(("pm_esdb free:"));
            
    while (i != PM_NULL_IDX)
    {
        PM_DEBUG(("[%lu]", i));
        i= pt_pm_esdb_arr[i].ui2_next;
    }
    PM_DEBUG(("\n")); 
        
    PM_DEBUG(("pm_esdb: dump end===================================\n"));   
    
    return PMR_OK;  
}
#endif /* PM_DEBUG_ENABLE */

#if PM_DEBUG_DB_AUTOCOMPARE
/*-----------------------------------------------------------------------------
 * Name: pm_esdb_debug_precondition
 *
 * Description: This API copys the whole ESDB related data to debug ESDB for 
 *              comparion. 
 *
 * Inputs:  -    
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *----------------------------------------------------------------------------*/
INT32 pm_esdb_debug_precondition(VOID)
{
     b_dbg_db_init= b_db_init;
     ui2_dbg_esdb_num_total = ui2_esdb_num_total;
     ui2_dbg_esdb_num_free = ui2_esdb_num_free;
     ui2_dbg_esdb_free_nodes = ui2_esdb_free_nodes;
     x_memcpy(pt_dbg_pm_esdb_arr, pt_pm_esdb_arr, sizeof(PM_COMPONENT_INFO_T)*ui2_dbg_esdb_num_total);
     
     return PMR_OK;   
    
}

/*-----------------------------------------------------------------------------
 * Name: pm_esdb_debug_postcondition_dump
 *
 * Description: This API compares the current ESDB with the previous backup 
 *              data and dump the difference.
 *
 * Inputs:  -    
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *----------------------------------------------------------------------------*/
INT32 pm_esdb_debug_postcondition_dump(VOID)
{
    UINT32 i;
    
    PM_DEBUG(("pm_esdb: postcondition begin\n")); 
    
    if (b_db_init != b_dbg_db_init)
        PM_DEBUG(("pm_esdb: b_db_init= %d -> %d\n", b_dbg_db_init, b_db_init));
    if (ui2_dbg_esdb_num_total != ui2_esdb_num_total)
        PM_DEBUG(("pm_esdb: ui2_esdb_num_total = %d -> %d\n", ui2_dbg_esdb_num_total, ui2_esdb_num_total));
    if (ui2_esdb_num_free != ui2_dbg_esdb_num_free)
        PM_DEBUG(("pm_esdb: ui2_esdb_num_free = %d -> %d\n", ui2_dbg_esdb_num_free, ui2_esdb_num_free));  
    if (ui2_esdb_free_nodes != ui2_dbg_esdb_free_nodes)
        PM_DEBUG(("pm_esdb: ui2_esdb_free_nodes = %d -> %d\n", ui2_dbg_esdb_free_nodes, ui2_esdb_free_nodes));     

#if PM_DEBUG_DB_DETAIL     
    for (i=0; i<ui2_esdb_num_total; i++)
    {   
        if (pt_pm_esdb_arr[i].t_handle_link.ui4_data[0] != pt_dbg_pm_esdb_arr[i].t_handle_link.ui4_data[0])
            PM_DEBUG(("pm_esdb[%lu]: t_handle_link.ui4_data[0] = %lu -> %lu\n", i, pt_dbg_pm_esdb_arr[i].t_handle_link.ui4_data[0], pt_pm_esdb_arr[i].t_handle_link.ui4_data[0]));
        if (pt_pm_esdb_arr[i].t_handle_link.ui4_data[1] != pt_dbg_pm_esdb_arr[i].t_handle_link.ui4_data[1])
            PM_DEBUG(("pm_esdb[%lu]: t_handle_link.ui4_data[1] = %lu -> %lu\n", i, pt_dbg_pm_esdb_arr[i].t_handle_link.ui4_data[1], pt_pm_esdb_arr[i].t_handle_link.ui4_data[1]));                             
        if (pt_pm_esdb_arr[i].t_stream_comp_id.e_type != pt_dbg_pm_esdb_arr[i].t_stream_comp_id.e_type)
            PM_DEBUG(("pm_esdb[%lu]: t_stream_comp_id.e_type = %d -> %d\n", i, pt_dbg_pm_esdb_arr[i].t_stream_comp_id.e_type, pt_pm_esdb_arr[i].t_stream_comp_id.e_type));
        if (pt_pm_esdb_arr[i].t_stream_comp_id.pv_stream_tag != pt_dbg_pm_esdb_arr[i].t_stream_comp_id.pv_stream_tag)
            PM_DEBUG(("pm_esdb[%lu]: t_stream_comp_id.pv_stream_tag = %lu -> %lu\n", i, (UINT32)(pt_dbg_pm_esdb_arr[i].t_stream_comp_id.pv_stream_tag), (UINT32)( pt_pm_esdb_arr[i].t_stream_comp_id.pv_stream_tag)));
        if (pt_pm_esdb_arr[i].t_sink_comp_info.e_type != pt_dbg_pm_esdb_arr[i].t_sink_comp_info.e_type)
            PM_DEBUG(("pm_esdb[%lu]: t_sink_comp_info.e_type = %d -> %d\n", i, pt_dbg_pm_esdb_arr[i].t_sink_comp_info.e_type, pt_pm_esdb_arr[i].t_sink_comp_info.e_type));
        if (pt_pm_esdb_arr[i].t_sink_comp_info.ui2_id != pt_dbg_pm_esdb_arr[i].t_sink_comp_info.ui2_id)
            PM_DEBUG(("pm_esdb[%lu]: t_sink_comp_info.ui2_id = %d -> %d\n", i, (pt_dbg_pm_esdb_arr[i].t_sink_comp_info.ui2_id), ( pt_pm_esdb_arr[i].t_sink_comp_info.ui2_id)));
        if (pt_pm_esdb_arr[i].pf_filter_nfy != pt_dbg_pm_esdb_arr[i].pf_filter_nfy)
            PM_DEBUG(("pm_esdb[%lu]: pf_filter_nfy = %lu -> %lu\n", i, (UINT32)(pt_dbg_pm_esdb_arr[i].pf_filter_nfy), (UINT32)(pt_pm_esdb_arr[i].pf_filter_nfy)));      
        if (pt_pm_esdb_arr[i].pv_filter_tag != pt_dbg_pm_esdb_arr[i].pv_filter_tag)
            PM_DEBUG(("pm_esdb[%lu]: pv_filter_tag = %lu -> %lu\n", i, (UINT32)(pt_dbg_pm_esdb_arr[i].pv_filter_tag), (UINT32)(pt_pm_esdb_arr[i].pv_filter_tag)));
        if (pt_pm_esdb_arr[i].pf_ca_nfy != pt_dbg_pm_esdb_arr[i].pf_ca_nfy)
            PM_DEBUG(("pm_esdb[%lu]: pf_ca_nfy = %lu -> %lu\n", i, (UINT32)(pt_dbg_pm_esdb_arr[i].pf_ca_nfy), (UINT32)(pt_pm_esdb_arr[i].pf_ca_nfy)));      
        if (pt_pm_esdb_arr[i].pv_ca_tag != pt_dbg_pm_esdb_arr[i].pv_ca_tag)
            PM_DEBUG(("pm_esdb[%lu]: pv_ca_tag = %lu -> %lu\n", i, (UINT32)(pt_dbg_pm_esdb_arr[i].pv_ca_tag), (UINT32)(pt_pm_esdb_arr[i].pv_ca_tag)));

        if (pt_pm_esdb_arr[i].e_request_state != pt_dbg_pm_esdb_arr[i].e_request_state)
            PM_DEBUG(("pm_esdb[%lu]: e_request_state = %d -> %d\n", i, pt_dbg_pm_esdb_arr[i].e_request_state, pt_pm_esdb_arr[i].e_request_state));
        if (pt_pm_esdb_arr[i].e_current_state != pt_dbg_pm_esdb_arr[i].e_current_state)
            PM_DEBUG(("pm_esdb[%lu]: e_current_state = %d -> %d\n", i, pt_dbg_pm_esdb_arr[i].e_current_state, pt_pm_esdb_arr[i].e_current_state));            
        if ( pt_pm_esdb_arr[i].e_filter_status != pt_dbg_pm_esdb_arr[i].e_filter_status)
            PM_DEBUG(("pm_esdb[%lu]: e_filter_status = %d -> %d\n", i, pt_dbg_pm_esdb_arr[i].e_filter_status, pt_pm_esdb_arr[i].e_filter_status));
        if ( pt_pm_esdb_arr[i].ui4_detail_code != pt_dbg_pm_esdb_arr[i].ui4_detail_code)
            PM_DEBUG(("pm_esdb[%lu]: ui4_detail_code = %lu -> %lu\n", i, pt_dbg_pm_esdb_arr[i].ui4_detail_code, pt_pm_esdb_arr[i].ui4_detail_code));
        
        if ( pt_pm_esdb_arr[i].t_scdb_info.h_scdb != pt_dbg_pm_esdb_arr[i].t_scdb_info.h_scdb)
            PM_DEBUG(("pm_esdb[%lu]: t_scdb_info.h_scdb = %lu -> %lu\n", i, pt_dbg_pm_esdb_arr[i].t_scdb_info.h_scdb, pt_pm_esdb_arr[i].t_scdb_info.h_scdb));
        if ( pt_pm_esdb_arr[i].t_scdb_info.t_component_pid != pt_dbg_pm_esdb_arr[i].t_scdb_info.t_component_pid)
            PM_DEBUG(("pm_esdb[%lu]: t_scdb_info.t_component_pid = %d -> %d\n", i, pt_dbg_pm_esdb_arr[i].t_scdb_info.t_component_pid, pt_pm_esdb_arr[i].t_scdb_info.t_component_pid));

        if ( pt_pm_esdb_arr[i].t_ca_info.h_ca != pt_dbg_pm_esdb_arr[i].t_ca_info.h_ca)
            PM_DEBUG(("pm_esdb[%lu]: t_ca_info.h_ca = %lu -> %lu\n", i, pt_dbg_pm_esdb_arr[i].t_ca_info.h_ca, pt_pm_esdb_arr[i].t_ca_info.h_ca));
        if ( pt_pm_esdb_arr[i].t_ca_info.ui2_ca_sys_id != pt_dbg_pm_esdb_arr[i].t_ca_info.ui2_ca_sys_id)
            PM_DEBUG(("pm_esdb[%lu]: t_ca_info.ui2_ca_sys_id = %d -> %d\n", i, pt_dbg_pm_esdb_arr[i].t_ca_info.ui2_ca_sys_id, pt_pm_esdb_arr[i].t_ca_info.ui2_ca_sys_id));
        if ( pt_pm_esdb_arr[i].t_ca_info.e_status != pt_dbg_pm_esdb_arr[i].t_ca_info.e_status)
            PM_DEBUG(("pm_esdb[%lu]: t_ca_info.e_status = %d -> %d\n", i, pt_dbg_pm_esdb_arr[i].t_ca_info.e_status, pt_pm_esdb_arr[i].t_ca_info.e_status));
     
        if ( pt_pm_esdb_arr[i].t_cp_info.h_cp != pt_dbg_pm_esdb_arr[i].t_cp_info.h_cp)
            PM_DEBUG(("pm_esdb[%lu]: t_cp_info.h_cp = %lu -> %lu\n", i, pt_dbg_pm_esdb_arr[i].t_cp_info.h_cp, pt_pm_esdb_arr[i].t_cp_info.h_cp));
        if ( pt_pm_esdb_arr[i].t_cp_info.ui4_cp_sys_bitmask != pt_dbg_pm_esdb_arr[i].t_cp_info.ui4_cp_sys_bitmask)
            PM_DEBUG(("pm_esdb[%lu]: t_cp_info.ui4_cp_sys_bitmask = %lu -> %lu\n", i, pt_dbg_pm_esdb_arr[i].t_cp_info.ui4_cp_sys_bitmask, pt_pm_esdb_arr[i].t_cp_info.ui4_cp_sys_bitmask));
        if ( pt_pm_esdb_arr[i].t_cp_info.ui4_cp_setting != pt_dbg_pm_esdb_arr[i].t_cp_info.ui4_cp_setting)
            PM_DEBUG(("pm_esdb[%lu]: t_cp_info.ui4_cp_setting = %lu -> %lu\n", i, pt_dbg_pm_esdb_arr[i].t_cp_info.ui4_cp_setting, pt_pm_esdb_arr[i].t_cp_info.ui4_cp_setting));   
        
        if ( pt_pm_esdb_arr[i].t_filter_info.h_filter != pt_dbg_pm_esdb_arr[i].t_filter_info.h_filter)
            PM_DEBUG(("pm_esdb[%lu]: t_filter_info.h_filter = %lu -> %lu\n", i, pt_dbg_pm_esdb_arr[i].t_filter_info.h_filter, pt_pm_esdb_arr[i].t_filter_info.h_filter));
        if ( pt_pm_esdb_arr[i].t_filter_info.t_filter_cond != pt_dbg_pm_esdb_arr[i].t_filter_info.t_filter_cond )
            PM_DEBUG(("pm_esdb[%lu]: t_filter_info.t_filter_cond = %lu -> %lu\n", i, (UINT32)pt_dbg_pm_esdb_arr[i].t_filter_info.t_filter_cond, (UINT32)pt_pm_esdb_arr[i].t_filter_info.t_filter_cond));        
        
        if ( pt_pm_esdb_arr[i].t_crypto_info.h_des != pt_dbg_pm_esdb_arr[i].t_crypto_info.h_des )
            PM_DEBUG(("pm_esdb[%lu]: t_crypto_info.h_des = %lu -> %lu\n", i, pt_dbg_pm_esdb_arr[i].t_crypto_info.h_des, pt_pm_esdb_arr[i].t_crypto_info.h_des));
        if ( pt_pm_esdb_arr[i].t_crypto_info.t_des_cond != pt_dbg_pm_esdb_arr[i].t_crypto_info.t_des_cond )
            PM_DEBUG(("pm_esdb[%lu]: t_crypto_info.t_des_cond = %lu -> %lu\n", i, (UINT32)(pt_dbg_pm_esdb_arr[i].t_crypto_info.t_des_cond), (UINT32)(pt_pm_esdb_arr[i].t_crypto_info.t_des_cond)));
        
        if ( pt_pm_esdb_arr[i].ui2_service != pt_dbg_pm_esdb_arr[i].ui2_service)
            PM_DEBUG(("pm_esdb[%lu]: ui2_service = %d -> %d\n", i, pt_dbg_pm_esdb_arr[i].ui2_service, pt_pm_esdb_arr[i].ui2_service));     
        if ( pt_pm_esdb_arr[i].ui2_next != pt_dbg_pm_esdb_arr[i].ui2_next)
            PM_DEBUG(("pm_esdb[%lu]: ui2_next = %d -> %d\n\n", i, pt_dbg_pm_esdb_arr[i].ui2_next, pt_pm_esdb_arr[i].ui2_next));                   
    }
#endif /* PM_DEBUG_DB_DETAIL */     
    
    /* Show Free service node */
    i= ui2_esdb_free_nodes;
    PM_DEBUG(("pm_esdb free:"));
            
    while (i != PM_NULL_IDX)
    {
        PM_DEBUG(("[%lu]", i));
        i= pt_pm_esdb_arr[i].ui2_next;
    }
    PM_DEBUG(("\n")); 
        
    PM_DEBUG(("pm_esdb: postcondition end\n"));   
    
    return PMR_OK;  
    
    
}
#endif /* PM_DEBUG_DB_AUTOCOMPARE */

/*-----------------------------------------------------------------------------
 * Name: pmi_db_init_svnode
 *
 * Description: This API initialize the SVDB node. 
 *
 * Inputs:  pt_esdb_node     Reference to SVDB node.
 *                   
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID pmi_db_init_svnode(PM_SERVICE_INFO_T* pt_svdb_node)
{
    handle_link_init(&(pt_svdb_node->t_handle_link)); 
    pt_svdb_node->h_connection = NULL_HANDLE;
    pt_svdb_node->e_process_type = PM_PRESENT;
    pt_svdb_node->i4_speed = 0;
    pt_svdb_node->e_cond = PM_COND_CLOSE;
    pt_svdb_node->ui2_node_head = PM_NULL_IDX;  
    pt_svdb_node->h_ca_service = NULL_HANDLE;
#if (PM_ENABLE_CA)
    pt_svdb_node->pv_ca_ctrl = NULL;
#endif    

#if (PM_ENABLE_MULTIMEDIA)    
    x_memset(&(pt_svdb_node->t_media_fmt), 0, sizeof(MEDIA_FORMAT_T));
#endif
}

/*-----------------------------------------------------------------------------
 * Name: pmi_svdb_alloc
 *
 * Description: This API allocate a free SVDB node. 
 *
 * Inputs:  ppt_svdb_node    Contains reference to pointer of SVDB node
 *     
 * Outputs: ppt_svdb_node    Contains reference to pointer of SVDB node for new 
 *                           allocated one. 
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     DB is not initialized yet.
 *          PMR_INV_ARG      Invalid arguments.
 *          PMR_FAILED       No free SVDB node.
 ----------------------------------------------------------------------------*/
INT32 pmi_svdb_alloc(PM_SERVICE_INFO_T** ppt_svdb_node)
{
    INT32        i4_return;
    
    if (ppt_svdb_node != NULL)
    {
        *ppt_svdb_node = NULL;
    }    
    i4_return = PMR_NOT_INIT;
    
    if (b_db_init)
    {
        i4_return = PMR_INV_ARG;

        /* Validate arguments. */
        if (ppt_svdb_node != NULL)                  
        {
            i4_return = PMR_FAILED;

            /* Get next free handle control structure. */
            
            if ((*ppt_svdb_node = get_sv_node_from_idx (ui2_svdb_free_nodes)) != NULL)
            {
                ui2_svdb_free_nodes = (*ppt_svdb_node)->ui2_next;
                ui2_svdb_num_free--;
                
                (*ppt_svdb_node)->ui2_next = PM_NULL_IDX;

                i4_return = PMR_OK;
            }            
        }
        else
        {
            PM_DBG_ERROR( "ppt_svdb_node is NULL\n" );
        }
    } 
    
    if (i4_return != PMR_OK)
    {
         PM_DBG_ERROR("Failed to allocate service\n");        
    }
           
    return (i4_return);
}

/*-----------------------------------------------------------------------------
 * Name: pmi_svdb_free
 *
 * Description: This API free the specified SVDB node. 
 *
 * Inputs:  ppt_esdb_node    Contains reference to pointer of SVDB node to be 
 *                           free
 *     
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     DB is not initialized yet.
 *          PMR_INV_ARG      Invalid arguments.
 ----------------------------------------------------------------------------*/
INT32 pmi_svdb_free(PM_SERVICE_INFO_T* pt_svdb_node)
{
    INT32        i4_return;
    UINT16       ui2_index;    

    i4_return = PMR_NOT_INIT;

    if (b_db_init)
    {
        i4_return = PMR_INV_ARG;

        /* Validate handle. */
        if ((ui2_index = get_index_from_sv_node(pt_svdb_node)) != PM_NULL_IDX)
        {
            i4_return = PMR_OK;
            
            pmi_db_init_svnode(pt_svdb_node);
            
            pt_svdb_node->ui2_next = ui2_svdb_free_nodes;
            ui2_svdb_free_nodes = ui2_index;
            ui2_svdb_num_free++;      
        }
    }
    
    if (i4_return != PMR_OK)
    {
         PM_DBG_ERROR("Failed to free service\n");        
    }    
    
    return (i4_return);    
}

/*-----------------------------------------------------------------------------
 * Name: pmi_svdb_is_valid
 *
 * Description: This API free the specified SVDB node. 
 *
 * Inputs:  pt_esdb_node     Contains reference to pointer of SVDB node to be 
 *                           checked
 *     
 * Outputs: -
 *
 * Returns: TRUE             Valid
 *          FALSE            Invalid
 ----------------------------------------------------------------------------*/
BOOL  pmi_svdb_is_valid(PM_SERVICE_INFO_T* pt_svdb_node)
{
    if ((b_db_init)&&(pt_svdb_node!= NULL))
    {
        /* Validate SVDB node */
        if (get_index_from_sv_node(pt_svdb_node) != PM_NULL_IDX)
            return TRUE;          
        else
        {
            PM_DBG_ERROR("Service is not valid\n");
            return FALSE;
        }
    }
    else
    {
        PM_DBG_ERROR("Service is not valid\n");
        return FALSE;
    }
} 

#if PM_DEBUG_ENABLE
/*-----------------------------------------------------------------------------
 * Name: pmi_svdb_dump
 *
 * Description: This API dump whole SVDB related data. 
 *
 * Inputs:  -    
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *----------------------------------------------------------------------------*/
INT32 pmi_svdb_dump(VOID)
{
    UINT32 i,j;
    
    PM_DEBUG(("pm_svdb: dump begin===================================\n")); 
    
    PM_DEBUG(("pm_svdb: b_db_init= %d\n", b_db_init));
    PM_DEBUG(("pm_svdb: ui2_svdb_num_total = %d\n", ui2_svdb_num_total));
    PM_DEBUG(("pm_svdb: ui2_svdb_num_free = %d\n", ui2_svdb_num_free));  
    PM_DEBUG(("pm_svdb: ui2_svdb_free_nodes = %d\n", ui2_svdb_free_nodes));     

#if PM_DEBUG_DB_DETAIL     
    for (i=0; i<ui2_svdb_num_total; i++)
    {   
        PM_DEBUG(("pm_svdb[%lu]: t_handle_link.ui4_data[0] = %lu\n", i, pt_pm_svdb_arr[i].t_handle_link.ui4_data[0]));
        PM_DEBUG(("pm_svdb[%lu]: t_handle_link.ui4_data[1] = %lu\n", i, pt_pm_svdb_arr[i].t_handle_link.ui4_data[1])); 
                     
        PM_DEBUG(("pm_svdb[%lu]: h_connection = %lu\n", i, pt_pm_svdb_arr[i].h_connection));
        PM_DEBUG(("pm_svdb[%lu]: e_process_type = %d\n", i, pt_pm_svdb_arr[i].e_process_type));
        PM_DEBUG(("pm_svdb[%lu]: i4_speed = %lu\n", i, pt_pm_svdb_arr[i].i4_speed));        
        PM_DEBUG(("pm_svdb[%lu]: e_cond = %d\n", i, pt_pm_svdb_arr[i].e_cond));
        PM_DEBUG(("pm_svdb[%lu]: ui2_next = %d\n", i, pt_pm_svdb_arr[i].ui2_next));
        PM_DEBUG(("pm_svdb[%lu]: ui2_node_head = %d\n", i, pt_pm_svdb_arr[i].ui2_node_head));  
        PM_DEBUG(("pm_svdb[%lu]: h_ca_service = %lu\n", i, pt_pm_svdb_arr[i].h_ca_service));                 
    }
#endif /* PM_DEBUG_DB_DETAIL  */      
    
    /* Show Free service node */
    i= ui2_svdb_free_nodes;
    PM_DEBUG(("pm_svdb free:"));
            
    while (i != PM_NULL_IDX)
    {
        PM_DEBUG(("[%lu]", i));
        i= pt_pm_svdb_arr[i].ui2_next;
    }
    PM_DEBUG(("\n")); 
    
    /* Show Service List */
    for (i=0; i<ui2_svdb_num_total; i++)
    {   
        if (pt_pm_svdb_arr[i].ui2_node_head != PM_NULL_IDX)
        {
            PM_DEBUG(("pm_svdb[%lu]: ", i));
            j= pt_pm_svdb_arr[i].ui2_node_head;
            
            while (j != PM_NULL_IDX)
            {
                PM_DEBUG(("[%lu]", j));
                j= pt_pm_esdb_arr[j].ui2_next;
            }
            PM_DEBUG(("\n"));   
        }                              
    }     
        
    PM_DEBUG(("pm_svdb: dump end===================================\n"));   
    
    return PMR_OK;  
}
#endif /* PM_DEBUG_ENABLE */

#if PM_DEBUG_DB_AUTOCOMPARE
/*-----------------------------------------------------------------------------
 * Name: pm_svdb_debug_precondition
 *
 * Description: This API copys the whole SVDB related data to debug SVDB for 
 *              comparion. 
 *
 * Inputs:  -    
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *----------------------------------------------------------------------------*/
INT32 pm_svdb_debug_precondition(VOID)
{
     ui2_dbg_svdb_num_total = ui2_svdb_num_total;
     ui2_dbg_svdb_num_free = ui2_svdb_num_free;
     ui2_dbg_svdb_free_nodes = ui2_svdb_free_nodes;
     x_memcpy(pt_dbg_pm_svdb_arr, pt_pm_svdb_arr, sizeof(PM_SERVICE_INFO_T)*ui2_dbg_svdb_num_total);
     
     return PMR_OK;   
}

/*-----------------------------------------------------------------------------
 * Name: pm_svdb_debug_postcondition_dump
 *
 * Description: This API compares the current SVDB with the previous backup 
 *              data and dump the difference.
 *
 * Inputs:  -    
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *----------------------------------------------------------------------------*/
INT32 pm_svdb_debug_postcondition_dump(VOID)
{
    UINT32 i,j;
    
    PM_DEBUG(("pm_svdb: postcondition dump begin\n")); 
    
    if ( ui2_svdb_num_total != ui2_dbg_svdb_num_total  )
        PM_DEBUG(("pm_svdb: ui2_svdb_num_total = %d -> %d\n", ui2_dbg_svdb_num_total, ui2_svdb_num_total));
    if ( ui2_svdb_num_free != ui2_dbg_svdb_num_free )
        PM_DEBUG(("pm_svdb: ui2_svdb_num_free = %d -> %d\n", ui2_dbg_svdb_num_free, ui2_svdb_num_free));  
    if ( ui2_svdb_free_nodes != ui2_dbg_svdb_free_nodes )
        PM_DEBUG(("pm_svdb: ui2_svdb_free_nodes = %d -> %d\n", ui2_dbg_svdb_free_nodes, ui2_svdb_free_nodes));     
   
#if PM_DEBUG_DB_DETAIL     
    for (i=0; i<ui2_svdb_num_total; i++)
    {   
        if ( pt_pm_svdb_arr[i].t_handle_link.ui4_data[0] != pt_dbg_pm_svdb_arr[i].t_handle_link.ui4_data[0] )
            PM_DEBUG(("pm_svdb[%lu]: t_handle_link.ui4_data[0] = %lu -> %lu\n", i, pt_dbg_pm_svdb_arr[i].t_handle_link.ui4_data[0], pt_pm_svdb_arr[i].t_handle_link.ui4_data[0]));
        if ( pt_pm_svdb_arr[i].t_handle_link.ui4_data[1] != pt_dbg_pm_svdb_arr[i].t_handle_link.ui4_data[1] )
            PM_DEBUG(("pm_svdb[%lu]: t_handle_link.ui4_data[1] = %lu -> %lu\n", i, pt_dbg_pm_svdb_arr[i].t_handle_link.ui4_data[1], pt_pm_svdb_arr[i].t_handle_link.ui4_data[1]));        
        
        if ( pt_pm_svdb_arr[i].h_connection != pt_dbg_pm_svdb_arr[i].h_connection  )
            PM_DEBUG(("pm_svdb[%lu]: h_connection = %lu -> %lu\n", i, pt_dbg_pm_svdb_arr[i].h_connection, pt_pm_svdb_arr[i].h_connection));
        if ( pt_pm_svdb_arr[i].e_process_type != pt_dbg_pm_svdb_arr[i].e_process_type)
            PM_DEBUG(("pm_svdb[%lu]: e_process_type = %d -> %d\n", i, pt_dbg_pm_svdb_arr[i].e_process_type, pt_pm_svdb_arr[i].e_process_type));
         if ( pt_pm_svdb_arr[i].i4_speed != pt_dbg_pm_svdb_arr[i].i4_speed)
            PM_DEBUG(("pm_svdb[%lu]: i4_speed = %lu -> %lu\n", i, pt_dbg_pm_svdb_arr[i].i4_speed, pt_pm_svdb_arr[i].i4_speed));
        if ( pt_pm_svdb_arr[i].e_cond != pt_dbg_pm_svdb_arr[i].e_cond )
            PM_DEBUG(("pm_svdb[%lu]: e_cond = %d -> %d\n", i, pt_dbg_pm_svdb_arr[i].e_cond, pt_pm_svdb_arr[i].e_cond));
        if ( pt_pm_svdb_arr[i].ui2_next != pt_dbg_pm_svdb_arr[i].ui2_next  )
            PM_DEBUG(("pm_svdb[%lu]: ui2_next = %d -> %d\n", i, pt_dbg_pm_svdb_arr[i].ui2_next, pt_pm_svdb_arr[i].ui2_next));
        if ( pt_pm_svdb_arr[i].ui2_node_head != pt_dbg_pm_svdb_arr[i].ui2_node_head )
            PM_DEBUG(("pm_svdb[%lu]: ui2_node_head = %d -> %d\n", i, pt_dbg_pm_svdb_arr[i].ui2_node_head, pt_pm_svdb_arr[i].ui2_node_head));  
        if ( pt_pm_svdb_arr[i].h_ca_service != pt_dbg_pm_svdb_arr[i].h_ca_service )
            PM_DEBUG(("pm_svdb[%lu]: h_ca_service = %lu -> %lu\n", i, pt_dbg_pm_svdb_arr[i].h_ca_service,  pt_pm_svdb_arr[i].h_ca_service));    
        
    }
#endif /* PM_DEBUG_DB_DETAIL */      
    
    /* Show Free service node */
    i= ui2_svdb_free_nodes;
    PM_DEBUG(("pm_svdb free:"));
            
    while (i != PM_NULL_IDX)
    {
        PM_DEBUG(("[%lu]", i));
        i= pt_pm_svdb_arr[i].ui2_next;
    }
    PM_DEBUG(("\n")); 
    
    /* Show Service List */
    for (i=0; i<ui2_svdb_num_total; i++)
    {   
        if (pt_pm_svdb_arr[i].ui2_node_head != PM_NULL_IDX)
        {
            PM_DEBUG(("pm_svdb[%lu]: ", i));
            j= pt_pm_svdb_arr[i].ui2_node_head;
            
            while (j != PM_NULL_IDX)
            {
                PM_DEBUG(("[%lu]", j));
                j= pt_pm_esdb_arr[j].ui2_next;
            }
            PM_DEBUG(("\n"));   
        }                              
    }     
        
    PM_DEBUG(("pm_svdb: postcondition dump end\n"));   
    
    return PMR_OK;  
    
}   
#endif /* PM_DEBUG_DB_AUTOCOMPARE */

/*-----------------------------------------------------------------------------
 * Name: pmi_db_insert_to_service
 *
 * Description: This API attachs an ESDB node to SVDB node. 
 *
 * Inputs:  pt_esdb_node     Reference to ESDB node.
 *          pt_svdb_node     Reference to SVDB node.
 *                   
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     DB is not initialized yet.
 *          PMR_INV_ARG      Invalid arguments.The index of SVDB array.
 ----------------------------------------------------------------------------*/
INT32 pmi_db_insert_to_service(PM_COMPONENT_INFO_T* pt_esdb_node, PM_SERVICE_INFO_T* pt_svdb_node)
{
    INT32        i4_return;
    UINT16       ui2_service_index, ui2_es_index;
    
    i4_return = PMR_NOT_INIT;
    
    if (b_db_init)
    {
        i4_return = PMR_INV_ARG;
                
        if ((pt_esdb_node != NULL) && (pt_svdb_node != NULL)) 
        {            
            ui2_service_index = get_index_from_sv_node(pt_svdb_node);
            ui2_es_index = get_index_from_es_node(pt_esdb_node);
            
            if ((ui2_service_index != PM_NULL_IDX) && (ui2_es_index != PM_NULL_IDX))
            {
                /* Attach the ES node to Service */
                pt_esdb_node->ui2_service = ui2_service_index;     
                
                /* Attach the ES node to head of Service's ES list */           
                pt_esdb_node->ui2_next = pt_svdb_node->ui2_node_head;
                pt_svdb_node->ui2_node_head = get_index_from_es_node(pt_esdb_node);
                
                i4_return = PMR_OK;        
            }
        }        
        else
        {
            PM_DBG_ERROR( "parameter is invaild\n" );
        }
    }
    
    if (i4_return != PMR_OK)
    {
         PM_DBG_ERROR("Failed to add ES to service\n");        
    }
    
    return (i4_return);       
}

/*-----------------------------------------------------------------------------
 * Name: pmi_db_remove_from_service
 *
 * Description: This API detachs an ESDB node from SVDB node. 
 *
 * Inputs:  pt_esdb_node     Reference to ESDB node.
 *          pt_svdb_node     Reference to SVDB node.
 *                   
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     DB is not initialized yet.
 *          PMR_INV_ARG      Invalid arguments.The index of SVDB array.
 ----------------------------------------------------------------------------*/
INT32 pmi_db_remove_from_service(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    INT32        i4_return;
    UINT16       ui2_curr_index;
    UINT16       ui2_esnode_index;
    PM_SERVICE_INFO_T* pt_svdb_node;
    PM_COMPONENT_INFO_T* pt_curr_es_node;
    PM_COMPONENT_INFO_T* pt_last_es_node;    
    
    i4_return = PMR_NOT_INIT;
    
    if (b_db_init)
    {
        i4_return = PMR_INV_ARG;
        
        if ((pt_esdb_node != NULL) && (get_index_from_es_node(pt_esdb_node) != PM_NULL_IDX))             
        {   
            ui2_esnode_index = get_index_from_es_node(pt_esdb_node);
            pt_svdb_node = get_sv_node_from_idx (pt_esdb_node->ui2_service);
            
            /* Check service node exist or not */
            if (pt_svdb_node != NULL)
            {            
                ui2_curr_index = pt_svdb_node->ui2_node_head;
                pt_curr_es_node = get_es_node_from_idx (ui2_curr_index);
                if (pt_curr_es_node == NULL)
                {
                    return PMR_FAILED;
                }
                
                if (ui2_curr_index == ui2_esnode_index)
                {     
                    /* Remove the ES node from the head of Service's ES list */           
                    pt_svdb_node->ui2_node_head = pt_curr_es_node->ui2_next;  
                    
                    /* Dis-attach the ES node from Service */
                    pt_curr_es_node->ui2_service = PM_NULL_IDX; 
                    pt_curr_es_node->ui2_next = PM_NULL_IDX; 
                    i4_return = PMR_OK;            
                }
                else
                {    
                    pt_last_es_node = pt_curr_es_node;
                    ui2_curr_index = pt_curr_es_node->ui2_next;   
                    pt_curr_es_node = get_es_node_from_idx (ui2_curr_index);
                    if (pt_curr_es_node == NULL)
                    {
                        return PMR_FAILED;
                    }

                    while(ui2_curr_index != PM_NULL_IDX)
                    {
                         if (ui2_curr_index == ui2_esnode_index)
                         {
                            /* Remove the ES node from the Service's ES list */
                            pt_last_es_node->ui2_next = pt_curr_es_node->ui2_next;
                            
                            /* Dis-attach the ES node from Service */
                            pt_curr_es_node->ui2_service = PM_NULL_IDX;
                            pt_curr_es_node->ui2_next = PM_NULL_IDX;
                            i4_return = PMR_OK;                         
                            break;
                         }    
                         else
                         {
                            pt_last_es_node = pt_curr_es_node;
                            ui2_curr_index = pt_curr_es_node->ui2_next;   
                            pt_curr_es_node = get_es_node_from_idx (ui2_curr_index);
                            if (pt_curr_es_node == NULL)
                            {
                                return PMR_FAILED;
                            }
                         }                 
                    }                  
                }
            }            
            else
                i4_return = PMR_OK;  
        }        
        else
        {
            PM_DBG_ERROR( "pt_esdb_node is invaild\n" );
        }
    }
    
    if (i4_return != PMR_OK)
    {
         PM_DBG_ERROR("Failed to remove ES from service\n");        
    }    
    
    return (i4_return);    
}

/*-----------------------------------------------------------------------------
 * Name: pmi_db_parse_service
 *
 * Description: This API applies the specified parser function to each ESDB node 
 *              of the specified SVDB node 
 *
 * Inputs:  pt_svdb_node     Reference to SVDB node.
 *          pf_pmdb_parser   Contains the reference to the parser function. 
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     DB is not initialized yet.
 *          PMR_INV_ARG      Invalid arguments.
 ----------------------------------------------------------------------------*/
INT32 pmi_db_parse_service(PM_SERVICE_INFO_T* pt_svdb_node, pm_db_parser_fct pf_pmdb_parser)
{
    INT32        i4_return;
    UINT16       ui2_curr_index;    
    PM_COMPONENT_INFO_T* pt_curr_es_node;
        
    i4_return = PMR_NOT_INIT;
    
    if (b_db_init)
    {
        i4_return = PMR_INV_ARG;
        
        if ((pt_svdb_node != NULL) && (get_index_from_sv_node(pt_svdb_node) != PM_NULL_IDX) && (pf_pmdb_parser != NULL))
        {   
            ui2_curr_index = pt_svdb_node->ui2_node_head;                             
            while(ui2_curr_index != PM_NULL_IDX)
            {
                pt_curr_es_node = get_es_node_from_idx (ui2_curr_index);
                if (pt_curr_es_node == NULL)
                {
                    return PMR_FAILED;
                }
                
                /* Retrive the next ES node's index first */                
                ui2_curr_index = pt_curr_es_node->ui2_next;
                
                /* Apply the parser func. to ES node */
                pf_pmdb_parser(pt_curr_es_node);                                   
            }      
            i4_return = PMR_OK;      
        }        
        else
        {
            PM_DBG_ERROR( "parameter is invaild\n" );
        }
    }    
    return (i4_return);    
}

/*-----------------------------------------------------------------------------
 * Name: pmi_db_get_service_from_index
 *
 * Description: This API returns the index of ESDB array by reference of ESDB 
 *              node. 
 *
 * Inputs:  ui2_h_idx        The index for SVDB array.                   
 * Outputs: ppt_svdb_node    Contains the reference to the pointer to the 
 *                           returned SVDB node.
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     DB is not initialized yet.
 *          PMR_INV_ARG      Invalid arguments.
 ----------------------------------------------------------------------------*/
INT32 pmi_db_get_service_from_index(PM_SERVICE_INFO_T** ppt_svdb_node, UINT16  ui2_h_idx)
{
    INT32        i4_return;
        
    i4_return = PMR_NOT_INIT;
    
    if (b_db_init)
    {
        i4_return = PMR_INV_ARG;
        
        if (ppt_svdb_node != NULL)
        {   
            i4_return = PMR_FAILED;
            *ppt_svdb_node = get_sv_node_from_idx(ui2_h_idx);
            if (*ppt_svdb_node != NULL)
                i4_return = PMR_OK;      
        }        
    }    
    
    if (i4_return != PMR_OK)
    {
        PM_DBG_ERROR("Service index is not valid\n");
    }
    
    return (i4_return);    
}
/*-----------------------------------------------------------------------------
 * Name: pmi_db_count_es
 *
 * Description: This API is used to count the ES list in service 
 *              node. 
 *
 * Inputs:  pt_svdb_node     Contains the reference to SVDB node.
 *                  
 * Outputs: pui2_es_count    Contains the returned count. 
 *
 * Returns: PMR_OK           Success
 *          PMR_NOT_INIT     DB is not initialized yet.
 *          PMR_INV_ARG      Invalid arguments.
 ----------------------------------------------------------------------------*/
INT32 pmi_db_count_es(PM_SERVICE_INFO_T* pt_svdb_node, UINT16  *pui2_es_count)
{
    UINT16               ui2_curr_index;    
    PM_COMPONENT_INFO_T* pt_curr_es_node;
    INT32                i4_return;
        
    i4_return = PMR_NOT_INIT;
    
    if (b_db_init)
    {
        i4_return = PMR_INV_ARG;
        
        if ((pt_svdb_node != NULL) && (get_index_from_sv_node(pt_svdb_node) != PM_NULL_IDX) && (pui2_es_count != NULL))
        {   
            *pui2_es_count = 0;
            ui2_curr_index = pt_svdb_node->ui2_node_head;  
                                       
            while(ui2_curr_index != PM_NULL_IDX)
            {
                pt_curr_es_node = get_es_node_from_idx (ui2_curr_index);
                if (pt_curr_es_node == NULL)
                {
                    return PMR_FAILED;
                }

                *pui2_es_count = (*pui2_es_count) + ((UINT16)1);
                
                /* Retrive the next ES node's index */                
                ui2_curr_index = pt_curr_es_node->ui2_next;                                              
            }      
            i4_return = PMR_OK;      
        }              
        else
        {
            PM_DBG_ERROR( "parameter is invaild\n" );
        }
    }    
    
    return (i4_return);    
}

#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: pmi_svdb_dump_4cli
 *
 * Description: This API dump whole SVDB related data for CLI command.
 *
 * Inputs:  -    
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *----------------------------------------------------------------------------*/
INT32 pmi_service_dump_4cli(VOID)
{
    UINT32              i;
    UINT16              ui2_svdb_used_num = ui2_svdb_num_total - ui2_svdb_num_free - 1;
    UINT16              ui2_svdb_used_counter;
    UINT16              ui2_svdb_free_idx;
    PM_SERVICE_INFO_T*  pt_svc_node;
    
    x_dbg_stmt( 
        "SVC node total number is (%d), used number is (%d), the first one is reserved\n", 
        ui2_svdb_num_total,
        ui2_svdb_used_num );

    ui2_svdb_used_counter = 0;
    for (i = 1; i < ui2_svdb_num_total; i++)
    {   
        UINT16  ui2_handle_num;
        
        ui2_handle_num = handle_num_links( &pt_pm_svdb_arr[i].t_handle_link );
        if (0 != ui2_handle_num)
        {
            /* this is a used svdb node */
            ui2_svdb_used_counter++;
            
            /* output the svdb node detail information */
            x_dbg_stmt( "\tsvdbnode index is (%d)\n", i );

            switch (pt_pm_svdb_arr[i].e_process_type)
            {
                case PM_PRESENT:
                    x_dbg_stmt( "\t\tprocess type is PM_PRESENT" );
                    break;
                case PM_DETECTION:
                    x_dbg_stmt( "\t\tprocess type is PM_DETECTION" );
                    break;
                case PM_RECORDING:
                    x_dbg_stmt( "\t\tprocess type is PM_RECORDING" );
                    break;
                case PM_MULTIMEDIA:
                    x_dbg_stmt( "\t\tprocess type is PM_MULTIMEDIA" );
                    break;
                default:
                    DBG_ASSERT( 0, __LINE__ );
                    break;
            }
                
            switch (pt_pm_svdb_arr[i].e_cond)
                {
                case PM_COND_CLOSE:
                    x_dbg_stmt( "\t\tcondition is PM_COND_CLOSE" );
                    break;
                case PM_COND_OPEN:
                    x_dbg_stmt( "\t\tcondition is PM_COND_OPEN" );
                    break;
                case PM_COND_PLAY:
                    x_dbg_stmt( "\t\tcondition is PM_COND_PLAY" );
                    break;
                case PM_COND_STOP:
                    x_dbg_stmt( "\t\tcondition is PM_COND_STOP" );
                    break;
                default:
                    DBG_ASSERT( 0, __LINE__ );
                    break;
            }
                             
            x_dbg_stmt( "\t\tthis SVDB node has following ES node:\n" );

            {
                UINT16                  ui2_idx = 1;
                PM_COMPONENT_INFO_T*    pt_curr_es_node;
                UINT16                  ui2_es_idx = pt_pm_svdb_arr[i].ui2_node_head;

                while (PM_NULL_IDX != ui2_es_idx)
                {
                    pt_curr_es_node = get_es_node_from_idx(ui2_es_idx);
                    if (pt_curr_es_node == NULL)
                    {
                        break;
                    }

                    x_dbg_stmt( "\t\t\t[%d]\n", ui2_idx );
                    pmi_component_dump_4cli(ui2_es_idx);
                    ui2_es_idx = pt_curr_es_node->ui2_next;
                    ui2_idx++;
                }
            }
        }                              
    }

    DBG_ASSERT( ui2_svdb_used_counter == ui2_svdb_used_num, __LINE__ );

    x_dbg_stmt( "total free svdbnode number is (%d), the index is:", ui2_svdb_num_free );
    ui2_svdb_free_idx = ui2_svdb_free_nodes;    
    while (PM_NULL_IDX != ui2_svdb_free_idx)
    {
        x_dbg_stmt( " (%d)", ui2_svdb_free_idx );
        
        pt_svc_node = get_sv_node_from_idx( ui2_svdb_free_idx );
        ui2_svdb_free_idx = pt_svc_node->ui2_next;
    }     
    x_dbg_stmt( "\n" );
    
    return PMR_OK;  
}

/*-----------------------------------------------------------------------------
 * Name: pmi_component_dump_4cli
 *
 * Description: This API dump ESDB related data for CLI command.
 *
 * Inputs:  -    
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *----------------------------------------------------------------------------*/
INT32 pmi_component_dump_4cli( UINT16 ui2_es_idx )
{
    PM_COMPONENT_INFO_T*    pt_curr_es_node;
    
    pt_curr_es_node = get_es_node_from_idx(ui2_es_idx);

    switch (ESNODE_PROC_TYPE(pt_curr_es_node))
    {
        case PM_PRESENT:
            x_dbg_stmt( "\t\t\tcomponent type is PM_PRESENT\n" );
            break;
            
        case PM_RECORDING:
            x_dbg_stmt( "\t\t\tcomponent type is PM_RECORDING\n" );
            break;
            
        case PM_MULTIMEDIA:
            x_dbg_stmt( "\t\t\tcomponent type is PM_MULTIMEDIA\n" );
            break;
            
        default:
            x_dbg_stmt( "\t\t\tcomponent type is PM_DETECTION\n" );
            break;
    }

    x_dbg_stmt( 
        "\t\t\tpid(%d) stream type(%d) filter status(%d) filter type(%d) request state(%d) DEST type(%d) DEST ID(%d)\n",
        pt_curr_es_node->t_scdb_info.t_component_pid,
        pt_curr_es_node->t_stream_comp_id.e_type,
        pt_curr_es_node->e_filter_status,
        pt_curr_es_node->t_filter_type,
        pt_curr_es_node->e_request_state,
        ESNODE_SINK_INFO(pt_curr_es_node).e_type, 
        ESNODE_SINK_INFO(pt_curr_es_node).ui2_id );

    return PMR_OK;  
}

/*-----------------------------------------------------------------------------
 * Name: pmi_db_resource_statistics_4cli
 *
 * Description: This API dump db statistics.
 *
 * Inputs:  -    
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *----------------------------------------------------------------------------*/
INT32 pmi_db_resource_statistics_4cli(VOID)
{    
    x_dbg_stmt("{PM}Service Total:%d Free:%d\n", ui2_svdb_num_total-1, ui2_svdb_num_free);
    x_dbg_stmt("{PM}ES Total:%d Free:%d\n", ui2_esdb_num_total-1, ui2_esdb_num_free);
    
    return PMR_OK;
}
#endif


 
