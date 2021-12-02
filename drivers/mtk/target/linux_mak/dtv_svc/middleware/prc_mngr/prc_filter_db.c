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
 * $RCSfile: prc_filter_db.c,v $
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
#include "prc_mngr/prc_thread.h"

#if (PM_ENABLE_FILTER_CTRL)
#include "prc_mngr/prc_filter_db.h"
#endif

#if (PM_ENABLE_FILTER_CTRL)
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

 
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/* PM_ESDB */
static BOOL                     b_fliter_db_init = FALSE;       /* Tracks if PM_FILTERDB has been initialized. */  
static PM_FILTER_CTRL_OBJ_T*    pt_pm_filterdb_arr = NULL;      /* FILTERDB Data store */
static UINT16                   ui2_filterdb_num_total = 0;     /* Total number of PM_FILTERDB node. */
static UINT16                   ui2_filterdb_num_free = 0;      /* Number of free PM_FILTERDB node. */

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
 extern SELECT_T    pm_comp_sel_specific_used_one(
            DRV_TYPE_T          e_type,
            UINT16              ui2_id,
            VOID*               pv_comp_sel_data,
            const VOID*         pv_comp_data,
            SIZE_T              z_comp_data_len,
            UINT16              ui2_num_active,
            UINT16              ui2_num_wait,
            CTRL_TYPE_T         e_ctrl,
            BOOL                b_would_conflict ); 
                                               
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
VOID pmi_filter_db_nfy_fct(HANDLE_T   h_obj,
                           UINT8      ui1_port,
                           RM_COND_T  e_nfy_cond,
                           VOID*      pv_tag,
                           UINT32     ui4_data)
{
    /* ToDo */
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
    } 
    else
    {
        PM_DBG_ERROR("Invalid input for filter notification\n"); 
    }
}                                                 
                                               
INT32 pmi_filter_db_init(UINT16 ui2_max_filter_ctrl_size)
{
    INT32       i4_return;
    UINT16      i;
        
    if (b_fliter_db_init != TRUE)
    {        
        if ((pt_pm_filterdb_arr = (PM_FILTER_CTRL_OBJ_T*) x_mem_alloc (sizeof (PM_FILTER_CTRL_OBJ_T) * (ui2_max_filter_ctrl_size))) != NULL)
        {            
            b_fliter_db_init = TRUE;       
            
             /* Initialize  DB */
            ui2_filterdb_num_total = ui2_max_filter_ctrl_size;             
            ui2_filterdb_num_free = ui2_max_filter_ctrl_size; 
            
            for(i=0; i<ui2_filterdb_num_total; i++)
            {
                pt_pm_filterdb_arr[i].b_used = FALSE;
            }      
            i4_return = PMR_OK;
        }
        else
            i4_return = PMR_NOT_ENOUGH_MEM;
    }
    else
        i4_return = PMR_ALREADY_INIT;   
        
    return i4_return;    
}

PM_FILTER_CTRL_OBJ_T* pmi_filter_db_get_ctrl_by_idx (UINT16  ui2_idx)
{
    PM_FILTER_CTRL_OBJ_T* pt_filter_ctrl;

    pt_filter_ctrl = NULL;
    
    if (ui2_idx < ui2_filterdb_num_total)
    {
        pt_filter_ctrl  = &(pt_pm_filterdb_arr[ui2_idx]);
    }    
    else
    {
        PM_DBG_ERROR( "filter ctrl index is invaild\n" );
    }
    return (pt_filter_ctrl);
}

UINT16 pmi_filter_db_get_idx_by_ctrl( PM_FILTER_CTRL_OBJ_T* pt_filter_ctrl )
{
    UINT16    ui2_idx;
    
    ui2_idx = PM_NULL_IDX;
            
    if ((((UINT16)(pt_filter_ctrl - pt_pm_filterdb_arr)) < ui2_filterdb_num_total) && 
        pt_filter_ctrl >= pt_pm_filterdb_arr)
    {
        ui2_idx = (UINT16)(pt_filter_ctrl - pt_pm_filterdb_arr);
    }     
    else
    {
        PM_DBG_ERROR( "filter ctrl point is invaild\n" );
    }
    
    return ui2_idx;
}


