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
 * $RCSfile: _sm_sess_rcrd_rutil.c,v $
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

#include "dt/x_dt.h"

/*#include "strm_mngr/_sm_utils.h"*/
#include "strm_mngr/sess_rcrd_hdlr/_sm_sess_rcrd_rutil.h"
#include "strm_mngr/sess_rcrd_hdlr/_sm_sess_rcrd_pvr.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define SM_SESS_REC_INIT_REQ_CNT_MAX      30
#define SM_SESS_REC_REQ_CNT_DELTA         20

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
 * Name:  _sm_sess_rcrd_rec_util_nfy_fct
 *
 * Description: This function is record utilty notify function
 *
 * Inputs:  
 *       h_rec_util:       record utilty handle
 *       pv_nfy_tag:       notify tag
 *       e_ntfy_code:      record utility condition
 *       ui4_data:         additonal data
 *
 * Outputs: 
 *       Send message to session to update record utility condition in SM
 *
 * Returns: 
 *       TRUE:  can reset record utility DB if reset notify code
 *       FALSE: can not reset DB if reset notify code
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-10 : initial
 ------------------------------------------------------------------------------*/
static BOOL _sm_sess_rcrd_rec_util_nfy_fct(HANDLE_T              h_rec_util,
                                           VOID*                 pv_nfy_tag,
                                           REC_UTIL_NTFY_CODE_T  e_ntfy_code,
                                           UINT32                ui4_data)
{
    SM_SESS_RCRD_MSG_T     t_msg;
    
    if (NULL == pv_nfy_tag)
    {
        return FALSE;
    }

    x_memset(&t_msg, 0, sizeof(SM_SESS_RCRD_MSG_T));
    t_msg.e_msg_type = SM_SESS_RCRD_MSG_TYPE_RUTIL;
    t_msg.h_sess = (HANDLE_T) pv_nfy_tag;
    
    t_msg.u.t_rutil_msg.e_rutil_cond = e_ntfy_code;
    t_msg.u.t_rutil_msg.ui4_reason = 0;

    if (REC_UTIL_NTFY_CODE_WRITE_ACK == e_ntfy_code)
    {
        t_msg.u.t_rutil_msg.u_util_ack.t_write_ack= *((REC_UTIL_WRITE_ACK_INFO_T*)ui4_data);
    }
    else if (REC_UTIL_NTFY_CODE_RESET_REQ == e_ntfy_code)
    {
        t_msg.u.t_rutil_msg.u_util_ack.ui4_reset_req_id = ui4_data;
    }
    else
    {
    }
    sm_rcrd_sess_send_msg(&t_msg);

    
    if (REC_UTIL_NTFY_CODE_RESET_REQ == e_ntfy_code)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  _sm_sess_rcrd_rutil_req_init
 *
 * Description: This function init data req 
 *
 * Inputs:  
 *       SM_SESS_REC_WR_REQ_T* pt_rec_util_write_req 
 *
 * Outputs: NULL
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-04-21 : initial
 ------------------------------------------------------------------------------*/
VOID _sm_sess_rcrd_rutil_req_init(SM_SESS_REC_WR_REQ_T* at_rec_util_write_req,
                                  UINT32 ui4_size)
{
    UINT32                  ui4_idx;

    if (NULL == at_rec_util_write_req)
    {
        SM_RSS_DBG_ERR(("_sm_sess_rcrd_rutil_req_init:arg is null!\r\n"));
        return;
    }
    
    SM_RSS_DBG_INFO(("_sm_sess_rcrd_rutil_req_init:ui4_size=%u\r\n", ui4_size));
    for (ui4_idx = 0; ui4_idx < ui4_size; ++ui4_idx)
    {
        at_rec_util_write_req[ui4_idx].e_stat = SM_SESS_REC_STAT_FREE;
        at_rec_util_write_req[ui4_idx].ui4_req_id = REC_UTIL_INV_REQ_ID;
    }

    return;
    
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_rutil_open
 *
 * Description: This function opens record utility for writting TS stream
 *
 * Inputs:  
 *       h_sess:      session handle
 *       pt_sess_obj: session object 
 *
 * Outputs: 
 *       pt_sess_obj: open a record utilty and get a handle
 *
 * Returns: 
 *        SMR_OK: Succeed
 *        Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-10 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_rutil_open(HANDLE_T        h_sess, 
                              SM_SESS_RCRD_T* pt_sess_obj)
{
    INT32                  i4_ret;
    REC_UTIL_COND_T        e_cond;
    /*SM_SESS_RCRD_MSG_T     t_msg;*/

    if (NULL == pt_sess_obj)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_open:arg is null!\r\n"));
        return SMR_INV_ARG;
    }

    /*Open record utility*/
    i4_ret = rec_util_open(pt_sess_obj->t_sess_cfg.s_name_rec_util,
                           _sm_sess_rcrd_rec_util_nfy_fct,
                           (VOID*)(h_sess),
                           &(pt_sess_obj->t_rutil_info.h_record_util));
    if (i4_ret != SMR_OK)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_open:open rutil err=%d!\r\n",
                        i4_ret));
        return i4_ret;   
    }
    sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_RUTI_OPENED,
                                   h_sess,
                                   0);
    /*Init write req*/
    pt_sess_obj->t_rutil_info.pt_rec_util_write_req = 
        (SM_SESS_REC_WR_REQ_T*)x_mem_alloc(sizeof(SM_SESS_REC_WR_REQ_T) 
                                           * SM_SESS_REC_INIT_REQ_CNT_MAX);
    if (NULL == pt_sess_obj->t_rutil_info.pt_rec_util_write_req)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_open:allocate failed!\r\n"));
        return SMR_INSUFFICIENT_MEMORY;
    }
    _sm_sess_rcrd_rutil_req_init(pt_sess_obj->t_rutil_info.pt_rec_util_write_req,
                                 SM_SESS_REC_INIT_REQ_CNT_MAX);

    pt_sess_obj->t_rutil_info.ui4_req_total_cnt = SM_SESS_REC_INIT_REQ_CNT_MAX;
    pt_sess_obj->t_rutil_info.ui4_req_running_cnt = 0;
    
    
    /*Get record utility state*/
    i4_ret = rec_util_get_cond(pt_sess_obj->t_rutil_info.h_record_util,
                               &e_cond);
    if ((SMR_OK == i4_ret) && (REC_UTIL_COND_READY == e_cond))
    {
        sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_RUTI_READY,
                                       h_sess,
                                       0);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_rutil_close
 *
 * Description: This function closes record utility
 *
 * Inputs:  
 *       h_sess:     session handle
 *       pt_sess_obj:session object
 *
 * Outputs: 
 *       pt_sess_obj: close record utility
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-10 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_rutil_close(HANDLE_T        h_sess, 
                               SM_SESS_RCRD_T* pt_sess_obj)
{
    INT32                  i4_ret = SMR_OK;

    if (pt_sess_obj->t_rutil_info.pt_rec_util_write_req != NULL)
    {
        x_mem_free(pt_sess_obj->t_rutil_info.pt_rec_util_write_req);
        pt_sess_obj->t_rutil_info.pt_rec_util_write_req  = NULL;
        pt_sess_obj->t_rutil_info.ui4_req_total_cnt = 0;
    }

    /*free ring buffer, This is a case where record utility close before pvr component*/
    if (pt_sess_obj->t_rutil_info.t_buf_info.pv_buf != NULL)
    {
        i4_ret = rec_util_free_pvr_buf(pt_sess_obj->t_rutil_info.h_record_util,
                                       &(pt_sess_obj->t_rutil_info.t_buf_info));
        if (i4_ret != REC_UTILR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_close:free pvr buffer failed(%d)!\r\n",
                            i4_ret));
        }
        x_memset(&(pt_sess_obj->t_rutil_info.t_buf_info), 
                 0 ,
                 sizeof(REC_UTIL_PBA_BUF_INFO_T));
    }
    
    if (pt_sess_obj->t_rutil_info.h_record_util != NULL_HANDLE)
    {
        i4_ret = rec_util_close(pt_sess_obj->t_rutil_info.h_record_util);
        if (i4_ret != RMR_OK)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_close:close rutil err=%d!\r\n",
                            i4_ret));
            /* Pretend closed*/
            /*sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_RUTI_CLOSED,
                                           h_sess,
                                           0);*/
        }
    }
    else
    {
        /* Pretend closed, if handle is not existed */
        /*sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_RUTI_CLOSED,
                                       h_sess,
                                       0);*/
    }

    sm_rcrd_sess_send_msg_do_event(SM_EVN_SESS_RCRD_RUTI_CLOSED,
                                   h_sess,
                                   0);
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_rutil_get_req
 *
 * Description: This function gets a record utility writting item
 *
 * Inputs:  
 *       pt_sess_obj:  session object pointer
 *
 * Outputs: 
 *       pt_write_req: record utilty request item
 *
 * Returns: 
 *        SMR_OK: Succeed
 *        Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-17 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_rutil_get_req(SM_SESS_RCRD_T*        pt_sess_obj,
                                 SM_SESS_REC_WR_REQ_T** ppt_write_req)
{
    UINT32                 ui4_inx;
    SM_SESS_REC_WR_REQ_T*  pt_req_iter;
    UINT32                 ui4_req_total_cnt;
    
    if ((NULL == pt_sess_obj) || (NULL == ppt_write_req))
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_get_req:args are null!\r\n"));
        return SMR_INV_ARG;
    }
    
    pt_req_iter = pt_sess_obj->t_rutil_info.pt_rec_util_write_req;
    if (NULL == pt_req_iter)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_get_req:not alloc reqs!\r\n"));
        return SMR_NOT_ALLOW;
    }

    ui4_req_total_cnt = pt_sess_obj->t_rutil_info.ui4_req_total_cnt; 
    for (ui4_inx = 0; ui4_inx < ui4_req_total_cnt; ++ui4_inx)
    {
        if (SM_SESS_REC_STAT_FREE == pt_req_iter[ui4_inx].e_stat)
        {
            break;
        }
    }

    if (ui4_inx < ui4_req_total_cnt)
    {
        *ppt_write_req = pt_req_iter + ui4_inx;
    }
    else
    {
    
        pt_req_iter = pt_sess_obj->t_rutil_info.pt_rec_util_write_req;
        pt_sess_obj->t_rutil_info.pt_rec_util_write_req = (SM_SESS_REC_WR_REQ_T*)
            x_mem_alloc((ui4_req_total_cnt + SM_SESS_REC_REQ_CNT_DELTA)
                         * sizeof(SM_SESS_REC_WR_REQ_T));
        if (NULL == pt_sess_obj->t_rutil_info.pt_rec_util_write_req)
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_get_req:fail to alloc reqs!\r\n"));
            return SMR_INSUFFICIENT_MEMORY;
        }
        _sm_sess_rcrd_rutil_req_init(pt_sess_obj->t_rutil_info.pt_rec_util_write_req,
                                     ui4_req_total_cnt + SM_SESS_REC_REQ_CNT_DELTA);
        
        x_memcpy(pt_sess_obj->t_rutil_info.pt_rec_util_write_req,
                 pt_req_iter,
                 sizeof(SM_SESS_REC_WR_REQ_T) * ui4_req_total_cnt);
        x_mem_free(pt_req_iter);
        pt_req_iter = NULL;
        pt_sess_obj->t_rutil_info.ui4_req_total_cnt = ui4_req_total_cnt + SM_SESS_REC_REQ_CNT_DELTA;
        *ppt_write_req = &(pt_sess_obj->t_rutil_info.pt_rec_util_write_req[ui4_req_total_cnt]);
    }

    (*ppt_write_req)->e_stat = SM_SESS_REC_STAT_USED;
    ++(pt_sess_obj->t_rutil_info.ui4_req_running_cnt);
    SM_RSS_DBG_INFO(("sm_sess_rcrd_rutil_get_req:get req idx(%u)!\r\n", ui4_inx));

    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_rutil_put_req
 *
 * Description: This function frees a record utility writting item
 *
 * Inputs:  
 *       pt_sess_obj:  session object pointer
 *       ui4_req_id:   req id
 *
 * Outputs: NULL
 *
 * Returns: 
 *        SMR_OK: Succeed
 *        Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-17 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_rutil_put_req(SM_SESS_RCRD_T*      pt_sess_obj,
                                  UINT32               ui4_req_id)
{
    UINT32                 ui4_inx;
    UINT32                 ui4_req_total_cnt;
    SM_SESS_REC_WR_REQ_T*  pt_write_req;
    
    if ((NULL == pt_sess_obj) 
         || (REC_UTIL_INV_REQ_ID == ui4_req_id)) 
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_put_req:arg is null!\r\n"));
        return SMR_INV_ARG;
    }

    pt_write_req = pt_sess_obj->t_rutil_info.pt_rec_util_write_req;
    if (NULL == pt_write_req)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_put_req:not alloc reqs!\r\n"));
        return SMR_INV_ARG;
    }

    ui4_req_total_cnt = pt_sess_obj->t_rutil_info.ui4_req_total_cnt; 
    for (ui4_inx = 0; ui4_inx < ui4_req_total_cnt; ++ ui4_inx)
    {
        if (pt_write_req[ui4_inx].ui4_req_id == ui4_req_id)
        {
            break;
        }
    }

    if (ui4_inx < ui4_req_total_cnt)
    {
        pt_write_req[ui4_inx].e_stat = SM_SESS_REC_STAT_FREE;
        pt_write_req[ui4_inx].ui4_req_id = REC_UTIL_INV_REQ_ID; /*invalid*/
        --(pt_sess_obj->t_rutil_info.ui4_req_running_cnt);
        SM_RSS_DBG_INFO(("sm_sess_rcrd_rutil_put_req:receive reqs(%d) ack !"
                         " ui4_req_running_cnt = %d "
                         "ui4_inx = %u\r\n",
                         ui4_req_id,
                         pt_sess_obj->t_rutil_info.ui4_req_running_cnt,
                         ui4_inx));
    }
    else
    {
        /*Do not find */
        SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_put_req:receive dismatch reqs(%d) ack !"
                        " ui4_req_running_cnt = %d\r\n",
                        ui4_req_id,
                        pt_sess_obj->t_rutil_info.ui4_req_running_cnt));
        return SMR_NOT_ALLOW;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_rutil_record
 *
 * Description: This function do recording data
 *
 * Inputs:  
 *       pt_sess_obj:  session object pointer
 *       pt_tick_data: data will be recorded and matedata
 *
 * Outputs: NULL
 *
 * Returns: 
 *        SMR_OK: Succeed
 *        Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-17 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_sess_rcrd_rutil_record(SM_SESS_RCRD_T*    pt_sess_obj, 
                                PVR_TICK_DATA_T*   pt_tick_data)
{
    INT32                   i4_ret;
    REC_UTIL_TICK_DATA_T    t_rutil_tick_data;
    SM_SESS_REC_WR_REQ_T*   pt_write_req;
    
#ifdef DEBUG
    static UINT32           ui4_tick_num = 0 ;
    static BOOL             b_rec = FALSE;
    if (b_rec)
    {
        SM_RSS_DBG_INFO(("Record Tick Data[%u]:pv_metadata_tag=0X%08x;  "
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

    if(NULL == pt_sess_obj)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_record:arg is null!\r\n"));
        return SMR_INV_ARG;
    }

    if ((SM_SESS_COND_STOPPING != pt_sess_obj->t_base.e_sess_cond)
        && (SM_SESS_COND_STARTED != pt_sess_obj->t_base.e_sess_cond))
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_record:(%u) state not allow to record !\r\n",
                        pt_sess_obj->t_base.e_sess_cond));
        return SMR_NOT_ALLOW;
    }
    
    i4_ret = SMR_OK;
    
    /*1. get a free req */
    i4_ret = sm_sess_rcrd_rutil_get_req(pt_sess_obj, &pt_write_req);
    if (i4_ret != SMR_OK)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_record:get req err=%d!\r\n",
                        i4_ret));
        return SMR_INTERNAL_ERROR;
    }

    /*2.call record utility API to write*/
    x_memset(&t_rutil_tick_data, 0, sizeof(REC_UTIL_TICK_DATA_T));
    t_rutil_tick_data.ui4_flags = 0;
    t_rutil_tick_data.t_time =  x_dt_get_utc(NULL, NULL);
    t_rutil_tick_data.ui4_flags |= REC_UTIL_TICK_DATA_FLAG_TIME_VALID;
    if (NULL != pt_tick_data)
    {
        x_memcpy(&(t_rutil_tick_data.t_tick_data), 
                 (VOID*)pt_tick_data, 
                 sizeof(PVR_TICK_DATA_T));
        t_rutil_tick_data.ui4_flags |= REC_UTIL_TICK_DATA_FLAG_DATA_VALID;
    }
    else
    {
    }
    i4_ret = rec_util_req_write(pt_sess_obj->t_rutil_info.h_record_util,
                                &t_rutil_tick_data,
                                &(pt_write_req->ui4_req_id));
    if (REC_UTILR_OK == i4_ret)
    {
        SM_RSS_DBG_INFO(("sm_sess_rcrd_rutil_record:send reqs(%d) "
                         " data_len=%d "
                         " ui4_req_running_cnt = %d\r\n",
                         pt_write_req->ui4_req_id,
                         (t_rutil_tick_data.ui4_flags & REC_UTIL_TICK_DATA_FLAG_DATA_VALID) ? t_rutil_tick_data.t_tick_data.ui4_pkt_count * 192 : 0,
                         pt_sess_obj->t_rutil_info.ui4_req_running_cnt));
    }
    else
    {
        pt_write_req->e_stat = SM_SESS_REC_STAT_FREE;
        pt_write_req->ui4_req_id = REC_UTIL_INV_REQ_ID; /*invalid*/
        --(pt_sess_obj->t_rutil_info.ui4_req_running_cnt);
        
        if (REC_UTIL_REQR_REACH_BOUND == i4_ret)
        {
            SM_RSS_DBG_INFO(("sm_sess_rcrd_rutil_record:reach bound!\r\n"));
            sm_state_on_event(&pt_sess_obj->t_state,
                              SM_EVN_SESS_RCRD_RUTI_REACH_BOUND,
                              (VOID*)pt_sess_obj,
                              NULL,
                              NULL);
        }
        else if (REC_UTILR_STRG_FULL == i4_ret)
        {
            SM_RSS_DBG_INFO(("sm_sess_rcrd_rutil_record:rutil write full!\r\n"));
            sm_state_on_event(&pt_sess_obj->t_state,
                              SM_EVN_SESS_RCRD_RUTI_STRG_FULL,
                              (VOID*)pt_sess_obj,
                              NULL,
                              NULL);
        }
        else if (REC_UTILR_REQ_NOT_ALLOW == i4_ret)
        {
            SM_RSS_DBG_INFO(("sm_sess_rcrd_rutil_record:not allow to wtite due to full or bound!\r\n"));
        }
        else  
        {
            SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_record:rutil write err=%d!\r\n", i4_ret));
            sm_state_on_event(&pt_sess_obj->t_state,
                              SM_EVN_SESS_RCRD_RUTI_ERR, /*SM_EVN_SESS_RCRD_COMP_ERR,*/
                              (VOID*)pt_sess_obj,
                              NULL,
                              NULL);
            return SMR_DRV_COMP_ERROR;
        }
         
    }
    
    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_rutil_is_stopped
 *
 * Description: This function check if all requests are done
 *
 * Inputs:  
 *       pt_sess_obj:  session object pointer
 *
 * Outputs: NULL
 *
 * Returns: 
 *        TRUE: ALL are done
 *        FALSE:At least one req is runnig
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-17 : initial
 ------------------------------------------------------------------------------*/
