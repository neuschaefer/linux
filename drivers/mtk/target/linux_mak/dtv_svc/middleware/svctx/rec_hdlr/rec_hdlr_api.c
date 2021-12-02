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
 * $RCSfile:  $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains Record Handler implementations.
 *---------------------------------------------------------------------------*/

#ifdef SYS_RECORD_SUPPORT

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "svctx/rec_hdlr/rec_hdlr_cli.h"
#include "svctx/rec_hdlr/rec_hdlr.h"
#include "svctx/rec_hdlr/rec_svc_req.h"
#include "svctx/svctx_mgr.h"
#include "dbg/def_dbg_level_mw.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"
#include "svctx/ch_hdlr/ch_hdlr.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#define DBG_LEVEL_MODULE     rec_hdlr_get_dbg_level()


#define SVC_REQ_AMOUNT_FACTOR   (2)

typedef struct _RH_MDB_CTX_T
{
    HANDLE_T        h_rec_util;
    
} RH_MDB_CTX_T;


/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/
static BOOL     b_g_init = FALSE;
static UINT16   ui2_g_rec_hdlr_dbg_level = DBG_INIT_LEVEL_MW_REC_HDLR;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static BOOL _rec_hdlr_check_valid_rec_cfg (
    const SVCTX_REC_CONFIG_T*   pt_rec_cfg);

static INT32 _rec_hdlr_meta_db_open (
    SVCTX_T*                    pt_svctx,
    SVCTX_REC_META_DB_OPEN_T*   pt_open);

static INT32 _rec_hdlr_meta_db_close (
    SVCTX_T*                    pt_svctx,
    SVCTX_REC_META_DB_CLOSE_T*  pt_close);
    
static INT32 _rec_hdlr_meta_db_store (
    SVCTX_T*                    pt_svctx,
    SVCTX_REC_META_DB_STORE_T*  pt_store);
    
static INT32 _rec_hdlr_meta_db_set_info (
    SVCTX_T*                    pt_svctx,
    SVCTX_REC_SET_META_INFO_T*  pt_set_info);

static INT32 _rec_hdlr_meta_db_get_info (
    SVCTX_T*                    pt_svctx,
    SVCTX_REC_GET_META_INFO_T*  pt_get_info);
    
static INT32 _rec_hdlr_meta_db_get_info_len (
    SVCTX_T*                       pt_svctx,
    SVCTX_REC_GET_META_INFO_LEN_T* pt_get_info_len);

/*-----------------------------------------------------------------------------
                    internal functions 
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    API functions 
 ----------------------------------------------------------------------------*/
static SVC_REQ_T* _rec_hdlr_allocate_svc_req (VOID)
{
    REC_SVC_REQ_T*      pt_svc_req;
    INT32               i4_ret;
    
    i4_ret = rec_svc_req_allocate (&pt_svc_req);
    
    if (i4_ret != SVCTXR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to allocate SVC REQ. (L%d)\n", __LINE__));
        return NULL;
    }
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Called I/F: %s (0x%X) \n", __FUNCTION__, pt_svc_req));
    
    return ((SVC_REQ_T *) pt_svc_req);
}

static VOID _rec_hdlr_free_svc_req (
    SVC_REQ_T*                  pt_svc_req)
{
    INT32   i4_ret;
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Called I/F: %s (0x%X) \n", __FUNCTION__, pt_svc_req));
    
    i4_ret = rec_svc_req_free ((REC_SVC_REQ_T *) pt_svc_req);
    
    if (i4_ret != SVCTXR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to free SVC REQ. (L%d)\n", __LINE__));
    }
}

static SVC_REQ_T* _rec_hdlr_create_svc_req (
    SVCTX_T*                    pt_svctx,
    HANDLE_T                    h_svctx,
    SVCTX_HDLR_T*               pt_hdlr,
    UINT32                      ui4_stream_mode,
    const SRC_DESC_T*           pt_src_desc,
    const SNK_DESC_T*           pt_snk_desc,
    x_svctx_select_nfy_fct      pf_nfy,
    VOID*                       pv_nfy_tag)
{
    REC_SVC_REQ_T*   pt_svc_req = NULL;
    INT32            i4_ret;
    
    pt_svc_req = (REC_SVC_REQ_T*) svctx_allocate_svc_req (pt_hdlr); 
    
    if (pt_svc_req == NULL)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to allocate SVC REQ. (L%d)\n", __LINE__));
        return NULL;
    }
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Called I/F: %s (0x%X) \n", __FUNCTION__, pt_svc_req));
    
    i4_ret = rec_svc_preproc_svc (pt_svctx,
                                  h_svctx,
                                  pt_hdlr,
                                  pt_src_desc,
                                  pt_snk_desc,
                                  pf_nfy,
                                  pv_nfy_tag,
                                  pt_svc_req );

    if (i4_ret != SVCTXR_OK)                    
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to preproc SVC REQ (%d). (L%d)\n", i4_ret, __LINE__));
        
        svctx_free_svc_req( (SVC_REQ_T*) pt_svc_req); 
        
        return NULL;
    }
    
    return ((SVC_REQ_T *) pt_svc_req);
}     

static SVC_REQ_T* _rec_hdlr_create_surf_svc_req(
    SVCTX_T*                    pt_svctx,
    HANDLE_T                    h_svctx,
    SVCTX_HDLR_T*               pt_hdlr,
    UINT32                      ui4_nw_chk_mask,
    UINT32                      ui4_nw_chk_value,
    SURF_MODE_T                 e_surf_mode,
    SVL_REC_T*                  pt_svl_rec )
{
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Called I/F: %s () \n", __FUNCTION__));
    
    return NULL;
}    

static INT32 _rec_hdlr_select_svc (
    SVCTX_T*                    pt_svctx)
{
    INT32     i4_ret;

    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Called I/F: %s (0x%X) \n", 
               __FUNCTION__, 
               pt_svctx->pt_running_svc_req));

    i4_ret = rec_svc_init_svc (pt_svctx);
    
    if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_PENDING)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to init svc (%d). (L%d)\n", i4_ret, __LINE__));
        return i4_ret;
    }
    
    return i4_ret;
}

