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
 * $RCSfile: rec_util.c,v $
 * $Revision: #2 $
 * $Date: 2012/06/20 $
 * $Author: weihua.chen $
 * $CCRevision: /main/DTV_X_HQ_int/6 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: ab263bfe093fb6a9cf962d72692852e1 $
 *
 * Description:
 *         This file contains Event Context Selector specific functions.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "os/memory/mem_lib.h"
#include "dbg/dbg.h"
#include "dbg/u_dbg.h"
#include "file_mngr/x_fm.h"
#include "file_mngr/x_fm_mfw.h"
#include "file_mngr/u_fm.h"
#include "evctx/evctx_atsc_eng/u_evctx_atsc_eng.h"
#include "evctx/evctx_dvb_eng/u_evctx_dvb_eng.h"
#include "evctx/evctx_isdb_eng/u_evctx_isdb_eng.h"
#include "cli/x_cli.h"
#include "rec_util/_rec_util.h"
#include "rec_util/rec_util_db.h"
#include "rec_util/rec_util_msg.h"
#include "rec_util/rec_util_thread.h"
#include "rec_util/rec_util_utils.h"
#include "rec_util/rec_util_meta.h"

 /*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL
#define DBG_CHECK_WRITTEN_TS_DATA(_pt_db, _ui8_file_pos, _pui1_data, _z_data_size) \
            if (b_en_data_checking)                                                \
            {                                                                      \
                _rec_util_dbg_check_written_ts_data ((_pt_db), (_ui8_file_pos), (_pui1_data), (_z_data_size));   \
            }
#else
#define DBG_CHECK_WRITTEN_TS_DATA(_pt_db, _ui8_file_pos, _pui1_data, _z_data_size)
#endif

#define REC_UTIL_PADDING_TICK_NUM  ((UINT32)10)
#define REC_UTIL_MAX_REQ_NUM       ((UINT32)4)
 /*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
extern REC_UTIL_SYS_CONFIG_T  t_rec_util_sys_cfg;
static REC_UTIL_DB_HEAD_T*    pt_rec_db_head_list = NULL;

/*static UINT32                 ui4_max_req_id      = 0;*/

#ifdef CLI_LVL_ALL
BOOL                          b_en_data_checking  = FALSE;
#endif
 
static UINT32                 ui4_rec_util_req_id = REC_UTIL_INV_REQ_ID;
static UINT32                 ui4_fm_req_timer_delay;
/*-----------------------------------------------------------------------------
                functions declaraions
----------------------------------------------------------------------------*/
static INT32 _rec_util_check_lba_valid(UINT32                 ui4_lba,
                                       REC_UTIL_DB_T*         pt_db);
                                       
static INT32 _rec_util_do_fifo_req_process(HANDLE_T             h_rec_util,
                                         REC_UTIL_DB_HEAD_T*  pt_db_head);                                       

static INT32 _rec_util_flush_pvr_buf(HANDLE_T   h_rec_util);

static INT32 _rec_util_search_db_head(const CHAR*             ps_name_db,
                                        REC_UTIL_DB_HEAD_T**    ppt_db_head);
static BOOL  _rec_util_free_handle(HANDLE_T        h_handle,
                                          HANDLE_TYPE_T   e_type,
                                          VOID*           pv_obj,
                                          VOID*           pv_tag,
                                          BOOL            b_req_handle);

static INT32 _rec_util_insert_db_head_to_list(REC_UTIL_DB_HEAD_T* pt_db_head);

static INT32 _rec_util_free_db_head(REC_UTIL_DB_HEAD_T*  pt_db_head);


/*-----------------------------------------------------------------------------
                static functions implementation
----------------------------------------------------------------------------*/
static INT32 _rec_util_open(
                                const CHAR*                 ps_name_db,
                                rec_util_nfy_fct            pf_nfy,
                                VOID*                       pv_nfy_tag,
                                HANDLE_T*                   ph_rec_util,
                                REC_UTIL_CLIENT_T**         ppt_client_obj,
                                REC_UTIL_DB_HEAD_T**        ppt_db_head,
                                REC_UTIL_DB_T**             ppt_db)
{
    INT32                   i4_ret              = REC_UTILR_OK;
    REC_UTIL_DB_HEAD_T*     pt_db_head          = NULL;
    REC_UTIL_DB_T*          pt_db               = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj       = NULL;
    UINT32                  ui4_name_len;
    HANDLE_T                h_rec_db_head;
    HANDLE_T                h_rec_util;
    BOOL                    b_new_db_head       = FALSE;
    REC_UTIL_HDL_LINK_T*    pt_hdl_link         = NULL; 
    REC_UTIL_REQ_LINK_T*    pt_req_link         = NULL;
    REC_UTIL_REQ_T*         pt_req              = NULL;
    BOOL                    b_found             = FALSE;
    

    ui4_name_len = x_strlen(ps_name_db); 
    do
    {
        /*search db head by db name*/
        i4_ret = _rec_util_search_db_head(ps_name_db, &pt_db_head);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR(("rec_util_open: search db head error!\r\n"));
            break;
        }
        if (NULL == pt_db_head)
        {
            /*alloc db head object*/
            pt_db_head = (REC_UTIL_DB_HEAD_T*)x_mem_alloc(sizeof(REC_UTIL_DB_HEAD_T));
            if (NULL == pt_db_head)
            {
                REC_UTIL_DBG_ERR(("rec_util_open: db head obj mem alloc error!\r\n"));
                i4_ret = REC_UTILR_OUT_OF_MEMORY;
                break;
            }
            x_memset(pt_db_head, 0, sizeof(REC_UTIL_DB_HEAD_T));
            
            /* allocate db head handle */
            if (HR_OK != handle_alloc(RU_HT_DB_HEAD_HANDLER, pt_db_head, 
                                      NULL, _rec_util_free_handle,
                                      &h_rec_db_head))
            {
                REC_UTIL_DBG_ERR(("rec_util_open: db head obj handle alloc error!\r\n"));
                x_mem_free(pt_db_head);
                i4_ret = REC_UTILR_OUT_OF_HANDLE;
                break;
            }
            _rec_util_insert_db_head_to_list(pt_db_head);
            pt_db_head->h_rec_db_head  = h_rec_db_head;
            if((pt_db_head->s_db_name = x_mem_alloc(ui4_name_len + 1)) == NULL)
            {
                x_dbg_stmt("Rec_util:Out of memory line %d \n",__LINE__);
                handle_free (h_rec_db_head, FALSE);
                h_rec_db_head = NULL_HANDLE;
                i4_ret = REC_UTILR_OUT_OF_MEMORY;
                break;
            }
            x_strcpy(pt_db_head->s_db_name, ps_name_db);
            pt_db_head->e_cond         = REC_UTIL_COND_TRANSITION;
            b_new_db_head              = TRUE;
        }
        
        /*alloc client object*/
        pt_client_obj = (REC_UTIL_CLIENT_T*)x_mem_alloc(sizeof(REC_UTIL_CLIENT_T));
        if (NULL == pt_client_obj)
        {
            REC_UTIL_DBG_ERR(("rec_util_open: client obj mem alloc error!\r\n"));
            i4_ret = REC_UTILR_OUT_OF_MEMORY;
            break;
        }
        x_memset(pt_client_obj, 0, sizeof(REC_UTIL_CLIENT_T));
        
        /* allocate client handle */
        if (HR_OK != handle_alloc(RU_HT_CLIENT_HANDLER, pt_client_obj, 
                                  NULL, _rec_util_free_handle, 
                                  &h_rec_util))
        {
            x_mem_free(pt_client_obj);
            pt_client_obj = NULL;
            i4_ret = REC_UTILR_OUT_OF_HANDLE;
            break;
        }
        pt_client_obj->h_rec_util     = h_rec_util;
        pt_client_obj->pf_nfy         = pf_nfy;
        pt_client_obj->pv_tag         = pv_nfy_tag;
        pt_client_obj->pt_rec_db_head = pt_db_head;
        
        /*link client handle in db head obj*/
        pt_hdl_link = (REC_UTIL_HDL_LINK_T*)x_mem_alloc(sizeof(REC_UTIL_HDL_LINK_T));
        if (NULL == pt_hdl_link)
        {
            REC_UTIL_DBG_ERR(("rec_util_open: handle link mem alloc error!\r\n"));
            handle_free(h_rec_util, TRUE);
            i4_ret = REC_UTILR_OUT_OF_MEMORY;
            break;
        }
        x_memset(pt_hdl_link, 0, sizeof(REC_UTIL_HDL_LINK_T));
        pt_hdl_link->h_rec_util = h_rec_util;
        DLIST_INSERT_TAIL(pt_hdl_link, &pt_db_head->t_client_handle_list, t_link);
        /*return h_rec_util*/
        *ph_rec_util = h_rec_util;  

        /*search set cfg req*/
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db) /*only has db head obj*/
        {
            break;
        }
        
        /*search set_cfg_req from db obj's running_req_list*/
        DLIST_FOR_EACH(pt_req_link, &pt_db->t_running_req_list, t_link) 
        {
            pt_req = &(pt_req_link->t_rec_util_req);
            if (pt_req->e_req_type == REC_UTIL_REQ_SET_CFG) 
            {
                b_found = TRUE;
                break;
            }
        }
        /*remove hdl from set cfg req hdl list, may be failed*/
        if (b_found)
        {
            ++(pt_req->u_data.t_set_cfg_req.ui1_client_hdl_num);
        }   
    }while(0);
    
    if (i4_ret == REC_UTILR_OK)
    {
        *ph_rec_util    = h_rec_util;
        *ppt_client_obj = pt_client_obj;
        *ppt_db_head    = pt_db_head;
        *ppt_db         = pt_db;
    }    
    else
    {
        if (b_new_db_head)
        {
            _rec_util_free_db_head(pt_db_head);
        }
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
* Name: _rec_util_abort_fm_async
*
* Description: This function abort all write async of certain req. 
*
* Inputs:  pt_req_link
*              Handle of the opened file.
*
* Outputs: -
*
* Returns: -
----------------------------------------------------------------------------*/
static VOID _rec_util_abort_fm_async(REC_UTIL_REQ_LINK_T* pt_req_link)
{
    UINT8                       ui1_idx;
    HANDLE_T                    h_req;
    INT32                       i4_ret;
    
    if(NULL == pt_req_link)
    {
        REC_UTIL_DBG_ERR(("_rec_util_abort_fm_async:invalid param!\r\n"));
        return;
    }

    if (REC_UTIL_REQ_FM_WRITE == pt_req_link->t_rec_util_req.e_req_type)
    {
        if (pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer->b_active)
        {
            i4_ret = x_timer_stop (pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer->h_timer);
            if (i4_ret != OSR_OK)
            {
                REC_UTIL_DBG_ERR (("Fail to stop timer (%d)\r\n", i4_ret));
            }
            pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer->b_active = FALSE;
        }
        
        if(0 == pt_req_link->t_rec_util_req.u_data.t_fm_write_req.ui1_fm_req_cnt)
        { 
           
        }
        else
        {
            for(ui1_idx = 0; 
                ui1_idx < REC_UTIL_FM_WRITE_MAX_NUM_PER_UTIL_REQ; 
                ++ui1_idx)
            {
                h_req = pt_req_link->t_rec_util_req.u_data.t_fm_write_req.ah_req[ui1_idx];
                if(NULL_HANDLE != h_req)
                {
                    i4_ret = x_fm_mfw_abort_async(h_req);
                    if(FMR_OK != i4_ret)
                    {
                        x_dbg_stmt("{REC_UTIL} ERR: Fail to abort FM req 0x%X (%d). \r\n", h_req, i4_ret);
                    }
                    
                    pt_req_link->t_rec_util_req.u_data.t_fm_write_req.ah_req[ui1_idx] = NULL_HANDLE;
                    --(pt_req_link->t_rec_util_req.u_data.t_fm_write_req.ui1_fm_req_cnt);
                }
            }
        }

        pt_req_link->t_rec_util_req.u_data.t_fm_write_req.b_fm_reqs_done = TRUE;        
    }
    else
    {
        REC_UTIL_DBG_ERR(("_rec_util_abort_fm_async:not fm write req!\r\n"));
    }
}
    
/*-----------------------------------------------------------------------------
* Name: _rec_util_async_write_nfy
*
* Description: This function is called by the fm's async write function
*              when async write is complete. 
*
* Inputs:  h_file
*              Handle of the opened file.
*          pv_tag
*              Tag value, which will be returned in the callback.
*          e_async_cond
*              Contains the async write cond.
*          ui4_data
*              Contains the write data size.
*
* Outputs: -
*
* Returns: -
----------------------------------------------------------------------------*/
static VOID _rec_util_async_write_nfy(HANDLE_T                   h_req, 
                                      VOID*                      pv_tag, 
                                      FM_ASYNC_COND_T            e_async_cond, 
                                      UINT32                     ui4_data)
{
    REC_UTIL_MSG_T              t_msg;
    HANDLE_T                    h_rec_util;
    
    if (pv_tag != NULL)
    {
        h_rec_util = (HANDLE_T)pv_tag;
        t_msg.e_msg_type = REC_UTIL_MSG_FM_ASYNC_WRITE_ACK;
        t_msg.u_data.t_fm_async_write.h_rec_util = h_rec_util;
        t_msg.u_data.t_fm_async_write.e_async_cond = e_async_cond;
        t_msg.u_data.t_fm_async_write.h_req = h_req;
        t_msg.u_data.t_fm_async_write.ui4_data = ui4_data;    
        rec_util_send_msg(&t_msg);
    }
}

/*-----------------------------------------------------------------------------
* Name: _rec_util_write_timeout_nfy
*
* Description: This function is called by timer. 
*
* Inputs:  h_rec_util
*              Handle of the rec_util_db_head.
*          pv_tag
*              Tag value, which will be returned in the callback.
*          
*
* Outputs: -
*
* Returns: -
----------------------------------------------------------------------------*/

static VOID _rec_util_write_timeout_nfy(HANDLE_T                h_timer_hdl,
                                     VOID*                      pv_tag)
{
    REC_UTIL_REQ_LINK_T*    pt_req_link;
    REC_UTIL_REQ_T*         pt_req;
    REC_UTIL_MSG_T          t_msg;

    pt_req_link = (REC_UTIL_REQ_LINK_T*)pv_tag;
    
    if(NULL != pt_req_link)
    {
        pt_req = &(pt_req_link->t_rec_util_req);

        x_memset(&t_msg, 0, sizeof(REC_UTIL_MSG_T));    
        t_msg.e_msg_type = REC_UTIL_MSG_WRITE_TIMEOUT;  
        t_msg.u_data.t_write_timeout.h_rec_util = pt_req->h_rec_util;      
        t_msg.u_data.t_write_timeout.ui4_abort_req_id = pt_req->ui4_req_id;
        
        rec_util_send_msg(&t_msg);
    }
    else
    {
        
    }
}

#ifdef CLI_LVL_ALL
static BOOL _rec_util_dbg_valid_ts_data(const UINT8*            pui1_data,
                                        UINT32                  ui4_data_len,
                                        UINT32*                 pui4_total_pkt,
                                        UINT32*                 pui4_err_pkt)
{
    UINT32      ui4_i;
    BOOL        b_valid;
    
    *pui4_total_pkt = 0;
    *pui4_err_pkt = 0;
    
    b_valid = TRUE;
    for (ui4_i = 0; ui4_i < ui4_data_len; ui4_i += REC_UTIL_LOGIC_BLOCK_SIZE)
    {
        ++ (*pui4_total_pkt);
        if (pui1_data[ui4_i + 4] != 0x47)
        {
            b_valid = FALSE;
            ++ (*pui4_err_pkt);
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_INVALID_TS_DATA);
        }
    }
    
    return b_valid;
}

static VOID _rec_util_dbg_check_written_ts_data (REC_UTIL_DB_T*      pt_db,
                                                 UINT64              ui8_file_pos,
                                                 UINT8*              pui1_data,
                                                 SIZE_T              z_data_size)
{
    UINT64  ui8_ofs_pos;
    UINT64  ui8_lba_pos;
    UINT32  ui4_diff;
    UINT32  ui4_total_pkt;
    UINT32  ui4_err_pkt;

    if (z_data_size >= REC_UTIL_LOGIC_BLOCK_SIZE)
    {
        ui8_ofs_pos = ui8_file_pos - pt_db->ui8_tsb_begin_pos;
        ui8_lba_pos = ((ui8_ofs_pos + REC_UTIL_LOGIC_BLOCK_SIZE - 1) / REC_UTIL_LOGIC_BLOCK_SIZE) * REC_UTIL_LOGIC_BLOCK_SIZE;
        ui4_diff = (UINT32) (ui8_lba_pos - ui8_ofs_pos);
        
        /*x_dbg_stmt ("{REC_UTIL} Write To: 0x%llX \n", ui8_file_pos);*/
        if (!_rec_util_dbg_valid_ts_data (pui1_data + ui4_diff,
                                          z_data_size - (SIZE_T) ui4_diff,
                                          & ui4_total_pkt,
                                          & ui4_err_pkt))
        {
            x_dbg_stmt("{REC_UTIL} ERR: Invalid TS Data (%d/%d)! \n",
                       ui4_err_pkt,
                       ui4_total_pkt );
        }
    }    
}        
#endif

/*-----------------------------------------------------------------------------
 * Name: rec_util_get_db_head_list
 *
 * Description: This API generate the new req id.
 *              
 * Inputs:  NULL
 *          
 * Outputs: NULL
 *
 * Returns: pt_rec_db_head_list      
 *
 ----------------------------------------------------------------------------*/
 REC_UTIL_DB_HEAD_T* rec_util_get_db_head_list(VOID)
 {
    return pt_rec_db_head_list;
 }



/*-----------------------------------------------------------------------------
 * Name: _rec_util_gen_req_id
 *
 * Description: This API generate the new req id.
 *              
 * Inputs:  pt_db
 *          
 * Outputs: NULL
 *
 * Returns: UINT32      the req id.
 ----------------------------------------------------------------------------*/
 static UINT32 _rec_util_gen_req_id(VOID)
 {
    ++(ui4_rec_util_req_id);
    if (REC_UTIL_INV_REQ_ID == ui4_rec_util_req_id)
    {
        ui4_rec_util_req_id = 0;
    }
    
    return ui4_rec_util_req_id;
 }

  /*-----------------------------------------------------------------------------
 * Name: _rec_util_gen_tick_num
 *
 * Description: This API generate the new tick num.
 *              
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: UINT32      the new tick num.
 ----------------------------------------------------------------------------*/
static UINT32 _rec_util_gen_tick_num(REC_UTIL_DB_T*     pt_db)
{
    ++(pt_db->ui4_last_tick_num);
    if ((UINT32)0xFFFFFFFF == pt_db->ui4_last_tick_num)
    {
        pt_db->ui4_last_tick_num = 0;
    }
    
    return pt_db->ui4_last_tick_num;
}

 /*-----------------------------------------------------------------------------
 * Name: _rec_util_search_db_head
 *
 * Description: This API searchs a rec db head by db name.
 *              
 * Inputs:  s_name_db    Specifies the name of the record utility. Different names 
 *                       refer to different separated database.
 *          
 * Outputs: ppt_db_head  Contains the searched rec db head.
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess.
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_search_db_head(const CHAR*             ps_name_db,
                                      REC_UTIL_DB_HEAD_T**    ppt_db_head)
{
    INT32                   i4_ret = REC_UTILR_OK;
    REC_UTIL_DB_HEAD_T*     pt_db_head;

    /*check param*/
    if ((NULL == ps_name_db) || (NULL == ppt_db_head))
    {
        REC_UTIL_DBG_ERR(("_rec_util_search_db_head: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    pt_db_head = pt_rec_db_head_list;
    *ppt_db_head = NULL;
    while (NULL != pt_db_head)
    {
        if (x_strcmp(ps_name_db, pt_db_head->s_db_name) == 0)
        {
            *ppt_db_head = pt_db_head;
            break;
        }
        pt_db_head = pt_db_head->pt_next;
    }
    
    return i4_ret;
}

 /*-----------------------------------------------------------------------------
 * Name: _rec_util_insert_db_head_to_list
 *
 * Description: This API insert a rec db head to db head list.
 *              
 * Inputs:  pt_db_head   Specify the db head object.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess.
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_insert_db_head_to_list(REC_UTIL_DB_HEAD_T* pt_db_head)
{
    REC_UTIL_DB_HEAD_T* pt_cur_node = NULL;

    if (NULL == pt_db_head)
    {
        REC_UTIL_DBG_ERR(("_rec_util_insert_db_head_to_list: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    if (NULL == pt_rec_db_head_list)
    {
        pt_rec_db_head_list = pt_db_head;
        return REC_UTILR_OK;
    }
    
    pt_cur_node = pt_rec_db_head_list;
    while (NULL != pt_cur_node->pt_next)
    {
        pt_cur_node = pt_cur_node->pt_next;
    }
    
    pt_cur_node->pt_next = pt_db_head;
    return REC_UTILR_OK;
}

  /*-----------------------------------------------------------------------------
 * Name: _rec_util_remove_db_head_from_list
 *
 * Description: This API remove a rec db head from db head list.
 *              
 * Inputs:  pt_db_head   Specify the db head object.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess
 *          REC_UTILR_INTERNAL_ERROR    Fail.
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_remove_db_head_from_list(REC_UTIL_DB_HEAD_T* pt_db_head)
{
    REC_UTIL_DB_HEAD_T* pt_prev_node = NULL;
    REC_UTIL_DB_HEAD_T* pt_next_node = NULL;
    BOOL                b_found = FALSE;

    if (NULL == pt_db_head)
    {
        REC_UTIL_DBG_ERR(("_rec_util_remove_db_head_from_list: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    if (pt_rec_db_head_list == pt_db_head)
    {
        pt_rec_db_head_list = pt_rec_db_head_list->pt_next;
        return REC_UTILR_OK;
    }
    
    pt_next_node = pt_rec_db_head_list;
    while (NULL != pt_next_node)
    {
        if (pt_next_node == pt_db_head)
        {
            b_found = TRUE;
            break;
        }
        pt_prev_node = pt_next_node;
        pt_next_node = pt_next_node->pt_next;
    }
    
    if (!b_found)
    {
        REC_UTIL_DBG_ERR(("_rec_util_remove_db_head_from_list: not found db head obj!\r\n"));
        return REC_UTILR_INTERNAL_ERROR;
    }

    if(NULL == pt_prev_node)
    {
        pt_rec_db_head_list = pt_db_head->pt_next;
    }
    else
    {
        pt_prev_node->pt_next = pt_db_head->pt_next;
    }
    
    return REC_UTILR_OK;
}
  
 /*-----------------------------------------------------------------------------
 * Name: _rec_util_free_handle
 *
 * Description: This API is called when a record utility handle is freed.
 *
 * Inputs:  h_handle      Contains the handle that should be freed.
 *          e_type        Contains the handle type.
 *          pv_obj        References the handle's object.
 *          pv_tag        References the handle's tag.
 *          b_req_handle  Is set to TRUE if the handle free was initiated with
 *                        this handle else FALSE.
 *
 * Outputs: -
 *
 * Returns: TRUE if the handle free is allowed, FALSE otherwise.
 ----------------------------------------------------------------------------*/
static BOOL _rec_util_free_handle(HANDLE_T        h_handle,
                                  HANDLE_TYPE_T   e_type,
                                  VOID*           pv_obj,
                                  VOID*           pv_tag,
                                  BOOL            b_req_handle)
{
    REC_UTIL_DB_HEAD_T*    pt_db_head_obj = NULL;
    REC_UTIL_CLIENT_T*     pt_client_obj  = NULL;

    if (pv_obj != NULL)
    {
        if (e_type == RU_HT_DB_HEAD_HANDLER)
        {
            pt_db_head_obj = (REC_UTIL_DB_HEAD_T*)pv_obj;
            x_mem_free(pt_db_head_obj);
        }
        else if (e_type == RU_HT_CLIENT_HANDLER)
        {
            pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
            x_mem_free(pt_client_obj);
        }
        pv_obj = NULL;
        return TRUE;
    }
    REC_UTIL_DBG_ERR(("_rec_util_free_handle: free handle error!\r\n"));
    
    return FALSE;
} 

 /*-----------------------------------------------------------------------------
 * Name: _rec_util_search_handle_link
 *
 * Description: This API search a handle link by h_rec_util.
 *
 * Inputs:  h_rec_util    Contains the handle that should be searched.
 *          pt_hdl_list   Contains the handle link list.
 *
 * Outputs: ppt_hdl_link  Contains the searched handle link.
 *
 * Returns: TRUE if the handle link is found, FALSE otherwise.
 ----------------------------------------------------------------------------*/
 static BOOL _rec_util_search_handle_link(HANDLE_T                        h_rec_util,
                                          REC_UTIL_HDL_LIST_T*            pt_hdl_list,
                                          REC_UTIL_HDL_LINK_T**           ppt_hdl_link)
{
    REC_UTIL_HDL_LINK_T*    pt_hdl_link;

    if ((NULL == ppt_hdl_link) || (NULL == pt_hdl_list)) 
    {
        REC_UTIL_DBG_ERR(("_rec_util_search_handle_link: invalid arguments!\r\n"));
        return FALSE;
    }
    
    DLIST_FOR_EACH(pt_hdl_link, pt_hdl_list, t_link) 
    {
        if (pt_hdl_link->h_rec_util == h_rec_util) 
        {
            *ppt_hdl_link = pt_hdl_link;
            return TRUE;
        }
    }
    
    REC_UTIL_DBG_ERR(("_rec_util_search_handle_link: not found this handle link!\r\n"));
    
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: _rec_util_remove_req_link
 *
 * Description: This API remove a req link from a queue, and put it in free_req_list.
 *
 * Inputs:  pt_req_link         Contains the req link that should be removed.
 *          pt_this_list        Contains the req link list.
 *          pt_free_req_list    Contains the free req list.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _rec_util_remove_req_link(REC_UTIL_REQ_LINK_T*       pt_req_link,
                                              REC_UTIL_REQ_LIST_T*       pt_this_list,
                                              REC_UTIL_REQ_LIST_T*       pt_free_req_list)
{
    REC_UTIL_REQ_LINK_T*            pt_req_iter;
    BOOL                            b_found = FALSE;
    
    if ((NULL == pt_req_link) || (NULL == pt_this_list) || (NULL == pt_free_req_list))
    {
        REC_UTIL_DBG_ERR(("_rec_util_remove_req_link:"
                          "pt_req_link      = %p\r\n"
                          "pt_this_list     = %p\r\n"
                          "pt_free_req_list = %p\r\n",
                          pt_req_link,
                          pt_this_list,
                          pt_free_req_list));
        return;
    }
    
    DLIST_FOR_EACH(pt_req_iter, pt_this_list, t_link) 
    {
        if (pt_req_iter == pt_req_link)
        {
            DLIST_REMOVE(pt_req_link, pt_this_list, t_link);
            b_found = TRUE;
            break;
        }
    }
    
    if (b_found)
    {
        x_memset(pt_req_link, 0, sizeof(REC_UTIL_REQ_LINK_T));
        DLIST_INSERT_TAIL(pt_req_link, pt_free_req_list, t_link);
    }
    else
    {
        REC_UTIL_DBG_ERR (("Not find req link(0x%08x) at L%d. \n", (UINT32)pt_req_link,  __LINE__));
    }
}

/*-----------------------------------------------------------------------------
 * Name: _rec_util_check_list_status
 *
 * Description: This API get status of the type specified by e_req_type and handle it.
 *
 * Inputs:  pt_req_list    Contains the req list to be handled.
 *
 * Returns: this is not used by now for further extern
            REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess
 *          REC_UTILR_INTERNAL_ERROR     fail
 ----------------------------------------------------------------------------*/

static INT32 _rec_util_check_list_status(REC_UTIL_REQ_LIST_T*     pt_running_req_list,
                                              REC_UTIL_REQ_TYPE        e_req_type )
{
    INT32 i4_ret                        = REC_UTILR_OK;
    REC_UTIL_REQ_LINK_T*    pt_req_link = NULL;
    UINT8 ui1_req_num                   = 0;
    
    if(!pt_running_req_list)
    {
        return REC_UTILR_INV_ARG;
    }
    DLIST_FOR_EACH_BACKWARD(pt_req_link,pt_running_req_list,t_link) 
    {
        if (pt_req_link->t_rec_util_req.e_req_type== e_req_type)
        {
            if(++ui1_req_num >= REC_UTIL_MAX_REQ_NUM)
            {
                if(pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result == REC_UTIL_REQR_TIME_OUT)
                {
                    x_dbg_stmt("\nAbort write req id = %d\n",pt_req_link->t_rec_util_req.ui4_req_id);
                    _rec_util_abort_fm_async(pt_req_link);
                    pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result =  REC_UTIL_REQR_ABORTED;
                }
                else
                {
                    /*REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);*/
                }
            }
        }            
    }
    /*it should process the running list to release the req*/

    /*we do nothing and left these thing to others,because of the complicate parameteres of fifo_process*/

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _rec_util_get_req_link
 *
 * Description: This API get a req link from free_req_list.
 *
 * Inputs:  pt_free_req_list    Contains the free req list.
 *
 * Outputs: ppt_req_link        Contains the req link that get from free req list.
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess
 *          REC_UTILR_OUT_OF_MEMORY     fail
 ----------------------------------------------------------------------------*/
 static INT32 _rec_util_get_req_link(REC_UTIL_REQ_LIST_T*     pt_free_req_list,
                                     REC_UTIL_REQ_LINK_T**    ppt_req_link)
{
    REC_UTIL_REQ_LINK_T* pt_req_link;
    
    if (NULL == ppt_req_link)
    {
        REC_UTIL_DBG_ERR(("_rec_util_get_req_link: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    if (!DLIST_IS_EMPTY(pt_free_req_list))
    {
        
    }
    else
    {
        pt_req_link = (REC_UTIL_REQ_LINK_T*)x_mem_alloc(sizeof(REC_UTIL_REQ_LINK_T));
        if (NULL == pt_req_link)
        {
            REC_UTIL_DBG_ERR(("_rec_util_get_req_link: req link mem alloc fail!\r\n"));
            return REC_UTILR_OUT_OF_MEMORY;
        }
        x_memset(pt_req_link, 0, sizeof(REC_UTIL_REQ_LINK_T));
        DLIST_INSERT_TAIL(pt_req_link, pt_free_req_list, t_link);
    }
    
    pt_req_link = DLIST_HEAD(pt_free_req_list);
    *ppt_req_link = pt_req_link;
    DLIST_REMOVE(pt_req_link, pt_free_req_list, t_link);
    return REC_UTILR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _rec_util_alloc_timer_link
 *
 * Description: This API allocate a timer link.
 *
 * Inputs:  pt_db           Contains the DB.
 *
 * Outputs: ppt_timer_link  Contains the timer link.
 *
 * Returns: REC_UTILR_OK                Sucess
 *          REC_UTILR_OUT_OF_MEMORY     Out of memory
 *          REC_UTILR_OTHER_MODULE_FAIL Other module's fail
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_alloc_timer_link (REC_UTIL_DB_T*          pt_db,
                                         REC_UTIL_TIMER_LINK_T** ppt_timer_link)
{
    REC_UTIL_TIMER_LINK_T*      pt_link_new;
    INT32                       i4_ret;
    
    *ppt_timer_link = NULL;

    if (!DLIST_IS_EMPTY (&pt_db->t_timer_list_free))
    {
        pt_link_new = DLIST_HEAD (&pt_db->t_timer_list_free);
        DLIST_REMOVE (pt_link_new, &pt_db->t_timer_list_free, t_link);
    }
    else
    {
        pt_link_new = NULL;
        
        do 
        {
            pt_link_new = (REC_UTIL_TIMER_LINK_T *) x_mem_alloc (sizeof (REC_UTIL_TIMER_LINK_T));
            if (pt_link_new == NULL)
            {
                i4_ret = REC_UTILR_OUT_OF_MEMORY;
                break;
            }
            x_memset ((VOID *) pt_link_new, 0, sizeof (REC_UTIL_TIMER_LINK_T));
            
            i4_ret = x_timer_create (& pt_link_new->h_timer);
            if (i4_ret != OSR_OK)
            {
                i4_ret = REC_UTILR_OTHER_MODULE_FAIL;
                break;
            }
            
            pt_link_new->b_active = FALSE;
            
            i4_ret = REC_UTILR_OK;
        } while (0);
        
        if (i4_ret != REC_UTILR_OK)
        {
            if (pt_link_new)
            {
                if (pt_link_new->h_timer != NULL_HANDLE)
                {
                    x_timer_delete (pt_link_new->h_timer);
                }
                
                x_mem_free ((VOID *) pt_link_new);
            }
            
            return i4_ret;
        }
    }
    
    DLIST_INSERT_TAIL (pt_link_new, & pt_db->t_timer_list_alloc, t_link);
    *ppt_timer_link = pt_link_new;
    
    return REC_UTILR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _rec_util_free_timer_link
 *
 * Description: This API free a timer link.
 *
 * Inputs:  pt_db           Contains the DB object
 *          pt_timer_link   Contains the timer link to be free
 *
 * Outputs: -
 *
 * Returns: REC_UTILR_OK                Sucess
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_free_timer_link (REC_UTIL_DB_T*          pt_db,
                                             REC_UTIL_TIMER_LINK_T*   pt_timer_link)
{
    REC_UTIL_TIMER_LINK_T*  pt_timer_iter = NULL;
    BOOL                    b_found       = FALSE;

    if ((!pt_db) || (!pt_timer_link))
    {
        return REC_UTILR_INV_ARG;
    }
    
    if (pt_timer_link->b_active)
    {
        /* this case should not happen */
        REC_UTIL_DBG_ERR (("Internal error at L%d. \n", __LINE__));
        x_timer_stop (pt_timer_link->h_timer);
        pt_timer_link->b_active = FALSE;
    }
    

    DLIST_FOR_EACH(pt_timer_iter, &(pt_db->t_timer_list_alloc), t_link) 
    {
        if (pt_timer_iter == pt_timer_link)
        {
            DLIST_REMOVE (pt_timer_link, &pt_db->t_timer_list_alloc, t_link);
            b_found = TRUE;
            break;
        }
    }


    if (b_found)
    {
        DLIST_INSERT_TAIL (pt_timer_link, &pt_db->t_timer_list_free, t_link);
    }
    else
    {
        REC_UTIL_DBG_ERR (("Not find timer link(0x%08x) at L%d. \n", (UINT32)pt_timer_link,  __LINE__));
    }
    
    return REC_UTILR_OK;
}
        
/*-----------------------------------------------------------------------------
 * Name: _rec_util_free_db
 *
 * Description: This API free db object.
 *              
 * Inputs:  pt_db   Specify the db object.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess
 *          REC_UTILR_INTERNAL_ERROR    Fail.
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_free_db(REC_UTIL_DB_T*  pt_db)
{
    INT32                       i4_ret;
    UINT8                       ui1_tmp;
    REC_UTIL_REQ_LINK_T*        pt_req_link    = NULL;
    REC_UTIL_SET_CFG_REQ_T*     pt_set_cfg_req = NULL;
    REC_UTIL_TIMER_LINK_T*      pt_timer_link;

    /*free db object*/
    if (NULL == pt_db)
    {
        return REC_UTILR_OK;
    }
    switch(pt_db->e_profile)
    {
        case REC_UTIL_DB_PROFILE_UNLIMIT:
        {
                            /*free reqs from pending_req_list*/
            while (!DLIST_IS_EMPTY(&pt_db->t_pending_req_list))
            {
                pt_req_link = DLIST_HEAD(&pt_db->t_pending_req_list);
                DLIST_REMOVE(pt_req_link, &pt_db->t_pending_req_list, t_link);
                x_mem_free(pt_req_link);
            }
            
            /*free reqs from running_req_list*/
            while (!DLIST_IS_EMPTY(&pt_db->t_running_req_list))
            {
                pt_req_link = DLIST_HEAD(&pt_db->t_running_req_list);
                /*abort this requst, no need wait??*/
                if (REC_UTIL_REQ_FM_WRITE == pt_req_link->t_rec_util_req.e_req_type)
                {
                    pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = 
                        REC_UTIL_REQR_ABORTED;
                    _rec_util_abort_fm_async(pt_req_link);
                }
                else if (REC_UTIL_REQ_SET_CFG == pt_req_link->t_rec_util_req.e_req_type)
                {
                    /*free pt_cfg*/
                    pt_set_cfg_req = &(pt_req_link->t_rec_util_req.u_data.t_set_cfg_req);
                    if (NULL != pt_set_cfg_req->pt_cfg)
                    {
                        if (NULL != (pt_set_cfg_req->pt_cfg->t_strg_desc.ps_strg_path))
                        {
                            x_mem_free((VOID*)(pt_set_cfg_req->pt_cfg->t_strg_desc.ps_strg_path));
                            pt_set_cfg_req->pt_cfg->t_strg_desc.ps_strg_path = NULL;
                        }
                        x_mem_free(pt_set_cfg_req->pt_cfg);
                        pt_set_cfg_req->pt_cfg = NULL;
                    }
                }
                else
                {
                    REC_UTIL_DBG_INFO(("_rec_util_free_db: not support now!\r\n"));
                }
                DLIST_REMOVE(pt_req_link, &pt_db->t_running_req_list, t_link);
                x_mem_free(pt_req_link);
            }
            
            /*remove reqs from free_req_list*/
            while (!DLIST_IS_EMPTY(&pt_db->t_free_req_list))
            {
                pt_req_link = DLIST_HEAD(&pt_db->t_free_req_list);
                DLIST_REMOVE(pt_req_link, &pt_db->t_free_req_list, t_link);
                x_mem_free(pt_req_link);
            }
            
            /*check if t_timer_list_alloc is empty*/
            if (!DLIST_IS_EMPTY (&pt_db->t_timer_list_alloc))
            {
                /* this case should not happen */
                REC_UTIL_DBG_ERR (("Internal error at L%d. \n", __LINE__));
                
                while (!DLIST_IS_EMPTY (&pt_db->t_timer_list_alloc))
                {
                    pt_timer_link = DLIST_HEAD (&pt_db->t_timer_list_alloc);
                    _rec_util_free_timer_link (pt_db, pt_timer_link);
                }
            }
            
            /*remove timer links from t_timer_list_free*/
            while (!DLIST_IS_EMPTY (&pt_db->t_timer_list_free))
            {
                pt_timer_link = DLIST_HEAD (&pt_db->t_timer_list_free);
                DLIST_REMOVE (pt_timer_link, &pt_db->t_timer_list_free, t_link);
                x_timer_delete (pt_timer_link->h_timer);
                x_mem_free ((VOID *) pt_timer_link);
            }
            
            /*free strg path*/
            if (NULL != pt_db->t_cfg.t_strg_desc.ps_strg_path)
            {
                x_mem_free((VOID*)(pt_db->t_cfg.t_strg_desc.ps_strg_path));
                pt_db->t_cfg.t_strg_desc.ps_strg_path = NULL;
            }

            /*create tick idx tick detail and other info tbl*/
            if(REC_UTIL_IDX_TBL_OPT_DISABLE != pt_db->t_cfg.e_idx_tbl_opt)
            {
                /*free tick idx tbl*/
                if (NULL_HANDLE != pt_db->h_pvr_tick_idx_tbl)
                {
                    rec_util_rque_delete(pt_db->h_pvr_tick_idx_tbl);
                    pt_db->h_pvr_tick_idx_tbl = NULL_HANDLE;
                }
                
                /*free tick detail ring buffer*/
                if (REC_UTIL_GRB_NULL_HANDLE != pt_db->h_pvr_grb_detail)
                {
                    rec_util_grb_deinit(pt_db->h_pvr_grb_detail);
                    pt_db->h_pvr_grb_detail = REC_UTIL_GRB_NULL_HANDLE;
                }
                
                /*free info ring buffer*/
                for (ui1_tmp = REC_DB_INFO_FIRST_KEY_TYPE; ui1_tmp < REC_DB_INFO_TYPE_MAX; ++ui1_tmp)
                {
                    if (REC_UTIL_GRB_NULL_HANDLE != pt_db->at_info_grb_tbl[ui1_tmp])
                    {
                        rec_util_grb_deinit(pt_db->at_info_grb_tbl[ui1_tmp]);
                        pt_db->at_info_grb_tbl[ui1_tmp] = REC_UTIL_GRB_NULL_HANDLE;
                    }
                }

                /*free signal change ring buffer*/
                for (ui1_tmp = SIGNAL_FROM_Y_TO_N; ui1_tmp < SIGNAL_CHG_TYPE_MAX; ++ui1_tmp)
                {
                    if (REC_UTIL_GRB_NULL_HANDLE != pt_db->at_signal_grb_chg_tbl[ui1_tmp])
                    {
                        rec_util_grb_deinit(pt_db->at_signal_grb_chg_tbl[ui1_tmp]);
                        pt_db->at_signal_grb_chg_tbl[ui1_tmp] = REC_UTIL_GRB_NULL_HANDLE;
                    }
                }
            }
            
            /*free align buffer*/
            if (NULL != pt_db->pv_align_buf_addr)
            {
                x_mem_free(pt_db->pv_align_buf_addr);
                pt_db->pv_align_buf_addr = NULL;
                pt_db->pui1_align_buffer = NULL;
            }
            
            if (NULL_HANDLE != pt_db->h_file)
            {
                i4_ret = x_fm_mfw_close(pt_db->h_file);
                if(FMR_OK != i4_ret)
                {
                    REC_UTIL_DBG_ERR(("_rec_util_free_db: close file fail!\r\n"));
                    return REC_UTILR_INV_HANDLE;
                }
            }
                    
                    break;
        }
        case REC_UTIL_DB_PROFILE_LIMIT_1 :
            break;
        default:
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);

    
    }
    if(pt_db->s_head_file)
    {
        x_mem_free(pt_db->s_head_file);
    }
    i4_ret = rec_util_meta_rfi_deinit(&pt_db->t_ctx_rfi);
    if(i4_ret != REC_UTILR_OK)
    {
        REC_UTIL_DBG_ERR(("Deinit rfi context failed %d\n",i4_ret));
    }    
    x_memset(pt_db, 0, sizeof (REC_UTIL_DB_T));
    
    x_mem_free(pt_db);
    
    return REC_UTILR_OK;
}

  /*-----------------------------------------------------------------------------
 * Name: _rec_util_free_db
 *
 * Description: This API free db object and db head object.
 *              
 * Inputs:  pt_db_head   Specify the db head object.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess
 *          REC_UTILR_INTERNAL_ERROR    Fail.
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_free_db_head(REC_UTIL_DB_HEAD_T*  pt_db_head)
{
    INT32                   i4_ret = REC_UTILR_OK;
    REC_UTIL_DB_T*          pt_db = NULL;

    if (NULL == pt_db_head)
    {
        REC_UTIL_DBG_ERR(("_rec_util_free_db_head: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    /*free db object*/
    pt_db = pt_db_head->pt_db_obj;
    if (NULL != pt_db)
    {
        i4_ret = _rec_util_free_db(pt_db);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("_rec_util_free_db_head: free db fail!\r\n"));
        }
    }
    
    /*free db head*/
    i4_ret = _rec_util_remove_db_head_from_list(pt_db_head);
    if (REC_UTILR_OK != i4_ret)
    {
        REC_UTIL_DBG_ERR(("_rec_util_free_db_head: remove db head obj in list fail!\r\n"));
    }
    if(pt_db_head->s_db_name)
    {
        x_mem_free(pt_db_head->s_db_name);
        pt_db_head->s_db_name = NULL;
    }
    handle_free(pt_db_head->h_rec_db_head, TRUE);
    
    return REC_UTILR_OK;
}

  /*-----------------------------------------------------------------------------
 * Name: _rec_util_free_db
 *
 * Description: This API convert the evctx key type to rec util info type.
 *              
 * Inputs:  e_key_type   Specify the info's evctx key type.
 *          
 * Outputs: -
 *
 * Returns: REC_UTIL_INFO_TYPE_T           rec util info type.
 ----------------------------------------------------------------------------*/
static REC_UTIL_INFO_TYPE_T _rec_util_covert_evctx_key_type(
#ifdef MW_EDB_SUPPORT
                          EDB_KEY_TYPE_T                  e_key_type
#else                    
                          EVCTX_KEY_TYPE_T                e_key_type
#endif                    

)
{
    REC_UTIL_INFO_TYPE_T e_info_type = REC_DB_INFO_TYPE_MAX;
        
#ifndef MW_EDB_SUPPORT        
    switch (e_key_type)
    {
        case EVCTX_KEY_TYPE_START_TIME:
            e_info_type = REC_DB_INFO_TYPE_START_TIME;
            break;
        case EVCTX_KEY_TYPE_DURATION:
            e_info_type = REC_DB_INFO_TYPE_DURATION;
            break;
        case EVCTX_KEY_TYPE_EVENT_TITLE:
            e_info_type = REC_DB_INFO_TYPE_EVENT_TITLE;
            break;
        case EVCTX_KEY_TYPE_EVENT_TEXT:
            e_info_type = REC_DB_INFO_TYPE_EVENT_TEXT;
            break;
        case EVCTX_KEY_TYPE_SVC_NAME:
            e_info_type = REC_DB_INFO_TYPE_SVC_NAME;
            break;
        case EVCTX_KEY_TYPE_SVC_TEXT:
            e_info_type = REC_DB_INFO_TYPE_SVC_TEXT;
            break;
        case EVCTX_KEY_TYPE_RATING:
            e_info_type = REC_DB_INFO_TYPE_RATING;
            break;
        case EVCTX_KEY_TYPE_EVENT_EXT_TEXT:
            e_info_type = REC_DB_INFO_TYPE_EVENT_EXT_TEXT;
            break;
        case EVCTX_KEY_TYPE_ELAPSED_TIME:
            e_info_type = REC_DB_INFO_TYPE_ELAPSED_TIME;
            break;
        default:
            REC_UTIL_DBG_INFO(("_rec_util_covert_evctx_key_type: not support key type!\r\n"));
            break;
    }
#else
	switch (e_key_type)
	{
		case EDB_KEY_TYPE_START_TIME:
			e_info_type = REC_DB_INFO_TYPE_START_TIME;
			break;
		case EDB_KEY_TYPE_DURATION:
			e_info_type = REC_DB_INFO_TYPE_DURATION;
			break;
		case EDB_KEY_TYPE_EVENT_TITLE_BY_ID:
			e_info_type = REC_DB_INFO_TYPE_EVENT_TITLE_BY_ID;
			break;
		case EDB_KEY_TYPE_EVENT_DETAIL_BY_ID:
			e_info_type = REC_DB_INFO_TYPE_EVENT_DETAIL_BY_ID;
			break;
		case EDB_KEY_TYPE_RATING_LIST_BY_ID:
			e_info_type = REC_DB_INFO_TYPE_RATING;
			break;
		default:
			REC_UTIL_DBG_INFO(("_rec_util_covert_evctx_key_type: not support key type!\r\n"));
			break;
	}
#endif
    return e_info_type;
}

  /*-----------------------------------------------------------------------------
 * Name: _rec_util_compare_rating_info
 *
 * Description: This API compare the two rating info is same or not.
 *              
 * Inputs:  pv_info         Specify the info stored in rec util db obj.
 *          pv_new_info     Specify the new info.
 *          e_brdcst_type   Specify the brdcst type.
 *          
 * Outputs: -
 *
 * Returns: TRUE            The two info is different.
 *          FALSE           The two info is same.
 ----------------------------------------------------------------------------*/
static BOOL _rec_util_compare_rating_info(VOID*                       pv_info,
                                          VOID*                       pv_new_info,
                                          BRDCST_TYPE_T               e_brdcst_type)
{
    EVCTX_ATSC_RATING_LIST_T*   pt_atsc_rating_list = NULL;
    EVCTX_DVB_RATING_LIST_T*    pt_dvb_rating_list = NULL;
    EVCTX_ATSC_RATING_LIST_T*   pt_new_atsc_rating_list = NULL;
    EVCTX_DVB_RATING_LIST_T*    pt_new_dvb_rating_list = NULL;
	EVCTX_ISDB_RATING_LIST_T*   pt_isdb_rating_list     = NULL;
    EVCTX_ISDB_RATING_LIST_T*   pt_new_isdb_rating_list = NULL;
    UINT16                      ui2_idx;

    /*atsc type*/
    if (BRDCST_TYPE_ATSC == e_brdcst_type)
    {
        pt_atsc_rating_list     = (EVCTX_ATSC_RATING_LIST_T*)pv_info;
        pt_new_atsc_rating_list = (EVCTX_ATSC_RATING_LIST_T*)pv_new_info;
        while (NULL != pt_atsc_rating_list)
        {
            if (NULL == pt_new_atsc_rating_list)
            {
                return TRUE;
            }
            
            if ((pt_atsc_rating_list->ui2_rating_region != pt_new_atsc_rating_list->ui2_rating_region)
                 || (pt_atsc_rating_list->ui2_rated_dimensions != pt_new_atsc_rating_list->ui2_rated_dimensions))
            {
                return TRUE;
            }
            
            if (x_strlen(pt_atsc_rating_list->ps_rating_descrption) 
                 != x_strlen(pt_new_atsc_rating_list->ps_rating_descrption))
            {
                return TRUE;
            }
            
            if (x_memcmp(pt_atsc_rating_list->ps_rating_descrption, 
                         pt_new_atsc_rating_list->ps_rating_descrption,
                         x_strlen(pt_atsc_rating_list->ps_rating_descrption)) != 0)
            {
                return TRUE;
            }
            
            for (ui2_idx=0; ui2_idx < pt_atsc_rating_list->ui2_rated_dimensions; ui2_idx++)
            {
                if (pt_atsc_rating_list->pt_ratings[ui2_idx].ui2_rating_value != 
                    pt_new_atsc_rating_list->pt_ratings[ui2_idx].ui2_rating_value)
                {
                    return TRUE;
                }
                if (pt_atsc_rating_list->pt_ratings[ui2_idx].ui2_dimension !=
                    pt_new_atsc_rating_list->pt_ratings[ui2_idx].ui2_dimension)
                {
                    return TRUE;
                }
            }
            pt_atsc_rating_list = pt_atsc_rating_list->pt_next;
            pt_new_atsc_rating_list = pt_new_atsc_rating_list->pt_next;
        }
        
        if (NULL != pt_new_atsc_rating_list)
        {
            return TRUE;
        }
    }
    /*dvb type*/
    else if(BRDCST_TYPE_DVB == e_brdcst_type ||
            BRDCST_TYPE_DTMB == e_brdcst_type)
    {
        pt_dvb_rating_list = (EVCTX_DVB_RATING_LIST_T*)pv_info;
        pt_new_dvb_rating_list = (EVCTX_DVB_RATING_LIST_T*)pv_new_info;
        while (NULL != pt_dvb_rating_list)
        {
            if (NULL == pt_new_dvb_rating_list)
            {
                return TRUE;
            }
            
            if ((pt_dvb_rating_list->ui2_rating_region != pt_new_dvb_rating_list->ui2_rating_region) ||
                (pt_dvb_rating_list->ui2_rated_dimensions != pt_new_dvb_rating_list->ui2_rated_dimensions))
            {
                return TRUE;
            }
            
            /*check dvb rating*/
            if ((pt_dvb_rating_list->t_rating.ui2_rating_value != 
                 pt_new_dvb_rating_list->t_rating.ui2_rating_value) 
                 || (pt_dvb_rating_list->t_rating.ui2_dimension != 
                     pt_new_dvb_rating_list->t_rating.ui2_dimension))
            {
                return TRUE;
            }
            
            if (x_memcmp(pt_dvb_rating_list->t_country_code, 
                         pt_new_dvb_rating_list->t_country_code,
                         ISO_3166_COUNT_LEN) != 0)  
            {
                return TRUE;
            }
            
            pt_dvb_rating_list = pt_dvb_rating_list->pt_next;
            pt_new_dvb_rating_list = pt_new_dvb_rating_list->pt_next;
        }
        
        if (NULL != pt_new_dvb_rating_list)
        {
            return TRUE;
        }
    }
	else if (BRDCST_TYPE_ISDB == e_brdcst_type)
	{
		pt_isdb_rating_list = (EVCTX_ISDB_RATING_LIST_T*)pv_info;
        pt_new_isdb_rating_list = (EVCTX_ISDB_RATING_LIST_T*)pv_new_info;
        while (NULL != pt_isdb_rating_list)
        {
            if (NULL == pt_new_isdb_rating_list)
            {
                return TRUE;
            }
            
            if ((pt_isdb_rating_list->ui2_rating_region != pt_new_isdb_rating_list->ui2_rating_region) ||
                (pt_isdb_rating_list->ui2_rated_dimensions != pt_new_isdb_rating_list->ui2_rated_dimensions))
            {
                return TRUE;
            }
            
            /*check isdb rating*/
            if ((pt_isdb_rating_list->t_rating.ui2_rating_value != 
                 pt_new_isdb_rating_list->t_rating.ui2_rating_value) 
                 || (pt_isdb_rating_list->t_rating.ui2_dimension != 
                     pt_new_isdb_rating_list->t_rating.ui2_dimension))
            {
                return TRUE;
            }
            
            if (x_memcmp(pt_isdb_rating_list->t_country_code, 
                         pt_new_isdb_rating_list->t_country_code,
                         ISO_3166_COUNT_LEN) != 0)  
            {
                return TRUE;
            }
            
            pt_isdb_rating_list = pt_isdb_rating_list->pt_next;
            pt_new_isdb_rating_list = pt_new_isdb_rating_list->pt_next;
        }
        
        if (NULL != pt_new_isdb_rating_list)
        {
            return TRUE;
        }
	}
    else/*unknown or unsupport type*/
    {
        REC_UTIL_DBG_ERR(("_rec_util_compare_rating_info: not support brdcst type!\r\n"));
        return FALSE;/*return equal, so not need copy*/
    }
    
    return FALSE;
}
  
  /*-----------------------------------------------------------------------------
 * Name: _rec_util_check_info_update
 *
 * Description: This API check info is update or not.
 *              
 * Inputs:  pt_info_data    Specify the info stored in rec util db obj.
 *          pt_update_info  Specify the new info.
 *          e_brdcst_type   Specify the brdcst type.
 *          
 * Outputs: -
 *
 * Returns: TRUE            The info is updated.
 *          FALSE           The info is same as before.
 ----------------------------------------------------------------------------*/
static BOOL _rec_util_check_info_update(
      REC_UTIL_INFO_DATA_T*         pt_info_data, 
      REC_UTIL_UPDATE_INFO_T*       pt_update_info,
      BRDCST_TYPE_T                 e_brdcst_type)
{
      BOOL                          b_update = TRUE;
      VOID*                       pv_info = (VOID*)pt_info_data;
      
      if ((NULL == pt_info_data) || (NULL == pt_update_info))
      {
          return FALSE;
      }
      if (pt_info_data->z_info_len != pt_update_info->z_info_len)
      {
          return b_update;
      }
      else if (0 == pt_update_info->z_info_len)
      {/*update info is NULL*/
          return FALSE;
      }
      pv_info = (VOID*)((UINT8*)pv_info+sizeof(SIZE_T));
      switch (pt_update_info->e_info_type)
      {
      case REC_DB_INFO_TYPE_START_TIME:/* TIME_T */
      case REC_DB_INFO_TYPE_DURATION:
#ifndef MW_EDB_SUPPORT
      case REC_DB_INFO_TYPE_ELAPSED_TIME:
      case REC_DB_INFO_TYPE_EVENT_TITLE:/* UTF-8 string */
      case REC_DB_INFO_TYPE_EVENT_TEXT:
      case REC_DB_INFO_TYPE_SVC_NAME:
      case REC_DB_INFO_TYPE_SVC_TEXT:
      case REC_DB_INFO_TYPE_EVENT_EXT_TEXT:
#else 
	  case REC_DB_INFO_TYPE_EVENT_TITLE_BY_ID:
	  case REC_DB_INFO_TYPE_EVENT_DETAIL_BY_ID:
#endif
           if (0 == x_memcmp(pv_info, 
                             pt_update_info->pv_info, 
                             pt_info_data->z_info_len))
            {/*info is same*/
                b_update = FALSE;
            }
            break;
      case REC_DB_INFO_TYPE_RATING:
            b_update = _rec_util_compare_rating_info(
                                  pv_info, 
                                  pt_update_info->pv_info, 
                                  e_brdcst_type);
            break;
      case REC_DB_INFO_TYPE_SCDB_INFO:
      case REC_DB_INFO_TYPE_RRT_INFO:
            if (0 == x_memcmp(pv_info, 
                              pt_update_info->pv_info, 
                              pt_info_data->z_info_len))
            {/*info is same*/
                b_update = FALSE;
            }
            break;
      default:
            REC_UTIL_DBG_ERR(("_rec_util_check_info_update: error key type!\r\n"));
            break;
       }
       return b_update;
}


/*-----------------------------------------------------------------------------
 * Name: _rec_util_copy_rating_info
 *
 * Description: This API copy rating info to db.
 *              
 * Inputs:  pv_buf          Specify the db buf which will the rating info be stored.
 *          pv_info         Specify the rating info to be copyed.
 *          z_info_len      Specify the rating info length
 *          e_brdcst_type   Specify the brdcst type.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess
 *          REC_UTILR_INTERNAL_ERROR    Fail.
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_copy_rating_info(VOID*                   pv_buf, 
                                        VOID*                   pv_info,
                                        SIZE_T                  z_info_len,
                                        BRDCST_TYPE_T           e_brdcst_type)
{
    EVCTX_ATSC_RATING_LIST_T*   pt_dst_atsc_rating_list = NULL;
    EVCTX_DVB_RATING_LIST_T*    pt_dst_dvb_rating_list = NULL;
    EVCTX_ATSC_RATING_LIST_T*   pt_src_atsc_rating_list = NULL;
    EVCTX_DVB_RATING_LIST_T*    pt_src_dvb_rating_list = NULL;
	EVCTX_ISDB_RATING_LIST_T*   pt_dst_isdb_rating_list  = NULL;
    EVCTX_ISDB_RATING_LIST_T*   pt_src_isdb_rating_list  = NULL; 
    UINT16                      ui2_idx;
    UINT8*                      pui1_write_ptr;    
    EVCTX_ATSC_RATING_T*        pt_atsc_ratings = NULL;
    SIZE_T                      z_copy_data_len = 0;
    UINT32                      ui4_ptr_ofst;

    pui1_write_ptr = (UINT8*)pv_buf;
    /*atsc type*/
    if (BRDCST_TYPE_ATSC == e_brdcst_type)
    {
    
       /**************************************************************************       
               ui2_rating_region           <---
               ui2_rated_dimensions            | sizeof(EVCTX_ATSC_RATING_LIST_T)
       ------- ps_rating_description           |
       | ----- pt_ratings                      |
       | |  -- pt_next                     <---                   +
       | |  |
       | ----->  pt_ratings[0]                      <---
       |    |    pt_ratings[1]                          | ui2_rated_dimensions *
       |    |    pt_ratings[2]                          | sizeof(EVCTX_ATSC_RATING_T)
       |    |        :                                  |
       |    |    pt_ratings[ui2_rated_dimensions-1] <---          +
       --------> "TV-PG-L-S"                        <---  x_strlen(pt_rating_description)+1
            |
            |
            -> ui2_rating_region   (the next region)
               ui2_rated_dimensions
               pt_rating_description
               pt_ratings
               pt_next
       ***************************************************************************/
        pt_dst_atsc_rating_list = (EVCTX_ATSC_RATING_LIST_T*)pv_buf;
        pt_src_atsc_rating_list = (EVCTX_ATSC_RATING_LIST_T*)pv_info;
        while (NULL != pt_src_atsc_rating_list)
        {
            if (NULL == pt_dst_atsc_rating_list)
            {
                REC_UTIL_DBG_ERR(("_rec_util_copy_rating_info: dest atsc rating list is NULL!\r\n"));
                REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
                return REC_UTILR_INTERNAL_ERROR;
            }
            
            pt_dst_atsc_rating_list->ui2_rating_region    = 
                                     pt_src_atsc_rating_list->ui2_rating_region;
            pt_dst_atsc_rating_list->ui2_rated_dimensions = 
                                     pt_src_atsc_rating_list->ui2_rated_dimensions;

            pui1_write_ptr  += sizeof(EVCTX_ATSC_RATING_LIST_T);
            z_copy_data_len += sizeof(EVCTX_ATSC_RATING_LIST_T);
            pt_atsc_ratings  = (EVCTX_ATSC_RATING_T*)pui1_write_ptr;
            
            /*set pt_ratings ptr*/
            pt_dst_atsc_rating_list->pt_ratings = pt_atsc_ratings;
            for (ui2_idx = 0; ui2_idx < pt_dst_atsc_rating_list->ui2_rated_dimensions; ++ui2_idx)
            {
                pt_atsc_ratings->ui2_dimension    = 
                                pt_src_atsc_rating_list->pt_ratings[ui2_idx].ui2_dimension;
                pt_atsc_ratings->ui2_rating_value = 
                                pt_src_atsc_rating_list->pt_ratings[ui2_idx].ui2_rating_value;
                ++pt_atsc_ratings;
                z_copy_data_len += sizeof(EVCTX_ATSC_RATING_T);
            }
            
            pui1_write_ptr = (UINT8*)pt_atsc_ratings;/*end of rating tbl*/
            /*set ps_rating_description ptr*/
            if (0 != x_strlen(pt_src_atsc_rating_list->ps_rating_descrption))
            {
                pt_dst_atsc_rating_list->ps_rating_descrption = (CHAR*)pui1_write_ptr;
                x_memcpy(pt_dst_atsc_rating_list->ps_rating_descrption, 
                         pt_src_atsc_rating_list->ps_rating_descrption,
                         x_strlen(pt_src_atsc_rating_list->ps_rating_descrption) + 1);
                pui1_write_ptr += (x_strlen(pt_src_atsc_rating_list->ps_rating_descrption) + 1); /*end of descrip*/
                z_copy_data_len += x_strlen(pt_src_atsc_rating_list->ps_rating_descrption) + 1;
            }
            else
            {
                pt_dst_atsc_rating_list->ps_rating_descrption = NULL;
            }
            
            /*get next rating_list*/
            ui4_ptr_ofst = (UINT32)pt_src_atsc_rating_list->pt_next -
                            (UINT32)pt_src_atsc_rating_list;
                            
            pt_src_atsc_rating_list = pt_src_atsc_rating_list->pt_next;
            if (NULL != pt_src_atsc_rating_list)
            {   
                /*has next list*/
                pt_dst_atsc_rating_list->pt_next = 
                    (EVCTX_ATSC_RATING_LIST_T*)((UINT32)pt_dst_atsc_rating_list 
                                                + ui4_ptr_ofst);   
                pt_dst_atsc_rating_list = pt_dst_atsc_rating_list->pt_next;
                pui1_write_ptr = (UINT8*)pt_dst_atsc_rating_list;                                
            }
            else
            {
                /*the last one*/
                pt_dst_atsc_rating_list->pt_next = NULL;
                pt_dst_atsc_rating_list = pt_dst_atsc_rating_list->pt_next;
            }
            
            
        }
    }
    /*dvb type*/
    else if(BRDCST_TYPE_DVB == e_brdcst_type ||
            BRDCST_TYPE_DTMB == e_brdcst_type)
    {
        /*******************************************************************************
                ui2_rating_region           <---
                ui2_rated_dimensions            |
         ------ ps_rating_description           | sizeof(EVCTX_DVB_RATING_LIST_T)
        |  ---- pt_ratings                      |
        |  |    pt_next -> NULL (for compatible with generic rating structure)
        |  |                                    |
        |  -----> pt_ratings[0]                 |
         -----> "GBR"                       <---  (ISO_3166_COUNT_LEN)
        ********************************************************************************/
        pt_dst_dvb_rating_list = (EVCTX_DVB_RATING_LIST_T*)pv_buf;
        pt_src_dvb_rating_list = (EVCTX_DVB_RATING_LIST_T*)pv_info;
        if (NULL == pt_dst_dvb_rating_list)
        {
            REC_UTIL_DBG_ERR(("_rec_util_copy_rating_info: dest dvb rating list is NULL!\r\n"));
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
            return REC_UTILR_INTERNAL_ERROR;
        }
        pt_dst_dvb_rating_list->ui2_rating_region    = 
                                pt_src_dvb_rating_list->ui2_rating_region;
        pt_dst_dvb_rating_list->ui2_rated_dimensions = 
                                pt_src_dvb_rating_list->ui2_rated_dimensions;
        pt_dst_dvb_rating_list->ps_country_code      = 
                                (CHAR*)&(pt_dst_dvb_rating_list->t_country_code);
        pt_dst_dvb_rating_list->pt_ratings = &(pt_dst_dvb_rating_list->t_rating);
        if (NULL != pt_src_dvb_rating_list->pt_next)
        {
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
        }
        pt_dst_dvb_rating_list->pt_next = NULL;
        
        
        pt_dst_dvb_rating_list->t_rating.ui2_rating_value = 
                               pt_src_dvb_rating_list->t_rating.ui2_rating_value;
        pt_dst_dvb_rating_list->t_rating.ui2_dimension    = 
                               pt_src_dvb_rating_list->t_rating.ui2_dimension;
        
        x_memcpy(pt_dst_dvb_rating_list->t_country_code, 
                 pt_src_dvb_rating_list->t_country_code,
                 ISO_3166_COUNT_LEN);
        
        pui1_write_ptr += sizeof(EVCTX_DVB_RATING_LIST_T);
        z_copy_data_len += sizeof(EVCTX_DVB_RATING_LIST_T);
    }
	else if(BRDCST_TYPE_ISDB == e_brdcst_type)
	{
		/*******************************************************************************
                ui2_rating_region           <---
                ui2_rated_dimensions            |
         ------ ps_rating_description           | sizeof(EVCTX_ISDB_RATING_LIST_T)
        |  ---- pt_ratings                      |
        |  |    pt_next -> NULL (for compatible with generic rating structure)
        |  |                                    |
        |  -----> pt_ratings[0]                 |
         -----> "GBR"                       <---  (ISO_3166_COUNT_LEN)
        ********************************************************************************/
        pt_dst_isdb_rating_list = (EVCTX_ISDB_RATING_LIST_T*)pv_buf;
        pt_src_isdb_rating_list = (EVCTX_ISDB_RATING_LIST_T*)pv_info;
        if (NULL == pt_dst_isdb_rating_list)
        {
            REC_UTIL_DBG_ERR(("_rec_util_copy_rating_info: dest dvb rating list is NULL!\r\n"));
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
            return REC_UTILR_INTERNAL_ERROR;
        }
        pt_dst_isdb_rating_list->ui2_rating_region    = 
                                pt_src_isdb_rating_list->ui2_rating_region;
        pt_dst_isdb_rating_list->ui2_rated_dimensions = 
                                pt_src_isdb_rating_list->ui2_rated_dimensions;
        pt_dst_isdb_rating_list->ps_country_code      = 
                                (CHAR*)&(pt_src_isdb_rating_list->t_country_code);
        pt_dst_isdb_rating_list->pt_ratings = &(pt_src_isdb_rating_list->t_rating);
        if (NULL != pt_src_isdb_rating_list->pt_next)
        {
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
        }
        pt_dst_isdb_rating_list->pt_next = NULL;
        
        
        pt_dst_isdb_rating_list->t_rating.ui2_rating_value = 
                               pt_src_isdb_rating_list->t_rating.ui2_rating_value;
        pt_dst_isdb_rating_list->t_rating.ui2_dimension    = 
                               pt_src_isdb_rating_list->t_rating.ui2_dimension;
        
        x_memcpy(pt_dst_isdb_rating_list->t_country_code, 
                 pt_src_isdb_rating_list->t_country_code,
                 ISO_3166_COUNT_LEN);
        
        pui1_write_ptr += sizeof(EVCTX_ISDB_RATING_LIST_T);
        z_copy_data_len += sizeof(EVCTX_ISDB_RATING_LIST_T);
	}
    else
    {
        /*report err and abort*/
        REC_UTIL_DBG_ERR(("_rec_util_copy_rating_info: doesn't support e_brdcst_type!\r\n"));
        REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
    }
    
    if ((UINT32)pui1_write_ptr > (UINT32)pv_buf+ z_info_len)
    {
        REC_UTIL_DBG_ERR(("_rec_util_copy_rating_info: error size!\r\n"));
        REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
        return REC_UTILR_INTERNAL_ERROR;
    }
    
    return REC_UTILR_OK;
}
  /*-----------------------------------------------------------------------------
 * Name: _rec_util_copy_info
 *
 * Description: This API copy info to db.
 *              
 * Inputs:  pv_buf          Specify the db buf which will the info be stored.
 *          pv_info         Specify the info to be copyed.
 *          pt_info_key     Specify the info key.
 *          z_info_len      Specify the info length
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess
 *          REC_UTILR_INTERNAL_ERROR    Fail.
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_copy_info(VOID*                   pv_buf, 
                                 VOID*                   pv_info,
                                 REC_UTIL_INFO_KEY_T*    pt_info_key,
                                 SIZE_T                  z_info_len)
{
    INT32                       i4_ret = REC_UTILR_OK;
    REC_UTIL_INFO_TYPE_T        e_info_type;
    BRDCST_TYPE_T               e_brdcst_type;

    if ((NULL == pv_buf) || (NULL == pt_info_key))
    {
        REC_UTIL_DBG_ERR(("_rec_util_copy_info: invalid argument!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    if (0 == z_info_len)
    {
        return REC_UTILR_OK;
    }
    e_info_type = pt_info_key->e_info_type;
    e_brdcst_type = pt_info_key->e_brdcst_type;
    switch (e_info_type)
    {
        case REC_DB_INFO_TYPE_START_TIME: /* TIME_T */
        case REC_DB_INFO_TYPE_DURATION:
#ifndef MW_EDB_SUPPORT 
        case REC_DB_INFO_TYPE_ELAPSED_TIME:
            x_memcpy(pv_buf, pv_info, z_info_len);
            break;
        case REC_DB_INFO_TYPE_EVENT_TITLE:/*UTF-8 string*/
        case REC_DB_INFO_TYPE_EVENT_TEXT:
        case REC_DB_INFO_TYPE_SVC_NAME:
        case REC_DB_INFO_TYPE_SVC_TEXT:
        case REC_DB_INFO_TYPE_EVENT_EXT_TEXT:
#else   
		case REC_DB_INFO_TYPE_EVENT_TITLE_BY_ID:
		case REC_DB_INFO_TYPE_EVENT_DETAIL_BY_ID:
#endif
            x_memcpy(pv_buf, pv_info, z_info_len);
            break;
        case REC_DB_INFO_TYPE_RATING:
            i4_ret = _rec_util_copy_rating_info(pv_buf, pv_info, z_info_len, e_brdcst_type);
            break;
        case REC_DB_INFO_TYPE_SCDB_INFO:
        case REC_DB_INFO_TYPE_RRT_INFO:
            x_memcpy(pv_buf, pv_info, z_info_len);
            break;
        default:
            REC_UTIL_DBG_ERR(("_rec_util_copy_info: error key type!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
    }
    return i4_ret;
}

 /*-----------------------------------------------------------------------------
 * Name: _rec_util_update_info
 *
 * Description: This API update info.
 *              
 * Inputs:  pt_db           Specify the db object.
 *          pt_update_info  Specify the update info.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess
 *          REC_UTILR_INTERNAL_ERROR    Fail.
 ----------------------------------------------------------------------------*/    
static INT32 _rec_util_update_info(REC_UTIL_DB_T*              pt_db,
                                   REC_UTIL_UPDATE_INFO_T*     pt_update_info)
{
    INT32                       i4_ret          = REC_UTILR_OK;
    REC_UTIL_INFO_KEY_T         t_info_key;
    REC_UTIL_INFO_TYPE_T        e_info_type;
    UINT32                      ui4_new_rec_len  = 0;    /*new size info*/
    BOOL                        b_info_update    = TRUE; /*info update check*/        
    REC_UTIL_GRB_DATA_T         t_data;
    REC_UTIL_INFO_DATA_T*       pt_last_info = NULL;
    UINT32                      ui4_write_ofst = 0;

    if ((NULL == pt_db) || (NULL == pt_update_info))
    {
        REC_UTIL_DBG_ERR(("_rec_util_update_info: invalid argument!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    e_info_type              = pt_update_info->e_info_type;
    t_info_key.e_info_type   = e_info_type;
    t_info_key.e_brdcst_type = pt_db->t_cfg.e_brdcst_type;


    i4_ret = rec_util_grb_get_tail_entry(pt_db->at_info_grb_tbl[e_info_type],
                                         &t_data);

    if ((i4_ret != REC_UTILR_GRB_OK) && (i4_ret != REC_UTILR_GRB_SEEK_ENTRY_FINISHED))
    {
        REC_UTIL_DBG_ERR(("_rec_util_update_info:get last info error(%d)!\r\n",
                          i4_ret));
        return REC_UTILR_INTERNAL_ERROR;
    }

    /*if not dvb or atsc type, and info type is rating*/
    if( (REC_DB_INFO_TYPE_RATING == e_info_type) &&
        (BRDCST_TYPE_DVB != t_info_key.e_brdcst_type)  &&
        (BRDCST_TYPE_DTMB != t_info_key.e_brdcst_type) &&
        (BRDCST_TYPE_ISDB != t_info_key.e_brdcst_type) &&
        (BRDCST_TYPE_ATSC != t_info_key.e_brdcst_type))
    {
        REC_UTIL_DBG_ERR((  "_rec_util_update_info:rating info not support for "
                            "brdcst type(%d)!\r\n", t_info_key.e_brdcst_type));
        return REC_UTILR_OK;
    }
    
    if (REC_UTIL_GRB_NULL_DATA != t_data.pv_tag)
    {
        pt_last_info = (REC_UTIL_INFO_DATA_T*)((UINT8*)(t_data.pv_data) + 4);
        /*check info is really update*/
        b_info_update = _rec_util_check_info_update(pt_last_info, 
                                                    pt_update_info,
                                                    t_info_key.e_brdcst_type);
        if (!b_info_update)
        {
            /*info is same as before, not need update info*/
            REC_UTIL_DBG_INFO(("_rec_util_update_info: same %u info at lba %u!\r\n",
                               e_info_type,
                               pt_update_info->ui4_lba));
            return REC_UTILR_OK;
        }
    }
    
    ui4_new_rec_len = pt_update_info->z_info_len + 8;
    
    /*allocate a entry to held this info*/
    i4_ret = rec_util_grb_alloc_entry(pt_db->at_info_grb_tbl[e_info_type],
                                       ui4_new_rec_len,
                                       &t_data);
    if (i4_ret != REC_UTILR_GRB_OK)
    {
       REC_UTIL_DBG_ERR(("_rec_util_update_info:alloc entry error(%d)!\r\n",
                         i4_ret));
       return REC_UTILR_INTERNAL_ERROR;
    }
    
    /*lba*/
    i4_ret = rec_util_grb_set_entry(pt_db->at_info_grb_tbl[e_info_type],
                                    &t_data, ui4_write_ofst, 
                                    (VOID*)(&pt_update_info->ui4_lba), 
                                    sizeof(UINT32));
    ui4_write_ofst += sizeof(UINT32);
    
    if (i4_ret != REC_UTILR_GRB_OK)
    {
       REC_UTIL_DBG_ERR(("_rec_util_update_info:set lba error(%d)!\r\n",
                         i4_ret));
       return REC_UTILR_INTERNAL_ERROR;
    }
    
    /*info length*/
    i4_ret = rec_util_grb_set_entry(pt_db->at_info_grb_tbl[e_info_type], 
                                    &t_data, ui4_write_ofst, 
                                    (VOID*)(&pt_update_info->z_info_len), 
                                    sizeof(UINT32));
    ui4_write_ofst += sizeof(UINT32);
    
    if (i4_ret != REC_UTILR_GRB_OK)
    {
       REC_UTIL_DBG_ERR(("_rec_util_update_info:set info length error(%d)!\r\n",
                         i4_ret));
       return REC_UTILR_INTERNAL_ERROR;
    }
    /*copy info*/
    i4_ret = _rec_util_copy_info((UINT8*)(t_data.pv_data) + ui4_write_ofst, 
                                 pt_update_info->pv_info, 
                                 &t_info_key, 
                                 pt_update_info->z_info_len);
    if (REC_UTILR_OK != i4_ret)
    {
        REC_UTIL_DBG_ERR(("_rec_util_update_info: _rec_util_copy_info %d fail!\r\n", e_info_type));
        return i4_ret;
    }
    
    return i4_ret;
}


  /*-----------------------------------------------------------------------------
 * Name: _rec_util_lba_in_query_range
 *
 * Description: This API check lba is in query range or not.
 *              
 * Inputs:  ui4_lba               Specifies the searched lba.
 *          ui4_lba_begin         Specifies the begin of the query range.
 *          ui4_lba_end           Specifies the end of the query range.
 *          ui4_avail_max_lba     Specifies availble max lba
 *          
 * Outputs: -
 *
 * Returns: TRUE            lba is in query range.
 *          FALSE           lba is not in query range.
 ----------------------------------------------------------------------------*/
static BOOL _rec_util_lba_in_query_range(UINT32      ui4_lba,
                                         UINT32      ui4_lba_begin,
                                         UINT32      ui4_lba_end,
                                         UINT32      ui4_avail_max_lba)
{    
    /*if equals start or end lba num*/
    if ((ui4_lba == ui4_lba_begin) || (ui4_lba == ui4_lba_end))
    {
        return TRUE;
    }

    /*end lba num is no less than start one*/
    if (ui4_lba_end >= ui4_lba_begin)
    {
        if ((ui4_lba > ui4_lba_begin) && (ui4_lba < ui4_lba_end))
        {
            return TRUE;
        }
    }
    else
    {
        if ((ui4_lba > ui4_lba_begin) && (ui4_lba <= ui4_avail_max_lba))
        {
            return TRUE;
        }
        else if (ui4_lba < ui4_lba_end)
        {
            return TRUE;
        }
        else
        {
            
        }
        
    }
    
    return FALSE;
}

  /*-----------------------------------------------------------------------------
 * Name: _rec_util_search_first_lba
 *
 * Description: This API search first lba from info table.
 *              
 * Inputs:  ui4_lba_begin   Specifies the begin of the query range.
 *          pt_db          Specifies the end of the query range.
 *          b_backward      Specifies if search from ui4_lba_end to ui4_lba_begin or not.
 *          
 * Outputs: pui1_first_lba  The first index in tbl which lba is equal to ui4_lba.
 *
 * Returns: TRUE            Found
 *          FALSE           Not found.
 ----------------------------------------------------------------------------*/
static BOOL _rec_util_search_first_lba(REC_UTIL_DB_T*         pt_db,
                                       REC_UTIL_GRB_HANDLE_T  h_grb,
                                       UINT32                 ui4_lba_begin,
                                       UINT32                 ui4_lba_end,
                                       BOOL                   b_backward,
                                       UINT32*                pui1_first_lba,
                                       REC_UTIL_GRB_DATA_T*   pt_data)
 {
    UINT32                          ui4_lba;
    BOOL                            b_lba_in_range = FALSE;
    BOOL                            b_ret  = FALSE;
    REC_UTIL_GRB_DATA_T             t_data;

    if ((NULL == pt_db) || (NULL == pui1_first_lba))
    {
        REC_UTIL_DBG_ERR(("_rec_util_search_first_lba: invalid argument!\r\n"));
        return FALSE;
    }
    
    t_data.pv_tag = REC_UTIL_GRB_NULL_DATA;
    *pui1_first_lba = 0;
    
    /*search from begin to end*/
    while(REC_UTILR_GRB_OK == rec_util_grb_seek_next_entry(h_grb,b_backward, &t_data))
    {
        ui4_lba = *((UINT32*)t_data.pv_data);
        b_lba_in_range = _rec_util_lba_in_query_range(ui4_lba, ui4_lba_begin, 
                                                      ui4_lba_end, pt_db->ui4_avail_max_lba);
        /*current out of range and next one in range (or next one doesn't exist)*/
        if (b_lba_in_range)
        {     
            *pui1_first_lba = ui4_lba;
            b_ret = TRUE;
            if(NULL != pt_data)
            {
                (*pt_data) = t_data;
            }
            break; 
        }
    }
    
    return b_ret;
}

 /*-----------------------------------------------------------------------------
  * Name: _rec_util_search_first_updated_lba
  *
  * Description: This API search the first updated lba from certain pos in info table.
  *              
  * Inputs:  ui4_lba_begin   Specifies the begin of the query range.
  *          pt_db          Specifies the end of the query range.
  *          b_backward      Specifies if search from ui4_lba_end to ui4_lba_begin or not.
  *          
  * Outputs: pui1_first_lba  The first index in tbl which lba is equal to ui4_lba.
  *
  * Returns: TRUE            Found
  *          FALSE           Not found.
  ----------------------------------------------------------------------------*/
 static BOOL _rec_util_search_first_updated_lba(REC_UTIL_DB_T*         pt_db,
                                        REC_UTIL_GRB_HANDLE_T  h_grb,
                                        UINT32                 ui4_lba_begin,
                                        UINT32                 ui4_lba_end,
                                        BOOL                   b_backward,
                                        UINT32*                pui1_first_lba)
  {
     UINT32                          ui4_lba = 0;
     BOOL                            b_lba_in_range = FALSE;
     BOOL                            b_ret  = FALSE;
     REC_UTIL_GRB_DATA_T             t_data;
 
     if ((NULL == pt_db) || (NULL == pui1_first_lba))
     {
         REC_UTIL_DBG_ERR(("_rec_util_search_first_updated_lba: invalid argument!\r\n"));
         return FALSE;
     }
     
     t_data.pv_tag = REC_UTIL_GRB_NULL_DATA;
     *pui1_first_lba = 0;
     
     /*search from begin to end*/
     while(REC_UTILR_GRB_OK == rec_util_grb_seek_next_entry(h_grb,b_backward, &t_data))
     {
         ui4_lba = *((UINT32*)t_data.pv_data);
         b_lba_in_range = _rec_util_lba_in_query_range(ui4_lba, ui4_lba_begin, 
                                                       ui4_lba_end, pt_db->ui4_avail_max_lba);
         /*current out of range and next one in range (or next one doesn't exist)*/
         if (b_lba_in_range)
         {     
             *pui1_first_lba = ui4_lba;
             b_ret = TRUE;
             break; 
         }
     }

     if(b_lba_in_range && b_backward)
     {
         if(ui4_lba == ui4_lba_begin)  
         {
            b_ret = FALSE;
         }
         else if(ui4_lba == 0)
         {
            *pui1_first_lba = pt_db->ui4_avail_max_lba;
         }
         else
         {
            *pui1_first_lba = ui4_lba - 1;
         }
     }
     
     return b_ret;
 }
 

 /*-----------------------------------------------------------------------------
 * Name: _rec_util_get_closest_lba
 *
 * Description: This API get the closet lba to ui4_lba_begin.
 *              
 * Inputs: 
 *          pt_db
 *          ui4_lba_begin  Specifies the begin lba.
 *          b_backward     Specifies if search from ui4_lba_end to ui4_lba_begin or not.
 *          ui4_lba_1      Specifies a lba value.
 *          ui4_lba_2      Specifies the other lba value.
 *          
 * Outputs: 
 *          pui4_closest_lba    Specifies the closest lba.
 *
 * Returns: INT32          
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_get_closest_lba(REC_UTIL_DB_T*         pt_db, 
                                       UINT32                 ui4_lba_begin,
                                       BOOL                   b_backward,
                                       UINT32                 ui4_lba_1,
                                       UINT32                 ui4_lba_2,
                                       UINT32*                pui4_closest_lba)
{
    INT32          i4_ret;
    UINT64         ui8_lba_1      = (UINT64)ui4_lba_1;
    UINT64         ui8_lba_2      = (UINT64) ui4_lba_2;
    UINT64         ui8_lba_begin  = (UINT64)ui4_lba_begin;
    
    /*check lba*/
    i4_ret = _rec_util_check_lba_valid(ui4_lba_begin, pt_db);
    if (REC_UTILR_OK != i4_ret)
    {
        REC_UTIL_DBG_ERR(("_rec_util_get_closest_lba: check ui4_lba_begin fail(%d)!\r\n",
                          i4_ret));
        return REC_UTILR_INTERNAL_ERROR;
    }
    /*check lba*/
    i4_ret = _rec_util_check_lba_valid(ui4_lba_1, pt_db);
    if (REC_UTILR_OK != i4_ret)
    {
        REC_UTIL_DBG_ERR(("_rec_util_get_closest_lba: check ui4_lba_1 fail(%d)!\r\n",
                          i4_ret));
        return REC_UTILR_INTERNAL_ERROR;
    }
    /*check lba*/
    i4_ret = _rec_util_check_lba_valid(ui4_lba_2, pt_db);
    if (REC_UTILR_OK != i4_ret)
    {
        REC_UTIL_DBG_ERR(("_rec_util_get_closest_lba: check ui4_lba_2 fail(%d)!\r\n",
                          i4_ret));
        return REC_UTILR_INTERNAL_ERROR;
    }

    if (pt_db->b_lba_wrap_over)
    {
        if (ui8_lba_1 < pt_db->ui4_range_start_lba)
        {
            ui8_lba_1 += (UINT64)(pt_db->ui4_avail_max_lba + 1);
        }
        if (ui8_lba_2 < pt_db->ui4_range_start_lba)
        {
            ui8_lba_2 += (UINT64)(pt_db->ui4_avail_max_lba + 1);
        }
        if (ui8_lba_begin < pt_db->ui4_range_start_lba)
        {
            ui8_lba_begin += (UINT64)(pt_db->ui4_avail_max_lba + 1);
        }
    }

    if (b_backward)
    {
        if ((ui8_lba_1 <= ui8_lba_begin) && (ui8_lba_2 <= ui8_lba_begin))
        {
            if (ui8_lba_2 <= ui8_lba_1)
            {
                *pui4_closest_lba = ui4_lba_1;
                return REC_UTILR_OK;
            }
            else
            {
                *pui4_closest_lba = ui4_lba_2;
                return REC_UTILR_OK;
            }
        }
        else if (ui8_lba_1 <= ui8_lba_begin)
        {
            *pui4_closest_lba = ui4_lba_1;
            return REC_UTILR_OK;
        }
        else if (ui8_lba_2 <= ui8_lba_begin)
        {
            *pui4_closest_lba = ui4_lba_2;
            return REC_UTILR_OK;
        }
        else
        {
        }
        *pui4_closest_lba = 0;
        return REC_UTILR_INTERNAL_ERROR;
    }
    else
    {   
        if ((ui8_lba_1 >= ui8_lba_begin) && (ui8_lba_2 >= ui8_lba_begin))
        {
            if (ui8_lba_2 >= ui8_lba_1)
            {
                *pui4_closest_lba = ui4_lba_1;
                return REC_UTILR_OK;
            }
            else
            {
                *pui4_closest_lba = ui4_lba_2;
                return REC_UTILR_OK;
            }
        }
        else if (ui8_lba_1 >= ui8_lba_begin)
        {
            *pui4_closest_lba = ui4_lba_1;
            return REC_UTILR_OK;
        }
        else if (ui8_lba_2 >= ui8_lba_begin)
        {
            *pui4_closest_lba = ui4_lba_2;
            return REC_UTILR_OK;
        }
        else
        {
        }
        *pui4_closest_lba = 0;
        return REC_UTILR_INTERNAL_ERROR;
    }
    
}
  

/*-----------------------------------------------------------------------------
* Name:  _rec_util_strg_write
*
* Description: This function writes tick data to storage.Request data may be split into 
*              several parts which are written by x_fm_write_async.In addition,fucntion 
*              rec_util_do_db_write will collects write result notified by FM
*
* Note: (1) TS File range: Start: file offset(May 1M) 
*                          Size: pt_cfg->t_strg_desc.ui8_buf_sec_ns 
                                  * pt_cfg->t_strg_desc.ui4_sec_size 
*       (2)Write data length:TS pack(Block) alignement and Minimal access sector number alignment
*          record utility must guarentee align buffer memory aligns. All relates to DMA
*       (3)Ring buffer read data my be split due to ring buffer mechanism, so does TS data range.
*          All cause a data may be written to storage by sevral times
*       (4)PVR ring buffer size and init read/write ptr are access & block size align,
*          so does TS data range(range size are access & block size align, init lba and
*          file offset are access align)
*
* Inputs:  
*       pt_db :     DB obejct
*       ui4_total_sz: Specify how many data need to write
*       pt_req_link:Specify infomation of data needed to write to storage
*       
* Outputs: NULL  
*
* Returns: REC_UTILR_OK,          Success. 
*          Non zero value:        Fail
*
* Author : lianming lin(mtk40234)
*
* History:
*   (1)2009-05-07 : initial
------------------------------------------------------------------------------*/
static INT32 _rec_util_strg_write(REC_UTIL_DB_T*          pt_db,
                                       UINT32                  ui4_total_sz,
                                       REC_UTIL_REQ_LINK_T*    pt_req_link)
{
    UINT64                      ui8_tsb_left_sz; /*indicate space between current file pos and end*/
    UINT64                      ui8_old_pos;     /*file pos before fm write*/
    UINT64                      ui8_new_pos;     /*file pos after fm write*/
    REC_UTIL_FM_WRITE_REQ_T*    pt_fm_write_req = NULL;
    HANDLE_T                    h_req;
    INT32                       i4_ret = REC_UTILR_OK;
    UINT32                      ui4_pvr_buf_left_sz;    /*indicate space between PVR read and end ,which used to check if the req data are split into two parts*/
    UINT32                      ui4_fm_write_sz;        /*indicate write size, minimal access align */
    UINT32                      ui4_actual_write_sz = 0;/*data number in one write which cause PVR read ptr change*/
    UINT32                      ui4_remainder_sz = 0;   /*remainder in one write,just written on storage but do not influence PVR read ptr */
    UINT32                      ui4_left_write_sz;      /*indicate left req data,used to control while loop*/
    BOOL                        b_complete = FALSE;     /*indicate a req if is done*/
    BOOL                        b_write_ok = TRUE;
    BOOL                        b_strg_full = FALSE;
    UINT8*                      pui1_fm_write_ptr;      /*indicate write pointer*/
    UINT8                       ui1_fm_req_num = 0;     /*indicate how many fm writes to complete a record req*/
    UINT64                      ui8_temp;
    UINT32                      ui4_last_write_left_sz;
    #ifdef _REC_UTIL_UT_SUPPORT
    BOOL                        b_pvr_ptr_wrap_over;        /*indicate pvr ptr need roll back e.g. pvr ptr get to the pvr range end*/
    BOOL                        b_file_ptr_to_tsb_end;      /*indicate file ptr need roll back e.g. file ptr get to the TS range end*/
    UINT8                       ui1_fm_req_num_file_to_end; /*indicate which fm writes file ptr to the end*/
    #endif    
    
    /*1. check parameter */
    if ((NULL == pt_db) || (NULL == pt_req_link) || (0 == ui4_total_sz))
    {
        REC_UTIL_DBG_ERR(("_rec_util_strg_write: invalid argument!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    /*2. get req info*/
    ui4_left_write_sz      = ui4_total_sz;
    pt_fm_write_req        = &(pt_req_link->t_rec_util_req.u_data.t_fm_write_req);
    ui4_last_write_left_sz = 0;

    #ifdef _REC_UTIL_UT_SUPPORT
    /*Just unit test log ,case 1.1 */
    if (ui4_total_sz < pt_db->ui4_dev_access_sz)
    {     
    }
    
    /*Just unit test for PVR wrap-over*/
    ui4_pvr_buf_left_sz = (UINT32)(pt_db->t_pvr_buf_status.pui1_end_ptr) 
                           - (UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr);
    if (ui4_total_sz > ui4_pvr_buf_left_sz)
    {
        b_pvr_ptr_wrap_over = TRUE;
    }
    else
    {
        b_pvr_ptr_wrap_over = FALSE;
    }
    
    b_file_ptr_to_tsb_end      = FALSE; 
    ui1_fm_req_num_file_to_end = 0; 
    #endif
    
	 REC_UTIL_DBG_INFO(("<<_rec_util_strg_write:[begin]req[%u]\r\n"
                       "ui4_total_sz        = 0x%08x\r\n"
                       "last_write_left_sz  = 0x%08x\r\n"
                       "PVR read ptr        = 0x%08x\r\n"
                       "PVR write ptr       = 0x%08x\r\n"
                       "PVR start ptr       = 0x%08x\r\n"
                       "PVR end ptr         = 0x%08x\r\n>>\r\n",
                       pt_req_link->t_rec_util_req.ui4_req_id,
                       ui4_total_sz,
                       pt_db->ui4_last_write_left_sz,
                       pt_db->t_pvr_buf_status.pui1_read_ptr,
                       pt_db->t_pvr_buf_status.pui1_write_ptr,
                       pt_db->t_pvr_buf_status.pui1_start_ptr,
                       pt_db->t_pvr_buf_status.pui1_end_ptr));
    
    /*3. write to storage*/
    while (!b_complete)
    {
        if (ui1_fm_req_num >= REC_UTIL_FM_WRITE_MAX_NUM_PER_UTIL_REQ)
        {
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
            REC_UTIL_DBG_ERR((  "_rec_util_strg_write: write req num(%d)error!\r\n",
                                ui1_fm_req_num));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }

        ui4_remainder_sz     = 0;
        ui4_actual_write_sz  = 0;
        
        /*3.1 ring buffer read pointer*/
        ui4_left_write_sz  += ui4_last_write_left_sz;
        pui1_fm_write_ptr   = pt_db->t_pvr_buf_status.pui1_read_ptr;
        ui4_fm_write_sz     = ui4_left_write_sz;
        
        /*3.2 get pvr buf size from read pointer to end */
        ui4_pvr_buf_left_sz = (UINT32)(pt_db->t_pvr_buf_status.pui1_end_ptr) 
                               - (UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr);                    
        /*get current file position*/
 	  i4_ret = x_fm_mfw_lseek(pt_db->h_file, 0, FM_SEEK_CUR, &ui8_old_pos);
 	  if (FMR_OK != i4_ret)
 	  {
 		  REC_UTIL_DBG_ERR(("Fail to seek TS-FIFO old pos (%d)!\r\n", i4_ret));
 		  i4_ret = REC_UTILR_INTERNAL_ERROR;
 		  break;
 	  }  
 	  if (ui8_old_pos != pt_db->ui8_tsb_cur_pos)
 	  {
 		  	REC_UTIL_DBG_ERR(("The pt_db's cur_pos does not equal to pos gotten from seek\r\n"
							  "pt_db->ui8_tsb_cur_pos = %#x\r\n"
							  "seek cur pos           = %#x\r\n",
							  pt_db->ui8_tsb_cur_pos,
							  ui8_old_pos));
			i4_ret = REC_UTILR_INTERNAL_ERROR; 
 	  }
 	  
 	  /*file position need sector align and block align should be checked by init lba pos*/
 	  rec_util_div_u64(pt_db->ui8_tsb_cur_pos - pt_db->ui8_tsb_begin_pos, 
 					   (UINT64)(pt_db->ui4_dev_access_sz), 
 					   &ui8_temp);
 	  if (ui8_temp != 0)
 	  {
 		  REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_FILE_POS_NOT_SECTOR_ALIGN);
 	  }     
        
        /*if TS-FIFO write ptr get to the end need warp over to the begin*/
        if (pt_db->ui8_tsb_cur_pos == pt_db->ui8_tsb_end_pos)
        {
            i4_ret = x_fm_mfw_lseek(pt_db->h_file, 
                                (INT64)(pt_db->ui8_tsb_begin_pos), 
                                 FM_SEEK_BGN, 
                                 &ui8_new_pos);
            if (FMR_OK != i4_ret)
            {
                x_dbg_stmt("{REC_UTIL} ERR: Fail to seek TS-FIFO begin (%d)!\r\n", i4_ret);
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            }
            pt_db->ui8_tsb_cur_pos = ui8_new_pos;
        }
        
        /* 3.3 get TS data size from current position to end position*/
        ui8_tsb_left_sz = pt_db->ui8_tsb_end_pos - pt_db->ui8_tsb_cur_pos;
        
        /*3.4 check if request tick data are in two parts: read-end, start-write
          need at least two time writes*/
        if (ui4_fm_write_sz > ui4_pvr_buf_left_sz)
        {
            ui4_fm_write_sz = ui4_pvr_buf_left_sz;
        }
        else
        {
        }
        
        /*3.5 check if TS free space can held such a write data,if not file pointer need roll*/
        if ((UINT64)ui4_fm_write_sz >= ui8_tsb_left_sz)
        {
            ui4_fm_write_sz = (UINT32)ui8_tsb_left_sz;
             rec_util_div_u64(ui8_tsb_left_sz, 
                              (UINT64)(pt_db->ui4_dev_access_sz), 
                              &ui8_temp);
            if (ui8_temp != 0)
            {
                REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_FILE_POS_NOT_SECTOR_ALIGN);
            }
            
            ui4_left_write_sz   -= ui4_fm_write_sz;
            ui4_actual_write_sz  = ui4_fm_write_sz;
            ui4_remainder_sz     = 0;
            #ifdef _REC_UTIL_UT_SUPPORT
            b_file_ptr_to_tsb_end      = TRUE;
            ui1_fm_req_num_file_to_end = ui1_fm_req_num;
            #endif
            
        }
        else
        {
            ui4_left_write_sz   -= ui4_fm_write_sz;
            ui4_actual_write_sz  = ui4_fm_write_sz;
            rec_util_align_write_sz(ui4_fm_write_sz, 
                                    pt_db->ui4_dev_access_sz,
                                    &ui4_fm_write_sz,
                                    &ui4_remainder_sz);
            ui4_actual_write_sz -= ui4_remainder_sz;
        }
        ui4_last_write_left_sz   = ui4_remainder_sz;

        /*3.6 update PVR ring buffer read pointer, do not care about whether write successfully*/ 
        pt_db->t_pvr_buf_status.pui1_read_ptr += ui4_actual_write_sz;
        pt_fm_write_req->z_data_sz            += ui4_actual_write_sz;
        /*check if read pointer get to the end*/
        if ((UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr) 
              == (UINT32)(pt_db->t_pvr_buf_status.pui1_end_ptr))
        {
            pt_db->t_pvr_buf_status.pui1_read_ptr = pt_db->t_pvr_buf_status.pui1_start_ptr;
        }
        else if ((UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr) 
                 > (UINT32)(pt_db->t_pvr_buf_status.pui1_end_ptr))
        {
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
        }
        else
        {
        }
        
        REC_UTIL_DBG_INFO(("<<_rec_util_strg_write:before req[%u][%u]\r\n"
                           "ui4_left_write_sz   = 0x%x\r\n"
                           "ui4_write_sz        = 0x%x\r\n"
                           "ui4_actual_write_sz = 0x%08x\r\n"
                           "ui4_remainder_sz    = 0x%08x\r\n"
                           "b_file_ptr_to_end   = %d\r\n"
                           "ui8_tsb_left_sz     = 0x%08llx\r\n"
                           "PVR read ptr        = 0x%08x\r\n>>\r\n",
                           pt_req_link->t_rec_util_req.ui4_req_id,
                           ui1_fm_req_num,
                           ui4_left_write_sz,
                           ui4_fm_write_sz,
                           ui4_actual_write_sz,
                           ui4_remainder_sz,
                           ((UINT64)ui4_actual_write_sz == ui8_tsb_left_sz) ? TRUE : FALSE,
                           ui8_tsb_left_sz,
                           pt_db->t_pvr_buf_status.pui1_read_ptr));
        
        /*3.7 check if it is TS data and write data from current file position*/
        DBG_CHECK_WRITTEN_TS_DATA(pt_db, 
                                  ui8_old_pos, 
                                  pui1_fm_write_ptr, 
                                  ui4_actual_write_sz + ui4_remainder_sz);
        #if 0
        UINT32 idx = 0;
        x_dbg_stmt("\nPosition before write %#0.10llx\n",ui8_old_pos);
        for(;idx<10;idx++)
        {
            x_dbg_stmt("%#0.2x,",pui1_fm_write_ptr[idx]);
        }
        #endif
        i4_ret = x_fm_mfw_write_async(pt_db->h_file, 
                                  (VOID*)pui1_fm_write_ptr, 
                                  ui4_actual_write_sz, 
                                  128, 
                                  _rec_util_async_write_nfy, 
                                  (VOID*)pt_req_link->t_rec_util_req.h_rec_util, 
                                  &h_req);                           


        if (FMR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("Fail to call x_fm_mfw_write_async () (%d) !\r\n", i4_ret));
            b_write_ok = FALSE;

            if(FMR_FILE_SYSTEM_FULL == i4_ret)
            {
                b_strg_full = TRUE;
            }
        }
        else
        {
            /*where what all we can do is praying it will be OK!*/
            pt_fm_write_req->ah_req[ui1_fm_req_num] = h_req;
            ++ui1_fm_req_num;
            /*change to new position without file seeking*/
            pt_db->ui8_tsb_cur_pos += ui4_actual_write_sz;
        }

    #ifdef _REC_UTIL_UT_SUPPORT
         /*record new position for calculating storage left*/
            i4_ret = x_fm_mfw_lseek(pt_db->h_file, 0, FM_SEEK_CUR, &ui8_new_pos);
            if (FMR_OK != i4_ret)
            {
            REC_UTIL_DBG_ERR(("_rec_util_strg_write:Fail to seek at L%d (%d)!\r\n", 
                              __LINE__, i4_ret));
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            }
         if (ui8_new_pos != pt_db->ui8_tsb_cur_pos)
        {
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
            }
    #endif       

         REC_UTIL_DBG_INFO(("<<_rec_util_strg_write:after req[%u][%u]\r\n"
                            
                            "ui8_new_pos         = 0x%08llx\r\n"
                            "ui4_actual_write_sz = 0x%08x\r\n"
                            "h_req               = 0x%08x\r\n",
                            pt_req_link->t_rec_util_req.ui4_req_id,
                            ui1_fm_req_num - 1,
                            
                            pt_db->ui8_tsb_cur_pos,
                            ui4_actual_write_sz,
                            h_req));

        /*If all req data are written down indicates complete req*/
        if (0 == ui4_left_write_sz)
        {
            b_complete = TRUE;
            i4_ret = REC_UTILR_OK;
        }

        if (ui4_left_write_sz + ui4_remainder_sz < pt_db->ui4_dev_access_sz)
        {
            b_complete = TRUE;
            ui4_last_write_left_sz = ui4_left_write_sz + ui4_remainder_sz;
            i4_ret = REC_UTILR_OK;
        }

    } /*while*/
    
    if (REC_UTILR_OK == i4_ret)
    {
        /*record req num for later fm notify ack collect*/
        pt_fm_write_req->ui1_fm_req_cnt = ui1_fm_req_num;
        
        pt_db->ui4_last_write_left_sz   = ui4_last_write_left_sz;

        
        #ifdef _REC_UTIL_UT_SUPPORT
        /*PVR buffer doesn't wrap over*/
        if ((FALSE == b_pvr_ptr_wrap_over) && (TRUE == b_file_ptr_to_tsb_end))
        {
            /*case 3.1 TS-FIFO does not wrap over*/
            if (ui1_fm_req_num_file_to_end == ui1_fm_req_num - (UINT8)1)
            {
                
            }
            /*case 4.not more than two times fm write*/
            else
            {   
                /*should be first fm write to cause TS-FIFO wrap over */
                if (ui1_fm_req_num_file_to_end != 0)
                {
                    REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
                }
            }
        }
        
        /*PVR buffer wrap over ,this may be incorrect because of logic change,Before:Write another 512,Now: left the data  size = total % device_access_size*/
        if ((TRUE == b_pvr_ptr_wrap_over) && (TRUE == b_file_ptr_to_tsb_end))
        {
            /*case 5.1 TS-FIFO does not wrap over*/
            if (ui1_fm_req_num_file_to_end == ui1_fm_req_num - (UINT8)1)
            {
                
            }
            /*TS-FIFO also wrap over, not more than three time fm writes*/
            else
            {
                
                if (ui1_fm_req_num_file_to_end >  2)
                {
                    REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
                }

                /*two time fm writes to complete one util req ,case 6.3*/
                if (2 == ui1_fm_req_num)
                {   
                    /*should be first time to wrap over TS-FIFO pointer*/
                    if (ui1_fm_req_num_file_to_end != 0)
                    {
                        REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
                    }
                }
           }
        }
        #endif
        
        REC_UTIL_DBG_INFO(("<<_rec_util_strg_write:[end]:req[%u]\r\n"
                           "total FM reqs              = %u\r\n"
                            "pui1_written_data          = 0x%08x\r\n"
                            "z_data_sz                  = 0x%08x\r\n"
                           "ui4_write_left_sz          = 0x%08x\r\n>>\r\n",
                           pt_req_link->t_rec_util_req.ui4_req_id,
                           ui1_fm_req_num,
                            pt_fm_write_req->pui1_written_data,
                           pt_fm_write_req->z_data_sz,
                           ui4_last_write_left_sz));
    }
    else 
    {
        /*write fail, send msg and free memory*/
    }

    if(b_strg_full)
    {
        REC_UTIL_DBG_ERR(("_rec_util_strg_write:req[%u] strg full!\r\n"));
        i4_ret = REC_UTILR_STRG_FULL;
    }
    else if(!b_write_ok)
    {
        REC_UTIL_DBG_ERR(("_rec_util_strg_write:req[%u] not all data write ok!\r\n"));
        i4_ret = REC_UTILR_NOT_ALL_WRITE_OK;
    }
    
    return i4_ret;
}


 static INT32 _rec_util_remove_old_info(REC_UTIL_DB_T*        pt_db,
                                        REC_UTIL_GRB_HANDLE_T h_grb)
 {
     UINT32                  ui4_lba_begin;
     UINT32                  ui4_lba_end;
     
     INT32                   i4_ret;
     
     UINT32                  ui4_idx;
     UINT32                  ui4_update_cnt    = 0;     /*indicate info entries before valid range*/
     BOOL                    b_lba_in_range    = FALSE;
     REC_UTIL_GRB_DATA_T     t_data;
     REC_UTIL_GRB_DATA_T     t_next_data;
     UINT32                  ui4_lba;
     UINT32                  ui4_next_lba;
 
     if (REC_UTILR_GRB_SEEK_ENTRY_FINISHED == 
         rec_util_grb_get_head_entry(h_grb, &t_data))
     {
         REC_UTIL_DBG_INFO(("_rec_util_remove_old_info:h_handle[%u] empty!\r\n" , h_grb));
         return REC_UTILR_OK;
     }
     
     ui4_lba_begin    = pt_db->ui4_range_start_lba;
     ui4_lba_end    = pt_db->ui4_range_end_lba;
     
     ui4_update_cnt = 0;
     
     while (TRUE)
     {
         
         i4_ret = rec_util_grb_seek_next_entry(h_grb,
                                               FALSE,
                                               &t_data);
         if (i4_ret != REC_UTILR_GRB_OK && i4_ret != REC_UTILR_GRB_SEEK_ENTRY_FINISHED)
         {
             REC_UTIL_DBG_ERR(("_rec_util_remove_old_info: "
                               "forward,get lba error(%d)!\r\n",
                               i4_ret));
             return REC_UTILR_INTERNAL_ERROR;
         }
         if(REC_UTIL_GRB_NULL_DATA == t_data.pv_tag)
         {
             break;
         }
         ui4_lba = *((UINT32*)t_data.pv_data);
         /*If in range*/
         b_lba_in_range = _rec_util_lba_in_query_range(ui4_lba, 
                                                       ui4_lba_begin,
                                                       ui4_lba_end,
                                                       pt_db->ui4_avail_max_lba);
         if (b_lba_in_range)
         {
             break;
         }  
         else
         {
             t_next_data = t_data;
            i4_ret = rec_util_grb_seek_next_entry(h_grb, FALSE,
                                               &t_next_data);
            if(REC_UTIL_GRB_NULL_DATA != t_next_data.pv_tag)
            {
                ui4_next_lba = *((UINT32*)t_next_data.pv_data);
                b_lba_in_range = _rec_util_lba_in_query_range(ui4_next_lba, 
                                                              ui4_lba_begin,
                                                              ui4_lba_end,
                                                              pt_db->ui4_avail_max_lba);
                if(!b_lba_in_range)
                {
                     ++ui4_update_cnt;
                }
            }
            else
            {
                break;
            }
            
         }
         
        
     }
 
     /*keep a old info in ring buffer for later search*/
     if (ui4_update_cnt > 1)
     {
         for (ui4_idx = 0; ui4_idx < (UINT32)(ui4_update_cnt - 1); ++ui4_idx)
         {
             i4_ret = rec_util_grb_free_head_entry(h_grb);
             if (i4_ret != REC_UTILR_GRB_OK)
             {
                 REC_UTIL_DBG_ERR(("_rec_util_remove_old_info: "
                                   "forward,get lba error(%d)!\r\n",
                                   i4_ret));
                 return REC_UTILR_INTERNAL_ERROR;
             }
         }   
     }
     return REC_UTILR_OK;
 }


 /*-----------------------------------------------------------------------------
 * Name: _rec_util_update_info_read_ptrs
 *
 * Description: This API update the db info tbl's read pointers.
 *              
 * Inputs:  pt_db           Specify the db object.
 *          
 * Outputs: -
 *
 * Returns: -
 
----------------------------------------------------------------------------*/ 
   
static INT32 _rec_util_update_info_read_ptrs(REC_UTIL_DB_T*      pt_db,
                                             PVR_TICK_INDEX_T*   pt_tick_idx_start,
                                             PVR_TICK_INDEX_T*   pt_tick_idx_end)
{
    HANDLE_T                h_handle;
    INT32                   i4_ret;
    UINT32                  ui4_idx;
    
    if ((NULL == pt_db) || (NULL == pt_tick_idx_start) || (NULL == pt_tick_idx_end))
    {
        REC_UTIL_DBG_ERR(("_rec_util_update_info_read_ptrs: para is NULL!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    for (ui4_idx = REC_DB_INFO_FIRST_KEY_TYPE; ui4_idx < REC_DB_INFO_TYPE_MAX; ++ui4_idx)
    {
        h_handle = pt_db->at_info_grb_tbl[ui4_idx];
        i4_ret   = _rec_util_remove_old_info(pt_db, h_handle);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR(("_rec_util_update_info_read_ptrs: "
                              "remove old info error(%d)!\r\n",
                              i4_ret));
            return REC_UTILR_INTERNAL_ERROR;
        }
        
    }

    for (ui4_idx = SIGNAL_FROM_Y_TO_N; ui4_idx < SIGNAL_CHG_TYPE_MAX; ++ui4_idx)
    {
        h_handle = pt_db->at_signal_grb_chg_tbl[ui4_idx];
        i4_ret   = _rec_util_remove_old_info(pt_db, h_handle);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR(("_rec_util_update_info_read_ptrs: "
                              "remove old signal change info error(%d)!\r\n",
                              i4_ret));
            return REC_UTILR_INTERNAL_ERROR;
        }
    }

    return REC_UTILR_OK;
}
 
 /*-----------------------------------------------------------------------------
 * Name: _rec_util_recalc_ticks
 *
 * Description: This API  checks if  the last tick data override the start tick's data. These will happan if tick nums is not full 
 *                    whereas the ts buffer is full and may override the some area of storage which is  not played by mm_hdlr. 
 *              
 * Inputs:  pt_db           Specify the db object.
 *          
 * Outputs: pb_range_update-TRUE   valid range is changed.
 *                          FALSE  valid range is not changed.
 *
 * Returns: 
 *
  *History:  lianming 2010-07-12
 ----------------------------------------------------------------------------*/ 
static INT32 _rec_util_recalc_ticks(REC_UTIL_DB_T*          pt_db)
{
    INT32                           i4_ret;

    UINT32                          ui4_tick_index_start;
    UINT32                          ui4_tick_index_end;
    
    UINT32                          ui4_total_entry_cnt;
    UINT32                          ui4_valid_tick_num;
    UINT32                          ui4_pkt_cnt;
    
    PVR_TICK_INDEX_T*               pt_start_tick_index;
    PVR_TICK_INDEX_T*               pt_end_tick_index;

    UINT32                          ui4_start_idx_start_lba;
    UINT32                          ui4_start_idx_end_lba;
    UINT32                          ui4_end_idx_start_lba;
    UINT32                          ui4_end_idx_end_lba;

    BOOL                            b_overlap;
    UINT32                          ui4_tick_idx_sz;
    
    if (REC_UTIL_BOUND_TYPE_TIME == pt_db->t_cfg.e_bound_type)
    {
        return REC_UTILR_OK;
    }

    ui4_tick_idx_sz = sizeof(PVR_TICK_INDEX_T);
    do
    {
        b_overlap = FALSE;
        i4_ret = rec_util_rque_get_status(pt_db->h_pvr_tick_idx_tbl,
                                          &ui4_tick_index_start,
                                          &ui4_tick_index_end,
                                          &ui4_valid_tick_num,
                                          &ui4_total_entry_cnt);
        if (i4_ret != REC_UTILR_RQUE_OK)
        {
            REC_UTIL_DBG_ERR(("_rec_util_recalc_ticks:get tick index range error(%d)!\r\n",
                              i4_ret));
            return REC_UTILR_INTERNAL_ERROR;
        } 

        if (ui4_tick_index_start == ui4_tick_index_end)
        {
            return REC_UTILR_OK;
        }
        
        /*get end idx info*/
        i4_ret = rec_util_rque_get_entry( pt_db->h_pvr_tick_idx_tbl, 
                                          ui4_tick_index_end, 
                                         (UINT8**)(&pt_end_tick_index));
        if (REC_UTILR_RQUE_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("_rec_util_recalc_ticks:get end tick error(%d)!\r\n",
                              i4_ret));
            return REC_UTILR_INTERNAL_ERROR;
        }
        ui4_end_idx_start_lba = pt_end_tick_index->ui4_lba;
        ui4_pkt_cnt = PVR_TICK_DETAIL_GET_BLKS(pt_end_tick_index->pt_tick_detail);
        if (ui4_pkt_cnt != 0)
        {
            ui4_end_idx_end_lba = (ui4_end_idx_start_lba + ui4_pkt_cnt - 1) 
                                    % (pt_db->ui4_avail_max_lba + 1);
        }
        else
        {
            /*if end idx is empty, return ok*/
            /*ui4_end_idx_end_lba = ui4_end_idx_start_lba;*/
            return REC_UTILR_OK;
        }
        
        /*get start idx info*/
        i4_ret = rec_util_rque_get_entry( pt_db->h_pvr_tick_idx_tbl, 
                                          ui4_tick_index_start, 
                                          (UINT8**)(&pt_start_tick_index));
        if (REC_UTILR_RQUE_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("_rec_util_recalc_ticks:get start tick error(%d)!\r\n",
                              i4_ret));
            return REC_UTILR_INTERNAL_ERROR;
        }
        ui4_start_idx_start_lba = pt_start_tick_index->ui4_lba;
        ui4_pkt_cnt = PVR_TICK_DETAIL_GET_BLKS(pt_start_tick_index->pt_tick_detail);
        if (ui4_pkt_cnt != 0)
        {
            ui4_start_idx_end_lba = (ui4_start_idx_start_lba + ui4_pkt_cnt - 1) 
                                    % (pt_db->ui4_avail_max_lba + 1);
        }
        else
        {
            ui4_start_idx_end_lba = ui4_start_idx_start_lba;
        }
        

        /*check overlap*/
        #if 1
        /*the start  idx lba range is continuous*/
        if (ui4_start_idx_start_lba < ui4_start_idx_end_lba)
        {
            /*the end idx lba range is continuous*/
            if (ui4_end_idx_start_lba < ui4_end_idx_end_lba)
            {
                if (!((ui4_end_idx_end_lba < ui4_start_idx_start_lba )
                     || (ui4_end_idx_start_lba > ui4_start_idx_end_lba )))
                {
                    b_overlap = TRUE;
                }
                /*the end idx lba range is continuous*/
                /*
                             |------------------------------------------------------------------------
                                                     |                            |                                                 |                                   |
                             ui4_end_idx_start_lba            ui4_end_idx_end_lba      ui4_start_idx_start_lba               ui4_start_idx_end_lba
                                    |                                                                                             |__________________|
                                    |_______________________|
                            
                            |--------------------------------------------------------------------------
                                               |                                              |                                           |                                 |
                            ui4_start_idx_start_lba         ui4_start_idx_end_lba              ui4_end_idx_start_lba          ui4_end_idx_end_lba     
                                               |_____________________|
                                                                                                                                   |_______________________|
                           */
            }
            /*ui4_end_idx_start_lba > ui4_end_idx_end_lba*/
            else  
            {    
                if (!((ui4_end_idx_start_lba > ui4_start_idx_end_lba)
                    && (ui4_end_idx_end_lba < ui4_start_idx_start_lba)))
                {
                    b_overlap = TRUE;
                }
                
                /*
                            |---------------------------------------------------------------------
                                                     |                            |                                        |                                   |
                            ui4_end_idx_end_lba          ui4_start_idx_start_lba          ui4_start_idx_end_lba          ui4_end_idx_start_lba     
                
                         */
            }
        }
        else
        {
            
            if (ui4_end_idx_start_lba < ui4_end_idx_end_lba)
            {
                if (!((ui4_end_idx_start_lba > ui4_start_idx_end_lba)
                       && (ui4_end_idx_end_lba < ui4_start_idx_start_lba)))
                {
                    b_overlap = TRUE;
                }
                /*
                            |---------------------------------------------------------------------
                                                      |                            |                                        |                                   |
                              ui4_start_idx_end_lba   ui4_end_idx_start_lba             ui4_end_idx_end_lba    ui4_start_idx_start_lba      
                          */
            }
            else
            {
                /*this case must overlap*/  
                b_overlap = TRUE;
            
            }
        }
        /*In brief expression*/
        #else
        if (ui4_end_idx_start_lba < ui4_end_idx_end_lba)
        {
             /*the end idx lba range is continuous*/
            if (((ui4_start_idx_start_lba >= ui4_end_idx_start_lba)
                && (ui4_start_idx_start_lba <= ui4_end_idx_end_lba))
                ||
               ((ui4_start_idx_end_lba >= ui4_end_idx_start_lba)
                && (ui4_start_idx_end_lba <= ui4_end_idx_end_lba)))
            {
                b_overlap = TRUE;
            }
        }
        /*ui4_end_idx_start_lba > ui4_end_idx_end_lba*/
        else  
        {    
            /*the start idx lba range is continuous*/
            if (((ui4_end_idx_start_lba >= ui4_start_idx_start_lba) 
                 && (ui4_end_idx_start_lba <= ui4_start_idx_end_lba)) 
                ||
               ((ui4_end_idx_end_lba >= ui4_start_idx_start_lba)
                 && (ui4_end_idx_end_lba <= ui4_start_idx_end_lba)))
            {
                b_overlap = TRUE;
            }
        }
        #endif
        
        if (!b_overlap)
        {
            break;
        }
        else
        {
            i4_ret = rec_util_rque_read(pt_db->h_pvr_tick_idx_tbl, 
                                        NULL, 
                                        &ui4_tick_idx_sz);
            if (REC_UTILR_RQUE_OK != i4_ret)
            {
                REC_UTIL_DBG_ERR(("_rec_util_recalc_ticks:read start tick error(%d)!\r\n",
                                i4_ret));
                return REC_UTILR_INTERNAL_ERROR;
            }
        }
  
    }while(1);

    return REC_UTILR_OK;
}
 /*-----------------------------------------------------------------------------
 * Name: _rec_util_write_tick_data
 *
 * Description: This API write tick data to db.
 *              
 * Inputs:  pt_db           Specify the db object.
 *          pt_tick_data    Specify the written tick data.
 *          pt_req_link     Specify the fm write req.
 *          
 * Outputs: pb_range_update-TRUE   valid range is changed.
 *                          FALSE  valid range is not changed.
 *
 * Returns: 
 ----------------------------------------------------------------------------*/ 
static INT32 _rec_util_write_tick_data(REC_UTIL_DB_T*          pt_db,
                                       REC_UTIL_TICK_DATA_T*   pt_tick_data,
                                       REC_UTIL_REQ_LINK_T*    pt_req_link,
                                       BOOL*                   pb_range_update)
{
    UINT32                          ui4_tick_idx_flags = 0;
    UINT32                          ui4_idx;
    PVR_PIC_INFO_T*                 pt_pic_info        = NULL;
    UINT32                          ui4_valid_tick_num = 0;
    UINT32                          ui4_tick_num;
    PVR_TICK_DETAIL_T               t_empty_tick_detail;
    PVR_TICK_DETAIL_T*              pt_tick_detail     = NULL;
    UINT32                          ui4_pkt_cnt;
    UINT32                          ui4_blk_entries;
    UINT32                          ui4_tick_detail_len;
    INT32                           i4_ret;
    PVR_TICK_INDEX_T                t_tick_index;
    UINT32                          ui4_tick_index_start;
    UINT32                          ui4_tick_index_end;
    UINT32                          ui4_total_entry_cnt;
    PVR_TICK_INDEX_T                t_start_tick_index;
    PVR_TICK_INDEX_T*               pt_end_tick_index;
    UINT32                          ui4_data_len;
    UINT32                          ui4_tick_end_lba; 
    REC_UTIL_GRB_DATA_T             t_data;
    VOID*                           pv_data_addr;

    if (NULL == pt_db           ||
        NULL == pb_range_update || 
        NULL == pt_tick_data    || 
        NULL == pt_req_link)
    {
        REC_UTIL_DBG_ERR(("_rec_util_write_tick_data:param is null!\r\n"));
        return REC_UTILR_INTERNAL_ERROR;
    }

    if (REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
    {
        REC_UTIL_DBG_ERR((  "_rec_util_write_tick_data: ignore write tick "
                            "info for req(%d)\r\n", 
                            pt_req_link->t_rec_util_req.ui4_req_id));
        return   REC_UTILR_IDX_TBL_DISABLED;
    }
    *pb_range_update = FALSE;
    
    /*1.generate tick num*/
    ui4_tick_num = _rec_util_gen_tick_num(pt_db);

    /*2.If empty tick*/
    if (((0 == (pt_tick_data->ui4_flags & REC_UTIL_TICK_DATA_FLAG_DATA_VALID))) 
          || (((pt_tick_data->ui4_flags & REC_UTIL_TICK_DATA_FLAG_DATA_VALID) != 0) 
               && (0 == pt_tick_data->t_tick_data.ui4_pkt_count)))
    {
        ui4_tick_idx_flags                   |= (pt_db->ui4_tick_mask_code | PVR_TICK_INDEX_FLAG_EMPTY);
        ui4_pkt_cnt                          = 0;
        ui4_blk_entries                      = 0;
        t_empty_tick_detail.ui4_tick_num     = ui4_tick_num;
        t_empty_tick_detail.ui4_blks_entries = ui4_blk_entries;
        pt_tick_detail                       = &t_empty_tick_detail;
        
    }
    
    /*3.tick data is valid*/
    else
    {
        if(REC_UTIL_REQR_OK != pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result)
        {
            ui4_tick_idx_flags |= (pt_db->ui4_tick_mask_code | PVR_TICK_INDEX_FLAG_BAD);
        }
        
        pt_tick_detail  = (PVR_TICK_DETAIL_T*)(&(pt_tick_data->t_tick_data.pt_metadata->t_tick_detail));
        ui4_pkt_cnt     = pt_tick_data->t_tick_data.ui4_pkt_count;
        /*parse tick detail for tick index tags*/
        pt_pic_info     = (PVR_PIC_INFO_T*)((UINT8*)pt_tick_detail + sizeof(PVR_TICK_DETAIL_T));
        ui4_blk_entries = PVR_TICK_DETAIL_GET_ENTRIES(pt_tick_detail);

        for (ui4_idx = 0; ui4_idx < ui4_blk_entries; ++ui4_idx)
        {   
            if ((PVR_PIC_INFO_GET_PIC_TYPE(pt_pic_info) & PVR_FRAME_TYPE_MASK_I) != 0)
            {
                ui4_tick_idx_flags |= (pt_db->ui4_tick_mask_code | PVR_TICK_INDEX_FLAG_HAS_I_FRAME);
                break;
            }
            ++pt_pic_info;
        }
        
    }

    /*tick ui4_tick_num + ui4_blks_entries + */
    ui4_tick_detail_len  = sizeof(PVR_TICK_DETAIL_T);
    /*pic info*/
    ui4_tick_detail_len += sizeof(PVR_PIC_INFO_T) * ui4_blk_entries;
    /*ui8_system_time*/
    if (pt_tick_data->ui4_flags & REC_UTIL_TICK_DATA_FLAG_TIME_VALID)
    {
        ui4_tick_detail_len += sizeof(TIME_T);
        ui4_tick_idx_flags  |= (pt_db->ui4_tick_mask_code | PVR_TICK_INDEX_FLAG_HAS_SYS_TIME);
    }

    /*just get a entry to held tick detail*/
    i4_ret = rec_util_grb_alloc_entry(pt_db->h_pvr_grb_detail,
                                       ui4_tick_detail_len,                                       
                                       &t_data);
    if (i4_ret != REC_UTILR_GRB_OK)
    {
       REC_UTIL_DBG_ERR(("_rec_util_write_tick_data:alloc entry error(%d)!\r\n", i4_ret));
       return REC_UTILR_INTERNAL_ERROR;
    }
    
    /*tick num*/
    i4_ret = rec_util_grb_set_entry(pt_db->h_pvr_grb_detail,
                                    &t_data, 0, 
                                    (UINT8*)&ui4_tick_num, sizeof(UINT32));
    if (i4_ret != REC_UTILR_GRB_OK)
    {
       REC_UTIL_DBG_ERR(("_rec_util_write_tick_data:set tick num error(%d)!\r\n",
                         i4_ret));
       return REC_UTILR_INTERNAL_ERROR;
    }
    
    /*ui4_blk_entries*/
    i4_ret = rec_util_grb_set_entry(pt_db->h_pvr_grb_detail,
                                    &t_data, 4, 
                                    (UINT8*)&pt_tick_detail->ui4_blks_entries, 
                                    sizeof(UINT32));
    if (i4_ret != REC_UTILR_GRB_OK)
    {
       REC_UTIL_DBG_ERR(("_rec_util_write_tick_data:set blk entry error(%d)!\r\n",
                         i4_ret));
       return REC_UTILR_INTERNAL_ERROR;
    }
    
    /*pt_pic_info*/
    if (ui4_blk_entries != 0)              
    {
        pt_pic_info = (PVR_PIC_INFO_T*)((UINT8*)pt_tick_detail + sizeof(PVR_TICK_DETAIL_T));
        i4_ret = rec_util_grb_set_entry(pt_db->h_pvr_grb_detail,
                                         &t_data, 8, (UINT8*)pt_pic_info, 
                                         sizeof(PVR_PIC_INFO_T) * ui4_blk_entries);
                 
        if (i4_ret != REC_UTILR_GRB_OK)
        {
           REC_UTIL_DBG_ERR(("_rec_util_write_tick_data:set pic info error(%d)!\r\n",
                             i4_ret));
           return REC_UTILR_INTERNAL_ERROR;
        }
    }

    /*system time*/ 
    if ((pt_tick_data->ui4_flags & REC_UTIL_TICK_DATA_FLAG_TIME_VALID) != 0)
    {
         i4_ret = rec_util_grb_set_entry(pt_db->h_pvr_grb_detail,
                                          &t_data, 8 + sizeof(PVR_PIC_INFO_T) * ui4_blk_entries, 
                                         (UINT8*)&(pt_tick_data->t_time), 
                                         sizeof(TIME_T));
        if (i4_ret != REC_UTILR_GRB_OK)
        {
           REC_UTIL_DBG_ERR(("_rec_util_write_tick_data: set system time error(%d)!\r\n",
                             i4_ret));
           return REC_UTILR_INTERNAL_ERROR;
        }
    }
    
    /*4. Check if tick index table is full*/
    if (rec_util_rque_is_full(pt_db->h_pvr_tick_idx_tbl))
    {
        i4_ret = rec_util_rque_read(pt_db->h_pvr_tick_idx_tbl, NULL, NULL);
        if (i4_ret != REC_UTILR_RQUE_OK)
        {
            REC_UTIL_DBG_ERR(("_rec_util_write_tick_data:read tick index error(%d)!\r\n",
                              i4_ret));
            return REC_UTILR_INTERNAL_ERROR;
        }
        
        /*update tick detail*/
        i4_ret = rec_util_grb_free_head_entry(pt_db->h_pvr_grb_detail);
        if (i4_ret != REC_UTILR_GRB_OK)
        {
            REC_UTIL_DBG_ERR(("_rec_util_write_tick_data:read tick detail error(%d)!\r\n",
                              i4_ret));
            return REC_UTILR_INTERNAL_ERROR;
        }
    }
    
    /*5.write tick index*/
    i4_ret = rec_util_grb_get_data_addr (pt_db->h_pvr_grb_detail, 
                                         &t_data,
                                         NULL,
                                         &pv_data_addr);
    if (i4_ret != REC_UTILR_GRB_OK)
    {
        REC_UTIL_DBG_ERR(("_rec_util_write_tick_data:get data addr fail (%d)!\r\n",
                          i4_ret));
        return REC_UTILR_INTERNAL_ERROR;
    }
    
    t_tick_index.ui4_flags      = ui4_tick_idx_flags;
    t_tick_index.pt_tick_detail = (PVR_TICK_DETAIL_T*) pv_data_addr;
    t_tick_index.ui4_tick_num   = ui4_tick_num;
    t_tick_index.ui4_lba        = pt_db->ui4_avail_cur_lba;
    i4_ret = rec_util_rque_write(pt_db->h_pvr_tick_idx_tbl, 
                                 (UINT8*)(&t_tick_index), 
                                 sizeof(PVR_TICK_INDEX_T));
    if (i4_ret != REC_UTILR_RQUE_OK)
    {
        REC_UTIL_DBG_ERR(("_rec_util_write_tick_data:write tick index error(%d)!\r\n",
                          i4_ret));
        return REC_UTILR_INTERNAL_ERROR;
    }
    
    if (ui4_pkt_cnt != 0)
    {
        ui4_tick_end_lba = (pt_db->ui4_avail_cur_lba + ui4_pkt_cnt - 1) 
                            % (pt_db->ui4_avail_max_lba + 1);
        pt_db->ui4_avail_cur_lba = (pt_db->ui4_avail_cur_lba + ui4_pkt_cnt)
                                    % (pt_db->ui4_avail_max_lba + 1);
    }
    else
    {
        ui4_tick_end_lba = pt_db->ui4_avail_cur_lba;
        /*REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);*/
    }
    
    /*6. calculate valid tick number, in case of the end tick flict with start tick, 
              might need read some ticks off: note LBA is a mod value!!!
              This should be Timeshift with ring ts buffer case
              To handle end tick data override start tick data , need read tick index...
       */
    i4_ret = _rec_util_recalc_ticks(pt_db);
    if (REC_UTILR_OK != i4_ret)
    {
        REC_UTIL_DBG_ERR(("_rec_util_write_tick_data:recalc tick tbl error(%d)!\r\n",
                          i4_ret));
        return REC_UTILR_INTERNAL_ERROR;
    }
    
    i4_ret = rec_util_rque_get_status(pt_db->h_pvr_tick_idx_tbl,
                                      &ui4_tick_index_start,
                                      &ui4_tick_index_end,
                                      &ui4_valid_tick_num,
                                      &ui4_total_entry_cnt);
    if (i4_ret != REC_UTILR_RQUE_OK)
    {
        REC_UTIL_DBG_ERR(("_rec_util_write_tick_data:get tick index range error(%d)!\r\n",
                          i4_ret));
        return REC_UTILR_INTERNAL_ERROR;
    }

    /*not valid range*/
    if (ui4_valid_tick_num <= pt_db->t_cfg.ui4_tk_extra)
    {   
        REC_UTIL_DBG_INFO(("_rec_util_write_tick_data:range is not updated\r\n",
                          i4_ret));
        return REC_UTILR_OK;
    }

    /*valid range has updated*/
    pt_db->b_has_valid_range   = TRUE;
    *pb_range_update           = TRUE;
    
    /*update valid range*/
    pt_db->t_valid_range.ui4_start = (ui4_tick_index_start + pt_db->t_cfg.ui4_tk_extra) 
                                      % ui4_total_entry_cnt;
    pt_db->t_valid_range.ui4_end   = ui4_tick_index_end;

    /*get first tick index*/
    ui4_data_len = sizeof(PVR_TICK_INDEX_T);
    i4_ret       =  rec_util_rque_copy_entry(pt_db->h_pvr_tick_idx_tbl,
                                             pt_db->t_valid_range.ui4_start,
                                             (UINT8*)&t_start_tick_index,
                                             &ui4_data_len);
    if (i4_ret != REC_UTILR_RQUE_OK)
    {
        REC_UTIL_DBG_ERR(("_rec_util_write_tick_data:get start tick index error(%d)!\r\n",
                          i4_ret));
        return REC_UTILR_INTERNAL_ERROR;
    }

    /*end tick index must be this one*/
    pt_end_tick_index = &t_tick_index;  

    /*set valid_lba_min*/
    pt_db->ui4_range_start_lba = t_start_tick_index.ui4_lba;
    pt_db->ui4_range_end_lba   = ui4_tick_end_lba;
    if (pt_db->ui4_range_end_lba < pt_db->ui4_range_start_lba)
    {   
        pt_db->b_lba_wrap_over = TRUE;
    }
    else
    {
        pt_db->b_lba_wrap_over = FALSE;
    }


    /*7. update info:update info and singal change table*/
    i4_ret = _rec_util_update_info_read_ptrs(pt_db, &t_start_tick_index, pt_end_tick_index);
    if (i4_ret != REC_UTILR_RQUE_OK)
    {
        REC_UTIL_DBG_ERR(("_rec_util_write_tick_data:update info error(%d)!\r\n",
                          i4_ret));
        return REC_UTILR_INTERNAL_ERROR;
    }

    return REC_UTILR_OK;
}

  /*-----------------------------------------------------------------------------
 * Name: _rec_util_check_signal_chg
 *
 * Description: This API check signal change or not.
 *              
 * Inputs:  pt_db           Specify the db object.
 *          e_sig_new_state Specify signal state.
 *          
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/    
static VOID _rec_util_check_signal_chg(REC_UTIL_DB_T*              pt_db,
                                       REC_UTIL_SIGNAL_STATUS_T    e_sig_new_state)
{
    REC_UTIL_SIGNAL_CHG_TYPE_T  e_chg_type;
    REC_UTIL_GRB_HANDLE_T       h_grb;
    INT32                       i4_ret;

    if (SIGNAL_STATUS_NONE == pt_db->e_sig_status)
    {
        pt_db->e_sig_status = e_sig_new_state;
        return;
    }
    
    if (pt_db->e_sig_status == e_sig_new_state)
    {
        return;
    }
    
    pt_db->e_sig_status = e_sig_new_state;
    
    if (HAS_SIGNAL == e_sig_new_state)
    {
        e_chg_type = SIGNAL_FROM_N_TO_Y;
    }
    else
    {
        e_chg_type = SIGNAL_FROM_Y_TO_N;
    }

    h_grb = pt_db->at_signal_grb_chg_tbl[e_chg_type];
    i4_ret   = rec_util_grb_write_data(h_grb,
                                  (UINT8*)(&pt_db->ui4_avail_cur_lba),
                                   sizeof(UINT32));
    if (i4_ret != REC_UTILR_GRB_OK)
    {
       REC_UTIL_DBG_ERR(("_rec_util_check_signal_chg:write entry error(%d)!\r\n",
                         i4_ret));
       return;
    }

    return;
    
}


 /*-----------------------------------------------------------------------------
 * Name: _rec_util_check_permit_for_set_cfg
 *
 * Description: This API check all db clients are permit or not. If all client are permitted,
 *              it will enter set cfg req next state.
 *              
 * Inputs:  h_rec_util          Specify the db client handle.
 *          ui4_req_id          Specify the permitted req id.
 *          pt_db_head          Specify the db head obj.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess
 *          REC_UTILR_INTERNAL_ERROR    Fail.
 ----------------------------------------------------------------------------*/    
static INT32 _rec_util_check_permit_for_set_cfg(HANDLE_T h_rec_util,
                                    UINT32               ui4_req_id,
                                    REC_UTIL_DB_HEAD_T*  pt_db_head)
{
    INT32                   i4_ret = REC_UTILR_OK;
    REC_UTIL_REQ_LINK_T*    pt_req_link = NULL;
    REC_UTIL_SET_CFG_REQ_T* pt_set_cfg_req = NULL;
    BOOL                    b_found = FALSE;
    REC_UTIL_DB_T*          pt_db = NULL;
    
    pt_db = pt_db_head->pt_db_obj;
    if (NULL == pt_db)
    {
        REC_UTIL_DBG_ERR(("_rec_util_check_permit_for_set_cfg: db obj is NULL!\r\n"));
        return REC_UTILR_INTERNAL_ERROR;
    }
    
    /*search set cfg req from running_req_list*/
    DLIST_FOR_EACH(pt_req_link, &(pt_db->t_running_req_list), t_link) 
    {
        if (pt_req_link->t_rec_util_req.e_req_type == REC_UTIL_REQ_SET_CFG)
        {
            pt_set_cfg_req = &(pt_req_link->t_rec_util_req.u_data.t_set_cfg_req);
            if (pt_req_link->t_rec_util_req.ui4_req_id == ui4_req_id)
            {
                b_found = TRUE;
                break;
            }
        }
    }
    if (!b_found)
    {
        REC_UTIL_DBG_ERR(("_rec_util_check_permit_for_set_cfg: not found set cfg req!\r\n"));
        return REC_UTILR_INV_REQ_ID;
    }
    
    /*check req state*/
    if (REC_UTIL_SET_CFG_STATE_WAIT_PERMIT != pt_set_cfg_req->ui4_cur_req_state)
    {
        REC_UTIL_DBG_ERR(("_rec_util_check_permit_for_set_cfg: error set_cfg_req state!\r\n"));
        return REC_UTILR_INTERNAL_ERROR;
    }
    
    --(pt_set_cfg_req->ui1_client_hdl_num);
    /*check client_hdl_list is NULL*/
    if (0 != pt_set_cfg_req->ui1_client_hdl_num)
    {
        REC_UTIL_DBG_INFO(("_rec_util_check_permit_for_set_cfg: should wait other permit!\r\n"));
        return REC_UTILR_OK;
    }
    
    /*change set_cfg_req state and do fifo to invoke process*/
    pt_set_cfg_req->ui4_cur_req_state = REC_UTIL_REQ_STATE_NONE;
    i4_ret = _rec_util_do_fifo_req_process( pt_req_link->t_rec_util_req.h_rec_util,
                                            pt_db_head);
    if(REC_UTILR_OK != i4_ret)
    {
        REC_UTIL_DBG_ERR(("_rec_util_check_permit_for_set_cfg: call fifo error!\r\n"));
        return REC_UTILR_INTERNAL_ERROR;
    }
    
    return i4_ret;
}

static INT32 _rec_util_check_lba_valid(UINT32                 ui4_lba,
                                       REC_UTIL_DB_T*         pt_db)
{
    BOOL    b_lba_in_range = FALSE;
    UINT32  ui4_lba_start;
    UINT32  ui4_lba_end;

    if (NULL == pt_db)
    {
        REC_UTIL_DBG_ERR(("_rec_util_check_lba_valid:invalid argument!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    /*check lba*/
    if (pt_db->b_has_valid_range)
    {
        ui4_lba_start  = pt_db->ui4_range_start_lba;
        ui4_lba_end    = pt_db->ui4_range_end_lba;
        b_lba_in_range = _rec_util_lba_in_query_range(ui4_lba, ui4_lba_start, 
                                                      ui4_lba_end,
                                                      pt_db->ui4_avail_max_lba);
    }
    else
    {
        REC_UTIL_DBG_ERR(("_rec_util_check_lba_valid:not has valid range!\r\n"));
        return REC_UTILR_INV_LBA;
    }
    
    if (!b_lba_in_range)
    {
        REC_UTIL_DBG_ERR(("_rec_util_check_lba_valid:[%u, %u] ,lba(%u) is invalid!\r\n",
                          ui4_lba_start,
                          ui4_lba_end,
                          ui4_lba));
        return REC_UTILR_INV_LBA;
    }
    else
    {
        return REC_UTILR_OK;
    }
}

static VOID _rec_util_get_pba_param(
                REC_UTIL_DB_T*                  pt_db,
                const REC_UTIL_PVR_BUF_CNST_T*  pt_buf_cnst,
                REC_UTIL_PBA_PARAM_T*           pt_param)
{
    REC_UTIL_CONFIG_T*              pt_cfg      = NULL;
    BOOL                            b_found     = FALSE;
    UINT16                          ui2_idx     = 0;
    REC_UTIL_BITRATE_MAP_ITEM_T*    pt_bitrate_item = NULL;
    UINT32                          ui4_buf_sz_align;
    
    if(NULL == pt_db || NULL == pt_param)
    {
        REC_UTIL_DBG_ERR(("_rec_util_get_pba_param:param invalid!\r\n"));
        REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
        return;
    }
    
    pt_cfg = &(pt_db->t_cfg);
    pt_param->e_brdcst_type     = pt_cfg->e_brdcst_type;
    pt_param->e_brdcst_medium   = pt_cfg->e_brdcst_medium;
    pt_param->pv_rec_purpose    = pt_cfg->pv_rec_purpose;
    pt_param->ui4_mem_align     = rec_util_get_smallest_multi_num(pt_buf_cnst->ui4_align_buf_start,
                                                                  pt_cfg->t_strg_desc.ui4_mem_align);
    ui4_buf_sz_align            = rec_util_get_smallest_multi_num(REC_UTIL_LOGIC_BLOCK_SIZE,
                                                                  pt_cfg->t_strg_desc.ui4_sec_size);
    ui4_buf_sz_align            = rec_util_get_smallest_multi_num(ui4_buf_sz_align,
                                                                  pt_buf_cnst->ui4_align_buf_size);                                                              
    for(ui2_idx = 0; ui2_idx < t_rec_util_sys_cfg.ui2_bitrate_map_items; ++ui2_idx)
    {
        pt_bitrate_item = &(t_rec_util_sys_cfg.at_bitrate_map[ui2_idx]);
        if( (pt_bitrate_item->e_brdcst_medium == pt_cfg->e_brdcst_medium) &&
            (pt_bitrate_item->e_brdcst_type == pt_cfg->e_brdcst_type))
        {
            b_found = TRUE;
            break;
        }
    }

    if(!b_found)
    {
        REC_UTIL_DBG_ERR(("_rec_util_get_pba_param:seach bitrate fail!\r\n"));
        REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
        return;
    }
    
    pt_param->z_size            = (SIZE_T) ((((UINT64) t_rec_util_sys_cfg.ui4_pvr_buf_depth) *
                                               ((UINT64) pt_bitrate_item->ui4_byte_per_second))/1000);

    pt_param->z_size            =   ((pt_param->z_size + 
                                    (ui4_buf_sz_align - 1))/
                                    ui4_buf_sz_align)*
                                    (ui4_buf_sz_align);
    
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _rec_util_do_accidental_req
 *
 * Description: This function process req type SET_CFG, SET_ADDON_TICK_FLAG, 
 *              SET_FINALIZED. 
 *
 * Inputs:  h_rec_util          Specify the db client obj handle.
 *          pt_db_head          Specify the DB header.
 *          pt_req_link         Specify req
 *
 * Outputs: NULL
 *
 * Returns: REC_UTILR_OK,          Success. 
 *          Non zero value:        Fail
 *
 * Author : ql.wang(mtk40234)
 ----------------------------------------------------------------------------*/
 
 static INT32 _rec_util_do_accidental_req(HANDLE_T               h_rec_util,
                                  REC_UTIL_DB_HEAD_T*            pt_db_head,
                                  REC_UTIL_REQ_LINK_T*           pt_req_link)
 {
     INT32                   i4_ret      = REC_UTILR_OK;
     REC_UTIL_DB_T*          pt_db       =   NULL;
     REC_UTIL_REQ_TYPE       e_req_type;
     REC_UTIL_MSG_T          t_msg;
 
     if( NULL_HANDLE == h_rec_util ||
         NULL == pt_db_head ||
         NULL == pt_req_link)
     {
         REC_UTIL_DBG_ERR(("_rec_util_do_accidental_req: invalid arguments!\r\n"));
         return REC_UTILR_INV_ARG;    
     }
 
     e_req_type  = pt_req_link->t_rec_util_req.e_req_type;
     pt_db       = pt_db_head->pt_db_obj;
     
     do{
         if(REC_UTIL_REQ_SET_CFG == e_req_type)
         {
             /*it must be the last one req in running req list*/
             if(NULL != DLIST_NEXT(pt_req_link, t_link))
             {
                 REC_UTIL_DBG_ERR((  "_rec_util_do_accidental_req:set cfg is not "
                                     "the last req in running req list"));
                 i4_ret = REC_UTILR_INTERNAL_ERROR;
                 break;
             }
             REC_UTIL_DBG_INFO(("_rec_util_do_accidental_req: do cfg!\r\n"));
             /*send do cfg msg*/
             x_memset(&t_msg, 0, sizeof(REC_UTIL_MSG_T));
             t_msg.e_msg_type                         = REC_UTIL_MSG_DO_CFG;
             t_msg.u_data.t_do_cfg.h_rec_util         = h_rec_util;
             t_msg.u_data.t_do_cfg.h_db_head          = pt_db_head->h_rec_db_head;
             t_msg.u_data.t_do_cfg.pt_cfg             = 
                 pt_req_link->t_rec_util_req.u_data.t_set_cfg_req.pt_cfg;
             t_msg.u_data.t_do_cfg.ui4_set_cfg_req_id = 
                 pt_req_link->t_rec_util_req.ui4_req_id;
             rec_util_send_msg(&t_msg);
         }
         else if(REC_UTIL_REQ_SET_FINALIZED == e_req_type)
         {
             if(pt_req_link->t_rec_util_req.u_data.t_set_finalized_req.b_finalized)
             {
            	
        		
                 i4_ret = _rec_util_flush_pvr_buf(h_rec_util);
            	 if (i4_ret != REC_UTILR_OK)
        		 {
        			 REC_UTIL_DBG_ERR(("sm_rcrd_sess_state_do_opened:flush pvr buffer data failed(%d)!\r\n",
        							i4_ret));
        		 }
                 
                 i4_ret = x_fm_mfw_close(pt_db->h_file);
                 if(FMR_OK != i4_ret)
                 {
                     REC_UTIL_DBG_ERR((  "_rec_util_do_accidental_req:close file "
                                         "handle error!\r\n"));
                     i4_ret = REC_UTILR_INTERNAL_ERROR;                    
                 }
                 pt_db->h_file = NULL_HANDLE;
             }
             else
             {
                 i4_ret = x_fm_mfw_open(FM_ROOT_HANDLE, pt_db->t_cfg.t_strg_desc.ps_strg_path, 
                                        FM_READ_WRITE|FM_OPEN_APPEND, 0666, TRUE, &(pt_db->h_file));
                 if (FMR_OK != i4_ret)
                 {
                     pt_db->h_file = NULL_HANDLE;
                     REC_UTIL_DBG_ERR(("_rec_util_do_accidental_req: fm reopen fail!\r\n"));
                     i4_ret = REC_UTILR_INTERNAL_ERROR;
                 }
             }
             break;
         }
         else if(REC_UTIL_REQ_SET_ADDON_TICK_FLAG == e_req_type)
         {
             pt_db->ui4_tick_mask_code = 
                 pt_req_link->t_rec_util_req.u_data.t_set_addon_tick_flag_req.ui4_mask_code;
             break;    
         }
         else
         {
             REC_UTIL_DBG_ERR((  "_rec_util_do_accidental_req: req type(%d) not "
                                 "support!\r\n", e_req_type));
             i4_ret = REC_UTILR_NOT_SUPPORT;
             break;
         }
     }while(0);
 
     return i4_ret;
 }
 /*-----------------------------------------------------------------------------
 * Name:  _rec_util_write_ack
 *
 * Description: This function ack wirte req. It must send ack msg  even if there exists error
 *
 * Inputs:  h_rec_util             Specify the db client obj handle.
 *              pt_db_head          Specify the DB header.
 *              pt_write_req        Specify write req
 *
 * Outputs: NULL
 *
 * Returns: REC_UTILR_OK,          Success. 
 *          Non zero value:        Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *        (1)2009-05-18 : init by Lianming Lin
 ----------------------------------------------------------------------------*/
 static INT32 _rec_util_write_ack(HANDLE_T                       h_rec_util,
                                       REC_UTIL_DB_HEAD_T*            pt_db_head,
                                       REC_UTIL_REQ_LINK_T*           pt_write_req)
 {
     INT32                           i4_ret = REC_UTILR_OK;
     REC_UTIL_FM_WRITE_REQ_T*        pt_fm_write_req = NULL;
     REC_UTIL_TICK_DATA_T*           pt_tick_data = NULL;
     BOOL                            b_valid_range = FALSE;
     REC_UTIL_MSG_T                  t_msg;
     REC_UTIL_WRITE_ACK_INFO_T*      pt_write_ack = NULL;
     
     PVR_TICK_INDEX_T*               pt_tick_start = NULL;
     PVR_TICK_INDEX_T*               pt_tick_end = NULL;
     
     
     /*get fm req*/
     pt_fm_write_req = &(pt_write_req->t_rec_util_req.u_data.t_fm_write_req);
     
     /*1.need write tick detail*/
     pt_tick_data    = &(pt_fm_write_req->t_tick_data);
     
    
     if ((REC_UTIL_IDX_TBL_OPT_DISABLE != pt_db_head->pt_db_obj->t_cfg.e_idx_tbl_opt)
          && !((pt_fm_write_req->i4_result == REC_UTIL_REQR_REACH_BOUND) 
                && (REC_UTIL_BUF_TYPE_PLAIN == pt_db_head->pt_db_obj->t_cfg.e_buf_type)))
     { 
         /*not empty tick data*/
         if (pt_fm_write_req->pui1_written_data != NULL)
         {
             _rec_util_check_signal_chg(pt_db_head->pt_db_obj, HAS_SIGNAL);
             i4_ret = _rec_util_write_tick_data(pt_db_head->pt_db_obj, 
                                                pt_tick_data,
                                                pt_write_req,
                                                &b_valid_range);
             if (i4_ret != REC_UTILR_OK)
             {
                 REC_UTIL_DBG_ERR(("_rec_util_write_ack: write tick error!\r\n"));
             }
             
         }
         /*empty tick data*/
         else
         {
             
             _rec_util_check_signal_chg(pt_db_head->pt_db_obj, NO_SIGNAL);
             if (REC_UTIL_EMPTY_TK_OPT_IGNORE == pt_db_head->pt_db_obj->t_cfg.e_empty_tk_opt)
             {
                 REC_UTIL_DBG_INFO(("_rec_util_write_ack: ignore empty tick!\r\n"));
             }
             else
             {
                 REC_UTIL_DBG_INFO(("_rec_util_write_ack: write empty tick!\r\n"));
                 /*If tick KEEP should write tick to memory*/
                 i4_ret = _rec_util_write_tick_data(pt_db_head->pt_db_obj, 
                                                    pt_tick_data,
                                                    pt_write_req,
                                                    &b_valid_range);
                 if (i4_ret != REC_UTILR_OK)
                 {
                     REC_UTIL_DBG_ERR(("_rec_util_write_ack: write empty tick error!\r\n"));
                 }
             }
         }
     }
     
     /*2.send write ack message*/
     x_memset(&t_msg, 0, sizeof(REC_UTIL_MSG_T));
     t_msg.e_msg_type = REC_UTIL_MSG_NFY_WRITE_ACK;
     t_msg.u_data.t_write_ack.h_rec_util = h_rec_util;
     t_msg.u_data.t_write_ack.h_db_head  = pt_db_head->h_rec_db_head;
     pt_write_ack = &(t_msg.u_data.t_write_ack);
     
     pt_write_ack->ui4_req_id = pt_write_req->t_rec_util_req.ui4_req_id;
     pt_write_ack->i4_result = pt_fm_write_req->i4_result;
     
     if ((pt_tick_data->ui4_flags & REC_UTIL_TICK_DATA_FLAG_DATA_VALID) != 0)
     {
         pt_write_ack->ui4_flags |= REC_UTIL_WRITE_ACK_FLAG_METADATA_TAG_VALID;
         pt_write_ack->pv_metadata_tag = pt_tick_data->t_tick_data.pv_metadata_tag;
     }
     if (pt_fm_write_req->pui1_written_data != 0)
     {
         pt_write_ack->ui4_flags |= REC_UTIL_WRITE_ACK_FLAG_WRITTEN_DATA_VALID;
         pt_write_ack->pv_written_data = (VOID*)pt_fm_write_req->pui1_written_data;
         pt_write_ack->z_data_len = pt_fm_write_req->z_data_sz;
     }
     
     /*Send ack message*/
     rec_util_send_msg(&t_msg);
 
     REC_UTIL_DBG_INFO(("_rec_util_write_ack:req_id[%d],"
                         "index range[%u,%u], lba range[%u,%u],"
                         " ack result(%d)\r\n",
                         (UINT32)pt_write_req->t_rec_util_req.ui4_req_id,
                         pt_db_head->pt_db_obj->t_valid_range.ui4_start,
                         pt_db_head->pt_db_obj->t_valid_range.ui4_end,
                         pt_db_head->pt_db_obj->ui4_range_start_lba,
                         pt_db_head->pt_db_obj->ui4_range_end_lba,
                         pt_write_ack->i4_result));  
 
     if( (REC_UTIL_REQR_REACH_BOUND == pt_fm_write_req->i4_result) 
          || (REC_UTIL_REQR_STRG_FULL == pt_fm_write_req->i4_result))
     {
         pt_db_head->pt_db_obj->i4_tsb_write_status = pt_fm_write_req->i4_result;
     }
     
     /*3.If valid range is updated notify range updated msg to all clients*/
     if (b_valid_range)
     {
         x_memset(&t_msg, 0, sizeof(REC_UTIL_MSG_T));
         t_msg.e_msg_type = REC_UTIL_MSG_NFY_VALID_RANGE_UPDATED;
         t_msg.u_data.t_valid_range_nfy.h_rec_util = h_rec_util;
         t_msg.u_data.t_valid_range_nfy.h_db_head  = pt_db_head->h_rec_db_head;
         
         t_msg.u_data.t_valid_range_nfy.b_info_updated = pt_db_head->pt_db_obj->b_out_range_info_update;
         t_msg.u_data.t_valid_range_nfy.t_valid_range.ui4_start = pt_db_head->pt_db_obj->t_valid_range.ui4_start;
         t_msg.u_data.t_valid_range_nfy.t_valid_range.ui4_end = pt_db_head->pt_db_obj->t_valid_range.ui4_end;
         /*Send range updated message*/
         rec_util_send_msg(&t_msg);
         pt_db_head->pt_db_obj->b_out_range_info_update = FALSE;
 
 
         if (REC_UTILR_RQUE_OK != 
             rec_util_rque_get_entry(pt_db_head->pt_db_obj->h_pvr_tick_idx_tbl, 
                                         pt_db_head->pt_db_obj->t_valid_range.ui4_start, 
                                         (UINT8**)(&pt_tick_start)))
         {
             REC_UTIL_DBG_ERR(("_rec_util_write_ack: get start tick(%d)" 
                               "error!\r\n", pt_db_head->pt_db_obj->t_valid_range.ui4_start));
         }
 
         if (REC_UTILR_RQUE_OK != 
             rec_util_rque_get_entry(pt_db_head->pt_db_obj->h_pvr_tick_idx_tbl, 
                                         pt_db_head->pt_db_obj->t_valid_range.ui4_end, 
                                         (UINT8**)(&pt_tick_end)))
         {
             REC_UTIL_DBG_ERR(("_rec_util_write_ack: get end tick(%d)" 
                               "error!\r\n", pt_db_head->pt_db_obj->t_valid_range.ui4_end));
         }
 
         if(pt_tick_start->ui4_tick_num > pt_tick_end->ui4_tick_num)
         {
             REC_UTIL_DBG_ERR(("_rec_util_write_ack: tick index range(%d,%d)," 
                               "tick num range(%d,%d)!\r\n", 
                               pt_db_head->pt_db_obj->t_valid_range.ui4_start,
                               pt_db_head->pt_db_obj->t_valid_range.ui4_end,
                               pt_tick_start->ui4_tick_num,
                               pt_tick_end->ui4_tick_num));
 
             REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
         }                    
     }
 
     return i4_ret;
 }
 /*-----------------------------------------------------------------------------
 * Name:  _rec_util_do_fifo_req_process
 *
 * Description: This function send write ack by FIFO rule. 
 *
 * Inputs:  h_rec_util          Specify the db client obj handle.
 *          pt_db_head          Specify the DB header.
 *
 * Outputs: NULL
 *
 * Returns: REC_UTILR_OK,             Success. 
 *          Non zero value:        Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *        (1)2009-05-18 : init by Lianming Lin
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_do_fifo_req_process(HANDLE_T              h_rec_util,
                                           REC_UTIL_DB_HEAD_T*   pt_db_head)
{
    INT32                           i4_ret            = REC_UTILR_OK;
    REC_UTIL_REQ_LINK_T*            pt_req_link_cur   = NULL;
    REC_UTIL_REQ_LINK_T*            pt_req_link_next  = NULL;

    /*this can not be null*/
    if (NULL == pt_db_head)
    {
        REC_UTIL_DBG_ERR(("_rec_util_do_fifo_req_process:pt_db_head is null\r\n"));
        return REC_UTILR_INV_ARG;
    }
 
     /*search running req list ,send ack by FIFO rules*/
    pt_req_link_cur = DLIST_HEAD(&(pt_db_head->pt_db_obj->t_running_req_list));
    while (NULL != pt_req_link_cur)
    {
        pt_req_link_next = DLIST_NEXT(pt_req_link_cur, t_link);
        if (REC_UTIL_REQ_FM_WRITE == pt_req_link_cur->t_rec_util_req.e_req_type)
        {
            if (TRUE == pt_req_link_cur->t_rec_util_req.u_data.t_fm_write_req.b_fm_reqs_done)
            {
                REC_UTIL_DBG_INFO(("_rec_util_do_fifo_req_process: write req_id[%d]\r\n",
                                   (UINT32)pt_req_link_cur->t_rec_util_req.ui4_req_id));      
                /*send the ack of the first req */
                i4_ret = _rec_util_write_ack(h_rec_util, pt_db_head, pt_req_link_cur);
                if (i4_ret != REC_UTILR_OK)
                {
                    REC_UTIL_DBG_ERR(("_rec_util_do_fifo_req_process:"
                                      "ack req[%u] failed(%d)!\r\n",
                                      pt_req_link_cur->t_rec_util_req.ui4_req_id,
                                      i4_ret));
                }
                else
                {
                    REC_UTIL_DBG_INFO(("_rec_util_do_fifo_req_process:fifo ack req[%u]ok!\r\n",
                                       pt_req_link_cur->t_rec_util_req.ui4_req_id));
                } 
                
                /*free timer*/
                _rec_util_free_timer_link (pt_db_head->pt_db_obj,
                                           pt_req_link_cur->t_rec_util_req.u_data.t_fm_write_req.pt_timer);
                
                /*free write req no matter ack is ok*/
                _rec_util_remove_req_link(pt_req_link_cur, 
                                          &(pt_db_head->pt_db_obj->t_running_req_list), 
                                          &(pt_db_head->pt_db_obj->t_free_req_list));
            }
            else
            {
                /*If first req is not done do not ack. ack should be as FIFO*/
                REC_UTIL_DBG_INFO(("_rec_util_do_fifo_req_process: req_id[%d] "
                                   "hasn't finished\r\n",
                                   (UINT32)pt_req_link_cur->t_rec_util_req.ui4_req_id));
                break;
             }
        }
        else/*process other req type*/
        {
            i4_ret = _rec_util_do_accidental_req(h_rec_util, 
                                                 pt_db_head, 
                                                 pt_req_link_cur);
            if (REC_UTILR_OK != i4_ret)
            {
                REC_UTIL_DBG_ERR(("_rec_util_do_fifo_req_process:"
                                  "do accidental req[%u] failed(%d)!\r\n",
                                  pt_req_link_cur->t_rec_util_req.ui4_req_id,
                                  i4_ret));
                //break;
            }
            /*free write req*/
            _rec_util_remove_req_link(pt_req_link_cur, 
                                      &(pt_db_head->pt_db_obj->t_running_req_list), 
                                      &(pt_db_head->pt_db_obj->t_free_req_list));
            
         }
         pt_req_link_cur = pt_req_link_next;
     }
 
     return i4_ret;
 }
INT32 _rec_util_create_db_by_file(const CHAR * ps_head_file,
                                       REC_UTIL_DB_T ** ppt_db)
{
    REC_UTIL_DB_T*      pt_db ;
    INT32               i4_ret;
    SIZE_T              z_len_file;

    
    z_len_file = x_strlen(ps_head_file);
    
    if (z_len_file == 0)
    {
        return REC_UTILR_INV_ARG;
    }

    pt_db  = NULL;
    i4_ret = REC_UTILR_OK;
    
    do
    {
        /* 1. new a db object */
        pt_db = (REC_UTIL_DB_T*) x_mem_alloc(sizeof(REC_UTIL_DB_T));
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_do_cfg: malloc a db fail!\r\n"));
            i4_ret = REC_UTILR_OUT_OF_MEMORY;
            break;
        }
        x_memset(pt_db, 0, sizeof(REC_UTIL_DB_T));
        
        /* assign profile */
        pt_db->e_profile = REC_UTIL_DB_PROFILE_LIMIT_1;

        /* keep head file name */
        pt_db->s_head_file = x_mem_alloc (z_len_file + 1);
        if (pt_db->s_head_file == NULL)
        {
            REC_UTIL_DBG_ERR(("rec_util_do_cfg: malloc s_head_file fail!\r\n"));
            i4_ret = REC_UTILR_OUT_OF_MEMORY;
            break;
        }
        
        x_strcpy (pt_db->s_head_file, ps_head_file);
        
        /* init RFI context */
        i4_ret = rec_util_meta_rfi_init (& pt_db->t_ctx_rfi);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR(("rec_util_do_cfg: init rfi context fail (%d). \r\n", i4_ret));
            break;
        } 
    
    } while (FALSE);
    
    if (i4_ret == REC_UTILR_OK)
    {
        *ppt_db = pt_db;
    }
    else
    {
        if (pt_db != NULL)
        {
            _rec_util_free_db (pt_db);
            pt_db = NULL;
        }
    }
    
    return i4_ret;
}

#ifndef REC_UTIL_AYNC_WRITE
 /*-----------------------------------------------------------------------------
  * Name:  _rec_util_req_sync_write
  *
  * Description: This function process client sync write request however USB write is still async. 
  *
  * Inputs:  h_rec_util   Specifies the client handle
  *             pt_data      Contains the data and info to be written.
  *
  * Outputs: pui4_req_id  Contains the request id.
  *
  * Returns: REC_UTILR_OK,          Success. 
  *               Non zero value:        Fail
  *
  * Author : lianming lin(mtk40234)
  *
  * History:
  *        (2)2010-08-03 : inital  by Lianming Lin
  -------------------------------------------------------------------------*/
 static INT32 _rec_util_req_sync_write (HANDLE_T                     h_rec_util,
                                               const REC_UTIL_TICK_DATA_T*  pt_data,
                                               UINT32*                      pui4_req_id)
 {
     INT32                   i4_ret        = REC_UTILR_OK;
     HANDLE_TYPE_T           e_handle_type;
     REC_UTIL_DB_HEAD_T*     pt_db_head    = NULL;
     REC_UTIL_CLIENT_T*      pt_client_obj = NULL;
     REC_UTIL_DB_T*          pt_db         = NULL;
     VOID*                   pv_obj        = NULL;
     REC_UTIL_REQ_LINK_T*    pt_req_link   = NULL;
     UINT32                  ui4_req_id;
     UINT32                  ui4_req_total_sz;      /*req size*/
     UINT32                  ui4_pvr_avail_total_sz;/*all data need be read in PVR ring buffer*/
     UINT16                  ui2_blks;
     UINT8                   ui1_hdl_cnt;
     UINT8*                  pui1_last_packet;
     
     /*1.Check param*/
     if ((NULL == pt_data) || (NULL == pui4_req_id))
     {
         REC_UTIL_DBG_ERR(("_rec_util_req_sync_write: invalid argument!\r\n"));
         return REC_UTILR_INV_ARG;
     }
     /*Assert metadata is not null*/
     if (((pt_data->ui4_flags & REC_UTIL_TICK_DATA_FLAG_DATA_VALID) != 0)
         && (NULL == pt_data->t_tick_data.pt_metadata))
     {
         REC_UTIL_DBG_ERR(("_rec_util_req_sync_write: pt_metadata is null!\r\n"));
         return REC_UTILR_INV_ARG;
     }
 
     /*2.get client obejct*/
     i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
     if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
     {
         REC_UTIL_DBG_ERR(("_rec_util_req_sync_write: invalid handle!\r\n"));
         return REC_UTILR_INV_HANDLE;
     }
 
     rec_util_lock_with_tag(REC_UTIL_TAG_REQ_WRITE);
     do
     {
         /*Get DB obejct*/
         pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
         pt_db_head    = pt_client_obj->pt_rec_db_head;
         if (NULL == pt_db_head)
         {
             REC_UTIL_DBG_ERR(("_rec_util_req_sync_write: db head obj is NULL!\r\n"));
             i4_ret = REC_UTILR_INTERNAL_ERROR;
             break;
         }
         if (REC_UTIL_COND_READY != pt_db_head->e_cond)
         {
             REC_UTIL_DBG_ERR(("_rec_util_req_sync_write: db obj is not READY!\r\n"));
             i4_ret = REC_UTILR_INV_COND;
             break;
         }
         pt_db = pt_db_head->pt_db_obj;
         if (NULL == pt_db)
         {
             REC_UTIL_DBG_ERR(("_rec_util_req_sync_write: db obj is NULL!\r\n"));
             i4_ret = REC_UTILR_INTERNAL_ERROR;
             break;
         }         
         /* check profile */
         if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
         {
             REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
             i4_ret = REC_UTILR_REQ_NOT_ALLOW;
             break;
         }
         if (pt_db->b_finalized)
         {
             REC_UTIL_DBG_ERR(("_rec_util_req_sync_write: db finalized!\r\n"));
             i4_ret = REC_UTILR_REQ_FINALIZED;
             break;
         }
 
         if ((REC_UTIL_REQR_REACH_BOUND == pt_db->i4_tsb_write_status) 
             || (REC_UTIL_REQR_STRG_FULL == pt_db->i4_tsb_write_status))
         {
             REC_UTIL_DBG_ERR(("_rec_util_req_sync_write: strg reach bound or full!\r\n"));
             i4_ret = REC_UTILR_REQ_NOT_ALLOW;
             break;
         }
         
         /*3.Generate write req id*/
         ui4_req_id = _rec_util_gen_req_id();
         *pui4_req_id = ui4_req_id;
         ++(pt_db->ui4_req_write_call_num);
         
         /*3.1 check if running list's reqs  are over the limit and ignore its result*/
         _rec_util_check_list_status(&(pt_db->t_running_req_list),REC_UTIL_REQ_FM_WRITE);
         /*4.get a req*/
         i4_ret = _rec_util_get_req_link(&(pt_db->t_free_req_list), &pt_req_link);
         if (REC_UTILR_OK != i4_ret)
         {
             REC_UTIL_DBG_ERR(("_rec_util_req_sync_write:_rec_util_get_req_link error!\r\n"));
             break;
         }
         pt_req_link->t_rec_util_req.e_req_type = REC_UTIL_REQ_FM_WRITE;
         pt_req_link->t_rec_util_req.h_rec_util = h_rec_util;
         pt_req_link->t_rec_util_req.ui4_req_id = ui4_req_id;
 
         pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer = NULL;
         i4_ret = _rec_util_alloc_timer_link (pt_db, 
                                              & (pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer));
         if (REC_UTILR_OK != i4_ret)
         {
             REC_UTIL_DBG_ERR(("_rec_util_req_sync_write:_rec_util_alloc_timer_link error!\r\n"));
             pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer = NULL;
             break;
         }   
         pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = REC_UTIL_REQR_NOT_INIT;
         pt_req_link->t_rec_util_req.u_data.t_fm_write_req.b_fm_reqs_done = FALSE;
         x_memcpy(&(pt_req_link->t_rec_util_req.u_data.t_fm_write_req.t_tick_data), 
                  pt_data, sizeof(REC_UTIL_TICK_DATA_T));
 
         /*5.If tick data is empty*/
         if ((0 == (pt_data->ui4_flags & REC_UTIL_TICK_DATA_FLAG_DATA_VALID)) 
              || (((pt_data->ui4_flags & REC_UTIL_TICK_DATA_FLAG_DATA_VALID) != 0) 
                   && (0 == pt_data->t_tick_data.ui4_pkt_count)))
         {
             pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pui1_written_data = NULL;
             pt_req_link->t_rec_util_req.u_data.t_fm_write_req.z_data_sz         = 0;
             pt_req_link->t_rec_util_req.u_data.t_fm_write_req.b_fm_reqs_done    = TRUE;
             pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = REC_UTIL_REQR_OK;
             
             /*5.1 insert into running req list to collect fm ack info*/
             DLIST_INSERT_TAIL(pt_req_link, &pt_db->t_running_req_list, t_link);
             REC_UTIL_DBG_INFO(( "_rec_util_req_sync_write: write req(%d), empty tick!\r\n",
                                 pt_req_link->t_rec_util_req.ui4_req_id));
 
             /*5.2 ack write*/
             i4_ret = _rec_util_do_fifo_req_process(h_rec_util, pt_db_head);
             if (REC_UTILR_OK != i4_ret)
             {
                 REC_UTIL_DBG_ERR(("_rec_util_req_sync_write: empty tick,do fifo ack fail!\r\n"));
                 //i4_ret = REC_UTILR_INTERNAL_ERROR;
                 //break;
             }
             i4_ret = REC_UTILR_OK;   /*need not free req*/
             break;
         }
         /*6.The following need write tick data to storage and metadata to memory*/
         else
         {
             UINT32   ui4_ofs_last_pkt;
             /*6.1 Check Tick data write pointer*/
             if (pt_data->t_tick_data.pui1_data != pt_db->t_pvr_buf_status.pui1_write_ptr)
             {
                 REC_UTIL_DBG_ERR(("<<_rec_util_req_sync_write: input data is error"
                                   "report pui1_data = 0x%08x\r\n"
                                   "pui1_write_ptr   = 0x%08x\r\n>>\r\n",
                                   pt_data->t_tick_data.pui1_data,
                                   pt_db->t_pvr_buf_status.pui1_write_ptr));
                 i4_ret = REC_UTILR_INTERNAL_ERROR;
                 break;
             }
 
             /*6.2 get last packet timecode for flush usage*/
             ui4_ofs_last_pkt = (pt_data->t_tick_data.ui4_pkt_count - 1) * REC_UTIL_LOGIC_BLOCK_SIZE;
             if (((UINT32) (((UINT32) pt_db->t_pvr_buf_status.pui1_write_ptr) + ui4_ofs_last_pkt))
                  < ((UINT32) pt_db->t_pvr_buf_status.pui1_end_ptr))
             {
                 pui1_last_packet = (UINT8 *) (((UINT32) pt_db->t_pvr_buf_status.pui1_write_ptr) 
                                                 + ui4_ofs_last_pkt);
             }
             else
             {
                 pui1_last_packet = (UINT8 *) (((UINT32) pt_db->t_pvr_buf_status.pui1_start_ptr)
                                                 +  (((UINT32) pt_db->t_pvr_buf_status.pui1_write_ptr)
                                                 +   ui4_ofs_last_pkt
                                                 -  ((UINT32) pt_db->t_pvr_buf_status.pui1_end_ptr))); 
             }    
             
             pt_db->ui4_last_packet_timecode = GET_UINT32_FROM_PTR_BIG_END(pui1_last_packet);
                 
             /*6.3. Check block number*/
             ui2_blks = PVR_TICK_DETAIL_GET_BLKS(&(pt_data->t_tick_data.pt_metadata->t_tick_detail));
             if ((UINT32)ui2_blks != pt_data->t_tick_data.ui4_pkt_count)
             {
                 REC_UTIL_DBG_ERR(("<<_rec_util_req_sync_write:input tick data error!"
                                   "ui4_blks_entries = 0x%08x\r\n"
                                   "ui4_pkt_count    = 0x%08x\r\n"
                                   "ui2_blks         = 0x%08x\r\n",
                                   pt_data->t_tick_data.pt_metadata->t_tick_detail.ui4_blks_entries, 
                                   pt_data->t_tick_data.ui4_pkt_count,
                                   ui2_blks));
                 /*REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);*/
             }
             
             /*6.4 Update PVR buffer writer pointer for later check */
             ui4_req_total_sz = pt_data->t_tick_data.ui4_pkt_count * REC_UTIL_LOGIC_BLOCK_SIZE;
             /*Assume r=w is full . only at first time it means empty, when r=w again it means full*/
             if ((((UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr) > (UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr))
                       && ((UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr 
                         - (UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr))
                         < ui4_req_total_sz))
                    ||  
                 (((UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr) < (UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr))
                       && ((UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr) 
                        - (UINT32)(pt_db->t_pvr_buf_status.pui1_start_ptr)
                        + (UINT32)(pt_db->t_pvr_buf_status.pui1_end_ptr)
                        - (UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr)
                        < ui4_req_total_sz)))
             {
                 REC_UTIL_DBG_ERR(("_rec_util_req_sync_write:"
                                   "PVR ring buffer will be full and discard data!"
                                   "(read=0x%08x, write=0x%08x, data_sz=0x%08x)\r\n",
                                   pt_db->t_pvr_buf_status.pui1_read_ptr, 
                                   pt_db->t_pvr_buf_status.pui1_write_ptr,
                                   ui4_req_total_sz));
                 REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_PVR_CTRL_OVERFLOW);
             }
             
             REC_UTIL_DBG_INFO(("_rec_util_req_sync_write:req[%u] size = %u\r\n",
                                ui4_req_id,
                                ui4_req_total_sz));
 
             /*6.5 update write pointer whatever fm write result is*/
             pt_db->t_pvr_buf_status.pui1_write_ptr += ui4_req_total_sz;
             if (pt_db->t_pvr_buf_status.pui1_write_ptr >= pt_db->t_pvr_buf_status.pui1_end_ptr)
             {
                 pt_db->t_pvr_buf_status.pui1_write_ptr =  (UINT8*)
                       (((UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr) 
                       - 
                       (UINT32)(pt_db->t_pvr_buf_status.pui1_end_ptr)) 
                       + 
                       (UINT32)(pt_db->t_pvr_buf_status.pui1_start_ptr));
             }
             
             /*6.5 calculate total write data size*/
             if (pt_db->t_pvr_buf_status.pui1_read_ptr > pt_db->t_pvr_buf_status.pui1_write_ptr)
             {
                 ui4_pvr_avail_total_sz = (UINT32)(pt_db->t_pvr_buf_status.pui1_end_ptr )
                                           - (UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr)
                                           + (UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr) 
                                           - (UINT32)(pt_db->t_pvr_buf_status.pui1_start_ptr);
             }
             else
             {
                 ui4_pvr_avail_total_sz = (UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr) - 
                                          (UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr);
             }
             
             /*6.6 set  fm_write_req for write tick data. the read pointer will update when data is written to USB*/
             pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pui1_written_data =
                          pt_db->t_pvr_buf_status.pui1_read_ptr;
             pt_req_link->t_rec_util_req.u_data.t_fm_write_req.z_data_sz = 0;
 
             /* every rec_uitl_request at most tree times fm_async_write, constriant: init lba,ts begin offset, tsb alignement*/
             for (ui1_hdl_cnt = 0; 
                  ui1_hdl_cnt < REC_UTIL_FM_WRITE_MAX_NUM_PER_UTIL_REQ; 
                  ui1_hdl_cnt++)
             {
                 pt_req_link->t_rec_util_req.u_data.t_fm_write_req.ah_req[ui1_hdl_cnt] = NULL_HANDLE;
             }
             
             REC_UTIL_DBG_INFO(("<<_rec_util_req_sync_write:req[%d]"
                                "pui1_read_ptr  = 0x%08x\r\n"
                                "pui1_write_ptr = 0x%08x\r\n>>\r\n", 
                                ui4_req_id,
                                pt_db->t_pvr_buf_status.pui1_read_ptr,
                                pt_db->t_pvr_buf_status.pui1_write_ptr));
             
             /*inser into running req list to collect fm ack info*/
             DLIST_INSERT_TAIL(pt_req_link, &(pt_db->t_running_req_list), t_link);
 
             /*6.7 check the left space size, if reach bound and is plain buf then 
                           cancle req and nfy to client*/
             if ((pt_db->ui8_tsb_end_pos - pt_db->ui8_tsb_cur_pos)  
                  < (UINT64) ui4_pvr_avail_total_sz)
             {
                 REC_UTIL_DBG_INFO(("<<_rec_util_req_sync_write:req[%d]"
                                    "reach bound\r\n", ui4_req_id));
                                    
                 pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result =
                         REC_UTIL_REQR_REACH_BOUND;
             }
 
             if ((pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result == REC_UTIL_REQR_REACH_BOUND) 
                  && (pt_db->t_cfg.e_buf_type == REC_UTIL_BUF_TYPE_PLAIN))
             {
                 pt_req_link->t_rec_util_req.u_data.t_fm_write_req.b_fm_reqs_done = TRUE;
                 i4_ret = _rec_util_do_fifo_req_process(h_rec_util, pt_db_head);
                 if (REC_UTILR_OK != i4_ret)
                 {
                     REC_UTIL_DBG_ERR(("_rec_util_req_sync_write: plain buf full, do fifo ack fail!\r\n"));
                     //i4_ret = REC_UTILR_INTERNAL_ERROR;
                     //break;
                 }
                 i4_ret = REC_UTILR_OK;   /*need not free req*/
                 break;    
             }
             
             /*6.8 start write timer*/
             i4_ret = x_timer_start( pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer->h_timer,
                                     ui4_fm_req_timer_delay, 
                                     X_TIMER_FLAG_ONCE, 
                                     _rec_util_write_timeout_nfy, 
                                     (VOID*) pt_req_link);
             if (OSR_OK != i4_ret)
             {
                 REC_UTIL_DBG_ERR(("_rec_util_req_sync_write: start timer fail!\r\n"));
                 i4_ret = REC_UTILR_INTERNAL_ERROR;
                 break;    
             }
             pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer->b_active = TRUE;
             
             /*6.9 write to storage*/
             i4_ret = _rec_util_strg_write(pt_db, ui4_pvr_avail_total_sz, pt_req_link);
             if (REC_UTILR_OK != i4_ret)
             {
                 REC_UTIL_DBG_ERR(("_rec_util_req_sync_write: write req(%d) fail(%d)!\r\n",
                                   pt_req_link->t_rec_util_req.ui4_req_id, 
                                   i4_ret));
                 if ((REC_UTILR_NOT_ALL_WRITE_OK == i4_ret) ||
                     (REC_UTILR_STRG_FULL == i4_ret))
                 {
                     if (REC_UTILR_NOT_ALL_WRITE_OK == i4_ret)
                     {
                         pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = 
                             REC_UTIL_REQR_ABORTED;
                     }
                     else
                     {
                         pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = 
                             REC_UTIL_REQR_STRG_FULL;
                     }
 
                     _rec_util_abort_fm_async(pt_req_link);
                     i4_ret = _rec_util_do_fifo_req_process(h_rec_util, pt_db_head);
                     if (REC_UTILR_OK != i4_ret)
                     {
                         REC_UTIL_DBG_ERR(("_rec_util_req_sync_write: abort req, do fifo ack fail!\r\n"));
                         //i4_ret = REC_UTILR_INTERNAL_ERROR;
                         //break;
                     }
                     i4_ret = REC_UTILR_OK;   /*need not free req*/
                 }    
                 break;
             }
             
             REC_UTIL_DBG_INFO(("_rec_util_req_sync_write: write req(%d), write hdl num(%d)!\r\n",
                                pt_req_link->t_rec_util_req.ui4_req_id,
                                pt_req_link->t_rec_util_req.u_data.t_fm_write_req.ui1_fm_req_cnt));
         }
         
     }while(0);
     rec_util_unlock_with_tag(REC_UTIL_TAG_REQ_WRITE);
 
     if (REC_UTILR_OK != i4_ret)
     {
         /*return to free req list*/
         if (pt_req_link != NULL)
         { 
             if (pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer)
             {
                 _rec_util_free_timer_link (pt_db, 
                                            pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer);
             }
             
             _rec_util_remove_req_link(pt_req_link, 
                                       &(pt_db_head->pt_db_obj->t_running_req_list), 
                                       &(pt_db_head->pt_db_obj->t_free_req_list));
         }
     }
     return i4_ret;
 }
#else
/*-----------------------------------------------------------------------------
  * Name:  _rec_util_req_aync_write
  *
  * Description: This function process client async write request. 
  *
  * Inputs:  h_rec_util   Specifies the client handle
  *             pt_data      Contains the data and info to be written.
  *
  * Outputs: pui4_req_id  Contains the request id.
  *
  * Returns: REC_UTILR_OK,       Success. 
  *               Non zero value:        Fail
  *
  * Author : lianming lin(mtk40234)
  *
  * History:
  *        (2)2010-08-02 : modified by Lianming Lin
  -------------------------------------------------------------------------*/
 static INT32 _rec_util_req_aync_write (HANDLE_T                     h_rec_util,
                                               const REC_UTIL_TICK_DATA_T*  pt_data,
                                               UINT32*                      pui4_req_id)
 {
     INT32                   i4_ret = REC_UTILR_OK;
     HANDLE_TYPE_T           e_handle_type;
     VOID*                   pv_obj = NULL;
     UINT32                  ui4_req_id;
     REC_UTIL_MSG_T          t_msg;
     
     /*1.Check param*/
     if ((NULL == pt_data) || (NULL == pui4_req_id))
     {
         REC_UTIL_DBG_ERR(("_rec_util_req_aync_write: invalid argument!\r\n"));
         return REC_UTILR_INV_ARG;
     }
     
     /*Assert metadata is not null*/
     if (((pt_data->ui4_flags & REC_UTIL_TICK_DATA_FLAG_DATA_VALID) != 0)
         && (NULL == pt_data->t_tick_data.pt_metadata))
     {
         REC_UTIL_DBG_ERR(("_rec_util_req_aync_write: pt_metadata is null!\r\n"));
         return REC_UTILR_INV_ARG;
     }
 
     /*2.get client obejct*/
     i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
     if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
     {
         REC_UTIL_DBG_ERR(("_rec_util_req_aync_write: invalid handle!\r\n"));
         return REC_UTILR_INV_HANDLE;
     }
     
     /*3.Generate write req id*/
     ui4_req_id = _rec_util_gen_req_id();
     *pui4_req_id = ui4_req_id;
 
     x_memset(&t_msg, 0, sizeof(REC_UTIL_MSG_T));
     t_msg.e_msg_type                                 = REC_UTIL_MSG_ASYNC_WRITE_REQ;
     t_msg.u_data.t_write_req.h_rec_util              = h_rec_util;
     x_memcpy((VOID*)&(t_msg.u_data.t_write_req.t_data), 
              (VOID*)pt_data, sizeof(REC_UTIL_TICK_DATA_T));
     t_msg.u_data.t_write_req.ui4_req_id              = ui4_req_id;
     rec_util_send_msg(&t_msg);
     
     return REC_UTILR_OK;
 
 }
#endif

/*-----------------------------------------------------------------------------
                    public functions implementation for internal call 
 ----------------------------------------------------------------------------*/


  /*-----------------------------------------------------------------------------
 * Name: rec_util_free_db_head_list
 *
 * Description: This API will free all db head obj in head list.
 *              
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess
 *          REC_UTILR_INTERNAL_ERROR    Fail.
 ----------------------------------------------------------------------------*/    
INT32 rec_util_free_db_head_list(VOID)
{
    INT32                   i4_ret = REC_UTILR_OK;
    REC_UTIL_DB_HEAD_T*     pt_db_head = NULL;
    
    while(NULL != pt_rec_db_head_list)
    {
        pt_db_head = pt_rec_db_head_list;
        pt_rec_db_head_list = pt_rec_db_head_list->pt_next;
        i4_ret = _rec_util_free_db_head(pt_db_head);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_free_db_head_list: free db head fail!\r\n"));
            break;
        }
    }    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_nfy_all_clients
 *
 * Description: This API will notify all db client objs.
 *              
 * Inputs:  h_db_head       Specify the db head handle.
 *          e_nfy_code      Specify the notify code.
 *          pv_nfy_tag      Specify the nfy tag.
 *          ui4_nfy_data    Specify the nfy data.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess
 *          REC_UTILR_INTERNAL_ERROR    Fail.
 ----------------------------------------------------------------------------*/    
INT32 rec_util_nfy_all_clients(HANDLE_T                h_db_head,
                               REC_UTIL_NTFY_CODE_T    e_nfy_code,
                               VOID*                   pv_nfy_tag,
                               UINT32                  ui4_nfy_data)
{
    INT32                       i4_ret = REC_UTILR_OK;
    HANDLE_TYPE_T               e_handle_type;
    VOID*                       pv_obj = NULL;
    REC_UTIL_DB_HEAD_T*         pt_db_head = NULL;
    REC_UTIL_CLIENT_T*          pt_client_obj = NULL;
    REC_UTIL_HDL_LINK_T*        pt_hdl_link = NULL;
    HANDLE_T                    h_iter;
    BOOL                        b_return = FALSE;

    i4_ret = handle_get_type_obj(h_db_head, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_DB_HEAD_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_nfy_all_clients: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    pt_db_head = (REC_UTIL_DB_HEAD_T*)pv_obj;
    
    DLIST_FOR_EACH(pt_hdl_link, &pt_db_head->t_client_handle_list, t_link)
    {
        h_iter = pt_hdl_link->h_rec_util;
        i4_ret = handle_get_type_obj(h_iter, &e_handle_type, &pv_obj);
        if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
        {
            REC_UTIL_DBG_ERR(("rec_util_nfy_all_clients: invalid handle!\r\n"));
            return REC_UTILR_INV_HANDLE;
        }
        
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        if (NULL != pt_client_obj->pf_nfy)
        {
            b_return = (pt_client_obj->pf_nfy)(h_iter, pt_client_obj->pv_tag, e_nfy_code, ui4_nfy_data);

            if (REC_UTIL_NTFY_CODE_RESET_REQ == e_nfy_code)
            {
                /*permit reset*/
                if (b_return)
                {
                    i4_ret = _rec_util_check_permit_for_set_cfg(h_iter, ui4_nfy_data, pt_db_head);
                    if (REC_UTILR_OK != i4_ret)
                    {
                        REC_UTIL_DBG_ERR(("rec_util_nfy_all_clients: check permit error!\r\n"));
                    }
                }
            }            
        }
        else
        {
            REC_UTIL_DBG_ERR(("rec_util_nfy_all_clients: client has no nfy func!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
    }
    if (REC_UTIL_NTFY_CODE_COND_CHANGED == e_nfy_code)
    {
        if (((REC_UTIL_COND_CHG_NTFY_INFO_T*)ui4_nfy_data)->ui4_data != 0)
        {
            x_mem_free((REC_UTIL_SHARE_DATA_T *)(((REC_UTIL_COND_CHG_NTFY_INFO_T*)ui4_nfy_data)->ui4_data));
            ((REC_UTIL_COND_CHG_NTFY_INFO_T*)ui4_nfy_data)->ui4_data = 0;
        }
    }
    return i4_ret;
}

  /*-----------------------------------------------------------------------------
 * Name: rec_util_nfy_self_client
 *
 * Description: This API will notify self db client obj.
 *              
 * Inputs:  h_rec_util      Specify the db client obj handle.
 *          e_nfy_code      Specify the notify code.
 *          pv_nfy_tag      Specify the nfy tag.
 *          ui4_nfy_data    Specify the nfy data.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess
 *          REC_UTILR_INTERNAL_ERROR    Fail.
 ----------------------------------------------------------------------------*/    
INT32 rec_util_nfy_self_client(HANDLE_T                h_rec_util,
                               REC_UTIL_NTFY_CODE_T    e_nfy_code,
                               VOID*                   pv_nfy_tag,
                               UINT32                  ui4_nfy_data)
{
    INT32                       i4_ret = REC_UTILR_OK;
    HANDLE_TYPE_T               e_handle_type;
    VOID*                       pv_obj = NULL;
    REC_UTIL_CLIENT_T*          pt_client_obj = NULL;

    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_nfy_selt_client: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
    if (NULL != pt_client_obj->pf_nfy)
    {
        pv_nfy_tag = pt_client_obj->pv_tag;
        (pt_client_obj->pf_nfy)(h_rec_util, pv_nfy_tag, e_nfy_code, ui4_nfy_data);
    }
    
    return i4_ret;
}

  /*-----------------------------------------------------------------------------
 * Name: rec_util_do_cfg
 *
 * Description: This API will do db cfg operation.
 *              
 * Inputs:  h_rec_util      Specify the db client obj handle.
 *          pt_do_cfg       Specify the db cfg informaiton.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess
 *          REC_UTILR_INTERNAL_ERROR    Fail.
 ----------------------------------------------------------------------------*/ 

INT32 rec_util_do_cfg(HANDLE_T                    h_rec_util,
                          REC_UTIL_DO_CFG_INFO_T*     pt_do_cfg)
{
    INT32                       i4_ret         = REC_UTILR_OK;
    HANDLE_TYPE_T               e_handle_type;
    VOID*                       pv_obj         = NULL;
    REC_UTIL_DB_HEAD_T*         pt_db_head     = NULL;
    REC_UTIL_CLIENT_T*          pt_client_obj  = NULL;
    REC_UTIL_DB_T*              pt_db          = NULL;
    REC_UTIL_CONFIG_T*          pt_cfg         = NULL;
    SIZE_T                      z_head_file;
    UINT8                       ui1_idx;
    UINT32                      ui4_tick_num;
    REC_UTIL_MSG_T              t_msg;
    INT32                       i4_cfg_result = 0;
    UINT64                      ui8_file_end_pos;
    UINT64                      ui8_temp;
    UINT64                      ui8_ts_range_sz;
    UINT64                      ui8_init_lba_pos;
    UINT64                      ui8_small_multi_num;
    UINT64                      ui8_cur_pos;
    UINT32                      ui4_ini_sz;
    REC_UTIL_GRB_MEM_MODE_T     e_grb_mem_mode;

    /*1.Check paras*/
    if ((NULL == pt_do_cfg) || (NULL == pt_do_cfg->pt_cfg))
    {
        REC_UTIL_DBG_ERR(("rec_util_do_cfg: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }

    /*2. get object*/
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_do_cfg: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }

    /*3. Do cfg*/
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_do_cfg: db head obj is NULL!\r\n"));
            return REC_UTILR_INTERNAL_ERROR;
        }
        
        /*3.1 check cond*/
        if (REC_UTIL_COND_TRANSITION != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_do_cfg: db obj is not TRANSITION!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        /*3.2 free old db obj*/
        if (NULL != pt_db_head->pt_db_obj)
        {
            i4_ret = _rec_util_free_db(pt_db_head->pt_db_obj);
            if (REC_UTILR_OK != i4_ret)
            {
                REC_UTIL_DBG_ERR(("rec_util_do_cfg: free db obj fail!\r\n"));
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            }
            pt_db_head->pt_db_obj = NULL;
        }

        /*3.4 new a db object*/
        pt_db = (REC_UTIL_DB_T*)x_mem_alloc(sizeof(REC_UTIL_DB_T));
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_do_cfg: malloc a db fail!\r\n"));
            i4_ret = REC_UTILR_OUT_OF_MEMORY;
            break;
        }
        x_memset(pt_db, 0, sizeof(REC_UTIL_DB_T));

        /*3.5 save cfg info*/
        x_memcpy(&(pt_db->t_cfg), pt_do_cfg->pt_cfg, sizeof(REC_UTIL_CONFIG_T));
        /*free the req cfg memory*/
        x_mem_free(pt_do_cfg->pt_cfg);
        pt_cfg = &(pt_db->t_cfg);

        /*3.5.1*/
        pt_db->e_profile = REC_UTIL_DB_PROFILE_UNLIMIT;
        
        z_head_file = x_strlen(pt_cfg->t_strg_desc.ps_strg_path);

        pt_db->s_head_file = x_mem_alloc(z_head_file + 1);

        if(pt_db->s_head_file == NULL)
        {
            REC_UTIL_DBG_ERR(("Our of memory line %d\n",__LINE__));
            break;
        }
        x_strcpy(pt_db->s_head_file,pt_cfg->t_strg_desc.ps_strg_path);
        
        /*3.6. calculate tick index table entry count and create table*/
        if (REC_UTIL_BOUND_TYPE_TIME == pt_cfg->e_bound_type)
        {  
            ui4_tick_num = pt_cfg->u_data.t_time_bound_info.ui4_ticks 
                           + pt_cfg->ui4_tk_extra;
        }
        else
        {
            /*space bound*/
            /*if space bound should prohibit ring ts buffer, here do not add code to check!*/
            if (REC_UTIL_BUF_TYPE_PLAIN != pt_cfg->e_buf_type)
            {
                REC_UTIL_DBG_ERR(("rec_util_do_cfg: Space bound should use plain ts buffer!!!\r\n"));
            }

            /*no tick index table*/
            if ((REC_UTIL_SPACE_BOUND_TIME_UNLIMIT == pt_cfg->u_data.t_space_bound_info.ui4_idx_tbl_ticks)  
                && (REC_UTIL_IDX_TBL_OPT_DISABLE != pt_cfg->e_idx_tbl_opt))
            {
                REC_UTIL_DBG_ERR(("rec_util_do_cfg: unlimit ticks and idx enable!\r\n"));
                i4_ret = REC_UTILR_NOT_SUPPORT;
                break;
            }
            
            ui4_tick_num = pt_cfg->u_data.t_space_bound_info.ui4_idx_tbl_ticks 
                           + pt_cfg->ui4_tk_extra;
        }

        /*create tick idx tick detail and other info tbl*/
        if (REC_UTIL_IDX_TBL_OPT_DISABLE != pt_cfg->e_idx_tbl_opt)
        {
            REC_UTIL_RQUE_MEM_MODE_T    e_rque_mem_mode;
            pt_db->ui4_tick_mask_code = 0;
            
            /*3.7 create tick index table*/
            #ifdef LINUX_TURNKEY_SOLUTION
            {
                e_rque_mem_mode = REC_UTIL_RQUE_MEM_MODE_SHARE;
            }
            #else
            {
                e_rque_mem_mode = REC_UTIL_RQUE_MEM_MODE_NORMAL;
            }
            #endif
            
            i4_ret = rec_util_rque_create ( e_rque_mem_mode,
                                            sizeof(PVR_TICK_INDEX_T),
                                            ui4_tick_num,
                                            REC_UTIL_PADDING_TICK_NUM,
                                           &(pt_db->h_pvr_tick_idx_tbl));
            if (i4_ret != REC_UTILR_RQUE_OK)
            {
                REC_UTIL_DBG_ERR(("rec_util_do_cfg: create tick index table failed(%d)\r\n",
                                  i4_ret));
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
     
            }
            
            /*3.8 set tick ctrl block info*/
            i4_ret = rec_util_rque_get_info (pt_db->h_pvr_tick_idx_tbl, 
                                             (UINT8**)&(pt_db->t_pvr_tick_cb.pv_start_address),
                                             (UINT8**)&(pt_db->t_pvr_tick_cb.pv_end_address),
                                              &(pt_db->t_pvr_tick_cb.ui4_entry_size));
            if (i4_ret != REC_UTILR_RQUE_OK)
            {
                REC_UTIL_DBG_ERR(("rec_util_do_cfg: get tick index table info failed(%d)\r\n",
                                  i4_ret));
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
     
            }
            pt_db->t_pvr_tick_cb.ui4_entry_num          = (ui4_tick_num + REC_UTIL_PADDING_TICK_NUM);
            pt_db->t_pvr_tick_cb.ui4_tick_period        = pt_cfg->ui4_ms_tick_period;
            pt_db->t_pvr_tick_cb.ui4_max_vld_entry_num  = ui4_tick_num;
            
            /*3.9 create tick detail ring buffer*/
            #ifdef LINUX_TURNKEY_SOLUTION
            {
                e_grb_mem_mode = REC_UTIL_GRB_MEM_MODE_SHARE;
            }
            #else
            {
                e_grb_mem_mode = REC_UTIL_GRB_MEM_MODE_NORMAL;
            }
            #endif
            ui4_ini_sz = REC_DB_ALIGN_DATA(REC_UTIL_TICK_DETAIL_PREDICT_SZ  
                                           + REC_UTIL_GRB_ENTRY_OVERHEAD ) * ui4_tick_num / 6;
            
            i4_ret = rec_util_grb_init(&(pt_db->h_pvr_grb_detail), 
                                       e_grb_mem_mode, 
                                       ui4_ini_sz);
            if (i4_ret != REC_UTILR_GRB_OK)
            {
                REC_UTIL_DBG_ERR(("rec_util_do_cfg: create tick detail ring buffer failed(%d)\r\n",
                                  i4_ret));
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            }
            
            /*3.10 create info ring buffer*/
            for(ui1_idx = 0; ui1_idx < REC_DB_INFO_TYPE_MAX; ++ui1_idx)
            {
                switch (ui1_idx)
                {
                case REC_DB_INFO_TYPE_START_TIME: /* TIME_T */
                case REC_DB_INFO_TYPE_DURATION:
#ifndef MW_EDB_SUPPORT
                case REC_DB_INFO_TYPE_ELAPSED_TIME:
                    ui4_ini_sz = 4 * (REC_UTIL_GRB_ENTRY_OVERHEAD  + 8);
                    break;
                case REC_DB_INFO_TYPE_EVENT_TITLE:/* UTF-8 string */
                    ui4_ini_sz = 4 * (REC_UTIL_GRB_ENTRY_OVERHEAD  + 64); 
                    break;
                case REC_DB_INFO_TYPE_SVC_NAME:
                    ui4_ini_sz = 4 * (REC_UTIL_GRB_ENTRY_OVERHEAD  + 20);
                    break;
                case REC_DB_INFO_TYPE_EVENT_TEXT:
                case REC_DB_INFO_TYPE_SVC_TEXT:
                case REC_DB_INFO_TYPE_EVENT_EXT_TEXT:
                    ui4_ini_sz = 4 * (REC_UTIL_GRB_ENTRY_OVERHEAD  + 256);
                    break;
#else
				case REC_DB_INFO_TYPE_EVENT_TITLE_BY_ID:
					/*for edb correponding evctx's svc name and event title*/ 
					ui4_ini_sz = 4 * (REC_UTIL_GRB_ENTRY_OVERHEAD  + 64 + 20);
					break;
				case REC_DB_INFO_TYPE_EVENT_DETAIL_BY_ID:
					/*for edb correponding evctx's event text and event_ext_text*/
					ui4_ini_sz = 4 * (REC_UTIL_GRB_ENTRY_OVERHEAD  + 256 + 256);
					break;
#endif          
				case REC_DB_INFO_TYPE_RATING:
                    ui4_ini_sz = 4 * (REC_UTIL_GRB_ENTRY_OVERHEAD  + (16 + 8 * 4 + 20));
                    break;
                case REC_DB_INFO_TYPE_SCDB_INFO:
                    ui4_ini_sz = 4 * (REC_UTIL_GRB_ENTRY_OVERHEAD  + sizeof (SCDB_REC_T) * 4);
                    break;
                case REC_DB_INFO_TYPE_RRT_INFO:
                    ui4_ini_sz = 4 * (REC_UTIL_GRB_ENTRY_OVERHEAD + 32);
                    break;							
               default:
                   REC_UTIL_DBG_ERR(("rec_util_do_cfg: error key type!\r\n"));
                   i4_ret = REC_UTILR_INTERNAL_ERROR;
                   break;
               }
               i4_ret = rec_util_grb_init (&(pt_db->at_info_grb_tbl[ui1_idx]), 
                                            REC_UTIL_GRB_MEM_MODE_NORMAL, 
                                            ui4_ini_sz);
               if(REC_UTILR_GRB_OK != i4_ret)
               {
                   REC_UTIL_DBG_ERR(("rec_util_do_cfg: create info ring buffer failed(%d)\r\n",
                                      i4_ret));
                    i4_ret = REC_UTILR_INTERNAL_ERROR;
                    break;
               }
            }
           
            /*3.11 create signal change ring buffer*/
            for (ui1_idx = 0; ui1_idx < SIGNAL_CHG_TYPE_MAX; ++ui1_idx)
            {
                i4_ret = rec_util_grb_init (&(pt_db->at_signal_grb_chg_tbl[ui1_idx]),
                                             REC_UTIL_GRB_MEM_MODE_NORMAL,
                                             REC_UTIL_INFO_TBL_MAX_SZ);
                if (i4_ret != REC_UTILR_GRB_OK)
                {
                    REC_UTIL_DBG_ERR(("rec_util_do_cfg: create signal change ring buffer failed(%d)\r\n",
                                      i4_ret));
                    i4_ret = REC_UTILR_INTERNAL_ERROR;
                    break;
                
                }
            }
        }
        /*3.12 storage info*/
        pt_db->ui4_dev_access_sz = pt_cfg->t_strg_desc.ui4_min_access_sec_ns 
                                   * pt_cfg->t_strg_desc.ui4_sec_size;
        pt_db->ui4_strg_mem_align = pt_cfg->t_strg_desc.ui4_mem_align;
        
        /*3.13 Open file for recording :TS-FIFO*/
        i4_ret = x_fm_mfw_open( FM_ROOT_HANDLE, pt_cfg->t_strg_desc.ps_strg_path, 
                                FM_READ_WRITE, 0666, TRUE, &(pt_db->h_file));
        if (FMR_OK != i4_ret)
        {
            pt_db->h_file = NULL_HANDLE;
            REC_UTIL_DBG_ERR(("rec_util_do_cfg: fm open fail!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }

        pt_db->b_finalized = FALSE;
        pt_db->i4_tsb_write_status = REC_UTIL_REQR_OK;
        
        /*3.14 TS ring buffer info*/
        pt_db->ui8_tsb_cur_pos   = 0;
        pt_db->ui8_tsb_begin_pos = (UINT64)(pt_cfg->t_strg_desc.ui8_offset_sec_ns 
                                    * (UINT64)(pt_cfg->t_strg_desc.ui4_sec_size));
        i4_ret = x_fm_mfw_lseek(pt_db->h_file, 0, FM_SEEK_END, &ui8_file_end_pos);
        if (FMR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_do_cfg: fm seek fail!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }

        if (pt_db->ui8_tsb_begin_pos > ui8_file_end_pos)
        {
            REC_UTIL_DBG_ERR(("rec_util_do_cfg:file size is smaller then offset"
                              "(0x%llx, 0x%llx)!\r\n", 
                              ui8_file_end_pos, 
                              pt_db->ui8_tsb_begin_pos));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        /*file offset position need sector align*/
        rec_util_div_u64(pt_db->ui8_tsb_begin_pos, 
                         (UINT64)(pt_cfg->t_strg_desc.ui4_sec_size),
                         &ui8_temp);
        if (ui8_temp != 0)
        {
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_FILE_POS_NOT_SECTOR_ALIGN);
        }

        /*calculate small multi number of sector size(e.g.512) ansd TS packet size(192)*/
        ui8_small_multi_num = rec_util_get_smallest_multi_num(REC_UTIL_LOGIC_BLOCK_SIZE,
                                                              pt_cfg->t_strg_desc.ui4_sec_size);
        if(REC_UTIL_BOUND_TYPE_TIME == pt_cfg->e_bound_type)
        {
            ui8_ts_range_sz = pt_cfg->u_data.t_time_bound_info.ui8_tsb_sec_ns
                              * (UINT64)(pt_cfg->t_strg_desc.ui4_sec_size);
        }
        else
        {
            ui8_ts_range_sz = pt_cfg->u_data.t_space_bound_info.ui8_sec_ns;
            ui8_ts_range_sz *= pt_cfg->t_strg_desc.ui4_sec_size;
            if(REC_UTIL_SPACE_BOUND_UNLIMIT == pt_cfg->u_data.t_space_bound_info.ui8_sec_ns)
            {
                ui8_ts_range_sz = 0xFFFFFFFFFFFFFFFF;
            }
        }
        
        ui8_ts_range_sz = rec_util_div_u64(ui8_ts_range_sz, 
                                           ui8_small_multi_num,
                                           &ui8_temp);
        /*calculate TS ring buffer range size*/
        ui8_ts_range_sz *= ui8_small_multi_num;
        pt_db->ui8_tsb_block_cnt = rec_util_div_u64(ui8_ts_range_sz, 
                                                    REC_UTIL_LOGIC_BLOCK_SIZE,
                                                    NULL);
         pt_db->ui8_tsb_end_pos = (UINT64)(ui8_ts_range_sz + pt_db->ui8_tsb_begin_pos);
/*
        if (ui8_file_end_pos < pt_db->ui8_tsb_end_pos)
        {
            REC_UTIL_DBG_ERR(("rec_util_do_cfg:strg size is smaller then cfg size"
                              "(0x%llx, 0x%llx, 0x%llx)!\r\n",
                              ui8_file_end_pos, 
                              pt_db->ui8_tsb_begin_pos, 
                              pt_db->ui8_tsb_end_pos));
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
*/
        pt_db->t_pvr_tick_cb.ui4_lba_init     = pt_cfg->ui4_lba_init;
        pt_db->t_pvr_tick_cb.ui8_fifo_offset  = (pt_cfg->t_strg_desc.ui8_offset_sec_ns 
                                                 * (UINT64)(pt_cfg->t_strg_desc.ui4_sec_size));
        pt_db->t_pvr_tick_cb.ui4_fifo_pkt_num = (UINT32)(pt_db->ui8_tsb_block_cnt);
        
        /*3.15 this used to handle PVR flush case*/
        pt_db->pv_align_buf_addr = x_mem_alloc((SIZE_T)ui8_small_multi_num
                                                + (SIZE_T)(pt_db->ui4_strg_mem_align));
        if (NULL == pt_db->pv_align_buf_addr)
        {
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        pt_db->pui1_align_buffer = (UINT8*)REC_DB_ALIGN_ADDR_FORWARD(pt_db->pv_align_buf_addr,
                                                                     pt_db->ui4_strg_mem_align);
        pt_db->ui4_alig_buffer_sz = (UINT32)ui8_small_multi_num;


        /*3.16 calculate init TS buffer position and set file position there*/
        pt_db->ui4_avail_cur_lba = pt_cfg->ui4_lba_init;
        rec_util_div_u64((UINT64)pt_cfg->ui4_lba_init, 
                         (UINT64)pt_db->ui8_tsb_block_cnt,
                         &ui8_temp);
        ui8_init_lba_pos = ui8_temp * (UINT64)REC_UTIL_LOGIC_BLOCK_SIZE 
                            + pt_db->ui8_tsb_begin_pos;
        ui8_temp = ui8_init_lba_pos;
        rec_util_div_u64(ui8_temp, 
                        (UINT64)(pt_cfg->t_strg_desc.ui4_sec_size),
                        &ui8_temp);
        if (ui8_temp != 0)
        {
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_FILE_POS_NOT_SECTOR_ALIGN);
        }
        if (ui8_init_lba_pos <= pt_db->ui8_tsb_end_pos)
        {
            i4_ret = x_fm_mfw_lseek(pt_db->h_file, 
                                (INT64)ui8_init_lba_pos, 
                                FM_SEEK_BGN, 
                                &ui8_cur_pos);
        }
        else
        {
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        pt_db->ui8_tsb_cur_pos = ui8_cur_pos;
        if (i4_ret == FMR_OK)
        {
            REC_UTIL_DBG_INFO(("rec_util_do_cfg:Init seek "
                               "(offset=0x%llx, lba_init=%u, cur_pos=0x%08llx)\n",
                               pt_db->ui8_tsb_begin_pos,
                               pt_cfg->ui4_lba_init,
                               ui8_cur_pos));
        }
        
        /*calculate max LBA value(not max count!)*/
        pt_db->ui4_avail_max_lba = ((UINT32)(0xFFFFFFFF) / (UINT32)(pt_db->ui8_tsb_block_cnt)) 
                                    * (UINT32)(pt_db->ui8_tsb_block_cnt) - (UINT32)1;

        pt_db->b_has_valid_range = FALSE;
 
        pt_db->ui4_last_tick_num = (UINT32)0xFFFFFFFF;        
        
        /* init RFI context */
        i4_ret = rec_util_meta_rfi_init (& pt_db->t_ctx_rfi);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR(("rec_util_do_cfg: init rfi context fail (%d). \r\n", i4_ret));
            break;
        }         
        DLIST_INIT (&pt_db->t_running_req_list);
        DLIST_INIT (&pt_db->t_pending_req_list);
        DLIST_INIT (&pt_db->t_free_req_list);
        DLIST_INIT (&pt_db->t_timer_list_alloc);
        DLIST_INIT (&pt_db->t_timer_list_free);

        /*set db head info*/
        pt_db_head->pt_db_obj    = pt_db;
        /*set db cond*/
        pt_db_head->e_cond       = REC_UTIL_COND_READY;

        ui4_fm_req_timer_delay = ((pt_db->t_cfg.ui4_ms_tick_period)*9)/10;
        /*3.17 send cond change nfy msg*/
        x_memset(&t_msg, 0, sizeof(REC_UTIL_MSG_T));
        
        REC_UTIL_SHARE_DATA_T*          pt_share_data;
        pt_share_data =  x_mem_alloc(sizeof(REC_UTIL_SHARE_DATA_T));
        if(pt_share_data)
        {
            x_memset(pt_share_data, 0, sizeof(REC_UTIL_SHARE_DATA_T));
            pt_share_data->ps_strg_path               = pt_db->t_cfg.t_strg_desc.ps_strg_path;
            pt_share_data->t_tick_cb.pv_start_address = pt_db->t_pvr_tick_cb.pv_start_address;
            pt_share_data->t_tick_cb.pv_end_address   = pt_db->t_pvr_tick_cb.pv_end_address;
            pt_share_data->t_tick_cb.ui4_entry_num    = pt_db->t_pvr_tick_cb.ui4_entry_num;
            pt_share_data->t_tick_cb.ui4_entry_size   = pt_db->t_pvr_tick_cb.ui4_entry_size;
            pt_share_data->t_tick_cb.ui4_tick_period  = pt_db->t_pvr_tick_cb.ui4_tick_period;
            pt_share_data->t_tick_cb.ui4_lba_init     = pt_db->t_pvr_tick_cb.ui4_lba_init;
            pt_share_data->t_tick_cb.ui8_fifo_offset  = pt_db->t_pvr_tick_cb.ui8_fifo_offset;
            pt_share_data->t_tick_cb.ui4_fifo_pkt_num = pt_db->t_pvr_tick_cb.ui4_fifo_pkt_num;
            pt_share_data->t_tick_cb.ui4_max_vld_entry_num = pt_db->t_pvr_tick_cb.ui4_max_vld_entry_num;
            DBG_INFO(("\nRec util end addr %d,\n"
                            "start addr %d\n"
                            "entry num  %d\n"
                            "entry size %d\n"
                            "max entry num %d\n"
                            "tick period %d\n"
                            "lba int     %d\n"
                            "offset      %lld\n"
                            "packet num  %d\n",
                            pt_db->t_pvr_tick_cb.pv_end_address,
                            pt_db->t_pvr_tick_cb.pv_start_address,
                            pt_db->t_pvr_tick_cb.ui4_entry_num,
                            pt_db->t_pvr_tick_cb.ui4_entry_size,
                            pt_db->t_pvr_tick_cb.ui4_max_vld_entry_num,
                            pt_db->t_pvr_tick_cb.ui4_tick_period,
                            pt_db->t_pvr_tick_cb.ui4_lba_init,
                            pt_db->t_pvr_tick_cb.ui8_fifo_offset,
                            pt_db->t_pvr_tick_cb.ui4_fifo_pkt_num));   
        }
        
        
        t_msg.e_msg_type = REC_UTIL_MSG_NFY_COND_CHG;
        t_msg.u_data.t_cond_chg_nfy.h_rec_util = h_rec_util;
        t_msg.u_data.t_cond_chg_nfy.h_db_head  = pt_db_head->h_rec_db_head;
        t_msg.u_data.t_cond_chg_nfy.e_cond_new = pt_db_head->e_cond;
        t_msg.u_data.t_cond_chg_nfy.e_reason   = REC_UTIL_COND_CHG_REASON_SET_CONFIG_DONE;
        t_msg.u_data.t_cond_chg_nfy.ui4_data   = (int)pt_share_data;
        rec_util_send_msg(&t_msg);
        i4_cfg_result = REC_UTILR_OK;        
    }while(0);
    
    if (REC_UTILR_OK != i4_ret)
    {
        if (NULL != pt_db)
        {
            _rec_util_free_db(pt_db);
        }
        i4_cfg_result = REC_UTIL_REQR_FAIL;
    }
    
    /*4. send set cfg ack msg*/
    x_memset(&t_msg, 0, sizeof(REC_UTIL_MSG_T));
    t_msg.e_msg_type = REC_UTIL_MSG_NFY_SET_CFG_ACK;
    t_msg.u_data.t_set_cfg_ack.h_rec_util = h_rec_util;
    t_msg.u_data.t_set_cfg_ack.h_db_head  = pt_db_head->h_rec_db_head;
    t_msg.u_data.t_set_cfg_ack.i4_result  = i4_cfg_result;
    t_msg.u_data.t_set_cfg_ack.ui4_req_id = pt_do_cfg->ui4_set_cfg_req_id;
    rec_util_send_msg(&t_msg);
    
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: rec_util_do_write_timeout
 *
 * Description: This API will do abort check when received an abort msg nfy.
 *              
 * Inputs:  h_rec_util       Specify the db client obj handle.
 *          pt_write_timeout Specify the abort ack informaiton.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OK                Sucess
 *          REC_UTILR_INTERNAL_ERROR    Fail.
 ----------------------------------------------------------------------------*/    
INT32 rec_util_do_write_timeout(HANDLE_T                    h_rec_util,
                                REC_UTIL_WRITE_TIMEOUT_T*   pt_write_timeout)
{
    INT32                           i4_ret          = REC_UTILR_OK;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj = NULL;
    REC_UTIL_DB_HEAD_T*             pt_db_head = NULL;
    REC_UTIL_DB_T*                  pt_db;
    REC_UTIL_CLIENT_T*              pt_client_obj = NULL;
    REC_UTIL_REQ_LINK_T*            pt_req_link;
    BOOL                            b_found;

    if (NULL == pt_write_timeout || NULL_HANDLE == h_rec_util)
    {
        REC_UTIL_DBG_ERR(("rec_util_do_write_timeout: invalid argument!\r\n"));
        return REC_UTILR_INV_ARG;
    }

    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_do_db_write: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
    pt_db_head    = pt_client_obj->pt_rec_db_head;
    pt_db         = pt_db_head->pt_db_obj;

    b_found = FALSE;
    DLIST_FOR_EACH (pt_req_link, &pt_db->t_running_req_list, t_link)
    {
        if (pt_req_link->t_rec_util_req.ui4_req_id == pt_write_timeout->ui4_abort_req_id)
        {
            b_found = TRUE;
            break;
        }
    }
    
    if (!b_found)
    {
        /* this write req is already been removed */
        return REC_UTILR_OK;
    }

    if (REC_UTIL_REQ_FM_WRITE == pt_req_link->t_rec_util_req.e_req_type)
    {
        pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer->b_active = FALSE;
        pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = 
                REC_UTIL_REQR_TIME_OUT;
        //_rec_util_abort_fm_async(pt_req_link);
    }
    else
    {
        REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
    }
    
    i4_ret = _rec_util_do_fifo_req_process(h_rec_util,
                                           pt_db_head);
    if(REC_UTILR_OK != i4_ret)
    {
        REC_UTIL_DBG_ERR(("rec_util_do_write_timeout: do fifo write fail!\r\n"));
        return i4_ret;
    }
    
    return i4_ret;
}
  

/*-----------------------------------------------------------------------------
* Name:  rec_util_do_db_write
*
* Description: This function collect FM req ack and send ack to client. 
*
* Inputs:  h_rec_util          Specify the db client obj handle.
*          pt_fm_async_write   Specify the fm write ack informaiton.
*
* Outputs: pui4_req_id  Contains the request id.
*
* Returns: REC_UTILR_OK,             Success. 
*          Non zero value:        Fail
*
* Author : lianming lin(mtk40234)
*
* History:
*        (1)2009-03-07 : init by Lijun liu
*        (2)2009-05-07 : modified by Lianming Lin
----------------------------------------------------------------------------*/
INT32 rec_util_do_db_write(HANDLE_T                    h_rec_util, 
                                 REC_UTIL_FM_WRITE_INFO_T*   pt_fm_async_write,
                                 UINT32*                     pui4_req_id)
{
    INT32                           i4_ret = REC_UTILR_OK;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj = NULL;
    REC_UTIL_DB_HEAD_T*             pt_db_head = NULL;
    REC_UTIL_CLIENT_T*              pt_client_obj = NULL;
    REC_UTIL_DB_T*                  pt_db = NULL;
    REC_UTIL_REQ_LINK_T*            pt_req_link = NULL;
    REC_UTIL_FM_WRITE_REQ_T*        pt_fm_write_req = NULL;
    BOOL                            b_fm_write_req_found = FALSE;
    UINT8                           ui1_fm_req_num = 0;

    /*1.check parameters*/
    *pui4_req_id = REC_UTIL_INV_REQ_ID;
    if (NULL == pt_fm_async_write)
    {
        REC_UTIL_DBG_ERR(("rec_util_do_db_write: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }

    /*2.get client object*/
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_do_db_write: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
    pt_db_head = pt_client_obj->pt_rec_db_head;
    if (NULL == pt_db_head)
    {
        REC_UTIL_DBG_ERR(("rec_util_do_db_write: db head obj is NULL!\r\n"));
        return REC_UTILR_INTERNAL_ERROR;
    }
    pt_db = pt_db_head->pt_db_obj;
    if (NULL == pt_db)
    {
        REC_UTIL_DBG_ERR(("rec_util_do_db_write: db obj is NULL!\r\n"));
        return REC_UTILR_INTERNAL_ERROR;
    }
     
    /*3.search fm_write_req from running_req_list by req_id*/
    DLIST_FOR_EACH(pt_req_link, &(pt_db->t_running_req_list), t_link) 
    {
        if (REC_UTIL_REQ_FM_WRITE == pt_req_link->t_rec_util_req.e_req_type)
        {
            for (ui1_fm_req_num = 0; 
                 ui1_fm_req_num < REC_UTIL_FM_WRITE_MAX_NUM_PER_UTIL_REQ; 
                 ++ui1_fm_req_num)
            {
                if (pt_req_link->t_rec_util_req.u_data.t_fm_write_req.ah_req[ui1_fm_req_num]  
                     == pt_fm_async_write->h_req)
                {
                    b_fm_write_req_found = TRUE;
                    *pui4_req_id = pt_req_link->t_rec_util_req.ui4_req_id;
                    pt_fm_write_req = &(pt_req_link->t_rec_util_req.u_data.t_fm_write_req);
                    
                    pt_fm_write_req->ah_req[ui1_fm_req_num] = NULL_HANDLE;
                    --(pt_fm_write_req->ui1_fm_req_cnt);
                    break;
                }
            }
            if (b_fm_write_req_found)
            {
                break;
            }
        }
    }
     
    if (!b_fm_write_req_found 
        || (TRUE == pt_fm_write_req->b_fm_reqs_done))
    {
        REC_UTIL_DBG_ERR(("rec_util_do_db_write: not find fm write req(0x%08x) "
                          " or has been aborted!\r\n", 
                          pt_fm_async_write->h_req));
        return REC_UTILR_INV_REQ_ID;
    }

    if (FM_ASYNC_COND_WRITE_OK == pt_fm_async_write->e_async_cond)
    {
        REC_UTIL_DBG_INFO(("rec_util_do_db_write:write req[%d][%u] finished (OK) \n", 
                           pt_req_link->t_rec_util_req.ui4_req_id,
                           ui1_fm_req_num));           
    }
    /*write fail, to abort all write req handle*/
    else if (FM_ASYNC_COND_FAIL == pt_fm_async_write->e_async_cond)
    {
        x_dbg_stmt("{REC_UTIL} ERR: FM write req[%d][%u] finished (FAIL!) \n", 
                   pt_req_link->t_rec_util_req.ui4_req_id,
                   ui1_fm_req_num);
        pt_fm_write_req->i4_result = REC_UTIL_REQR_ABORTED;
        _rec_util_abort_fm_async(pt_req_link);
        
    }
    /*else if(FM_ASYNC_COND_EOF == pt_fm_async_write->e_async_cond)
        {
            x_dbg_stmt( "{REC_UTIL} ERR: FM write req[%d][%u] finished (STRG FULL!) \n", 
                         pt_req_link->t_rec_util_req.ui4_req_id,
                         ui1_fm_req_num);
            pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = 
                REC_UTIL_REQR_STRG_FULL;
            _rec_util_abort_fm_async(pt_req_link);
        }*/
    /*abort by APP or timeout or write fail*/
    else if((FM_ASYNC_COND_ABORT_FAIL == pt_fm_async_write->e_async_cond) 
            || (FM_ASYNC_COND_ABORT_OK == pt_fm_async_write->e_async_cond))
    {
        
    }
    else
    {
        REC_UTIL_DBG_ERR((  "rec_util_do_db_write: not support req cond(%d)\r\n", 
                            pt_fm_async_write->e_async_cond));
        i4_ret = REC_UTILR_NOT_SUPPORT;
    }

    if (0 == pt_fm_write_req->ui1_fm_req_cnt) 
    {
        if(!(pt_fm_write_req->b_fm_reqs_done))
        {
            if (pt_fm_write_req->pt_timer->b_active)
            {
                i4_ret = x_timer_stop (pt_fm_write_req->pt_timer->h_timer);
                if (i4_ret != OSR_OK)
                {
                    REC_UTIL_DBG_ERR (("Fail to stop timer (%d)\r\n", i4_ret));
                }
                pt_fm_write_req->pt_timer->b_active = FALSE;
            }

            pt_fm_write_req->b_fm_reqs_done = TRUE;
            pt_fm_write_req->i4_result      = REC_UTIL_REQR_OK;
        }
        /*send write ack by FIFO rule*/
        REC_UTIL_DBG_INFO((  "rec_util_do_db_write:do_fifo_write_ack "
                            "req(%d)!\r\n", *pui4_req_id));
        i4_ret = _rec_util_do_fifo_req_process(h_rec_util, pt_db_head);
        if(REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_do_db_write:do_fifo_write_ack fail!\r\n"));
        }
    }

    return i4_ret;
}

#ifdef REC_UTIL_AYNC_WRITE
 /*-----------------------------------------------------------------------------
  * Name:  rec_util_req_async_write_hdlr
  *
  * Description: This function process client sync write request however USB write is still async. 
  *
  * Inputs:  h_rec_util   Specifies the client handle
  *              pt_data      Contains the data and info to be written.
  *              pui4_req_id  Contains the request id.
  *
  *Outputs: NULL
  *
  * Returns: REC_UTILR_OK,          Success. 
  *               Non zero value:        Fail
  *
  * Author : lianming lin(mtk40234)
  *
  * History:
  *        (2)2010-08-03 : inital  by Lianming Lin
  -------------------------------------------------------------------------*/
INT32 rec_util_req_async_write_hdlr ( HANDLE_T                     h_rec_util,
                                              const REC_UTIL_TICK_DATA_T*  pt_data,
                                              UINT32                       ui4_req_id )
{
    INT32                   i4_ret        = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    REC_UTIL_DB_HEAD_T*     pt_db_head    = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj = NULL;
    REC_UTIL_DB_T*          pt_db         = NULL;
    VOID*                   pv_obj        = NULL;
    REC_UTIL_REQ_LINK_T*    pt_req_link   = NULL;
    UINT32                  ui4_req_total_sz;      /*req size*/
    UINT32                  ui4_pvr_avail_total_sz;/*all data need be read in PVR ring buffer*/
    UINT16                  ui2_blks;
    UINT8                   ui1_hdl_cnt;
    UINT8*                  pui1_last_packet;
     
    /*1.Check param; omit check due to internal inerface*/
     
    /*2.get client obejct*/
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr: invalid handle!\r\n"));
        REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
        return REC_UTILR_INV_HANDLE;
    }
 
    do
    {
        /*Get DB obejct*/
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head    = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
            return REC_UTILR_INTERNAL_ERROR;;
        }
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
            return REC_UTILR_INTERNAL_ERROR;;
        }
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
            return REC_UTILR_INTERNAL_ERROR;;
        }
        
        /*3.record write req num*/
        ++(pt_db->ui4_req_write_call_num);

        /*4.get a req*/
        i4_ret = _rec_util_get_req_link(&(pt_db->t_free_req_list), &pt_req_link);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr:_rec_util_get_req_link error!\r\n"));
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
            return REC_UTILR_INTERNAL_ERROR;;
        }
        pt_req_link->t_rec_util_req.e_req_type = REC_UTIL_REQ_FM_WRITE;
        pt_req_link->t_rec_util_req.h_rec_util = h_rec_util;
        pt_req_link->t_rec_util_req.ui4_req_id = ui4_req_id;

        pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer = NULL;
        i4_ret = _rec_util_alloc_timer_link(pt_db, 
                                 &(pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer));
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr:_rec_util_alloc_timer_link error!\r\n"));
            pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer = NULL;
            pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = 
                                    REC_UTIL_REQR_FAIL;
            break;
        }   
        pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = REC_UTIL_REQR_NOT_INIT;
        pt_req_link->t_rec_util_req.u_data.t_fm_write_req.b_fm_reqs_done = FALSE;
        x_memcpy(&(pt_req_link->t_rec_util_req.u_data.t_fm_write_req.t_tick_data), 
                 pt_data, sizeof(REC_UTIL_TICK_DATA_T));
          
        if (pt_db->b_finalized)
        {
            REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr: db finalized!\r\n"));
            i4_ret = REC_UTILR_REQ_FINALIZED;
            pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = 
                                    REC_UTIL_REQR_FINALIZED;
            break;
        }

        if ((REC_UTIL_REQR_REACH_BOUND == pt_db->i4_tsb_write_status) 
            || (REC_UTIL_REQR_STRG_FULL == pt_db->i4_tsb_write_status))
        {
            REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr: strg reach bound or full!\r\n"));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = 
                                    REC_UTIL_REQR_NOT_ALLOW;
            break;
        }
         
 
        /*5.If tick data is empty*/
        if ((0 == (pt_data->ui4_flags & REC_UTIL_TICK_DATA_FLAG_DATA_VALID)) 
             || (((pt_data->ui4_flags & REC_UTIL_TICK_DATA_FLAG_DATA_VALID) != 0) 
                 && (0 == pt_data->t_tick_data.ui4_pkt_count)))
        {
            pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pui1_written_data = NULL;
            pt_req_link->t_rec_util_req.u_data.t_fm_write_req.z_data_sz         = 0;
            pt_req_link->t_rec_util_req.u_data.t_fm_write_req.b_fm_reqs_done    = TRUE;
            pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = REC_UTIL_REQR_OK;

            /*5.1 insert into running req list to collect fm ack info*/
            DLIST_INSERT_TAIL(pt_req_link, &pt_db->t_running_req_list, t_link);
            REC_UTIL_DBG_INFO(("rec_util_req_async_write_hdlr: write req(%d), empty tick!\r\n",
                             pt_req_link->t_rec_util_req.ui4_req_id));

            /*5.2 ack write*/
            i4_ret = _rec_util_do_fifo_req_process(h_rec_util, pt_db_head);
            if (REC_UTILR_OK != i4_ret)
            {
                REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr:empty tick, do fifo ack fail!\r\n"));
            }
            i4_ret = REC_UTILR_OK;   /*need not ack failed msg to clients */
            break;
        }
        /*6.The following need write tick data to storage and metadata to memory*/
        else
        {
            UINT32   ui4_ofs_last_pkt;
            /*6.1 Check Tick data write pointer*/
            if (pt_data->t_tick_data.pui1_data != pt_db->t_pvr_buf_status.pui1_write_ptr)
            {
                REC_UTIL_DBG_ERR(("<<rec_util_req_async_write_hdlr: input data is error"
                                  "report pui1_data = 0x%08x\r\n"
                                  "pui1_write_ptr   = 0x%08x\r\n>>\r\n",
                                  pt_data->t_tick_data.pui1_data,
                                  pt_db->t_pvr_buf_status.pui1_write_ptr));
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = 
                                    REC_UTIL_REQR_FAIL;
                break;
            }

             /*6.2 get last packet timecode for flush usage*/
             ui4_ofs_last_pkt = (pt_data->t_tick_data.ui4_pkt_count - 1) * REC_UTIL_LOGIC_BLOCK_SIZE;
             if (((UINT32) (((UINT32) pt_db->t_pvr_buf_status.pui1_write_ptr) + ui4_ofs_last_pkt))
                  < ((UINT32) pt_db->t_pvr_buf_status.pui1_end_ptr))
             {
                 pui1_last_packet = (UINT8 *) (((UINT32) pt_db->t_pvr_buf_status.pui1_write_ptr) 
                                         + ui4_ofs_last_pkt);
             }
             else
             {
                 pui1_last_packet = (UINT8 *) (((UINT32) pt_db->t_pvr_buf_status.pui1_start_ptr)
                                         +  (((UINT32) pt_db->t_pvr_buf_status.pui1_write_ptr)
                                         +   ui4_ofs_last_pkt
                                         -  ((UINT32) pt_db->t_pvr_buf_status.pui1_end_ptr))); 
             }    
             
            pt_db->ui4_last_packet_timecode = GET_UINT32_FROM_PTR_BIG_END(pui1_last_packet);
             
            /*6.3. Check block number*/
            ui2_blks = PVR_TICK_DETAIL_GET_BLKS(&(pt_data->t_tick_data.pt_metadata->t_tick_detail));
            if ((UINT32)ui2_blks != pt_data->t_tick_data.ui4_pkt_count)
            {
                REC_UTIL_DBG_ERR(("<<rec_util_req_async_write_hdlr:input tick data error!"
                                  "ui4_blks_entries = 0x%08x\r\n"
                                  "ui4_pkt_count    = 0x%08x\r\n"
                                  "ui2_blks         = 0x%08x\r\n",
                                  pt_data->t_tick_data.pt_metadata->t_tick_detail.ui4_blks_entries, 
                                  pt_data->t_tick_data.ui4_pkt_count,
                                  ui2_blks));
                /*REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);*/
            }
         
            /*6.4 Update PVR buffer writer pointer for later check */
            ui4_req_total_sz = pt_data->t_tick_data.ui4_pkt_count * REC_UTIL_LOGIC_BLOCK_SIZE;
            /*Assume r=w is full . only at first time it means empty, when r=w again it means full*/
            if ((((UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr) > (UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr))
                 && ((UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr 
                   - (UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr))
                   < ui4_req_total_sz))
               ||  
               (((UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr) < (UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr))
                  && (  (UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr) 
                   - (UINT32)(pt_db->t_pvr_buf_status.pui1_start_ptr)
                   + (UINT32)(pt_db->t_pvr_buf_status.pui1_end_ptr)
                   - (UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr)
                   < ui4_req_total_sz)))
            {
                REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr:"
                                  "PVR ring buffer will be full and discard data!"
                                  "(read=0x%08x, write=0x%08x, data_sz=0x%08x)\r\n",
                                  pt_db->t_pvr_buf_status.pui1_read_ptr, 
                                  pt_db->t_pvr_buf_status.pui1_write_ptr,
                                  ui4_req_total_sz));
                REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_PVR_CTRL_OVERFLOW);
            }

            REC_UTIL_DBG_INFO(("rec_util_req_async_write_hdlr:req[%u] size = %u\r\n",
                               ui4_req_id,
                               ui4_req_total_sz));

            /*6.5 update write pointer whatever fm write result is*/
            pt_db->t_pvr_buf_status.pui1_write_ptr += ui4_req_total_sz;
            if (pt_db->t_pvr_buf_status.pui1_write_ptr >= pt_db->t_pvr_buf_status.pui1_end_ptr)
            {
                pt_db->t_pvr_buf_status.pui1_write_ptr =  (UINT8*)
                    (((UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr) 
                    - 
                    (UINT32)(pt_db->t_pvr_buf_status.pui1_end_ptr)) 
                    + 
                    (UINT32)(pt_db->t_pvr_buf_status.pui1_start_ptr));
            }
         
            /*6.5 calculate total write data size*/
            if (pt_db->t_pvr_buf_status.pui1_read_ptr > pt_db->t_pvr_buf_status.pui1_write_ptr)
            {
                ui4_pvr_avail_total_sz = (UINT32)(pt_db->t_pvr_buf_status.pui1_end_ptr )
                                         - (UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr)
                                         + (UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr) 
                                         - (UINT32)(pt_db->t_pvr_buf_status.pui1_start_ptr);
            }
            else
            {
                ui4_pvr_avail_total_sz = (UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr) - 
                                        (UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr);
            }
         
            /*6.6 set  fm_write_req for write tick data. the read pointer will update when data is written to USB*/
            pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pui1_written_data =
                        pt_db->t_pvr_buf_status.pui1_read_ptr;
            pt_req_link->t_rec_util_req.u_data.t_fm_write_req.z_data_sz = 0;

            /* every rec_uitl_request at most tree times fm_async_write, constriant: init lba,ts begin offset, tsb alignement*/
            for (ui1_hdl_cnt = 0; 
                ui1_hdl_cnt < REC_UTIL_FM_WRITE_MAX_NUM_PER_UTIL_REQ; 
                ui1_hdl_cnt++)
            {
                pt_req_link->t_rec_util_req.u_data.t_fm_write_req.ah_req[ui1_hdl_cnt] = NULL_HANDLE;
            }
         
            REC_UTIL_DBG_INFO(("<<rec_util_req_async_write_hdlr:req[%d]"
                               "pui1_read_ptr  = 0x%08x\r\n"
                               "pui1_write_ptr = 0x%08x\r\n>>\r\n", 
                               ui4_req_id,
                               pt_db->t_pvr_buf_status.pui1_read_ptr,
                               pt_db->t_pvr_buf_status.pui1_write_ptr));

            /*inser into running req list to collect fm ack info*/
            DLIST_INSERT_TAIL(pt_req_link, &(pt_db->t_running_req_list), t_link);

            /*6.7 check the left space size, if reach bound and is plain buf then 
                       cancle req and nfy to client*/
            if ((pt_db->ui8_tsb_end_pos - pt_db->ui8_tsb_cur_pos)  
                < (UINT64) ui4_pvr_avail_total_sz)
            {
                REC_UTIL_DBG_INFO(("<<rec_util_req_async_write_hdlr:req[%d]"
                                   "reach bound\r\n", ui4_req_id));
                         
                pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result =
                        REC_UTIL_REQR_REACH_BOUND;
            }

            if ((pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result == REC_UTIL_REQR_REACH_BOUND) 
                && (pt_db->t_cfg.e_buf_type == REC_UTIL_BUF_TYPE_PLAIN))
            {
                pt_req_link->t_rec_util_req.u_data.t_fm_write_req.b_fm_reqs_done = TRUE;
                i4_ret = _rec_util_do_fifo_req_process(h_rec_util, pt_db_head);
                if (REC_UTILR_OK != i4_ret)
                {
                    REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr:plain buf full, do fifo ack fail!\r\n"));
                }
                i4_ret = REC_UTILR_OK; /*need not ack failed msg to clients */
                break;    
            }
         
            /*6.8 start write timer*/
            i4_ret = x_timer_start( pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer->h_timer,
                                     ui4_fm_req_timer_delay, 
                                     X_TIMER_FLAG_ONCE, 
                                     _rec_util_write_timeout_nfy, 
                                     (VOID*) pt_req_link);
            if (OSR_OK != i4_ret)
            {
                REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr: start timer fail!\r\n"));
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = 
                                    REC_UTIL_REQR_FAIL;
                break;    
            }
            pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer->b_active = TRUE;
         
            /*6.9 write to storage*/
            i4_ret = _rec_util_strg_write(pt_db, ui4_pvr_avail_total_sz, pt_req_link);
            if (REC_UTILR_OK != i4_ret)
            {
                REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr: write req(%d) fail(%d)!\r\n",
                                  pt_req_link->t_rec_util_req.ui4_req_id, 
                                  i4_ret));
                if ((REC_UTILR_NOT_ALL_WRITE_OK == i4_ret) 
                    || (REC_UTILR_STRG_FULL == i4_ret))
                {
                    if (REC_UTILR_NOT_ALL_WRITE_OK == i4_ret)
                    {
                        pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = 
                                    REC_UTIL_REQR_ABORTED;
                    }
                    else
                    {
                        pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = 
                                    REC_UTIL_REQR_STRG_FULL;
                    }

                    _rec_util_abort_fm_async(pt_req_link);
                    i4_ret = _rec_util_do_fifo_req_process(h_rec_util, pt_db_head);
                    if (REC_UTILR_OK != i4_ret)
                    {
                        REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr: abort,do fifo ack fail!\r\n"));
                    }
                    i4_ret = REC_UTILR_OK; /*need not ack failed msg to clients */
                }  
                else
                {
                    pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = 
                                    REC_UTIL_REQR_FAIL;
                }
                break;
            }

            REC_UTIL_DBG_INFO(("rec_util_req_async_write_hdlr: write req(%d), write hdl num(%d)!\r\n",
                                pt_req_link->t_rec_util_req.ui4_req_id,
                                pt_req_link->t_rec_util_req.u_data.t_fm_write_req.ui1_fm_req_cnt));
        }

    }while(0);

    if (REC_UTILR_OK != i4_ret)
    {
        /*ack failed message to clients*/
        i4_ret = _rec_util_do_fifo_req_process(h_rec_util, pt_db_head);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr: failed message, do fifo ack fail!\r\n"));
        }
        REC_UTIL_DBG_ERR(("rec_util_req_async_write_hdlr: failed message ack!\r\n"));
    }
    else
    {
        /*normal......generally need not ack here*/
    }
    return i4_ret;
 }
#endif

/*-----------------------------------------------------------------------------
                     public functions implementation for external call 
  ----------------------------------------------------------------------------*/


 /*-----------------------------------------------------------------------------
 * Name: rec_util_open
 *
 * Description: This API opens a client of the record utility with specified name.
 *              
 * Inputs:  ps_name_db   Specifies the name of the record utility. Different names 
 *                       refer to different separated database.
 *          
 *          pf_nfy       Specifies the notification function for this client
 *
 *          pv_nfy_tag   Specifies the Tag
 *
 * Outputs: ph_rec_util  Contains the client handle
 *
 * Returns: REC_UTILR_INV_ARG           Invalid arguments
 *          REC_UTILR_OUT_OF_MEMORY     Out of memory.
 *          REC_UTILR_OUT_OF_HANDLE     Out of handles.
 ----------------------------------------------------------------------------*/
INT32 rec_util_open (const CHAR*                 ps_name_db,
                         rec_util_nfy_fct            pf_nfy,
                         VOID*                       pv_nfy_tag,
                         HANDLE_T*                   ph_rec_util)
{
    INT32                   i4_ret              = REC_UTILR_OK;
    REC_UTIL_DB_HEAD_T*     pt_db_head          = NULL;
    REC_UTIL_DB_T*          pt_db               = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj       = NULL;
    HANDLE_T                h_rec_util;
    
    /*check param*/
    if ((NULL == ps_name_db) || (NULL == ph_rec_util) || (NULL == pf_nfy) || (0 == x_strlen(ps_name_db)))
    {
        REC_UTIL_DBG_ERR(("rec_util_open: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_OPEN);
    
    i4_ret = _rec_util_open (ps_name_db,
                             pf_nfy,
                             pv_nfy_tag,
                             & h_rec_util,
                             & pt_client_obj,
                             & pt_db_head,
                             & pt_db );
    if (i4_ret == REC_UTILR_OK)
    {
        *ph_rec_util = h_rec_util;
    }
    else
    {
        REC_UTIL_DBG_ERR(("Fail to open client (%d). (L%d)\r\n", i4_ret, __LINE__));
    }

    rec_util_unlock_with_tag(REC_UTIL_TAG_OPEN);
    
    return i4_ret;
}
 /*-----------------------------------------------------------------------------
  * Name: rec_util_open_by_file
  *
  * Description: This API opens a client of the record utility according to the 
  *              specified file. If this API sucessfully return, the library will 
  *              ensure the client already link to the corresponding database with 
  *              REC_UTIL_COND_READY state.
  *              
  * Inputs:  ps_head_file  Specifies the head file that represents the recorded data set.
  *          
  *          pf_nfy        Specifies the notification function for this client
  *
  *          pv_nfy_tag    Specifies the Tag
  *
  * Outputs: ph_rec_util   Contains the client handle
  *
  * Returns: REC_UTILR_INV_ARG           Invalid arguments or file not exist
  *          REC_UTILR_OUT_OF_MEMORY     Out of memory.
  *          REC_UTILR_OUT_OF_HANDLE     Out of handles.
  ----------------------------------------------------------------------------*/

INT32 rec_util_open_by_file (const CHAR*                 ps_name_file,
                                 rec_util_nfy_fct            pf_nfy,
                                 VOID*                       pv_nfy_tag,
                                 HANDLE_T*                   ph_rec_util)
{
    INT32                   i4_ret              = REC_UTILR_OK;
    REC_UTIL_DB_HEAD_T*     pt_db_head          = NULL;
    REC_UTIL_DB_T*          pt_db               = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj       = NULL;
    FM_FILE_INFO_T          t_file_info         = {0};
    BOOL                    b_opened            = FALSE;
    HANDLE_T                h_rec_util;
    
    /*check param*/
    if ((NULL == ps_name_file) || (NULL == ph_rec_util) || (NULL == pf_nfy) || (0 == x_strlen(ps_name_file)))
    {
        REC_UTIL_DBG_ERR(("rec_util_open: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    i4_ret = x_fm_mfw_get_info_by_name (FM_ROOT_HANDLE, 
                                        ps_name_file,
                                        & t_file_info);
    if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_NO_ENTRY)
        {
            REC_UTIL_DBG_ERR(("File not exist (%d). (L%d)\r\n", i4_ret, __LINE__));
        }
        else
        {
            REC_UTIL_DBG_ERR(("Fail to get file info (%d). (L%d)\r\n", i4_ret, __LINE__));
        }
        return REC_UTILR_INV_ARG;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_OPEN_BY_FILE);

    do
    {    
        i4_ret = _rec_util_open (ps_name_file,
                                 pf_nfy,
                                 pv_nfy_tag,
                                 & h_rec_util,
                                 & pt_client_obj,
                                 & pt_db_head,
                                 & pt_db );
        if(i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR(("Fail to open rec_util (%d). (L%d)\r\n", i4_ret, __LINE__));
            return i4_ret;
        }

        b_opened = TRUE;
        
        switch (pt_db_head->e_cond)
        {
            case REC_UTIL_COND_TRANSITION:
                
                i4_ret = _rec_util_create_db_by_file(ps_name_file,
                                                     &pt_db);
                
                if(i4_ret != REC_UTILR_OK)
                {
                    REC_UTIL_DBG_ERR(("Create DB fail %d ,line %d",i4_ret,__LINE__));
                    break;
                }
                pt_db_head->pt_db_obj  =  pt_db;
                pt_db_head->e_cond     =  REC_UTIL_COND_READY;
                break;
                
            case REC_UTIL_COND_READY:
                
                if (pt_db)
                {
                    if (pt_db->e_profile != REC_UTIL_DB_PROFILE_LIMIT_1)
                    {
                        REC_UTIL_DBG_ERR(("DB already opened, but not with proper profile. (L%d)\r\n", __LINE__));
                        i4_ret = REC_UTILR_OP_NOT_ALLOWED;
                        break;
                    }
                }
                else
                {
                    REC_UTIL_DBG_ERR(("Internal error (L%d)\r\n", __LINE__));
                    i4_ret = REC_UTILR_INTERNAL_ERROR;
                    break;
                }
                break;
                
            default:                
                REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_CREATE_MSG_QUEUE);
        }
    }while(FALSE);

    rec_util_unlock_with_tag(REC_UTIL_TAG_OPEN_BY_FILE);
    
    if (i4_ret == REC_UTILR_OK)
    {
        *ph_rec_util = h_rec_util;
    }
    else
    {
        REC_UTIL_DBG_ERR(("Fail to open client (%d). (L%d)\r\n", i4_ret, __LINE__));
        
        if(b_opened)
        {
            rec_util_close(h_rec_util);
        }
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_close
 *
 * Description: This API closes a client of the record utility
 *              
 * Inputs:  h_rec_util  Specifies the client handle
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_HANDLE   Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 rec_util_close(HANDLE_T                    h_rec_util)
{
    INT32                   i4_ret             = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj             = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head         = NULL;
    REC_UTIL_DB_T*          pt_db              = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj      = NULL;
    REC_UTIL_HDL_LINK_T*    pt_hdl_link        = NULL;
    REC_UTIL_REQ_LINK_T*    pt_req_link        = NULL;
    REC_UTIL_REQ_T*         pt_req             = NULL;
    BOOL                    b_found            = FALSE;

    /*check param*/
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_close: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_CLOSE);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            REC_UTIL_DBG_ERR(("rec_util_close: pt_db_head is NULL!\r\n"));
            break;
        }
        
        /*delink h_rec_util from db head object's handle link*/
        if (FALSE == _rec_util_search_handle_link(h_rec_util, 
                                                  &(pt_db_head->t_client_handle_list), 
                                                  &pt_hdl_link))
        {
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            REC_UTIL_DBG_ERR(("rec_util_close: search hanle link error!\r\n"));
            break;
        }
        DLIST_REMOVE(pt_hdl_link, &pt_db_head->t_client_handle_list, t_link);
        x_mem_free(pt_hdl_link);
        pt_hdl_link = NULL;
        
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db) /*only has db head obj*/
        {
            break;
        }
        
        /*search set_cfg_req from db obj's running_req_list*/
        DLIST_FOR_EACH(pt_req_link, &pt_db->t_running_req_list, t_link) 
        {
            pt_req = &(pt_req_link->t_rec_util_req);
            if (pt_req->e_req_type == REC_UTIL_REQ_SET_CFG) 
            {
                b_found = TRUE;
                break;
            }
        }
        /*remove hdl from set cfg req hdl list, may be failed*/
        if (b_found)
        {
            --(pt_req->u_data.t_set_cfg_req.ui1_client_hdl_num);
        }     
    }while(0);
    
    if (REC_UTILR_OK == i4_ret)
    {
        /*delete client obj, free handle*/
        handle_free(h_rec_util, TRUE);
        
        /*check db head obj's handle list*/
        if (DLIST_IS_EMPTY(&pt_db_head->t_client_handle_list))
        {
            /*free db head object and db object*/
            i4_ret = _rec_util_free_db_head(pt_db_head);
        }
    }
    rec_util_unlock_with_tag(REC_UTIL_TAG_CLOSE);
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_get_cond
 *
 * Description: This API gets current condition.
 *              
 * Inputs:  h_rec_util  Specifies the client handle
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_HANDLE   Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 rec_util_get_cond (HANDLE_T                    h_rec_util,
                              REC_UTIL_COND_T*            pe_cond)
{
    INT32                   i4_ret = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj = NULL;

    /*check param*/
    if (NULL == pe_cond)
    {
        REC_UTIL_DBG_ERR(("rec_util_get_cond: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_cond: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_COND);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_cond: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        /*get db cond*/
        *pe_cond = pt_db_head->e_cond;
        
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_COND);
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_req_load_file (NOT SUPPORTED in current stage.)
 *
 * Description: This API sends a request to load the database from specified path.
 *              Note that rec_util will be in READ_ONLY mode when load by file
 *              
 * Inputs:  h_rec_util  Specifies the client handle
 *          ps_path     Specifies the full storage file path from root
 *          
 * Outputs: pui4_req_id  Contains the request id.
 *
 * Returns: REC_UTILR_INV_HANDLE   Invalid handle.
 *          REC_UTILR_NOT_SUPPORT  (NOT SUPPORTED in current stage.)
 ----------------------------------------------------------------------------*/
extern INT32 rec_util_req_load_file(HANDLE_T                    h_rec_util,
                                         const CHAR*                 ps_path,
                                         UINT64                      ui8_offset,
                                         UINT32*                     pui4_req_id)
{
    /*not support now*/
    return REC_UTILR_NOT_SUPPORT;
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_req_store_file  (NOT SUPPORTED in current stage.)
 *
 * Description: This API sends a request to store the database to specifed path
 *              
 * Inputs:  h_rec_util  Specifies the client handle
 *          ps_path     Specifies the full storage file path from root
 *          
 * Outputs: pui4_req_id  Contains the request id.
 *
 * Returns: REC_UTILR_INV_HANDLE   Invalid handle.
 *          REC_UTILR_INV_COND     Invalid condition, should be in READY condition
 *          REC_UTILR_NOT_SUPPORT  (NOT SUPPORTED in current stage.)
 ----------------------------------------------------------------------------*/
INT32 rec_util_req_store_file(HANDLE_T                    h_rec_util,
                              const CHAR*                 ps_path,
                              UINT64                      ui8_offset,
                              UINT32*                     pui4_req_id)
{
    /*not support now*/
    return REC_UTILR_NOT_SUPPORT;
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_set_finalized  
 *
 * Description: This API sends a request to set file handle condition
 *              
 * Inputs:  h_rec_util  Specifies the client handle
 *          b_finalized Specifies file handle condition
 *          
 *
 * Returns: REC_UTILR_INV_HANDLE    Invalid handle.
 *          REC_UTILR_INV_COND      Invalid condition, should be in READY condition
 ----------------------------------------------------------------------------*/

INT32 rec_util_set_finalized (
    HANDLE_T                    h_rec_util,
    BOOL                        b_finalized)
{
    INT32                   i4_ret          = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj          = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head      = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj   = NULL;
    REC_UTIL_DB_T*          pt_db           = NULL;
    REC_UTIL_REQ_LINK_T*    pt_req_link     = NULL;
    BOOL                    b_found         = FALSE;

    /*check param*/
    if (NULL_HANDLE == h_rec_util)
    {
        REC_UTIL_DBG_ERR(("rec_util_set_finalized: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_set_finalized: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_SET_FINALIZED);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head || NULL == pt_db_head->pt_db_obj)
        {
            REC_UTIL_DBG_ERR(("rec_util_set_finalized: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }

        if(REC_UTIL_COND_TRANSITION == pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR((  "rec_util_set_finalized: req not allow for "
                                "transition !\r\n"));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        pt_db = pt_db_head->pt_db_obj;
        
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        
        DLIST_FOR_EACH(pt_req_link, &(pt_db->t_running_req_list), t_link)
        {
            if(REC_UTIL_REQ_SET_FINALIZED == pt_req_link->t_rec_util_req.e_req_type)
            {
                b_found = TRUE;
                break;
            }
        }
        
        if(b_found)
        {
            if(pt_db->b_finalized == b_finalized)
            {
                i4_ret = REC_UTILR_OK;
                break;
            }
            else
            {
                /*roll back to old cond*/
                pt_db->b_finalized = b_finalized;
                DLIST_REMOVE(pt_req_link, &(pt_db->t_running_req_list), t_link);
                i4_ret = REC_UTILR_OK;                
                break;
            }
        }
        else
        {
            if(pt_db->b_finalized == b_finalized)
            {
                i4_ret = REC_UTILR_OK;
                break;
            }
            else
            {
                /*change cond first to break or allow the following req*/
                pt_db->b_finalized = b_finalized;
            }
        }
        
        /*send req to change file handle cond*/
        i4_ret = _rec_util_get_req_link(&(pt_db->t_free_req_list), &pt_req_link);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_set_finalized: _rec_util_get_req_link error!\r\n"));
            break;
        }

        pt_req_link->t_rec_util_req.e_req_type = REC_UTIL_REQ_SET_FINALIZED;
        pt_req_link->t_rec_util_req.h_rec_util = h_rec_util;
        pt_req_link->t_rec_util_req.ui4_req_id = _rec_util_gen_req_id();
        pt_req_link->t_rec_util_req.u_data.t_set_finalized_req.b_finalized = b_finalized;

        DLIST_INSERT_TAIL(pt_req_link, &pt_db->t_running_req_list, t_link);
        REC_UTIL_DBG_INFO(( "rec_util_set_finalized: set finalized(%d)!\r\n",
                            b_finalized?1:0));

        /*in case of no req write existing in running req list, so call func to 
        process this req to insure cond change*/
        i4_ret = _rec_util_do_fifo_req_process(h_rec_util, pt_db_head);
        if(REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_set_finalized: call fifo error!\r\n"));
            break;
        }
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_SET_FINALIZED);
    
    return i4_ret;

}

/*-----------------------------------------------------------------------------
 * Name: rec_util_set_addon_tick_flag  
 *
 * Description: This API sends a request to change tick flag mask code
 *              
 * Inputs:  h_rec_util  Specifies the client handle
 *          ui4_flag    Specifies new mask code
 *          
 *
 * Returns: REC_UTILR_INV_HANDLE    Invalid handle.
 *          REC_UTILR_INV_COND      Invalid condition, should be in READY condition
 ----------------------------------------------------------------------------*/

INT32 rec_util_set_addon_tick_flag (
    HANDLE_T                    h_rec_util,
    UINT32                      ui4_flag)
{
    INT32                   i4_ret          = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj          = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head      = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj   = NULL;
    REC_UTIL_DB_T*          pt_db           = NULL;
    REC_UTIL_REQ_LINK_T*    pt_req_link     = NULL;
    
    /*check param*/
    if (NULL_HANDLE == h_rec_util)
    {
        REC_UTIL_DBG_ERR(("rec_util_set_addon_tick_flag: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_set_addon_tick_flag: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_SET_ADDON_TICK_FLAG);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head || NULL == pt_db_head->pt_db_obj)
        {
            REC_UTIL_DBG_ERR(("rec_util_set_addon_tick_flag: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }

        if(REC_UTIL_COND_TRANSITION == pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR((  "rec_util_set_addon_tick_flag: req not allow for "
                                "transition !\r\n"));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_tick_count: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }

        pt_db = pt_db_head->pt_db_obj;

        /*send req to change addon tick flag mask code*/
        i4_ret = _rec_util_get_req_link(&(pt_db->t_free_req_list), &pt_req_link);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_set_addon_tick_flag: _rec_util_get_req_link error!\r\n"));
            break;
        }

        pt_req_link->t_rec_util_req.e_req_type = REC_UTIL_REQ_SET_ADDON_TICK_FLAG;
        pt_req_link->t_rec_util_req.h_rec_util = h_rec_util;
        pt_req_link->t_rec_util_req.ui4_req_id = _rec_util_gen_req_id();
        pt_req_link->t_rec_util_req.u_data.t_set_addon_tick_flag_req.ui4_mask_code 
                = ui4_flag;

        DLIST_INSERT_TAIL(pt_req_link, &pt_db->t_running_req_list, t_link);
        REC_UTIL_DBG_INFO(( "rec_util_set_addon_tick_flag: set addon tick flag(%d)!\r\n",
                            ui4_flag));

        /*in case of no req write existing in running req list, so call func to 
        process this req to insure change mask code*/
        i4_ret = _rec_util_do_fifo_req_process(h_rec_util, pt_db_head);
        if(REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_set_addon_tick_flag: call fifo error!\r\n"));
            break;
        }
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_SET_ADDON_TICK_FLAG);
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_req_set_config
 *
 * Description: This API sends a request to set config. This API makes the database 
 *              to enter TRANSITION state. Changing config also clears and 
 *              resets the whole database. A reset procedure will be triggered as follows
 *               (1) send REC_UTIL_NTFY_CODE_RESET_REQ to all clients
 *               (2) wait for all clients' permission on the reset request
 *               (3) reset done. 
 *              When reset done, the database will be re-setup according to the new config.
 *              After the whole config procedure is done, the database enters READY state.
 *              Then REC_UTIL_NTFY_CODE_SET_CONFIG_ACK will be sent to the request originator. 
 *              Note that REC_UTIL_NTFY_CODE_COND_CHANGED will be sent whenever the database 
 *              changes condition between READY and TRANSITION state.
 * Cond Confilct: (1)   e_idx_tbl_opt disable and e_empty_tk_opt keep
 *                (2)   e_idx_tbl_opt disable and e_buf_type ring
 *                (3)   e_idx_tbl_opt disable and e_bound_type time
 *
 * Inputs:  h_rec_util   Specifies the client handle
 *          pt_cfg       Contains the configuration info.
 *          
 * Outputs: pui4_req_id  Contains the reset request id.
 *
 * Returns: REC_UTILR_INV_HANDLE   Invalid handle.
 *          REC_UTILR_INV_COND     Invalid condition, should be in READY condition except
 *                                 for the initial config of a newly created database 
 *                                 (in TRANSITION condition).
 *          REC_UTILR_INV_ARG      Invalid arguments in pt_cfg.
 ----------------------------------------------------------------------------*/
INT32 rec_util_req_set_config (HANDLE_T                    h_rec_util,
                                     const REC_UTIL_CONFIG_T*    pt_cfg,
                                     UINT32*                     pui4_req_id)
{
    INT32                   i4_ret          = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj          = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head      = NULL;
    REC_UTIL_DB_T*          pt_db           = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj   = NULL;
    REC_UTIL_MSG_T          t_msg;
    REC_UTIL_REQ_LINK_T*    pt_req_link     = NULL;
    REC_UTIL_HDL_LINK_T*    pt_hdl_link     = NULL;
    UINT8                   ui1_hdl_num     = 0;
    REC_UTIL_CONFIG_T*      pt_new_cfg      = NULL;
    CHAR*                   ps_strg_path    = NULL;
    REC_UTIL_COND_T         e_old_cond;

    /*check param*/
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_req_set_config: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    if ((NULL == pt_cfg) || (NULL == pui4_req_id))
    {
        REC_UTIL_DBG_ERR(("rec_util_req_set_config: invalid param!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    if (NULL == pt_cfg->t_strg_desc.ps_strg_path)
    {
        REC_UTIL_DBG_ERR(("rec_util_req_set_config: strg path is NULL!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    if ((BRDCST_TYPE_ATSC != pt_cfg->e_brdcst_type) &&
        (BRDCST_TYPE_DVB != pt_cfg->e_brdcst_type)  &&
        (BRDCST_TYPE_DTMB != pt_cfg->e_brdcst_type) &&
        (BRDCST_TYPE_ISDB != pt_cfg->e_brdcst_type) &&
        (BRDCST_TYPE_UNKNOWN != pt_cfg->e_brdcst_type))
    {
        REC_UTIL_DBG_ERR(("rec_util_req_set_config: brdcst type error!\r\n"));
        return REC_UTILR_INV_ARG;
    }

    /*pvr case:  no index table disable,  there is not empty tick handle, just ack to client*/
    if ((REC_UTIL_IDX_TBL_OPT_DISABLE == pt_cfg->e_idx_tbl_opt) &&
        (REC_UTIL_EMPTY_TK_OPT_KEEP == pt_cfg->e_empty_tk_opt))
    {
        REC_UTIL_DBG_ERR((  "rec_util_req_set_config: e_idx_tbl_opt and "
                            "e_empty_tk_opt conflict!\r\n"));
        return REC_UTILR_INV_ARG;
    }  

    /*Pvr : No  index table, just  plain buffer , -----only take Space into account!!!
           1) if record timer which app starts expires APP will stop recording,
           2) if space reach full  notify to app to stop recording
       */
    if ((REC_UTIL_BUF_TYPE_RING == pt_cfg->e_buf_type) &&
        (REC_UTIL_IDX_TBL_OPT_DISABLE == pt_cfg->e_idx_tbl_opt))
    {
        REC_UTIL_DBG_ERR((  "rec_util_req_set_config: e_idx_tbl_opt and "
                            "buf type conflict!\r\n"));
        return REC_UTILR_INV_ARG;
    }

    /*Timeshift: need index table: -----take time and space into account!!!
           1) if tick full what should we do:  now just read automately , But:
               if ring ts buffer , may cause garbage data please refer to 3)
               if plain ts buffer, it seems ok... 
           2) tick index enable + limited plain ts buffer, if space bound reach just ack without write data and tick.
                It will notify app and the following write req just be acked without any write.
           3) tick index enable + ring ts buffer, if space full reach just return to the ts buffer begin :
               here a important limitation: data write first then update valid range, so may cause old start lba's 
               data is overridden(TS buffer pointer return to begin), _rec_util_recalc_ticks  try to handle this but
               still leaving a limitation, afterall data is written firstly!
      */
    if ((REC_UTIL_BOUND_TYPE_TIME == pt_cfg->e_bound_type) &&
        (REC_UTIL_IDX_TBL_OPT_DISABLE == pt_cfg->e_idx_tbl_opt))
    {
        REC_UTIL_DBG_ERR((  "rec_util_req_set_config: e_idx_tbl_opt and "
                            "bound type conflict!\r\n"));
        return REC_UTILR_INV_ARG;
    }

    if ((REC_UTIL_IDX_TBL_OPT_ENABLE == pt_cfg->e_idx_tbl_opt) &&
        ((REC_UTIL_BOUND_TYPE_SPACE == pt_cfg->e_bound_type) && 
         (REC_UTIL_SPACE_BOUND_TIME_UNLIMIT == 
         pt_cfg->u_data.t_space_bound_info.ui4_idx_tbl_ticks)))
    {
        REC_UTIL_DBG_ERR((  "rec_util_req_set_config: e_idx_tbl_opt enable and "
                            "bound type unlimited ticks!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    /*check lba_init which position should be setor alignment*/
    if (((pt_cfg->ui4_lba_init * REC_UTIL_LOGIC_BLOCK_SIZE) % pt_cfg->t_strg_desc.ui4_sec_size) != 0)
    {
        REC_UTIL_DBG_ERR(("rec_util_req_set_config: lba init not align!\r\n"));
        return REC_UTILR_INV_ARG;
    }

    REC_UTIL_DBG_INFO(("rec_util_req_set_config:(offset = 0x%llx, lba_init = 0x%llx)\r\n",
                       pt_cfg->t_strg_desc.ui8_offset_sec_ns 
                       * ((UINT64)(pt_cfg->t_strg_desc.ui4_sec_size)),
                       pt_cfg->ui4_lba_init));
    
    rec_util_lock_with_tag(REC_UTIL_TAG_SET_CFG);
    
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head    = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            REC_UTIL_DBG_ERR(("rec_util_req_set_config: pt_db_head is NULL!\r\n"));
            break;
        }

        /*if old cond is transition, db should be null;
          or old cond is ready, db should not be null*/
        if( ((REC_UTIL_COND_TRANSITION == pt_db_head->e_cond) && 
             (NULL != pt_db_head->pt_db_obj)) || 
            ((REC_UTIL_COND_READY == pt_db_head->e_cond) &&
             (NULL == pt_db_head->pt_db_obj)) )
        {
            REC_UTIL_DBG_ERR(("rec_util_req_set_config: invalid condition!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        if (pt_db_head->e_cond == REC_UTIL_COND_READY &&
            pt_db_head->pt_db_obj->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
       
        /*1. change cond to transition*/
        e_old_cond         = pt_db_head->e_cond;

        if(REC_UTIL_COND_READY == e_old_cond)
        {
            pt_db_head->e_cond = REC_UTIL_COND_TRANSITION;

            /*2. send cond chg nfy msg*/
            x_memset(&t_msg, 0, sizeof(REC_UTIL_MSG_T));
            t_msg.e_msg_type = REC_UTIL_MSG_NFY_COND_CHG;
            t_msg.u_data.t_cond_chg_nfy.h_rec_util = h_rec_util;
            t_msg.u_data.t_cond_chg_nfy.h_db_head  = pt_db_head->h_rec_db_head;
            t_msg.u_data.t_cond_chg_nfy.e_cond_new = pt_db_head->e_cond;
            t_msg.u_data.t_cond_chg_nfy.e_reason   = REC_UTIL_COND_CHG_REASON_SET_CONFIG_REQ;
            t_msg.u_data.t_cond_chg_nfy.ui4_data   = 0;
            rec_util_send_msg(&t_msg);
        }

        /*3. malloc and copy t_cfg*/
        *pui4_req_id = _rec_util_gen_req_id();    
        pt_new_cfg = (REC_UTIL_CONFIG_T*)x_mem_alloc(sizeof(REC_UTIL_CONFIG_T));
        if (NULL == pt_new_cfg)
        {
            REC_UTIL_DBG_ERR(("rec_util_req_set_config: new cfg obj alloc error!\r\n"));
            i4_ret = REC_UTILR_OUT_OF_MEMORY;
            break;
        }
        x_memcpy(pt_new_cfg, pt_cfg, sizeof(REC_UTIL_CONFIG_T));
        
        ps_strg_path = (CHAR*)x_mem_alloc(x_strlen(pt_cfg->t_strg_desc.ps_strg_path)+1);
        if (NULL == ps_strg_path)
        {
            REC_UTIL_DBG_ERR(("rec_util_req_set_config: new strg path alloc error!\r\n"));
            i4_ret = REC_UTILR_OUT_OF_MEMORY;
            break;
        }
        
        x_strcpy(ps_strg_path, 
                 pt_cfg->t_strg_desc.ps_strg_path);
                 
        pt_new_cfg->t_strg_desc.ps_strg_path = ps_strg_path;

        /*if old cond is transition*/
        if(REC_UTIL_COND_TRANSITION == e_old_cond)
        {
            /*4.1 do cfg*/
            x_memset(&t_msg, 0, sizeof(REC_UTIL_MSG_T));
            t_msg.e_msg_type = REC_UTIL_MSG_DO_CFG;
            t_msg.u_data.t_do_cfg.h_rec_util         = h_rec_util;
            t_msg.u_data.t_do_cfg.h_db_head          = pt_db_head->h_rec_db_head;
            t_msg.u_data.t_do_cfg.pt_cfg             = pt_new_cfg;
            t_msg.u_data.t_do_cfg.ui4_set_cfg_req_id = *pui4_req_id;
            rec_util_send_msg(&t_msg);
        }
        else/*if old cond is ready*/
        {      
            pt_db = pt_db_head->pt_db_obj;
            if (NULL == pt_db)
            {
                REC_UTIL_DBG_ERR(("rec_util_req_set_config: db obj is NULL!\r\n"));
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            }
            
            /*remove reqs from pending_req_list*/
            while (!DLIST_IS_EMPTY(&pt_db->t_pending_req_list))
            {
                pt_req_link = DLIST_HEAD(&pt_db->t_pending_req_list);
                
                if (REC_UTIL_REQ_FM_WRITE == pt_req_link->t_rec_util_req.e_req_type)
                {
                    _rec_util_free_timer_link(pt_db,
                                              pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer);
                }    
                
                _rec_util_remove_req_link(pt_req_link, 
                                          &pt_db->t_pending_req_list, 
                                          &pt_db->t_free_req_list);
            }

            /*check client handle*/
            pt_hdl_link = DLIST_HEAD(&pt_db_head->t_client_handle_list);
            if (NULL == pt_hdl_link)
            {
                REC_UTIL_DBG_ERR(("rec_util_req_set_config: client handle list is empty!\r\n"));
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            }

            /*set value for req link*/
            i4_ret = _rec_util_get_req_link(&(pt_db->t_free_req_list), &pt_req_link);
            if (REC_UTILR_OK != i4_ret)
            {
                REC_UTIL_DBG_ERR(("rec_util_req_set_config: _rec_util_get_req_link error!\r\n"));
                break;
            }
            
            pt_req_link->t_rec_util_req.e_req_type = REC_UTIL_REQ_SET_CFG;
            pt_req_link->t_rec_util_req.h_rec_util = h_rec_util;
            pt_req_link->t_rec_util_req.ui4_req_id = *pui4_req_id;
            pt_req_link->t_rec_util_req.u_data.t_set_cfg_req.ui4_cur_req_state = REC_UTIL_REQ_STATE_NONE;
            pt_req_link->t_rec_util_req.u_data.t_set_cfg_req.pt_cfg = pt_new_cfg;

            /*copy client handle list*/
            DLIST_FOR_EACH(pt_hdl_link, &pt_db_head->t_client_handle_list, t_link) 
            {                
                if (ui1_hdl_num >= REC_UTIL_REQ_CLIENT_HANLE_MAX_NUM)
                {
                    REC_UTIL_DBG_ERR(("rec_util_req_set_config: client handle too much!\r\n"));
                    i4_ret = REC_UTILR_INTERNAL_ERROR;
                    break;
                }
   
                ++ui1_hdl_num;               
            }
            if (REC_UTILR_OK != i4_ret)
            {
                break;
            }
            
            pt_req_link->t_rec_util_req.u_data.t_set_cfg_req.ui1_client_hdl_num = ui1_hdl_num;
            
            /*4.2 if only one client*/
            if ((NULL != pt_hdl_link) && (pt_hdl_link->h_rec_util == h_rec_util) &&             
                (NULL == DLIST_NEXT(pt_hdl_link, t_link)))
            {
                 /*insert req to list, and wait for excute, in case of there is 
                                none write req in list, so call do fifo at once to invoke do cfg*/
                 DLIST_INSERT_TAIL(pt_req_link, &pt_db->t_running_req_list, t_link);

                 i4_ret = _rec_util_do_fifo_req_process(h_rec_util, pt_db_head);
                 if(REC_UTILR_OK != i4_ret)
                 {
                    REC_UTIL_DBG_ERR((  "rec_util_req_set_config: single handle call "
                                        "fifo error!\r\n"));
                    break;
                 }
            }
            else /*4.3 has multi client handle, wait for permit*/
            {
                pt_req_link->t_rec_util_req.u_data.t_set_cfg_req.ui4_cur_req_state 
                    = REC_UTIL_SET_CFG_STATE_WAIT_PERMIT;
                    
                DLIST_INSERT_TAIL(pt_req_link, &pt_db->t_running_req_list, t_link);
                
                /*nfy all clients reset cfg*/
                x_memset(&t_msg, 0, sizeof(REC_UTIL_MSG_T));
                t_msg.e_msg_type = REC_UTIL_MSG_NFY_RESET_REQ;
                t_msg.u_data.t_reset_req_ack.h_rec_util         = h_rec_util;
                t_msg.u_data.t_reset_req_ack.h_db_head          = pt_db_head->h_rec_db_head;
                t_msg.u_data.t_reset_req_ack.ui4_reset_req_id   = *pui4_req_id;
                /*send msg for wait permit or do cfg*/
                rec_util_send_msg(&t_msg);
            }
        }
    }while(0);
    
    if (REC_UTILR_OK != i4_ret)
    {
        if (NULL != pt_new_cfg)
        {
            x_mem_free(pt_new_cfg);
        }
        
        if (NULL != ps_strg_path)
        {
            x_mem_free(ps_strg_path);
        }
        
        if (pt_req_link != NULL)
        {
            _rec_util_remove_req_link(pt_req_link, 
                                      &(pt_db_head->pt_db_obj->t_running_req_list), 
                                      &(pt_db_head->pt_db_obj->t_free_req_list));
        }
    }
    rec_util_unlock_with_tag(REC_UTIL_TAG_SET_CFG);
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_get_config
 *
 * Description: This API gets the current config info.
 *        
 * Inputs: h_rec_util  Specifies the client handle
 *          
 * Outputs: pt_cfg      Contains the current config info.
 *
 * Returns: REC_UTILR_INV_HANDLE   Invalid handle.
 *          REC_UTILR_INV_COND     Invalid condition, should be in READY condition
 ----------------------------------------------------------------------------*/
INT32 rec_util_get_config (HANDLE_T                    h_rec_util,
                                REC_UTIL_CONFIG_T*          pt_cfg)
{
    INT32                   i4_ret = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head = NULL;
    REC_UTIL_DB_T*          pt_db = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj = NULL;

    /*check param*/
    if (NULL == pt_cfg)
    {
        REC_UTIL_DBG_ERR(("rec_util_get_config: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_config: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_CFG);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_config: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        /*get db cofig*/
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_config: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }        
        /*get db obj*/
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_config: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break; 
        }        
        /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        pt_cfg->ui4_flags    = pt_db->t_cfg.ui4_flags;
        pt_cfg->e_buf_type   = pt_db->t_cfg.e_buf_type;
        pt_cfg->e_bound_type = pt_db->t_cfg.e_bound_type;
        
        if (pt_db->t_cfg.e_bound_type == REC_UTIL_BOUND_TYPE_TIME)
        {
            pt_cfg->u_data.t_time_bound_info.ui4_flags = pt_db->t_cfg.u_data.t_time_bound_info.ui4_flags;
            pt_cfg->u_data.t_time_bound_info.ui4_ticks = pt_db->t_cfg.u_data.t_time_bound_info.ui4_ticks;
        }
        else if (pt_db->t_cfg.e_bound_type == REC_UTIL_BOUND_TYPE_SPACE)
        {
            pt_cfg->u_data.t_space_bound_info.ui4_flags = pt_db->t_cfg.u_data.t_space_bound_info.ui4_flags;
            pt_cfg->u_data.t_space_bound_info.ui8_sec_ns = 
                pt_db->t_cfg.u_data.t_space_bound_info.ui8_sec_ns;
        }
        else
        {
            REC_UTIL_DBG_ERR(("rec_util_get_config: wrong bound type!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        pt_cfg->e_brdcst_type      = pt_db->t_cfg.e_brdcst_type;
        pt_cfg->e_empty_tk_opt     = pt_db->t_cfg.e_empty_tk_opt;
        pt_cfg->ui4_ms_tick_period = pt_db->t_cfg.ui4_ms_tick_period;
        pt_cfg->ui4_tk_extra       = pt_db->t_cfg.ui4_tk_extra;
        pt_cfg->ui4_lba_init       = pt_db->t_cfg.ui4_lba_init;
        x_memcpy(&(pt_cfg->t_strg_desc), 
                 &(pt_db->t_cfg.t_strg_desc), 
                 sizeof(REC_UTIL_STRG_DESC_T));

        pt_cfg->t_pvr_buf_info.pui1_buf_addr    = pt_db->t_cfg.t_pvr_buf_info.pui1_buf_addr;
        pt_cfg->t_pvr_buf_info.z_buf_len        = pt_db->t_cfg.t_pvr_buf_info.z_buf_len;
        pt_cfg->t_pvr_buf_status.pui1_read_ptr  = pt_db->t_cfg.t_pvr_buf_status.pui1_read_ptr;
        pt_cfg->t_pvr_buf_status.pui1_write_ptr = pt_db->t_cfg.t_pvr_buf_status.pui1_write_ptr;
        pt_cfg->t_sess_key                      = pt_db->t_cfg.t_sess_key;
        
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_CFG);
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_permit_reset_req
 *
 * Description: This API permits a previously received reset request.
 *              
 * Inputs:  h_rec_util  Specifies the client handle
 *          ui4_req_id  Specifies the request id sent along with REC_UTIL_NTFY_CODE_RESET_REQ 
 *                      notification.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_HANDLE        Invalid handle.
 *          REC_UTILR_INV_COND          Invalid condition, should be in READY condition
 *          REC_UTILR_INV_REQ_ID        Invalid request id. This must be a request id initialed for 
 *                                      database to send reset request.
 ----------------------------------------------------------------------------*/
INT32 rec_util_permit_reset_req (HANDLE_T                    h_rec_util,
                                        UINT32                      ui4_req_id)
{
    INT32                   i4_ret = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj = NULL;

    /*check param*/
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_permit_reset_req: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    if (REC_UTIL_INV_REQ_ID == ui4_req_id)
    {
        REC_UTIL_DBG_ERR(("rec_util_permit_reset_req: invalid req id!\r\n"));
        return REC_UTILR_INV_REQ_ID;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_PERMIT_RESET_REQ);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_permit_reset_req: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        i4_ret = _rec_util_check_permit_for_set_cfg(h_rec_util, ui4_req_id, pt_db_head);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_permit_reset_req: check permit error!\r\n"));
        }
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_PERMIT_RESET_REQ);
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_flush_pvr_buf_status
 *
 * Description: This API flushes the PVR buffer status kept in Record Utility.
 *              This API should be called only when no any running write requests, 
 *              otherwise it will return REC_UTILR_INV_OP. It's the caller's 
 *              responsibility to make sure all write requests are finished before 
 *              calling this API.
 *              
 * Inputs:  h_rec_util     Specifies the client handle
 *          pt_buf_status  Contains the latest PVR buffer status.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_HANDLE   Invalid handle.
 *          REC_UTILR_INV_COND     Invalid condition, should be in READY condition
 *          REC_UTILR_INV_OP       The API is called when some write requests are 
 *                                 still under running.
 *          REC_UTILR_INV_ARG      Invalid buffer status, e.g. out of the buffer 
 *                                 boundary defined by PVR_BUFFER_INFO_T
 ----------------------------------------------------------------------------*/
INT32 rec_util_flush_pvr_buf_status (HANDLE_T                    h_rec_util,
                                     const PVR_BUFFER_INFO_T*    pt_buf_info,
                                     const PVR_BUFFER_STATUS_T*  pt_buf_status)
{
    INT32                   i4_ret                = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj                = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head            = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj         = NULL;
    REC_UTIL_DB_T*          pt_db                 = NULL;
    UINT32                  ui4_small_multi_num;
    #if 0
    UINT32                  ui4_read              = 0;
    UINT32                  ui4_write             = 0;
    UINT64                  ui8_old_pos;
    UINT64                  ui8_new_pos;
    UINT64                  ui8_remainder;
    UINT64                  ui8_tsb_left_sz;
    UINT32                  ui4_null_packet_cnt;
    UINT32                  ui4_write_size;
    UINT8*                  pui1_null_packet_begin;
    UINT8*                  pui1_null_packet;
    UINT32                  ui4_idx;
    BOOL                    b_file_ptr_roll       = FALSE;/*indicate file ptr need roll back e.g. file ptr get to the TS range end*/
    UINT32                  ui4_last_packet_timecode;
    BOOL                    b_set_tick_bad_flag   = FALSE;
    #endif
    /*1.check param*/
    if ((NULL == pt_buf_status) 
         || (NULL == pt_buf_info)
         || (0 == pt_buf_info->z_buf_len))
    {    
        REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf_status: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }

    /*2.get client object*/
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf_status: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }

    /*3.update PVR info*/
    rec_util_lock_with_tag(REC_UTIL_TAG_FLUSH_PVR_BUF_STATUS);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf_status: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf_status: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf_status: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        /*4.Check PVR ring buffer*/
        ui4_small_multi_num = rec_util_get_smallest_multi_num(REC_UTIL_LOGIC_BLOCK_SIZE,
                                                              pt_db->t_cfg.t_strg_desc.ui4_sec_size);
        if ((pt_buf_info->z_buf_len % ui4_small_multi_num != 0)
             || ((UINT32)(pt_buf_info->pui1_buf_addr) % pt_db->ui4_strg_mem_align != 0) 
             || (((UINT32)(pt_buf_status->pui1_read_ptr) - (UINT32)(pt_buf_info->pui1_buf_addr))
                   % ui4_small_multi_num != 0)
             || ((UINT32)(pt_buf_info->pui1_buf_addr) != (UINT32)(pt_buf_status->pui1_read_ptr)))
        {
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_FILE_POS_NOT_SECTOR_AND_BLOCK_ALIGN);
        }

		#if 0
        /*5. check if there exist remainder data which are not read from PVR ring buffer
             reccord utility should be not writing or record session should not flush PVR*/
        /*   search running req list ,check if write req exist*/
        DLIST_FOR_EACH(pt_req_link, &(pt_db_head->pt_db_obj->t_running_req_list), t_link) 
        {
            if (REC_UTIL_REQ_FM_WRITE == pt_req_link->t_rec_util_req.e_req_type)
            {
                b_found = TRUE;
                break;
            }
        }
        if (b_found)
        {
            REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf_status:writing...\r\n"));
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
            i4_ret = REC_UTILR_OP_NOT_ALLOWED;
            break;
        }
        
        if (pt_db->ui4_last_write_left_sz != 0)
        {
            if (NULL == pt_db->pui1_align_buffer)
            {
                REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            }
            x_memset(pt_db->pui1_align_buffer, 0, pt_db->ui4_alig_buffer_sz);
            
            /*read remainder data from storage(has written down last time but not sector align)*/
            i4_ret = x_fm_mfw_lseek(pt_db->h_file, 
                                0, 
                                FM_SEEK_CUR, 
                                &ui8_old_pos);
            if (FMR_OK != i4_ret)
            {
                REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf_status: "
                                  "x_fm_lseek current pos fail(%d)!\r\n", 
                                  i4_ret));
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            } 

            i4_ret = x_fm_mfw_read(pt_db->h_file,
                              (VOID*)(pt_db->pui1_align_buffer),
                               pt_db->ui4_dev_access_sz,
                               &ui4_read);
            /*should not occur even if EOF*/
            if ((FMR_OK != i4_ret) || (ui4_read != pt_db->ui4_dev_access_sz))
            {
                REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf_status: "
                                  "x_fm_read (%d)!\r\n", 
                                  i4_ret));
                /*file position rewind*/
                i4_ret = x_fm_mfw_lseek(pt_db->h_file, 
                                    ((INT64)(ui8_old_pos)), 
                                    FM_SEEK_BGN, 
                                    &ui8_old_pos);
                if (FMR_OK != i4_ret)
                {
                    REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf_status: "
                                      "x_fm_lseek old current pos fail(%d)!\r\n", 
                                      i4_ret));
                    i4_ret = REC_UTILR_INTERNAL_ERROR;
                    break;
                }   
                b_set_tick_bad_flag = TRUE;
            }
            else
            {
                /*file position rewind*/
                i4_ret = x_fm_mfw_lseek(pt_db->h_file, 
                                        -((INT64)(pt_db->ui4_dev_access_sz)), 
                                        FM_SEEK_CUR, 
                                        &ui8_old_pos);
                if (FMR_OK != i4_ret)
                {
                    REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf_status: "
                                      "x_fm_lseek old current pos fail(%d)!\r\n", 
                                      i4_ret));
                    i4_ret = REC_UTILR_INTERNAL_ERROR;
                    break;
                } 
            }
            if (ui8_old_pos != pt_db->ui8_tsb_cur_pos)
            {
                REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            }
            REC_UTIL_DBG_INFO(("rec_util_flush_pvr_buf_status:ui8_old_pos=0x%08llx\r\n",
                               ui8_old_pos));
            
            /*caculate write how many null packet*/
            rec_util_div_u64(ui8_old_pos -pt_db->ui8_tsb_begin_pos, 
                            (UINT64)ui4_small_multi_num,
                            &ui8_remainder);
            ui4_write_size = ui4_small_multi_num - (UINT32)ui8_remainder;
            if ((0 == ui4_write_size)
                 || ((ui4_write_size % pt_db->ui4_dev_access_sz) != 0) 
                 || (((ui4_write_size - pt_db->ui4_last_write_left_sz)
                          % REC_UTIL_LOGIC_BLOCK_SIZE)
                     != 0))
            {
                REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_FILE_POS_NOT_SECTOR_ALIGN);
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            }
            REC_UTIL_DBG_INFO(("rec_util_flush_pvr_buf_status:ui4_write_size=0x%08x\r\n",
                               ui4_write_size));
            
            ui8_tsb_left_sz = pt_db->ui8_tsb_end_pos - pt_db->ui8_tsb_cur_pos;
            if (ui8_tsb_left_sz < ui4_write_size)
            {
                REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_FILE_POS_NOT_SECTOR_ALIGN);
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            }
            else if (ui8_tsb_left_sz == ui4_write_size)
            {
                b_file_ptr_roll = TRUE;
            }
            else
            {
                
            }
            
            /*calculate how many null packets*/
            ui4_null_packet_cnt = (ui4_write_size - pt_db->ui4_last_write_left_sz) 
                                   / REC_UTIL_LOGIC_BLOCK_SIZE;
            REC_UTIL_DBG_INFO(("<<rec_util_flush_pvr_buf_status:\r\n"
                               "ui8_tsb_left_sz     = 0x%08llx\r\n"
                               "b_file_ptr_roll     = 0x%08x\r\n"
                               "ui4_null_packet_cnt = 0x%08x\r\n>>\r\n",
                               ui8_tsb_left_sz,
                               b_file_ptr_roll,
                               ui4_null_packet_cnt));
            
            /*create NULL packet*/
            pui1_null_packet_begin = pt_db->pui1_align_buffer + pt_db->ui4_last_write_left_sz;
            pui1_null_packet = pui1_null_packet_begin;
            ui4_last_packet_timecode = pt_db->ui4_last_packet_timecode;
            for (ui4_idx = 0; ui4_idx < ui4_null_packet_cnt; ++ui4_idx)
            {
                ++ui4_last_packet_timecode;
                PUT_UINT32_TO_PTR_BIG_END(ui4_last_packet_timecode, (UINT32*)(pui1_null_packet)); /*NULL PID*/
                pui1_null_packet[0 + 4] = 0x47;   /*TS sync word*/
                PUT_UINT16_TO_PTR_BIG_END(0x1FFF, (UINT16*)(pui1_null_packet + 1 + 4)); /*NULL PID*/
                pui1_null_packet[3 + 4] = 0x10;   /*no adaption field*/
                
                pui1_null_packet += REC_UTIL_LOGIC_BLOCK_SIZE;
            }

            /*write remiander + null packets*/
            i4_ret = x_fm_mfw_lseek(pt_db->h_file, 
                                    0, 
                                    FM_SEEK_CUR, 
                                    &ui8_old_pos);
            if (FMR_OK != i4_ret)
            {
                REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf_status: "
                                  "x_fm_lseek current pos fail(%d)!\r\n", 
                                  i4_ret));
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            } 
            i4_ret = x_fm_mfw_write(pt_db->h_file,
                                    (VOID*)(pt_db->pui1_align_buffer),
                                     ui4_write_size,
                                     &ui4_write);
            /*should not occur even if EOF*/
            if ((FMR_OK != i4_ret) || (ui4_write != ui4_write_size))
            {
                REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf_status: "
                                  "x_fm_write fail(%d)!\r\n", 
                                  i4_ret));
                /*get new file position*/
                i4_ret = x_fm_mfw_lseek(pt_db->h_file, 
                                    (INT64)(ui8_old_pos + ui4_write_size), 
                                    FM_SEEK_BGN, 
                                    &ui8_new_pos);
                if (FMR_OK != i4_ret)
                {
                    REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf_status: "
                                      "x_fm_lseek new current pos fail(%d)!\r\n", 
                                      i4_ret));
                    i4_ret = REC_UTILR_INTERNAL_ERROR;
                    break;
                }
                b_set_tick_bad_flag = TRUE;                                  
            }
            
            /*get new file position*/
            i4_ret = x_fm_mfw_lseek(pt_db->h_file, 
                                    0, 
                                    FM_SEEK_CUR, 
                                    &ui8_new_pos);
            if (FMR_OK != i4_ret)
            {
                REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf_status: "
                                  "x_fm_lseek new current pos fail(%d)!\r\n", 
                                  i4_ret));
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            }

            /*notation: if read or write fail, the system should just run,
                           but perhaps the correspoding tick flag should be set to bad flag*/
            if(b_set_tick_bad_flag)
            {

            }
            
            if (TRUE == b_file_ptr_roll)
            {
                i4_ret = x_fm_mfw_lseek(pt_db->h_file, 
                                    (INT64)(pt_db->ui8_tsb_begin_pos), 
                                     FM_SEEK_BGN, 
                                     &ui8_new_pos);
                if (FMR_OK != i4_ret)
                {
                    REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf_status: "
                                      "x_fm_lseek TS range begin failed(%d)!\r\n", 
                                      i4_ret));
                    i4_ret = REC_UTILR_INTERNAL_ERROR;
                    break;
                }
            }
            pt_db->ui8_tsb_cur_pos = ui8_new_pos;
            REC_UTIL_DBG_INFO(("rec_util_flush_pvr_buf_status:ui8_new_pos=0x%08x\r\n",
                               ui8_new_pos));
            
            /*need not unlock this data because PVR has been flushed*/
            pt_db->ui4_last_write_left_sz = 0;

            /*lba + ui4_pkt_cnt > 0xFFFFFFFF*/
            if (pt_db->ui4_avail_cur_lba + ui4_null_packet_cnt > pt_db->ui4_avail_max_lba)
            {   
                pt_db->b_lba_wrap_over = TRUE;
            }
            else
            {
                pt_db->b_lba_wrap_over = FALSE;
            }
            
            pt_db->ui4_avail_cur_lba = (pt_db->ui4_avail_cur_lba + ui4_null_packet_cnt) 
                                        % (pt_db->ui4_avail_max_lba + 1);
            pt_db->ui4_last_packet_timecode = ui4_last_packet_timecode;
        }
        #endif
        /*reset cfg buf info*/
        pt_db->t_cfg.t_pvr_buf_status.pui1_read_ptr  = pt_buf_status->pui1_read_ptr;
        pt_db->t_cfg.t_pvr_buf_status.pui1_write_ptr = pt_buf_status->pui1_read_ptr;
        pt_db->t_cfg.t_pvr_buf_info.pui1_buf_addr    = pt_buf_info->pui1_buf_addr;
        pt_db->t_cfg.t_pvr_buf_info.z_buf_len        = pt_buf_info->z_buf_len;
        
        /*reset buf status*/
        pt_db->t_pvr_buf_status.pui1_read_ptr  = (UINT8*)(pt_buf_status->pui1_read_ptr);
        pt_db->t_pvr_buf_status.pui1_write_ptr = (UINT8*)(pt_buf_status->pui1_read_ptr);
        pt_db->t_pvr_buf_status.pui1_start_ptr = pt_buf_info->pui1_buf_addr;
        pt_db->t_pvr_buf_status.pui1_end_ptr   = pt_db->t_pvr_buf_status.pui1_start_ptr
                                                  +  pt_buf_info->z_buf_len;
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_FLUSH_PVR_BUF_STATUS);
    
    return i4_ret;    
}


/*-----------------------------------------------------------------------------
 * Name: _rec_util_flush_pvr_buf
 *
 * Description: This API flushes the PVR buffer at the just before stopping Record
 *              Utility,it will flush the left data in pvr buffer to storage.This API should
 *              be called only when no any running write requests, 
 *              otherwise it will return REC_UTILR_INV_OP. It's the caller's 
 *              responsibility to make sure all write requests are finished before 
 *              calling this API.
 *              
 * Inputs:  h_rec_util     Specifies the client handle
 *          
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_HANDLE   Invalid handle.
 *          REC_UTILR_INV_COND     Invalid condition, should be in READY condition
 *          REC_UTILR_INV_OP       The API is called when some write requests are 
 *                                 still under running.
 *          REC_UTILR_INV_ARG      Invalid buffer status, e.g. out of the buffer 
 *                                 boundary defined by PVR_BUFFER_INFO_T
----------------------------------------------------------------------------*/
static INT32 _rec_util_flush_pvr_buf (HANDLE_T   h_rec_util)
{
    INT32                   i4_ret                = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj                = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head            = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj         = NULL;
    REC_UTIL_DB_T*          pt_db                 = NULL;
    BOOL                    b_found               = FALSE;
    REC_UTIL_REQ_LINK_T*    pt_req_link           = NULL;
    UINT32                  ui4_small_multi_num;
    UINT32                  ui4_write             = 0;
    UINT64                  ui8_new_pos;
    UINT64                  ui8_remainder;
    UINT64                  ui8_tsb_left_sz;
    UINT32                  ui4_null_packet_cnt;
    UINT32                  ui4_write_size;
    UINT8*                  pui1_null_packet_begin;
    UINT8*                  pui1_null_packet;
    UINT32                  ui4_idx;
    BOOL                    b_file_ptr_roll       = FALSE;/*indicate file ptr need roll back e.g. file ptr get to the TS range end*/
    UINT32                  ui4_last_packet_timecode;
    BOOL                    b_set_tick_bad_flag   = FALSE;
    
	REC_UTIL_DBG_INFO(("Now we are in the flush pvr buffer function!, Caller(%d) \r\n", __builtin_return_address(0)));	
	
    /*1.get client object*/
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }

    /*2.valid params*/
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
		if ((pt_db->ui4_last_write_left_sz == 0)
            || (REC_UTIL_REQR_STRG_FULL   == pt_db->i4_tsb_write_status)
            || (REC_UTIL_REQR_REACH_BOUND == pt_db->i4_tsb_write_status))
		{
			REC_UTIL_DBG_INFO(("rec_util_flush_pvr_buf:all data have been write to storage\r\n"));
			i4_ret = REC_UTILR_OK;
			break;
		}

		/*judge if the file has been closed */
		if ( !x_handle_valid( pt_db->h_file))
		{
			REC_UTIL_DBG_INFO(("rec_util_flush_pvr_buf:file has been closed\r\n"));
			break;
		}
        /*3.Get the least mutli of storage access size and block size*/
        ui4_small_multi_num = rec_util_get_smallest_multi_num(REC_UTIL_LOGIC_BLOCK_SIZE,
                                                              pt_db->t_cfg.t_strg_desc.ui4_sec_size);

        /*4. check if there exist remainder data which are not read from PVR ring buffer
             reccord utility should be not writing or record session should not flush PVR*/
        /*   search running req list ,check if write req exist*/
        DLIST_FOR_EACH(pt_req_link, &(pt_db_head->pt_db_obj->t_running_req_list), t_link) 
        {
            if (REC_UTIL_REQ_FM_WRITE == pt_req_link->t_rec_util_req.e_req_type)
            {
                b_found = TRUE;
                break;
            }
        }
        if (b_found)
        {
            REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf:writing...\r\n"));
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
            i4_ret = REC_UTILR_OP_NOT_ALLOWED;
            break;
        }


        if (pt_db->ui4_last_write_left_sz != 0)
        {
            if (NULL == pt_db->pui1_align_buffer)
            {
                REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            }
            x_memset(pt_db->pui1_align_buffer, 0, pt_db->ui4_alig_buffer_sz);
            

            /*check the value match, it will not happen*/
            if ((((UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr) >= (UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr))
                && (pt_db->ui4_last_write_left_sz !=
                    pt_db->t_pvr_buf_status.pui1_write_ptr - pt_db->t_pvr_buf_status.pui1_read_ptr))
                ||
                (((UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr) < (UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr))
                && (pt_db->ui4_last_write_left_sz !=
                    (UINT32)(pt_db->t_pvr_buf_status.pui1_end_ptr) 
                    - (UINT32)(pt_db->t_pvr_buf_status.pui1_read_ptr) 
                    + (UINT32)(pt_db->t_pvr_buf_status.pui1_write_ptr)
                    - (UINT32)(pt_db->t_pvr_buf_status.pui1_start_ptr))))
            {
                REC_UTIL_DBG_ERR(("<<rec_util_flush_pvr_buf:rec_util_flush_data_dismatch.\r\n"
                                  "pvr read ptr    = 0x%08x\r\n"
                                  "pvr write ptr   = 0x%08x\r\n",
                                  pt_db->t_pvr_buf_status.pui1_read_ptr,
                                  pt_db->t_pvr_buf_status.pui1_write_ptr));
                //REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);                
                i4_ret = REC_UTILR_OP_NOT_ALLOWED;
                break;
            }

            /*copy the left data from pvr ring buffer to db's buffer*/
            x_memcpy(pt_db->pui1_align_buffer, pt_db->t_pvr_buf_status.pui1_read_ptr, pt_db->ui4_last_write_left_sz);

            /*caculate write how many null packet*/
            rec_util_div_u64(pt_db->ui8_tsb_cur_pos - pt_db->ui8_tsb_begin_pos, 
                             (UINT64)ui4_small_multi_num,
                             &ui8_remainder);
            ui4_write_size = ui4_small_multi_num - (UINT32)ui8_remainder;
            if ((0 == ui4_write_size)
                 || ((ui4_write_size % pt_db->ui4_dev_access_sz) != 0) 
                 || (((ui4_write_size - pt_db->ui4_last_write_left_sz)
                          % REC_UTIL_LOGIC_BLOCK_SIZE)
                     != 0))
            {
	            REC_UTIL_DBG_ERR(("rec_util flush pvr:\r\n"
								   "ui4_wrire_size  = 0x%08x\r\n"
								   "ui8_remainder   = 0x%08x\r\n",
								   ui4_write_size,
								   ui8_remainder));
                //REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_FILE_POS_NOT_SECTOR_ALIGN);
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            }
            REC_UTIL_DBG_INFO(("rec_util_flush_pvr_buf:ui4_write_size=0x%08x\r\n"
								"ui8_remainder                       =0x%08llx\r\n"
								"last_write_left_sz                  =0x%08x\r\n",
					            ui4_write_size,
				                ui8_remainder,
				                pt_db->ui4_last_write_left_sz));
            
            ui8_tsb_left_sz = pt_db->ui8_tsb_end_pos - pt_db->ui8_tsb_cur_pos;
            if (ui8_tsb_left_sz < ui4_write_size)
            {
                //REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_FILE_POS_NOT_SECTOR_ALIGN);
                i4_ret = REC_UTILR_INTERNAL_ERROR;
                break;
            }
            else if (ui8_tsb_left_sz == ui4_write_size)
            {
                b_file_ptr_roll = TRUE;
            }
            else
            {
            }
            
            /*calculate how many null packets*/
            ui4_null_packet_cnt = (ui4_write_size - pt_db->ui4_last_write_left_sz) 
                                  / REC_UTIL_LOGIC_BLOCK_SIZE;
            REC_UTIL_DBG_INFO(("<<rec_util_flush_pvr_buf:\r\n"
                               "ui8_tsb_left_sz     = 0x%08llx\r\n"
                               "b_file_ptr_roll     = 0x%08x\r\n"
                               "ui4_null_packet_cnt = 0x%08x\r\n>>\r\n",
                               ui8_tsb_left_sz,
                               b_file_ptr_roll,
                               ui4_null_packet_cnt));
            
            /*create NULL packet*/
            pui1_null_packet_begin = pt_db->pui1_align_buffer + pt_db->ui4_last_write_left_sz;
            pui1_null_packet = pui1_null_packet_begin;
            ui4_last_packet_timecode = pt_db->ui4_last_packet_timecode;
            for (ui4_idx = 0; ui4_idx < ui4_null_packet_cnt; ++ui4_idx)
            {
                ++ui4_last_packet_timecode;
                PUT_UINT32_TO_PTR_BIG_END(ui4_last_packet_timecode, (UINT32*)(pui1_null_packet)); /*NULL PID*/
                pui1_null_packet[0 + 4] = 0x47;   /*TS sync word*/
                PUT_UINT16_TO_PTR_BIG_END(0x1FFF, (UINT16*)(pui1_null_packet + 1 + 4)); /*NULL PID*/
                pui1_null_packet[3 + 4] = 0x10;   /*no adaption field*/
                
                pui1_null_packet += REC_UTIL_LOGIC_BLOCK_SIZE;
            }
			/*for CR DTV00326785 in linux case sync file position after async write*/
		    i4_ret = x_fm_mfw_lseek(pt_db->h_file, 0, FM_SEEK_CUR, &ui8_new_pos);
  	 	    if (FMR_OK != i4_ret)
    	 	{
    	 		REC_UTIL_DBG_ERR(("Fail to seek TS-FIFO cur pos (%d)!\r\n", i4_ret));
    	 		i4_ret = REC_UTILR_INTERNAL_ERROR;
    	 		break;
    	 	}
           
            i4_ret = x_fm_mfw_write(pt_db->h_file,
                                    (VOID*)(pt_db->pui1_align_buffer),
                                    ui4_write_size,
                                    &ui4_write);
			REC_UTIL_DBG_INFO(("<<rec_util_flush_pvr_buf:after the write the ui4_write=%d\r\n",ui4_write));

             /*should not occur even if EOF*/
            if ((FMR_OK != i4_ret) || (ui4_write != ui4_write_size))
            {
                REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf: "
                                  "x_fm_write fail(%d)!\r\n", 
                                  i4_ret));
                /*get new file position*/
                i4_ret = x_fm_mfw_lseek(pt_db->h_file, 
                                       (INT64)(pt_db->ui8_tsb_cur_pos + ui4_write_size), 
                                       FM_SEEK_BGN, 
                                       &ui8_new_pos);
                if (FMR_OK != i4_ret)
                {
                    REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf: "
                                      "x_fm_lseek new current pos fail(%d)!\r\n", 
                                      i4_ret));
                    i4_ret = REC_UTILR_INTERNAL_ERROR;
                    break;
                }
                b_set_tick_bad_flag = TRUE;                                  
            }

            /*notation: if read or write fail, the system should just run,
                           but perhaps the correspoding tick flag should be set to bad flag*/
            if(b_set_tick_bad_flag)
            {

            }
            
            if (TRUE == b_file_ptr_roll)
            {
                i4_ret = x_fm_mfw_lseek( pt_db->h_file, 
                                        (INT64)(pt_db->ui8_tsb_begin_pos), 
                                         FM_SEEK_BGN, 
                                         &ui8_new_pos);
                if (FMR_OK != i4_ret)
                {
                    REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf: "
                                      "x_fm_lseek TS range begin failed(%d)!\r\n", 
                                      i4_ret));
                    i4_ret = REC_UTILR_INTERNAL_ERROR;
                    break;
                }
            }
            else
            {
					/*get new file position*///
			    i4_ret = x_fm_mfw_lseek(pt_db->h_file, 
										0, 
										FM_SEEK_CUR, 
										&ui8_new_pos);
				REC_UTIL_DBG_INFO(("rec_util_flush_pvr_buf:seek ui8_new_pos=0x%08llx\r\n",
		                           ui8_new_pos));
				if (FMR_OK != i4_ret)
				{
					REC_UTIL_DBG_ERR(("rec_util_flush_pvr_buf: "
									  "x_fm_lseek new current pos fail(%d)!\r\n", 
									  i4_ret));
					i4_ret = REC_UTILR_INTERNAL_ERROR;
					break;
				}
				if (ui8_new_pos != pt_db->ui8_tsb_cur_pos + ui4_write_size )
				{
					REC_UTIL_DBG_INFO(("rec_util_flush_pvr_buf:seek pos dismatch with the calc" 
                                        "ui8_new_pos=0x%08x\r\n"
                                        "tsb_cur_pos=0x%08x\r\n",
                                        ui8_new_pos,
                                        pt_db->ui8_tsb_cur_pos + ui4_write_size));
					//REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
                    i4_ret = REC_UTILR_INTERNAL_ERROR;
				}
            	                
            }
            pt_db->ui8_tsb_cur_pos = ui8_new_pos;
            REC_UTIL_DBG_INFO(("rec_util_flush_pvr_buf:ui8_new_pos=0x%08x\r\n",
                               ui8_new_pos));
            
            /*need not unlock this data because PVR has been flushed*/
            pt_db->ui4_last_write_left_sz = 0;

            /*lba + ui4_pkt_cnt > 0xFFFFFFFF*/
            if (pt_db->ui4_avail_cur_lba + ui4_null_packet_cnt > pt_db->ui4_avail_max_lba)
            {   
                pt_db->b_lba_wrap_over = TRUE;
            }
            else
            {
                pt_db->b_lba_wrap_over = FALSE;
            }
            
            pt_db->ui4_avail_cur_lba = (pt_db->ui4_avail_cur_lba + ui4_null_packet_cnt) 
                                        % (pt_db->ui4_avail_max_lba + 1);
            pt_db->ui4_last_packet_timecode = ui4_last_packet_timecode;
        }
        
    }while(0);
        
    return i4_ret;    
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_flush_pvr_buf
 *
 * Description: This API flushes the PVR buffer at the just before stopping Record
 *              Utility,it will flush the left data in pvr buffer to storage.This API should
 *              be called only when no any running write requests, 
 *              otherwise it will return REC_UTILR_INV_OP. It's the caller's 
 *              responsibility to make sure all write requests are finished before 
 *              calling this API.
 *              
 * Inputs:  h_rec_util     Specifies the client handle
 *          
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_HANDLE   Invalid handle.
 *          REC_UTILR_INV_COND     Invalid condition, should be in READY condition
 *          REC_UTILR_INV_OP       The API is called when some write requests are 
 *                                 still under running.
 *          REC_UTILR_INV_ARG      Invalid buffer status, e.g. out of the buffer 
 *                                 boundary defined by PVR_BUFFER_INFO_T
----------------------------------------------------------------------------*/
INT32  rec_util_flush_pvr_buf (HANDLE_T   h_rec_util)
{
	INT32                   i4_ret = REC_UTILR_OK;
	rec_util_lock_with_tag(REC_UTIL_TAG_FLUSH_PVR_BUF);
    i4_ret = _rec_util_flush_pvr_buf(h_rec_util);
	rec_util_unlock_with_tag(REC_UTIL_TAG_FLUSH_PVR_BUF);
	return  i4_ret;
	
}
/*-----------------------------------------------------------------------------
 * Name: rec_util_get_shared_data
 *
 * Description: This API gets the shared data, including, e.g., the memory location of metadata 
 *             and the physical location of storage.
 *        
 * Inputs:  h_rec_util    Specifies the client handle
 *        
 * Outputs: pt_share_data  Contains the metadata and storage info
 *
 * Returns: REC_UTILR_INV_HANDLE   Invalid handle.
 *          REC_UTILR_INV_COND     Invalid condition, should be in READY condition
 ----------------------------------------------------------------------------*/
INT32 rec_util_get_shared_data (HANDLE_T                    h_rec_util,
                                REC_UTIL_SHARE_DATA_T*      pt_share_data)
{
    INT32                   i4_ret = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj = NULL;
    REC_UTIL_DB_T*          pt_db = NULL;

    /*check param*/
    if (NULL == pt_share_data)
    {    
        REC_UTIL_DBG_ERR(("rec_util_get_shared_data: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_shared_data: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_SHARED_DATA);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_shared_data: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        /*get db shared data*/
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_shared_data: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_shared_data: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }        
        /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }

        if(REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_shared_data: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }
        
        pt_share_data->ps_strg_path               = pt_db->t_cfg.t_strg_desc.ps_strg_path;
        pt_share_data->t_tick_cb.pv_start_address = pt_db->t_pvr_tick_cb.pv_start_address;
        pt_share_data->t_tick_cb.pv_end_address   = pt_db->t_pvr_tick_cb.pv_end_address;
        pt_share_data->t_tick_cb.ui4_entry_num    = pt_db->t_pvr_tick_cb.ui4_entry_num;
        pt_share_data->t_tick_cb.ui4_entry_size   = pt_db->t_pvr_tick_cb.ui4_entry_size;
        pt_share_data->t_tick_cb.ui4_tick_period  = pt_db->t_pvr_tick_cb.ui4_tick_period;
        pt_share_data->t_tick_cb.ui4_lba_init     = pt_db->t_pvr_tick_cb.ui4_lba_init;
        pt_share_data->t_tick_cb.ui8_fifo_offset  = pt_db->t_pvr_tick_cb.ui8_fifo_offset;
        pt_share_data->t_tick_cb.ui4_fifo_pkt_num = pt_db->t_pvr_tick_cb.ui4_fifo_pkt_num;
        pt_share_data->t_tick_cb.ui4_max_vld_entry_num = pt_db->t_pvr_tick_cb.ui4_max_vld_entry_num;
        
        REC_UTIL_DBG_INFO(("rec_util_get_shared_data:\r\n"
                           "pv_start_address = %s\r\n"
                           "pv_start_address = 0x%x\r\n"
                           "pv_end_address   = 0x%x\r\n"
                           "ui4_entry_num    = %u\r\n"
                           "ui4_entry_size   = %u\r\n"
                           "ui4_tick_period  = %u\r\n"
                           "ui4_lba_init     = %u\r\n"
                           "ui8_fifo_offset  = %llu\r\n"
                           "ui4_fifo_pkt_num = %u\r\n",
                           pt_share_data->ps_strg_path,
                           pt_share_data->t_tick_cb.pv_start_address,
                           pt_share_data->t_tick_cb.pv_end_address,
                           pt_share_data->t_tick_cb.ui4_entry_num,
                           pt_share_data->t_tick_cb.ui4_entry_size,
                           pt_share_data->t_tick_cb.ui4_tick_period,
                           pt_share_data->t_tick_cb.ui4_lba_init,
                           pt_share_data->t_tick_cb.ui8_fifo_offset,
                           pt_share_data->t_tick_cb.ui4_fifo_pkt_num));
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_SHARED_DATA);
    
    return i4_ret;
}

/*----------------------------------------------------------------------
 * Name: rec_util_get_strg_info_by_path
 *
 * Description: This API gets storage info by strg path, e.g. the memory alignment of storage.
 *        
 * Inputs:  ps_strg_path   Specifies the strg path
 *        
 * Outputs: pt_strg_info   Contains the storage info.
 *
 * Returns: REC_UTILR_INV_HANDLE   Invalid handle.
 *          REC_UTILR_OK           Success.
 ---------------------------------------------------------------------------*/
INT32 rec_util_get_strg_info_by_path (const CHAR*                 ps_strg_path,
                                      REC_UTIL_STRG_INFO_T*       pt_strg_info)
{
    INT32                   i4_ret = REC_UTILR_OK;
    FM_FS_INFO_T            t_fs_info;

    /*check param*/
    if (NULL == pt_strg_info)
    {
        REC_UTIL_DBG_ERR(("rec_util_get_strg_info_by_path: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_STRG_INFO_BY_PATH);
    do
    {
        i4_ret = x_fm_get_fs_info(FM_ROOT_HANDLE, ps_strg_path, &t_fs_info);
        if (FMR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_strg_info_by_path: get fs info fail!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        pt_strg_info->ui4_access_mem_align = t_fs_info.ui4_alignment;
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_STRG_INFO_BY_PATH);
    
    return i4_ret;    
}

/*----------------------------------------------------------------------
 * Name: rec_util_get_strg_info
 *
 * Description: This API gets storage info, e.g. the memory alignment of storage.
 *        
      
 * Inputs:  h_rec_util  Specifies the client handle
 *        
 * Outputs: pt_strg_info   Contains the storage info.
 *
 * Returns: REC_UTILR_INV_HANDLE   Invalid handle.
 *          REC_UTILR_INV_COND     Invalid condition, should be in READY condition
 ---------------------------------------------------------------------------*/
INT32 rec_util_get_strg_info (HANDLE_T                    h_rec_util,
                              REC_UTIL_STRG_INFO_T*       pt_strg_info)
{
    INT32                   i4_ret = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj = NULL;
    REC_UTIL_DB_T*          pt_db = NULL;
    FM_FS_INFO_T            t_fs_info;

    /*check param*/
    if (NULL == pt_strg_info)
    {
        REC_UTIL_DBG_ERR(("rec_util_get_strg_info: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_strg_info: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_STRG_INFO);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_strg_info: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        /*get db strg info*/
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_strg_info: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_strg_info: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }        
        /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        
        i4_ret = x_fm_get_fs_info(FM_ROOT_HANDLE, pt_db->t_cfg.t_strg_desc.ps_strg_path,
                                  &t_fs_info);
        if (FMR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_strg_info: get fs info fail!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        pt_strg_info->ui4_access_mem_align = t_fs_info.ui4_alignment;
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_STRG_INFO);
    
    return i4_ret;    
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_get_valid_range
 *
 * Description: This API gets the current valid range for safe playback.
 *              
 * Inputs:  h_rec_util      Specifies the client handle
 *          
 * Outputs: pt_valid_range  Contains the valid range in the tick index array of metadata.
 *
 * Returns: REC_UTILR_INV_HANDLE   Invalid handle.
 *          REC_UTILR_INV_COND     Invalid condition, should be in READY condition
 *          REC_UTILR_INV_RANGE    No valid range available, e.g. current valid range is empty.
 ----------------------------------------------------------------------------*/
INT32 rec_util_get_valid_range (HANDLE_T                    h_rec_util,
                                MM_BUF_RANGE_INFO_T*        pt_valid_range)
{
    INT32                   i4_ret          = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj          = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head      = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj   = NULL;
    REC_UTIL_DB_T*          pt_db           = NULL;

    /*check param*/
    if (NULL == pt_valid_range)
    {
        REC_UTIL_DBG_ERR(("rec_util_get_valid_range: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_valid_range: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_VALID_RANGE);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head    = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_valid_range: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        /*get db valid range*/
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_valid_range: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_valid_range: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }        
        /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }

        if(REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_valid_range: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }
        
        if (!pt_db->b_has_valid_range)
        {
            REC_UTIL_DBG_INFO(("rec_util_get_valid_range: not has valid range now!\r\n"));
            i4_ret = REC_UTILR_INV_RANGE;
            break;
        }
        
        pt_valid_range->ui4_start = pt_db->t_valid_range.ui4_start;
        pt_valid_range->ui4_end   = pt_db->t_valid_range.ui4_end;
        
        REC_UTIL_DBG_INFO(("rec_util_get_valid_range: valid range[%u,%u]\r\n",
                           pt_valid_range->ui4_start,
                           pt_valid_range->ui4_end));
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_VALID_RANGE);
    
    return i4_ret;    
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_get_valid_range_tick_index
 *
 * Description: This API gets the current valid range and the correspond start and end  tick index  for safe playback.
 *              
 * Inputs:  h_rec_util      Specifies the client handle
 *          
 * Outputs: pt_valid_range  Contains the valid range in the tick index array of metadata.
 *		   pt_start_tick_index  Contains the start tick index.
 *              pt_end_tick_index   Contains the end tick index.
 *
 * Returns: REC_UTILR_INV_HANDLE   Invalid handle.
 *          REC_UTILR_INV_COND     Invalid condition, should be in READY condition
 *          REC_UTILR_INV_RANGE    No valid range available, e.g. current valid range is empty.
 ----------------------------------------------------------------------------*/
INT32 rec_util_get_valid_range_tick_index(HANDLE_T                    h_rec_util,
                                MM_BUF_RANGE_INFO_T*        pt_valid_range,
                                PVR_TICK_INDEX_T*           pt_start_tick_index,
                                PVR_TICK_INDEX_T*           pt_end_tick_index)
{
    INT32                   i4_ret          = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj          = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head      = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj   = NULL;
    REC_UTIL_DB_T*          pt_db           = NULL;
	UINT32                  ui4_data_sz;

    /*check param*/
    if ((NULL == pt_valid_range) 
        || (!pt_start_tick_index)
        || (!pt_end_tick_index))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_valid_range: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_valid_range: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_VALID_RANGE_TICK_INDEX);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head    = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_valid_range_tick_index: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        /*get db valid range*/
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_valid_range_tick_index: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_valid_range_tick_index: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }

        if(REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_valid_range_tick_index: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }
        
        if (!pt_db->b_has_valid_range)
        {
            REC_UTIL_DBG_INFO(("rec_util_get_valid_range_tick_index: not has valid range now!\r\n"));
            i4_ret = REC_UTILR_INV_RANGE;
            break;
        }
        
        pt_valid_range->ui4_start = pt_db->t_valid_range.ui4_start;
        pt_valid_range->ui4_end   = pt_db->t_valid_range.ui4_end;


		ui4_data_sz = sizeof(PVR_TICK_INDEX_T);
		
		i4_ret =  rec_util_rque_copy_entry(pt_db->h_pvr_tick_idx_tbl,
                                           pt_db->t_valid_range.ui4_start,
                                           (UINT8*)pt_start_tick_index,
                                           &ui4_data_sz);
        if (i4_ret != REC_UTILR_RQUE_OK)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_valid_range_tick_index:get start tick index(%u) error(%d)!\r\n",
                              pt_db->t_valid_range.ui4_start,
                              i4_ret));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
		
		i4_ret =  rec_util_rque_copy_entry(pt_db->h_pvr_tick_idx_tbl,
										   pt_db->t_valid_range.ui4_end,
										   (UINT8*)pt_end_tick_index,
										   &ui4_data_sz);
		if (i4_ret != REC_UTILR_RQUE_OK)
		{
			REC_UTIL_DBG_ERR(("rec_util_get_valid_range_tick_index:get end tick index(%u) error(%d)!\r\n",
							  pt_db->t_valid_range.ui4_end,
							  i4_ret));
			i4_ret = REC_UTILR_INTERNAL_ERROR;
			break;
		}

        REC_UTIL_DBG_INFO(("rec_util_get_valid_range_tick_index: valid range[%u,%u]\r\n",
                           pt_valid_range->ui4_start,
                           pt_valid_range->ui4_end));
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_VALID_RANGE_TICK_INDEX);
    
    return i4_ret;    
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_get_tick_count
 *
 * Description: This API return the call num of rec_util_req_write.
 *              
 * Inputs:  h_rec_util      Specifies the client handle
 *          
 * Outputs: pui4_count      Contains the rec_util_req_write call num.
 *
 * Returns: REC_UTILR_INV_HANDLE   Invalid handle.
 *          REC_UTILR_INV_COND     Invalid condition, should be in READY condition
 *          REC_UTILR_INV_RANGE    No valid range available, e.g. current valid range is empty.
 ----------------------------------------------------------------------------*/

extern INT32 rec_util_get_tick_count (
    HANDLE_T                    h_rec_util,
    UINT32*                     pui4_count)
{
    INT32                   i4_ret          = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj          = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head      = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj   = NULL;
    REC_UTIL_DB_T*          pt_db           = NULL;

    /*check param*/
    if (NULL == pui4_count)
    {
     REC_UTIL_DBG_ERR(("rec_util_get_tick_count: invalid arguments!\r\n"));
     return REC_UTILR_INV_ARG;
    }
    
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
     REC_UTIL_DBG_ERR(("rec_util_get_tick_count: invalid handle!\r\n"));
     return REC_UTILR_INV_HANDLE;
    }

    rec_util_lock_with_tag(REC_UTIL_TAG_GET_TICK_COUNT);
    do
    {
         pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
         pt_db_head    = pt_client_obj->pt_rec_db_head;
         if (NULL == pt_db_head)
         {
             REC_UTIL_DBG_ERR(("rec_util_get_tick_count: db head obj is NULL!\r\n"));
             i4_ret = REC_UTILR_INTERNAL_ERROR;
             break;
         }
         
         /*get db valid range*/
         if (REC_UTIL_COND_READY != pt_db_head->e_cond)
         {
             REC_UTIL_DBG_ERR(("rec_util_get_tick_count: db obj is not READY!\r\n"));
             i4_ret = REC_UTILR_INV_COND;
             break;
         }
         
         pt_db = pt_db_head->pt_db_obj;
         if (NULL == pt_db)
         {
             REC_UTIL_DBG_ERR(("rec_util_get_tick_count: db obj is NULL!\r\n"));
             i4_ret = REC_UTILR_INTERNAL_ERROR;
             break;
         }         
         /* check profile */
         if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
         {
             REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
             i4_ret = REC_UTILR_REQ_NOT_ALLOW;
             break;
         }
         *pui4_count = pt_db->ui4_req_write_call_num;
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_TICK_COUNT);

    return i4_ret;  

}



/*-----------------------------------------------------------------------------
 * Name:  rec_util_req_write
 *
 * Description: This function process client write request. 
 *
 * Inputs:  h_rec_util   Specifies the client handle
 *          pt_data      Contains the data and info to be written.
 *
 * Outputs: pui4_req_id  Contains the request id.
 *
 * Returns: REC_UTILR_OK,          Success. 
 *          Non zero value:        Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *        (1)2009-03-07 : init by Lijun Liu
 *        (2)2009-05-07 : modified by Lianming Lin
  *       (3)2010-08-02 : modified by Lianming Lin
 -------------------------------------------------------------------------*/
INT32 rec_util_req_write (HANDLE_T                     h_rec_util,
                              const REC_UTIL_TICK_DATA_T*  pt_data,
                              UINT32*                      pui4_req_id)
{
#ifdef REC_UTIL_AYNC_WRITE
    return _rec_util_req_aync_write(h_rec_util, pt_data, pui4_req_id);
#else
    return _rec_util_req_sync_write(h_rec_util, pt_data, pui4_req_id);
#endif
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_abort_req
 *
 * Description: This API sends a request to abort the specified request. Note that only the 
 *              request originator can abort the request.
 *              
 * Inputs:  h_rec_util  Specifies the client handle
 *          ui4_req_id  Specifies the request id.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_HANDLE      Invalid handle.
 *          REC_UTILR_INV_COND        Invalid condition, should be in READY condition
 *          REC_UTILR_OP_NOT_ALLOWED  This request is not allowed to be aborted.
 *          REC_UTILR_INV_REQ_ID      Invalid request id, e.g., the request already done.
 ----------------------------------------------------------------------------*/
INT32 rec_util_abort_req (HANDLE_T                    h_rec_util,
                          UINT32                      ui4_req_id)
{
    INT32                   i4_ret = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj = NULL;
    REC_UTIL_DB_T*          pt_db = NULL;
    REC_UTIL_REQ_LINK_T*    pt_req_link = NULL;
    BOOL                    b_found = FALSE;
    BOOL                    b_req_is_pending = FALSE;
 
    /*check param*/
    if (REC_UTIL_INV_REQ_ID == ui4_req_id)
    {
        REC_UTIL_DBG_ERR(("rec_util_abort_req: invalid req id!\r\n"));
        return REC_UTILR_INV_REQ_ID;
    }
    
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_abort_req: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_ABORT_REQ);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_abort_req: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        /*get db valid range*/
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_abort_req: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_abort_req: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }        
        /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        /*search req from pending_req_list by req_id*/
        DLIST_FOR_EACH(pt_req_link, &pt_db->t_pending_req_list, t_link) 
        {
            if (pt_req_link->t_rec_util_req.ui4_req_id == ui4_req_id)
            {
                REC_UTIL_DBG_INFO(("rec_util_abort_req: req is in pending_req_list!\r\n"));
                b_found = TRUE;
                b_req_is_pending = TRUE;
                break;
            }
        }
        
        if (!b_found)
        {
            /*search req from running_req_list by req_id*/
            DLIST_FOR_EACH(pt_req_link, &pt_db->t_running_req_list, t_link) 
            {
                if (pt_req_link->t_rec_util_req.ui4_req_id == ui4_req_id)
                {
                    REC_UTIL_DBG_INFO(("rec_util_abort_req: req is in running_req_list!\r\n"));
                    b_found = TRUE;
                    break;
                }
            }
        }
        
        if (!b_found)
        {
            REC_UTIL_DBG_ERR(("rec_util_abort_req: this req is done!\r\n"));
            i4_ret = REC_UTILR_INV_REQ_ID;
            break;
        }
        
        /*check this req's type*/
        if (REC_UTIL_REQ_SET_CFG == pt_req_link->t_rec_util_req.e_req_type)
        {
            REC_UTIL_DBG_ERR(("rec_util_abort_req: set_cfg_req can't abort!\r\n"));
            i4_ret = REC_UTILR_OP_NOT_ALLOWED;
            break;
        }
        
        /*check h_req_util*/
        if (h_rec_util != pt_req_link->t_rec_util_req.h_rec_util)
        {
            REC_UTIL_DBG_ERR(("rec_util_abort_req: h_rec_util is not the request originator!\r\n"));
            i4_ret = REC_UTILR_OP_NOT_ALLOWED;
            break;
        }
        /*abort this req,abort the pending req*/
        if (b_req_is_pending)
        {
            if (REC_UTIL_REQ_FM_WRITE == pt_req_link->t_rec_util_req.e_req_type)
            {
                _rec_util_free_timer_link (pt_db, pt_req_link->t_rec_util_req.u_data.t_fm_write_req.pt_timer);
            }
            _rec_util_remove_req_link(pt_req_link, &(pt_db->t_pending_req_list), &(pt_db->t_free_req_list));
        }
        /*abort the running req*/
        else
        {
            if (REC_UTIL_REQ_FM_WRITE == pt_req_link->t_rec_util_req.e_req_type)
            {
                pt_req_link->t_rec_util_req.u_data.t_fm_write_req.i4_result = 
                    REC_UTIL_REQR_ABORTED;
                _rec_util_abort_fm_async(pt_req_link);
                i4_ret = _rec_util_do_fifo_req_process(pt_req_link->t_rec_util_req.h_rec_util, 
                                                    pt_db_head);
                if(REC_UTILR_OK != i4_ret)
                {
                    REC_UTIL_DBG_ERR(("rec_util_abort_req: do fifo ack fail!\r\n"));
                    i4_ret = REC_UTILR_OP_NOT_ALLOWED;
                    break;
                }
            }
            else
            {
                REC_UTIL_DBG_ERR(("rec_util_abort_req: not support now!\r\n"));
                i4_ret = REC_UTILR_INV_REQ_ID;
                break;
            }
        }
    }while(0);
    
    rec_util_unlock_with_tag(REC_UTIL_TAG_ABORT_REQ);
    
    return i4_ret;    
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_get_lba_by_pvr_w_ptr
 *
 * Description: This API converts PVR write pointer to the current LBA.
 *              
 * Inputs:  h_rec_util      Specifies the client handle
 *          pui1_pvr_w_ptr  Contains PVR's write pointer.
 *          
 * Outputs: pui4_lba        Contains the LBA
 *
 * Returns: REC_UTILR_INV_HANDLE  Invalid handle.
 *          REC_UTILR_INV_COND    Invalid condition, should be in READY condition
 *          REC_UTILR_INV_ARG     Invalid PVR write pointer (e.g. not align to logical buffer unit (192 bytes).)
 ----------------------------------------------------------------------------*/
INT32 rec_util_get_lba_by_pvr_buf_status (HANDLE_T                    h_rec_util,
                                          const PVR_BUFFER_STATUS_T*  pt_pvr_buf_st,
                                          UINT32*                     pui4_lba)
{
    INT32                   i4_ret = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj = NULL;
    REC_UTIL_DB_T*          pt_db = NULL;
    UINT32                  ui4_data_size;
    UINT32                  ui4_lba;

    /*check param*/
    if (NULL == pt_pvr_buf_st)
    {
        REC_UTIL_DBG_ERR(("rec_util_get_lba_by_pvr_buf_status: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_lba_by_pvr_buf_status: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_LBA_BY_STATUS);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_lba_by_pvr_buf_status: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_lba_by_pvr_buf_status: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_lba_by_pvr_buf_status: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        if (pt_pvr_buf_st->pui1_write_ptr >= pt_db->t_pvr_buf_status.pui1_write_ptr)
        {
            ui4_data_size = (UINT32)(pt_pvr_buf_st->pui1_write_ptr 
                                     - pt_db->t_pvr_buf_status.pui1_write_ptr);
        }
        else
        {
            ui4_data_size = (UINT32)(pt_db->t_cfg.t_pvr_buf_info.z_buf_len  
                                     + pt_pvr_buf_st->pui1_write_ptr 
                                     - pt_db->t_pvr_buf_status.pui1_write_ptr);
        }
        
        if (0 != (ui4_data_size % REC_UTIL_LOGIC_BLOCK_SIZE))
        {
            REC_UTIL_DBG_ERR(("rec_util_get_lba_by_pvr_buf_status: size error!\r\n"));
            i4_ret = REC_UTILR_INV_ARG;
            break;
        }
        
        ui4_lba = (pt_db->ui4_avail_cur_lba + ui4_data_size / REC_UTIL_LOGIC_BLOCK_SIZE - 1)
                   % (pt_db->ui4_avail_max_lba + 1);
        *pui4_lba = ui4_lba;
        
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_LBA_BY_STATUS);
    
    return i4_ret;    
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_update_event_info
 *
 * Description: This API updates a type of event info at the specified LBA.
 *              This API follows the rule: All types of information must 
 *              always be updated in increased time-order, or, LBA-order. 
 *              Therefore, the LBA ui4_lba must always be equal or newer 
 *              than the LBA of the last updated event info. 
 *              On the other hand, once ui4_lba is in the valid range, the notification 
 *              REC_UTIL_NTFY_CODE_VALID_RANGE_INFO_UPDATED will be sent to all clients.
 *              
 * Inputs:  h_rec_util        Specifies the client handle
 *          ui4_lba           Specifies the LBA that the event info update occurs at. 
 *                            It must always be equal or larger than the LBA of 
 *                            the last updated event info.
 *          e_key_type        Specifies the event info key type.
 *          z_event_info_len  Specifies the event info length.
 *          pv_event_info     Contains the event info data.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_ARG        Invalid arguments.
 *          REC_UTILR_INV_HANDLE     Invalid handle.
 *          REC_UTILR_INV_COND       Invalid condition, should be in READY condition
 *          REC_UTILR_INV_LBA_ORDER  The LBA does not follow the rule: All types of info
 *                                   must always be updated in increased LBA-order.
 ----------------------------------------------------------------------------*/
INT32 rec_util_update_event_info (HANDLE_T                   h_rec_util,
	                                  UINT32                      ui4_lba,
#ifdef MW_EDB_SUPPORT
				                      EDB_KEY_TYPE_T              e_key_type,
#else                    
	                                  EVCTX_KEY_TYPE_T            e_key_type,
#endif                    
	                                  SIZE_T                      z_event_info_len,
	                                  const VOID*                 pv_event_info)
{
    INT32                   i4_ret         = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj         = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head     = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj  = NULL;
    REC_UTIL_DB_T*          pt_db          = NULL;
    REC_UTIL_UPDATE_INFO_T  t_update_info;
    UINT32                  ui4_lba_start;
    UINT32                  ui4_lba_end;
    REC_UTIL_MSG_T          t_msg;
    BOOL                    b_lba_in_range = FALSE;

    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_update_event_info: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_UPDATE_EVT_INFO);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_event_info: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_event_info: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_event_info: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }

        if(REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_event_info: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }
        
        t_update_info.e_info_type = _rec_util_covert_evctx_key_type(e_key_type);
        if (REC_DB_INFO_TYPE_MAX == t_update_info.e_info_type)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_event_info: key type error!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        t_update_info.ui4_lba    = ui4_lba;
        t_update_info.z_info_len = z_event_info_len;
        t_update_info.pv_info    = (VOID*)pv_event_info;
        i4_ret = _rec_util_update_info(pt_db, &t_update_info);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_event_info: update event info buf error!\r\n"));
            break;
        }
        
        /*check if valid range info update*/
        if (pt_db->b_has_valid_range)
        {
            ui4_lba_start  = pt_db->ui4_range_start_lba;
            ui4_lba_end    = pt_db->ui4_range_end_lba;
            b_lba_in_range = _rec_util_lba_in_query_range(ui4_lba, 
                                                          ui4_lba_start, 
                                                          ui4_lba_end,
                                                          pt_db->ui4_avail_max_lba);
            if (b_lba_in_range)
            {
                /*update info in valid range*/
                x_memset(&t_msg, 0, sizeof(REC_UTIL_MSG_T));
                t_msg.e_msg_type = REC_UTIL_MSG_NFY_VALID_RANGE_INFO_UPDATED;
                t_msg.u_data.t_valid_range_info_nfy. h_rec_util = h_rec_util;
                t_msg.u_data.t_valid_range_info_nfy.h_db_head   = pt_db_head->h_rec_db_head;
                rec_util_send_msg(&t_msg);
            }
            else
            {
                /*update info not in valid range,info update will be notified later*/
                pt_db->b_out_range_info_update = TRUE;
            }
        }
        else
        {
            pt_db->b_out_range_info_update = TRUE;
        }
        
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_UPDATE_EVT_INFO);
    
    return i4_ret;    
}

INT32 rec_util_update_rrt_info (HANDLE_T                    h_rec_util,
                                UINT32                      ui4_lba,
                                SIZE_T                      z_info_len,
                                const VOID*                 pv_info)
{
    INT32                   i4_ret         = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj         = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head     = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj  = NULL;
    REC_UTIL_DB_T*          pt_db          = NULL;
    REC_UTIL_UPDATE_INFO_T  t_update_info;
    UINT32                  ui4_lba_start;
    UINT32                  ui4_lba_end;
    REC_UTIL_MSG_T          t_msg;
    BOOL                    b_lba_in_range = FALSE;

    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_update_rrt_info: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_UPDATE_RRT_INFO);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_rrt_info: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_rrt_info: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_rrt_info: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }

        if(REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_rrt_info: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }
        
        t_update_info.e_info_type = REC_DB_INFO_TYPE_RRT_INFO;
        
        
        t_update_info.ui4_lba    = ui4_lba;
        t_update_info.z_info_len = z_info_len;
        t_update_info.pv_info    = (VOID*)pv_info;
        i4_ret = _rec_util_update_info(pt_db, &t_update_info);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_rrt_info: update event info buf error!\r\n"));
            break;
        }
        
        /*check if valid range info update*/
        if (pt_db->b_has_valid_range)
        {
            ui4_lba_start  = pt_db->ui4_range_start_lba;
            ui4_lba_end    = pt_db->ui4_range_end_lba;
            b_lba_in_range = _rec_util_lba_in_query_range(ui4_lba, 
                                                          ui4_lba_start, 
                                                          ui4_lba_end,
                                                          pt_db->ui4_avail_max_lba);
            if (b_lba_in_range)
            {
                /*update info in valid range*/
                x_memset(&t_msg, 0, sizeof(REC_UTIL_MSG_T));
                t_msg.e_msg_type = REC_UTIL_MSG_NFY_VALID_RANGE_INFO_UPDATED;
                t_msg.u_data.t_valid_range_info_nfy. h_rec_util = h_rec_util;
                t_msg.u_data.t_valid_range_info_nfy.h_db_head   = pt_db_head->h_rec_db_head;
                rec_util_send_msg(&t_msg);
            }
            else
            {
                /*update info not in valid range,info update will be notified later*/
                pt_db->b_out_range_info_update = TRUE;
            }
        }
        else
        {
            pt_db->b_out_range_info_update = TRUE;
        }
        
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_UPDATE_RRT_INFO);
    
    return i4_ret;    
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_update_scdb_info
 *
 * Description: This API updates scdb info at the specified location.
 *              This API follows the rule: information of the same type must 
 *              always be updated in increased time-order, or, LBA-order. 
 *              Therefore, the LBA ui4_lba must always be newer than the LBA of 
 *              the last updated scdb info.
 *              On the other hand, once ui4_lba is in the valid range, the notification 
 *              REC_UTIL_NTFY_CODE_VALID_RANGE_INFO_UPDATED will be sent to all clients.
 *              
 * Inputs:  h_rec_util        Specifies the client handle
 *          ui4_lba           Specifies the LBA that the scdb info update occurs at.
 *          ui2_scdb_rec_num  Specifies the scdb record number.
 *          at_scdb_recs      Contains the scdb records.
 *          
 * Outputs: -
 *
 * Returns: REC_UTILR_INV_ARG        Invalid arguments.
 *          REC_UTILR_INV_HANDLE     Invalid handle.
 *          REC_UTILR_INV_COND       Invalid condition, should be in READY condition
 *          REC_UTILR_INV_LBA_ORDER  The LBA does not follow the rule: info of the same type
 *                                   must always be updated in increased LBA-order.
 ----------------------------------------------------------------------------*/
INT32 rec_util_update_scdb_info (HANDLE_T                    h_rec_util,
                                 UINT32                      ui4_lba,
                                 UINT16                      ui2_scdb_rec_num,
                                 const SCDB_REC_T*           at_scdb_recs)
{
    INT32                   i4_ret         = REC_UTILR_OK;
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj         = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head     = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj  = NULL;
    REC_UTIL_DB_T*          pt_db          = NULL;
    REC_UTIL_UPDATE_INFO_T  t_update_info;
    UINT32                  ui4_lba_start;
    UINT32                  ui4_lba_end;
    REC_UTIL_MSG_T          t_msg;
    BOOL                    b_lba_in_range;

    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_update_scdb_info: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_UPDATE_SCDB_INFO);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_scdb_info: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_scdb_info: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_scdb_info: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
         /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        if(REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_scdb_info: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }
        
        t_update_info.e_info_type = REC_DB_INFO_TYPE_SCDB_INFO;
        t_update_info.ui4_lba     = ui4_lba;
        t_update_info.z_info_len  = ui2_scdb_rec_num * sizeof(SCDB_REC_T);
        t_update_info.pv_info     = (VOID*)at_scdb_recs;
        i4_ret = _rec_util_update_info(pt_db, &t_update_info);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_update_scdb_info: update scdb info buf error!\r\n"));
            break;
        }
        
        /*check valid range info update*/
        if (pt_db->b_has_valid_range)
        {
            ui4_lba_start = pt_db->ui4_range_start_lba;
            ui4_lba_end   = pt_db->ui4_range_end_lba;
            b_lba_in_range = _rec_util_lba_in_query_range(ui4_lba, 
                                                          ui4_lba_start, 
                                                          ui4_lba_end,
                                                          pt_db->ui4_avail_max_lba);
            /*update info in valid range*/
            if (b_lba_in_range)
            {   
                x_memset(&t_msg, 0, sizeof(REC_UTIL_MSG_T));
                t_msg.e_msg_type = REC_UTIL_MSG_NFY_VALID_RANGE_INFO_UPDATED;
                t_msg.u_data.t_valid_range_info_nfy.h_rec_util = h_rec_util;
                t_msg.u_data.t_valid_range_info_nfy.h_db_head  = pt_db_head->h_rec_db_head;
                rec_util_send_msg(&t_msg);
            }
            /*update info not in valid range,will be notified later*/
            else
            {
                pt_db->b_out_range_info_update = TRUE;
            }
        }
        else
        {
            pt_db->b_out_range_info_update = TRUE;
        }
        
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_UPDATE_SCDB_INFO);
    
    return i4_ret;    
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_query_info_update
 *
 * Description: This API querys the first info update in the range from ui4_lba_begin to ui4_lba_end.
 *              
 * Inputs:  h_rec_util     Specifies the client handle
 *          ui4_lba_begin  Specifies the begin of the query range.
 *          ui4_lba_end    Specifies the end of the query range.
 *          b_backward     Specifies if search from ui4_lba_end to ui4_lba_begin or not.
 *
 * Outputs: pui4_mask_first     Contains the changed info mask of the first update.
 *          pui4_lba_first      Contains the location of the first update.
 *
 * Returns: REC_UTILR_INV_HANDLE  Invalid handle.
 *          REC_UTILR_INV_COND    Invalid condition, should be in READY condition
 *          REC_UTILR_INV_LBA     Invalid LBA. The specified LBA is not found in the database, 
 *                                in other words, the range is neither found in the valid range 
 *                                nor found in the extra space.
 ----------------------------------------------------------------------------*/
INT32 rec_util_query_info_update(HANDLE_T                    h_rec_util,
                                 UINT32                      ui4_lba_current,
                                 BOOL                        b_backward,
                                 UINT32*                     pui4_mask_first,
                                 UINT32*                     pui4_lba_first)
{
    INT32                       i4_ret                 = REC_UTILR_OK;
    HANDLE_TYPE_T               e_handle_type;
    VOID*                       pv_obj                 = NULL;
    REC_UTIL_DB_HEAD_T*         pt_db_head             = NULL;
    REC_UTIL_CLIENT_T*          pt_client_obj          = NULL;
    REC_UTIL_DB_T*              pt_db                  = NULL;
    UINT32                      ui4_first_lba          = 0;
    UINT8                       ui1_type;
    BOOL                        b_found                = FALSE;
    UINT32                      ui4_mask;
    BOOL                        b_first_info_exist     = FALSE;
    UINT32                      ui4_closest_lba;
    UINT32                      ui4_lba_begin;
    UINT32                      ui4_lba_end;
    UINT32                      ui4_lba_exchange;

    /*check param*/
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_query_info_update: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_QUERY_INFO_UPDATE);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_query_info_update: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_query_info_update: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_query_info_update: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
         /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        if(REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_query_info_update: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }
        
        *pui4_mask_first = 0;
        *pui4_lba_first  = 0;
        
        if (FALSE == pt_db->b_has_valid_range)
        {
            REC_UTIL_DBG_INFO(("rec_util_query_info_update: has no valid range now!\r\n"));
            break;
        }
        
        /*check lba*/
        i4_ret = _rec_util_check_lba_valid(ui4_lba_current, pt_db);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_query_info_update: check lba fail(%d)!\r\n",
                              i4_ret));
            break;
        }

        ui4_lba_begin = ui4_lba_current;
        ui4_lba_end   = pt_db->ui4_range_end_lba;

        if(b_backward)
        {
            ui4_lba_exchange = ui4_lba_begin;
            ui4_lba_begin = pt_db->ui4_range_start_lba;
            ui4_lba_end = ui4_lba_exchange;
        }
        
        /*search lba range in info_tbl*/
        for (ui1_type = REC_DB_INFO_FIRST_KEY_TYPE; ui1_type < REC_DB_INFO_TYPE_MAX; ui1_type++)
        {
            b_found = _rec_util_search_first_updated_lba(pt_db,
                                                         pt_db->at_info_grb_tbl[ui1_type],
                                                         ui4_lba_begin,
                                                         ui4_lba_end,
                                                         b_backward,
                                                         &ui4_first_lba);
            if (!b_found)
            {
                REC_UTIL_DBG_INFO(("rec_util_query_info_update: not found event(%d) lba!\r\n",
                                   ui1_type));
            }
            else
            {   
                if (REC_DB_INFO_TYPE_RATING == ui1_type)
                {
                    ui4_mask = REC_UTIL_INFO_MASK_RATING;
                }
                else if(REC_DB_INFO_TYPE_RRT_INFO == ui1_type)
                {
                    ui4_mask = REC_UTIL_INFO_MASK_RRT_INFO;
                }
                else if(REC_DB_INFO_TYPE_SCDB_INFO == ui1_type)
                {
                    ui4_mask = REC_UTIL_INFO_MASK_SCDB_UPDATE;
                }
                else
                {
                    ui4_mask = REC_UTIL_INFO_MASK_EVENT_INFO;
                }
                
                if (!b_first_info_exist)
                {
                    *pui4_lba_first    = ui4_first_lba;
                    *pui4_mask_first   = ui4_mask;
                    b_first_info_exist = TRUE;
                    continue;
                }

                /*compare current info lba with last searched one to get the nearst one*/
                i4_ret = _rec_util_get_closest_lba(pt_db,
                                                   b_backward?ui4_lba_end:ui4_lba_begin,
                                                   b_backward, 
                                                   *pui4_lba_first,
                                                   ui4_first_lba,
                                                   &ui4_closest_lba);
                if (i4_ret != REC_UTILR_OK)
                {
                    REC_UTIL_DBG_ERR(("rec_util_query_info_update: get event closest lba err!"
                        "current lba(%u), lba range(%u,%u),backward(%d),"
                        "first lba(%u), second lba(%u)\r\n",
                        ui4_lba_begin, pt_db->ui4_range_start_lba,
                        pt_db->ui4_range_end_lba, (INT32)b_backward,
                        (*pui4_lba_first), ui4_first_lba));
                    i4_ret = REC_UTILR_INTERNAL_ERROR;
                    break;
                }
                
                if (ui4_first_lba == ui4_closest_lba)
                {
                    if (ui4_first_lba == *pui4_lba_first)
                    {
                        *pui4_mask_first |= ui4_mask;
                    }
                    else
                    {
                        *pui4_mask_first = ui4_mask;
                    }
                }
                *pui4_lba_first = ui4_closest_lba;
            }
        }
        
        /*search lba range in signal_chg_tbl*/
        for (ui1_type = SIGNAL_FROM_Y_TO_N; ui1_type < SIGNAL_CHG_TYPE_MAX; ui1_type++)
        {
            b_found = _rec_util_search_first_updated_lba(pt_db,
                                                         pt_db->at_signal_grb_chg_tbl[ui1_type],
                                                         ui4_lba_begin,
                                                         ui4_lba_end,
                                                         b_backward,
                                                         &ui4_first_lba);
            if (b_found)
            {
                if (SIGNAL_FROM_Y_TO_N == ui1_type)
                {
                    ui4_mask = REC_UTIL_INFO_MASK_SIGNAL_Y_TO_N;
                }
                else
                {
                    ui4_mask = REC_UTIL_INFO_MASK_SIGNAL_N_TO_Y;
                }
                if (!b_first_info_exist)
                {
                    *pui4_lba_first    = ui4_first_lba;
                    *pui4_mask_first   = ui4_mask;
                    b_first_info_exist = TRUE;
                    continue;
                }
                
                i4_ret = _rec_util_get_closest_lba(pt_db,
                                                   b_backward?ui4_lba_end:ui4_lba_begin,
                                                   b_backward, 
                                                   *pui4_lba_first,
                                                   ui4_first_lba,
                                                   &ui4_closest_lba);
                if (i4_ret != REC_UTILR_OK)
                {
                    REC_UTIL_DBG_ERR(("rec_util_query_info_update: get event closest lba err!"
                        "current lba(%u), lba range(%u,%u),backward(%d),"
                        "first lba(%u), second lba(%u)\r\n",
                        ui4_lba_begin, pt_db->ui4_range_start_lba,
                        pt_db->ui4_range_end_lba, (INT32)b_backward,
                        (*pui4_lba_first), ui4_first_lba));
                    i4_ret = REC_UTILR_INTERNAL_ERROR;
                    break;
                }
                
                if (ui4_first_lba == ui4_closest_lba)
                {
                    if (ui4_first_lba == *pui4_lba_first)
                    {
                        *pui4_mask_first |= ui4_mask;
                    }
                    else
                    {
                        *pui4_mask_first = ui4_mask;
                    }
                }
                *pui4_lba_first = ui4_closest_lba;
            }
        }
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_QUERY_INFO_UPDATE);
    
    return i4_ret;  
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_get_event_info_len
 *
 * Description: This API gets the length of the specified type of event info at 
 *              the specified location.
 *              
 * Inputs:  h_rec_util  Specifies the client handle
 *          ui4_lba     Specifies the LBA of the event info to be get.
 *          e_key_type  Specifies event key type.
 *
 * Outputs: pz_event_info_len  Contains the event info length.
 *
 * Returns: REC_UTILR_INV_ARG    Invalid arguments.
 *          REC_UTILR_INV_HANDLE Invalid handle.
 *          REC_UTILR_INV_COND   Invalid condition, should be in READY condition
 *          REC_UTILR_INV_LBA    Invalid LBA. The specified LBA is not found in the database, 
 *                               in other words, the range is neither found in the valid range 
 *                               nor found in the extra space.
 ----------------------------------------------------------------------------*/
INT32 rec_util_get_event_info_len(HANDLE_T                    h_rec_util,
	                                  UINT32                      ui4_lba,
#ifdef MW_EDB_SUPPORT
			                          EDB_KEY_TYPE_T              e_key_type,
#else                    
	                                  EVCTX_KEY_TYPE_T            e_key_type,
#endif                    
	                                  SIZE_T*                     pz_event_info_len)  
{
    INT32                           i4_ret          = REC_UTILR_OK;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj          = NULL;
    REC_UTIL_DB_HEAD_T*             pt_db_head      = NULL;
    REC_UTIL_CLIENT_T*              pt_client_obj   = NULL;
    REC_UTIL_DB_T*                  pt_db           = NULL;
    BOOL                            b_found         = FALSE;
    REC_UTIL_INFO_TYPE_T            e_info_type;
    UINT32                          ui4_lba_begin;
    UINT32                          ui4_lba_end;
    UINT32                          ui4_lba_seek;
    REC_UTIL_GRB_DATA_T             t_data;
    
    /*check param*/
    if (NULL == pz_event_info_len)
    {
        REC_UTIL_DBG_ERR(("rec_util_get_event_info_len: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    e_info_type = _rec_util_covert_evctx_key_type(e_key_type);
    if (e_info_type >= REC_DB_INFO_TYPE_SCDB_INFO)
    {
        REC_UTIL_DBG_ERR(("rec_util_get_event_info_len: invalid type!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_event_info_len: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_EVT_INFO_LEN);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head    = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_event_info_len: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_event_info_len: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_event_info_len: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        /* check profile */
       if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
       {
           REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
           i4_ret = REC_UTILR_REQ_NOT_ALLOW;
           break;
       }

        if(REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_event_info_len: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }
        
        if(!pt_db->b_has_valid_range)
        {
            REC_UTIL_DBG_INFO(("rec_util_get_event_info_len: has no valid range!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        *pz_event_info_len = 0;
        /*check lba*/
        i4_ret = _rec_util_check_lba_valid(ui4_lba, pt_db);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_event_info_len: check lba fail!\r\n"));
            break;
        }
        

        ui4_lba_begin = pt_db->ui4_range_end_lba;
        ui4_lba_end = ui4_lba;
        b_found = _rec_util_search_first_lba(pt_db,
                                             pt_db->at_info_grb_tbl[e_info_type],
                                             ui4_lba_begin,
                                             ui4_lba_end,
                                             TRUE,
                                             &ui4_lba_seek,
                                             &t_data);
        /*not found*/
        if (!b_found)
        {   
            REC_UTIL_DBG_INFO(( "rec_util_get_event_info_len:no event(%d) at lba(%u)"
                                ", lba range (%u,%u)!\r\n",
                                e_info_type, ui4_lba, 
                                pt_db->ui4_range_start_lba, pt_db->ui4_range_end_lba));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        else
        {
            (*pz_event_info_len) = *((UINT32*)(((UINT8*)t_data.pv_data) + 4));
        }
        
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_EVT_INFO_LEN);
    
    return i4_ret;    
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_get_event_info
 *
 * Description: This API gets event info of the specified key type at the specified location.
 *              
 * Inputs:  h_rec_util          Specifies the client handle
 *          ui4_lba             Specifies the LBA of the event info to be get.
 *          e_key_type          Specifies the event key type.
 *          *pz_event_info_len  Specifies the size of pv_event_info buffer.
 *      
 * Outputs: pz_event_info_len   Contains the actual event info length
 *          pv_event_info       Contains the event info data.
 *
 * Returns: REC_UTILR_INV_ARG         Invalid arguments.
 *          REC_UTILR_INV_HANDLE      Invalid handle.
 *          REC_UTILR_INV_COND        Invalid condition, should be in READY condition
 *          REC_UTILR_INV_LBA         Invalid LBA. The specified LBA is not found in the database, 
 *                                    in other words, the range is neither found in the valid range 
 *                                    nor found in the extra space.
 *          REC_UTILR_NOT_ENOUGH_BUF  The input buffer pv_event_info is not enough. 
 *                                    This API will output the actual length in pz_event_info_len.
 ----------------------------------------------------------------------------*/
INT32 rec_util_get_event_info (HANDLE_T                    h_rec_util,
                               UINT32                      ui4_lba,
#ifdef MW_EDB_SUPPORT
                               EDB_KEY_TYPE_T              e_key_type,
#else                    
                               EVCTX_KEY_TYPE_T            e_key_type,
#endif                    
                               SIZE_T*                     pz_event_info_len,  /* IN / OUT */
                               VOID*                       pv_event_info)
{
    INT32                       i4_ret = REC_UTILR_OK;
    HANDLE_TYPE_T               e_handle_type;
    VOID*                       pv_obj = NULL;
    REC_UTIL_DB_HEAD_T*         pt_db_head = NULL;
    REC_UTIL_CLIENT_T*          pt_client_obj = NULL;
    REC_UTIL_DB_T*              pt_db = NULL;
    BOOL                        b_found = FALSE;
    SIZE_T                      z_info_len;
    REC_UTIL_INFO_TYPE_T        e_info_type;
    REC_UTIL_INFO_KEY_T         t_info_key;
    UINT8*                      pui1_info = NULL;
    UINT32                      ui4_lba_begin;
    UINT32                      ui4_lba_end;
    UINT32                      ui4_lba_seek;
    REC_UTIL_GRB_DATA_T         t_data;
    
    /*check param*/
    if ((NULL == pz_event_info_len) || (NULL == pv_event_info))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_event_info: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    e_info_type = _rec_util_covert_evctx_key_type(e_key_type);
    if (e_info_type >= REC_DB_INFO_TYPE_SCDB_INFO)
    {
        REC_UTIL_DBG_ERR(("rec_util_get_event_info: invalid type!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_event_info: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_EVT_INFO);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_event_info: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_event_info: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_event_info: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }         /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        if(REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_event_info: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }
        
        if(!pt_db->b_has_valid_range)
        {
            REC_UTIL_DBG_INFO(("rec_util_get_event_info: has no valid range!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        /*check lba*/
        i4_ret = _rec_util_check_lba_valid(ui4_lba, pt_db);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_INFO(("rec_util_get_event_info: check lba fail!\r\n"));
            break;
        }
        
        ui4_lba_begin = pt_db->ui4_range_end_lba;
        ui4_lba_end = ui4_lba;
        b_found = _rec_util_search_first_lba(pt_db,
                                             pt_db->at_info_grb_tbl[e_info_type],
                                             ui4_lba_begin,
                                             ui4_lba_end,
                                             TRUE,
                                             &ui4_lba_seek,
                                             &t_data);
        if (!b_found)
        {    
            REC_UTIL_DBG_INFO(("rec_util_get_event_info:no event(%d) at lba(%u)"
                               ", lba range (%u,%u)!\r\n",
                               e_info_type, ui4_lba, 
                               pt_db->ui4_range_start_lba, pt_db->ui4_range_end_lba));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        z_info_len = *((UINT32*)(((UINT8*)t_data.pv_data) + 4));
        if (*pz_event_info_len < z_info_len)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_event_info: buf size(%u) not enough (%u)!\r\n",
                              z_info_len,
                              *pz_event_info_len));
            i4_ret = REC_UTILR_NOT_ENOUGH_BUF;
            *pz_event_info_len = z_info_len;
            break;
        }
        
        *pz_event_info_len       = z_info_len;
        t_info_key.e_info_type   = e_info_type;
        t_info_key.e_brdcst_type = pt_db->t_cfg.e_brdcst_type;
        
        pui1_info = (UINT8*)t_data.pv_data + 8;
        
        i4_ret = _rec_util_copy_info(pv_event_info, 
                                     (UINT8*)pui1_info, 
                                     &t_info_key, 
                                     z_info_len);
        
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_event_info: copy info fail(%d)!\r\n",
                              i4_ret));
            break;
        }
        
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_EVT_INFO);
    return i4_ret;    
}

extern INT32 rec_util_get_rrt_info_len (
    HANDLE_T                    h_rec_util,
    UINT32                      ui4_lba,
    SIZE_T*                     pz_info_len)
{
    INT32                           i4_ret          = REC_UTILR_OK;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj          = NULL;
    REC_UTIL_DB_HEAD_T*             pt_db_head      = NULL;
    REC_UTIL_CLIENT_T*              pt_client_obj   = NULL;
    REC_UTIL_DB_T*                  pt_db           = NULL;
    BOOL                            b_found         = FALSE;
    REC_UTIL_INFO_TYPE_T            e_info_type;
    UINT32                          ui4_lba_begin;
    UINT32                          ui4_lba_end;
    UINT32                          ui4_lba_seek;
    REC_UTIL_GRB_DATA_T             t_data;
    
    /*check param*/
    if (NULL == pz_info_len)
    {
        REC_UTIL_DBG_ERR(("rec_util_get_rrt_info_len: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    e_info_type = REC_DB_INFO_TYPE_RRT_INFO;
    
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_rrt_info_len: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_RRT_INFO_LEN);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head    = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_rrt_info_len: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_rrt_info_len: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_rrt_info_len: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        if(REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_rrt_info_len: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }
        
        if(!pt_db->b_has_valid_range)
        {
            REC_UTIL_DBG_INFO(("rec_util_get_rrt_info_len: has no valid range!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        *pz_info_len = 0;
        /*check lba*/
        i4_ret = _rec_util_check_lba_valid(ui4_lba, pt_db);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_rrt_info_len: check lba fail!\r\n"));
            break;
        }
        

        ui4_lba_begin = pt_db->ui4_range_end_lba;
        ui4_lba_end = ui4_lba;
        b_found = _rec_util_search_first_lba(pt_db,
                                             pt_db->at_info_grb_tbl[e_info_type],
                                             ui4_lba_begin,
                                             ui4_lba_end,
                                             TRUE,
                                             &ui4_lba_seek,
                                             &t_data);
        /*not found*/
        if (!b_found)
        {   
            REC_UTIL_DBG_INFO(( "rec_util_get_rrt_info_len:no event(%d) at lba(%u)"
                                ", lba range (%u,%u)!\r\n",
                                e_info_type, ui4_lba, 
                                pt_db->ui4_range_start_lba, pt_db->ui4_range_end_lba));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        else
        {
            (*pz_info_len) = *((UINT32*)((UINT8*)t_data.pv_data + 4));
        }
        
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_RRT_INFO_LEN);
    
    return i4_ret; 

}

extern INT32 rec_util_get_rrt_info (
    HANDLE_T                    h_rec_util,
    UINT32                      ui4_lba,
    SIZE_T*                     pz_info_len, /* IN / OUT */
    VOID*                       pv_info)
{
    INT32                       i4_ret = REC_UTILR_OK;
    HANDLE_TYPE_T               e_handle_type;
    VOID*                       pv_obj = NULL;
    REC_UTIL_DB_HEAD_T*         pt_db_head = NULL;
    REC_UTIL_CLIENT_T*          pt_client_obj = NULL;
    REC_UTIL_DB_T*              pt_db = NULL;
    BOOL                        b_found = FALSE;
    SIZE_T                      z_info_len;
    REC_UTIL_INFO_TYPE_T        e_info_type;
    REC_UTIL_INFO_KEY_T         t_info_key;
    UINT8*                      pui1_info = NULL;
    UINT32                      ui4_lba_begin;
    UINT32                      ui4_lba_end;
    UINT32                      ui4_lba_seek;
    REC_UTIL_GRB_DATA_T         t_data;
    
    /*check param*/
    if ((NULL == pz_info_len) || (NULL == pv_info))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_rrt_info: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    e_info_type = REC_DB_INFO_TYPE_RRT_INFO;
    
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_rrt_info: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_RRT_INFO);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_rrt_info: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_rrt_info: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_rrt_info: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }        
        /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        if(REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_rrt_info: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }

        if(!pt_db->b_has_valid_range)
        {
            REC_UTIL_DBG_INFO(("rec_util_get_rrt_info: has no valid range!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        /*check lba*/
        i4_ret = _rec_util_check_lba_valid(ui4_lba, pt_db);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_INFO(("rec_util_get_rrt_info: check lba fail!\r\n"));
            break;
        }
        
        ui4_lba_begin = pt_db->ui4_range_end_lba;
        ui4_lba_end = ui4_lba;
        b_found = _rec_util_search_first_lba(pt_db,
                                             pt_db->at_info_grb_tbl[e_info_type],
                                             ui4_lba_begin,
                                             ui4_lba_end,
                                             TRUE,
                                             &ui4_lba_seek,
                                             &t_data);
        if (!b_found)
        {    
            REC_UTIL_DBG_INFO(("rec_util_get_rrt_info:no event(%d) at lba(%u)"
                               ", lba range (%u,%u)!\r\n",
                               e_info_type, ui4_lba, 
                               pt_db->ui4_range_start_lba, pt_db->ui4_range_end_lba));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        z_info_len = *((UINT32*)((UINT8*)t_data.pv_data + 4));
        if (*pz_info_len < z_info_len)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_rrt_info: buf size(%u) not enough (%u)!\r\n",
                              z_info_len,
                              *pz_info_len));
            i4_ret = REC_UTILR_NOT_ENOUGH_BUF;
            *pz_info_len = z_info_len;
            break;
        }
        
        *pz_info_len       = z_info_len;
        t_info_key.e_info_type   = e_info_type;
        t_info_key.e_brdcst_type = pt_db->t_cfg.e_brdcst_type;
        
        pui1_info = (UINT8*)t_data.pv_data + 8;
        
        i4_ret = _rec_util_copy_info(pv_info, 
                                     (UINT8*)pui1_info, 
                                     &t_info_key, 
                                     z_info_len);
        
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_rrt_info: copy info fail(%d)!\r\n",
                              i4_ret));
            break;
        }
        
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_RRT_INFO);
    return i4_ret;    

}

/*-----------------------------------------------------------------------------
 * Name: rec_util_get_scdb_info_rec_num
 *
 * Description: This API gets the scdb record number at the specified location.
 *              The record util only keeps the scdb records of the ES streams it records.
 *              
 * Inputs:  h_rec_util  Specifies the client handle
 *          ui4_lba     Specifies the LBA of the scdb info to be get.
 *
 * Outputs: pui2_scdb_rec_num  Contains the scdb record number at the specified location.
 *
 * Returns: REC_UTILR_INV_HANDLE Invalid handle.
 *          REC_UTILR_INV_COND   Invalid condition, should be in READY condition
 *          REC_UTILR_INV_LBA    Invalid LBA. The specified LBA is not found in the database, 
 *                               in other words, the range is neither found in the valid range 
 *                               nor found in the extra space.
 ----------------------------------------------------------------------------*/
INT32 rec_util_get_scdb_info_rec_num (HANDLE_T                    h_rec_util,
                                      UINT32                      ui4_lba,
                                      UINT16*                     pui2_scdb_rec_num)
{
    INT32                       i4_ret = REC_UTILR_OK;
    HANDLE_TYPE_T               e_handle_type;
    VOID*                       pv_obj = NULL;
    REC_UTIL_DB_HEAD_T*         pt_db_head = NULL;
    REC_UTIL_CLIENT_T*          pt_client_obj = NULL;
    REC_UTIL_DB_T*              pt_db = NULL;
    REC_UTIL_INFO_DATA_T*       pt_info = NULL;
    BOOL                        b_found = FALSE;
    SIZE_T                      z_info_len;
    
    UINT32                      ui4_lba_begin;
    UINT32                      ui4_lba_end;
    UINT32                      ui4_lba_seek;
    REC_UTIL_GRB_DATA_T         t_data;
    
    /*check param*/
    if (NULL == pui2_scdb_rec_num)
    {
        REC_UTIL_DBG_ERR(("rec_util_get_scdb_info_rec_num: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_scdb_info_rec_num: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_SCDB_INFO_REC_NUM);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_scdb_info_rec_num: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_scdb_info_rec_num: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_scdb_info_rec_num: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }        
        /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        if(REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_scdb_info_rec_num: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }
        
        *pui2_scdb_rec_num = 0;
        
        /*check lba*/
        if(!pt_db->b_has_valid_range)
        {
            REC_UTIL_DBG_INFO(("rec_util_get_scdb_info_rec_num: has no valid range!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }

        i4_ret = _rec_util_check_lba_valid(ui4_lba, pt_db);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_scdb_info_rec_num: check lba fail!\r\n"));
            break;
        }
        ui4_lba_begin = pt_db->ui4_range_end_lba;
        ui4_lba_end = ui4_lba;
        b_found = _rec_util_search_first_lba(pt_db,
                                             pt_db->at_info_grb_tbl[REC_DB_INFO_TYPE_SCDB_INFO],
                                             ui4_lba_begin,
                                             ui4_lba_end,
                                             TRUE,
                                             &ui4_lba_seek,
                                             &t_data);
        if (!b_found)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_scdb_info_rec_num: not found info!\r\n"));
            i4_ret = REC_UTILR_INV_LBA;
            break;
        }
        
        pt_info = (REC_UTIL_INFO_DATA_T*)(((UINT8*)t_data.pv_data) + 4);
        if (NULL == pt_info)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_scdb_info_rec_num: the info is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }  
        
        z_info_len         = pt_info->z_info_len;
        *pui2_scdb_rec_num = (UINT16)(z_info_len / sizeof(SCDB_REC_T));
        
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_SCDB_INFO_REC_NUM);
    return i4_ret;    
}

/*-----------------------------------------------------------------------------
 * Name: rec_util_get_scdb_info
 *
 * Description: This API gets the scdb records at the specified location.
 *              The record util only keeps the scdb records of the ES streams it records.
 *              
 * Inputs:  h_rec_util            Specifies the client handle
 *          ui4_lba               Specifies the LBA of the scdb info to be get.
 *          (*pui2_scdb_rec_num)  Contains the number of record the input buffer can contain.
 *
 * Outputs: pui2_scdb_rec_num     Contains the scdb record number at the specified location.
 *          at_scdb_recs          Contains the scdb records.
 *
 * Returns: REC_UTILR_INV_HANDLE      Invalid handle.
 *          REC_UTILR_INV_COND        Invalid condition, should be in READY condition
 *          REC_UTILR_INV_LBA         Invalid LBA. The specified LBA is not found in the database, 
 *                                    in other words, the range is neither found in the valid range 
 *                                    nor found in the extra space.
 *          REC_UTILR_NOT_ENOUGH_BUF  The input buffer pv_event_info is not enough. 
 *                                    This API will output the actual length in pz_event_info_len.
 ----------------------------------------------------------------------------*/
INT32 rec_util_get_scdb_info (HANDLE_T                    h_rec_util,
                              UINT32                      ui4_lba,
                              UINT16*                     pui2_scdb_rec_num,
                              SCDB_REC_T*                 at_scdb_recs)
{
    INT32                       i4_ret        = REC_UTILR_OK;
    HANDLE_TYPE_T               e_handle_type;
    VOID*                       pv_obj        = NULL;
    REC_UTIL_DB_HEAD_T*         pt_db_head    = NULL;
    REC_UTIL_CLIENT_T*          pt_client_obj = NULL;
    REC_UTIL_DB_T*              pt_db         = NULL;
    REC_UTIL_INFO_DATA_T*       pt_info       = NULL;
    BOOL                        b_found       = FALSE;
    UINT16                      ui2_rec_num;
    REC_UTIL_INFO_KEY_T         t_info_key;
    VOID*                       pv_info       = NULL;
    
    UINT32                      ui4_lba_begin;
    UINT32                      ui4_lba_end;
    UINT32                      ui4_lba_seek;
    REC_UTIL_GRB_DATA_T         t_data;
    
    /*check param*/
    if ((NULL == pui2_scdb_rec_num) || (NULL == at_scdb_recs))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_scdb_info: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_scdb_info: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_SCDB_INFO);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head    = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_scdb_info: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_scdb_info: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_scdb_info: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }        
        /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        if(REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_scdb_info: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }
        
        /*check lba*/
        if(!pt_db->b_has_valid_range)
        {
            REC_UTIL_DBG_INFO(("rec_util_get_scdb_info: has no valid range!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        i4_ret = _rec_util_check_lba_valid(ui4_lba, pt_db);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_INFO(("rec_util_get_scdb_info: check lba fail!\r\n"));
            break;
        }
        
        ui4_lba_begin = pt_db->ui4_range_end_lba;
        ui4_lba_end = ui4_lba;
        b_found = _rec_util_search_first_lba(pt_db,
                                             pt_db->at_info_grb_tbl[REC_DB_INFO_TYPE_SCDB_INFO],
                                             ui4_lba_begin,
                                             ui4_lba_end,
                                             TRUE,
                                             &ui4_lba_seek,
                                             &t_data);
        if (!b_found)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_scdb_info: not found info!\r\n"));
            i4_ret = REC_UTILR_INV_LBA;
            break;
        }
        
        pt_info = (REC_UTIL_INFO_DATA_T*)(((UINT8*)t_data.pv_data) + 4);
        if (NULL == pt_info)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_scdb_info: the info is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }  
        
        ui2_rec_num = (UINT16)((pt_info->z_info_len) / sizeof(SCDB_REC_T));
        if (*pui2_scdb_rec_num < ui2_rec_num)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_scdb_info: buf size not enough!\r\n"));
            i4_ret = REC_UTILR_NOT_ENOUGH_BUF;
            break;
        }
        
        *pui2_scdb_rec_num       = ui2_rec_num;
        t_info_key.e_info_type   = REC_DB_INFO_TYPE_SCDB_INFO;
        t_info_key.e_brdcst_type = pt_db->t_cfg.e_brdcst_type;
        
        pv_info = (VOID*)pt_info;
        pv_info = (VOID*)((UINT8*)pv_info + sizeof(SIZE_T));
        i4_ret = _rec_util_copy_info((VOID*)at_scdb_recs, pv_info, 
                                     &t_info_key, pt_info->z_info_len);
        if (REC_UTILR_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_scdb_info: copy info fail!\r\n"));
            break;
        }
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_SCDB_INFO);
    
    return i4_ret;    
}

INT32 rec_util_get_tick_index(HANDLE_T                    h_rec_util,
                              UINT32                      ui4_index,
                              PVR_TICK_INDEX_T*           pt_tick_index)
{
    INT32                           i4_ret            = REC_UTILR_OK;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj            = NULL;
    REC_UTIL_DB_HEAD_T*             pt_db_head        = NULL;
    REC_UTIL_CLIENT_T*              pt_client_obj     = NULL;
    REC_UTIL_DB_T*                  pt_db             = NULL;
    UINT32                          ui4_data_sz;

    /*check param*/
    if (NULL == pt_tick_index)
    {
        REC_UTIL_DBG_ERR(("rec_util_get_tick_index: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_tick_index: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_TICK_INDEX);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head    = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_tick_index: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_tick_index: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_tick_index: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }

        if(REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_tick_index: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }

        if(pt_db->t_valid_range.ui4_start <= pt_db->t_valid_range.ui4_end)
        {
            if((ui4_index < pt_db->t_valid_range.ui4_start) ||
                ui4_index > pt_db->t_valid_range.ui4_end)
            {
                REC_UTIL_DBG_ERR((  "rec_util_get_tick_index: out of "
                                    "range[%d/%d](%d)!\r\n",
                                    pt_db->t_valid_range.ui4_start,
                                    pt_db->t_valid_range.ui4_end,
                                    ui4_index));
                i4_ret = REC_UTILR_INV_RANGE;
                break;
            }
        }
        else
        {
            if((ui4_index > pt_db->t_valid_range.ui4_end) &&
                ui4_index < pt_db->t_valid_range.ui4_start)
            {
                REC_UTIL_DBG_ERR((  "rec_util_get_tick_index: out of "
                                    "range[%d/%d](%d)!\r\n",
                                    pt_db->t_valid_range.ui4_start,
                                    pt_db->t_valid_range.ui4_end,
                                    ui4_index));
                i4_ret = REC_UTILR_INV_RANGE;
                break;
            }
        }
        
        ui4_data_sz = sizeof(PVR_TICK_INDEX_T);
        i4_ret =  rec_util_rque_copy_entry(pt_db->h_pvr_tick_idx_tbl,
                                          ui4_index,
                                          (UINT8*)pt_tick_index,
                                          &ui4_data_sz);
        if (i4_ret != REC_UTILR_RQUE_OK)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_tick_index:get tick index(%u) error(%d)!\r\n",
                              ui4_index,
                              i4_ret));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        } 

        REC_UTIL_DBG_INFO((  "rec_util_get_tick_index:range(%d,%d), "
                            "index(%d),tick num(%d)\r\n",
                            pt_db->t_valid_range.ui4_start,
                            pt_db->t_valid_range.ui4_end,
                            ui4_index,
                            pt_tick_index->ui4_tick_num));                   
        
    } while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_TICK_INDEX);
    
    return i4_ret;    
}

INT32 rec_util_get_sys_time_by_tick(HANDLE_T               h_rec_util,
                                    PVR_TICK_INDEX_T*      pt_tick_index,
                                    TIME_T*                pt_sys_time)
{
    HANDLE_TYPE_T               e_handle_type;
    VOID*                       pv_obj;
    REC_UTIL_DB_HEAD_T*         pt_db_head;
    REC_UTIL_CLIENT_T*          pt_client_obj;
    REC_UTIL_DB_T*              pt_db;
    INT32                       i4_ret;
    PVR_TICK_DETAIL_T*          pt_tick_detail = NULL;
    UINT8*                      pui1_data      = NULL;
    UINT32                      ui4_blks_entries;
        
    /*Check arguments*/
    if ((NULL_HANDLE == h_rec_util) 
        || (NULL == pt_tick_index) 
        || (NULL == pt_sys_time))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_sys_time_by_tick: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }

    i4_ret = REC_UTILR_OK;

    /*get record client object*/
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) 
        || (RU_HT_CLIENT_HANDLER != e_handle_type) 
        || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_sys_time_by_tick: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_SYS_TIME);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head    = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_sys_time_by_tick: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_sys_time_by_tick: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_sys_time_by_tick: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }        
        /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        if(REC_UTIL_IDX_TBL_OPT_DISABLE == pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_sys_time_by_tick: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }
        
        pt_tick_detail = pt_tick_index->pt_tick_detail;
        if (NULL == pt_tick_detail)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_sys_time_by_tick: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }

        if (!(pt_tick_index->ui4_flags & PVR_TICK_INDEX_FLAG_HAS_SYS_TIME))
        {
            REC_UTIL_DBG_ERR(("rec_util_get_sys_time_by_tick: no system time!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        pui1_data    = (UINT8*)pt_tick_detail;

        /*skip tick number*/
        pui1_data   += sizeof(UINT32);   
        
         /*ui4_blks_entries*/
         ui4_blks_entries = PVR_TICK_DETAIL_GET_ENTRIES(pt_tick_detail);
        
        pui1_data   += sizeof(UINT32); 
        
        /*pic info*/
        pui1_data   += sizeof(PVR_PIC_INFO_T) * ui4_blks_entries;  

        /*system time*/
        x_memcpy(pt_sys_time, pui1_data, sizeof(TIME_T));
        
        REC_UTIL_DBG_INFO(("rec_util_get_sys_time_by_tick:sys_time=0x%llx\r\n", 
                           *pt_sys_time));
        
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_SYS_TIME);
    
    return i4_ret;    
}


INT32 rec_util_get_sess_key (HANDLE_T                    h_rec_util,
                                   MM_CRYPT_INFO_T*            pt_sess_key)
{
    INT32                      i4_ret;
    REC_UTIL_CONFIG_T          t_cfg;

    i4_ret = rec_util_get_config(h_rec_util, &t_cfg);
    if (REC_UTILR_OK == i4_ret)
    {
        *pt_sess_key = t_cfg.t_sess_key;
    }
    
    return i4_ret;
}
INT32 rec_util_get_file_size(HANDLE_T                    h_rec_util,
                                UINT64 *                     pui8_size)
{
    HANDLE_TYPE_T               e_handle_type;
    VOID*                       pv_obj;
    REC_UTIL_DB_HEAD_T*         pt_db_head;
    REC_UTIL_CLIENT_T*          pt_client_obj;
    REC_UTIL_DB_T*              pt_db;
    INT32                       i4_ret;
        
    /*Check arguments*/
    if ((NULL_HANDLE == h_rec_util) 
        || (NULL == pui8_size))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_sys_time_by_tick: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }

    i4_ret = REC_UTILR_OK;

    /*get record client object*/
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) 
        || (RU_HT_CLIENT_HANDLER != e_handle_type) 
        || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_get_sys_time_by_tick: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_GET_FILE_SIZE);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head    = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_sys_time_by_tick: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_sys_time_by_tick: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_sys_time_by_tick: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }

        if(REC_UTIL_IDX_TBL_OPT_DISABLE != pt_db->t_cfg.e_idx_tbl_opt)
        {
            REC_UTIL_DBG_ERR(("rec_util_get_sys_time_by_tick: tick opt disabled!\r\n"));
            i4_ret = REC_UTILR_IDX_TBL_DISABLED;
            break;
        }
        
        *pui8_size = pt_db->ui8_tsb_cur_pos;
        
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_GET_FILE_SIZE);
    
    return i4_ret;    
}


INT32 rec_util_alloc_pvr_buf (
    HANDLE_T                        h_rec_util,
    const REC_UTIL_PVR_BUF_CNST_T*  pt_buf_cnst,
    REC_UTIL_PBA_BUF_INFO_T*        pt_buf_info)
{
    HANDLE_TYPE_T               e_handle_type;
    VOID*                       pv_obj;
    REC_UTIL_DB_HEAD_T*         pt_db_head;
    REC_UTIL_CLIENT_T*          pt_client_obj;
    REC_UTIL_DB_T*              pt_db;
    INT32                       i4_ret;
    REC_UTIL_PBA_PARAM_T        t_pba_param;
    SIZE_T                      z_max_pvr_buf_sz;
    UINT32                      ui4_pool_buf_sz_align;
    UINT32                      ui4_adjust_sz_align;
        
    /*Check arguments*/
    if ((NULL_HANDLE == h_rec_util) || 
        (NULL == pt_buf_info))
    {
        REC_UTIL_DBG_ERR(("rec_util_alloc_pvr_buf: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }

    i4_ret = REC_UTILR_OK;

    /*get record client object*/
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) 
        || (RU_HT_CLIENT_HANDLER != e_handle_type) 
        || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_alloc_pvr_buf: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_ALLOC_PVR_BUF);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head    = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_alloc_pvr_buf: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_alloc_pvr_buf: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_alloc_pvr_buf: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        _rec_util_get_pba_param(pt_db, pt_buf_cnst, &t_pba_param);

        /*if can get ring buffer from pool*/
        i4_ret = rec_util_get_ring_buffer(pt_buf_info, &ui4_pool_buf_sz_align);
        if ((i4_ret != REC_UTILR_OK) && (i4_ret != REC_UTILR_PVR_BUF_NOT_POOLED))
        {
            REC_UTIL_DBG_ERR(("rec_util_alloc_pvr_buf: get ring buffer failed!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        if (i4_ret == REC_UTILR_OK)
        {
            /*check memory alignment*/
            ui4_adjust_sz_align = ui4_pool_buf_sz_align;
            
            if (pt_buf_cnst->ui4_align_buf_start)
            {
                if ((UINT32)(pt_buf_info->pv_buf) % pt_buf_cnst->ui4_align_buf_start != 0)
                {
                    pt_buf_info->pv_buf = (VOID*) (((UINT32)(pt_buf_info->pv_buf) + pt_buf_cnst->ui4_align_buf_start - 1 ) 
                                                    / pt_buf_cnst->ui4_align_buf_start);
                    pt_buf_info->z_size -=  pt_buf_cnst->ui4_align_buf_start;
                }
            }
            
            if (pt_buf_cnst->ui4_align_buf_size)
            {
                if (ui4_pool_buf_sz_align % pt_buf_cnst->ui4_align_buf_size != 0)
                {
                    ui4_adjust_sz_align  = rec_util_get_smallest_multi_num(pt_buf_cnst->ui4_align_buf_size,
                                                                           ui4_adjust_sz_align);
                    REC_UTIL_DBG_INFO(("ring buffer alignment:pool(%u), size align(%u), adjust(%u), Line(%u)\r\n",
                                       ui4_pool_buf_sz_align, pt_buf_cnst->ui4_align_buf_size, 
                                       ui4_adjust_sz_align, __LINE__));
                }
            }

            /*if alignemnt is different should crop..*/
            pt_buf_info->z_size = (SIZE_T)((((UINT32)(pt_buf_info->z_size))
                                           / ui4_adjust_sz_align)
                                           * ui4_adjust_sz_align);
            break;
        }
        else
        {
            /*if  not pooled dynamicly  allocate ring buffer*/
            if (!(t_rec_util_sys_cfg.t_fct_tbl_pvr_buf.pf_query_avail_pvr_buf
                  ((VOID*)t_rec_util_sys_cfg.pv_tag_pvr_buf_cb,
                  &t_pba_param,
                  &z_max_pvr_buf_sz)))
            {
                REC_UTIL_DBG_ERR(("rec_util_alloc_pvr_buf: query avail buf failed!\r\n"));
                i4_ret = REC_UTILR_ALLOC_BUF_FAIL;
                break;
            }
            
            if (z_max_pvr_buf_sz < t_pba_param.z_size)
            {
                REC_UTIL_DBG_ERR((  "rec_util_alloc_pvr_buf: query avail buf size too "
                                    "small!\r\n"));
                i4_ret = REC_UTILR_ALLOC_BUF_FAIL;
                break;
            }
            
            if (!(t_rec_util_sys_cfg.t_fct_tbl_pvr_buf.pf_alloc_pvr_buf
                  ((VOID*)t_rec_util_sys_cfg.pv_tag_pvr_buf_cb,
                  &t_pba_param,
                  pt_buf_info)))
            {
                REC_UTIL_DBG_ERR(("rec_util_alloc_pvr_buf: alloc pvr buf failed!\r\n"));
                i4_ret = REC_UTILR_ALLOC_BUF_FAIL;
                break;
            }
        }
        
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_ALLOC_PVR_BUF);
    
    return i4_ret;    
}

INT32 rec_util_free_pvr_buf (
    HANDLE_T                        h_rec_util,
    const REC_UTIL_PBA_BUF_INFO_T*  pt_buf_info)
{
    HANDLE_TYPE_T               e_handle_type;
    VOID*                       pv_obj;
    REC_UTIL_DB_HEAD_T*         pt_db_head;
    REC_UTIL_CLIENT_T*          pt_client_obj;
    REC_UTIL_DB_T*              pt_db;
    INT32                       i4_ret;
        
    /*Check arguments*/
    if ((NULL_HANDLE == h_rec_util) || 
        (NULL == pt_buf_info))
    {
        REC_UTIL_DBG_ERR(("rec_util_free_pvr_buf: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }

    i4_ret = REC_UTILR_OK;

    /*get record client object*/
    i4_ret = handle_get_type_obj(h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) 
        || (RU_HT_CLIENT_HANDLER != e_handle_type) 
        || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_free_pvr_buf: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag(REC_UTIL_TAG_FREE_PVR_BUF);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T*)pv_obj;
        pt_db_head    = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_free_pvr_buf: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_free_pvr_buf: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_free_pvr_buf: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }     
        /* check profile */
        if (pt_db->e_profile != REC_UTIL_DB_PROFILE_UNLIMIT)
        {
            REC_UTIL_DBG_ERR(("Operation is not allowed. (L%d)\r\n", __LINE__));
            i4_ret = REC_UTILR_REQ_NOT_ALLOW;
            break;
        }
        /*check if get from pooled*/
        if (rec_util_pvr_buf_is_pooled(pt_buf_info))
        {
            
            i4_ret = rec_util_put_ring_buffer(pt_buf_info);
            break;
        }
        else
        {
            /*if not pooled dynamicly free*/
            if (!(t_rec_util_sys_cfg.t_fct_tbl_pvr_buf.pf_free_pvr_buf
                  ((VOID*)t_rec_util_sys_cfg.pv_tag_pvr_buf_cb, 
                  pt_buf_info)))
            {
                REC_UTIL_DBG_ERR(("rec_util_free_pvr_buf: free pvr buf failed!\r\n"));
                i4_ret = REC_UTILR_FREE_BUF_FAIL;
                break;
            } 
        }
        
    }while(0);
    rec_util_unlock_with_tag(REC_UTIL_TAG_FREE_PVR_BUF);
    
    return i4_ret;    

}
INT32 rec_util_rfi_load (
    HANDLE_T                        h_rec_util)
{
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj        = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head    = NULL;
    REC_UTIL_DB_T*          pt_db         = NULL;
    INT32                   i4_ret        = REC_UTILR_OK;
    
    i4_ret = handle_get_type_obj (h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_rfi_load: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag (REC_UTIL_TAG_RFI_LOAD);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T *) pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_load: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        /* check db condition */
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_load: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        /*get db obj*/
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_load: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break; 
        }
        
        /* load rfi */
        i4_ret = rec_util_meta_rfi_load (& pt_db->t_ctx_rfi,
                                         pt_db->s_head_file);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_load: Fail to load RFI (%d)\r\n", i4_ret));
            break;
        }
        
    } while (0);
    
    rec_util_unlock_with_tag (REC_UTIL_TAG_RFI_LOAD);
    
    return i4_ret;     
}
    
INT32 rec_util_rfi_store (
    HANDLE_T                        h_rec_util)
{
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj        = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head    = NULL;
    REC_UTIL_DB_T*          pt_db         = NULL;
    INT32                   i4_ret        = REC_UTILR_OK;
    
    i4_ret = handle_get_type_obj (h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_rfi_store: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag (REC_UTIL_TAG_RFI_STORE);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T *) pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_store: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        /* check db condition */
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_store: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        /*get db obj*/
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_store: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break; 
        }
        
        /* store rfi */
        i4_ret = rec_util_meta_rfi_store (& pt_db->t_ctx_rfi);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_store: Fail to store RFI (%d)\r\n", i4_ret));
            break;
        }
        
    } while (0);
    
    rec_util_unlock_with_tag (REC_UTIL_TAG_RFI_STORE);
    
    return i4_ret;    
}

INT32 rec_util_rfi_set_info (
    HANDLE_T                        h_rec_util,
    UINT32                          ui4_key_id,
    const VOID*                     pv_set_info,
    SIZE_T                          z_set_info )
{
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj        = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head    = NULL;
    REC_UTIL_DB_T*          pt_db         = NULL;
    INT32                   i4_ret        = REC_UTILR_OK;

    /*check param*/
    if ( ui4_key_id == 0  ||
        !(z_set_info == 0 ||
          pv_set_info != NULL))
    {
        REC_UTIL_DBG_ERR(("rec_util_rfi_set_info: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    i4_ret = handle_get_type_obj (h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_rfi_set_info: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag (REC_UTIL_TAG_RFI_SET_INFO);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T *) pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_set_info: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        /* check db condition */
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_set_info: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        /*get db obj*/
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_set_info: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break; 
        }
        
        /* set rfi info */
        i4_ret = rec_util_meta_rfi_set_info (& pt_db->t_ctx_rfi,
                                             ui4_key_id,
                                             pv_set_info,
                                             z_set_info);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_set_info: Fail to set RFI info (%d)\r\n", i4_ret));
            break;
        }
        
    } while (0);
    
    rec_util_unlock_with_tag (REC_UTIL_TAG_RFI_SET_INFO);
    
    return i4_ret;
}

INT32 rec_util_rfi_get_info (
    HANDLE_T                        h_rec_util,
    UINT32                          ui4_key_id,
    VOID*                           pv_get_info,
    SIZE_T*                         pz_get_info )
{
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj        = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head    = NULL;
    REC_UTIL_DB_T*          pt_db         = NULL;
    INT32                   i4_ret        = REC_UTILR_OK;
    
    /*check param*/
    if (ui4_key_id == 0     ||
        pz_get_info == NULL ||
        *pz_get_info == 0   ||
        pv_get_info == NULL)
    {
        REC_UTIL_DBG_ERR(("rec_util_rfi_get_info: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    i4_ret = handle_get_type_obj (h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_rfi_get_info: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag (REC_UTIL_TAG_RFI_GET_INFO);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T *) pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_get_info: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        /* check db condition */
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_get_info: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        /*get db obj*/
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_get_info: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break; 
        }
        
        /* get rfi info */
        i4_ret = rec_util_meta_rfi_get_info (& pt_db->t_ctx_rfi,
                                             ui4_key_id,
                                             pv_get_info,
                                             pz_get_info);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_get_info: Fail to get RFI info (%d)\r\n", i4_ret));
            break;
        }
        
    } while (0);
    
    rec_util_unlock_with_tag (REC_UTIL_TAG_RFI_GET_INFO);
    
    return i4_ret;    
}
    
INT32 rec_util_rfi_get_info_len (
    HANDLE_T                        h_rec_util,
    UINT32                          ui4_key_id,
    SIZE_T*                         pz_get_info)
{
    HANDLE_TYPE_T           e_handle_type;
    VOID*                   pv_obj        = NULL;
    REC_UTIL_CLIENT_T*      pt_client_obj = NULL;
    REC_UTIL_DB_HEAD_T*     pt_db_head    = NULL;
    REC_UTIL_DB_T*          pt_db         = NULL;
    INT32                   i4_ret        = REC_UTILR_OK;
    
    /*check param*/
    if (ui4_key_id == 0 ||
        pz_get_info == NULL)
    {
        REC_UTIL_DBG_ERR(("rec_util_rfi_get_info_len: invalid arguments!\r\n"));
        return REC_UTILR_INV_ARG;
    }
    
    i4_ret = handle_get_type_obj (h_rec_util, &e_handle_type, &pv_obj);
    if ((HR_OK != i4_ret) || (RU_HT_CLIENT_HANDLER != e_handle_type) || (NULL == pv_obj))
    {
        REC_UTIL_DBG_ERR(("rec_util_rfi_get_info_len: invalid handle!\r\n"));
        return REC_UTILR_INV_HANDLE;
    }
    
    rec_util_lock_with_tag (REC_UTIL_TAG_RFI_GET_INFO_LEN);
    do
    {
        pt_client_obj = (REC_UTIL_CLIENT_T *) pv_obj;
        pt_db_head = pt_client_obj->pt_rec_db_head;
        if (NULL == pt_db_head)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_get_info_len: db head obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break;
        }
        
        /* check db condition */
        if (REC_UTIL_COND_READY != pt_db_head->e_cond)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_get_info_len: db obj is not READY!\r\n"));
            i4_ret = REC_UTILR_INV_COND;
            break;
        }
        
        /*get db obj*/
        pt_db = pt_db_head->pt_db_obj;
        if (NULL == pt_db)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_get_info_len: db obj is NULL!\r\n"));
            i4_ret = REC_UTILR_INTERNAL_ERROR;
            break; 
        }
        
        /* get rfi info len */
        i4_ret = rec_util_meta_rfi_get_info_len (& pt_db->t_ctx_rfi,
                                                 ui4_key_id,
                                                 pz_get_info);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR(("rec_util_rfi_get_info_len: Fail to get RFI info len (%d)\r\n", i4_ret));
            break;
        }
        
    } while (0);
    
    rec_util_unlock_with_tag (REC_UTIL_TAG_RFI_GET_INFO_LEN);
    
    return i4_ret;    
}  

