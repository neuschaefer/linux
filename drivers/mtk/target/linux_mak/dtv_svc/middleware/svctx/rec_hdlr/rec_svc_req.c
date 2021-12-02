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
#include "svctx/rec_hdlr/rec_svc_req.h"
#include "svl/x_svl.h"
#include "tsl/x_tsl.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#define DBG_LEVEL_MODULE     rec_hdlr_get_dbg_level()


/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

static BOOL                      b_g_svc_req_init = FALSE;
static DLIST_T(_REC_SVC_REQ_T)   t_g_using_svc_req_q;
static DLIST_T(_REC_SVC_REQ_T)   t_g_pooled_svc_req_q;
static REC_SVC_REQ_T*            at_g_svc_req_objs = NULL;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static VOID _rec_svc_req_reset (
    REC_SVC_REQ_T*         pt_this);

static BOOL _rec_svc_req_valid (
    const REC_SVC_REQ_T*    pt_this);

static BOOL _rec_svc_req_check_same_digital_svc (
    REC_SVC_REQ_T*          pt_chker,
    REC_SVC_REQ_T*          pt_chkee);

/*-----------------------------------------------------------------------------
                    public functions 
 ----------------------------------------------------------------------------*/
INT32 rec_svc_req_init(
    UINT16                      ui2_amount)
{
    UINT16      ui2_i;
    
    if (b_g_svc_req_init == TRUE) 
    {
        return SVCTXR_OK;
    }
    
    at_g_svc_req_objs = (REC_SVC_REQ_T *) x_mem_alloc (sizeof (REC_SVC_REQ_T) * ui2_amount );
    
    if (at_g_svc_req_objs == NULL)
    {
        return SVCTXR_NO_RESOURCE;
    }
    
    x_memset (at_g_svc_req_objs, 0, sizeof(REC_SVC_REQ_T) * ui2_amount);
    
    DLIST_INIT (& t_g_using_svc_req_q);
    DLIST_INIT (& t_g_pooled_svc_req_q);
    
    for (ui2_i = 0; ui2_i < ui2_amount; ui2_i ++)
    {
        DLIST_INSERT_TAIL (& at_g_svc_req_objs[ui2_i],
                           & t_g_pooled_svc_req_q,
                           t_dlink);
    }
    
    b_g_svc_req_init = TRUE;

    return SVCTXR_OK;
}    

VOID rec_svc_req_deinit(VOID)
{
    if (b_g_svc_req_init == FALSE) 
    {
        return;
    }

    DLIST_INIT (& t_g_using_svc_req_q);
    DLIST_INIT (& t_g_pooled_svc_req_q);

    x_mem_free(at_g_svc_req_objs);

    b_g_svc_req_init = FALSE;
}    