INT32 pmi_filter_db_check(HANDLE_T h_src, DRV_TYPE_T t_filter_type, PM_COMPONENT_INFO_T t_esdb_node, UINT16* pui2_filter_id)
{
    UINT16      i;
    DRV_TYPE_T  e_type;
    UINT16      ui2_id;
    UINT8       ui1_num_inp_ports;
    UINT8       ui1_num_out_ports;
    UINT32      ui4_comp_flags;
    BOOL        b_found = FALSE; 
    
    switch (t_filter_type)
    {
        case DRVT_DEMUX_TS_PES_PACKET:
            break;
        default:
            return PMR_NOT_SURPPORT;
    }
    
    if (rm_get_comp_info_from_handle (h_src, &e_type, &ui2_id, &ui1_num_inp_ports, &ui1_num_out_ports, &ui4_comp_flags) == RMR_OK)
    {         
        b_found = FALSE;
        for(i=0; ((i<ui2_filterdb_num_total) && (b_found == FALSE)); i++)
        {
            if (pt_pm_filterdb_arr[i].b_used == TRUE)
            {
                if ((pt_pm_filterdb_arr[i].e_src_type == e_type) &&
                    (pt_pm_filterdb_arr[i].ui2_src_id == ui2_id) &&
                    (pt_pm_filterdb_arr[i].e_filter_type == t_filter_type))
                {
                    switch (t_filter_type)
                    {
                        case DRVT_DEMUX_TS_PES_PACKET:
                            if ((pt_pm_filterdb_arr[i].t_pid == ESNODE_PID(&t_esdb_node)) &&
                                (pt_pm_filterdb_arr[i].e_stream_type == ESNODE_DESCR_TYPE(&t_esdb_node)))
                            {
                                *pui2_filter_id = pt_pm_filterdb_arr[i].ui2_filter_id;
                                b_found = TRUE;
                            }                                
                            break;
                        default:                            
                            break;
                    }
                }               
                    
            }
        }
        
        if (b_found == FALSE)
        {
            *pui2_filter_id = ANY_COMP_ID;
        }
            
        return PMR_OK;
    }
    else
    {
        return PMR_FAILED;
    }
}