static INT32 _rec_hdlr_stop_svc (
    SVCTX_T*                    pt_svctx,
    BOOL                        b_stop_only)
{
    REC_SVC_REQ_T*  pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    REC_SVC_REQ_T*  pt_pending_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_pending_svc_req;
    BOOL            b_reuse_pipe;
    INT32           i4_ret = SVCTXR_OK;
    
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Called I/F: %s (run=0x%X, pend=0x%X, stop_only=%d) \n", 
               __FUNCTION__, 
               pt_running_svc_req,
               pt_pending_svc_req,
               b_stop_only ));
    
    if (pt_svctx->e_state != SVCTX_COND_READY) 
    {
        if (pt_pending_svc_req != NULL) 
        {
            rec_hdlr_invoke_client_svc_nfy_fct (pt_svctx,
                                                SVCTX_NTFY_CODE_INTERRUPTED,
                                                ST_UNKNOWN);
        } 
        else
        {
            rec_hdlr_invoke_client_svc_nfy_fct (pt_svctx,
                                                SVCTX_NTFY_CODE_IN_STOPPING,
                                                ST_UNKNOWN);
        }
    }    
    
    b_reuse_pipe = FALSE;
    if (pt_pending_svc_req != NULL && b_stop_only == FALSE) 
    {
        SVL_REC_T   t_svl_rec_running, t_svl_rec_pending;
        
        do
        {
            if (pt_running_svc_req->e_src_rec != SRC_TYPE_BRDCST_SVL ||
                pt_pending_svc_req->e_src_rec != SRC_TYPE_BRDCST_SVL)
            {
                break;
            }
            
            i4_ret = rec_svc_req_get_svl_rec (pt_running_svc_req,
                                              & t_svl_rec_running );
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get svl rec (%d). (L%d)\n", i4_ret, __LINE__));
                break;
            }
            
            i4_ret = rec_svc_req_get_svl_rec (pt_pending_svc_req,
                                              & t_svl_rec_pending );
            if (RET_FAIL (i4_ret))
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get svl rec (%d). (L%d)\n", i4_ret, __LINE__));
                break;
            }
            
            /* only running and pending (next request) are all digital, the "reuse pipe"
             * mechanism will be applied. */
            if (t_svl_rec_running.uheader.t_rec_hdr.e_brdcst_type == BRDCST_TYPE_ANALOG ||
                t_svl_rec_pending.uheader.t_rec_hdr.e_brdcst_type == BRDCST_TYPE_ANALOG)
            {
                break;
            }
            if(!pt_pending_svc_req->u_src.t_svl.t_param.b_ninja_change)
            {
                if (t_svl_rec_running.uheader.t_rec_hdr.ui2_tsl_id     == t_svl_rec_pending.uheader.t_rec_hdr.ui2_tsl_id &&
#ifndef MW_NOT_DISCONNECT_WHEN_NW_CHANGE
                t_svl_rec_running.uheader.t_rec_hdr.ui2_tsl_rec_id == t_svl_rec_pending.uheader.t_rec_hdr.ui2_tsl_rec_id)
#else
                t_svl_rec_running.uheader.t_rec_hdr.ui2_tsl_rec_id == t_svl_rec_pending.uheader.t_rec_hdr.ui2_tsl_rec_id &&
                pt_running_svc_req->u_src.t_svl.ui2_on_id == pt_pending_svc_req->u_src.t_svl.ui2_on_id &&
                pt_running_svc_req->u_src.t_svl.ui2_ts_id == pt_pending_svc_req->u_src.t_svl.ui2_ts_id &&
                pt_running_svc_req->u_src.t_svl.ui4_freq  == pt_pending_svc_req->u_src.t_svl.ui4_freq)
#endif
                {
#ifdef MW_TV_AV_BYPASS_SUPPORT
                    /*
                    if (t_svl_rec_running.uheader.t_rec_hdr.e_brdcst_type == BRDCST_TYPE_ATSC)
                    {
                        b_reuse_pipe = FALSE;
                    }
                    else*/ 
                    #if 0
                    if (pt_svctx->b_fake_disc)
                    {
                        SVCTX_T* pt_lead;

                        /* Check the consistency between lead and associated services */
                        pt_lead = svctx_mgr_get_link_lead (ACTIVE_LINK_NOW, 
                                                           pt_svctx->pt_running_svc_req->e_src_type);
                                                          
                        if (pt_lead) 
                        {
                        	x_dbg_stmt("{SVCTX} INFO:[REC_HDLR] Our fake (1) ,Lead's fake(%d) LINE(%d)",pt_lead->b_fake_disc,__LINE__);		
                            b_reuse_pipe = !pt_lead->b_fake_disc;
                        }
    					else
    					{
    					    x_dbg_stmt("<<<<<<<<<<<<<<<<<<Stop svc>>>>>>>>>>>>>>>>>>>>>>");
    						b_reuse_pipe = !(pt_svctx->b_fake_disc);
    					}
                    }
                    else
                    {
                        b_reuse_pipe = !svctx_is_reuse_pipe_disabled();
                    }
                    #else
                    
                    SVCTX_T* pt_lead;
                    pt_lead = svctx_mgr_get_link_lead(ACTIVE_LINK_NOW, pt_svctx->pt_running_svc_req->e_src_type);
                    if (pt_svctx->b_fake_disc) 
                    {
                        /* Check the consistency between lead and associated services */
                         if (pt_lead) 
                        {
                        	DBG_INFO(("{SVCTX} INFO:[REC_HDLR] Our fake (1) ,Lead's fake(%d) LINE(%d)",pt_lead->b_fake_disc,__LINE__));		
                            b_reuse_pipe = !pt_lead->b_fake_disc;
                        }
                    }
                    else 
                    {
                        /*keep consistent with main(ch_hdlr)*/
                        if (pt_lead && !pt_lead->b_fake_disc) 
                        {
                            /* Fix me! Too rough , must find a elegant way*/
                            if(((CH_SVC_REQ_T*)pt_lead->pt_running_svc_req)->pt_player)
                            {
                                if (((CH_SVC_REQ_T*)pt_lead->pt_running_svc_req)->pt_player->e_conn_cond == CM_COND_CONNECTED)
                                {
                                    b_reuse_pipe = !svctx_is_reuse_pipe_disabled();
                                }
                                else
                                {
                                    b_reuse_pipe = FALSE;
                                } 
                            }
                            else
                            {
                                b_reuse_pipe = FALSE;
                            }
                        } 
                        else 
                        {                     
                            b_reuse_pipe = FALSE;
                        }
                    }
                    #endif
#else
                    b_reuse_pipe = TRUE;
#endif                
                }
            }

            else
            {
                if(t_svl_rec_running.uheader.t_rec_hdr.ui2_tsl_id == t_svl_rec_pending.uheader.t_rec_hdr.ui2_tsl_id &&
                 t_svl_rec_running.uheader.t_rec_hdr.ui2_tsl_rec_id == t_svl_rec_pending.uheader.t_rec_hdr.ui2_tsl_rec_id)
                {
                    b_reuse_pipe = TRUE;
                }
                else
                {
                    b_reuse_pipe = FALSE;
                }
            }
        } while (FALSE);
        
    }
    
    if (b_reuse_pipe)
    {
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Judged to reuse pipe. (L%d)\n", __LINE__));
    }
    
    i4_ret = rec_svc_deinit_svc (pt_svctx, b_reuse_pipe);
    
    if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_PENDING)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to deinit svc (%d). (L%d)\n", i4_ret, __LINE__));
        return i4_ret;
    }

    return i4_ret;
}

static INT32 _rec_hdlr_unblock_svc_req (
    SVCTX_T*                    pt_svctx)
{
    INT32   i4_ret;
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Called I/F: %s () \n", __FUNCTION__));
    
    if (!pt_svctx->pt_running_svc_req)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong state to call _rec_hdlr_unblock_svc_req(). (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    i4_ret = rec_svc_update_block_status (pt_svctx);
    
    if (RET_FAIL (i4_ret))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to check svc block (%d). (L%d)\n", 
                    i4_ret, 
                    __LINE__));
    }

    return i4_ret;
}