INT32 rec_svc_req_allocate (
    REC_SVC_REQ_T**             ppt_this)
{
    REC_SVC_REQ_T*  pt_this = NULL;
    HANDLE_T        h_timer_conn, h_timer_wd;
    BOOL            b_svc_req_alloced; 
    INT32           i4_ret;
    
    if (!b_g_svc_req_init) 
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] SVC Request Object module not init. (L%d)\n", __LINE__));
        return SVCTXR_NOT_INIT;
    }
    
    if (ppt_this == NULL) 
    {
        return SVCTXR_INV_ARG;
    }

    h_timer_conn = NULL_HANDLE;
    h_timer_wd   = NULL_HANDLE;
    b_svc_req_alloced = FALSE;
    i4_ret = SVCTXR_OK;
    
    do 
    {
        pt_this = DLIST_HEAD(&t_g_pooled_svc_req_q);
        
        if (pt_this == NULL) 
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] No more SVC Request Object available. (L%d)\n", __LINE__));
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }
        
        DLIST_REMOVE(pt_this, 
                     & t_g_pooled_svc_req_q, 
                     t_dlink);
                     
        DLIST_INSERT_HEAD(pt_this, 
                          & t_g_using_svc_req_q, 
                          t_dlink);
                          
        b_svc_req_alloced = TRUE;
        
        _rec_svc_req_reset (pt_this);
        
        i4_ret = x_timer_create (&h_timer_wd);

        if (i4_ret != OSR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to create watchdog timer (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        i4_ret = x_timer_create (&h_timer_conn);
        
        if (i4_ret != OSR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to create conn timer (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        pt_this->h_timer_wd      = h_timer_wd;
        pt_this->b_on_timer_wd   = FALSE;
        pt_this->h_timer_conn    = h_timer_conn;
        pt_this->b_on_timer_conn = FALSE;
        
    } while (FALSE);
    
    if (RET_OK (i4_ret))
    {
        *ppt_this = pt_this;
    }
    else
    {
        if (h_timer_wd != NULL_HANDLE)
        {
            x_timer_delete (h_timer_wd);
        }
        
        if (h_timer_conn != NULL_HANDLE)
        {
            x_timer_delete (h_timer_conn);
        }
        
        if (b_svc_req_alloced)
        {
            DLIST_REMOVE (pt_this, 
                          & t_g_using_svc_req_q, 
                          t_dlink);
                          
            DLIST_INSERT_HEAD (pt_this, 
                               & t_g_pooled_svc_req_q, 
                               t_dlink);
        }
        
        i4_ret = SVCTXR_FAIL;
    }
    
    return i4_ret;
}    


INT32 rec_svc_req_free (
    REC_SVC_REQ_T*              pt_this)
{

    if (!b_g_svc_req_init) 
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] SVC Request Object module not init. (L%d)\n", __LINE__));
        return SVCTXR_NOT_INIT;
    }
    
    if (!_rec_svc_req_valid (pt_this))
    {
        return SVCTXR_INV_ARG;
    }
    
    if (pt_this->pv_crnt_evn_data != (VOID *) pt_this->aui1_crnt_evn)
    {
        x_mem_free (pt_this->pv_crnt_evn_data);
    }
    
    if (pt_this->at_scdb_recs != pt_this->at_scdb_rec_buf)
    {
        x_mem_free ((VOID *) pt_this->at_scdb_recs);
    }
    
    if (pt_this->h_timer_wd != NULL_HANDLE)
    {
        x_timer_delete (pt_this->h_timer_wd);
        pt_this->h_timer_wd = NULL_HANDLE;
    }
    
    if (pt_this->h_timer_conn != NULL_HANDLE)
    {
        x_timer_delete (pt_this->h_timer_conn);
        pt_this->h_timer_conn = NULL_HANDLE;
    }
    
    if (pt_this->t_pvr.pv_fst_evn_ttl)
    {
        x_mem_free (pt_this->t_pvr.pv_fst_evn_ttl);
        pt_this->t_pvr.pv_fst_evn_ttl = NULL;
        pt_this->t_pvr.z_buf_fst_evn_ttl = 0;
        pt_this->t_pvr.z_fst_evn_ttl = 0;
    }
        
    DLIST_REMOVE (pt_this, 
                  & t_g_using_svc_req_q, 
                  t_dlink);
                  
    DLIST_INSERT_HEAD (pt_this, 
                       & t_g_pooled_svc_req_q, 
                       t_dlink);

    return SVCTXR_OK;
}    

