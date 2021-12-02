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
 * $RCSfile: prc_thread.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: Felisa Hsiao $
 * $MD5HEX: 5be8fc53e8b648733c3373dcdf748f12 $
 *
 * Description: 
 *         This file contains the Process Manager thread's implemenattion and 
 *         exported APIs. 
 *         The major task is to handle the notifiaction from other modules.
 * History:
 *                
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "prc_mngr/prc_mngr.h"
#include "prc_mngr/prc_common.h"
#include "prc_mngr/prc_thread.h"
#include "prc_mngr/prc_db.h"
#include "prc_mngr/prc_scdb.h"
#include "prc_mngr/prc_filter.h"
#include "prc_mngr/prc_lock.h"
#include "prc_mngr/prc_util.h"
#include "prc_mngr/prc_ca.h"
#include "prc_mngr/prc_desc.h"
#include "prc_mngr/prc_debug.h"
#include "prc_mngr/prc_filter_db.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define MSG_RETRY_COUNT  10
#define MSG_RETRY_DELAY  ((UINT32) 100)

typedef VOID (*prc_fct) (HANDLE_T  h_obj,
                         UINT32    ui4_data_1,
                         UINT32    ui4_data_2);

typedef struct _MSG_T
{
    prc_fct  prc;

    HANDLE_T  h_obj;
    UINT32    ui4_data_1;
    UINT32    ui4_data_2;
}   MSG_T;

 
typedef struct  _PM_TIMER_REC_TAG_T
{
    VOID*                       pv_tag1;
    VOID*                       pv_tag2;
    VOID*                       pv_tag3;
    VOID*                       pv_tag4;
    VOID*                       pv_tag5;
}   PM_TIMER_REC_TAG_T;


typedef INT32 ( * PMI_SUPER_TIME_NFY)( PM_TIMER_REC_TAG_T* pt_tag );


typedef struct  _PM_TIMER_REC_T
{
    UINT32                      ui4_timestamp;
    UINT32                      ui4_duration;

    UINT32                      ui4_timer_id;
    PMI_SUPER_TIME_NFY          pf_time_nfy;
    PM_TIMER_REC_TAG_T          t_tag;
    
    struct _PM_TIMER_REC_T*     pt_next;
}   PM_TIMER_REC_T;


typedef struct  _PM_SUPER_TIMER_T
{
    HANDLE_T                    h_super_timer;
    UINT32                      ui4_tick_open;
    PM_TIMER_REC_T*             pt_head_rec;
}   PM_SUPER_TIMER_T;


#define PM_SCRS_DELAY       ((UINT32)900)
static UINT32               ui4_g_pmi_super_timer_id;
static PM_SUPER_TIMER_T     t_g_pm_super_timer;
static UINT16               ui2_super_timer_mark;
static UINT32               ui4_pm_sys_tick_period = ((UINT32)5);

#ifdef MW_COMHEM_DELAY_SCRAMBLE_NOTIFY
/* when bootup, comhem logo does not want scramble notify to be seen by user. 
Need to ignore two scramble notify when bootup.
*/
HANDLE_T  h_boot_timer = NULL_HANDLE;
static BOOL b_1st_bootup = TRUE;
#define PM_BOOT_NOTIFY_DELAY       ((UINT32)38000)  /* 38 sec */
#endif
 
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static HANDLE_T h_thread;   /* Thread. */
static HANDLE_T h_msg_q;    /* Message queue. */ 

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern HP_TYPE_T pm_es_filter_feedback_fct (UINT16         ui2_count,
                         UINT16         ui2_max_count,
                         HANDLE_T       h_handle,
                         HANDLE_TYPE_T  e_type,
                         VOID*          pv_obj,
                         VOID*          pv_tag,
                         VOID*          pv_parse_data);
extern HP_TYPE_T pm_es_ca_feedback_fct (UINT16         ui2_count,
                         UINT16         ui2_max_count,
                         HANDLE_T       h_handle,
                         HANDLE_TYPE_T  e_type,
                         VOID*          pv_obj,
                         VOID*          pv_tag,
                         VOID*          pv_parse_data);                         
extern HP_TYPE_T pm_es_cp_feedback_fct (UINT16         ui2_count,
                         UINT16         ui2_max_count,
                         HANDLE_T       h_handle,
                         HANDLE_TYPE_T  e_type,
                         VOID*          pv_obj,
                         VOID*          pv_tag,
                         VOID*          pv_parse_data);
static VOID send_msg (MSG_T*  pt_msg);
static VOID thread_main_fct (VOID*  pv_argv);
static VOID pmi_thread_filter_opened_op(HANDLE_T h_filter, UINT32 argv1, UINT32 reserved);
static VOID pmi_thread_filter_closed_op(HANDLE_T h_filter, UINT32 argv1, UINT32 reserved);
static VOID pmi_thread_desc_opened_op(HANDLE_T h_desc, UINT32 argv1, UINT32 reserved);
static VOID pmi_thread_desc_closed_op(HANDLE_T h_desc, UINT32 argv1, UINT32 reserved);

#if PM_ENABLE_CA
static VOID pmi_thread_cp_status_updated_op(HANDLE_T h_ca, UINT32 argv1, UINT32 reserved);
#endif

#if (PM_ENABLE_SCRAMBLE_DETECTION)
//extern VOID pmi_thread_scramble_status_updated_op(HANDLE_T h_filter, UINT32 argv1, UINT32 reserved);
VOID pmi_super_timer_hdlr(HANDLE_T h_obj, UINT32 ui4_data_1, UINT32 ui4_data_2);
#endif