static INT32 _rec_hdlr_internal_msg (
    SVCTX_T*                    pt_svctx,
    SVCTX_MSG_T*                pt_msg)
{
    INT32   i4_ret;
    
    i4_ret = rec_svc_internal_msg (pt_svctx,
                                   pt_msg);
    
    if (i4_ret == SVCTXR_REC_MSG_SVC_DEINITED)
    {
        /* service is deinited */
        
        i4_ret = svctx_ntfy_svc_req_stopped (pt_svctx);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to call svctx_ntfy_svc_req_stopped () (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    
    return SVCTXR_OK;
}    

static INT32 _rec_hdlr_replace_svc_req (
    SVCTX_T*                    pt_svctx)
{
    REC_SVC_REQ_T*  pt_replacer = (REC_SVC_REQ_T*) pt_svctx->pt_running_svc_req;
    REC_SVC_REQ_T*  pt_replacee = (REC_SVC_REQ_T*) pt_svctx->pt_pending_svc_req;
    INT32           i4_ret;
    
    DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Called I/F: %s (run=0x%X, pend=0x%X) \n", 
               __FUNCTION__,
               pt_svctx->pt_running_svc_req,
               pt_svctx->pt_pending_svc_req));
    
    i4_ret = SVCTXR_FAIL;
    do
    {
        /* check if the same service */
        if (!rec_svc_req_equals (pt_replacer,
                                 pt_replacee ))
        {
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (!pt_svctx->b_svc_selected)
        {
            i4_ret = SVCTXR_FAIL;
            break;
        }
#endif        
        
        DBG_INFO (("{SVCTX} INFO: [REC_HDLR] Ignore the same service to replace. \n"));
        i4_ret = SVCTXR_OK;
        
    } while (0);
    
    return i4_ret;
}

static INT32 _rec_hdlr_set (
    SVCTX_T*                    pt_svctx,
    SVCTX_SET_TYPE_T            e_set_type,
    VOID*                       pv_set_info,
    SIZE_T                      z_set_info_size)
{
    REC_SVC_REQ_T*      pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32               i4_ret;
    
    /* handle static commands first */
    switch (e_set_type)
    {
    case SVCTX_REC_SET_TYPE_DEL_REC_DATA:
    {
        SVCTX_REC_DEL_DATA_T*  pt_del_info = (SVCTX_REC_DEL_DATA_T *) pv_set_info;
        
        if (pt_del_info == NULL                              ||
            z_set_info_size != sizeof (SVCTX_REC_DEL_DATA_T) ||
            pt_del_info->ui4_strct_ver != 1                  ||
            pt_del_info->s_head_file == NULL                 ||
            x_strlen (pt_del_info->s_head_file) == 0)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid rec data del info. (L%d)\n", __LINE__));
            return SVCTXR_INV_SET_INFO;
        }
        
        i4_ret = rec_svc_util_clear_rec_data (pt_del_info->s_head_file);
        
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to clear record data (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;
    }
    
    case SVCTX_REC_SET_TYPE_META_DB_OPEN:
    {
        SVCTX_REC_META_DB_OPEN_T*   pt_mdb_open = (SVCTX_REC_META_DB_OPEN_T *) pv_set_info;
        
        if (pt_mdb_open == NULL)
        {
            return SVCTXR_INV_SET_INFO;
        }
        
        i4_ret = _rec_hdlr_meta_db_open (pt_svctx, pt_mdb_open);
        
        break;   
    }
    
    case SVCTX_REC_SET_TYPE_META_DB_CLOSE:
    {
        SVCTX_REC_META_DB_CLOSE_T*  pt_mdb_close = (SVCTX_REC_META_DB_CLOSE_T *) pv_set_info;

        if (pt_mdb_close == NULL)
        {
            return SVCTXR_INV_SET_INFO;
        }

        i4_ret = _rec_hdlr_meta_db_close (pt_svctx, pt_mdb_close);
        
        break;
    }
    
    case SVCTX_REC_SET_TYPE_META_DB_STORE:
    {
        SVCTX_REC_META_DB_STORE_T*  pt_mdb_store = (SVCTX_REC_META_DB_STORE_T *) pv_set_info;
        
        if (pt_mdb_store == NULL)
        {
            return SVCTXR_INV_SET_INFO;
        }
        
        i4_ret = _rec_hdlr_meta_db_store (pt_svctx, pt_mdb_store);
        
        break;
    } 
    
    case SVCTX_REC_SET_TYPE_META_INFO:
    {
        SVCTX_REC_SET_META_INFO_T*  pt_mdb_set_info = (SVCTX_REC_SET_META_INFO_T *) pv_set_info;
        
        if (pt_mdb_set_info == NULL)
        {
            return SVCTXR_INV_SET_INFO;
        }
        
        i4_ret = _rec_hdlr_meta_db_set_info (pt_svctx, pt_mdb_set_info);
        
        break;
    }
    
    default:
        i4_ret = SVCTXR_CONTINUE;        
        
    }
    
    if (i4_ret != SVCTXR_CONTINUE)
    {
        return i4_ret;
    }
    
    if (pt_running_svc_req == NULL)
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] No running service request. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    switch (e_set_type)
    {
    case SVCTX_REC_SET_TYPE_START_REC:
    {
        SVCTX_REC_CONFIG_T*  pt_rec_cfg = (SVCTX_REC_CONFIG_T *) pv_set_info;
        
        if (pt_rec_cfg == NULL                              ||
            z_set_info_size != sizeof (SVCTX_REC_CONFIG_T)  ||
            ! _rec_hdlr_check_valid_rec_cfg (pt_rec_cfg) )
        {
            DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Invalid record config. (L%d)\n", __LINE__));
            return SVCTXR_INV_SET_INFO;
        }
        
        i4_ret = rec_svc_req_start_rec (pt_svctx, pt_rec_cfg);
        break;
    }

    case SVCTX_REC_SET_TYPE_STOP_REC:
        i4_ret = rec_svc_req_stop_rec (pt_svctx);
        break;

    case SVCTX_REC_SET_TYPE_PAUSE_REC:
        i4_ret = rec_svc_req_pause_rec (pt_svctx);
        break;

    case SVCTX_REC_SET_TYPE_RESUME_REC:
        i4_ret = rec_svc_req_resume_rec (pt_svctx);
        break;
    
    case SVCTX_REC_SET_TYPE_ES_LIST:
    {
        SVCTX_REC_ES_LIST_T*    pt_es_list = (SVCTX_REC_ES_LIST_T *) pv_set_info;
        UINT16                  ui2_i, ui2_k;
        
        if (pt_es_list == NULL                                      ||
            pt_es_list->ui4_strct_ver != 1                          ||
            z_set_info_size != sizeof (SVCTX_REC_ES_LIST_T)         ||
            ( pt_es_list->ui2_es_num != 0 &&
              pt_es_list->at_es_info == NULL)                       ||
            ( pt_es_list->ui2_idx_prim_video != SVCTX_REC_ES_LIST_IDX_NULL &&
              pt_es_list->ui2_idx_prim_video >= pt_es_list->ui2_es_num) )
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid ES List info. (L%d)\n", __LINE__));
            return SVCTXR_INV_SET_INFO;
        }
        
        if (pt_es_list->ui2_es_num > REC_SVC_MAX_RCRD_STRM_NUM)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Too many ES. (L%d)\n", __LINE__));
            return SVCTXR_INV_SET_INFO;
        }
        
        /* check if there's duplicated pid in at_es_info */
        if (pt_es_list->ui2_es_num > 0)
        {
            for (ui2_i = 0; ui2_i < pt_es_list->ui2_es_num; ui2_i ++)
            {
                for (ui2_k = 0; ui2_k < pt_es_list->ui2_es_num; ui2_k ++)
                {
                    if ((ui2_i != ui2_k)        &&
                        (pt_es_list->at_es_info[ui2_k].t_pid == pt_es_list->at_es_info[ui2_i].t_pid))
                    {
                        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Duplicated PID found. (L%d)\n", __LINE__));
                        return SVCTXR_INV_SET_INFO;
                    }
                }
            }
        }
    
        i4_ret = rec_svc_req_set_es_list (pt_svctx, pt_es_list);
        
        break;
    }    
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case SVCTX_INTL_COMMON_SET_TYPE_QUEUED_REQ_REMOVED:
    {
        if (pv_set_info != NULL ||
            z_set_info_size != 0)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid arguments. (L%d)\n", __LINE__));
            return SVCTXR_INV_SET_INFO;
        }
        
        i4_ret = rec_svc_proc_queued_req_removed (pt_svctx);
        if (RET_FAIL (i4_ret))
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to handle queued req removed (%d). (L%d)\n", i4_ret, __LINE__));
        }
        break;
    }