INT32 pmi_filter_db_add(HANDLE_T h_src, DRV_TYPE_T t_filter_type, PM_COMPONENT_INFO_T* pt_esdb_node)
{
    
    UINT16      i;
    DRV_TYPE_T  e_type;
    UINT16      ui2_id;
    UINT8       ui1_num_inp_ports;
    UINT8       ui1_num_out_ports;
    UINT32      ui4_comp_flags;
    BOOL        b_found = FALSE;    
    PM_FILTER_CTRL_OBJ_T* pt_filter_ctrl = NULL;     
    INT32       i4_rt_val;   
    
    switch (t_filter_type)
    {
        case DRVT_DEMUX_TS_PES_PACKET:
            break;
        default:
            return PMR_NOT_SURPPORT;
    }    

    
    if (rm_get_comp_info_from_handle (h_src, &e_type, &ui2_id, &ui1_num_inp_ports, &ui1_num_out_ports, &ui4_comp_flags) == RMR_OK)
    {   
        /* Search existed one first */      
        b_found = FALSE;
        for(i=0; ((i<ui2_filterdb_num_total) && (b_found == FALSE)); i++)
        {
            if (pt_pm_filterdb_arr[i].b_used == TRUE)
            {
                if ((pt_pm_filterdb_arr[i].e_src_type == e_type) &&
                    (pt_pm_filterdb_arr[i].ui2_src_id == ui2_id) &&
                    (pt_pm_filterdb_arr[i].e_filter_type == t_filter_type))
                {
                    switch (t_filter_type)
                    {
                        case DRVT_DEMUX_TS_PES_PACKET:
                            if ((pt_pm_filterdb_arr[i].t_pid == ESNODE_PID(pt_esdb_node)) && 
                                (pt_pm_filterdb_arr[i].e_stream_type == ESNODE_DESCR_TYPE(pt_esdb_node)))
                            {
                                pt_filter_ctrl = &(pt_pm_filterdb_arr[i]);
                                b_found = TRUE;
                            }                                
                            break;
                        default:                            
                            break;
                    }
                }                                   
            }
        }
        
        if (b_found == FALSE)
        {            
            /* Allocate a new one */
            if (ui2_filterdb_num_free == 0)
                return PMR_FAILED;
                
            for(i=0; i<ui2_filterdb_num_total; i++)
            {
                if (pt_pm_filterdb_arr[i].b_used == FALSE)
                {  
                    pt_pm_filterdb_arr[i].b_used = TRUE;
                    pt_pm_filterdb_arr[i].t_info_valid_mask = PM_FILTER_INFO_ALL_INVALID;
                    pt_pm_filterdb_arr[i].ui2_reference_count = 1;
                    pt_pm_filterdb_arr[i].e_src_type = e_type;
                    pt_pm_filterdb_arr[i].ui2_src_id = ui2_id;
                    
                    pt_pm_filterdb_arr[i].e_filter_type = t_filter_type;                    
                    rm_get_comp_info_from_handle (ESNODE_FILTER_HANDLE(pt_esdb_node), &e_type, &ui2_id, &ui1_num_inp_ports, &ui1_num_out_ports, &ui4_comp_flags) ;                    
                    pt_pm_filterdb_arr[i].ui2_filter_id     = ui2_id;
                    pt_pm_filterdb_arr[i].h_filter          = NULL_HANDLE;
                    pt_pm_filterdb_arr[i].t_cond            = PM_CA_COND_UNKNOWN;
                    pt_pm_filterdb_arr[i].t_last_cond       = PM_CA_COND_UNKNOWN;
                    pt_pm_filterdb_arr[i].ui4_last_cond_stamp = 0;
                    pt_pm_filterdb_arr[i].ui2_time_nfy_mark = 0;

                    pt_pm_filterdb_arr[i].e_desc_type       = DRVT_DESCRAMBLER;
                    pt_pm_filterdb_arr[i].ui2_desc_id       = 0;
                    pt_pm_filterdb_arr[i].ui2_desc_counter  = 0;
                    
                    /* Open filter for connection control */
                    i4_rt_val = rm_open_comp(h_src, OBJ_COMP_REL_SRC, t_filter_type, pt_pm_filterdb_arr[i].ui2_filter_id, 
                                         NULL, CTRL_TYPE_SHARED, NULL, pm_comp_sel_specific_used_one, &(pt_pm_filterdb_arr[i]), pmi_filter_db_nfy_fct, 
                                         &(pt_pm_filterdb_arr[i].h_filter), &(pt_pm_filterdb_arr[i].t_filter_cond));     
                                         
                    if ((i4_rt_val == RMR_OK) || (i4_rt_val == RMR_ARBITRATION))
                    {                                                            
                        i4_rt_val = rm_connect(pt_pm_filterdb_arr[i].h_filter, RM_CONN_TYPE_HANDLE, 
                                               (const void *)(h_src), sizeof(HANDLE_T), 
                                               &(pt_pm_filterdb_arr[i]),
                                               &(pt_pm_filterdb_arr[i].t_filter_cond));  
                                               
                        if ((i4_rt_val == RMR_OK) || (i4_rt_val == RMR_ARBITRATION))             
                        {
                            pt_pm_filterdb_arr[i].pt_head = pt_esdb_node;    
                            pt_pm_filterdb_arr[i].e_stream_type = ESNODE_DESCR_TYPE(pt_esdb_node);
                            
                            ESNODE_FILTER_OBJ_CTRL(pt_esdb_node) = &(pt_pm_filterdb_arr[i]);
                            ESNODE_FILTER_OBJ_NEXT(pt_esdb_node) = NULL;
                            
                            ui2_filterdb_num_free--;
                            return PMR_OK;                             
                        }
                        else
                        {
                            rm_close(pt_pm_filterdb_arr[i].h_filter, TRUE, &(pt_pm_filterdb_arr[i].t_filter_cond));
                            pt_pm_filterdb_arr[i].b_used = FALSE;
                            return PMR_FAILED; 
                        }
                    }
                    else
                    {
                        pt_pm_filterdb_arr[i].b_used = FALSE;
                        return PMR_FAILED; 
                    }
                }
            }  
            return PMR_FAILED; 
            
        }
        else
        {
            /* Insert it to existed one */            
            ESNODE_FILTER_OBJ_CTRL(pt_esdb_node) = pt_filter_ctrl;
            
            /* insert this ES to filer control object */
            pt_filter_ctrl->ui2_reference_count++;
            ESNODE_FILTER_OBJ_NEXT(pt_esdb_node) = pt_filter_ctrl->pt_head;
            pt_filter_ctrl->pt_head = pt_esdb_node;       
            
            return PMR_OK;     
        } 
    }
    else
    {
        return PMR_FAILED;
    }           
} 

