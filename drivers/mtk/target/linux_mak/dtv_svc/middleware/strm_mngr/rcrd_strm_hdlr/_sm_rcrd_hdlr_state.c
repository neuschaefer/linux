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
 * $RCSfile: _sm_rcrd_hdlr_state.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains Stream Manager's record stream state 
 *         transfer info.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/x_mm_common.h"
#include "handle/u_handle.h"
#include "handle/x_handle.h"
#include "util/x_lnk_list.h"
#include "os/inc/x_os.h"

#include "hash/hash_api.h"
#include "scdb/x_scdb_api.h"

#include "strm_mngr/rcrd_strm_hdlr/_sm_rcrd_hdlr_state.h"
#include "strm_mngr/rcrd_strm_hdlr/_sm_rcrd_hdlr_pat_pmt.h"
#include "strm_mngr/rcrd_strm_hdlr/_sm_rcrd_hdlr_rutil.h"
#include "strm_mngr/rcrd_strm_hdlr/_sm_rcrd_hdlr_pvr.h"

#include "strm_mngr/sess_rcrd_hdlr/u_sm_sess_rcrd.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_strm_state_record_latest_event
 *
 * Description: This function records latest events for doing state machine transfering 
 *
 * Inputs:  
 *       pt_rs_obj:    record stream handle
 *       e_event:      event e.g.PM-DMX/RM-PVR/Record Utiltiy/SCDB/APP
 *         
 * Outputs: 
 *       Record latest events in record stream object
 *
 * Returns: TRUE:  record.
 *          FALSE: has be recorded before
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
static BOOL _sm_rcrd_strm_state_record_latest_event(SM_RCRD_STREAM_T* pt_rs_obj,
                                                    UINT32            ui4_event)
{
    if (NULL == pt_rs_obj)
    {
        return FALSE;
    }
    
    switch (SM_EVN_GET_GRP(ui4_event))
    {
        case SM_EVN_RSH_GRP_PVR:
            if (pt_rs_obj->ui4_lastest_pvr_event == ui4_event)
            {
                return FALSE;
            }
            pt_rs_obj->ui4_lastest_pvr_event = ui4_event;
            break;

        case SM_EVN_RSH_GRP_PM:

            if (pt_rs_obj->ui4_lastest_pm_event == ui4_event)
            {
                return FALSE;
            }
            pt_rs_obj->ui4_lastest_pm_event = ui4_event;
            break;
            
        case SM_EVN_RSH_GRP_RUTI:
            if (pt_rs_obj->ui4_lastest_rutil_event == ui4_event)
            {
                return FALSE;
            }
            pt_rs_obj->ui4_lastest_rutil_event = ui4_event;
            break;
        case SM_EVN_RSH_GRP_TM:
            if (pt_rs_obj->ui4_lastest_tm_event == ui4_event)
            {
                return FALSE;
            }
            pt_rs_obj->ui4_lastest_tm_event = ui4_event;
            break;
            
        case SM_EVN_GRP_CA:    
            if (pt_rs_obj->ui4_lastest_ca_event == ui4_event)
            {
                return FALSE;
            }
            pt_rs_obj->ui4_lastest_ca_event = ui4_event;
            break;
            
        case SM_EVN_RSH_GRP_APP:
        default:
            /* Ignore un-handled events */
            return FALSE;
    }
    
    
    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_strm_scdb_nfy_fct
 *
 * Description: This function is scdb notify function
 *
 * Inputs:  
 *       h_scdb:   SCDB handle
 *       e_cond:   SCDB condition
 *       e_cond:   condition change reson
 *       pv_tag:   notify tag
 *       ui4_data: additional data
 *
 * Outputs: 
 *       Send a message to stream handler to check if relate to itself
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
static VOID _sm_rcrd_strm_scdb_nfy_fct(HANDLE_T        h_scdb,
                                       SCDB_COND_T     e_cond,
                                       UINT32          ui4_reason,
                                       VOID*           pv_tag,
                                       UINT32          ui4_data)
{
    SM_RCRD_STRM_HDLR_MSG_T    t_msg;
    HANDLE_T                   h_stream = (HANDLE_T)pv_tag;
    
    t_msg.e_msg_type = SM_RCRD_STRM_MSG_TYPE_SCDB;
    t_msg.h_stream = h_stream;
    t_msg.u.t_rcrd_scdb_msg.e_scdb_cond = e_cond;
    t_msg.u.t_rcrd_scdb_msg.ui4_reason = ui4_reason;
    t_msg.u.t_rcrd_scdb_msg.ui4_data = ui4_data;
        
    sm_rs_send_msg(&t_msg);
    
    return;
}

#ifdef SYS_RECORD_SUPPORT
static UINT32 _sm_rcrd_strm_pm_ca_cond_to_rsh(PM_CA_COND_T e_ca_cond)
{
    switch (e_ca_cond)
    {
        case PM_CA_COND_CLOSED:
            return SM_EVN_CA_COND_CLOSED;

        case PM_CA_COND_UNKNOWN:
            return SM_EVN_CA_COND_UNKNOWN;

        case PM_CA_COND_FREE:
            return SM_EVN_CA_COND_FREE;

        case PM_CA_COND_AUTHORIZED:
            return SM_EVN_CA_COND_AUTHORIZED;

        case PM_CA_COND_AUTHORIZED_CA_SYS_ID:
            return SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID;

        case PM_CA_COND_NOT_AUTHORIZED:
            return SM_EVN_CA_COND_NOT_AUTHORIZED;

        default:
        return SM_EVN_CA_COND_CLOSED;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_strm_state_ca_nfy
 *
 * Description: This function is PM's  CA notify function
 *
 * Inputs:  
 *       h_component:    PM component handle
 *       e_nfy_cond:     PM condition
 *       ui4_detail_code:PM detailed condition
 *       pv_nfy_tag:     notify tag
 *
 * Outputs: 
 *       Send a message to stream handler to update PM status
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-09-21 : initial
 ------------------------------------------------------------------------------*/
static VOID _sm_rcrd_strm_state_ca_nfy(HANDLE_T       h_component,
                                                PM_CA_COND_T   e_nfy_cond,
                                                UINT32         ui4_detail_code,
                                                VOID*          pv_nfy_tag)
{
    SM_RCRD_STRM_HDLR_MSG_T  t_msg;
    HANDLE_T                 h_stream = (HANDLE_T)pv_nfy_tag;

    t_msg.u.t_msg_ca.ui4_ca_event     = _sm_rcrd_strm_pm_ca_cond_to_rsh(e_nfy_cond);

    t_msg.h_stream                    = h_stream;
    t_msg.e_msg_type                  = SM_RCRD_STRM_MSG_TYPE_CA;
    t_msg.u.t_msg_ca.ui4_detail_code  = ui4_detail_code;

    sm_rs_send_msg(&t_msg);
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_strm_cp_nfy
 *
 * Description: This function is PM's  CP notify function
 *
 * Inputs:  
 *       h_component:    PM component handle
 *       t_cp_info:     
 *       pv_tag:
 *
 * Outputs: 
 *       Send a message to stream handler to update PM status
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-09-21 : initial
 ------------------------------------------------------------------------------*/
static VOID _sm_rcrd_strm_cp_nfy(HANDLE_T        h_component,
                                         CP_INFO_LIST_T  t_cp_info,
                                         VOID*           pv_tag)
{
    CP_INFO_LIST_T*          pt_cp_info_lst = (CP_INFO_LIST_T*)&t_cp_info;
    SM_RCRD_STRM_HDLR_MSG_T  t_msg;

    if ((h_component == NULL_HANDLE) || (!pv_tag))
    {
        return;
    }

    t_msg.e_msg_type                                 = SM_RCRD_STRM_MSG_TYPE_CP;
    t_msg.h_stream                                   = (HANDLE_T)pv_tag;
    t_msg.u.t_msg_cci.h_component                    = h_component;
    t_msg.u.t_msg_cci.t_pm_cp_info_lst.z_cp_info_num = pt_cp_info_lst->ui1_ctrl_size;

    if (t_msg.u.t_msg_cci.t_pm_cp_info_lst.z_cp_info_num > CP_TYPE_MAX)
    {
        SM_RSH_DBG_ERR(("_sm_rcrd_strm_cp_nfy(0x%.8X) with invalid size %u!\r\n",
                        (UINT32)t_msg.h_stream,
                        t_msg.u.t_msg_cci.t_pm_cp_info_lst.z_cp_info_num));
        return;
    }


    if  ( pt_cp_info_lst->ui1_ctrl_size !=0 )
    {
        x_memcpy(t_msg.u.t_msg_cci.t_pm_cp_info_lst.at_cp_info,
                 pt_cp_info_lst->at_cp_ctrl_list,
                 sizeof(CP_INFO_T) * pt_cp_info_lst->ui1_ctrl_size);
    }

    sm_rs_send_msg(&t_msg);
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_strm_state_pm_nfy
 *
 * Description: This function is PM notify function
 *
 * Inputs:  
 *       h_component:    PM component handle
 *       e_nfy_cond:     PM condition
 *       ui4_detail_code:PM detailed condition
 *       pv_nfy_tag:     notify tag
 *
 * Outputs: 
 *       Send a message to stream handler to update PM status
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
static VOID _sm_rcrd_strm_state_pm_nfy(HANDLE_T    h_component,
                                       PM_COND_T   e_nfy_cond,
                                       UINT32      ui4_detail_code,
                                       VOID*       pv_nfy_tag)
{
    SM_RCRD_STRM_HDLR_MSG_T     t_msg;
    HANDLE_T                    h_stream;

    h_stream = (HANDLE_T)pv_nfy_tag;
    
    switch(e_nfy_cond)
    {
    case PM_COND_CLOSED:
        t_msg.u.t_rcrd_pm_msg.ui4_pm_event = SM_EVN_RSH_PM_COND_CLOSED;
        break;

    case PM_COND_PLAYED:
        t_msg.u.t_rcrd_pm_msg.ui4_pm_event = SM_EVN_RSH_PM_COND_PLAYED;
        break;

    case PM_COND_STOPPED:
        t_msg.u.t_rcrd_pm_msg.ui4_pm_event = SM_EVN_RSH_PM_COND_STOPPED;
        break;

    case PM_COND_OPENING:
    case PM_COND_STOPPING:
    case PM_COND_CLOSING:
    case PM_COND_PLAYING:
    default:
        return;
    }

    t_msg.h_stream                        = h_stream;
    t_msg.e_msg_type                      = SM_RCRD_STRM_MSG_TYPE_PM;
    t_msg.u.t_rcrd_pm_msg.ui4_detail_code = ui4_detail_code;

    sm_rs_send_msg(&t_msg);

    return;
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_opening
 *
 * Description: This function opens stream related components
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:         event e.g.PM-DMX/RM-PVR/Record Utiltiy/SCDB/APP
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Open some compnents
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_opening(UINT32    ui4_previous_state,
                                    UINT32    ui4_event,
                                    UINT32    ui4_curr_state,
                                    VOID*     pv_tag1,
                                    VOID*     pv_tag2)
{
    INT32                      i4_ret;
    SM_SESSION_T*              pt_sess_obj = NULL;
    SM_RCRD_STREAM_T*          pt_rs_obj   = NULL;
    HANDLE_T                   h_stream;
    #ifdef SYS_RECORD_SUPPORT
    PM_REC_ES_REQUEST_T        t_rec_component;
    #endif    
    
    SM_ASSERT(pv_tag1);
    
    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    h_stream  = pt_rs_obj->t_base.h_stream;

    /*1.Record latest events*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    /*Get session object*/
    pt_sess_obj = (SM_SESSION_T*)sm_get_sess_obj(pt_rs_obj->t_base.h_sess);
    if (NULL == pt_sess_obj)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_opening: sm_get_sess_obj(0x%.8x) "
                        "error!\r\n",
                        (UINT32)(pt_rs_obj->t_base.h_sess)));

        sm_rs_send_msg_do_event(SM_EVN_RSH_INTERNAL_ERR, h_stream, 0);
        return SMR_SESS_NOT_EXIST;
    }
    
    /*2.Open SCDB so that stream can perceive changes of SCDB*/
    i4_ret = x_scdb_open(pt_rs_obj->t_base.h_connection,
                         &pt_rs_obj->t_base.h_scdb,
                         (VOID*)h_stream,
                         _sm_rcrd_strm_scdb_nfy_fct);
    if (i4_ret != SCDBR_OK)
    {
        pt_rs_obj->t_base.h_scdb = NULL_HANDLE;
        SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_opening:scdb(0x%.8X) error=%d!\r\n",
                        (UINT32)h_stream, 
                        i4_ret));
        
        sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
        return SMR_STRM_COMP_ID_NOT_FOUND;
    }
    
    /*3.If it is PMT open Record Utility*/
    if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
        && (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp))
    {
        i4_ret = sm_rcrd_strm_rutil_open(h_stream, pt_rs_obj);
        if (i4_ret != SMR_OK)
        {
            pt_rs_obj->t_rutil_info.h_record_util = NULL_HANDLE;
            SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_opening:Record utility(0x%.8X) error=%d!\r\n",
                           (UINT32)h_stream,
                            i4_ret));
        
            sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
            return SMR_INTERNAL_ERROR;
        }
    }
    
    /*4.Open PVR stream driver*/
    i4_ret = sm_rcrd_strm_pvr_open(h_stream, pt_rs_obj);
    if (i4_ret != SMR_OK)
    {
        pt_rs_obj->t_pvr_descr.h_comp = NULL_HANDLE;
        SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_opening:pvr(0x%.8X) error=%d!\r\n",
                       (UINT32)h_stream,
                       i4_ret));
        sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
        return SMR_DRV_COMP_ERROR;
    }
    
    /*5.Add PM component*/
    #ifdef SYS_RECORD_SUPPORT
    x_memset((VOID*)&t_rec_component, 0, sizeof(PM_REC_ES_REQUEST_T));
    t_rec_component.e_es_info_type                                  = PM_RECORDING_ES_WI_SETTING;
    t_rec_component.u_es_info.t_stream_setting.t_pid                = pt_rs_obj->t_pid;
    if (RSH_RCRD_TYPE_ES == pt_rs_obj->t_rcrd_type)
    {
        t_rec_component.u_es_info.t_stream_setting.ui4_stream_type_mask = pt_rs_obj->t_es_rcrd_info.ui4_type_mask;
        t_rec_component.e_ca_require_lvl = PM_CA_REQUIRE_MANDATORY;
    }
    else
    {
        t_rec_component.u_es_info.t_stream_setting.ui4_stream_type_mask = 0;
        t_rec_component.e_ca_require_lvl = PM_CA_REQUIRE_NO;
    }
    if (RSH_RCRD_TYPE_PCR == pt_rs_obj->t_rcrd_type)
    {
        t_rec_component.u_es_info.t_stream_setting.e_setting_info = PM_RECORDING_SETTING_PCR;
    }
    else
    {
        t_rec_component.u_es_info.t_stream_setting.e_setting_info = PM_RECORDING_UNKNOWN;
    }
    t_rec_component.h_component                       = NULL_HANDLE;
    
    t_rec_component.t_filter_nfy_info.pf_nfy          = _sm_rcrd_strm_state_pm_nfy;
    t_rec_component.t_filter_nfy_info.pv_tag          = (VOID*)h_stream;
    t_rec_component.t_filter_nfy_info.e_cond          = PM_COND_CLOSED;
    t_rec_component.t_filter_nfy_info.ui4_detail_code = 0;

    t_rec_component.t_sink_comp_info.e_type = pt_rs_obj->t_pvr_descr.t_info.e_type;
    t_rec_component.t_sink_comp_info.ui2_id = pt_rs_obj->t_pvr_descr.t_info.ui2_comp_id;

    t_rec_component.t_ca_nfy_info.pf_nfy          = _sm_rcrd_strm_state_ca_nfy;
    t_rec_component.t_ca_nfy_info.pv_tag          = (VOID*)h_stream;
    t_rec_component.t_ca_nfy_info.e_cond          = PM_CA_COND_CLOSED;
    t_rec_component.t_ca_nfy_info.ui4_detail_code = 0;

    t_rec_component.t_cp_nfy_info.pf_nfy                  = _sm_rcrd_strm_cp_nfy;
    t_rec_component.t_cp_nfy_info.pv_tag                  = (VOID*)h_stream;
    t_rec_component.t_cp_nfy_info.t_cp_info.ui1_ctrl_size = 0;
    
    i4_ret = pm_add_component(pt_sess_obj->h_pm_service, (VOID*)&t_rec_component);
    if (i4_ret != PMR_OK)
    {
        pt_rs_obj->t_base.h_pm_comp = NULL_HANDLE;
        SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_opening:pm(0x%.8X) error=%d!\r\n",
                       (UINT32)h_stream, 
                       i4_ret));

        sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
        return SMR_INTERNAL_ERROR;
    }
    pt_rs_obj->t_base.h_pm_comp = t_rec_component.h_component;

    _sm_rcrd_strm_state_pm_nfy(pt_rs_obj->t_base.h_pm_comp,
                               t_rec_component.t_filter_nfy_info.e_cond,
                               0,
                               (VOID*)h_stream);
    #endif

    /*6.generate pat*/
    if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
        && ((RSH_TBL_GRP_PSI_PAT == pt_rs_obj->t_sect_info.e_tbl_grp)
             || (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp)))
    {
        i4_ret = sm_rcrd_strm_load_pat(pt_rs_obj);
        if (i4_ret != PMR_OK)
        {
            pt_rs_obj->h_tm_root = NULL_HANDLE;
            pt_rs_obj->h_pat     = NULL_HANDLE;
            SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_opening:load pat error=%d!\r\n",
                           i4_ret));
        
            sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
            return SMR_INTERNAL_ERROR;
        }
    }
    
    /*7.If
          6.1 PM is stopped 
          6.2 Record utility is opened(PMT only)
          6.3 PAT is set ok
          6.4 When PVR is opened 
              6.4.1 Init PVR e.g.
              
        Then stream enters opened state 
    */
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_opened
 *
 * Description: This function indicates stream is opened
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:         event e.g.PM-DMX/RM-PVR/Record Utiltiy/SCDB/APP
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Set component as ready
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_opened(UINT32     ui4_previous_state,
                                   UINT32     ui4_event,
                                   UINT32     ui4_curr_state,
                                   VOID*      pv_tag1,
                                   VOID*      pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    
    SM_ASSERT(pv_tag1);
    
    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    /*h_stream = pt_rs_obj->t_base.h_stream;*/

    /*1 Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_starting
 *
 * Description: This function starts stream related components
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:         event e.g.PM-DMX/RM-PVR/Record Utiltiy/SCDB/APP
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Start component 
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_starting(UINT32     ui4_previous_state,
                                     UINT32     ui4_event,
                                     UINT32     ui4_curr_state,
                                     VOID*      pv_tag1,
                                     VOID*      pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    HANDLE_T              h_stream;
    INT32                 i4_ret;
    
    SM_ASSERT(pv_tag1);

    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    h_stream  = pt_rs_obj->t_base.h_stream;
    
    /*1. Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    /*2.Set something when starting---reserved for later*/
        
    /*3. play PVR stream */
    i4_ret = sm_rcrd_strm_pvr_set(h_stream, PVR_SET_TYPE_CTRL, (VOID*)TRUE, sizeof(BOOL));
    if (i4_ret != SMR_OK)
    {
        /*pt_rs_obj->t_pvr_descr.h_comp = NULL_HANDLE;*/
        SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_starting:pvr(0x%.8X) error=%d!\r\n",
                       (UINT32)h_stream,
                       i4_ret));
        sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
        return SMR_DRV_COMP_ERROR;
    }
    sm_rs_send_msg_do_event(SM_EVN_RSH_PVR_PLAYED,
                            h_stream,
                            0);
