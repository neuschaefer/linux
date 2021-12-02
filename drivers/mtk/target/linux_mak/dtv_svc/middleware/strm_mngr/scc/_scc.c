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
 * $RCSfile: _scc.c,v $
 * $Revision: #2 $
 * $Date: 2012/07/20 $
 * $Author: xingju.wang $
 *
 * Description: 
 *         This header file contains specific implementation of SCC Video 
 *         Component.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_vid_plane.h"
#include "strm_mngr/sm_drv_map.h"
#include "strm_mngr/x_sm.h"
#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/video_hdlr/u_sm_video_hdlr.h"
#include "strm_mngr/sm_drv_comp.h"
#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/scc/_scc.h"
#include "strm_mngr/scc/_scc_vid.h"
#include "strm_mngr/scc/_scc_disp.h"
#include "strm_mngr/scc/_scc_aud.h"
#include "res_mngr/drv/u_drv_cust.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define SCC_THREAD_NAME          "scc"
#define SCC_THREAD_STACK_SIZE    (2 * 1024)
#define SCC_THREAD_PRIORITY      (128)

#define SCC_MSGQ_NAME            "scc_mq"
#define SCC_MSGQ_MAX_NUM         (32)

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static HANDLE_T   h_scc_thread = NULL_HANDLE;
static HANDLE_T   h_scc_msgq   = NULL_HANDLE;
static HANDLE_T   h_scc_lock   = NULL_HANDLE;
static HANDLE_T   h_scc_pmx_lock = NULL_HANDLE;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
static VOID _scc_rm_nfy_fct( 
    HANDLE_T   h_obj,
    UINT8      ui1_port,
    RM_COND_T  e_nfy_cond,
    VOID*      pv_tag,
    UINT32     ui4_data)
{
}

static SELECT_T _scc_rm_comp_collector_fct(
    DRV_TYPE_T   e_drv_type,
    UINT16       ui2_id,
    VOID*        pv_comp_sel_data,
    const VOID*  pv_comp_data,
    SIZE_T       z_comp_data_len,
    UINT16       ui2_num_active,
    UINT16       ui2_num_wait,
    CTRL_TYPE_T  e_ctrl,
    BOOL         b_would_conflict )
{
    INT32     i4_idx = 0;
    UINT16*   pui2_comp_id = (UINT16*)pv_comp_sel_data;
    
    while( pui2_comp_id[i4_idx] != ANY_COMP_ID )
    {
        i4_idx++;
    }
    
    SM_ASSERT( i4_idx < SCC_COMP_MAX_NUM_DRV_COMPS );
    
    pui2_comp_id[i4_idx] = ui2_id;
    
    return SEL_NEXT;
}

static VOID _scc_handle_msg_aud_hdphone_nfy( SM_SCC_MSG_T* pt_msg )
{
    HANDLE_T                         h_aud = NULL_HANDLE;
    INT32                            i4_ret;
    HANDLE_TYPE_T                    e_type;
    SCC_AUD_COMP_T*                  pt_aud_comp;
    x_scc_aud_hdphone_detect_nfy_fct pf_aud_hdphone_detect_nfy;
    VOID*                            pf_aud_hdphone_detect_nfy_tag;

    if ( !pt_msg )
    {
        return;
    }

    _scc_lock();
    
    h_aud = (HANDLE_T)pt_msg->u.t_aud_hdphone_nfy.pv_nfy_tag;
    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        _scc_unlock();
        return;
    }
    
    
    pf_aud_hdphone_detect_nfy = pt_aud_comp->pf_aud_hdphone_detect_nfy;
    pf_aud_hdphone_detect_nfy_tag = pt_aud_comp->pf_aud_hdphone_detect_nfy_tag;
    
    _scc_unlock();
    
    if ( pf_aud_hdphone_detect_nfy )
    {
        pf_aud_hdphone_detect_nfy(
            pf_aud_hdphone_detect_nfy_tag,
           pt_msg->u.t_aud_hdphone_nfy.e_hdphone_cond,
            pt_msg->u.t_aud_hdphone_nfy.ui4_data_1,
            pt_msg->u.t_aud_hdphone_nfy.ui4_data_2 );
    } 
}

static VOID _scc_handle_msg_aud_upload_data_nfy( SM_SCC_MSG_T* pt_msg )
{
    HANDLE_T                         h_aud = NULL_HANDLE;
    INT32                            i4_ret;
    HANDLE_TYPE_T                    e_type;
    SCC_AUD_COMP_T*                  pt_aud_comp;
    x_scc_aud_upload_data_nfy_fct pf_aud_upload_data_nfy;
    VOID*                            pf_aud_upload_data_nfy_tag;

    if ( !pt_msg )
    {
        return;
    }

    _scc_lock();
    
    h_aud = (HANDLE_T)pt_msg->u.t_aud_upload_data_nfy.pv_nfy_tag;
    i4_ret = handle_get_type_obj( h_aud,
                                  &e_type,
                                  (VOID**)&pt_aud_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_aud_comp )
    {
        _scc_unlock();
        return;
    }
    
    
    pf_aud_upload_data_nfy = pt_aud_comp->pf_aud_upload_data_nfy;
    pf_aud_upload_data_nfy_tag = pt_aud_comp->pf_aud_upload_data_nfy_tag;
    
    _scc_unlock();
    
    if ( pf_aud_upload_data_nfy )
    {
        pf_aud_upload_data_nfy(
            pf_aud_upload_data_nfy_tag,
            pt_msg->u.t_aud_upload_data_nfy.ui4_data_1,
            pt_msg->u.t_aud_upload_data_nfy.ui4_data_2 );
    } 
}