/*-----------------------------------------------------------------------------
 * Name: pmi_thread_filter_opened_op
 *
 * Description: This API is responsible to execute the related operations for 
 *              the opened filter.
 *
 * Inputs:  h_filter         Contains the filter handle.
 *          argv1            Contains the reference to ESDB node.
 *          reserved         Ignored
 * Outputs: argv1            Contains the reference to ESDB node which includes 
 *                           the updated status.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/     
static VOID pmi_thread_filter_opened_op(HANDLE_T h_filter, UINT32 argv1, UINT32 reserved)
{
    PM_COMPONENT_INFO_T* pt_esdb_node;
    PM_SERVICE_INFO_T*   pt_svdb_node; 
    PM_PROCESSING_TYPE_T e_prc_type;
    STREAM_TYPE_T        e_stream_type;
    DRV_SET_TYPE_T          e_set_type;
    RM_COND_T            t_cond;
    PM_ES_STATE_T        e_last_request_state;
    
    pt_esdb_node = (PM_COMPONENT_INFO_T*)argv1;
    
    if ((h_filter == NULL_HANDLE) || (pt_esdb_node == NULL))
    {
        PM_DBG_ERROR("Invalid input for filter open\n");
        return;
    }
        
    pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_esdb_node));
    if (pt_svdb_node != NULL)
    {          
        e_prc_type = SVNODE_PROC_TYPE(pt_svdb_node);
        e_last_request_state = ESNODE_STATE(pt_esdb_node);
        switch (e_prc_type)
        {
            case PM_PRESENT:
            case PM_DETECTION:
        #if (PM_ENABLE_RECORD)
            case PM_RECORDING:
        #endif
            #if (!PM_AUTO_CONNECT_BY_RM) 
                /* Connect to HW component */                       
                if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_CONNECT) == TRUE) &&
                    (pmi_filter_connect(pt_svdb_node, pt_esdb_node) == PMR_OK))
                {                                        
                    t_cond = rm_cond_chg(ESNODE_FILTER_COND(pt_esdb_node), RM_COND_IGNORE, COND_GRP_CONN_STATE);                                        
                    switch (t_cond)
                    {
                        case RM_COND_CONNECTED:  
                            ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_DEFAULT_STATE;
                            break;                                                
                        case RM_COND_CONNECTING:
                            ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_OPENING;
                            PM_DBG_INFO("ES: pid :0x%x status: %d\n", ESNODE_PID(pt_esdb_node), ESNODE_FILTER_STATUS(pt_esdb_node));
                            return;
                        default:
                            PM_DBG_ERROR("ES: pid :0x%x invalid connection condition: %d\n", ESNODE_PID(pt_esdb_node), t_cond);
                            PM_ABORT(DBG_CAT_INV_OP, PM_DBG_ABRT_INVALID_CONNECTION_COND);
                            break;
                    }
                    ESNODE_STATE(pt_esdb_node) = e_last_request_state;
                }                                    
            #endif /* (!PM_AUTO_CONNECT_BY_RM) */
            
                e_stream_type = ESNODE_DESCR_TYPE(pt_esdb_node);
                e_set_type    = DEMUX_SET_TYPE_PID;

                #if (PM_ENABLE_RECORD)
                if (PM_RECORDING == e_prc_type)
                {
                    /* when it is recording type e_set_type should be RECDEMUX_SET_TYPE_PID */
                    /* if it WI_SETTING, we will ingore stream type */
                    /* because there is not any stream type when adding component */
                    if (PM_RECORDING_ES_WI_SETTING == ESNODE_REC_REQUEST_TYPE(pt_esdb_node))
                    {
                        e_stream_type = ST_VIDEO;
                    }
                    
                    if (ST_AUDIO != e_stream_type && ST_VIDEO != e_stream_type)
                    {
                        e_stream_type = ST_UNKNOWN;
                    }

                    e_set_type = RECDEMUX_SET_TYPE_PID;
                }
                #endif
            
                /* Set PID */
                switch (e_stream_type)
                {
                    case ST_AUDIO:
                    case ST_VIDEO:
                        pmi_dbg_time_measure_begin("[PRC]rm_set(SET_TYPE_PID)");
                        
                        if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0 , e_set_type, 
                               (const void *)((UINT32)ESNODE_PID(pt_esdb_node)), sizeof(MPEG_2_PID_T), &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK)
                        {                 
                            pmi_dbg_time_measure_end("[PRC]rm_set(SET_TYPE_PID)");                                        
                            ESNODE_STATE(pt_esdb_node) = e_last_request_state;
                        }
                        else
                        {
                            pmi_dbg_time_measure_end("[PRC]rm_set(SET_TYPE_PID)");
                            PM_DBG_ERROR("F[%5d] set PID failed\n", ESNODE_PID(pt_esdb_node));
                        }
                        break;
                #if (PM_ALLOCATE_PCR)
                    case ST_PCR:
                        pmi_dbg_time_measure_begin("[PRC]rm_set(SET_TYPE_PID)");
                        
                        if (rm_set(ESNODE_FILTER_HANDLE(pt_esdb_node), FALSE, 0 , e_set_type, 
                               ESNODE_PID(pt_esdb_node), sizeof(MPEG_2_PID_T), &(ESNODE_FILTER_COND(pt_esdb_node))) == RMR_OK)
                        {                            
                            pmi_dbg_time_measure_end("[PRC]rm_set(SET_TYPE_PID)");
                            ESNODE_STATE(pt_esdb_node) = e_last_request_state;
                        }
                        else
                        {
                            pmi_dbg_time_measure_end("[PRC]rm_set(SET_TYPE_PID)");
                            PM_DBG_ERROR("F[%5d] set PID failed\n", ESNODE_PID(pt_esdb_node));
                        }
                        break;
                #endif
                    default:
                        PM_DBG_ERROR("Unsupported stream type: %d\n", e_stream_type);
                        break;
                }
                
                /* Play */
                if (ESNODE_STATE(pt_esdb_node) == PM_ES_STATE_WAIT_PLAY)
                {
                    if ((pmi_valid_request(pt_esdb_node, PM_ES_STATE_WAIT_PLAY) == TRUE) &&
                        (pmi_filter_enable(pt_esdb_node) == PMR_OK))
                    {
                        #if (PM_ENABLE_RECORD)
                            if (PM_RECORDING == e_prc_type)
                            {
                                ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_PLAYED;
                                t_cond = PM_COND_PLAYED;
                                
                                /* Call Notify Function */
                                handle_parse(&(ESNODE_LINK(pt_esdb_node)), pm_es_filter_feedback_fct, &t_cond);
                            }
                            else
                        #endif
                            {
                    #if (PM_ENABLE_CA == 0)
                        ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_PLAYED;
                        t_cond = PM_COND_PLAYED;
                        
                        /* Call Notify Function */
                        handle_parse(&(ESNODE_LINK(pt_esdb_node)), pm_es_filter_feedback_fct, &t_cond);
                    #else
                        /* ToDo */
                    #endif
                    }
                }
                }
                else
                {
                    ESNODE_STATE(pt_esdb_node) = PM_ES_STATE_STOPPED;
                    ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_STOPPED;                    
                    t_cond = PM_COND_STOPPED; 
                    
                    /* Call Notify Function */
                    handle_parse(&(ESNODE_LINK(pt_esdb_node)), pm_es_filter_feedback_fct, &t_cond);                    
                }                 
                break;
            default:
                break;
        }   
        
        PM_DBG_INFO("ES: pid %0x%x filter status: %d\n", ESNODE_PID(pt_esdb_node), ESNODE_FILTER_STATUS(pt_esdb_node));
    }
    else
    {
        PM_DBG_ERROR("Invalid service\n");
    }
}  

/*-----------------------------------------------------------------------------
 * Name: pmi_thread_filter_closed_op
 *
 * Description: This API is responsible to execute the related operations for 
 *              the closed filter.
 *
 * Inputs:  h_filter         Contains the filter handle.
 *          argv1            Contains the reference to ESDB node.
 *          reserved         Ignored
 * Outputs: argv1            Contains the reference to ESDB node which includes 
 *                           the updated status.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/     
static VOID pmi_thread_filter_closed_op(HANDLE_T h_filter, UINT32 argv1, UINT32 reserved)
{
    PM_COMPONENT_INFO_T* pt_esdb_node;
    PM_SERVICE_INFO_T*   pt_svdb_node; 
    PM_COND_T            t_cond;
    
    pt_esdb_node = (PM_COMPONENT_INFO_T*)argv1;
    
    if ((h_filter == NULL_HANDLE) || (pt_esdb_node == NULL))
    {
        PM_DBG_ERROR("Invalid input for filter close\n");
        return;
    }
        
    pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_esdb_node));
    if (pt_svdb_node != NULL)
    { 
        /* Remove from Service */
        pmi_db_remove_from_service(pt_esdb_node);
        
        /* Release SCDB query */
        pmi_scdb_release(pt_esdb_node);
        
        /* free all HW resource */   
          
        /* ToDo: Free CA, CP related HW resource */   
        
        /* Update status */
        ESNODE_FILTER_STATUS(pt_esdb_node) = PM_COND_CLOSED;                    
        t_cond = PM_COND_CLOSED; 
                        
        /* Call Notify Function */
        handle_parse(&(ESNODE_LINK(pt_esdb_node)), pm_es_filter_feedback_fct, &t_cond);
        
        /* Delink and Free Handle */
        handle_free_all(&(ESNODE_LINK(pt_esdb_node))); 
        
        /* Free ES node */
        pmi_esdb_free(pt_esdb_node);         
          
    }
    else
    {
        PM_DBG_ERROR("Invalid service\n");
    }
}      
/*-----------------------------------------------------------------------------
 * Name: pmi_thread_desc_opened_op
 *
 * Description: This API is responsible to execute the related operations for 
 *              the opened descrambler.
 *
 * Inputs:  h_desc         Contains the filter handle.
 *          argv1            Contains the reference to ESDB node.
 *          reserved         Ignored
 * Outputs: argv1            Contains the reference to ESDB node which includes 
 *                           the updated status.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/     
static VOID pmi_thread_desc_opened_op(HANDLE_T h_desc, UINT32 argv1, UINT32 reserved)
{
    PM_COMPONENT_INFO_T* pt_esdb_node;
    
    pt_esdb_node = (PM_COMPONENT_INFO_T*)argv1;
    
    if ((h_desc == NULL_HANDLE) || (pt_esdb_node == NULL))
    {
        PM_DBG_ERROR("Invalid input for descrambler open\n");
        return;
    }
    
    /* ToDo: connect descrambler to pes filter, set PID and enable it */
}  