BOOL sm_sess_rcrd_rutil_is_stopped(SM_SESS_RCRD_T*    pt_sess_obj)
{
    UINT32                 ui4_inx;
    UINT32                 ui4_req_total_cnt;
    SM_SESS_REC_WR_REQ_T*  pt_write_req;
    
    if (NULL == pt_sess_obj) 
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_is_stopped:arg is null!\r\n"));
        return FALSE;
    }

    pt_write_req = pt_sess_obj->t_rutil_info.pt_rec_util_write_req;
    if (NULL == pt_write_req)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_is_stopped:not alloc reqs!\r\n"));
        return TRUE;
    }

    ui4_req_total_cnt = pt_sess_obj->t_rutil_info.ui4_req_total_cnt; 
    for (ui4_inx = 0; ui4_inx < ui4_req_total_cnt; ++ ui4_inx)
    {
        if (SM_SESS_REC_STAT_USED == pt_write_req->e_stat)
        {
            return FALSE;
        }
        ++pt_write_req;
    }

    return TRUE;    
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_check_rutil_req_status
 *
 * Description: This function check if all requests states
 *
 * Inputs:  
 *       pt_sess_obj:  session object pointer
 *
 * Outputs: NULL
 *
 * Returns:
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-04-15 : initial
 ------------------------------------------------------------------------------*/
VOID sm_sess_rcrd_check_rutil_req_status(SM_SESS_RCRD_T*    pt_sess_obj)
{
    UINT32                 ui4_inx;
    UINT32                 ui4_req_total_cnt;
    SM_SESS_REC_WR_REQ_T*  pt_write_req;
    
    if (NULL == pt_sess_obj) 
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_status:arg is null!\r\n"));
        return;
    }

    pt_write_req = pt_sess_obj->t_rutil_info.pt_rec_util_write_req;
    if (NULL == pt_write_req)
    {
        SM_RSS_DBG_ERR(("sm_sess_rcrd_rutil_status:not alloc reqs!\r\n"));
        return;
    }

    ui4_req_total_cnt = pt_sess_obj->t_rutil_info.ui4_req_total_cnt; 
    for (ui4_inx = 0; ui4_inx < ui4_req_total_cnt; ++ ui4_inx)
    {
        if (SM_SESS_REC_STAT_USED == pt_write_req->e_stat)
        {
            SM_RSS_DBG_INFO(("\r\nsm_sess_rcrd_rutil_status:req(%d) is running!\r\n"));
        }
        ++pt_write_req;
    }

    return;    
}

