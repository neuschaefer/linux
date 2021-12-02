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
 * $RCSfile: prc_freeca_db.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/3 $
 * $SWAuthor: Felisa Hsiao $
 * $MD5HEX: 69970dcca7d752d316487b537d815b88 $
 *
 * Description: 
 *         This file is the implementation of CA Core's database.
 *
 * History:
 *           
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "prc_mngr.h"
#include "prc_freeca_db.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define CA_DEBUG_ENABLE    0
#define CA_DEBUG_DB_DETAIL   0
#define CA_DEBUG(_s_)    x_dbg_stmt _s_
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/ 
static BOOL b_cadb_init = FALSE;                       /* Tracks if CA_ESDB has been initialized. */  
static CADB_ES_OBJ_T* pt_ca_esdb_arr = NULL;           /* CA_ESDB Data store */
static UINT16 ui2_ca_esdb_num_total = 0;               /* Total number of CA_ESDB node. */
static UINT16 ui2_ca_esdb_num_free = 0;                /* Number of free CA_ESDB node. */
static UINT16 ui2_ca_esdb_free_nodes = 0;              /* The head of free nodes */
static CADB_SV_OBJ_T* pt_ca_svdb_arr = NULL;           /* CA_SVDB Data store */
static UINT16 ui2_ca_svdb_num_total = 0;               /* Total number of CA_SVDB node. */
static UINT16 ui2_ca_svdb_num_free = 0;                /* Number of free CA_SVDB node. */
static UINT16 ui2_ca_svdb_free_nodes = 0;              /* The head of free nodes */

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