/*-----------------------------------------------------------------------------
 * Name: pmi_thread_desc_closed_op
 *
 * Description: This API is responsible to execute the related operations for 
 *              the closed descrambler.
 *
 * Inputs:  h_filter         Contains the filter handle.
 *          argv1            Contains the reference to ESDB node.
 *          reserved         Ignored
 * Outputs: argv1            Contains the reference to ESDB node which includes 
 *                           the updated status.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/     
static VOID pmi_thread_desc_closed_op(HANDLE_T h_desc, UINT32 argv1, UINT32 reserved)
{
    PM_COMPONENT_INFO_T* pt_esdb_node;
      
    pt_esdb_node = (PM_COMPONENT_INFO_T*)argv1;
    
    if ((h_desc == NULL_HANDLE) || (pt_esdb_node == NULL))
    {
        PM_DBG_ERROR("Invalid input for descrambler close\n");
        return;
    }
        
    /* ToDo, Do what ?? */
}      
/*-----------------------------------------------------------------------------
 * Name: pmi_scdb_update_op
 *
 * Description: This API is responsible to execute the related operations for 
 *              the scdb update filter.
 *
 * Inputs:  h_scdb           Contains the scdb handle.
 *          argv1            Contains the reference to ESDB node.
 *          reserved         Ignored
 * Outputs: argv1            Contains the reference to ESDB node which includes 
 *                           the updated status.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/     
static VOID pmi_thread_scdb_update_op(HANDLE_T h_scdb, UINT32 argv1, UINT32 reserved)
{
    PM_COMPONENT_INFO_T* pt_esdb_node;
    PM_SERVICE_INFO_T*   pt_svdb_node; 
    BOOL                 b_updated;   
    
    pt_esdb_node = (PM_COMPONENT_INFO_T*)argv1;
    
    if ((h_scdb == NULL_HANDLE) || (pt_esdb_node == NULL))
    {
        PM_DBG_ERROR("Invalid input for SCDB update\n");
        return;  
    }
        
    pmi_db_get_service_from_index(&pt_svdb_node, ESNODE_SERVICE_INDEX(pt_esdb_node));
    if (pt_svdb_node != NULL)
    { 
        if (pmi_scdb_query_update(pt_esdb_node, &b_updated) == PMR_OK)        
        {
            if (b_updated == TRUE)
            {
            #if (PM_ENABLE_CA)                
                pmi_descrambler_update(pt_esdb_node);
            #endif
                pmi_filter_update(pt_esdb_node);            
            }
        }
        else
        {
            /* ToDo: how to handle error */
            PM_DBG_ERROR("Query SCDB update failed\n");
        }          
    }
    else
    {
        PM_DBG_ERROR("Invalid service\n");
    }
}      
#if PM_ENABLE_CA
/*-----------------------------------------------------------------------------
 * Name: pmi_thread_ca_status_updated_op
 *
 * Description: This API is responsible to execute the related operations for 
 *              the CA status update.
 *
 * Inputs:  h_ca             Contains the CA handle.
 *          argv1            Contains the reference to ESDB node.
 *          reserved         Ignored
 * Outputs: argv1            Contains the reference to ESDB node which includes 
 *                           the updated status.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/     
static void pmi_thread_cp_status_updated_op(HANDLE_T h_ca, UINT32 argv1, UINT32 reserved)
{
    PM_COMPONENT_INFO_T* pt_esdb_node;    
    PM_CA_COND_T         t_cond;

  
    pt_esdb_node = (PM_COMPONENT_INFO_T*)argv1;
    
    if ((h_ca == NULL_HANDLE) || (pt_esdb_node == NULL))
    {
        PM_DBG_ERROR("Invalid input for ca status update\n");
        return;
    }
        
    t_cond = ESNODE_CA_COND(pt_esdb_node); 
                  
    /* Call Notify Function */
    handle_parse(&(ESNODE_LINK(pt_esdb_node)), pm_es_cp_feedback_fct, &t_cond);  
}      
#endif /* PM_ENABLE_CA */


#if (PM_ENABLE_SCRAMBLE_DETECTION)
VOID  pmi_super_timer_nfy(
            HANDLE_T                    t_timer_handle,
            VOID*                       pv_tag )
{
    MSG_T                       t_msg;

    t_msg.prc        = pmi_super_timer_hdlr;
    t_msg.h_obj      = 0;
    t_msg.ui4_data_1 = (UINT32)pv_tag;
    t_msg.ui4_data_2 = 0;

    send_msg (&t_msg);
}


VOID pmi_super_timer_hdlr(HANDLE_T h_obj, UINT32 ui4_data_1, UINT32 ui4_data_2)
{
    PM_SUPER_TIMER_T*           pt_super_timer  = (PM_SUPER_TIMER_T*)ui4_data_1;
    UINT32                      ui4_timestamp  = x_os_get_sys_tick();
    PM_TIMER_REC_T              t_temp_head;
    PM_TIMER_REC_T*             pt_moving_rec = &t_temp_head;
    
    ui4_timestamp -= pt_super_timer->ui4_tick_open;
    ui4_timestamp *= ui4_pm_sys_tick_period;/* 1 tick = x miliseconds */

    t_temp_head.pt_next = pt_super_timer->pt_head_rec;

    while (NULL != pt_moving_rec->pt_next)
    {
        if (pt_moving_rec->pt_next->ui4_timestamp <= ui4_timestamp + 20 )
        {
            PM_TIMER_REC_T*  pt_cur_rec = pt_moving_rec->pt_next;

            pt_moving_rec->pt_next     = pt_moving_rec->pt_next->pt_next;
            pt_super_timer->pt_head_rec = t_temp_head.pt_next;

            pt_cur_rec->pf_time_nfy( &pt_cur_rec->t_tag );

            x_mem_free( pt_cur_rec );
        }
        else
        {
            break;
        }
    }

    if (NULL == pt_super_timer->pt_head_rec)
    {
        pt_super_timer->ui4_tick_open = x_os_get_sys_tick();/* refresh tick */
    }
    else
    {
        INT32   i4_ret;
        UINT32  ui4_tick = ui4_timestamp;
        
        i4_ret = x_timer_start(  
                        pt_super_timer->h_super_timer,
                        (pt_super_timer->pt_head_rec->ui4_timestamp - ui4_tick),
                        X_TIMER_FLAG_ONCE,
                        pmi_super_timer_nfy,
                        pt_super_timer );
        if (OSR_OK != i4_ret)
        {
            PM_DBG_ERROR( "x_timer_start FAIL(0x%x)\r\n", i4_ret );
        }
    }
}


INT32 pmi_super_timer_enable( PM_SUPER_TIMER_T* pt_super_timer )
{
    INT32                   i4_ret;

    ui4_g_pmi_super_timer_id = 10;
    ui2_super_timer_mark     = 10;
    ui4_pm_sys_tick_period   = x_os_get_sys_tick_period();

    pt_super_timer->h_super_timer = NULL_HANDLE;
    pt_super_timer->ui4_tick_open = x_os_get_sys_tick();
    pt_super_timer->pt_head_rec   = NULL;

    i4_ret = x_timer_create( &pt_super_timer->h_super_timer );
    if (OSR_OK != i4_ret)
    {                    
        return PMR_NOT_ENOUGH_MEM;
    }

    return PMR_OK;
}


