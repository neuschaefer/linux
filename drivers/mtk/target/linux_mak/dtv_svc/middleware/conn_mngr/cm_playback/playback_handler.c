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
 * $RCSfile: playback_handler.c,v $
 * $Revision: #4 $
 * $Date: 2012/11/02 $
 * $Author: he.yang $
 *
 * Description:
 *         This file contains the Playback Connection Handler main functions
 *         including the handler registration, unregistration and thread
 *         main function.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifdef LINUX_TURNKEY_SOLUTION
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>
#ifndef ANDROID
#include <sys/shm.h>
#endif
#endif

#include "conn_mngr/cm_playback/x_playback_handler.h"
#include "conn_mngr/cm_playback/_playback_handler.h"
#include "conn_mngr/cm_playback/playback_handler_cmd.h"
#include "os/inc/x_os.h"
#include "conn_mngr/x_cm.h"
#include "conn_mngr/cm.h"
#include "conn_mngr/cm_handler_api.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_buf_agt.h"
#include "scdb/x_scdb_api.h"
#include "scdb/scdb_api.h"
#include "svl/x_svl.h"
#include "file_mngr/x_fm.h"
#include "file_mngr/x_fm_mfw.h"
#include "tbl_mngr/x_tm.h"
#include "tbl_mngr/psi_eng/x_psi_eng.h"
#include "tbl_mngr/atsc_si_eng/x_atsc_si_eng.h"
#include "tbl_mngr/dvb_si_eng/x_dvb_si_eng.h"
#include "dt/x_dt.h"
#include "dbg/def_dbg_level_mw.h"
#include "inet/http/x_httpc_api.h"

#ifdef ENABLE_MMS_SUPPORT
#include "inet/mms/include/libmms/mms_streaming.h"
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifndef __UCLIBC__
#define PLAYBACK_HANDLER_USE_ASYNC_FM_READ  1
#else
#define PLAYBACK_HANDLER_USE_ASYNC_FM_READ  0
#endif

#define PLAYBACK_HANDLER_NO_STREAM_TAG_ASSIGNED  ((UINT32) 0x80000000)

#define PLAYBACK_HANDLER_CIRCULAR_READ_UNIT ((UINT32)64*1024)

#define PLAYBACK_HANDLER_HTTP_PRE_BUF_SIZE ((UINT32)0x800000)

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static UINT32 ui4_total_time = 0;
static UINT32 ui4_start_time = 0;
static UINT32 ui4_end_time = 0;
static UINT32 ui4_total_bytes = 0;

static PLAYBACK_CONN_T*  pt_conn_list = NULL;

static HANDLE_T  h_playback_handler      = NULL_HANDLE;
static HANDLE_T  h_playback_handler_msgq = NULL_HANDLE;
static HANDLE_T  h_playback_handler_sema = NULL_HANDLE;
static BOOL      b_init                  = FALSE;
static BOOL      b_log_cmpb              = FALSE;

static UINT16  ui2_playback_handler_dbg_level = DBG_INIT_LEVEL_MW_CM_PLAYBACK;

static DATA_FMT_T  ae_compatible_audio_formats[] = {
    DATA_FMT_MP3,
    DATA_FMT_MP2,
    DATA_FMT_AAC,
    DATA_FMT_UNKNOWN /* end marker */
};

static INT32
playback_conn_update_scdb_audio (PLAYBACK_CONN_T*  pt_conn,
                                 DATA_FMT_T        e_data_fmt);

static PLAYBACK_BUF_AGT_T*
playback_conn_get_buf_agt_by_handle (HANDLE_T  h_buffer_agent);

static VOID
playback_conn_set_item_position (PLAYBACK_CONN_T*  pt_conn,
                                 PLAYBACK_ITEM_T*  pt_item,
                                 UINT64            ui8_position);

static VOID playback_conn_close_svl (PLAYBACK_CONN_T*  pt_conn);
static VOID playback_conn_delete_clock (PLAYBACK_BUF_AGT_T*  pt_buf_agt);
static VOID playback_conn_delete_tables (PLAYBACK_CONN_T*     pt_conn,
                                         PLAYBACK_BUF_AGT_T*  pt_buf_agt);
static INT32 playback_conn_transfer_circular_data (PLAYBACK_CONN_T*     pt_conn,
                                                   PLAYBACK_BUF_AGT_T*  pt_buf_agt);
static PLAYBACK_FORMAT_INFO_T  at_formats[] = {
    /* audio formats */
    {
        ae_compatible_audio_formats,
        playback_conn_update_scdb_audio,
        PLAYBACK_FLAG_NONE
    },
    /* end marker */
    {
        NULL
    }
};



VOID playback_handler_cmpb_log(BOOL b_enable)
{
    b_log_cmpb = b_enable;
}


VOID playback_handler_pperf_log(VOID)
{
    x_dbg_stmt("****************************************\n");
    x_dbg_stmt("** total tick = %d\n", ui4_total_time);
    x_dbg_stmt("** total bytes = %d\n", ui4_total_bytes);
#ifdef LINUX_TURNKEY_SOLUTION
    x_dbg_stmt("** HZ = %d\n", sysconf(_SC_CLK_TCK));
#else
    x_dbg_stmt("** HZ = %d\n", x_os_get_sys_tick_period());
#endif
    x_dbg_stmt("****************************************\n");
}

VOID playback_handler_rperf_log(VOID)
{
    ui4_total_time = 0;
    ui4_start_time = 0;
    ui4_end_time = 0;
    ui4_total_bytes = 0;
}

static VOID playback_uri_circular_nfy (HANDLE_T         h_req,
                                       VOID*            pv_tag,
                                       FM_ASYNC_COND_T  e_nfy_cond,
                                       UINT32           ui4_data)
{
    PLAYBACK_BUF_AGT_T* pt_buf_agt;

    pt_buf_agt = playback_conn_get_buf_agt_by_handle((HANDLE_T)pv_tag);

    if (b_log_cmpb)
    {
        x_dbg_stmt("[CMPB] playback_uri_circular_nfy %d %d\n", e_nfy_cond, ui4_data);
    }

    if (pt_buf_agt)
    {
        if (pt_buf_agt->t_circular_req.h_buffer_agent == (HANDLE_T)pv_tag)
        {
            pt_buf_agt->t_circular_req.e_nfy_cond = e_nfy_cond;
            pt_buf_agt->t_circular_req.ui4_data_len = ui4_data;
            cm_send_msg_no_retry(h_playback_handler_msgq,
                                 PLAYBACK_HANDLER_CIRCULAR_REQUEST,
                                 (UINT32) pt_buf_agt->t_circular_req.h_buffer_agent,
                                 0,
                                 pt_buf_agt->h_ch_conn);
        }
        else
        {
            DBG_ERROR((DBG_PREFIX"playback_uri_circular_nfy wrong buffer agent handle 1\n"));
        }
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"playback_uri_circular_nfy wrong buffer agent handle 2\n"));
    }
}


static VOID playback_uri_issue_req (PLAYBACK_CONN_T*     pt_conn,
                                    PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    if (!pt_conn->b_disconnecting)
    {
        if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT)
        {
            if (!pt_buf_agt->b_issue_circular_req)
            {
                UINT32                      ui4_fill_size;
                UINT32                      ui4_free_size;
                BOOL                        b_issue_req;
                INT32                       i4_res = 0;
                PLAYBACK_ITEM_T*            pt_item;
                PLAYBACK_DATA_REQUEST_T*    pt_request;

                pt_item = pt_buf_agt->pt_transferring_item;

                if (pt_item)
                {
                    pt_request = pt_buf_agt->pt_requests_head;
                    ui4_fill_size = playback_cb_get_filled_size(pt_buf_agt->pt_cb);
                    ui4_free_size = playback_cb_get_free_size(pt_buf_agt->pt_cb);

#ifdef ENABLE_MMS_SUPPORT
                    if (!pt_item->u.t_uri.b_mms)
                    {
#endif
                        if ((NULL==pt_buf_agt->pt_unconsumed_requests)
                            && (pt_request!=NULL))
                        {
                            if ((pt_request->ui8_transfer_position+pt_buf_agt->pt_cb->ui4_pre_space < pt_item->ui8_position)
                                || (pt_request->ui8_transfer_position > pt_item->ui8_position + ui4_fill_size))
                            {
                                if (b_log_cmpb)
                                {
                                    x_dbg_stmt("[CMPB] playback_uri_issue_req reset a position=%d\n", (UINT32)pt_request->ui8_transfer_position);
                                }
                                /*reset tranfer item position*/
                                playback_cb_reset(pt_buf_agt->pt_cb);
                                playback_conn_set_item_position (pt_conn,
                                                                 pt_item,
                                                                 pt_request->ui8_transfer_position);
                                ui4_fill_size = playback_cb_get_filled_size(pt_buf_agt->pt_cb);
                                ui4_free_size = playback_cb_get_free_size(pt_buf_agt->pt_cb);
                            }
                        }
#ifdef ENABLE_MMS_SUPPORT
                    }
#endif

                    if ((pt_item->ui8_position+ui4_fill_size) >= pt_item->ui8_end_offset)
                    {
                        return;
                    }

                    b_issue_req = FALSE;
                    if (pt_buf_agt->b_need_pull)
                    {
                        if (ui4_free_size >= 2*PLAYBACK_HANDLER_CIRCULAR_READ_UNIT)
                        {
                            b_issue_req = TRUE;
                        }
                        else
                        {
                            b_issue_req = FALSE;
                            pt_buf_agt->b_need_pull = FALSE;
                        }
                    }
                    else
                    {
                        if ((ui4_fill_size<=pt_buf_agt->t_flow_ctrl.ui4_re_buferf_threshold)
                            && (ui4_free_size >= 2*PLAYBACK_HANDLER_CIRCULAR_READ_UNIT))
                        {
                            pt_buf_agt->b_need_pull = TRUE;
                            b_issue_req = TRUE;
                        }
                    }

                    if (!b_issue_req)
                    {
                        if ((NULL==pt_buf_agt->pt_unconsumed_requests)
                            && (pt_request!=NULL))
                        {
                            if (((pt_request->ui8_transfer_position + pt_buf_agt->pt_cb->ui4_pre_space) >= pt_item->ui8_position) &&
                                ((pt_request->ui8_transfer_position+pt_request->z_size)<=(pt_item->ui8_position+ui4_fill_size)))
                            {
                            }
                            else
                            {
                                b_issue_req = TRUE;
                                pt_buf_agt->b_need_pull = TRUE;
                                if (b_log_cmpb)
                                {
                                    x_dbg_stmt("[CMPB] playback_uri_issue_req reset b position=%d\n", pt_request->ui8_transfer_position);
                                }
                                playback_cb_reset(pt_buf_agt->pt_cb);
                                playback_conn_set_item_position (pt_conn,
                                                                 pt_item,
                                                                 pt_request->ui8_transfer_position);
                                ui4_fill_size = playback_cb_get_filled_size(pt_buf_agt->pt_cb);
                                ui4_free_size = playback_cb_get_free_size(pt_buf_agt->pt_cb);
                            }
                        }
                    }

                    if (b_issue_req)
                    {
                        pt_item = pt_buf_agt->pt_transferring_item;
                        if (pt_item)
                        {
                            UINT8* pui1_array;

                            pt_buf_agt->t_circular_req.ui4_data_len = 0;
                            pt_buf_agt->t_circular_req.h_buffer_agent = pt_buf_agt->h_buffer_agent;
                            pt_buf_agt->b_issue_circular_req = TRUE;


                            if ((pt_buf_agt->pt_cb->pui1_tail>=pt_buf_agt->pt_cb->pui1_head)
                                && ((UINT32)((UINT32)pt_buf_agt->pt_cb->pui1_tail-(UINT32)pt_buf_agt->pt_cb->pui1_begin)>=pt_buf_agt->pt_cb->ui4_size))
                            {
                                pui1_array = pt_buf_agt->pt_cb->pui1_begin;
                            }
                            else
                            {
                                pui1_array = pt_buf_agt->pt_cb->pui1_tail;
                            }

#ifdef ENABLE_MMS_SUPPORT
                            if (pt_item->u.t_uri.b_mms)
                            {
                                UINT32 ui4_mms_read = 0;

                                i4_res = x_mms_fm_pull_read(pt_item->u.t_uri.h_uri,
                                                            0,
                                                            (CHAR*)pui1_array,
                                                            PLAYBACK_HANDLER_CIRCULAR_READ_UNIT,
                                                            &ui4_mms_read);

                                if ((MMS_ERROR_CODE_OK == i4_res) || (MMS_ERROR_CODE_FILE_EOF == i4_res))
                                {
                                    if (MMS_ERROR_CODE_OK == i4_res)
                                    {
                                        pt_buf_agt->t_circular_req.e_nfy_cond = FM_ASYNC_COND_READ_OK;
                                    }
                                    else
                                    {
                                        pt_buf_agt->t_circular_req.e_nfy_cond = FM_ASYNC_COND_EOF;
                                    }
                                    pt_buf_agt->t_circular_req.ui4_data_len = ui4_mms_read;
            
                                    cm_send_msg_no_retry(h_playback_handler_msgq,
                                                         PLAYBACK_HANDLER_CIRCULAR_REQUEST,
                                                         (UINT32) pt_buf_agt->t_circular_req.h_buffer_agent,
                                                         0,
                                                         pt_buf_agt->h_ch_conn);
                                }
                                else
                                {
                                    i4_res = FMR_CORE;
                                }
                            }
                            else 
#endif
							if (pt_item->u.t_uri.b_url)
                            {
    #ifdef INET_SUPPORT
                                i4_res = x_httpc_fm_read_async(pt_item->u.t_uri.h_uri,
                                                               pui1_array,
                                                               PLAYBACK_HANDLER_CIRCULAR_READ_UNIT,
                                                               pt_item->u.t_uri.ui1_priority,
                                                               playback_uri_circular_nfy,
                                                               (VOID*) pt_buf_agt->t_circular_req.h_buffer_agent,
                                                               &pt_buf_agt->t_circular_req.h_async_read);
                                if (b_log_cmpb)
                                {
                                    x_dbg_stmt("[CMPB] playback_uri_issue_req x_httpc_fm_read_async %d\n", i4_res);
                                }

                                if (FMR_EOF == i4_res)
                                {
                                    x_dbg_stmt("[CMPB] CM_PLAYBACK_CONN_INFO_EOF 1\n");
                                    cm_nfy(pt_conn->h_cm_conn,
                                           CM_PLAYBACK_CONN_INFO_EOF,
                                           0);
                                }
    #endif
                            }
                            else
                            {
                                i4_res = x_fm_mfw_read_async(pt_item->u.t_uri.h_uri,
                                                             pui1_array,
                                                             PLAYBACK_HANDLER_CIRCULAR_READ_UNIT,
                                                             pt_item->u.t_uri.ui1_priority,
                                                             playback_uri_circular_nfy,
                                                             (VOID*) pt_buf_agt->t_circular_req.h_buffer_agent,
                                                             &pt_buf_agt->t_circular_req.h_async_read);
                            }

                            if (i4_res != FMR_OK)
                            {
                                DBG_ERROR((DBG_PREFIX"playback_uri_issue_req could not read async file handle %d\n",
                                pt_item->u.t_uri.h_uri));
                                pt_buf_agt->b_need_pull = FALSE;
                                pt_buf_agt->b_issue_circular_req = FALSE;

                                if (FMR_CORE == i4_res)
                                {
                                    x_dbg_stmt("[CMPB] playback_uri_issue_req b_conn_error=TRUE\n");
                                    if (!pt_buf_agt->b_conn_error)
                                    {
                                        pt_buf_agt->b_conn_error = TRUE;
                                        cm_nfy(pt_conn->h_cm_conn,
                                               CM_PLAYBACK_CONN_INFO_FILE_ERROR,
                                               0);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: playback_handler_get_dbg_level
 *
 * Description: This API gets the debug level of the Playback Handler.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: the Playback Handler debug level.
 ----------------------------------------------------------------------------*/
UINT16 playback_handler_get_dbg_level (VOID)
{
    return ui2_playback_handler_dbg_level;
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_set_dbg_level
 *
 * Description: This API sets the debug level of the Playback Handler.
 *
 * Inputs:  ui2_dbg_level  Contains the new debug level.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The debug level was set successfully.
 *          CMR_FAIL  The debug level could not be set.
 ----------------------------------------------------------------------------*/
INT32 playback_handler_set_dbg_level (UINT16  ui2_dbg_level)
{
    ui2_playback_handler_dbg_level = ui2_dbg_level;

    return CMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_link_conn
 *
 * Description: This API adds a connection structure to the global list.
 *
 * Inputs:  pt_conn    References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_handler_link_conn (PLAYBACK_CONN_T*  pt_conn)
{
    pt_conn->pt_next = pt_conn_list;
    pt_conn_list     = pt_conn;
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_delink_conn
 *
 * Description: This API removes a connection structure from the global list.
 *
 * Inputs:  pt_conn    References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_handler_delink_conn (PLAYBACK_CONN_T*  pt_conn)
{
    PLAYBACK_CONN_T**  ppt_conn;

    ppt_conn = & pt_conn_list;

    while (*ppt_conn != NULL)
    {
        if (*ppt_conn == pt_conn)
        {
            (*ppt_conn)      = pt_conn->pt_next;
            pt_conn->pt_next = NULL;

            break;
        }

        ppt_conn = & ((*ppt_conn)->pt_next);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_list_connections
 *
 * Description: This API lists all the playback connections.
 *
 * Inputs:  b_send_message  Specifies whether a message should be sent to the
 *                          main loop so the connections are listed in the
 *                          context of the playback handler main loop.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID playback_handler_list_connections (BOOL  b_send_message)
{
#ifdef DEBUG
    if (! b_send_message)
    {
        PLAYBACK_CONN_T*  pt_conn = pt_conn_list;

        x_dbg_stmt(DBG_PREFIX"Playback handler connections:\n");

        while (pt_conn != NULL)
        {
            x_dbg_stmt(DBG_PREFIX" %d\n", pt_conn->h_ch_conn);

            pt_conn = pt_conn->pt_next;
        }

        x_dbg_stmt(DBG_PREFIX"End of connections\n");
    }
    else
    {
        cm_send_msg_retry(h_playback_handler_msgq,
                          PLAYBACK_HANDLER_MSG_LIST_CONNECTIONS,
                          0, 0, NULL_HANDLE);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_list_playback_items
 *
 * Description: This API lists all the playback items.
 *
 * Inputs:  b_send_message  Specifies whether a message should be sent to the
 *                          main loop so the connections are listed in the
 *                          context of the playback handler main loop.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID playback_handler_list_playback_items (HANDLE_T  h_ch_conn,
                                           BOOL      b_send_message)
{
#ifdef DEBUG
    if (! b_send_message)
    {
        PLAYBACK_CONN_T*  pt_conn;
        HANDLE_TYPE_T     e_type;
        INT32             i4_res;

        i4_res = handle_get_type_obj(h_ch_conn, & e_type, (VOID**) & pt_conn);

        if ((i4_res == HR_OK) && (e_type == PHT_CONNECTION))
        {
            PLAYBACK_BUF_AGT_T*  pt_buf_agt;

            pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

            while (pt_buf_agt != NULL)
            {
                PLAYBACK_ITEM_T*  pt_item;

                x_dbg_stmt(DBG_PREFIX"Playback items (%d):\n", h_ch_conn);

                pt_item = pt_buf_agt->pt_items_head;

                while (pt_item != NULL)
                {
                    const CHAR*  ps_state;

                    switch (pt_item->e_state)
                    {
                        case PLAYBACK_ITEM_STATE_QUEUED:
                            ps_state = "QUEUED";
                            break;

                        case PLAYBACK_ITEM_STATE_UNQUEUED:
                            ps_state = "UNQUEUED";
                            break;

                        case PLAYBACK_ITEM_STATE_TRANSFER:
                            ps_state = "TRANSFER";
                            break;

                        default:
                            ps_state = "unknown";
                    }         
					
                    x_dbg_stmt(DBG_PREFIX
                               " id:%u (format:%d, type:%d, state:%s)\n",
                               pt_item->ui4_id,
                               pt_item->e_format,
                               pt_item->e_type,
                               ps_state);
					
                    pt_item = pt_item->pt_next;
                }

                pt_buf_agt = pt_buf_agt->pt_next;
            }
        }
    }
    else
    {
        cm_send_msg_retry(h_playback_handler_msgq,
                          PLAYBACK_HANDLER_MSG_LIST_PLAYBACK_ITEMS,
                          0, 0, h_ch_conn);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_list_all_playback_items
 *
 * Description: This API lists all the playback items for all the connections.
 *
 * Inputs:  b_send_message  Specifies whether a message should be sent to the
 *                          main loop so the connections are listed in the
 *                          context of the playback handler main loop.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID playback_handler_list_all_playback_items (BOOL  b_send_message)
{
#ifdef DEBUG
    if (! b_send_message)
    {
        PLAYBACK_CONN_T*  pt_conn = pt_conn_list;

        while (pt_conn != NULL)
        {
            playback_handler_list_playback_items(pt_conn->h_ch_conn, FALSE);

            pt_conn = pt_conn->pt_next;
        }
    }
    else
    {
        cm_send_msg_retry(h_playback_handler_msgq,
                          PLAYBACK_HANDLER_MSG_LIST_ALL_PLAYBACK_ITEMS,
                          0, 0, NULL_HANDLE);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_list_data_requests
 *
 * Description: This API lists all the data requests of a specific connection.
 *
 * Inputs:  b_send_message  Specifies whether a message should be sent to the
 *                          main loop so the connections are listed in the
 *                          context of the playback handler main loop.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID playback_handler_list_data_requests (HANDLE_T  h_ch_conn,
                                          BOOL      b_send_message)
{
#ifdef DEBUG
    if (! b_send_message)
    {
        PLAYBACK_CONN_T*  pt_conn;
        HANDLE_TYPE_T     e_type;
        INT32             i4_res;

        i4_res = handle_get_type_obj(h_ch_conn, & e_type, (VOID**) & pt_conn);

        if ((i4_res == HR_OK) && (e_type == PHT_CONNECTION))
        {
            PLAYBACK_BUF_AGT_T*  pt_buf_agt;

            pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

            while (pt_buf_agt != NULL)
            {
                PLAYBACK_DATA_REQUEST_T*  pt_request;

                x_dbg_stmt(DBG_PREFIX"Pending data requests (%d):\n",
                           h_ch_conn);

                pt_request = pt_buf_agt->pt_requests_head;

                if (pt_request != NULL)
                {
                    do
                    {
                        x_dbg_stmt(DBG_PREFIX" id:%u (size:%d)\n",
                                   pt_request->ui4_req_id,
                                   pt_request->z_size);

                        pt_request = pt_request->pt_next;
                    }
                    while (pt_request != NULL);
                }
                else
                {
                    x_dbg_stmt(DBG_PREFIX" (none)\n");
                }

                x_dbg_stmt(DBG_PREFIX"Unconsumed data requests (%d):\n",
                           h_ch_conn);

                pt_request = pt_buf_agt->pt_unconsumed_requests;

                if (pt_request != NULL)
                {
                    do
                    {
                        x_dbg_stmt(DBG_PREFIX" id:%u (size:%u, item:%u)\n",
                                   pt_request->ui4_req_id,
                                   pt_request->z_size,
                                   pt_request->pt_item ? pt_request->pt_item->ui4_id : 0);

                        pt_request = pt_request->pt_next;
                    }
                    while (pt_request != NULL);
                }
                else
                {
                    x_dbg_stmt(DBG_PREFIX" (none)\n");
                }

                pt_buf_agt = pt_buf_agt->pt_next;
            }
        }
    }
    else
    {
        cm_send_msg_retry(h_playback_handler_msgq,
                          PLAYBACK_HANDLER_MSG_LIST_DATA_REQUESTS,
                          0, 0, h_ch_conn);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_list_all_data_requests
 *
 * Description: This API lists all the data requests for all the connections.
 *
 * Inputs:  b_send_message  Specifies whether a message should be sent to the
 *                          main loop so the connections are listed in the
 *                          context of the playback handler main loop.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID playback_handler_list_all_data_requests (BOOL  b_send_message)
{
#ifdef DEBUG
    if (! b_send_message)
    {
        PLAYBACK_CONN_T*  pt_conn = pt_conn_list;

        while (pt_conn != NULL)
        {
            playback_handler_list_data_requests(pt_conn->h_ch_conn, FALSE);

            pt_conn = pt_conn->pt_next;
        }
    }
    else
    {
        cm_send_msg_retry(h_playback_handler_msgq,
                          PLAYBACK_HANDLER_MSG_LIST_ALL_DATA_REQUESTS,
                          0, 0, NULL_HANDLE);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_list_items_and_requests
 *
 * Description: This API lists all the playback items and data requests of a
 *              specific connection.
 *
 * Inputs:  h_ch_conn       Contains the connection handle.
 *          b_send_message  Specifies whether a message should be sent to the
 *                          main loop so the items and requests are listed in
 *                          the context of the playback handler main loop.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID playback_handler_list_items_and_requests (HANDLE_T  h_ch_conn,
                                               BOOL      b_send_message)
{
#ifdef DEBUG
    if (! b_send_message)
    {
        playback_handler_list_playback_items(h_ch_conn, FALSE);

        playback_handler_list_data_requests(h_ch_conn, FALSE);
    }
    else
    {
        cm_send_msg_retry(h_playback_handler_msgq,
                          PLAYBACK_HANDLER_MSG_LIST_ITEMS_AND_REQUESTS,
                          0, 0, h_ch_conn);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_list_all_items_and_requests
 *
 * Description: This API lists all the playback items and data requests for all
 *              the connections.
 *
 * Inputs:  b_send_message  Specifies whether a message should be sent to the
 *                          main loop so the items and requests are listed in
 *                          the context of the playback handler main loop.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID playback_handler_list_all_items_and_requests (BOOL  b_send_message)
{
#ifdef DEBUG
    if (! b_send_message)
    {
        PLAYBACK_CONN_T*  pt_conn = pt_conn_list;

        while (pt_conn != NULL)
        {
            playback_handler_list_items_and_requests(pt_conn->h_ch_conn,
                                                     FALSE);

            pt_conn = pt_conn->pt_next;
        }
    }
    else
    {
        cm_send_msg_retry(h_playback_handler_msgq,
                          PLAYBACK_HANDLER_MSG_LIST_ALL_ITEMS_AND_REQUESTS,
                          0, 0, NULL_HANDLE);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_show_scdb
 *
 * Description: This API shows the content of a specific connection's SCDB.
 *
 * Inputs:  b_send_message  Specifies whether a message should be sent to the
 *                          main loop so the connections are listed in the
 *                          context of the playback handler main loop.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID playback_handler_show_scdb (HANDLE_T  h_ch_conn,
                                 BOOL      b_send_message)
{
#ifdef DEBUG
    if (! b_send_message)
    {
        PLAYBACK_CONN_T*  pt_conn;
        HANDLE_TYPE_T     e_type;
        INT32             i4_res;

        i4_res = handle_get_type_obj(h_ch_conn, & e_type, (VOID**) & pt_conn);

        if ((i4_res == HR_OK) && (e_type == PHT_CONNECTION))
        {
            cm_conn_show_scdb(pt_conn->h_cm_conn, DBG_PREFIX);
        }
    }
    else
    {
        cm_send_msg_retry(h_playback_handler_msgq,
                          PLAYBACK_HANDLER_MSG_SHOW_SCDB,
                          0, 0, h_ch_conn);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_show_all_scdbs
 *
 * Description: This API shows all the SCDBs for all the connections.
 *
 * Inputs:  b_send_message  Specifies whether a message should be sent to the
 *                          main loop so the SCDBs are listed in the context of
 *                          the playback handler main loop.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID playback_handler_show_all_scdbs (BOOL  b_send_message)
{
    if (! b_send_message)
    {
        PLAYBACK_CONN_T*  pt_conn = pt_conn_list;

        while (pt_conn != NULL)
        {
            playback_handler_show_scdb(pt_conn->h_ch_conn, FALSE);

            pt_conn = pt_conn->pt_next;
        }
    }
    else
    {
        cm_send_msg_retry(h_playback_handler_msgq,
                          PLAYBACK_HANDLER_MSG_SHOW_ALL_SCDBS,
                          0, 0, NULL_HANDLE);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_show_pat
 *
 * Description: This API shows the content of a specific connection's PAT.
 *
 * Inputs:  h_ch_conn       Contains the connection handle.
 *          b_send_message  Specifies whether a message should be sent to the
 *                          main loop so the content of the PAT is listed in
 *                          the context of the playback handler main loop.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID playback_handler_show_pat (HANDLE_T  h_ch_conn,
                                BOOL      b_send_message)
{
#ifdef DEBUG
    if (! b_send_message)
    {
        PLAYBACK_CONN_T*  pt_conn;
        HANDLE_TYPE_T     e_type;
        INT32             i4_res;

        i4_res = handle_get_type_obj(h_ch_conn, & e_type, (VOID**) & pt_conn);

        if ((i4_res == HR_OK) && (e_type == PHT_CONNECTION))
        {
            PLAYBACK_BUF_AGT_T*  pt_buf_agt;

            pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

            while (pt_buf_agt != NULL)
            {
                cm_show_pat(pt_buf_agt->h_pat, DBG_PREFIX);

                pt_buf_agt = pt_buf_agt->pt_next;
            }
        }
    }
    else
    {
        cm_send_msg_retry(h_playback_handler_msgq,
                          PLAYBACK_HANDLER_MSG_SHOW_PAT,
                          0, 0, h_ch_conn);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_show_all_pats
 *
 * Description: This API shows all the PATs for all the connections.
 *
 * Inputs:  b_send_message  Specifies whether a message should be sent to the
 *                          main loop so the PATs are listed in the context of
 *                          the playback handler main loop.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID playback_handler_show_all_pats (BOOL  b_send_message)
{
    if (! b_send_message)
    {
        PLAYBACK_CONN_T*  pt_conn = pt_conn_list;

        while (pt_conn != NULL)
        {
            playback_handler_show_pat(pt_conn->h_ch_conn, FALSE);

            pt_conn = pt_conn->pt_next;
        }
    }
    else
    {
        cm_send_msg_retry(h_playback_handler_msgq,
                          PLAYBACK_HANDLER_MSG_SHOW_ALL_PATS,
                          0, 0, NULL_HANDLE);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_show_pmt
 *
 * Description: This API shows the content of a specific connection's PMT.
 *
 * Inputs:  h_ch_conn       Contains the connection handle.
 *          b_send_message  Specifies whether a message should be sent to the
 *                          main loop so the content of the PMT is listed in
 *                          the context of the playback handler main loop.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID playback_handler_show_pmt (HANDLE_T  h_ch_conn,
                                BOOL      b_send_message)
{
#ifdef DEBUG
    if (! b_send_message)
    {
        PLAYBACK_CONN_T*  pt_conn;
        HANDLE_TYPE_T     e_type;
        INT32             i4_res;

        i4_res = handle_get_type_obj(h_ch_conn, & e_type, (VOID**) & pt_conn);

        if ((i4_res == HR_OK) && (e_type == PHT_CONNECTION))
        {
            PLAYBACK_BUF_AGT_T*  pt_buf_agt;

            pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

            while (pt_buf_agt != NULL)
            {
                cm_show_pmt(pt_buf_agt->h_pmt, DBG_PREFIX);

                pt_buf_agt = pt_buf_agt->pt_next;
            }
        }
    }
    else
    {
        cm_send_msg_retry(h_playback_handler_msgq,
                          PLAYBACK_HANDLER_MSG_SHOW_PMT,
                          0, 0, h_ch_conn);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_show_all_pmts
 *
 * Description: This API shows all the PMTs for all the connections.
 *
 * Inputs:  b_send_message  Specifies whether a message should be sent to the
 *                          main loop so the PMTs are listed in the context of
 *                          the playback handler main loop.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID playback_handler_show_all_pmts (BOOL  b_send_message)
{
    if (! b_send_message)
    {
        PLAYBACK_CONN_T*  pt_conn = pt_conn_list;

        while (pt_conn != NULL)
        {
            playback_handler_show_pmt(pt_conn->h_ch_conn, FALSE);

            pt_conn = pt_conn->pt_next;
        }
    }
    else
    {
        cm_send_msg_retry(h_playback_handler_msgq,
                          PLAYBACK_HANDLER_MSG_SHOW_ALL_PMTS,
                          0, 0, NULL_HANDLE);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_assign_id
 *
 * Description: This API returns a unique id for a specified connection's
 *              item to play back.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static UINT32 playback_conn_assign_id (HANDLE_T  h_ch_conn)
{
    CRIT_STATE_T      t_state;
    PLAYBACK_CONN_T*  pt_conn;
    UINT32            ui4_id;
    INT32             i4_res;

    t_state = x_crit_start();

    i4_res = handle_get_obj(h_ch_conn, (VOID**) & pt_conn);

    if (i4_res == HR_OK)
    {
        do
        {
            pt_conn->ui4_playback_item_id++;
        }
        while (pt_conn->ui4_playback_item_id
               ==
               PLAYBACK_HANDLER_INVALID_ITEM_ID);

        ui4_id = pt_conn->ui4_playback_item_id;
    }
    else
    {
        ui4_id = PLAYBACK_HANDLER_INVALID_ITEM_ID;
    }

    x_crit_end(t_state);

    return ui4_id;
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_get_playback_item
 *
 * Description: This API allocates memory for a playback item.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: a valid playback item, or NULL.
 ----------------------------------------------------------------------------*/
static PLAYBACK_ITEM_T* playback_handler_get_playback_item (VOID)
{
    PLAYBACK_ITEM_T*  pt_item;

    pt_item = (PLAYBACK_ITEM_T *) x_mem_alloc(sizeof(PLAYBACK_ITEM_T));

    if (pt_item != NULL)
    {
        pt_item->pt_scdb_info = NULL;
        pt_item->pt_next      = NULL;
    }

    DBG_INFO((DBG_PREFIX"Allocated memory %p for playback item\n", pt_item));

    return pt_item;
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_release_item
 *
 * Description: This API releases a playback item. The file is closed if the
 *              item was a file, and the memory is freed.
 *
 * Inputs:  pt_item  References the item structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_handler_release_item (PLAYBACK_ITEM_T*  pt_item)
{
    if ((pt_item->e_type == PLAYBACK_ITEM_TYPE_FILE)
        &&
        (pt_item->u.t_file.h_file != NULL_HANDLE))
    {
        INT32  i4_res;

        DBG_INFO((DBG_PREFIX"Closing file handle %d\n",
                  pt_item->u.t_file.h_file));

        i4_res = x_fm_mfw_close(pt_item->u.t_file.h_file);

        if (i4_res != FMR_OK)
        {
            DBG_ERROR((DBG_PREFIX"Could not close file handle %d\n",
                       pt_item->u.t_file.h_file));
        }
    }
    else if ((pt_item->e_type == PLAYBACK_ITEM_TYPE_PULL)
        &&
        (pt_item->u.t_pull.t_pull_info.h_pb != NULL_HANDLE))
    {
#ifdef LINUX_TURNKEY_SOLUTION
        if (pt_item->u.t_pull.t_pull_info.pf_dissociate)
        {
            pt_item->u.t_pull.t_pull_info.pf_dissociate(
                pt_item->u.t_pull.h_pull,
                pt_item->u.t_pull.t_pull_info.pv_app_tag
                );
        }
#endif
        pt_item->u.t_pull.t_pull_info.pf_close(pt_item->u.t_pull.h_pull,
                                               pt_item->u.t_pull.t_pull_info.pv_app_tag);
    }
    else if ((pt_item->e_type == PLAYBACK_ITEM_TYPE_URI)
        &&
        (pt_item->u.t_uri.h_uri != NULL_HANDLE))
    {
        INT32  i4_res = 0;

        DBG_INFO((DBG_PREFIX"Closing URI handle %d, b_url=%d\n",
                  pt_item->u.t_uri.h_uri, pt_item->u.t_uri.b_url));

#ifdef ENABLE_MMS_SUPPORT
        if (pt_item->u.t_uri.b_mms)
        {
            if (pt_item->u.t_uri.b_close_uri)
            {
                i4_res = x_mms_fm_pull_close(pt_item->u.t_uri.h_uri, 0);
            }
        }
        else 
#endif
		if (pt_item->u.t_uri.b_url)
        {
#ifdef INET_SUPPORT
            if (pt_item->u.t_uri.b_close_uri)
            {
                i4_res = x_httpc_fm_close(pt_item->u.t_uri.h_uri);
            }
#endif
        }
        else
        {
            i4_res = x_fm_mfw_close(pt_item->u.t_uri.h_uri);
        }

        if (i4_res != FMR_OK)
        {
            DBG_ERROR((DBG_PREFIX"Could not close URI handle %d, b_url=%d\n",
                       pt_item->u.t_uri.h_uri, pt_item->u.t_uri.b_url));
        }
    }

    if (pt_item->pt_scdb_info != NULL)
    {
        DBG_INFO((DBG_PREFIX"Freeing playback item's SCDB info\n"));

        x_mem_free(pt_item->pt_scdb_info);
    }

    DBG_INFO((DBG_PREFIX"Freeing playback item memory %p (playback item %u)\n",
              pt_item, pt_item->ui4_id));

    x_mem_free(pt_item);
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_free_request
 *
 * Description: This API is called when a data request handle is freed.
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
static BOOL playback_conn_free_request (HANDLE_T       h_ch_conn,
                                        HANDLE_TYPE_T  e_type,
                                        VOID*          pv_obj,
                                        VOID*          pv_tag,
                                        BOOL           b_req_handle)
{
    BOOL  b_res;

    if (! b_req_handle)
    {
        PLAYBACK_DATA_REQUEST_T*  pt_request;

        pt_request = (PLAYBACK_DATA_REQUEST_T*) pv_obj;

        DBG_INFO((DBG_PREFIX
                  "Freeing data request memory %p (data request %u)\n",
                  pt_request, pt_request->ui4_req_id));

        x_mem_free(pt_request);

        b_res = TRUE;
    }
    else
    {
        DBG_INFO((DBG_PREFIX"Not allowed to delete handle (%d)\n", h_ch_conn));

        b_res = FALSE;
    }

    return b_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_get_data_request
 *
 * Description: This API allocates memory for a data request.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: a valid data request, or NULL.
 ----------------------------------------------------------------------------*/
static PLAYBACK_DATA_REQUEST_T* playback_handler_get_data_request (VOID)
{
    PLAYBACK_DATA_REQUEST_T*  pt_request;
    HANDLE_T                  h_request;
    INT32                     i4_res;

    i4_res = handle_alloc_and_obj(PHT_DATA_REQUEST,
                                  NULL,
                                  sizeof(PLAYBACK_DATA_REQUEST_T),
                                  playback_conn_free_request,
                                  & h_request,
                                  (VOID**) & pt_request);

    if ((i4_res == HR_OK) && (pt_request != NULL))
    {
        pt_request->h_request = h_request;

        DBG_INFO((DBG_PREFIX"Allocated memory %p for data request\n",
                  pt_request));
    }
    else
    {
        pt_request = NULL;

        DBG_ERROR((DBG_PREFIX"Could not get data request (%d)\n", i4_res));
    }

    return pt_request;
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_release_data_request
 *
 * Description: This API releases a data request. The memory is simply freed.
 *
 * Inputs:  pt_request  References the data request structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_handler_release_data_request (PLAYBACK_DATA_REQUEST_T*  pt_request)
{
    handle_free(pt_request->h_request, FALSE);
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_free_handle
 *
 * Description: This API is called when an playback connection handle is freed.
 *
 * Inputs:  h_buffer_agent  Contains the handle that should be freed.
 *          e_type          Contains the handle type.
 *          pv_obj          References the handle's object.
 *          pv_tag          References the handle's tag.
 *          b_req_handle    Is set to TRUE if the handle free was initiated
 *                          with this handle else FALSE.
 *
 * Outputs: -
 *
 * Returns: TRUE if the handle free is allowed, FALSE otherwise.
 ----------------------------------------------------------------------------*/
static BOOL playback_buf_agt_free_handle (HANDLE_T       h_buffer_agent,
                                          HANDLE_TYPE_T  e_type,
                                          VOID*          pv_obj,
                                          VOID*          pv_tag,
                                          BOOL           b_req_handle)
{
    BOOL  b_res;

    DBG_INFO((DBG_PREFIX"Deleting buffer agent object (%d).\n",
              h_buffer_agent));

    if (! b_req_handle)
    {
        PLAYBACK_BUF_AGT_T*  pt_buf_agt;
        pt_buf_agt = pv_obj;
        if (pt_buf_agt->pt_cb)
        {
            playback_cb_delete(pt_buf_agt->pt_cb);
            pt_buf_agt->pt_cb = NULL;
        }

        x_mem_free(pv_obj);

        b_res = TRUE;
    }
    else
    {
        DBG_INFO((DBG_PREFIX"Not allowed to delete handle (%d)\n",
                  h_buffer_agent));

        b_res = FALSE;
    }

    return b_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_buf_agt_create
 *
 * Description: This API creates a buffer agent structure.
 *
 * Inputs:  ps_name  Specifies the name of the buffer agent to create.
 *
 * Outputs: -
 *
 * Returns: Pointer to buffer agent structure.
 ----------------------------------------------------------------------------*/
PLAYBACK_BUF_AGT_T* playback_buf_agt_create (const CHAR*  ps_name)
{
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
    HANDLE_T             h_buffer_agent;
    INT32                i4_res;

    i4_res = handle_alloc_and_obj(PHT_BUFFER_AGENT,
                                  NULL,
                                  sizeof(PLAYBACK_BUF_AGT_T),
                                  playback_buf_agt_free_handle,
                                  & h_buffer_agent,
                                  (VOID**) & pt_buf_agt);

    if ((i4_res == HR_OK) && (pt_buf_agt != NULL))
    {
        x_strncpy(pt_buf_agt->s_name, ps_name, RM_NAME_LEN);

        pt_buf_agt->s_name[RM_NAME_LEN] = '\0';

        pt_buf_agt->h_buffer_agent               = h_buffer_agent;
        pt_buf_agt->e_rm_connect_buf_agt_cond    = RM_COND_IGNORE;
        pt_buf_agt->e_rm_disconnect_buf_agt_cond = RM_COND_IGNORE;
        pt_buf_agt->b_nfy_transfer_start         = TRUE;
        pt_buf_agt->b_shared_memory              = FALSE;
    }
    else
    {
        pt_buf_agt = NULL;
    }

    return pt_buf_agt;
}

/*-----------------------------------------------------------------------------
 * Name: playback_buf_agt_delete
 *
 * Description: This API deletes a buffer agent structure.
 *
 * Inputs:  pt_request  References the data request structure to delete.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID playback_buf_agt_delete (PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    handle_free(pt_buf_agt->h_buffer_agent, FALSE);
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_get_buf_agt
 *
 * Description: This API finds a buffer agent structure given a resource
 *              manager component handle.
 *
 * Inputs:  pt_conn    References the connection structure.
 *          h_rm_comp  Contains the component handle.
 *
 * Outputs: -
 *
 * Returns: Pointer to buffer agent structure, or NULL if not found.
 ----------------------------------------------------------------------------*/
static PLAYBACK_BUF_AGT_T*
playback_conn_get_buf_agt (PLAYBACK_CONN_T*  pt_conn,
                           HANDLE_T          h_rm_comp)
{
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        if (pt_buf_agt->h_rm_buffer_agent == h_rm_comp)
        {
            break;
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }

    return pt_buf_agt;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_get_buf_agt_by_handle
 *
 * Description: This API finds a buffer agent structure given a handle.
 *
 * Inputs:  h_buffer_agent  Contains the component handle.
 *
 * Outputs: -
 *
 * Returns: Pointer to buffer agent structure, or NULL if not found.
 ----------------------------------------------------------------------------*/
static PLAYBACK_BUF_AGT_T*
playback_conn_get_buf_agt_by_handle (HANDLE_T  h_buffer_agent)
{
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
    HANDLE_TYPE_T        e_type;
    INT32                i4_res;

    i4_res = handle_get_type_obj(h_buffer_agent,
                                 & e_type, (VOID**) & pt_buf_agt);

    if ((i4_res != HR_OK) || (e_type != PHT_BUFFER_AGENT))
    {
        pt_buf_agt = NULL;
    }

    return pt_buf_agt;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_get_buf_agt_by_index
 *
 * Description: This API finds a buffer agent structure given an index.
 *
 * Inputs:  pt_conn    References the connection structure.
 *          ui4_index  Contains the buffer agent index.
 *
 * Outputs: -
 *
 * Returns: Pointer to buffer agent structure, or NULL if not found.
 ----------------------------------------------------------------------------*/
static PLAYBACK_BUF_AGT_T*
playback_conn_get_buf_agt_by_index (PLAYBACK_CONN_T*  pt_conn,
                                    UINT32            ui4_index)
{
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        if (ui4_index-- == 0)
        {
            break;
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }

    /* if there is only one buffer agent, return it anyway */
    if ((pt_buf_agt == NULL)
        &&
        (pt_conn->t_connect_info.pt_buf_agt_head != NULL)
        &&
        (pt_conn->t_connect_info.pt_buf_agt_head->pt_next == NULL))
    {
        pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;
    }

    return pt_buf_agt;
}

#if 0
/*-----------------------------------------------------------------------------
 * Name: playback_conn_get_buf_agt_index
 *
 * Description: This API finds a buffer agent index.
 *
 * Inputs:  pt_conn             References the connection structure.
 *          pt_buf_agt_target   target buffer agent
 *
 *
 * Outputs: pui4_index          Contains the buffer agent index.
 *
 * Returns: CMR_OK if can be found
 ----------------------------------------------------------------------------*/
static INT32
playback_conn_get_buf_agt_index (PLAYBACK_CONN_T*       pt_conn,
                                 PLAYBACK_BUF_AGT_T*    pt_buf_agt_target,
                                 UINT32*                pui4_index)
{
    UINT32                  ui4_index;
    PLAYBACK_BUF_AGT_T*     pt_buf_agt;

    if (NULL == pui4_index)
    {
        return CMR_INV_ARG;
    }

    ui4_index = 0;
    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        if (pt_buf_agt == pt_buf_agt_target)
        {
            break;
        }

        ui4_index++;
        pt_buf_agt = pt_buf_agt->pt_next;
    }

    /* if there is only one buffer agent, return it anyway */
    if (pt_buf_agt == NULL)
    {
        return CMR_FAIL;
    }

    *pui4_index = ui4_index;
    return CMR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: playback_conn_get_buf_agt_from_pat
 *
 * Description: This API finds a buffer agent structure given a PAT handle.
 *
 * Inputs:  pt_conn  References the connection structure.
 *          h_pat    Contains the PAT handle.
 *
 * Outputs: -
 *
 * Returns: Pointer to buffer agent structure, or NULL if not found.
 ----------------------------------------------------------------------------*/
static PLAYBACK_BUF_AGT_T*
playback_conn_get_buf_agt_from_pat (PLAYBACK_CONN_T*  pt_conn,
                                    HANDLE_T          h_pat)
{
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        if (pt_buf_agt->h_pat == h_pat)
        {
            break;
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }

    return pt_buf_agt;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_get_buf_agt_from_pmt
 *
 * Description: This API finds a buffer agent structure given a PMT handle.
 *
 * Inputs:  pt_conn  References the connection structure.
 *          h_pmt    Contains the PMT handle.
 *
 * Outputs: -
 *
 * Returns: Pointer to buffer agent structure, or NULL if not found.
 ----------------------------------------------------------------------------*/
static PLAYBACK_BUF_AGT_T*
playback_conn_get_buf_agt_from_pmt (PLAYBACK_CONN_T*  pt_conn,
                                    HANDLE_T          h_pmt)
{
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        if (pt_buf_agt->h_pmt == h_pmt)
        {
            break;
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }

    return pt_buf_agt;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_get_buf_agt_from_mgt
 *
 * Description: This API finds a buffer agent structure given a MGT handle.
 *
 * Inputs:  pt_conn  References the connection structure.
 *          h_mgt    Contains the MGT handle.
 *
 * Outputs: -
 *
 * Returns: Pointer to buffer agent structure, or NULL if not found.
 ----------------------------------------------------------------------------*/
static PLAYBACK_BUF_AGT_T*
playback_conn_get_buf_agt_from_mgt (PLAYBACK_CONN_T*  pt_conn,
                                    HANDLE_T          h_mgt)
{
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        if (pt_buf_agt->h_mgt == h_mgt)
        {
            break;
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }

    return pt_buf_agt;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_get_buf_agt_from_vct
 *
 * Description: This API finds a buffer agent structure given a VCT handle.
 *
 * Inputs:  pt_conn  References the connection structure.
 *          h_vct    Contains the VCT handle.
 *
 * Outputs: -
 *
 * Returns: Pointer to buffer agent structure, or NULL if not found.
 ----------------------------------------------------------------------------*/
static PLAYBACK_BUF_AGT_T*
playback_conn_get_buf_agt_from_vct (PLAYBACK_CONN_T*  pt_conn,
                                    HANDLE_T          h_vct)
{
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        if (pt_buf_agt->h_vct == h_vct)
        {
            break;
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }

    return pt_buf_agt;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_get_buf_agt_from_eit
 *
 * Description: This API finds a buffer agent structure given an EIT handle.
 *
 * Inputs:  pt_conn  References the connection structure.
 *          h_eit    Contains the EIT handle.
 *
 * Outputs: -
 *
 * Returns: Pointer to buffer agent structure, or NULL if not found.
 ----------------------------------------------------------------------------*/
static PLAYBACK_BUF_AGT_T*
playback_conn_get_buf_agt_from_eit (PLAYBACK_CONN_T*  pt_conn,
                                    HANDLE_T          h_eit)
{
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        if (pt_buf_agt->h_eit0 == h_eit)
        {
            break;
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }

    return pt_buf_agt;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_disconnected
 *
 * Description: This API executes the final steps of the disconnection, i.e.
 *              deleting the handle and notifying the client.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_disconnected (PLAYBACK_CONN_T*  pt_conn)
{
    HANDLE_T  h_cm_conn;
    UINT32    ui4_reason;

    h_cm_conn  = pt_conn->h_cm_conn;
    ui4_reason = pt_conn->ui4_disconnection_reason;

    DBG_INFO((DBG_PREFIX"Notify CM: disconnected (%d, reason=%d)\n",
              pt_conn->h_ch_conn, ui4_reason));

    handle_free(pt_conn->h_ch_conn, FALSE);

    cm_nfy(h_cm_conn, CM_COND_DISCONNECTED, ui4_reason);
}

/*-----------------------------------------------------------------------------
 * Name: playback_virtual_source_closed
 *
 * description: This API is called when the virtual source component is closed.
 *              The connection will then be disconnected completely.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_virtual_source_closed (PLAYBACK_CONN_T*  pt_conn)
{
    pt_conn->h_rm_virtual_source = NULL_HANDLE;

    playback_conn_disconnected(pt_conn);
}

/*-----------------------------------------------------------------------------
 * Name: playback_close_virtual_source
 *
 * description: This API closes the connection's virtual source driver
 *              component (if any).
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_close_virtual_source (PLAYBACK_CONN_T*  pt_conn)
{
    if (pt_conn->h_rm_virtual_source != NULL_HANDLE)
    {
        INT32  i4_res;

        DBG_INFO((DBG_PREFIX"Virtual source being closed (%d)\n",
                  pt_conn->h_ch_conn));

        i4_res = rm_close(pt_conn->h_rm_virtual_source, FALSE, NULL);

        if (i4_res == RMR_OK)
        {
            playback_virtual_source_closed(pt_conn);
        }
        else if (i4_res == RMR_ASYNC_NFY)
        {
            /* nothing to do here */
        }
        else
        {
            DBG_ERROR((DBG_PREFIX
                       "Virtual source could not be closed (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));
        }
    }
    else
    {
        playback_virtual_source_closed(pt_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_are_all_buf_agt_closed
 *
 * Description: This API checks whether all the buffer agents are closed.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: TRUE if all the buffer agents are closed, FALSE otherwise.
 ----------------------------------------------------------------------------*/
static BOOL
playback_conn_are_all_buf_agt_closed (const PLAYBACK_CONN_T*  pt_conn)
{
    const PLAYBACK_BUF_AGT_T*  pt_buf_agt;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        if (pt_buf_agt->h_rm_buffer_agent != NULL_HANDLE)
        {
            return FALSE;
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: playback_buffer_agent_closed
 *
 * Description: This API is called when a buffer agent component is closed.
 *              The vitual source is closed when all the buffer agents are
 *              closed.
 *
 * Inputs:  pt_conn     References the connection structure.
 *          pt_buf_agt  References the buffer agent structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_buffer_agent_closed (PLAYBACK_CONN_T*     pt_conn,
                                          PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    pt_buf_agt->h_rm_buffer_agent = NULL_HANDLE;

    if (playback_conn_are_all_buf_agt_closed(pt_conn))
    {
        playback_close_virtual_source(pt_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_close_buffer_agent
 *
 * Description: This API closes a buffer agent driver component.
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_buf_agt  References the buffer agent structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_close_buffer_agent (PLAYBACK_CONN_T*     pt_conn,
                                         PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    INT32  i4_res;

    DBG_INFO((DBG_PREFIX"Buffer agent being closed (%d)\n",
              pt_conn->h_ch_conn));

    i4_res = rm_close(pt_buf_agt->h_rm_buffer_agent, FALSE, NULL);

    if (i4_res == RMR_OK)
    {
        DBG_INFO((DBG_PREFIX"Buffer agent closed synchronously (%d)\n",
                  pt_conn->h_ch_conn));

        playback_buffer_agent_closed(pt_conn, pt_buf_agt);
    }
    else if (i4_res == RMR_ASYNC_NFY)
    {
        /* nothing to do here */
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Buffer agent could not be closed (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_res));
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_close_conn
 *
 * Description: This API closes the connection. This involves closing the
 *              buffer agent.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_close_conn (PLAYBACK_CONN_T*  pt_conn)
{
    if (! pt_conn->b_closing)
    {
        PLAYBACK_BUF_AGT_T*  pt_buf_agt;
        BOOL                 b_conn_close_source;

        pt_conn->b_closing  = TRUE;
        b_conn_close_source = TRUE;

        pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

        playback_conn_close_svl(pt_conn);

        while (pt_buf_agt != NULL)
        {
            PLAYBACK_BUF_AGT_T*  pt_next;

            pt_next = pt_buf_agt->pt_next;

            playback_conn_delete_clock(pt_buf_agt);
            playback_conn_delete_tables(pt_conn, pt_buf_agt);

            if (pt_buf_agt->h_rm_buffer_agent != NULL_HANDLE)
            {
                playback_close_buffer_agent(pt_conn, pt_buf_agt);

                b_conn_close_source = FALSE;
            }

            pt_buf_agt = pt_next;
        }

        if (b_conn_close_source)
        {
            playback_close_virtual_source(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_are_all_buf_agt_disconnected
 *
 * Description: This API checks whether all the buffer agents are disconnected.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: TRUE if all the buffer agents are disconnected, FALSE otherwise.
 ----------------------------------------------------------------------------*/
static BOOL
playback_conn_are_all_buf_agt_disconnected (const PLAYBACK_CONN_T*  pt_conn)
{
    const PLAYBACK_BUF_AGT_T*  pt_buf_agt;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        if (! pt_buf_agt->b_buffer_agent_disconnected)
        {
            return FALSE;
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: playback_buffer_agent_disconnected
 *
 * Description: This API is called when the buffer agent component is
 *              disconnected.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_buffer_agent_disconnected (PLAYBACK_CONN_T*     pt_conn,
                                    PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    if (! pt_buf_agt->b_buffer_agent_disconnected)
    {
        pt_buf_agt->b_buffer_agent_disconnected = TRUE;

        pt_conn->b_disconnecting = TRUE;

        if (playback_conn_are_all_buf_agt_disconnected(pt_conn))
        {
            playback_close_conn(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_disconnect_buffer_agent
 *
 * Description: This API disconnects the buffer agent component.
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_buf_agt  References the buffer agent structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_disconnect_buffer_agent (PLAYBACK_CONN_T*     pt_conn,
                                              PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    INT32  i4_res;

    DBG_INFO((DBG_PREFIX"Buffer agent being disconnected (%d)\n",
              pt_conn->h_ch_conn));

    i4_res = rm_disconnect(pt_buf_agt->h_rm_buffer_agent,
                           RM_DISC_TYPE_IGNORE,
                           NULL, /* disconnection info */
                           0, /* disconnection info size */
                           & pt_buf_agt->e_rm_disconnect_buf_agt_cond);

    if (i4_res == RMR_OK)
    {
        DBG_INFO((DBG_PREFIX"Buffer agent disconnected synchronously (%d)\n",
                  pt_conn->h_ch_conn));

        playback_buffer_agent_disconnected(pt_conn, pt_buf_agt);
    }
    else if (i4_res == RMR_ASYNC_NFY)
    {
        /* nothing to do here */
    }
    else
    {
        DBG_ERROR((DBG_PREFIX
                   "Could not disconnect buffer agent (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_res));
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_disconnect_conn_2nd
 *
 * Description: This API initiates the 2nd phase disconnection of the connection.
 *
 * Inputs:  pt_conn     References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_disconnect_conn_2nd (PLAYBACK_CONN_T*  pt_conn)
{
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
    BOOL                 b_close_conn;

    DBG_INFO((DBG_PREFIX"Disconnecting connection 2nd %d\n", pt_conn->h_ch_conn));

    b_close_conn                      = TRUE;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        PLAYBACK_BUF_AGT_T*  pt_next;

        pt_next = pt_buf_agt->pt_next;

        if (pt_buf_agt->e_rm_connect_buf_agt_cond != RM_COND_IGNORE)
        {
            playback_disconnect_buffer_agent(pt_conn, pt_buf_agt);

            b_close_conn = FALSE;
        }

        pt_buf_agt = pt_next;
    }

    if (b_close_conn)
    {
        playback_close_conn(pt_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_disconnect_conn
 *
 * Description: This API initiates the disconnection of the connection.
 *
 * Inputs:  pt_conn     References the connection.
 *          ui4_reason  Contains the disconnection reason.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_disconnect_conn (PLAYBACK_CONN_T*  pt_conn,
                                      UINT32            ui4_reason)
{
    if (! pt_conn->b_disconnecting)
    {
        PLAYBACK_BUF_AGT_T*  pt_buf_agt;
        BOOL b_circular_running;

        DBG_INFO((DBG_PREFIX"Disconnecting connection (%d, reason=%d)\n",
                  pt_conn->h_ch_conn, ui4_reason));

        pt_conn->ui4_disconnection_reason = ui4_reason;
        pt_conn->b_disconnecting          = TRUE;

        pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

        b_circular_running = FALSE;

        while (pt_buf_agt != NULL)
        {
            PLAYBACK_BUF_AGT_T*  pt_next;

            pt_next = pt_buf_agt->pt_next;

            if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT)
            {
                if (pt_buf_agt->b_issue_circular_req)
                {
                    if(pt_buf_agt->pt_transferring_item->u.t_uri.b_url)
                    {
#ifdef INET_SUPPORT
                        x_httpc_fm_abort_async(pt_buf_agt->pt_transferring_item->u.t_uri.h_uri, pt_buf_agt->t_circular_req.h_async_read);
#endif
                    }
                    else
                    {
                        x_fm_mfw_abort_async(pt_buf_agt->t_circular_req.h_async_read);
                    }
                    b_circular_running = TRUE;
                }
            }
            pt_buf_agt = pt_next;
        }

        if (!b_circular_running)
        {
            playback_disconnect_conn_2nd(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_delete_tables
 *
 * Description: This API frees all the table handles (PAT, PMT, etc).
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_delete_tables (PLAYBACK_CONN_T*     pt_conn,
                                         PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    INT32  i4_res;

    /* delete EIT */
    if (pt_buf_agt->h_eit0 != NULL_HANDLE)
    {
        DBG_INFO((DBG_PREFIX"Deleting EIT %d (%d)\n",
                  pt_buf_agt->h_eit0, pt_conn->h_ch_conn));

        i4_res = x_tm_free(pt_buf_agt->h_eit0);

        if (i4_res != TMR_OK)
        {
            DBG_ERROR((DBG_PREFIX"Could not delete EIT (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));
        }

        pt_buf_agt->h_eit0          = NULL_HANDLE;
        pt_buf_agt->b_eit0_acquired = FALSE;
    }

    /* delete VCT */
    if (pt_buf_agt->h_vct != NULL_HANDLE)
    {
        DBG_INFO((DBG_PREFIX"Deleting VCT %d (%d)\n",
                  pt_buf_agt->h_vct, pt_conn->h_ch_conn));

        i4_res = x_tm_free(pt_buf_agt->h_vct);

        if (i4_res != TMR_OK)
        {
            DBG_ERROR((DBG_PREFIX"Could not delete VCT (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));
        }

        pt_buf_agt->h_vct          = NULL_HANDLE;
        pt_buf_agt->b_vct_acquired = FALSE;
    }

    /* delete MGT */
    if (pt_buf_agt->h_mgt != NULL_HANDLE)
    {
        DBG_INFO((DBG_PREFIX"Deleting MGT %d (%d)\n",
                  pt_buf_agt->h_mgt, pt_conn->h_ch_conn));

        i4_res = x_tm_free(pt_buf_agt->h_mgt);

        if (i4_res != TMR_OK)
        {
            DBG_ERROR((DBG_PREFIX"Could not delete MGT (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));
        }

        pt_buf_agt->h_mgt          = NULL_HANDLE;
        pt_buf_agt->b_mgt_acquired = FALSE;
    }

    /* delete PMT */
    if (pt_buf_agt->h_pmt != NULL_HANDLE)
    {
        DBG_INFO((DBG_PREFIX"Deleting PMT %d (%d)\n",
                  pt_buf_agt->h_pmt, pt_conn->h_ch_conn));

        i4_res = x_tm_free(pt_buf_agt->h_pmt);

        if (i4_res != TMR_OK)
            {
                DBG_ERROR((DBG_PREFIX"Could not delete PMT (%d, res=%d)\n",
                           pt_conn->h_ch_conn, i4_res));
            }

        pt_buf_agt->h_pmt          = NULL_HANDLE;
        pt_buf_agt->b_pmt_acquired = FALSE;
    }

    /* delete PAT */
    if (pt_buf_agt->h_pat != NULL_HANDLE)
    {
        DBG_INFO((DBG_PREFIX"Deleting PAT %d (%d)\n",
                  pt_buf_agt->h_pat, pt_conn->h_ch_conn));

        i4_res = x_tm_free(pt_buf_agt->h_pat);

        if (i4_res != TMR_OK)
        {
            DBG_ERROR((DBG_PREFIX"Could not delete PAT (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));
        }

        pt_buf_agt->h_pat          = NULL_HANDLE;
        pt_buf_agt->b_pat_acquired = FALSE;
    }

    /* delete root */
    if (pt_buf_agt->h_tm_root != NULL_HANDLE)
    {
        DBG_INFO((DBG_PREFIX"Deleting table root %d (%d)\n",
                  pt_buf_agt->h_tm_root, pt_conn->h_ch_conn));

        i4_res = x_tm_free(pt_buf_agt->h_tm_root);

        if (i4_res != TMR_OK)
        {
            DBG_ERROR((DBG_PREFIX"Could not delete root (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));
        }

        pt_buf_agt->h_tm_root = NULL_HANDLE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_get_stream_tag
 *
 * Description: This API returns a stream tag (i.e. a VOID* pointer).
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: a non-NULL pointer that is used as a stream tag.
 ----------------------------------------------------------------------------*/
static VOID* playback_get_stream_tag(VOID)
{
    static VOID*  pv_stream_tag = NULL;
    CRIT_STATE_T  t_state;
    VOID*         pv_res;

    t_state = x_crit_start();

    pv_stream_tag = (VOID*) (((CHAR*)pv_stream_tag) + 1);

    if (pv_stream_tag == NULL)
    {
        pv_stream_tag = (VOID*) (((CHAR*)pv_stream_tag) + 1);
    }

    pv_res = pv_stream_tag;

    x_crit_end(t_state);

    return pv_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_scdb_inter_pmt
 *
 * Description: This API computes the intersection of the streams in a PMT and
 *              the streams in a SCDB. The SCDB is modified accordingly.
 *              SCDB <-- SCDB inter PMT.
 *
 * Inputs:  h_pmt   Contains the PMT handle.
 *          h_scdb  Contains the SCDB handle.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_scdb_inter_pmt (PLAYBACK_CONN_T*     pt_conn,
                                          PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                          UINT32               ui4_flags)
{
    STREAM_COMP_ID_T  t_stream_comp_id;
    SCDB_REC_T        t_scdb_rec;
    UINT32            ui4_ver_id;
    UINT16            ui2_idx;
    SCDB_COMMON_T     t_scdb_common;

    ui4_ver_id = SCDB_NULL_VER_ID;
    ui2_idx    = 0;

    /* PCR PID is only updated from the first buffer agent for now */

    if ((pt_buf_agt == pt_conn->t_connect_info.pt_buf_agt_head)
        &&
        (x_scdb_get_common(pt_conn->h_scdb, & t_scdb_common) == SCDBR_OK))
    {
        MPEG_2_PID_T  t_pcr_pid;

        if (x_psi_get_pcr_pid (pt_buf_agt->h_pmt,
                               & t_pcr_pid,
                               NULL) == TMR_OK)
        {
            if (t_pcr_pid != t_scdb_common.u_data.t_mpeg.ui2_pcr)
            {
                t_scdb_common.u_data.t_mpeg.ui2_pcr = t_pcr_pid;

                scdb_set_common(pt_conn->h_scdb, & t_scdb_common);
            }
        }
    }

    while (x_scdb_get_rec_by_idx(pt_conn->h_scdb,
                                 ST_UNKNOWN,
                                 ui2_idx,
                                 & t_stream_comp_id,
                                 & t_scdb_rec,
                                 & ui4_ver_id)
           == SCDBR_OK)
    {
        UINT32  ui4_buf_agt_index;

        ui4_ver_id = SCDB_NULL_VER_ID;

        ui4_buf_agt_index = CM_GET_SRC_INDEX(t_scdb_rec.ui4_private);

        if ((t_stream_comp_id.e_type != ST_CLOSED_CAPTION)
            &&
            (ui4_buf_agt_index == pt_buf_agt->ui4_index)
            &&
            ! cm_pmt_has_stream(pt_buf_agt->h_pmt,
                                & t_stream_comp_id,
                                & t_scdb_rec,
                                ui4_flags))
        {
            INT32  i4_res;

            i4_res = scdb_del_rec(pt_conn->h_scdb, & t_stream_comp_id);

            if (i4_res != SCDBR_OK)
            {
                ABORT(DBG_CAT_NO_RECOVERY, DBG_ABRT_CANNOT_DELETE_SCDB_REC);
            }

            ui2_idx = 0;
        }
        else
        {
            /* the new PMT still contains the stream - keep it */

            ui2_idx++;
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_update_scdb_video_stream
 *
 * Description: This API updates (or adds) a video stream in the SCDB. The
 *              video stream will have the PID specified as parameter.
 *
 * Inputs:  pt_conn          References the connection.
 *          t_pid            Contains the PID of the video stream.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The SCDB has been updated successfully.
 *          CMR_FAIL  The SCDB could not be updated.
 ----------------------------------------------------------------------------*/
static INT32
playback_conn_update_scdb_video_stream (PLAYBACK_CONN_T*     pt_conn,
                                        PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                        MPEG_2_PID_T         t_pid,
                                        UINT16               ui2_pmt_index,
                                        UINT8                ui1_stream_type)
{
    STREAM_COMP_ID_T  t_stream_comp_id;
    SCDB_REC_T        t_scdb_rec;
    STREAM_COMP_ID_T  t_stream_comp_id_bk;
    SCDB_REC_T        t_scdb_rec_bk;
    INT32             i4_res;
    UINT32            ui4_ver_id;
    UINT16            ui2_index;
    BOOL              b_found;

    x_memset((VOID*)&t_scdb_rec,    0, sizeof(SCDB_REC_T));
    x_memset((VOID*)&t_scdb_rec_bk, 0, sizeof(SCDB_REC_T));
    ui4_ver_id = SCDB_NULL_VER_ID;
    ui2_index  = 0;
    b_found    = FALSE;

    while ((! b_found)
           &&
           (x_scdb_get_rec_by_idx(pt_conn->h_scdb,
                                  ST_VIDEO,
                                  ui2_index,
                                  & t_stream_comp_id,
                                  & t_scdb_rec,
                                  & ui4_ver_id) == SCDBR_OK))
    {
        UINT32  ui4_buf_agt_index;

        ui4_buf_agt_index = CM_GET_SRC_INDEX(t_scdb_rec.ui4_private);

        b_found =
            (t_scdb_rec.u.t_video_mpeg.ui2_pid == t_pid)
            &&
            (ui4_buf_agt_index == pt_buf_agt->ui4_index);

        if(ui2_index == 0)
        {
            t_stream_comp_id_bk = t_stream_comp_id;
            t_scdb_rec_bk       = t_scdb_rec;
        }

        ui2_index++;
    }

    /* new tag value for new stream, no change if stream already exists */
    if (! b_found)
    {
        /* reset this record  */
        x_memset((VOID*)&t_scdb_rec, 0, sizeof(SCDB_REC_T));

        t_stream_comp_id.e_type        = ST_VIDEO;
        t_stream_comp_id.pv_stream_tag = playback_get_stream_tag();

        t_scdb_rec.e_rec_type               = SCDB_REC_TYPE_VIDEO_MPEG;
        t_scdb_rec.u.t_video_mpeg.ui2_pid   = t_pid;
        t_scdb_rec.u.t_video_mpeg.e_vid_fmt = VID_FMT_UNKNOWN;
        t_scdb_rec.u.t_video_mpeg.b_default = TRUE;

        t_scdb_rec.ui4_private = CM_SET_SRC_INDEX(0, pt_buf_agt->ui4_index);
    }

    cm_get_mvc_ext_info(pt_buf_agt->h_pmt, ui2_pmt_index, &t_scdb_rec.u.t_video_mpeg.t_mvc_ext);
    cm_get_rd3d_reg_info(pt_buf_agt->h_pmt, ui2_pmt_index, &t_scdb_rec.u.t_video_mpeg.t_rd3d);

    switch (ui1_stream_type)
    {
        case STREAM_TYPE_VIDEO_11172_2:
            t_scdb_rec.u.t_video_mpeg.e_enc = VID_ENC_MPEG_1;
            break;

        case STREAM_TYPE_VIDEO_13818_2:
        case STREAM_TYPE_VIDEO_DIGICIPHER_2:
            t_scdb_rec.u.t_video_mpeg.e_enc = VID_ENC_MPEG_2;
            break;

        case STREAM_TYPE_VIDEO_14496_2:
            t_scdb_rec.u.t_video_mpeg.e_enc = VID_ENC_MPEG_4;
            break;

        case STREAM_TYPE_VIDEO_14496_10:
            t_scdb_rec.u.t_video_mpeg.e_enc = VID_ENC_H264;
            break;

        case STREAM_TYPE_VIDEO_MVC:
            t_scdb_rec.u.t_video_mpeg.e_enc = VID_ENC_H264;
            t_scdb_rec.u.t_video_mpeg.t_mvc_ext.b_mvc_video = TRUE;
            t_scdb_rec.u.t_video_mpeg.t_mvc_ext.e_mvc_src   = MVC_SRC_TYPE_STRM_MVC;
            pt_buf_agt->b_vdec_mvc          = TRUE;
            break;

        case STREAM_TYPE_VIDEO_SMPTE_421M:
            t_scdb_rec.u.t_video_mpeg.e_enc = VID_ENC_WVC1;
            break;

        case STREAM_TYPE_VIDEO_AVS:
            t_scdb_rec.u.t_video_mpeg.e_enc = VID_ENC_AVS;
            break;

        default:
            t_scdb_rec.u.t_video_mpeg.e_enc = VID_ENC_UNKNOWN;
            break;
    }

    if((t_scdb_rec_bk.u.t_video_mpeg.t_mvc_ext.b_mvc_video && 
       (t_scdb_rec_bk.u.t_video_mpeg.t_mvc_ext.e_mvc_src == MVC_SRC_TYPE_STRM_TAG)) && 
       (t_scdb_rec.u.t_video_mpeg.t_mvc_ext.b_mvc_video && 
       (t_scdb_rec.u.t_video_mpeg.t_mvc_ext.e_mvc_src == MVC_SRC_TYPE_STRM_MVC)))
    {
        t_scdb_rec_bk.u.t_video_mpeg.t_mvc_ext.b_mvc_video = FALSE;
        t_scdb_rec_bk.u.t_video_mpeg.t_mvc_ext.b_mvc_ext_desc = FALSE;
        t_scdb_rec_bk.u.t_video_mpeg.t_mvc_ext.e_mvc_src   = MVC_SRC_TYPE_UNKNOWN;
        i4_res = scdb_update_rec(pt_conn->h_scdb,
                                 & t_stream_comp_id_bk,
                                 & t_scdb_rec_bk);
    }
    else if((t_scdb_rec.u.t_video_mpeg.t_mvc_ext.b_mvc_video && 
            (t_scdb_rec.u.t_video_mpeg.t_mvc_ext.e_mvc_src == MVC_SRC_TYPE_STRM_TAG)) && 
            (t_scdb_rec_bk.u.t_video_mpeg.t_mvc_ext.b_mvc_video && 
            (t_scdb_rec_bk.u.t_video_mpeg.t_mvc_ext.e_mvc_src == MVC_SRC_TYPE_STRM_MVC)))
    {
        x_dbg_stmt("====  first mvc from stream, second is mvc tag ======\r\n");
        t_scdb_rec.u.t_video_mpeg.t_mvc_ext.b_mvc_video = FALSE;
        t_scdb_rec.u.t_video_mpeg.t_mvc_ext.b_mvc_ext_desc = FALSE;
        t_scdb_rec.u.t_video_mpeg.t_mvc_ext.e_mvc_src   = MVC_SRC_TYPE_UNKNOWN;
    }
    i4_res = scdb_update_rec(pt_conn->h_scdb,
                             & t_stream_comp_id,
                             & t_scdb_rec);

    i4_res = (i4_res == SCDBR_OK) ? CMR_OK : CMR_FAIL;

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_update_scdb_from_info
 *
 * Description: This API updates SCDB streams given a PMT loop index, a PID,
 *              a PMT stream type and a language.
 *
 * Inputs:  pt_conn          References the connection.
 *          ui2_index        Contains the PMT loop index (valid only if b_pmt
 *                           is set to TRUE).
 *          t_pid            Contains the PID of the stream.
 *          ui1_stream_type  Contains the stream type.
 *          ps_language      Contains the language (may be NULL).
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The SCDB has been updated successfully.
 *          CMR_FAIL  The SCDB could not be updated.
 ----------------------------------------------------------------------------*/
static INT32
playback_conn_update_scdb_from_info (PLAYBACK_CONN_T*     pt_conn,
                                     PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                     UINT16               ui2_index,
                                     MPEG_2_PID_T         t_pid,
                                     UINT8                ui1_stream_type,
                                     const CHAR*          ps_language,
                                     UINT32               ui4_flags)
{
    INT32  i4_res = CMR_OK;
    
    x_dbg_stmt("===== %s  ui1_stream_type %d ====\r\n", __func__, ui1_stream_type);
    switch (ui1_stream_type)
    {
        case STREAM_TYPE_VIDEO_11172_2:
        case STREAM_TYPE_VIDEO_13818_2:
        case STREAM_TYPE_VIDEO_14496_2:
        case STREAM_TYPE_VIDEO_14496_10:
        case STREAM_TYPE_VIDEO_MVC:
        case STREAM_TYPE_VIDEO_DIGICIPHER_2:
        case STREAM_TYPE_VIDEO_SMPTE_421M:
        case STREAM_TYPE_VIDEO_AVS:
        {
            if ((ui1_stream_type == STREAM_TYPE_VIDEO_DIGICIPHER_2)
                &&
                ! CM_IS_ATSC_SCTE_UNKNOWN(ui4_flags))
            {
                /* STREAM_TYPE_VIDEO_DIGICIPHER_2 is only for ATSC/SCTE */

                break;
            }

            i4_res = playback_conn_update_scdb_video_stream(pt_conn,
                                                            pt_buf_agt,
                                                            t_pid,
                                                            ui2_index,
                                                            ui1_stream_type);

            if (CM_IS_DVB(ui4_flags))
            {
                DBG_INFO((DBG_PREFIX
                          "Updating Teletext streams in SCDB using PMT (%d)\n",
                          pt_buf_agt->h_pmt));

                cm_update_scdb_teletext_streams_from_pmt_entry(
                    pt_conn->h_scdb,
                    pt_buf_agt->h_pmt,
                    ui2_index,
                    t_pid,
                    TRUE,    /*b_vbi,*/
                    playback_get_stream_tag);
            }
        }
        break;

        case STREAM_TYPE_AUDIO_11172_3:
        case STREAM_TYPE_AUDIO_13818_3:
        case STREAM_TYPE_AUDIO_14496_3:
        case STREAM_TYPE_AUDIO_13818_7:
        case STREAM_TYPE_AUDIO_ATSC_A53:
        case STREAM_TYPE_AUDIO_AVS:
        case STREAM_TYPE_AUDIO_MSRT24:
        case STREAM_TYPE_AUDIO_ATT_LABS_G729A:
        case STREAM_TYPE_AUDIO_DTS_EXPT_XLL:
        case STREAM_TYPE_AUDIO_DTS_GSM610:
        case STREAM_TYPE_AUDIO_EAC3:
        {
            if (ui1_stream_type == STREAM_TYPE_AUDIO_ATSC_A53)
            {
                if (CM_IS_DVB(ui4_flags))
                {
                    cm_update_from_descriptors(pt_conn->h_scdb,
                                               pt_buf_agt->h_pmt,
                                               ui2_index,
                                               pt_buf_agt->ui4_index,
                                               t_pid,
                                               ui1_stream_type,
                                               ui4_flags,
                                               playback_get_stream_tag);

                    break;
                }
                else if (! CM_IS_ATSC_SCTE_UNKNOWN(ui4_flags))
                {
                    /* STREAM_TYPE_AUDIO_ATSC_A53 is only for ATSC/SCTE */

                    break;
                }
            }

            i4_res = cm_update_scdb_audio(pt_conn->h_scdb,
                                          pt_buf_agt->h_pmt,
                                          ui2_index,
                                          pt_buf_agt->ui4_index,
                                          t_pid,
                                          ui1_stream_type,
                                          ps_language,
                                          ui4_flags,
                                          playback_get_stream_tag);
        }
        break;

        case STREAM_TYPE_PES_PRIVATE_13818_1:
        {
            if (CM_IS_DVB(ui4_flags))
            {
                BOOL  b_2_languages_1_stream;
                BOOL  b_dual_mono_fake_stereo;

                b_2_languages_1_stream =
                    pt_conn->t_connect_info.b_2_languages_1_stream;

                b_dual_mono_fake_stereo =
                    pt_conn->t_connect_info.b_dual_mono_fake_stereo;

                cm_update_pmt_pes_private_stream(pt_conn->h_scdb,
                                                 pt_buf_agt->h_pmt,
                                                 ui2_index,
                                                 t_pid,
                                                 FALSE,
                                                 b_2_languages_1_stream,
                                                 b_dual_mono_fake_stereo,
                                                 playback_get_stream_tag);
            }
        }
        break;

        default:
        {
            if (CM_IS_DVB(ui4_flags))
            {
                BOOL  b_2_languages_1_stream;
                BOOL  b_dual_mono_fake_stereo;

                b_2_languages_1_stream =
                    pt_conn->t_connect_info.b_2_languages_1_stream;

                b_dual_mono_fake_stereo =
                    pt_conn->t_connect_info.b_dual_mono_fake_stereo;

                cm_update_pmt_audio_stream(pt_conn->h_scdb,
                                           pt_buf_agt->h_pmt,
                                           ui2_index,
                                           t_pid,
                                           b_2_languages_1_stream,
                                           b_dual_mono_fake_stereo,
                                           playback_get_stream_tag);
            }
        }
        break;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_scdb_union_pmt
 *
 * Description: This API computes the union of the streams in a PMT and
 *              the streams in a SCDB. Streams that are already in the SCDB are
 *              not added again. The SCDB is modified accordingly.
 *              SCDB <-- SCDB + PMT.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_scdb_union_pmt (PLAYBACK_CONN_T*     pt_conn,
                                          PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                          UINT32               ui4_flags)
{
    PSI_STREAM_INFO_T  t_stream_info;
    UINT16             ui2_index;

    ui2_index = 0;

    while (x_psi_get_stream_entry(pt_buf_agt->h_pmt,
                                  ui2_index,
                                  & t_stream_info,
                                  NULL)
           == TMR_OK)
    {
        UINT8  ui1_stream_type = t_stream_info.ui1_stream_type;

        /*
          This will add new streams if they do not already exist in the SCDB.
          Note that a single PMT entry may contain multiple 'streams' (e.g.
          video + teletext).
        */

        playback_conn_update_scdb_from_info(pt_conn,
                                            pt_buf_agt,
                                            ui2_index,
                                            t_stream_info.t_pid,
                                            ui1_stream_type,
                                            NULL,
                                            ui4_flags);

        ui2_index++;
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_update_scdb_from_pmt
 *
 * Description: This API updates the content of an existing SCDB using the PMT.
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_buf_agt  References the buffer agent structure.
 *          ui4_flags   Contains the flags.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_conn_update_scdb_from_pmt (PLAYBACK_CONN_T*     pt_conn,
                                    PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                    UINT32               ui4_flags)
{
    INT32  i4_res;
    PLAYBACK_ITEM_T*  pt_item;

    DBG_INFO((DBG_PREFIX"Updating SCDB using PMT (%d)\n", pt_conn->h_ch_conn));

    i4_res = scdb_lock(pt_conn->h_scdb);
    pt_item = pt_buf_agt->pt_transferring_item;

    if (i4_res == SCDBR_OK)
    {
        if (!((pt_item->t_extra.t_mpeg2_info.ui4_flags & CM_FLAG_APPEND_SCDB_SUPPORT)
            &&  pt_buf_agt->b_vdec_mvc))
        {
            playback_conn_scdb_inter_pmt(pt_conn, pt_buf_agt, ui4_flags);
        }

        playback_conn_scdb_union_pmt(pt_conn, pt_buf_agt, ui4_flags);

        if (scdb_unlock(pt_conn->h_scdb) != SCDBR_OK)
        {
            ABORT(DBG_CAT_NO_RECOVERY,
                  DBG_ABRT_CANNOT_UNLOCK_SCDB_AFTER_UPDATE);
        }
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Could not lock SCDB (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_res));

        ABORT(DBG_CAT_NO_RECOVERY, DBG_ABRT_CANNOT_LOCK_SCDB);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_load_nfy
 *
 * Description: This API is called when a table object is acquired, updated or
 *              unavailable. This is an API called by playback_load_*_nfy (for
 *              instance playback_load_pat_nfy).
 *
 * Inputs:  h_obj               Contains a handle to the table object.
 *          e_obj_type          Contains the table object type.
 *          e_nfy_cond          Contains the notification reason.
 *          pv_nfy_tag          Contains the tag value specified when loading
 *                              the table.
 *          e_unavailable_code  Contains the code of the message to send in
 *                              case the table object becomes unavailable.
 *          e_update_code       Contains the code of the message to send in
 *                              case the table object is updated.
 *
 * Outputs: -
 *
 * Returns: NFY_RET_PROCESSED  The notification has been processed.
 *          NFY_RET_RETRY      The notify function could not process the
 *                             notification and the Table Manager should retry
 *                             again later.
 ----------------------------------------------------------------------------*/
NFY_RET_T playback_load_nfy (HANDLE_T       h_obj,
                             HANDLE_TYPE_T  e_obj_type,
                             TM_COND_T      e_nfy_cond,
                             VOID*          pv_nfy_tag,
                             UINT32         e_unavailable_code,
                             UINT32         e_update_code)
{
    NFY_RET_T  e_res;
    UINT32     e_code;
    HANDLE_T   h_ch_conn;

    e_res = NFY_RET_PROCESSED;

    h_ch_conn = (HANDLE_T) pv_nfy_tag;

    switch (e_nfy_cond)
    {
        case TM_COND_UNAVAILABLE:
        {
            DBG_INFO((DBG_PREFIX"Table object %d (type=%d) unavailable (%d)\n",
                      h_obj, e_obj_type, h_ch_conn));

            e_code = e_unavailable_code;
        }
        break;

        case TM_COND_AVAILABLE:
        case TM_COND_UPDATE:
        {
            DBG_INFO((DBG_PREFIX"Table object %d (type=%d) updated (%d)\n",
                      h_obj, e_obj_type, h_ch_conn));

            e_code = e_update_code;
        }
        break;

        default:
        {
            e_code = PLAYBACK_HANDLER_MSG_NONE;
        }
        break;
    }

    if (e_code != PLAYBACK_HANDLER_MSG_NONE)
    {
        cm_send_msg_no_retry(h_playback_handler_msgq, e_code, h_obj, 0,
                             h_ch_conn);
    }

    return e_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_load_eit0_nfy
 *
 * Description: This API is called when the EIT is acquired, updated or
 *              unavailable.
 *
 * Inputs:  h_eit0      Contains a handle to the EIT.
 *          e_obj_type  Contains the object type (should be TMT_ATSC_EIT).
 *          e_nfy_cond  Contains the notification reason.
 *          pv_nfy_tag  Contains the tag value specified then loading the EIT.
 *          ui4_data    Contains additional data.
 *
 * Outputs: -
 *
 * Returns: NFY_RET_PROCESSED  The notification has been processed.
 *          NFY_RET_RETRY      The notify function could not process the
 *                             notification and the Table Manager should retry
 *                             again later.
 ----------------------------------------------------------------------------*/
static NFY_RET_T playback_load_eit0_nfy (HANDLE_T       h_eit0,
                                         HANDLE_TYPE_T  e_obj_type,
                                         TM_COND_T      e_nfy_cond,
                                         VOID*          pv_nfy_tag,
                                         UINT32         ui4_data)
{
    return playback_load_nfy(h_eit0,
                             e_obj_type,
                             e_nfy_cond,
                             pv_nfy_tag,
                             PLAYBACK_HANDLER_MSG_EIT_UNAVAILABLE,
                             PLAYBACK_HANDLER_MSG_EIT_UPDATED);
}

/*-----------------------------------------------------------------------------
 * Name: playback_load_vct_nfy
 *
 * Description: This API is called when the VCT is acquired, updated or
 *              unavailable.
 *
 * Inputs:  h_vct       Contains a handle to the VCT.
 *          e_obj_type  Contains the object type (should be TMT_ATSC_VCT).
 *          e_nfy_cond  Contains the notification reason.
 *          pv_nfy_tag  Contains the tag value specified then loading the VCT.
 *          ui4_data    Contains additional data.
 *
 * Outputs: -
 *
 * Returns: NFY_RET_PROCESSED  The notification has been processed.
 *          NFY_RET_RETRY      The notify function could not process the
 *                             notification and the Table Manager should retry
 *                             again later.
 ----------------------------------------------------------------------------*/
static NFY_RET_T playback_load_vct_nfy (HANDLE_T       h_vct,
                                        HANDLE_TYPE_T  e_obj_type,
                                        TM_COND_T      e_nfy_cond,
                                        VOID*          pv_nfy_tag,
                                        UINT32         ui4_data)
{
    return playback_load_nfy(h_vct,
                             e_obj_type,
                             e_nfy_cond,
                             pv_nfy_tag,
                             PLAYBACK_HANDLER_MSG_VCT_UNAVAILABLE,
                             PLAYBACK_HANDLER_MSG_VCT_UPDATED);
}

/*-----------------------------------------------------------------------------
 * Name: playback_load_mgt_nfy
 *
 * Description: This API is called when the MGT is acquired, updated or
 *              unavailable.
 *
 * Inputs:  h_mgt       Contains a handle to the MGT.
 *          e_obj_type  Contains the object type (should be TMT_ATSC_MGT).
 *          e_nfy_cond  Contains the notification reason.
 *          pv_nfy_tag  Contains the tag value specified then loading the MGT.
 *          ui4_data    Contains additional data.
 *
 * Outputs: -
 *
 * Returns: NFY_RET_PROCESSED  The notification has been processed.
 *          NFY_RET_RETRY      The notify function could not process the
 *                             notification and the Table Manager should retry
 *                             again later.
 ----------------------------------------------------------------------------*/
static NFY_RET_T playback_load_mgt_nfy (HANDLE_T       h_mgt,
                                        HANDLE_TYPE_T  e_obj_type,
                                        TM_COND_T      e_nfy_cond,
                                        VOID*          pv_nfy_tag,
                                        UINT32         ui4_data)
{
    return playback_load_nfy(h_mgt,
                             e_obj_type,
                             e_nfy_cond,
                             pv_nfy_tag,
                             PLAYBACK_HANDLER_MSG_NONE,
                             PLAYBACK_HANDLER_MSG_MGT_UPDATED);
}

/*-----------------------------------------------------------------------------
 * Name: playback_load_pmt_nfy
 *
 * Description: This API is called when the PMT is acquired, updated or
 *              unavailable.
 *
 * Inputs:  h_pmt       Contains a handle to the PMT.
 *          e_obj_type  Contains the object type (should be TMT_PSI_PMT).
 *          e_nfy_cond  Contains the notification reason.
 *          pv_nfy_tag  Contains the tag value specified then loading the PMT.
 *          ui4_data    Contains additional data.
 *
 * Outputs: -
 *
 * Returns: NFY_RET_PROCESSED  The notification has been processed.
 *          NFY_RET_RETRY      The notify function could not process the
 *                             notification and the Table Manager should retry
 *                             again later.
 ----------------------------------------------------------------------------*/
static NFY_RET_T playback_load_pmt_nfy (HANDLE_T       h_pmt,
                                        HANDLE_TYPE_T  e_obj_type,
                                        TM_COND_T      e_nfy_cond,
                                        VOID*          pv_nfy_tag,
                                        UINT32         ui4_data)
{
    x_dbg_stmt("===== %s ====\r\n", __func__);
    return playback_load_nfy(h_pmt,
                             e_obj_type,
                             e_nfy_cond,
                             pv_nfy_tag,
                             PLAYBACK_HANDLER_MSG_PMT_UNAVAILABLE,
                             PLAYBACK_HANDLER_MSG_PMT_UPDATED);
}

/*-----------------------------------------------------------------------------
 * Name: playback_load_pat_nfy
 *
 * Description: This API is called when the PAT is acquired, updated or
 *              unavailable.
 *
 * Inputs:  h_pat       Contains a handle to the PAT.
 *          e_obj_type  Contains the object type (should be TMT_PSI_PAT).
 *          e_nfy_cond  Contains the notification reason.
 *          pv_nfy_tag  Contains the tag value specified then loading the PAT.
 *          ui4_data    Contains additional data.
 *
 * Outputs: -
 *
 * Returns: NFY_RET_PROCESSED  The notification has been processed.
 *          NFY_RET_RETRY      The notify function could not process the
 *                             notification and the Table Manager should retry
 *                             again later.
 ----------------------------------------------------------------------------*/
static NFY_RET_T playback_load_pat_nfy (HANDLE_T       h_pat,
                                        HANDLE_TYPE_T  e_obj_type,
                                        TM_COND_T      e_nfy_cond,
                                        VOID*          pv_nfy_tag,
                                        UINT32         ui4_data)
{
    x_dbg_stmt("===== %s ====\r\n", __func__);
    return playback_load_nfy(h_pat,
                             e_obj_type,
                             e_nfy_cond,
                             pv_nfy_tag,
                             PLAYBACK_HANDLER_MSG_NONE,
                             PLAYBACK_HANDLER_MSG_PAT_UPDATED);
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_update_scdb_cc_streams_from_pmt
 *
 * Description: This API updates the SCDB's closed caption streams based on the
 *              PMT.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_conn_update_scdb_cc_streams_from_pmt (PLAYBACK_CONN_T*     pt_conn,
                                               PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    DBG_INFO((DBG_PREFIX"Updating CC streams in SCDB using PMT (%d)\n",
              pt_conn->h_ch_conn));

    cm_update_scdb_cc_streams_from_pmt(pt_conn->h_scdb,
                                       pt_buf_agt->h_pmt,
                                       pt_buf_agt->pv_cc_stream_tag,
                                       playback_get_stream_tag);
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_pmt_updated
 *
 * Description: This API is called when a PMT is updated and updates the SCDB
 *              accordingly.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
static INT32 playback_conn_pmt_updated (PLAYBACK_CONN_T*     pt_conn,
                                        PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    PLAYBACK_ITEM_T*  pt_item;

    pt_buf_agt->b_pmt_acquired = TRUE;

#if 0
    if ((pt_buf_agt->pt_unconsumed_requests != NULL)
        &&
        ((pt_item = pt_buf_agt->pt_unconsumed_requests->pt_item) != NULL)
        &&
        (pt_item->e_format == DATA_FMT_MPEG_2))
#else
    pt_item = pt_buf_agt->pt_transferring_item;
    if ((pt_item != NULL)
        &&
        (pt_item->e_format == DATA_FMT_MPEG_2))
#endif
    {
        UINT32    ui4_flags;

        if (TRUE == pt_conn->t_connect_info.b_2_languages_1_stream)
        {
            pt_item->t_extra.t_mpeg2_info.ui4_flags |= CM_FLAG_2_LANGUAGES_1_STREAM;
        }

        if (TRUE == pt_conn->t_connect_info.b_dual_mono_fake_stereo)
        {
            pt_item->t_extra.t_mpeg2_info.ui4_flags |= CM_FLAG_DUAL_MONO_FAKE_STEREO;
        }

        ui4_flags = pt_item->t_extra.t_mpeg2_info.ui4_flags;

        playback_conn_update_scdb_from_pmt(pt_conn, pt_buf_agt, ui4_flags);

        if (CM_IS_ATSC_SCTE_UNKNOWN(ui4_flags)
            &&
            (! pt_buf_agt->b_eit0_acquired))
        {
            playback_conn_update_scdb_cc_streams_from_pmt(pt_conn, pt_buf_agt);
        }
    }

    return CMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_acquire_pmt
 *
 * Description: This API starts the acquisition of a PMT.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
static INT32 playback_conn_acquire_pmt (PLAYBACK_CONN_T*        pt_conn,
                                        PLAYBACK_BUF_AGT_T*     pt_buf_agt)
{
    PLAYBACK_ITEM_T*  pt_item;
    INT32             i4_res;

#if 0
    if ((pt_buf_agt->pt_unconsumed_requests != NULL)
        &&
        ((pt_item = pt_buf_agt->pt_unconsumed_requests->pt_item) != NULL)
        &&
        (pt_item->e_format == DATA_FMT_MPEG_2))
#else
    pt_item = pt_buf_agt->pt_transferring_item;
    if ((pt_item != NULL)
        &&
        (pt_item->e_format == DATA_FMT_MPEG_2))
#endif
    {
        PLAYBACK_HANDLER_ITEM_SVC_INFO_T*  pt_svc_info;
        TM_COND_T                          e_cond;

        pt_svc_info = & pt_item->t_extra.t_mpeg2_info.t_svc_info;

        if(pt_svc_info->ui2_svc_pid != 0)
        {
            PSI_PID_SVC_ID_INFO_T              t_pid_svc_info;
            UINT16                             ui2_svc_idx   = 0;
            
            do
            {
                x_memset(&t_pid_svc_info,0,sizeof(PSI_PID_SVC_ID_INFO_T));

                if(TMR_OK == x_psi_get_svc_entry(pt_buf_agt->h_pat,
                                              ui2_svc_idx,
                                              &t_pid_svc_info,
                                              &e_cond))
                {
                    if(pt_svc_info->ui2_svc_pid == t_pid_svc_info.t_pid)
                    {
                        pt_svc_info->ui2_svc_id = t_pid_svc_info.ui2_svc_id;
                        break;
                    }
                }
                else
                {
                    break;
                }

                ui2_svc_idx ++ ;
                
            }while(TRUE);
            
        }
        x_dbg_stmt("\n====== svc id : %x=======\n", pt_svc_info->ui2_svc_id);
        if (pt_svc_info->ui2_svc_id != 0)
        {
            DBG_INFO((DBG_PREFIX"Loading PMT with svc_id=%u (%d)\n",
                      pt_svc_info->ui2_svc_id, pt_conn->h_ch_conn));

            i4_res = x_psi_load_pmt_by_svc_id(pt_buf_agt->h_pat,
                                              pt_svc_info->ui2_svc_id,
                                              TM_SRC_TYPE_SAME_AS_ROOT,
                                              NULL,
                                              (VOID*) pt_conn->h_ch_conn,
                                              playback_load_pmt_nfy,
                                              & pt_buf_agt->h_pmt,
                                              & e_cond);
        }
        else
        {
            DBG_INFO((DBG_PREFIX"Loading PMT with index=%u (%d)\n",
                      pt_svc_info->ui2_index, pt_conn->h_ch_conn));

            i4_res = x_psi_load_pmt_by_idx(pt_buf_agt->h_pat,
                                           pt_svc_info->ui2_index,
                                           TM_SRC_TYPE_SAME_AS_ROOT,
                                           NULL,
                                           (VOID*) pt_conn->h_ch_conn,
                                           playback_load_pmt_nfy,
                                           & pt_buf_agt->h_pmt,
                                           & e_cond);
        }

        if (i4_res == TMR_OK)
        {
            if (e_cond == TM_COND_AVAILABLE)
            {
                DBG_INFO((DBG_PREFIX"PMT loaded (%d)\n", pt_conn->h_ch_conn));

                i4_res = playback_conn_pmt_updated(pt_conn, pt_buf_agt);
            }
        }
        else
        {
            DBG_ERROR((DBG_PREFIX"Could not load PMT (%d)\n", i4_res));
        }

        i4_res = (i4_res == TMR_OK) ? CMR_OK : CMR_FAIL;
    }
    else
    {
        i4_res = CMR_FAIL;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_update_scdb_cc_streams_from_eit
 *
 * Description: This API updates the SCDB's closed caption streams based on the
 *              EIT.
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_buf_agt  References the buffer agent.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_conn_update_scdb_cc_streams_from_eit (PLAYBACK_CONN_T*     pt_conn,
                                               PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    TIME_T  t_current_time;

    DBG_INFO((DBG_PREFIX"Updating CC streams in SCDB using EIT (%d)\n",
              pt_conn->h_ch_conn));

    t_current_time = x_dt_clock_get_utc(pt_buf_agt->h_clock, NULL, NULL);

    cm_update_scdb_cc_streams_from_eit(pt_conn->h_scdb,
                                       pt_buf_agt->h_eit0,
                                       t_current_time,
                                       pt_buf_agt->pv_cc_stream_tag,
                                       playback_get_stream_tag);
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_eit_updated
 *
 * Description: This API is called when the EIT is updated and updates the CC
 *              streams in the SCDB.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_eit_updated (PLAYBACK_CONN_T*     pt_conn,
                                       PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    DT_COND_T  e_cond;

    pt_buf_agt->b_eit0_acquired = TRUE;

    x_dt_clock_get_utc(pt_buf_agt->h_clock, NULL, & e_cond);

    if (e_cond == DT_SYNC_RUNNING)
    {
        pt_buf_agt->b_time_sync = TRUE;

        playback_conn_update_scdb_cc_streams_from_eit(pt_conn, pt_buf_agt);
    }
    else
    {
        DBG_INFO((DBG_PREFIX"Time not synchronized yet (%d)\n",
                  pt_conn->h_ch_conn));
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_acquire_eit
 *
 * Description: This API starts the acquisition of a EIT-0.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_acquire_eit (PLAYBACK_CONN_T*     pt_conn,
                                       PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    PLAYBACK_ITEM_T*  pt_item;

    if ((pt_buf_agt->pt_unconsumed_requests != NULL)
        &&
        ((pt_item = pt_buf_agt->pt_unconsumed_requests->pt_item) != NULL)
        &&
        (pt_item->e_format == DATA_FMT_MPEG_2))
    {
        PLAYBACK_HANDLER_ITEM_SVC_INFO_T*  pt_svc_info;
        TM_COND_T                          e_cond;
        INT32                              i4_res;
        UINT16                             ui2_program_number;
        UINT16                             ui2_src_id;

        ui2_program_number = 0;
        ui2_src_id         = 0;

        pt_svc_info = & pt_item->t_extra.t_mpeg2_info.t_svc_info;

        if (pt_svc_info->ui2_svc_id != 0)
        {
            ui2_program_number = pt_svc_info->ui2_svc_id;
        }
        else
        {
            PSI_PID_SVC_ID_INFO_T  t_svc_info;

            i4_res = x_psi_get_svc_entry(pt_buf_agt->h_pat,
                                         pt_svc_info->ui2_index,
                                         & t_svc_info,
                                         NULL);

            if (i4_res == TMR_OK)
            {
                ui2_program_number = t_svc_info.ui2_svc_id;
            }
        }

        i4_res = cm_get_src_id_from_program_number(& ui2_src_id,
                                                   pt_buf_agt->h_vct,
                                                   ui2_program_number);

        if (i4_res == CMR_OK)
        {
            DBG_INFO((DBG_PREFIX"Loading EIT-0 with src_id=%d (%d)\n",
                      ui2_src_id, pt_conn->h_ch_conn));

            if (pt_buf_agt->h_eit0 != NULL_HANDLE)
            {
                x_tm_free(pt_buf_agt->h_eit0);

                pt_buf_agt->h_eit0          = NULL_HANDLE;
                pt_buf_agt->b_eit0_acquired = FALSE;
            }

            i4_res =
                x_atsc_si_load_eit_0_by_src_id(pt_buf_agt->h_mgt,
                                               ui2_src_id,
                                               TM_SRC_TYPE_SAME_AS_ROOT,
                                               NULL, /* pv_src_info */
                                               (VOID *) pt_conn->h_ch_conn,
                                               playback_load_eit0_nfy,
                                               & pt_buf_agt->h_eit0,
                                               & e_cond);

            if (i4_res == TMR_OK)
            {
                if (e_cond == TM_COND_AVAILABLE)
                {
                    DBG_INFO((DBG_PREFIX"EIT loaded (%d)\n",
                              pt_conn->h_ch_conn));

                    playback_conn_eit_updated(pt_conn, pt_buf_agt);
                }
                else
                {
                    DBG_INFO((DBG_PREFIX"EIT loading... (%d)\n",
                              pt_conn->h_ch_conn));
                }
            }
            else
            {
                DBG_ERROR((DBG_PREFIX"Could not load EIT (%d, res=%d).\n",
                           pt_conn->h_ch_conn, i4_res));
            }
        }
        else
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not get source id (program number %u)\n",
                       ui2_program_number));
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_pat_updated
 *
 * Description: This API is called when the PAT is updated (for the first time)
 *              and tries to acquire the PMT.
 *
 * Inputs:  pt_conn                 References the connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine was successful.
 *          CMR_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
static INT32 playback_conn_pat_updated (PLAYBACK_CONN_T*     pt_conn,
                                        PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    INT32                       i4_res;

    do
    {
        UINT16                      ui2_i;
        UINT16                      ui2_num_entry = 0;
        SIZE_T                      z_len = sizeof(UINT16);
        PSI_PID_SVC_ID_INFO_T       t_pid_svc_id;
        TM_COND_T                   e_cond;

        i4_res = x_tm_get_obj ( pt_buf_agt->h_pat,
                                TM_GET_TYPE_NUM_FIRST_LOOP_ENTRIES,
                                NULL,
                                (VOID *) & ui2_num_entry,
                                & z_len,
                                & e_cond);
        if ( i4_res != TMR_OK )
        {
            break;
        }
        
        if (!(e_cond == TM_COND_AVAILABLE ||
              e_cond == TM_COND_UPDATE))
        {
            /* wait for next PAT notify for stable state */
            break;
        }
        
        for (ui2_i = 0; ((ui2_i < ui2_num_entry) && (ui2_i < SUPPORT_PMT_MAX)); ui2_i ++)
        {
            i4_res = x_psi_get_pat_entry (pt_buf_agt->h_pat, 
                                          ui2_i,
                                          & t_pid_svc_id,
                                          & e_cond );
            
            if (i4_res == TMR_OK)
            {
                if (!(e_cond == TM_COND_AVAILABLE ||
                      e_cond == TM_COND_UPDATE))
                {
                    /* wait for next PAT notify for stable state */
                    break;
                }
                pt_buf_agt->t_pmt_list.ui1_pmt_num = ui2_i + 1;/*To avoid check*/
                pt_buf_agt->t_pmt_list.at_pmt_map[ui2_i].t_pid       = t_pid_svc_id.t_pid;
                pt_buf_agt->t_pmt_list.at_pmt_map[ui2_i].ui2_svc_id  = t_pid_svc_id.ui2_svc_id;
            }
            else
            {
                DBG_ERROR (("{SVCTX}  ERR: [REC_HDLR] Fail to get pat entry [%d/%d] (i4_res=%d). (L%d)\n", 
                            ui2_i, ui2_num_entry, i4_res, __LINE__)); 
                break;
            }
        }
    }while(0);
    
    cm_nfy( pt_conn->h_cm_conn,
            CM_PLAYBACK_CONN_INFO_PAT_GOT,
            (UINT32)&(pt_buf_agt->t_pmt_list));
    
    if ((! pt_buf_agt->b_pat_acquired) || (pt_buf_agt->h_pmt == NULL_HANDLE))
    {
        pt_buf_agt->b_pat_acquired = TRUE;

        i4_res = playback_conn_acquire_pmt(pt_conn, pt_buf_agt);

        if (pt_buf_agt->b_vct_acquired)
        {
            /*
              The PAT is needed since we'll find the source id in the VCT
              based on the service id (program number) in the PAT.
            */

            playback_conn_acquire_eit(pt_conn, pt_buf_agt);
        }
    }
    else
    {
        i4_res = CMR_OK;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_acquire_pat
 *
 * Description: This API starts the acquisition of a PAT.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
static INT32 playback_conn_acquire_pat (PLAYBACK_CONN_T*     pt_conn,
                                        PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    INT32  i4_res;

    if (pt_buf_agt->h_pat == NULL_HANDLE)
    {
        TM_COND_T  e_cond;

        DBG_INFO((DBG_PREFIX"Loading PAT (%d)\n", pt_conn->h_ch_conn));

        x_dbg_stmt("===== %s ====\r\n", __func__);
        i4_res = x_psi_load_pat(pt_buf_agt->h_tm_root,
                                TM_SRC_TYPE_SAME_AS_ROOT,
                                NULL,
                                (VOID *) pt_conn->h_ch_conn, /* tag */
                                playback_load_pat_nfy,
                                & pt_buf_agt->h_pat,
                                & e_cond);

        if (i4_res == TMR_OK)
        {
            if (e_cond == TM_COND_AVAILABLE)
            {
                DBG_INFO((DBG_PREFIX"PAT loaded (%d)\n", pt_conn->h_ch_conn));

                i4_res = playback_conn_pat_updated(pt_conn, pt_buf_agt);
            }
        }
        else
        {
            DBG_ERROR((DBG_PREFIX"Could not load PAT (%d)\n", i4_res));

            i4_res = CMR_FAIL;
        }
    }
    else
    {
        i4_res = CMR_OK;
    }

    return i4_res;
}

static VOID playback_conn_vct_updated (PLAYBACK_CONN_T*     pt_conn,
                                       PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    if ((! pt_buf_agt->b_vct_acquired) || (pt_buf_agt->h_eit0 == NULL_HANDLE))
    {
        pt_buf_agt->b_vct_acquired = TRUE;

        if (pt_buf_agt->b_pat_acquired)
        {
            /*
              The PAT is needed since we'll find the source id in the VCT
              based on the service id (program number) in the PAT.
            */

            playback_conn_acquire_eit(pt_conn, pt_buf_agt);
        }
    }
}

static VOID playback_conn_acquire_vct (PLAYBACK_CONN_T*     pt_conn,
                                       PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    TM_COND_T  e_cond;
    INT32      i4_res;

    DBG_INFO((DBG_PREFIX"Loading CVCT (%d)\n", pt_conn->h_ch_conn));

    i4_res = x_atsc_si_load_cvct(pt_buf_agt->h_mgt,
                                 TM_SRC_TYPE_SAME_AS_ROOT,
                                 NULL, /* pv_src_info */
                                 (VOID *) pt_conn->h_ch_conn, /* tag */
                                 playback_load_vct_nfy,
                                 & pt_buf_agt->h_vct,
                                 & e_cond);

    if (i4_res != TMR_OK)
    {
        DBG_INFO((DBG_PREFIX"No CVCT, now loading TVCT (%d)\n",
                  pt_conn->h_ch_conn));

        i4_res = x_atsc_si_load_tvct(pt_buf_agt->h_mgt,
                                     TM_SRC_TYPE_SAME_AS_ROOT,
                                     NULL, /* pv_src_info */
                                     (VOID *) pt_conn->h_ch_conn, /* tag */
                                     playback_load_vct_nfy,
                                     & pt_buf_agt->h_vct,
                                     & e_cond);
    }

    if (i4_res == TMR_OK)
    {
        if (e_cond == TM_COND_AVAILABLE)
        {
            DBG_INFO((DBG_PREFIX"VCT loaded (%d)\n", pt_conn->h_ch_conn));

            playback_conn_vct_updated(pt_conn, pt_buf_agt);
        }
        else
        {
            DBG_INFO((DBG_PREFIX"VCT loading... (%d)\n", pt_conn->h_ch_conn));
        }
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Could not load VCT (%d, res=%d).\n",
                   pt_conn->h_ch_conn, i4_res));
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_mgt_updated
 *
 * Description: This API is called when the MGT is updated (for the first time)
 *              and tries to acquire the EIT.
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_buf_agt  References the buffer agent.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_mgt_updated (PLAYBACK_CONN_T*     pt_conn,
                                       PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    if ((! pt_buf_agt->b_mgt_acquired) || (pt_buf_agt->h_vct == NULL_HANDLE))
    {
        pt_buf_agt->b_mgt_acquired = TRUE;

        playback_conn_acquire_vct(pt_conn, pt_buf_agt);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_acquire_mgt
 *
 * Description: This API starts the acquisition of a MGT.
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_buf_agt  References the buffer agent.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_acquire_mgt (PLAYBACK_CONN_T*     pt_conn,
                                       PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    TM_COND_T  e_cond;
    INT32      i4_res;

    DBG_INFO((DBG_PREFIX"Loading MGT (%d)\n", pt_conn->h_ch_conn));

    i4_res = x_atsc_si_load_mgt(pt_buf_agt->h_tm_root,
                                TM_SRC_TYPE_SAME_AS_ROOT,
                                NULL, /* pv_src_info */
                                (VOID*) pt_conn->h_ch_conn,
                                playback_load_mgt_nfy,
                                & pt_buf_agt->h_mgt,
                                & e_cond);

    if (i4_res == TMR_OK)
    {
        if (e_cond == TM_COND_AVAILABLE)
        {
            DBG_INFO((DBG_PREFIX"MGT loaded (%d)\n", pt_conn->h_ch_conn));

            playback_conn_mgt_updated(pt_conn, pt_buf_agt);
        }
        else
        {
            DBG_INFO((DBG_PREFIX"MGT loading... (%d)\n", pt_conn->h_ch_conn));
        }
    }
    else
    {
        DBG_INFO((DBG_PREFIX"Could not load MGT (%d, res=%d)\n",
                  pt_conn->h_ch_conn, i4_res));
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_acquire_tables
 *
 * Description: This API starts the table acquisition.
 *
 * Inputs:  pt_conn     References the connection structure.
 *          pt_buf_agt  References the buffer agent.
 *          pt_item     References the playback item.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 playback_conn_acquire_tables (PLAYBACK_CONN_T*     pt_conn,
                                           PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                           PLAYBACK_ITEM_T*     pt_item)
{
    INT32  i4_res;

    i4_res = TMR_OK;

    if (pt_buf_agt->h_tm_root == NULL_HANDLE)
    {
        CHAR   s_name[SYS_NAME_LEN + 1];

        x_sprintf(s_name, "PB-%u", pt_buf_agt->h_rm_buffer_agent);

        i4_res = x_tm_open_root(s_name,
                                TM_SRC_TYPE_CONN_HANDLE,
                                (VOID*) pt_conn->h_cm_conn,
                                NULL, /* tag */
                                NULL, /* notify function */
                                & pt_buf_agt->h_tm_root,
                                NULL);

        if (i4_res != TMR_OK)
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not create root (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));
        }
    }

    if (i4_res == TMR_OK)
    {
        i4_res = playback_conn_acquire_pat(pt_conn, pt_buf_agt);

        if (i4_res == CMR_OK)
        {
            UINT32  ui4_flags;

            ui4_flags = pt_item->t_extra.t_mpeg2_info.ui4_flags;

            if (CM_IS_ATSC_SCTE_UNKNOWN(ui4_flags))
            {
                playback_conn_acquire_mgt(pt_conn, pt_buf_agt);
            }
        }
    }
    else
    {
        i4_res = CMR_FAIL;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_svl_nfy
 *
 * Description: This API is called when the SVL is modified (or closed) and may
 *              send a message to the playback handler message loop.
 *
 * Inputs:  h_svl       Contains the SVL handle.
 *          e_cond      Contains the new condition.
 *          ui4_reason  Specifies the reason for the new condition.
 *          pv_tag      Contains the tag value passed when the SVL was opened.
 *          ui4_data    Contains additional data.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_svl_nfy (HANDLE_T    h_svl,
                              SVL_COND_T  e_cond,
                              UINT32      ui4_reason,
                              VOID*       pv_tag,
                              UINT32      ui4_data)
{

}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_open_svl
 *
 * Description: This API opens the connection's SVL.
 *
 * Inputs:  pt_conn  References the connection structure.
 *          pt_item  References a playback item.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_open_svl (PLAYBACK_CONN_T*  pt_conn,
                                    PLAYBACK_ITEM_T*  pt_item)
{
    UINT16  ui2_svl_id;

    ui2_svl_id = pt_item->t_extra.t_mpeg2_info.t_svc_info.ui2_svl_id;

    if (ui2_svl_id != SVL_NULL_ID)
    {
        HANDLE_T  h_old_svl;
        INT32     i4_res;

        h_old_svl = pt_conn->h_svl;

        i4_res = x_svl_open(ui2_svl_id,
                            (VOID*) pt_conn->h_ch_conn, /* tag */
                            playback_svl_nfy,
                            & pt_conn->h_svl);

        if (i4_res != SVLR_OK)
        {
            DBG_ERROR((DBG_PREFIX"Could not open SVL (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));
        }

        x_svl_close(h_old_svl);
    }
    else
    {
        DBG_INFO((DBG_PREFIX"No SVL to open (%d)\n", pt_conn->h_ch_conn));
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_get_info_from_svl
 *
 * Description: This API retrieves the service id from an SVL record.
 *
 * Inputs:  pt_conn  References the connection structure.
 *          pt_item  References a playback item.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_get_info_from_svl (PLAYBACK_CONN_T*  pt_conn,
                                             PLAYBACK_ITEM_T*  pt_item)
{
    if (pt_conn->h_svl != NULL_HANDLE)
    {
        SVL_REC_T  t_svl_rec;
        UINT32     ui4_svl_version;
        INT32      i4_res;
        UINT16     ui2_svl_rec_id;

        ui2_svl_rec_id =
            pt_item->t_extra.t_mpeg2_info.t_svc_info.ui2_svl_rec_id;

        i4_res = x_svl_get_rec(pt_conn->h_svl,
                               ui2_svl_rec_id,
                               & t_svl_rec,
                               & ui4_svl_version);

        if (i4_res == SVLR_OK)
        {
            pt_item->t_extra.t_mpeg2_info.t_svc_info.ui2_svc_id =
                t_svl_rec.uheader.t_rec_hdr.ui2_prog_id;
        }
        else
        {
            DBG_ERROR((DBG_PREFIX"Could not get SVL record (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));
        }
    }
    else
    {
        DBG_INFO((DBG_PREFIX"No SVL to get info from (%d)\n",
                  pt_conn->h_ch_conn));
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_close_svl
 *
 * Description: This API closes the connection's SVL.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_close_svl (PLAYBACK_CONN_T*  pt_conn)
{
    if (pt_conn->h_svl != NULL_HANDLE)
    {
        INT32  i4_res;

        i4_res = x_svl_close(pt_conn->h_svl);

        if (i4_res != SVLR_OK)
        {
            DBG_ERROR((DBG_PREFIX"Could not close SVL (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_create_scdb
 *
 * Description: This API creates a SCDB for a connection.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The SCDB has been created and initialized.
 *          CMR_FAIL  The SCDB could not be created.
 ----------------------------------------------------------------------------*/
static INT32 playback_conn_create_scdb (PLAYBACK_CONN_T*  pt_conn)
{
    SCDB_COMMON_T  t_scdb_common;
    INT32          i4_res;

    t_scdb_common.e_data_fmt  = DATA_FMT_UNKNOWN;
    t_scdb_common.e_feed_mode = FEED_MODE_PULL;

    t_scdb_common.u_data.t_mpeg.ui2_pcr = MPEG_2_NULL_PID;

    i4_res = scdb_create(pt_conn->h_cm_conn,
                         & t_scdb_common,
                         & pt_conn->h_scdb);

    i4_res = (i4_res == SCDBR_OK) ? CMR_OK : CMR_FAIL;

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_update_scdb_audio
 *
 * Description: This API adds or update a digital audio record with the right
 *              encoding and layer.
 *
 * Inputs:  pt_conn     References the connection.
 *          e_data_fmt  Contains the data format (e.g. MP3).
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The SCDB has been created and initialized.
 *          CMR_FAIL  The SCDB could not be created.
 ----------------------------------------------------------------------------*/
static INT32
playback_conn_update_scdb_audio (PLAYBACK_CONN_T*  pt_conn,
                                 DATA_FMT_T        e_data_fmt)
{
    STREAM_COMP_ID_T  t_stream_comp_id;
    SCDB_REC_T        t_scdb_rec;
    UINT32            ui4_version;
    INT32             i4_res;
    AUD_FMT_T         e_aud_fmt;
    AUD_ENC_T         e_aud_enc;
    AUD_LAYER_T       e_aud_layer;
    BOOL              b_update_record;

    e_aud_fmt = AUD_FMT_UNKNOWN;

    switch (e_data_fmt)
    {
        case DATA_FMT_MP3:
        {
            e_aud_enc   = AUD_ENC_MPEG_1;
            e_aud_layer = AUD_LAYER_3;
        }
        break;

        case DATA_FMT_MP2:
        {
            e_aud_enc   = AUD_ENC_MPEG_1;
            e_aud_layer = AUD_LAYER_2;
        }
        break;

        case DATA_FMT_AAC:
        {
            e_aud_enc   = AUD_ENC_AAC;
            e_aud_layer = AUD_LAYER_UNKNOWN;
        }
        break;

        default:
        {
            e_aud_enc   = AUD_ENC_UNKNOWN;
            e_aud_layer = AUD_LAYER_UNKNOWN;
        }
        break;
    }

    ui4_version     = SCDB_NULL_VER_ID;
    b_update_record = FALSE;

    if ((x_scdb_get_rec_by_idx(pt_conn->h_scdb,
                               ST_AUDIO, 0,
                               & t_stream_comp_id,
                               & t_scdb_rec,
                               & ui4_version) != SCDBR_OK)
        ||
        (t_scdb_rec.e_rec_type != SCDB_REC_TYPE_AUDIO_DIGITAL))
    {
        t_stream_comp_id.e_type        = ST_AUDIO;
        t_stream_comp_id.pv_stream_tag = playback_get_stream_tag();

        b_update_record = TRUE;
    }

    if (b_update_record
        ||
        (t_scdb_rec.u.t_audio_digital.e_aud_fmt != e_aud_fmt)
        ||
        (t_scdb_rec.u.t_audio_digital.e_enc != e_aud_enc)
        ||
        (t_scdb_rec.u.t_audio_digital.e_layer != e_aud_layer))
    {
        t_scdb_rec.e_rec_type  = SCDB_REC_TYPE_AUDIO_DIGITAL;
        t_scdb_rec.ui4_private = 0;

        t_scdb_rec.u.t_audio_digital.e_aud_fmt = e_aud_fmt;
        t_scdb_rec.u.t_audio_digital.e_enc     = e_aud_enc;
        t_scdb_rec.u.t_audio_digital.e_layer   = e_aud_layer;
        t_scdb_rec.u.t_audio_digital.b_default = TRUE;

        DBG_INFO((DBG_PREFIX
                  "Digital audio SCDB record, encoding %d, layer %d (%d)\n",
                  e_aud_enc, e_aud_layer, pt_conn->h_ch_conn));

        i4_res = scdb_update_rec(pt_conn->h_scdb,
                                 & t_stream_comp_id,
                                 & t_scdb_rec);

        i4_res = (i4_res == SCDBR_OK) ? CMR_OK : CMR_FAIL;
    }
    else
    {
        i4_res = CMR_OK;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_get_update_scdb_fct
 *
 * Description: This API gets the function to update the SCDB based on the new
 *              data format and the old data format.
 *
 * Inputs:  e_data_fmt      Contains the new data format.
 *          e_old_data_fmt  Contains the old data format.
 *
 * Outputs: pb_reset  Specifies whether the SCDB should be emptied.
 *
 * Returns: a valid function pointer, or NULL.
 ----------------------------------------------------------------------------*/
static playback_update_scdb_fct
playback_handler_get_update_scdb_fct (DATA_FMT_T  e_data_fmt,
                                      DATA_FMT_T  e_old_data_fmt,
                                      BOOL*       pb_reset)
{
    const PLAYBACK_FORMAT_INFO_T*  pt_format_info;
    const DATA_FMT_T*              pe_data_fmt;

    pt_format_info = & at_formats[0];

    while ((pe_data_fmt = pt_format_info->pe_compatible_formats) != NULL)
    {
        while (*pe_data_fmt != DATA_FMT_UNKNOWN)
        {
            if (*pe_data_fmt == e_data_fmt) /* found format */
            {
                if (e_data_fmt == e_old_data_fmt)
                {
                    /* same formats */

                    *pb_reset = ((pt_format_info->ui1_flags
                                  &
                                  PLAYBACK_FLAG_ALWAYS_RESET_SCDB) != 0);
                }
                else
                {
                    /* formats are different */

                    pe_data_fmt = pt_format_info->pe_compatible_formats;

                    while (*pe_data_fmt != DATA_FMT_UNKNOWN)
                    {
                        if (*pe_data_fmt == e_old_data_fmt)
                        {
                            /* formats are different but 'compatible' */

                            *pb_reset =
                                ((pt_format_info->ui1_flags
                                  &
                                  PLAYBACK_FLAG_RESET_SCDB_IF_DIFFERENT) != 0);

                            break;
                        }

                        pe_data_fmt++;
                    }
                }

                return pt_format_info->pf_update_scdb;
            }

            pe_data_fmt++;
        }

        pt_format_info++;
    }

    DBG_INFO((DBG_PREFIX
              "No predefined SCDB update function for data format %d\n",
              e_data_fmt));

    return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_set_scdb_format
 *
 * Description: This API sets the format in the common part of the connection's
 *              SCDB.
 *
 * Inputs:  pt_conn     References the connection.
 *          e_data_fmt  Contains the new data format.
 *
 * Outputs: pe_previous_fmt  Contains the previous format (if not NULL).
 *
 * Returns: CMR_OK    The SCDB has been created and initialized.
 *          CMR_FAIL  The SCDB could not be created.
 ----------------------------------------------------------------------------*/
static INT32 playback_conn_set_scdb_format (PLAYBACK_CONN_T*  pt_conn,
                                            DATA_FMT_T        e_data_fmt,
                                            DATA_FMT_T*       pe_previous_fmt)
{
    SCDB_COMMON_T  t_scdb_common;
    INT32          i4_res;

    i4_res = x_scdb_get_common(pt_conn->h_scdb, & t_scdb_common);

    if (i4_res == SCDBR_OK)
    {
        if (pe_previous_fmt != NULL)
        {
            *pe_previous_fmt = t_scdb_common.e_data_fmt;
        }

        if (e_data_fmt != t_scdb_common.e_data_fmt)
        {
            DBG_INFO((DBG_PREFIX"Setting SCDB data format to %d (%d)\n",
                      e_data_fmt, pt_conn->h_ch_conn));

            t_scdb_common.e_data_fmt = e_data_fmt;

            i4_res = scdb_set_common(pt_conn->h_scdb, & t_scdb_common);

            if (i4_res == SCDBR_OK)
            {
                i4_res = CMR_OK;
            }
            else
            {
                DBG_ERROR((DBG_PREFIX"Could not set SCDB common part (%d)\n",
                           i4_res));

                i4_res = CMR_FAIL;
            }
        }
        else
        {
            i4_res = CMR_OK;
        }
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Could not get SCDB common part (%d)\n",
                   i4_res));

        i4_res = CMR_FAIL;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_update_scdb_format
 *
 * Description: This API updates the format in the common part of the
 *              connection's SCDB.
 *
 * Inputs:  pt_conn     References the connection.
 *          e_data_fmt  Contains the new data format.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The SCDB has been created and initialized.
 *          CMR_FAIL  The SCDB could not be created.
 ----------------------------------------------------------------------------*/
static INT32 playback_conn_update_scdb_format (PLAYBACK_CONN_T*     pt_conn,
                                               PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                               DATA_FMT_T           e_data_fmt)
{
    DATA_FMT_T  e_previous_data_fmt;
    INT32       i4_res;

    i4_res = playback_conn_set_scdb_format(pt_conn, e_data_fmt,
                                           & e_previous_data_fmt);

    if (i4_res == CMR_OK)
    {
        playback_update_scdb_fct  pf_update_scdb;
        BOOL                      b_reset;

        b_reset = TRUE;

        pf_update_scdb =
            playback_handler_get_update_scdb_fct(e_data_fmt,
                                                 e_previous_data_fmt,
                                                 & b_reset);

        if (b_reset)
        {
            DBG_INFO((DBG_PREFIX"Emptying SCDB (%d)\n", pt_conn->h_ch_conn));

            cm_remove_stream_type_from_scdb(pt_conn->h_scdb, ST_UNKNOWN);
        }

        if (pf_update_scdb != NULL)
        {
            i4_res = pf_update_scdb(pt_conn, e_data_fmt);
        }
        else
        {
            /* unrecognized format */
            i4_res = CMR_OK;
        }
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_streams_are_equivalent
 *
 * Description: This API checks if 2 streams are equivalent. Depending on the
 *              stream type, the criteria for "equivalence" may be different.
 *
 * Inputs:  pt_stream_1  References the first stream component ID (type+tag)
 *          pt_rec_1     References the first SCDB record.
 *          pt_stream_2  References the second stream component ID (type+tag)
 *          pt_rec_2     References the second SCDB record.
 *
 * Outputs: -
 *
 * Returns: TRUE   The streams are equivalent.
 *          FALSE  The streams are not equivalent or the algorithm could not
 *                 decide whether they were equivalent.
 ----------------------------------------------------------------------------*/
static BOOL
playback_handler_streams_are_equivalent (const STREAM_COMP_ID_T*  pt_stream_1,
                                         const SCDB_REC_T*        pt_rec_1,
                                         const STREAM_COMP_ID_T*  pt_stream_2,
                                         const SCDB_REC_T*        pt_rec_2)
{
    BOOL  b_equivalent;

    b_equivalent =
        (pt_stream_1->e_type == pt_stream_2->e_type)
        &&
        (pt_rec_1->e_rec_type == pt_rec_2->e_rec_type);

    if (b_equivalent)
    {
        switch (pt_rec_1->e_rec_type)
        {
            case SCDB_REC_TYPE_AUDIO_MPEG:
            {
                b_equivalent =
                    ((pt_rec_1->u.t_audio_mpeg.ui2_pid
                      ==
                      pt_rec_2->u.t_audio_mpeg.ui2_pid)
                     &&
                     (pt_rec_1->u.t_audio_mpeg.ui1_index
                      ==
                      pt_rec_2->u.t_audio_mpeg.ui1_index));
            }
            break;

            case SCDB_REC_TYPE_VIDEO_MPEG:
            {
                b_equivalent =
                    (pt_rec_1->u.t_video_mpeg.ui2_pid
                     ==
                     pt_rec_2->u.t_video_mpeg.ui2_pid);
            }
            break;

            case SCDB_REC_TYPE_CLOSED_CAPTION:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_SUBTITLE_DVB:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_TELETEXT_DVB:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_VBI_TELETEXT_DVB:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_VBI_DATA_DVB:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_PRIVATE_MPEG:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_DSMCC_MPEG:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_AUDIO_DIGITAL:
            {
                b_equivalent =
                    ((pt_rec_1->u.t_audio_digital.e_enc
                      ==
                      pt_rec_2->u.t_audio_digital.e_enc)
                     &&
                     (pt_rec_1->u.t_audio_digital.e_layer
                      ==
                      pt_rec_2->u.t_audio_digital.e_layer));
            }
            break;

            case SCDB_REC_TYPE_AUDIO_AVI:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_VIDEO_AVI:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_SUBTITLE_DIVX:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_AUDIO_MP4:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_VIDEO_MP4:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_AUDIO_ASF:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_VIDEO_ASF:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_AUDIO_MKV:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_VIDEO_MKV:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_AUDIO_PS:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_VIDEO_PS:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_AUDIO_RM:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_VIDEO_RM:
            {
                b_equivalent = FALSE;
            }
            break;

            case SCDB_REC_TYPE_AUDIO_ANALOG:
            case SCDB_REC_TYPE_VIDEO_ANALOG:
            default:
            {
                DBG_ERROR((DBG_PREFIX"Missing support for record type (%d)\n",
                           pt_rec_1->e_rec_type));

                b_equivalent = FALSE;
            }
            break;
        }
    }

    return b_equivalent;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_assign_stream_tag
 *
 * Description: This API assigns a stream tag, either new or copied from an
 *              equivalent stream.
 *
 * Inputs:  pt_stream_comp  References the stream component ID.
 *          pt_scdb_rec     References the SCDB record.
 *          h_scdb          Contains the SCDB handle.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_assign_stream_tag (STREAM_COMP_ID_T*  pt_stream_comp,
                                             const SCDB_REC_T*  pt_scdb_rec,
                                             HANDLE_T           h_scdb)
{
    STREAM_COMP_ID_T  t_stream_comp_id;
    SCDB_REC_T        t_scdb_rec;
    VOID*             pv_stream_tag;
    UINT32            ui4_version;
    UINT16            ui2_index;

    ui4_version = SCDB_NULL_VER_ID;
    ui2_index   = 0;

    pv_stream_tag = NULL;

    while (x_scdb_get_rec_by_idx(h_scdb,
                                 ST_UNKNOWN, ui2_index,
                                 & t_stream_comp_id,
                                 & t_scdb_rec,
                                 & ui4_version) == SCDBR_OK)
    {
        BOOL  b_equivalent;

        b_equivalent =
            playback_handler_streams_are_equivalent(pt_stream_comp,
                                                    pt_scdb_rec,
                                                    & t_stream_comp_id,
                                                    & t_scdb_rec);

        if (b_equivalent)
        {
            INT32  i4_res;

            /*
              The new stream will use the same stream tag since the 2 streams
              are "equivalent".
            */

            pv_stream_tag = t_stream_comp_id.pv_stream_tag;

            /*
              The stream must be removed now to prevent another stream to get
              the same stream tag.
            */

            i4_res = scdb_del_rec(h_scdb, & t_stream_comp_id);

            if (i4_res != SCDBR_OK)
            {
                DBG_ERROR((DBG_PREFIX
                           "Could not delete stream from SCDB (%d)\n",
                           i4_res));
            }

            break;
        }

        ui4_version = SCDB_NULL_VER_ID;
        ui2_index++;
    }

    if (pv_stream_tag == NULL)
    {
        pv_stream_tag = playback_get_stream_tag();
    }

    pt_stream_comp->pv_stream_tag = pv_stream_tag;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_update_scdb_streams
 *
 * Description: This API updates the SCDB according to the item's list of
 *              streams.
 *
 * Inputs:  pt_conn  References the connection.
 *          pt_item  References the playback item. Cannot be NULL.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The SCDB has been updated successfully.
 *          CMR_FAIL  The SCDB could not be updated.
 ----------------------------------------------------------------------------*/
static INT32
playback_conn_update_scdb_streams (PLAYBACK_CONN_T*        pt_conn,
                                   PLAYBACK_BUF_AGT_T*     pt_buf_agt,
                                   const PLAYBACK_ITEM_T*  pt_item)
{
    UINT32  ui4_i;

    /*
      first, assign new tags or reuse existing tags for streams that do not
      have any tag already assigned (the streams tags can be assigned by the
      playback handler client when an item is queued)
    */

    for (ui4_i = 0; ui4_i < pt_item->ui4_nb_scdb_records; ui4_i++)
    {
        PLAYBACK_HANDLER_ITEM_SCDB_INFO_T*  pt_scdb_info;

        pt_scdb_info = & pt_item->pt_scdb_info[ui4_i];

        if (pt_scdb_info->t_scdb_rec.ui4_private
            &
            PLAYBACK_HANDLER_NO_STREAM_TAG_ASSIGNED)
        {
            playback_conn_assign_stream_tag(& pt_scdb_info->t_stream_id,
                                            & pt_scdb_info->t_scdb_rec,
                                            pt_conn->h_scdb);
        }
    }

    /*
      second, empty existing SCDB (note that some streams may have been removed
      already during the tag assignment above)
    */
    cm_remove_stream_type_from_scdb(pt_conn->h_scdb, ST_UNKNOWN);

    /*
      third, set the data format of the SCDB
    */
    playback_conn_set_scdb_format(pt_conn, pt_item->e_format, NULL);

    /*
      fourth, add streams from list
    */
    for (ui4_i = 0; ui4_i < pt_item->ui4_nb_scdb_records; ui4_i++)
    {
        PLAYBACK_HANDLER_ITEM_SCDB_INFO_T*  pt_scdb_info;

        pt_scdb_info = & pt_item->pt_scdb_info[ui4_i];

        scdb_add_rec(pt_conn->h_scdb,
                     & pt_scdb_info->t_stream_id,
                     & pt_scdb_info->t_scdb_rec);
    }

    return CMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_delete_clock
 *
 * Description: This API deletes the connection's clock.
 *
 * Inputs:  pt_buf_agt  References the buffer agent.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_delete_clock (PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    if (pt_buf_agt->h_clock != NULL_HANDLE)
    {
        x_handle_free(pt_buf_agt->h_clock);

        pt_buf_agt->h_clock = NULL_HANDLE;
    }

    pt_buf_agt->b_time_sync = FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_dt_nfy
 *
 * Description: This API is called by the DT library to notify changes in the
 *              clock's status.
 *
 * Inputs:  h_clock  Contains the clock handle.
 *          pv_tag   Private tag specified when the clock was created.
 *          e_cond   Contains the clock's condition.
 *          t_delta  Contains a time delta. Unused.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_dt_nfy (HANDLE_T   h_clock,
                                  VOID*      pv_tag,
                                  DT_COND_T  e_cond,
                                  TIME_T     t_delta)
{
    CRIT_STATE_T         t_state;
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
    HANDLE_T             h_buffer_agent;
    HANDLE_T             h_ch_conn;
    INT32                i4_res;

    h_buffer_agent = (HANDLE_T) pv_tag;

    t_state = x_crit_start();

    i4_res = handle_get_obj(h_buffer_agent, (VOID**) & pt_buf_agt);

    h_ch_conn = (i4_res == HR_OK) ? pt_buf_agt->h_ch_conn : NULL_HANDLE;

    x_crit_end(t_state);

    if ((e_cond == DT_SYNC_RUNNING) && (h_ch_conn != NULL_HANDLE))
    {
        cm_send_msg_no_retry(h_playback_handler_msgq,
                             PLAYBACK_HANDLER_MSG_TIME_SYNC, h_buffer_agent, 0,
                             h_ch_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_create_clock
 *
 * Description: This API creates the connection's clock based on the playback
 *              item's properties (ATSC or DVB).
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_buf_agt  References the buffer agent.
 *          pt_item     References the playback item.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The clock was created successfully.
 *          CMR_FAIL  The clock could not be created
 ----------------------------------------------------------------------------*/
static INT32 playback_conn_create_clock (PLAYBACK_CONN_T*        pt_conn,
                                         PLAYBACK_BUF_AGT_T*     pt_buf_agt,
                                         const PLAYBACK_ITEM_T*  pt_item)
{
    CHAR   s_clock_name[SYS_NAME_LEN + 1];
    INT32  i4_res;

    x_sprintf(s_clock_name, "PBc-%u", pt_buf_agt->h_buffer_agent);

    i4_res = x_dt_clock_create(s_clock_name,
                               playback_conn_dt_nfy,
                               (VOID*) pt_buf_agt->h_buffer_agent,
                               & pt_buf_agt->h_clock);

    if (i4_res == DTR_OK)
    {
        DT_SYNC_SRC_TYPE_T  e_sync_type;
        UINT32              ui4_flags;

        ui4_flags = pt_item->t_extra.t_mpeg2_info.ui4_flags;

        if (CM_IS_ATSC_SCTE_UNKNOWN(ui4_flags))
        {
            e_sync_type = DT_SYNC_WITH_ATSC_STT;
        }
        else if (CM_IS_DVB(ui4_flags))
        {
            e_sync_type = DT_SYNC_WITH_DVB_TDT_OR_TOT;
        }

        i4_res = x_dt_clock_set_sync_src(pt_buf_agt->h_clock,
                                         e_sync_type,
                                         DT_SRC_TYPE_CONN_HANDLE,
                                         (VOID*) pt_conn->h_cm_conn);

        if (i4_res != DTR_OK)
        {
            DBG_ERROR((DBG_PREFIX"Could not set clock's sync source (%d)\n",
                       i4_res));

            playback_conn_delete_clock(pt_buf_agt);
        }

        i4_res = (i4_res == DTR_OK) ? CMR_OK : CMR_FAIL;
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Could not create clock (%d)\n", i4_res));

        i4_res = CMR_FAIL;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_reset_scdb_cc
 *
 * Description: This API resets the CC streams in the SCDB. It will either
 *              remove all the closed caption streams (if the item is not ATSC,
 *              SCTE or unknown) or remove the advanced closed caption streams
 *              and resets the 4 default closed caption streams.
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_buf_agt  References the buffer agent.
 *          pt_item     References the playback item. May be NULL.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_reset_scdb_cc (PLAYBACK_CONN_T*     pt_conn,
                                         PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                         PLAYBACK_ITEM_T*     pt_item)
{
    UINT32  ui4_flags;

    ui4_flags = pt_item->t_extra.t_mpeg2_info.ui4_flags;

    if (CM_IS_ATSC_SCTE_UNKNOWN(ui4_flags))
    {
        cm_remove_adv_cc_streams_from_scdb(pt_conn->h_scdb, NULL_HANDLE, 0);

        cm_reset_scdb_cc_streams(pt_conn->h_scdb,
                                 pt_buf_agt->pv_cc_stream_tag);
    }
    else
    {
        cm_remove_stream_type_from_scdb(pt_conn->h_scdb, ST_CLOSED_CAPTION);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_update_scdb
 *
 * Description: This API updates the SCDB according to the playback item.
 *
 * Inputs:  pt_conn  References the connection.
 *          pt_item  References the playback item. May be NULL.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The SCDB has been updated successfully.
 *          CMR_FAIL  The SCDB could not be updated.
 ----------------------------------------------------------------------------*/
static INT32 playback_conn_update_scdb (PLAYBACK_CONN_T*     pt_conn,
                                        PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                        PLAYBACK_ITEM_T*     pt_item,
                                        UINT32               ui4_prev_flags)
{
    INT32  i4_res;

    if (pt_buf_agt->b_shared_memory)
    {
        if ((pt_item==NULL)||(pt_item->pt_scdb_info == NULL))
        {
            return CMR_OK;
        }
    }

    i4_res = scdb_lock(pt_conn->h_scdb);

    if (i4_res == SCDBR_OK)
    {
        if (pt_item == NULL)
        {
            if (pt_buf_agt->b_shared_memory)
            {
                i4_res = CMR_OK;
            }
            else
            {
                if ((ui4_prev_flags & CM_PLAYBACK_FLAG_STICKY_SCDB) == 0)
                {
                    i4_res = playback_conn_update_scdb_format(pt_conn,
                                                              pt_buf_agt,
                                                              DATA_FMT_UNKNOWN);
                }
                else
                {
                    DBG_INFO((DBG_PREFIX"Sticky SCDB\n"));

                    i4_res = CMR_OK;
                }
            }
        }
        else if (pt_item->pt_scdb_info == NULL)
        {
            if (pt_buf_agt->b_shared_memory)
            {
                i4_res = CMR_OK;
            }
            else
            {
                i4_res = playback_conn_update_scdb_format(pt_conn,
                                                          pt_buf_agt,
                                                          pt_item->e_format);
            }
        }
        else
        {
            i4_res = playback_conn_update_scdb_streams(pt_conn,
                                                       pt_buf_agt,
                                                       pt_item);
        }

        scdb_unlock(pt_conn->h_scdb);

        if (i4_res == CMR_OK)
        {
            if ((pt_item != NULL) && (pt_item->e_format == DATA_FMT_MPEG_2))
            {
                DBG_INFO((DBG_PREFIX"Acquiring tables\n"));

                playback_conn_open_svl(pt_conn, pt_item);

                playback_conn_get_info_from_svl(pt_conn, pt_item);

                playback_conn_create_clock(pt_conn, pt_buf_agt, pt_item);

                i4_res = playback_conn_acquire_tables(pt_conn, pt_buf_agt,
                                                      pt_item);

                playback_conn_reset_scdb_cc(pt_conn, pt_buf_agt, pt_item);
            }
            else
            {
                if (!pt_buf_agt->b_shared_memory)
                {
                    playback_conn_close_svl(pt_conn);

                    playback_conn_delete_clock(pt_buf_agt);

                    playback_conn_delete_tables(pt_conn, pt_buf_agt);

                    i4_res = scdb_lock(pt_conn->h_scdb);

                    if (i4_res == SCDBR_OK)
                    {
                        cm_remove_stream_type_from_scdb(pt_conn->h_scdb,
                                                        ST_CLOSED_CAPTION);
                        scdb_unlock(pt_conn->h_scdb);
                    }
                }
            }
        }
        else
        {
            DBG_ERROR((DBG_PREFIX"Could not update SCDB (%d)\n", i4_res));
        }
    }
    else
    {
        i4_res = CMR_FAIL;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_request_data_transfer
 *
 * Description: This API sends a 'transfer data' request to the main message
 *              loop.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK  The request was sent successfully.
 *          < 0     The request could not be sent.
 ----------------------------------------------------------------------------*/
static INT32 playback_conn_request_data_transfer (PLAYBACK_CONN_T*  pt_conn)
{
    INT32  i4_res;

    i4_res = cm_send_msg_retry(h_playback_handler_msgq,
                               PLAYBACK_HANDLER_MSG_TRANSFER_DATA,
                               0, 0,
                               pt_conn->h_ch_conn);

    if (i4_res != CMR_OK)
    {
        DBG_ERROR((DBG_PREFIX"Could not start data transfer (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_res));
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_ack_data_request
 *
 * Description: This API calls the buffer agent's ack function to let it know
 *              that the data request succeeded, failed, or was canceled.
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_request  References the data request.
 *          e_cond      Contains the condition.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_conn_ack_data_request (PLAYBACK_CONN_T*             pt_conn,
                                PLAYBACK_BUF_AGT_T*          pt_buf_agt,
                                PLAYBACK_DATA_REQUEST_T*     pt_request,
                                BUF_AGT_DATA_REQ_ACK_COND_T  e_cond)
{
    BUF_AGT_ACK_NFY_T  t_ack;
    UINT32             ui4_offset;
    INT32              i4_res;

    t_ack.t_block.ui4_flags = 0;

    if (pt_request->b_last)
    {
        DBG_INFO((DBG_PREFIX"Transfer end (item %u)\n",
                  pt_request->pt_item->ui4_id));

        cm_nfy(pt_conn->h_cm_conn,
               CM_PLAYBACK_CONN_INFO_ITEM_TRANSFER_END,
               pt_request->pt_item ? pt_request->pt_item->ui4_id : 0);

        if (!pt_buf_agt->b_shared_memory)
        {
            /* to let the buffer agent know we reached the end of file/stream */
            t_ack.t_block.ui4_flags |= BUF_AGT_GET_DATA_EOF;
        }
    }

    ui4_offset = pt_request->ui4_offset;

    t_ack.e_cond             = e_cond;
    t_ack.t_block.ui8_offset = pt_request->ui8_transfer_position;
    t_ack.t_block.pui1_buff  = (UINT8*) pt_request->pv_buffer + ui4_offset;
    t_ack.t_block.z_size     = pt_request->z_size - ui4_offset;
    t_ack.t_block.ui4_req_id = pt_request->ui4_req_id;
    t_ack.t_block.e_data_fmt = (pt_request->pt_item != NULL) ?
        pt_request->pt_item->e_format : DATA_FMT_UNKNOWN;

    if (b_log_cmpb)
    {
        x_dbg_stmt("[CMPB] ack data pos=%d size=%d\n", (UINT32)pt_request->ui8_transfer_position, (UINT32)pt_request->z_size);
    }

    if (pt_request->pt_item != NULL)
    {
        switch (pt_request->pt_item->e_format)
        {
            case DATA_FMT_AUDIO_ES:
            case DATA_FMT_VIDEO_ES:
            {
                t_ack.t_block.t_extra.t_es_info.t_pts[0] =
                    pt_request->t_pts;
                t_ack.t_block.t_extra.t_es_info.ui4_pts_size[0] =
                    t_ack.t_block.z_size;
                t_ack.t_block.t_extra.t_es_info.ui2_pts_cnt = 1;
                t_ack.t_block.t_extra.t_es_info.b_eof       = FALSE;
            }
            break;

            case DATA_FMT_IBC:
            {
                x_memcpy(&t_ack.t_block.t_extra.t_ctrl_ibc, &pt_request->pt_item->t_extra.t_ibc_info, sizeof(BUF_AGT_CTRL_IBC_T));
            }
            break;

            default:
                break;
        }
    }

    i4_res = rm_set(pt_buf_agt->h_rm_buffer_agent,
                    FALSE,
                    ANY_PORT_NUM,
                    BUF_AGT_SET_TYPE_ACK,
                    (VOID*) & t_ack,
                    sizeof(BUF_AGT_ACK_NFY_T),
                    NULL);

    if (i4_res != RMR_OK)
    {
        DBG_INFO((DBG_PREFIX
                  "Buffer agent's ACK failed (%d, res=%d)\n",
                  pt_conn->h_ch_conn, i4_res));
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_nfy_align_info
 *
 * Description: This API notifies the buffer agent of a change of alignment and
 *              block size. This is only done
 *
 * Inputs:  pt_buf_agt      References the buffer agent.
 *          ui4_alignment   Contains the alignment.
 *          ui4_block_size  Contains the block size.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_conn_nfy_align_info (const PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                              const PLAYBACK_ITEM_T*     pt_item)
{
    if ((pt_item != NULL) && (pt_item->e_type == PLAYBACK_ITEM_TYPE_FILE))
    {
        BUF_AGT_SRC_ALIGN_INFO_T  t_src_align_info;
        INT32                     i4_res;

        DBG_INFO((DBG_PREFIX
                  "Setting buffer agent alignment (%u) and block zize (%u)\n",
                  pt_item->u.t_file.ui4_alignment,
                  pt_item->u.t_file.ui4_block_size));

        t_src_align_info.ui4_align        = pt_item->u.t_file.ui4_alignment;
        t_src_align_info.ui4_min_blk_size = pt_item->u.t_file.ui4_block_size;

        i4_res = rm_set(pt_buf_agt->h_rm_buffer_agent,
                        FALSE,
                        ANY_PORT_NUM,
                        BUF_AGT_SET_TYPE_SRC_ALIGN_INFO,
                        & t_src_align_info,
                        sizeof(BUF_AGT_SRC_ALIGN_INFO_T),
                        NULL);

        if (i4_res != RMR_OK)
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not set align info (%d, res=%d)\n",
                       pt_buf_agt->h_ch_conn, i4_res));
        }
    }
    else if ((pt_item != NULL) && (pt_item->e_type == PLAYBACK_ITEM_TYPE_URI))
    {
        BUF_AGT_SRC_ALIGN_INFO_T  t_src_align_info;
        INT32                     i4_res;

        DBG_INFO((DBG_PREFIX
                  "Setting buffer agent alignment (%u) and block zize (%u)\n",
                  pt_item->u.t_file.ui4_alignment,
                  pt_item->u.t_file.ui4_block_size));

        t_src_align_info.ui4_align        = pt_item->u.t_uri.ui4_alignment;
        t_src_align_info.ui4_min_blk_size = pt_item->u.t_uri.ui4_block_size;

        i4_res = rm_set(pt_buf_agt->h_rm_buffer_agent,
                        FALSE,
                        ANY_PORT_NUM,
                        BUF_AGT_SET_TYPE_SRC_ALIGN_INFO,
                        & t_src_align_info,
                        sizeof(BUF_AGT_SRC_ALIGN_INFO_T),
                        NULL);

        if (i4_res != RMR_OK)
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not set align info (%d, res=%d)\n",
                       pt_buf_agt->h_ch_conn, i4_res));
        }
    }
    else if ((pt_item != NULL) && (pt_item->e_type == PLAYBACK_ITEM_TYPE_PULL))
    {
        BUF_AGT_SRC_ALIGN_INFO_T  t_src_align_info;
        INT32                     i4_res;

        DBG_INFO((DBG_PREFIX
                  "Setting buffer agent alignment (%u) and block zize (%u)\n",
                  pt_item->u.t_file.ui4_alignment,
                  pt_item->u.t_file.ui4_block_size));

        t_src_align_info.ui4_align        = pt_item->u.t_pull.ui4_alignment;
        t_src_align_info.ui4_min_blk_size = pt_item->u.t_pull.ui4_block_size;

        i4_res = rm_set(pt_buf_agt->h_rm_buffer_agent,
                        FALSE,
                        ANY_PORT_NUM,
                        BUF_AGT_SET_TYPE_SRC_ALIGN_INFO,
                        & t_src_align_info,
                        sizeof(BUF_AGT_SRC_ALIGN_INFO_T),
                        NULL);

        if (i4_res != RMR_OK)
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not set align info (%d, res=%d)\n",
                       pt_buf_agt->h_ch_conn, i4_res));
        }
    }
    else if (pt_buf_agt->b_shared_memory)
    {
        BUF_AGT_SRC_ALIGN_INFO_T  t_src_align_info;

        t_src_align_info.ui4_align        = 1;
        t_src_align_info.ui4_min_blk_size = 1;

        rm_set(pt_buf_agt->h_rm_buffer_agent,
               FALSE,
               ANY_PORT_NUM,
               BUF_AGT_SET_TYPE_SRC_ALIGN_INFO,
               & t_src_align_info,
               sizeof(BUF_AGT_SRC_ALIGN_INFO_T),
               NULL);
    }

}
/*-----------------------------------------------------------------------------
 * Name: playback_conn_fm_nfy
 *
 * Description: This API is the file manager notification function. It is for
 *              instance called when a read operation completed successfully.
 *
 * Inputs:  pv_tag      References a tag.
 *          e_nfy_cond  Contains the notification condition.
 *          ui4_data    Contains some additional data.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 playback_conn_pull_nfy (PLAYBACK_CTRL_PULL_EVENT_T    e_event,
                                    VOID*                          pv_tag,
                                    HANDLE_T                       h_req,
                                    UINT32                         ui4_data)
{
    CRIT_STATE_T              t_state;
    PLAYBACK_DATA_REQUEST_T*  pt_request;
    HANDLE_T                  h_request;
    HANDLE_T                  h_ch_conn;
    HANDLE_T                  h_buffer_agent;
    HANDLE_TYPE_T             e_type;
    INT32                     i4_res;
    BOOL                      b_aborted;
    BOOL                      b_flush_done;
    BOOL                      b_valid;

    h_request      = (HANDLE_T) pv_tag;
    h_ch_conn      = NULL_HANDLE;
    h_buffer_agent = NULL_HANDLE;
    pt_request     = NULL;
    b_aborted      = FALSE;
    b_flush_done   = FALSE;

    if (b_log_cmpb)
    {
        x_dbg_stmt("[CMPB] playback_conn_pull_nfy %d %d\n", e_event, ui4_data);
    }

    t_state = x_crit_start();

    i4_res = handle_get_type_obj(h_request, & e_type, (VOID*) & pt_request);

    b_valid =
        (i4_res == HR_OK) &&
        (e_type == PHT_DATA_REQUEST) &&
        (pt_request != NULL);

    if (b_valid)
    {
        h_ch_conn      = pt_request->h_ch_conn;
        h_buffer_agent = pt_request->h_buffer_agent;

        pt_request->b_done = TRUE;

        b_aborted    = pt_request->b_aborted;
        b_flush_done = pt_request->b_flush_done;
    }

    x_crit_end(t_state);

    if (b_valid)
    {
        if (b_aborted)
        {
            DBG_INFO((DBG_PREFIX
                      "PULL notification %d, but request was aborted (%d)\n",
                      e_event, pt_request->h_ch_conn));

            if ((e_event == PLAYBACK_CTRL_PULL_READ_OK)
                ||
                (e_event == PLAYBACK_CTRL_PULL_ABORT_OK)
                ||
                (e_event == PLAYBACK_CTRL_PULL_FAIL)
                ||
                (e_event == PLAYBACK_CTRL_PULL_READ_EOS))
            {
                playback_handler_release_data_request(pt_request);

                if (b_flush_done)
                {
                    cm_send_msg_no_retry(h_playback_handler_msgq,
                                         PLAYBACK_HANDLER_MSG_FLUSH_DONE,
                                         0, (UINT32) h_buffer_agent,
                                         h_ch_conn);
                }
            }
        }
        else
        {
            UINT32  ui4_code;

            switch (e_event)
            {
                case PLAYBACK_CTRL_PULL_READ_OK:
                {
                    ui4_code = PLAYBACK_HANDLER_MSG_ACK_DATA_TRANSFER_OK;
                }
                break;

                case PLAYBACK_CTRL_PULL_FAIL:
                {
                    ui4_code = PLAYBACK_HANDLER_MSG_ACK_DATA_TRANSFER_FAILED;
                }
                break;

                case PLAYBACK_CTRL_PULL_READ_EOS:
                {
                    ui4_code = PLAYBACK_HANDLER_MSG_ACK_DATA_TRANSFER_EOF;
                }
                break;

                case PLAYBACK_CTRL_PULL_ABORT_OK:
                case PLAYBACK_CTRL_PULL_ABORT_FAIL:
                default:
                {
                    ui4_code = PLAYBACK_HANDLER_MSG_NONE;
                }
                break;
            }

            if (ui4_code != PLAYBACK_HANDLER_MSG_NONE)
            {
                cm_send_msg_no_retry(h_playback_handler_msgq,
                                     ui4_code, (UINT32) pt_request, ui4_data,
                                     h_ch_conn);
            }
            else
            {
                DBG_INFO((DBG_PREFIX"Ignored Pull condition %d (%d)\n",
                          e_event, h_ch_conn));
            }
        }
    }

    return 0;
}


/*-----------------------------------------------------------------------------
 * Name: playback_conn_fm_nfy
 *
 * Description: This API is the file manager notification function. It is for
 *              instance called when a read operation completed successfully.
 *
 * Inputs:  pv_tag      References a tag.
 *          e_nfy_cond  Contains the notification condition.
 *          ui4_data    Contains some additional data.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
#if 1  //PLAYBACK_HANDLER_USE_ASYNC_FM_READ
static VOID playback_conn_fm_nfy (HANDLE_T         h_req,
                                  VOID*            pv_tag,
                                  FM_ASYNC_COND_T  e_nfy_cond,
                                  UINT32           ui4_data)
{
    CRIT_STATE_T              t_state;
    PLAYBACK_DATA_REQUEST_T*  pt_request;
    HANDLE_T                  h_request;
    HANDLE_T                  h_ch_conn;
    HANDLE_T                  h_buffer_agent;
    HANDLE_TYPE_T             e_type;
    INT32                     i4_res;
    BOOL                      b_aborted;
    BOOL                      b_flush_done;
    BOOL                      b_valid;

    h_request      = (HANDLE_T) pv_tag;
    h_ch_conn      = NULL_HANDLE;
    h_buffer_agent = NULL_HANDLE;
    pt_request     = NULL;
    b_aborted      = FALSE;
    b_flush_done   = FALSE;

#ifdef LINUX_TURNKEY_SOLUTION
    struct tms t_tms;

    ui4_end_time = times(&t_tms);
#else
    ui4_end_time = x_os_get_sys_tick();
#endif

    ui4_total_time += (ui4_end_time-ui4_start_time);
    ui4_total_bytes += ui4_data;


    t_state = x_crit_start();

    i4_res = handle_get_type_obj(h_request, & e_type, (VOID*) & pt_request);

    b_valid =
        (i4_res == HR_OK) &&
        (e_type == PHT_DATA_REQUEST) &&
        (pt_request != NULL);

    if (b_valid)
    {
        h_ch_conn      = pt_request->h_ch_conn;
        h_buffer_agent = pt_request->h_buffer_agent;

        pt_request->b_done = TRUE;

        b_aborted    = pt_request->b_aborted;
        b_flush_done = pt_request->b_flush_done;
    }

    x_crit_end(t_state);

    if (b_valid)
    {
        if (b_aborted)
        {
            DBG_INFO((DBG_PREFIX
                      "FM notification %d, but request was aborted (%d)\n",
                      e_nfy_cond, pt_request->h_ch_conn));

            if ((e_nfy_cond == FM_ASYNC_COND_READ_OK)
                ||
                (e_nfy_cond == FM_ASYNC_COND_ABORT_OK)
                ||
                (e_nfy_cond == FM_ASYNC_COND_FAIL)
                ||
                (e_nfy_cond == FM_ASYNC_COND_EOF)
                ||
                (e_nfy_cond == FM_ASYNC_COND_DLNA_NO_RESPONSE))
            {
                playback_handler_release_data_request(pt_request);

                if (b_flush_done)
                {
                    cm_send_msg_no_retry(h_playback_handler_msgq,
                                         PLAYBACK_HANDLER_MSG_FLUSH_DONE,
                                         0, (UINT32) h_buffer_agent,
                                         h_ch_conn);
                }
            }
        }
        else
        {
            UINT32  ui4_code;

            switch (e_nfy_cond)
            {
                case FM_ASYNC_COND_READ_OK:
                {
                    ui4_code = PLAYBACK_HANDLER_MSG_ACK_DATA_TRANSFER_OK;
                }
                break;

                case FM_ASYNC_COND_FAIL:
                case FM_ASYNC_COND_READ_TIMEOUT:
                {
                    ui4_code = PLAYBACK_HANDLER_MSG_ACK_DATA_TRANSFER_FAILED;
                }
                break;

                case FM_ASYNC_COND_EOF:
                case FM_ASYNC_COND_DLNA_NO_RESPONSE:
                {
                    ui4_code = PLAYBACK_HANDLER_MSG_ACK_DATA_TRANSFER_EOF;
                }
                break;

                case FM_ASYNC_COND_WRITE_OK:
                case FM_ASYNC_COND_ABORT_OK:
                case FM_ASYNC_COND_ABORT_FAIL:
                default:
                {
                    ui4_code = PLAYBACK_HANDLER_MSG_NONE;
                }
                break;
            }

            if (ui4_code != PLAYBACK_HANDLER_MSG_NONE)
            {
                cm_send_msg_no_retry(h_playback_handler_msgq,
                                     ui4_code, (UINT32) pt_request, ui4_data,
                                     h_ch_conn);
            }
            else
            {
                DBG_INFO((DBG_PREFIX"Ignored FM condition %d (%d)\n",
                          e_nfy_cond, h_ch_conn));
            }
        }
    }
}
#endif
/*-----------------------------------------------------------------------------
 * Name: playback_conn_transfer_data_from_pull
 *
 * Description: This API reads data from a file to fulfil a data request. A
 *              first attempt is made using an asynchronous read. If it fails
 *              then a synchronous read is attempted.
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_request  References the data request.
 *          pt_item     References the playback item (file).
 *          z_size      Specifies the buffer size (as opposed to the request
 *                      size).
 *
 * Outputs: pz_read  Contains the number of bytes read (if known).
 *          pb_eof   Specifies whether end of file was reached.
 *          pb_skip  Specifies whether the item should be skipped (if error).
 *
 * Returns: CMR_OK         The transfer has been done synchronously.
 *          CMR_ASYNC_NFY  The transfer request was accepted and will be
                           processed asynchronously.
 *          CMR_FAIL       The transfer failed.
 ----------------------------------------------------------------------------*/
static INT32
playback_conn_transfer_data_from_pull (PLAYBACK_CONN_T*          pt_conn,
                                       PLAYBACK_BUF_AGT_T*       pt_buf_agt,
                                       PLAYBACK_DATA_REQUEST_T*  pt_request,
                                       PLAYBACK_ITEM_T*          pt_item,
                                       SIZE_T                    z_size,
                                       SIZE_T*                   pz_read,
                                       BOOL*                     pb_eof,
                                       BOOL*                     pb_skip)
{
    UINT32  ui4_block_size;
    UINT32  ui4_alignment;
    INT32   i4_res;

    if(pt_item == NULL)
    {
        return CMR_FAIL;
    }
    
    ui4_block_size = pt_item->u.t_pull.ui4_block_size;
    ui4_alignment  = pt_item->u.t_pull.ui4_alignment;

    if (((ui4_block_size == 0)
         ||
         ((z_size % ui4_block_size) == 0))
        &&
        ((ui4_alignment == 0)
         ||
         (((UINT32)pt_request->pv_buffer % ui4_alignment) == 0)))
    {
        DBG_INFO((DBG_PREFIX
                  "Copying %u byte%s from file (id:%u) to 0x%08x (%d)\n",
                  pt_request->z_size,
                  (pt_request->z_size > 1) ? "s" : "",
                  pt_item->ui4_id,
                  pt_request->pv_buffer,
                  pt_conn->h_ch_conn));

#ifdef LINUX_TURNKEY_SOLUTION
        if (NULL == pt_buf_agt->pv_shared_private)
        {
            BUF_AGT_BUFFER_SHARED_MEMORY_T      t_bagt_shared_mem;
            SIZE_T                              z_shared_size;
            x_memset(&t_bagt_shared_mem, 0, sizeof(BUF_AGT_BUFFER_SHARED_MEMORY_T));
            z_shared_size = sizeof(BUF_AGT_BUFFER_SHARED_MEMORY_T);
            rm_get(pt_buf_agt->h_rm_buffer_agent,
                   FALSE,
                   ANY_PORT_NUM,
                   BUF_AGT_GET_TYPE_SHARED_MEMORY,
                   (VOID*) &t_bagt_shared_mem,
                   &z_shared_size,
                   NULL);

            pt_buf_agt->pv_shared_buffer = t_bagt_shared_mem.pv_buffer;
            pt_buf_agt->pv_shared_private = t_bagt_shared_mem.pv_private;
            pt_buf_agt->z_shared_buffer_size = t_bagt_shared_mem.z_buffer_size;

            if (pt_item->u.t_pull.t_pull_info.pf_associate)
            {
                pt_item->u.t_pull.t_pull_info.pf_associate(
                    pt_item->u.t_pull.h_pull,
                    pt_item->u.t_pull.t_pull_info.pv_app_tag,
                    (UINT32)pt_buf_agt->pv_shared_private,
                    (UINT32)pt_buf_agt->z_shared_buffer_size
                    );
            }
        }

        if (pt_item->u.t_pull.t_pull_info.pf_associate)
        {
            UINT32  ui4_phy_addr;
            UINT32  ui4_virt_addr;
            UINT32  ui4_fbm_size;
            UINT32  ui4_addr;

            x_get_fbm_info(&ui4_phy_addr, &ui4_virt_addr, &ui4_fbm_size);

            ui4_addr = ui4_phy_addr+((UINT32)pt_request->pv_buffer - ui4_virt_addr);
            if (b_log_cmpb)
            {
                x_dbg_stmt("[CMPB] before pf_read_async\n");
            }

            i4_res = pt_item->u.t_pull.t_pull_info.pf_read_async(
                pt_item->u.t_pull.h_pull,
                pt_item->u.t_pull.t_pull_info.pv_app_tag,
                (UINT8*)ui4_addr,
                (UINT32)z_size,
                playback_conn_pull_nfy,
                (VOID*) pt_request->h_request,
                & pt_request->h_fm_req);

            if (b_log_cmpb)
            {
                x_dbg_stmt("[CMPB] after pf_read_async %d\n", (UINT32)z_size);
            }

        }
        else
#endif
        {
            i4_res = pt_item->u.t_pull.t_pull_info.pf_read_async(
                pt_item->u.t_pull.h_pull,
                pt_item->u.t_pull.t_pull_info.pv_app_tag,
                (UINT8*)pt_request->pv_buffer,
                (UINT32)z_size,
                playback_conn_pull_nfy,
                (VOID*) pt_request->h_request,
                & pt_request->h_fm_req);
        }

        if (i4_res >= CMR_OK)
        {
            i4_res = CMR_ASYNC_NFY;
        }
        else if (i4_res == CMR_EOS)
        {
            *pb_eof  = TRUE;
            *pz_read = 0;
            i4_res   = CMR_OK;
        }
        else
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not read asynchronously (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));

            i4_res = CMR_FAIL;
        }

        if (i4_res != CMR_FAIL)
        {
        }
        else
        {
            if (pt_conn->t_connect_info.b_disc_if_file_error)
            {
                cm_send_msg_retry(h_playback_handler_msgq,
                                  PLAYBACK_HANDLER_MSG_DISCONNECT_REQUEST,
                                  PLAYBACK_DISC_FILE_ERROR, 0,
                                  pt_conn->h_ch_conn);
            }
            else
            {
                cm_nfy(pt_conn->h_cm_conn,
                       CM_PLAYBACK_CONN_INFO_FILE_ERROR,
                       pt_item ? pt_item->ui4_id : 0);
            }
        }
    }
    else
    {
        if ((ui4_block_size != 0) && ((z_size % ui4_block_size) != 0))
        {
            DBG_INFO((DBG_PREFIX
                      "Buffer size incompatibility: %u %% %u != 0 (%d)\n",
                      z_size,
                      ui4_block_size,
                      pt_conn->h_ch_conn));
        }

        if ((ui4_alignment != 0) &&
            (((UINT32)pt_request->pv_buffer % ui4_alignment) != 0))
        {
            DBG_INFO((DBG_PREFIX
                      "Address(%p) should be aligned on %u bytes (%d)\n",
                      pt_request->pv_buffer,
                      ui4_alignment,
                      pt_conn->h_ch_conn));
        }

        playback_conn_nfy_align_info(pt_buf_agt, pt_item);

        *pb_skip = FALSE;

        i4_res = CMR_FAIL;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_transfer_data_from_file
 *
 * Description: This API reads data from a file to fulfil a data request. A
 *              first attempt is made using an asynchronous read. If it fails
 *              then a synchronous read is attempted.
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_request  References the data request.
 *          pt_item     References the playback item (file).
 *          z_size      Specifies the buffer size (as opposed to the request
 *                      size).
 *
 * Outputs: pz_read  Contains the number of bytes read (if known).
 *          pb_eof   Specifies whether end of file was reached.
 *          pb_skip  Specifies whether the item should be skipped (if error).
 *
 * Returns: CMR_OK         The transfer has been done synchronously.
 *          CMR_ASYNC_NFY  The transfer request was accepted and will be
                           processed asynchronously.
 *          CMR_FAIL       The transfer failed.
 ----------------------------------------------------------------------------*/
static INT32
playback_conn_transfer_data_from_file (PLAYBACK_CONN_T*          pt_conn,
                                       PLAYBACK_BUF_AGT_T*       pt_buf_agt,
                                       PLAYBACK_DATA_REQUEST_T*  pt_request,
                                       PLAYBACK_ITEM_T*          pt_item,
                                       SIZE_T                    z_size,
                                       SIZE_T*                   pz_read,
                                       BOOL*                     pb_eof,
                                       BOOL*                     pb_skip)
{
    UINT32  ui4_block_size;
    UINT32  ui4_alignment;
    INT32   i4_res;
#if PLAYBACK_HANDLER_USE_ASYNC_FM_READ
    struct tms t_tms;
#endif

    if(pt_item == NULL)
    {
        return CMR_FAIL;
    }
    
    ui4_block_size = pt_item->u.t_file.ui4_block_size;
    ui4_alignment  = pt_item->u.t_file.ui4_alignment;

    if (((ui4_block_size == 0)
         ||
         ((z_size % ui4_block_size) == 0))
        &&
        ((ui4_alignment == 0)
         ||
         (((UINT32)pt_request->pv_buffer % ui4_alignment) == 0)))
    {
        DBG_INFO((DBG_PREFIX
                  "Copying %u byte%s from file (id:%u) to 0x%08x (%d)\n",
                  pt_request->z_size,
                  (pt_request->z_size > 1) ? "s" : "",
                  pt_item->ui4_id,
                  pt_request->pv_buffer,
                  pt_conn->h_ch_conn));

#if PLAYBACK_HANDLER_USE_ASYNC_FM_READ
#ifdef LINUX_TURNKEY_SOLUTION
        ui4_start_time = times(&t_tms);
#else
        ui4_start_time = x_os_get_sys_tick();
#endif
        i4_res = x_fm_mfw_read_async(pt_item->u.t_file.h_file,
                                     pt_request->pv_buffer,
                                     z_size,
                                     pt_item->u.t_file.ui1_priority,
                                     playback_conn_fm_nfy,
                                     (VOID*) pt_request->h_request,
                                     & pt_request->h_fm_req);

        if (i4_res >= FMR_OK)
        {
            i4_res = CMR_ASYNC_NFY;
        }
        else if (i4_res == FMR_EOF)
        {
            *pb_eof  = TRUE;
            *pz_read = 0;
            i4_res   = CMR_OK;
        }
        else
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not read asynchronously (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));

            i4_res = CMR_FAIL;
        }
#else
        /* this is to pretend the asynchronous read failed */
        i4_res = CMR_FAIL;

        if (i4_res == CMR_FAIL)
        {
            UINT32  ui4_read = 0;

            i4_res = x_fm_mfw_read(pt_item->u.t_file.h_file,
                                   pt_request->pv_buffer,
                                   z_size,
                                   & ui4_read);

            if (i4_res == FMR_OK)
            {
                *pz_read = ui4_read;
                i4_res   = CMR_OK;
            }
            else if (i4_res == FMR_EOF)
            {
                *pb_eof  = TRUE;
                *pz_read = 0;
                i4_res   = CMR_OK;
            }
            else
            {
                DBG_ERROR((DBG_PREFIX
                           "Could not read synchronously (%d, res=%d)\n",
                           pt_conn->h_ch_conn, i4_res));

                i4_res = CMR_FAIL;
            }
        }
#endif

        if (i4_res != CMR_FAIL)
        {
            if (! *pb_eof)
            {
                INT32  i4_fm_res;
                BOOL   b_eof;

                b_eof = FALSE;

                i4_fm_res = x_fm_mfw_feof(pt_item->u.t_file.h_file, & b_eof);

                if ((i4_fm_res == FMR_OK) && b_eof)
                {
                    DBG_INFO((DBG_PREFIX"Reached item %u's end of file (%d)\n",
                              pt_item->ui4_id,
                              pt_conn->h_ch_conn));

                    *pb_eof = TRUE;
                }
            }
        }
        else
        {
            if (pt_conn->t_connect_info.b_disc_if_file_error)
            {
                cm_send_msg_retry(h_playback_handler_msgq,
                                  PLAYBACK_HANDLER_MSG_DISCONNECT_REQUEST,
                                  PLAYBACK_DISC_FILE_ERROR, 0,
                                  pt_conn->h_ch_conn);
            }
            else
            {
                cm_nfy(pt_conn->h_cm_conn,
                       CM_PLAYBACK_CONN_INFO_FILE_ERROR,
                       pt_item->ui4_id);
            }
        }
    }
    else
    {
        if ((ui4_block_size != 0) && ((z_size % ui4_block_size) != 0))
        {
            DBG_INFO((DBG_PREFIX
                      "Buffer size incompatibility: %u %% %u != 0 (%d)\n",
                      z_size,
                      ui4_block_size,
                      pt_conn->h_ch_conn));
        }

        if ((ui4_alignment != 0) &&
            (((UINT32)pt_request->pv_buffer % ui4_alignment) != 0))
        {
            DBG_INFO((DBG_PREFIX
                      "Address(%p) should be aligned on %u bytes (%d)\n",
                      pt_request->pv_buffer,
                      ui4_alignment,
                      pt_conn->h_ch_conn));
        }

        playback_conn_nfy_align_info(pt_buf_agt, pt_item);

        *pb_skip = FALSE;

        i4_res = CMR_FAIL;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_transfer_data_from_uri
 *
 * Description: This API reads data from a URI to fulfil a data request. A
 *              first attempt is made using an asynchronous read. If it fails
 *              then a synchronous read is attempted.
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_request  References the data request.
 *          pt_item     References the playback item (file).
 *          z_size      Specifies the buffer size (as opposed to the request
 *                      size).
 *
 * Outputs: pz_read  Contains the number of bytes read (if known).
 *          pb_eof   Specifies whether end of file was reached.
 *          pb_skip  Specifies whether the item should be skipped (if error).
 *
 * Returns: CMR_OK         The transfer has been done synchronously.
 *          CMR_ASYNC_NFY  The transfer request was accepted and will be
                           processed asynchronously.
 *          CMR_FAIL       The transfer failed.
 ----------------------------------------------------------------------------*/
static INT32
playback_conn_transfer_data_from_uri  (PLAYBACK_CONN_T*          pt_conn,
                                       PLAYBACK_BUF_AGT_T*       pt_buf_agt,
                                       PLAYBACK_DATA_REQUEST_T*  pt_request,
                                       PLAYBACK_ITEM_T*          pt_item,
                                       SIZE_T                    z_size,
                                       SIZE_T*                   pz_read,
                                       BOOL*                     pb_eof,
                                       BOOL*                     pb_skip)
{
    UINT32  ui4_block_size;
    UINT32  ui4_alignment;
    INT32   i4_res = 0;

    if(pt_item == NULL)
    {
        return CMR_FAIL;
    }
    
    ui4_block_size = pt_item->u.t_file.ui4_block_size;
    ui4_alignment  = pt_item->u.t_file.ui4_alignment;

    if (((ui4_block_size == 0)
         ||
         ((z_size % ui4_block_size) == 0))
        &&
        ((ui4_alignment == 0)
         ||
         (((UINT32)pt_request->pv_buffer % ui4_alignment) == 0)))
    {
        DBG_INFO((DBG_PREFIX
                  "Copying %u byte%s from file (id:%u) to 0x%08x (%d)\n",
                  pt_request->z_size,
                  (pt_request->z_size > 1) ? "s" : "",
                  pt_item->ui4_id,
                  pt_request->pv_buffer,
                  pt_conn->h_ch_conn));

        if (pt_item->u.t_uri.b_url)
        {
#ifdef INET_SUPPORT
            i4_res = x_httpc_fm_read_async(pt_item->u.t_uri.h_uri,
                                           pt_request->pv_buffer,
                                           z_size,
                                           pt_item->u.t_uri.ui1_priority,
                                           playback_conn_fm_nfy,
                                           (VOID*) pt_request->h_request,
                                           & pt_request->h_fm_req);
            if (b_log_cmpb)
            {
                x_dbg_stmt("[CMPB] playback_conn_transfer_data_from_uri x_httpc_fm_read_async %d\n", i4_res);
            }
#endif
        }
        else
        {
            i4_res = x_fm_mfw_read_async(pt_item->u.t_uri.h_uri,
                                         pt_request->pv_buffer,
                                         z_size,
                                         pt_item->u.t_uri.ui1_priority,
                                         playback_conn_fm_nfy,
                                         (VOID*) pt_request->h_request,
                                         & pt_request->h_fm_req);
        }

        if (i4_res >= FMR_OK)
        {
            i4_res = CMR_ASYNC_NFY;
        }
        else if (i4_res == FMR_EOF)
        {
            *pb_eof  = TRUE;
            *pz_read = 0;
            i4_res   = CMR_OK;
        }
        else
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not read asynchronously (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));

            i4_res = CMR_FAIL;
        }

        if (i4_res != CMR_FAIL)
        {
            if (! *pb_eof)
            {
                INT32  i4_fm_res = 0;
                BOOL   b_eof;

                b_eof = FALSE;

                if (pt_item->u.t_uri.b_url)
                {
#ifdef INET_SUPPORT
                    i4_fm_res = x_httpc_fm_feof(pt_item->u.t_uri.h_uri, & b_eof);
#endif
                }
                else
                {
                    i4_fm_res = x_fm_mfw_feof(pt_item->u.t_uri.h_uri, & b_eof);
                }

                if ((i4_fm_res == FMR_OK) && b_eof)
                {
                    DBG_INFO((DBG_PREFIX"Reached item %u's end of file (%d)\n",
                              pt_item->ui4_id,
                              pt_conn->h_ch_conn));

                    *pb_eof = TRUE;
                }
            }
        }
        else
        {
            if (pt_conn->t_connect_info.b_disc_if_file_error)
            {
                cm_send_msg_retry(h_playback_handler_msgq,
                                  PLAYBACK_HANDLER_MSG_DISCONNECT_REQUEST,
                                  PLAYBACK_DISC_FILE_ERROR, 0,
                                  pt_conn->h_ch_conn);
            }
            else
            {
                cm_nfy(pt_conn->h_cm_conn,
                       CM_PLAYBACK_CONN_INFO_FILE_ERROR,
                       pt_item ? pt_item->ui4_id : 0);
            }
        }
    }
    else
    {
        if ((ui4_block_size != 0) && ((z_size % ui4_block_size) != 0))
        {
            DBG_INFO((DBG_PREFIX
                      "Buffer size incompatibility: %u %% %u != 0 (%d)\n",
                      z_size,
                      ui4_block_size,
                      pt_conn->h_ch_conn));
        }

        if ((ui4_alignment != 0) &&
            (((UINT32)pt_request->pv_buffer % ui4_alignment) != 0))
        {
            DBG_INFO((DBG_PREFIX
                      "Address(%p) should be aligned on %u bytes (%d)\n",
                      pt_request->pv_buffer,
                      ui4_alignment,
                      pt_conn->h_ch_conn));
        }

        playback_conn_nfy_align_info(pt_buf_agt, pt_item);

        *pb_skip = FALSE;

        i4_res = CMR_FAIL;
    }

    return i4_res;
}


/*-----------------------------------------------------------------------------
 * Name: playback_conn_transfer_data_from_buffer
 *
 * Description: This API reads data from a buffer to fulfil a data request.
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_request  References the data request.
 *          pt_item     References the playback item (buffer).
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The transfer has been done synchronously.
 ----------------------------------------------------------------------------*/
static INT32
playback_conn_transfer_data_from_buffer (PLAYBACK_CONN_T*          pt_conn,
                                         PLAYBACK_BUF_AGT_T*       pt_buf_agt,
                                         PLAYBACK_DATA_REQUEST_T*  pt_request,
                                         PLAYBACK_ITEM_T*          pt_item,
                                         SIZE_T*                   pz_read)
{
    VOID*  pv_source;

    pv_source = (VOID *)
        ((CHAR *) pt_item->u.t_buffer.pv_buffer + pt_item->ui8_position);

    DBG_INFO((DBG_PREFIX
              "Copying %u byte%s from 0x%08x (id:%u) to 0x%08x (%d)\n",
              pt_request->z_size,
              (pt_request->z_size > 1) ? "s" : "",
              pv_source,
              pt_item->ui4_id,
              pt_request->pv_buffer,
              pt_conn->h_ch_conn));

    if (pt_buf_agt->b_shared_memory)
    {
        pt_request->ui4_buf_agt_flags |= BUF_AGT_SHARED_MEMORY;
        pt_request->pv_buffer = pt_item->u.t_buffer.pv_buffer;
        pt_request->z_size = (SIZE_T)pt_item->u.t_buffer.ui8_size;
        *pz_read = pt_request->z_size;
    }
    else
    {
        x_memcpy(pt_request->pv_buffer, pv_source, pt_request->z_size);
    }

    return CMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_queue_unconsumed_request
 *
 * Description: This API adds a data request to the list of unconsumed data
 *              requests.
 *
 * Inputs:  pt_buf_agt  References the buffer agent structure.
 *          pt_request  References the data request to queue.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_conn_queue_unconsumed_request (PLAYBACK_BUF_AGT_T*       pt_buf_agt,
                                        PLAYBACK_DATA_REQUEST_T*  pt_request)
{
    PLAYBACK_DATA_REQUEST_T**  ppt_tail;

    ppt_tail = & pt_buf_agt->pt_unconsumed_requests;

    while (*ppt_tail != NULL)
    {
        ppt_tail = & (*ppt_tail)->pt_next;
    }

    *ppt_tail = pt_request;
    pt_request->pt_next = NULL;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_remove_item_from_list
 *
 * Description: This API removes a playback item from the list (and updates the
 *              head and tail accordingly).
 *
 * Inputs:  pt_buf_agt  References the buffer agent structure.
 *          pt_item     References the playback item to remove.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_conn_remove_item_from_list (PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                     PLAYBACK_ITEM_T*     pt_item)
{
    PLAYBACK_ITEM_T**  ppt_item;
    PLAYBACK_ITEM_T*   pt_previous_item;

    ppt_item         = & pt_buf_agt->pt_items_head;
    pt_previous_item = NULL;

    while (*ppt_item != NULL)
    {
        if (*ppt_item == pt_item)
        {
            *ppt_item = pt_item->pt_next;

            if (pt_item == pt_buf_agt->pt_items_tail)
            {
                pt_buf_agt->pt_items_tail = pt_previous_item;
            }

            break;
        }

        pt_previous_item = *ppt_item;
        ppt_item = & (*ppt_item)->pt_next;
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_mark_item_as_transferred
 *
 * Description: This API marks a playback item as transferred by updating the
 *              list of unconsumed requests. It finds the last request for this
 *              item in the list and sets its b_last value to TRUE. If no
 *              request is found, then the item is removed from the list and
 *              released.
 *
 * Inputs:  pt_conn  References the connection.
 *          pt_item  References the playback item.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_conn_mark_item_as_transferred (PLAYBACK_CONN_T*     pt_conn,
                                        PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                        PLAYBACK_ITEM_T*     pt_item)
{
    if ((pt_item->ui4_flags & CM_PLAYBACK_FLAG_KEEP_UNTIL_UNQUEUED) == 0)
    {
        PLAYBACK_DATA_REQUEST_T*  pt_request;

        pt_request = pt_buf_agt->pt_unconsumed_requests;

        while (pt_request != NULL)
        {
            if (pt_request->pt_item == pt_item)
            {
                while ((pt_request->pt_next != NULL)
                       &&
                       (pt_request->pt_next->pt_item == pt_item))
                {
                    pt_request = pt_request->pt_next;
                }

                DBG_INFO((DBG_PREFIX
                          "Request %u marked as last one for item %u (%d)\n",
                          pt_request->ui4_req_id, pt_item->ui4_id));

                pt_request->b_last = TRUE;

                cm_nfy(pt_conn->h_cm_conn,
                       CM_PLAYBACK_CONN_INFO_ITEM_TRANSFER_END,
                       pt_item->ui4_id);

                break;
            }

            pt_request = pt_request->pt_next;
        }

        if (pt_request == NULL)
        {
            if (pt_item->e_format != DATA_FMT_IBC)
            {
                cm_nfy(pt_conn->h_cm_conn,
                       CM_PLAYBACK_CONN_INFO_ITEM_CONSUMED,
                       pt_item->ui4_id);
            }

            playback_conn_remove_item_from_list(pt_buf_agt, pt_item);

            playback_handler_release_item(pt_item);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_set_item_position
 *
 * Description: This API sets the position of a given playback item.
 *
 * Inputs:  pt_conn       References the connection.
 *          pt_item       References the playback item.
 *          ui8_position  Contains the new position.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_conn_set_item_position (PLAYBACK_CONN_T*  pt_conn,
                                 PLAYBACK_ITEM_T*  pt_item,
                                 UINT64            ui8_position)
{
    pt_item->ui8_position = ui8_position;
    if (pt_item->e_type == PLAYBACK_ITEM_TYPE_PULL)
    {
        UINT32  ui4_seek_alignment;
        INT32   i4_res;
        UINT64  ui8_cur_pos = 0;

        ui4_seek_alignment = pt_item->u.t_pull.ui4_block_size;

        pt_item->ui4_offset = (ui4_seek_alignment != 0) ?
            (UINT32) (ui8_position % ui4_seek_alignment) : 0;

        pt_item->ui8_position -= pt_item->ui4_offset;

        DBG_INFO((DBG_PREFIX
                  "Seek to position %llu (+%u) in file (id:%u) (%d)\n",
                  pt_item->ui8_position, pt_item->ui4_offset,
                  pt_item->ui4_id, pt_conn->h_ch_conn));

        i4_res = pt_item->u.t_pull.t_pull_info.pf_byteseek(
                                pt_item->u.t_pull.h_pull,
                                pt_item->u.t_pull.t_pull_info.pv_app_tag,
                                pt_item->ui8_position,
                                FM_SEEK_BGN,
                                &ui8_cur_pos);

        if (i4_res != CMR_OK)
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not seek to position %llu (%d, res=%d)\n",
                       pt_item->ui8_position, pt_conn->h_ch_conn, i4_res));
        }

    }
    else if (pt_item->e_type == PLAYBACK_ITEM_TYPE_FILE)
    {
        UINT64  ui8_current_pos;
        UINT32  ui4_seek_alignment;
        INT32   i4_res;

        ui4_seek_alignment = pt_item->u.t_file.ui4_block_size;

        pt_item->ui4_offset = (ui4_seek_alignment != 0) ?
            (UINT32) (ui8_position % ui4_seek_alignment) : 0;

        pt_item->ui8_position -= pt_item->ui4_offset;

        DBG_INFO((DBG_PREFIX
                  "Seek to position %llu (+%u) in file (id:%u) (%d)\n",
                  pt_item->ui8_position, pt_item->ui4_offset,
                  pt_item->ui4_id, pt_conn->h_ch_conn));

        i4_res = x_fm_mfw_lseek(pt_item->u.t_file.h_file,
                                (INT64) pt_item->ui8_position,
                                FM_SEEK_BGN,
                                & ui8_current_pos);

        if (i4_res != FMR_OK)
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not seek to position %llu (%d, res=%d)\n",
                       pt_item->ui8_position, pt_conn->h_ch_conn, i4_res));
        }
    }
    else if (pt_item->e_type == PLAYBACK_ITEM_TYPE_URI)
    {
        UINT64  ui8_current_pos;
        UINT32  ui4_seek_alignment;
        INT32   i4_res = 0;

        ui4_seek_alignment = pt_item->u.t_file.ui4_block_size;

        pt_item->ui4_offset = (ui4_seek_alignment != 0) ?
            (UINT32) (ui8_position % ui4_seek_alignment) : 0;

        pt_item->ui8_position -= pt_item->ui4_offset;

        DBG_INFO((DBG_PREFIX
                  "Seek to position %llu (+%u) in file (id:%u) (%d)\n",
                  pt_item->ui8_position, pt_item->ui4_offset,
                  pt_item->ui4_id, pt_conn->h_ch_conn));

#ifdef ENABLE_MMS_SUPPORT
        if (pt_item->u.t_uri.b_mms)
        {
            i4_res = FMR_OK;
        }
        else 
#endif
		if (pt_item->u.t_uri.b_url)
        {
#ifdef INET_SUPPORT
            i4_res = x_httpc_fm_lseek(pt_item->u.t_uri.h_uri,
                                      (INT64) pt_item->ui8_position,
                                      FM_SEEK_BGN,
                                      & ui8_current_pos);
#endif
        }
        else
        {
            i4_res = x_fm_mfw_lseek(pt_item->u.t_uri.h_uri,
                                    (INT64) pt_item->ui8_position,
                                    FM_SEEK_BGN,
                                    & ui8_current_pos);
        }

        if (i4_res != FMR_OK)
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not seek to position %llu (%d, res=%d)\n",
                       pt_item->ui8_position, pt_conn->h_ch_conn, i4_res));
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_transfer_data
 *
 * Description: This API reads data from a file or a buffer to fulfil a data
 *              request. The first (oldest) data request in the list is removed
 *              and processed, using the first (oldest) playback item (file or
 *              buffer). The size of the data request may be modified in the
 *              event the remaining data in the playback item is less than the
 *              request size.
 *
 * Inputs:  pt_conn     References the connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK         The transfer has been done synchronously.
 *          CMR_ASYNC_NFY  The transfer request was accepted and will be
                           processed asynchronously.
 *          CMR_FAIL       The transfer failed.
 ----------------------------------------------------------------------------*/
static INT32 playback_conn_transfer_data (PLAYBACK_CONN_T*     pt_conn,
                                          PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    PLAYBACK_ITEM_T*  pt_item;
    INT32             i4_res;

    i4_res = CMR_FAIL;

    if (NULL == pt_buf_agt)
    {
        return i4_res;
    }
    if ((pt_buf_agt->pt_items_head == NULL) && (pt_buf_agt->pt_items_tail == NULL))
    {
        return i4_res;
    }

    if (! pt_conn->b_disconnecting
        &&
        (pt_buf_agt->pt_requests_head != NULL)
        &&
        ((pt_item = pt_buf_agt->pt_transferring_item) != NULL)
        &&
        ((pt_buf_agt->b_shared_memory)||(!pt_buf_agt->b_quick_consume)||(pt_buf_agt->pt_unconsumed_requests == NULL))
        &&
        ((pt_item->e_state != PLAYBACK_ITEM_STATE_QUEUED)
         ||
         ((pt_item->ui4_flags & CM_PLAYBACK_FLAG_TRANSFER_WHEN_CONSUMED) == 0)
         ||
         (pt_buf_agt->pt_unconsumed_requests == NULL)))
    {
        PLAYBACK_DATA_REQUEST_T*  pt_request;
        PLAYBACK_ITEM_T*          pt_next;
        UINT64                    ui8_actual_position;
        UINT64                    ui8_left;
        SIZE_T                    z_size;
        BOOL                      b_eof;
        BOOL                      b_skip;
        BOOL                      b_keep_until_unqueued;

        /* retrieve request (list is not updated yet) */
        pt_request = pt_buf_agt->pt_requests_head;

        /* retrieve next playback item */
        pt_next = pt_item->pt_next;

        /* we may have to change the position in the item */
        if (((pt_item->ui4_flags & CM_PLAYBACK_FLAG_CANNOT_SET_OFFSET) == 0)
            &&
            (pt_request->ui4_buf_agt_flags & BUF_AGT_SET_DATA_OFFSET))
        {
            playback_conn_set_item_position(pt_conn,
                                            pt_item,
                                            pt_request->ui8_transfer_position);
        }

        /* compute transfer size */
        ui8_actual_position = (pt_item->ui8_position + pt_item->ui4_offset);

        ui8_left = (pt_item->ui8_end_offset > ui8_actual_position) ?
            (pt_item->ui8_end_offset - pt_item->ui8_position) : 0;

        z_size = pt_request->z_size;
        b_eof  = (ui8_left <= (UINT64) pt_request->z_size);
        b_skip = TRUE;

        b_keep_until_unqueued =
            ((pt_item->ui4_flags & CM_PLAYBACK_FLAG_KEEP_UNTIL_UNQUEUED) != 0);

        if (ui8_left < (UINT64) pt_request->z_size)
        {
            DBG_INFO((DBG_PREFIX
                      "Data request changed to %u bytes (from %u) (%d)\n",
                      (SIZE_T) ui8_left,
                      pt_request->z_size,
                      pt_conn->h_ch_conn));

            pt_request->z_size = (SIZE_T) ui8_left;
        }

        if ((pt_request->z_size != 0) || (pt_buf_agt->b_shared_memory) )
        {
            SIZE_T  z_read;
            UINT32  ui4_offset;

            /* request can be removed from list */
            pt_buf_agt->pt_requests_head =
                pt_buf_agt->pt_requests_head->pt_next;

            if (pt_buf_agt->pt_requests_head == NULL)
            {
                pt_buf_agt->pt_requests_tail = NULL;
            }

            z_read = pt_request->z_size;

            if (pt_buf_agt->b_nfy_transfer_start)
            {
                pt_item->e_state = PLAYBACK_ITEM_STATE_TRANSFER;

                cm_nfy(pt_conn->h_cm_conn,
                       CM_PLAYBACK_CONN_INFO_ITEM_TRANSFER_START,
                       pt_item->ui4_id);

                pt_buf_agt->b_nfy_transfer_start = FALSE;
            }

            DBG_INFO((DBG_PREFIX
                      "Data transfer (req=%u) from offset %llu (%d)\n",
                      pt_request->ui4_req_id,
                      pt_item->ui8_position,
                      pt_conn->h_ch_conn));

            switch (pt_item->e_type)
            {
                case PLAYBACK_ITEM_TYPE_PULL:
                {
                    i4_res =
                        playback_conn_transfer_data_from_pull(pt_conn,
                                                              pt_buf_agt,
                                                              pt_request,
                                                              pt_item,
                                                              z_size,
                                                              & z_read,
                                                              & b_eof,
                                                              & b_skip);
                }
                break;

                case PLAYBACK_ITEM_TYPE_FILE:
                {
                    i4_res =
                        playback_conn_transfer_data_from_file(pt_conn,
                                                              pt_buf_agt,
                                                              pt_request,
                                                              pt_item,
                                                              z_size,
                                                              & z_read,
                                                              & b_eof,
                                                              & b_skip);
                }
                break;

                case PLAYBACK_ITEM_TYPE_BUFFER:
                {
                    i4_res =
                        playback_conn_transfer_data_from_buffer(pt_conn,
                                                                pt_buf_agt,
                                                                pt_request,
                                                                pt_item,
                                                                & z_read);
                }
                break;

                case PLAYBACK_ITEM_TYPE_URI:
                {
                    i4_res =
                        playback_conn_transfer_data_from_uri(pt_conn,
                                                             pt_buf_agt,
                                                             pt_request,
                                                             pt_item,
                                                             z_size,
                                                             & z_read,
                                                             & b_eof,
                                                             & b_skip);
                }
                break;

                default:
                {
                    i4_res = CMR_FAIL;
                }
                break;
            }

            switch (pt_item->e_format)
            {
                case DATA_FMT_AUDIO_ES:
                case DATA_FMT_VIDEO_ES:
                {
                    pt_request->t_pts = pt_item->t_extra.t_es_info.t_pts;
                }
                break;

                default:
                    break;
            }

            ui4_offset = pt_item->ui4_offset;

            pt_request->pt_item               = pt_item;
            pt_request->ui8_transfer_position = pt_item->ui8_position;
            pt_request->ui4_offset            = ui4_offset;

            pt_request->b_last = b_eof && ! b_keep_until_unqueued;

            pt_item->ui8_position += z_read;
            pt_item->ui4_offset = 0;

            if (((i4_res == CMR_OK) && (z_read > 0)) || (pt_item->e_format==DATA_FMT_IBC))
            {
                /* synchronous transfer completed */

                DBG_INFO((DBG_PREFIX
                          "Ack data request (id:%u, asked:%u, read:%u): "
                          "DONE (%d)\n",
                          pt_request->ui4_req_id,
                          pt_request->z_size,
                          z_read,
                          pt_conn->h_ch_conn));

                pt_request->z_size = z_read;
                pt_request->b_done = TRUE;

                playback_conn_ack_data_request(pt_conn,
                                               pt_buf_agt,
                                               pt_request,
                                               BUF_AGT_DATA_REQ_ACK_COND_DONE);

                playback_conn_queue_unconsumed_request(pt_buf_agt,
                                                       pt_request);
            }
            else if (i4_res == CMR_ASYNC_NFY)
            {
                playback_conn_queue_unconsumed_request(pt_buf_agt,
                                                       pt_request);
            }
            else
            {
                b_eof = b_skip;

                if (b_skip)
                {
                    playback_conn_mark_item_as_transferred(pt_conn,
                                                           pt_buf_agt,
                                                           pt_item);
                }
                else
                {
                    pt_item->ui8_position -= z_read;
                    pt_item->ui4_offset = ui4_offset;
                }

                DBG_ERROR((DBG_PREFIX
                           "Could not transfer data (%d, res=%d)\n",
                           pt_conn->h_ch_conn, i4_res));

                DBG_INFO((DBG_PREFIX
                          "Ack %u-byte data request %u: ERROR (%d)\n",
                          pt_request->z_size,
                          pt_request->ui4_req_id,
                          pt_conn->h_ch_conn));

                playback_conn_ack_data_request(
                    pt_conn, pt_buf_agt, pt_request,
                    BUF_AGT_DATA_REQ_ACK_COND_ERROR);

                playback_handler_release_data_request(pt_request);
            }
        }
        else
        {
            DBG_INFO((DBG_PREFIX"Zero-byte transfer\n"));

            if (pt_request->ui4_buf_agt_flags & BUF_AGT_SET_DATA_OFFSET)
            {
                /*
                  The request can be removed from list and acknowledged. This
                  is to allow a subsequent request, empty or not, to be
                  processed.
                */

                pt_buf_agt->pt_requests_head =
                    pt_buf_agt->pt_requests_head->pt_next;

                if (pt_buf_agt->pt_requests_head == NULL)
                {
                    pt_buf_agt->pt_requests_tail = NULL;
                }

                pt_request->ui4_offset = 0;

                playback_conn_ack_data_request(
                    pt_conn, pt_buf_agt, pt_request,
                    BUF_AGT_DATA_REQ_ACK_COND_DONE);

                playback_handler_release_data_request(pt_request);
            }
            else
            {
                /*
                  The request stays in the list and will be processed when
                  there is data to read.
                */

                pt_request->z_size = z_size;
            }

            playback_conn_mark_item_as_transferred(pt_conn, pt_buf_agt,
                                                   pt_item);
        }

        b_eof = b_eof && ! b_keep_until_unqueued;

        /* update items list if current item has been fully transferred */
        if (b_eof)
        {
            DBG_INFO((DBG_PREFIX"Done with playback item %u (%d)\n",
                      pt_item->ui4_id, pt_conn->h_ch_conn));

            pt_buf_agt->pt_transferring_item = pt_next;
            pt_buf_agt->b_nfy_transfer_start = TRUE;

            if (pt_buf_agt->pt_transferring_item != NULL)
            {
                playback_conn_nfy_align_info(pt_buf_agt,
                                             pt_buf_agt->pt_transferring_item);

                DBG_INFO((DBG_PREFIX"Next item to transfer: %u (%d)\n",
                          pt_buf_agt->pt_transferring_item->ui4_id,
                          pt_conn->h_ch_conn));
            }
            else
            {
                DBG_INFO((DBG_PREFIX
                          "No next item to transfer for now (%d)\n",
                          pt_conn->h_ch_conn));
            }
        }

        if (pt_buf_agt->pt_transferring_item != NULL)
        {
            BOOL  b_wait;

            b_wait = ((pt_buf_agt->pt_transferring_item->ui4_flags
                       &
                       CM_PLAYBACK_FLAG_TRANSFER_WHEN_CONSUMED) != 0);

            if (b_eof && b_wait)
            {
                DBG_INFO((DBG_PREFIX
                          "Next transfer waiting for consumed item\n"));
            }
            else
            {
                playback_conn_request_data_transfer(pt_conn);
            }
        }
        else
        {
            if (pt_buf_agt->pt_unconsumed_requests == NULL)
            {
                playback_conn_update_scdb(pt_conn, pt_buf_agt, NULL,
                                          pt_item->ui4_flags);
            }

            cm_nfy(pt_conn->h_cm_conn,
                   CM_PLAYBACK_CONN_INFO_QUEUE_EMPTY,
                   0);
        }
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_transfer_circular_data
 *
 * Description: This API reads data from a file or a buffer to fulfil a data
 *              request. The first (oldest) data request in the list is removed
 *              and processed, using the first (oldest) playback item (file or
 *              buffer). The size of the data request may be modified in the
 *              event the remaining data in the playback item is less than the
 *              request size.
 *
 * Inputs:  pt_conn     References the connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK         The transfer has been done synchronously.
 *          CMR_ASYNC_NFY  The transfer request was accepted and will be
                           processed asynchronously.
 *          CMR_FAIL       The transfer failed.
 ----------------------------------------------------------------------------*/
static INT32 playback_conn_transfer_circular_data (PLAYBACK_CONN_T*     pt_conn,
                                                   PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    PLAYBACK_ITEM_T*  pt_item;
    INT32             i4_res;

    i4_res = CMR_FAIL;

    if (pt_buf_agt->e_mode != PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT)
    {
        return i4_res;
    }

    /*check if any request can complete*/
    if (! pt_conn->b_disconnecting
        &&
        (pt_buf_agt->pt_requests_head != NULL)
        &&
        ((pt_item = pt_buf_agt->pt_transferring_item) != NULL)
        &&
        ((pt_item->e_state != PLAYBACK_ITEM_STATE_QUEUED)
         ||
         ((pt_item->ui4_flags & CM_PLAYBACK_FLAG_TRANSFER_WHEN_CONSUMED) == 0)
         ||
         (pt_buf_agt->pt_unconsumed_requests == NULL)))
    {
        PLAYBACK_DATA_REQUEST_T*  pt_request;
        UINT32                    ui4_request_offset;
        UINT32                    ui4_seek_alignment;
        UINT64                    ui8_request_position;
        UINT32                    ui4_circular_pos;
        UINT32                    ui4_fill_size;

        ui4_fill_size = playback_cb_get_filled_size(pt_buf_agt->pt_cb);
        do
        {
            /* retrieve request (list is not updated yet) */
            pt_request = pt_buf_agt->pt_requests_head;

            if (pt_request)
            {

                ui8_request_position = pt_request->ui8_transfer_position;
                ui4_seek_alignment = pt_item->u.t_file.ui4_block_size;
                ui4_request_offset = (ui4_seek_alignment != 0) ?
                    (UINT32) (ui8_request_position % ui4_seek_alignment) : 0;
                ui8_request_position -= ui4_request_offset;


                if (ui8_request_position < pt_item->ui8_position)
                {
                    if (((ui8_request_position + pt_buf_agt->pt_cb->ui4_pre_space) >= pt_item->ui8_position) &&
                        ((ui8_request_position+pt_request->z_size)<=(pt_item->ui8_position+ui4_fill_size)))
                    {
                        if (b_log_cmpb)
                        {
                            x_dbg_stmt("[CMPB] playback_conn_transfer_circular_data search in pre_space %d %d %d %d %d\n",
                                        (UINT32)pt_item->ui8_position,
                                        (UINT32)ui4_fill_size,
                                        (UINT32)ui8_request_position,
                                        (UINT32)pt_request->z_size,
                                        (UINT32)pt_buf_agt->pt_cb->ui4_pre_space);
                        }
                    }
                    else if (((ui8_request_position + pt_buf_agt->pt_cb->ui4_pre_space) >= pt_item->ui8_position) &&
                        ((ui8_request_position+pt_request->z_size) >= pt_item->ui8_end_offset))
                    {
                        pt_request->z_size = (UINT32)(pt_item->ui8_end_offset-ui8_request_position);
                    }
                    else
                    {
                        break;
                    }

                    playback_cb_minus(pt_buf_agt->pt_cb, (pt_item->ui8_position-ui8_request_position));
                    pt_item->ui8_position = ui8_request_position;

                    pt_request->pt_item               = pt_item;
                    pt_request->ui8_transfer_position = ui8_request_position;
                    pt_request->ui4_offset            = ui4_request_offset;


                    if ((ui8_request_position+pt_request->z_size) >= pt_item->ui8_end_offset)
                    {
                        pt_request->b_last = TRUE;
                    }
                    else
                    {
                        pt_request->b_last = FALSE;
                    }

                    pt_request->b_done = TRUE;
                }
                else if ((ui8_request_position+pt_request->z_size)<=(pt_item->ui8_position+ui4_fill_size))
                {
                    pt_request->pt_item               = pt_item;
                    pt_request->ui8_transfer_position = ui8_request_position;
                    pt_request->ui4_offset            = ui4_request_offset;
                    pt_request->b_last = FALSE;
                    pt_request->b_done = TRUE;
                }
                else if ((ui8_request_position+pt_request->z_size) >= pt_item->ui8_end_offset)
                {
                    if (pt_item->ui8_position+ui4_fill_size >= pt_item->ui8_end_offset)
                    {
                        pt_request->pt_item               = pt_item;
                        pt_request->ui8_transfer_position = ui8_request_position;
                        pt_request->ui4_offset            = ui4_request_offset;
                        pt_request->z_size                = (UINT32)(pt_item->ui8_end_offset-ui8_request_position);
                        pt_request->b_last = TRUE;
                        pt_request->b_done = TRUE;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }

                if (b_log_cmpb)
                {
                    x_dbg_stmt("[CMPB] playback_conn_transfer_circular_data %d\n", (UINT32)pt_item->ui8_position);
                }

                ui4_circular_pos = (UINT32)(ui8_request_position-pt_item->ui8_position);
                playback_cb_copy(pt_buf_agt->pt_cb, pt_request->pv_buffer,
                                 (UINT32*)&pt_request->z_size,
                                 ui4_circular_pos);


                pt_buf_agt->pt_requests_head = pt_buf_agt->pt_requests_head->pt_next;
                if (pt_buf_agt->pt_requests_head == NULL)
                {
                    pt_buf_agt->pt_requests_tail = NULL;
                }

                playback_conn_ack_data_request(pt_conn,
                                               pt_buf_agt,
                                               pt_request,
                                               BUF_AGT_DATA_REQ_ACK_COND_DONE);

                playback_conn_queue_unconsumed_request(pt_buf_agt,
                                                       pt_request);


                i4_res = CMR_OK;
            }
        } while(pt_request);
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_cancel_head_request
 *
 * Description: This API cancels the first (oldest) data request in the list.
 *
 * Inputs:  pt_conn             References the connection.
 *          b_nfy_canceled      Specifies if a notification should be sent.
 *          b_request_transfer  Specifies if a new request should be sent.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_conn_cancel_head_request (PLAYBACK_CONN_T*     pt_conn,
                                   PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                   BOOL                 b_nfy_canceled,
                                   BOOL                 b_request_transfer)
{
    if (pt_buf_agt->pt_requests_head != NULL)
    {
        PLAYBACK_DATA_REQUEST_T*  pt_request;

        /* retrieve request and update list */
        pt_request = pt_buf_agt->pt_requests_head;

        pt_buf_agt->pt_requests_head = pt_buf_agt->pt_requests_head->pt_next;

        if (pt_buf_agt->pt_requests_head == NULL)
        {
            pt_buf_agt->pt_requests_tail = NULL;
        }

        if (b_nfy_canceled)
        {
            DBG_INFO((DBG_PREFIX
                      "Ack %u-byte data request (id:%u): CANCELED (%d)\n",
                      pt_request->z_size,
                      pt_request->ui4_req_id,
                      pt_conn->h_ch_conn));

            playback_conn_ack_data_request(pt_conn, pt_buf_agt, pt_request,
                                           BUF_AGT_DATA_REQ_ACK_COND_CANCELED);
        }

        playback_handler_release_data_request(pt_request);

        if (b_request_transfer)
        {
            playback_conn_request_data_transfer(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_cancel_all_requests
 *
 * Description: This API cancels all the data requests in the list.
 *
 * Inputs:  pt_conn             References the connection.
 *          b_nfy_canceled      Specifies if a notification should be sent.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_conn_cancel_all_requests (PLAYBACK_CONN_T*     pt_conn,
                                   PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                   BOOL                 b_nfy_canceled)
{
    while (pt_buf_agt->pt_requests_head != NULL)
    {
        playback_conn_cancel_head_request(pt_conn, pt_buf_agt, b_nfy_canceled,
                                          FALSE);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_queue_item
 *
 * Description: This API adds a playback item to the queue.
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_buf_agt  References the buffer agent structure.
 *          pt_item     References a playback item.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_queue_item (PLAYBACK_CONN_T*     pt_conn,
                                      PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                      PLAYBACK_ITEM_T*     pt_item)
{
    if (pt_buf_agt->pt_items_tail != NULL)
    {
        pt_buf_agt->pt_items_tail->pt_next = pt_item;
    }
    else
    {
        pt_buf_agt->pt_items_head = pt_item;

        if (pt_buf_agt->pt_unconsumed_requests == NULL)
        {
            playback_conn_update_scdb(pt_conn, pt_buf_agt, pt_item, 0);
        }
    }

    if (pt_buf_agt->pt_transferring_item == NULL)
    {
        pt_buf_agt->pt_transferring_item = pt_item;

        playback_conn_nfy_align_info(pt_buf_agt,
                                     pt_buf_agt->pt_transferring_item);

        if (pt_buf_agt->pt_requests_head != NULL)
        {
            if ((pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_PULL)
                || (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_DIRECT))
            {
                playback_conn_request_data_transfer(pt_conn);
            }
            else if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT)
            {
                if (pt_buf_agt->pt_requests_head)
                {
                    playback_conn_request_data_transfer(pt_conn);
                }
            }
        }
    }

    pt_buf_agt->pt_items_tail = pt_item;
    pt_item->e_state          = PLAYBACK_ITEM_STATE_QUEUED;

    cm_nfy(pt_conn->h_cm_conn,
           CM_PLAYBACK_CONN_INFO_ITEM_QUEUED,
           pt_item->ui4_id);
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_unqueue_item
 *
 * Description: This API removes a playback item (specified by its id) from the
 *              queue. If no item is found in the queue with such an id then
 *              the upper layer is notified the item id was invalid.
 *
 * Inputs:  pt_conn      References the connection.
 *          ui4_item_id  Specifies the id of the item to remove.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_unqueue_item (PLAYBACK_CONN_T*     pt_conn,
                                        UINT32               ui4_item_id)
{
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
    PLAYBACK_ITEM_T**    ppt_item;
    PLAYBACK_ITEM_T*     pt_previous;
    BOOL                 b_found;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    ppt_item    = & pt_buf_agt->pt_items_head;
    pt_previous = NULL;
    b_found     = FALSE;

    /* if id is PLAYBACK_HANDLER_INVALID_ITEM_ID, first item will match */
    if ((ui4_item_id == PLAYBACK_HANDLER_INVALID_ITEM_ID)
        &&
        (*ppt_item != NULL))
    {
        ui4_item_id = (*ppt_item)->ui4_id;

        DBG_INFO((DBG_PREFIX"Removing first item %u from queue\n",
                  ui4_item_id));
    }
    else
    {
        DBG_INFO((DBG_PREFIX"Removing item %u from queue\n", ui4_item_id));
    }

    while (pt_buf_agt != NULL)
    {
        ppt_item = & pt_buf_agt->pt_items_head;

        while (*ppt_item != NULL)
        {
            /*
              We find the item in the list with the same id and not unqueued
              already.
            */

            if (((*ppt_item)->ui4_id == ui4_item_id)
                &&
                ((*ppt_item)->e_state != PLAYBACK_ITEM_STATE_UNQUEUED))
            {
                PLAYBACK_ITEM_T*       pt_next;
                PLAYBACK_ITEM_STATE_T  e_state;
                BOOL                   b_transferring_item;

                pt_next              = (*ppt_item)->pt_next;
                e_state              = (*ppt_item)->e_state;
                (*ppt_item)->e_state = PLAYBACK_ITEM_STATE_UNQUEUED;
                b_found              = TRUE;

                b_transferring_item =
                    (*ppt_item == pt_buf_agt->pt_transferring_item);

                if (b_transferring_item)
                {
                    pt_buf_agt->pt_transferring_item =
                        pt_buf_agt->pt_transferring_item->pt_next;
                    pt_buf_agt->b_nfy_transfer_start = TRUE;

                    if (pt_buf_agt->pt_transferring_item != NULL)
                    {
                        playback_conn_nfy_align_info(
                            pt_buf_agt,
                            pt_buf_agt->pt_transferring_item);

                        DBG_INFO((DBG_PREFIX"Next item to transfer: %u (%d)\n",
                                  pt_buf_agt->pt_transferring_item->ui4_id,
                                  pt_conn->h_ch_conn));
                    }
                    else
                    {
                        DBG_INFO((DBG_PREFIX
                                  "No next item to transfer for now (%d)\n",
                                  pt_conn->h_ch_conn));
                    }
                }
                else
                {
                    DBG_INFO((DBG_PREFIX
                              "Unqueued item %u was not transferring\n",
                              ui4_item_id));
                }

                cm_nfy(pt_conn->h_cm_conn,
                       CM_PLAYBACK_CONN_INFO_ITEM_UNQUEUED,
                       ui4_item_id);

                /*
                  If the item was in QUEUED state then it can be removed and
                  deleted (since transfer was not started).
                */
                if (e_state == PLAYBACK_ITEM_STATE_QUEUED)
                {
                    DBG_INFO((DBG_PREFIX"Unqueued item %u can be deleted\n",
                              ui4_item_id));

                    if (b_transferring_item)
                    {
                        if (pt_buf_agt->pt_unconsumed_requests == NULL)
                        {
                            playback_conn_update_scdb(pt_conn,
                                                      pt_buf_agt,
                                                      pt_next,
                                                      (*ppt_item)->ui4_flags);
                        }

                        if (pt_next == NULL)
                        {
                            cm_nfy(pt_conn->h_cm_conn,
                                   CM_PLAYBACK_CONN_INFO_QUEUE_EMPTY,
                                   0);
                        }
                    }

                    if (*ppt_item == pt_buf_agt->pt_items_tail)
                    {
                        pt_buf_agt->pt_items_tail = pt_previous;
                    }

                    /* since item is removed, it can be deleted */
                    playback_handler_release_item(*ppt_item);

                    *ppt_item = pt_next;
                }
                else
                {
                    PLAYBACK_DATA_REQUEST_T*  pt_request;

                    DBG_INFO((DBG_PREFIX
                              "Unqueued item %u being/already transferred\n",
                              ui4_item_id));

                    pt_request = pt_buf_agt->pt_unconsumed_requests;

                    /*
                      Item is in the process of being transferred or has been
                      transferred: we find the last request for this item and
                      mark it as being the last one.
                    */
                    while (pt_request != NULL)
                    {
                        if (pt_request->pt_item == *ppt_item)
                        {
                            while ((pt_request->pt_next != NULL)
                                   &&
                                   (pt_request->pt_next->pt_item == *ppt_item))
                            {
                                pt_request = pt_request->pt_next;
                            }

                            DBG_INFO((DBG_PREFIX
                                      "Request %u marked 'last' for item %u\n",
                                      pt_request->ui4_req_id,
                                      pt_request->pt_item->ui4_id));

                            pt_request->b_last = TRUE;

                            break;
                        }

                        pt_request = pt_request->pt_next;
                    }

                    if (pt_request == NULL)
                    {
                        /*
                          There is no unconsumed request for this item so the
                          item can be removed from the list.
                        */

                        DBG_INFO((DBG_PREFIX
                                  "No unconsumed request for item %u\n",
                                  ui4_item_id));

                        if (b_transferring_item)
                        {
                            if (pt_buf_agt->pt_unconsumed_requests == NULL)
                            {
                                UINT32  ui4_prev_flags;

                                ui4_prev_flags = (*ppt_item)->ui4_flags;

                                playback_conn_update_scdb(pt_conn,
                                                          pt_buf_agt,
                                                          pt_next,
                                                          ui4_prev_flags);
                            }

                            if (pt_next == NULL)
                            {
                                cm_nfy(pt_conn->h_cm_conn,
                                       CM_PLAYBACK_CONN_INFO_QUEUE_EMPTY,
                                       0);
                            }
                        }

                        if (*ppt_item == pt_buf_agt->pt_items_tail)
                        {
                            pt_buf_agt->pt_items_tail = pt_previous;
                        }

                        playback_handler_release_item(*ppt_item);

                        *ppt_item = pt_next;
                    }
                }

                break;
            }

            pt_previous = *ppt_item;
            ppt_item = & (*ppt_item)->pt_next;
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }

    if (! b_found)
    {
        DBG_INFO((DBG_PREFIX"Did not find playback item %u to unqueue\n",
                  ui4_item_id));

        cm_nfy(pt_conn->h_cm_conn,
               CM_PLAYBACK_CONN_INFO_UNQUEUE_INVALID_ID,
               ui4_item_id);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_empty_queue
 *
 * Description: This API empties the queue of playback items. All items are
 *              released.
 *
 * Inputs:  pt_conn      References the connection.
 *          b_force      Specifies if items should be released regardless of
 *                       the item state.
 *          b_nfy_empty  Specifies whether a QUEUE_EMPTY notification should be
 *                       sent.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_empty_queue (PLAYBACK_CONN_T*  pt_conn,
                                       BOOL              b_force,
                                       BOOL              b_nfy_empty)
{
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        PLAYBACK_ITEM_T*  pt_item;

        pt_item = pt_buf_agt->pt_items_head;

        pt_buf_agt->pt_items_tail = NULL;

        if (b_force)
        {
            /* all items will be released so list will be empty */
            pt_buf_agt->pt_items_head = NULL;
        }
        else
        {
            /*
              we split the list in 2 lists: the one that is going to be deleted
              (pt_item will be the head of this list) and the one we keep.
            */

            while ((pt_item != NULL) &&
                   (pt_item->e_state != PLAYBACK_ITEM_STATE_QUEUED))
            {
                pt_buf_agt->pt_items_tail = pt_item;
                pt_item = pt_item->pt_next;
            }

            /* if tail is NULL then all items have been removed */
            if (pt_buf_agt->pt_items_tail == NULL)
            {
                pt_buf_agt->pt_items_head = NULL;
            }
            else
            {
                pt_buf_agt->pt_items_tail->pt_next = NULL;
            }
        }

        pt_buf_agt->pt_transferring_item = NULL;

        while (pt_item != NULL)
        {
            PLAYBACK_ITEM_T*  pt_next;

            pt_next = pt_item->pt_next;

            playback_handler_release_item(pt_item);

            pt_item = pt_next;
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }

    if (b_nfy_empty)
    {
        cm_nfy(pt_conn->h_cm_conn, CM_PLAYBACK_CONN_INFO_QUEUE_EMPTY, 0);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_queue_data_request
 *
 * Description: This API adds a data request to the queue. If the playback
 *              items queue is not empty then a data transfer request is sent.
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_request  References the data request to queue.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32
playback_conn_queue_data_request (PLAYBACK_CONN_T*          pt_conn,
                                  PLAYBACK_BUF_AGT_T*       pt_buf_agt,
                                  PLAYBACK_DATA_REQUEST_T*  pt_request)
{
    if (! pt_conn->b_disconnecting)
    {
        if (pt_request->ui4_buf_agt_flags & BUF_AGT_SET_DATA_OFFSET)
        {
            DBG_INFO((DBG_PREFIX
                      "Queuing %u-byte (offset %llu) data request %u (%d)\n",
                      pt_request->z_size,
                      pt_request->ui8_transfer_position,
                      pt_request->ui4_req_id,
                      pt_conn->h_ch_conn));
        }
        else
        {
            DBG_INFO((DBG_PREFIX"Queuing %u-byte data request %u (%d)\n",
                      pt_request->z_size,
                      pt_request->ui4_req_id,
                      pt_conn->h_ch_conn));
        }

        if (pt_buf_agt->pt_requests_tail != NULL)
        {
            pt_buf_agt->pt_requests_tail->pt_next = pt_request;
        }
        else
        {
            pt_buf_agt->pt_requests_head = pt_request;

            playback_conn_request_data_transfer(pt_conn);
        }

        pt_buf_agt->pt_requests_tail = pt_request;
    }
    else
    {
        /* connection is disconnecting, so request must be failed */

        playback_conn_ack_data_request(pt_conn, pt_buf_agt, pt_request,
                                       BUF_AGT_DATA_REQ_ACK_COND_ERROR);

        playback_handler_release_data_request(pt_request);
    }

    return CMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_data_consumed
 *
 * Description: This API is called when the data from a previous data request
 *              has been 'consumed' (whatever that means).
 *
 * Inputs:  pt_conn        References the connection structure.
 *          ui4_req_id     Contains the request id.
 *          b_release      Specifies whether the request should be released.
 *          b_remove_item  Specifies whether the item should be unqueued if the
 *                         request is the last one for this item.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_data_consumed (PLAYBACK_CONN_T*     pt_conn,
                                         PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                         UINT32               ui4_req_id,
                                         BOOL                 b_release,
                                         BOOL                 b_remove_item)
{
    PLAYBACK_DATA_REQUEST_T**  ppt_request;
    PLAYBACK_DATA_REQUEST_T*   pt_previous;
    UINT32                     ui4_playback_item_id;
    UINT32                     ui4_prev_flags;
    BOOL                       b_nfy_item_consumed;

    ppt_request          = & pt_buf_agt->pt_unconsumed_requests;
    pt_previous          = NULL;
    ui4_playback_item_id = PLAYBACK_HANDLER_INVALID_ITEM_ID;
    ui4_prev_flags       = 0;
    b_nfy_item_consumed  = FALSE;

    while (*ppt_request != NULL)
    {
        if ((*ppt_request)->ui4_req_id == ui4_req_id)
        {
            PLAYBACK_DATA_REQUEST_T*  pt_request;

            DBG_INFO((DBG_PREFIX
                      "Data from request %u (item %u) is consumed (%d)\n",
                      ui4_req_id,
                      ((*ppt_request)->pt_item ?(*ppt_request)->pt_item->ui4_id:0),
                      pt_conn->h_ch_conn));

            if (*ppt_request != pt_buf_agt->pt_unconsumed_requests)
            {
                DBG_INFO((DBG_PREFIX
                          "WARNING: data consumed out of order (%d)\n"
                          DBG_PREFIX
                          "Request %u consumed before request %u (%d)\n",
                          pt_conn->h_ch_conn,
                          (*ppt_request)->ui4_req_id,
                          pt_buf_agt->pt_unconsumed_requests->ui4_req_id,
                          pt_conn->h_ch_conn));

                /* previous request is marked as being the last */
                if ( ((*ppt_request)->b_last) &&
                     (pt_previous!=NULL) )
                {
                    (*ppt_request)->b_last = pt_previous->b_last;
                    pt_previous->b_last    = TRUE;
                }
            }

            pt_request   = *ppt_request;

            /*shift the item position*/
            if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT)
            {
                PLAYBACK_DATA_REQUEST_T* pt_unconsumed_request;
                UINT32 ui4_fill_size;
                UINT64 ui8_req_pos;
                UINT64 ui8_min_req_pos = (UINT64)(-1);
                UINT32 ui4_size;

                pt_unconsumed_request = pt_buf_agt->pt_unconsumed_requests;
                while (pt_unconsumed_request)
                {
                    if (pt_unconsumed_request->ui8_transfer_position < ui8_min_req_pos)
                    {
                        ui8_min_req_pos = pt_unconsumed_request->ui8_transfer_position;
                    }
                    pt_unconsumed_request = pt_unconsumed_request->pt_next;
                }

                ui8_req_pos = pt_request->ui8_transfer_position;
                if (ui8_req_pos <= ui8_min_req_pos)
                {
                    if(ui8_req_pos + pt_request->z_size > pt_buf_agt->pt_transferring_item->ui8_position)
                    {/* It maybe exist read backward, therefore, the ui4_size maybe very large*/
                        ui4_size = (UINT32)(ui8_req_pos + pt_request->z_size - pt_buf_agt->pt_transferring_item->ui8_position);
                        playback_cb_skip(pt_buf_agt->pt_cb, &ui4_size);
                        pt_buf_agt->pt_transferring_item->ui8_position = ui8_req_pos + pt_request->z_size;
                        ui4_fill_size = playback_cb_get_filled_size(pt_buf_agt->pt_cb);
                        if (b_log_cmpb)
                        {
                            x_dbg_stmt("[CMPB] playback_conn_data_consumed position=%d size=%d\n", (UINT32)ui8_req_pos, pt_request->z_size);
                        }

                        if (ui4_fill_size < PLAYBACK_HANDLER_CIRCULAR_READ_UNIT)
                        {
                            x_dbg_stmt("[CMPB] !!!data underflow\n");
                        }
                    }
                }
            }

            *ppt_request = pt_request->pt_next;

            if (pt_request->pt_item != NULL)
            {
                ui4_playback_item_id = pt_request->pt_item->ui4_id;
                ui4_prev_flags       = pt_request->pt_item->ui4_flags;

                if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT)
                {
                    b_nfy_item_consumed = FALSE;
                }
                else if (pt_buf_agt->b_shared_memory)
                {
                    /*buffer agent with push mode and use share memory*/
                    if (pt_request->pt_item->e_format == DATA_FMT_IBC)
                    {
                        b_nfy_item_consumed = FALSE;
                    }
                    else
                    {
                        b_nfy_item_consumed = TRUE;
                    }
                }
                else
                {
                    /*b_nfy_item_consumed  = pt_request->b_last && b_remove_item; Removed for bad interleave file may read after close*/
                }

                if (b_nfy_item_consumed)
                {
                    PLAYBACK_DATA_REQUEST_T*    pt_tmp = pt_request->pt_next;

                    playback_conn_remove_item_from_list(pt_buf_agt,
                                                        pt_request->pt_item);
                    /*
                       If there are any unconsumed requests for this item,
                       we also need to set their pt_item to NULL
                     */
                    while(pt_tmp != NULL)
                    {
                        if (pt_tmp->pt_item == pt_request->pt_item)
                        {
                            DBG_INFO((DBG_PREFIX
                                      "Also set playback item of request %u to NULL (%d)\n",
                                      pt_tmp->ui4_req_id,
                                      pt_conn->h_ch_conn));

                            pt_tmp->pt_item = NULL;
                        }
                        pt_tmp = pt_tmp->pt_next;
                    }

                    playback_handler_release_item(pt_request->pt_item);

                    pt_request->pt_item = NULL;
                }
            }
            else
            {
                DBG_INFO((DBG_PREFIX
                         "The playback item of request %u is already released by previous request (%d)\n",
                         pt_request->ui4_req_id,
                         pt_conn->h_ch_conn));
            }

            if (b_release)
            {
                playback_handler_release_data_request(pt_request);
            }

            break;
        }

        pt_previous = *ppt_request;
        ppt_request = & (*ppt_request)->pt_next;
    }

    if (ui4_playback_item_id == PLAYBACK_HANDLER_INVALID_ITEM_ID)
    {
        DBG_INFO((DBG_PREFIX
                  "WARNING: no such unconsumed data request id %u (%d)\n",
                  ui4_req_id, pt_conn->h_ch_conn));
    }
    else if (pt_buf_agt->pt_unconsumed_requests == NULL)
    {
        DBG_INFO((DBG_PREFIX"All requests are consumed (%d)\n",
                  pt_conn->h_ch_conn));
    }

    /*after consumed, try to issue a data request*/
    if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT)
    {
        playback_uri_issue_req (pt_conn, pt_buf_agt);
    }

    if (b_nfy_item_consumed)
    {
        PLAYBACK_ITEM_T*  pt_item;

        DBG_INFO((DBG_PREFIX"Playback item %u consumed (%d)\n",
                  ui4_playback_item_id, pt_conn->h_ch_conn));

        cm_nfy(pt_conn->h_cm_conn,
               CM_PLAYBACK_CONN_INFO_ITEM_CONSUMED,
               ui4_playback_item_id);

        pt_item = (pt_buf_agt->pt_unconsumed_requests != NULL) ?
            pt_buf_agt->pt_unconsumed_requests->pt_item :
            pt_buf_agt->pt_items_head;

        if (!pt_buf_agt->b_shared_memory)
        {
            playback_conn_update_scdb(pt_conn, pt_buf_agt, pt_item,
                                      ui4_prev_flags);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_release_all_unconsumed_requests
 *
 * Description: This API releases all the data requests that have been
 *              processed but that are not consumed yet.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_conn_release_all_unconsumed_requests (PLAYBACK_CONN_T*  pt_conn)
{
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        PLAYBACK_DATA_REQUEST_T*  pt_request;

        pt_request = pt_buf_agt->pt_unconsumed_requests;
        pt_buf_agt->pt_unconsumed_requests = NULL;

        while (pt_request != NULL)
        {
            PLAYBACK_DATA_REQUEST_T*  pt_next;

            pt_next = pt_request->pt_next;

#if PLAYBACK_HANDLER_USE_ASYNC_FM_READ
            if (pt_request->b_done)
            {
                /*
                  The request has been completed so it can be released now
                */
                playback_handler_release_data_request(pt_request);
            }
            else
            {
                if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_PULL)
                {
                    PLAYBACK_ITEM_T* pt_item;

                    pt_item = pt_buf_agt->pt_transferring_item;

                    /*
                      The request will be freed when the ABORT_OK or ABORT_FAIL
                      notification is received
                    */

                    DBG_INFO((DBG_PREFIX"Aborting file request (req=%u)\n",
                              pt_request->ui4_req_id));

                    pt_request->pt_next   = NULL;
                    pt_request->b_aborted = TRUE;

                    if (pt_item)
                    {
                        if (pt_item->e_type == PLAYBACK_ITEM_TYPE_PULL)
                        {
                            if (b_log_cmpb)
                            {
                                x_dbg_stmt("[CMPB] before pf_abort_read_async\n");
                            }
                            pt_item->u.t_pull.t_pull_info.pf_abort_read_async(
                                pt_item->u.t_pull.h_pull,
                                pt_item->u.t_pull.t_pull_info.pv_app_tag,
                                pt_request->h_fm_req);
                            if (b_log_cmpb)
                            {
                                x_dbg_stmt("[CMPB] after pf_abort_read_async\n");
                            }
                        }
                        else
                        {
                            x_fm_mfw_abort_async(pt_request->h_fm_req);
                        }
                    }
                }
                else if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_DIRECT)
                {
                    PLAYBACK_ITEM_T* pt_item;

                    pt_item = pt_buf_agt->pt_transferring_item;

                    if (pt_item)
                    {
                        if (pt_item->u.t_uri.b_url)
                        {
#ifdef INET_SUPPORT
                            x_httpc_fm_abort_async(pt_item->u.t_uri.h_uri, pt_request->h_fm_req);
#endif
                        }
                        else
                        {
                            x_fm_mfw_abort_async(pt_request->h_fm_req);
                        }
                    }
                }
            }
#else
            playback_handler_release_data_request(pt_request);
#endif

            pt_request = pt_next;
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_flush_done
 *
 * Description: This API tells the buffer agent that the flush operation is
 *              completed.
 *
 * Inputs:  pt_conn     References the connection structure.
 *          pt_buf_agt  References the buffer agent structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_flush_done (PLAYBACK_CONN_T*     pt_conn,
                                      PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    INT32  i4_res;

    pt_buf_agt->ui4_flush--;

    i4_res = rm_set(pt_buf_agt->h_rm_buffer_agent,
                    FALSE,
                    ANY_PORT_NUM,
                    BUF_AGT_SET_TYPE_FLUSH_DONE,
                    NULL, 0,
                    NULL);

    if (i4_res != RMR_OK)
    {
        DBG_ERROR((DBG_PREFIX"Could not set FLUSH_DONE (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_res));
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_consume_all_requests
 *
 * Description: This API consumed all the data requests that have been
 *              processed but that are not consumed yet.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_conn_consume_all_requests (PLAYBACK_CONN_T*     pt_conn,
                                    PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    PLAYBACK_DATA_REQUEST_T*  pt_request;
    BOOL                      b_aborted;

    b_aborted = FALSE;

    while ((pt_request = pt_buf_agt->pt_unconsumed_requests) != NULL)
    {
        BOOL  b_remove_item = FALSE;

		if(pt_request->pt_item != NULL)
		{	
			b_remove_item = (pt_request->pt_item->e_state == PLAYBACK_ITEM_STATE_UNQUEUED);
		}
        playback_conn_data_consumed(pt_conn, pt_buf_agt,
                                    pt_request->ui4_req_id,
                                    FALSE, b_remove_item);

#if PLAYBACK_HANDLER_USE_ASYNC_FM_READ
        if (pt_request->b_done)
        {
            /*
              The request has been completed so it can be released now
            */
            playback_handler_release_data_request(pt_request);
        }
        else
        {
            if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_PULL)
            {
                BOOL  b_flush_done;
                PLAYBACK_ITEM_T* pt_item;

                pt_item = pt_buf_agt->pt_transferring_item;

                b_flush_done = (pt_buf_agt->pt_unconsumed_requests == NULL);

                /*
                  The request will be freed when the ABORT_OK or ABORT_FAIL
                  notification is received
                */
                pt_request->pt_next      = NULL;
                pt_request->b_aborted    = TRUE;
                pt_request->b_flush_done = b_flush_done;

                DBG_INFO((DBG_PREFIX"Aborting file request (req=%u)\n",
                          pt_request->ui4_req_id));

                if (pt_item)
                {
                    if (pt_item->e_type == PLAYBACK_ITEM_TYPE_PULL)
                    {
                        if (b_log_cmpb)
                        {
                            x_dbg_stmt("[CMPB] before pf_abort_read_async\n");
                        }
                        pt_item->u.t_pull.t_pull_info.pf_abort_read_async(
                            pt_item->u.t_pull.h_pull,
                            pt_item->u.t_pull.t_pull_info.pv_app_tag,
                            pt_request->h_fm_req);
                        if (b_log_cmpb)
                        {
                            x_dbg_stmt("[CMPB] after pf_abort_read_async\n");
                        }
                    }
                    else
                    {
                        x_fm_mfw_abort_async(pt_request->h_fm_req);
                    }
                }

                b_aborted = TRUE;
            }
            else if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_DIRECT)
            {
                PLAYBACK_ITEM_T* pt_item;
                BOOL  b_flush_done;

                b_flush_done = (pt_buf_agt->pt_unconsumed_requests == NULL);

                /*
                  The request will be freed when the ABORT_OK or ABORT_FAIL
                  notification is received
                */
                pt_request->pt_next      = NULL;
                pt_request->b_aborted    = TRUE;
                pt_request->b_flush_done = b_flush_done;

                DBG_INFO((DBG_PREFIX"Aborting file request (req=%u)\n",
                          pt_request->ui4_req_id));


                pt_item = pt_buf_agt->pt_transferring_item;

                if (pt_item)
                {
                    if (pt_item->u.t_uri.b_url)
                    {
#ifdef INET_SUPPORT
                        x_httpc_fm_abort_async(pt_item->u.t_uri.h_uri, pt_request->h_fm_req);
#endif
                    }
                    else
                    {
                        x_fm_mfw_abort_async(pt_request->h_fm_req);
                    }
                    b_aborted = TRUE;
                }
            }
        }
#else
        playback_handler_release_data_request(pt_request);
#endif
    }

    if (! b_aborted)
    {
        playback_conn_flush_done(pt_conn, pt_buf_agt);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_nfy_connected
 *
 * Description: This API will send a CM_COND_CONNECTED notification the first
 *              time it is called for a given connection. Subsequent calls do
 *              nothing.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID playback_nfy_connected (PLAYBACK_CONN_T*  pt_conn)
{
    if (! pt_conn->b_cm_nfy_connected)
    {
        DBG_INFO((DBG_PREFIX"Notifying 'CONNECTED' (%d)\n",
                  pt_conn->h_ch_conn));

        cm_nfy(pt_conn->h_cm_conn, CM_COND_CONNECTED, 0);

        pt_conn->b_cm_nfy_connected = TRUE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_are_all_buf_agt_connected
 *
 * Description: This API checks whether all the buffer agents are connected.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: TRUE if all the buffer agents are connected, FALSE otherwise.
 ----------------------------------------------------------------------------*/
static BOOL
playback_conn_are_all_buf_agt_connected (const PLAYBACK_CONN_T*  pt_conn)
{
    const PLAYBACK_BUF_AGT_T*  pt_buf_agt;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        if (! pt_buf_agt->b_buffer_agent_connected)
        {
            return FALSE;
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: playback_buffer_agent_connected
 *
 * Description: This API is called when the buffer agent is connected. The
 *              Connection Manager is then notified that the connection is now
 *              in the CONNECTED state.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_buffer_agent_connected (PLAYBACK_CONN_T*     pt_conn,
                                             PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    if (! pt_conn->b_disconnecting)
    {
        if (! pt_buf_agt->b_buffer_agent_connected)
        {
            PLAYBACK_MODE_CTRL_T*   pt_mode_ctrl;

            pt_mode_ctrl = &pt_conn->t_connect_info.t_mode_ctrl;

            if (pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit)
            {
                pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit = ((pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit + 511)/512)*512;
            }
            else
            {
                pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit = PLAYBACK_HANDLER_CIRCULAR_READ_UNIT;
            }

            pt_buf_agt->b_buffer_agent_connected = TRUE;

            if (pt_mode_ctrl->e_op_mode == PLAYBACK_CONN_OPERATION_MODE_CIRCULAR)
            {
                INT32               i4_res;
                BUF_MODE_CTRL_T     t_mode_ctrl;

                x_memset(&t_mode_ctrl, 0, sizeof(BUF_MODE_CTRL_T));

                t_mode_ctrl.e_op_mode = BUF_AGT_OPERATION_MODE_CIRCULAR;
                switch (pt_mode_ctrl->t_feeder_buf_info.e_mode)
                {
                case PLAYBACK_FEEDER_BUF_MODE_TIME:
                    t_mode_ctrl.t_feeder_buf_info.e_mode = BUF_AGT_FEEDER_BUF_MODE_TIME;
                    t_mode_ctrl.t_feeder_buf_info.u.ui4_time_ms = pt_mode_ctrl->t_feeder_buf_info.u.ui4_time_ms;
                    break;

                default:
                    t_mode_ctrl.t_feeder_buf_info.e_mode = BUF_AGT_FEEDER_BUF_MODE_TIME;
                    t_mode_ctrl.t_feeder_buf_info.u.ui4_time_ms = 0;;
                    break;
                }

                t_mode_ctrl.u.t_flow_ctrl.ui4_buffer_len = pt_mode_ctrl->u.t_flow_ctrl.ui4_buffer_len;
                t_mode_ctrl.u.t_flow_ctrl.ui4_keep_buffer_threshold = pt_mode_ctrl->u.t_flow_ctrl.ui4_keep_buffer_threshold;
                t_mode_ctrl.u.t_flow_ctrl.ui4_re_buferf_threshold = pt_mode_ctrl->u.t_flow_ctrl.ui4_re_buferf_threshold;
                t_mode_ctrl.ui4_pid = pt_mode_ctrl->ui4_pid;
                t_mode_ctrl.b_quick_consume = TRUE;
                pt_buf_agt->b_quick_consume = TRUE;
                i4_res = rm_set(pt_buf_agt->h_rm_buffer_agent,
                                FALSE,
                                ANY_PORT_NUM,
                                BUF_AGT_SET_OPERATION_MODE,
                                (VOID*) & t_mode_ctrl,
                                sizeof(BUF_MODE_CTRL_T),
                                NULL);

                if (RMR_OK == i4_res)
                {
                    pt_buf_agt->e_mode = PLAYBACK_BUFFER_MODE_CIRCULAR_DIRECT;
                }
                else
                {
                    pt_buf_agt->t_flow_ctrl.ui4_buffer_len
                        = ((t_mode_ctrl.u.t_flow_ctrl.ui4_buffer_len + (pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit-1))/pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit)*pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit;
                    pt_buf_agt->t_flow_ctrl.ui4_keep_buffer_threshold = ((t_mode_ctrl.u.t_flow_ctrl.ui4_keep_buffer_threshold + 511)/512)*512;
                    pt_buf_agt->t_flow_ctrl.ui4_re_buferf_threshold = ((t_mode_ctrl.u.t_flow_ctrl.ui4_re_buferf_threshold + 511)/512)*512;

                    pt_buf_agt->pt_cb = playback_cb_new(pt_buf_agt->t_flow_ctrl.ui4_buffer_len, pt_buf_agt->t_flow_ctrl.ui4_keep_buffer_threshold, 512);

                    pt_buf_agt->e_mode = PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT;
                }
            }
            //else if (pt_buf_agt->b_shared_memory)
            else if (pt_mode_ctrl->e_op_mode == PLAYBACK_CONN_OPERATION_MODE_PUSH_SHARE_MEM)
            {
                BUF_MODE_CTRL_T         t_mode_ctrl;
                PLAYBACK_MODE_CTRL_T*   pt_mode_ctrl;

                pt_mode_ctrl = &pt_conn->t_connect_info.t_mode_ctrl;

                if (pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit)
                {
                    pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit = ((pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit + 511)/512)*512;
                }
                else
                {
                    pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit = PLAYBACK_HANDLER_CIRCULAR_READ_UNIT;
                }

                x_memset(&t_mode_ctrl, 0, sizeof(BUF_MODE_CTRL_T));
                switch (pt_mode_ctrl->t_feeder_buf_info.e_mode)
                {
                case PLAYBACK_FEEDER_BUF_MODE_TIME:
                    t_mode_ctrl.t_feeder_buf_info.e_mode = BUF_AGT_FEEDER_BUF_MODE_TIME;
                    t_mode_ctrl.t_feeder_buf_info.u.ui4_time_ms = pt_mode_ctrl->t_feeder_buf_info.u.ui4_time_ms;
                    break;

                default:
                    t_mode_ctrl.t_feeder_buf_info.e_mode = BUF_AGT_FEEDER_BUF_MODE_TIME;
                    t_mode_ctrl.t_feeder_buf_info.u.ui4_time_ms = 0;;
                    break;
                }

                t_mode_ctrl.e_op_mode = BUF_AGT_OPERATION_MODE_PUSH_SHARE_MEM;
                t_mode_ctrl.ui4_pid = pt_mode_ctrl->ui4_pid;
                pt_buf_agt->b_shared_memory = TRUE;
                pt_buf_agt->b_quick_consume = FALSE;
                t_mode_ctrl.b_quick_consume = FALSE;
                rm_set(pt_buf_agt->h_rm_buffer_agent,
                       FALSE,
                       ANY_PORT_NUM,
                       BUF_AGT_SET_OPERATION_MODE,
                       (VOID*) & t_mode_ctrl,
                       sizeof(BUF_MODE_CTRL_T),
                       NULL);
            }
            else if (pt_mode_ctrl->e_op_mode == PLAYBACK_CONN_OPERATION_MODE_PULL)
            {
                BUF_MODE_CTRL_T         t_mode_ctrl;
                PLAYBACK_MODE_CTRL_T*   pt_mode_ctrl;

                pt_mode_ctrl = &pt_conn->t_connect_info.t_mode_ctrl;

                if (pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit)
                {
                    pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit = ((pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit + 511)/512)*512;
                }
                else
                {
                    pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit = PLAYBACK_HANDLER_CIRCULAR_READ_UNIT;
                }

                x_memset(&t_mode_ctrl, 0, sizeof(BUF_MODE_CTRL_T));
                switch (pt_mode_ctrl->t_feeder_buf_info.e_mode)
                {
                case PLAYBACK_FEEDER_BUF_MODE_TIME:
                    t_mode_ctrl.t_feeder_buf_info.e_mode = BUF_AGT_FEEDER_BUF_MODE_TIME;
                    t_mode_ctrl.t_feeder_buf_info.u.ui4_time_ms = pt_mode_ctrl->t_feeder_buf_info.u.ui4_time_ms;
                    break;

                default:
                    t_mode_ctrl.t_feeder_buf_info.e_mode = BUF_AGT_FEEDER_BUF_MODE_TIME;
                    t_mode_ctrl.t_feeder_buf_info.u.ui4_time_ms = 0;;
                    break;
                }

                t_mode_ctrl.e_op_mode = BUF_AGT_OPERATION_MODE_PULL;
                t_mode_ctrl.ui4_pid = pt_mode_ctrl->ui4_pid;
                t_mode_ctrl.b_quick_consume = TRUE;
                pt_buf_agt->b_quick_consume = TRUE;
                rm_set(pt_buf_agt->h_rm_buffer_agent,
                       FALSE,
                       ANY_PORT_NUM,
                       BUF_AGT_SET_OPERATION_MODE,
                       (VOID*) & t_mode_ctrl,
                       sizeof(BUF_MODE_CTRL_T),
                       NULL);
            }
            else
            {
                BUF_MODE_CTRL_T         t_mode_ctrl;
                PLAYBACK_MODE_CTRL_T*   pt_mode_ctrl;

                pt_mode_ctrl = &pt_conn->t_connect_info.t_mode_ctrl;

                if (pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit)
                {
                    pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit = ((pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit + 511)/512)*512;
                }
                else
                {
                    pt_conn->t_connect_info.t_mode_ctrl.ui4_access_unit = PLAYBACK_HANDLER_CIRCULAR_READ_UNIT;
                }

                x_memset(&t_mode_ctrl, 0, sizeof(BUF_MODE_CTRL_T));
                switch (pt_mode_ctrl->t_feeder_buf_info.e_mode)
                {
                case PLAYBACK_FEEDER_BUF_MODE_TIME:
                    t_mode_ctrl.t_feeder_buf_info.e_mode = BUF_AGT_FEEDER_BUF_MODE_TIME;
                    t_mode_ctrl.t_feeder_buf_info.u.ui4_time_ms = pt_mode_ctrl->t_feeder_buf_info.u.ui4_time_ms;
                    break;

                default:
                    t_mode_ctrl.t_feeder_buf_info.e_mode = BUF_AGT_FEEDER_BUF_MODE_TIME;
                    t_mode_ctrl.t_feeder_buf_info.u.ui4_time_ms = 0;;
                    break;
                }

                t_mode_ctrl.e_op_mode = BUF_AGT_OPERATION_MODE_PULL;
                t_mode_ctrl.ui4_pid = pt_mode_ctrl->ui4_pid;
                t_mode_ctrl.b_quick_consume = FALSE;
                pt_buf_agt->b_quick_consume = FALSE;
                rm_set(pt_buf_agt->h_rm_buffer_agent,
                       FALSE,
                       ANY_PORT_NUM,
                       BUF_AGT_SET_OPERATION_MODE,
                       (VOID*) & t_mode_ctrl,
                       sizeof(BUF_MODE_CTRL_T),
                       NULL);
            }

            if (playback_conn_are_all_buf_agt_connected(pt_conn))
            {
                playback_nfy_connected(pt_conn);
            }
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_buffer_agent_nfy
 *
 * Description: This API is the function the buffer agent driver calls to
 *              request some more data, notify the handler data was consumed,
 *              or request all the requests need to be flushed (i.e. deleted).
 *
 * Inputs:  pv_nfy_tag  Specifies the notification tag.
 *          e_nfy_cond  Contains the notification condition.
 *          ui4_data_1  Contains some additional data.
 *          ui4_data_2  Contains some additional data.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static BOOL playback_buffer_agent_nfy (VOID*               pv_nfy_tag,
                                       BUF_AGT_NFY_COND_T  e_nfy_cond,
                                       UINT32              ui4_data_1,
                                       UINT32              ui4_data_2)
{
    CRIT_STATE_T         t_state;
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
    HANDLE_T             h_buffer_agent;
    HANDLE_T             h_ch_conn;
    UINT32               ui4_code;
    INT32                i4_res;
    BOOL                 b_res;

    h_buffer_agent = (HANDLE_T) pv_nfy_tag;

    t_state = x_crit_start();

    i4_res = handle_get_obj(h_buffer_agent, (VOID**) & pt_buf_agt);

    if (i4_res == HR_OK)
    {
        h_ch_conn = pt_buf_agt->h_ch_conn;
        b_res     = TRUE;
    }
    else
    {
        h_ch_conn  = NULL_HANDLE;
        e_nfy_cond = BUF_AGT_NFY_COND_UNKNOWN;
        b_res      = FALSE;
    }

    x_crit_end(t_state);

    switch (e_nfy_cond)
    {
        case BUF_AGT_NFY_COND_DATA_REQ:
        {
            BUF_AGT_DATA_BLK_T*       pt_block;
            PLAYBACK_DATA_REQUEST_T*  pt_request;

            pt_block = (BUF_AGT_DATA_BLK_T *) ui4_data_1;

            pt_request = playback_handler_get_data_request();

            if (pt_request != NULL)
            {
                pt_request->pt_next               = NULL;
                pt_request->pt_item               = NULL;
                pt_request->ui8_transfer_position = pt_block->ui8_offset;
                pt_request->h_ch_conn             = h_ch_conn;
                pt_request->h_buffer_agent        = h_buffer_agent;
                pt_request->h_fm_req              = NULL_HANDLE;
                pt_request->pv_buffer             = pt_block->pui1_buff;
                pt_request->z_size                = pt_block->z_size;
                pt_request->ui4_offset            = 0;
                pt_request->ui4_req_id            = pt_block->ui4_req_id;
                pt_request->ui4_buf_agt_flags     = pt_block->ui4_flags;
                pt_request->b_last                = FALSE;
                pt_request->b_done                = FALSE;
                pt_request->b_aborted             = FALSE;

                ui4_code   = PLAYBACK_HANDLER_MSG_DATA_REQUEST;
                ui4_data_1 = (UINT32) pt_request;

                if (b_log_cmpb)
                {
                    x_dbg_stmt("[CMPB] data req pos=%d size=%d\n",
                    (UINT32)pt_request->ui8_transfer_position,
                                (UINT32)pt_request->z_size);
                }
            }
            else
            {
                ui4_code = PLAYBACK_HANDLER_MSG_NONE;

                b_res = FALSE;
            }
        }
        break;

        case BUF_AGT_NFY_COND_DATA_CONSUMED:
        {
            if (b_log_cmpb)
            {
                x_dbg_stmt("[CMPB] BUF_AGT_NFY_COND_DATA_CONSUMED\n");
            }
            ui4_code = PLAYBACK_HANDLER_MSG_DATA_CONSUMED;
        }
        break;

        case BUF_AGT_NFY_COND_FLUSH_REQ:
        {
            ui4_code = PLAYBACK_HANDLER_MSG_FLUSH_REQUEST;
        }
        break;

#if 0
        case BUF_AGT_NFY_COND_DATA_OVERFLOW:
        {
            ui4_code = PLAYBACK_HANDLER_CIRCULAR_OVERFLOW;
        }
        break;

        case BUF_AGT_NFY_COND_DATA_UNDERFLOW:
        {
            ui4_code = PLAYBACK_HANDLER_CIRCULAR_UNDERFLOW;
        }
        break;
#endif

        case BUF_AGT_NFY_COND_UNKNOWN:
        default:
        {
            ui4_code = PLAYBACK_HANDLER_MSG_NONE;
        }
        break;
    }

    if (ui4_code != PLAYBACK_HANDLER_MSG_NONE)
    {
        INT32  i4_res;

        i4_res = cm_send_msg_retry(h_playback_handler_msgq,
                                      ui4_code,
                                      ui4_data_1, (UINT32) h_buffer_agent,
                                      h_ch_conn);

        if ((i4_res != CMR_OK)
            &&
            (ui4_code == PLAYBACK_HANDLER_MSG_DATA_REQUEST))
        {
            PLAYBACK_DATA_REQUEST_T*  pt_request;

            pt_request = (PLAYBACK_DATA_REQUEST_T *) ui4_data_1;

            playback_handler_release_data_request(pt_request);
        }

        b_res = (i4_res == CMR_OK);
    }

    return b_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_connect_buffer_agent
 *
 * Description: This API connects the buffer agent.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_connect_buffer_agent (PLAYBACK_CONN_T*     pt_conn,
                                           PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    BUF_AGT_NFY_INFO_T  t_nfy_info;
    INT32               i4_res;

    DBG_INFO((DBG_PREFIX"Connecting buffer agent (%d)\n", pt_conn->h_ch_conn));

    t_nfy_info.pv_tag = (VOID*) pt_buf_agt->h_buffer_agent;
    t_nfy_info.pf_nfy = playback_buffer_agent_nfy;

    i4_res = rm_set(pt_buf_agt->h_rm_buffer_agent,
                    FALSE,
                    ANY_PORT_NUM,
                    BUF_AGT_SET_TYPE_NFY_FCT,
                    (VOID*) & t_nfy_info,
                    sizeof(t_nfy_info),
                    NULL);

    if (i4_res == RMR_OK)
    {
        if (pt_buf_agt->b_shared_memory)
        {
            /*set buffer agent to share memory mode*/
            i4_res = rm_set(pt_buf_agt->h_rm_buffer_agent,
                            FALSE,
                            ANY_PORT_NUM,
                            BUF_AGT_SET_TYPE_SHARED_MEMORY,
                            (VOID*) (pt_buf_agt->b_shared_memory ? 1:0),
                            sizeof(BOOL),
                            NULL);
        }

        rm_set(pt_buf_agt->h_rm_buffer_agent,
                            FALSE,
                            ANY_PORT_NUM,
                            BUF_AGT_SET_TYPE_SOURCE_TYPE,
                            (VOID*) pt_conn->t_connect_info.e_bagt_src_type,
                            sizeof(UINT32),
                            NULL);

        i4_res = rm_connect(pt_buf_agt->h_rm_buffer_agent,
                            RM_CONN_TYPE_IGNORE,
                            NULL, 0,
                            (VOID*) pt_conn->h_ch_conn, /* tag */
                            & pt_buf_agt->e_rm_connect_buf_agt_cond);

        if (i4_res == RMR_OK)
        {
            DBG_INFO((DBG_PREFIX"Buffer agent connected synchronously (%d)\n",
                      pt_conn->h_ch_conn));

            playback_buffer_agent_connected(pt_conn, pt_buf_agt);
        }
        else if (i4_res == RMR_ASYNC_NFY)
        {
            /* nothing to do here */
        }
        else
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not connect buffer agent (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));

            playback_disconnect_conn(pt_conn,
                                     PLAYBACK_DISC_COULD_NOT_CONNECT_COMP);
        }
    }
    else
    {
        DBG_ERROR((DBG_PREFIX
                   "Could not set buffer agent's nfy function (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_res));

        playback_disconnect_conn(pt_conn,
                                 PLAYBACK_DISC_COULD_NOT_SET_COMP_PROPERTIES);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_connect_conn
 *
 * Description: This API connects all the driver components: currently only a
 *              buffer agent.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_connect_conn (PLAYBACK_CONN_T*  pt_conn)
{
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        if (! pt_conn->b_disconnecting)
        {
            playback_connect_buffer_agent(pt_conn, pt_buf_agt);
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_are_all_buf_agt_opened
 *
 * Description: This API checks whether all the buffer agents are opened.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: TRUE if all the buffer agents are opened, FALSE otherwise.
 ----------------------------------------------------------------------------*/
static BOOL
playback_conn_are_all_buf_agt_opened (const PLAYBACK_CONN_T*  pt_conn)
{
    const PLAYBACK_BUF_AGT_T*  pt_buf_agt;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        if (pt_buf_agt->h_rm_buffer_agent == NULL_HANDLE)
        {
            return FALSE;
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: playback_buffer_agent_opened
 *
 * Description: This API is called when the buffer agent is opened. It will
 *              then try to connect the component.
 *
 * Inputs:  pt_conn     References the connection structure.
 *          pt_buf_agt  References the buffer agent structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_buffer_agent_opened (PLAYBACK_CONN_T*     pt_conn,
                                          PLAYBACK_BUF_AGT_T*  pt_buf_agt)
{
    if (! pt_buf_agt->b_buffer_agent_opened)
    {
        DBG_INFO((DBG_PREFIX"Buffer agent now opened (%d)\n",
                  pt_conn->h_ch_conn));

        pt_buf_agt->b_buffer_agent_opened = TRUE;

        pt_buf_agt->e_mode = PLAYBACK_BUFFER_MODE_PULL;

        if (playback_conn_are_all_buf_agt_opened(pt_conn))
        {
            playback_connect_conn(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_comp_rm_buffer_agent_nfy
 *
 * Description: This API is called by the Resource Manager to notify the
 *              handler of changes in the state or condition of the driver
 *              component.
 *
 * Inputs:  h_comp      Contains a handle to the tuner driver component.
 *          ui1_port    Contains the port number.
 *          e_nfy_cond  Contains the notify condition.
 *          pv_tag      Contains the tag value specified in rm_open_comp.
 *          ui4_data    Contains additional data (depends on e_nfy_cond).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_comp_rm_buffer_agent_nfy (HANDLE_T   h_comp,
                                               UINT8      ui1_port,
                                               RM_COND_T  e_nfy_cond,
                                               VOID*      pv_tag,
                                               UINT32     ui4_data)
{
    HANDLE_T   h_ch_conn = (HANDLE_T) pv_tag;
    RM_COND_T  e_grp_cond;

    DBG_INFO((DBG_PREFIX"Buffer agent RM nfy 0x%08x (data %d) (%d)\n",
              e_nfy_cond, ui4_data, h_ch_conn));

    e_grp_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE, COND_GRP_OPERATION);

    if (e_grp_cond == RM_COND_CLIENT_GET_OPERATION)
    {
        GET_CLIENT_NFY_T*  pt_get;

        pt_get = (GET_CLIENT_NFY_T*) ui4_data;

        if (pt_get != NULL)
        {
            pt_get->i4_return = RMR_INV_GET;

            switch (pt_get->e_get_type)
            {
                default:
                    break;
            }
        }
    }
    else if (e_grp_cond == RM_COND_IGNORE)
    {
        CRIT_STATE_T      t_state;
        PLAYBACK_CONN_T*  pt_conn;
        INT32             i4_res;

        t_state = x_crit_start();

        i4_res = handle_get_obj(h_ch_conn, (VOID**) & pt_conn);

        if (i4_res == HR_OK)
        {
            PLAYBACK_BUF_AGT_T*  pt_buf_agt;
            RM_COND_T            e_rm_connect_cond;
            RM_COND_T            e_rm_disconnect_cond;
            RM_COND_T            e_rm_cond;
            UINT32               ui4_code;

            pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

            do
            {
                if (pt_buf_agt == NULL)
                {
                    x_crit_end(t_state);

                    return;
                }

                if (pt_buf_agt->h_rm_buffer_agent == h_comp)
                {
                    break;
                }

                pt_buf_agt = pt_buf_agt->pt_next;
            }
            while (1);

            e_rm_connect_cond    = pt_buf_agt->e_rm_connect_buf_agt_cond;
            e_rm_disconnect_cond = pt_buf_agt->e_rm_disconnect_buf_agt_cond;

            x_crit_end(t_state);

            ui4_code = PLAYBACK_HANDLER_MSG_NONE;

            if (e_rm_connect_cond == RM_COND_IGNORE)
            {
                /*
                  rm_connect has not been called yet - only OPENED and CLOSED
                  are relevant
                */

                e_rm_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE,
                                        COND_GRP_OBJ_STATE);

                switch (e_rm_cond)
                {
                    case RM_COND_OPENED:
                        ui4_code = PLAYBACK_HANDLER_MSG_BUFFER_AGENT_OPENED;
                        break;

                    case RM_COND_CLOSED:
                        ui4_code = PLAYBACK_HANDLER_MSG_BUFFER_AGENT_CLOSED;
                        break;

                    default:
                        break;
                }
            }
            else
            {
                e_rm_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE,
                                        COND_GRP_CONN_STATE);

                switch (e_rm_cond)
                {
                    case RM_COND_CONNECTED:
                    {
                        if (e_rm_disconnect_cond == RM_COND_IGNORE)
                        {
                            ui4_code =
                                PLAYBACK_HANDLER_MSG_BUFFER_AGENT_CONNECTED;
                        }
                        else
                        {
                            ui4_code =
                                PLAYBACK_HANDLER_MSG_BUFFER_AGENT_DISCONNECTED;
                        }
                    }
                    break;

                    case RM_COND_DISCONNECTED:
                    {
                        ui4_code =
                            PLAYBACK_HANDLER_MSG_BUFFER_AGENT_DISCONNECTED;
                    }
                    break;

                    case RM_COND_CONNECTING:
                    {
                        if (e_rm_disconnect_cond != RM_COND_IGNORE)
                        {
                            ui4_code =
                                PLAYBACK_HANDLER_MSG_BUFFER_AGENT_DISCONNECTED;
                        }
                    }
                    break;

                    default:
                        break;
                }
            }

            if (ui4_code != PLAYBACK_HANDLER_MSG_NONE)
            {
                cm_send_msg_retry(h_playback_handler_msgq,
                                  ui4_code, ui4_data, (UINT32) h_comp,
                                  h_ch_conn);
            }
        }
        else
        {
            x_crit_end(t_state);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_open_buffer_agent
 *
 * Description: This API opens a buffer agent driver component.
 *
 * Inputs:  pt_conn     References the connection.
 *          pt_buf_agt  References the buffer agent structure.
 *          h_rm_obj    Contains the handle the buffer agent should be open on.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_open_buffer_agent (PLAYBACK_CONN_T*     pt_conn,
                                        PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                        HANDLE_T             h_rm_obj)
{
    CM_COMP_RM_SELECT_T  t_select;
    CHAR*                ps_buffer_agent_name;
    RM_COND_T            e_rm_cond;
    UINT32               ui4_reason;
    INT32                i4_res;

    DBG_INFO((DBG_PREFIX"Opening buffer agent (%d)...\n",
              pt_conn->h_ch_conn));

    ui4_reason = PLAYBACK_DISC_OTHER_REASON;
    i4_res     = CMR_OK;

    cm_comp_rm_select_init(& t_select);

    t_select.b_select_if_conflict =
        ! pt_conn->t_connect_info.b_disc_if_comp_busy;
    t_select.b_would_conflict = FALSE;

    ps_buffer_agent_name =
        (pt_buf_agt->s_name[0] != '\0') ? pt_buf_agt->s_name : NULL;

    i4_res = rm_open_comp(h_rm_obj,
                          OBJ_COMP_REL_IGNORE,
                          DRVT_BUF_AGENT,
                          ANY_COMP_ID,
                          ps_buffer_agent_name,
                          CTRL_TYPE_SHARED,
                          (VOID*) & t_select, /* pv_comp_sel_data */
                          cm_comp_rm_select, /* pf_comp_sel */
                          (VOID*) pt_conn->h_ch_conn, /* tag */
                          playback_comp_rm_buffer_agent_nfy,
                          & pt_buf_agt->h_rm_buffer_agent,
                          & e_rm_cond);

    switch (i4_res)
    {
        case RMR_OK:
        {
            e_rm_cond = rm_cond_chg(e_rm_cond,
                                    RM_COND_IGNORE,
                                    COND_GRP_OBJ_STATE);

            if (e_rm_cond == RM_COND_OPENED)
            {
                playback_buffer_agent_opened(pt_conn, pt_buf_agt);
            }
            else if (t_select.b_would_conflict)
            {
                cm_nfy(pt_conn->h_cm_conn,
                       CM_COND_CONNECTION_INFO,
                       CM_CONN_INFO_COMP_BUSY);
            }

            i4_res = CMR_OK;
        }
        break;

        case RMR_ASYNC_NFY:
        case RMR_ARBITRATION:
            i4_res = CMR_OK;
            break;

        case RMR_NO_COMP_FOUND:
        case RMR_INV_NAME:
            ui4_reason =
                t_select.b_would_conflict ? PLAYBACK_DISC_COMP_BUSY :
                PLAYBACK_DISC_NO_COMP_FOUND;
            i4_res = CMR_NO_COMP_FOUND;
            break;

        case RMR_OUT_OF_HANDLES:
            i4_res = CMR_OUT_OF_HANDLES;
            break;

        case RMR_NOT_ENOUGH_MEM:
            i4_res = CMR_NOT_ENOUGH_MEM;
            break;

        default:
            i4_res = CMR_FAIL;
    }

    if (i4_res != CMR_OK)
    {
        DBG_ERROR((DBG_PREFIX"Could not open buffer agent (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_res));

        playback_disconnect_conn(pt_conn, ui4_reason);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_open_all_buffer_agents
 *
 * Description: This API tries to open all the buffer agents.
 *
 * Inputs:  pt_conn   References the connection structure.
 *          h_rm_obj  Contains the handle the buffer agents should be open on
 *                    (either a pipe or a virtual source).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_open_all_buffer_agents (PLAYBACK_CONN_T*  pt_conn,
                                             HANDLE_T          h_rm_obj)
{
    PLAYBACK_BUF_AGT_T*  pt_buf_agt;

    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

    while (pt_buf_agt != NULL)
    {
        if (! pt_conn->b_disconnecting)
        {
            playback_open_buffer_agent(pt_conn, pt_buf_agt, h_rm_obj);
        }

        pt_buf_agt = pt_buf_agt->pt_next;
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_virtual_source_opened
 *
 * Description: This API is called when the virtual source is opened. It will
 *              then try to open all the buffer agents.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_virtual_source_opened (PLAYBACK_CONN_T*     pt_conn)
{
    if (! pt_conn->b_virtual_source_opened)
    {
        DBG_INFO((DBG_PREFIX"Virtual source now opened (%d)\n",
                  pt_conn->h_ch_conn));

        pt_conn->b_virtual_source_opened = TRUE;

        playback_open_all_buffer_agents(pt_conn, pt_conn->h_rm_virtual_source);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_comp_rm_virtual_source_nfy
 *
 * Description: This API is called by the Resource Manager to notify the
 *              handler of changes in the state or condition of the driver
 *              component.
 *
 * Inputs:  h_comp      Contains a handle to the tuner driver component.
 *          ui1_port    Contains the port number.
 *          e_nfy_cond  Contains the notify condition.
 *          pv_tag      Contains the tag value specified in rm_open_comp.
 *          ui4_data    Contains additional data (depends on e_nfy_cond).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_comp_rm_virtual_source_nfy (HANDLE_T   h_comp,
                                                 UINT8      ui1_port,
                                                 RM_COND_T  e_nfy_cond,
                                                 VOID*      pv_tag,
                                                 UINT32     ui4_data)
{
    HANDLE_T   h_ch_conn = (HANDLE_T) pv_tag;
    RM_COND_T  e_grp_cond;

    DBG_INFO((DBG_PREFIX"Virtual source RM nfy 0x%08x (data %d) (%d)\n",
              e_nfy_cond, ui4_data, h_ch_conn));

    e_grp_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE, COND_GRP_OPERATION);

    if (e_grp_cond == RM_COND_CLIENT_GET_OPERATION)
    {
        GET_CLIENT_NFY_T*  pt_get;

        pt_get = (GET_CLIENT_NFY_T*) ui4_data;

        if (pt_get != NULL)
        {
            pt_get->i4_return = RMR_INV_GET;

            switch (pt_get->e_get_type)
            {
                default:
                    break;
            }
        }
    }
    else if (e_grp_cond == RM_COND_IGNORE)
    {
        RM_COND_T  e_rm_cond;
        UINT32     ui4_code;

        ui4_code = PLAYBACK_HANDLER_MSG_NONE;

        /* only OPENED and CLOSED are relevant */

        e_rm_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE,
                                COND_GRP_OBJ_STATE);

        switch (e_rm_cond)
        {
            case RM_COND_OPENED:
                ui4_code = PLAYBACK_HANDLER_MSG_VIRTUAL_SOURCE_OPENED;
                break;

            case RM_COND_CLOSED:
                ui4_code = PLAYBACK_HANDLER_MSG_VIRTUAL_SOURCE_CLOSED;
                break;

            default:
                break;
        }

        if (ui4_code != PLAYBACK_HANDLER_MSG_NONE)
        {
            cm_send_msg_retry(h_playback_handler_msgq,
                              ui4_code, ui4_data, (UINT32) h_comp,
                              h_ch_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_open_virtual_source
 *
 * Description: This API opens a virtual source component.
 *
 * Inputs:  pt_conn     References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_open_virtual_source (PLAYBACK_CONN_T*  pt_conn)
{
    CM_COMP_RM_SELECT_T  t_select;
    CHAR*                ps_source_name;
    HANDLE_T             h_rm_pipe;
    RM_COND_T            e_rm_cond;
    UINT32               ui4_reason;
    INT32                i4_res;

    DBG_INFO((DBG_PREFIX"Opening virtual source (%d)...\n",
              pt_conn->h_ch_conn));

    ui4_reason = PLAYBACK_DISC_OTHER_REASON;
    i4_res     = CMR_OK;

    cm_comp_rm_select_init(& t_select);

    t_select.b_select_if_conflict =
        ! pt_conn->t_connect_info.b_disc_if_comp_busy;
    t_select.b_would_conflict = FALSE;

    ps_source_name =
        (pt_conn->t_connect_info.s_virtual_source_name[0] != '\0') ?
        pt_conn->t_connect_info.s_virtual_source_name : NULL;

    h_rm_pipe = cm_pipe_get_rm_pipe(pt_conn->h_cm_pipe);

    i4_res = rm_open_comp(h_rm_pipe,
                          OBJ_COMP_REL_IGNORE,
                          DRVT_VIRTUAL,
                          ANY_COMP_ID,
                          ps_source_name,
                          CTRL_TYPE_SHARED,
                          (VOID*) & t_select, /* pv_comp_sel_data */
                          cm_comp_rm_select, /* pf_comp_sel */
                          (VOID*) pt_conn->h_ch_conn, /* tag */
                          playback_comp_rm_virtual_source_nfy,
                          & pt_conn->h_rm_virtual_source,
                          & e_rm_cond);

    switch (i4_res)
    {
        case RMR_OK:
        {
            e_rm_cond = rm_cond_chg(e_rm_cond,
                                    RM_COND_IGNORE,
                                    COND_GRP_OBJ_STATE);

            if (e_rm_cond == RM_COND_OPENED)
            {
                playback_virtual_source_opened(pt_conn);
            }
            else if (t_select.b_would_conflict)
            {
                cm_nfy(pt_conn->h_cm_conn,
                       CM_COND_CONNECTION_INFO,
                       CM_CONN_INFO_COMP_BUSY);
            }

            i4_res = CMR_OK;
        }
        break;

        case RMR_ASYNC_NFY:
        case RMR_ARBITRATION:
            i4_res = CMR_OK;
            break;

        case RMR_NO_COMP_FOUND:
        case RMR_INV_NAME:
            ui4_reason =
                t_select.b_would_conflict ? PLAYBACK_DISC_COMP_BUSY :
                PLAYBACK_DISC_NO_COMP_FOUND;
            i4_res = CMR_NO_COMP_FOUND;
            break;

        case RMR_OUT_OF_HANDLES:
            i4_res = CMR_OUT_OF_HANDLES;
            break;

        case RMR_NOT_ENOUGH_MEM:
            i4_res = CMR_NOT_ENOUGH_MEM;
            break;

        default:
            i4_res = CMR_FAIL;
    }

    if (i4_res != CMR_OK)
    {
        DBG_ERROR((DBG_PREFIX"Could not open virtual source (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_res));

        playback_disconnect_conn(pt_conn, ui4_reason);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_open_conn
 *
 * Description: This API starts the actual connection process by opening the
 *              component(s). Currently only a buffer agent component is being
 *              opened.
 *
 * Inputs:  pt_conn    References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_open_conn (PLAYBACK_CONN_T*  pt_conn)
{
    /*
      pt_conn->t_connect_info.pt_buf_agt_head cannot be NULL since a buffer
      agent is always created.
    */

    if ((pt_conn->t_connect_info.pt_buf_agt_head->pt_next != NULL)
        ||
        ((pt_conn->ui4_flags & PLAYBACK_VIRTUAL_SOURCE_NAME_MASK) != 0))
    {
        playback_open_virtual_source(pt_conn);
    }
    else
    {
        HANDLE_T  h_rm_pipe;

        h_rm_pipe = cm_pipe_get_rm_pipe(pt_conn->h_cm_pipe);

        playback_open_all_buffer_agents(pt_conn, h_rm_pipe);
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_free_handle
 *
 * Description: This API is called when an playback connection handle is freed.
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
static BOOL playback_conn_free_handle (HANDLE_T       h_ch_conn,
                                       HANDLE_TYPE_T  e_type,
                                       VOID*          pv_obj,
                                       VOID*          pv_tag,
                                       BOOL           b_req_handle)
{
    BOOL  b_res;

    DBG_INFO((DBG_PREFIX"Deleting connection handle (%d).\n",
              h_ch_conn));

    if (! b_req_handle)
    {
        PLAYBACK_CONN_T*     pt_conn;
        PLAYBACK_BUF_AGT_T*  pt_buf_agt;
        INT32                i4_res;

        pt_conn = (PLAYBACK_CONN_T*) pv_obj;    
        
        pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

        while (pt_buf_agt != NULL)
        {
            /* release all data requests without notifying buffer agent */
            playback_conn_cancel_all_requests(pt_conn, pt_buf_agt, FALSE);

            pt_buf_agt = pt_buf_agt->pt_next;
        }

        /* release all requests that have not been consumed yet */
        playback_conn_release_all_unconsumed_requests(pt_conn);

        /* release all playback items */
        playback_conn_empty_queue(pt_conn, TRUE, FALSE);

        /* delete all tables and clocks */
        pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

        while (pt_buf_agt != NULL)
        {
            playback_conn_delete_clock(pt_buf_agt);

            playback_conn_delete_tables(pt_conn, pt_buf_agt);

            pt_buf_agt = pt_buf_agt->pt_next;
        }

        /* delete SCDB */
        if (pt_conn->h_scdb != NULL_HANDLE)
        {
            DBG_INFO((DBG_PREFIX"Deleting SCDB (%d)\n", pt_conn->h_ch_conn));

            i4_res = scdb_delete(pt_conn->h_scdb);

            if (i4_res != SCDBR_OK)
            {
                DBG_ERROR((DBG_PREFIX"Failed to delete SCDB (%d, res=%d)\n",
                           pt_conn->h_ch_conn, i4_res));
            }
        }

        cm_send_msg_retry(h_playback_handler_msgq,
                          PLAYBACK_HANDLER_MSG_DECREASE_CONN_COUNT, 0, 0,
                          NULL_HANDLE);

        playback_handler_delink_conn(pt_conn);

        /* freeing all buffer agents that were dynamically allocated */
        pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

        while (pt_buf_agt != NULL)
        {
            PLAYBACK_BUF_AGT_T*  pt_next;

            pt_next = pt_buf_agt->pt_next;

            handle_free(pt_buf_agt->h_buffer_agent, FALSE);

            pt_buf_agt = pt_next;
        }

        /* freeing object itself */
        x_mem_free(pv_obj);

        b_res = TRUE;
    }
    else
    {
        DBG_INFO((DBG_PREFIX"Not allowed to delete handle (%d)\n", h_ch_conn));

        b_res = FALSE;
    }

    return b_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_disconnect
 *
 * Description: This API is called by the Connection Manager to perform a
 *              disconnection.
 *
 * Inputs:  h_ch_conn  Contains a handle to a playback handler's connection.
 *
 * Outputs: pe_cond  Contains the connection condition (DISCONNECTING, etc).
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  Disconnection failed.
 ----------------------------------------------------------------------------*/
static INT32 playback_disconnect (HANDLE_T    h_ch_conn,
                                  CM_COND_T*  pe_cond)
{
    INT32  i4_res;

    i4_res =
        cm_send_msg_retry(h_playback_handler_msgq,
                          PLAYBACK_HANDLER_MSG_DISCONNECT_REQUEST,
                          PLAYBACK_DISC_AS_REQUESTED, 0,
                          h_ch_conn);

    if (i4_res == CMR_OK)
    {
        *pe_cond = CM_COND_DISCONNECTING;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_connect
 *
 * Description: This API is called by the Connection Manager to perform a
 *              connection.
 *
 * Inputs:  h_cm_conn         Contains a handle to the Connection Manager's
 *                            connection.
 *          h_parent_ch_conn  Contains the handler connection handle to the
 *                            parent connection (should be NULL_HANDLE).
 *          pt_commands       References a CONN_END-terminated array of
 *                            commands.
 *
 * Outputs: ph_ch_conn  Contains a handle to the handler's connection.
 *          pe_cond     Contains the connection condition (CONNECTING, etc).
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  Connection failed.
 ----------------------------------------------------------------------------*/
static INT32 playback_connect (HANDLE_T             h_cm_conn,
                               HANDLE_T*            ph_ch_conn,
                               HANDLE_T             h_parent_ch_conn,
                               const CM_COMMAND_T*  pt_commands,
                               CM_COND_T*           pe_cond)
{
    PLAYBACK_CONNECT_INFO_T  t_connect_info;
    UINT32                   ui4_flags;
    INT32                    i4_res;

    DBG_INFO((DBG_PREFIX"Connecting... (CM connection %d)\n",
              h_cm_conn));

    x_memset(& t_connect_info, 0, sizeof(t_connect_info));

    t_connect_info.b_disc_if_comp_busy  = TRUE;
    t_connect_info.b_disc_if_file_error = TRUE;

    ui4_flags = 0;

    if (pt_commands != NULL)
    {
        i4_res = playback_handler_parse_commands(& t_connect_info,
                                                 & ui4_flags,
                                                 pt_commands);

        /* a connection always needs at least one buffer agent */

        if ((i4_res == CMR_OK) && (t_connect_info.pt_buf_agt_head == NULL))
        {
            t_connect_info.pt_buf_agt_head = playback_buf_agt_create("");

            if (t_connect_info.pt_buf_agt_head == NULL)
            {
                i4_res = CMR_FAIL;
            }
        }
    }
    else
    {
        i4_res = CMR_INV_ARG;
    }

    if (i4_res == CMR_OK)
    {
        HANDLE_T          h_ch_conn;
        PLAYBACK_CONN_T*  pt_conn;

        i4_res = handle_alloc_and_obj(PHT_CONNECTION,
                                      NULL,
                                      sizeof(PLAYBACK_CONN_T),
                                      playback_conn_free_handle,
                                      & h_ch_conn,
                                      (VOID**) & pt_conn);

        if ((i4_res == HR_OK) && (pt_conn != NULL))
        {
            DBG_INFO((DBG_PREFIX"Created connection %d (CM connection=%d)\n",
                      h_ch_conn, h_cm_conn));

            pt_conn->t_connect_info = t_connect_info;

            pt_conn->ui4_disconnection_reason = PLAYBACK_DISC_OTHER_REASON;

            pt_conn->h_cm_pipe            = cm_connection_get_pipe(h_cm_conn);
            pt_conn->h_cm_conn            = h_cm_conn;
            pt_conn->h_ch_conn            = h_ch_conn;
            pt_conn->ui4_flags            = ui4_flags;

            i4_res = playback_conn_create_scdb(pt_conn);

            if (i4_res == CMR_OK)
            {
                PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                UINT32               ui4_index;

                pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;
                ui4_index  = 0;

                while (pt_buf_agt != NULL)
                {
                    /* Tags for the CC1-CC4 (closed caption) streams */
                    pt_buf_agt->pv_cc_stream_tag[0] =
                        playback_get_stream_tag();
                    pt_buf_agt->pv_cc_stream_tag[1] =
                        playback_get_stream_tag();
                    pt_buf_agt->pv_cc_stream_tag[2] =
                        playback_get_stream_tag();
                    pt_buf_agt->pv_cc_stream_tag[3] =
                        playback_get_stream_tag();

                    pt_buf_agt->h_ch_conn = h_ch_conn;
                    pt_buf_agt->ui4_index = ui4_index++;

                    pt_buf_agt = pt_buf_agt->pt_next;
                }

                i4_res =
                    cm_send_msg_retry(h_playback_handler_msgq,
                                      PLAYBACK_HANDLER_MSG_CONNECT_REQUEST,
                                      0, 0,
                                      h_ch_conn);

                if (i4_res == CMR_OK)
                {
                    *ph_ch_conn = h_ch_conn;

                    *pe_cond = CM_COND_CONNECTING;
                }
                else
                {
                    handle_free(h_ch_conn, FALSE);
                }
            }
            else
            {
                handle_free(h_ch_conn, FALSE);
            }
        }
        else
        {
            i4_res = CMR_FAIL;
        }
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_get_item_attributes
 *
 * Description: This API gets the attributes of a playback item.
 *
 * Inputs:  pt_info  References a structure containing information about the
 *                   operation.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_conn_get_item_attributes (PLAYBACK_GET_INFO_T*  pt_info)
{
    PLAYBACK_CONN_T*  pt_conn;
    HANDLE_TYPE_T     e_type;
    INT32             i4_res;

    i4_res = handle_get_type_obj(pt_info->h_ch_conn,
                                 & e_type,
                                 (VOID**) & pt_conn);

    if ((i4_res == HR_OK) && (e_type == PHT_CONNECTION))
    {
        PLAYBACK_HANDLER_ITEM_ATTRIBUTES_T*  pt_att;
        PLAYBACK_BUF_AGT_T*                  pt_buf_agt;
        PLAYBACK_ITEM_T*                     pt_item;

        pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;
        pt_item    = NULL;

        if (pt_info->e_ctrl_type == PLAYBACK_CTRL_GET_ITEM_ATTRIBUTES)
        {
            pt_att = (PLAYBACK_HANDLER_ITEM_ATTRIBUTES_T*) pt_info->pv_info;

            if (pt_att->ui4_id != PLAYBACK_HANDLER_INVALID_ITEM_ID)
            {
                while (pt_buf_agt != NULL)
                {
                    pt_item = pt_buf_agt->pt_items_head;

                    while (pt_item != NULL)
                    {
                        if (pt_item->ui4_id == pt_att->ui4_id)
                        {
                            break;
                        }

                        pt_item = pt_item->pt_next;
                    }

                    if (pt_item != NULL)
                    {
                        break;
                    }

                    pt_buf_agt = pt_buf_agt->pt_next;
                }
            }
            else
            {
                pt_item = pt_buf_agt->pt_items_head;
            }
        }
        else /* PLAYBACK_CTRL_GET_ITEM_ATTRIBUTES_BY_INDEX */
        {
            PLAYBACK_HANDLER_ITEM_ATTRIBUTES_BY_INDEX_T*  pt_tmp;

            pt_tmp = (PLAYBACK_HANDLER_ITEM_ATTRIBUTES_BY_INDEX_T*)
                pt_info->pv_info;

            pt_att = & pt_tmp->t_attributes;

            pt_buf_agt =
                playback_conn_get_buf_agt_by_index(pt_conn,
                                                   pt_tmp->ui4_buf_agt_index);

            if (pt_buf_agt != NULL)
            {
                UINT32  ui4_index;

                pt_item = pt_buf_agt->pt_items_head;

                ui4_index = pt_tmp->ui4_index;

                while (pt_item != NULL)
                {
                    if ((! pt_tmp->b_ignore_unqueued)
                        ||
                        (pt_item->e_state != PLAYBACK_ITEM_STATE_UNQUEUED))
                    {
                        if (ui4_index-- == 0)
                        {
                            break;
                        }
                    }

                    pt_item = pt_item->pt_next;
                }
            }
        }

        if (pt_item != NULL)
        {
            BUF_AGT_POSITION_T        t_position;
            SIZE_T                    z_size;

            pt_att->ui4_id                 = pt_item->ui4_id;
            pt_att->ui8_start_offset       = pt_item->ui8_start_offset;
            pt_att->ui8_end_offset         = pt_item->ui8_end_offset;
            pt_att->ui8_transfer_position  = pt_item->ui8_position;
            pt_att->ui8_transfer_position += pt_item->ui4_offset;
            pt_att->ui8_consumed_position  = pt_att->ui8_transfer_position;

            z_size = sizeof(BUF_AGT_POSITION_T);

            i4_res = rm_get(pt_buf_agt->h_rm_buffer_agent,
                            FALSE,
                            ANY_PORT_NUM,
                            BUF_AGT_GET_TYPE_POSITION,
                            (VOID*) & t_position,
                            & z_size,
                            NULL);

            if (i4_res == RMR_OK)
            {
                PLAYBACK_DATA_REQUEST_T*  pt_request;

                DBG_INFO((DBG_PREFIX
                          "Request %u being consumed, offset %u (%d)\n",
                          t_position.ui4_current_req_id,
                          t_position.ui4_current_byte_offset,
                          pt_conn->h_ch_conn));

                pt_request = pt_buf_agt->pt_unconsumed_requests;

                while (pt_request != NULL)
                {
                    if ((pt_request->pt_item == pt_item)
                        &&
                        (pt_request->ui4_req_id
                         ==
                         t_position.ui4_current_req_id))
                    {
                        pt_att->ui8_consumed_position =
                            pt_request->ui8_transfer_position +
                            t_position.ui4_current_byte_offset;

                        break;
                    }

                    pt_request = pt_request->pt_next;
                }
            }

            pt_att->ui4_flags =
                PLAYBACK_HANDLER_START_OFFSET_FLAG |
                PLAYBACK_HANDLER_END_OFFSET_FLAG |
                PLAYBACK_HANDLER_POSITION_FLAG;

            i4_res = CMR_OK;
        }
        else
        {
            i4_res = CMR_FAIL;
        }
    }
    else
    {
        i4_res = CMR_FAIL;
    }

    pt_info->i4_res = i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_get_src_comp
 *
 * Description: This API finds the source component given a specific stream
 *              tag.
 *
 * Inputs:  pt_src_comp  References the source component structure.
 *          h_ch_conn    Contains a handle to a connection.
 *          h_scdb       Contains the handle to the connection's SCDB.
 *
 * Outputs: pt_src_comp  Contains the source handle as well upon success.
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  "get" operation failed.
 ----------------------------------------------------------------------------*/
static INT32 playback_get_src_comp (CM_CTRL_RM_SRC_COMP_T*  pt_src_comp,
                                    HANDLE_T                h_ch_conn,
                                    HANDLE_T                h_scdb)
{
    INT32  i4_res;

    i4_res = scdb_lock(h_scdb);

    if (i4_res == SCDBR_OK)
    {
        STREAM_COMP_ID_T  t_stream_comp_id;
        SCDB_REC_T        t_scdb_rec;
        UINT32            ui4_ver_id;
        UINT16            ui2_index;

        ui4_ver_id = SCDB_NULL_VER_ID;
        ui2_index  = 0;

        while (x_scdb_get_rec_by_idx(h_scdb,
                                     ST_UNKNOWN,
                                     ui2_index,
                                     & t_stream_comp_id,
                                     & t_scdb_rec,
                                     & ui4_ver_id) == SCDBR_OK)
        {
            if (t_stream_comp_id.pv_stream_tag == pt_src_comp->pv_stream_tag)
            {
                CRIT_STATE_T      t_state;
                PLAYBACK_CONN_T*  pt_conn;
                UINT32            ui4_index;
                HANDLE_TYPE_T     e_type;

                ui4_index = CM_GET_SRC_INDEX(t_scdb_rec.ui4_private);

                t_state = x_crit_start();

                i4_res = handle_get_type_obj(h_ch_conn,
                                             & e_type,
                                             (VOID**) & pt_conn);

                if ((i4_res == HR_OK) && (e_type == PHT_CONNECTION))
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;

                    pt_buf_agt = playback_conn_get_buf_agt_by_index(pt_conn,
                                                                    ui4_index);

                    if (pt_buf_agt != NULL)
                    {
                        pt_src_comp->h_rm_comp = pt_buf_agt->h_rm_buffer_agent;

                        i4_res = CMR_OK;
                    }
                }
                else
                {
                    i4_res = CMR_FAIL;
                }

                x_crit_end(t_state);

                break;
            }

            ui4_ver_id = SCDB_NULL_VER_ID;
            ui2_index++;
        }

        scdb_unlock(h_scdb);
    }
    else
    {
        i4_res = CMR_FAIL;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_get
 *
 * Description: This API gets information about a connection.
 *
 * Inputs:  h_conn        Contains a handle to the handler's connection.
 *          e_ctrl_type   Contains the control code.
 *
 * Outputs: pv_ctrl_data  Contains information about the connection.
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  "get" operation failed.
 ----------------------------------------------------------------------------*/
static INT32 playback_get (HANDLE_T        h_ch_conn,
                           CM_CTRL_TYPE_T  e_ctrl_type,
                           VOID*           pv_ctrl_data)
{
    INT32  i4_res;
    PLAYBACK_CONN_T*  pt_conn;
    HANDLE_TYPE_T     e_type;
    CRIT_STATE_T      t_state;
    INT32             i4_handle_res;

    i4_res = CMR_FAIL;

    switch (e_ctrl_type)
    {
        case PLAYBACK_CTRL_GET_BUFFER_FULLNESS:
        {
            i4_res =
                cm_send_msg_retry(h_playback_handler_msgq,
                                  PLAYBACK_HANDLER_CIRCULAR_FULLNESS,
                                  (UINT32) pv_ctrl_data, 0,
                                  h_ch_conn);

            if (i4_res == CMR_OK)
            {
                /* operation is made synchronous with this semaphore */
                x_sema_lock(h_playback_handler_sema, X_SEMA_OPTION_WAIT);
            }
        }
        break;
        case PLAYBACK_CTRL_GET_BUFFER_STATUS:
        {
            i4_res =
                cm_send_msg_retry(h_playback_handler_msgq,
                                  PLAYBACK_HANDLER_CIRCULAR_STATUS,
                                  (UINT32) pv_ctrl_data, 0,
                                  h_ch_conn);

            if (i4_res == CMR_OK)
            {
                /* operation is made synchronous with this semaphore */
                x_sema_lock(h_playback_handler_sema, X_SEMA_OPTION_WAIT);
            }
        }
        break;

        case PLAYBACK_CTRL_GET_TIME_TO_SIZE:
        {
            i4_res =
                cm_send_msg_retry(h_playback_handler_msgq,
                                  PLAYBACK_HANDLER_TIME_TO_SIZE,
                                  (UINT32) pv_ctrl_data, 0,
                                  h_ch_conn);

            if (i4_res == CMR_OK)
            {
                /* operation is made synchronous with this semaphore */
                x_sema_lock(h_playback_handler_sema, X_SEMA_OPTION_WAIT);
            }
        }
        break;

        case CM_CTRL_RM_SRC_COMP:
        {
            t_state = x_crit_start();

            i4_handle_res = handle_get_type_obj(h_ch_conn,
                                                & e_type,
                                                (VOID**) & pt_conn);

            if ((i4_handle_res == HR_OK) && (e_type == PHT_CONNECTION))
            {
                CM_CTRL_RM_SRC_COMP_T*  pt_src_comp;
                PLAYBACK_BUF_AGT_T*     pt_buf_agt;

                pt_src_comp = (CM_CTRL_RM_SRC_COMP_T*) pv_ctrl_data;

                pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

                if (pt_buf_agt != NULL)
                {
                    if (pt_buf_agt->pt_next == NULL)
                    {
                        pt_src_comp->h_rm_comp = pt_buf_agt->h_rm_buffer_agent;

                        x_crit_end(t_state);

                        i4_res = CMR_OK;
                    }
                    else
                    {
                        HANDLE_T  h_scdb;

                        h_scdb = pt_conn->h_scdb;

                        x_crit_end(t_state);

                        i4_res = playback_get_src_comp(pt_src_comp,
                                                       h_ch_conn, h_scdb);
                    }
                }
                else
                {
                    x_crit_end(t_state);
                }
            }
            else
            {
                x_crit_end(t_state);
            }
        }
        break;

        case PLAYBACK_CTRL_GET_ITEM_ATTRIBUTES:
        case PLAYBACK_CTRL_GET_ITEM_ATTRIBUTES_BY_INDEX:
        {
            PLAYBACK_GET_INFO_T  t_info;

            t_info.h_ch_conn   = h_ch_conn;
            t_info.e_ctrl_type = e_ctrl_type;
            t_info.pv_info     = pv_ctrl_data;

            /* set here, but another thread will change the value */
            t_info.i4_res = CMR_FAIL;

            i4_res =
                cm_send_msg_retry(h_playback_handler_msgq,
                                  PLAYBACK_HANDLER_MSG_GET_ITEM_ATTRIBUTES,
                                  (UINT32) & t_info, 0,
                                  NULL_HANDLE);

            if (i4_res == CMR_OK)
            {
                /* operation is made synchronous with this semaphore */
                x_sema_lock(h_playback_handler_sema, X_SEMA_OPTION_WAIT);

                i4_res = t_info.i4_res;
            }
        }
        break;

#ifdef TIME_SHIFT_SUPPORT
        case PLAYBACK_CTRL_GET_CURR_UTC:
        {
            t_state = x_crit_start();

            i4_handle_res = handle_get_type_obj(h_ch_conn,
                                                & e_type,
                                                (VOID**) & pt_conn);

            if ((i4_handle_res == HR_OK) && (e_type == PHT_CONNECTION))
            {
                PLAYBACK_BUF_AGT_T*    pt_buf_agt;

                pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

                if (pt_buf_agt != NULL)
                {
                    TIME_T*      pt_current_time = (TIME_T*)pv_ctrl_data;
                    DT_COND_T    e_cond;

                    *pt_current_time = x_dt_clock_get_utc(pt_buf_agt->h_clock, NULL, & e_cond);

                    if (e_cond == DT_FREE_RUNNING || e_cond == DT_SYNC_RUNNING)
                    {
                        i4_res = CMR_OK;
                    }
                }
            }

            x_crit_end(t_state);
        }
        break;
#endif

        case PLAYBACK_CTRL_GET_SHARED_MEMORY:
        {
            /*zink: share memeory*/
            i4_res =
                cm_send_msg_retry(h_playback_handler_msgq,
                                  PLAYBACK_HANDLER_SHARED_MEMORY,
                                  (UINT32) pv_ctrl_data, 0,
                                  h_ch_conn);

            if (i4_res == CMR_OK)
            {
                /* operation is made synchronous with this semaphore */
                x_sema_lock(h_playback_handler_sema, X_SEMA_OPTION_WAIT);
            }
        }
        break;
        
        case PLAYBACK_CTRL_GET_HTTP_DOWNLOAD_SPEED:
        {
            i4_res =
                cm_send_msg_retry(h_playback_handler_msgq,
                                  PLAYBACK_HANDLER_HTTP_DOWNLOAD_SPEED,
                                  (UINT32) pv_ctrl_data, 0,
                                  h_ch_conn);
            if (i4_res == CMR_OK)
            {
                /* operation is made synchronous with this semaphore */
                x_sema_lock(h_playback_handler_sema, X_SEMA_OPTION_WAIT);
            }
        }
        break;
        default:
            i4_res = CMR_INV_ARG;
            break;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_set_item_position_and_reset
 *
 * Description: This API sets the position of a given playback item and may
 *              reset the state of the playback items and requests.
 *
 * Inputs:  pt_conn       References the connection.
 *          pt_item       References the playback item.
 *          ui8_position  Contains the new position.
 *          b_reset       Specifies whether items and requests should be reset.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID
playback_conn_set_item_position_and_reset (PLAYBACK_CONN_T*     pt_conn,
                                           PLAYBACK_BUF_AGT_T*  pt_buf_agt,
                                           PLAYBACK_ITEM_T*     pt_item,
                                           UINT64               ui8_position,
                                           BOOL                 b_reset)
{
    UINT64  ui8_current_pos;
    INT32   i4_res = 0;

    playback_conn_set_item_position(pt_conn, pt_item, ui8_position);

    if (b_reset)
    {
        PLAYBACK_DATA_REQUEST_T*  pt_request;

        /* transfer will start from this item again */

        pt_buf_agt->pt_transferring_item = pt_item;

        playback_conn_nfy_align_info(pt_buf_agt,
                                     pt_buf_agt->pt_transferring_item);

        /* all following requests are reset: b_last is reset to FALSE */

        pt_request = pt_buf_agt->pt_unconsumed_requests;

        while (pt_request != NULL)
        {
            if (pt_request->pt_item == pt_item)
            {
                do
                {
                    pt_request->b_last = FALSE;

                    pt_request = pt_request->pt_next;
                }
                while (pt_request != NULL);

                break;
            }

            pt_request = pt_request->pt_next;
        }

        /* all following items are reset to their start positions */

        pt_item = pt_item->pt_next;

        while (pt_item != NULL)
        {
            pt_item->ui8_position = pt_item->ui8_start_offset;

            if (pt_item->e_type == PLAYBACK_ITEM_TYPE_PULL)
            {
                UINT32  ui4_seek_alignment;
                UINT64  ui8_cur_pos = 0;

                ui4_seek_alignment = pt_item->u.t_pull.ui4_block_size;

                pt_item->ui4_offset = (ui4_seek_alignment != 0) ?
                    (UINT32) (pt_item->ui8_position % ui4_seek_alignment) : 0;

                pt_item->ui8_position -= pt_item->ui4_offset;

                DBG_INFO((DBG_PREFIX
                          "Seek to position %llu (+%u) (id:%u) (%d)\n",
                          pt_item->ui8_position, pt_item->ui4_offset,
                          pt_item->ui4_id, pt_conn->h_ch_conn));

                i4_res = pt_item->u.t_pull.t_pull_info.pf_byteseek(
                                pt_item->u.t_pull.h_pull,
                                pt_item->u.t_pull.t_pull_info.pv_app_tag,
                                pt_item->ui8_position,
                                FM_SEEK_BGN,
                                &ui8_cur_pos);

                if (i4_res != CMR_OK)
                {
                    DBG_ERROR((DBG_PREFIX
                       "Could not return to position %llu (%d, res=%d)\n",
                       pt_item->ui8_start_offset, pt_conn->h_ch_conn, i4_res));
                }

            }
            else if (pt_item->e_type == PLAYBACK_ITEM_TYPE_FILE)
            {
                UINT32  ui4_seek_alignment;

                ui4_seek_alignment = pt_item->u.t_file.ui4_block_size;

                pt_item->ui4_offset = (ui4_seek_alignment != 0) ?
                    (UINT32) (pt_item->ui8_position % ui4_seek_alignment) : 0;

                pt_item->ui8_position -= pt_item->ui4_offset;

                DBG_INFO((DBG_PREFIX
                          "Seek to position %llu (+%u) (id:%u) (%d)\n",
                          pt_item->ui8_position, pt_item->ui4_offset,
                          pt_item->ui4_id, pt_conn->h_ch_conn));

                i4_res = x_fm_mfw_lseek(pt_item->u.t_file.h_file,
                                        (INT64) pt_item->ui8_position,
                                        FM_SEEK_BGN,
                                        & ui8_current_pos);

                if (i4_res != FMR_OK)
                {
                    DBG_ERROR((DBG_PREFIX
                       "Could not return to position %llu (%d, res=%d)\n",
                       pt_item->ui8_start_offset, pt_conn->h_ch_conn, i4_res));
                }
            }
            else if (pt_item->e_type == PLAYBACK_ITEM_TYPE_URI)
            {
                UINT32  ui4_seek_alignment;

                ui4_seek_alignment = pt_item->u.t_uri.ui4_block_size;

                pt_item->ui4_offset = (ui4_seek_alignment != 0) ?
                    (UINT32) (pt_item->ui8_position % ui4_seek_alignment) : 0;

                pt_item->ui8_position -= pt_item->ui4_offset;

                DBG_INFO((DBG_PREFIX
                          "Seek to position %llu (+%u) (id:%u) (%d)\n",
                          pt_item->ui8_position, pt_item->ui4_offset,
                          pt_item->ui4_id, pt_conn->h_ch_conn));

#ifdef ENABLE_MMS_SUPPORT
                if (pt_item->u.t_uri.b_mms)
                {
                    i4_res = FMR_OK;
                }
                else 
#endif
				if (pt_item->u.t_uri.b_url)
                {
#ifdef INET_SUPPORT
                    i4_res = x_httpc_fm_lseek(pt_item->u.t_uri.h_uri,
                                              (INT64) pt_item->ui8_position,
                                              FM_SEEK_BGN,
                                              & ui8_current_pos);
#endif
                }
                else
                {
                    i4_res = x_fm_mfw_lseek(pt_item->u.t_uri.h_uri,
                                            (INT64) pt_item->ui8_position,
                                            FM_SEEK_BGN,
                                            & ui8_current_pos);
                }
                if (i4_res != FMR_OK)
                {
                    DBG_ERROR((DBG_PREFIX
                       "Could not return to position %llu (%d, res=%d)\n",
                       pt_item->ui8_start_offset, pt_conn->h_ch_conn, i4_res));
                }
            }

            pt_item = pt_item->pt_next;
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_set_item_attributes
 *
 * Description: This API sets the attributes of a given playback item.
 *
 * Inputs:  pt_info  References a structure containing information about the
 *                   operation.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_set_item_attributes (PLAYBACK_SET_INFO_T*  pt_info)
{
    PLAYBACK_CONN_T*  pt_conn;
    HANDLE_TYPE_T     e_type;
    INT32             i4_res;

    i4_res = handle_get_type_obj(pt_info->h_ch_conn,
                                 & e_type, (VOID**) & pt_conn);

    if ((i4_res == HR_OK) && (e_type == PHT_CONNECTION))
    {
        PLAYBACK_HANDLER_ITEM_ATTRIBUTES_T*  pt_att;
        PLAYBACK_BUF_AGT_T*                  pt_buf_agt;

        pt_att = (PLAYBACK_HANDLER_ITEM_ATTRIBUTES_T*) pt_info->pv_info;

        pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

        while (pt_buf_agt != NULL)
        {
            PLAYBACK_ITEM_T*  pt_item;
            BOOL              b_transferred;

            pt_item       = pt_buf_agt->pt_items_head;
            b_transferred = TRUE;

            DBG_INFO((DBG_PREFIX"Setting attributes of item %u (%d)\n",
                      pt_att->ui4_id, pt_info->h_ch_conn));

            while (pt_item != NULL)
            {
                if (pt_item == pt_buf_agt->pt_transferring_item)
                {
                    b_transferred = FALSE;
                }

                if (pt_item->ui4_id == pt_att->ui4_id)
                {
                    break;
                }

                pt_item = pt_item->pt_next;
            }

            if (pt_item != NULL)
            {
                if (pt_att->ui4_flags & PLAYBACK_HANDLER_START_OFFSET_FLAG)
                {
                    DBG_INFO((DBG_PREFIX
                              "New start offset of item %u: %llu (%d)\n",
                              pt_att->ui4_id,
                              pt_att->ui8_start_offset,
                              pt_info->h_ch_conn));

                    pt_item->ui8_start_offset = pt_att->ui8_start_offset;
                }

                if (pt_att->ui4_flags & PLAYBACK_HANDLER_END_OFFSET_FLAG)
                {
                    DBG_INFO((DBG_PREFIX
                              "New end offset of item %u: %llu (%d)\n",
                              pt_att->ui4_id,
                              pt_att->ui8_end_offset,
                              pt_info->h_ch_conn));

                    pt_item->ui8_end_offset = pt_att->ui8_end_offset;
                }

                if (pt_att->ui4_flags & PLAYBACK_HANDLER_POSITION_FLAG)
                {
                    UINT64  ui8_position;

                    DBG_INFO((DBG_PREFIX
                              "New position in item %u: %llu (%d)\n",
                              pt_att->ui4_id,
                              pt_att->ui8_transfer_position,
                              pt_info->h_ch_conn));

                    ui8_position = pt_att->ui8_transfer_position;

                    playback_conn_set_item_position_and_reset(pt_conn,
                                                              pt_buf_agt,
                                                              pt_item,
                                                              ui8_position,
                                                              b_transferred);
                }

                i4_res = CMR_OK;

                break;
            }

            pt_buf_agt = pt_buf_agt->pt_next;
        }
    }
    else
    {
        i4_res = CMR_FAIL;
    }

    pt_info->i4_res = i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_conn_set_item_svc_info
 *
 * Description: This API sets the service info of a given playback item.
 *
 * Inputs:  pt_info  References a structure containing information about the
 *                   operation.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_conn_set_item_svc_info (PLAYBACK_SET_INFO_T*  pt_info)
{
    PLAYBACK_CONN_T*  pt_conn;
    HANDLE_TYPE_T     e_type;
    INT32             i4_res;

    i4_res = handle_get_type_obj(pt_info->h_ch_conn,
                                 & e_type, (VOID**) & pt_conn);

    if ((i4_res == HR_OK) && (e_type == PHT_CONNECTION))
    {
        PLAYBACK_HANDLER_ITEM_SVC_INFO_BY_ID_T*  pt_svc_info;
        PLAYBACK_BUF_AGT_T*                      pt_buf_agt;
        PLAYBACK_ITEM_T*                         pt_item;

        pt_svc_info = (PLAYBACK_HANDLER_ITEM_SVC_INFO_BY_ID_T*)
            pt_info->pv_info;

        DBG_INFO((DBG_PREFIX"Setting service info of item %u (%d)\n",
                      pt_svc_info->ui4_id, pt_info->h_ch_conn));

        pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;
        pt_item    = NULL;

        while (pt_buf_agt != NULL)
        {
            pt_item = pt_buf_agt->pt_items_head;

            while ((pt_item != NULL)
                   &&
                   (pt_item->ui4_id != pt_svc_info->ui4_id))
            {
                pt_item = pt_item->pt_next;
            }

            if (pt_item != NULL)
            {
                break;
            }

            pt_buf_agt = pt_buf_agt->pt_next;
        }

        if (pt_item != NULL)
        {
            /* FIXME - not implemented yet */

            i4_res = CMR_OK;
        }
    }
    else
    {
        i4_res = CMR_FAIL;
    }

    pt_info->i4_res = i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: playback_set
 *
 * Description: This API sets information about a connection.
 *
 * Inputs:  h_conn        Contains a handle to the handler's connection.
 *          e_ctrl_type   Contains the control code.
 *          pv_ctrl_data  Contains information about the connection.
 *
 * Outputs: pv_ctrl_data  May have been updated.
 *
 * Returns: CMR_OK       Routine successful.
 *          CMR_INV_ARG  Unknown control code or information.
 *          CMR_FAIL     "set" operation failed.
 ----------------------------------------------------------------------------*/
static INT32 playback_set (HANDLE_T        h_ch_conn,
                           CM_CTRL_TYPE_T  e_ctrl_type,
                           VOID*           pv_ctrl_data)
{
    INT32  i4_res;

    i4_res = CMR_FAIL;

    switch (e_ctrl_type)
    {
        case PLAYBACK_CTRL_SET_RELEASE_TABLES:
        {
            PLAYBACK_CONN_T*     pt_conn    = NULL;
            PLAYBACK_BUF_AGT_T*  pt_buf_agt = NULL;
            
            i4_res = handle_get_obj(h_ch_conn, (VOID**) & pt_conn);
            if (pt_conn == NULL)
            {
                x_dbg_stmt("playback_set handle_get_obj fail\n");
                return CMR_FAIL;
            }
            pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

            while (pt_buf_agt != NULL)
            {
                playback_conn_delete_tables(pt_conn, pt_buf_agt);
                pt_buf_agt = pt_buf_agt->pt_next;
            }
            x_dbg_stmt("cm_playbck: release tables.\n");
        }
        break;
        case PLAYBACK_CTRL_SET_MODIFY_SCDB:
        {
            PLAYBACK_HANDLER_ITEM_SCDB_INFO_T* pt_scdb_info;

            pt_scdb_info = x_mem_alloc(sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T));
            if (NULL == pt_scdb_info)
            {
                i4_res = CMR_FAIL;
                break;
            }

            *pt_scdb_info = *((PLAYBACK_HANDLER_ITEM_SCDB_INFO_T*)pv_ctrl_data);
            i4_res = cm_send_msg_retry(h_playback_handler_msgq,
                                       PLAYBACK_HANDLER_MODIFY_SCDB,
                                       (UINT32)pt_scdb_info,
                                       0,
                                       h_ch_conn);
        }
            break;

        case PLAYBACK_CTRL_SET_PRELOAD:
        {
            UINT32 ui4_buf_agt_index;
            ui4_buf_agt_index = (UINT32)pv_ctrl_data;
            i4_res = cm_send_msg_retry(h_playback_handler_msgq,
                                       PLAYBACK_HANDLER_CIRCULAR_PRELOAD,
                                       0,
                                       ui4_buf_agt_index,
                                       h_ch_conn);
        }

            break;

        case PLAYBACK_CTRL_SET_QUEUE_FILE:
        case PLAYBACK_CTRL_SET_QUEUE_BUFFER:
        case PLAYBACK_CTRL_SET_QUEUE_URI:
        case PLAYBACK_CTRL_SET_QUEUE_PULL:
        {
            PLAYBACK_ITEM_T*  pt_item;
            UINT32            ui4_id;
            PLAYBACK_HANDLER_QUEUE_BUFFER_T*  pt_ctrl=0;

            if (e_ctrl_type != PLAYBACK_CTRL_SET_QUEUE_BUFFER)
            {
                ui4_id = playback_conn_assign_id(h_ch_conn);
            }
            else
            {
                pt_ctrl = (PLAYBACK_HANDLER_QUEUE_BUFFER_T *) pv_ctrl_data;
                ui4_id = (UINT32)pt_ctrl->pv_data;
            }

            if (((ui4_id != PLAYBACK_HANDLER_INVALID_ITEM_ID)||((pt_ctrl!=0)&&(pt_ctrl->e_format == DATA_FMT_IBC)))
                &&
                ((pt_item = playback_handler_get_playback_item()) != NULL))
            {
                PLAYBACK_HANDLER_ITEM_SCDB_INFO_T*  pt_scdb_info;
                UINT32                              ui4_nb_records;
                UINT32                              ui4_buf_agt_index;
                BOOL                                b_ok;

                if (e_ctrl_type == PLAYBACK_CTRL_SET_QUEUE_PULL)
                {
                    PLAYBACK_HANDLER_QUEUE_PULL_T*  pt_ctrl;
                    UINT64                          ui8_position;
                    UINT32                          ui4_seek_alignment;

                    pt_ctrl = (PLAYBACK_HANDLER_QUEUE_PULL_T *) pv_ctrl_data;

                    pt_scdb_info      = pt_ctrl->pt_scdb_info;
                    ui4_nb_records    = pt_ctrl->ui4_nb_records;
                    ui4_buf_agt_index = pt_ctrl->ui4_buf_agt_index;

                    pt_ctrl->ui4_id = ui4_id;

                    pt_item->t_extra               = pt_ctrl->t_extra;
                    pt_item->e_format              = pt_ctrl->e_format;
                    pt_item->ui8_start_offset      = pt_ctrl->ui8_start_offset;
                    pt_item->ui8_end_offset        = pt_ctrl->ui8_end_offset;
                    pt_item->u.t_pull.t_pull_info  = pt_ctrl->t_pull_info;
                    pt_item->ui4_flags             = pt_ctrl->ui4_flags;
                    pt_item->ui4_offset            = 0;
                    pt_item->u.t_pull.ui1_priority = 127;
                    pt_item->e_type                = PLAYBACK_ITEM_TYPE_PULL;


                    i4_res = pt_item->u.t_pull.t_pull_info.pf_open(
                                            pt_item->u.t_pull.t_pull_info.h_pb,
                                            &pt_item->u.t_pull.h_pull,
                                            pt_item->u.t_pull.t_pull_info.pv_app_tag);

                    if (i4_res == CMR_OK)
                    {
                        pt_item->u.t_pull.ui4_block_size = 1;
                        pt_item->u.t_pull.ui4_alignment = 1;

                        ui8_position = pt_item->ui8_start_offset;

                        ui4_seek_alignment =
                            pt_item->u.t_pull.ui4_block_size;

                        pt_item->ui4_offset = (ui4_seek_alignment != 0) ?
                            (UINT32) (ui8_position % ui4_seek_alignment) :
                            0;

                        pt_item->ui8_start_offset -= pt_item->ui4_offset;

#if 0 /*not necessary*/
                        i4_res = pt_item->u.t_pull.t_pull_info.pf_byteseek(
                                                pt_item->u.t_pull.h_pull,
                                                pt_item->u.t_pull.t_pull_info.pv_app_tag,
                                                pt_item->ui8_start_offset,
                                                FM_SEEK_BGN,
                                                &ui8_position);

                        if (i4_res != CMR_OK)
                        {
                            DBG_ERROR((DBG_PREFIX
                                       "Could not seek (%d, res=%d)\n",
                                       h_ch_conn, i4_res));
                        }
#endif
                    }
                    else
                    {
                        DBG_ERROR((DBG_PREFIX
                                   "Could not open (%d, res=%d)\n",
                                   h_ch_conn, i4_res));
                    }

                    b_ok = (i4_res == CMR_OK);
                }
                else if (e_ctrl_type == PLAYBACK_CTRL_SET_QUEUE_URI)
                {
                    PLAYBACK_HANDLER_QUEUE_URI_T*  pt_ctrl;
                    HANDLE_T                       h_uri;
                    BOOL                           b_uri_file = FALSE;

                    pt_ctrl = (PLAYBACK_HANDLER_QUEUE_URI_T *) pv_ctrl_data;

                    pt_scdb_info      = pt_ctrl->pt_scdb_info;
                    ui4_nb_records    = pt_ctrl->ui4_nb_records;
                    ui4_buf_agt_index = pt_ctrl->ui4_buf_agt_index;

                    pt_ctrl->ui4_id = ui4_id;

                    pt_item->t_extra               = pt_ctrl->t_extra;
                    pt_item->e_format              = pt_ctrl->e_format;
                    pt_item->ui8_start_offset      = pt_ctrl->ui8_start_offset;
                    pt_item->ui8_end_offset        = pt_ctrl->ui8_end_offset;
                    pt_item->ui4_flags             = pt_ctrl->ui4_flags|CM_PLAYBACK_FLAG_KEEP_UNTIL_UNQUEUED;
                    pt_item->ui4_offset            = 0;
                    pt_item->u.t_uri.h_uri         = NULL_HANDLE;
                    pt_item->u.t_uri.ui1_priority  = 127;
                    pt_item->e_type                = PLAYBACK_ITEM_TYPE_URI;

                    DBG_INFO((DBG_PREFIX"Opening URI \"%s\" (%d)\n",
                              pt_ctrl->ps_path, h_ch_conn));

#ifdef ENABLE_MMS_SUPPORT
                    if (0 == x_strncmp("mms://", pt_ctrl->ps_path, 6) ||
                        0 == x_strncmp("mmst://", pt_ctrl->ps_path, 7))
                    {
                        pt_item->u.t_uri.b_mms = TRUE;
                    }
                    else 
#endif
                    if (0 == x_strncmp("http://", pt_ctrl->ps_path, 7))
                    {
#ifdef INET_SUPPORT
                        pt_item->u.t_uri.b_url = TRUE;
#else
                        break;
#endif
                    }
                    else if (0 == x_strncmp("https://", pt_ctrl->ps_path, 8))
                    {
#ifdef INET_SUPPORT
                        pt_item->u.t_uri.b_url = TRUE;
#else
                        break;
#endif
                    }
                    else if (0 == x_strncmp("file://", pt_ctrl->ps_path, 7))
                    {
                        pt_item->u.t_uri.b_url = FALSE;
                        b_uri_file = TRUE;
                    }
                    else
                    {
                        pt_item->u.t_uri.b_url = FALSE;
                        b_uri_file = FALSE;
                    }

#ifdef ENABLE_MMS_SUPPORT
                    if (pt_item->u.t_uri.b_mms)
                    {
                        if (pt_ctrl->h_uri)
                        {
                            pt_item->u.t_uri.b_close_uri = FALSE;
                            h_uri = pt_ctrl->h_uri;
                            i4_res = FMR_OK;
                        }
                        else
                        {
                            pt_item->u.t_uri.b_close_uri = TRUE;
                            i4_res = x_mms_fm_pull_open(pt_ctrl->ps_path,
                                                         /*PLAYBACK_HANDLER_CIRCULAR_READ_UNIT*2,*/
                                                         PLAYBACK_HANDLER_HTTP_PRE_BUF_SIZE,
                                                         & h_uri);
                        }

                        if (b_log_cmpb)
                        {
                            x_dbg_stmt("[CMPB] playback_set x_mms_fm_pull_open %d\n", i4_res);
                        }

                        if (i4_res == FMR_OK)
                        {
                            UINT64  ui8_position;
                            UINT32  ui4_seek_alignment;

                            pt_item->u.t_uri.h_uri = h_uri;

                            pt_item->u.t_uri.ui4_block_size = 512;
                            pt_item->u.t_uri.ui4_alignment = 32;

                            ui8_position = pt_item->ui8_start_offset;

                            ui4_seek_alignment =
                                pt_item->u.t_uri.ui4_block_size;

                            pt_item->ui4_offset = (ui4_seek_alignment != 0) ?
                                (UINT32) (ui8_position % ui4_seek_alignment) :
                                0;

                            pt_item->ui8_start_offset -= pt_item->ui4_offset;

                        }
                        else
                        {
                            DBG_ERROR((DBG_PREFIX
                                       "Could not open file \"%s\" (%d, res=%d)\n",
                                       pt_ctrl->ps_path, h_ch_conn, i4_res));
                        }
                    }
                    else 
#endif
					if (pt_item->u.t_uri.b_url == FALSE) /*file mode*/
                    {
                        if (b_uri_file)
                        {
                            i4_res = x_fm_mfw_open(FM_ROOT_HANDLE,
                                                   pt_ctrl->ps_path+7,
                                                   FM_READ_ONLY,
                                                   FM_MODE_USR_READ,
                                                   TRUE, /* no cache */
                                                   & h_uri);
                        }
                        else
                        {
                            i4_res = x_fm_mfw_open(FM_ROOT_HANDLE,
                                                   pt_ctrl->ps_path,
                                                   FM_READ_ONLY,
                                                   FM_MODE_USR_READ,
                                                   TRUE, /* no cache */
                                                   & h_uri);
                        }

                        if (i4_res == FMR_OK)
                        {
                            FM_FS_INFO_T  t_fs_info;
                            UINT64        ui8_current_pos;

                            if (b_uri_file)
                            {
                                i4_res = x_fm_mfw_get_fs_info(FM_ROOT_HANDLE,
                                                              pt_ctrl->ps_path+7,
                                                              & t_fs_info);
                            }
                            else
                            {
                                i4_res = x_fm_mfw_get_fs_info(FM_ROOT_HANDLE,
                                                              pt_ctrl->ps_path,
                                                              & t_fs_info);
                            }

                            if (i4_res == FMR_OK)
                            {
                                UINT64  ui8_position;
                                UINT32  ui4_seek_alignment;

                                pt_item->u.t_uri.h_uri = h_uri;

                                pt_item->u.t_uri.ui4_block_size =
                                    t_fs_info.ui4_min_blk_size;
                                pt_item->u.t_uri.ui4_alignment =
                                    t_fs_info.ui4_alignment;

                                ui8_position = pt_item->ui8_start_offset;

                                ui4_seek_alignment =
                                    pt_item->u.t_uri.ui4_block_size;

                                pt_item->ui4_offset = (ui4_seek_alignment != 0) ?
                                    (UINT32) (ui8_position % ui4_seek_alignment) :
                                    0;

                                pt_item->ui8_start_offset -= pt_item->ui4_offset;

                                DBG_INFO((DBG_PREFIX"Block size: %u bytes (%d)\n"
                                          DBG_PREFIX"Alignment: %u bytes (%d)\n",
                                          t_fs_info.ui4_min_blk_size,
                                          h_ch_conn,
                                          t_fs_info.ui4_alignment,
                                          h_ch_conn));

                                i4_res =
                                    x_fm_mfw_lseek(h_uri,
                                                   (INT64) pt_item->ui8_start_offset,
                                                   FM_SEEK_BGN,
                                                   & ui8_current_pos);

                                if (i4_res != FMR_OK)
                                {
                                    DBG_ERROR((DBG_PREFIX
                                               "Could not seek (%d, res=%d)\n",
                                               h_ch_conn, i4_res));
                                }
                            }
                            else
                            {
                                DBG_ERROR((DBG_PREFIX
                                           "Could not get FS info (%d, res=%d)\n",
                                           h_ch_conn, i4_res));
                            }
                        }
                        else
                        {
                            DBG_ERROR((DBG_PREFIX
                                       "Could not open file \"%s\" (%d, res=%d)\n",
                                       pt_ctrl->ps_path, h_ch_conn, i4_res));
                        }
                    }
                    else
                    {
#ifdef INET_SUPPORT
                        if (pt_ctrl->h_uri)
                        {
                            pt_item->u.t_uri.b_close_uri = FALSE;
                            h_uri = pt_ctrl->h_uri;
                            i4_res = FMR_OK;
                        }
                        else
                        {
                            pt_item->u.t_uri.b_close_uri = TRUE;
                            i4_res = x_httpc_fm_open_proxy_agent_timeout_cookie(
                                                        pt_ctrl->ps_path, 
                                                        pt_ctrl->ps_proxy_url, 
                                                        pt_ctrl->ps_agent_url, 
                                                        0,
                                                        PLAYBACK_HANDLER_HTTP_PRE_BUF_SIZE, 
                                                        &h_uri,
                                                        pt_ctrl->ps_cookie_url);
                        }

                        if (b_log_cmpb)
                        {
                            x_dbg_stmt("[CMPB] playback_set x_httpc_fm_open %d\n", i4_res);
                        }

                        if (i4_res == FMR_OK)
                        {
                            UINT64  ui8_current_pos;
                            UINT64  ui8_position;
                            UINT32  ui4_seek_alignment;

                            pt_item->u.t_uri.h_uri = h_uri;

                            pt_item->u.t_uri.ui4_block_size = 512;
                            pt_item->u.t_uri.ui4_alignment = 32;

                            ui8_position = pt_item->ui8_start_offset;

                            ui4_seek_alignment =
                                pt_item->u.t_uri.ui4_block_size;

                            pt_item->ui4_offset = (ui4_seek_alignment != 0) ?
                                (UINT32) (ui8_position % ui4_seek_alignment) :
                                0;

                            pt_item->ui8_start_offset -= pt_item->ui4_offset;

                            i4_res =
                                x_httpc_fm_lseek(h_uri,
                                                 (INT64) pt_item->ui8_start_offset,
                                                 FM_SEEK_BGN,
                                                 & ui8_current_pos);

                            if (i4_res != FMR_OK)
                            {
                                DBG_ERROR((DBG_PREFIX
                                           "Could not seek (%d, res=%d)\n",
                                           h_ch_conn, i4_res));
                            }
                        }
                        else
                        {
                            DBG_ERROR((DBG_PREFIX
                                       "Could not open file \"%s\" (%d, res=%d)\n",
                                       pt_ctrl->ps_path, h_ch_conn, i4_res));
                        }
#endif
                    }
                    b_ok = (i4_res == FMR_OK);
                }
                else if (e_ctrl_type == PLAYBACK_CTRL_SET_QUEUE_FILE)
                {
                    PLAYBACK_HANDLER_QUEUE_FILE_T*  pt_ctrl;
                    HANDLE_T                        h_file;

                    pt_ctrl = (PLAYBACK_HANDLER_QUEUE_FILE_T *) pv_ctrl_data;

                    pt_scdb_info      = pt_ctrl->pt_scdb_info;
                    ui4_nb_records    = pt_ctrl->ui4_nb_records;
                    ui4_buf_agt_index = pt_ctrl->ui4_buf_agt_index;

                    pt_ctrl->ui4_id = ui4_id;

                    pt_item->t_extra               = pt_ctrl->t_extra;
                    pt_item->e_format              = pt_ctrl->e_format;
                    pt_item->ui8_start_offset      = pt_ctrl->ui8_start_offset;
                    pt_item->ui8_end_offset        = pt_ctrl->ui8_end_offset;
                    pt_item->ui4_flags             = pt_ctrl->ui4_flags;
                    pt_item->ui4_offset            = 0;
                    pt_item->u.t_file.h_file       = NULL_HANDLE;
                    pt_item->u.t_file.ui1_priority = 127;
                    pt_item->e_type                = PLAYBACK_ITEM_TYPE_FILE;

                    DBG_INFO((DBG_PREFIX"Opening file \"%s\" (%d)\n",
                              pt_ctrl->ps_path, h_ch_conn));

                    i4_res = x_fm_mfw_open(FM_ROOT_HANDLE,
                                           pt_ctrl->ps_path,
                                           FM_READ_ONLY,
                                           FM_MODE_USR_READ,
                                           TRUE, /* no cache */
                                           & h_file);

                    if (i4_res == FMR_OK)
                    {
                        FM_FS_INFO_T  t_fs_info;
                        UINT64        ui8_current_pos;

                        i4_res = x_fm_mfw_get_fs_info(FM_ROOT_HANDLE,
                                                      pt_ctrl->ps_path,
                                                      & t_fs_info);

                        if (i4_res == FMR_OK)
                        {
                            UINT64  ui8_position;
                            UINT32  ui4_seek_alignment;

                            pt_item->u.t_file.h_file = h_file;

                            pt_item->u.t_file.ui4_block_size =
                                t_fs_info.ui4_min_blk_size;
                            pt_item->u.t_file.ui4_alignment =
                                t_fs_info.ui4_alignment;

                            ui8_position = pt_item->ui8_start_offset;

                            ui4_seek_alignment =
                                pt_item->u.t_file.ui4_block_size;

                            pt_item->ui4_offset = (ui4_seek_alignment != 0) ?
                                (UINT32) (ui8_position % ui4_seek_alignment) :
                                0;

                            pt_item->ui8_start_offset -= pt_item->ui4_offset;

                            DBG_INFO((DBG_PREFIX"Block size: %u bytes (%d)\n"
                                      DBG_PREFIX"Alignment: %u bytes (%d)\n",
                                      t_fs_info.ui4_min_blk_size,
                                      h_ch_conn,
                                      t_fs_info.ui4_alignment,
                                      h_ch_conn));

                            i4_res =
                                x_fm_mfw_lseek(h_file,
                                               (INT64) pt_item->ui8_start_offset,
                                               FM_SEEK_BGN,
                                               & ui8_current_pos);

                            if (i4_res != FMR_OK)
                            {
                                DBG_ERROR((DBG_PREFIX
                                           "Could not seek (%d, res=%d)\n",
                                           h_ch_conn, i4_res));
                            }
                        }
                        else
                        {
                            DBG_ERROR((DBG_PREFIX
                                       "Could not get FS info (%d, res=%d)\n",
                                       h_ch_conn, i4_res));
                        }
                    }
                    else
                    {
                        DBG_ERROR((DBG_PREFIX
                                   "Could not open file \"%s\" (%d, res=%d)\n",
                                   pt_ctrl->ps_path, h_ch_conn, i4_res));
                    }

                    b_ok = (i4_res == FMR_OK);
                }
                else
                {
                    PLAYBACK_HANDLER_QUEUE_BUFFER_T*  pt_ctrl;

                    pt_ctrl = (PLAYBACK_HANDLER_QUEUE_BUFFER_T *) pv_ctrl_data;

                    pt_scdb_info      = pt_ctrl->pt_scdb_info;
                    ui4_nb_records    = pt_ctrl->ui4_nb_records;
                    ui4_buf_agt_index = pt_ctrl->ui4_buf_agt_index;

                    pt_ctrl->ui4_id = ui4_id;

                    pt_item->t_extra              = pt_ctrl->t_extra;
                    pt_item->e_format             = pt_ctrl->e_format;
                    pt_item->ui8_start_offset     = pt_ctrl->ui8_start_offset;
                    pt_item->ui8_end_offset       = pt_ctrl->ui8_end_offset;
                    pt_item->ui4_flags            = pt_ctrl->ui4_flags;
                    pt_item->ui4_offset           = 0;
                    pt_item->u.t_buffer.pv_buffer = pt_ctrl->pv_data;
                    pt_item->u.t_buffer.ui8_size  = pt_ctrl->ui8_size;
                    pt_item->e_type               = PLAYBACK_ITEM_TYPE_BUFFER;

                    b_ok = TRUE;
                }

                if (b_ok && (pt_scdb_info != NULL) && (ui4_nb_records > 0))
                {
                    SIZE_T  z_size;

                    pt_item->ui4_nb_scdb_records = ui4_nb_records;

                    z_size  = ui4_nb_records;
                    z_size *= sizeof(PLAYBACK_HANDLER_ITEM_SCDB_INFO_T);

                    pt_item->pt_scdb_info = x_mem_alloc(z_size);

                    if (pt_item->pt_scdb_info != NULL)
                    {
                        UINT32  ui4_i;

                        x_memcpy(pt_item->pt_scdb_info, pt_scdb_info, z_size);

                        /* mark streams that have no tag assigned */

                        for (ui4_i = 0; ui4_i < ui4_nb_records; ui4_i++)
                        {
                            PLAYBACK_HANDLER_ITEM_SCDB_INFO_T*  pt_scdb_info;

                            pt_scdb_info = & pt_item->pt_scdb_info[ui4_i];

                            pt_scdb_info->t_scdb_rec.ui4_private =
                                CM_SET_SRC_INDEX(0, ui4_buf_agt_index);

                            if (pt_scdb_info->t_stream_id.pv_stream_tag
                                == NULL)
                            {
                                pt_scdb_info->t_scdb_rec.ui4_private |=
                                    PLAYBACK_HANDLER_NO_STREAM_TAG_ASSIGNED;
                            }
                        }
                    }
                    else
                    {
                        DBG_ERROR((DBG_PREFIX
                                   "Could not allocate SCDB info\n"));
                    }

                    b_ok = (pt_item->pt_scdb_info != NULL);
                }

                if (b_ok)
                {
                    pt_item->pt_next      = NULL;
                    pt_item->ui4_id       = ui4_id;
                    pt_item->ui8_position = pt_item->ui8_start_offset;

                    i4_res = cm_send_msg_retry(h_playback_handler_msgq,
                                               PLAYBACK_HANDLER_MSG_QUEUE_ITEM,
                                               (UINT32) pt_item,
                                               ui4_buf_agt_index,
                                               h_ch_conn);
                }
                else
                {
                    i4_res = CMR_FAIL;
                }

                if (i4_res != CMR_OK)
                {
                    DBG_ERROR((DBG_PREFIX"Could not queue item (%d)\n",
                               h_ch_conn));

                    playback_handler_release_item(pt_item);
                }
            }
        }
        break;

        case PLAYBACK_CTRL_SET_UNQUEUE:
        {
            i4_res = cm_send_msg_retry(h_playback_handler_msgq,
                                       PLAYBACK_HANDLER_MSG_UNQUEUE_ITEM,
                                       (UINT32) pv_ctrl_data, 0,
                                       h_ch_conn);
        }
        break;

        case PLAYBACK_CTRL_SET_EMPTY_QUEUE:
        {
            i4_res = cm_send_msg_retry(h_playback_handler_msgq,
                                       PLAYBACK_HANDLER_MSG_EMPTY_QUEUE,
                                       0, 0,
                                       h_ch_conn);
        }
        break;

        case PLAYBACK_CTRL_SET_ITEM_ATTRIBUTES:
        case PLAYBACK_CTRL_SET_ITEM_SVC_INFO_BY_ID:
        {
            PLAYBACK_SET_INFO_T  t_info;
            UINT32               ui4_code;

            t_info.h_ch_conn = h_ch_conn;
            t_info.pv_info   = pv_ctrl_data;

            /* set here, but another thread will change the value */
            t_info.i4_res = CMR_FAIL;

            ui4_code = (e_ctrl_type == PLAYBACK_CTRL_SET_ITEM_ATTRIBUTES) ?
                PLAYBACK_HANDLER_MSG_SET_ITEM_ATTRIBUTES :
                PLAYBACK_HANDLER_MSG_SET_ITEM_SVC_INFO;

            i4_res =
                cm_send_msg_retry(h_playback_handler_msgq,
                                  ui4_code,
                                  (UINT32) & t_info, 0,
                                  NULL_HANDLE);

            if (i4_res == CMR_OK)
            {
                /* operation is made synchronous with this semaphore */
                x_sema_lock(h_playback_handler_sema, X_SEMA_OPTION_WAIT);

                i4_res = t_info.i4_res;
            }
        }
        break;

        default:
            i4_res = CMR_INV_ARG;
    }

    return i4_res;
}

HANDLE_T  my_brdcst_pmt;
/*-----------------------------------------------------------------------------
 * Name: playback_handler_thread_main
 *
 * Description: This API is the Playback Connection Handler's thread entry
 *              point.
 *
 * Inputs:  pv_arg  References the playback handler information.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID playback_handler_thread_main (VOID* pv_arg)
{
    PLAYBACK_HANDLER_INFO_T*  pt_info;
    UINT32                    ui4_nb_connections;
    BOOL                      b_exit_when_done;
    BOOL                      b_exit_now;

    pt_info = (PLAYBACK_HANDLER_INFO_T*) pv_arg;

    ui4_nb_connections = 0;

    b_exit_when_done = FALSE;
    b_exit_now       = FALSE;

#ifdef INET_SUPPORT
    x_httpc_init_module();
#endif

    while (! b_exit_now)
    {
        PLAYBACK_CONN_T*  pt_conn;
        CM_MSG_T          t_msg;
        SIZE_T            z_msg_size;
        UINT16            ui2_index;
        BOOL              b_error;

        pt_conn = NULL;
        b_error = FALSE;

        z_msg_size = sizeof(CM_MSG_T);

        if (x_msg_q_receive(& ui2_index,
                            & t_msg,
                            & z_msg_size,
                            & pt_info->h_playback_handler_msgq, 1,
                            X_MSGQ_OPTION_WAIT)
            != OSR_OK)
        {
            DBG_ERROR((DBG_PREFIX"x_msg_q_receive failed\n"));

            /* failure is ignored */
            continue;
        }

        if ((t_msg.h_obj != NULL_HANDLE) && ! x_handle_valid(t_msg.h_obj))
        {
            DBG_INFO((DBG_PREFIX"non-NULL invalid handle %d (code=%d)\n",
                      t_msg.h_obj, t_msg.ui4_code));

            /* messages with invalid non-NULL handle are ignored */
            b_error = TRUE;
        }

        if (t_msg.h_obj != NULL_HANDLE)
        {
            if (handle_get_obj(t_msg.h_obj, (VOID**) & pt_conn) != HR_OK)
            {
                DBG_ERROR((DBG_PREFIX"handle_get_obj failed\n"));

                b_error = TRUE;
            }
        }

        /* in case of error, some resources may need to be deleted */
        if (b_error)
        {
            switch (t_msg.ui4_code)
            {
                case PLAYBACK_HANDLER_MSG_DATA_REQUEST:
                {
                    PLAYBACK_DATA_REQUEST_T*  pt_request;

                    pt_request = (PLAYBACK_DATA_REQUEST_T *) t_msg.ui4_data_1;

                    playback_handler_release_data_request(pt_request);
                }
                break;

                case PLAYBACK_HANDLER_MSG_QUEUE_ITEM:
                {
                    PLAYBACK_ITEM_T*  pt_item;

                    pt_item = (PLAYBACK_ITEM_T *) t_msg.ui4_data_1;

                    playback_handler_release_item(pt_item);
                }
                break;

                default:
                    break;
            }

            continue;
        }

        switch (t_msg.ui4_code)
        {
#if 0
            case PLAYBACK_HANDLER_CIRCULAR_OVERFLOW:
            {
                UINT32                  ui4_idx;
                INT32                   i4_ret;
                PLAYBACK_BUF_AGT_T*     pt_buf_agt;
                HANDLE_T                h_obj;

                h_obj = (HANDLE_T) t_msg.ui4_data_2;

                pt_buf_agt = playback_conn_get_buf_agt_by_handle(h_obj);

                if (pt_buf_agt != NULL)
                {
                    i4_ret = playback_conn_get_buf_agt_index (
                                pt_conn,
                                pt_buf_agt,
                                &ui4_idx);

                    if (CMR_OK == i4_ret)
                    {
                        cm_nfy(
                            pt_conn->h_cm_conn,
                            CM_PLAYBACK_CONN_INFO_BUFFER_OVERFLOW,
                            ui4_idx);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_CIRCULAR_UNDERFLOW:
            {
                UINT32                  ui4_idx;
                INT32                   i4_ret;
                PLAYBACK_BUF_AGT_T*     pt_buf_agt;
                HANDLE_T                h_obj;

                h_obj = (HANDLE_T) t_msg.ui4_data_2;

                pt_buf_agt = playback_conn_get_buf_agt_by_handle(h_obj);

                if (pt_buf_agt != NULL)
                {
                    i4_ret = playback_conn_get_buf_agt_index (
                                pt_conn,
                                pt_buf_agt,
                                &ui4_idx);

                    if (CMR_OK == i4_ret)
                    {
                        cm_nfy(
                            pt_conn->h_cm_conn,
                            CM_PLAYBACK_CONN_INFO_BUFFER_UNDERFLOW,
                            ui4_idx);
                    }
                }
            }
            break;
#endif

            case PLAYBACK_HANDLER_SHARED_MEMORY:
            {
                if (pt_conn)
                {
                    PLAYBACK_HANDLER_SHARED_MEMORY_T*   pt_shared_mem;
                    PLAYBACK_BUF_AGT_T*                 pt_buf_agt;
                    UINT32                              ui4_buf_agt_index;
                    SIZE_T                              z_size;
                    BUF_AGT_BUFFER_SHARED_MEMORY_T      t_bagt_shared_mem;

                    pt_shared_mem = (PLAYBACK_HANDLER_SHARED_MEMORY_T*)t_msg.ui4_data_1;
                    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;
                    ui4_buf_agt_index = pt_shared_mem->ui4_buf_agt_index;

                    while ((pt_buf_agt != NULL) && (ui4_buf_agt_index-- > 0))
                    {
                        pt_buf_agt = pt_buf_agt->pt_next;
                    }

                    if (pt_buf_agt != NULL)
                    {
                        if (pt_buf_agt->pv_shared_buffer)
                        {
                            pt_shared_mem->pv_buffer =
                            pt_buf_agt->pv_shared_buffer;

                            pt_shared_mem->pv_private =
                            pt_buf_agt->pv_shared_private;

                            pt_shared_mem->z_buffer_size =
                            pt_buf_agt->z_shared_buffer_size;
                        }
                        else
                        {
                            x_memset(&t_bagt_shared_mem, 0, sizeof(BUF_AGT_BUFFER_SHARED_MEMORY_T));
                            z_size = sizeof(BUF_AGT_BUFFER_SHARED_MEMORY_T);
                            rm_get(pt_buf_agt->h_rm_buffer_agent,
                                   FALSE,
                                   ANY_PORT_NUM,
                                   BUF_AGT_GET_TYPE_SHARED_MEMORY,
                                   (VOID*) &t_bagt_shared_mem,
                                   &z_size,
                                   NULL);

                            pt_buf_agt->pv_shared_buffer = t_bagt_shared_mem.pv_buffer;
                            pt_buf_agt->pv_shared_private = t_bagt_shared_mem.pv_private;
                            pt_buf_agt->z_shared_buffer_size = t_bagt_shared_mem.z_buffer_size;

                            pt_shared_mem->pv_buffer = t_bagt_shared_mem.pv_buffer;
                            pt_shared_mem->pv_private = t_bagt_shared_mem.pv_private;
                            pt_shared_mem->z_buffer_size = t_bagt_shared_mem.z_buffer_size;
                        }
                    }
                }
                x_sema_unlock(h_playback_handler_sema);

            }
            break;

            case PLAYBACK_HANDLER_CIRCULAR_FULLNESS:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    PLAYBACK_GET_BUFFER_FULLNESS_T* pt_fullness;
                    BUF_AGT_BUFFER_FULLNESS_T t_fullness;
                    UINT32 ui4_fill_size;
                    SIZE_T z_size;

                    pt_fullness = (PLAYBACK_GET_BUFFER_FULLNESS_T*)t_msg.ui4_data_1;

                    if (pt_fullness)
                    {
                        pt_buf_agt =
                        playback_conn_get_buf_agt_by_index(pt_conn,
                                                           pt_fullness->ui4_buf_agt_index);

                        if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT)
                        {
                            PLAYBACK_MODE_CTRL_T*   pt_mode_ctrl;

                            pt_mode_ctrl = &pt_conn->t_connect_info.t_mode_ctrl;

                            pt_fullness->ui4_length = pt_mode_ctrl->u.t_flow_ctrl.ui4_buffer_len - pt_mode_ctrl->u.t_flow_ctrl.ui4_keep_buffer_threshold;
                            if (pt_buf_agt->pt_cb != NULL)
                            {
                                ui4_fill_size = playback_cb_get_filled_size(pt_buf_agt->pt_cb);
                                if (ui4_fill_size > pt_fullness->ui4_length)
                                {
                                    pt_fullness->ui4_percentage = 100;
                                }
                                else
                                {
                                    if (pt_fullness->ui4_length)
                                    {
                                        pt_fullness->ui4_percentage = ui4_fill_size*100/pt_fullness->ui4_length;
                                    }
                                    else
                                    {
                                        pt_fullness->ui4_percentage = 100;
                                    }
                                    pt_fullness->ui4_length = ui4_fill_size;
                                }
                            }
                            else
                            {
                                pt_fullness->ui4_percentage = 0;
                                pt_fullness->ui4_length = 0;
                            }
                        }
                        else
                        {
                            z_size = sizeof(BUF_AGT_BUFFER_FULLNESS_T);
                            rm_get(pt_buf_agt->h_rm_buffer_agent,
                                   FALSE,
                                   ANY_PORT_NUM,
                                   BUF_AGT_GET_TYPE_BUFFER_FULLNESS,
                                   (VOID*) &t_fullness,
                                   &z_size,
                                   NULL);
                            pt_fullness->ui4_length = t_fullness.ui4_length;
                            pt_fullness->ui4_percentage = t_fullness.ui4_percentage;
                        }
                    }
                }
                x_sema_unlock(h_playback_handler_sema);
            }
                break;

            case PLAYBACK_HANDLER_CIRCULAR_STATUS:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*             pt_buf_agt;
                    PLAYBACK_GET_BUFFER_STATUS_T*   pt_status;
                    UINT32                          ui4_fill_size = 0;

                    pt_status = (PLAYBACK_GET_BUFFER_STATUS_T*)t_msg.ui4_data_1;

                    if (pt_status)
                    {
                        pt_buf_agt =
                        playback_conn_get_buf_agt_by_index(pt_conn,
                                                           pt_status->ui4_buf_agt_index);
                                                           
                        if (pt_buf_agt)
                        {
                            if (pt_buf_agt->pt_cb != NULL)
                            {
                                ui4_fill_size = playback_cb_get_filled_size(pt_buf_agt->pt_cb);
                                
                            }
                            if (pt_buf_agt->pt_transferring_item)
                            {
                                pt_status->ui8_begin_pos = pt_buf_agt->pt_transferring_item->ui8_position;
                                pt_status->ui8_end_pos = pt_status->ui8_begin_pos + ui4_fill_size;
                            }
                        }
                    }
                }
                x_sema_unlock(h_playback_handler_sema);
            }
                break;

            case PLAYBACK_HANDLER_TIME_TO_SIZE:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    PLAYBACK_HANDLER_TIME_TO_SIZE_T* pt_time2size;
                    BUF_AGT_BUFFER_TIME_TO_SIZE_T t_time2size;
                    SIZE_T z_size;

                    pt_time2size = (PLAYBACK_HANDLER_TIME_TO_SIZE_T*)t_msg.ui4_data_1;

                    if (pt_time2size)
                    {
                        pt_buf_agt =
                        playback_conn_get_buf_agt_by_index(pt_conn,
                                                           pt_time2size->ui4_buf_agt_index);

                        z_size = sizeof(BUF_AGT_BUFFER_TIME_TO_SIZE_T);
                        t_time2size.ui4_ms = pt_time2size->ui4_ms;
                        t_time2size.ui4_size = 0;
                        rm_get(pt_buf_agt->h_rm_buffer_agent,
                               FALSE,
                               ANY_PORT_NUM,
                               BUF_AGT_GET_TYPE_TIME_TO_SIZE,
                               (VOID*) &t_time2size,
                               &z_size,
                               NULL);
                        pt_time2size->ui4_size = t_time2size.ui4_size;
                    }
                }
                x_sema_unlock(h_playback_handler_sema);
            }
                break;

            case PLAYBACK_HANDLER_MODIFY_SCDB:
            {
                INT32 i4_ret;
                PLAYBACK_HANDLER_ITEM_SCDB_INFO_T* pt_scdb_info;

                pt_scdb_info = (PLAYBACK_HANDLER_ITEM_SCDB_INFO_T*)t_msg.ui4_data_1;

                if (NULL == pt_scdb_info)
                {
                    break;
                }

                if (pt_conn)
                {
                    i4_ret = scdb_lock(pt_conn->h_scdb);
#if 0
                    scdb_del_rec(pt_conn->h_scdb,
                                 &pt_scdb_info->t_stream_id);

                    scdb_unlock(pt_conn->h_scdb);
                    scdb_lock(pt_conn->h_scdb);

                    scdb_add_rec(pt_conn->h_scdb,
                                 &pt_scdb_info->t_stream_id,
                                 &pt_scdb_info->t_scdb_rec);
#else
                    scdb_update_rec(pt_conn->h_scdb,
                                    &pt_scdb_info->t_stream_id,
                                    &pt_scdb_info->t_scdb_rec);
#endif
                    if (i4_ret == SCDBR_OK)
                    {
                        scdb_unlock(pt_conn->h_scdb);
                    }
                }

                x_mem_free(pt_scdb_info);
            }
                break;

            case PLAYBACK_HANDLER_CIRCULAR_PRELOAD:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;

                    pt_buf_agt =
                        playback_conn_get_buf_agt_by_index(pt_conn,
                                                           t_msg.ui4_data_2);

                    playback_uri_issue_req(pt_conn, pt_buf_agt);

                    if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_DIRECT)
                    {
                        rm_set(pt_buf_agt->h_rm_buffer_agent,
                               FALSE,
                               ANY_PORT_NUM,
                               BUF_AGT_SET_TYPE_PRELOAD,
                               (VOID*) NULL,
                               0,
                               NULL);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_CIRCULAR_REQUEST:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*             pt_buf_agt;
                    HANDLE_T                        h_buffer_agent;
                    PLAYBACK_CIRCULAR_REQUEST_T*    pt_circular_req;

                    h_buffer_agent = (HANDLE_T) t_msg.ui4_data_1;

                    pt_buf_agt = playback_conn_get_buf_agt_by_handle(h_buffer_agent);

                    if (pt_buf_agt != NULL)
                    {
                        pt_buf_agt->b_issue_circular_req = FALSE;
                        pt_circular_req = &pt_buf_agt->t_circular_req;
                        if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT)
                        {
                            UINT32 ui4_fill_size;
                            UINT32 ui4_full_size;
                            UINT32 ui4_idx;
                            INT32  i4_ret;

                            UNUSED(ui4_fill_size);
                            UNUSED(ui4_full_size);
                            UNUSED(ui4_idx);
                            UNUSED(i4_ret);

                            switch (pt_circular_req->e_nfy_cond)
                            {
                            case FM_ASYNC_COND_EOF:
                            case FM_ASYNC_COND_DLNA_NO_RESPONSE:
                                /*store item end position*/
                                pt_buf_agt->pt_transferring_item->ui8_end_offset =
                                    pt_buf_agt->pt_transferring_item->ui8_position +
                                    playback_cb_get_filled_size(pt_buf_agt->pt_cb) +
                                    pt_circular_req->ui4_data_len;
                                if (pt_circular_req->e_nfy_cond == FM_ASYNC_COND_EOF)
                                {
                                    x_dbg_stmt("[CMPB] CM_PLAYBACK_CONN_INFO_EOF 3\n");
                                    cm_nfy(pt_conn->h_cm_conn,
                                           CM_PLAYBACK_CONN_INFO_EOF,
                                           0);
                                }

                            case FM_ASYNC_COND_READ_OK:
                                if (pt_buf_agt->pt_cb != NULL)
                                {
                                    /*handle the data request*/
                                    playback_cb_add(pt_buf_agt->pt_cb, pt_circular_req->ui4_data_len);
                                    playback_conn_transfer_circular_data (pt_conn, pt_buf_agt);
                                    playback_uri_issue_req(pt_conn, pt_buf_agt);
    #if 0
                                    ui4_fill_size = playback_cb_get_filled_size(pt_buf_agt->pt_cb);
                                    ui4_full_size = pt_buf_agt->t_flow_ctrl.ui4_buffer_len
                                                  - pt_buf_agt->t_flow_ctrl.ui4_keep_buffer_threshold;

                                    if (ui4_fill_size >= ui4_full_size)
                                    {
                                        i4_ret = playback_conn_get_buf_agt_index (
                                                    pt_conn,
                                                    pt_buf_agt,
                                                    &ui4_idx);

                                        if (CMR_OK == i4_ret)
                                        {
                                            cm_nfy(
                                                pt_conn->h_cm_conn,
                                                CM_PLAYBACK_CONN_INFO_BUFFER_OVERFLOW,
                                                ui4_idx);
                                        }
                                    }
    #endif
                                }
                                break;

                            case FM_ASYNC_COND_FAIL:
                                x_dbg_stmt("[CMPB] FM_ASYNC_COND_FAIL b_conn_error=TRUE\n");

                                if (!pt_buf_agt->b_conn_error)
                                {
                                    pt_buf_agt->b_conn_error = TRUE;
                                    cm_nfy(pt_conn->h_cm_conn,
                                           CM_PLAYBACK_CONN_INFO_FILE_ERROR,
                                           0);
                                }
                                break;

                            case FM_ASYNC_COND_READ_TIMEOUT:
                                playback_uri_issue_req(pt_conn, pt_buf_agt);
                                break;

                            case FM_ASYNC_COND_WRITE_OK:
                            case FM_ASYNC_COND_ABORT_OK:
                            case FM_ASYNC_COND_ABORT_FAIL:
                            default:
                                break;
                            }
                        }

                        if (pt_conn->b_disconnecting)
                        {
                            BOOL b_circular_running;

                            pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;
                            b_circular_running = FALSE;

                            while (pt_buf_agt != NULL)
                            {
                                PLAYBACK_BUF_AGT_T*  pt_next;

                                pt_next = pt_buf_agt->pt_next;

                                if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT)
                                {
                                    if (pt_buf_agt->b_issue_circular_req)
                                    {
                                        b_circular_running = TRUE;
                                        break;
                                    }
                                }
                                pt_buf_agt = pt_next;
                            }

                            if (!b_circular_running)
                            {
                                cm_send_msg_retry(h_playback_handler_msgq,
                                                  PLAYBACK_HANDLER_CIRCULAR_STOPPED,
                                                  0, 0,
                                                  t_msg.h_obj);
                            }
                        }
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_CIRCULAR_STOPPED:
            {
                if (pt_conn)
                {
                    if (pt_conn->b_disconnecting)
                    {
                        playback_disconnect_conn_2nd (pt_conn);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_CONNECT_REQUEST:
            {
                if (pt_conn)
                {
                    DBG_INFO((DBG_PREFIX"Connection request (%d, %p)\n",
                              t_msg.h_obj, pt_conn));

                    ui4_nb_connections++;

                    DBG_INFO((DBG_PREFIX"Handler has %u connection%s\n",
                              ui4_nb_connections,
                              (ui4_nb_connections > 1) ? "s" : ""));

                    playback_handler_link_conn(pt_conn);

                    playback_open_conn(pt_conn);
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_DISCONNECT_REQUEST:
            {
                if (pt_conn)
                {
                    DBG_INFO((DBG_PREFIX"Disconnection request (%d, %p)\n",
                              t_msg.h_obj, pt_conn));

                    playback_disconnect_conn(pt_conn, t_msg.ui4_data_1);
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_VIRTUAL_SOURCE_OPENED:
            {
                if (pt_conn)
                {
                    playback_virtual_source_opened(pt_conn);
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_VIRTUAL_SOURCE_CLOSED:
            {
                if (pt_conn)
                {
                    playback_virtual_source_closed(pt_conn);
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_BUFFER_AGENT_OPENED:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_rm_buffer_agent;

                    h_rm_buffer_agent = (HANDLE_T) t_msg.ui4_data_2;

                    pt_buf_agt = playback_conn_get_buf_agt(pt_conn,
                                                           h_rm_buffer_agent);

                    if (pt_buf_agt != NULL)
                    {
                        playback_buffer_agent_opened(pt_conn, pt_buf_agt);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_BUFFER_AGENT_CLOSED:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_rm_buffer_agent;

                    h_rm_buffer_agent = (HANDLE_T) t_msg.ui4_data_2;

                    pt_buf_agt = playback_conn_get_buf_agt(pt_conn,
                                                           h_rm_buffer_agent);

                    if (pt_buf_agt != NULL)
                    {
                        playback_buffer_agent_closed(pt_conn, pt_buf_agt);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_BUFFER_AGENT_CONNECTED:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_rm_buffer_agent;

                    h_rm_buffer_agent = (HANDLE_T) t_msg.ui4_data_2;

                    pt_buf_agt = playback_conn_get_buf_agt(pt_conn,
                                                           h_rm_buffer_agent);

                    DBG_INFO((DBG_PREFIX
                              "Buffer agent connected asynchronously (%d)\n",
                              pt_conn->h_ch_conn));

                    if (pt_buf_agt != NULL)
                    {
                        playback_buffer_agent_connected(pt_conn, pt_buf_agt);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_BUFFER_AGENT_DISCONNECTED:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_rm_buffer_agent;

                    h_rm_buffer_agent = (HANDLE_T) t_msg.ui4_data_2;

                    pt_buf_agt = playback_conn_get_buf_agt(pt_conn,
                                                           h_rm_buffer_agent);

                    DBG_INFO((DBG_PREFIX
                              "Buffer agent disconnected asynchronously (%d)\n",
                              pt_conn->h_ch_conn));

                    if (pt_buf_agt != NULL)
                    {
                        playback_buffer_agent_disconnected(pt_conn, pt_buf_agt);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_TRANSFER_DATA:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;

                    pt_buf_agt = pt_conn->t_connect_info.pt_buf_agt_head;

                    while (pt_buf_agt != NULL)
                    {
                        if (pt_buf_agt->ui4_flush == 0)
                        {
                            if ((pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_PULL)
                                ||(pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_DIRECT))
                            {
                                playback_conn_transfer_data(pt_conn, pt_buf_agt);
                            }
                            else if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT)
                            {
                                playback_conn_transfer_circular_data(pt_conn, pt_buf_agt);
                                playback_uri_issue_req (pt_conn, pt_buf_agt);
                            }
                        }
                        else
                        {
                            playback_conn_cancel_head_request(pt_conn, pt_buf_agt,
                                                              TRUE, TRUE);
                        }

                        pt_buf_agt = pt_buf_agt->pt_next;
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_DATA_REQUEST:
            {
                if (pt_conn)
                {
                    PLAYBACK_DATA_REQUEST_T*  pt_request;
                    PLAYBACK_BUF_AGT_T*       pt_buf_agt;
                    HANDLE_T                  h_obj;

                    pt_request = (PLAYBACK_DATA_REQUEST_T *) t_msg.ui4_data_1;

                    h_obj = (HANDLE_T) t_msg.ui4_data_2;

                    pt_buf_agt = playback_conn_get_buf_agt_by_handle(h_obj);

                    if (pt_buf_agt != NULL)
                    {
                        playback_conn_queue_data_request(pt_conn, pt_buf_agt,
                                                         pt_request);
                        if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT)
                        {
                            playback_conn_transfer_circular_data(pt_conn, pt_buf_agt);
                            playback_uri_issue_req (pt_conn, pt_buf_agt);
                        }
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_ACK_DATA_TRANSFER_OK:
            {
                if (pt_conn)
                {
                    PLAYBACK_DATA_REQUEST_T*  pt_request;
                    PLAYBACK_BUF_AGT_T*       pt_buf_agt;
                    HANDLE_T                  h_obj;

                    pt_request = (PLAYBACK_DATA_REQUEST_T *) t_msg.ui4_data_1;

                    h_obj = pt_request->h_buffer_agent;

                    pt_buf_agt = playback_conn_get_buf_agt_by_handle(h_obj);

                    if (pt_buf_agt != NULL)
                    {
                        BUF_AGT_DATA_REQ_ACK_COND_T  e_cond;

                        e_cond = BUF_AGT_DATA_REQ_ACK_COND_DONE;

                        DBG_INFO((DBG_PREFIX
                                  "Ack data request (id:%u, asked:%u, read:%u): "
                                  "DONE ASYNC (%d)\n",
                                  pt_request->ui4_req_id,
                                  pt_request->z_size,
                                  t_msg.ui4_data_2,
                                  pt_conn->h_ch_conn));

                        /*
                          ui4_data_2 may be greather than z_size (because the
                          buffer was bigger than the actual request). In such case,
                          z_size is not modified.
                        */
                        if (t_msg.ui4_data_2 < pt_request->z_size)
                        {
                            pt_request->z_size = t_msg.ui4_data_2;
                        }

                        playback_conn_ack_data_request(pt_conn, pt_buf_agt,
                                                       pt_request, e_cond);
                    }
                    else
                    {
                        DBG_INFO((DBG_PREFIX
                                  "Could not get buffer agent from handle\n"));
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_ACK_DATA_TRANSFER_FAILED:
            {
                if (pt_conn)
                {
                    PLAYBACK_DATA_REQUEST_T*  pt_request;
                    PLAYBACK_BUF_AGT_T*       pt_buf_agt;
                    HANDLE_T                  h_obj;
                    UINT32                    ui4_item_id;

                    pt_request = (PLAYBACK_DATA_REQUEST_T *) t_msg.ui4_data_1;

                    h_obj = pt_request->h_buffer_agent;
                    ui4_item_id = (pt_request->pt_item ? pt_request->pt_item->ui4_id : 0);

                    pt_buf_agt = playback_conn_get_buf_agt_by_handle(h_obj);

                    if (pt_buf_agt != NULL)
                    {
                        BUF_AGT_DATA_REQ_ACK_COND_T  e_cond;

                        e_cond = BUF_AGT_DATA_REQ_ACK_COND_ERROR;

                        DBG_INFO((DBG_PREFIX
                                  "Ack %d-byte data request (id:%u): ERROR (%d)\n",
                                  pt_request->z_size,
                                  pt_request->ui4_req_id,
                                  pt_conn->h_ch_conn));

                        playback_conn_ack_data_request(pt_conn, pt_buf_agt,
                                                       pt_request, e_cond);

                        playback_conn_data_consumed(pt_conn, pt_buf_agt,
                                                    pt_request->ui4_req_id,
                                                    TRUE, TRUE);

                        cm_nfy(pt_conn->h_cm_conn,
                               CM_PLAYBACK_CONN_INFO_FILE_ERROR,
                               ui4_item_id);

                        playback_conn_transfer_data(pt_conn, pt_buf_agt);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_ACK_DATA_TRANSFER_EOF:
            {
                if (pt_conn)
                {
                    PLAYBACK_DATA_REQUEST_T*  pt_request;
                    PLAYBACK_BUF_AGT_T*       pt_buf_agt;
                    HANDLE_T                  h_obj;

                    pt_request = (PLAYBACK_DATA_REQUEST_T *) t_msg.ui4_data_1;

                    h_obj = pt_request->h_buffer_agent;

                    pt_buf_agt = playback_conn_get_buf_agt_by_handle(h_obj);

                    if (pt_buf_agt != NULL)
                    {
                        BUF_AGT_DATA_REQ_ACK_COND_T  e_cond;

                        e_cond = BUF_AGT_DATA_REQ_ACK_COND_DONE;

                        DBG_INFO((DBG_PREFIX
                                  "Ack data request (id:%u, asked:%u, read:%u): "
                                  "DONE ASYNC EOF (%d)\n",
                                  pt_request->ui4_req_id,
                                  pt_request->z_size,
                                  t_msg.ui4_data_2,
                                  pt_conn->h_ch_conn));

                        /*
                          z_size is set to 0 and the request is the last one
                        */
                        /* pt_request->z_size = 0; */
                        pt_request->b_last = TRUE;

                        playback_conn_ack_data_request(pt_conn, pt_buf_agt,
                                                       pt_request, e_cond);
                    }
                    else
                    {
                        DBG_INFO((DBG_PREFIX
                                  "Could not get buffer agent from handle\n"));
                    }

                    cm_nfy(pt_conn->h_cm_conn,
                           CM_PLAYBACK_CONN_INFO_EOF,
                           0);
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_DATA_CONSUMED:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_obj;
                    UINT32               ui4_req_id;

                    ui4_req_id = t_msg.ui4_data_1;

                    h_obj = (HANDLE_T) t_msg.ui4_data_2;

                    pt_buf_agt = playback_conn_get_buf_agt_by_handle(h_obj);

                    if (pt_buf_agt != NULL)
                    {
                        playback_conn_data_consumed(pt_conn, pt_buf_agt,
                                                    ui4_req_id, TRUE, TRUE);
                        if ((pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_PULL)
                            || (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_DIRECT))
                        {
                            playback_conn_transfer_data(pt_conn, pt_buf_agt);
                        }
                        else
                        {
                            playback_conn_transfer_circular_data(pt_conn, pt_buf_agt);
                        }

    #if 0
                        if (pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT)
                        {
                            UINT32 ui4_fill_size;
                            UINT32 ui4_idx;
                            INT32  i4_ret;

                            if (pt_buf_agt->pt_cb != NULL)
                            {
                                ui4_fill_size = playback_cb_get_filled_size(pt_buf_agt->pt_cb);
                                if (ui4_fill_size <= pt_buf_agt->t_flow_ctrl.ui4_keep_buffer_threshold)
                                {
                                    i4_ret = playback_conn_get_buf_agt_index (
                                                pt_conn,
                                                pt_buf_agt,
                                                &ui4_idx);

                                    if (CMR_OK == i4_ret)
                                    {
                                        cm_nfy(
                                            pt_conn->h_cm_conn,
                                            CM_PLAYBACK_CONN_INFO_BUFFER_UNDERFLOW,
                                            ui4_idx);
                                    }
                                }
                            }
                        }
    #endif
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_FLUSH_REQUEST:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_obj;

                    DBG_INFO((DBG_PREFIX"Flush request (%d)\n",
                              pt_conn->h_ch_conn));

                    h_obj = (HANDLE_T) t_msg.ui4_data_2;

                    pt_buf_agt = playback_conn_get_buf_agt_by_handle(h_obj);

                    if (pt_buf_agt != NULL)
                    {
                        pt_buf_agt->ui4_flush++;

                        playback_conn_cancel_all_requests(pt_conn, pt_buf_agt,
                                                          TRUE);

                        playback_conn_consume_all_requests(pt_conn, pt_buf_agt);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_FLUSH_DONE:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_obj;

                    h_obj = (HANDLE_T) t_msg.ui4_data_2;

                    pt_buf_agt = playback_conn_get_buf_agt_by_handle(h_obj);

                    if (pt_buf_agt != NULL)
                    {
                        playback_conn_flush_done(pt_conn, pt_buf_agt);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_QUEUE_ITEM:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    PLAYBACK_ITEM_T*     pt_item;

                    pt_item = (PLAYBACK_ITEM_T *) t_msg.ui4_data_1;

                    pt_buf_agt =
                        playback_conn_get_buf_agt_by_index(pt_conn,
                                                           t_msg.ui4_data_2);

                    if (pt_buf_agt != NULL)
                    {
                        if ((pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT)
                            && (pt_item->e_type != PLAYBACK_ITEM_TYPE_URI))
                        {
                            playback_handler_release_item(pt_item);
                        }
                        else
                        {
                            playback_conn_queue_item(pt_conn, pt_buf_agt, pt_item);

                            if (pt_item->e_type == PLAYBACK_ITEM_TYPE_BUFFER)
                            {
                                if (pt_item->u.t_buffer.ui8_size == 0)
                                {
                                    if ((pt_item->ui4_flags&CM_PLAYBACK_FLAG_SHARED_MEMORY) == 0)
                                    {
                                        playback_conn_remove_item_from_list(pt_buf_agt, pt_item);
                                        playback_handler_release_item(pt_item);
                                        if (pt_item == pt_buf_agt->pt_transferring_item)
                                        {
                                            pt_buf_agt->pt_transferring_item = pt_buf_agt->pt_items_head;
                                        }
                                    }
                                }
                            }
                            else if ((pt_buf_agt->e_mode == PLAYBACK_BUFFER_MODE_CIRCULAR_INDIRECT)
                            && (pt_item->e_type == PLAYBACK_ITEM_TYPE_URI))
                            {
                                playback_uri_issue_req (pt_conn, pt_buf_agt);
                            }
                        }
                    }
                    else
                    {
                        playback_handler_release_item(pt_item);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_UNQUEUE_ITEM:
            {
                if (pt_conn)
                {
                    UINT32  ui4_id;

                    ui4_id = t_msg.ui4_data_1;

                    playback_conn_unqueue_item(pt_conn, ui4_id);
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_EMPTY_QUEUE:
            {
                if (pt_conn)
                {
                    playback_conn_empty_queue(pt_conn, FALSE, TRUE);
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_SET_ITEM_ATTRIBUTES:
            {
                PLAYBACK_SET_INFO_T*  pt_info;

                pt_info = (PLAYBACK_SET_INFO_T*) t_msg.ui4_data_1;

                playback_conn_set_item_attributes(pt_info);

                /* operation is made synchronous with this semaphore */
                x_sema_unlock(h_playback_handler_sema);
            }
            break;

            case PLAYBACK_HANDLER_MSG_GET_ITEM_ATTRIBUTES:
            {
                PLAYBACK_GET_INFO_T*  pt_info;

                pt_info = (PLAYBACK_GET_INFO_T*) t_msg.ui4_data_1;

                playback_conn_get_item_attributes(pt_info);

                /* operation is made synchronous with this semaphore */
                x_sema_unlock(h_playback_handler_sema);
            }
            break;

            case PLAYBACK_HANDLER_MSG_SET_ITEM_SVC_INFO:
            {
                PLAYBACK_SET_INFO_T*  pt_info;

                pt_info = (PLAYBACK_SET_INFO_T*) t_msg.ui4_data_1;

                playback_conn_set_item_svc_info(pt_info);

                /* operation is made synchronous with this semaphore */
                x_sema_unlock(h_playback_handler_sema);
            }
            break;

            case PLAYBACK_HANDLER_MSG_PAT_UPDATED:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_pat;

                    DBG_INFO((DBG_PREFIX"PAT updated (%d)\n", pt_conn->h_ch_conn));

                    h_pat = (HANDLE_T) t_msg.ui4_data_1;

                    pt_buf_agt = playback_conn_get_buf_agt_from_pat(pt_conn,
                                                                    h_pat);

                    if (pt_buf_agt != NULL)
                    {
                        playback_conn_pat_updated(pt_conn, pt_buf_agt);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_PMT_UPDATED:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_pmt;

                    DBG_INFO((DBG_PREFIX"PMT updated (%d)\n", pt_conn->h_ch_conn));

                    h_pmt = (HANDLE_T) t_msg.ui4_data_1;

                    pt_buf_agt = playback_conn_get_buf_agt_from_pmt(pt_conn,
                                                                    h_pmt);

                    if (pt_buf_agt != NULL)
                    {
                        playback_conn_pmt_updated(pt_conn, pt_buf_agt);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_MGT_UPDATED:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_mgt;

                    DBG_INFO((DBG_PREFIX"MGT updated (%d)\n", pt_conn->h_ch_conn));

                    h_mgt = (HANDLE_T) t_msg.ui4_data_1;

                    pt_buf_agt = playback_conn_get_buf_agt_from_mgt(pt_conn,
                                                                    h_mgt);

                    if (pt_buf_agt != NULL)
                    {
                        playback_conn_mgt_updated(pt_conn, pt_buf_agt);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_VCT_UPDATED:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_vct;

                    DBG_INFO((DBG_PREFIX"VCT updated (%d)\n", pt_conn->h_ch_conn));

                    h_vct = (HANDLE_T) t_msg.ui4_data_1;

                    pt_buf_agt = playback_conn_get_buf_agt_from_vct(pt_conn,
                                                                    h_vct);

                    if (pt_buf_agt != NULL)
                    {
                        playback_conn_vct_updated(pt_conn, pt_buf_agt);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_EIT_UPDATED:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_eit;

                    DBG_INFO((DBG_PREFIX"EIT updated (%d)\n", pt_conn->h_ch_conn));

                    h_eit = (HANDLE_T) t_msg.ui4_data_1;

                    pt_buf_agt = playback_conn_get_buf_agt_from_eit(pt_conn,
                                                                    h_eit);

                    if (pt_buf_agt != NULL)
                    {
                        playback_conn_eit_updated(pt_conn, pt_buf_agt);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_PMT_UNAVAILABLE:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_pmt;

                    DBG_INFO((DBG_PREFIX"PMT unavailable (%d)\n",
                              pt_conn->h_ch_conn));

                    h_pmt = (HANDLE_T) t_msg.ui4_data_1;

                    pt_buf_agt = playback_conn_get_buf_agt_from_pmt(pt_conn,
                                                                    h_pmt);

                    if (pt_buf_agt != NULL)
                    {
                        pt_buf_agt->h_pmt          = NULL_HANDLE;
                        pt_buf_agt->b_pmt_acquired = FALSE;

                        playback_conn_pmt_updated(pt_conn, pt_buf_agt);
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_VCT_UNAVAILABLE:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_vct;

                    DBG_INFO((DBG_PREFIX"VCT unavailable (%d)\n",
                              pt_conn->h_ch_conn));

                    h_vct = (HANDLE_T) t_msg.ui4_data_1;

                    pt_buf_agt = playback_conn_get_buf_agt_from_pmt(pt_conn,
                                                                    h_vct);

                    if (pt_buf_agt != NULL)
                    {
                        pt_buf_agt->h_vct          = NULL_HANDLE;
                        pt_buf_agt->b_vct_acquired = FALSE;
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_EIT_UNAVAILABLE:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_eit;

                    h_eit = (HANDLE_T) t_msg.ui4_data_1;

                    pt_buf_agt = playback_conn_get_buf_agt_from_pmt(pt_conn,
                                                                    h_eit);

                    if (pt_buf_agt != NULL)
                    {
                        pt_buf_agt->h_eit0          = NULL_HANDLE;
                        pt_buf_agt->b_eit0_acquired = FALSE;

                        if (pt_buf_agt->b_pmt_acquired)
                        {
                            playback_conn_update_scdb_cc_streams_from_pmt(
                                pt_conn,
                                pt_buf_agt);
                        }
                        else
                        {
                            cm_reset_scdb_cc_streams(pt_conn->h_scdb,
                                                     pt_buf_agt->pv_cc_stream_tag);
                        }
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_TIME_SYNC:
            {
                if (pt_conn)
                {
                    PLAYBACK_BUF_AGT_T*  pt_buf_agt;
                    HANDLE_T             h_obj;

                    h_obj = (HANDLE_T) t_msg.ui4_data_1;

                    pt_buf_agt = playback_conn_get_buf_agt_by_handle(h_obj);

                    if (pt_buf_agt != NULL)
                    {
                        if (! pt_buf_agt->b_time_sync)
                        {
                            pt_buf_agt->b_time_sync = TRUE;

                            if (pt_buf_agt->b_eit0_acquired)
                            {
                                DBG_INFO((DBG_PREFIX
                                          "Time sync'ed, parsing EIT (%d)\n",
                                          pt_conn->h_ch_conn));

                                playback_conn_update_scdb_cc_streams_from_eit(
                                    pt_conn,
                                    pt_buf_agt);
                            }
                            else
                            {
                                DBG_INFO((DBG_PREFIX"Time sync'ed, "
                                          "EIT not acquired yet (%d)\n",
                                          pt_conn->h_ch_conn));
                            }
                        }
                    }
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_LIST_CONNECTIONS:
            {
                playback_handler_list_connections(FALSE);
            }
            break;

            case PLAYBACK_HANDLER_MSG_LIST_PLAYBACK_ITEMS:
            {
                if (pt_conn)
                {
                    playback_handler_list_playback_items(pt_conn->h_ch_conn,
                                                         FALSE);
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_LIST_ALL_PLAYBACK_ITEMS:
            {
                playback_handler_list_all_playback_items(FALSE);
            }
            break;

            case PLAYBACK_HANDLER_MSG_LIST_DATA_REQUESTS:
            {
                if (pt_conn)
                {
                    playback_handler_list_data_requests(pt_conn->h_ch_conn,
                                                        FALSE);
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_LIST_ALL_DATA_REQUESTS:
            {
                playback_handler_list_all_data_requests(FALSE);
            }
            break;

            case PLAYBACK_HANDLER_MSG_LIST_ITEMS_AND_REQUESTS:
            {
                if (pt_conn)
                {
                    playback_handler_list_items_and_requests(pt_conn->h_ch_conn,
                                                             FALSE);
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_LIST_ALL_ITEMS_AND_REQUESTS:
            {
                playback_handler_list_all_items_and_requests(FALSE);
            }
            break;

            case PLAYBACK_HANDLER_MSG_SHOW_SCDB:
            {
                if (pt_conn)
                {
                    playback_handler_show_scdb(pt_conn->h_ch_conn, FALSE);
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_SHOW_ALL_SCDBS:
            {
                playback_handler_show_all_scdbs(FALSE);
            }
            break;

            case PLAYBACK_HANDLER_MSG_SHOW_PAT:
            {
                if (pt_conn)
                {
                    playback_handler_show_pat(pt_conn->h_ch_conn, FALSE);
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_SHOW_ALL_PATS:
            {
                playback_handler_show_all_pats(FALSE);
            }
            break;

            case PLAYBACK_HANDLER_MSG_SHOW_PMT:
            {
                if (pt_conn)
                {
                    playback_handler_show_pmt(pt_conn->h_ch_conn, FALSE);
                }
            }
            break;

            case PLAYBACK_HANDLER_MSG_SHOW_ALL_PMTS:
            {
                playback_handler_show_all_pmts(FALSE);
            }
            break;

            case PLAYBACK_HANDLER_MSG_DECREASE_CONN_COUNT:
            {
                ui4_nb_connections--;

                DBG_INFO((DBG_PREFIX"Handler now has %u connection%s\n",
                          ui4_nb_connections,
                          (ui4_nb_connections > 1) ? "s" : ""));
            }
            break;

            case PLAYBACK_HANDLER_MSG_EXIT:
            {
                b_exit_when_done = TRUE;
            }
            break;

            case PLAYBACK_HANDLER_HTTP_DOWNLOAD_SPEED:
            {
#ifdef INET_SUPPORT
                if (pt_conn)
                {
                    DOUBLE*                 pd_speed    = NULL;
                    PLAYBACK_BUF_AGT_T*     pt_buf_agt  = NULL;

                    pd_speed = (DOUBLE*)t_msg.ui4_data_1;

                    if (pd_speed)
                    {
                        pt_buf_agt = playback_conn_get_buf_agt_by_index(pt_conn, 0);                        
                        if((pt_buf_agt != NULL)                         &&
                           (pt_buf_agt->pt_transferring_item != NULL)   &&
                           (pt_buf_agt->pt_transferring_item->u.t_uri.h_uri != NULL_HANDLE))
                        {
                            INT32 i4_ret = 0;
                            i4_ret = x_httpc_fm_get_speed_download(pt_buf_agt->pt_transferring_item->u.t_uri.h_uri,
                                                                   pd_speed);
                            if(i4_ret != FMR_OK)
                            {
                                *pd_speed = 0;
                            }
                        }
                    }
                }
#endif
                x_sema_unlock(h_playback_handler_sema);
            }
                break;
            default:
                /* unrecognized messages are ignored */
                break;
        }

        b_exit_now = b_exit_when_done && (ui4_nb_connections == 0);
    }

    x_msg_q_delete(pt_info->h_playback_handler_msgq);

    h_playback_handler_msgq = NULL_HANDLE;
}

/*-----------------------------------------------------------------------------
 * Name: playback_handler_set_init
 *
 * Description: This API sets the b_init flag (protected by critical section)
 *              and returns the old value of b_init.
 *
 * Inputs:  b_flag  Contains the new value for b_init.
 *
 * Outputs: pui2_gen  Contains a generation number.
 *
 * Returns: Old value of b_init.
 ----------------------------------------------------------------------------*/
static BOOL playback_handler_set_init (BOOL     b_flag,
                                       UINT16*  pui2_gen)
{
    static UINT16  ui2_gen = 0;

    CRIT_STATE_T  t_state;
    BOOL          b_res;

    t_state = x_crit_start();

    b_res  = b_init;
    b_init = b_flag;

    h_playback_handler = NULL_HANDLE;

    if (b_flag)
    {
         /* generation number in the range 0-65536 */
        *pui2_gen = ui2_gen++;
    }

    x_crit_end(t_state);

    return b_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_playback_reg_handler
 *
 * Description: This API registers the Playback Connection Handler.
 *
 * Inputs:  ps_names         References a string that contains all the names
 *                           for the Playback Connection Handler.
 *          pt_thread_descr  References information about the thread to create.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  The connection handler is not registered.
 ----------------------------------------------------------------------------*/
INT32
x_playback_reg_handler (const CHAR*            ps_name,
                        const THREAD_DESCR_T*  pt_thread_descr)
{
    CM_HANDLER_FCT_TBL_T  t_playback_fct_tbl =
        {
            playback_connect,
            playback_disconnect,
            playback_get,
            playback_set
        };

    INT32   i4_res = CMR_NOT_INIT;
    UINT16  ui2_gen;

    DBG_API((DBG_PREFIX"x_playback_reg_handler(%s, %p)\n",
             (ps_name == NULL) ? "NULL" : ps_name, pt_thread_descr));

    if (! playback_handler_set_init(TRUE, & ui2_gen))
    {
        SIZE_T  z_stack_size;
        UINT8   ui1_priority;
        CHAR    s_name[SYS_NAME_LEN + 1];

        if (ps_name == NULL)
        {
            ps_name = PLAYBACK_HANDLER_DEFAULT_NAME;
        }

        z_stack_size = PLAYBACK_HANDLER_DEFAULT_STACK_SIZE;
        ui1_priority = PLAYBACK_HANDLER_DEFAULT_PRIORITY;

        if (pt_thread_descr != NULL)
        {
            if (pt_thread_descr->z_stack_size != DEFAULT_STACK_SIZE)
            {
                z_stack_size = pt_thread_descr->z_stack_size;
            }

            if (pt_thread_descr->ui1_priority != DEFAULT_PRIORITY)
            {
                ui1_priority = pt_thread_descr->ui1_priority;
            }
        }

        if (h_playback_handler_sema == NULL_HANDLE)
        {
            x_sema_create(& h_playback_handler_sema, X_SEMA_TYPE_COUNTING, 0);
        }

        x_sprintf(s_name, "playback-%u", ui2_gen);

        if (x_msg_q_create(& h_playback_handler_msgq,
                           s_name,
                           sizeof(CM_MSG_T),
                           PLAYBACK_HANDLER_MSG_QUEUE_SIZE)
            == OSR_OK)
        {
            PLAYBACK_HANDLER_INFO_T  t_info;
            HANDLE_T                 h_thread;

            t_info.h_playback_handler_msgq = h_playback_handler_msgq;

            if (x_thread_create(& h_thread,
                                s_name,
                                z_stack_size,
                                ui1_priority,
                                playback_handler_thread_main,
                                sizeof(t_info), (VOID*) & t_info)
                == OSR_OK)
            {
                i4_res = cm_reg_handler(ps_name,
                                        & t_playback_fct_tbl,
                                        & h_playback_handler);

                if (i4_res == CMR_OK)
                {
                    playback_handler_cli_init();
                }
                else
                {
                    /* exit thread */
                    cm_send_msg_retry(h_playback_handler_msgq,
                                      PLAYBACK_HANDLER_MSG_EXIT, 0, 0,
                                      NULL_HANDLE);

                    i4_res = CMR_NOT_INIT;
                }
            }
            else
            {
                x_msg_q_delete(h_playback_handler_msgq);
            }
        }

        /* if something failed... */
        if (i4_res != CMR_OK)
        {
            (VOID) playback_handler_set_init(FALSE, NULL);
        }
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_playback_unreg_handler
 *
 * Description: This API unregisters the Playback Connection Handler.
 *
 * Inputs:  b_force  Contains a boolean value that specifies whether or not the
 *                   connections maintained by this handler should be closed.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  The connection handler is not unregistered.
 ----------------------------------------------------------------------------*/
INT32 x_playback_unreg_handler (BOOL b_force)
{
    HANDLE_T  h_handler;
    HANDLE_T  h_msgq;
    INT32     i4_res;

    h_handler = h_playback_handler;
    h_msgq    = h_playback_handler_msgq;

    DBG_API((DBG_PREFIX"x_playback_unreg_handler(%s)\n",
             b_force ? "TRUE" : "FALSE"));

    if (playback_handler_set_init(FALSE, NULL))
    {
        i4_res = cm_unreg_handler(h_handler, b_force);

        if (i4_res == CMR_OK)
        {
            i4_res = cm_send_msg_retry(h_msgq,
                                       PLAYBACK_HANDLER_MSG_EXIT, 0, 0,
                                       NULL_HANDLE);
        }
        else
        {
            i4_res = CMR_FAIL;
        }
    }
    else
    {
        i4_res = CMR_NOT_INIT;
    }

    return i4_res;
}