static CADB_ES_OBJ_T* get_es_node_from_idx (UINT16  ui2_h_idx); 
static UINT16 get_index_from_es_node (CADB_ES_OBJ_T* pt_node);
static CADB_SV_OBJ_T* get_sv_node_from_idx (UINT16  ui2_h_idx);
static UINT16 get_index_from_sv_node (CADB_SV_OBJ_T* pt_node);
static VOID pmi_freeca_db_init_svnode(CADB_SV_OBJ_T* pt_svdb_node);
static VOID pmi_freeca_db_init_esnode(CADB_ES_OBJ_T* pt_esdb_node);

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
static CADB_ES_OBJ_T* get_es_node_from_idx (UINT16  ui2_h_idx)
{
    CADB_ES_OBJ_T* pt_node;

    pt_node = NULL;
    
    if ((ui2_h_idx > CA_NULL_IDX)&&(ui2_h_idx < ui2_ca_esdb_num_total))
    {
        pt_node  = &(pt_ca_esdb_arr[ui2_h_idx]);
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
static UINT16 get_index_from_es_node (CADB_ES_OBJ_T* pt_node)
{
    UINT16    ui2_idx;
    
    ui2_idx = CA_NULL_IDX;
            
    if ((((UINT16)(pt_node - pt_ca_esdb_arr)) < ui2_ca_esdb_num_total) && 
        (((UINT16)(pt_node - pt_ca_esdb_arr)) > CA_NULL_IDX))
    {
        ui2_idx = (UINT16)(pt_node - pt_ca_esdb_arr);    
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
static CADB_SV_OBJ_T* get_sv_node_from_idx (UINT16  ui2_h_idx)
{
    CADB_SV_OBJ_T* pt_node;

    pt_node = NULL;
    
    if ((ui2_h_idx > CA_NULL_IDX)&&(ui2_h_idx < ui2_ca_svdb_num_total))
    {
        pt_node  = &(pt_ca_svdb_arr[ui2_h_idx]);
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
static UINT16 get_index_from_sv_node (CADB_SV_OBJ_T* pt_node)
{
    UINT16    ui2_idx;
    
    ui2_idx = CA_NULL_IDX;
            
    if ((((UINT16)(pt_node - pt_ca_svdb_arr)) < ui2_ca_svdb_num_total) && 
        (((UINT16)(pt_node - pt_ca_svdb_arr)) > CA_NULL_IDX))
    {
        ui2_idx = (UINT16)(pt_node - pt_ca_svdb_arr);
    }      
    return ui2_idx;
}

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_db_init
 *
 * Description: This API allocate DB and initialize the whole DB related data. 
 *
 * Inputs:  ui2_num_services Number of SVDB node.
 *          ui2_num_comps    Number of ESDB node.
 *                   
 * Outputs: -
 *
 * Returns: CA_OK             Success.
 *          CA_ALREADY_INIT   Already initialize.
 *          CA_NOT_ENOUGH_MEM Failure due to memory allocation failed
 ----------------------------------------------------------------------------*/    
INT32 pmi_freeca_db_init(UINT16  ui2_num_services, UINT16  ui2_num_comps)
{
    INT32     i4_return;
    UINT16    ui2_i;
    
    i4_return = CA_ALREADY_INIT;
    
    if (! (b_cadb_init))
    {
        i4_return = CA_NOT_ENOUGH_MEM;

        ui2_ca_esdb_num_total = ui2_num_comps + ((UINT16)1);
        ui2_ca_svdb_num_total = ui2_num_services + ((UINT16)1);
        
        /* Allocate memory first */        
        if (((pt_ca_svdb_arr = (CADB_SV_OBJ_T*) x_mem_alloc (sizeof (CADB_SV_OBJ_T) * ui2_ca_svdb_num_total)) != NULL) && 
            ((pt_ca_esdb_arr = (CADB_ES_OBJ_T*) x_mem_alloc (sizeof (CADB_ES_OBJ_T) * ui2_ca_esdb_num_total)) != NULL))
           
        {
            i4_return = CA_OK;
            b_cadb_init = TRUE;       
            
             /* Initialize ES DB */
            ui2_ca_esdb_num_free = 0;
            ui2_ca_esdb_free_nodes = CA_NULL_IDX;
        
            for (ui2_i=1; ui2_i<ui2_ca_esdb_num_total; ui2_i++)
            { 
                pt_ca_esdb_arr[ui2_i].h_ca = NULL_HANDLE;
                x_memset(&(pt_ca_esdb_arr[ui2_i].t_stream_comp_id), 0, sizeof(STREAM_COMP_ID_T));      
                pt_ca_esdb_arr[ui2_i].t_component_pid = 0;   
                pt_ca_esdb_arr[ui2_i].pf_nfy = NULL;
                pt_ca_esdb_arr[ui2_i].pv_tag = NULL;   
                pt_ca_esdb_arr[ui2_i].e_cond = PM_CA_COND_CLOSED;
                pt_ca_esdb_arr[ui2_i].ui4_detail_code = 0;     
                pt_ca_esdb_arr[ui2_i].t_cpinfo.ui1_ctrl_size =0;               
                
                pt_ca_esdb_arr[ui2_i].ui2_service = CA_NULL_IDX;
                pt_ca_esdb_arr[ui2_i].ui2_next = ui2_ca_esdb_free_nodes;   
                ui2_ca_esdb_free_nodes = ui2_i;
                ui2_ca_esdb_num_free++;
            }
            
            /* Initialize Service DB */
            ui2_ca_svdb_num_free = 0;
            ui2_ca_svdb_free_nodes = CA_NULL_IDX;            
            
            for(ui2_i=1; ui2_i<ui2_ca_svdb_num_total; ui2_i++)
            {  
                pt_ca_svdb_arr[ui2_i].h_connection = NULL_HANDLE;
                pt_ca_svdb_arr[ui2_i].h_ca_service = NULL_HANDLE;
                pt_ca_svdb_arr[ui2_i].e_process_type = PM_PRESENT;
                pt_ca_svdb_arr[ui2_i].h_pat = NULL_HANDLE;
                pt_ca_svdb_arr[ui2_i].e_pat_cond = TM_COND_UNAVAILABLE;
                pt_ca_svdb_arr[ui2_i].h_pmt = NULL_HANDLE;
                pt_ca_svdb_arr[ui2_i].e_pmt_cond = TM_COND_UNAVAILABLE;
                pt_ca_svdb_arr[ui2_i].b_used = FALSE;
                
                pt_ca_svdb_arr[ui2_i].ui2_node_head = CA_NULL_IDX;
                pt_ca_svdb_arr[ui2_i].ui2_next = ui2_ca_svdb_free_nodes;   
                ui2_ca_svdb_free_nodes = ui2_i;
                ui2_ca_svdb_num_free++;
            }      
        } 
        else
        {
            /* Free memory */             
            if (pt_ca_svdb_arr != NULL)
                x_mem_free(pt_ca_svdb_arr); 
            if (pt_ca_esdb_arr != NULL)
                x_mem_free(pt_ca_esdb_arr);
        }
    }
           
    return i4_return;  
}

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_db_init_esnode
 *
 * Description: This API initialize the ESDB node. 
 *
 * Inputs:  pt_esdb_node     Reference to ESDB node.
 *                   
 * Outputs: pt_esdb_node     Reference to ESDB node have been initialized.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID pmi_freeca_db_init_esnode(CADB_ES_OBJ_T* pt_esdb_node)
{
    pt_esdb_node->h_ca = NULL_HANDLE;
    x_memset(&(pt_esdb_node->t_stream_comp_id), 0, sizeof(STREAM_COMP_ID_T));     
    pt_esdb_node->t_component_pid = 0;       
    pt_esdb_node->pf_nfy = NULL;
    pt_esdb_node->pv_tag = NULL;   
    pt_esdb_node->e_cond = PM_CA_COND_CLOSED;
    pt_esdb_node->ui4_detail_code = 0;     
    pt_esdb_node->t_cpinfo.ui1_ctrl_size = 0; 
              
    
    pt_esdb_node->ui2_service = CA_NULL_IDX;    
}   

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_esdb_alloc
 *
 * Description: This API allocate a free ESDB node. 
 *
 * Inputs:  ppt_esdb_node    Contains reference to pointer of ESDB node
 *     
 * Outputs: ppt_esdb_node    Contains reference to pointer of ESDB node for new 
 *                           allocated one. 
 *
 * Returns: CA_OK           Success
 *          CA_NOT_INIT     DB is not initialized yet.
 *          CA_INV_ARG      Invalid arguments.
 *          CA_FAILED       No free ESDB node.
 ----------------------------------------------------------------------------*/
INT32 pmi_freeca_esdb_alloc(CADB_ES_OBJ_T** ppt_esdb_node)
{
    INT32        i4_return;
    
    if (ppt_esdb_node != NULL)
    {
        *ppt_esdb_node = NULL;
    }
    i4_return = CA_NOT_INIT;
    
    if (b_cadb_init)
    {
        i4_return = CA_INV_ARG;

        /* Validate arguments. */
        if (ppt_esdb_node != NULL)                  
        {
            i4_return = CA_FAILED;

            /* Get next free handle control structure. */
            
            if ((*ppt_esdb_node = get_es_node_from_idx (ui2_ca_esdb_free_nodes)) != NULL)
            {
                ui2_ca_esdb_free_nodes = (*ppt_esdb_node)->ui2_next;
                ui2_ca_esdb_num_free--;
                
                (*ppt_esdb_node)->ui2_next = CA_NULL_IDX;

                i4_return = CA_OK;
            }            
        }
    }
    
    return (i4_return);
}    

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_esdb_free
 *
 * Description: This API free the specified ESDB node. 
 *
 * Inputs:  ppt_esdb_node    Contains reference to pointer of ESDB node to be 
 *                           free
 *     
 * Outputs: -
 *
 * Returns: CA_OK           Success
 *          CA_NOT_INIT     DB is not initialized yet.
 *          CA_INV_ARG      Invalid arguments.
 ----------------------------------------------------------------------------*/
INT32 pmi_freeca_esdb_free(CADB_ES_OBJ_T* pt_esdb_node)
{
    INT32        i4_return;
    UINT16       ui2_index;    

    i4_return = CA_NOT_INIT;

    if (b_cadb_init)
    {
        i4_return = CA_INV_ARG;

        /* Validate handle. */
        if ((ui2_index = get_index_from_es_node(pt_esdb_node)) != CA_NULL_IDX)
        {
            i4_return = CA_OK;
            
            /* Reinitialize node */
            pmi_freeca_db_init_esnode(pt_esdb_node);  
            
            pt_esdb_node->ui2_next = ui2_ca_esdb_free_nodes;
            ui2_ca_esdb_free_nodes = ui2_index;
            ui2_ca_esdb_num_free++;           
        }
    }
    
    return (i4_return);    
} 

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_esdb_is_valid
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
BOOL  pmi_freeca_esdb_is_valid(CADB_ES_OBJ_T* pt_esdb_node)
{
    if ((b_cadb_init)&&(pt_esdb_node!= NULL))
    {
        /* Validate ESDB */
        if (get_index_from_es_node(pt_esdb_node) != CA_NULL_IDX)
            return TRUE;          
        else
            return FALSE;
    }
    else
        return FALSE;    
} 
#if CA_DEBUG_ENABLE
/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_esdb_dump
 *
 * Description: This API dump whole ESDB related data. 
 *
 * Inputs:  -    
 * Outputs: -
 *
 * Returns: CA_OK           Success
 *----------------------------------------------------------------------------*/
INT32 pmi_freeca_esdb_dump(VOID)
{
    UINT32 i;
    
    CA_DEBUG(("ca_esdb: dump begin===================================\n")); 
    
    CA_DEBUG(("ca_esdb: b_cadb_init= %d\n", b_cadb_init));
    CA_DEBUG(("ca_esdb: ui2_ca_esdb_num_total = %d\n", ui2_ca_esdb_num_total));
    CA_DEBUG(("ca_esdb: ui2_ca_esdb_num_free = %d\n", ui2_ca_esdb_num_free));  
    CA_DEBUG(("ca_esdb: ui2_ca_esdb_free_nodes = %d\n", ui2_ca_esdb_free_nodes));     

#if CA_DEBUG_DB_DETAIL     
    for (i=0; i<ui2_ca_esdb_num_total; i++)
    {   
        CA_DEBUG(("ca_esdb[%d]: h_ca = %lu\n", i, pt_ca_esdb_arr[i].h_ca));
        CA_DEBUG(("ca_esdb[%d]: t_stream_comp_id.e_type = %d\n", i, pt_ca_esdb_arr[i].t_stream_comp_id.e_type));
        CA_DEBUG(("ca_esdb[%d]: t_stream_comp_id.pv_stream_tag = %lu\n", i, (UINT32)(pt_ca_esdb_arr[i].t_stream_comp_id.pv_stream_tag)));
        CA_DEBUG(("ca_esdb[%d]: pf_nfy = %lu\n", i, (UINT32)(pt_ca_esdb_arr[i].pf_nfy)));       
        CA_DEBUG(("ca_esdb[%d]: pv_tag = %lu\n", i, (UINT32)(pt_ca_esdb_arr[i].pv_tag)));
        CA_DEBUG(("ca_esdb[%d]: e_cond = %d\n", i, (UINT32)(pt_ca_esdb_arr[i].e_cond)));       
        CA_DEBUG(("ca_esdb[%d]: ui4_detail_code = 0x%x\n", i, (UINT32)(pt_ca_esdb_arr[i].ui4_detail_code)));
        CA_DEBUG(("ca_esdb[%d]: ui1_ctrl_size = %d\n", i, (UINT32)(pt_ca_esdb_arr[i].t_cpinfo.ui1_ctrl_size)));
        
        CA_DEBUG(("ca_esdb[%d]: ui2_service = %d\n", i, pt_ca_esdb_arr[i].ui2_service));     
        CA_DEBUG(("ca_esdb[%d]: ui2_next = %d\n\n", i, pt_ca_esdb_arr[i].ui2_next));                   
    }
#endif  /* CA_DEBUG_DB_DETAIL */    
    
    /* Show Free service node */
    i= ui2_ca_esdb_free_nodes;
    CA_DEBUG(("ca_esdb free:"));
            
    while (i != CA_NULL_IDX)
    {
        CA_DEBUG(("[%d]", i));
        i= pt_ca_esdb_arr[i].ui2_next;
    }
    CA_DEBUG(("\n")); 
        
    CA_DEBUG(("ca_esdb: dump end===================================\n"));   
    
    return CA_OK;  
}
#endif /* CA_DEBUG_ENABLE */

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_db_init_svnode
 *
 * Description: This API initialize the SVDB node. 
 *
 * Inputs:  pt_esdb_node     Reference to SVDB node.
 *                   
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID pmi_freeca_db_init_svnode(CADB_SV_OBJ_T* pt_svdb_node)
{   
    pt_svdb_node->h_connection = NULL_HANDLE;
    pt_svdb_node->h_ca_service = NULL_HANDLE;
    pt_svdb_node->e_process_type = PM_PRESENT;   
    pt_svdb_node->h_pat = NULL_HANDLE;
    pt_svdb_node->e_pat_cond = TM_COND_UNAVAILABLE;
    pt_svdb_node->h_pmt = NULL_HANDLE;
    pt_svdb_node->e_pmt_cond = TM_COND_UNAVAILABLE;    
    pt_svdb_node->b_used = FALSE;
    pt_svdb_node->ui2_node_head = CA_NULL_IDX;      
}

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_svdb_alloc
 *
 * Description: This API allocate a free SVDB node. 
 *
 * Inputs:  ppt_svdb_node    Contains reference to pointer of SVDB node
 *     
 * Outputs: ppt_svdb_node    Contains reference to pointer of SVDB node for new 
 *                           allocated one. 
 *
 * Returns: CA_OK           Success
 *          CA_NOT_INIT     DB is not initialized yet.
 *          CA_INV_ARG      Invalid arguments.
 *          CA_FAILED       No free SVDB node.
 ----------------------------------------------------------------------------*/
INT32 pmi_freeca_svdb_alloc(CADB_SV_OBJ_T** ppt_svdb_node)
{
    INT32        i4_return;
    
    if (ppt_svdb_node != NULL)
    {
        *ppt_svdb_node = NULL;
    }
    i4_return = CA_NOT_INIT;
    
    if (b_cadb_init)
    {
        i4_return = CA_INV_ARG;

        /* Validate arguments. */
        if (ppt_svdb_node != NULL)                  
        {
            i4_return = CA_FAILED;

            /* Get next free handle control structure. */
            
            if ((*ppt_svdb_node = get_sv_node_from_idx (ui2_ca_svdb_free_nodes)) != NULL)
            {
                ui2_ca_svdb_free_nodes = (*ppt_svdb_node)->ui2_next;
                ui2_ca_svdb_num_free--;
                
                (*ppt_svdb_node)->ui2_next = CA_NULL_IDX;

                i4_return = CA_OK;
            }            
        }
    }    
    return (i4_return);
}

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_svdb_free
 *
 * Description: This API free the specified SVDB node. 
 *
 * Inputs:  ppt_esdb_node    Contains reference to pointer of SVDB node to be 
 *                           free
 *     
 * Outputs: -
 *
 * Returns: CA_OK           Success
 *          CA_NOT_INIT     DB is not initialized yet.
 *          CA_INV_ARG      Invalid arguments.
 ----------------------------------------------------------------------------*/
INT32 pmi_freeca_svdb_free(CADB_SV_OBJ_T* pt_svdb_node)
{
    INT32        i4_return;
    UINT16       ui2_index;    

    i4_return = CA_NOT_INIT;

    if (b_cadb_init)
    {
        i4_return = CA_INV_ARG;

        /* Validate handle. */
        if ((ui2_index = get_index_from_sv_node(pt_svdb_node)) != CA_NULL_IDX)
        {
            i4_return = CA_OK;
            
            pmi_freeca_db_init_svnode(pt_svdb_node);
            
            pt_svdb_node->ui2_next = ui2_ca_svdb_free_nodes;
            ui2_ca_svdb_free_nodes = ui2_index;
            ui2_ca_svdb_num_free++;      
        }
    }
    
    return (i4_return);    
}

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_svdb_is_valid
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
BOOL  pmi_freeca_svdb_is_valid(CADB_SV_OBJ_T* pt_svdb_node)
{
    if ((b_cadb_init)&&(pt_svdb_node!= NULL))
    {
        /* Validate SVDB node */
        if (get_index_from_sv_node(pt_svdb_node) != CA_NULL_IDX)
            return TRUE;          
        else
            return FALSE;
    }
    else
        return FALSE;    
} 

#if CA_DEBUG_ENABLE
/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_svdb_dump
 *
 * Description: This API dump whole SVDB related data. 
 *
 * Inputs:  -    
 * Outputs: -
 *
 * Returns: CA_OK           Success
 *----------------------------------------------------------------------------*/
INT32 pmi_freeca_svdb_dump(VOID)
{
    UINT32 i,j;
    
    CA_DEBUG(("ca_svdb: dump begin===================================\n")); 
    
    CA_DEBUG(("ca_svdb: b_cadb_init= %d\n", b_cadb_init));
    CA_DEBUG(("ca_svdb: ui2_ca_svdb_num_total = %d\n", ui2_ca_svdb_num_total));
    CA_DEBUG(("ca_svdb: ui2_ca_svdb_num_free = %d\n", ui2_ca_svdb_num_free));  
    CA_DEBUG(("ca_svdb: ui2_ca_svdb_free_nodes = %d\n", ui2_ca_svdb_free_nodes));     

#if CA_DEBUG_DB_DETAIL     
    for (i=0; i<ui2_ca_svdb_num_total; i++)
    {       
        CA_DEBUG(("ca_svdb[%d]: h_connection = %lu\n", i, pt_ca_svdb_arr[i].h_connection));
        CA_DEBUG(("ca_svdb[%d]: h_ca_service = %lu\n", i, pt_ca_svdb_arr[i].h_ca_service));                 
        CA_DEBUG(("ca_svdb[%d]: e_process_type = %d\n", i, pt_ca_svdb_arr[i].e_process_type));
        CA_DEBUG(("ca_svdb[%d]: b_used = %d\n", i, pt_ca_svdb_arr[i].b_used));
        CA_DEBUG(("ca_svdb[%d]: ui2_next = %d\n", i, pt_ca_svdb_arr[i].ui2_next));
        CA_DEBUG(("ca_svdb[%d]: ui2_node_head = %d\n", i, pt_ca_svdb_arr[i].ui2_node_head));  
        
    }
#endif /* CA_DEBUG_DB_DETAIL  */      
    
    /* Show Free service node */
    i= ui2_ca_svdb_free_nodes;
    CA_DEBUG(("ca_svdb free:"));
            
    while (i != CA_NULL_IDX)
    {
        CA_DEBUG(("[%d]", i));
        i= pt_ca_svdb_arr[i].ui2_next;
    }
    CA_DEBUG(("\n")); 
    
    /* Show Service List */
    for (i=0; i<ui2_ca_svdb_num_total; i++)
    {   
        if (pt_ca_svdb_arr[i].ui2_node_head != CA_NULL_IDX)
        {
            CA_DEBUG(("ca_svdb[%d]: ", i));
            j= pt_ca_svdb_arr[i].ui2_node_head;
            
            while (j != CA_NULL_IDX)
            {
                CA_DEBUG(("[%d]", j));
                j= pt_ca_esdb_arr[j].ui2_next;
            }
            CA_DEBUG(("\n"));   
        }                              
    }     
        
    CA_DEBUG(("ca_svdb: dump end===================================\n"));   
    
    return CA_OK;  
}
#endif /* CA_DEBUG_ENABLE */

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_db_insert_to_service
 *
 * Description: This API attachs an ESDB node to SVDB node. 
 *
 * Inputs:  pt_esdb_node     Reference to ESDB node.
 *          pt_svdb_node     Reference to SVDB node.
 *                   
 * Outputs: -
 *
 * Returns: CA_OK           Success
 *          CA_NOT_INIT     DB is not initialized yet.
 *          CA_INV_ARG      Invalid arguments.The index of SVDB array.
 ----------------------------------------------------------------------------*/
INT32 pmi_freeca_db_insert_to_service(CADB_ES_OBJ_T* pt_esdb_node, CADB_SV_OBJ_T* pt_svdb_node)
{
    INT32        i4_return;
    UINT16       ui2_service_index, ui2_es_index;
    
    i4_return = CA_NOT_INIT;
    
    if (b_cadb_init)
    {
        i4_return = CA_INV_ARG;
                
        if ((pt_esdb_node != NULL) && (pt_svdb_node != NULL)) 
        {            
            ui2_service_index = get_index_from_sv_node(pt_svdb_node);
            ui2_es_index = get_index_from_es_node(pt_esdb_node);
            
            if ((ui2_service_index != CA_NULL_IDX) && (ui2_es_index != CA_NULL_IDX))
            {
                /* Attach the ES node to Service */
                pt_esdb_node->ui2_service = ui2_service_index;     
                
                /* Attach the ES node to head of Service's ES list */           
                pt_esdb_node->ui2_next = pt_svdb_node->ui2_node_head;
                pt_svdb_node->ui2_node_head = get_index_from_es_node(pt_esdb_node);
                
                i4_return = CA_OK;        
            }
        }        
    }
    
    return (i4_return);       
}

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_db_remove_from_service
 *
 * Description: This API detachs an ESDB node from SVDB node. 
 *
 * Inputs:  pt_esdb_node     Reference to ESDB node.
 *          pt_svdb_node     Reference to SVDB node.
 *                   
 * Outputs: -
 *
 * Returns: CA_OK           Success
 *          CA_NOT_INIT     DB is not initialized yet.
 *          CA_INV_ARG      Invalid arguments.The index of SVDB array.
 ----------------------------------------------------------------------------*/
INT32 pmi_freeca_db_remove_from_service(CADB_ES_OBJ_T* pt_esdb_node)
{
    INT32        i4_return;
    UINT16       ui2_curr_index;
    UINT16       ui2_esnode_index;
    CADB_SV_OBJ_T* pt_svdb_node;
    CADB_ES_OBJ_T* pt_curr_es_node;
    CADB_ES_OBJ_T* pt_last_es_node;    
    
    i4_return = CA_NOT_INIT;
    
    if (b_cadb_init)
    {
        i4_return = CA_INV_ARG;
        
        if ((pt_esdb_node != NULL) && (get_index_from_es_node(pt_esdb_node) != CA_NULL_IDX))             
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
                    return CA_FAILED;
                }
                
                if (ui2_curr_index == ui2_esnode_index)
                {     
                    /* Remove the ES node from the head of Service's ES list */           
                    pt_svdb_node->ui2_node_head = pt_curr_es_node->ui2_next;  
                    
                    /* Dis-attach the ES node from Service */
                    pt_curr_es_node->ui2_service = CA_NULL_IDX; 
                    pt_curr_es_node->ui2_next = CA_NULL_IDX; 
                    i4_return = CA_OK;            
                }
                else
                {    
                    pt_last_es_node = pt_curr_es_node;
                    ui2_curr_index = pt_curr_es_node->ui2_next;   
                    pt_curr_es_node = get_es_node_from_idx (ui2_curr_index);
                    if (pt_curr_es_node == NULL)
                    {
                        return CA_FAILED;
                    }

                    while(ui2_curr_index != CA_NULL_IDX)
                    {
                         if (ui2_curr_index == ui2_esnode_index)
                         {
                            /* Remove the ES node from the Service's ES list */
                            pt_last_es_node->ui2_next = pt_curr_es_node->ui2_next;
                            
                            /* Dis-attach the ES node from Service */
                            pt_curr_es_node->ui2_service = CA_NULL_IDX;
                            pt_curr_es_node->ui2_next = CA_NULL_IDX;
                            i4_return = CA_OK;                         
                            break;
                         }      
                         else
                         {
                            /* Fix CR 13810 */
                            pt_last_es_node = pt_curr_es_node;
                            ui2_curr_index = pt_curr_es_node->ui2_next;   
                            pt_curr_es_node = get_es_node_from_idx (ui2_curr_index);
                            if (pt_curr_es_node == NULL)
                            {
                                return CA_FAILED;
                            }
                         }          
                    }                  
                }
            }            
            else
                i4_return = CA_OK;  
        }        
    }
    
    return (i4_return);    
}

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_db_parse_service
 *
 * Description: This API applies the specified parser function to each ESDB node 
 *              of the specified SVDB node 
 *
 * Inputs:  pt_svdb_node     Reference to SVDB node.
 *          pv_tag           Contains the option argument.
 *          pf_cadb_parser   Contains the reference to the parser function. 
 * Outputs: -
 *
 * Returns: CA_OK           Success
 *          CA_NOT_INIT     DB is not initialized yet.
 *          CA_INV_ARG      Invalid arguments.
 ----------------------------------------------------------------------------*/
INT32 pmi_freeca_db_parse_service(CADB_SV_OBJ_T* pt_svdb_node, void *pv_tag, cadb_parser_fct pf_cadb_parser)
{
    INT32        i4_return;
    UINT16       ui2_curr_index;    
    CADB_ES_OBJ_T* pt_curr_es_node;
        
    i4_return = CA_NOT_INIT;
    
    if (b_cadb_init)
    {
        i4_return = CA_INV_ARG;
        
        if ((pt_svdb_node != NULL) && (get_index_from_sv_node(pt_svdb_node) != CA_NULL_IDX) && (pf_cadb_parser != NULL))
        {   
            ui2_curr_index = pt_svdb_node->ui2_node_head;                             
            while(ui2_curr_index != CA_NULL_IDX)
            {
                pt_curr_es_node = get_es_node_from_idx (ui2_curr_index);
                if (pt_curr_es_node == NULL)
                {
                     return CA_FAILED;
                }
                
                /* Retrive the next ES node's index first */                
                ui2_curr_index = pt_curr_es_node->ui2_next;
                
                /* Apply the parser func. to ES node */
                pf_cadb_parser(pt_curr_es_node, pv_tag);                                   
            }      
            i4_return = CA_OK;      
        }        
    }    
    return (i4_return);    
}

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_db_get_service_from_index
 *
 * Description: This API returns the index of ESDB array by reference of ESDB 
 *              node. 
 *
 * Inputs:  ui2_h_idx        The index for SVDB array.                   
 * Outputs: ppt_svdb_node    Contains the reference to the pointer to the 
 *                           returned SVDB node.
 *
 * Returns: CA_OK           Success
 *          CA_NOT_INIT     DB is not initialized yet.
 *          CA_INV_ARG      Invalid arguments.
 ----------------------------------------------------------------------------*/
INT32 pmi_freeca_db_get_service_from_index(CADB_SV_OBJ_T** ppt_svdb_node, UINT16  ui2_h_idx)
{
    INT32        i4_return;
        
    i4_return = CA_NOT_INIT;
    
    if (b_cadb_init)
    {
        i4_return = CA_INV_ARG;
        
        if (ppt_svdb_node != NULL)
        {   
            i4_return = CA_FAILED;
            *ppt_svdb_node = get_sv_node_from_idx(ui2_h_idx);
            if (*ppt_svdb_node != NULL)
                i4_return = CA_OK;      
        }        
    }    
    return (i4_return);    
}

/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_db_get_service_from_es
 *
 * Description: This API returns the SVDB node from ESDB node. 
 *
 * Inputs:  pt_esdb_node     Contains the reference to ESDB node.                   
 * Outputs: ppt_svdb_node    Contains the reference to the pointer to the 
 *                           returned SVDB node.
 * Returns: CA_OK           Success
 *          CA_NOT_INIT     DB is not initialized yet.
 *          CA_INV_ARG      Invalid arguments.
 ----------------------------------------------------------------------------*/
INT32 pmi_freeca_db_get_service_from_es(CADB_SV_OBJ_T** ppt_svdb_node, CADB_ES_OBJ_T* pt_esdb_node)
{
    INT32        i4_return;
        
    i4_return = CA_NOT_INIT;
    
    if (b_cadb_init)
    {
        i4_return = CA_INV_ARG;
        
        if (ppt_svdb_node != NULL)
        {   
            i4_return = CA_FAILED;
            *ppt_svdb_node = get_sv_node_from_idx(CA_ESNODE_SERVICE_INDEX(pt_esdb_node));
            if (*ppt_svdb_node != NULL)
                i4_return = CA_OK;      
        }        
    }    
    return (i4_return);    
    
}
/*-----------------------------------------------------------------------------
 * Name: pmi_freeca_db_count_es
 *
 * Description: This API is used to count the ES list in service 
 *              node. 
 *
 * Inputs:  pt_svdb_node     Contains the reference to SVDB node.
 *                  
 * Outputs: pui2_es_count    Contains the returned count. 
 *
 * Returns: CA_OK           Success
 *          CA_NOT_INIT     DB is not initialized yet.
 *          CA_INV_ARG      Invalid arguments.
 ----------------------------------------------------------------------------*/
INT32 pmi_freeca_db_count_es(CADB_SV_OBJ_T* pt_svdb_node, UINT16  *pui2_es_count)
{
    UINT16               ui2_curr_index;    
    CADB_ES_OBJ_T* pt_curr_es_node;
    INT32                i4_return;
        
    i4_return = CA_NOT_INIT;
    
    if (b_cadb_init)
    {
        i4_return = CA_INV_ARG;
        
        if ((pt_svdb_node != NULL) && (get_index_from_sv_node(pt_svdb_node) != CA_NULL_IDX) && (pui2_es_count != NULL))
        {   
            *pui2_es_count = 0;
            ui2_curr_index = pt_svdb_node->ui2_node_head;  
                                       
            while(ui2_curr_index != CA_NULL_IDX)
            {
                pt_curr_es_node = get_es_node_from_idx (ui2_curr_index);
                if (pt_curr_es_node == NULL)
                {
                    return CA_FAILED;
                }

                *pui2_es_count = (*pui2_es_count) + ((UINT16)1);
                
                /* Retrive the next ES node's index */                
                ui2_curr_index = pt_curr_es_node->ui2_next;                                              
            }      
            i4_return = CA_OK;      
        }              
    }    
    
    return (i4_return);    
}