INT32 pmi_super_timer_add(
            PM_SUPER_TIMER_T*               pt_super_timer,
            UINT32                          ui4_duration,/* miliseconds */
            PMI_SUPER_TIME_NFY              pf_time_nfy,
            PM_TIMER_REC_TAG_T*             pt_tag,
            UINT32*                         pui4_timer_id )
{
    INT32                   i4_ret;
    PM_TIMER_REC_T*         pt_new_rec;
    PM_TIMER_REC_T*         pt_old_head     = pt_super_timer->pt_head_rec;
    UINT32                  ui4_timestamp   = x_os_get_sys_tick();

    pt_new_rec = (PM_TIMER_REC_T*)x_mem_alloc(sizeof(PM_TIMER_REC_T));
    if (NULL == pt_new_rec)
    {
        return PMR_NOT_ENOUGH_MEM;
    }

    pt_new_rec->ui4_duration  = ui4_duration;    
    pt_new_rec->pf_time_nfy   = pf_time_nfy;
    pt_new_rec->t_tag         = *pt_tag;
    pt_new_rec->ui4_timestamp = ui4_timestamp - pt_super_timer->ui4_tick_open;
    pt_new_rec->ui4_timestamp *= ui4_pm_sys_tick_period;/* 1 tick = x miliseconds */
    pt_new_rec->ui4_timestamp += ui4_duration;

    if ((UINT32)0xffffffff == ui4_g_pmi_super_timer_id)
    {
        ui4_g_pmi_super_timer_id = 0;
    }

    if (NULL != pui4_timer_id)
    {
        *pui4_timer_id = pt_new_rec->ui4_timer_id = (++ui4_g_pmi_super_timer_id);
    }

    /* add new record to list AND sort*/
    {
        PM_TIMER_REC_T      t_temp_head;
        PM_TIMER_REC_T*     pt_moving_rec = &t_temp_head;

        t_temp_head.pt_next = pt_super_timer->pt_head_rec;

        while (NULL != pt_moving_rec->pt_next)
        {
            if (pt_new_rec->ui4_timestamp < pt_moving_rec->pt_next->ui4_timestamp)
            {
                break;
            }

            pt_moving_rec = pt_moving_rec->pt_next;
        }

        pt_new_rec->pt_next       = pt_moving_rec->pt_next;
        pt_moving_rec->pt_next    = pt_new_rec;

        pt_super_timer->pt_head_rec = t_temp_head.pt_next;
    }

    /* start timer */
    {
        PM_TIMER_REC_T*         pt_new_head = pt_super_timer->pt_head_rec;
        UINT32                  ui4_tick    = ui4_timestamp;
        
        if (pt_old_head != pt_new_head)
        {
            if (NULL != pt_old_head)
            {
                i4_ret = x_timer_stop( pt_super_timer->h_super_timer );
                if (OSR_OK != i4_ret)
                {
                    return PMR_NOT_ENOUGH_MEM;
                }
            }

            ui4_tick -= pt_super_timer->ui4_tick_open;
            ui4_tick *= ui4_pm_sys_tick_period;

            i4_ret = x_timer_start(  
                            pt_super_timer->h_super_timer,
                            pt_new_head->ui4_timestamp-ui4_tick,
                            X_TIMER_FLAG_ONCE,
                            pmi_super_timer_nfy,
                            pt_super_timer );
            if (OSR_OK != i4_ret)
            {
                return PMR_NOT_ENOUGH_MEM;
            }
        }
    }
    
    return PMR_OK;
}


UINT16 pmi_super_timer_get_mark()
{
    return ui2_super_timer_mark = ui2_super_timer_mark % (UINT16)0xFFFE + (UINT16)1;
}

#ifdef MW_COMHEM_DELAY_SCRAMBLE_NOTIFY
VOID pmi_bootup_timer_cb(
                HANDLE_T                    t_timer_handle,
                VOID*                       pv_tag)
{
    b_1st_bootup = FALSE;
    x_dbg_stmt("Bootup notify check time out \n\t");
    return ;
}

INT32 pmi_bootup_timer_create_start(VOID)
{
    INT32                   i4_ret;
    i4_ret = x_timer_create( &h_boot_timer );
    if (OSR_OK != i4_ret)
    {                    
        return PMR_FAILED;
    }

    i4_ret = x_timer_start(h_boot_timer,
                           (PM_BOOT_NOTIFY_DELAY),
                            X_TIMER_FLAG_ONCE,
                            pmi_bootup_timer_cb,
                            NULL); 
    if (OSR_OK != i4_ret)
    {                    
        return PMR_FAILED;
    }    

    return PMR_OK;
}
#endif

INT32 pmi_thread_scramble_status_detect( PM_TIMER_REC_TAG_T* pt_tag )
{
    INT32                   i4_ret          = PMR_OK;
    UINT32                  ui4_check_sum;
    UINT16                  ui2_esnode_idx;
    UINT16                  ui2_timer_mark;
    UINT32                  ui4_last_stamp;
    PM_COMPONENT_INFO_T*    pt_esdb_node  = NULL;
    PM_TIMER_REC_TAG_T      t_rec_tag;
    UINT32                  ui4_timer_id;

    ui4_check_sum  = (UINT32)pt_tag->pv_tag1;
    ui4_last_stamp = (UINT32)pt_tag->pv_tag2;

    ui2_esnode_idx = (UINT16)(ui4_check_sum / (UINT32)0x10000);
    ui2_timer_mark = (UINT16)(ui4_check_sum % (UINT32)0x10000);
    
    pt_esdb_node = get_es_node_from_idx( ui2_esnode_idx );

    /* the ES became invaild */
    if (NULL == pt_esdb_node || ESNODE_CA_TIME_MARK(pt_esdb_node) != ui2_timer_mark)
    {
        return i4_ret;
    }

    PM_SCRS_DBG( "[save stamp:%d][last stamp:%d][cond:%d]\n", ESNODE_CA_LAST_STAMP(pt_esdb_node), ui4_last_stamp, ESNODE_CA_LAST_COND(pt_esdb_node) );

    /* the SCRS condition becomes static */
    if (ESNODE_CA_LAST_STAMP(pt_esdb_node) <= ui4_last_stamp)
    {
        PM_CA_COND_T                t_last_cond;
        
        PM_SCRS_DBG( "check timer end\n" );
        
        t_last_cond = ESNODE_CA_LAST_COND(pt_esdb_node);

        ESNODE_CA_LAST_COND(pt_esdb_node) = PM_CA_COND_UNKNOWN;
        ESNODE_CA_TIME_MARK(pt_esdb_node) = 0;
        ESNODE_CA_LAST_STAMP(pt_esdb_node) = 0;

        /* notify user */
        if (ESNODE_CA_COND(pt_esdb_node) != t_last_cond)
        {
            PM_SCRS_DBG( "check timer end callback [cond:%d]\n", t_last_cond );
            ESNODE_CA_COND(pt_esdb_node) = t_last_cond;
            handle_parse(&(ESNODE_LINK(pt_esdb_node)), pm_es_ca_feedback_fct, &ESNODE_CA_COND(pt_esdb_node));
        }
    }
    /* continue to waiting for static status */
    else
    {
        PM_SCRS_DBG( "check timer continue\n" );
        
        if (PM_CA_COND_AUTHORIZED != ESNODE_CA_COND(pt_esdb_node))
        {
            ESNODE_CA_COND(pt_esdb_node) = PM_CA_COND_AUTHORIZED;
            handle_parse(&(ESNODE_LINK(pt_esdb_node)), pm_es_ca_feedback_fct, &ESNODE_CA_COND(pt_esdb_node));
        }
        
        ESNODE_CA_TIME_MARK(pt_esdb_node) = ui2_timer_mark = pmi_super_timer_get_mark();

        ui4_check_sum     = ui2_esnode_idx*(UINT32)0x10000 + ui2_timer_mark;
        t_rec_tag.pv_tag1 = (VOID*)ui4_check_sum;
        t_rec_tag.pv_tag2 = (VOID*)ESNODE_CA_LAST_STAMP(pt_esdb_node);

        pmi_super_timer_add( 
                    &t_g_pm_super_timer, 
                    PM_SCRS_DELAY, 
                    pmi_thread_scramble_status_detect, 
                    &t_rec_tag, 
                    &ui4_timer_id );
    }

    return i4_ret;
}