INT32 pmi_filter_db_remove(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    PM_FILTER_CTRL_OBJ_T* pt_filter_ctrl = NULL; 
    PM_COMPONENT_INFO_T* pt_esdb_prev = NULL;
    PM_COMPONENT_INFO_T* pt_esdb_curr = NULL;
    UINT16 ui2_check_count;
    BOOL b_found = FALSE;
    INT32       i4_rm_ret; 
    
    pt_filter_ctrl = ESNODE_FILTER_OBJ_CTRL(pt_esdb_node);
    
    if (pt_filter_ctrl == NULL)
        return PMR_FAILED;
    
    if (pt_filter_ctrl->pt_head == pt_esdb_node)
    {
        pt_filter_ctrl->pt_head = ESNODE_FILTER_OBJ_NEXT(pt_esdb_node);
        pt_filter_ctrl->ui2_reference_count--;
        
        ESNODE_FILTER_OBJ_CTRL(pt_esdb_node) = NULL;
        ESNODE_FILTER_OBJ_NEXT(pt_esdb_node) = NULL;
        
        b_found = TRUE;
    }
    else
    {
        pt_esdb_prev = pt_filter_ctrl->pt_head;
        pt_esdb_curr = ESNODE_FILTER_OBJ_NEXT(pt_filter_ctrl->pt_head);
        ui2_check_count = 1;
        b_found = FALSE;
        
        while((pt_esdb_curr != NULL)&&
              (ui2_check_count < pt_filter_ctrl->ui2_reference_count)&&
              (b_found == FALSE))
        {        
            if (pt_esdb_curr == pt_esdb_node)
            {
                b_found = TRUE;
                ESNODE_FILTER_OBJ_NEXT(pt_esdb_prev) = ESNODE_FILTER_OBJ_NEXT(pt_esdb_node);
                pt_filter_ctrl->ui2_reference_count--;
                ui2_check_count++;
                
                ESNODE_FILTER_OBJ_CTRL(pt_esdb_node) = NULL;
                ESNODE_FILTER_OBJ_NEXT(pt_esdb_node) = NULL;
            } 
            else
            {
                pt_esdb_prev = pt_esdb_curr;
                pt_esdb_curr = ESNODE_FILTER_OBJ_NEXT(pt_esdb_curr);
                ui2_check_count++;                
            }
        }        
       
    }
    
    if (b_found ==  TRUE)
    {
        if (pt_filter_ctrl->ui2_reference_count == 0)
        {
            /* Free HW resource */
            i4_rm_ret = rm_disconnect(pt_filter_ctrl->h_filter, RM_DISC_TYPE_IGNORE, NULL, 0, 
                                      &(pt_filter_ctrl->t_filter_cond)); 
                                      
            if (i4_rm_ret == RMR_OK)
            { 
                i4_rm_ret = rm_close(pt_filter_ctrl->h_filter, TRUE, &(pt_filter_ctrl->t_filter_cond));                                  
                
                if (i4_rm_ret == RMR_OK)
                {                
                    /* Free the filter control */
                    pt_filter_ctrl->b_used = FALSE;
                    ui2_filterdb_num_free++;
                    return PMR_OK;   
                }  
                else
                {
                    return PMR_FAILED;               
                }
            }      
            else
            {
                return PMR_FAILED;           
            }
        }
        else
            return PMR_OK; 
    }
    else        
        return PMR_FAILED; 
}
INT32 pmi_filter_db_execute_play(PM_COMPONENT_INFO_T* pt_esdb_node, BOOL* pb_execute)
{
    PM_FILTER_CTRL_OBJ_T* pt_filter_ctrl = NULL; 
    
    pt_filter_ctrl = ESNODE_FILTER_OBJ_CTRL(pt_esdb_node);
    if (pt_filter_ctrl->e_filter_status == PM_COND_PLAYED)
    {
        *pb_execute = FALSE;
    return PMR_OK;
    }
    else
    {
        *pb_execute = TRUE;
        return PMR_OK;          
    }   

}
INT32 pmi_filter_db_execute_stop(PM_COMPONENT_INFO_T* pt_esdb_node, BOOL* pb_execute)
{
    PM_FILTER_CTRL_OBJ_T* pt_filter_ctrl = NULL; 
    PM_COMPONENT_INFO_T* pt_esdb_curr = NULL;
    UINT16 ui2_check_count, ui2_stop_count;    
    
    pt_filter_ctrl = ESNODE_FILTER_OBJ_CTRL(pt_esdb_node);
    if (pt_filter_ctrl->e_filter_status == PM_COND_STOPPED)
    {
        *pb_execute = FALSE;
        return PMR_OK;        
    }
    
    pt_esdb_curr = pt_filter_ctrl->pt_head;
    ui2_check_count = 0;
    ui2_stop_count = 0;
    
    while((pt_esdb_curr != NULL)&&(ui2_check_count < pt_filter_ctrl->ui2_reference_count))
    {   
        if (ESNODE_FILTER_STATUS(pt_esdb_curr) != PM_COND_PLAYED)     
        {
            ui2_stop_count++;
        }
        ui2_check_count++;
        pt_esdb_curr = ESNODE_FILTER_OBJ_NEXT(pt_esdb_curr);             
    }     
    
    if (ui2_stop_count == pt_filter_ctrl->ui2_reference_count - ((UINT16)1))
        *pb_execute = TRUE;     
    else   
        *pb_execute = FALSE;
    
    return PMR_OK;
}

