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

/*#include "strm_mngr/sm_slctr.h"
#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/sm_drv_comp.h"
#include "strm_mngr/sm_drv_map.h"*/
#include "strm_mngr/sess_rcrd_hdlr/_sm_sess_rcrd_pvr.h"

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
 * Name:  _sm_sess_rcrd_pvr_rm_nfy_fct
 *
 * Description: This function is PVR CTRL RM notify function
 *
 * Inputs:  
 *       h_obj:      PVR CTRL component handle
 *       ui1_port:   Input port
 *       e_nfy_cond: PVR condition
 *       pv_tag:     notify tag
 *       ui4_data:   additional info e.g. reason info
 *
 * Outputs: 
 *       Send message to session to update PVR condition in SM
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-10 : initial
 ------------------------------------------------------------------------------*/
static VOID _sm_sess_rcrd_pvr_rm_nfy_fct(HANDLE_T   h_obj,
                                         UINT8      ui1_port,
                                         RM_COND_T  e_nfy_cond,
                                         VOID*      pv_tag,
                                         UINT32     ui4_data)
{
    SM_SESS_RCRD_MSG_T     t_msg;
    
    if (NULL == pv_tag)
    {
        return;
    }
    
    t_msg.e_msg_type = SM_SESS_RCRD_MSG_TYPE_PVR;
    t_msg.h_sess = (HANDLE_T)pv_tag;
    
    t_msg.u.t_pvr_msg.h_comp = h_obj;
    t_msg.u.t_pvr_msg.ui1_port = ui1_port;
    t_msg.u.t_pvr_msg.e_nfy_cond = e_nfy_cond;
    t_msg.u.t_pvr_msg.ui4_data = ui4_data;
    t_msg.u.t_pvr_msg.b_should_update_comp_descr = TRUE;
    
    sm_rcrd_sess_send_msg(&t_msg);
}

/*-----------------------------------------------------------------------------
 * Name:  _rm_comp_sel_fct_pvr_ctrl
 *
 * Description: This function  is a RM select callback when open a component
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
 *       b_would_conflict: If conflict
 *
 * Outputs: 
 *       Send message to session to update PVR condition in SM
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-10 : initial
 ------------------------------------------------------------------------------*/