#endif

    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid set type (%d). (L%d)\n", e_set_type, __LINE__));
        i4_ret = SVCTXR_INV_COMMAND;
    }
    
    return i4_ret;
}

static INT32 _rec_hdlr_get (
    SVCTX_T*                    pt_svctx,
    SVCTX_GET_TYPE_T            e_get_type,
    VOID*                       pv_get_info,
    SIZE_T*                     pz_get_info_size)
{
    REC_SVC_REQ_T*      pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32               i4_ret;

    switch (e_get_type)
    {
        case SVCTX_REC_GET_TYPE_META_INFO_LEN:
        {
            SVCTX_REC_GET_META_INFO_LEN_T* pt_get_info_len = (SVCTX_REC_GET_META_INFO_LEN_T *) pv_get_info;
            
            if (*pz_get_info_size != sizeof (SVCTX_REC_GET_META_INFO_LEN_T) ||
                pt_get_info_len == NULL)
            {
                return SVCTXR_INV_GET_INFO;
            }
            
            *pz_get_info_size = sizeof (SVCTX_REC_GET_META_INFO_LEN_T);
            
            i4_ret = _rec_hdlr_meta_db_get_info_len (pt_svctx, pt_get_info_len);
            
            break;
        } 
        
        case SVCTX_REC_GET_TYPE_META_INFO:
        {
            SVCTX_REC_GET_META_INFO_T*  pt_get_info = (SVCTX_REC_GET_META_INFO_T *) pv_get_info;
            
            if (*pz_get_info_size != sizeof (SVCTX_REC_GET_META_INFO_T) ||
                pt_get_info == NULL)
            {
                return SVCTXR_INV_GET_INFO;
            }

            *pz_get_info_size = sizeof (SVCTX_REC_GET_META_INFO_T);
            
            i4_ret = _rec_hdlr_meta_db_get_info (pt_svctx, pt_get_info);
            
            break;
        }
        
        default:
            i4_ret = SVCTXR_CONTINUE;    
    }
    
    if (i4_ret != SVCTXR_CONTINUE)
    {
        return i4_ret;
    }

    if (pt_running_svc_req == NULL)
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] No running service request. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    switch (e_get_type)
    {
    case SVCTX_REC_GET_TYPE_TICK_PERIOD:
    {
        UINT32*    pui4_tick_period = (UINT32 *) pv_get_info;
        
        if (*pz_get_info_size < sizeof (UINT32))
        {
            DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Invalid get info. (L%d)\n", __LINE__));
            return SVCTXR_INV_GET_INFO;
        }
        
        if (pt_running_svc_req->pt_rec_cfg == NULL)
        {
            DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Invalid state to get tick period. (L%d)\n", __LINE__));
            return SVCTXR_WRONG_STATE;
        }
        
        *pui4_tick_period = pt_running_svc_req->pt_rec_cfg->ui4_ms_tick_period;
        *pz_get_info_size = sizeof (UINT32);
        
        break;
    }
            
    case SVCTX_REC_GET_TYPE_CAPABILITY:
    {    
        SVCTX_REC_CAP_INFO_T*   pt_cap_info = (SVCTX_REC_CAP_INFO_T *) pv_get_info;
        
        if (*pz_get_info_size < sizeof (SVCTX_REC_CAP_INFO_T))
        {
            DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Invalid get info. (L%d)\n", __LINE__));
            return SVCTXR_INV_GET_INFO;
        }
        
        i4_ret = rec_svc_get_capability (pt_svctx,
                                         pt_cap_info );

        if (i4_ret != SVCTXR_OK)
        {
            DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Fail to get capability. (L%d)\n", __LINE__));
            return i4_ret;
        }                                          
        
        *pz_get_info_size = sizeof (SVCTX_REC_CAP_INFO_T);
        
        break;
    }
        
    case SVCTX_REC_GET_TYPE_SYS_RES_INFO:
    {
        SVCTX_REC_SYS_RES_INFO_T*  pt_res_info = (SVCTX_REC_SYS_RES_INFO_T *) pv_get_info;
        
        if (*pz_get_info_size < sizeof (SVCTX_REC_SYS_RES_INFO_T))
        {
            DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Invalid get info. (L%d)\n", __LINE__));
            return SVCTXR_INV_GET_INFO;
        }
        
        i4_ret = rec_svc_get_sys_res_info (pt_svctx,
                                           pt_res_info);

        if (i4_ret != SVCTXR_OK)
        {
            DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Fail to get sys res info. (L%d)\n", __LINE__));
            return i4_ret;
        }                                          
        
        *pz_get_info_size = sizeof (SVCTX_REC_SYS_RES_INFO_T);
      
        break;
    }
        
    case SVCTX_REC_GET_TYPE_STATUS:
    {
        SVCTX_REC_STATUS_T*  pt_rec_status = (SVCTX_REC_STATUS_T *) pv_get_info;
        
        if (*pz_get_info_size < sizeof (SVCTX_REC_STATUS_T))
        {
            DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Invalid get info. (L%d)\n", __LINE__));
            return SVCTXR_INV_GET_INFO;
        }
        
        i4_ret = rec_svc_get_rec_status (pt_svctx,
                                         pt_rec_status);
        
        if (i4_ret != SVCTXR_OK)
        {
            DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Fail to get record status. (L%d)\n", __LINE__));
            return i4_ret;
        }                                          
        
        *pz_get_info_size = sizeof (SVCTX_REC_STATUS_T);
        
        break;
    }
    case SVCTX_REC_GET_TYPE_FILE_SIZE:
    {
        if(*pz_get_info_size != sizeof (UINT64))
        {
            DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Invalid get info. (L%d)\n", __LINE__));
            return SVCTXR_INV_GET_INFO;
        }
        
        i4_ret = rec_svc_get_file_size(pt_svctx,
                                       pv_get_info);
        
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Fail to get record status. (L%d)\n", __LINE__));
            return i4_ret;
        } 
        
        break;
    }
    default:
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Invalid get type (%d). (L%d)\n", e_get_type, __LINE__));
        return SVCTXR_INV_COMMAND;
    }        
    
    return SVCTXR_OK;
}    

static INT32 _rec_hdlr_open_scdb (
    SVCTX_T*                    pt_svctx,
    STREAM_TYPE_T               e_pipe_type,
    HANDLE_T*                   ph_scdb,
    VOID*                       pv_tag,
    x_scdb_nfy_fct              pf_nfy)
{
    REC_SVC_REQ_T*      pt_running_svc_req = (REC_SVC_REQ_T *) pt_svctx->pt_running_svc_req;
    INT32               i4_ret;
    
    if (pt_running_svc_req == NULL)
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] No running service request. (L%d)\n", __LINE__));
        return SVCTXR_WRONG_STATE;
    }
    
    i4_ret = rec_svc_open_scdb (pt_svctx, 
                                ph_scdb,
                                pv_tag,
                                pf_nfy);

    if (i4_ret != SCDBR_OK)
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Fail to open SCDB (%d). (L%d)\n", i4_ret, __LINE__));
        return SVCTXR_FAIL;
    }
    
    return SVCTXR_OK;
}    