static VOID _scc_handle_msg_vid_lb_detect_nfy( SM_SCC_MSG_T* pt_msg )
{
    HANDLE_T                    h_vid = NULL_HANDLE;
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_type;
    SCC_VIDEO_COMP_T*           pt_vid_comp;
    x_scc_vid_lb_detect_nfy_fct pf_vid_lb_detect_nfy;
    VOID*                       pv_vid_lb_detect_nfy_tag;

    if ( !pt_msg )
    {
        return;
    }

    _scc_lock();

    h_vid = (HANDLE_T)pt_msg->u.t_vid_lb_detect_nfy.pv_nfy_tag;
    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        _scc_unlock();
        return;
    }

    pf_vid_lb_detect_nfy = pt_vid_comp->pf_lb_detect_nfy;
    pv_vid_lb_detect_nfy_tag = pt_vid_comp->pv_lb_detect_nfy_tag;
    
    _scc_unlock();
    
    if ( pf_vid_lb_detect_nfy )
    {
        pf_vid_lb_detect_nfy(
            pv_vid_lb_detect_nfy_tag,
            pt_msg->u.t_vid_lb_detect_nfy.ui4_src_width,
            pt_msg->u.t_vid_lb_detect_nfy.ui4_src_height,
            pt_msg->u.t_vid_lb_detect_nfy.ui4_active_width,
            pt_msg->u.t_vid_lb_detect_nfy.ui4_active_height,
            pt_msg->u.t_vid_lb_detect_nfy.ui4_active_x,
            pt_msg->u.t_vid_lb_detect_nfy.ui4_active_y );
    } 
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _scc_comp_sel_fct
 *
 * Description: <function description>
 *
 * Inputs:  e_type           References.
 *          ui2_id           References.
 *          pv_comp_sel_data References.
 *          pv_comp_data     References.
 *          z_comp_data_len  References.
 *          b_in_use         References.
 *
 * Outputs: - 
 *
 * Returns: SEL_COMP_BREAK  Success.
 *
 ----------------------------------------------------------------------------*/
static SELECT_T _scc_comp_sel_fct( 
                    DRV_TYPE_T   e_type,
                    UINT16       ui2_id,
                    VOID*        pv_comp_sel_data,
                    const VOID*  pv_comp_data,
                    SIZE_T       z_comp_data_len,
                    UINT16       ui2_num_active,
                    UINT16       ui2_num_wait,
                    CTRL_TYPE_T  e_ctrl,
                    BOOL         b_would_conflict )
{
    SM_DRV_COMP_INFO_T     t_drv_descr = SM_DRV_COMP_INFO_INIT;

    t_drv_descr.e_type = e_type;
    t_drv_descr.ui2_comp_id = ui2_id;

    if ( sm_drv_map_exists( &t_drv_descr ) || b_would_conflict )
    {
        return SEL_NEXT;
    }
    
    return SEL_COMP_BREAK;
}
/*-----------------------------------------------------------------------------
 * Name: _scc_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  h_obj      References.
 *          ui1_port   References.
 *          e_nfy_cond References.
 *          pv_tag     References.
 *          ui4_data   References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _scc_nfy_fct( HANDLE_T   h_obj,
                            UINT8      ui1_port,
                            RM_COND_T  e_nfy_cond,
                            VOID*      pv_tag,
                            UINT32     ui4_data)
{

}
 /*-----------------------------------------------------------------------------
 * Name: _scc_iframe_build_comp_connect
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 _scc_iframe_build_comp_connect(HANDLE_T            h_vid)
{
    INT32                       i4_ret;
    RM_COND_T                   e_cond;
    SM_DRV_COMP_DESCR_T         t_temple_descr = SM_DRV_COMP_DESCR_INIT;
    HANDLE_TYPE_T               e_type;
    SCC_VIDEO_COMP_T*           pt_vid_comp;

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    /* open video decoder component */
    x_strcpy(t_temple_descr.t_info.s_logical_name, pt_vid_comp->s_grp_name);
    t_temple_descr.t_info.ui1_in_port = ANY_PORT_NUM;
    t_temple_descr.t_info.ui1_num_in_port = ANY_PORT_NUM;
    t_temple_descr.t_info.ui1_num_out_port = ANY_PORT_NUM;
    t_temple_descr.t_info.ui1_out_port = ANY_PORT_NUM;
    t_temple_descr.t_info.ui2_comp_id = ANY_COMP_ID;
    t_temple_descr.t_info.b_shared_comp_id = FALSE;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if ( RMR_OK != _scc_open_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_iframe ) )