static SELECT_T _rm_comp_sel_fct_pvr_ctrl(DRV_TYPE_T   e_type,
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
    
    t_drv_descr.e_type         = e_type;
    t_drv_descr.ui2_comp_id    = ui2_id;
    x_memset(t_drv_descr.s_logical_name, 0, SM_DRV_LOGICAL_NAME_LEN);

    if (sm_drv_map_exists(&t_drv_descr) || (TRUE == b_would_conflict))
    {
        return SEL_NEXT;
    }
    
    return SEL_COMP_BREAK;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_pvr_open
 *
 * Description: This function opens a PVR CTRL component for recording
 *
 * Inputs:  
 *       h_sess:      session handle
 *       pt_sess_obj: session object
 *
 * Outputs: 
 *       pt_sess_obj: open a pvr ctrl component
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-10 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_pvr_open(HANDLE_T        h_sess, 
                            SM_SESS_RCRD_T* pt_sess_obj)
{
    INT32                  i4_ret;
    RM_COND_T              e_cond;
    SM_SESS_RCRD_MSG_T     t_msg;
    SM_DRV_COMP_DESCR_T*   pt_comp_descr; 

    if (NULL == pt_sess_obj)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_open:arg is null!\r\n"));
        return SMR_INV_ARG;
    }

    pt_comp_descr = &(pt_sess_obj->t_pvr_descr);

    /*get logical name*/
    x_strcpy(pt_comp_descr->t_info.s_logical_name, 
             pt_sess_obj->t_base.s_dest_grp_name);
    
    pt_sess_obj->t_pvr_descr.t_info.e_type = DRVT_PVR_CTRL;

    

    i4_ret = sm_drv_comp_open_ex(NULL_HANDLE,
                                 pt_comp_descr->t_info.e_type,
                                 pt_comp_descr->t_info.ui2_comp_id,
                                 NULL, //pt_comp_descr->t_info.s_logical_name,
                                 (VOID*)pt_comp_descr,
                                 &_rm_comp_sel_fct_pvr_ctrl,
                                 (VOID*)h_sess,
                                 &_sm_sess_rcrd_pvr_rm_nfy_fct,
                                 pt_comp_descr);
    if (i4_ret != SMR_OK)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_open:sm_drv_comp_open_ex err=%d!\r\n",
                        i4_ret));
        return i4_ret;   
    }

    /*Filter out object state*/
    e_cond = sm_rm_cond_filter(pt_comp_descr->e_comp_cond,
                               COND_GRP_OBJ_STATE);
    if (RM_COND_OPENED == e_cond)
    {
        t_msg.e_msg_type = SM_SESS_RCRD_MSG_TYPE_PVR;
        t_msg.h_sess = h_sess;
        
        t_msg.u.t_pvr_msg.h_comp = pt_comp_descr->h_comp;
        t_msg.u.t_pvr_msg.ui1_port = pt_comp_descr->t_info.ui1_in_port;
        t_msg.u.t_pvr_msg.e_nfy_cond = pt_comp_descr->e_comp_cond;
        t_msg.u.t_pvr_msg.ui4_data = 0;
        t_msg.u.t_pvr_msg.b_should_update_comp_descr = FALSE;
        
        sm_rcrd_sess_send_msg(&t_msg);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_pvr_close
 *
 * Description: This function closes a PVR CTRL component 
 *
 * Inputs:  
 *       h_sess:      session handle
 *       pt_sess_obj: session object
 *
 * Outputs: 
 *       pt_sess_obj: close a pvr ctrl component
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-10 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_pvr_close(HANDLE_T        h_sess, 
                             SM_SESS_RCRD_T* pt_sess_obj)
{
    INT32                  i4_ret = SMR_OK;
    RM_COND_T              e_cond;
    SM_SESS_RCRD_MSG_T     t_msg;
    BOOL                   b_pvr_ctrl;
    SIZE_T                 t_size = sizeof(BOOL);

    if ((pt_sess_obj->t_pvr_descr.h_comp != NULL_HANDLE)
         && (pt_sess_obj->t_pvr_descr.e_comp_cond != RM_COND_CLOSED))
    {
        /*should stop PVR if come here from error state, first get then set*/
        i4_ret = rm_get(pt_sess_obj->t_pvr_descr.h_comp,
                        FALSE,
                        ANY_PORT_NUM,
                        PVR_GET_TYPE_CTRL,
                        (VOID*)&b_pvr_ctrl,
                        &t_size,
                        &(pt_sess_obj->t_pvr_descr.e_comp_cond));
        if (i4_ret != RMR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_close:rm get pvr ctrl state err=%!\r\n", i4_ret));
        }

        if (b_pvr_ctrl != FALSE)
        {
            b_pvr_ctrl = FALSE;
            i4_ret = rm_set(pt_sess_obj->t_pvr_descr.h_comp,
                            FALSE,
                            ANY_PORT_NUM,
                            PVR_SET_TYPE_CTRL,
                            (VOID*)(UINT32)b_pvr_ctrl,
                            (SIZE_T)(sizeof(BOOL)),
                            &(pt_sess_obj->t_pvr_descr.e_comp_cond));
            if (i4_ret != RMR_OK)
            {
                SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_close:rm set pvr ctrl err=%d!\r\n", i4_ret));
            }
        }

        /*close PVR*/
        i4_ret = sm_sess_rcrd_pvr_ctrl_close_deinit(pt_sess_obj);
        if (i4_ret != RMR_OK)
        {
        
            SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_close:deinit err=%d!\r\n",
                            i4_ret));
        }
        
        i4_ret = sm_drv_comp_close(&(pt_sess_obj->t_pvr_descr));
        if (i4_ret != RMR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_close:close PVR Ctrl err=%d!\r\n",
                            i4_ret));
        }

	    /*Filter out object state*/
	    e_cond = sm_rm_cond_filter(pt_sess_obj->t_pvr_descr.e_comp_cond,
	                               COND_GRP_OBJ_STATE);
	    if (RM_COND_CLOSED == e_cond)
	    {
	        
	        t_msg.e_msg_type = SM_SESS_RCRD_MSG_TYPE_PVR;
	        t_msg.h_sess = h_sess;
	        
	        t_msg.u.t_pvr_msg.h_comp     = pt_sess_obj->t_pvr_descr.h_comp;
	        t_msg.u.t_pvr_msg.ui1_port   = pt_sess_obj->t_pvr_descr.t_info.ui1_in_port;
	        t_msg.u.t_pvr_msg.e_nfy_cond = pt_sess_obj->t_pvr_descr.e_comp_cond;
	        t_msg.u.t_pvr_msg.ui4_data   = 0;
	        t_msg.u.t_pvr_msg.b_should_update_comp_descr = FALSE;
	        
	        sm_rcrd_sess_send_msg(&t_msg);
	    }
    }
    else
    {
        /* Pretend closed, if handle is not existed */
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PVR_CLOSED, h_sess, 0);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_pvr_set
 *
 * Description: This function set PVR ctrl component
 *
 * Inputs:  
 *       h_stream:       session handle
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
INT32 sm_sess_rcrd_pvr_set(HANDLE_T       h_sess,
                           DRV_SET_TYPE_T e_set_type,
                           VOID*          pv_set_info,
                           SIZE_T         z_set_info_len)
{
    INT32                 i4_ret;
    SM_SESS_RCRD_T*       pt_sess_obj;

    pt_sess_obj = (SM_SESS_RCRD_T*)sm_get_sess_obj(h_sess);
    
    if (NULL == pt_sess_obj)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_set:args are null!\r\n"));
        return SMR_INV_HANDLE;
    }
    
    
    if ((pt_sess_obj->t_pvr_descr.h_comp != NULL_HANDLE)
         && (pt_sess_obj->t_pvr_descr.e_comp_cond != RM_COND_CLOSED))
    {
        i4_ret = rm_set(pt_sess_obj->t_pvr_descr.h_comp,
                        FALSE,
                        ANY_PORT_NUM,
                        e_set_type,
                        pv_set_info,
                        z_set_info_len,
                        &(pt_sess_obj->t_pvr_descr.e_comp_cond));
        if (i4_ret != RMR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_set:rm set type=%d err=%d!\r\n",
                            e_set_type,
                            i4_ret));
            return SMR_DRV_COMP_ERROR;
        }
    }
    else
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_get:comp hdlr is invalid!\r\n"));
        return SMR_INV_HANDLE;
    }

    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_pvr_get
 *
 * Description: This function gets a PVR ctrl component info
 *
 * Inputs:  
 *       h_sess:          session handle
 *       e_get_type:      get command
 *       pv_get_info:     get info buffer
 *       pz_get_info_len: get info lenght
 *
 * Outputs: 
 *       pv_get_info:    get info buffer
 *       pz_get_info_len: get info lenght
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-14 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_pvr_get(HANDLE_T       h_sess,
                           DRV_GET_TYPE_T e_get_type,
                           VOID*          pv_get_info,
                           SIZE_T*        pz_get_info_len)
{
    INT32                 i4_ret;
    SM_SESS_RCRD_T*       pt_sess_obj;

    pt_sess_obj = (SM_SESS_RCRD_T*)sm_get_sess_obj(h_sess);
    
    if (NULL == pt_sess_obj)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_get:args are null!\r\n"));
        return SMR_INV_HANDLE;
    }
    
    if ((pt_sess_obj->t_pvr_descr.h_comp != NULL_HANDLE)
         && (pt_sess_obj->t_pvr_descr.e_comp_cond != RM_COND_CLOSED))
    {
        i4_ret = rm_get(pt_sess_obj->t_pvr_descr.h_comp,
                        FALSE,
                        ANY_PORT_NUM,
                        e_get_type,
                        pv_get_info,
                        pz_get_info_len,
                        &(pt_sess_obj->t_pvr_descr.e_comp_cond));
        if (i4_ret != RMR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_get:rm get type=%d err=%!\r\n",
                            e_get_type,
                            i4_ret));
            return SMR_DRV_COMP_ERROR;
        }
    }
    else
    {
        return SMR_INV_HANDLE;
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_pvr_ctrl
 *
 * Description: This function enables/disables PVR CTRL component
 *
 * Inputs:  
 *       h_sess:     session handle
 *       b_pvr_ctrl: enable(true) or disable(false)
 *
 * Outputs: NULL
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-10 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_pvr_ctrl(SM_SESS_RCRD_T* pt_sess_obj,
                            BOOL            b_pvr_ctrl)
{
    INT32               i4_ret;
    
    if ((pt_sess_obj->t_pvr_descr.h_comp != NULL_HANDLE)
         && (pt_sess_obj->t_pvr_descr.e_comp_cond != RM_COND_CLOSED))
    {
        i4_ret = rm_set(pt_sess_obj->t_pvr_descr.h_comp,
                        FALSE,
                        ANY_PORT_NUM,
                        PVR_SET_TYPE_CTRL,
                        (VOID*)(UINT32)b_pvr_ctrl,
                        sizeof(BOOL),
                        &(pt_sess_obj->t_pvr_descr.e_comp_cond));
        if (i4_ret != RMR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl:rm set err=%!\r\n", i4_ret));
            return SMR_DRV_COMP_ERROR;
        }
    }

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_sess_rcrd_pvr_nfy_fct
 *
 * Description: This function is PVR CTRL notify
 *
 * Inputs:  
 *       pv_tag:  notify tag
 *       e_nfy_cond: PVR notify code 
 *       ui4_data:  additional data
 *
 * Outputs: NULL
 *
 * Returns: 
 *        TRUE:  ??
 *        FALSE  ??Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-11 : initial
 ------------------------------------------------------------------------------*/

static BOOL _sm_sess_rcrd_pvr_nfy_fct (VOID*             pv_tag,
                                       PVR_NFY_COND_T    e_nfy_cond,
                                       UINT32            ui4_data)
{
    /*INT32                   i4_ret;*/
    SM_SESS_RCRD_MSG_T     t_msg;
    
#ifdef DEBUG
    PVR_TICK_DATA_T*       pt_tick_data;
    static UINT32          ui4_tick_num = 0;
    static BOOL            b_tick = FALSE;
    static BOOL            b_msg = FALSE;
    if (b_tick)
    {
        SM_RSS_DBG_INFO(("PRV Tick[%u]: pv_tag=%u; e_nfy_cond=%d, ui4_data=%u\r\n", 
                         ui4_tick_num, (UINT32)pv_tag, e_nfy_cond, ui4_data));
    }
#endif
    if (NULL == pv_tag)
    {
        return FALSE;
    }
    x_memset(&t_msg, 0, sizeof(SM_SESS_RCRD_MSG_T));
    t_msg.e_msg_type = SM_SESS_RCRD_MSG_TYPE_PVR_CTRL_NFY;
    t_msg.h_sess = (HANDLE_T)pv_tag;
    
    t_msg.u.t_pvr_tick_msg.e_nfy_cond = e_nfy_cond;
    if ((PVR_COND_TICK == e_nfy_cond) && (NULL != (PVR_TICK_DATA_T*)ui4_data))
    {
    
        #ifdef DEBUG
        if (b_tick)
        {
            pt_tick_data = (PVR_TICK_DATA_T*)ui4_data;
            SM_RSS_DBG_INFO(("PVR Tick Data[%u]:pv_metadata_tag=0X%08x;  "
                             " pt_metadata=0X%08x;  "
                             " pui1_data=0X%08x;  "
                             " ui4_pkt_count=%u\r\n", 
                             ui4_tick_num++, 
                             (UINT32)(pt_tick_data->pv_metadata_tag),
                             (UINT32)(pt_tick_data->pt_metadata),
                             (UINT32)(pt_tick_data->pui1_data),
                             pt_tick_data->ui4_pkt_count));
        }

        #endif
        x_memcpy((VOID*)&(t_msg.u.t_pvr_tick_msg.t_tick_data), 
                 (VOID*)ui4_data, 
                 sizeof(PVR_TICK_DATA_T));
        #ifdef DEBUG
        if (b_msg)
        {
            pt_tick_data = &(t_msg.u.t_pvr_tick_msg.t_tick_data);
            SM_RSS_DBG_INFO(("MSG Tick Data[%u]:pv_metadata_tag=0X%08x;  "
                             " pt_metadata=0X%08x;  "
                             " pui1_data=0X%08x;  "
                             " ui4_pkt_count=%u\r\n", 
                             (ui4_tick_num - 1), 
                             (UINT32)(pt_tick_data->pv_metadata_tag),
                             (UINT32)(pt_tick_data->pt_metadata),
                             (UINT32)(pt_tick_data->pui1_data),
                             pt_tick_data->ui4_pkt_count));
        }

        #endif
    }
	else 
	{
		t_msg.u.t_pvr_tick_msg.ui4_data = ui4_data;
	}
     
    sm_rcrd_sess_send_msg(&t_msg);

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_pvr_ctrl_open_init
 *
 * Description: This function sets PVR CTRL info when open a PVR Ctrl
 *              Init PVR Ctrl, Including:
 *              PVR_SET_TYPE_BUFFER_ALIGN,  PVR_SET_TYPE_TICK_PERIOD
 *              PVR_SET_TYPE_NFY_FRAME_TYPES, PVR_SET_TYPE_NFY_FCT
 *
 * Inputs:  
 *       pt_sess_obj:  session object pointer
 *
 * Outputs: NULL
 *
 * Returns: 
 *        SMR_OK:  Succeed
 *        Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-11 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_pvr_ctrl_open_init(SM_SESS_RCRD_T* pt_sess_obj)
{
    INT32                      i4_ret;
    PVR_NFY_INFO_T             t_pvr_nfy_info;
	SIZE_T                     t_size;
	PVR_CAP_INFO_T             t_pvr_cap_info;
	PVR_CTRL_CONFIG_T          t_ctrl_init_cfg;
	REC_UTIL_PVR_BUF_CNST_T    t_buf_cnst;

    if (NULL == pt_sess_obj)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl_open_init:arg is null!\r\n"));
        return SMR_INV_ARG;
    }

	if (pt_sess_obj->b_pvr_init)
	{
        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl_open_init:PVR init done!\r\n"));
        return SMR_OK;
    }
    
    do
    {
        /*1. Get PVR cap info*/
        t_size = sizeof(PVR_CAP_INFO_T);
        i4_ret = sm_sess_rcrd_pvr_get(pt_sess_obj->t_base.h_sess,
                                      PVR_GET_TYPE_CAP_INFO,
                                      (VOID*)&t_pvr_cap_info,
                                      &t_size);
        if (i4_ret != REC_UTILR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl_open_init:get pvr cap info failed(%d)!\r\n", 
                            i4_ret));
            break;
        }
        
        /*2. allocate PVR buffer*/
        t_buf_cnst.ui4_align_buf_start = t_pvr_cap_info.ui4_align_buf_start;
        t_buf_cnst.ui4_align_buf_size  = t_pvr_cap_info.ui4_align_buf_size;
        i4_ret = rec_util_alloc_pvr_buf(pt_sess_obj->t_rutil_info.h_record_util,
                                        &t_buf_cnst,
                                        &(pt_sess_obj->t_rutil_info.t_buf_info));
        if (i4_ret != REC_UTILR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl_open_init:alloc pvr buffer failed(%d)!\r\n",
                            i4_ret));
            break;
        }
        
        /*3. Set init*/
        pt_sess_obj->t_rutil_info.pv_buf_mw = x_mmap_share_mem (pt_sess_obj->t_rutil_info.t_buf_info.pv_buf,
                                                                pt_sess_obj->t_rutil_info.t_buf_info.z_size);
        if (pt_sess_obj->t_rutil_info.pv_buf_mw == NULL)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl_open_init:mmap pvr buffer failed(%d)!\r\n",
                            i4_ret));
            break;
        }
        
        t_ctrl_init_cfg.ui4_tick_period  = pt_sess_obj->t_sess_cfg.ui4_ms_tick_period;
        t_ctrl_init_cfg.ui1_nfy_frm_mask = pt_sess_obj->t_sess_cfg.ui1_frame_mask;
        t_ctrl_init_cfg.pv_buf_phy       = pt_sess_obj->t_rutil_info.t_buf_info.pv_buf;
        t_ctrl_init_cfg.pv_buf_mw        = pt_sess_obj->t_rutil_info.pv_buf_mw;
        t_ctrl_init_cfg.z_buf_size         = pt_sess_obj->t_rutil_info.t_buf_info.z_size;
        
        i4_ret = sm_sess_rcrd_pvr_set(pt_sess_obj->t_base.h_sess,
                                      PVR_SET_TYPE_INIT_CTRL,
                                      (VOID*)&t_ctrl_init_cfg,
                                       sizeof(PVR_CTRL_CONFIG_T));
        if (i4_ret != RMR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl_open_init:set PVR_SET_TYPE_INIT_CTRL err=%d!\r\n",
                            i4_ret));
            break;
        }

	    /*4. Set notify*/
	    t_pvr_nfy_info.pf_nfy = _sm_sess_rcrd_pvr_nfy_fct;
	    t_pvr_nfy_info.pv_tag = (VOID*)(pt_sess_obj->t_base.h_sess);
	        
	    i4_ret = sm_sess_rcrd_pvr_set(pt_sess_obj->t_base.h_sess,
	                                  PVR_SET_TYPE_NFY_FCT,
	                                  (VOID*)(&t_pvr_nfy_info),
	                                  sizeof(PVR_NFY_INFO_T));
	    if (i4_ret != RMR_OK)
	    {
	        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl_open_init:set NFY_FCT err=%d!\r\n",
	                        i4_ret));
			break;
	    }

		pt_sess_obj->b_pvr_init = TRUE;

	    return SMR_OK;
		
	} while(0);
	
	if (i4_ret != SMR_OK)
	{
	    if (pt_sess_obj->t_rutil_info.t_buf_info.pv_buf != NULL)
    	{
			i4_ret = rec_util_free_pvr_buf(pt_sess_obj->t_rutil_info.h_record_util,
										   &(pt_sess_obj->t_rutil_info.t_buf_info));
			if (i4_ret != REC_UTILR_OK)
			{
				SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl_open_init:free pvr buffer failed(%d)!\r\n",
								i4_ret));
			}
			x_memset(&(pt_sess_obj->t_rutil_info.t_buf_info), 
				     0 ,
				     sizeof(REC_UTIL_PBA_BUF_INFO_T));
    	}
		
		sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PVR_ERR,
									   pt_sess_obj->t_base.h_sess,
									   0);
		
	}
	return i4_ret;
	
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_pvr_ctrl_close_deinit
 *
 * Description: This function sets PVR CTRL info when open a PVR Ctrl
 *              Init PVR Ctrl, Including:
 *              PVR_SET_TYPE_BUFFER_ALIGN,  PVR_SET_TYPE_TICK_PERIOD
 *              PVR_SET_TYPE_NFY_FRAME_TYPES, PVR_SET_TYPE_NFY_FCT
 *
 * Inputs:  
 *       pt_sess_obj:  session object pointer
 *
 * Outputs: NULL
 *
 * Returns: 
 *        SMR_OK:  Succeed
 *        Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2010-02-03 : deinitial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_pvr_ctrl_close_deinit(SM_SESS_RCRD_T* pt_sess_obj)
{
    INT32       i4_ret = SMR_OK;

    if (NULL == pt_sess_obj)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl_close_deinit:arg is null!\r\n"));
        return SMR_INV_ARG;
    }
    
    /* mem unmap the buffer */    
    if (pt_sess_obj->t_rutil_info.pv_buf_mw != NULL)
    {
        x_munmap_share_mem (pt_sess_obj->t_rutil_info.pv_buf_mw,
                            pt_sess_obj->t_rutil_info.t_buf_info.z_size);
    }
    /*free ring buffer*/
    if (pt_sess_obj->t_rutil_info.t_buf_info.pv_buf != NULL)
    {
        i4_ret = rec_util_free_pvr_buf(pt_sess_obj->t_rutil_info.h_record_util,
                                       &(pt_sess_obj->t_rutil_info.t_buf_info));
        if (i4_ret != REC_UTILR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl_close_deinit:free pvr buffer failed(%d)!\r\n",
                            i4_ret));
        }
        x_memset(&(pt_sess_obj->t_rutil_info.t_buf_info), 
                 0 ,
                 sizeof(REC_UTIL_PBA_BUF_INFO_T));
    }
    
    /* Set deinit*/
    if ((pt_sess_obj->t_pvr_descr.h_comp != NULL_HANDLE)
         && (pt_sess_obj->t_pvr_descr.e_comp_cond != RM_COND_CLOSED))
    {
        i4_ret = sm_sess_rcrd_pvr_set(pt_sess_obj->t_base.h_sess,
                                      PVR_SET_TYPE_DEINIT_CTRL,
                                      (VOID*)NULL,
                                       0);
    }
    
    return i4_ret;
    
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_pvr_ctrl_play_init
 *
 * Description: This function inits PVR CTRL when play a PVR Ctrl
 *
 * Inputs:  
 *       pt_sess_obj:  session object pointer
 *
 * Outputs: NULL
 *
 * Returns: 
 *        SMR_OK:  Succeed
 *        Others:  Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-11 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_pvr_ctrl_play_init(SM_SESS_RCRD_T* pt_sess_obj)
{
    INT32                  i4_ret;
    /*PVR_NFY_INFO_T         t_pvr_nfy_info;*/
    PVR_BUFFER_STATUS_T    t_pvr_buffer_status;
    PVR_BUFFER_INFO_T      t_pvr_buffer_info;
    SIZE_T                 t_size;

    if (NULL == pt_sess_obj)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl_play_init:arg is null!\r\n"));
        return SMR_INV_ARG;
    }

    /*  Do:
        PVR_SET_TYPE_FLUSH_BUFFER
        PVR_GET_TYPE_BUFFER_STATUS (&t_pvr_buffer_status)
        PVR_GET_TYPE_BUFFER_INFO (&t_pvr_buffer_info)
        rec_util_flush_pvr_buf_status (&t_buf_status)
    
    */
    /*0.flsuh pvr buffer remainder data*/
	i4_ret = rec_util_flush_pvr_buf(pt_sess_obj->t_rutil_info.h_record_util);
	if (i4_ret != REC_UTILR_OK)
	{
		SM_RSS_DBG_ERR(("sm_rcrd_sess_state_do_opened:flush pvr buffer data failed(%d)!\r\n",
						i4_ret));
	}
    /*1. PVR_SET_TYPE_FLUSH_BUFFER*/
    i4_ret = sm_sess_rcrd_pvr_set(pt_sess_obj->t_base.h_sess,
                                  PVR_SET_TYPE_FLUSH_BUFFER,
                                  (VOID*)NULL,
                                  0);
    if (i4_ret != RMR_OK)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl_play_init:FLUSH_BUFFER err=%d!\r\n",
                        i4_ret));
        return SMR_DRV_COMP_ERROR;
    }
    /* send flushed event */
    sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_PVR_FLUSHED,
                                   pt_sess_obj->t_base.h_sess,
                                   0);
    
    /*2. PVR_GET_TYPE_BUFFER_STATUS*/
    t_size = sizeof(PVR_BUFFER_STATUS_T);
    i4_ret = sm_sess_rcrd_pvr_get(pt_sess_obj->t_base.h_sess,
                                  PVR_GET_TYPE_BUFFER_STATUS,
                                  (VOID*)&t_pvr_buffer_status,
                                  &t_size);
    if (i4_ret != RMR_OK)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl_play_init: get BUFFER_STATUS err=%d!\r\n",
                        i4_ret));
        return SMR_DRV_COMP_ERROR;
    }
    
    /*3. PVR_GET_TYPE_BUFFER_INFO*/
    t_size = sizeof(PVR_BUFFER_INFO_T);
    i4_ret = sm_sess_rcrd_pvr_get(pt_sess_obj->t_base.h_sess,
                                  PVR_GET_TYPE_BUFFER_INFO,
                                  (VOID*)&t_pvr_buffer_info,
                                  &t_size);
    if (i4_ret != RMR_OK)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl_play_init: get BUFFER_INFO err=%d!\r\n",
                        i4_ret));
        return SMR_DRV_COMP_ERROR;
    }
    
    /*4. rec_util_flush_pvr_buf_status*/
    i4_ret = rec_util_flush_pvr_buf_status(pt_sess_obj->t_rutil_info.h_record_util,
                                           &t_pvr_buffer_info,
                                           &t_pvr_buffer_status);
    if (i4_ret != RMR_OK)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_ctrl_play_init:rec_util_flush_pvr err=%d!\r\n",
                        i4_ret));
        return SMR_DRV_COMP_ERROR;
    }
    
    return SMR_OK;
}