static BOOL _rec_hdlr_is_handler (
    SRC_TYPE_T                  e_src_type)
{
    BOOL     b_ret;
    
    switch(e_src_type) 
    {
        case SRC_TYPE_RECORD:
            b_ret = TRUE;
            break;
            
        default:
            b_ret = FALSE;
    }

    return b_ret;
}

static VOID _rec_hdlr_deinit (VOID)
{
    /* TODO */
    
    rec_svc_req_deinit();
    crnt_evn_deinit();
    
    b_g_init = FALSE;
}

static INT32 _rec_hdlr_init (VOID)
{
    SVCTX_HDLR_FCT_TABLE_T t_fct_table = {0};
    INT32                  i4_ret;

    if (b_g_init == TRUE) 
    {
        return SVCTXR_OK;
    }
    
    /* check handler specific message's size */
    if (!(sizeof (SVCTX_REC_CM_PIPE_NFY_MSG_T)     <= SVCTX_MAX_HDLR_MSG_SIZE &&
          sizeof (SVCTX_REC_CONN_NFY_MSG_T)        <= SVCTX_MAX_HDLR_MSG_SIZE &&
          sizeof (SVCTX_REC_PAT_NFY_MSG_T)         <= SVCTX_MAX_HDLR_MSG_SIZE &&
          sizeof (SVCTX_REC_RRCTX_MSG_T)           <= SVCTX_MAX_HDLR_MSG_SIZE &&
          sizeof (SVCTX_REC_UTIL_NFY_MSG_T)        <= SVCTX_MAX_HDLR_MSG_SIZE &&
          sizeof (SVCTX_REC_STRM_MSG_T)            <= SVCTX_MAX_HDLR_MSG_SIZE &&
          sizeof (SVCTX_REC_REQ_START_REC_MSG_T)   <= SVCTX_MAX_HDLR_MSG_SIZE &&
          sizeof (SVCTX_REC_REQ_SET_ES_LIST_MSG_T) <= SVCTX_MAX_HDLR_MSG_SIZE ))
    {
        return SVCTXR_FAIL;
    }
    
    /* check memory block size */
    if (!((sizeof (REC_UTIL_CONFIG_T) + 128)                         <= REC_SVC_MAX_MEM_BLK_SIZE &&
          (sizeof (SVCTX_REC_ES_INFO_T) * REC_SVC_MAX_RCRD_STRM_NUM) <= REC_SVC_MAX_MEM_BLK_SIZE))
    {
        return SVCTXR_FAIL;
    }

    /* check event data size */
    if (!(REC_SVC_MAX_EVN_DATA_SIZE >= sizeof (SVCTX_REC_CM_PIPE_NFY_MSG_T)     &&
          REC_SVC_MAX_EVN_DATA_SIZE >= sizeof (SVCTX_REC_CONN_NFY_MSG_T)        &&
          REC_SVC_MAX_EVN_DATA_SIZE >= sizeof (SVCTX_SESS_MSG_T)                &&
          REC_SVC_MAX_EVN_DATA_SIZE >= sizeof (SVCTX_REC_STRM_MSG_T)            &&
          REC_SVC_MAX_EVN_DATA_SIZE >= sizeof (REC_SVC_EVN_STRM_CLOSED_INFO_T)  &&
          REC_SVC_MAX_EVN_DATA_SIZE >= sizeof (SVCTX_REC_PAT_NFY_MSG_T)         &&
          REC_SVC_MAX_EVN_DATA_SIZE >= sizeof (SVCTX_REC_UTIL_NFY_MSG_T)        &&
          REC_SVC_MAX_EVN_DATA_SIZE >= sizeof (SVCTX_REC_REQ_START_REC_MSG_T)   &&
          REC_SVC_MAX_EVN_DATA_SIZE >= sizeof (SVCTX_REC_REQ_SET_ES_LIST_MSG_T) ))
    {
        return SVCTXR_FAIL;
    }
    
    t_fct_table.pf_allocate_svc_req           = _rec_hdlr_allocate_svc_req;
    t_fct_table.pf_free_svc_req               = _rec_hdlr_free_svc_req;
    t_fct_table.pf_create_svc_req             = _rec_hdlr_create_svc_req;
    t_fct_table.pf_create_surf_svc_req        = _rec_hdlr_create_surf_svc_req;  /* catcher */

    t_fct_table.pf_allocate_strm_req          = NULL;
    t_fct_table.pf_free_strm_req              = NULL;
    t_fct_table.pf_create_strm_req            = NULL;

    t_fct_table.pf_get_scdb_num_recs          = NULL;
    t_fct_table.pf_get_cp_info                = NULL;
    t_fct_table.pf_get_ca_info                = NULL;
    t_fct_table.pf_get_using_scdb_info        = NULL;
    t_fct_table.pf_get_strm_req_type          = NULL;
    t_fct_table.pf_get_strm_comp_id           = NULL;

    t_fct_table.pf_select_svc_req             = _rec_hdlr_select_svc;
    t_fct_table.pf_replace_svc_req            = _rec_hdlr_replace_svc_req;
    t_fct_table.pf_stop_svc_req               = _rec_hdlr_stop_svc;
    t_fct_table.pf_unblock_svc_req            = _rec_hdlr_unblock_svc_req;      /* catcher */
    t_fct_table.pf_is_blocked_source          = NULL;

    t_fct_table.pf_select_strm_req            = NULL;
    t_fct_table.pf_stop_strm_req              = NULL;
    t_fct_table.pf_internal_msg               = _rec_hdlr_internal_msg;
    t_fct_table.pf_set_session_attr           = NULL;
    t_fct_table.pf_get_session_attr           = NULL;
    t_fct_table.pf_set_stream_attr            = NULL;
    t_fct_table.pf_get_stream_attr            = NULL;

    t_fct_table.pf_get_signal_level           = NULL;
    t_fct_table.pf_get_dbm_signal_level       = NULL;
    t_fct_table.pf_get_vbi_broker             = NULL;
    t_fct_table.pf_get_stream_handle          = NULL;

    t_fct_table.pf_set                        = _rec_hdlr_set;
    t_fct_table.pf_get                        = _rec_hdlr_get;
    t_fct_table.pf_get_current_event_info_len = NULL;
    t_fct_table.pf_get_current_event_info     = NULL;

    t_fct_table.pf_freeze                     = NULL;
    t_fct_table.pf_unfreeze                   = NULL;
    
    t_fct_table.pf_open_scdb                  = _rec_hdlr_open_scdb;
    t_fct_table.pf_open_stream                = NULL;
    t_fct_table.pf_get_conn_attr              = NULL;
    t_fct_table.pf_set_conn_attr              = NULL;
    t_fct_table.pf_get_conn_op                = NULL;
    t_fct_table.pf_set_conn_op                = NULL;
    t_fct_table.pf_get_conn_handle            = NULL;
    
    t_fct_table.pf_is_handler                 = _rec_hdlr_is_handler;
    t_fct_table.pf_deinit                     = _rec_hdlr_deinit;
    
    i4_ret = svctx_register_handler (REC_DEFAULT_HDLR_NAME,
                                     & t_fct_table );
    
    if (i4_ret != SVCTXR_OK)
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] svctx_register_handler() failed, ret = %d)\n", i4_ret));
        return i4_ret;
    }
    
    i4_ret = rec_svc_req_init ((UINT16) (svctx_get_obj_amount() * SVC_REQ_AMOUNT_FACTOR) );
    
    if (i4_ret != SVCTXR_OK) 
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] ch_svc_req_init() failed, ret = %d)\n", i4_ret));
        return i4_ret;
    }
    