BOOL rec_svc_req_equals (
    REC_SVC_REQ_T*          pt_chker,
    REC_SVC_REQ_T*          pt_chkee )
{
    if (pt_chkee == NULL || pt_chker == NULL) 
    {
        return FALSE;
    }
    
    if (pt_chker->e_src_rec != pt_chkee->e_src_rec) 
    {
        return FALSE;
    }
    
    if (x_strcmp (pt_chker->s_src_grp_name, 
                  pt_chkee->s_src_grp_name) != 0)
    {
        return FALSE;
    }
    
    if (pt_chker->e_src_rec == SRC_TYPE_BRDCST_SVL) 
    {
        REC_SVC_SRC_BRDCST_SVL_T*   pt_src_brdcst_svl_chker = & pt_chker->u_src.t_svl;
        REC_SVC_SRC_BRDCST_SVL_T*   pt_src_brdcst_svl_chkee = & pt_chkee->u_src.t_svl;
        REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl_chker = & pt_chker->u_res.t_svl;
        REC_SVC_RES_BRDCST_SVL_T*   pt_res_brdcst_svl_chkee = & pt_chkee->u_res.t_svl;
        
        if (pt_src_brdcst_svl_chker->t_param.ui2_svc_lst_id != 
            pt_src_brdcst_svl_chkee->t_param.ui2_svc_lst_id)
        {
            return FALSE;
        }
        
        if (pt_res_brdcst_svl_chker->e_brdcst_type != BRDCST_TYPE_ANALOG &&
            pt_res_brdcst_svl_chkee->e_brdcst_type != BRDCST_TYPE_ANALOG )
        {
            if (!_rec_svc_req_check_same_digital_svc (pt_chker,
                                                      pt_chkee))
            {
                return FALSE;
            }
        }
        
        if (pt_src_brdcst_svl_chker->t_param.t_silent.b_enable == TRUE) 
        {
            if (pt_src_brdcst_svl_chkee->t_param.t_silent.b_enable == TRUE) 
            {
                /* pt_chker = silent tuning v.s. pt_chkee = silent tuning */
                if (pt_src_brdcst_svl_chker->t_param.t_silent.ui2_svl_rec_id !=
                    pt_src_brdcst_svl_chkee->t_param.t_silent.ui2_svl_rec_id) 
                {
                    return FALSE;
                }
            } 
            else 
            {
                /* pt_chker = silent tuning v.s. pt_chkee = normal tuning */ 
                if (pt_src_brdcst_svl_chker->t_param.t_silent.ui2_svl_rec_id !=
                    pt_src_brdcst_svl_chkee->t_param.ui2_svl_rec_id) 
                {
                    return FALSE;
                }
            }
        } 
        else 
        {
            if (pt_src_brdcst_svl_chkee->t_param.t_silent.b_enable == TRUE) 
            {
                /* pt_chker = normal tuning v.s. pt_chkee = silent tuning */
                if (pt_src_brdcst_svl_chker->t_param.ui2_svl_rec_id !=
                    pt_src_brdcst_svl_chkee->t_param.t_silent.ui2_svl_rec_id) 
                {
                    return FALSE;
                }
            } 
            else 
            {
                /* pt_chker = normal tuning v.s. pt_chkee = normal tuning */
                if (pt_src_brdcst_svl_chker->t_param.ui2_svl_rec_id !=
                    pt_src_brdcst_svl_chkee->t_param.ui2_svl_rec_id) 
                {
                    return FALSE;
                }  
            }
        }
        
        if (pt_src_brdcst_svl_chker->t_param.b_detect_tv_sys != 
            pt_src_brdcst_svl_chkee->t_param.b_detect_tv_sys)
        {
            return FALSE;
        }
        
        if (pt_src_brdcst_svl_chker->t_param.e_scart_mode != 
            pt_src_brdcst_svl_chkee->t_param.e_scart_mode)
        {
            return FALSE;
        }
        
        if (pt_res_brdcst_svl_chker->e_brdcst_type == BRDCST_TYPE_ANALOG &&
            pt_res_brdcst_svl_chkee->e_brdcst_type == BRDCST_TYPE_ANALOG )
        {
            /* TODO: check b_fine_tune in Analog channel */
        }
        
        if ( x_strcmp (pt_src_brdcst_svl_chker->t_param.t_ch_encode.s_scart_name,
                       pt_src_brdcst_svl_chkee->t_param.t_ch_encode.s_scart_name ))
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
    
    if (x_strcmp (pt_chker->t_parent.s_snk_grp_name, 
                  pt_chkee->t_parent.s_snk_grp_name) != 0)
    {
        return FALSE;
    }
    
    return TRUE;
}
    
INT32 rec_svc_req_get_svl_rec (
    const REC_SVC_REQ_T*        pt_svc_req,
    SVL_REC_T*                  pt_svl_rec) 
{
    const REC_SVC_SRC_BRDCST_SVL_T*  pt_src_brdcst_svl;
    UINT32                           ui4_ver_id     = SVL_NULL_VER_ID;
    UINT16                           ui2_svl_rec_id = SVL_NULL_REC_ID;
    UINT16                           ui2_svl_lst_id;
    HANDLE_T                         h_svl;
    BOOL                             b_svl_opened;
    INT32                            i4_ret;
    
    switch (pt_svc_req->e_src_rec)
    {
    case SRC_TYPE_BRDCST_SVL:
        pt_src_brdcst_svl = & pt_svc_req->u_src.t_svl;
        break;
    
    default:
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Unsupported source type (%d). (L%d)\n", 
                    pt_svc_req->e_src_rec, 
                    __LINE__));
        return SVCTXR_NOT_SUPPORT;
    }