VOID pmi_thread_scramble_status_judge(HANDLE_T h_filter, UINT32 argv1, UINT32 reserved)
{
    DEMUX_SCRS_T            t_scramble_status;  
    PM_COMPONENT_INFO_T*    pt_esdb_node = NULL; 
    PM_CA_COND_T            t_cur_cond = PM_CA_COND_UNKNOWN;
    UINT32                  ui4_timer_id;
    PM_TIMER_REC_TAG_T      t_rec_tag;
    UINT32                  ui4_check_sum;
    UINT16                  ui2_esnode_idx;
    UINT16                  ui2_timer_mark;
    UINT32                  ui4_delay = PM_SCRS_DELAY;
    
    pt_esdb_node = (PM_COMPONENT_INFO_T*)argv1;  
    t_scramble_status = (DEMUX_SCRS_T)reserved;    
    
    if(pt_esdb_node == NULL)
    {
        PM_DBG_ERROR("Invalid Filter Object\n");
        return;
    }
    
    switch (t_scramble_status)
    {
    case DEMUX_SCRS_UNKNOWN:
        t_cur_cond = PM_CA_COND_UNKNOWN;           
        PM_SCRS_DBG( "PM_CA_COND_UNKNOWN\n" );
        break;
    case DEMUX_SCRS_CLEAR:
        PM_SCRS_DBG( "DEMUX_SCRS_CLEAR\n" );
        #if PM_ENABLE_CA
            if(TRUE == pmi_is_ca_sys_id_match_pmt())
            {
                t_cur_cond = PM_CA_COND_AUTHORIZED_CA_SYS_ID;
            }
            else
            {
                t_cur_cond = PM_CA_COND_AUTHORIZED;              
            }
        #else
            t_cur_cond = PM_CA_COND_AUTHORIZED;
        #endif
        break;
    case DEMUX_SCRS_SCRAMBLED:
        PM_SCRS_DBG( "DEMUX_SCRS_SCRAMBLED\n" );
        t_cur_cond = PM_CA_COND_NOT_AUTHORIZED;                        
        break;
    }

    /* save the notification */
    ESNODE_CA_LAST_COND(pt_esdb_node) = t_cur_cond;
    ESNODE_CA_LAST_STAMP(pt_esdb_node) = x_os_get_sys_tick();

    PM_SCRS_DBG( "NEW LAST STAMP[%d]\n", ESNODE_CA_LAST_STAMP(pt_esdb_node) );
    /* x_dbg_stmt("----pmi_thread_scramble_status_judge Get Status %d ----\n\r", t_scramble_status); */

    /* SCRS has not be started */
    if (0 == ESNODE_CA_TIME_MARK(pt_esdb_node))
    {
        /* notify SM, if the timer has not been started */
        #ifdef MW_COMHEM_DELAY_SCRAMBLE_NOTIFY
        if ((t_cur_cond == PM_CA_COND_AUTHORIZED) || 
            (t_cur_cond == PM_CA_COND_AUTHORIZED_CA_SYS_ID))        
        {
            ESNODE_CA_COND(pt_esdb_node) = t_cur_cond;
            handle_parse(&(ESNODE_LINK(pt_esdb_node)), pm_es_ca_feedback_fct, &ESNODE_CA_COND(pt_esdb_node));
            if(TRUE == b_1st_bootup)
            {
                b_1st_bootup = FALSE;
            }
        }
        #else
           ESNODE_CA_COND(pt_esdb_node) = t_cur_cond;
           handle_parse(&(ESNODE_LINK(pt_esdb_node)), pm_es_ca_feedback_fct, &ESNODE_CA_COND(pt_esdb_node));
        #endif

        /* perpare parameter and started timer */
        {
            ESNODE_CA_TIME_MARK(pt_esdb_node) = ui2_timer_mark = pmi_super_timer_get_mark();
            ui2_esnode_idx = get_index_from_es_node( pt_esdb_node );
            ui4_check_sum     = ui2_esnode_idx*(UINT32)0x10000 + ui2_timer_mark;
            t_rec_tag.pv_tag1 = (VOID*)ui4_check_sum;
            t_rec_tag.pv_tag2 = (VOID*)ESNODE_CA_LAST_STAMP(pt_esdb_node);

            PM_SCRS_DBG( "start check timer[stamp:%d]\n", ESNODE_CA_LAST_STAMP(pt_esdb_node) );

            #if PM_ENABLE_CA
            ui4_delay = (PM_SCRS_DELAY);
            #ifdef MW_COMHEM_DELAY_SCRAMBLE_NOTIFY
            if(TRUE == b_1st_bootup)
            {
                ui4_delay = PM_BOOT_NOTIFY_DELAY;
                /* x_dbg_stmt("----pmi_thread_scramble_status_judge 1st boot delay ----\n\r"); */
            }
            else if ((CAM_INSERT == pmi_get_cam_wait_status()) ||
                (CAM_INSERT_CIPLUS == pmi_get_cam_wait_status()) )
            {
                ui4_delay = 5 *(PM_SCRS_DELAY);
                /* x_dbg_stmt("----pmi_thread_scramble_status_judge delay  CI/CI+ CAM ----\n\r"); */
            }
            else
            {
                ui4_delay = (PM_SCRS_DELAY);
            }
            #endif
            #endif
            pmi_super_timer_add( 
                        &t_g_pm_super_timer, 
                        ui4_delay, 
                        pmi_thread_scramble_status_detect, 
                        &t_rec_tag, 
                        &ui4_timer_id );
        }
    }
    #if PM_ENABLE_CA
    #ifdef MW_COMHEM_DELAY_SCRAMBLE_NOTIFY
    else
    {
        if ((CAM_INSERT == pmi_get_cam_wait_status()) ||
            (CAM_INSERT_CIPLUS == pmi_get_cam_wait_status()))
        {
            if ((t_cur_cond == PM_CA_COND_AUTHORIZED) ||             
                (t_cur_cond == PM_CA_COND_AUTHORIZED_CA_SYS_ID))
            {
                /* x_dbg_stmt("----pmi_thread_scramble_status_judge Clear Notify Imm ----\n\r"); */
                ESNODE_CA_COND(pt_esdb_node) = t_cur_cond;
                handle_parse(&(ESNODE_LINK(pt_esdb_node)), pm_es_ca_feedback_fct, &ESNODE_CA_COND(pt_esdb_node));
            }
        }            
    }
    #endif
    #endif    
}


#if (PM_ENABLE_FILTER_CTRL)
VOID  pmi_thread_scramble_status_ctrl_nfy( PM_FILTER_CTRL_OBJ_T* pt_filter_ctrl )
{
    PM_COMPONENT_INFO_T*    pt_esdb_curr = NULL;
    UINT16                  ui2_check_count;
    
    pt_esdb_curr = pt_filter_ctrl->pt_head;
    ui2_check_count = 0;    
    
    while((pt_esdb_curr != NULL)&&(ui2_check_count < pt_filter_ctrl->ui2_reference_count))
    {
        ESNODE_CA_COND(pt_esdb_curr) = pt_filter_ctrl->t_cond;
        handle_parse(&(ESNODE_LINK(pt_esdb_curr)), pm_es_ca_feedback_fct, &ESNODE_CA_COND(pt_esdb_curr));
        
        ui2_check_count++;
        pt_esdb_curr = ESNODE_FILTER_OBJ_NEXT(pt_esdb_curr);
    }
}