#ifdef CLI_LVL_ALL
    rec_hdlr_cli_attach_cmd_tbl();
#endif
    
    b_g_init = TRUE;

    return SVCTXR_OK;
    
}    

static BOOL _rec_hdlr_check_valid_rec_cfg (
    const SVCTX_REC_CONFIG_T*   pt_rec_cfg)
{
    if (!(pt_rec_cfg->ui4_strct_ver == 1         &&
          pt_rec_cfg->ui4_flags == 0             &&
          pt_rec_cfg->pf_rec_nfy != NULL         &&
          pt_rec_cfg->h_svctx_nfy != NULL_HANDLE && 
          pt_rec_cfg->ui4_ms_tick_period != 0))
    {
        return FALSE;        
    }
    
    switch (pt_rec_cfg->e_profile)
    {
    case SVCTX_REC_PROFILE_SIMPLE:
    case SVCTX_REC_PROFILE_ADVANCED:
        break;
        
    default:
        return FALSE;
    }
     
    switch (pt_rec_cfg->e_buf_mode)
    {
    case SVCTX_REC_BUF_MODE_PLAIN:
    case SVCTX_REC_BUF_MODE_RING:
        break;
    
    default:
        return FALSE;
    }
     
    if (pt_rec_cfg->pv_bound_info == NULL)
    {
        return FALSE;
    }
    
    switch (pt_rec_cfg->e_bound_type)
    {
    case SVCTX_REC_BOUND_TYPE_TIME:
    {
        SVCTX_REC_TIME_BOUND_INFO_T*    pt_binfo_time = 
                    (SVCTX_REC_TIME_BOUND_INFO_T *) pt_rec_cfg->pv_bound_info;

        if (!(pt_binfo_time->ui4_strct_ver == 1 && 
              pt_binfo_time->ui4_flags == 0     &&
              pt_binfo_time->ui4_ticks != 0     &&
              (pt_binfo_time->ui8_tsb_cap % pt_rec_cfg->t_strg_desc.ui4_sec_size) == 0 ))
        {
            return FALSE;
        }                                            
        break;
    }
        
    case SVCTX_REC_BOUND_TYPE_SPACE:
    {
        SVCTX_REC_SPACE_BOUND_INFO_T*    pt_binfo_space = 
                    (SVCTX_REC_SPACE_BOUND_INFO_T *) pt_rec_cfg->pv_bound_info;

        if (!(pt_binfo_space->ui4_strct_ver == 1 && 
              pt_binfo_space->ui4_flags == 0     &&
              (pt_binfo_space->ui8_size == SVCTX_REC_SPACE_BOUND_UNLIMIT ||
               pt_binfo_space->ui8_size != 0)    &&
              (pt_binfo_space->ui4_tick_cap == SVCTX_REC_SPACE_BOUND_TIME_UNLIMIT ||
               pt_binfo_space->ui4_tick_cap != 0) ))
        {
            return FALSE;
        }                                            
        break;
    }
    
    default:
        return FALSE;
    }
    
    if (!(pt_rec_cfg->s_db_name != NULL &&
          x_strlen (pt_rec_cfg->s_db_name) != 0))
    {
       return FALSE; 
    }
     
    if (!(pt_rec_cfg->t_strg_desc.ui4_strct_ver == 1             &&
          pt_rec_cfg->t_strg_desc.s_strg_path != NULL            &&
          x_strlen (pt_rec_cfg->t_strg_desc.s_strg_path) != 0    &&
          pt_rec_cfg->t_strg_desc.ui4_mem_align != 0             &&
          pt_rec_cfg->t_strg_desc.ui4_sec_size != 0              &&
          pt_rec_cfg->t_strg_desc.ui4_min_access_sec_ns != 0     &&
          (pt_rec_cfg->t_strg_desc.ui4_sec_size % pt_rec_cfg->t_strg_desc.ui4_mem_align) == 0
           /* [note] not to check max & best sector number          */
           /* pt_rec_cfg->t_strg_desc.ui4_max_access_sec_ns != 0 && */
           /* pt_rec_cfg->t_strg_desc.ui4_best_access_sec_ns != 0   */
        ))
    {
        return FALSE;
    }
    
    switch (pt_rec_cfg->e_stop_opt)
    {
    case SVCTX_REC_STOP_OPT_NO_CARE:
    case SVCTX_REC_STOP_OPT_FINALIZE:
        break;
    
    default:
        return FALSE;    
    }
    
    switch (pt_rec_cfg->e_cont_opt)
    {
    case SVCTX_REC_CONT_OPT_NORMAL:
    {
        SVCTX_REC_CONT_INFO_NORMAL_T*   pt_normal = 
            (SVCTX_REC_CONT_INFO_NORMAL_T *) pt_rec_cfg->pv_cont_info;
        
        if (!(pt_normal->ui4_strct_ver == 1 &&
              pt_normal->ui4_flags == 0 ))
        {
            return FALSE;
        }
        break;
    }
        
    case SVCTX_REC_CONT_OPT_CONT:
    {
        SVCTX_REC_CONT_INFO_CONT_T*     pt_cont = 
            (SVCTX_REC_CONT_INFO_CONT_T *) pt_rec_cfg->pv_cont_info;
        
        if (!(pt_cont->ui4_strct_ver == 1   &&
              pt_cont->ui4_flags == 0       &&
              x_strlen (pt_cont->s_strg_path) != 0 ))
        {
            return FALSE;
        }
        break;
    }
        
    default:
        return FALSE;
    }
    
    switch (pt_rec_cfg->e_no_data_opt)
    {
    case SVCTX_REC_NO_DATA_OPT_KEEP_REC:
        break;
    
    case SVCTX_REC_NO_DATA_OPT_SKIP_REC:
        break;
        
    default:
        return FALSE;
    }
    
    if (!(x_strlen (pt_rec_cfg->s_lang) <= 3))
    {
        return FALSE;
    }
     
    return TRUE;
}


/*-----------------------------------------------------------------------------
                    Meta DB functions
 ----------------------------------------------------------------------------*/
static BOOL _rec_hdlr_meta_db_cb_rec_util_nfy (
    HANDLE_T                    h_rec_util,
    VOID*                       pv_nfy_tag,
    REC_UTIL_NTFY_CODE_T        e_ntfy_code,
    UINT32                      ui4_data)
{
    BOOL    b_ret;
    
    switch (e_ntfy_code)
    {
    case REC_UTIL_NTFY_CODE_COND_CHANGED:
        b_ret = FALSE;
        break;
        
    case REC_UTIL_NTFY_CODE_VALID_RANGE_UPDATED:
        b_ret = FALSE;
        break;
        
    case REC_UTIL_NTFY_CODE_VALID_RANGE_INFO_UPDATED:
        b_ret = FALSE;
        break;
        
    case REC_UTIL_NTFY_CODE_RESET_REQ:
        b_ret = TRUE;
        break;
        
    case REC_UTIL_NTFY_CODE_LOAD_FILE_ACK:
        b_ret = FALSE;
        break;
        
    case REC_UTIL_NTFY_CODE_STORE_FILE_ACK:
        b_ret = FALSE;
        break;
    
    case REC_UTIL_NTFY_CODE_WRITE_ACK:
        b_ret = FALSE;
        break;
        
    case REC_UTIL_NTFY_CODE_SET_CONFIG_ACK:
        b_ret = FALSE;
        break;
        
    default:
        b_ret = FALSE;
    }
    
    return b_ret;
}