#ifdef SYS_MHP_SUPPORT
    if (pt_src_brdcst_svl->t_param.t_silent.b_enable)
    {
        ui2_svl_rec_id = pt_src_brdcst_svl->t_param.t_silent.ui2_svl_rec_id;
        ui2_svl_lst_id = pt_src_brdcst_svl->t_param.t_silent.ui2_svc_lst_id;
    }
    else
    {
        ui2_svl_rec_id = pt_src_brdcst_svl->t_param.ui2_svl_rec_id;
        ui2_svl_lst_id = pt_src_brdcst_svl->t_param.ui2_svc_lst_id;
    }
#else 
    if (pt_src_brdcst_svl->t_param.t_silent.b_enable)
    {
        ui2_svl_rec_id = pt_src_brdcst_svl->t_param.t_silent.ui2_svl_rec_id;
    }
    else
    {
        ui2_svl_rec_id = pt_src_brdcst_svl->t_param.ui2_svl_rec_id;        
    }
    ui2_svl_lst_id = pt_src_brdcst_svl->t_param.ui2_svc_lst_id;
#endif
    b_svl_opened = FALSE;
    
    do 
    {
        i4_ret = x_svl_open (ui2_svl_lst_id,
                             NULL,
                             NULL,
                             & h_svl );
        
        if (i4_ret != SVLR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open SVL (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        b_svl_opened = TRUE;
        
        i4_ret = x_svl_get_rec (h_svl,
                                ui2_svl_rec_id,
                                pt_svl_rec,
                                & ui4_ver_id );
    
        if (i4_ret != SVLR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get SVL Record (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
    } while (FALSE);
    
    if (b_svl_opened)
    {
        INT32   i4_ret_svl;
        
        i4_ret_svl = x_svl_close (h_svl);
        
        if (i4_ret_svl != SVLR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close SVL (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    
    return i4_ret;
}

INT32 rec_svc_req_get_tsl_rec (
    const REC_SVC_REQ_T*        pt_svc_req,
    const SVL_REC_T*            pt_svl_rec,
    TSL_REC_T*                  pt_tsl_rec)
{
    HANDLE_T        h_tsl;
    BOOL            b_tsl_opened;
    UINT32          ui4_ver_id = TSL_NULL_VER_ID;
    INT32           i4_ret;

    if (pt_svl_rec->uheader.t_rec_hdr.e_brdcst_type == BRDCST_TYPE_ANALOG)
    {
        DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Cannot get TSL info in Analog channel. (L%d)\n", __LINE__));
        return SVCTXR_INV_ARG;
    }
    
    b_tsl_opened = FALSE;
    
    do
    {
        i4_ret = x_tsl_open (pt_svl_rec->uheader.t_rec_hdr.ui2_tsl_id,
                             NULL,
                             NULL,
                             & h_tsl);
                             
        if (i4_ret != TSLR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to open TSL (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
        b_tsl_opened = TRUE;
        
        i4_ret = x_tsl_get_rec (h_tsl,
                                pt_svl_rec->uheader.t_rec_hdr.ui2_tsl_rec_id,
                                pt_tsl_rec,
                                & ui4_ver_id );
        if (i4_ret != TSLR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get TSL record (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret = SVCTXR_FAIL;
            break;
        }
        
    } while (FALSE);
    
    if (b_tsl_opened)
    {
        INT32   i4_ret_tmp;
        
        i4_ret_tmp = x_tsl_close (h_tsl);
        
        if (i4_ret_tmp != TSLR_OK)
        {
            DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to close TSL (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    
    return i4_ret;
}
    
/*-----------------------------------------------------------------------------
                    internal functions 
 ----------------------------------------------------------------------------*/
static VOID _rec_svc_req_reset (
    REC_SVC_REQ_T*         pt_this)
{
    UINT32      ui4_i;
    
    if (pt_this == NULL) 
    {
        return;
    }

    svctx_reset_svc_req (&pt_this->t_parent);
    
    pt_this->e_src_rec          = 0;
    pt_this->s_src_grp_name[0]  = '\0';
    x_memset (&pt_this->u_src.t_svl, 0, sizeof (REC_SVC_SRC_BRDCST_SVL_T));
    x_memset (&pt_this->u_src.t_freq, 0, sizeof (REC_SVC_SRC_BRDCST_FREQ_T)); 
    x_memset (&pt_this->u_src.t_avc, 0, sizeof (REC_SVC_SRC_AVC_T));
    pt_this->pt_rec_cfg         = NULL;
    x_memset (&pt_this->t_sess_key, 0, sizeof (MM_CRYPT_INFO_T));
    pt_this->pf_rec_nfy         = NULL;
    pt_this->pv_rec_nfy_tag     = NULL;
    pt_this->e_media_type       = REC_SVC_MEDIA_TYPE_INVALID;
    pt_this->e_state            = REC_SVC_STATE_NOT_INIT;
    pt_this->e_src_op_type      = REC_SVC_SRC_OP_TYPE_NONE;
    pt_this->ui4_src_op_flag    = 0;
    pt_this->ui4_st_flag        = 0;
    pt_this->ui2_num_q_evn      = 0;
    DLIST_INIT (&pt_this->t_evn_q);
    pt_this->ui4_sn_evn         = 0;
    pt_this->b_on_timer_wd      = FALSE;
    pt_this->h_timer_wd         = NULL_HANDLE;
    pt_this->h_cm_pipe          = NULL_HANDLE;
    pt_this->e_cond_cm_pipe     = CM_COND_CLOSED;
    pt_this->h_conn             = NULL_HANDLE;
    pt_this->e_cond_conn        = CM_COND_DISCONNECTED;
    pt_this->h_timer_conn       = NULL_HANDLE;
    pt_this->b_on_timer_conn    = FALSE;
    pt_this->e_auth_st          = SVCTX_REC_AUTH_STATUS_UNKNOWN;
    pt_this->b_svc_blked        = FALSE;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_this->t_svcst.e_phase      = PLAYER_ACTION_NONE;
    pt_this->t_svcst.ph_pipe      = & pt_this->h_cm_pipe;
    pt_this->t_svcst.pe_pipe_cond = & pt_this->e_cond_cm_pipe;
    pt_this->t_svcst.ph_conn      = & pt_this->h_conn;
    pt_this->t_svcst.pe_conn_cond = & pt_this->e_cond_conn;
    pt_this->t_svcst.ph_sess      = NULL;
    pt_this->t_svcst.pe_sess_cond = NULL;
#endif

    pt_this->b_act_q_in_mgr     = FALSE;
    x_memset (pt_this->aui1_act_msg_q, 0, REC_SVC_ACT_MSG_Q_MAX_NUM);
    pt_this->ui2_num_act_msg    = 0;
    pt_this->h_scdb             = NULL_HANDLE;
    pt_this->e_cond_scdb        = SCDB_CLOSED;
    pt_this->h_rrctx            = NULL_HANDLE;
    pt_this->pv_buf_rrctx       = NULL;
    pt_this->z_len_buf_rrctx    = 0;
    pt_this->pt_crnt_evn        = NULL;
    pt_this->pv_crnt_evn_data   = (VOID *) pt_this->aui1_crnt_evn;
    pt_this->z_crnt_evn_len     = REC_SVC_PRG_EVN_DATA_SIZE_INIT;
    pt_this->at_scdb_recs       = pt_this->at_scdb_rec_buf;
    pt_this->ui2_num_scdb_recs  = REC_SVC_SCDB_BUF_REC_NUM_INIT;
    pt_this->h_rec_util         = NULL_HANDLE;
    pt_this->e_cond_rec_util    = REC_UTIL_COND_TRANSITION;
    pt_this->ui4_req_id_set_cfg = 0;
    pt_this->h_rcrd_sess        = NULL_HANDLE;
    pt_this->e_cond_sess        = SM_SESS_COND_CLOSED;
    pt_this->ui4_sess_flag      = 0;
    pt_this->b_preset_conn_info = FALSE;
    x_memset (& pt_this->t_conn_info_reuse, 0, sizeof (REC_SVC_CONN_INFO_T));
    x_memset (& pt_this->t_conn_info_preset, 0, sizeof (REC_SVC_CONN_INFO_T));
    pt_this->h_conn_reusing_by_cm = NULL_HANDLE;
    DLIST_INIT (& pt_this->t_wking_strms);
    DLIST_INIT (& pt_this->t_alloc_blks);	
    x_memset (&pt_this->u_res.t_svl, 0, sizeof (REC_SVC_RES_BRDCST_SVL_T));
    /* for turning on the warning to error mechanism 10_09_13 */
    #if 0
    x_memset (&pt_this->u_res.t_freq, 0, sizeof (REC_SVC_RES_BRDCST_FREQ_T));
    x_memset (&pt_this->u_res.t_avc, 0, sizeof (REC_SVC_RES_AVC_T));
    #endif
    x_memset (pt_this->at_blk_pool, 0, sizeof (REC_SVC_MEM_BLK_T) * REC_SVC_POOL_MEM_BLK_NUM);
    DLIST_INIT (&pt_this->t_free_blks);
    for (ui4_i = 0; ui4_i < REC_SVC_POOL_MEM_BLK_NUM; ui4_i ++)
    {
        DLIST_INSERT_TAIL (& pt_this->at_blk_pool[ui4_i], 
                           & pt_this->t_free_blks,
                           t_dlink);
    }
    
    x_memset (pt_this->at_strm_pool, 0, sizeof (REC_SVC_STRM_INFO_T) * REC_SVC_POOL_RCRD_STRM_NUM);
    DLIST_INIT (&pt_this->t_free_strms);
    for (ui4_i = 0; ui4_i < REC_SVC_POOL_RCRD_STRM_NUM; ui4_i ++)
    {
        DLIST_INSERT_TAIL (& pt_this->at_strm_pool[ui4_i], 
                           & pt_this->t_free_strms,
                           t_dlink);
    }

    x_memset (pt_this->at_evn_pool, 0, sizeof (REC_SVC_EVN_INFO_T) * REC_SVC_EVN_Q_MAX_NUM);
    DLIST_INIT (&pt_this->t_free_evns);
    for (ui4_i = 0; ui4_i < REC_SVC_EVN_Q_MAX_NUM; ui4_i ++)
    {
        DLIST_INSERT_TAIL (& pt_this->at_evn_pool[ui4_i], 
                           & pt_this->t_free_evns,
                           t_dlink);
    }

    x_memset (pt_this->aui1_crnt_evn, 0, REC_SVC_PRG_EVN_DATA_SIZE_INIT);
    
#ifdef CLI_LVL_ALL 
    pt_this->b_inform_paused = FALSE;
#endif    
}
    

static BOOL _rec_svc_req_valid (
    const REC_SVC_REQ_T*    pt_this)
{
    REC_SVC_REQ_T*   pt_svc_req;
    
    DLIST_FOR_EACH(pt_svc_req, &t_g_using_svc_req_q, t_dlink) 
    {
        if (pt_svc_req == pt_this) 
        {
            return TRUE;
        }
    }
    
    DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] SVC Req(0x%08x) does not exist. (L%d)\n", pt_this, __LINE__));
    
    return FALSE;
}

static BOOL _rec_svc_req_check_same_digital_svc (
    REC_SVC_REQ_T*              pt_chker,
    REC_SVC_REQ_T*              pt_chkee)
{
    if (pt_chker->e_src_rec == SRC_TYPE_BRDCST_SVL &&
        pt_chkee->e_src_rec == SRC_TYPE_BRDCST_SVL)
    {
        if (pt_chker->u_src.t_svl.ui2_on_id  == pt_chkee->u_src.t_svl.ui2_on_id  &&
            pt_chker->u_src.t_svl.ui2_ts_id  == pt_chkee->u_src.t_svl.ui2_ts_id  &&
            pt_chker->u_src.t_svl.ui2_svc_id == pt_chkee->u_src.t_svl.ui2_svc_id )
        {
            return TRUE;
        }
    }
    
    return FALSE;
}    
    
#endif