INT32 pmi_thread_scramble_status_ctrl_detect( PM_TIMER_REC_TAG_T* pt_tag )
{
    INT32                   i4_ret          = PMR_OK;
    UINT32                  ui4_check_sum;
    UINT16                  ui2_filter_ctrl_idx;
    UINT16                  ui2_timer_mark;
    UINT32                  ui4_last_stamp;
    PM_FILTER_CTRL_OBJ_T*   pt_filter_ctrl  = NULL;
    PM_TIMER_REC_TAG_T      t_rec_tag;
    UINT32                  ui4_timer_id;

    ui4_check_sum  = (UINT32)pt_tag->pv_tag1;
    ui4_last_stamp = (UINT32)pt_tag->pv_tag2;

    ui2_filter_ctrl_idx = (UINT16)(ui4_check_sum / (UINT32)0x10000);
    ui2_timer_mark = (UINT16)(ui4_check_sum % (UINT32)0x10000);
    
    pt_filter_ctrl = pmi_filter_db_get_ctrl_by_idx( ui2_filter_ctrl_idx );

    /* the filter control became invaild */
    if (NULL == pt_filter_ctrl || FALSE == pt_filter_ctrl->b_used || pt_filter_ctrl->ui2_time_nfy_mark != ui2_timer_mark)
    {
        return i4_ret;
    }

    /* the SCRS condition becomes static */
    if (pt_filter_ctrl->ui4_last_cond_stamp <= ui4_last_stamp)
    {
        PM_CA_COND_T                t_last_cond;

        PM_SCRS_DBG( "check timer end\n" );
        
        t_last_cond = pt_filter_ctrl->t_last_cond;

        pt_filter_ctrl->t_last_cond = PM_CA_COND_UNKNOWN;
        pt_filter_ctrl->ui4_last_cond_stamp = 0;
        pt_filter_ctrl->ui2_time_nfy_mark = 0;

        /* notify user */
        if (pt_filter_ctrl->t_cond != t_last_cond)
        {
            pt_filter_ctrl->t_cond = t_last_cond;
            pmi_thread_scramble_status_ctrl_nfy( pt_filter_ctrl );
        }
    }
    /* continue to waiting for static status */
    else
    {
        PM_SCRS_DBG( "check timer continue\n" );
        
        if (PM_CA_COND_AUTHORIZED != pt_filter_ctrl->t_cond)
        {
            pt_filter_ctrl->t_cond = PM_CA_COND_AUTHORIZED;
            pmi_thread_scramble_status_ctrl_nfy( pt_filter_ctrl );
        }
        
        pt_filter_ctrl->ui2_time_nfy_mark = ui2_timer_mark = pmi_super_timer_get_mark();

        ui4_check_sum     = ui2_filter_ctrl_idx*(UINT32)0x10000 + ui2_timer_mark;
        t_rec_tag.pv_tag1 = (VOID*)ui4_check_sum;
        t_rec_tag.pv_tag2 = (VOID*)pt_filter_ctrl->ui4_last_cond_stamp;

        pmi_super_timer_add( 
                    &t_g_pm_super_timer, 
                    PM_SCRS_DELAY, 
                    pmi_thread_scramble_status_ctrl_detect, 
                    &t_rec_tag, 
                    &ui4_timer_id );
    }

    return i4_ret;
}


VOID pmi_thread_scramble_status_ctrl_judge(HANDLE_T h_filter, UINT32 argv1, UINT32 reserved)
{
    DEMUX_SCRS_T            t_scramble_status;  
    PM_FILTER_CTRL_OBJ_T*   pt_filter_ctrl = NULL; 
    PM_CA_COND_T            t_cur_cond = PM_CA_COND_UNKNOWN;
    UINT32                  ui4_timer_id;
    PM_TIMER_REC_TAG_T      t_rec_tag;
    UINT32                  ui4_check_sum;
    UINT16                  ui2_filter_ctrl_idx;
    UINT16                  ui2_timer_mark;
    UINT32                  ui4_delay = PM_SCRS_DELAY;
    
    pt_filter_ctrl = (PM_FILTER_CTRL_OBJ_T*)argv1;  
    t_scramble_status = (DEMUX_SCRS_T)reserved;    
    
    if(pt_filter_ctrl == NULL)
    {
        PM_DBG_ERROR("Invalid Filter Object\n");
        return;
    }
    
    switch (t_scramble_status)
    {
    case DEMUX_SCRS_UNKNOWN:
        PM_SCRS_DBG( "DEMUX_SCRS_UNKNOWN\n" );
        t_cur_cond = PM_CA_COND_UNKNOWN;           
        break;
    case DEMUX_SCRS_CLEAR:
        PM_SCRS_DBG( "DEMUX_SCRS_CLEAR\n" );
        #if PM_ENABLE_CA
            if(TRUE == pmi_is_ca_sys_id_match_pmt())
            {
                t_cur_cond = PM_CA_COND_AUTHORIZED_CA_SYS_ID;
            }
            else
            {
                t_cur_cond = PM_CA_COND_AUTHORIZED;              
            }
        #else
            t_cur_cond = PM_CA_COND_AUTHORIZED;
        #endif
        break;
    case DEMUX_SCRS_SCRAMBLED:
        PM_SCRS_DBG( "DEMUX_SCRS_SCRAMBLED\n" );
        t_cur_cond = PM_CA_COND_NOT_AUTHORIZED;                        
        break;
    }

    /* save the notification */
    pt_filter_ctrl->t_last_cond = t_cur_cond;
    pt_filter_ctrl->ui4_last_cond_stamp = x_os_get_sys_tick();

    PM_SCRS_DBG( "NEW LAST STAMP[%d]\n", pt_filter_ctrl->ui4_last_cond_stamp );
    /* x_dbg_stmt("----pmi_thread_scramble_status_ctrl_judge Get Status %d ----\n\r", t_scramble_status); */

    /* SCRS has not be started */
    if (0 == pt_filter_ctrl->ui2_time_nfy_mark)
    {
        /* notify SM, if the timer has not been started */
        #ifdef MW_COMHEM_DELAY_SCRAMBLE_NOTIFY
        if ((t_cur_cond == PM_CA_COND_AUTHORIZED) || 
            (t_cur_cond == PM_CA_COND_AUTHORIZED_CA_SYS_ID))
        {
            pt_filter_ctrl->t_cond = t_cur_cond;
            pmi_thread_scramble_status_ctrl_nfy( pt_filter_ctrl );
            if(TRUE == b_1st_bootup)
            {
                b_1st_bootup = FALSE;
            }
        }
        #else
        pt_filter_ctrl->t_cond = t_cur_cond;
        pmi_thread_scramble_status_ctrl_nfy( pt_filter_ctrl );
        #endif

        /* perpare parameter and started timer */
        {
            pt_filter_ctrl->ui2_time_nfy_mark = ui2_timer_mark = pmi_super_timer_get_mark();
            ui2_filter_ctrl_idx = pmi_filter_db_get_idx_by_ctrl( pt_filter_ctrl );
            ui4_check_sum     = ui2_filter_ctrl_idx*(UINT32)0x10000 + ui2_timer_mark;
            t_rec_tag.pv_tag1 = (VOID*)ui4_check_sum;
            t_rec_tag.pv_tag2 = (VOID*)pt_filter_ctrl->ui4_last_cond_stamp;

            PM_SCRS_DBG( "start check timer[stamp:%d]\n", pt_filter_ctrl->ui4_last_cond_stamp );

            #if PM_ENABLE_CA
            ui4_delay = (PM_SCRS_DELAY);
            #ifdef MW_COMHEM_DELAY_SCRAMBLE_NOTIFY
            if(TRUE == b_1st_bootup)
            {
                ui4_delay = PM_BOOT_NOTIFY_DELAY;
                /* x_dbg_stmt("----pmi_thread_scramble_status_ctrl_judge 1st boot delay ----\n\r"); */
            }            
            else if (((CAM_INSERT == pmi_get_cam_wait_status()) ||
                (CAM_INSERT_CIPLUS == pmi_get_cam_wait_status())) || 
                (TRUE == b_1st_bootup))
            {
                ui4_delay = 5 *(PM_SCRS_DELAY);
                /* x_dbg_stmt("----pmi_thread_scramble_status_ctrl_judge delay  CI/CI+ CAM ----\n\r"); */
            }
            else
            {
                ui4_delay = (PM_SCRS_DELAY);
            }
            #endif
            #endif            
            pmi_super_timer_add( 
                        &t_g_pm_super_timer, 
                        ui4_delay, 
                        pmi_thread_scramble_status_ctrl_detect, 
                        &t_rec_tag, 
                        &ui4_timer_id );
        }
    }
    #if PM_ENABLE_CA
    #ifdef MW_COMHEM_DELAY_SCRAMBLE_NOTIFY
    else
    {
        if ((CAM_INSERT == pmi_get_cam_wait_status()) ||
            (CAM_INSERT_CIPLUS == pmi_get_cam_wait_status()))
        {
            if ((t_cur_cond == PM_CA_COND_AUTHORIZED) ||             
                (t_cur_cond == PM_CA_COND_AUTHORIZED_CA_SYS_ID))
            {
                /* x_dbg_stmt("----pmi_thread_scramble_status_ctrl_judge Clear Notify Imm ----\n\r"); */
                pt_filter_ctrl->t_cond = t_cur_cond;
                pmi_thread_scramble_status_ctrl_nfy( pt_filter_ctrl );
            }
        }            
    }
    #endif
    #endif

}
#endif  /* #if (PM_ENABLE_FILTER_CTRL) */
#endif  /* #if (PM_ENABLE_SCRAMBLE_DETECTION) */

