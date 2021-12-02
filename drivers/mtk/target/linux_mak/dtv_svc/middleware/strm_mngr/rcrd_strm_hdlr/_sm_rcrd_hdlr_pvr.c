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
 * $RCSfile: _sm_sess_rcrd_pvr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/10 $
 * $SWAuthor: Lianming Lin $
 * $MD5HEX: 9651a12385e0d96d6be213500a0475f8 $
 *
 * Description: 
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/x_mm_common.h"
#include "handle/u_handle.h"
#include "handle/x_handle.h"
#include "util/x_lnk_list.h"
#include "os/inc/x_os.h"

#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"

#include "strm_mngr/rcrd_strm_hdlr/_sm_rcrd_hdlr_pvr.h"
#include "strm_mngr/sess_rcrd_hdlr/sm_sess_rcrd.h"

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
 * Name:  _sm_rcrd_strm_pvr_rm_nfy_fct
 *
 * Description: This function is PVR stream RM notify function
 *
 * Inputs:  
 *       h_obj:      PVR Stream component handle
 *       ui1_port:   Input port
 *       e_nfy_cond: PVR condition
 *       pv_tag:     notify tag
 *       ui4_data:   additional info e.g. reason info
 *
 * Outputs: 
 *       Send a message to session to update PVR condition in SM
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-14 : initial
 ------------------------------------------------------------------------------*/
static VOID _sm_rcrd_strm_pvr_rm_nfy_fct(HANDLE_T   h_obj,
                                         UINT8      ui1_port,
                                         RM_COND_T  e_nfy_cond,
                                         VOID*      pv_tag,
                                         UINT32     ui4_data)
{
    SM_RCRD_STRM_HDLR_MSG_T     t_msg;
    
    if (NULL == pv_tag)
    {
        return;
    }
    
    t_msg.e_msg_type = SM_RCRD_STRM_MSG_TYPE_PVR; 
    t_msg.h_stream = (HANDLE_T)pv_tag;
    
    t_msg.u.t_pvr_msg.h_comp = h_obj;
    t_msg.u.t_pvr_msg.ui1_port = ui1_port;
    t_msg.u.t_pvr_msg.e_nfy_cond = e_nfy_cond;
    t_msg.u.t_pvr_msg.ui4_data = ui4_data;
    t_msg.u.t_pvr_msg.b_should_update_comp_descr = TRUE;
    
    sm_rs_send_msg(&t_msg);
}

/*-----------------------------------------------------------------------------
 * Name:  _rm_comp_sel_fct_pvr_strm
 *
 * Description: This function is RM select callback when open a component
 *
 * Inputs:  
 *       e_type:           driver type
 *       ui2_id:           driver ID
 *       e_nfy_cond:       PVR condition
 *       pv_comp_sel_data: select data passed when open a component
 *       pv_comp_data:     component info set when register a component
 *       z_comp_data_len:  component info length
 *       ui2_num_active:   actvie elements number generated by this component
 *       ui2_num_wait:     waiting elements number will be generated by this component
 *       e_ctrl:           open mode
 *       b_would_conflict: if conflicts with other component
 *
 * Outputs: 
 *       Select a component when finds multiple components match given profile in RM
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-14 : initial
 ------------------------------------------------------------------------------*/