static BOOL _rec_hdlr_meta_db_cb_handle_free (
     HANDLE_T                   h_handle,
     HANDLE_TYPE_T              e_type,
     VOID*                      pv_obj,
     VOID*                      pv_tag,
     BOOL                       b_req_handle)
{
    if (!b_req_handle)
    {
        x_mem_free (pv_obj);
        return TRUE;
    }
    else
    {
        /* not allow direct x_handle_free () */
        return FALSE;
    }
}    
    
static INT32 _rec_hdlr_meta_db_open (
    SVCTX_T*                    pt_svctx,
    SVCTX_REC_META_DB_OPEN_T*   pt_open)
{
    INT32           i4_ret;
    HANDLE_T        h_rec_util, h_mdb_new;
    BOOL            b_ru_inited, b_mdb_alloced;
    RH_MDB_CTX_T*   pt_mdb_new = NULL;
    
    if (pt_open->ui4_strct_ver != 1 ||
        x_strlen (pt_open->s_head_file) == 0)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid arguments to open Meta-DB. (L%d)\n", __LINE__));
        return SVCTXR_INV_ARG;
    }

    b_ru_inited   = FALSE;
    b_mdb_alloced = FALSE;
    h_mdb_new     = NULL_HANDLE;
    do
    {
        /* open rec_util */
        i4_ret = rec_util_open_by_file (pt_open->s_head_file,
                                        _rec_hdlr_meta_db_cb_rec_util_nfy,
                                        NULL,
                                        & h_rec_util );
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open REC_UTIL (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_REC_EXTNL_MODULE_FAIL;
            break;
        }
        b_ru_inited = TRUE;
        
        /* load rfi */
        i4_ret = rec_util_rfi_load (h_rec_util);
        if (i4_ret != REC_UTILR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to load RFI (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_REC_EXTNL_MODULE_FAIL;
            break;
        }
        
        /* allocate handle */
        i4_ret = handle_alloc_and_obj (SVCTXT_REC_HDLR_META_DB,
                                       NULL,
                                       sizeof (RH_MDB_CTX_T),
                                       _rec_hdlr_meta_db_cb_handle_free,
                                       & h_mdb_new,
                                       (VOID **) &pt_mdb_new);
        if (i4_ret != HR_OK || !pt_mdb_new)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to alloc MDB handle (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }
        i4_ret = SVCTXR_OK;
        b_mdb_alloced = TRUE;
                                  
        pt_mdb_new->h_rec_util = h_rec_util;
        
    } while (FALSE);        
        
    if (RET_OK (i4_ret))
    {
        pt_open->h_meta_db = h_mdb_new;
    }
    else
    {
        if (b_mdb_alloced)
        {
            handle_free (h_mdb_new, FALSE);
        }
        
        if (b_ru_inited)
        {
            rec_util_close (h_rec_util);
        }
    }
        
    return i4_ret;
}    

static INT32 _rec_hdlr_meta_db_close (
    SVCTX_T*                    pt_svctx,
    SVCTX_REC_META_DB_CLOSE_T*  pt_close)
{
    RH_MDB_CTX_T*   pt_mdb;
    HANDLE_TYPE_T   e_hdl_type;
    INT32           i4_ret;
    
    if (pt_close->ui4_strct_ver != 1 ||
        !x_handle_valid (pt_close->h_meta_db))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid arguments to close Meta-DB. (L%d)\n", __LINE__));
        return SVCTXR_INV_ARG;
    }
    
    if (HR_OK != handle_get_type_obj (pt_close->h_meta_db,
                                      & e_hdl_type,
                                     (VOID **) &pt_mdb ) ||
        e_hdl_type != SVCTXT_REC_HDLR_META_DB            ||
        pt_mdb == NULL )
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid handle. (L%d)\n", __LINE__));
        return SVCTXR_INV_HANDLE;
    }
    
    /* auto store before closed */
    i4_ret = rec_util_rfi_store (pt_mdb->h_rec_util);
    if (i4_ret != REC_UTILR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to store RFI (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    /* close rec_util */
    i4_ret = rec_util_close (pt_mdb->h_rec_util);
    if (i4_ret != REC_UTILR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close REC_UTIL (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    /* free mdb object */
    i4_ret = handle_free (pt_close->h_meta_db, FALSE);
    if (i4_ret != HR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to free mdb handle (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return SVCTXR_OK;
}    

static INT32 _rec_hdlr_meta_db_store (
    SVCTX_T*                    pt_svctx,
    SVCTX_REC_META_DB_STORE_T*  pt_store)
{
    HANDLE_TYPE_T   e_hdl_type;
    RH_MDB_CTX_T*   pt_mdb;
    INT32           i4_ret;
    
    if (pt_store->ui4_strct_ver != 1 ||
        !x_handle_valid (pt_store->h_meta_db))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid arguments to store Meta-DB. (L%d)\n", __LINE__));
        return SVCTXR_INV_ARG;
    }
    
    if (HR_OK != handle_get_type_obj (pt_store->h_meta_db,
                                      & e_hdl_type,
                                     (VOID **) &pt_mdb ) ||
        e_hdl_type != SVCTXT_REC_HDLR_META_DB            ||
        pt_mdb == NULL )
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid handle. (L%d)\n", __LINE__));
        return SVCTXR_INV_HANDLE;
    }
    
    i4_ret = rec_util_rfi_store (pt_mdb->h_rec_util);
    if (i4_ret != REC_UTILR_OK)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to store RFI file (%d). (L%d)\n", i4_ret, __LINE__));
        return SVCTXR_REC_EXTNL_MODULE_FAIL;
    }
    
    return SVCTXR_OK;
}    

static INT32 _rec_hdlr_meta_db_set_info (
    SVCTX_T*                    pt_svctx,
    SVCTX_REC_SET_META_INFO_T*  pt_set_info)
{
    HANDLE_TYPE_T   e_hdl_type;
    RH_MDB_CTX_T*   pt_mdb;
    INT32           i4_ret;
    
    if (pt_set_info->ui4_strct_ver != 1                            ||
        !x_handle_valid (pt_set_info->h_meta_db)                   ||
        pt_set_info->e_key_type == SVCTX_REC_META_KEY_TYPE_UNKNOWN ||
        !(pt_set_info->z_data_len == 0 ||
          pt_set_info->pv_data != NULL))
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid arguments to store Meta-DB. (L%d)\n", __LINE__));
        return SVCTXR_INV_ARG;
    }
    
    if (HR_OK != handle_get_type_obj (pt_set_info->h_meta_db,
                                      & e_hdl_type,
                                     (VOID **) &pt_mdb ) ||
        e_hdl_type != SVCTXT_REC_HDLR_META_DB            ||
        pt_mdb == NULL )
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid handle. (L%d)\n", __LINE__));
        return SVCTXR_INV_HANDLE;
    }
    
    i4_ret = rec_util_rfi_set_info (pt_mdb->h_rec_util,
                                    (UINT32) pt_set_info->e_key_type,
                                    pt_set_info->pv_data,
                                    pt_set_info->z_data_len);
    if (i4_ret != REC_UTILR_OK)
    {
        if (i4_ret == REC_UTILR_NOT_FOUND)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] RFI key id (%d) not found. (L%d)\n", pt_set_info->e_key_type, __LINE__));
            i4_ret = SVCTXR_REC_INFO_NOT_FOUND;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to set RFI info (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_REC_EXTNL_MODULE_FAIL;
        }
        return i4_ret;
    }
    
    return SVCTXR_OK;
}        