/*-----------------------------------------------------------------------------
 * Name: pmi_thread_filter_opened
 *
 * Description: This API is responsible to send message to PM thread to handle 
 *              the operation for the opened filter.
 *
 * Inputs:  h_filter         Contains the filter handle.
 *          pt_esdb_node     Contains the reference to ESDB node. 
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/     
void pmi_thread_filter_opened(HANDLE_T h_filter, PM_COMPONENT_INFO_T* pt_esdb_node)
{
    MSG_T t_msg;

    if ((h_filter != NULL_HANDLE)&&(pt_esdb_node != NULL))
    {
        t_msg.prc        = pmi_thread_filter_opened_op;
        t_msg.h_obj      = h_filter;
        t_msg.ui4_data_1 = (UINT32) pt_esdb_node;
        t_msg.ui4_data_2 = 0;
        
        send_msg (&t_msg);
    }
    else
    {
        PM_DBG_ERROR("Invalid input for filter open filter:0x%08x ES:0x%08x\n",
                   h_filter, pt_esdb_node);
    }
    
}    

/*-----------------------------------------------------------------------------
 * Name: pmi_thread_filter_closed
 *
 * Description: This API is responsible to send message to PM thread to handle 
 *              the operation for the closed filter.
 *
 * Inputs:  h_filter         Contains the filter handle.
 *          pt_esdb_node     Contains the reference to ESDB node. 
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
void pmi_thread_filter_closed(HANDLE_T h_filter, PM_COMPONENT_INFO_T* pt_esdb_node)
{
    MSG_T t_msg;

    if ((h_filter != NULL_HANDLE)&&(pt_esdb_node != NULL))
    {
        t_msg.prc        = pmi_thread_filter_closed_op;
        t_msg.h_obj      = h_filter;
        t_msg.ui4_data_1 = (UINT32) pt_esdb_node;
        t_msg.ui4_data_2 = 0;
        
        send_msg (&t_msg);
    }
    else
    {
        PM_DBG_ERROR("Invalid input for filter close filter:0x%08x ES:0x%08x\n",h_filter, pt_esdb_node);
    }
}   
/*-----------------------------------------------------------------------------
 * Name: pmi_thread_desc_opened
 *
 * Description: This API is responsible to send message to PM thread to handle 
 *              the operation for the opened descrambler.
 *
 * Inputs:  h_desc         Contains the descrambler handle.
 *          pt_esdb_node     Contains the reference to ESDB node. 
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/     
void pmi_thread_desc_opened(HANDLE_T h_desc, PM_COMPONENT_INFO_T* pt_esdb_node)
{
    MSG_T t_msg;

    if ((h_desc != NULL_HANDLE)&&(pt_esdb_node != NULL))
    {
        t_msg.prc        = pmi_thread_desc_opened_op;
        t_msg.h_obj      = h_desc;
        t_msg.ui4_data_1 = (UINT32) pt_esdb_node;
        t_msg.ui4_data_2 = 0;
        
        send_msg (&t_msg);
    }
    else
    {
        PM_DBG_ERROR("Invalid input for descrambler open :0x%08x ES:0x%08x\n",
                   h_desc, pt_esdb_node);
    }
    
}    

/*-----------------------------------------------------------------------------
 * Name: pmi_thread_desc_closed
 *
 * Description: This API is responsible to send message to PM thread to handle 
 *              the operation for the closed descrambler.
 *
 * Inputs:  h_desc         Contains the descrambler handle.
 *          pt_esdb_node     Contains the reference to ESDB node. 
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
void pmi_thread_desc_closed(HANDLE_T h_desc, PM_COMPONENT_INFO_T* pt_esdb_node)
{
    MSG_T t_msg;

    if ((h_desc != NULL_HANDLE)&&(pt_esdb_node != NULL))
    {
        t_msg.prc        = pmi_thread_desc_closed_op;
        t_msg.h_obj      = h_desc;
        t_msg.ui4_data_1 = (UINT32) pt_esdb_node;
        t_msg.ui4_data_2 = 0;
        
        send_msg (&t_msg);
    }
    else
    {
        PM_DBG_ERROR("Invalid input for descrambler close :0x%08x ES:0x%08x\n",h_desc, pt_esdb_node);
    }
}   
/*-----------------------------------------------------------------------------
 * Name: pmi_thread_scdb_updated
 *
 * Description: This API is responsible to send message to PM thread to handle 
 *              the operation for the scdb update.
 *
 * Inputs:  h_scdb           Contains the scdb handle.
 *          pt_esdb_node     Contains the reference to ESDB node. 
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/  
VOID pmi_thread_scdb_updated(HANDLE_T h_scdb, PM_COMPONENT_INFO_T*pt_esdb_node)
{
    MSG_T t_msg;

    if ((h_scdb != NULL_HANDLE)&&(pt_esdb_node != NULL))
    {
        t_msg.prc        = pmi_thread_scdb_update_op;
        t_msg.h_obj      = h_scdb;
        t_msg.ui4_data_1 = (UINT32) pt_esdb_node;
        t_msg.ui4_data_2 = 0; 
        
        send_msg (&t_msg);
    }    
    else
    {
        PM_DBG_ERROR("Invalid input for scdb update scdb:0x%08x ES:0x%08x\n",h_scdb, pt_esdb_node);
    }    
} 
#if PM_ENABLE_CA
/*-----------------------------------------------------------------------------
 * Name: pmi_thread_ca_updated
 *
 * Description: This API is responsible to send message to PM thread to handle 
 *              the operation for CA status update.
 *
 * Inputs:  h_ca             Contains the CA handle.
 *          pt_esdb_node     Contains the reference to ESDB node. 
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
void pmi_thread_cp_updated(HANDLE_T h_ca, PM_COMPONENT_INFO_T*pt_esdb_node)
{
    MSG_T t_msg;

    if ((h_ca != NULL_HANDLE)&&(pt_esdb_node != NULL))
    {
        t_msg.prc        = pmi_thread_cp_status_updated_op;
        t_msg.h_obj      = h_ca;
        t_msg.ui4_data_1 = (UINT32) pt_esdb_node;
        t_msg.ui4_data_2 = 0;
        
        send_msg (&t_msg);
    }
    else
    {
        PM_DBG_ERROR("Invalid input for ca update ca:0x%08x ES:0x%08x\n",
                    h_ca, pt_esdb_node);
    }     
}   
#endif /* PM_ENABLE_CA */

