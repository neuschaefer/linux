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
 * $RCSfile: avc_handler.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/13 $
 * $SWAuthor: Felisa Hsiao $
 * $MD5HEX: 0ab0deeae753376bea53d108a3e236f2 $
 *
 * Description:
 *         This file contains the AVC Connection Handler main functions,
 *         including the handler registration, unregistration and thread main
 *         function.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "conn_mngr/avc_handler/x_avc_handler.h"
#include "conn_mngr/avc_handler/_avc_handler.h"
#include "conn_mngr/avc_handler/avc_handler_cmd.h"
#include "conn_mngr/avc_handler/avc_handler_debug.h"
#include "os/inc/x_os.h"
#include "conn_mngr/x_cm.h"
#include "conn_mngr/cm.h"
#include "conn_mngr/cm_handler_api.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_avc.h"
#include "scdb/x_scdb_api.h"
#include "scdb/scdb_api.h"
#include "dbg/def_dbg_level_mw.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define AVC_HANDLER_CONN_MAGIC  ((UINT32) 20051229)

#define VALIDATE_CONN(_pt_conn) \
do { \
    if ((_pt_conn)->ui4_magic != AVC_HANDLER_CONN_MAGIC) \
        ABORT(DBG_CAT_NO_RECOVERY, 2); \
} while(0)

#define AVC_HANDLER_ASSERT(_cond) \
do {if (! (_cond)) ABORT(DBG_CAT_NO_RECOVERY, 666);} while(0)

#define AVC_HANDLER_MSG_PRIORITY       ((UINT8)  1)
#define AVC_HANDLER_MSG_RETRIES_COUNT  ((UINT32) 10)
#define AVC_HANDLER_MSG_RETRY_DELAY    ((UINT32) 50)


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

static HANDLE_T  h_avc_handler;
static HANDLE_T  h_avc_handler_msgq;
static BOOL      b_init = FALSE;

static AVC_HANDLER_TIME_OUT_T  t_time_outs;

static const AVC_HANDLER_TIME_OUT_T  t_default_time_outs =
{
    0
};

static UINT16  ui2_avc_handler_dbg_level = DBG_INIT_LEVEL_MW_AVC_HANDLER;
static BOOL    b_avc_handler_dump_scdb   = FALSE;


/*-----------------------------------------------------------------------------
 * Name: avc_handler_get_dbg_level
 *
 * Description: This API gets the debug level of the AVC Handler.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: the AVC Handler debug level.
 ----------------------------------------------------------------------------*/
