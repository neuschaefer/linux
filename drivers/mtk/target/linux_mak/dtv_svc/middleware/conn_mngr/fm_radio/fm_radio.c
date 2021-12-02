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
 * $RCSfile: fm_radio.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains the FM Radio Connection Handler main functions,
 *         including the handler registration, unregistration and thread main
 *         function.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "conn_mngr/fm_radio/x_fm_radio.h"
#include "conn_mngr/fm_radio/_fm_radio.h"
#include "conn_mngr/fm_radio/fm_radio_cmd.h"
#include "os/inc/x_os.h"
#include "conn_mngr/x_cm.h"
#include "conn_mngr/cm.h"
#include "conn_mngr/cm_handler_api.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_tuner.h"
#include "scdb/x_scdb_api.h"
#include "scdb/scdb_api.h"
#include "svl/x_svl.h"
#include "dbg/def_dbg_level_mw.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define FM_RADIO_CONN_MAGIC  ((UINT32) 24112310)

#define VALIDATE_CONN(_pt_conn) \
do { \
    if ((_pt_conn)->ui4_magic != FM_RADIO_CONN_MAGIC) \
        ABORT(DBG_CAT_NO_RECOVERY, 2); \
} while(0)

#define FM_RADIO_ASSERT(_cond) \
do {if (! (_cond)) ABORT(DBG_CAT_NO_RECOVERY, 666);} while(0)

#define FM_RADIO_MSG_PRIORITY       ((UINT8)   1)
#define FM_RADIO_MSG_RETRIES_COUNT  ((UINT32) 10)
#define FM_RADIO_MSG_RETRY_DELAY    ((UINT32) 50)

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

static HANDLE_T  h_fm_radio_handler;
static HANDLE_T  h_fm_radio_msgq;
static BOOL      b_init = FALSE;

static UINT16  ui2_fm_radio_dbg_level = DBG_INIT_LEVEL_MW_FM_RADIO;

/*-----------------------------------------------------------------------------
 * Name: fm_radio_get_dbg_level
 *
 * Description: This API gets the debug level of the FM Radio Handler.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: the FM Radio Handler debug level.
 ----------------------------------------------------------------------------*/
UINT16 fm_radio_get_dbg_level (
    VOID
)
{
    return ui2_fm_radio_dbg_level;
}

/*-----------------------------------------------------------------------------
 * Name: analog brdcst_set_dbg_level
 *
 * Description: This API sets the debug level of the FM Radio Handler.
 *
 * Inputs:  ui2_dbg_level  Contains the new debug level.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The debug level was set successfully.
 *          CMR_FAIL  The debug level could not be set.
 ----------------------------------------------------------------------------*/