/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_pvr_unlock_data
 *
 * Description: This function unlock PVR ctrl component
 *
 * Inputs:  
 *       h_sess:              session handle
 *       pt_rutil_wr_ack:  write ack ino
 *
 * Outputs: NULL
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-09-21 : initial
 ------------------------------------------------------------------------------*/
VOID sm_sess_rcrd_pvr_unlock_data(HANDLE_T                   h_sess,
                                              REC_UTIL_WRITE_ACK_INFO_T*  pt_rutil_wr_ack)
{
    INT32                      i4_ret;
    SM_SESS_RCRD_T*            pt_sess_obj;
    PVR_UNLOCK_BUFFER_INFO_T   t_pvr_unlock_buf_info;

    pt_sess_obj = (SM_SESS_RCRD_T*)sm_get_sess_obj(h_sess);
    if (NULL == pt_sess_obj)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_unlock_data:pt_sess_obj is null!\r\n"));
        return;
    }

    if (NULL == pt_rutil_wr_ack)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_unlock_data:pt_rutil_wr_ack is null!\r\n"));
        return;
    }
    
    /*Unlock pv_written_data*/
    if ((pt_rutil_wr_ack->ui4_flags & REC_UTIL_WRITE_ACK_FLAG_WRITTEN_DATA_VALID) != 0)
    {
        t_pvr_unlock_buf_info.pui1_data_addr = pt_rutil_wr_ack->pv_written_data;
        t_pvr_unlock_buf_info.z_data_len     = pt_rutil_wr_ack->z_data_len;
        i4_ret = sm_sess_rcrd_pvr_set(pt_sess_obj->t_base.h_sess,
                                      PVR_SET_TYPE_UNLOCK_BUFFER,
                                      (VOID*)(&t_pvr_unlock_buf_info),
                                       (SIZE_T)(sizeof(PVR_UNLOCK_BUFFER_INFO_T)));
        if (i4_ret != SMR_OK)
        {
            /*debug error*/
            SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_unlock_data:free pvr buffer err=%d!\r\n",    i4_ret));
            
            sm_state_on_event(&pt_sess_obj->t_state,SM_EVN_SESS_RCRD_COMP_ERR, (VOID*)pt_sess_obj,
                              NULL,
                              NULL);
            return;
        }
    }
    
    /*Unlock pv_metadata_tag*/
    if ((pt_rutil_wr_ack->ui4_flags & REC_UTIL_WRITE_ACK_FLAG_METADATA_TAG_VALID) != 0)
    {
        i4_ret = sm_sess_rcrd_pvr_set(pt_sess_obj->t_base.h_sess,PVR_SET_TYPE_UNLOCK_METADATA,
                                      (pt_rutil_wr_ack->pv_metadata_tag),
                                       0);
        if (i4_ret != RMR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_pvr_unlock_data:free pvr metadata err=%d!\r\n", i4_ret));
            sm_state_on_event(&pt_sess_obj->t_state, SM_EVN_SESS_RCRD_COMP_ERR, (VOID*)pt_sess_obj,
                              NULL,
                              NULL);
            return;
        }
    }

    return ;
}