UINT16 avc_handler_get_dbg_level (VOID)
{
    return ui2_avc_handler_dbg_level;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_set_dbg_level
 *
 * Description: This API sets the debug level of the AVC Handler.
 *
 * Inputs:  ui2_dbg_level  Contains the new debug level.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The debug level was set successfully.
 *          CMR_FAIL  The debug level could not be set.
 ----------------------------------------------------------------------------*/
INT32 avc_handler_set_dbg_level (UINT16  ui2_dbg_level)
{
    ui2_avc_handler_dbg_level = ui2_dbg_level;

    return CMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_set_dump_scdb
 *
 * Description: This API specifies whether the AVC Handler shows the content of
 *              the SCDB.
 *
 * Inputs:  b_dump_scdb  Specifies whether the SCDB is dumped.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID avc_handler_set_dump_scdb (BOOL  b_dump_scdb)
{
    b_avc_handler_dump_scdb = b_dump_scdb;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_timer_cb
 *
 * Description: This API is called when the a timer expires and sends a
 *              message to the message loop. If sending the message failed the
 *              timer is started again with a delay of
 *              AVC_HANDLER_MSG_RETRY_DELAY milliseconds.
 *
 * Inputs:  h_timer  Contains the timer handle.
 *          e_code   Contains the message code.
 *          pf_cb    References the timer callback routine.
 *          pv_tag   Contains the timer tag (in this case: a connection handle)
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_timer_cb (HANDLE_T           h_timer,
                                  UINT32             e_code,
                                  x_os_timer_cb_fct  pf_cb,
                                  VOID*              pv_tag)
{
    HANDLE_T  h_ch_conn;
    INT32     i4_res;

    h_ch_conn = (HANDLE_T) pv_tag;

    i4_res = cm_send_msg_no_retry(h_avc_handler_msgq,
                                  e_code, 0, 0,
                                  h_ch_conn);

    if (i4_res != CMR_OK)
    {
        x_timer_start(h_timer,
                      AVC_HANDLER_MSG_RETRY_DELAY,
                      X_TIMER_FLAG_ONCE,
                      pf_cb,
                      pv_tag);
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_tv_signal_timer_cb
 *
 * Description: This API is called when the TV signal timer expires and sends a
 *              message to the message loop.
 *
 * Inputs:  h_timer  Contains the timer handle.
 *          pv_tag   Contains the timer tag (in this case: a connection handle)
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_tv_signal_timer_cb (HANDLE_T  h_timer,
                                            VOID*     pv_tag)
{
    avc_handler_timer_cb(h_timer,
                         AVC_HANDLER_MSG_TV_SIGNAL_TIMER_EXPIRED,
                         avc_handler_tv_signal_timer_cb,
                         pv_tag);
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_conn_disconnected
 *
 * Description: This API executes the final steps of the disconnection, i.e.
 *              deleting the hander and notifying the client.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_conn_disconnected (AVC_HANDLER_CONN_T*  pt_conn)
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
 * Name: avc_handler_avc_closed
 *
 * Description: This API is called when the AVC component is closed. If a
 *              connection was using the component then the connection handle
 *              is deleted.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_avc_closed (AVC_HANDLER_CONN_T*  pt_conn)
{
    pt_conn->h_rm_avc = NULL_HANDLE;

    if (pt_conn->h_rm_tv_decoder == NULL_HANDLE)
    {
        avc_handler_conn_disconnected(pt_conn);
    }
    else
    {
        DBG_INFO((DBG_PREFIX"AVC closed, but TV decoder not closed yet\n"));
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_close_avc
 *
 * Description: This API closes the AVC driver component.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_close_avc (AVC_HANDLER_CONN_T*  pt_conn)
{
    INT32  i4_res;

    DBG_INFO((DBG_PREFIX"AVC being closed (%d)\n",
              pt_conn->h_ch_conn));

    i4_res = rm_close(pt_conn->h_rm_avc, FALSE, NULL);

    if (i4_res == RMR_OK)
    {
        DBG_INFO((DBG_PREFIX"AVC closed synchronously (%d)\n",
                  pt_conn->h_ch_conn));

        avc_handler_avc_closed(pt_conn);
    }
    else if (i4_res == RMR_ASYNC_NFY)
    {
        /* nothing to do here */
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"AVC could not be closed (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_res));
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_tv_decoder_closed
 *
 * Description: This API is called when the TV decoder component is closed.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_tv_decoder_closed (AVC_HANDLER_CONN_T*  pt_conn)
{
    pt_conn->h_rm_tv_decoder = NULL_HANDLE;

    if (pt_conn->h_rm_avc == NULL_HANDLE)
    {
        avc_handler_conn_disconnected(pt_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_close_tv_decoder
 *
 * Description: This API closed the TV decoder driver component.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_close_tv_decoder (AVC_HANDLER_CONN_T*  pt_conn)
{
    INT32  i4_res;

    DBG_INFO((DBG_PREFIX"Decoder being closed (%d)\n",
              pt_conn->h_ch_conn));

    i4_res = rm_close(pt_conn->h_rm_tv_decoder, FALSE, NULL);

    if (i4_res == RMR_OK)
    {
        DBG_INFO((DBG_PREFIX"Decoder closed synchronously (%d)\n",
                  pt_conn->h_ch_conn));

        avc_handler_tv_decoder_closed(pt_conn);
    }
    else if (i4_res == RMR_ASYNC_NFY)
    {
        /* nothing to do here */
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Decoder could not be closed (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_res));
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_close_conn
 *
 * Description: This API closes the TV decoder and the AVC (if needed).
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_close_conn (AVC_HANDLER_CONN_T*  pt_conn)
{
    if (! pt_conn->b_closing)
    {
        HANDLE_T  h_rm_avc;
        BOOL      b_conn_disconnected;

        pt_conn->b_closing = TRUE;

        h_rm_avc = pt_conn->h_rm_avc;

        b_conn_disconnected =
            (pt_conn->h_rm_tv_decoder == NULL_HANDLE)
            &&
            (pt_conn->h_rm_avc == NULL_HANDLE);

        if (pt_conn->h_rm_tv_decoder != NULL_HANDLE)
        {
            avc_handler_close_tv_decoder(pt_conn);
        }

        if (h_rm_avc != NULL_HANDLE)
        {
            avc_handler_close_avc(pt_conn);
        }

        if (b_conn_disconnected)
        {
            avc_handler_conn_disconnected(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_avc_disconnected
 *
 * Description: This API is called when the AVC component is disconnected.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_avc_disconnected (AVC_HANDLER_CONN_T*  pt_conn)
{
    if (! pt_conn->b_avc_disconnected)
    {
        pt_conn->b_avc_disconnected = TRUE;

        if (pt_conn->b_tv_decoder_disconnected
            ||
            (pt_conn->e_rm_connect_decoder_cond == RM_COND_IGNORE))
        {
            avc_handler_close_conn(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_disconnect_avc
 *
 * Description: This API disconnects the AVC driver component.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_disconnect_avc (AVC_HANDLER_CONN_T*  pt_conn)
{
    INT32  i4_res;

    DBG_INFO((DBG_PREFIX"AVC being disconnected (%d)\n",
              pt_conn->h_ch_conn));

    i4_res = rm_disconnect(pt_conn->h_rm_avc,
                           RM_DISC_TYPE_IGNORE,
                           NULL, /* disconnection info */
                           0, /* disconnection info size */
                           & pt_conn->e_rm_disconnect_avc_cond);

    if (i4_res == RMR_OK)
    {
        DBG_INFO((DBG_PREFIX"AVC disconnected synchronously (%d)\n",
                  pt_conn->h_ch_conn));
    }
    else if (i4_res == RMR_ASYNC_NFY)
    {
        /* nothing to do here */
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Could not disconnect AVC (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_res));
    }

    avc_handler_avc_disconnected(pt_conn);
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_tv_decoder_disconnected
 *
 * Description: This API is called when the TV decoder is disconnected.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_tv_decoder_disconnected (AVC_HANDLER_CONN_T*  pt_conn)
{
    if (! pt_conn->b_tv_decoder_disconnected)
    {
        pt_conn->b_tv_decoder_disconnected = TRUE;

        if (pt_conn->b_avc_disconnected)
        {
            avc_handler_close_conn(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_disconnect_tv_decoder
 *
 * Description: This API disconnects the decoder driver component.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_disconnect_tv_decoder (AVC_HANDLER_CONN_T*  pt_conn)
{
    INT32  i4_res;

    DBG_INFO((DBG_PREFIX"Decoder being disconnected (%d)\n",
              pt_conn->h_ch_conn));

    if (pt_conn->b_tv_decoder_ctrl_play_done)
    {
        i4_res = rm_set(pt_conn->h_rm_tv_decoder,
                        FALSE,
                        ANY_PORT_NUM,
                        TV_DEC_SET_TYPE_CTRL,
                        (VOID*) TV_DEC_CTRL_STOP,
                        sizeof(TV_DEC_CTRL_T),
                        NULL);
    }

    i4_res = rm_disconnect(pt_conn->h_rm_tv_decoder,
                           RM_DISC_TYPE_IGNORE,
                           NULL, /* disconnection info */
                           0, /* disconnection info size */
                           & pt_conn->e_rm_disconnect_decoder_cond);

    if (i4_res == RMR_OK)
    {
        DBG_INFO((DBG_PREFIX"Decoder disconnected synchronously (%d)\n",
                  pt_conn->h_ch_conn));
    }
    else if (i4_res == RMR_ASYNC_NFY)
    {
        /* nothing to do here */
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Decoder could not be disconnected (%d)\n",
                   i4_res));
    }

    avc_handler_tv_decoder_disconnected(pt_conn);
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_disconnect_conn
 *
 * Description: This API requests the disconnection of the AVC driver
 *              component.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_disconnect_conn (AVC_HANDLER_CONN_T*  pt_conn,
                                         UINT32               ui4_reason)
{
    if (! pt_conn->b_disconnecting)
    {
        BOOL  b_close_conn;

        DBG_INFO((DBG_PREFIX"Disconnecting connection (%d, reason=%d)\n",
                  pt_conn->h_ch_conn, ui4_reason));

        pt_conn->b_disconnecting = TRUE;

        pt_conn->ui4_disconnection_reason = ui4_reason;

        b_close_conn =
            (pt_conn->e_rm_connect_decoder_cond == RM_COND_IGNORE)
            &&
            (pt_conn->e_rm_connect_avc_cond == RM_COND_IGNORE);

        if (pt_conn->e_rm_connect_decoder_cond != RM_COND_IGNORE)
        {
            avc_handler_disconnect_tv_decoder(pt_conn);
        }

        if (pt_conn->e_rm_connect_avc_cond != RM_COND_IGNORE)
        {
            avc_handler_disconnect_avc(pt_conn);
        }

        if (b_close_conn)
        {
            avc_handler_close_conn(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_get_stream_tag
 *
 * Description: This API returns a stream tag (i.e. a VOID* pointer).
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: a non-NULL pointer that is used as a stream tag.
 ----------------------------------------------------------------------------*/
static VOID* avc_handler_get_stream_tag(VOID)
{
    static VOID*  pv_stream_tag = NULL;

    pv_stream_tag = (VOID*) (((CHAR*)pv_stream_tag) + 1);

    if (pv_stream_tag == NULL)
    {
        pv_stream_tag = (VOID*) (((CHAR*)pv_stream_tag) + 1);
    }

    return pv_stream_tag;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_dump_scdb
 *
 * Description: This API prints out the streams of a SCDB.
 *
 * Inputs:  h_scdb  Contains a handle to a SCDB.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_dump_scdb (HANDLE_T  h_scdb)
{
#ifdef DEBUG
    STREAM_COMP_ID_T  t_stream_comp_id;
    SCDB_REC_T        t_scdb_rec;
    UINT32            ui4_version;
    UINT16            ui2_index = 0;

    x_dbg_stmt(DBG_PREFIX"SCDB content:\n");

    ui4_version = SCDB_NULL_VER_ID;

    while (x_scdb_get_rec_by_idx(h_scdb,
                                 ST_UNKNOWN,
                                 ui2_index,
                                 & t_stream_comp_id,
                                 & t_scdb_rec,
                                 & ui4_version)
           == SCDBR_OK)
    {
        switch (t_stream_comp_id.e_type)
        {
            case ST_VIDEO:
            {
                SCDB_VIDEO_ANALOG_T*  pt_video_analog;

                pt_video_analog = & t_scdb_rec.u.t_video_analog;

                x_dbg_stmt(DBG_PREFIX"Video (color system %d, tag 0x%08x)\n",
                           pt_video_analog->e_color_sys,
                           t_stream_comp_id.pv_stream_tag);
            }
            break;

            case ST_AUDIO:
            {
                x_dbg_stmt(DBG_PREFIX"Audio (tag 0x%08x)\n",
                           t_stream_comp_id.pv_stream_tag);
            }
            break;

            case ST_CLOSED_CAPTION:
            {
                SCDB_CLOSED_CAPTION_T*  pt_atsc_cc;

                pt_atsc_cc = & t_scdb_rec.u.t_atsc_cc;

                x_dbg_stmt
                    (DBG_PREFIX"CC%d \"%s\"  E%d W%d %s (tag 0x%08x)\n",
                     pt_atsc_cc->udata.t_cc_atsc_line_21.ui1_cc_id,
                     pt_atsc_cc->s_lang,
                     pt_atsc_cc->udata.t_cc_atsc_line_21.b_easy_reader,
                     pt_atsc_cc->udata.t_cc_atsc_line_21.b_wide_aspect,
                     pt_atsc_cc->udata.t_cc_atsc_line_21.b_det_in_descr?
                     "DETECTED" : "UNDETECTED",
                     t_stream_comp_id.pv_stream_tag);
            }
            break;

            case ST_UNKNOWN:
            default:
                x_dbg_stmt(DBG_PREFIX"Unknown stream type (%d)\n",
                           t_stream_comp_id.e_type);
                break;
        }

        ui2_index++;
    }

    x_dbg_stmt(DBG_PREFIX"End of SCDB content\n");
#endif
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_add_scdb_cc_streams
 *
 * Description: This API adds the closed caption streams to the SCDB.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_add_scdb_cc_streams (AVC_HANDLER_CONN_T*  pt_conn)
{
    if (! cm_scdb_has_stream_type(pt_conn->h_scdb, ST_CLOSED_CAPTION))
    {
        STREAM_COMP_ID_T  t_stream_desc;
        SCDB_REC_T        t_scdb_rec = {0};
        UINT8             ui1_i;

        DBG_INFO((DBG_PREFIX"Adding CC streams to SCDB (%d)\n",
                  pt_conn->h_ch_conn));

        t_stream_desc.e_type  = ST_CLOSED_CAPTION;
        t_scdb_rec.e_rec_type = SCDB_REC_TYPE_CLOSED_CAPTION;

        t_scdb_rec.u.t_atsc_cc.e_type    = CC_ATSC_LINE_21;
        t_scdb_rec.u.t_atsc_cc.s_lang[0] = '\0';
        t_scdb_rec.u.t_atsc_cc.b_default = FALSE;

        t_scdb_rec.u.t_atsc_cc.udata.t_cc_atsc_line_21.b_easy_reader  = FALSE;
        t_scdb_rec.u.t_atsc_cc.udata.t_cc_atsc_line_21.b_wide_aspect  = FALSE;
        t_scdb_rec.u.t_atsc_cc.udata.t_cc_atsc_line_21.b_det_in_descr = FALSE;

        for (ui1_i = 1; ui1_i <= 4; ui1_i++)
        {
            t_stream_desc.pv_stream_tag = avc_handler_get_stream_tag();

            t_scdb_rec.u.t_atsc_cc.udata.t_cc_atsc_line_21.ui1_cc_id = ui1_i;

            scdb_add_rec(pt_conn->h_scdb, & t_stream_desc, & t_scdb_rec);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_remove_scdb_cc_streams
 *
 * Description: This API removes all the closed caption streams from the SCDB.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_remove_scdb_cc_streams (AVC_HANDLER_CONN_T*  pt_conn)
{
    cm_remove_stream_type_from_scdb(pt_conn->h_scdb, ST_CLOSED_CAPTION);
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_add_audio_to_scdb
 *
 * Description: This API adds an audio stream to the SCDB.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  The operation could not be fully completed.
 ----------------------------------------------------------------------------*/
static INT32 avc_handler_add_audio_to_scdb (AVC_HANDLER_CONN_T*  pt_conn)
{
    HANDLE_T  h_scdb;
    INT32     i4_res;

    DBG_INFO((DBG_PREFIX"Adding audio stream to SCDB (%d)\n",
              pt_conn->h_ch_conn));

    h_scdb = pt_conn->h_scdb;

    i4_res = scdb_lock(h_scdb);

    if (i4_res == SCDBR_OK)
    {
        STREAM_COMP_ID_T  t_stream_comp_id;
        SCDB_REC_T        t_scdb_rec = {0};

        t_stream_comp_id.e_type        = ST_AUDIO;
        t_stream_comp_id.pv_stream_tag = avc_handler_get_stream_tag();

        t_scdb_rec.e_rec_type = SCDB_REC_TYPE_AUDIO_ANALOG;

        t_scdb_rec.u.t_audio_analog.e_aud_fmt = AUD_FMT_UNKNOWN;
        t_scdb_rec.u.t_audio_analog.e_enc     = AUD_ENC_PCM;
        t_scdb_rec.u.t_audio_analog.b_default = TRUE;
        t_scdb_rec.u.t_audio_analog.b_enable_aud_sys_auto_detect = FALSE;

        i4_res = scdb_add_rec(h_scdb, & t_stream_comp_id, & t_scdb_rec);

        pt_conn->b_scdb_has_audio = (i4_res == SCDBR_OK);

        scdb_unlock(h_scdb);

        i4_res = (i4_res == SCDBR_OK) ? CMR_OK : CMR_FAIL;

        if (b_avc_handler_dump_scdb)
        {
            avc_handler_dump_scdb(pt_conn->h_scdb);
        }
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Could not lock SCDB (%d)\n", i4_res));

        i4_res = CMR_FAIL;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_remove_audio_from_scdb
 *
 * Description: This API removes all the audio streams from the SCDB.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_remove_audio_from_scdb (AVC_HANDLER_CONN_T*  pt_conn)
{
    INT32  i4_res;

    DBG_INFO((DBG_PREFIX"Removing audio stream from SCDB (%d)\n",
              pt_conn->h_ch_conn));

    i4_res = scdb_lock(pt_conn->h_scdb);

    if (i4_res == SCDBR_OK)
    {
        i4_res = cm_remove_stream_type_from_scdb(pt_conn->h_scdb, ST_AUDIO);

        pt_conn->b_scdb_has_audio = (i4_res != CMR_OK);

        if (b_avc_handler_dump_scdb)
        {
            avc_handler_dump_scdb(pt_conn->h_scdb);
        }

        scdb_unlock(pt_conn->h_scdb);
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Could not lock SCDB (%d)\n", i4_res));
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_add_video_to_scdb
 *
 * Description: This API adds a video stream to the SCDB.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  The operation could not be fully completed.
 ----------------------------------------------------------------------------*/
static INT32 avc_handler_add_video_to_scdb (AVC_HANDLER_CONN_T*  pt_conn)
{
    HANDLE_T  h_scdb;
    INT32     i4_res;

    DBG_INFO((DBG_PREFIX"Adding video stream to SCDB (%d)\n",
              pt_conn->h_ch_conn));

    h_scdb = pt_conn->h_scdb;

    i4_res = scdb_lock(h_scdb);

    if (i4_res == SCDBR_OK)
    {
        STREAM_COMP_ID_T  t_stream_comp_id;
        SCDB_REC_T        t_scdb_rec = {0};

        t_stream_comp_id.e_type        = ST_VIDEO;
        t_stream_comp_id.pv_stream_tag = avc_handler_get_stream_tag();

        t_scdb_rec.e_rec_type = SCDB_REC_TYPE_VIDEO_ANALOG;

        t_scdb_rec.u.t_video_analog.e_vid_fmt   = VID_FMT_UNKNOWN;
        t_scdb_rec.u.t_video_analog.e_color_sys = COLOR_SYS_UNKNOWN;
        t_scdb_rec.u.t_video_analog.b_default   = TRUE;
        t_scdb_rec.u.t_video_analog.b_detected  = FALSE;

        i4_res = scdb_add_rec(pt_conn->h_scdb,
                              & t_stream_comp_id,
                              & t_scdb_rec);

        if (b_avc_handler_dump_scdb)
        {
            avc_handler_dump_scdb(pt_conn->h_scdb);
        }

        scdb_unlock(pt_conn->h_scdb);

        i4_res = (i4_res == SCDBR_OK) ? CMR_OK : CMR_FAIL;
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Could not lock SCDB (%d)\n", i4_res));

        i4_res = CMR_FAIL;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_set_video_detected
 *
 * Description: This API marks the video stream in the SCDB as 'detected' or
 *              'undetected'.
 *
 * Inputs:  pt_conn     References the connection.
 *          b_detected  Specifies whether the video stream is now detected.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  The operation could not be fully completed.
 ----------------------------------------------------------------------------*/
static INT32 avc_handler_set_video_detected (AVC_HANDLER_CONN_T*  pt_conn,
                                             BOOL                 b_detected)
{
    INT32  i4_res;

    i4_res = scdb_lock(pt_conn->h_scdb);

    if (i4_res == SCDBR_OK)
    {
        STREAM_COMP_ID_T  t_stream_comp_id;
        SCDB_REC_T        t_scdb_rec;
        UINT32            ui4_version;

        ui4_version = SCDB_NULL_VER_ID;

        i4_res = x_scdb_get_rec_by_idx(pt_conn->h_scdb,
                                       ST_VIDEO, 0,
                                       & t_stream_comp_id,
                                       & t_scdb_rec,
                                       & ui4_version);

        if (i4_res == SCDBR_OK)
        {
            t_scdb_rec.u.t_video_analog.b_detected = b_detected;

            i4_res = scdb_update_rec(pt_conn->h_scdb,
                                     & t_stream_comp_id,
                                     & t_scdb_rec);
        }

        scdb_unlock(pt_conn->h_scdb);

        i4_res = (i4_res == SCDBR_OK) ? CMR_OK : CMR_FAIL;
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Could not lock SCDB (%d)\n", i4_res));

        i4_res = CMR_FAIL;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_video_detected
 *
 * Description: This API marks the video stream in the SCDB as 'detected'.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  The operation could not be fully completed.
 ----------------------------------------------------------------------------*/
static INT32 avc_handler_video_detected (AVC_HANDLER_CONN_T*  pt_conn)
{
    INT32  i4_res;

    DBG_INFO((DBG_PREFIX"Marking video stream as detected (%d)\n",
              pt_conn->h_ch_conn));

    i4_res = avc_handler_set_video_detected(pt_conn, TRUE);

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_video_undetected
 *
 * Description: This API marks the video stream in the SCDB as 'undetected'.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  The operation could not be fully completed.
 ----------------------------------------------------------------------------*/
static INT32 avc_handler_video_undetected (AVC_HANDLER_CONN_T*  pt_conn)
{
    INT32  i4_res;

    DBG_INFO((DBG_PREFIX"Marking video stream as undetected (%d)\n",
              pt_conn->h_ch_conn));

    i4_res = avc_handler_set_video_detected(pt_conn, FALSE);

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_create_scdb
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
static INT32 avc_handler_create_scdb (AVC_HANDLER_CONN_T*  pt_conn)
{
    SCDB_COMMON_T  t_scdb_common;
    INT32          i4_res = CMR_OK;

    t_scdb_common.e_data_fmt  = DATA_FMT_ANALOG;
    t_scdb_common.e_feed_mode = FEED_MODE_PUSH;

    i4_res = scdb_create(pt_conn->h_cm_conn,
                         & t_scdb_common,
                         & pt_conn->h_scdb);

    i4_res = (i4_res == SCDBR_OK) ? CMR_OK : CMR_FAIL;

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_nfy_connected
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
static VOID avc_handler_nfy_connected (AVC_HANDLER_CONN_T*  pt_conn)
{
    if (! pt_conn->b_cm_nfy_connected)
    {
        cm_nfy(pt_conn->h_cm_conn, CM_COND_CONNECTED, 0);

        pt_conn->b_cm_nfy_connected = TRUE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_create_timer
 *
 * Description: This API creates a timer (if needed) for a connection. The
 *              timer is used for TV signal detection.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The timer has been created successfully or not timer was
 *                    needed.
 *          CMR_FAIL  The timer could not be created.
 ----------------------------------------------------------------------------*/
static INT32 avc_handler_create_timer (AVC_HANDLER_CONN_T*  pt_conn)
{
    INT32  i4_res;

    if (pt_conn->t_connect_info.t_time_outs.ui4_tv_signal_time_out != 0)
    {
        i4_res = x_timer_create(& pt_conn->h_timer);

        i4_res = (i4_res == OSR_OK) ? CMR_OK : CMR_FAIL;
    }
    else
    {
        i4_res = CMR_OK;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_avc_nfy
 *
 * Description: This API is called when the AVC detects signal, a new
 *              color system or when an asynchronous operation is completed.
 *
 * Inputs:  pv_tag      Contains the tag value.
 *          e_nfy_cond  Contains the notify condition.
 *          ui4_data_1  Contains additional data (depending on e_nfy_cond).
 *          ui4_data_2  Contains additional data (depending on e_nfy_cond).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_avc_nfy (VOID*       pv_tag,
                                 AVC_COND_T  e_nfy_cond,
                                 UINT32      ui4_data_1,
                                 UINT32      ui4_data_2)
{
    UINT32  e_code;

    switch (e_nfy_cond)
    {
        case AVC_COND_SIGNAL_STATUS:
            e_code = AVC_HANDLER_MSG_AVC_SIGNAL_STATUS;
            break;

        default:
            e_code = AVC_HANDLER_MSG_NONE;
    }

    if (e_code != AVC_HANDLER_MSG_NONE)
    {
        HANDLE_T  h_ch_conn;

        h_ch_conn = (HANDLE_T) pv_tag;

        cm_send_msg_no_retry(h_avc_handler_msgq,
                             e_code, ui4_data_1, 0,
                             h_ch_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_avc_signal_status
 *
 * Description: This API is called when the signal status of the AVC component
 *              changes.
 *
 * Inputs:  pt_conn    References the connection.
 *          ui4_flags  Contains the signal status flags.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_avc_signal_status (AVC_HANDLER_CONN_T*  pt_conn,
                                           UINT32               ui4_flags)
{
    if (ui4_flags & AVC_STATE_TV_SIGNAL)
    {
        if (! pt_conn->b_scdb_video_detected)
        {
            avc_handler_video_detected(pt_conn);

            pt_conn->b_scdb_video_detected = TRUE;
        }
    }
    else
    {
        if (pt_conn->b_scdb_video_detected)
        {
            avc_handler_video_undetected(pt_conn);

            pt_conn->b_scdb_video_detected = FALSE;
        }
    }

    if (ui4_flags & AVC_STATE_AUDIO_SIGNAL)
    {
        if (! pt_conn->b_scdb_has_audio)
        {
            avc_handler_add_audio_to_scdb(pt_conn);
        }
    }
    else
    {
        if (pt_conn->b_scdb_has_audio)
        {
            avc_handler_remove_audio_from_scdb(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_tv_decoder_nfy
 *
 * Description: This API is called when the TV decoder detects signal, a new
 *              color system or when an asynchronous operation is completed.
 *
 * Inputs:  pv_tag      Contains the tag value.
 *          e_nfy_cond  Contains the notify condition.
 *          ui4_data_1  Contains additional data (depending on e_nfy_cond).
 *          ui4_data_2  Contains additional data (depending on e_nfy_cond).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_tv_decoder_nfy (VOID*          pv_tag,
                                        TV_DEC_COND_T  e_nfy_cond,
                                        UINT32         ui4_data_1,
                                        UINT32         ui4_data_2)
{
    UINT32  e_code;

    switch (e_nfy_cond)
    {
        case TV_DEC_COND_TV_SIGNAL:
        {
            switch ((TV_DEC_TV_SIG_T) ui4_data_1)
            {
                case TV_DEC_TV_SIG_STABLE:
                    e_code = AVC_HANDLER_MSG_TV_DEC_GOT_TV_SIGNAL;
                    break;
                case TV_DEC_TV_SIG_NONE:
                    e_code = AVC_HANDLER_MSG_TV_DEC_LOST_TV_SIGNAL;
                    break;
                default:
                    e_code = AVC_HANDLER_MSG_NONE;
                    break;
            }
        }
        break;

        case TV_DEC_COND_COLOR_SYS:
            e_code = AVC_HANDLER_MSG_TV_DEC_COLOR_SYS;
            break;

        case TV_DEC_COND_CTRL_DONE:
            if ((ui4_data_1 == TV_DEC_SET_TYPE_CTRL)
                &&
                (ui4_data_2 == TV_DEC_CTRL_PLAY))
            {
                e_code = AVC_HANDLER_MSG_TV_DEC_PLAY_DONE;
            }
            else
            {
                e_code = AVC_HANDLER_MSG_NONE;
            }
            break;

        default:
            e_code = AVC_HANDLER_MSG_NONE;
    }

    if (e_code != AVC_HANDLER_MSG_NONE)
    {
        HANDLE_T  h_ch_conn;

        h_ch_conn = (HANDLE_T) pv_tag;

        cm_send_msg_no_retry(h_avc_handler_msgq,
                             e_code, 0, 0,
                             h_ch_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_update_scdb_color_sys
 *
 * Description: This API updates the color system of the SCDB video stream.
 *
 * Inputs:  pt_conn          References the connection.
 *          e_drv_color_sys  Contains the new color system.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The SCDB has been updated successfully.
 *          CMR_FAIL  The SCDB could not be updated.
 ----------------------------------------------------------------------------*/
static INT32
avc_handler_update_scdb_color_sys (AVC_HANDLER_CONN_T*  pt_conn,
                                   TV_DEC_COLOR_SYS_T   e_drv_color_sys)
{
    INT32  i4_res;

    if (scdb_lock(pt_conn->h_scdb) == SCDBR_OK)
    {
        STREAM_COMP_ID_T  t_stream_comp_id;
        SCDB_REC_T        t_scdb_rec;
        UINT32            ui4_version;

        ui4_version = SCDB_NULL_VER_ID;

        i4_res = x_scdb_get_rec_by_idx(pt_conn->h_scdb,
                                       ST_VIDEO, 0,
                                       & t_stream_comp_id,
                                       & t_scdb_rec,
                                       & ui4_version);

        if (i4_res == SCDBR_OK)
        {
            t_scdb_rec.u.t_video_analog.e_color_sys =
                cm_drv_color_sys_to_color_sys(e_drv_color_sys);

            i4_res = scdb_update_rec(pt_conn->h_scdb,
                                     & t_stream_comp_id,
                                     & t_scdb_rec);

            if (i4_res == SCDBR_OK)
            {
                BOOL  b_closed_caption;

                b_closed_caption =
                    (e_drv_color_sys == TV_DEC_COLOR_SYS_NTSC)
                    ||
                    (e_drv_color_sys == TV_DEC_COLOR_SYS_NTSC_443)
                    ||
                    (e_drv_color_sys == TV_DEC_COLOR_SYS_PAL_M);

                if (b_closed_caption)
                {
                    avc_handler_add_scdb_cc_streams(pt_conn);
                }
                else
                {
                    avc_handler_remove_scdb_cc_streams(pt_conn);
                }

                if (b_avc_handler_dump_scdb)
                {
                    avc_handler_dump_scdb(pt_conn->h_scdb);
                }
            }

            i4_res = (i4_res == SCDBR_OK) ? CMR_OK : CMR_FAIL;
        }
        else
        {
            i4_res = CMR_FAIL;
        }

        scdb_unlock(pt_conn->h_scdb);
    }
    else
    {
        i4_res = CMR_FAIL;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_tv_decoder_color_sys_changed
 *
 * Description: This API is called when the color system of the TV decoder has
 *              changed.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The SCDB was updated successfully.
 *          CMR_FAIL  The SCDB could not be updated.
 ----------------------------------------------------------------------------*/
static INT32
avc_handler_tv_decoder_color_sys_changed (AVC_HANDLER_CONN_T*  pt_conn)
{
    TV_DEC_COLOR_SYS_T  e_drv_color_sys;
    SIZE_T              z_size;
    INT32               i4_res;

    z_size = sizeof(TV_DEC_COLOR_SYS_T);

    i4_res = rm_get(pt_conn->h_rm_tv_decoder,
                    FALSE,
                    ANY_PORT_NUM,
                    TV_DEC_GET_TYPE_COLOR_SYS,
                    (VOID*) & e_drv_color_sys,
                    & z_size,
                    NULL);

    if (i4_res == RMR_OK)
    {
        i4_res = avc_handler_update_scdb_color_sys(pt_conn, e_drv_color_sys);
    }
    else
    {
        i4_res = CMR_FAIL;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_tv_decoder_playing
 *
 * Description: This API is called after the TV decoder sends a PLAYING
 *              notification. The first time the TV decoder is 'playing', the
 *              timer is started (if it has to be).
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_tv_decoder_playing (AVC_HANDLER_CONN_T*  pt_conn)
{
    if (! pt_conn->b_tv_decoder_playing)
    {
        if (pt_conn->b_tv_decoder_for_tv_signal)
        {
            if (! pt_conn->t_connect_info.b_disc_if_tv_signal_lost)
            {
                avc_handler_nfy_connected(pt_conn);
            }
            else
            {
                UINT32  ui4_delay =
                    pt_conn->t_connect_info.t_time_outs.ui4_tv_signal_time_out;

                if (ui4_delay != 0)
                {
                    x_timer_start(pt_conn->h_timer,
                                  ui4_delay,
                                  X_TIMER_FLAG_ONCE,
                                  avc_handler_tv_signal_timer_cb,
                                  (VOID*) pt_conn->h_ch_conn);
                }
            }
        }

        pt_conn->b_tv_decoder_playing = TRUE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_tv_decoder_got_tv_signal
 *
 * Description: This API is called when the TV decoder detects a TV signal. The
 *              video stream in the SCDB is marked as 'detected' and a
 *              CONNECTED notification may be sent.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_tv_decoder_got_tv_signal (AVC_HANDLER_CONN_T*  pt_conn)
{
    if (pt_conn->h_timer != NULL_HANDLE)
    {
        x_timer_delete(pt_conn->h_timer);

        pt_conn->h_timer = NULL_HANDLE;
    }

    if (! pt_conn->b_scdb_video_detected)
    {
        avc_handler_video_detected(pt_conn);

        pt_conn->b_scdb_video_detected = TRUE;
    }

    avc_handler_nfy_connected(pt_conn);

    cm_nfy(pt_conn->h_cm_conn,
           CM_COND_CONNECTION_INFO,
           CM_CONN_INFO_GOT_TV_SIGNAL);

    pt_conn->b_tv_signal_detected = TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_tv_decoder_lost_tv_signal
 *
 * Description: This API is called when the TV decoder losts the TV signal. The
 *              video stream in the SCDB is marked as 'undetected' and a
 *              disconnection may occur.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_tv_decoder_lost_tv_signal (AVC_HANDLER_CONN_T*  pt_conn)
{
    if (pt_conn->t_connect_info.b_disc_if_tv_signal_lost)
    {
        avc_handler_disconnect_conn(pt_conn,
                                    AVC_DISC_TV_SIGNAL_LOST);
    }
    else
    {
        if (pt_conn->b_scdb_video_detected)
        {
            avc_handler_video_undetected(pt_conn);

            pt_conn->b_scdb_video_detected = FALSE;
        }

        cm_nfy(pt_conn->h_cm_conn,
               CM_COND_CONNECTION_INFO,
               CM_CONN_INFO_LOST_TV_SIGNAL);
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_scart_mode_to_drv_scart_mode
 *
 * Description: This API converts an AVC handler SCART mode to a driver SCART
 *              mode.
 *
 * Inputs:  e_mode  Contains the SCART mode as defined in u_avc_handler.h.
 *
 * Outputs: -
 *
 * Returns: the driver scart mode.
 ----------------------------------------------------------------------------*/
static TV_DEC_SCART_MODE_T
avc_handler_scart_mode_to_drv_scart_mode (AVC_HANDLER_SCART_MODE_T  e_mode)
{
    TV_DEC_SCART_MODE_T  e_tv_dec_scart_mode;

    switch (e_mode)
    {
        case AVC_HANDLER_SCART_MODE_S_VIDEO:
            e_tv_dec_scart_mode = TV_DEC_SCART_MODE_S_VIDEO;
            break;
        case AVC_HANDLER_SCART_MODE_COMPOSITE:
            e_tv_dec_scart_mode = TV_DEC_SCART_MODE_COMPOSITE;
            break;
        case AVC_HANDLER_SCART_MODE_RGB:
            e_tv_dec_scart_mode = TV_DEC_SCART_MODE_RGB;
            break;
        case AVC_HANDLER_SCART_MODE_MIX:
            e_tv_dec_scart_mode = TV_DEC_SCART_MODE_MIX;
            break;
        case AVC_HANDLER_SCART_MODE_AUTO:
            e_tv_dec_scart_mode = TV_DEC_SCART_MODE_AUTO;
            break;
        case AVC_HANDLER_SCART_MODE_COMPONENT:
            e_tv_dec_scart_mode = TV_DEC_SCART_MODE_COMPONENT;
            break;
        case AVC_HANDLER_SCART_MODE_UNKNOWN:
        default:
            e_tv_dec_scart_mode = TV_DEC_SCART_MODE_UNKNOWN;
            break;
    }

    return e_tv_dec_scart_mode;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_start_tv_decoder
 *
 * Description: This API is called when both AVC and TV decoder are connected
 *              and the decoder must be 'started'.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_start_tv_decoder (AVC_HANDLER_CONN_T*  pt_conn)
{
    TV_DEC_NFY_INFO_T  t_nfy_info;
    INT32              i4_res;

    t_nfy_info.pv_tag        = (VOID*) pt_conn->h_ch_conn;
    t_nfy_info.pf_tv_dec_nfy = avc_handler_tv_decoder_nfy;

    /* rm_set TV_DEC_SET_TYPE_NFY_FCT */
    i4_res = rm_set(pt_conn->h_rm_tv_decoder,
                    FALSE,
                    ANY_PORT_NUM,
                    TV_DEC_SET_TYPE_NFY_FCT,
                    (VOID*) & t_nfy_info,
                    sizeof(TV_DEC_NFY_INFO_T),
                    NULL);

    if (i4_res == RMR_OK)
    {
        TV_DEC_COLOR_SYS_T  t_color_sys;

        if ((pt_conn->t_connect_info.e_device_type == DEV_AVC_SCART)
            &&
            (pt_conn->ui4_flags & AVC_HANDLER_SCART_INP_MODE_MASK))
        {
            AVC_HANDLER_SCART_MODE_T  e_scart_mode;
            TV_DEC_SCART_MODE_T       e_tv_dec_scart_mode;

            /* for SCART component, the SCART mode can be set */

            e_scart_mode = pt_conn->t_connect_info.e_scart_mode;

            e_tv_dec_scart_mode =
                avc_handler_scart_mode_to_drv_scart_mode(e_scart_mode);

            i4_res = rm_set(pt_conn->h_rm_tv_decoder,
                            FALSE,
                            ANY_PORT_NUM,
                            TV_DEC_SET_TYPE_SCART_INP_MODE,
                            (VOID*) & e_tv_dec_scart_mode,
                            sizeof(TV_DEC_SCART_MODE_T),
                            NULL);

            if ((i4_res != RMR_OK) && (i4_res != RMR_ASYNC_NFY))
            {
                DBG_ERROR((DBG_PREFIX
                           "Could not set TV decoder's SCART input mode "
                           "(%d, res=%d)\n",
                           pt_conn->h_ch_conn, i4_res));
            }
        }

        /* Only composite video(av), s-vide, and scart uses TV decoder, */
        /* set color system to UNKNOWN to enable auto-detection */
        /* For all other source(such as component, HDMI), TV-D is not needed,
           and we do not set TVD audio system. Because other connections
           may be using TVD (such as ATV) */
        if ((pt_conn->t_connect_info.e_device_type == DEV_AVC_COMP_VIDEO) ||
            (pt_conn->t_connect_info.e_device_type == DEV_AVC_S_VIDEO)    ||
            (pt_conn->t_connect_info.e_device_type == DEV_AVC_SCART))
        {
            t_color_sys = TV_DEC_COLOR_SYS_UNKNOWN;

            i4_res = rm_set(pt_conn->h_rm_tv_decoder,
                            FALSE,
                            ANY_PORT_NUM,
                            TV_DEC_SET_TYPE_COLOR_SYS,
                            (VOID*) t_color_sys,
                            sizeof(TV_DEC_COLOR_SYS_T),
                            NULL);

            if ((i4_res != RMR_OK) && (i4_res != RMR_ASYNC_NFY))
            {
                DBG_ERROR((DBG_PREFIX
                           "Could not set TV decoder's color system "
                           "(%d, res=%d)\n",
                           pt_conn->h_ch_conn, i4_res));
            }
        }

        /* rm_set TV_DEC_SET_TYPE_CTRL */
        i4_res = rm_set(pt_conn->h_rm_tv_decoder,
                        FALSE,
                        ANY_PORT_NUM,
                        TV_DEC_SET_TYPE_CTRL,
                        (VOID*) TV_DEC_CTRL_PLAY,
                        sizeof(TV_DEC_CTRL_T),
                        NULL);

        if ((i4_res != RMR_OK) && (i4_res != RMR_ASYNC_NFY))
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not 'play' TV decoder (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));
        }

        pt_conn->b_tv_decoder_ctrl_play_done =
            (i4_res == RMR_OK) || (i4_res == RMR_ASYNC_NFY);
    }
    else
    {
        DBG_ERROR((DBG_PREFIX
                   "Could not set TV decoder's nfy function (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_res));
    }

    if ((i4_res != RMR_OK) && (i4_res != RMR_ASYNC_NFY))
    {
        avc_handler_disconnect_conn(
            pt_conn,
            AVC_DISC_COULD_NOT_SET_TV_DECODER_PROPERTIES);
    }

    /* AVC notify connected after start tvd */
    if (pt_conn->b_scdb_has_audio
        ||
        pt_conn->b_scdb_video_detected)
    {
        avc_handler_nfy_connected(pt_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_tv_decoder_connected
 *
 * Description: This API is called when the TV decoder component is connected.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_tv_decoder_connected (AVC_HANDLER_CONN_T*  pt_conn)
{
    if ((! pt_conn->b_disconnecting) && (! pt_conn->b_tv_decoder_connected))
    {
        pt_conn->b_tv_decoder_connected = TRUE;

        if (pt_conn->b_avc_connected)
        {
            avc_handler_start_tv_decoder(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_connect_tv_decoder
 *
 * Description: This API completes a connection starting from the connection
 *              of the component (assuming everything else before has been
 *              done).
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_connect_tv_decoder (AVC_HANDLER_CONN_T*  pt_conn)
{
    INT32  i4_res;

    DBG_INFO((DBG_PREFIX"Connecting TV decoder (%d)...\n",
              pt_conn->h_ch_conn));

    i4_res = rm_connect(pt_conn->h_rm_tv_decoder,
                        RM_CONN_TYPE_HANDLE,
                        (VOID*) pt_conn->h_rm_avc,
                        sizeof(HANDLE_T),
                        (VOID*) pt_conn->h_ch_conn, /* tag */
                        & pt_conn->e_rm_connect_decoder_cond);

    if (i4_res == RMR_OK)
    {
        DBG_INFO((DBG_PREFIX"TV decoder connected synchronously (%d)\n",
                  pt_conn->h_ch_conn));

        avc_handler_tv_decoder_connected(pt_conn);
    }
    else if (i4_res == RMR_ASYNC_NFY)
    {
        /* nothing to do here */
    }
    else
    {
        if ((i4_res == RMR_NO_RIGHTS)
            &&
            pt_conn->t_connect_info.b_tv_decoder_no_rights_ok)
        {
            DBG_INFO((DBG_PREFIX"No rights ok (%d)\n", pt_conn->h_ch_conn));

            avc_handler_tv_decoder_connected(pt_conn);
        }
        else
        {
            DBG_ERROR((DBG_PREFIX"Could not connect TV decoder (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_res));

            avc_handler_disconnect_conn(pt_conn,
                                        AVC_DISC_COULD_NOT_CONNECT_TV_DECODER);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_avc_connected
 *
 * Description: This API is called when the AVC component is connected.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_avc_connected (AVC_HANDLER_CONN_T*  pt_conn)
{
    if ((! pt_conn->b_disconnecting) && (! pt_conn->b_avc_connected))
    {
        pt_conn->b_avc_connected = TRUE;

        if ((! pt_conn->b_decoder_for_audio_signal)
            ||
            (! pt_conn->b_tv_decoder_for_tv_signal))
        {
            SIZE_T  z_size;
            UINT32  ui4_signal_status;
            INT32   i4_res;

            z_size            = sizeof(ui4_signal_status);
            ui4_signal_status = AVC_STATE_NONE;

            i4_res = rm_get(pt_conn->h_rm_avc,
                            FALSE,
                            ANY_PORT_NUM,
                            AVC_GET_TYPE_SIGNAL_STATUS,
                            & ui4_signal_status,
                            & z_size,
                            NULL);

            if (i4_res == RMR_OK)
            {
                DBG_INFO((DBG_PREFIX
                          "AVC connected: signal status is 0x%x (%d)\n",
                          ui4_signal_status, pt_conn->h_ch_conn));

                avc_handler_avc_signal_status(pt_conn, ui4_signal_status);
            }
            else
            {
                DBG_ERROR((DBG_PREFIX
                           "Could not get AVC signal status (%d, res=%d)\n",
                           pt_conn->h_ch_conn, i4_res));
            }
        }

        /* add the condition to avoid monitor bypass notify connected before start tvd */
        /* such as AV source audio connect will notify connected here */
        if (!(pt_conn->b_tv_decoder_for_tv_signal
            ||
            pt_conn->b_tv_decoder_for_color_system))
        {        
            if (pt_conn->b_scdb_has_audio
                ||
                pt_conn->b_scdb_video_detected)
            {
                avc_handler_nfy_connected(pt_conn);
            }
        }

        if (pt_conn->b_tv_decoder_connected)
        {
            avc_handler_start_tv_decoder(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_connect_avc
 *
 * Description: This API completes a connection starting from the connection
 *              of the component (assuming everything else before has been
 *              done).
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_connect_avc (AVC_HANDLER_CONN_T*  pt_conn)
{
    INT32  i4_res;

    DBG_INFO((DBG_PREFIX"Connecting AVC (%d)\n", pt_conn->h_ch_conn));

    i4_res = rm_connect(pt_conn->h_rm_avc,
                        AVC_CONN_TYPE_ANY,
                        NULL, 0,
                        (VOID*) pt_conn->h_ch_conn, /* tag */
                        & pt_conn->e_rm_connect_avc_cond);

    if (((i4_res == RMR_OK) || (i4_res == RMR_ASYNC_NFY))
        &&
        ((! pt_conn->b_decoder_for_audio_signal)
         ||
         (! pt_conn->b_tv_decoder_for_tv_signal)))
    {
        AVC_NFY_INFO_T  t_nfy_info;

        t_nfy_info.pv_tag     = (VOID*) pt_conn->h_ch_conn;
        t_nfy_info.pf_avc_nfy = avc_handler_avc_nfy;

        rm_set(pt_conn->h_rm_avc,
               FALSE,
               ANY_PORT_NUM,
               AVC_SET_TYPE_NFY_FCT,
               (VOID*) & t_nfy_info,
               sizeof(AVC_NFY_INFO_T),
               NULL);
    }

    if (i4_res == RMR_OK)
    {
        DBG_INFO((DBG_PREFIX"AVC connected synchronously (%d)\n",
                  pt_conn->h_ch_conn));

        avc_handler_avc_connected(pt_conn);
    }
    else if (i4_res == RMR_ASYNC_NFY)
    {
        /* nothing to do here */
    }
    else
    {
        if ((i4_res == RMR_NO_RIGHTS)
            &&
            pt_conn->t_connect_info.b_input_no_rights_ok)
        {
            DBG_INFO((DBG_PREFIX"No rights ok (%d)\n", pt_conn->h_ch_conn));

            avc_handler_avc_connected(pt_conn);
        }
        else
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not connect AVC component (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_res));

            avc_handler_disconnect_conn(pt_conn,
                                    AVC_DISC_COULD_NOT_CONNECT_AVC);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_connect_conn
 *
 * Description: This API connects the AVC and the TV decoder (if needed).
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_connect_conn (AVC_HANDLER_CONN_T*  pt_conn)
{
    if (! pt_conn->b_disconnecting)
    {
        avc_handler_connect_avc(pt_conn);
    }

    if ((! pt_conn->b_disconnecting) && pt_conn->b_tv_decoder_opened)
    {
        avc_handler_connect_tv_decoder(pt_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_tv_decoder_opened
 *
 * Description: This API is called when the TV decoder is opened (in the
 *              context of the handler message loop). It will try to connect
 *              the components.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_tv_decoder_opened (AVC_HANDLER_CONN_T*  pt_conn)
{
    if (! pt_conn->b_tv_decoder_opened)
    {
        DBG_INFO((DBG_PREFIX"TV decoder component now opened (%d)\n",
                  pt_conn->h_ch_conn));

        pt_conn->b_tv_decoder_opened = TRUE;

        if (pt_conn->b_avc_opened)
        {
            avc_handler_connect_conn(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_comp_rm_decoder_nfy
 *
 * Description: This API is called by the Resource Manager to notify the
 *              handler of changes in the state or condition of the driver
 *              component.
 *
 * Inputs:  h_comp      Contains a handle to the decoder driver component.
 *          ui1_port    Contains the port number.
 *          e_nfy_cond  Contains the notify condition.
 *          pv_tag      Contains the tag value specified in rm_open_comp.
 *          ui4_data    Contains additional data (depends on e_nfy_cond).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_comp_rm_decoder_nfy (HANDLE_T   h_comp,
                                             UINT8      ui1_port,
                                             RM_COND_T  e_nfy_cond,
                                             VOID*      pv_tag,
                                             UINT32     ui4_data)
{
    HANDLE_T   h_ch_conn = (HANDLE_T) pv_tag;
    RM_COND_T  e_grp_cond;

    DBG_INFO((DBG_PREFIX"Decoder RM nfy 0x%08x (data %d) (connection %d)\n",
              e_nfy_cond, ui4_data, h_ch_conn));

    e_grp_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE, COND_GRP_OPERATION);

    if (e_grp_cond == RM_COND_IGNORE)
    {
        CRIT_STATE_T         t_state;
        AVC_HANDLER_CONN_T*  pt_conn;
        HANDLE_TYPE_T        e_type;
        INT32                i4_res;

        t_state = x_crit_start();

        i4_res = handle_get_type_obj(h_ch_conn, & e_type, (VOID**) & pt_conn);

        if ((i4_res == HR_OK) && (e_type == AVCHT_CONNECTION))
        {
            RM_COND_T  e_rm_connect_cond;
            RM_COND_T  e_rm_disconnect_cond;
            RM_COND_T  e_rm_cond;
            UINT32     ui4_code;

            e_rm_connect_cond    = pt_conn->e_rm_connect_decoder_cond;
            e_rm_disconnect_cond = pt_conn->e_rm_disconnect_decoder_cond;

            ui4_code = AVC_HANDLER_MSG_NONE;

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
                        ui4_code = AVC_HANDLER_MSG_DECODER_OPENED;
                        break;

                    case RM_COND_CLOSED:
                        ui4_code = AVC_HANDLER_MSG_DECODER_CLOSED;
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
                            ui4_code = AVC_HANDLER_MSG_DECODER_CONNECTED;
                        }
                        else
                        {
                            ui4_code = AVC_HANDLER_MSG_DECODER_DISCONNECTED;
                        }
                    }
                    break;

                    case RM_COND_DISCONNECTED:
                    {
                        ui4_code = AVC_HANDLER_MSG_DECODER_DISCONNECTED;
                    }
                    break;

                    case RM_COND_CONNECTING:
                    {
                        if (e_rm_disconnect_cond != RM_COND_IGNORE)
                        {
                            ui4_code = AVC_HANDLER_MSG_DECODER_DISCONNECTED;
                        }
                    }
                    break;

                    default:
                        break;
                }
            }

            x_crit_end(t_state);

            if (ui4_code != AVC_HANDLER_MSG_NONE)
            {
                cm_send_msg_retry(h_avc_handler_msgq,
                                  ui4_code,
                                  ui4_data, 0,
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
 * Name: avc_handler_open_tv_decoder
 *
 * Description: This API opens the TV decoder driver component.
 *
 * Inputs:  pt_conn    References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_open_tv_decoder (AVC_HANDLER_CONN_T*  pt_conn)
{
    CM_COMP_RM_SELECT_T  t_select;
    RM_COND_T            e_rm_cond;
    CHAR*                ps_tv_decoder_name;
    UINT32               ui4_reason;
    INT32                i4_res;

    DBG_INFO((DBG_PREFIX"Opening TV decoder (%d)...\n", pt_conn->h_ch_conn));

    ui4_reason = AVC_DISC_OTHER_REASON;

    cm_comp_rm_select_init(& t_select);

    t_select.b_select_if_conflict =
        ! pt_conn->t_connect_info.b_disc_if_comp_busy;
    t_select.b_select_if_lower_priority =
        pt_conn->t_connect_info.b_tv_decoder_no_rights_ok;
    t_select.b_would_conflict     = FALSE;

    ps_tv_decoder_name =
        (pt_conn->ui4_flags & AVC_HANDLER_TV_DECODER_NAME_MASK) ?
        pt_conn->t_connect_info.s_tv_decoder_name : NULL;

    i4_res = rm_open_comp(pt_conn->h_rm_avc,
                          OBJ_COMP_REL_SRC,
                          DRVT_TV_DEC,
                          ANY_COMP_ID,
                          ps_tv_decoder_name,
                          CTRL_TYPE_MASTER_SLAVE,
                          (VOID*) & t_select, /* pv_comp_sel_data */
                          cm_comp_rm_select, /* pf_comp_sel */
                          (VOID*) pt_conn->h_ch_conn, /* tag */
                          avc_handler_comp_rm_decoder_nfy,
                          & pt_conn->h_rm_tv_decoder,
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
                avc_handler_tv_decoder_opened(pt_conn);
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
            ui4_reason = t_select.b_would_conflict ?
                AVC_DISC_COMP_BUSY :
                AVC_DISC_NO_TV_DECODER_FOUND;
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
        avc_handler_disconnect_conn(pt_conn, ui4_reason);
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_get_info_from_avc
 *
 * Description: This API gets information from the AVC driver component (e.g.
 *              if the AVC can carry audio and video, if the TV decoder is
 *              needed to detect TV signal and color system).
 *
 * Inputs:  pt_conn    References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 avc_handler_get_info_from_avc (AVC_HANDLER_CONN_T*  pt_conn)
{
    DRV_TYPE_T  e_type;
    UINT32      ui4_comp_flags;
    INT32       i4_res;

    i4_res = rm_get_comp_info_from_handle(pt_conn->h_rm_avc,
                                          & e_type,
                                          NULL,
                                          NULL,
                                          NULL,
                                          & ui4_comp_flags);

    i4_res = (i4_res == RMR_OK) ? CMR_OK : CMR_FAIL;

    if (i4_res == CMR_OK)
    {
        pt_conn->b_tv_decoder_for_tv_signal =
            ((ui4_comp_flags & AVC_FLAG_DEC_REQ_FOR_TV_SIG_DETECTION) != 0);

        pt_conn->b_tv_decoder_for_color_system =
            ((ui4_comp_flags &
              AVC_FLAG_DEC_REQ_FOR_COLOR_SYS_DETECTION) != 0);

        pt_conn->b_decoder_for_audio_signal =
            ((ui4_comp_flags & AVC_FLAG_DEC_REQ_FOR_AUD_SIG_DETECTION) != 0);
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_avc_opened
 *
 * Description: This API is called when a component is opened (in the context
 *              of the handler message loop). It will try to connect the
 *              components.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_avc_opened (AVC_HANDLER_CONN_T*  pt_conn)
{
    if (! pt_conn->b_avc_opened)
    {
        INT32  i4_res;

        DBG_INFO((DBG_PREFIX"AVC component now opened (%d)\n",
                  pt_conn->h_ch_conn));

        pt_conn->b_avc_opened = TRUE;

        i4_res = avc_handler_get_info_from_avc(pt_conn);

        if (i4_res == CMR_OK)
        {
            DBG_INFO((DBG_PREFIX"TV decoder to detect signal? %s (%d)\n",
                      pt_conn->b_tv_decoder_for_tv_signal ? "Yes" : "No",
                      pt_conn->h_ch_conn));

            DBG_INFO((DBG_PREFIX"TV decoder to detect color system? %s (%d)\n",
                      pt_conn->b_tv_decoder_for_color_system ? "Yes" : "No",
                      pt_conn->h_ch_conn));

            if (pt_conn->b_tv_decoder_opened
                ||
                ! (pt_conn->b_tv_decoder_for_tv_signal
                   ||
                   pt_conn->b_tv_decoder_for_color_system))
            {
                avc_handler_connect_conn(pt_conn);
            }
        }
        else
        {
            DBG_ERROR((DBG_PREFIX"Could not get information from AVC\n"));

            avc_handler_disconnect_conn(pt_conn, AVC_DISC_OTHER_REASON);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_comp_rm_avc_nfy
 *
 * Description: This API is called by the Resource Manager to notify the
 *              handler of changes in the state or condition of the driver
 *              component.
 *
 * Inputs:  h_comp      Contains a handle to the AVC driver component.
 *          ui1_port    Contains the port number.
 *          e_nfy_cond  Contains the notify condition.
 *          pv_tag      Contains the tag value specified in rm_open_comp.
 *          ui4_data    Contains additional data (depends on e_nfy_cond).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_comp_rm_avc_nfy (HANDLE_T   h_comp,
                                         UINT8      ui1_port,
                                         RM_COND_T  e_nfy_cond,
                                         VOID*      pv_tag,
                                         UINT32     ui4_data)
{
    HANDLE_T   h_ch_conn = (HANDLE_T) pv_tag;
    RM_COND_T  e_grp_cond;

    DBG_INFO((DBG_PREFIX"AVC RM nfy 0x%08x (data %d) (connection %d)\n",
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
        CRIT_STATE_T         t_state;
        AVC_HANDLER_CONN_T*  pt_conn;
        HANDLE_TYPE_T        e_type;
        INT32                i4_res;

        t_state = x_crit_start();

        i4_res = handle_get_type_obj(h_ch_conn, & e_type, (VOID**) & pt_conn);

        if ((i4_res == HR_OK) && (e_type == AVCHT_CONNECTION))
        {
            RM_COND_T  e_rm_connect_cond;
            RM_COND_T  e_rm_disconnect_cond;
            RM_COND_T  e_rm_cond;
            UINT32     ui4_code;

            e_rm_connect_cond    = pt_conn->e_rm_connect_avc_cond;
            e_rm_disconnect_cond = pt_conn->e_rm_disconnect_avc_cond;

            ui4_code = AVC_HANDLER_MSG_NONE;

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
                        ui4_code = AVC_HANDLER_MSG_AVC_OPENED;
                        break;

                    case RM_COND_CLOSED:
                        ui4_code = AVC_HANDLER_MSG_AVC_CLOSED;
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
                            ui4_code = AVC_HANDLER_MSG_AVC_CONNECTED;
                        }
                        else
                        {
                            ui4_code = AVC_HANDLER_MSG_AVC_DISCONNECTED;
                        }
                    }
                    break;

                    case RM_COND_DISCONNECTED:
                    {
                        ui4_code = AVC_HANDLER_MSG_AVC_DISCONNECTED;
                    }
                    break;

                    case RM_COND_CONNECTING:
                    {
                        if (e_rm_disconnect_cond != RM_COND_IGNORE)
                        {
                            ui4_code = AVC_HANDLER_MSG_AVC_DISCONNECTED;
                        }
                    }
                    break;

                    default:
                        break;
                }
            }

            x_crit_end(t_state);

            if (ui4_code != AVC_HANDLER_MSG_NONE)
            {
                cm_send_msg_retry(h_avc_handler_msgq,
                                  ui4_code,
                                  ui4_data, 0,
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
 * Name: avc_handler_comp_rm_combi_nfy
 *
 * Description: This API is called by the Resource Manager to notify the
 *              handler of changes in the state or condition of the driver
 *              component. The implementation is empty on purpose.
 *
 * Inputs:  h_comp      Contains a handle to the COMBI driver component.
 *          ui1_port    Contains the port number.
 *          e_nfy_cond  Contains the notify condition.
 *          pv_tag      Contains the tag value specified in rm_open_comp.
 *          ui4_data    Contains additional data (depends on e_nfy_cond).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_comp_rm_combi_nfy (HANDLE_T   h_comp,
                                           UINT8      ui1_port,
                                           RM_COND_T  e_nfy_cond,
                                           VOID*      pv_tag,
                                           UINT32     ui4_data)
{
    /* nothing */
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_get_info_from_combi
 *
 * Description: This API retrieves the type and id of the actual component in
 *              the COMBI component (using the index provided in the connection
 *              commands).
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 avc_handler_get_info_from_combi (AVC_HANDLER_CONN_T*  pt_conn)
{
    HANDLE_T  h_rm_pipe;
    HANDLE_T  h_rm_combi;
    INT32     i4_res;

    h_rm_pipe = cm_pipe_get_rm_pipe(pt_conn->h_cm_pipe);

    i4_res = rm_open_comp(h_rm_pipe,
                          OBJ_COMP_REL_IGNORE,
                          DRVT_AVC_COMBI,
                          ANY_COMP_ID,
                          pt_conn->s_src_name,
                          CTRL_TYPE_MONITOR,
                          NULL,
                          NULL,
                          NULL,
                          avc_handler_comp_rm_combi_nfy,
                          & h_rm_combi,
                          NULL);

    if (i4_res == RMR_OK)
    {
        AVC_INDEX_COMP_ID_T  t_index_comp_id;
        SIZE_T               z_size;

        t_index_comp_id.ui2_index =
            pt_conn->t_connect_info.ui2_combi_index;

        z_size = sizeof(AVC_INDEX_COMP_ID_T);

        i4_res = rm_get(h_rm_combi,
                        TRUE,
                        ANY_PORT_NUM,
                        AVC_GET_TYPE_COMP_ID,
                        (VOID*) & t_index_comp_id,
                        & z_size,
                        NULL);

        if (i4_res == RMR_OK)
        {
            DEVICE_TYPE_T  e_device_type;

            i4_res = CMR_OK;

            switch (t_index_comp_id.t_comp_id.e_type)
            {
                case DRVT_AVC_COMP_VIDEO:
                    e_device_type = DEV_AVC_COMP_VIDEO;
                    break;
                case DRVT_AVC_S_VIDEO:
                    e_device_type = DEV_AVC_S_VIDEO;
                    break;
                case DRVT_AVC_Y_PB_PR:
                    e_device_type = DEV_AVC_Y_PB_PR;
                    break;
                case DRVT_AVC_VGA:
                    e_device_type = DEV_AVC_VGA;
                    break;
                case DRVT_AVC_SCART:
                    e_device_type = DEV_AVC_SCART;
                    break;
                case DRVT_AVC_DVI:
                    e_device_type = DEV_AVC_DVI;
                    break;
                case DRVT_AVC_HDMI:
                    e_device_type = DEV_AVC_HDMI;
                    break;
                case DRVT_AVC_AUDIO_INP:
                    e_device_type = DEV_AVC_AUDIO_INP;
                    break;
                case DRVT_AVC_SPDIF:
                    e_device_type = DEV_AVC_SPDIF;
                    break;
                case DRVT_AVC_COMBI:
                default:
                    e_device_type = DEV_AVC_RESERVED;
                    i4_res = CMR_FAIL;
            }

            pt_conn->t_connect_info.e_device_type = e_device_type;

            pt_conn->ui2_comp_id = t_index_comp_id.t_comp_id.ui2_id;
        }
        else
        {
            i4_res = CMR_FAIL;
        }

        rm_close(h_rm_combi, FALSE, NULL);
    }
    else
    {
        i4_res = CMR_FAIL;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: avc_scart_comp_rm_select
 *
 * Description: This API is called when a SCART component is being opened. This
 *              API will find a component to open (if any). It will check the
 *              SCART component's flags before it calls the connection manager
 *              generic select function.
 *
 * Inputs:  e_type            Contains the driver type (DRVT_AVC_SCART).
 *          ui2_id            Contains the driver component id.
 *          pv_comp_sel_data  References private data passed in rm_open_comp.
 *          pv_comp_data      References the component's private data.
 *          z_comp_data_len   Contains the length of *pv_comp_data.
 *          ui2_num_active    Contains the number of handles to this component
 *                            in OPENED state.
 *          ui2_num_wait      Contains the number of handles to this component
 *                            in OPENING state.
 *          e_ctrl            Contains the control type of the component.
 *          b_would_conflict  Specifies if a conflict could occur.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static SELECT_T avc_scart_comp_rm_select (DRV_TYPE_T   e_type,
                                          UINT16       ui2_id,
                                          VOID*        pv_comp_sel_data,
                                          const VOID*  pv_comp_data,
                                          SIZE_T       z_comp_data_len,
                                          UINT16       ui2_num_active,
                                          UINT16       ui2_num_wait,
                                          CTRL_TYPE_T  e_ctrl,
                                          BOOL         b_would_conflict)
{
    SELECT_T  e_select;
    INT32     i4_res;
    UINT32    ui4_mask;
    UINT32    ui4_comp_flags;
    UINT8     ui1_num_inp_ports;
    UINT8     ui1_num_out_ports;
    BOOL      b_ignore;

    ui4_mask = AVC_FLAG_SCART_SOURCE | AVC_FLAG_SCART_SINK;

    i4_res = rm_get_comp_info_by_id(e_type,
                                    ui2_id,
                                    & ui1_num_inp_ports,
                                    & ui1_num_out_ports,
                                    & ui4_comp_flags);

    b_ignore =
        (i4_res != RMR_OK)
        ||
        ((ui4_comp_flags & ui4_mask) != AVC_FLAG_SCART_SOURCE);

    if (! b_ignore)
    {
        e_select = cm_comp_rm_select(e_type,
                                     ui2_id,
                                     pv_comp_sel_data,
                                     pv_comp_data,
                                     z_comp_data_len,
                                     ui2_num_active,
                                     ui2_num_wait,
                                     e_ctrl,
                                     b_would_conflict);
    }
    else
    {
        DBG_INFO((DBG_PREFIX
                  "SCART component not a source (type=%d, id=%d)\n",
                  e_type, ui2_id));

        e_select = SEL_NEXT;
    }

    return e_select;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_open_avc
 *
 * Description: This API opens the AVC driver component.
 *
 * Inputs:  pt_conn    References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_open_avc (AVC_HANDLER_CONN_T*  pt_conn)
{
    CM_COMP_RM_SELECT_T  t_select;
    rm_comp_sel_fct      pf_select;
    DRV_TYPE_T           e_drv_type;
    UINT32               ui4_reason;
    INT32                i4_res;

    DBG_INFO((DBG_PREFIX"Opening AVC (%s) on connection (%d)...\n",
              avc_device_type_str(pt_conn->t_connect_info.e_device_type),
              pt_conn->h_ch_conn));

    ui4_reason = AVC_DISC_OTHER_REASON;

    cm_comp_rm_select_init(& t_select);

    t_select.b_select_if_conflict =
        ! pt_conn->t_connect_info.b_disc_if_comp_busy;
    t_select.b_select_if_lower_priority =
        pt_conn->t_connect_info.b_input_no_rights_ok;
    t_select.b_would_conflict = FALSE;

    pf_select = cm_comp_rm_select;

    i4_res = CMR_OK;

    switch (pt_conn->t_connect_info.e_device_type)
    {
        case DEV_AVC_COMP_VIDEO:
            e_drv_type = DRVT_AVC_COMP_VIDEO;
            break;
        case DEV_AVC_S_VIDEO:
            e_drv_type = DRVT_AVC_S_VIDEO;
            break;
        case DEV_AVC_Y_PB_PR:
            e_drv_type = DRVT_AVC_Y_PB_PR;
            break;
        case DEV_AVC_VGA:
            e_drv_type = DRVT_AVC_VGA;
            break;
        case DEV_AVC_SCART:
            pf_select  = avc_scart_comp_rm_select;
            e_drv_type = DRVT_AVC_SCART;
            break;
        case DEV_AVC_DVI:
            e_drv_type = DRVT_AVC_DVI;
            break;
        case DEV_AVC_HDMI:
            e_drv_type = DRVT_AVC_HDMI;
            break;
        case DEV_AVC_AUDIO_INP:
            e_drv_type = DRVT_AVC_AUDIO_INP;
            break;
        case DEV_AVC_SPDIF:
            e_drv_type = DRVT_AVC_SPDIF;
            break;
        case DEV_AVC_COMBI:
        default:
            i4_res = CMR_FAIL;
            break;
    }

    if (i4_res == CMR_OK)
    {
        CHAR*      ps_avc_name;
        RM_COND_T  e_rm_cond;
        HANDLE_T   h_rm_pipe;

        h_rm_pipe = cm_pipe_get_rm_pipe(pt_conn->h_cm_pipe);

        /*
          if COMBI then use NULL as the name as the comp id is known but name
          is not.
        */
        ps_avc_name = pt_conn->b_combi ? NULL : pt_conn->s_src_name;

        i4_res = rm_open_comp(h_rm_pipe,
                              OBJ_COMP_REL_IGNORE,
                              e_drv_type,
                              pt_conn->ui2_comp_id,
                              ps_avc_name,
                              CTRL_TYPE_MASTER_SLAVE,
                              (VOID*) & t_select, /* pv_comp_sel_data */
                              pf_select, /* pf_comp_sel */
                              (VOID*) pt_conn->h_ch_conn, /* tag */
                              avc_handler_comp_rm_avc_nfy,
                              & pt_conn->h_rm_avc,
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
                    avc_handler_avc_opened(pt_conn);
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
                ui4_reason = t_select.b_would_conflict ?
                    AVC_DISC_COMP_BUSY :
                    AVC_DISC_NO_AVC_FOUND;
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
    }

    if (i4_res != CMR_OK)
    {
        avc_handler_disconnect_conn(pt_conn, ui4_reason);
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_open_conn
 *
 * Description: This API starts the actual connection process by opening the 2
 *              components (AVC and TV decoder).
 *
 * Inputs:  pt_conn    References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_open_conn (AVC_HANDLER_CONN_T*  pt_conn)
{
    if (! pt_conn->b_disconnecting)
    {
        avc_handler_open_avc(pt_conn);
    }

    if ((! pt_conn->b_disconnecting)
        &&
        (pt_conn->b_tv_decoder_for_tv_signal
         ||
         pt_conn->b_tv_decoder_for_color_system))
    {
        avc_handler_open_tv_decoder(pt_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_conn_free_handle
 *
 * Description: This API is called when an AVC connection handle is freed.
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
static BOOL avc_handler_conn_free_handle (HANDLE_T       h_ch_conn,
                                          HANDLE_TYPE_T  e_type,
                                          VOID*          pv_obj,
                                          VOID*          pv_tag,
                                          BOOL           b_req_handle)
{
    BOOL  b_res;

    DBG_INFO((DBG_PREFIX"Deleting connection handle %d.\n",
              h_ch_conn));

    if (! b_req_handle)
    {
        AVC_HANDLER_CONN_T*  pt_conn;
        INT32                i4_res;

        pt_conn = (AVC_HANDLER_CONN_T*) pv_obj;

        /* delete SCDB */
        if (pt_conn->h_scdb != NULL_HANDLE)
        {
            DBG_INFO((DBG_PREFIX"Deleting SCDB %d\n", pt_conn->h_scdb));

            i4_res = scdb_delete(pt_conn->h_scdb);

            if (i4_res != SCDBR_OK)
            {
                DBG_ERROR((DBG_PREFIX"Deleting SCDB %d failed",
                           pt_conn->h_scdb));
            }
        }
        else
        {
            DBG_INFO((DBG_PREFIX"null SCDB, nothing to free\n"));
        }

        /* delete timer */
        if (pt_conn->h_timer != NULL_HANDLE)
        {
            DBG_INFO((DBG_PREFIX"Deleting timer %d\n", pt_conn->h_timer));

            i4_res = x_timer_delete(pt_conn->h_timer);

            if (i4_res != OSR_OK)
            {
                DBG_ERROR((DBG_PREFIX"Deleting timer %d failed",
                           pt_conn->h_timer));
            }
        }

        if (pt_conn->pui4_nb_connections != NULL)
        {
            (*pt_conn->pui4_nb_connections)--;

            DBG_INFO((DBG_PREFIX"Handler now has %d connection(s).\n",
                      (*pt_conn->pui4_nb_connections)));
        }

        pt_conn->ui4_magic = ~ AVC_HANDLER_CONN_MAGIC;

        x_mem_free(pv_obj);

        b_res = TRUE;
    }
    else
    {
        DBG_INFO((DBG_PREFIX"Not allowed to delete handle\n"));

        b_res = FALSE;
    }

    return b_res;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_disconnect
 *
 * Description: This API is called by the Connection Manager to perform a
 *              disconnection.
 *
 * Inputs:  h_ch_conn  Contains a handle to a AVC handler's connection.
 *
 * Outputs: pe_cond  Contains the connection condition (DISCONNECTING, etc).
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  Disconnection failed.
 ----------------------------------------------------------------------------*/
static INT32 avc_handler_disconnect (HANDLE_T    h_ch_conn,
                                     CM_COND_T*  pe_cond)
{
    INT32  i4_res;

    i4_res = cm_send_msg_retry(h_avc_handler_msgq,
                               AVC_HANDLER_MSG_DISCONNECT_REQUEST,
                               AVC_DISC_AS_REQUESTED, 0,
                               h_ch_conn);

    if (i4_res == CMR_OK)
    {
        *pe_cond = CM_COND_DISCONNECTING;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_connect
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
static INT32 avc_handler_connect (HANDLE_T             h_cm_conn,
                                  HANDLE_T*            ph_ch_conn,
                                  HANDLE_T             h_parent_ch_conn,
                                  const CM_COMMAND_T*  pt_commands,
                                  CM_COND_T*           pe_cond)
{
    AVC_HANDLER_CONNECT_INFO_T  t_connect_info;
    UINT32                      ui4_flags;
    INT32                       i4_res;

    DBG_INFO((DBG_PREFIX"Connecting... (CM connection %d)\n",
              h_cm_conn));

    x_memset(& t_connect_info, 0, sizeof(t_connect_info));

    t_connect_info.t_time_outs = t_time_outs;
    t_connect_info.b_disc_if_comp_busy = TRUE;
    t_connect_info.b_disc_if_tv_signal_lost = TRUE;

    ui4_flags = 0;

    if (pt_commands != NULL)
    {
        i4_res = avc_handler_parse_commands(& t_connect_info,
                                            & ui4_flags,
                                            pt_commands);
    }
    else
    {
        i4_res = CMR_INV_ARG;
    }

    if (i4_res == CMR_OK)
    {
        HANDLE_T               h_ch_conn;
        AVC_HANDLER_CONN_T*    pt_conn = NULL;

        i4_res = handle_alloc_and_obj(AVCHT_CONNECTION,
                                      NULL,
                                      sizeof(AVC_HANDLER_CONN_T),
                                      avc_handler_conn_free_handle,
                                      & h_ch_conn,
                                      (VOID**) & pt_conn);

        if ((i4_res == HR_OK) && (pt_conn != NULL))
        {
            DBG_INFO((DBG_PREFIX"Created connection %d (ptr=%x)\n",
                      h_ch_conn, pt_conn));

            cm_connection_get_src_name(h_cm_conn,
                                       pt_conn->s_src_name);

            pt_conn->t_connect_info = t_connect_info;

            pt_conn->ui4_disconnection_reason = AVC_DISC_OTHER_REASON;

            pt_conn->h_cm_pipe = cm_connection_get_pipe(h_cm_conn);
            pt_conn->h_cm_conn = h_cm_conn;
            pt_conn->h_ch_conn = h_ch_conn;

            pt_conn->ui4_flags = ui4_flags;

            pt_conn->ui4_magic   = AVC_HANDLER_CONN_MAGIC;
            pt_conn->ui2_comp_id = ANY_COMP_ID;

            i4_res = avc_handler_create_scdb(pt_conn);

            if (i4_res == CMR_OK)
            {
                avc_handler_create_timer(pt_conn);
            }

            if ((i4_res == CMR_OK)
                &&
                (pt_conn->t_connect_info.e_device_type == DEV_AVC_COMBI))
            {
                i4_res = avc_handler_get_info_from_combi(pt_conn);

                pt_conn->b_combi = TRUE;
            }

            if (i4_res == CMR_OK)
            {
                switch (pt_conn->t_connect_info.e_device_type)
                {
                    case DEV_AVC_COMP_VIDEO:
                    case DEV_AVC_S_VIDEO:
                    case DEV_AVC_Y_PB_PR:
                    case DEV_AVC_VGA:
                    case DEV_AVC_SCART:
                    case DEV_AVC_DVI:
                    case DEV_AVC_HDMI:
                    {
                        avc_handler_add_video_to_scdb(pt_conn);
                    }
                    break;

                    default:
                        break;
                }
            }

            if (i4_res == CMR_OK)
            {
                *pe_cond = CM_COND_CONNECTING;

                i4_res = cm_send_msg_retry(h_avc_handler_msgq,
                                           AVC_HANDLER_MSG_CONNECT_REQUEST,
                                           0, 0,
                                           h_ch_conn);

                if (i4_res == CMR_OK)
                {
                    *ph_ch_conn = h_ch_conn;
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
 * Name: avc_handler_get
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
static INT32 avc_handler_get (HANDLE_T        h_ch_conn,
                              CM_CTRL_TYPE_T  e_ctrl_type,
                              VOID*           pv_ctrl_data)
{
    INT32  i4_res;

    i4_res = CMR_OK;

    switch (e_ctrl_type)
    {
        case CM_CTRL_RM_SRC_COMP:
        {
            AVC_HANDLER_CONN_T*  pt_conn;
            CRIT_STATE_T         t_state;

            t_state = x_crit_start();

            if (handle_get_obj(h_ch_conn, (VOID**) & pt_conn) == HR_OK)
            {
                VALIDATE_CONN(pt_conn);

                ((CM_CTRL_RM_SRC_COMP_T*) pv_ctrl_data)->h_rm_comp =
                    pt_conn->h_rm_avc;
            }
            else
            {
                i4_res = CMR_FAIL;
            }

            x_crit_end(t_state);
        }
        break;

        case AVC_HANDLER_CTRL_GET_SCART_PIN_8_STATUS:
        case AVC_HANDLER_CTRL_GET_SCART_PIN_16_STATUS:
        case AVC_HANDLER_CTRL_GET_SCART_INP_CAP:
        {
            AVC_HANDLER_CONN_T*  pt_conn;
            HANDLE_T             h_rm_scart = NULL_HANDLE;

            if (pv_ctrl_data != NULL)
            {
                CRIT_STATE_T  t_state;

                t_state = x_crit_start();

                if (handle_get_obj(h_ch_conn, (VOID**) & pt_conn) == HR_OK)
                {
                    h_rm_scart = pt_conn->h_rm_avc;
                }
                else
                {
                    i4_res = CMR_FAIL;
                }

                x_crit_end(t_state);
            }
            else
            {
                i4_res = CMR_INV_ARG;
            }

            if (i4_res == CMR_OK)
            {
                switch (e_ctrl_type)
                {
                    case AVC_HANDLER_CTRL_GET_SCART_PIN_8_STATUS:
                    {
                        AVC_HANDLER_CTRL_SCART_PIN_8_STATUS_T*  pe_pin_8;
                        AVC_SCART_PIN_8_T                       e_pin_8;
                        SIZE_T                                  z_size;

                        pe_pin_8 = (AVC_HANDLER_CTRL_SCART_PIN_8_STATUS_T*)
                            pv_ctrl_data;

                        z_size = sizeof(AVC_SCART_PIN_8_T);

                        i4_res = rm_get(h_rm_scart,
                                        TRUE,
                                        ANY_PORT_NUM,
                                        AVC_GET_TYPE_SCART_PIN_8_STATUS,
                                        (VOID*) & e_pin_8,
                                        & z_size,
                                        NULL);

                        if (i4_res == RMR_OK)
                        {
                            switch (e_pin_8)
                            {
                                case AVC_SCART_PIN_8_NO_SIGNAL:
                                    *pe_pin_8 =
                                        AVC_HANDLER_SCART_PIN_8_NO_SIGNAL;
                                    break;
                                case AVC_SCART_PIN_8_ASPECT_16_9:
                                    *pe_pin_8 =
                                        AVC_HANDLER_SCART_PIN_8_ASPECT_16_9;
                                    break;
                                case AVC_SCART_PIN_8_ASPECT_4_3:
                                    *pe_pin_8 =
                                        AVC_HANDLER_SCART_PIN_8_ASPECT_4_3;
                                    break;
                                default:
                                    *pe_pin_8 =
                                        AVC_HANDLER_SCART_PIN_8_UNKNOWN;
                                    break;
                            }
                        }
                    }
                    break;

                    case AVC_HANDLER_CTRL_GET_SCART_PIN_16_STATUS:
                    {
                        AVC_HANDLER_CTRL_SCART_PIN_16_STATUS_T*  pe_pin_16;
                        AVC_SCART_PIN_16_T                       e_pin_16;
                        SIZE_T                                   z_size;

                        pe_pin_16 = (AVC_HANDLER_CTRL_SCART_PIN_16_STATUS_T*)
                            pv_ctrl_data;

                        z_size = sizeof(AVC_SCART_PIN_16_T);

                        i4_res = rm_get(h_rm_scart,
                                        TRUE,
                                        ANY_PORT_NUM,
                                        AVC_GET_TYPE_SCART_PIN_16_STATUS,
                                        (VOID*) & e_pin_16,
                                        & z_size,
                                        NULL);

                        if (i4_res == RMR_OK)
                        {
                            switch (e_pin_16)
                            {
                                case AVC_SCART_PIN_16_COMPOSITE:
                                    *pe_pin_16 =
                                        AVC_HANDLER_SCART_PIN_16_COMPOSITE;
                                    break;
                                case AVC_SCART_PIN_16_RGB:
                                    *pe_pin_16 =
                                        AVC_HANDLER_SCART_PIN_16_RGB;
                                    break;
                                case AVC_SCART_PIN_16_TOGGLE:
                                    *pe_pin_16 =
                                        AVC_HANDLER_SCART_PIN_16_TOGGLE;
                                    break;
                                default:
                                    *pe_pin_16 =
                                        AVC_HANDLER_SCART_PIN_16_UNKNOWN;
                                    break;
                            }
                        }
                    }
                    break;

                    case AVC_HANDLER_CTRL_GET_SCART_INP_CAP:
                    default:
                    {
                        AVC_HANDLER_CTRL_SCART_INP_CAP_T*  pt_inp_cap;
                        UINT32                             ui4_inp_cap;
                        SIZE_T                             z_size;

                        pt_inp_cap = (AVC_HANDLER_CTRL_SCART_INP_CAP_T*)
                            pv_ctrl_data;

                        z_size = sizeof(UINT32);

                        i4_res = rm_get(h_rm_scart,
                                        TRUE,
                                        ANY_PORT_NUM,
                                        AVC_GET_TYPE_SCART_INP_CAP,
                                        (VOID*) & ui4_inp_cap,
                                        & z_size,
                                        NULL);

                        if (i4_res == RMR_OK)
                        {
                            pt_inp_cap->ui4_flags = 0;

                            if (ui4_inp_cap & AVC_SCART_INP_CAP_S_VIDEO)
                            {
                                pt_inp_cap->ui4_flags |=
                                    AVC_HANDLER_SCART_INP_CAP_S_VIDEO;
                            }

                            if (ui4_inp_cap & AVC_SCART_INP_CAP_COMPOSITE)
                            {
                                pt_inp_cap->ui4_flags |=
                                    AVC_HANDLER_SCART_INP_CAP_COMPOSITE;
                            }

                            if (ui4_inp_cap & AVC_SCART_INP_CAP_RGB)
                            {
                                pt_inp_cap->ui4_flags |=
                                    AVC_HANDLER_SCART_INP_CAP_RGB;
                            }

                            if (ui4_inp_cap & AVC_SCART_INP_CAP_COMPONENT)
                            {
                                pt_inp_cap->ui4_flags |=
                                    AVC_HANDLER_SCART_INP_CAP_COMPONENT;
                            }
                        }
                    }
                    break;
                }

                i4_res = (i4_res == RMR_OK) ? CMR_OK : CMR_FAIL;
            }
        }
        break;

        case AVC_HANDLER_CTRL_GET_SCART_INP_MODE:
        {
            AVC_HANDLER_CONN_T*  pt_conn;
            HANDLE_T             h_rm_tv_decoder = NULL_HANDLE;

            if (pv_ctrl_data != NULL)
            {
                CRIT_STATE_T  t_state;

                t_state = x_crit_start();

                if (handle_get_obj(h_ch_conn, (VOID**) & pt_conn) == HR_OK)
                {
                    h_rm_tv_decoder = pt_conn->h_rm_tv_decoder;
                }
                else
                {
                    i4_res = CMR_FAIL;
                }

                x_crit_end(t_state);
            }
            else
            {
                i4_res = CMR_INV_ARG;
            }

            if (i4_res == CMR_OK)
            {
                AVC_HANDLER_SCART_MODE_T*  pe_scart_mode;
                TV_DEC_SCART_MODE_T        e_tv_dec_scart_mode;
                SIZE_T                     z_size;

                pe_scart_mode = (AVC_HANDLER_SCART_MODE_T*) pv_ctrl_data;

                z_size = sizeof(TV_DEC_SCART_MODE_T);

                i4_res = rm_get(h_rm_tv_decoder,
                                FALSE,
                                ANY_PORT_NUM,
                                TV_DEC_GET_TYPE_SCART_INP_MODE,
                                (VOID*) & e_tv_dec_scart_mode,
                                & z_size,
                                NULL);

                if (i4_res == RMR_OK)
                {
                    switch (e_tv_dec_scart_mode)
                    {
                        case TV_DEC_SCART_MODE_S_VIDEO:
                            *pe_scart_mode = AVC_HANDLER_SCART_MODE_S_VIDEO;
                            break;
                        case TV_DEC_SCART_MODE_COMPOSITE:
                            *pe_scart_mode = AVC_HANDLER_SCART_MODE_COMPOSITE;
                            break;
                        case TV_DEC_SCART_MODE_RGB:
                            *pe_scart_mode = AVC_HANDLER_SCART_MODE_RGB;
                            break;
                        case TV_DEC_SCART_MODE_MIX:
                            *pe_scart_mode = AVC_HANDLER_SCART_MODE_MIX;
                            break;
                        case TV_DEC_SCART_MODE_AUTO:
                            *pe_scart_mode = AVC_HANDLER_SCART_MODE_AUTO;
                            break;
                        case TV_DEC_SCART_MODE_COMPONENT:
                            *pe_scart_mode = AVC_HANDLER_SCART_MODE_COMPONENT;
                            break;
                        case TV_DEC_SCART_MODE_UNKNOWN:
                        default:
                            *pe_scart_mode = AVC_HANDLER_SCART_MODE_UNKNOWN;
                            break;
                    }
                }

                i4_res = (i4_res == RMR_OK) ? CMR_OK : CMR_FAIL;
            }
        }
        break;

        case AVC_HANDLER_CTRL_GET_TV_DEC_COLOR_SYS:
        {
            AVC_HANDLER_CONN_T*  pt_conn;
            HANDLE_T             h_rm_tv_decoder = NULL_HANDLE;

            if (pv_ctrl_data != NULL)
            {
                CRIT_STATE_T  t_state;

                t_state = x_crit_start();

                if (handle_get_obj(h_ch_conn, (VOID**) & pt_conn) == HR_OK)
                {
                    h_rm_tv_decoder = pt_conn->h_rm_tv_decoder;
                }
                else
                {
                    i4_res = CMR_FAIL;
                }

                x_crit_end(t_state);
            }
            else
            {
                i4_res = CMR_INV_ARG;
            }

            if (i4_res == CMR_OK)
            {
                COLOR_SYS_T*        pe_color_sys;
                TV_DEC_COLOR_SYS_T  e_tv_dec_color_sys;
                SIZE_T              z_size;

                pe_color_sys = (COLOR_SYS_T*) pv_ctrl_data;

                z_size = sizeof(TV_DEC_COLOR_SYS_T);

                i4_res = rm_get(h_rm_tv_decoder,
                                FALSE,
                                ANY_PORT_NUM,
                                TV_DEC_GET_TYPE_COLOR_SYS,
                                (VOID*) & e_tv_dec_color_sys,
                                & z_size,
                                NULL);

                if (i4_res == RMR_OK)
                {
                    *pe_color_sys =
                        cm_drv_color_sys_to_color_sys(e_tv_dec_color_sys);
                }

                i4_res = (i4_res == RMR_OK) ? CMR_OK : CMR_FAIL;
            }
        }
        break;

        case AVC_HANDLER_CTRL_GET_SIGNAL_STRENGTH:
        {
            AVC_HANDLER_CONN_T*  pt_conn;
            HANDLE_T             h_rm_tv_decoder = NULL_HANDLE;

            if (pv_ctrl_data != NULL)
            {
                CRIT_STATE_T  t_state;

                t_state = x_crit_start();

                if (handle_get_obj(h_ch_conn, (VOID**) & pt_conn) == HR_OK)
                {
                    h_rm_tv_decoder = pt_conn->h_rm_tv_decoder;
                }
                else
                {
                    i4_res = CMR_FAIL;
                }

                x_crit_end(t_state);
            }
            else
            {
                i4_res = CMR_INV_ARG;
            }

            if (i4_res == CMR_OK)
            {
                SIZE_T        z_size;

                z_size = sizeof(UINT8);

                i4_res = rm_get(h_rm_tv_decoder,
                                FALSE,
                                ANY_PORT_NUM,
                                TV_DEC_GET_TYPE_SIGNAL_STRENGTH,
                                pv_ctrl_data,
                                &z_size,
                                NULL);

                 i4_res = (i4_res == RMR_OK) ? CMR_OK : CMR_FAIL;
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
 * Name: avc_handler_set
 *
 * Description: This API sets information about a connection.
 *
 * Inputs:  h_conn        Contains a handle to the handler's connection.
 *          e_ctrl_type   Contains the control code.
 *          pv_ctrl_data  References information.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  "get" operation failed.
 ----------------------------------------------------------------------------*/
static INT32 avc_handler_set (HANDLE_T        h_ch_conn,
                              CM_CTRL_TYPE_T  e_ctrl_type,
                              VOID*           pv_ctrl_data)
{
    INT32  i4_res;

    i4_res = CMR_OK;

    switch (e_ctrl_type)
    {
        case AVC_HANDLER_CTRL_SET_SCART_INP_MODE:
        {
            AVC_HANDLER_CONN_T*  pt_conn;
            HANDLE_T             h_rm_tv_decoder = NULL_HANDLE;

            if (pv_ctrl_data != NULL)
            {
                CRIT_STATE_T  t_state;

                t_state = x_crit_start();

                if (handle_get_obj(h_ch_conn, (VOID**) & pt_conn) == HR_OK)
                {
                    h_rm_tv_decoder = pt_conn->h_rm_tv_decoder;
                }
                else
                {
                    i4_res = CMR_FAIL;
                }

                x_crit_end(t_state);
            }
            else
            {
                i4_res = CMR_INV_ARG;
            }

            if (i4_res == CMR_OK)
            {
                AVC_HANDLER_SCART_MODE_T  e_scart_mode;
                TV_DEC_SCART_MODE_T       e_tv_dec_scart_mode;

                e_scart_mode = *((AVC_HANDLER_SCART_MODE_T*) pv_ctrl_data);

                e_tv_dec_scart_mode =
                    avc_handler_scart_mode_to_drv_scart_mode(e_scart_mode);

                i4_res = rm_set(h_rm_tv_decoder,
                                FALSE,
                                ANY_PORT_NUM,
                                TV_DEC_SET_TYPE_SCART_INP_MODE,
                                (VOID*) & e_tv_dec_scart_mode,
                                sizeof(TV_DEC_SCART_MODE_T),
                                NULL);

                i4_res = (i4_res == RMR_OK) ? CMR_OK : CMR_FAIL;
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
 * Name: avc_handler_thread_main
 *
 * Description: This API is the AVC Connection Handler's thread entry point.
 *
 * Inputs:  pv_arg  References the AVC handler information.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID avc_handler_thread_main (VOID* pv_arg)
{
    AVC_HANDLER_INFO_T*  pt_info;
    UINT32               ui4_nb_connections;
    BOOL                 b_exit_when_done, b_exit_now;

    pt_info = (AVC_HANDLER_INFO_T*) pv_arg;

    ui4_nb_connections = 0;

    b_exit_when_done = FALSE;
    b_exit_now       = FALSE;

    while (! b_exit_now)
    {
        AVC_HANDLER_CONN_T*  pt_conn;
        CM_MSG_T             t_msg;
        SIZE_T               z_msg_size;
        UINT16               ui2_index;

        z_msg_size = sizeof(CM_MSG_T);

        if (x_msg_q_receive(& ui2_index,
                            & t_msg,
                            & z_msg_size,
                            & pt_info->h_avc_handler_msgq, 1,
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
            continue;
        }

        if (t_msg.h_obj != NULL_HANDLE)
        {
            if (handle_get_obj(t_msg.h_obj, (VOID**) & pt_conn) != HR_OK)
            {
                DBG_ERROR((DBG_PREFIX"handle_get_obj failed\n"));

                continue;
            }

            VALIDATE_CONN(pt_conn);
        }
        else
        {
            pt_conn = NULL;
        }

        switch (t_msg.ui4_code)
        {
            case AVC_HANDLER_MSG_CONNECT_REQUEST:
            {
                DBG_INFO((DBG_PREFIX"Connection request (%d, %p)\n",
                          t_msg.h_obj, pt_conn));

                ui4_nb_connections++;

                DBG_INFO((DBG_PREFIX"Handler has %d connection(s)\n",
                          ui4_nb_connections));

                pt_conn->pui4_nb_connections = & ui4_nb_connections;

                avc_handler_open_conn(pt_conn);
            }
            break;

            case AVC_HANDLER_MSG_DISCONNECT_REQUEST:
            {
                DBG_INFO((DBG_PREFIX"Disconnection request (%d, %p)\n",
                          t_msg.h_obj, pt_conn));

                avc_handler_disconnect_conn(pt_conn, AVC_DISC_AS_REQUESTED);
            }
            break;

            case AVC_HANDLER_MSG_AVC_OPENED:
            {
                avc_handler_avc_opened(pt_conn);
            }
            break;

            case AVC_HANDLER_MSG_AVC_CLOSED:
            {
                avc_handler_avc_closed(pt_conn);
            }
            break;

            case AVC_HANDLER_MSG_AVC_CONNECTED:
            {
                avc_handler_avc_connected(pt_conn);
            }
            break;

            case AVC_HANDLER_MSG_AVC_DISCONNECTED:
            {
                if (! pt_conn->b_disconnecting)
                {
                    pt_conn->b_disconnecting = TRUE;

                    avc_handler_disconnect_tv_decoder(pt_conn);
                }

                avc_handler_avc_disconnected(pt_conn);
            }
            break;

            case AVC_HANDLER_MSG_DECODER_OPENED:
            {
                avc_handler_tv_decoder_opened(pt_conn);
            }
            break;

            case AVC_HANDLER_MSG_DECODER_CLOSED:
            {
                avc_handler_tv_decoder_closed(pt_conn);
            }
            break;

            case AVC_HANDLER_MSG_DECODER_CONNECTED:
            {
                avc_handler_tv_decoder_connected(pt_conn);
            }
            break;

            case AVC_HANDLER_MSG_DECODER_DISCONNECTED:
            {
                if (! pt_conn->b_disconnecting)
                {
                    pt_conn->b_disconnecting = TRUE;

                    avc_handler_disconnect_avc(pt_conn);
                }

                avc_handler_tv_decoder_disconnected(pt_conn);
            }
            break;

            case AVC_HANDLER_MSG_TV_SIGNAL_TIMER_EXPIRED:
            {
                if (pt_conn->h_timer != NULL_HANDLE)
                {
                    avc_handler_disconnect_conn(
                        pt_conn,
                        AVC_DISC_TV_SIGNAL_TIMER_EXPIRED);
                }
            }
            break;

            case AVC_HANDLER_MSG_TV_DEC_PLAY_DONE:
            {
                DBG_INFO((DBG_PREFIX"TV decoder 'playing' (%d)\n",
                          pt_conn->h_ch_conn));

                avc_handler_tv_decoder_playing(pt_conn);
            }
            break;

            case AVC_HANDLER_MSG_TV_DEC_GOT_TV_SIGNAL:
            {
                DBG_INFO((DBG_PREFIX"TV signal detected (%d)\n",
                          pt_conn->h_ch_conn));

                avc_handler_tv_decoder_got_tv_signal(pt_conn);
            }
            break;

            case AVC_HANDLER_MSG_TV_DEC_LOST_TV_SIGNAL:
            {
                DBG_INFO((DBG_PREFIX"TV signal lost (%d)\n",
                          pt_conn->h_ch_conn));

                avc_handler_tv_decoder_lost_tv_signal(pt_conn);
            }
            break;

            case AVC_HANDLER_MSG_TV_DEC_COLOR_SYS:
            {
                DBG_INFO((DBG_PREFIX"TV color system detected (%d)\n",
                          pt_conn->h_ch_conn));

                avc_handler_tv_decoder_color_sys_changed(pt_conn);
            }
            break;

            case AVC_HANDLER_MSG_AVC_SIGNAL_STATUS:
            {
                DBG_INFO((DBG_PREFIX"AVC signal status changed to 0x%x (%d)\n",
                          t_msg.ui4_data_1,
                          pt_conn->h_ch_conn));

                avc_handler_avc_signal_status(pt_conn, t_msg.ui4_data_1);

                if (pt_conn->b_scdb_has_audio
                    ||
                    pt_conn->b_scdb_video_detected)
                {
                    avc_handler_nfy_connected(pt_conn);
                }
            }
            break;

            case AVC_HANDLER_MSG_EXIT:
            {
                b_exit_when_done = TRUE;
            }
            break;

            default:
                /* unrecognized messages are ignored */
                break;
        }

        b_exit_now = b_exit_when_done && (ui4_nb_connections == 0);
    }

    x_msg_q_delete(pt_info->h_avc_handler_msgq);

    h_avc_handler_msgq = NULL_HANDLE;
}

/*-----------------------------------------------------------------------------
 * Name: avc_handler_set_init
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
static BOOL avc_handler_set_init (BOOL     b_flag,
                                  UINT16*  pui2_gen)
{
    static UINT16  ui2_gen = 0;

    CRIT_STATE_T  t_state;
    BOOL          b_res;

    t_state = x_crit_start();

    b_res  = b_init;
    b_init = b_flag;

    h_avc_handler = NULL_HANDLE;

    if (b_flag)
    {
         /* generation number in the range 0-65536 */
        *pui2_gen = ui2_gen++;
    }

    x_crit_end(t_state);

    return b_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_avc_reg_handler
 *
 * Description: This API registers the AVC Connection Handler.
 *
 * Inputs:  ps_names         References a string that contains all the names
 *                           for the AVC Connection Handler.
 *          pt_thread_descr  References information about the thread to create.
 *          pt_time_outs     References the time-outs values.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  The connection handler is not registered.
 ----------------------------------------------------------------------------*/
INT32 x_avc_reg_handler (const CHAR*                    ps_name,
                         const THREAD_DESCR_T*          pt_thread_descr,
                         const AVC_HANDLER_TIME_OUT_T*  pt_time_outs)
{
    CM_HANDLER_FCT_TBL_T  t_avc_handler_fct_tbl =
        {
            avc_handler_connect,
            avc_handler_disconnect,
            avc_handler_get,
            avc_handler_set
        };

    INT32   i4_res = CMR_NOT_INIT;
    UINT16  ui2_gen;

    DBG_API((DBG_PREFIX"x_avc_handler_reg_handler(%s, %p)\n",
             (ps_name == NULL) ? "NULL" : ps_name, pt_thread_descr));

    if (! avc_handler_set_init(TRUE, & ui2_gen))
    {
        SIZE_T  z_stack_size;
        UINT8   ui1_priority;
        CHAR    s_name[16 + 1];

        if (ps_name == NULL)
        {
            ps_name = AVC_HANDLER_DEFAULT_NAME;
        }

        z_stack_size = AVC_HANDLER_DEFAULT_STACK_SIZE;
        ui1_priority = AVC_HANDLER_DEFAULT_PRIORITY;

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

        x_sprintf(s_name, "avc-%d", ui2_gen);

        if (x_msg_q_create(& h_avc_handler_msgq,
                           s_name,
                           sizeof(CM_MSG_T),
                           AVC_HANDLER_MSG_QUEUE_SIZE)
            == OSR_OK)
        {
            AVC_HANDLER_INFO_T  t_info;

            t_info.h_avc_handler_msgq = h_avc_handler_msgq;

            if (x_thread_create(& t_info.h_avc_handler_thread,
                                s_name,
                                z_stack_size,
                                ui1_priority,
                                avc_handler_thread_main,
                                sizeof(t_info), (VOID*) & t_info)
                == OSR_OK)
            {
                if (pt_time_outs != NULL)
                {
                    t_time_outs = *pt_time_outs;
                }
                else
                {
                    t_time_outs = t_default_time_outs;
                }

                i4_res = cm_reg_handler(ps_name,
                                        & t_avc_handler_fct_tbl,
                                        & h_avc_handler);

                if (i4_res == CMR_OK)
                {
                    avc_handler_cli_init();
                }
                else
                {
                    /* exit thread */
                    cm_send_msg_retry(h_avc_handler_msgq,
                                      AVC_HANDLER_MSG_EXIT, 0, 0,
                                      NULL_HANDLE);

                    i4_res = CMR_NOT_INIT;
                }
            }
            else
            {
                x_msg_q_delete(h_avc_handler_msgq);
            }
        }

        /* if something failed... */
        if (i4_res != CMR_OK)
        {
            (VOID) avc_handler_set_init(FALSE, NULL);
        }
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_avc_unreg_handler
 *
 * Description: This API unregisters the AVC Connection Handler.
 *
 * Inputs:  b_force  Contains a boolean value that specifies whether or not the
 *                   connections maintained by this handler should be closed.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  The connection handler is not unregistered.
 ----------------------------------------------------------------------------*/
INT32 x_avc_unreg_handler (BOOL b_force)
{
    HANDLE_T  h_handler, h_msgq;
    INT32     i4_res;

    h_handler = h_avc_handler;
    h_msgq    = h_avc_handler_msgq;

    DBG_API((DBG_PREFIX"x_avc_unreg_handler(%s)\n",
             b_force ? "TRUE" : "FALSE"));

    if (avc_handler_set_init(FALSE, NULL))
    {
        i4_res = cm_unreg_handler(h_handler, b_force);

        if (i4_res == CMR_OK)
        {
            i4_res = cm_send_msg_retry(h_msgq,
                                       AVC_HANDLER_MSG_EXIT, 0, 0,
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