#if 0
   /*4.Play PM componet order issue, change it to the function end  */
    i4_ret = pm_play_component(pt_rs_obj->t_base.h_pm_comp);
    if (i4_ret != PMR_OK)
    {
       SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_starting:pm_play_component(0x%.8x)=%d\r\n",
                       (UINT32)pt_rs_obj->t_base.h_pm_comp, 
                       i4_ret));
       sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
       return SMR_DRV_COMP_ERROR;
    }
#endif
   /*5. If PAT stream and PAT is available set PAT,if null set null packet*/
    if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
        && (RSH_TBL_GRP_PSI_PAT == pt_rs_obj->t_sect_info.e_tbl_grp))
    {
        if ((TM_COND_AVAILABLE == pt_rs_obj->e_pat_cond)
            || (TM_COND_UPDATE == pt_rs_obj->e_pat_cond))
        {
            i4_ret = sm_rcrd_strm_swap_pat(pt_rs_obj);
            if (i4_ret != SMR_OK)
            {
                 SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_starting:"
                                 "set PVR pat err=%d!\r\n", 
                                 i4_ret));
                 sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
                return SMR_DRV_COMP_ERROR;
            }
            else
            {
                SM_RSH_DBG_INFO(("sm_rcrd_strm_state_do_starting:"
                                 "set PVR pat OK\r\n"));
            }
        }
        else
        {
            i4_ret = sm_rcrd_strm_pvr_set_null_pacekt(pt_rs_obj->t_base.h_stream,
                                                      pt_rs_obj);
            if (i4_ret != SMR_OK)
            {
                 SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_starting:"
                                 "set PVR null pat err=%d!\r\n", 
                                 i4_ret));
                 sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
                return SMR_DRV_COMP_ERROR;
            }
            else
            {
                SM_RSH_DBG_INFO(("sm_rcrd_strm_state_do_starting:"
                                 "set PVR null pat OK\r\n"));
            }
        }
        
    }
    
   /*6. If PMT stream and Es list is not null,PMT is available swap ,
        if null set null packet*/
    if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
       && (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp))
    {
       if (((TM_COND_AVAILABLE == pt_rs_obj->e_pmt_cond)
            || (TM_COND_UPDATE == pt_rs_obj->e_pmt_cond))
            && (0 != pt_rs_obj->t_pmt_info.ui2_num_es))
       {
           i4_ret = sm_rcrd_strm_swap_pmt(pt_rs_obj);
           if (SMR_OK == i4_ret)
           {
               SM_RSH_DBG_INFO(("sm_rcrd_strm_state_do_starting:"
                                "set PVR pmt OK\r\n"));
           }
           else
           {
                SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_starting:"
                                "set PVR pmt err=%d!\r\n", 
                                i4_ret));
                sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
               return SMR_DRV_COMP_ERROR;
           }
       }
       else
       {
           i4_ret = sm_rcrd_strm_pvr_set_null_pacekt(pt_rs_obj->t_base.h_stream,
                                                     pt_rs_obj);
           if (SMR_OK == i4_ret)
           {
               SM_RSH_DBG_INFO(("sm_rcrd_strm_state_do_starting:"
                                "set PVR null pmt OK\r\n"));
           }
           else
           {
                SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_starting:"
                                "set PVR null pmt err=%d!\r\n", 
                                i4_ret));
                sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
               return SMR_DRV_COMP_ERROR;
           }
       }
    }
    /*new 8.Play PM componet order issue change it to the function end  */
    i4_ret = pm_play_component(pt_rs_obj->t_base.h_pm_comp);
    if (i4_ret != PMR_OK)
    {
       SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_starting:pm_play_component(0x%.8x)=%d\r\n",
                       (UINT32)pt_rs_obj->t_base.h_pm_comp, 
                       i4_ret));
       sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
       return SMR_DRV_COMP_ERROR;
    }

   /*7. If
        5.1 PM is played 
        5.2 Record utility is opened(PMT only)
        5.3 PVR is enabled 
        
      Then stream enters started state     
   */
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_started
 *
 * Description: This function indicates stream related components are all started
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:         event e.g.PM-DMX/RM-PVR/Record Utiltiy/SCDB/APP
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Notify info to APP 
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_started(UINT32     ui4_previous_state,
                                    UINT32     ui4_event,
                                    UINT32     ui4_curr_state,
                                    VOID*      pv_tag1,
                                    VOID*      pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    /*HANDLE_T              h_stream;*/
    
    SM_ASSERT(pv_tag1);

    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    /*h_stream = pt_rs_obj->t_base.h_stream;*/
    
    /*1. Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    /*2.Reserved for later*/

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_stopping
 *
 * Description: This function stops stream related components
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:         event e.g.PM-DMX/RM-PVR/Record Utiltiy/SCDB/APP
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Stops stream related components, SCDB and Record utility aren't necessary
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_stopping(UINT32     ui4_previous_state,
                                     UINT32     ui4_event,
                                     UINT32     ui4_curr_state,
                                     VOID*      pv_tag1,
                                     VOID*      pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    HANDLE_T              h_stream;
    INT32                 i4_ret;
    
    SM_ASSERT(pv_tag1);

    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    h_stream  = pt_rs_obj->t_base.h_stream;
    
    /*1. Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    /*2. Stop PM component */
    if ((pt_rs_obj->ui4_lastest_pm_event != SM_EVN_RSH_PM_COND_CLOSED)
         && (pt_rs_obj->ui4_lastest_pm_event != SM_EVN_RSH_PM_COND_STOPPED))
    {
        i4_ret = pm_stop_component(pt_rs_obj->t_base.h_pm_comp);
        if (i4_ret != PMR_OK)
        {
            if ((PMR_INV_HANDLE == i4_ret)
                 &&((SM_EVN_SESS_RCRD_APP_CLOSE_REQ == ui4_event)
                     || (SM_EVN_SESS_RCRD_APP_STOP_REQ == ui4_event)))
            {
                /* PM component may already closed by session layer,
                   Treat as OK */
            }
            else
            {
                SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_stopping:pm_stop_component(0x%.8x)"
                                "error =%d\r\n",
                               (UINT32)pt_rs_obj->t_base.h_pm_comp, 
                                i4_ret));
                sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
                return SMR_INTERNAL_ERROR;
            }
        }
    }

    /*3. Disable PVR stream */
    i4_ret = sm_rcrd_strm_pvr_set(h_stream, 
                                  PVR_SET_TYPE_CTRL, 
                                  (VOID*)FALSE, 
                                  sizeof(BOOL));
    if (i4_ret != SMR_OK)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_stopping:(0x%.8X) stop pvr error=%d!\r\n",
                       (UINT32)pt_rs_obj->t_pvr_descr.h_comp, 
                       i4_ret));

        sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream, 0);
        return SMR_INTERNAL_ERROR;
    }
    sm_rs_send_msg_do_event(SM_EVN_RSH_PVR_STOPPED, h_stream, 0);

    /*4.If 
          4.1 PVR is disabled
          4.2 PM is stopped
          4.3 Record utility is opened(PMT only)
          
        Then stream enters opened state   
    */
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_closing
 *
 * Description: This function closes stream related components
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:         event e.g.PM-DMX/RM-PVR/Record Utiltiy/SCDB/APP
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Closes stream related components
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_closing(UINT32     ui4_previous_state,
                                    UINT32     ui4_event,
                                    UINT32     ui4_curr_state,
                                    VOID*      pv_tag1,
                                    VOID*      pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    HANDLE_T              h_stream;
    INT32                 i4_ret;
    RM_COND_T             e_cond;
    
    SM_ASSERT(pv_tag1);

    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    h_stream  = pt_rs_obj->t_base.h_stream;
    
    /*1. Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);


    /* closing order??:
       Session : PM service->PVR stream ctrl->All PVR streams 
                                              Record utility
                                              SCDB
       Stream:   PM node->PVR stream->Record utility
                                      SCDB
   */
    /*2.Stop/delete PM component    */
    if (pt_rs_obj->t_base.h_pm_comp != NULL_HANDLE)
    {
        /* Stop PM component if previous state is ERROR, and PM condition
           is not in STOPPED/CLOSED state */
        if ((SM_COND_ERROR == ui4_previous_state) 
             &&  (pt_rs_obj->ui4_lastest_pm_event != SM_EVN_RSH_PM_COND_STOPPED)
             &&  (pt_rs_obj->ui4_lastest_pm_event != SM_EVN_RSH_PM_COND_CLOSED))
        {
            i4_ret = pm_stop_component( pt_rs_obj->t_base.h_pm_comp);
            if ( i4_ret != PMR_OK )
            {
                SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_closing:pm_stop_component(0x%.8x)"
                                "error=%d\r\n",
                                (UINT32)pt_rs_obj->t_base.h_pm_comp, 
                                i4_ret));
            }
        }
        
        /*PM will notify SM when delete node*/
        i4_ret = pm_del_component(pt_rs_obj->t_base.h_pm_comp);
        if (i4_ret != PMR_OK)
        {
            SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_closing: pm_del_component(0x%.8x)"
                            "error=%d\r\n",
                            (UINT32)pt_rs_obj->t_base.h_pm_comp, 
                            i4_ret));
            sm_rs_send_msg_do_event(SM_EVN_RSH_PM_COND_CLOSED, h_stream, 0);
        }
    }
    else
    {
        sm_rs_send_msg_do_event(SM_EVN_RSH_PM_COND_CLOSED, h_stream, 0);
    }

    /*3.Disconnect/close PVR stream , if disconnected ok it will close component*/
    if (NULL_HANDLE != pt_rs_obj->t_pvr_descr.h_comp)
    {
        e_cond = sm_rm_cond_filter(pt_rs_obj->t_pvr_descr.e_comp_cond,
                                   COND_GRP_CONN_STATE);
        if (RM_COND_CONNECTED == e_cond)
        {
            i4_ret = sm_rcrd_strm_pvr_disconnect(h_stream, pt_rs_obj);
            if (i4_ret != SMR_OK)
            {
                SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_closing:PVR disconnect fail=%d!\r\n",
                                i4_ret));
                /* In release version, pretend that PVR is disconnected. */
                sm_rs_send_msg_do_event(SM_EVN_RSH_PVR_DISCONNECTED,  h_stream,  0);
            }
            
        }
        else
        {
            /*close PVR Stream*/
            i4_ret = sm_rcrd_strm_pvr_close(h_stream, pt_rs_obj);
            if (i4_ret != SMR_OK)
            {
                SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_closing:PVR close fail=%d!\r\n",
                                i4_ret));
                sm_rs_send_msg_do_event(SM_EVN_RSH_PVR_CLOSED, h_stream, 0);
            }
            /*pt_rs_obj->t_pvr_descr.h_comp = NULL_HANDLE;*/
            
        }
    }
    else
    {
        sm_rs_send_msg_do_event(SM_EVN_RSH_PVR_CLOSED, h_stream, 0);
    }
    
    /* 4.close SCDB */
    if (pt_rs_obj->t_base.h_scdb != NULL_HANDLE)
    {
        i4_ret = x_scdb_close(pt_rs_obj->t_base.h_scdb);
        if (i4_ret != SCDBR_OK)
        {
            SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_closing: x_scdb_close(0x%.8x)error=%d\r\n",
                            (UINT32)pt_rs_obj->t_base.h_scdb, 
                            i4_ret));
        }
        pt_rs_obj->t_base.h_scdb = NULL_HANDLE;
    }
    
    /*5.Close Record Utility*/
    if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
        && (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp))
    {
        i4_ret = sm_rcrd_strm_rutil_close(h_stream, pt_rs_obj);
        if (i4_ret != SMR_OK)
        {
            SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_closing:Record utility(0x%.8X) error=%d!\r\n",
                            (UINT32)h_stream,
                            i4_ret));
        }
        pt_rs_obj->t_rutil_info.h_record_util = NULL_HANDLE;
        sm_rs_send_msg_do_event(SM_EVN_RSH_RUTI_CLOSED, h_stream, 0);
    }
    
    /*6.Close PMT*/
    if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
        && (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp))
    {
        if (x_handle_valid(pt_rs_obj->h_pmt))
        {
            i4_ret = sm_rcrd_strm_free_tm_obj(pt_rs_obj->h_pmt);
            if (i4_ret != SMR_OK)
            {
                SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_closing:PMT(0x%.8X) error=%d!\r\n",
                                (UINT32)pt_rs_obj->h_pmt,
                                i4_ret));
            }
            pt_rs_obj->h_pmt     = NULL_HANDLE;
        }
    }
    
    /*7.Close PAT Root*/
    if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
        && ((RSH_TBL_GRP_PSI_PAT == pt_rs_obj->t_sect_info.e_tbl_grp)
             || (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp)))
    {
        if (x_handle_valid(pt_rs_obj->h_pat))
        {
            i4_ret = sm_rcrd_strm_free_tm_obj(pt_rs_obj->h_pat);
            if (i4_ret != SMR_OK)
            {
                SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_closing:PAT(0x%.8X) error=%d!\r\n",
                                (UINT32)pt_rs_obj->h_pat,
                                i4_ret));
            }
            pt_rs_obj->h_pat        = NULL_HANDLE;
            pt_rs_obj->b_pat_loaded = FALSE;
        }
        
        if (x_handle_valid(pt_rs_obj->h_tm_root))
        {
            i4_ret = sm_rcrd_strm_free_tm_obj(pt_rs_obj->h_tm_root);
            if (i4_ret != SMR_OK)
            {
                SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_closing:PAT(0x%.8X) error=%d!\r\n",
                                (UINT32)pt_rs_obj->h_tm_root,
                                i4_ret));
            }
            pt_rs_obj->h_tm_root = NULL_HANDLE;
        }
        
    }
    
    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_closed
 *
 * Description: This function indicates stream related components are closed
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:         event e.g.PM-DMX/RM-PVR/Record Utiltiy/SCDB/APP
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Notify info to APP
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_closed(UINT32     ui4_previous_state,
                                   UINT32     ui4_event,
                                   UINT32     ui4_curr_state,
                                   VOID*      pv_tag1,
                                   VOID*      pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    
    SM_ASSERT(pv_tag1);

    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    
    /*1. Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    /*2.Reserved for later*/
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_error
 *
 * Description: This function shows stream hander enters error state
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:         event e.g.PM-DMX/RM-PVR/Record Utiltiy/SCDB/APP
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs:NULL
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_error(UINT32     ui4_previous_state,
                                  UINT32     ui4_event,
                                  UINT32     ui4_curr_state,
                                  VOID*      pv_tag1,
                                  VOID*      pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    
    SM_ASSERT(pv_tag1);

    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    
    /*1. Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    /*2.Notify something to APP if error---reserved for later*/

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_ignore_excpt
 *
 * Description: This function handles exception events in ignore state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Do exception handling 
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_ignore_excpt(UINT32   ui4_event,
                                         UINT32   ui4_curr_state,
                                         VOID*    pv_tag1,
                                         VOID*    pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    
    SM_ASSERT(pv_tag1);

    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    
    /*1. Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    /*2.Debug if error---reserved for later*/

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_opening_excpt
 *
 * Description: This function handles exception events in opening state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Do exception handling 
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_opening_excpt(UINT32   ui4_event,
                                          UINT32   ui4_curr_state,
                                          VOID*    pv_tag1,
                                          VOID*    pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    
    SM_ASSERT(pv_tag1);

    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    
    /*1. Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    /*2.Debug if error---reserved for later*/

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_opened_excpt
 *
 * Description: This function handles exception events in opened state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Do exception handling 
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_opened_excpt(UINT32   ui4_event,
                                         UINT32   ui4_curr_state,
                                         VOID*    pv_tag1,
                                         VOID*    pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    
    SM_ASSERT(pv_tag1);

    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    
    /*1. Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    /*2.Debug if error---reserved for later*/

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_starting_excpt
 *
 * Description: This function handles exception events in starting state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Do exception handling 
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_starting_excpt(UINT32   ui4_event,
                                           UINT32   ui4_curr_state,
                                           VOID*    pv_tag1,
                                           VOID*    pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    
    SM_ASSERT(pv_tag1);

    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    
    /*1. Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    /*2.Debug if error---reserved for later*/

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_started_excpt
 *
 * Description: This function handles exception events in started state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Do exception handling 
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_started_excpt(UINT32   ui4_event,
                                          UINT32   ui4_curr_state,
                                          VOID*    pv_tag1,
                                          VOID*    pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    
    SM_ASSERT(pv_tag1);

    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    
    /*1. Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    /*2.Debug if error---reserved for later*/

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_stopping_excpt
 *
 * Description: This function handles exception events in stopping state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Do exception handling 
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_stopping_excpt(UINT32   ui4_event,
                                           UINT32   ui4_curr_state,
                                           VOID*    pv_tag1,
                                           VOID*    pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    
    SM_ASSERT(pv_tag1);

    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    
    /*1. Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    /*2.Debug if error---reserved for later*/

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_closing_excpt
 *
 * Description: This function handles exception events in closing state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Do exception handling 
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_closing_excpt(UINT32   ui4_event,
                                          UINT32   ui4_curr_state,
                                          VOID*    pv_tag1,
                                          VOID*    pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    
    SM_ASSERT(pv_tag1);

    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    
    /*1. Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    /*2.Debug if error---reserved for later*/

    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_closed_excpt
 *
 * Description: This function handles exception events in closed state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Do exception handling 
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_closed_excpt(UINT32   ui4_event,
                                         UINT32   ui4_curr_state,
                                         VOID*    pv_tag1,
                                         VOID*    pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    
    SM_ASSERT(pv_tag1);

    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    
    /*1. Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    /*2.Debug if error---reserved for later*/
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_error_excpt
 *
 * Description: This function handles exception events in error state
 *
 * Inputs:  
 *       e_event:         event e.g.PM/RM/Record Utiltiy/SCDB
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Do exception handling 
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_error_excpt(UINT32   ui4_event,
                                        UINT32   ui4_curr_state,
                                        VOID*    pv_tag1,
                                        VOID*    pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    
    SM_ASSERT(pv_tag1);

    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    
    /*1. Record event*/
    _sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event);

    /*2.Debug if error---reserved for later*/

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_notify_app
 *
 * Description: This function notifies record stream handler information
 *              to application e.g. record handler
 *
 * Inputs:  
 *       pv_tag :  record stream handle and stream object pointer
 *       e_cond :  stream current status
 *       ui4_event :current event 
 *       ui4_data :additional info e.g reason information
 * Outputs: NULL  
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-06 : initial
 ------------------------------------------------------------------------------*/
VOID sm_rcrd_strm_state_notify_app(VOID*          pv_tag,
                                   SM_COND_T      e_cond,
                                   UINT32         ui4_event,
                                   UINT32         ui4_data)
{
    INT32                        i4_ret;
    SM_RCRD_STRM_STATE_TAG_T*    pt_state_tag;
    SM_RCRD_STREAM_T*            pt_rs_obj;
    HANDLE_T                     h_stream;

    pt_state_tag = (SM_RCRD_STRM_STATE_TAG_T*)pv_tag;

    SM_ASSERT(pt_state_tag );
    SM_ASSERT(pt_state_tag->h_stream != NULL_HANDLE);
    SM_ASSERT(pt_state_tag->pt_strm_obj);

    pt_rs_obj = pt_state_tag->pt_strm_obj;
    h_stream  = pt_state_tag->h_stream;


    pt_rs_obj->t_base.e_condition = e_cond;

    i4_ret = pt_rs_obj->pf_sess_hdlr_sm_nfy(h_stream,
                                            pt_rs_obj->t_base.e_condition,
                                            pt_rs_obj->pv_sess_hdlr_sm_nfy_tag,
                                            ui4_event,
                                            ui4_data);
    if (i4_ret != SMR_OK)
    {
        /* Do nothing */
        SM_RSH_DBG_ERR(("sm_rcrd_strm_state_notify_app:notify error!\r\n"));
    }

    #ifdef DEBUG
    {
        const CHAR* ps_state = sm_state_event_to_str(ui4_event);

        if (ps_state != NULL)
        {
            SM_RSH_DBG_INFO(("sm_rcrd_strm_state_notify_app:"
                             "(0x%.8X),%s, %s, DATA(0x%.8X)\r\n",
                             (UINT32)h_stream,
                             sm_strm_cond_2_str(e_cond),
                             ps_state,
                             (UINT32)ui4_data));
        }
        else
        {
            SM_RSH_DBG_INFO(("sm_rcrd_strm_state_notify_app:"
                             "(0x%.8X), %s, EVN(0x%.8X), DATA(0x%.8X)\r\n",
                            (UINT32)h_stream,
                             sm_strm_cond_2_str(e_cond),
                             (UINT32)ui4_event,
                             (UINT32)ui4_data));
        }
    }
    #endif 

    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_strm_state_do_pvr_event
 *
 * Description: Handles PM events which are generated by PVR-RM notify messages 
 *              processing function
 *
 * Inputs:  
 *       h_stream:  stream handle
 *       pt_rs_obj: record stream obejct pointer 
 *       e_event:   PVR event
 *
 * Outputs: NULL
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_strm_state_do_pvr_event(HANDLE_T          h_stream,
                                              SM_RCRD_STREAM_T* pt_rs_obj,
                                              UINT32            ui4_event)
{
    INT32                 i4_ret = SMR_OK;

    switch (ui4_event)
    {
        /*If Opened connect to record demux*/    
        case SM_EVN_RSH_PVR_OPENED:
            i4_ret = sm_rcrd_strm_pvr_connect(h_stream, pt_rs_obj);
            if (i4_ret != SMR_OK)
            {
                pt_rs_obj->t_pvr_descr.h_comp = NULL_HANDLE;
                SM_RSH_DBG_ERR(("_sm_rcrd_strm_state_do_pvr_event:"
                                "pvr(0x%.8X) error=%d!\r\n",
                                (UINT32)h_stream,
                                i4_ret));
                return SMR_DRV_COMP_ERROR;
            }
            break;

        case SM_EVN_RSH_PVR_PLAYED:
            break;

        case SM_EVN_RSH_PVR_STOPPED:
            break;

        case SM_EVN_RSH_PVR_CONNECTED:
            SM_RSH_DBG_INFO(("_sm_rcrd_strm_state_do_pvr_event:(0x%.8X)PVR is connected\r\n",
                             h_stream));
            /*Set PVR related info */
            i4_ret = sm_rcrd_strm_init_pvr(h_stream, pt_rs_obj);
            if (i4_ret != SMR_OK)
            {
                SM_RSH_DBG_ERR(("_sm_rcrd_strm_state_do_pvr_event:pvr(0x%.8X) error=%d!\r\n",
                               (UINT32)h_stream,
                               i4_ret));
            }
            break;
            
        case SM_EVN_RSH_PVR_DISCONNECTED:
            SM_RSH_DBG_INFO(("_sm_rcrd_strm_state_do_pvr_event:(0x%.8X)PVR is disconnected\r\n",
                             h_stream));
            /*If disconnected close PVR component*/  
            /*close PVR Stream*/
            i4_ret = sm_rcrd_strm_pvr_close(h_stream, pt_rs_obj);
            if (i4_ret != SMR_OK)
            {
                SM_RSH_DBG_ERR(("_sm_rcrd_strm_state_do_pvr_event:"
                                "PVR close fail=%d!\r\n",
                                i4_ret));
                sm_rs_send_msg_do_event(SM_EVN_RSH_PVR_CLOSED, h_stream, 0);
            }
            /*pt_rs_obj->t_pvr_descr.h_comp = NULL_HANDLE;*/
            break;
            
        case SM_EVN_RSH_PVR_CLOSED:
            pt_rs_obj->t_pvr_descr.h_comp = NULL_HANDLE;
            break;
            
        case SM_EVN_RSH_PVR_ERR:
        default:
            return SMR_DRV_COMP_ERROR;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_strm_state_do_pm_event
 *
 * Description: Handles PM events which are generated by PM notify messages 
 *              processing function
 *
 * Inputs:  
 *       h_stream:  stream handle
 *       pt_rs_obj: record stream obejct pointer 
 *       e_event:   PM event
 *
 * Outputs: NULL
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_strm_state_do_pm_event(HANDLE_T          h_stream,
                                             SM_RCRD_STREAM_T* pt_rs_obj,
                                             UINT32            ui4_event)
{
    switch (ui4_event)
    {
        case SM_EVN_RSH_PM_COND_CLOSED:
            pt_rs_obj->t_base.h_pm_comp = NULL_HANDLE;
            break;

        case SM_EVN_RSH_PM_COND_PLAYED:
            break;

        case SM_EVN_RSH_PM_COND_STOPPED:
            break;
            
        /*case SM_EVN_RSH_PM_COND_OPENED:
            break;*/
            
        case SM_EVN_RSH_PM_COND_ERR:
        default:
            return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_strm_state_do_tm_event
 *
 * Description: Handles TM events which are generated by TM notify messages 
 *              processing function
 *
 * Inputs:  
 *       h_stream:  stream handle
 *       pt_rs_obj: record stream obejct pointer 
 *       e_event:   TM event
 *
 * Outputs: NULL
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_strm_state_do_tm_event(HANDLE_T          h_stream,
                                             SM_RCRD_STREAM_T* pt_rs_obj,
                                             UINT32            ui4_event)
{
    switch (ui4_event)
    {
        case SM_EVN_RSH_TM_PAT_SWAPED:
            /*Notify APP*/
            sm_rcrd_strm_state_notify_app(pt_rs_obj->t_state.pv_state_nfy_tag,
                                          pt_rs_obj->t_state.ui4_curr_state,
                                          SM_EVN_RSH_PAT_SWAPED,
                                          0);
            break;
            
        case SM_EVN_RSH_TM_PMT_SWAPED:
            /*Notify APP*/
            sm_rcrd_strm_state_notify_app(pt_rs_obj->t_state.pv_state_nfy_tag,
                                          pt_rs_obj->t_state.ui4_curr_state,
                                          SM_EVN_RSH_PMT_SWAPED,
                                          0);
            break;
            
        default:
            return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_strm_state_do_rutil_event
 *
 * Description: Handles Record utility events which are generated by Record utility 
 *               notify messages processing function
 *
 * Inputs:  
 *       h_stream:  stream handle
 *       pt_rs_obj: record stream obejct pointer 
 *       e_event:   Record utility event
 *
 * Outputs: NULL
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
static INT32 _sm_rcrd_strm_state_do_rutil_event(HANDLE_T          h_stream,
                                                SM_RCRD_STREAM_T* pt_rs_obj,
                                                UINT32            ui4_event)
{
    switch (ui4_event)
    {
        case SM_EVN_RSH_RUTI_OPENED:
            break;
        case SM_EVN_RSH_RUTI_READY:
            break;

        case SM_EVN_RSH_RUTI_TRANSITION:
            /*close stream*/
            sm_rs_send_msg_do_event(SM_EVN_RSH_APP_CLOSE_REQ, h_stream, 0);
            break;
        case SM_EVN_RSH_RUTI_CLOSED:
            pt_rs_obj->t_rutil_info.h_record_util = NULL_HANDLE;
            break;
            
        case SM_EVN_RSH_RUTI_ERR:
        default:
            return SMR_DRV_COMP_ERROR;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_drv_events
 *
 * Description: All related components will transer notify codes into corresponding  
 *              events and send to this function to judge if state transfer is satisfied
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:         event e.g.PM-DMX/RM-PVR/Record Utiltiy/SCDB/APP
 *       ui4_curr_state:  current state
 *       pv_tag1:         jenerally is stream obejct pointer
 *       pv_tag2:         jenerally is a NULL
 *
 * Outputs: 
 *       Do state transfering by send out state transfer events
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_drv_events(UINT32     ui4_curr_state,
                                       UINT32     ui4_event,
                                       UINT32     ui4_next_state,
                                       VOID*      pv_tag1,
                                       VOID*      pv_tag2)
{
    SM_RCRD_STREAM_T*     pt_rs_obj;
    HANDLE_T              h_stream;
    INT32                 i4_ret       = SMR_OK;
    BOOL                  b_all_opened = FALSE;
    BOOL                  b_all_played = FALSE;
    BOOL                  b_all_closed = FALSE;
    
    SM_ASSERT(pv_tag1);
    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    h_stream  = pt_rs_obj->t_base.h_stream;

    /*1.Focus events which may cause state transfer and 
        Skip the event if there is no change*/
    if ( !_sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event))
    {
        return SMR_OK;
    }

    /*2.Every group handles respective events*/
    switch (SM_EVN_GET_GRP(ui4_event))
    {
        case SM_EVN_RSH_GRP_PVR:
            i4_ret = _sm_rcrd_strm_state_do_pvr_event(h_stream, 
                                                      pt_rs_obj, 
                                                      ui4_event);
            break;

        case SM_EVN_RSH_GRP_PM:
            i4_ret = _sm_rcrd_strm_state_do_pm_event(h_stream,
                                                     pt_rs_obj, 
                                                     ui4_event);
            break;
        case SM_EVN_RSH_GRP_TM:
            i4_ret = _sm_rcrd_strm_state_do_tm_event(h_stream,
                                                     pt_rs_obj, 
                                                     ui4_event);
            break;

        case SM_EVN_RSH_GRP_RUTI:
            i4_ret = _sm_rcrd_strm_state_do_rutil_event(h_stream, 
                                                        pt_rs_obj,
                                                        ui4_event);
            break;

        default:
            SM_RSH_DBG_ERR(("sm_rcrd_strm_state_do_drv_events:receives invalid "
                            "event 0x%.8X\r\n",
                            ui4_event));
            SM_ABORT(SM_DBG_ABRT_INV_EVENT);
    }

    /*If Error send error event*/
    if (i4_ret != SMR_OK)
    {
        sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR, h_stream,  0);
        return i4_ret;
    }


    /*******************3.Collect events to send transfer events*****************/
    /*Need not care of TM event*/
    if (SM_EVN_RSH_GRP_TM == SM_EVN_GET_GRP(ui4_event))
    {
        return i4_ret;
    }
    
    /*3.1 Enter Opened which is ready for playback: 
          May cause by Open or Stop opration*/
    if ((SM_EVN_RSH_PM_COND_STOPPED == pt_rs_obj->ui4_lastest_pm_event)
         && (SM_EVN_RSH_PVR_CONNECTED == pt_rs_obj->ui4_lastest_pvr_event))
    {
        
        if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
            && (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp) )
        {
            /*Need check SM_EVN_RSH_TM_PAT_SWAPED???If no PAT consider OK???*/
            if (SM_EVN_RSH_RUTI_READY == pt_rs_obj->ui4_lastest_rutil_event)
            {
                b_all_opened = TRUE;
            }
        }
        else
        {
            b_all_opened = TRUE;
        }
        
        if (TRUE == b_all_opened)
        {
            sm_rs_send_msg_do_event(SM_EVN_RSH_ALL_OPENED, h_stream, 0);
        }
    }
    
    /*3.2 Enter Opened which is ready for playback: 
          May cause by Stop opration*/
    else if ((SM_EVN_RSH_PM_COND_STOPPED == pt_rs_obj->ui4_lastest_pm_event)
              && (SM_EVN_RSH_PVR_STOPPED == pt_rs_obj->ui4_lastest_pvr_event))
    {
        
        if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
            && (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp))
        {
            /*Need check SM_EVN_RSH_TM_PAT_SWAPED???If no PAT consider OK???*/
            if (SM_EVN_RSH_RUTI_READY == pt_rs_obj->ui4_lastest_rutil_event)
            {
                b_all_opened = TRUE;
            }
        }
        else
        {
            b_all_opened = TRUE;
        }
        
        if (TRUE == b_all_opened)
        {
            sm_rs_send_msg_do_event(SM_EVN_RSH_ALL_STOPPED, h_stream, 0);
        }
    }
    /*3.3 Enter Started: caused by Play operation*/
    else if ((SM_EVN_RSH_PM_COND_PLAYED == pt_rs_obj->ui4_lastest_pm_event)
              && (SM_EVN_RSH_PVR_PLAYED == pt_rs_obj->ui4_lastest_pvr_event))
    {
    
        if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
            && (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp) )
        {
            if (SM_EVN_RSH_RUTI_READY == pt_rs_obj->ui4_lastest_rutil_event)
            {
                b_all_played = TRUE;
            }
            else
            {
                b_all_played = FALSE;
            }
        }
        else
        {
            b_all_played = TRUE;
        }
        
        if (TRUE == b_all_played)
        {
            sm_rs_send_msg_do_event(SM_EVN_RSH_ALL_PLAYED, h_stream, 0);
        }
    }
    /*3.4 Enter Closed: caused by Close operation or error*/
    else if ((SM_EVN_RSH_PM_COND_CLOSED == pt_rs_obj->ui4_lastest_pm_event)
              && (SM_EVN_RSH_PVR_CLOSED == pt_rs_obj->ui4_lastest_pvr_event))
    {
        if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
             && (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp) )
        {
            if (SM_EVN_RSH_RUTI_CLOSED == pt_rs_obj->ui4_lastest_rutil_event)
            {
                b_all_closed = TRUE;
            }
            else
            {
                b_all_closed = FALSE;
            }
        }
        else
        {
            b_all_closed = TRUE;
        }

        if (TRUE == b_all_closed)
        {
            sm_rs_send_msg_do_event(SM_EVN_RSH_ALL_CLOSED, h_stream, 0);
        }
    }
    else
    {
        /*What to do?*/
    }
    
    return SMR_OK;
}


static VOID _sm_rcrd_strm_state_prc_ca_event(SM_RCRD_STREAM_T*  pt_rs_obj)
{

    if (NULL == pt_rs_obj)
    {
        SM_RSH_DBG_ERR(("prc_ca_event:null"));
        return;
    }
    
    switch(pt_rs_obj->ui4_lastest_ca_event)
    {
        case SM_EVN_CA_COND_CLOSED:
        case SM_EVN_CA_COND_UNKNOWN:
        case SM_EVN_CA_COND_FREE:
        case SM_EVN_CA_COND_AUTHORIZED:
        case SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID:
        case SM_EVN_CA_COND_NOT_AUTHORIZED:
            break;
        default:
            SM_RSH_DBG_INFO(("Record stream(0x%.8X), ERROR! Unknown CA event %u.\r\n",
                             (UINT32)pt_rs_obj->t_base.h_stream,
                             pt_rs_obj->ui4_lastest_ca_event));
            return;
    }
    
    SM_RSH_DBG_INFO(("Record stream(0x%.8X), event %u.\r\n",
                     (UINT32)pt_rs_obj->t_base.h_stream,
                      pt_rs_obj->ui4_lastest_ca_event));
    /*Notify APP*/
    sm_rcrd_strm_state_notify_app(pt_rs_obj->t_state.pv_state_nfy_tag,
                                  (SM_COND_T)pt_rs_obj->t_state.ui4_curr_state,
                                  pt_rs_obj->ui4_lastest_ca_event,
                                  pt_rs_obj->ui4_lastest_ca_detail_code);
    
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_state_do_ca_events
 *
 * Description: Notify ca evern to AP
 *
 * Inputs:  
 *       ui4_previous_state: previous state
 *       e_event:                 event e.g.
 *       ui4_curr_state:       current state
 *       pv_tag1:                jenerally is stream obejct pointer
 *       pv_tag2:                jenerally is a NULL
 *
 * Outputs: 
 *       Do state transfering by send out state transfer events
 *
 * Returns: SMR_OK:  Succeed.
 *          Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-09-21 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_state_do_ca_events(UINT32     ui4_curr_state,
                                                  UINT32     ui4_event,
                                                  UINT32     ui4_next_state,
                                                  VOID*      pv_tag1,
                                                  VOID*      pv_tag2)
{
    UINT32                ui4_detail_code = (UINT32)pv_tag2;
    SM_RCRD_STREAM_T*     pt_rs_obj       = NULL;

    SM_ASSERT(pv_tag1);
    pt_rs_obj = (SM_RCRD_STREAM_T*)pv_tag1;
    if (!_sm_rcrd_strm_state_record_latest_event(pt_rs_obj, ui4_event))
    {
        return SMR_OK;
    }

    pt_rs_obj->ui4_lastest_ca_detail_code = ui4_detail_code;

    if (SM_COND_STARTED == ui4_curr_state)
    {
        _sm_rcrd_strm_state_prc_ca_event(pt_rs_obj);
    }

    return SMR_OK;
}

INT32 sm_rcrd_strm_state_do_cci_events(UINT32     ui4_curr_state,
                                                   UINT32     ui4_event,
                                                   UINT32     ui4_next_state,
                                                   VOID*      pv_tag1,
                                                   VOID*      pv_tag2)
{
    UINT32                        ui4_i;
    INT32                         i4_ret          = SMR_OK;
    SM_RCRD_STRM_PM_CP_INFO_LST*  pt_cci_info_lst = NULL;
    SM_RCRD_STREAM_T*             pt_rs_obj       = NULL;
    HANDLE_T                      h_stream;
    SM_HANDLER_T*                 pt_hdlr_obj = NULL;

    SM_ASSERT(pv_tag1);
    SM_ASSERT(pv_tag2);
    
    pt_rs_obj       = (SM_RCRD_STREAM_T*)pv_tag1;
    pt_cci_info_lst = (SM_RCRD_STRM_PM_CP_INFO_LST*)pv_tag2;
    h_stream        = pt_rs_obj->t_base.h_stream;

    for(ui4_i = 0; ui4_i < pt_cci_info_lst->z_cp_info_num; ui4_i++)
    {
#if 0
        i4_ret = x_sm_set(h_stream, 
                          SM_STRM_SET_TYPE_CCI_INFO, 
                          (VOID*)&(pt_cci_info_lst->at_cp_info[ui4_i]),
                          sizeof(CP_INFO_T));
#else
        i4_ret = handle_get_obj( pt_rs_obj->t_base.h_handler,
                                 (VOID**)&pt_hdlr_obj );
                
        i4_ret = pt_hdlr_obj->t_fct_tbl.pf_set( h_stream,
                                                SM_STRM_SET_TYPE_CCI_INFO,
                                                (VOID*)&(pt_cci_info_lst->at_cp_info[ui4_i]),
                                                sizeof(CP_INFO_T) );
#endif
        if ( i4_ret != SMR_OK )
        {
            SM_RSH_DBG_ERR(("CCI:x_sm_set(0x%.8x, SM_STRM_SET_TYPE_CCI_INFO) return %d\r\n",
                             h_stream, 
                             i4_ret));
        }
    }

    /*Notify APP*/
    #if 0
    sm_rcrd_strm_state_notify_app(pt_rs_obj->t_state.pv_state_nfy_tag,
                                  (SM_COND_T)pt_rs_obj->t_state.ui4_curr_state,
                                  ui4_event,
                                  (UINT32)pt_cci_info_lst);
    #endif
    if ( pt_rs_obj->t_base.pf_nfy )
    {
        pt_rs_obj->t_base.pf_nfy( h_stream,
                                  pt_rs_obj->t_base.e_condition,
                                  pt_rs_obj->t_base.pv_nfy_tag,
                                  ui4_event,
                                  (UINT32)pt_cci_info_lst );
    }
    return SMR_OK;
}