INT32 fm_radio_set_dbg_level (
    UINT16    ui2_dbg_level
)
{
    ui2_fm_radio_dbg_level = ui2_dbg_level;

    return CMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_conn_disconnected
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
static VOID fm_radio_conn_disconnected (
    FM_RADIO_CONN_T*    pt_conn
)
{
    HANDLE_T            h_cm_conn;
    UINT32              ui4_reason;

    h_cm_conn  = pt_conn->h_cm_conn;
    ui4_reason = pt_conn->ui4_disconnection_reason;

    DBG_INFO((DBG_PREFIX"Notify CM: disconnected (%d, reason=%d)\n",
              pt_conn->h_ch_conn, ui4_reason));

    handle_free(pt_conn->h_ch_conn, FALSE);

    cm_nfy(h_cm_conn, CM_COND_DISCONNECTED, ui4_reason);
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_tuner_closed
 *
 * Description: This API is called when the tuner component is closed. If a
 *              connection was using the component then the connection handle
 *              is deleted.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_tuner_closed (
    FM_RADIO_CONN_T*    pt_conn
)
{
    pt_conn->h_rm_tuner = NULL_HANDLE;

    if (pt_conn->h_rm_audio_decoder == NULL_HANDLE)
    {
        fm_radio_conn_disconnected(pt_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_close_tuner
 *
 * Description: This API closes the tuner driver component.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_close_tuner (
    FM_RADIO_CONN_T*    pt_conn
)
{
    INT32  i4_ret;

    DBG_INFO((DBG_PREFIX"Tuner being closed (%d)\n",
              pt_conn->h_ch_conn));

    i4_ret = rm_close(pt_conn->h_rm_tuner, FALSE, NULL);

    if (i4_ret == RMR_OK)
    {
        DBG_INFO((DBG_PREFIX"Tuner closed synchronously (%d)\n",
                  pt_conn->h_ch_conn));

        fm_radio_tuner_closed(pt_conn);
    }
    else if (i4_ret == RMR_ASYNC_NFY)
    {
        /* nothing to do here */
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Tuner could not be closed (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_ret));
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_audio_decoder_closed
 *
 * Description: This API is called when the audio decoder component is closed.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_audio_decoder_closed (
    FM_RADIO_CONN_T*    pt_conn
)
{
    pt_conn->h_rm_audio_decoder = NULL_HANDLE;

    if (pt_conn->h_rm_tuner == NULL_HANDLE)
    {
        fm_radio_conn_disconnected(pt_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_close_audio_decoder
 *
 * Description: This API closes the audio decoder driver component.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_close_audio_decoder (
    FM_RADIO_CONN_T*    pt_conn
)
{
    INT32               i4_ret;

    DBG_INFO((DBG_PREFIX"Audio decoder being closed (%d)\n",
              pt_conn->h_ch_conn));

    i4_ret = rm_close(pt_conn->h_rm_audio_decoder, FALSE, NULL);

    if (i4_ret == RMR_OK)
    {
        DBG_INFO((DBG_PREFIX"Audio decoder closed synchronously (%d)\n",
                  pt_conn->h_ch_conn));

        fm_radio_audio_decoder_closed(pt_conn);
    }
    else if (i4_ret == RMR_ASYNC_NFY)
    {
        /* nothing to do here */
    }
    else
    {
        DBG_ERROR((DBG_PREFIX
                   "Audio decoder could not be closed (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_ret));
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_close_conn
 *
 * Description: This API closes the TV decoder and the tuner (if needed).
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_close_conn (
    FM_RADIO_CONN_T*    pt_conn
)
{
    if (!pt_conn->b_closing)
    {
        HANDLE_T    h_rm_tuner;
        BOOL        b_conn_disconnected;

        pt_conn->b_closing = TRUE;

        h_rm_tuner = pt_conn->h_rm_tuner;

        b_conn_disconnected =
            (pt_conn->h_rm_audio_decoder == NULL_HANDLE)
            && (pt_conn->h_rm_tuner == NULL_HANDLE);

        if (pt_conn->h_rm_audio_decoder != NULL_HANDLE)
        {
            fm_radio_close_audio_decoder(pt_conn);
        }

        if (h_rm_tuner != NULL_HANDLE)
        {
            fm_radio_close_tuner(pt_conn);
        }

        if (b_conn_disconnected)
        {
            fm_radio_conn_disconnected(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_tuner_disconnected
 *
 * Description: This API is called when the tuner component is disconnected.
 *
 * Inputs:  pt_conn        References the connection structure.
 *          ui4_rm_reason  Contains the RM disconnection reason.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_tuner_disconnected (
    FM_RADIO_CONN_T*    pt_conn,
    UINT32              ui4_rm_reason
)
{
    if (!pt_conn->b_tuner_disconnected)
    {
        pt_conn->b_tuner_disconnected = TRUE;

        if (!pt_conn->b_disconnecting)
        {
            UINT32  ui4_reason;

            switch (ui4_rm_reason)
            {
                case RM_DISC_REASON_AS_REQUESTED:
                    ui4_reason = FM_RADIO_DISC_AS_REQUESTED;
                    break;

                case TUNER_DISC_REASON_SIGNAL_LOSS:
                    ui4_reason = FM_RADIO_DISC_TUNER_SIGNAL_LOST;
                    break;

                case TUNER_DISC_REASON_UNKNOWN_LNB:
                    ui4_reason = FM_RADIO_DISC_TUNER_UNKNOWN_LNB;
                    break;

                case TUNER_DISC_REASON_OUT_OF_RANGE:
                    ui4_reason = FM_RADIO_DISC_TUNER_OUT_OF_RANGE;
                    break;

                default:
                    ui4_reason = FM_RADIO_DISC_OTHER_REASON;
                    break;
            }

            pt_conn->ui4_disconnection_reason = ui4_reason;
            pt_conn->b_disconnecting          = TRUE;
        }

        if ((pt_conn->b_audio_decoder_disconnected
             || (pt_conn->e_rm_connect_audio_decoder_cond == RM_COND_IGNORE))
            )
        {
            fm_radio_close_conn(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_disconnect_tuner
 *
 * Description: This API disconnects the tuner driver component.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_disconnect_tuner (
    FM_RADIO_CONN_T*    pt_conn
)
{
    INT32  i4_ret;

    DBG_INFO((DBG_PREFIX"Tuner being disconnected (%d)\n",
              pt_conn->h_ch_conn));

    i4_ret = rm_disconnect(pt_conn->h_rm_tuner,
                           RM_DISC_TYPE_IGNORE,
                           NULL, /* disconnection info */
                           0, /* disconnection info size */
                           & pt_conn->e_rm_disconnect_tuner_cond);

    if ((i4_ret == RMR_OK) || (pt_conn->h_rm_tuner == NULL_HANDLE))
    {
        DBG_INFO((DBG_PREFIX"Tuner disconnected synchronously (%d)\n",
                  pt_conn->h_ch_conn));
    }
    else if (i4_ret == RMR_ASYNC_NFY)
    {
        /* nothing to do here */
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Could not disconnect tuner (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_ret));
    }

    fm_radio_tuner_disconnected(pt_conn, RM_DISC_REASON_AS_REQUESTED);
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_audio_decoder_playing
 *
 * Description: This API is called when the audio decoder is "playing" (i.e.
 *              TV system detection is started).
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_audio_decoder_playing (
    FM_RADIO_CONN_T*    pt_conn
)
{
    DBG_INFO((DBG_PREFIX"Audio decoder detection started (%d)\n",
              pt_conn->h_ch_conn));

    if (!pt_conn->b_audio_decoder_playing)
    {
        pt_conn->b_audio_decoder_playing = TRUE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_start_channel_detection
 *
 * Description: This API is starts the TV system detection (using the audio
 *              decoder).
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The color system detection was started successfully.
 *          CMR_FAIL  The color system detection could not be started.
 ----------------------------------------------------------------------------*/
static INT32 fm_radio_start_channel_detection (
    FM_RADIO_CONN_T*    pt_conn
)
{
    INT32               i4_ret;

    i4_ret = rm_set(pt_conn->h_rm_audio_decoder,
                    FALSE,
                    ANY_PORT_NUM,
                    AUD_DEC_SET_TYPE_CTRL,
                    (VOID*) AUD_DEC_CTRL_PLAY,
                    sizeof(AUD_DEC_CTRL_T),
                    NULL);

    if (i4_ret == RMR_OK)
    {
        fm_radio_audio_decoder_playing(pt_conn);
    }

    if ((i4_ret != RMR_OK) && (i4_ret != RMR_ASYNC_NFY))
    {
        DBG_ERROR((DBG_PREFIX
                   "Could not start channel detection (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_ret));
    }

    i4_ret = ((i4_ret == RMR_OK) || (i4_ret == RMR_ASYNC_NFY))
             ? CMR_OK : CMR_FAIL;

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_tuner_status
 *
 * Description: This API is called when the tuner status (e.g. SIGNAL LOCKED)
 *              is changed.
 *
 * Inputs:  pt_conn   References the connection structure.
 *          e_status  Contains the status information from the tuner.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_tuner_status (
    FM_RADIO_CONN_T*       pt_conn,
    TUNER_COND_STATUS_T    e_status
)
{
    switch (e_status)
    {
        case TUNER_COND_STATUS_GOT_SIGNAL:
        {
            DBG_INFO((DBG_PREFIX"Tuner status: got signal (%d)\n",
                      pt_conn->h_ch_conn));

            cm_nfy(pt_conn->h_cm_conn,
                   CM_COND_CONNECTION_INFO,
                   CM_CONN_INFO_GOT_TUNER_SIGNAL);
        }
        break;

        case TUNER_COND_STATUS_LOST_SIGNAL:
        {
            DBG_INFO((DBG_PREFIX"Tuner status: lost signal (%d)\n",
                      pt_conn->h_ch_conn));

            cm_nfy(pt_conn->h_cm_conn,
                   CM_COND_CONNECTION_INFO,
                   CM_CONN_INFO_LOST_TUNER_SIGNAL);
        }
        break;

        case TUNER_COND_STATUS_EWS_ON:
        {
            DBG_INFO((DBG_PREFIX"Tuner status: EWS on (%d)\n",
                      pt_conn->h_ch_conn));

            cm_nfy(pt_conn->h_cm_conn,
                   CM_COND_CONNECTION_INFO,
                   CM_CONN_INFO_EWS_ON);
        }
        break;

        case TUNER_COND_STATUS_EWS_OFF:
        {
            DBG_INFO((DBG_PREFIX"Tuner status: EWS off (%d)\n",
                      pt_conn->h_ch_conn));

            cm_nfy(pt_conn->h_cm_conn,
                   CM_COND_CONNECTION_INFO,
                   CM_CONN_INFO_EWS_OFF);
        }
        break;

        default:
            break;
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_audio_decoder_disconnected
 *
 * Description: This API is called when the audio decoder component is
 *              disconnected.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_audio_decoder_disconnected (
    FM_RADIO_CONN_T*    pt_conn
)
{
    if (!pt_conn->b_audio_decoder_disconnected)
    {
        pt_conn->b_audio_decoder_disconnected = TRUE;

        if (pt_conn->b_tuner_disconnected)
        {
            fm_radio_close_conn(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_audio_decoder_stopped
 *
 * Description: This API is called after the audio decoder sends a STOPPED
 *              notification.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_audio_decoder_stopped (
    FM_RADIO_CONN_T*    pt_conn
)
{
    CM_COND_T                 e_cond;
    FM_RADIO_DETECT_DONE_T    t_msg;

    DBG_INFO((DBG_PREFIX"Audio decoder stopped (%d)\n", pt_conn->h_ch_conn));

    pt_conn->b_audio_decoder_playing = FALSE;

    /* fill notify data */
    e_cond = FM_RADIO_CONN_INFO_DETECT_DONE;

    t_msg.b_detected = (pt_conn->b_signal_detected == TRUE) ? TRUE : FALSE;
    t_msg.ui4_freq = pt_conn->t_connect_info.ui4_freq;
    t_msg.ui1_sgl_lvl = pt_conn->ui1_sgl_lvl;

    cm_nfy(pt_conn->h_cm_conn,
           e_cond,
           (UINT32)&t_msg);
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_disconnect_audio_decoder
 *
 * Description: This API disconnects the audio decoder driver component.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_disconnect_audio_decoder (
    FM_RADIO_CONN_T*    pt_conn
)
{
    INT32               i4_ret;

    DBG_INFO((DBG_PREFIX"Audio decoder being disconnected (%d)\n",
              pt_conn->h_ch_conn));

    i4_ret = rm_disconnect(pt_conn->h_rm_audio_decoder,
                           RM_DISC_TYPE_IGNORE,
                           NULL, /* disconnection info */
                           0, /* disconnection info size */
                           &pt_conn->e_rm_disconnect_audio_decoder_cond);

    if ((i4_ret == RMR_OK) || (pt_conn->h_rm_audio_decoder == NULL_HANDLE))
    {
        DBG_INFO((DBG_PREFIX"Audio decoder disconnected synchronously (%d)\n",
                  pt_conn->h_ch_conn));

        fm_radio_audio_decoder_disconnected(pt_conn);
    }
    else if (i4_ret == RMR_ASYNC_NFY)
    {
        /* nothing to do here */
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Audio decoder could not be disconnected (%d)\n",
                    i4_ret));
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_disconnect_conn
 *
 * Description: This API requests the disconnection of the tuner driver
 *              component.
 *
 * Inputs:  pt_conn     References the connection.
 *          ui4_reason  Contains the disconnection reason.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_disconnect_conn (
    FM_RADIO_CONN_T*    pt_conn,
    UINT32              ui4_reason
)
{
    if (!pt_conn->b_disconnecting)
    {
        BOOL    b_close_conn;

        DBG_INFO((DBG_PREFIX"Disconnecting connection (%d, reason=%d)\n",
                  pt_conn->h_ch_conn, ui4_reason));

        pt_conn->b_disconnecting = TRUE;
        pt_conn->ui4_disconnection_reason = ui4_reason;

        b_close_conn =
            (pt_conn->e_rm_connect_audio_decoder_cond == RM_COND_IGNORE)
            &&
            (pt_conn->e_rm_connect_tuner_cond == RM_COND_IGNORE);

        if (pt_conn->e_rm_connect_audio_decoder_cond != RM_COND_IGNORE
            && pt_conn->h_rm_audio_decoder != NULL_HANDLE)
        {
            fm_radio_disconnect_audio_decoder(pt_conn);
        }

        if (pt_conn->e_rm_connect_tuner_cond != RM_COND_IGNORE)
        {
            fm_radio_disconnect_tuner(pt_conn);
        }

        if (b_close_conn)
        {
            fm_radio_close_conn(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_get_stream_tag
 *
 * Description: This API returns a stream tag (i.e. a VOID* pointer).
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: a non-NULL pointer that is used as a stream tag.
 ----------------------------------------------------------------------------*/
static VOID* fm_radio_get_stream_tag(
    VOID
)
{
    static VOID*    pv_stream_tag = NULL;
    CRIT_STATE_T    t_state;
    VOID*           pv_res;

    t_state = x_crit_start();

    pv_stream_tag = (VOID*)(((CHAR*)pv_stream_tag) + 1);

    if (pv_stream_tag == NULL)
    {
        pv_stream_tag = (VOID*)(((CHAR*)pv_stream_tag) + 1);
    }

    pv_res = pv_stream_tag;

    x_crit_end(t_state);

    return pv_res;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_fill_default_scdb
 *
 * Description: This API fills the SCDB with default audio and video 'streams'.
 *              Closed-caption streams are not added.
 *
 * Inputs:  pt_conn  References the connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  The operation could not be fully completed.
 ----------------------------------------------------------------------------*/
static INT32 fm_radio_fill_default_scdb (
    FM_RADIO_CONN_T*    pt_conn
)
{
    HANDLE_T            h_scdb;
    INT32               i4_ret;

    h_scdb = pt_conn->h_scdb;

    i4_ret = scdb_lock(h_scdb);

    if (i4_ret == SCDBR_OK)
    {
        STREAM_COMP_ID_T    t_stream_comp_id;
        SCDB_REC_T          t_scdb_rec;
        UINT32              ui4_tv_sys;
        UINT32              ui4_audio_sys;

        /* we got the audio system from the SVL record */
        ui4_audio_sys = pt_conn->t_connect_info.ui4_aud_sys_mask;
        ui4_tv_sys = 0;

        /* audio */
        t_stream_comp_id.e_type        = ST_AUDIO;
        t_stream_comp_id.pv_stream_tag = fm_radio_get_stream_tag();

        t_scdb_rec.e_rec_type = SCDB_REC_TYPE_AUDIO_ANALOG;

        t_scdb_rec.u.t_audio_analog.ui4_tv_sys    = ui4_tv_sys;
        t_scdb_rec.u.t_audio_analog.ui4_audio_sys = ui4_audio_sys;
        t_scdb_rec.u.t_audio_analog.e_aud_fmt     = AUD_FMT_UNKNOWN;
        t_scdb_rec.u.t_audio_analog.e_enc         = AUD_ENC_FM_RADIO;
        t_scdb_rec.u.t_audio_analog.b_default     = TRUE;
        t_scdb_rec.u.t_audio_analog.b_enable_aud_sys_auto_detect = FALSE;

        i4_ret = scdb_add_rec(h_scdb, &t_stream_comp_id, &t_scdb_rec);

        scdb_unlock(h_scdb);

        i4_ret = (i4_ret == SCDBR_OK) ? CMR_OK : CMR_FAIL;
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Could not lock SCDB\n"));

        i4_ret = CMR_FAIL;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_create_scdb
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
static INT32 fm_radio_create_scdb (
    FM_RADIO_CONN_T*    pt_conn
)
{
    SCDB_COMMON_T       t_scdb_common;
    INT32               i4_ret = CMR_OK;

    t_scdb_common.e_data_fmt  = DATA_FMT_ANALOG;
    t_scdb_common.e_feed_mode = FEED_MODE_PUSH;

    i4_ret = scdb_create(pt_conn->h_cm_conn,
                         &t_scdb_common,
                         &pt_conn->h_scdb);

    i4_ret = (i4_ret == SCDBR_OK) ? CMR_OK : CMR_FAIL;

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_audio_decoder_nfy
 *
 * Description: This API is the audio decoder notification function.
 *
 * Inputs:  pt_conn     References the tag.
 *          e_nfy_cond  Contains the notification condition.
 *          ui4_data_1  Contains additional data.
 *          ui4_data_2  Contains additional data.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_audio_decoder_nfy (
    VOID*             pv_tag,
    AUD_DEC_COND_T    e_nfy_cond,
    UINT32            ui4_data_1,
    UINT32            ui4_data_2
)
{
    UINT32            e_code;

    DBG_INFO((DBG_PREFIX"Audio decoder nfy %d (data1=%d, data2=%d) (%d)\n",
              e_nfy_cond, ui4_data_1, ui4_data_2, (HANDLE_T)pv_tag));

    switch (e_nfy_cond)
    {
        case AUD_DEC_COND_CTRL_DONE:
        {
            switch (ui4_data_1)
            {
                case AUD_DEC_CTRL_PLAY:
                     e_code = FM_RADIO_MSG_AUDIO_DEC_PLAY_DONE;
                     break;
                case AUD_DEC_CTRL_STOP:
                     e_code = FM_RADIO_MSG_AUDIO_DEC_STOP_DONE;
                     break;
                default:
                    e_code = FM_RADIO_MSG_NONE;
            }
        }
        break;

        case AUD_DEC_COND_FM_RADIO_DET_DONE:
        {
            e_code = FM_RADIO_MSG_AUDIO_DEC_DETECT_DONE;
        }
        break;

        default:
            e_code = FM_RADIO_MSG_NONE;
    }

    if (e_code != FM_RADIO_MSG_NONE)
    {
        HANDLE_T    h_ch_conn;

        h_ch_conn = (HANDLE_T)pv_tag;

        cm_send_msg_no_retry(h_fm_radio_msgq,
                             e_code,
                             ui4_data_1, ui4_data_2,
                             h_ch_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_audio_decoder_detect_done
 *
 * Description: This API is called when the audio decoder detects the TV system
 *              (A, B, C, etc) and audio system.
 *
 * Inputs:  pt_conn    References the connection structure.
 *          ui4_tv     Contains the TV system mask.
 *          ui4_audio  Contains the audio system mask.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_audio_decoder_detect_done(
    FM_RADIO_CONN_T*          pt_conn,
    UINT32                    ui4_data_1,
    UINT32                    ui4_data_2
)
{
    INT32                     i4_ret;
    AUD_DEC_FM_RADIO_DET_T    e_result;

    DBG_INFO((DBG_PREFIX
              "FM channel detect done, results %d (%d)\n",
              ui4_data_1, pt_conn->h_ch_conn));

    e_result = (AUD_DEC_FM_RADIO_DET_T)ui4_data_1;

    pt_conn->b_signal_detected =
        (e_result == AUD_DEC_FM_RADIO_DETECTED)
        ? TRUE : FALSE;
    pt_conn->ui1_sgl_lvl = (UINT8)ui4_data_2;

    /* stop audio decoder */
    i4_ret = rm_set(pt_conn->h_rm_audio_decoder,
                    FALSE,
                    ANY_PORT_NUM,
                    AUD_DEC_SET_TYPE_CTRL,
                    (VOID*) AUD_DEC_CTRL_STOP,
                    sizeof(AUD_DEC_CTRL_T),
                    NULL);

    if (i4_ret == RMR_OK)
    {
        fm_radio_audio_decoder_stopped(pt_conn);
    }
    else if (i4_ret == RMR_ASYNC_NFY)
    {
        /* nothing to do here */
    }
    else
    {
        DBG_ERROR((DBG_PREFIX
                   "Could not stop audio decoder (%d, res=%d)\n",
                   pt_conn->h_ch_conn, i4_ret));
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_set_audio_decoder_nfy
 *
 * Description: This API is called when both tuner and audio decoder are
 *              connected and the audio decoder notification function is set.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_set_audio_decoder_nfy (
    FM_RADIO_CONN_T*      pt_conn
)
{
    AUD_DEC_NFY_INFO_T    t_nfy_info;
    INT32                 i4_ret;

    t_nfy_info.pv_tag         = (VOID*) pt_conn->h_ch_conn;
    t_nfy_info.pf_aud_dec_nfy = fm_radio_audio_decoder_nfy;

    /* rm_set AUD_DEC_SET_TYPE_NFY_FCT */
    i4_ret = rm_set(pt_conn->h_rm_audio_decoder,
                    FALSE,
                    ANY_PORT_NUM,
                    AUD_DEC_SET_TYPE_NFY_FCT,
                    (VOID*)&t_nfy_info,
                    sizeof(AUD_DEC_NFY_INFO_T),
                    NULL);

    if (i4_ret != RMR_OK)
    {
        fm_radio_disconnect_conn(
            pt_conn,
            FM_RADIO_DISC_COULD_NOT_SET_AUDIO_DECODER_PROPERTIES);
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_set_audio_decoder_fmt
 *
 * Description: This API is called when both tuner and audio decoder are
 *              connected and the audio decoder notification function is set.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_set_audio_decoder_fmt (
    FM_RADIO_CONN_T*      pt_conn
)
{
    INT32                 i4_ret;
    AUD_DEC_FMT_INFO_T    t_aud_fmt = {AUD_DEC_FMT_FMRDO_DET, NULL};

    i4_ret = rm_set(pt_conn->h_rm_audio_decoder,
                    FALSE,
                    ANY_PORT_NUM,
                    AUD_DEC_SET_TYPE_FMT,
                    (VOID*)&t_aud_fmt,
                    sizeof(AUD_DEC_FMT_INFO_T),
                    NULL);

    if (i4_ret != RMR_OK)
    {
        fm_radio_disconnect_conn(
            pt_conn,
            FM_RADIO_DISC_COULD_NOT_SET_AUDIO_DECODER_PROPERTIES);
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_nfy_connected
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
VOID fm_radio_nfy_connected (
    FM_RADIO_CONN_T*    pt_conn
)
{
     DBG_INFO((DBG_PREFIX"Notifying 'CONNECTED' (%d)\n",
                  pt_conn->h_ch_conn));

     cm_nfy(pt_conn->h_cm_conn, CM_COND_CONNECTED, 0);
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_audio_decoder_connected
 *
 * Description: This API is called when the audio decoder component is
 *              connected.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_audio_decoder_connected (
    FM_RADIO_CONN_T*    pt_conn
)
{
    if ((!pt_conn->b_disconnecting)
        && (!pt_conn->b_audio_decoder_connected))
    {
        pt_conn->b_audio_decoder_connected = TRUE;

        fm_radio_set_audio_decoder_nfy(pt_conn);

        if (pt_conn->ui4_flags
            & (FM_RADIO_DELIVERY_SYS_MASK | FM_RADIO_FREQUENCY_MASK))
        {
            fm_radio_set_audio_decoder_fmt(pt_conn);
        }

        fm_radio_start_channel_detection(pt_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_connect_audio_decoder
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
static VOID fm_radio_connect_audio_decoder (
    FM_RADIO_CONN_T*    pt_conn
)
{
    HANDLE_T            h_rm_comp;
    INT32               i4_ret;

    DBG_INFO((DBG_PREFIX"Connecting audio decoder (%d)...\n",
              pt_conn->h_ch_conn));


    h_rm_comp = pt_conn->h_rm_tuner;

    i4_ret = rm_connect(pt_conn->h_rm_audio_decoder,
                        RM_CONN_TYPE_HANDLE,
                        (VOID*)h_rm_comp,
                        sizeof(HANDLE_T),
                        (VOID*) pt_conn->h_ch_conn, /* tag */
                        &pt_conn->e_rm_connect_audio_decoder_cond);

    if (i4_ret == RMR_OK)
    {
        DBG_INFO((DBG_PREFIX"Audio decoder connected synchronously (%d)\n",
                  pt_conn->h_ch_conn));

        fm_radio_audio_decoder_connected(pt_conn);
    }
    else if (i4_ret == RMR_ASYNC_NFY)
    {
        /* nothing to do here */
    }
    else
    {
        if ((i4_ret == RMR_NO_RIGHTS)
            &&
            pt_conn->t_connect_info.b_audio_decoder_no_rights_ok)
        {
            DBG_INFO((DBG_PREFIX"No rights ok (%d)\n", pt_conn->h_ch_conn));

            fm_radio_audio_decoder_connected(pt_conn);
        }
        else
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not connect audio decoder (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_ret));

            fm_radio_disconnect_conn(
                pt_conn,
                FM_RADIO_DISC_COULD_NOT_CONNECT_AUDIO_DECODER);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_tuner_connected
 *
 * Description: This API is called when the tuner component is connected. The
 *              Connection Manager is then notified that the connection is now
 *              in the CONNECTED state.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_tuner_connected (
    FM_RADIO_CONN_T*    pt_conn
)
{
    if (!pt_conn->b_disconnecting)
    {
        if (!pt_conn->b_tuner_connected)
        {
            pt_conn->b_tuner_connected = TRUE;
        }
        fm_radio_nfy_connected(pt_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_fill_tune_info
 *
 * Description: This API fills the tune info structure depending on the
 *              connection type (terrestrial, satellite or cable) and the
 *              information (frequency, etc) provided by the caller.
 *
 * Inputs:  e_conn_type  Contains the connection type.
 *          pt_info      References a structure with the new tune information.
 *
 * Outputs: pt_tune   Will contain the tune information (frequency, etc).
 *          ppv_tune  Will contain the pointer to pass to rm_connect.
 *          pz_size   Will contain the size of the data passed to rm_connect.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_fill_tune_info (
    DRV_CONN_TYPE_T                   e_conn_type,
    const FM_RADIO_CONNECT_INFO_T*    pt_info,
    FM_RADIO_TUNE_INFO_T*             pt_tune,
    VOID**                            ppv_tune,
    SIZE_T*                           pz_size
)
{
    switch (e_conn_type)
    {
        case TUNER_CONN_TYPE_TER_ANA:
        {
            pt_tune->t_ter_ana.ui4_freq        = pt_info->ui4_freq;
            pt_tune->t_ter_ana.ui4_freq_bound  = pt_info->ui4_freq;
            pt_tune->t_ter_ana.ui4_central_freq = pt_info->ui4_freq;
            pt_tune->t_ter_ana.ui4_freq_offset  = 0;
            pt_tune->t_ter_ana.ui4_tv_sys_mask = 0;
            pt_tune->t_ter_ana.e_pol           = pt_info->e_pol;
            pt_tune->t_ter_ana.e_mod           = pt_info->e_mod;
            pt_tune->t_ter_ana.ui2_dir         = pt_info->ui2_dir;
            pt_tune->t_ter_ana.ui2_gain        = pt_info->ui2_gain;
            pt_tune->t_ter_ana.b_fine_tune     = pt_info->b_fine_tune;

            pt_tune->t_ter_ana.b_no_disc_nfy =
                ! pt_info->b_disc_if_tuner_signal_lost;

            *ppv_tune = (VOID *) & pt_tune->t_ter_ana;
            *pz_size  = sizeof(pt_tune->t_ter_ana);
        }
        break;

        case TUNER_CONN_TYPE_CAB_ANA:
        case TUNER_CONN_TYPE_SAT_ANA:
        default:
            *ppv_tune = NULL;
            *pz_size  = 0;
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_connect_tuner
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
static VOID fm_radio_connect_tuner (
    FM_RADIO_CONN_T*            pt_conn
)
{
    FM_RADIO_TUNE_INFO_T        t_tune_info;
    DRV_CONN_TYPE_T             e_conn_type;
    VOID*                       pv_connect_info;
    FM_RADIO_CONNECT_INFO_T*    pt_connect_info;
    SIZE_T                      z_size;
    INT32                       i4_ret;

    DBG_INFO((DBG_PREFIX"Connecting tuner (%d)\n", pt_conn->h_ch_conn));

    pt_connect_info = & pt_conn->t_connect_info;
    e_conn_type = pt_connect_info->ui4_del_sys;

    fm_radio_fill_tune_info(e_conn_type,
                            pt_connect_info,
                            &t_tune_info,
                            &pv_connect_info,
                            &z_size);

    if (pv_connect_info != NULL)
    {
        BOOL  b_disc_if_tuner_signal_lost =
            pt_conn->t_connect_info.b_disc_if_tuner_signal_lost;

        i4_ret = rm_set(pt_conn->h_rm_tuner,
                        TRUE,
                        ANY_PORT_NUM,
                        TUNER_SET_TYPE_NO_DISC_NFY,
                        (VOID*)!b_disc_if_tuner_signal_lost,
                        sizeof(BOOL),
                        NULL);

        if (i4_ret == RMR_OK)
        {
            i4_ret = rm_connect(pt_conn->h_rm_tuner,
                                e_conn_type,
                                pv_connect_info, z_size,
                                (VOID*)pt_conn->h_ch_conn, /* tag */
                                &pt_conn->e_rm_connect_tuner_cond);

            if (i4_ret == RMR_OK)
            {
                DBG_INFO((DBG_PREFIX"Tuner connected synchronously (%d)\n",
                          pt_conn->h_ch_conn));

                fm_radio_tuner_connected(pt_conn);
            }
            else if (i4_ret == RMR_ASYNC_NFY)
            {
                /* nothing to do here */
            }
            else
            {
                if ((i4_ret == RMR_NO_RIGHTS)
                    &&
                    pt_conn->t_connect_info.b_tuner_no_rights_ok)
                {
                    DBG_INFO((DBG_PREFIX"No rights ok (%d)\n",
                              pt_conn->h_ch_conn));

                    fm_radio_tuner_connected(pt_conn);
                }
                else
                {
                    DBG_ERROR((DBG_PREFIX
                               "Could not connect tuner (%d, res=%d)\n",
                               pt_conn->h_ch_conn, i4_ret));

                    fm_radio_disconnect_conn(
                        pt_conn,
                        FM_RADIO_DISC_COULD_NOT_CONNECT_TUNER);
                }
            }
        }
        else
        {
            DBG_ERROR((DBG_PREFIX
                       "Could not set tuner properties (%d, res=%d)\n",
                       pt_conn->h_ch_conn, i4_ret));

            fm_radio_disconnect_conn(
                pt_conn,
                FM_RADIO_DISC_COULD_NOT_SET_TUNER_PROPERTIES);
        }
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Unknown driver - Disconnect.\n"));

        fm_radio_disconnect_conn(pt_conn,
                                      FM_RADIO_DISC_OTHER_REASON);
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_connect_conn
 *
 * Description: This API connects all the driver components: tuner first, then
 *              SCART (if needed) and TV decoder.
 *
 * Inputs:  pt_conn     References the connection.
 *          ui4_reason  Contains the disconnection reason.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_connect_conn (
    FM_RADIO_CONN_T*    pt_conn
)
{
    if (!pt_conn->b_disconnecting)
    {
        fm_radio_connect_tuner(pt_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_audio_decoder_opened
 *
 * Description: This API is called when the audio decoder is opened (in the
 *              context of the handler message loop). It will try to connect
 *              the tuner.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_audio_decoder_opened (
    FM_RADIO_CONN_T*    pt_conn
)
{
    if (!pt_conn->b_audio_decoder_opened)
    {
        DBG_INFO((DBG_PREFIX"Audio decoder component now opened (%d)\n",
                  pt_conn->h_ch_conn));

        pt_conn->b_audio_decoder_opened = TRUE;

        if (!pt_conn->b_disconnecting)
        {
            fm_radio_connect_audio_decoder(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_comp_rm_audio_decoder_nfy
 *
 * Description: This API is called by the Resource Manager to notify the
 *              handler of changes in the state or condition of the driver
 *              component.
 *
 * Inputs:  h_comp      Contains a handle to the audio decoder component.
 *          ui1_port    Contains the port number.
 *          e_nfy_cond  Contains the notify condition.
 *          pv_tag      Contains the tag value specified in rm_open_comp.
 *          ui4_data    Contains additional data (depends on e_nfy_cond).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_comp_rm_audio_decoder_nfy (
    HANDLE_T     h_comp,
    UINT8        ui1_port,
    RM_COND_T    e_nfy_cond,
    VOID*        pv_tag,
    UINT32       ui4_data)
{
    HANDLE_T     h_ch_conn = (HANDLE_T) pv_tag;
    RM_COND_T    e_grp_cond;

    DBG_INFO((DBG_PREFIX
              "Audio decoder RM nfy 0x%08x (data %d) (%d)\n",
              e_nfy_cond, ui4_data, h_ch_conn));

    e_grp_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE, COND_GRP_OPERATION);

    if (e_grp_cond == RM_COND_IGNORE)
    {
        CRIT_STATE_T        t_state;
        FM_RADIO_CONN_T*    pt_conn;
        HANDLE_TYPE_T       e_type;
        INT32               i4_res;

        t_state = x_crit_start();

        i4_res = handle_get_type_obj(h_ch_conn, & e_type, (VOID**) & pt_conn);

        if ((i4_res == HR_OK) && (e_type == ABHT_CONNECTION))
        {
            RM_COND_T    e_rm_connect_cond;
            RM_COND_T    e_rm_disconnect_cond;
            RM_COND_T    e_rm_cond;
            UINT32       ui4_code;

            e_rm_connect_cond    = pt_conn->e_rm_connect_audio_decoder_cond;
            e_rm_disconnect_cond = pt_conn->e_rm_disconnect_audio_decoder_cond;

            ui4_code = FM_RADIO_MSG_NONE;

            e_rm_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE,
                                    COND_GRP_OBJ_STATE);

            if (e_rm_cond == RM_COND_CLOSED)
            {
                ui4_code = FM_RADIO_MSG_AUDIO_DEC_CLOSED;
            }
            else if (e_rm_connect_cond == RM_COND_IGNORE)
            {
                /*
                  rm_connect has not been called yet - only OPENED is relevant
                */

                switch (e_rm_cond)
                {
                    case RM_COND_OPENED:
                        ui4_code = FM_RADIO_MSG_AUDIO_DEC_OPENED;
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
                            ui4_code = FM_RADIO_MSG_AUDIO_DEC_CONNECTED;
                        }
                        else
                        {
                            ui4_code =
                                FM_RADIO_MSG_AUDIO_DEC_DISCONNECTED;
                        }
                    }
                    break;

                    case RM_COND_DISCONNECTED:
                    {
                        ui4_code = FM_RADIO_MSG_AUDIO_DEC_DISCONNECTED;
                    }
                    break;

                    case RM_COND_CONNECTING:
                    {
                        if (e_rm_disconnect_cond != RM_COND_IGNORE)
                        {
                            ui4_code =
                                FM_RADIO_MSG_AUDIO_DEC_DISCONNECTED;
                        }
                    }
                    break;

                    default:
                        break;
                }
            }

            x_crit_end(t_state);

            if (ui4_code != FM_RADIO_MSG_NONE)
            {
                cm_send_msg_retry(h_fm_radio_msgq,
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
 * Name: fm_radio_open_audio_decoder
 *
 * Description: This API opens the audio decoder driver component.
 *
 * Inputs:  pt_conn    References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_open_audio_decoder (
    FM_RADIO_CONN_T*       pt_conn
)
{
    CM_COMP_RM_SELECT_T    t_select;
    HANDLE_T               h_rm_comp;
    RM_COND_T              e_rm_cond;
    CHAR*                  ps_audio_decoder_name;
    UINT32                 ui4_reason;
    INT32                  i4_ret;

    ui4_reason = FM_RADIO_DISC_OTHER_REASON;

    cm_comp_rm_select_init(&t_select);

    t_select.b_select_if_conflict =
        !pt_conn->t_connect_info.b_disc_if_comp_busy;
    t_select.b_select_if_lower_priority =
        pt_conn->t_connect_info.b_audio_decoder_no_rights_ok;
    t_select.b_would_conflict = FALSE;

    ps_audio_decoder_name = NULL;

    h_rm_comp = pt_conn->h_rm_tuner;

    DBG_INFO((DBG_PREFIX"Opening audio decoder (%s) (%d)...\n",
              (ps_audio_decoder_name != NULL) ?
              ps_audio_decoder_name : "no name",
              pt_conn->h_ch_conn));

    i4_ret = rm_open_comp(h_rm_comp,
                          OBJ_COMP_REL_SRC,
                          DRVT_AUD_DEC,
                          /* component id is always specified to 0 */
                          0,
                          ps_audio_decoder_name,
                          CTRL_TYPE_SHARED,
                          (VOID*)&t_select,                  /* pv_comp_sel_data */
                          cm_comp_rm_select,                 /* pf_comp_sel */
                          (VOID*)pt_conn->h_ch_conn,         /* tag */
                          fm_radio_comp_rm_audio_decoder_nfy,
                          &pt_conn->h_rm_audio_decoder,
                          &e_rm_cond);

    switch (i4_ret)
    {
        case RMR_OK:
        {
            e_rm_cond = rm_cond_chg(e_rm_cond,
                                    RM_COND_IGNORE,
                                    COND_GRP_OBJ_STATE);

            if (e_rm_cond == RM_COND_OPENED)
            {
                fm_radio_audio_decoder_opened(pt_conn);
            }
            else if (t_select.b_would_conflict)
            {
                cm_nfy(pt_conn->h_cm_conn,
                       CM_COND_CONNECTION_INFO,
                       CM_CONN_INFO_COMP_BUSY);
            }

            i4_ret = CMR_OK;
        }
        break;

        case RMR_ASYNC_NFY:
        case RMR_ARBITRATION:
            i4_ret = CMR_OK;
            break;

        case RMR_NO_COMP_FOUND:
        case RMR_INV_NAME:
            ui4_reason = t_select.b_would_conflict ?
                FM_RADIO_DISC_COMP_BUSY :
                FM_RADIO_DISC_NO_AUDIO_DECODER_FOUND;
            i4_ret = CMR_NO_COMP_FOUND;
            break;

        case RMR_OUT_OF_HANDLES:
            i4_ret = CMR_OUT_OF_HANDLES;
            break;

        case RMR_NOT_ENOUGH_MEM:
            i4_ret = CMR_NOT_ENOUGH_MEM;
            break;

        default:
            i4_ret = CMR_FAIL;
    }

    if (i4_ret != CMR_OK)
    {
        DBG_ERROR((DBG_PREFIX"Could not open audio decoder (%d)\n", i4_ret));

        fm_radio_disconnect_conn(pt_conn, ui4_reason);
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_set_tune
 *
 * Description: This API changes the tuner's tune information (frequency, fine
 *              tune, etc).
 *
 * Inputs:  pt_conn  References the connection.
 *          pt_tune  References a structure with the new tune information.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/

VOID fm_radio_set_det_channel (
    FM_RADIO_CONN_T*    pt_conn
)
{
    if (!pt_conn->b_disconnecting)
    {
        if (!pt_conn->b_audio_decoder_connected)
        {
            fm_radio_open_audio_decoder(pt_conn);
        }
        else
        {
            fm_radio_start_channel_detection(pt_conn);
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_set_tune_freq
 *
 * Description: This API changes the tuner's tune information (frequency, fine
 *              tune, etc).
 *
 * Inputs:  pt_conn  References the connection.
 *          pt_tune  References a structure with the new tune information.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/

VOID fm_radio_set_tune_freq (
    FM_RADIO_CONN_T*    pt_conn,
    UINT32              ui4_freq
)
{
    /* update fsrequency */
    pt_conn->t_connect_info.ui4_freq = ui4_freq;

    /* reconnect */
    fm_radio_connect_tuner(pt_conn);
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_tuner_opened
 *
 * Description: This API is called when a component is opened (in the context
 *              of the handler message loop). It will try to connect the
 *              component.
 *
 * Inputs:  pt_conn  References the connection structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_tuner_opened (
    FM_RADIO_CONN_T*    pt_conn
)
{
    if (!pt_conn->b_tuner_opened)
    {
        DBG_INFO((DBG_PREFIX"Tuner component now opened (%d)\n",
                  pt_conn->h_ch_conn));

        pt_conn->b_tuner_opened = TRUE;

        fm_radio_connect_conn(pt_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_get_drv_type_from_del_sys
 *
 * Description: This API returns the driver type depending on the delivery
 *              system.
 *
 * Inputs:  ui4_del_sys  Contains the delivery system.
 *
 * Outputs: -
 *
 * Returns: DRVT_TUNER_CAB_ANA
 *          DRVT_TUNER_SAT_ANA
 *          DRVT_TUNER_TER_ANA
 *          DRVT_UNKNOWN
 ----------------------------------------------------------------------------*/
static DRV_TYPE_T fm_radio_get_drv_type_from_del_sys (
    UINT32        ui4_del_sys
)
{
    DRV_TYPE_T    e_drv_type;

    switch (ui4_del_sys)
    {
        case TUNER_CONN_TYPE_TER_ANA:
            e_drv_type = DRVT_TUNER_TER_ANA;
            break;

        case TUNER_CONN_TYPE_CAB_ANA:
        case TUNER_CONN_TYPE_SAT_ANA:
        default:
            e_drv_type = DRVT_UNKNOWN;
    }

    return e_drv_type;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_comp_rm_tuner_nfy
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
static VOID fm_radio_comp_rm_tuner_nfy (
    HANDLE_T     h_comp,
    UINT8        ui1_port,
    RM_COND_T    e_nfy_cond,
    VOID*        pv_tag,
    UINT32       ui4_data
)
{
    HANDLE_T    h_ch_conn = (HANDLE_T) pv_tag;
    RM_COND_T   e_grp_cond;

    DBG_INFO((DBG_PREFIX"Tuner RM nfy 0x%08x (data %d) (%d)\n",
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
                case RM_GET_TYPE_CONN_ORIG_HANDLE:
                {
                    if (pt_get->z_get_info_len >= sizeof(HANDLE_T))
                    {
                        *((HANDLE_T*) pt_get->pv_get_info) = h_comp;

                        pt_get->z_get_info_len = sizeof(HANDLE_T);

                        pt_get->i4_return = RMR_OK;
                    }
                    else
                    {
                        pt_get->z_get_info_len = sizeof(HANDLE_T);

                        pt_get->i4_return = RMR_NOT_ENOUGH_SPACE;
                    }
                }
                break;

                default:
                    break;
            }
        }
    }
    else if (e_grp_cond == RM_COND_DRV_STATUS_NFY)
    {
        cm_send_msg_retry(h_fm_radio_msgq,
                          FM_RADIO_MSG_TUNER_STATUS,
                          ui4_data, 0,
                          h_ch_conn);
    }
    else if (e_grp_cond == RM_COND_IGNORE)
    {
        CRIT_STATE_T           t_state;
        FM_RADIO_CONN_T*  pt_conn;
        HANDLE_TYPE_T          e_type;
        INT32                  i4_res;

        t_state = x_crit_start();

        i4_res = handle_get_type_obj(h_ch_conn, & e_type, (VOID**) & pt_conn);

        if ((i4_res == HR_OK) && (e_type == ABHT_CONNECTION))
        {
            RM_COND_T    e_rm_connect_cond;
            RM_COND_T    e_rm_disconnect_cond;
            RM_COND_T    e_rm_cond;
            UINT32       ui4_code;

            e_rm_connect_cond    = pt_conn->e_rm_connect_tuner_cond;
            e_rm_disconnect_cond = pt_conn->e_rm_disconnect_tuner_cond;

            ui4_code = FM_RADIO_MSG_NONE;

            e_rm_cond = rm_cond_chg(e_nfy_cond, RM_COND_IGNORE,
                                    COND_GRP_OBJ_STATE);

            if (e_rm_cond == RM_COND_CLOSED)
            {
                ui4_code = FM_RADIO_MSG_TUNER_CLOSED;
            }
            else if (e_rm_connect_cond == RM_COND_IGNORE)
            {
                /*
                  rm_connect has not been called yet - only OPENED is relevant
                */

                switch (e_rm_cond)
                {
                    case RM_COND_OPENED:
                        ui4_code = FM_RADIO_MSG_TUNER_OPENED;
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
                            ui4_code = FM_RADIO_MSG_TUNER_CONNECTED;
                        }
                        else
                        {
                            ui4_code = FM_RADIO_MSG_TUNER_DISCONNECTED;
                        }
                    }
                    break;

                    case RM_COND_DISCONNECTED:
                    {
                        ui4_code = FM_RADIO_MSG_TUNER_DISCONNECTED;
                    }
                    break;

                    case RM_COND_CONNECTING:
                    {
                        if (e_rm_disconnect_cond != RM_COND_IGNORE)
                        {
                            ui4_code = FM_RADIO_MSG_TUNER_DISCONNECTED;
                        }
                    }
                    break;

                    default:
                        break;
                }
            }

            x_crit_end(t_state);

            if (ui4_code != FM_RADIO_MSG_NONE)
            {
                cm_send_msg_retry(h_fm_radio_msgq,
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
 * Name: fm_radio_open_tuner
 *
 * Description: This API opens the tuner driver component.
 *
 * Inputs:  pt_conn    References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_open_tuner (
    FM_RADIO_CONN_T*    pt_conn
)
{
    DRV_TYPE_T          e_drv_type;
    UINT32              ui4_del_sys;
    UINT32              ui4_reason;
    INT32               i4_ret = CMR_OK;

    DBG_INFO((DBG_PREFIX"Opening tuner on connection (%d)...\n",
              pt_conn->h_ch_conn));

    ui4_reason = FM_RADIO_DISC_OTHER_REASON;

    ui4_del_sys = pt_conn->t_connect_info.ui4_del_sys;

    e_drv_type = fm_radio_get_drv_type_from_del_sys(ui4_del_sys);

    if (e_drv_type != DRVT_UNKNOWN)
    {
        CM_COMP_RM_SELECT_T    t_select;
        RM_COND_T              e_rm_cond;
        HANDLE_T               h_rm_pipe;

        h_rm_pipe = cm_pipe_get_rm_pipe(pt_conn->h_cm_pipe);

        cm_comp_rm_select_init(&t_select);

        t_select.b_select_if_conflict =
            !pt_conn->t_connect_info.b_disc_if_comp_busy;
        t_select.b_select_if_lower_priority =
            pt_conn->t_connect_info.b_tuner_no_rights_ok;
        t_select.b_would_conflict = FALSE;

        i4_ret = rm_open_comp(h_rm_pipe,
                              OBJ_COMP_REL_IGNORE,
                              e_drv_type,
                              ANY_COMP_ID,
                              pt_conn->s_src_name,
                              CTRL_TYPE_MASTER_SLAVE,
                              (VOID*)&t_select,           /* pv_comp_sel_data */
                              cm_comp_rm_select,          /* pf_comp_sel */
                              (VOID*) pt_conn->h_ch_conn, /* tag */
                              fm_radio_comp_rm_tuner_nfy,
                              & pt_conn->h_rm_tuner,
                              & e_rm_cond);
        switch (i4_ret)
        {
            case RMR_OK:
            {
                e_rm_cond = rm_cond_chg(e_rm_cond,
                                        RM_COND_IGNORE,
                                        COND_GRP_OBJ_STATE);

                if (e_rm_cond == RM_COND_OPENED)
                {
                    fm_radio_tuner_opened(pt_conn);
                }
                else if (t_select.b_would_conflict)
                {
                    cm_nfy(pt_conn->h_cm_conn,
                           CM_COND_CONNECTION_INFO,
                           CM_CONN_INFO_COMP_BUSY);
                }

                i4_ret = CMR_OK;
            }
            break;

            case RMR_ASYNC_NFY:
            case RMR_ARBITRATION:
                i4_ret = CMR_OK;
                break;

            case RMR_NO_COMP_FOUND:
            case RMR_INV_NAME:
                ui4_reason = t_select.b_would_conflict ?
                    FM_RADIO_DISC_COMP_BUSY :
                    FM_RADIO_DISC_NO_TUNER_FOUND;
                i4_ret = CMR_NO_COMP_FOUND;
                break;

            case RMR_OUT_OF_HANDLES:
                i4_ret = CMR_OUT_OF_HANDLES;
                break;

            case RMR_NOT_ENOUGH_MEM:
                i4_ret = CMR_NOT_ENOUGH_MEM;
                break;

            default:
                i4_ret = CMR_FAIL;
        }

        if (i4_ret != CMR_OK)
        {
            DBG_ERROR((DBG_PREFIX"Could not open tuner (%d)\n", i4_ret));

            fm_radio_disconnect_conn(pt_conn, ui4_reason);
        }
    }
    else
    {
        fm_radio_disconnect_conn(pt_conn,
                                 FM_RADIO_DISC_NO_TUNER_FOUND);
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_open_conn
 *
 * Description: This API starts the actual connection process by opening the 2
 *              components (tuner and TV decoder).
 *
 * Inputs:  pt_conn    References the connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_open_conn (
    FM_RADIO_CONN_T*  pt_conn
)
{
    if (!pt_conn->b_disconnecting)
    {
        fm_radio_open_tuner(pt_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_conn_free_handle
 *
 * Description: This API is called when an analog broadcast connection handle
 *              is freed.
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
static BOOL fm_radio_conn_free_handle (
    HANDLE_T         h_ch_conn,
    HANDLE_TYPE_T    e_type,
    VOID*            pv_obj,
    VOID*            pv_tag,
    BOOL             b_req_handle
)
{
    BOOL             b_ret;

    DBG_INFO((DBG_PREFIX"Deleting connection handle %d.\n",
              h_ch_conn));

    if (!b_req_handle)
    {
        FM_RADIO_CONN_T*    pt_conn;
        INT32               i4_ret;

        pt_conn = (FM_RADIO_CONN_T*) pv_obj;

        /* delete SCDB */
        if (pt_conn->h_scdb != NULL_HANDLE)
        {
            DBG_INFO((DBG_PREFIX"Deleting SCDB %d\n", pt_conn->h_scdb));

            i4_ret = scdb_delete(pt_conn->h_scdb);

            if (i4_ret != SCDBR_OK)
            {
                DBG_ERROR((DBG_PREFIX"Deleting SCDB %d failed",
                           pt_conn->h_scdb));
            }
        }
        else
        {
            DBG_INFO((DBG_PREFIX"null SCDB, nothing to free\n"));
        }

        /* closes SVL handle */
        if (pt_conn->h_svl != NULL_HANDLE)
        {
            DBG_INFO((DBG_PREFIX"Closing SVL handle\n"));

            i4_ret = x_svl_close(pt_conn->h_svl);

            if (i4_ret != SVLR_OK)
            {
                DBG_ERROR((DBG_PREFIX"Closing SVL handle %d failed",
                           pt_conn->h_svl));
            }
        }

        if (pt_conn->pui4_nb_connections != NULL)
        {
            (*pt_conn->pui4_nb_connections)--;

            DBG_INFO((DBG_PREFIX"Handler now has %d connection(s).\n",
                      (*pt_conn->pui4_nb_connections)));
        }

        pt_conn->ui4_magic = ~ FM_RADIO_CONN_MAGIC;

        x_mem_free(pv_obj);

        b_ret = TRUE;
    }
    else
    {
        DBG_INFO((DBG_PREFIX"Not allowed to delete handle\n"));
        b_ret = FALSE;
    }

    return b_ret;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_svl_nfy
 *
 * Description: This API is called when the SVL is modified (or closed) and may
 *              send a message to the analog broadcast handler message loop.
 *
 * Inputs:  h_svl       Contains the SVL handle.
 *          e_cond      Contains the new condition.
 *          ui4_reason  Specifies the reasong for the new condition.
 *          pv_tag      Contains the tag value passed when the SVL was opened.
 *          ui4_data    Contains additional data.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_svl_nfy (
    HANDLE_T      h_svl,
    SVL_COND_T    e_cond,
    UINT32        ui4_reason,
    VOID*         pv_tag,
    UINT32        ui4_data
)
{
    UINT32  e_code;

    switch (e_cond)
    {
        case SVL_UPDATED:
            e_code = FM_RADIO_MSG_SVL_UPDATED;
            break;

        case SVL_CLOSED:
            e_code = FM_RADIO_MSG_SVL_CLOSED;
            break;

        default:
            e_code = FM_RADIO_MSG_NONE;
            break;
    }

    if (e_code != FM_RADIO_MSG_NONE)
    {
        HANDLE_T  h_ch_conn;

        h_ch_conn = (HANDLE_T) pv_tag;

        cm_send_msg_no_retry(h_fm_radio_msgq,
                             e_code, ui4_data, 0,
                             h_ch_conn);
    }
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_open_svl
 *
 * Description: This API opens an SVL either by ID or by name.
 *
 * Inputs:  pt_conn          References an analog broadcast connection.
 *          pt_tsl_svl_info  References a structure with information about the
 *                           SVL to open.
 *          ui4_flags        Contains the connection flags.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
static INT32 fm_radio_open_svl (
    FM_RADIO_CONN_T*              pt_conn,
    const FM_RADIO_SVL_INFO_T*    pt_tsl_svl_info,
    UINT32                        ui4_flags
)
{
    INT32                         i4_ret;

    if (ui4_flags & FM_RADIO_SVC_LST_ID_MASK)
    {
        DBG_INFO((DBG_PREFIX"Opening SVL (svl_id=%d).\n",
                  pt_tsl_svl_info->ui2_svl_id));

        i4_ret = x_svl_open(pt_tsl_svl_info->ui2_svl_id,
                            (VOID*) pt_conn->h_ch_conn, /* tag */
                            fm_radio_svl_nfy,
                            &pt_conn->h_svl);
    }
    else
    {
        DBG_INFO((DBG_PREFIX"Opening SVL (svl_name=\"%s\").\n",
                  pt_tsl_svl_info->ps_svl_name));

        i4_ret = x_svl_open_by_name(pt_tsl_svl_info->ps_svl_name,
                                    (VOID*) pt_conn->h_ch_conn, /* tag */
                                    fm_radio_svl_nfy,
                                    &pt_conn->h_svl);
    }

    i4_ret = (i4_ret == SVLR_OK) ? CMR_OK : CMR_FAIL;

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_get_info_from_svl
 *
 * Description: This API retrieves information in the SVL database about the
 *              connection.
 *
 * Inputs:  pt_conn  References a broadcast connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
static INT32 fm_radio_get_info_from_svl (
    FM_RADIO_CONN_T*    pt_conn
)
{
    SVL_REC_T           t_svl_rec;
    UINT32              ui4_svl_version;
    INT32               i4_ret;

    ui4_svl_version = SVL_NULL_VER_ID;

    DBG_INFO((DBG_PREFIX"Getting SVL record (rec_id=%d).\n",
              pt_conn->t_connect_info.ui2_svl_rec_id));

    i4_ret = x_svl_get_rec(pt_conn->h_svl,
                           pt_conn->t_connect_info.ui2_svl_rec_id,
                           &t_svl_rec,
                           &ui4_svl_version);

    if (i4_ret == SVLR_OK)
    {
        FM_RADIO_CONNECT_INFO_T*    pt_info = &pt_conn->t_connect_info;

        /* freq is channel ID */
        pt_info->ui4_freq = t_svl_rec.uheader.t_rec_hdr.ui4_channel_id;

        i4_ret = CMR_OK;
    }
    else
    {
        DBG_ERROR((DBG_PREFIX"Could not get SVL record (%d).\n",
                   i4_ret));

        i4_ret = CMR_FAIL;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_svl_updated
 *
 * Description: This API is called when the connection handler receives a
 *              notification the SVL was updated.
 *              The "SCART bypass" property may be changed because the decoder
 *              may or may not be needed anymore.
 *
 * Inputs:  pt_conn  References a broadcast connection.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_svl_updated (
    FM_RADIO_CONN_T*      pt_conn
)
{
    /* do nothing */
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_disconnect
 *
 * Description: This API is called by the Connection Manager to perform a
 *              disconnection.
 *
 * Inputs:  h_ch_conn  Contains a handle to a broadcast handler's connection.
 *
 * Outputs: pe_cond  Contains the connection condition (DISCONNECTING, etc).
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  Disconnection failed.
 ----------------------------------------------------------------------------*/
static INT32 fm_radio_disconnect (
    HANDLE_T      h_ch_conn,
    CM_COND_T*    pe_cond
)
{
    INT32  i4_ret;

    i4_ret = cm_send_msg_retry(h_fm_radio_msgq,
                               FM_RADIO_MSG_DISCONNECT_REQUEST,
                               FM_RADIO_DISC_AS_REQUESTED, 0,
                               h_ch_conn);

    if (i4_ret == CMR_OK)
    {
        *pe_cond = CM_COND_DISCONNECTING;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_connect
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
static INT32 fm_radio_connect (
    HANDLE_T                   h_cm_conn,
    HANDLE_T*                  ph_ch_conn,
    HANDLE_T                   h_parent_ch_conn,
    const CM_COMMAND_T*        pt_commands,
    CM_COND_T*                 pe_cond
)
{
    FM_RADIO_SVL_INFO_T        t_svl_info;
    FM_RADIO_CONNECT_INFO_T    t_connect_info;
    UINT32                     ui4_flags;
    INT32                      i4_ret;

    DBG_INFO((DBG_PREFIX"Connecting... (CM connection %d)\n",
              h_cm_conn));

    x_memset(&t_svl_info, 0, sizeof(t_svl_info));
    x_memset(&t_connect_info, 0, sizeof(t_connect_info));

    t_connect_info.ui4_aud_sys_mask            = AUDIO_SYS_MASK_FM_RADIO;
    t_connect_info.e_pol                       = POL_UNKNOWN;
    t_connect_info.e_mod                       = MOD_FM_RADIO;
    t_connect_info.b_disc_if_comp_busy         = TRUE;
    t_connect_info.b_disc_if_tuner_signal_lost = TRUE;
    t_connect_info.ui2_dir                     = INV_DIRECTION;
    t_connect_info.ui2_gain                    = INV_GAIN;
    t_connect_info.ui4_del_sys                 = TUNER_CONN_TYPE_TER_ANA;

    ui4_flags = 0;

    if (pt_commands != NULL)
    {
        i4_ret = fm_radio_parse_commands(&t_svl_info,
                                         &t_connect_info,
                                         &ui4_flags,
                                         pt_commands);
    }
    else
    {
        i4_ret = CMR_INV_ARG;
    }

    if (i4_ret == CMR_OK)
    {
        HANDLE_T            h_ch_conn;
        FM_RADIO_CONN_T*    pt_conn = NULL;

        i4_ret = handle_alloc_and_obj(ABHT_CONNECTION,
                                      NULL,
                                      sizeof(FM_RADIO_CONN_T),
                                      fm_radio_conn_free_handle,
                                      &h_ch_conn,
                                      (VOID**)&pt_conn);

        if ((i4_ret == HR_OK) && (pt_conn != NULL))
        {
            DBG_INFO((DBG_PREFIX"Created connection %d (CM connection=%d)\n",
                      h_ch_conn, h_cm_conn));

            cm_connection_get_src_name(h_cm_conn,
                                       pt_conn->s_src_name);

            pt_conn->t_connect_info = t_connect_info;

            pt_conn->ui4_disconnection_reason =
                FM_RADIO_DISC_OTHER_REASON;

            pt_conn->h_cm_pipe = cm_connection_get_pipe(h_cm_conn);
            pt_conn->h_cm_conn = h_cm_conn;
            pt_conn->h_ch_conn = h_ch_conn;

            pt_conn->ui4_flags = ui4_flags;

            pt_conn->ui4_magic = FM_RADIO_CONN_MAGIC;

            if (ui4_flags
                & (FM_RADIO_SVC_LST_ID_MASK | FM_RADIO_SVC_LST_NAME_MASK))
            {
                i4_ret = fm_radio_open_svl(pt_conn,
                                           &t_svl_info,
                                           ui4_flags);

                if (i4_ret == CMR_OK)
                {
                    i4_ret = fm_radio_get_info_from_svl(pt_conn);
                }
            }

            if (!(ui4_flags & FM_RADIO_DELIVERY_SYS_MASK))
            {
                t_connect_info.ui4_del_sys = TUNER_CONN_TYPE_TER_ANA;
            }

            if (i4_ret == CMR_OK)
            {
                i4_ret = fm_radio_create_scdb(pt_conn);

                if (i4_ret == CMR_OK)
                {
                    i4_ret = fm_radio_fill_default_scdb(pt_conn);
                }
            }

            if (i4_ret == CMR_OK)
            {
                i4_ret = cm_send_msg_retry(h_fm_radio_msgq,
                                           FM_RADIO_MSG_CONNECT_REQUEST,
                                           0, 0,
                                           h_ch_conn);

                if (i4_ret == CMR_OK)
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
            i4_ret = CMR_FAIL;
        }
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_get
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
static INT32 fm_radio_get (
    HANDLE_T          h_ch_conn,
    CM_CTRL_TYPE_T    e_ctrl_type,
    VOID*             pv_ctrl_data
)
{
    INT32  i4_ret;

    i4_ret = CMR_OK;

    switch (e_ctrl_type)
    {
        case CM_CTRL_RM_SRC_COMP:
        {
            FM_RADIO_CONN_T*    pt_conn;
            CRIT_STATE_T        t_state;

            t_state = x_crit_start();

            if (handle_get_obj(h_ch_conn, (VOID**)&pt_conn) == HR_OK)
            {
                VALIDATE_CONN(pt_conn);

                ((CM_CTRL_RM_SRC_COMP_T*)pv_ctrl_data)->h_rm_comp =
                    pt_conn->h_rm_tuner;
            }
            else
            {
                i4_ret = CMR_FAIL;
            }

            x_crit_end(t_state);
        }
        break;


        default:
            i4_ret = CMR_INV_ARG;
            break;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_set
 *
 * Description: This API sets information about a connection.
 *
 * Inputs:  h_conn        Contains a handle to the handler's connection.
 *          e_ctrl_type   Contains the control code.
 *
 * Outputs: pv_ctrl_data  Contains information about the connection.
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  "get" operation failed.
 ----------------------------------------------------------------------------*/
static INT32 fm_radio_set (
    HANDLE_T          h_ch_conn,
    CM_CTRL_TYPE_T    e_ctrl_type,
    VOID*             pv_ctrl_data
)
{
    INT32            i4_ret;

    i4_ret = CMR_OK;

    switch (e_ctrl_type)
    {
        case FM_RADIO_CTRL_SET_DET_CHANNEL:
        {
            i4_ret = cm_send_msg_retry(h_fm_radio_msgq,
                                       FM_RADIO_MSG_SET_DET_CHANNEL,
                                       0,
                                       0,
                                       h_ch_conn);

            i4_ret = (i4_ret == CMR_OK) ? CMR_ASYNC_NFY : CMR_FAIL;
        }
        break;

        case FM_RADIO_CTRL_SET_TUNE_FREQ:
        {
            i4_ret = cm_send_msg_retry(h_fm_radio_msgq,
                                       FM_RADIO_MSG_SET_TUNE_FREQ,
                                       (UINT32)pv_ctrl_data,
                                       0,
                                       h_ch_conn);

            i4_ret = (i4_ret == CMR_OK) ? CMR_ASYNC_NFY : CMR_FAIL;
        }
        break;

        default:
            i4_ret = CMR_INV_ARG;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_thread_main
 *
 * Description: This API is the FM Radio Connection Handler's thread
 *              entry point.
 *
 * Inputs:  pv_arg  References the analog broadcast handler information.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID fm_radio_thread_main (
    VOID*                       pv_arg
)
{
    FM_RADIO_HANDLER_INFO_T*    pt_info;
    UINT32                      ui4_nb_connections;
    BOOL                        b_exit_when_done;
    BOOL                        b_exit_now;

    pt_info = (FM_RADIO_HANDLER_INFO_T*)pv_arg;

    ui4_nb_connections = 0;

    b_exit_when_done = FALSE;
    b_exit_now = FALSE;

    while (!b_exit_now)
    {
        FM_RADIO_CONN_T*    pt_conn;
        CM_MSG_T            t_msg;
        SIZE_T              z_msg_size;
        UINT16              ui2_index;

        z_msg_size = sizeof(CM_MSG_T);

        if (x_msg_q_receive(&ui2_index,
                            &t_msg,
                            &z_msg_size,
                            &pt_info->h_fm_radio_msgq, 1,
                            X_MSGQ_OPTION_WAIT)
            != OSR_OK)
        {
            DBG_ERROR((DBG_PREFIX"x_msg_q_receive failed\n"));

            /* failure is ignored */
            continue;
        }

        if ((t_msg.h_obj != NULL_HANDLE) && !x_handle_valid(t_msg.h_obj))
        {
            DBG_INFO((DBG_PREFIX"non-NULL invalid handle %d (code=%d)\n",
                      t_msg.h_obj, t_msg.ui4_code));

            /* messages with invalid non-NULL handle are ignored */
            continue;
        }

        if (t_msg.h_obj != NULL_HANDLE)
        {
            if (handle_get_obj(t_msg.h_obj, (VOID**)&pt_conn) != HR_OK)
            {
                DBG_ERROR((DBG_PREFIX"handle_get_obj failed\n"));

                continue;
            }
            VALIDATE_CONN(pt_conn);
        }
        else
        {
            pt_conn = NULL;

            continue;
        }

        switch (t_msg.ui4_code)
        {
            case FM_RADIO_MSG_CONNECT_REQUEST:
            {
                DBG_INFO((DBG_PREFIX"Connection request (%d, %p)\n",
                          t_msg.h_obj, pt_conn));

                ui4_nb_connections++;

                DBG_INFO((DBG_PREFIX"Handler has %d connection(s)\n",
                          ui4_nb_connections));

                pt_conn->pui4_nb_connections = &ui4_nb_connections;

                fm_radio_open_conn(pt_conn);
            }
            break;

            case FM_RADIO_MSG_DISCONNECT_REQUEST:
            {
                DBG_INFO((DBG_PREFIX"Disconnection request (%d, %p)\n",
                          t_msg.h_obj, pt_conn));

                fm_radio_disconnect_conn(pt_conn,
                                         FM_RADIO_DISC_AS_REQUESTED);
            }
            break;

            case FM_RADIO_MSG_TUNER_OPENED:
            {
                fm_radio_tuner_opened(pt_conn);
            }
            break;

            case FM_RADIO_MSG_TUNER_CLOSED:
            {
                fm_radio_tuner_closed(pt_conn);

                if (!pt_conn->b_disconnecting)
                {
                    fm_radio_disconnect_conn(
                        pt_conn,
                        FM_RADIO_DISC_OTHER_REASON);
                }
            }
            break;

            case FM_RADIO_MSG_TUNER_CONNECTED:
            {
                fm_radio_tuner_connected(pt_conn);
            }
            break;

            case FM_RADIO_MSG_TUNER_DISCONNECTED:
            {
                DBG_INFO((DBG_PREFIX"Tuner disconnected asynchronously (%d)\n",
                          pt_conn->h_ch_conn));

                if (!pt_conn->b_disconnecting
                    && pt_conn->h_rm_audio_decoder != NULL_HANDLE)
                {
                    pt_conn->b_disconnecting = TRUE;
                    fm_radio_disconnect_audio_decoder(pt_conn);
                }

                fm_radio_tuner_disconnected(pt_conn, t_msg.ui4_data_1);
            }
            break;

            case FM_RADIO_MSG_TUNER_STATUS:
            {
                TUNER_COND_STATUS_T    e_status;

                e_status = (TUNER_COND_STATUS_T) t_msg.ui4_data_1;
                fm_radio_tuner_status(pt_conn, e_status);
            }
            break;

            case FM_RADIO_MSG_AUDIO_DEC_OPENED:
            {
                fm_radio_audio_decoder_opened(pt_conn);
            }
            break;

            case FM_RADIO_MSG_AUDIO_DEC_CLOSED:
            {
                fm_radio_audio_decoder_closed(pt_conn);

                if (!pt_conn->b_disconnecting)
                {
                    fm_radio_disconnect_conn(
                        pt_conn,
                        FM_RADIO_DISC_OTHER_REASON);
                }
            }
            break;

            case FM_RADIO_MSG_AUDIO_DEC_CONNECTED:
            {
                fm_radio_audio_decoder_connected(pt_conn);
            }
            break;

            case FM_RADIO_MSG_AUDIO_DEC_DISCONNECTED:
            {
                DBG_INFO((DBG_PREFIX
                          "Audio decoder disconnected asynchronously (%d)\n",
                          pt_conn->h_ch_conn));

                if (!pt_conn->b_disconnecting)
                {
                    pt_conn->b_disconnecting = TRUE;

                    fm_radio_disconnect_tuner(pt_conn);
                }

                fm_radio_audio_decoder_disconnected(pt_conn);
            }
            break;

            case FM_RADIO_MSG_SVL_UPDATED:
            {
                DBG_INFO((DBG_PREFIX"SVL updated\n"));

                fm_radio_svl_updated(pt_conn);
            }
            break;

            case FM_RADIO_MSG_SVL_CLOSED:
            {
                DBG_INFO((DBG_PREFIX"SVL closed: disconnecting\n"));

                fm_radio_disconnect_conn(pt_conn,
                                         FM_RADIO_DISC_SVL_CLOSED);
            }
            break;

            case FM_RADIO_MSG_AUDIO_DEC_PLAY_DONE:
            {
                fm_radio_audio_decoder_playing(pt_conn);
            }
            break;

            case FM_RADIO_MSG_AUDIO_DEC_STOP_DONE:
            {
                fm_radio_audio_decoder_stopped(pt_conn);
            }
            break;

            case FM_RADIO_MSG_AUDIO_DEC_DETECT_DONE:
            {
                fm_radio_audio_decoder_detect_done(pt_conn,
                                                   t_msg.ui4_data_1,
                                                   t_msg.ui4_data_2);
            }
            break;

            case FM_RADIO_MSG_SET_DET_CHANNEL:
            {
                fm_radio_set_det_channel(pt_conn);
            }
            break;

            case FM_RADIO_MSG_SET_TUNE_FREQ:
            {
                fm_radio_set_tune_freq(pt_conn,
                                       t_msg.ui4_data_1);
            }
            break;

            case FM_RADIO_MSG_EXIT:
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
    x_msg_q_delete(pt_info->h_fm_radio_msgq);

    h_fm_radio_msgq = NULL_HANDLE;
}

/*-----------------------------------------------------------------------------
 * Name: fm_radio_set_init
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
static BOOL fm_radio_set_init (
    BOOL             b_flag,
    UINT16*          pui2_gen
)
{
    static UINT16    ui2_gen = 0;
    CRIT_STATE_T     t_state;
    BOOL             b_ret;

    t_state = x_crit_start();

    b_ret  = b_init;
    b_init = b_flag;

    h_fm_radio_handler = NULL_HANDLE;

    if (b_flag)
    {
         /* generation number in the range 0-65536 */
        *pui2_gen = ui2_gen++;
    }
    x_crit_end(t_state);

    return b_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_radio_reg_handler
 *
 * Description: This API registers the FM Radio Connection Handler.
 *
 * Inputs:  ps_names         References a string that contains all the names
 *                           for the FM Radio Connection Handler.
 *          pt_thread_descr  References information about the thread to create.
 *          pt_time_outs     References the time-outs values.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  The connection handler is not registered.
 ----------------------------------------------------------------------------*/
INT32 x_fm_radio_reg_handler (
    const CHAR*                   ps_name,
    const THREAD_DESCR_T *        pt_thread_descr
)
{
    INT32                         i4_ret = CMR_NOT_INIT;
    UINT16                        ui2_gen;
    CM_HANDLER_FCT_TBL_T          t_fm_radio_fct_tbl =
    {
        fm_radio_connect,
        fm_radio_disconnect,
        fm_radio_get,
        fm_radio_set
    };

    if (!fm_radio_set_init(TRUE, &ui2_gen))
    {
        SIZE_T    z_stack_size;
        UINT8     ui1_priority;
        CHAR      s_name[SYS_NAME_LEN + 1];

        if (ps_name == NULL)
        {
            ps_name = FM_RADIO_HANDLER_DEFAULT_NAME;
        }

        z_stack_size = FM_RADIO_HANDLER_DEFAULT_STACK_SIZE;
        ui1_priority = FM_RADIO_HANDLER_DEFAULT_PRIORITY;

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

        x_sprintf(s_name, "fm_radio-%d", ui2_gen);

        if (x_msg_q_create(&h_fm_radio_msgq,
                           s_name,
                           sizeof(CM_MSG_T),
                           FM_RADIO_HANDLER_MSG_QUEUE_SIZE)
            == OSR_OK)
        {
            FM_RADIO_HANDLER_INFO_T    t_info;

            t_info.h_fm_radio_msgq = h_fm_radio_msgq;

            if (x_thread_create(&t_info.h_fm_radio_thread,
                                s_name,
                                z_stack_size,
                                ui1_priority,
                                fm_radio_thread_main,
                                sizeof(t_info),
                                (VOID*)&t_info)
                == OSR_OK)
            {
                i4_ret = cm_reg_handler(ps_name,
                                        &t_fm_radio_fct_tbl,
                                        &h_fm_radio_handler);

                if (i4_ret == CMR_OK)
                {
                    fm_radio_cli_init();
                }
                else
                {
                    /* exit thread */
                    cm_send_msg_retry(h_fm_radio_msgq,
                                      FM_RADIO_MSG_EXIT, 0, 0,
                                      NULL_HANDLE);

                    i4_ret = CMR_NOT_INIT;
                }
            }
            else
            {
                x_msg_q_delete(h_fm_radio_msgq);
            }
        }

        /* if something failed... */
        if (i4_ret != CMR_OK)
        {
            (VOID)fm_radio_set_init(FALSE, NULL);
        }
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_radio_unreg_handler
 *
 * Description: This API unregisters the FM Radio Connection Handler.
 *
 * Inputs:  b_force  Contains a boolean value that specifies whether or not the
 *                   connections maintained by this handler should be closed.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  The connection handler is not unregistered.
 ----------------------------------------------------------------------------*/
INT32 x_fm_radio_unreg_handler (
    BOOL        b_force
)
{
    HANDLE_T    h_handler, h_msgq;
    INT32       i4_ret;

    h_handler = h_fm_radio_handler;
    h_msgq    = h_fm_radio_msgq;

    if (fm_radio_set_init(FALSE, NULL))
    {
        i4_ret = cm_unreg_handler(h_handler, b_force);

        if (i4_ret == CMR_OK)
        {
            i4_ret = cm_send_msg_retry(h_msgq,
                                       FM_RADIO_MSG_EXIT, 0, 0,
                                       NULL_HANDLE);
        }
        else
        {
            i4_ret = CMR_FAIL;
        }
    }
    else
    {
        i4_ret = CMR_NOT_INIT;
    }

    return i4_ret;
}