#endif
    {
        pt_vid_comp->h_dummy_pipe_iframe = NULL_HANDLE;
    }

    pt_vid_comp->t_iframe_vd_comp_descr = t_temple_descr;
    pt_vid_comp->t_iframe_vd_comp_descr.t_info.e_type = DRVT_VID_DEC;
    pt_vid_comp->t_iframe_vd_comp_descr.t_info.b_shared_comp_id = TRUE;
    i4_ret = sm_drv_comp_open( pt_vid_comp->h_dummy_pipe_iframe,
                               pt_vid_comp->t_iframe_vd_comp_descr.t_info.e_type,
                               pt_vid_comp->t_iframe_vd_comp_descr.t_info.ui2_comp_id,
                               pt_vid_comp->s_grp_name,
                               (VOID*)&pt_vid_comp->t_iframe_vd_comp_descr,
                               &_scc_comp_sel_fct,
                               (VOID*)pt_vid_comp,
                               &_scc_nfy_fct,
                               &pt_vid_comp->t_iframe_vd_comp_descr );
    if ( i4_ret != SMR_OK || pt_vid_comp->t_iframe_vd_comp_descr.h_comp == NULL_HANDLE )
    {
        return SMR_DRV_COMP_NOT_FOUND;
    }

    /* open plane mixer component */
    pt_vid_comp->t_iframe_pmx_comp_descr = t_temple_descr;
    pt_vid_comp->t_iframe_pmx_comp_descr.t_info.e_type = DRVT_PLA_MXR;
    i4_ret = sm_drv_comp_open( NULL_HANDLE,
                               pt_vid_comp->t_iframe_pmx_comp_descr.t_info.e_type,
                               pt_vid_comp->t_iframe_pmx_comp_descr.t_info.ui2_comp_id,
                               pt_vid_comp->s_grp_name,
                               (VOID*)&pt_vid_comp->t_iframe_pmx_comp_descr,
                               &_scc_comp_sel_fct,
                               (VOID*)pt_vid_comp,
                               &_scc_nfy_fct,
                               &pt_vid_comp->t_iframe_pmx_comp_descr );
    if ( i4_ret != SMR_OK || pt_vid_comp->t_iframe_pmx_comp_descr.h_comp == NULL_HANDLE )
    {
        sm_drv_comp_close( &pt_vid_comp->t_iframe_vd_comp_descr );
        return SMR_DRV_COMP_NOT_FOUND;
    }

    /* open video plane component */
    pt_vid_comp->t_iframe_vp_comp_descr = t_temple_descr;
    pt_vid_comp->t_iframe_vp_comp_descr.t_info.e_type = DRVT_VID_PLANE;

    i4_ret = sm_drv_comp_open( NULL_HANDLE,
                                  pt_vid_comp->t_iframe_vp_comp_descr.t_info.e_type,
                                  pt_vid_comp->t_iframe_vp_comp_descr.t_info.ui2_comp_id,
                                  pt_vid_comp->s_grp_name,
                                  (VOID*)&pt_vid_comp->t_iframe_vp_comp_descr,
                                  &_scc_comp_sel_fct,
                                  (VOID*)pt_vid_comp,
                                  &_scc_nfy_fct,
                                  &pt_vid_comp->t_iframe_vp_comp_descr );
    if ( i4_ret != SMR_OK || pt_vid_comp->t_iframe_vp_comp_descr.h_comp == NULL_HANDLE )
    {
        sm_drv_comp_close( &pt_vid_comp->t_iframe_vd_comp_descr );
        sm_drv_comp_close( &pt_vid_comp->t_iframe_pmx_comp_descr );
        return SMR_DRV_COMP_NOT_FOUND;
    }

    /* connet plane mixer to video plane */
    i4_ret = sm_drv_comp_connect( &pt_vid_comp->t_iframe_pmx_comp_descr, 
                                  pt_vid_comp->t_iframe_vp_comp_descr.h_comp,
                                  (VOID*)pt_vid_comp );
    if ( i4_ret != SMR_OK )
    {
        sm_drv_comp_close( &pt_vid_comp->t_iframe_vp_comp_descr );
        sm_drv_comp_close( &pt_vid_comp->t_iframe_pmx_comp_descr );
        sm_drv_comp_close( &pt_vid_comp->t_iframe_vd_comp_descr );
        return SMR_DRV_COMP_ERROR;
    }

    /* connet video plane to video deocder */
    i4_ret = sm_drv_comp_connect( &pt_vid_comp->t_iframe_vp_comp_descr, 
                                  pt_vid_comp->t_iframe_vd_comp_descr.h_comp,
                                  (VOID*)pt_vid_comp );
    if ( i4_ret != SMR_OK )
    {
        sm_drv_comp_disconnect(&pt_vid_comp->t_iframe_pmx_comp_descr);
        sm_drv_comp_close( &pt_vid_comp->t_iframe_vp_comp_descr );
        sm_drv_comp_close( &pt_vid_comp->t_iframe_pmx_comp_descr );
        sm_drv_comp_close( &pt_vid_comp->t_iframe_vd_comp_descr );
        return SMR_DRV_COMP_ERROR;
    }

    i4_ret = rm_set(
        pt_vid_comp->t_iframe_vp_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_PLA_SET_TYPE_CTRL,
        (VOID*)(UINT32)VID_PLA_CTRL_ENABLE,
        sizeof(VID_PLA_CTRL_T),
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        sm_drv_comp_disconnect(&pt_vid_comp->t_iframe_vp_comp_descr);
        sm_drv_comp_disconnect(&pt_vid_comp->t_iframe_pmx_comp_descr);
        sm_drv_comp_close( &pt_vid_comp->t_iframe_vp_comp_descr );
        sm_drv_comp_close( &pt_vid_comp->t_iframe_pmx_comp_descr );
        sm_drv_comp_close( &pt_vid_comp->t_iframe_vd_comp_descr );
        return SMR_DRV_COMP_ERROR;
    }
    
    return i4_ret;
    
}

#ifdef SYS_MHP_SUPPORT
 /*-----------------------------------------------------------------------------
 * Name: _scc_mhp_iframe_comp_open
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 _scc_mhp_iframe_comp_open(HANDLE_T            h_vid)
{
    INT32                       i4_ret;
    SM_DRV_COMP_DESCR_T         t_temple_descr = SM_DRV_COMP_DESCR_INIT;
    HANDLE_TYPE_T               e_type;
    SCC_VIDEO_COMP_T*           pt_vid_comp;

    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }


    /* open video decoder component */
    x_strcpy(t_temple_descr.t_info.s_logical_name, pt_vid_comp->s_grp_name);
    t_temple_descr.t_info.ui1_in_port = ANY_PORT_NUM;
    t_temple_descr.t_info.ui1_num_in_port = ANY_PORT_NUM;
    t_temple_descr.t_info.ui1_num_out_port = ANY_PORT_NUM;
    t_temple_descr.t_info.ui1_out_port = ANY_PORT_NUM;
    t_temple_descr.t_info.ui2_comp_id = ANY_COMP_ID;
    t_temple_descr.t_info.b_shared_comp_id = FALSE;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if ( RMR_OK != _scc_open_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_iframe ) )
#endif
    {
        pt_vid_comp->h_dummy_pipe_iframe = NULL_HANDLE;
    }

    pt_vid_comp->t_iframe_vd_comp_descr = t_temple_descr;
    pt_vid_comp->t_iframe_vd_comp_descr.t_info.e_type = DRVT_VID_DEC;
    pt_vid_comp->t_iframe_vd_comp_descr.t_info.b_shared_comp_id = TRUE;
    i4_ret = sm_drv_comp_open( pt_vid_comp->h_dummy_pipe_iframe,
                               pt_vid_comp->t_iframe_vd_comp_descr.t_info.e_type,
                               pt_vid_comp->t_iframe_vd_comp_descr.t_info.ui2_comp_id,
                               pt_vid_comp->s_grp_name,
                               (VOID*)&pt_vid_comp->t_iframe_vd_comp_descr,
                               &_scc_comp_sel_fct,
                               (VOID*)pt_vid_comp,
                               &_scc_nfy_fct,
                               &pt_vid_comp->t_iframe_vd_comp_descr );
    if ( i4_ret != SMR_OK || pt_vid_comp->t_iframe_vd_comp_descr.h_comp == NULL_HANDLE )
    {
        return SMR_DRV_COMP_NOT_FOUND;
    }

    return i4_ret;
}
#endif 