static SELECT_T _rm_comp_sel_fct_pvr_strm(DRV_TYPE_T   e_type,
                                          UINT16       ui2_id,
                                          VOID*        pv_comp_sel_data,
                                          const VOID*  pv_comp_data,
                                          SIZE_T       z_comp_data_len,
                                          UINT16       ui2_num_active,
                                          UINT16       ui2_num_wait,
                                          CTRL_TYPE_T  e_ctrl,
                                          BOOL         b_would_conflict)
{
    SM_DRV_COMP_INFO_T     t_drv_descr = SM_DRV_COMP_INFO_INIT;
    INT32                  i4_ret;

    t_drv_descr.e_type         = e_type;
    t_drv_descr.ui2_comp_id    = ui2_id;
    x_memset(t_drv_descr.s_logical_name, 0, SM_DRV_LOGICAL_NAME_LEN);
    
    i4_ret = rm_get_comp_info_by_id (e_type,
                                     ui2_id,
                                     & t_drv_descr.ui1_num_in_port,
                                     & t_drv_descr.ui1_num_out_port,
                                     & t_drv_descr.ui4_comp_flags );

    if (i4_ret != RMR_OK)
    {
        SM_RSH_DBG_ERR(("rm_get_comp_info_by_id () error = %d\r\n", i4_ret));
        return SEL_NEXT;
    }

    if (sm_drv_map_exists(&t_drv_descr) ||(TRUE == b_would_conflict))
    {
        return SEL_NEXT;
    }
    
    return SEL_COMP_BREAK;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_pvr_open
 *
 * Description: This function opens a PVR stream component to for recording
 *
 * Inputs:  
 *       h_stream:  stream handle
 *
 * Outputs: 
 *       pt_rs_obj: open a pvr stream component
 *
 * Returns: 
 *       SMR_OK: Succeed
 *       Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-14 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_pvr_open(HANDLE_T          h_stream, 
                            SM_RCRD_STREAM_T* pt_rs_obj)
{
    INT32                    i4_ret;
    SM_DRV_COMP_DESCR_T*     pt_comp_descr; 
    RM_COND_T                e_cond;
    SM_RCRD_STRM_HDLR_MSG_T  t_msg;
    
    if (NULL == pt_rs_obj)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_open:NULL argument!\r\n"));
        return SMR_INV_ARG;
    }

    i4_ret = SMR_OK;
    pt_comp_descr = &(pt_rs_obj->t_pvr_descr);

    if (RSH_RCRD_TYPE_PCR == pt_rs_obj->t_rcrd_type)
    {
        pt_rs_obj->t_pvr_descr.t_info.e_type = DRVT_PVR_STREAM_PCR;
    }
    else
    {
        pt_rs_obj->t_pvr_descr.t_info.e_type = DRVT_PVR_STREAM;
    }

    /*Open a component , may exist multiple same type drivers.*/
    i4_ret = sm_drv_comp_open(pt_rs_obj->t_base.h_source,
                              pt_rs_obj->t_pvr_descr.t_info.e_type,
                              pt_comp_descr->t_info.ui2_comp_id,
                              NULL, //pt_comp_descr->t_info.s_logical_name,
                              (VOID*)pt_comp_descr,
                              &_rm_comp_sel_fct_pvr_strm,
                              (VOID*)(h_stream),
                              &_sm_rcrd_strm_pvr_rm_nfy_fct,
                              &(pt_rs_obj->t_pvr_descr));
    
    if (i4_ret != SMR_OK)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_open:open pvr err=%d!\r\n", i4_ret));
        return i4_ret;   
    }

    /*Filter out object state*/
    e_cond = sm_rm_cond_filter(pt_comp_descr->e_comp_cond,
                               COND_GRP_OBJ_STATE);
    if (RM_COND_OPENED == e_cond)
    {
        t_msg.e_msg_type = SM_RCRD_STRM_MSG_TYPE_PVR;
        t_msg.h_stream = h_stream;
        
        t_msg.u.t_pvr_msg.h_comp = pt_comp_descr->h_comp;
        t_msg.u.t_pvr_msg.ui1_port = pt_comp_descr->t_info.ui1_in_port;
        t_msg.u.t_pvr_msg.e_nfy_cond = pt_comp_descr->e_comp_cond;
        t_msg.u.t_pvr_msg.ui4_data = 0;
        t_msg.u.t_pvr_msg.b_should_update_comp_descr = FALSE;
        
        sm_rs_send_msg(&t_msg);
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_init_pvr
 *
 * Description: This function inits a PVR stream component
 *
 * Inputs:  
 *       h_stream:  stream handle
 *
 * Outputs: 
 *       pt_rs_obj: init a pvr stream component
 *
 * Returns: 
 *       SMR_OK: Succeed
 *       Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-14 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_init_pvr(HANDLE_T          h_stream, 
                            SM_RCRD_STREAM_T* pt_rs_obj)
{
    INT32                i4_ret;
    
    if ((NULL_HANDLE == h_stream) || (NULL == pt_rs_obj))
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_init_pvr: NULL argument!\r\n"));
        return SMR_INV_ARG;
    }

    
    if (TRUE == pt_rs_obj->b_en_pic_search)
    {
        i4_ret = sm_rcrd_strm_pvr_set(pt_rs_obj->t_base.h_stream, 
                                      PVR_SET_TYPE_EN_PIC_SEARCH, 
                                      (VOID*)(UINT32)(pt_rs_obj->b_en_pic_search_value), 
                                      0);
        if (i4_ret != RMR_OK)
        {
            SM_RSH_DBG_ERR(("sm_rcrd_strm_init_pvr:set PVR pic search err=%d!\r\n", 
                            i4_ret));
           return SMR_DRV_COMP_ERROR;
        }
        else
        {
            pt_rs_obj->b_en_pic_search = FALSE;
            
            SM_RSH_DBG_INFO(("sm_rcrd_strm_init_pvr:set stream(pid=%u)PVR pic search OK!!!\r\n",
                             pt_rs_obj->t_pid));
        }

    }

    if (TRUE == pt_rs_obj->b_en_encryption)
    {
        i4_ret = sm_rcrd_strm_pvr_set(pt_rs_obj->t_base.h_stream, 
                                      PVR_SET_TYPE_ENCRYPTION, 
                                      (VOID*)&(pt_rs_obj->t_sess_key), 
                                      (SIZE_T)(sizeof(MM_CRYPT_INFO_T)));
        if (i4_ret != RMR_OK)
        {
            SM_RSH_DBG_ERR(("sm_rcrd_strm_init_pvr:set PVR_SET_TYPE_ENCRYPTION failed,"
                            "ret=%d!\r\n", 
                            i4_ret));
           return SMR_DRV_COMP_ERROR;
        }
        else
        {
            pt_rs_obj->b_en_encryption = FALSE;
            SM_RSH_DBG_INFO(("sm_rcrd_strm_init_pvr:set stream(pid=%u)"
                             "PVR_SET_TYPE_ENCRYPTION OK!!!\r\n",
                             pt_rs_obj->t_pid));
        }
        
    }
    
    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_pvr_close
 *
 * Description: This function closes a PVR stream component
 *
 * Inputs:  
 *       h_stream:  stream handle
 *
 * Outputs: 
 *       pt_rs_obj: close a pvr stream component
 *
 * Returns: 
 *       SMR_OK: Succeed
 *       Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-14 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_pvr_close(HANDLE_T          h_stream, 
                             SM_RCRD_STREAM_T* pt_rs_obj)
{
    INT32                    i4_ret;
    /*SM_DRV_COMP_DESCR_T*     pt_comp_descr; */
    RM_COND_T                e_cond;
    SM_RCRD_STRM_HDLR_MSG_T  t_msg;
    BOOL                     b_pvr_state;
    SIZE_T                   t_size = sizeof(BOOL);
    if ((NULL_HANDLE == h_stream) || (NULL == pt_rs_obj))
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_close:NULL argument!\r\n"));
        return SMR_INV_ARG;
    }

    i4_ret = RMR_OK;

    if ((pt_rs_obj->t_pvr_descr.h_comp != NULL_HANDLE)
         && (pt_rs_obj->t_pvr_descr.e_comp_cond != RM_COND_CLOSED))
    {
        /*Should stop pvr if  come here from error state*/
        i4_ret = rm_get(pt_rs_obj->t_pvr_descr.h_comp,
                        FALSE,
                        ANY_PORT_NUM,
                        PVR_GET_TYPE_CTRL,
                        (VOID*)&b_pvr_state,
                        &t_size,
                        &(pt_rs_obj->t_pvr_descr.e_comp_cond));
        if (i4_ret != RMR_OK)
        {
            SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_close:get pvr state fail=%d!\r\n", i4_ret));
        }
        if (b_pvr_state != FALSE)
        {
            b_pvr_state = FALSE;
            i4_ret = rm_set(pt_rs_obj->t_pvr_descr.h_comp,
                            FALSE,
                            ANY_PORT_NUM,
                            PVR_SET_TYPE_CTRL,
                            (VOID*)(UINT32)b_pvr_state,
                            (SIZE_T)(sizeof(BOOL)),
                            &(pt_rs_obj->t_pvr_descr.e_comp_cond));
            if (i4_ret != RMR_OK)
            {
                SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_close:set pvr state fail=%d!\r\n", i4_ret));
            }
        }

        /*close PVR*/
        i4_ret = sm_drv_comp_close(&pt_rs_obj->t_pvr_descr);
        if (i4_ret != RMR_OK)
        {
            /**/
        }
    }
    else
    {
        /* Pretend closed, if handle is not existed */
        sm_rs_send_msg_do_event(SM_EVN_RSH_PVR_CLOSED,
                                h_stream,
                                0);
    }

    /*Filter out object state*/
    e_cond = sm_rm_cond_filter(pt_rs_obj->t_pvr_descr.e_comp_cond,
                               COND_GRP_OBJ_STATE);
    if (RM_COND_CLOSED == e_cond)
    {
        t_msg.e_msg_type = SM_RCRD_STRM_MSG_TYPE_PVR;
        t_msg.h_stream = h_stream;
        
        t_msg.u.t_pvr_msg.h_comp = pt_rs_obj->t_pvr_descr.h_comp;
        t_msg.u.t_pvr_msg.ui1_port = pt_rs_obj->t_pvr_descr.t_info.ui1_in_port;
        t_msg.u.t_pvr_msg.e_nfy_cond = pt_rs_obj->t_pvr_descr.e_comp_cond;
        t_msg.u.t_pvr_msg.ui4_data = 0;
        t_msg.u.t_pvr_msg.b_should_update_comp_descr = FALSE;
        
        sm_rs_send_msg(&t_msg);
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_pvr_connect
 *
 * Description: This function connects a PVR stream component to record demux
 *
 * Inputs:  
 *       h_stream:  stream handle
 *
 * Outputs: 
 *       PVR stram connects to record demux successfully
 *
 * Returns: 
 *       SMR_OK: Succeed
 *       Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-14 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_pvr_connect(HANDLE_T          h_stream, 
                               SM_RCRD_STREAM_T* pt_rs_obj)
{
    INT32                    i4_ret;
    RM_COND_T                e_cond;
    SM_RCRD_STRM_HDLR_MSG_T  t_msg;
    HANDLE_T                 h_rec_dmx = NULL_HANDLE;
    
    if (NULL == pt_rs_obj)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_connect:NULL argument!\r\n"));
        return SMR_INV_ARG;
    }

    i4_ret = SMR_OK;
    
    i4_ret = pm_get_component_info( pt_rs_obj->t_base.h_pm_comp,
                                    &h_rec_dmx );

    if (i4_ret != PMR_OK)
    {
        SM_RSH_DBG_ERR(("pm_get_component_info:fail(%d) !\r\n", i4_ret));
        return i4_ret;
    }

    i4_ret = sm_drv_comp_connect(&pt_rs_obj->t_pvr_descr,
                                 h_rec_dmx,
                                 (VOID*)h_stream);
    if (i4_ret != SMR_OK)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_connect:fail(%d) to connect!\r\n", i4_ret));
        return i4_ret;
    }
    
    e_cond = sm_rm_cond_filter(pt_rs_obj->t_pvr_descr.e_comp_cond,
                               COND_GRP_CONN_STATE);
    if (RM_COND_CONNECTED == e_cond)
    {
        t_msg.e_msg_type = SM_RCRD_STRM_MSG_TYPE_PVR;
        t_msg.h_stream= h_stream;
        
        t_msg.u.t_pvr_msg.h_comp = pt_rs_obj->t_pvr_descr.h_comp;
        t_msg.u.t_pvr_msg.ui1_port = pt_rs_obj->t_pvr_descr.t_info.ui1_in_port;
        t_msg.u.t_pvr_msg.e_nfy_cond = pt_rs_obj->t_pvr_descr.e_comp_cond;
        t_msg.u.t_pvr_msg.ui4_data = 0;
        t_msg.u.t_pvr_msg.b_should_update_comp_descr = FALSE;
        
        sm_rs_send_msg(&t_msg);
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_pvr_disconnect
 *
 * Description: This function disconnects a PVR stream component with record demux
 *
 * Inputs:  
 *       h_stream:  stream handle
 *
 * Outputs: 
 *       PVR stram disconnects to record demux successfully
 *
 * Returns: 
 *       SMR_OK: Succeed
 *       Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-14 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_pvr_disconnect(HANDLE_T          h_stream, 
                                  SM_RCRD_STREAM_T* pt_rs_obj)
{
    INT32                    i4_ret;
    RM_COND_T                e_cond;
    SM_RCRD_STRM_HDLR_MSG_T  t_msg;

    if (NULL == pt_rs_obj)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_disconnect:NULL argument!\r\n"));
        return SMR_INV_ARG;
    }

    i4_ret = RMR_OK;

    if ((pt_rs_obj->t_pvr_descr.h_comp != NULL_HANDLE)
         && (pt_rs_obj->t_pvr_descr.e_comp_cond != RM_COND_CLOSED))
    {
        /* Disable PVR with  record demux*/
        i4_ret = sm_rcrd_strm_pvr_set(pt_rs_obj->t_base.h_stream, 
                                      PVR_SET_TYPE_CTRL, 
                                      (VOID*)(FALSE), 
                                      sizeof(BOOL));
        if (i4_ret != RMR_OK)
        {
            SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_disconnect:disable PVR err=%d!\r\n", 
                            i4_ret));
           return SMR_DRV_COMP_ERROR;
        }
        
        e_cond = sm_rm_cond_filter(pt_rs_obj->t_pvr_descr.e_comp_cond,
                                   COND_GRP_CONN_STATE);
        if (RM_COND_CONNECTED == e_cond)
        {
            /* Disconnect PVR with record demux*/
            i4_ret = sm_drv_comp_disconnect(&pt_rs_obj->t_pvr_descr);
            if (i4_ret != SMR_OK)
            {
                SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_disconnect:PVR fail=%d\r\n",
                                i4_ret));
                return i4_ret;
            }
        }
        else if (RM_COND_DISCONNECTED == e_cond)
        {
            SM_RSH_DBG_INFO(("sm_rcrd_strm_pvr_disconnect:PVR already DISCONNECT\r\n"));
        }
        else
        {
            /*What to do*/
        }
    }
    else
    {
        /* Pretend disconnected, if handle is not existed */
        SM_RSH_DBG_INFO(("sm_rcrd_strm_pvr_disconnect:pretend PVR already DISCONNECT\r\n"));
        sm_rs_send_msg_do_event(SM_EVN_RSH_PVR_DISCONNECTED,
                                h_stream,
                                0);
    }

    /*If disconnected send msg*/
    e_cond = sm_rm_cond_filter(pt_rs_obj->t_pvr_descr.e_comp_cond,
                               COND_GRP_CONN_STATE);
    if (RM_COND_DISCONNECTED == e_cond)
    {
        t_msg.e_msg_type = SM_RCRD_STRM_MSG_TYPE_PVR;
        t_msg.h_stream = h_stream;
        
        t_msg.u.t_pvr_msg.h_comp = pt_rs_obj->t_pvr_descr.h_comp;
        t_msg.u.t_pvr_msg.ui1_port = pt_rs_obj->t_pvr_descr.t_info.ui1_in_port;
        t_msg.u.t_pvr_msg.e_nfy_cond = pt_rs_obj->t_pvr_descr.e_comp_cond;
        t_msg.u.t_pvr_msg.ui4_data = 0;
        t_msg.u.t_pvr_msg.b_should_update_comp_descr = FALSE;
        
        sm_rs_send_msg(&t_msg);
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_pvr_set
 *
 * Description: This function sets PVR stream component
 *
 * Inputs:  
 *       h_stream:       stream handle
 *       e_set_type:     set command
 *       pv_set_info:    set info
 *       z_set_info_len: set info lenght
 *
 * Outputs: NULL
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-14 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_pvr_set(HANDLE_T       h_stream,
                           DRV_SET_TYPE_T e_set_type,
                           VOID*          pv_set_info,
                           SIZE_T         z_set_info_len)
{
    INT32                 i4_ret;
    SM_RCRD_STREAM_T*     pt_rs_obj;
    #if 0
    BOOL                  b_is_enbale;       
    SIZE_T                t_len;
    #endif

    pt_rs_obj = (SM_RCRD_STREAM_T*)sm_get_strm_obj(h_stream);
    if (NULL == pt_rs_obj)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_set:NULL argument!\r\n"));
        return SMR_INV_HANDLE;
    }

    
    if ((pt_rs_obj->t_pvr_descr.h_comp != NULL_HANDLE)
         && (pt_rs_obj->t_pvr_descr.e_comp_cond != RM_COND_CLOSED))
    {
        i4_ret = rm_set(pt_rs_obj->t_pvr_descr.h_comp,
                        FALSE,
                        ANY_PORT_NUM,
                        e_set_type,
                        pv_set_info,
                        z_set_info_len,
                        &(pt_rs_obj->t_pvr_descr.e_comp_cond));
        if (i4_ret != RMR_OK)
        {
            SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_set:set fail=%d\r\n",
                            i4_ret));
            return SMR_DRV_COMP_ERROR;
        }
    }
    else
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_set:NULL handle!\r\n"));
        return SMR_INV_HANDLE;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_pvr_get
 *
 * Description: This function get a PVR stream component info
 *
 * Inputs:  
 *       h_stream:       stream handle
 *       e_get_type:     get command
 *       pv_get_info:    get info buffer
 *       z_get_info_len: get info lenght
 *
 * Outputs: 
 *       pv_get_info:    get info buffer
 *       z_get_info_len: get info lenght
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-14 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_pvr_get(HANDLE_T       h_stream,
                           DRV_GET_TYPE_T e_get_type,
                           VOID*          pv_get_info,
                           SIZE_T*        pz_get_info_len)
{
    INT32                 i4_ret;
    SM_RCRD_STREAM_T*     pt_rs_obj;

    pt_rs_obj = (SM_RCRD_STREAM_T*)sm_get_strm_obj(h_stream);
    if (NULL == pt_rs_obj)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_get:NULL argument!\r\n"));
        return SMR_INV_HANDLE;
    }

    #if 0
    switch (e_get_type)
    {
        case PVR_GET_TYPE_CTRL:
            
            break;
        case PVR_GET_TYPE_BUFFER_STATUS:
            break;
        case PVR_GET_TYPE_BUFFER_INFO:
            break;
        case PVR_GET_TYPE_RESIDUAL_DATA:
            break;
            
        default:
            SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_get:err command=%d!\r\n", e_get_type));
            return SMR_INTERNAL_ERROR;
            
    }
    #endif
    
    if ((pt_rs_obj->t_pvr_descr.h_comp != NULL_HANDLE)
         && (pt_rs_obj->t_pvr_descr.e_comp_cond != RM_COND_CLOSED))
    {
        i4_ret = rm_get(pt_rs_obj->t_pvr_descr.h_comp,
                        FALSE,
                        ANY_PORT_NUM,
                        e_get_type,
                        pv_get_info,
                        pz_get_info_len,
                        &(pt_rs_obj->t_pvr_descr.e_comp_cond));
        if (i4_ret != RMR_OK)
        {
            SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_get:get fail=%d!\r\n", i4_ret));
            return SMR_DRV_COMP_ERROR;
        }
    }
    else
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_get:NULL handle!\r\n"));
        return SMR_INV_HANDLE;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_pvr_set_null_pacekt
 *
 * Description: This function sets null packet 
 *
 * Inputs:  
 *       h_stream:       stream handle
 *
 * Outputs: NULL
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-04-19 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_pvr_set_null_pacekt(HANDLE_T          h_stream, 
                                       SM_RCRD_STREAM_T* pt_rs_obj)
{
    INT32                i4_ret;
    PVR_SWAP_INFO_T      t_pat_info;

    x_memset(&t_pat_info, 0, sizeof(PVR_SWAP_INFO_T));
    t_pat_info.e_swap_type = PVR_SWAP_TYPE_NULL_PKT;
    i4_ret =  sm_rcrd_strm_pvr_set(h_stream,
                                   PVR_SET_TYPE_SWAP,
                                   (VOID*)&t_pat_info,
                                   (SIZE_T)sizeof(PVR_SWAP_INFO_T));
    if (SMR_OK == i4_ret)
    {
        if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
             && (RSH_TBL_GRP_PSI_PAT == pt_rs_obj->t_sect_info.e_tbl_grp))
        {
            sm_rs_send_msg_do_event(SM_EVN_RSH_TM_PAT_SWAPED,
                                    pt_rs_obj->t_base.h_stream,
                                    0);
            SM_RSH_DBG_INFO(("sm_rcrd_strm_pvr_set_null_pacekt:NULL PAT!\r\n"));
        }

        if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
             && (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp))
        {
            sm_rs_send_msg_do_event(SM_EVN_RSH_TM_PMT_SWAPED,
                                    pt_rs_obj->t_base.h_stream,
                                    0);
            SM_RSH_DBG_INFO(("sm_rcrd_strm_pvr_set_null_pacekt:NULL PMT!\r\n"));
        }
    
        
    }
    else
    {
        if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
             && (RSH_TBL_GRP_PSI_PAT == pt_rs_obj->t_sect_info.e_tbl_grp))
        {
            SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_set_null_pacekt:err NULL PAT!\r\n"));
        }

        if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
             && (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp))
        {
            SM_RSH_DBG_ERR(("sm_rcrd_strm_pvr_set_null_pacekt:err NULL PMT!\r\n"));
        }
    }
    
    return i4_ret;
}