static INT32 _rec_hdlr_meta_db_get_info (
    SVCTX_T*                    pt_svctx,
    SVCTX_REC_GET_META_INFO_T*  pt_get_info)
{
    HANDLE_TYPE_T   e_hdl_type;
    RH_MDB_CTX_T*   pt_mdb;
    SIZE_T          z_len_in_rfi;
    INT32           i4_ret;
    
    if (pt_get_info->ui4_strct_ver != 1          ||
        !x_handle_valid (pt_get_info->h_meta_db) ||
        pt_get_info->e_key_type == SVCTX_REC_META_KEY_TYPE_UNKNOWN )
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Invalid get info. (L%d)\n", __LINE__));
        return SVCTXR_INV_GET_INFO;
    }
    
    if (HR_OK != handle_get_type_obj (pt_get_info->h_meta_db,
                                      & e_hdl_type,
                                     (VOID **) &pt_mdb ) ||
        e_hdl_type != SVCTXT_REC_HDLR_META_DB            ||
        pt_mdb == NULL )
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid handle. (L%d)\n", __LINE__));
        return SVCTXR_INV_HANDLE;
    }
    
    i4_ret = rec_util_rfi_get_info_len (pt_mdb->h_rec_util,
                                        (UINT32) pt_get_info->e_key_type,
                                        & z_len_in_rfi);
    if (i4_ret != REC_UTILR_OK)
    {
        if (i4_ret == REC_UTILR_NOT_FOUND)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] RFI key id (%d) not found. (L%d)\n", pt_get_info->e_key_type, __LINE__));
            i4_ret = SVCTXR_REC_INFO_NOT_FOUND;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RFI info len (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_REC_EXTNL_MODULE_FAIL;
        }
        return i4_ret;
    }

    if (pt_get_info->z_buf_len < z_len_in_rfi ||
        pt_get_info->pv_buf == NULL)
    {
        return SVCTXR_REC_BUFF_TOO_SMALL;
    }
    
    pt_get_info->z_data_len = pt_get_info->z_buf_len;
    i4_ret = rec_util_rfi_get_info (pt_mdb->h_rec_util,
                                    (UINT32) pt_get_info->e_key_type,
                                    pt_get_info->pv_buf,
                                    & pt_get_info->z_data_len);
    if (i4_ret != REC_UTILR_OK)
    {
        if (i4_ret == REC_UTILR_NOT_FOUND)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] RFI key id (%d) not found. (L%d)\n", pt_get_info->e_key_type, __LINE__));
            i4_ret = SVCTXR_REC_INFO_NOT_FOUND;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RFI info (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_REC_EXTNL_MODULE_FAIL;
        }
        return i4_ret;
    }

    return SVCTXR_OK;    
}

static INT32 _rec_hdlr_meta_db_get_info_len (
    SVCTX_T*                       pt_svctx,
    SVCTX_REC_GET_META_INFO_LEN_T* pt_get_info_len)
{
    HANDLE_TYPE_T   e_hdl_type;
    RH_MDB_CTX_T*   pt_mdb;
    INT32           i4_ret;
    
    if (pt_get_info_len->ui4_strct_ver != 1          ||
        !x_handle_valid (pt_get_info_len->h_meta_db) ||
        pt_get_info_len->e_key_type == SVCTX_REC_META_KEY_TYPE_UNKNOWN)
    {
        DBG_ERROR(("{SVCTX}  ERR: [REC_HDLR] Invalid get info. (L%d)\n", __LINE__));
        return SVCTXR_INV_GET_INFO;
    }

    if (HR_OK != handle_get_type_obj (pt_get_info_len->h_meta_db,
                                      & e_hdl_type,
                                     (VOID **) &pt_mdb ) ||
        e_hdl_type != SVCTXT_REC_HDLR_META_DB            ||
        pt_mdb == NULL )
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Invalid handle. (L%d)\n", __LINE__));
        return SVCTXR_INV_HANDLE;
    }
    
    i4_ret = rec_util_rfi_get_info_len (pt_mdb->h_rec_util,
                                        (UINT32) pt_get_info_len->e_key_type,
                                        & pt_get_info_len->z_data_len );
    if (i4_ret != REC_UTILR_OK)
    {
        if (i4_ret == REC_UTILR_NOT_FOUND)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] RFI key id (%d) not found. (L%d)\n", pt_get_info_len->e_key_type, __LINE__));
            i4_ret = SVCTXR_REC_INFO_NOT_FOUND;
        }
        else
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get RFI info len (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_REC_EXTNL_MODULE_FAIL;
        }
        return i4_ret;
    }
    
    return SVCTXR_OK;
}
    
/*-----------------------------------------------------------------------------
                    public functions implementations
 ----------------------------------------------------------------------------*/
VOID rec_hdlr_invoke_client_svc_nfy_fct(
    SVCTX_T*                    pt_svctx,
    SVCTX_NTFY_CODE_T           e_code,
    STREAM_TYPE_T               e_stream_type)
{
    REC_SVC_REQ_T* pt_running_svc_req = (REC_SVC_REQ_T*) pt_svctx->pt_running_svc_req;

    if (pt_running_svc_req == NULL)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Internal Error. (L%d)\n", __LINE__));
        return ;
    }

    switch (e_code) 
    {
    case SVCTX_NTFY_CODE_SIGNAL_LOCKED:
        pt_svctx->b_signal_loss_notified = FALSE;
        pt_svctx->e_signal_status        = SIGNAL_STATUS_LOCKED;
        pt_svctx->b_enable_snow          = FALSE;
        
        if (pt_running_svc_req->e_cond_conn != CM_COND_CONNECTED)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Wrong conn-cond (%d) when notify Signal Locked. (L%d)\n", pt_running_svc_req->e_cond_conn, __LINE__));
            return;
        }
        break;
        
    case SVCTX_NTFY_CODE_SIGNAL_LOSS:
        
        /* TODO */
        pt_svctx->b_signal_loss_notified = TRUE;
        pt_svctx->e_signal_status = SIGNAL_STATUS_LOSS;
    
        break;
        
    default:
        
        /* not to send out as signal loss (for svctx_delay_no_video_related_nfy() ) */
        if (pt_svctx->e_signal_status == SIGNAL_STATUS_LOSS) 
        {
            return;
        }
        
    }
    
    svctx_notify_clients_svc_msg(pt_svctx,
                                 e_code,
                                 e_stream_type,
                                 pt_svctx->pt_running_svc_req->h_svctx,
                                 pt_svctx->pt_running_svc_req->pf_svc_nfy );
    
    
}

VOID rec_hdlr_set_dbg_level (
    UINT16                      ui2_dbg_level)
{
    ui2_g_rec_hdlr_dbg_level = ui2_dbg_level;
}        

UINT16 rec_hdlr_get_dbg_level (VOID)
{
    return ui2_g_rec_hdlr_dbg_level;
}    

/*-----------------------------------------------------------------------------
                    exported functions implementations
 ----------------------------------------------------------------------------*/

INT32 x_rec_hdlr_reg_handler(VOID)
{
    return _rec_hdlr_init();
}

#endif