/*-----------------------------------------------------------------------------
 * Name: _scc_iframe_teardown_comp_connect
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 _scc_iframe_teardown_comp_connect(HANDLE_T h_vid)
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp;
    RM_COND_T                    e_cond;
    
    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    i4_ret = rm_set(
        pt_vid_comp->t_iframe_vp_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_PLA_SET_TYPE_CTRL,
        (VOID*)(UINT32)VID_PLA_CTRL_DISABLE,
        sizeof(VID_PLA_CTRL_T),
        &e_cond);
    if (i4_ret != RMR_OK)
    {
        return SMR_INTERNAL_ERROR;
    }

    i4_ret = sm_drv_comp_disconnect(&pt_vid_comp->t_iframe_vp_comp_descr);
    if (i4_ret != SMR_OK)
    {
        return i4_ret;
    }

    i4_ret = sm_drv_comp_disconnect(&pt_vid_comp->t_iframe_pmx_comp_descr);
    if (i4_ret != SMR_OK)
    {
        return i4_ret;
    }

    i4_ret = sm_drv_comp_close( &pt_vid_comp->t_iframe_pmx_comp_descr );
    if (i4_ret != SMR_OK)
    {
        return i4_ret;
    }
    i4_ret = sm_drv_comp_close( &pt_vid_comp->t_iframe_vp_comp_descr );
    if (i4_ret != SMR_OK)
    {
        return i4_ret;
    }
    i4_ret = sm_drv_comp_close( &pt_vid_comp->t_iframe_vd_comp_descr );
    if (i4_ret != SMR_OK)
    {
        return i4_ret;
    }
    _scc_close_dummy_rm_pipe( &pt_vid_comp->h_dummy_pipe_iframe );
    x_memset(&pt_vid_comp->t_iframe_pmx_comp_descr, 0, sizeof (SM_DRV_COMP_DESCR_T));
    x_memset(&pt_vid_comp->t_iframe_vp_comp_descr, 0, sizeof (SM_DRV_COMP_DESCR_T));
    x_memset(&pt_vid_comp->t_iframe_vd_comp_descr, 0, sizeof (SM_DRV_COMP_DESCR_T));
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _scc_mute_build_comp_connect
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 _scc_mute_build_comp_connect(HANDLE_T                     h_vid,
                                   const SCC_VID_SUPER_BLANK_T* pt_blank)
{
    INT32                       i4_ret;
    RM_COND_T                   e_cond;
    SM_DRV_COMP_DESCR_T         t_temple_descr = SM_DRV_COMP_DESCR_INIT;
    HANDLE_TYPE_T               e_type;
    SCC_VIDEO_COMP_T*           pt_vid_comp;
    VID_PLA_SUPER_BLANK_T       t_drv_blank;
    
    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    x_strcpy(t_temple_descr.t_info.s_logical_name, pt_vid_comp->s_grp_name);
    t_temple_descr.t_info.ui1_in_port = ANY_PORT_NUM;
    t_temple_descr.t_info.ui1_num_in_port = ANY_PORT_NUM;
    t_temple_descr.t_info.ui1_num_out_port = ANY_PORT_NUM;
    t_temple_descr.t_info.ui1_out_port = ANY_PORT_NUM;
    t_temple_descr.t_info.ui2_comp_id = ANY_COMP_ID;
    t_temple_descr.t_info.b_shared_comp_id = FALSE;

    /* open plane mixer component */
    pt_vid_comp->t_mute_pmx_comp_descr = t_temple_descr;
    pt_vid_comp->t_mute_pmx_comp_descr.t_info.e_type = DRVT_PLA_MXR;
    i4_ret = sm_drv_comp_open( NULL_HANDLE,
                               pt_vid_comp->t_mute_pmx_comp_descr.t_info.e_type,
                               pt_vid_comp->t_mute_pmx_comp_descr.t_info.ui2_comp_id,
                               pt_vid_comp->s_grp_name,
                               (VOID*)&pt_vid_comp->t_mute_pmx_comp_descr,
                               &_scc_comp_sel_fct,
                               (VOID*)pt_vid_comp,
                               &_scc_nfy_fct,
                               &pt_vid_comp->t_mute_pmx_comp_descr );
    if ( i4_ret != SMR_OK || pt_vid_comp->t_mute_pmx_comp_descr.h_comp == NULL_HANDLE )
    {
        return SMR_DRV_COMP_NOT_FOUND;
    }

    /* open video plane component */
    pt_vid_comp->t_mute_vp_comp_descr = t_temple_descr;
    pt_vid_comp->t_mute_vp_comp_descr.t_info.e_type = DRVT_VID_PLANE;

    i4_ret = sm_drv_comp_open( NULL_HANDLE,
                               pt_vid_comp->t_mute_vp_comp_descr.t_info.e_type,
                               pt_vid_comp->t_mute_vp_comp_descr.t_info.ui2_comp_id,
                               pt_vid_comp->s_grp_name,
                               (VOID*)&pt_vid_comp->t_mute_vp_comp_descr,
                               &_scc_comp_sel_fct,
                               (VOID*)pt_vid_comp,
                               &_scc_nfy_fct,
                               &pt_vid_comp->t_mute_vp_comp_descr );
    if ( i4_ret != SMR_OK || pt_vid_comp->t_mute_vp_comp_descr.h_comp == NULL_HANDLE )
    {
        sm_drv_comp_close( &pt_vid_comp->t_mute_pmx_comp_descr );
        return SMR_DRV_COMP_NOT_FOUND;
    }

    /* connet plane mixer to video plane */
    i4_ret = sm_drv_comp_connect( &pt_vid_comp->t_mute_pmx_comp_descr, 
                                  pt_vid_comp->t_mute_vp_comp_descr.h_comp,
                                  (VOID*)pt_vid_comp );
    if ( i4_ret != SMR_OK )
    {
        sm_drv_comp_close( &pt_vid_comp->t_mute_vp_comp_descr );
        sm_drv_comp_close( &pt_vid_comp->t_mute_pmx_comp_descr );
        return SMR_DRV_COMP_ERROR;
    }

    i4_ret = rm_set(
        pt_vid_comp->t_mute_vp_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_PLA_SET_TYPE_CTRL,
        (VOID*)(UINT32)VID_PLA_CTRL_ENABLE,
        sizeof(VID_PLA_CTRL_T),
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        sm_drv_comp_disconnect(&pt_vid_comp->t_mute_pmx_comp_descr);
        sm_drv_comp_close( &pt_vid_comp->t_mute_vp_comp_descr );
        sm_drv_comp_close( &pt_vid_comp->t_mute_pmx_comp_descr );
        return SMR_DRV_COMP_ERROR;
    }

    x_memset( &t_drv_blank, 0, sizeof(VID_PLA_SUPER_BLANK_T) );
    t_drv_blank.b_enable = TRUE;
    t_drv_blank.t_bg_color.ui1_r = pt_blank->t_bg_color.ui1_red;
    t_drv_blank.t_bg_color.ui1_g = pt_blank->t_bg_color.ui1_green;
    t_drv_blank.t_bg_color.ui1_b = pt_blank->t_bg_color.ui1_blue;
    t_drv_blank.t_disp_region.ui4_x = pt_blank->t_disp_region.ui4_x;
    t_drv_blank.t_disp_region.ui4_y = pt_blank->t_disp_region.ui4_y;
    t_drv_blank.t_disp_region.ui4_width = pt_blank->t_disp_region.ui4_width;
    t_drv_blank.t_disp_region.ui4_height = pt_blank->t_disp_region.ui4_height;
    i4_ret = rm_set(
        pt_vid_comp->t_mute_vp_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_PLA_SET_TYPE_SUPER_BLANK,
        (VOID*)&t_drv_blank,
        sizeof(VID_PLA_SUPER_BLANK_T),
        &e_cond);
    if ( i4_ret != RMR_OK )
    {
        i4_ret = rm_set(
            pt_vid_comp->t_mute_vp_comp_descr.h_comp,
            FALSE,
            ANY_PORT_NUM,
            VID_PLA_SET_TYPE_CTRL,
            (VOID*)(UINT32)VID_PLA_CTRL_DISABLE,
            sizeof(VID_PLA_CTRL_T),
            &e_cond);
        sm_drv_comp_disconnect(&pt_vid_comp->t_mute_pmx_comp_descr);
        sm_drv_comp_close( &pt_vid_comp->t_mute_vp_comp_descr );
        sm_drv_comp_close( &pt_vid_comp->t_mute_pmx_comp_descr );
        return SMR_INV_SET_INFO;
    }
    
    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _scc_mute_teardown_comp_connect
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 _scc_mute_teardown_comp_connect(HANDLE_T h_vid)
{
    INT32                         i4_ret;
    HANDLE_TYPE_T                 e_type;
    SCC_VIDEO_COMP_T*             pt_vid_comp;
    RM_COND_T                     e_cond;
    VID_PLA_SUPER_BLANK_T         t_drv_blank;
    
    i4_ret = handle_get_type_obj( h_vid,
                                  &e_type,
                                  (VOID**)&pt_vid_comp );
    if ( i4_ret != HR_OK || e_type != SM_HT_SCC_GRP_NAME || !pt_vid_comp )
    {
        return SMR_INV_HANDLE;
    }

    x_memset( &t_drv_blank, 0, sizeof(VID_PLA_SUPER_BLANK_T) );
    t_drv_blank.b_enable = FALSE;
    i4_ret = rm_set(
        pt_vid_comp->t_mute_vp_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_PLA_SET_TYPE_SUPER_BLANK,
        (VOID*)&t_drv_blank,
        sizeof(VID_PLA_SUPER_BLANK_T),
        &e_cond);
    if (i4_ret != RMR_OK)
    {
        return SMR_INTERNAL_ERROR;
    }
            
    i4_ret = rm_set(
        pt_vid_comp->t_mute_vp_comp_descr.h_comp,
        FALSE,
        ANY_PORT_NUM,
        VID_PLA_SET_TYPE_CTRL,
        (VOID*)(UINT32)VID_PLA_CTRL_DISABLE,
        sizeof(VID_PLA_CTRL_T),
        &e_cond);
    if (i4_ret != RMR_OK)
    {
        return SMR_INTERNAL_ERROR;
    }

    i4_ret = sm_drv_comp_disconnect(&pt_vid_comp->t_mute_pmx_comp_descr);
    if (i4_ret != SMR_OK)
    {
        return i4_ret;
    }
    i4_ret = sm_drv_comp_close( &pt_vid_comp->t_mute_pmx_comp_descr );
    if (i4_ret != SMR_OK)
    {
        return i4_ret;
    }
    i4_ret = sm_drv_comp_close( &pt_vid_comp->t_mute_vp_comp_descr );
    if (i4_ret != SMR_OK)
    {
        return i4_ret;
    }
    x_memset(&pt_vid_comp->t_mute_pmx_comp_descr, 0, sizeof (SM_DRV_COMP_DESCR_T));
    x_memset(&pt_vid_comp->t_mute_vp_comp_descr, 0, sizeof (SM_DRV_COMP_DESCR_T));
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _scc_open_drv_from_handle
 *
 * Description: <function description>
 *
 * Inputs:  pv_arg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
HANDLE_T _scc_open_drv_from_handle( HANDLE_T    h_drv )
{
    INT32               i4_ret = RMR_OK;
    HANDLE_T            h_new_drv = NULL_HANDLE;
    DRV_TYPE_T          e_drv_type;
    UINT16              ui2_comp_id = 0;
    RM_COND_T           e_cond;
    
    i4_ret = rm_get_comp_info_from_handle( 
        h_drv,
        (DRV_TYPE_T*)&e_drv_type,
        (UINT16*)&ui2_comp_id,
        NULL,
        NULL,
        NULL );
    if ( i4_ret != RMR_OK )
    {
        SM_SLCTR_DBG_INFO(( 
            "[SCC] %s:%d rm_get_comp_info_from_handle() fail return %d\r\n",
            __FILE__, __LINE__,
            i4_ret ));
        return NULL_HANDLE;
    }
    
    i4_ret = rm_open_comp( NULL_HANDLE,
                           OBJ_COMP_REL_IGNORE,
                           e_drv_type,
                           ui2_comp_id,
                           NULL,
                           CTRL_TYPE_SHARED,
                           NULL,
                           NULL,
                           NULL,
                           _scc_rm_nfy_fct,
                           &h_new_drv,
                           &e_cond );
    if ( i4_ret != RMR_OK )
    {
        SM_SLCTR_DBG_INFO(( 
            "[SCC] %s:%d rm_open_comp() fail return %d\r\n",
            __FILE__, __LINE__,
            i4_ret ));
        return NULL_HANDLE;
    }
    
    return h_new_drv;
}

/*-----------------------------------------------------------------------------
 * Name: _scc_drv_grp_close
 *
 * Description: <function description>
 *
 * Inputs:  pv_arg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 _scc_drv_grp_close(
    HANDLE_T*           ph_hdl_array,
    SIZE_T              z_hdl_array_size )
{
    UINT32    ui4_idx = 0;
    INT32     i4_ret;
    RM_COND_T e_cond;
    
    if ( !ph_hdl_array || z_hdl_array_size == 0 )
    {
        return SMR_OK;
    }
    
    while( ui4_idx < z_hdl_array_size && ph_hdl_array[ui4_idx] != NULL_HANDLE )
    {
        i4_ret = rm_close( ph_hdl_array[ui4_idx++],
                           TRUE,
                           &e_cond );
        if ( i4_ret != RMR_OK )
        {
            SM_SLCTR_DBG_INFO(( 
                "[SCC] %s:%d rm_close() fail return %d\r\n",
                __FILE__, __LINE__,
                i4_ret ));
        }
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _scc_drv_grp_open_from_name
 *
 * Description: <function description>
 *
 * Inputs:  pv_arg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 _scc_drv_grp_open_from_name(
    HANDLE_T            h_obj,
    DRV_TYPE_T          e_drv_type,
    const CHAR*         s_grp_name,
    HANDLE_T*           ph_hdl_array,
    SIZE_T*             pz_hdl_array_size )
{
    INT32               i4_ret = RMR_OK;
    INT32               i4_idx = 0;
    UINT16              ui2_comp_max;
    UINT16              aui2_comp_id[SCC_COMP_MAX_NUM_DRV_COMPS];
    RM_COND_T           e_cond;
    HANDLE_T            h_new_drv = NULL_HANDLE;
    BOOL                b_is_open = FALSE;
    
    if ( !s_grp_name || !ph_hdl_array || !pz_hdl_array_size )
    {
        return SMR_INV_ARG;
    }
    
    i4_ret = rm_num_comps( e_drv_type, &ui2_comp_max );
    if ( i4_ret != RMR_OK )
    {
        SM_SLCTR_DBG_INFO(( 
            "[SCC] %s:%d rm_num_comps() fail return %d\r\n",
            __FILE__, __LINE__,
            i4_ret ));
        return SMR_INTERNAL_ERROR;
    }
    if ( ui2_comp_max > SCC_COMP_MAX_NUM_DRV_COMPS ||
         (SIZE_T)ui2_comp_max > (*pz_hdl_array_size) )
    {
        SM_SLCTR_DBG_INFO(( 
            "[SCC] _scc_open_drv_from_name() too many driver components of type %u\r\n",
            e_drv_type ));
        SM_ASSERT( 0 );
        return SMR_INTERNAL_ERROR;
    }
    
    for ( i4_idx = 0; i4_idx < SCC_COMP_MAX_NUM_DRV_COMPS; i4_idx++ )
    {
        aui2_comp_id[i4_idx] = ANY_COMP_ID;
    }
    
    /* Collect all driver components */
    i4_ret = rm_open_comp( 
        h_obj,
        OBJ_COMP_REL_IGNORE,
        e_drv_type,
        ANY_COMP_ID,
        s_grp_name,
#ifdef MW_TV_AV_BYPASS_SUPPORT
        ((e_drv_type==DRVT_VID_DEC || e_drv_type==DRVT_TV_DEC) ?
         CTRL_TYPE_MASTER_SLAVE: CTRL_TYPE_SHARED),
#else
        ((e_drv_type==DRVT_TV_DEC) ?
         CTRL_TYPE_MASTER_SLAVE: CTRL_TYPE_SHARED),
#endif
        aui2_comp_id,
        (rm_comp_sel_fct)_scc_rm_comp_collector_fct,
        NULL,
        _scc_rm_nfy_fct,
        &h_new_drv,
        &e_cond );
    
    i4_idx = 0;
    while( aui2_comp_id[i4_idx] != ANY_COMP_ID )
    {
        i4_ret = rm_open_comp( 
            h_obj,
            OBJ_COMP_REL_IGNORE,
            e_drv_type,
            aui2_comp_id[i4_idx],
            s_grp_name,
#ifdef MW_TV_AV_BYPASS_SUPPORT
            ((e_drv_type==DRVT_VID_DEC || e_drv_type==DRVT_TV_DEC)?CTRL_TYPE_MASTER_SLAVE:CTRL_TYPE_SHARED),
#else
            ((e_drv_type==DRVT_TV_DEC)?CTRL_TYPE_MASTER_SLAVE:CTRL_TYPE_SHARED),
#endif
            NULL,
            (rm_comp_sel_fct)NULL,
            NULL,
            _scc_rm_nfy_fct,
            &ph_hdl_array[i4_idx],
            &e_cond );
        if ( i4_ret != RMR_OK )
        {
            SM_SLCTR_DBG_INFO(( 
                "[SCC] %s:%d rm_open_comp() fail return %d\r\n",
                __FILE__, __LINE__,
                i4_ret ));
            
            _scc_drv_grp_close( ph_hdl_array, (SIZE_T)i4_idx );            
            return SMR_INTERNAL_ERROR;
        }
        b_is_open = TRUE;
        
        i4_idx++;
    }
    
    *pz_hdl_array_size = (SIZE_T)i4_idx;
    
    if (b_is_open)
    {
    return SMR_OK;
}
    else
    {
        return SMR_INTERNAL_ERROR;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _sm_scc_thread_main
 *
 * Description: <function description>
 *
 * Inputs:  pv_arg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _sm_scc_thread_main( VOID*  pv_arg )
{
    INT32                       i4_ret;
    UINT16                      ui2_index;
    SM_SCC_MSG_T                t_msg;
    SIZE_T                      z_size_1;  /*async*/
    SIZE_T                      z_size_2;  /*async*/
    SIZE_T                      z_msg_size;
    x_scc_aud_clip_nfy_fct      pf_aud_clip_nfy;
    x_scc_vid_iframe_nfy_fct    pf_vid_iframe_nfy;
    x_scc_vid_src_detect_nfy_fct    pf_vid_src_detect_nfy;
    DRV_CUSTOM_MISC_CRNT_INP_SRC_INFO_T t_crnt_inp_src_info;  /*async*/
    DRV_CUST_OPERATION_INFO_T   t_op_info;  /*async*/
    BOOL                        b_shutdown = FALSE;
    
    while( !b_shutdown )
    {
        z_msg_size = sizeof( SM_SCC_MSG_T );
        
        i4_ret = x_msg_q_receive( &ui2_index,
                                  &t_msg,
                                  &z_msg_size,
                                  &h_scc_msgq,
                                  (UINT16)1,
                                  X_MSGQ_OPTION_WAIT );
        if ( i4_ret != OSR_OK )
        {
            if ( i4_ret != OSR_NO_MSG )
            {
                SM_ABORT( SM_DBG_ABRT_CANNOT_RECV_FROM_MSG_Q );
            }
            continue;
        }
        
        switch( t_msg.e_msg_type )
        {
        case SM_SCC_MSG_TYPE_AUD_CLIP_NFY:
            pf_aud_clip_nfy = t_msg.u.t_aud_clip_nfy.pf_nfy_fct;
            if (pf_aud_clip_nfy)
            {
                pf_aud_clip_nfy(   t_msg.u.t_aud_clip_nfy.h_aud,
                                   t_msg.u.t_aud_clip_nfy.e_nfy_cond,
                                   t_msg.u.t_aud_clip_nfy.pv_nfy_tag,
                                   t_msg.u.t_aud_clip_nfy.ui4_data_1, 
                                   t_msg.u.t_aud_clip_nfy.ui4_data_2);
            }
            break;
            
        case SM_SCC_MSG_TYPE_IFRAME_NFY:
            pf_vid_iframe_nfy = t_msg.u.t_vid_iframe_nfy.pf_nfy_fct;
            if (pf_vid_iframe_nfy)
            {
                if ((t_msg.u.t_vid_iframe_nfy.e_nfy_cond == SCC_VID_IFRAME_COND_CTRL_DONE)
                    && (t_msg.u.t_vid_iframe_nfy.ui4_data_1 == SCC_VID_IFRAME_COND_END_CTRL_DONE))
                {   /* disconnect it and tear down component graph */
                    i4_ret = _scc_iframe_teardown_comp_connect(t_msg.u.t_vid_iframe_nfy.h_vid);
                    if ( i4_ret != SMR_OK )
                    {
                        t_msg.u.t_vid_iframe_nfy.ui4_data_2 = (UINT32)SMR_INTERNAL_ERROR;
                    }
                }
                pf_vid_iframe_nfy(  t_msg.u.t_vid_iframe_nfy.h_vid,
                                    t_msg.u.t_vid_iframe_nfy.e_nfy_cond,
                                    t_msg.u.t_vid_iframe_nfy.pv_nfy_tag,
                                    t_msg.u.t_vid_iframe_nfy.ui4_data_1, 
                                    t_msg.u.t_vid_iframe_nfy.ui4_data_2);
            }
            break;
            
        case SM_SCC_MSG_TYPE_DISP_CTRL:
            i4_ret = _scc_disp_set_ctrl( 
                t_msg.u.t_disp_ctrl.h_disp,
                t_msg.u.t_disp_ctrl.e_disp_ctrl );
            if ( i4_ret != SMR_OK )
            {
                SM_SLCTR_DBG_INFO(( "[SM] %s:%d, _scc_disp_set_ctrl() ERROR, return %d\r\n",
                    __FILE__, __LINE__, i4_ret ));
            }
            
            i4_ret = x_scc_comp_close( t_msg.u.t_disp_ctrl.h_disp );
            if ( i4_ret != SMR_OK )
            {
                SM_SLCTR_DBG_INFO(( "[SM] %s:%d, x_scc_comp_close() ERROR, return %d\r\n",
                    __FILE__, __LINE__, i4_ret ));
            }
            break;
        
        case SM_SCC_MSG_TYPE_SHUTDOWN:
            b_shutdown = TRUE;
            break;
        
        case SM_SCC_MSG_TYPE_HDPHONE_NFY:
            _scc_handle_msg_aud_hdphone_nfy( &t_msg );
            break;
        
        case SM_SCC_MSG_TYPE_UPLOAD_DATA_NFY:
            _scc_handle_msg_aud_upload_data_nfy( &t_msg );
            break;
        
        case SM_SCC_MSG_TYPE_LB_DETECT_NFY:
            _scc_handle_msg_vid_lb_detect_nfy( &t_msg );
            break;
        
        case SM_SCC_MSG_TYPE_SRC_DETECT_NFY:
            pf_vid_src_detect_nfy = t_msg.u.t_vid_src_detect_nfy.pf_nfy_fct;
            if (pf_vid_src_detect_nfy)
            {
                pf_vid_src_detect_nfy(   t_msg.u.t_vid_src_detect_nfy.h_vid,
                                         t_msg.u.t_vid_src_detect_nfy.e_nfy_cond,
                                         t_msg.u.t_vid_src_detect_nfy.pv_nfy_tag,
                                         t_msg.u.t_vid_src_detect_nfy.ui4_data_1, 
                                         t_msg.u.t_vid_src_detect_nfy.ui4_data_2);
            }
            break;

        case SM_SCC_MSG_TYPE_CRNT_INP_SRC_SET:  /*async*/
            z_size_1 = sizeof(DRV_CUSTOM_MISC_CRNT_INP_SRC_INFO_T);
            x_memset ( &t_crnt_inp_src_info, 
                0, 
                z_size_1);
            
            switch (t_msg.u.t_crnt_inp_src.e_inp_src_type)
            {
            case SCC_CRNT_INP_SRC_TV:
                t_crnt_inp_src_info.e_type = DRV_CUSTOM_MISC_CRNT_INP_SRC_TV;
                break;
            case SCC_CRNT_INP_SRC_COMP_VIDEO:
                t_crnt_inp_src_info.e_type = DRV_CUSTOM_MISC_CRNT_INP_SRC_COMP_VIDEO;
                break;
            case SCC_CRNT_INP_SRC_S_VIDEO:
                t_crnt_inp_src_info.e_type = DRV_CUSTOM_MISC_CRNT_INP_SRC_S_VIDEO;
                break;
            case SCC_CRNT_INP_SRC_Y_PB_PR:
                t_crnt_inp_src_info.e_type = DRV_CUSTOM_MISC_CRNT_INP_SRC_Y_PB_PR;
                break;
            case SCC_CRNT_INP_SRC_VGA:
                t_crnt_inp_src_info.e_type = DRV_CUSTOM_MISC_CRNT_INP_SRC_VGA;
                break;
            case SCC_CRNT_INP_SRC_SCART:
                t_crnt_inp_src_info.e_type = DRV_CUSTOM_MISC_CRNT_INP_SRC_SCART;
                break;
            case SCC_CRNT_INP_SRC_HDMI:
                t_crnt_inp_src_info.e_type = DRV_CUSTOM_MISC_CRNT_INP_SRC_HDMI;
                break;
            default:
                x_dbg_stmt("[SCC] Invalid input source type \r\n");
                continue;
            }
            t_crnt_inp_src_info.ui1_port_num = t_msg.u.t_crnt_inp_src.ui1_port_num;
            
            z_size_2 = sizeof(DRV_CUST_OPERATION_INFO_T);
            x_memset( &t_op_info, 
                       0, 
                       z_size_2);
            
            t_op_info.e_op_type = DRV_CUSTOM_OPERATION_TYPE_MISC_SET;
            t_op_info.u.t_misc_set_info.e_misc_type= DRV_CUSTOM_SET_CRNT_INP_SRC_INFO;
            t_op_info.u.t_misc_set_info.pv_set_info= (VOID*)&t_crnt_inp_src_info;
            t_op_info.u.t_misc_set_info.z_size= z_size_1;
            i4_ret = x_rm_set_comp (
                       DRVT_CUST_DRV,
                       DRV_CUST_COMP_ID,
                       FALSE,
                       ANY_PORT_NUM,
                       0,
                       &t_op_info,
                       z_size_2
                       );
            if (i4_ret != RMR_OK)
            {
                x_dbg_stmt("[SCC] Set current input source info Fail! i4_ret=%d\r\n", i4_ret);
            }
            break;
        
        default:
            SM_SLCTR_DBG_ERR(( "[SM] _sm_scc_thread_main(), "
                               "got unhandled message type=%d\r\n",
                               t_msg.e_msg_type ));
            continue;
        }
    }
    
    SM_SLCTR_DBG_INFO(( "[SM] _sm_scc_thread_main() exited\r\n" ));
}

/*-----------------------------------------------------------------------------
 * Name: _scc_send_msg
 *
 * Description: <function description>
 *
 * Inputs:  pt_msg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 _scc_send_msg( const SM_SCC_MSG_T* pt_msg )
{
    INT32    i4_ret;
    
    i4_ret = x_msg_q_send( h_scc_msgq,
                           (VOID*)pt_msg,
                           sizeof( SM_SCC_MSG_T ),
                           10 );
    if ( i4_ret != OSR_OK )
    {
        return SMR_INTERNAL_ERROR;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: scc_init
 *
 * Description: <function description>
 *
 * Inputs:  pv_arg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 scc_init( VOID )
{
    INT32    i4_ret;
    
    i4_ret = x_msg_q_create( &h_scc_msgq,
                             (CHAR*)SCC_MSGQ_NAME,
                             sizeof( SM_SCC_MSG_T ),
                             SCC_MSGQ_MAX_NUM );
    if ( i4_ret != OSR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_CREATE_MSG_Q );
    }

    i4_ret = x_thread_create( &h_scc_thread,
                              SCC_THREAD_NAME,
                              SCC_THREAD_STACK_SIZE,
                              SCC_THREAD_PRIORITY,
                              _sm_scc_thread_main,
                              0,
                              NULL );
    if ( i4_ret != OSR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_CREATE_THREAD );
    }
    
    i4_ret = x_sema_create( &h_scc_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_CREATE_SEMAPHORE );
    }

    i4_ret = x_sema_create( &h_scc_pmx_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_CREATE_SEMAPHORE );
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _scc_lock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID _scc_lock(VOID)
{
    INT32   i4_ret;
    
    i4_ret = x_sema_lock( h_scc_lock, X_SEMA_OPTION_WAIT );
    if ( i4_ret != OSR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_LOCK_SEMAPHORE );
    }
}

/*-----------------------------------------------------------------------------
 * Name: _scc_unlock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID _scc_unlock(VOID)
{
    INT32   i4_ret;
    
    i4_ret = x_sema_unlock( h_scc_lock );
    if ( i4_ret != OSR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE );
    }
}

/*-----------------------------------------------------------------------------
 * Name: _scc_pmx_lock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID _scc_pmx_lock(VOID)
{
    INT32   i4_ret;
    
    i4_ret = x_sema_lock( h_scc_pmx_lock, X_SEMA_OPTION_WAIT );
    if ( i4_ret != OSR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_LOCK_SEMAPHORE );
    }
}

/*-----------------------------------------------------------------------------
 * Name: _scc_pmx_unlock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID _scc_pmx_unlock(VOID)
{
    INT32   i4_ret;
    
    i4_ret = x_sema_unlock( h_scc_pmx_lock );
    if ( i4_ret != OSR_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE );
    }
}

/*-----------------------------------------------------------------------------
 * Name: _scc_open_dummy_pipe
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 _scc_open_dummy_rm_pipe( 
    HANDLE_T*           ph_pipe )
{
    INT32               i4_ret;
    RM_COND_T           e_cond;

    i4_ret = rm_open_pipe(
        NULL,
        NULL,
        255,
        RM_PIPE_FLAG_ALLOW_SLAVE_SET_OP,
        NULL,
        _scc_rm_nfy_fct,
        ph_pipe,
        &e_cond);

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _scc_close_dummy_rm_pipe
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
INT32 _scc_close_dummy_rm_pipe( 
    HANDLE_T*           ph_pipe )
{
    INT32               i4_ret;
    RM_COND_T           e_rm_cond;
    
    if ( *ph_pipe == NULL_HANDLE )
    {
        return RMR_INV_HANDLE;
    }
    
    i4_ret = rm_close(*ph_pipe, TRUE, &e_rm_cond);
    if ( i4_ret == RMR_OK )
    {
        *ph_pipe = NULL_HANDLE;
    }
    
    return i4_ret;
}