INT32 pmi_filter_db_execute_disconnect(PM_COMPONENT_INFO_T* pt_esdb_node, BOOL* pb_execute)
{
    PM_FILTER_CTRL_OBJ_T* pt_filter_ctrl = NULL;       
    
    pt_filter_ctrl = ESNODE_FILTER_OBJ_CTRL(pt_esdb_node);
    if ((pt_filter_ctrl->e_filter_status == PM_COND_STOPPED)&&(pt_filter_ctrl->ui2_reference_count == 1))
    {
        *pb_execute = TRUE;
        return PMR_OK;        
    } 
    else
    {
        *pb_execute = FALSE;
        return PMR_OK;           
    }
}
INT32 pmi_filter_db_ctrl_info_update(PM_COMPONENT_INFO_T* pt_esdb_node, PM_FILTER_INFO_MASK_T t_update_field)
{
    PM_FILTER_CTRL_OBJ_T* pt_filter_ctrl = NULL; 
    
    pt_filter_ctrl = ESNODE_FILTER_OBJ_CTRL(pt_esdb_node);
    
    if ((t_update_field & PM_FILTER_INFO_UPDATE_PID) != 0)
    {
        pt_filter_ctrl->t_pid = ESNODE_PID(pt_esdb_node);
        pt_filter_ctrl->t_info_valid_mask = (pt_filter_ctrl->t_info_valid_mask | PM_FILTER_INFO_UPDATE_PID);
    }
    
    if ((t_update_field & PM_FILTER_INFO_UPDATE_DEST_ID) != 0)
    {
        pt_filter_ctrl->t_dest_id = ESNODE_SINK_INFO(pt_esdb_node);
        pt_filter_ctrl->t_info_valid_mask = (pt_filter_ctrl->t_info_valid_mask | PM_FILTER_INFO_UPDATE_DEST_ID);
    }   
    
    if ((t_update_field & PM_FILTER_INFO_UPDATE_STATUS) != 0)
    {
        switch (ESNODE_STATE(pt_esdb_node))
        {
            case PM_ES_STATE_PLAYED:
                pt_filter_ctrl->e_filter_status = PM_COND_PLAYED;
                pt_filter_ctrl->t_info_valid_mask = (pt_filter_ctrl->t_info_valid_mask | PM_FILTER_INFO_UPDATE_STATUS);
                break;
            case PM_ES_STATE_STOPPED:
                pt_filter_ctrl->e_filter_status = PM_COND_STOPPED;
                pt_filter_ctrl->t_info_valid_mask = (pt_filter_ctrl->t_info_valid_mask | PM_FILTER_INFO_UPDATE_STATUS);
                break;
            case PM_ES_STATE_OPENED:
                pt_filter_ctrl->e_filter_status = PM_COND_STOPPED;
                pt_filter_ctrl->t_info_valid_mask = (pt_filter_ctrl->t_info_valid_mask | PM_FILTER_INFO_UPDATE_STATUS);
                break;
            default:
                /* ToDo */
                break;
        }      
    }  
    
    return PMR_OK;
}