#if (PM_ENABLE_SCRAMBLE_DETECTION)
/*-----------------------------------------------------------------------------
 * Name: pmi_thread_scramble_updated
 *
 * Description: This API is responsible to send message to real handler for
 *              demux scramble status update.
 *
 * Inputs:  h_filter            Contains the filter handle.
 *          pt_esdb_node        Contains the reference to ESDB node.
 *          t_scramble_status   Contains the scarmble status.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/     
VOID pmi_thread_scramble_updated(HANDLE_T             h_filter, 
                                 PM_COMPONENT_INFO_T* pt_esdb_node, 
                                 DEMUX_SCRS_T         t_scramble_status)
{
    MSG_T t_msg;    
    
    if ((h_filter != NULL_HANDLE)&&(pt_esdb_node != NULL))
    {
        t_msg.prc        = pmi_thread_scramble_status_judge;
        t_msg.h_obj      = h_filter;
        t_msg.ui4_data_1 = (UINT32) pt_esdb_node;
        t_msg.ui4_data_2 = (UINT32) t_scramble_status;
        
        send_msg (&t_msg);
    }
    else
    {
        PM_DBG_ERROR("Invalid input for scramble status update ca:0x%08x ES:0x%08x\n",
                    h_filter, pt_esdb_node);
    }      
} 

#if (PM_ENABLE_FILTER_CTRL)
/*-----------------------------------------------------------------------------
 * Name: pmi_thread_scramble_updated
 *
 * Description: This API is responsible to send message to real handler for
 *              demux scramble status update.
 *
 * Inputs:  h_filter            Contains the filter handle.
 *          pt_esdb_node        Contains the reference to ESDB node.
 *          t_scramble_status   Contains the scarmble status.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/  
VOID pmi_thread_scramble_updated_for_central_ctrl(HANDLE_T              h_filter, 
                                                  PM_FILTER_CTRL_OBJ_T* pt_filter_ctrl, 
                                                  DEMUX_SCRS_T          t_scramble_status)
{
    MSG_T t_msg;    
    
    if (pt_filter_ctrl != NULL)
    {
        t_msg.prc        = pmi_thread_scramble_status_ctrl_judge;
        t_msg.h_obj      = NULL_HANDLE;
        t_msg.ui4_data_1 = (UINT32) pt_filter_ctrl;
        t_msg.ui4_data_2 = (UINT32) t_scramble_status;
        
        send_msg (&t_msg);
    }
    else
    {
        PM_DBG_ERROR("Invalid input for scramble status update ca:0x%08x CTRL:0x%08x\n",
                    h_filter, pt_filter_ctrl);
    }          
    
}
#endif   /*  #if (PM_ENABLE_FILTER_CTRL) */
#endif
/*-----------------------------------------------------------------------------
 * Name: send_msg
 *
 * Description: This API sends a message to the Process Manager thread.
 *
 * Inputs:  pt_msg  References the message which shall be sent.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID send_msg (MSG_T*  pt_msg)
{
    BOOL   b_msg_sent;
    UINT16 ui2_cnt;


    ui2_cnt    = 0;
    b_msg_sent = FALSE;
    
    while ((! (b_msg_sent))             &&
           (ui2_cnt < MSG_RETRY_COUNT))
    {
        b_msg_sent = (x_msg_q_send (h_msg_q,
                                    ((VOID*) pt_msg),
                                    sizeof (MSG_T),
                                    PM_MSG_DEFAULT_PRIORITY) == OSR_OK);

        if (! (b_msg_sent))
        {
            /* Delay some time and retry sending te message. */
            x_thread_delay (MSG_RETRY_DELAY);

            ui2_cnt++;
        }
    }

    if (! (b_msg_sent))
    {
        /* Did not manage to send the message. */
        /* Something is seriously wrong.       */
        PM_DBG_ERROR("Send message failed\n");
        PM_ABORT (DBG_CAT_MESSAGE, PM_DBG_ABRT_CANNOT_SEND_MSG);
    }

}


/*-----------------------------------------------------------------------------
 * Name: thread_main
 *
 * Description: This is the entry point of the Process Manager thread. This
 *              API should NEVER return.
 *
 * Inputs:  pv_argv  Ignored.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID thread_main_fct (VOID*  pv_argv)
{
    MSG_T   t_msg;
    UINT16  ui2_idx;
    SIZE_T  z_msg_size;

    x_memset( &t_msg, 0, sizeof(MSG_T) );    
    
    /* Endless routine. This thread NEVER terminates. */
    while (TRUE)
    {
        /* Set the message queue receive arguments. */
        z_msg_size = sizeof (MSG_T);
        
        if (x_msg_q_receive (&ui2_idx,
                             ((VOID*) &t_msg),
                             &z_msg_size,
                             &h_msg_q,
                             1,
                             X_MSGQ_OPTION_WAIT) == OSR_OK)
        {
            pmi_lock_db_wr();
            t_msg.prc (t_msg.h_obj, t_msg.ui4_data_1, t_msg.ui4_data_2);
            pmi_unlock_db();
        }
        else
        {
            /* PM_ABORT when we have an error here. */
            PM_DBG_ERROR("Receive message failed\n");
            PM_ABORT(DBG_CAT_MESSAGE, PM_DBG_ABRT_CANNOT_RECEIVE_MSG);                  
        }
        
    }
}
/*-----------------------------------------------------------------------------
 * Name: pmi_thread_init
 *
 * Description: This API initializes the Process Managers thread and message
 *              queue.
 *
 * Inputs:  z_stack_size  Contains the stack size of the Resource Manager
 *                        thread.
 *          ui1_priority  Contains the priority of the Resource Manager
 *                        thread.
 *          ui2_num_msgs  Contains the number of messages which shall be
 *                        allocated.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
VOID pmi_thread_init (SIZE_T  z_stack_size,
                      UINT8   ui1_priority,
                      UINT16  ui2_num_msgs)
{
    if (ui2_num_msgs == DEFAULT_NUM_MSGS)
        ui2_num_msgs = PM_DEFAULT_NUM_MSGS;
    if (ui1_priority == DEFAULT_PRIORITY)
        ui1_priority = PM_DEFAULT_PRIORITY;
    if (z_stack_size == DEFAULT_STACK_SIZE)
        z_stack_size = PM_DEFAULT_STACK_SIZE;

    pmi_super_timer_enable( &t_g_pm_super_timer );
        
    if (x_msg_q_create (&h_msg_q, PM_MSG_Q_NAME, sizeof (MSG_T), ui2_num_msgs) != OSR_OK)
    {
        PM_DBG_ERROR("Create message queue failed\n");
        PM_ABORT(DBG_CAT_MESSAGE, PM_DBG_ABRT_CANNOT_CREATE_MSG_Q);         
    }
   
    /* And create the Resource Manager thread. */
    if (x_thread_create (&h_thread,
                         PM_THREAD_NAME,
                         z_stack_size,
                         ui1_priority,
                         thread_main_fct,
                         0,
                         NULL) != OSR_OK)
    {
        PM_DBG_ERROR("Create thread failed\n");
        PM_ABORT(DBG_CAT_MESSAGE, PM_DBG_ABRT_CANNOT_CREATE_THREAD);       
    }

    #if (PM_ENABLE_SCRAMBLE_DETECTION)
    /* ...??? */
    /* init super timer...... */
    #ifdef MW_COMHEM_DELAY_SCRAMBLE_NOTIFY
    pmi_bootup_timer_create_start();
    #endif
    #endif
}

 