INT32 pmi_filter_db_get_desc_info(
                PM_COMPONENT_INFO_T*            pt_esdb_node, 
                DRV_TYPE_T*                     pe_desc_type,
                UINT16*                         pui2_desc_id,
                UINT16*                         pui2_desc_counter )
{
    PM_FILTER_CTRL_OBJ_T* pt_filter_ctrl = NULL;
        
    pt_filter_ctrl      = ESNODE_FILTER_OBJ_CTRL(pt_esdb_node);
    if ( NULL == pt_filter_ctrl )
    {
        return PMR_NOT_SURPPORT;
    }
    
    *pe_desc_type       = pt_filter_ctrl->e_desc_type;
    *pui2_desc_id       = pt_filter_ctrl->ui2_desc_id;
    *pui2_desc_counter  = pt_filter_ctrl->ui2_desc_counter;
    
    return PMR_OK;
}


INT32 pmi_filter_db_set_desc_info(
                PM_COMPONENT_INFO_T*            pt_esdb_node, 
                DRV_TYPE_T                      e_desc_type,
                UINT16                          ui2_desc_id,
                UINT16                          ui2_desc_counter )
{
    PM_FILTER_CTRL_OBJ_T* pt_filter_ctrl = NULL; 
        
    pt_filter_ctrl = ESNODE_FILTER_OBJ_CTRL(pt_esdb_node);
    if ( NULL == pt_filter_ctrl )
    {
        return PMR_NOT_SURPPORT;
    }
    
    pt_filter_ctrl->e_desc_type      = e_desc_type;
    pt_filter_ctrl->ui2_desc_id      = ui2_desc_id;
    pt_filter_ctrl->ui2_desc_counter = ui2_desc_counter;
    
    return PMR_OK;
}

#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: pmi_filter_db_resource_statistics_4cli
 *
 * Description: This API dump db statistics.
 *
 * Inputs:  -    
 * Outputs: -
 *
 * Returns: PMR_OK           Success
 *----------------------------------------------------------------------------*/
INT32 pmi_filter_db_resource_statistics_4cli(VOID)
{    
    x_dbg_stmt("PID Filter Control Total:%d Free:%d\n", ui2_filterdb_num_total, ui2_filterdb_num_free);
    
    return PMR_OK;
}
#endif

#endif /* #if (PM_ENABLE_FILTER_CTRL) */
