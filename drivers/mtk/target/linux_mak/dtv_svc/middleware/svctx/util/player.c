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
 * $RCSfile: player.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/46 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: f0b308e28852e901dc8386943dc577db $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "svctx/svctx_dbg.h"
#include "time_msrt/x_time_msrt.h"
#include "svctx/util/player.h"
#include "svctx/svctx.h"
#include "strm_mngr/sess_gnrc_hdlr/u_sm_sess_gnrc.h"
#include "svctx/svctx_cli.h"
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define _STOP_CONN                          FALSE
#define _RECONNECT                          TRUE

#define PLAYER_TMS_PIPE_OPEN                "PIPE_OPEN"
#define PLAYER_TMS_PIPE_CLOSE               "PIPE_CLOSE"
#define PLAYER_TMS_CM_CONN                  "CM_CONN"
#define PLAYER_TMS_CM_DISCONN               "CM_DISCONN"
#define PLAYER_TMS_SCDB_OPEN                "SCDB_OPEN"
#define PLAYER_TMS_SCDB_CLOSE               "SCDB_CLOSE"
#define PLAYER_TMS_SESS_OPEN                "SESS_OPEN"
#define PLAYER_TMS_SESS_START               "SESS_START"
#define PLAYER_TMS_SESS_CLOSE               "SESS_CLOSE"

#ifndef MW_TV_AV_BYPASS_SUPPORT
#define _PLAYER_MSG_PIPE_NTFY               ((UINT8)200)
#define _PLAYER_MSG_CONN_NTFY               ((UINT8)201)
#define _PLAYER_MSG_SCDB_NTFY               ((UINT8)202)
#define _PLAYER_MSG_SESS_NTFY               ((UINT8)203)
#define _PLAYER_MSG_RECONN_DELAY_NTFY       ((UINT8)204)
#define _PLAYER_MSG_CONNECT_TIMEOUT_NTFY    ((UINT8)205)
#endif

#undef  _INFO_HEADER
#undef  _API_HEADER
#undef  _ERROR_HEADER
#define _INFO_HEADER        "{SVCTX:PLAYER}Info: "
#define _API_HEADER         "{SVCTX:PLAYER}API:  "
#define _ERROR_HEADER       "{SVCTX:PLAYER}Err:  "

#if 0
#undef  DBG_ERROR
#define DBG_ERROR(_stmt)    x_dbg_stmt _stmt
#endif
#if 0
#undef  DBG_API
#define DBG_API(_stmt)      x_dbg_stmt _stmt
#endif
#if 0
#undef  DBG_INFO
#define DBG_INFO(_stmt)     x_dbg_stmt _stmt
#endif
/*-----------------------------------------------------------------------------
 * SVCTX private methods declarations
 *---------------------------------------------------------------------------*/
/* pipe */
static INT32 _player_open_pipe(
                    PLAYER_T*                   pt_player
                    );
static INT32 _player_close_pipe(
                    PLAYER_T*                   pt_player
                    );
static INT32 _player_handle_pipe_msg (
                    PLAYER_T*                   pt_player,
                    HANDLE_T                    h_pipe,
                    CM_COND_T                   e_pipe_cond,
                    UINT32                      ui4_data
                    );
static VOID _player_pipe_nfy_fct(
                    HANDLE_T                    h_pipe,
                    CM_COND_T                   e_pipe_cond,
                    VOID*                       pv_nfy_tag,
                    UINT32                      ui4_data
                    );

/* conn */
static INT32 _player_open_conn(
                    PLAYER_T*                   pt_player
                    );
static INT32 _player_close_conn(
                    PLAYER_T*                   pt_player
                    );
static VOID _player_conn_nfy_fct(
                    HANDLE_T                    h_conn,
                    CM_COND_T                   e_conn_cond,
                    VOID*                       pv_nfy_tag,
                    UINT32                      ui4_data
                    );
static INT32 _player_handle_reconnect_timeout (
                    PLAYER_T*                   pt_player,
                    HANDLE_T                    h_timer
                    );
static INT32 _player_handle_connect_timeout (
                    PLAYER_T*                   pt_player,
                    HANDLE_T                    h_timer
                    );
static INT32 _player_reconnect(
                    PLAYER_T*                   pt_player
                    );
static VOID _player_reconnect_nfy_fct(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    );
static VOID _player_connect_timeout_nfy_fct(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    );

/* scdb */
static INT32 _player_open_scdb(
                    PLAYER_T*                   pt_player
                    );
static INT32 _player_close_scdb(
                    PLAYER_T*                   pt_player
                    );
static VOID _player_scdb_nfy_fct(
                    HANDLE_T                    h_scdb,
                    SCDB_COND_T                 e_scdb_cond,
                    UINT32                      ui4_reason,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    );
/* sess */
static INT32 _player_open_sess(
                    PLAYER_T*                   pt_player
                    );
static INT32 _player_close_sess(
                    PLAYER_T*                   pt_player
                    );
static INT32 _player_play_sess (
                    PLAYER_T*                   pt_player
                    );
static INT32 _player_sess_nfy_fct(
                    HANDLE_T                    h_sess,
                    SM_SESS_COND_T              e_sess_cond,
                    VOID*                       pv_sess_tag,
                    SM_EVN_TYPE_T               e_event,
                    UINT32                      ui4_data
                    );
/* message handler */
static INT32 _player_internal_msg (
                    VOID*                       pv_tag,
                    SVCTX_MSG_T*                pt_msg
                    );
/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
/* service request pool */
static DLIST_T(_PLAYER_T)   t_g_using_player_q;
static DLIST_T(_PLAYER_T)   t_g_pooled_player_q;
static PLAYER_T*            pt_g_player_objs    = NULL;
static BOOL                 b_g_init            = FALSE;
/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: player_init
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *-----------------------------------------------------------------------------*/
INT32 player_init(
                    UINT16                      ui2_amount
                    )
{
    UINT16 ui2_idx;

    DBG_API((_API_HEADER"%s("
                    "ui2_amount=%d, "
                    "each size=%d)\r\n",
                    __func__,
                    ui2_amount,
                    sizeof(PLAYER_T)
                    ));

    if (b_g_init == TRUE) {
        return SVCTXR_OK;
    }

    pt_g_player_objs = (PLAYER_T*) x_mem_alloc(sizeof(PLAYER_T) * ui2_amount);
    if(pt_g_player_objs == NULL) {
        DBG_ERROR((_ERROR_HEADER"create memory for PLAYER_T pool failed\r\n"));
        return SVCTXR_FAIL;
    }

    x_memset(pt_g_player_objs, 0, sizeof(PLAYER_T) * ui2_amount);

    DLIST_INIT(&t_g_using_player_q);
    DLIST_INIT(&t_g_pooled_player_q);

    for (ui2_idx = 0; ui2_idx < ui2_amount; ui2_idx++) {
        PLAYER_T* pt_player = &(pt_g_player_objs[ui2_idx]);

        DLIST_INSERT_HEAD(pt_player, &t_g_pooled_player_q, t_dlink);
    }

    b_g_init = TRUE;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: player_deinit
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *-----------------------------------------------------------------------------*/
VOID player_deinit(
                    VOID
                    )
{
    if (b_g_init == FALSE) {
        return;
    }

    x_mem_free(pt_g_player_objs);

    DLIST_INIT(&t_g_using_player_q);
    DLIST_INIT(&t_g_pooled_player_q);

    pt_g_player_objs = NULL;
    b_g_init         = FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *     player_open
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 player_open (
                    const CHAR*                 ps_snk_grp_name,
                    UINT32                      ui4_reconnect_delay,
                    player_nty_fct              pf_nty,
                    VOID*                       pv_tag,
#ifdef MW_TV_AV_BYPASS_SUPPORT
                    INT8                        i1_prio_diff,
#endif
                    PLAYER_T**                  ppt_player
                    )
{
    PLAYER_T* pt_player = NULL;
    INT32     i4_ret;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    INT32     i4_tmp;
#endif

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pf_nty == NULL || ppt_player == NULL || x_strlen(ps_snk_grp_name) == 0) {
        return SVCTXR_INV_ARG;
    }

    do {
        /* check the pool is empty or not */
        pt_player = DLIST_HEAD(&t_g_pooled_player_q);
        if (pt_player == NULL) {
            DBG_ERROR((_ERROR_HEADER"%s() failed! ret=SVCTXR_NO_RESOURCE\r\n", __func__));
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }

        /* move the instance from pooled to used */
        DLIST_REMOVE(pt_player, &t_g_pooled_player_q, t_dlink);
        DLIST_INSERT_HEAD(pt_player, &t_g_using_player_q, t_dlink);

        /* init */
        pt_player->h_pipe              = NULL_HANDLE;
        pt_player->h_conn              = NULL_HANDLE;
        pt_player->h_scdb              = NULL_HANDLE;
        pt_player->h_sess              = NULL_HANDLE;
        pt_player->e_pipe_cond         = CM_COND_CLOSED;
        pt_player->e_conn_cond         = CM_COND_DISCONNECTED;
        pt_player->e_scdb_cond         = SCDB_CLOSED;
        pt_player->e_sess_cond         = SM_SESS_COND_CLOSED;
        pt_player->e_snk_oper_type     = SNK_OPER_TYPE_NONE;
        pt_player->e_src_oper_type     = SRC_OPER_TYPE_NONE;
        pt_player->i4_speed            = 0;
        pt_player->ui4_reconnect_delay = ui4_reconnect_delay;
        pt_player->pf_nty              = pf_nty;
        pt_player->pv_tag              = pv_tag;
        pt_player->b_reuse_pipe        = FALSE;
        pt_player->pv_sess_data        = NULL;
        pt_player->z_sess_data_size    = 0;
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_player->ui1_act_queued      = 0;
        pt_player->e_action            = PLAYER_ACTION_NONE;
#endif
        x_strcpy(pt_player->s_snk_grp_name, ps_snk_grp_name);

        if (x_handle_valid(pt_player->h_connect_timer) == FALSE) {
            i4_ret = x_timer_create(&pt_player->h_connect_timer);
            if (i4_ret != OSR_OK) {
                DBG_ERROR((_ERROR_HEADER"create h_connect_timer failed! ret=%d\r\n", i4_ret));
                i4_ret = SVCTXR_FAIL;
                break;
            }
        }

        DBG_API((_API_HEADER"----------------%s(pt_player=0x%x)\r\n\r\n", __func__, pt_player));

#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_tmp = (INT32) PLAYER_PIPE_PRIORITY + (INT32) i1_prio_diff;

        if (i4_tmp >= 255)
        {
            pt_player->ui1_priority = 255;
        }
        else if (i4_tmp <= 0)
        {
            pt_player->ui1_priority = 0;
        }
        else
        {
            pt_player->ui1_priority = (UINT8) i4_tmp;
        }
#endif
        *ppt_player = pt_player;
        return SVCTXR_OK;
    } while (0);

    DBG_ERROR((_ERROR_HEADER"----------------%s() failed!\r\n\r\n", __func__));

    /* error handling */
    if (pt_player != NULL) {
        DBG_ERROR((_ERROR_HEADER"%s() failed, return player = 0x%x to pool\r\n",
                        __func__,
                        pt_player
                        ));
        DLIST_REMOVE(pt_player, &t_g_using_player_q, t_dlink);
        DLIST_INSERT_HEAD(pt_player, &t_g_pooled_player_q, t_dlink);
    }

    *ppt_player = NULL;
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *     player_close
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 player_close (
                    PLAYER_T*                   pt_player
                    )
{
    DBG_API((_API_HEADER"----------------%s(pt_player=0x%x)\r\n\r\n", __func__, pt_player));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_player->b_reuse_pipe == TRUE) {
        return SVCTXR_OK;
    }

    if (player_valid(pt_player) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid player = 0x%x\r\n", __func__, pt_player));
        return SVCTXR_OK;
    }

    if (x_handle_valid(pt_player->h_pipe) == TRUE) {
        DBG_ERROR((_ERROR_HEADER"%s(h_pipe is still valid)\r\n", __func__));
        return SVCTXR_FAIL;
    }

    if (x_handle_valid(pt_player->h_conn) == TRUE) {
        DBG_ERROR((_ERROR_HEADER"%s(h_conn is still valid)\r\n", __func__));
        return SVCTXR_FAIL;
    }

    if (x_handle_valid(pt_player->h_scdb) == TRUE) {
        DBG_ERROR((_ERROR_HEADER"%s(h_scdb is still valid)\r\n", __func__));
        return SVCTXR_FAIL;
    }

    if (x_handle_valid(pt_player->h_sess) == TRUE) {
        DBG_ERROR((_ERROR_HEADER"%s(h_sess is still valid)\r\n", __func__));
        return SVCTXR_FAIL;
    }

    if (x_handle_valid(pt_player->h_connect_timer) == TRUE) {
        x_timer_stop(pt_player->h_connect_timer);
    }

    pt_player->h_pipe              = NULL_HANDLE;
    pt_player->h_conn              = NULL_HANDLE;
    pt_player->h_scdb              = NULL_HANDLE;
    pt_player->h_sess              = NULL_HANDLE;
    pt_player->e_pipe_cond         = CM_COND_CLOSED;
    pt_player->e_conn_cond         = CM_COND_DISCONNECTED;
    pt_player->e_scdb_cond         = SCDB_CLOSED;
    pt_player->e_sess_cond         = SM_SESS_COND_CLOSED;
    pt_player->e_snk_oper_type     = SNK_OPER_TYPE_NONE;
    pt_player->e_src_oper_type     = SRC_OPER_TYPE_NONE;
    pt_player->i4_speed            = 0;
    pt_player->ui4_reconnect_delay = 0;
    pt_player->pf_nty              = NULL;
    pt_player->pv_tag              = NULL;
    pt_player->b_reuse_pipe        = FALSE;
    pt_player->pv_sess_data        = NULL;
    pt_player->z_sess_data_size    = 0;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_player->ui1_act_queued      = 0;
    pt_player->e_action            = PLAYER_ACTION_NONE;
    pt_player->ui1_priority        = PLAYER_PIPE_PRIORITY;
#endif
    DLIST_REMOVE(pt_player, &t_g_using_player_q, t_dlink);
    DLIST_INSERT_HEAD(pt_player, &t_g_pooled_player_q, t_dlink);

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *     player_play
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 player_play (
                    PLAYER_T*                   pt_player,
                    INT32                       i4_speed
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (player_valid(pt_player) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s(), invalid player = 0x%x\r\n", __func__, pt_player));
        return SVCTXR_NOT_EXIST;
    }

    DBG_API((_API_HEADER"%s(%s, %s)\r\n",
                    __func__,
                    s_g_src_oper_types[pt_player->e_src_oper_type],
                    s_g_snk_oper_types[pt_player->e_snk_oper_type]
                    ));

#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_player->e_action = PLAYER_ACTION_PLAY;
#endif
    pt_player->i4_speed = i4_speed;

    i4_ret = _player_play_sess(pt_player);

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      player_stop
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 player_stop(
                    PLAYER_T*                   pt_player
                    )
{
    INT32 i4_ret;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (player_valid(pt_player) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid player = 0x%x\r\n", __func__, pt_player));
        return SVCTXR_NOT_EXIST;
    }

    DBG_API((_API_HEADER"%s(%s, %s)\r\n",
                    __func__,
                    s_g_src_oper_types[pt_player->e_src_oper_type],
                    s_g_snk_oper_types[pt_player->e_snk_oper_type]
                    ));

#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_player->e_action = PLAYER_ACTION_STOP;
#endif
    i4_ret = _player_close_pipe(pt_player);
    if (i4_ret != SVCTXR_OK && i4_ret != SVCTXR_PENDING) {
        DBG_ERROR((_ERROR_HEADER"%s(), _player_close_pipe(player = 0x%x) failed, i4_ret=%d\r\n",
                    __func__,
                    pt_player,
                    i4_ret
                    ));
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      player_pause
 * Description
 *      The API is used to stop a running session.
 * Input arguments
 *      pt_this            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK
 *      SVCTXR_PENDING
 *-----------------------------------------------------------------------------*/
INT32 player_pause(
                    PLAYER_T*                   pt_player
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (player_valid(pt_player) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s(), invalid player = 0x%x\r\n", __func__, pt_player));
        return SVCTXR_NOT_EXIST;
    }

    DBG_API((_API_HEADER"%s(h_sess=%d, %s, %s, %s)\r\n",
                    __func__,
                    pt_player->h_sess,
                    s_g_sess_conds[pt_player->e_sess_cond],
                    s_g_src_oper_types[pt_player->e_src_oper_type],
                    s_g_snk_oper_types[pt_player->e_snk_oper_type]
                    ));

#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_player->e_action = PLAYER_ACTION_PAUSE;
#endif

    if (x_handle_valid(pt_player->h_sess) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid h_sess = %d\r\n", __func__, pt_player->h_sess));
        return SVCTXR_OK;
    }
#ifdef MW_TV_AV_BYPASS_SUPPORT
    else if (pt_player->ui1_act_queued > 0)
    {
        DBG_INFO((_INFO_HEADER"%s: %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    if (pt_player->e_snk_oper_type == SNK_OPER_TYPE_CLOSE) {
        DBG_INFO((_INFO_HEADER"session is in closing(h_sess=%d) discard stop session request\r\n",
                    pt_player->h_sess
                    ));
        return SVCTXR_OK;
    }

    if (pt_player->e_sess_cond == SM_SESS_COND_OPENED) {
        DBG_INFO((_INFO_HEADER"%s(SM_SESS_COND_OPENED) already paused\r\n", __func__));
        return SVCTXR_OK;
    }

    if (pt_player->e_sess_cond != SM_SESS_COND_STARTED) {
        DBG_INFO((_INFO_HEADER"%s() cannot execute, since %s\r\n",
                    __func__,
                    s_g_sess_conds[pt_player->e_sess_cond]
                    ));
        pt_player->e_snk_oper_type = SNK_OPER_TYPE_STOP;
        return SVCTXR_PENDING;
    }

    i4_ret = x_sm_stop_sess(pt_player->h_sess);
    if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) {
        pt_player->e_snk_oper_type = SNK_OPER_TYPE_STOP;
        pt_player->e_sess_cond     = SM_SESS_COND_STOPPING;
    } else if (i4_ret == SMR_ALREADY_STOPPED) {
        pt_player->e_snk_oper_type = SNK_OPER_TYPE_STOP;
    } else {
        DBG_ERROR((_ERROR_HEADER"x_sm_stop_sess() failed, ret=%d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    i4_ret = pt_player->pf_nty(
                    pt_player,
                    PLAYER_NTFY_CODE_SESS_STOPPING,
                    pt_player->pv_tag,
                    0
                    );
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      player_get_pipe_handle
 * Description
 *      The API is used to get pipe handle
 * Input arguments
 *      pt_player            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
HANDLE_T player_get_pipe_handle(
                    PLAYER_T*                   pt_player
                    )
{
    if (b_g_init == FALSE) {
        return NULL_HANDLE;
    }

    if (player_valid(pt_player) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid player = 0x%x\r\n", __func__, pt_player));
        return NULL_HANDLE;
    }

    return pt_player->h_pipe;
}
/*-----------------------------------------------------------------------------
 * Name
 *      player_get_conn_handle
 * Description
 *      The API is used to get conn handle
 * Input arguments
 *      pt_player            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
HANDLE_T player_get_conn_handle(
                    PLAYER_T*                   pt_player
                    )
{
    if (b_g_init == FALSE) {
        return NULL_HANDLE;
    }

    if (player_valid(pt_player) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid player = 0x%x\r\n", __func__, pt_player));
        return NULL_HANDLE;
    }

    return pt_player->h_conn;
}
/*-----------------------------------------------------------------------------
 * Name
 *      player_get_scdb_handle
 * Description
 *      The API is used to get scdb handle
 * Input arguments
 *      pt_player            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
HANDLE_T player_get_scdb_handle(
                    PLAYER_T*                   pt_player
                    )
{
    if (b_g_init == FALSE) {
        return NULL_HANDLE;
    }

    if (player_valid(pt_player) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid player = 0x%x\r\n", __func__, pt_player));
        return NULL_HANDLE;
    }

    return pt_player->h_scdb;
}
/*-----------------------------------------------------------------------------
 * Name
 *      player_get_oper_types
 * Description
 *      The API is used to current operation type
 * Input arguments
 *      pt_player            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
INT32 player_get_oper_types(
                    PLAYER_T*                   pt_player,
                    SRC_OPER_TYPE_T*            pe_src_oper_type,
                    SNK_OPER_TYPE_T*            pe_snk_oper_type
                    )
{
    if (player_valid(pt_player) == FALSE
            || pe_src_oper_type == NULL
            || pe_snk_oper_type == NULL) {
        return SVCTXR_INV_ARG;
    }

    *pe_src_oper_type = pt_player->e_src_oper_type;
    *pe_snk_oper_type = pt_player->e_snk_oper_type;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      player_get_sess_handle
 * Description
 *      The API is used to get sess handle
 * Input arguments
 *      pt_player            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
HANDLE_T player_get_sess_handle(
                    PLAYER_T*                   pt_player
                    )
{
    if (b_g_init == FALSE) {
        return NULL_HANDLE;
    }

    if (player_valid(pt_player) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid player = 0x%x\r\n", __func__, pt_player));
        return NULL_HANDLE;
    }

    return pt_player->h_sess;
}
/*-----------------------------------------------------------------------------
 * Name
 *      player_get_pipe_cond
 * Description
 *      The API is used to get conn cond
 * Input arguments
 *      pt_player            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
CM_COND_T player_get_pipe_cond(
                    PLAYER_T*                   pt_player
                    )
{
    if (b_g_init == FALSE) {
        return CM_COND_CLOSED;
    }

    if (player_valid(pt_player) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid player = 0x%x\r\n", __func__, pt_player));
        return CM_COND_CLOSED;
    }

    return pt_player->e_pipe_cond;
}
/*-----------------------------------------------------------------------------
 * Name
 *      player_get_conn_cond
 * Description
 *      The API is used to get conn cond
 * Input arguments
 *      pt_player            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
CM_COND_T player_get_conn_cond(
                    PLAYER_T*                   pt_player
                    )
{
    if (b_g_init == FALSE) {
        return CM_COND_DISCONNECTED;
    }

    if (player_valid(pt_player) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid player = 0x%x\r\n", __func__, pt_player));
        return CM_COND_DISCONNECTED;
    }

    return pt_player->e_conn_cond;
}
/*-----------------------------------------------------------------------------
 * Name
 *      player_get_scdb_cond
 * Description
 *      The API is used to get scdb cond
 * Input arguments
 *      pt_player            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
SCDB_COND_T player_get_scdb_cond(
                    PLAYER_T*                   pt_player
                    )
{
    if (b_g_init == FALSE) {
        return SCDB_CLOSED;
    }

    if (player_valid(pt_player) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid player = 0x%x\r\n", __func__, pt_player));
        return SCDB_CLOSED;
    }

    return pt_player->e_scdb_cond;
}
/*-----------------------------------------------------------------------------
 * Name
 *      player_get_sess_cond
 * Description
 *      The API is used to get sess cond
 * Input arguments
 *      pt_player            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
SM_SESS_COND_T player_get_sess_cond(
                    PLAYER_T*                   pt_player
                    )
{
    if (b_g_init == FALSE) {
        return SM_SESS_COND_CLOSED;
    }

    if (player_valid(pt_player) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid player = 0x%x\r\n", __func__, pt_player));
        return SM_SESS_COND_CLOSED;
    }

    return pt_player->e_sess_cond;
}
/*-----------------------------------------------------------------------------
 * Name
 *      player_valid
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
BOOL player_valid (
                    PLAYER_T*                   pt_chker
                    )
{
    PLAYER_T* pt_chkee;

    if (pt_chker == NULL) {
        return FALSE;
    }

    DLIST_FOR_EACH(pt_chkee, &t_g_using_player_q, t_dlink) {
        if (pt_chkee == pt_chker) {
            return TRUE;
        }
    }

    DBG_INFO((_INFO_HEADER"%s() - pt_chker=0x%08x not existed\r\n ", __func__, pt_chker));

    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name
 *      player_get_using_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
PLAYER_T* player_get_using_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_using_player_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      player_get_pooled_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
PLAYER_T* player_get_pooled_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_pooled_player_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      player_next
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
PLAYER_T* player_next(
                    PLAYER_T*                   pt_player
                    )
{
    if (b_g_init == FALSE || pt_player == NULL) {
        return NULL;
    }

    return (DLIST_NEXT(pt_player, t_dlink));
}
/*-----------------------------------------------------------------------------
 * Name
 *      player_print_self
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
VOID player_print_self(
                    PLAYER_T*                   pt_player
                    )
{
#ifdef CLI_LVL_ALL
    x_dbg_stmt("------------------------------------ size = %d\r\n", sizeof(PLAYER_T));
    x_dbg_stmt("player              - 0x%x\r\n", pt_player);
    if (pt_player != NULL) {
        x_dbg_stmt("h_pipe              - %-10d, %s\r\n", pt_player->h_pipe, s_g_cm_conds[pt_player->e_pipe_cond]);
        x_dbg_stmt("h_conn              - %-10d, %s\r\n", pt_player->h_conn, s_g_cm_conds[pt_player->e_conn_cond]);
        x_dbg_stmt("h_scdb              - %-10d, %s\r\n", pt_player->h_scdb, s_g_scdb_conds[pt_player->e_scdb_cond]);
        x_dbg_stmt("h_sess              - %-10d, %s\r\n", pt_player->h_sess, s_g_sess_conds[pt_player->e_sess_cond]);
        x_dbg_stmt("e_src_oper_type     - %s\r\n", s_g_src_oper_types[pt_player->e_src_oper_type]);
        x_dbg_stmt("e_snk_oper_type     - %s\r\n", s_g_snk_oper_types[pt_player->e_snk_oper_type]);
        x_dbg_stmt("i4_speed            - %d\r\n", pt_player->i4_speed);
        x_dbg_stmt("h_connect_timer     - %d\r\n", pt_player->h_connect_timer);
        x_dbg_stmt("ui4_reconnect_delay - %d\r\n", pt_player->ui4_reconnect_delay);
        x_dbg_stmt("s_snk_grp_name      - %s\r\n", pt_player->s_snk_grp_name);
        x_dbg_stmt("s_snk_grp_name_aux  - %s\r\n", pt_player->s_snk_grp_name_aux);
        x_dbg_stmt("pf_nty              - 0x%x\r\n", pt_player->pf_nty);
        x_dbg_stmt("pv_tag              - 0x%x\r\n", pt_player->pv_tag);
        x_dbg_stmt("b_reuse_pipe        - %s\r\n", (pt_player->b_reuse_pipe ? "Y" : "N"));
    }
    x_dbg_stmt("====================================\r\n");
#endif
}
/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
#ifdef MW_TV_AV_BYPASS_SUPPORT
static UINT8 _player_get_msg_id(PLAYER_ACTION_T e_action)
{
    switch (e_action)
    {
        case PLAYER_ACTION_PLAY:
            return _PLAYER_MSG_QUEUED_PLAY_NTFY;

        case PLAYER_ACTION_STOP:
            return _PLAYER_MSG_QUEUED_STOP_NTFY;

        case PLAYER_ACTION_PAUSE:
            return _PLAYER_MSG_QUEUED_PAUSE_NTFY;

        case PLAYER_ACTION_PIPE_OPEN:
            return _PLAYER_MSG_QUEUED_PIPE_OPEN_NTFY;

        case PLAYER_ACTION_PIPE_CLOSE:
            return _PLAYER_MSG_QUEUED_PIPE_CLOSE_NTFY;

        case PLAYER_ACTION_CONN_OPEN:
            return _PLAYER_MSG_QUEUED_CONN_OPEN_NTFY;

        case PLAYER_ACTION_CONN_CLOSE:
            return _PLAYER_MSG_QUEUED_CONN_CLOSE_NTFY;

        case PLAYER_ACTION_SESS_OPEN:
            return _PLAYER_MSG_QUEUED_SESS_OPEN_NTFY;

        case PLAYER_ACTION_SESS_PLAY:
            return _PLAYER_MSG_QUEUED_SESS_PLAY_NTFY;

        case PLAYER_ACTION_SESS_CLOSE:
            return _PLAYER_MSG_QUEUED_SESS_CLOSE_NTFY;

        case PLAYER_ACTION_RECONNECT:
            return _PLAYER_MSG_QUEUED_RECONNECT_NTFY;

        default:
            return _PLAYER_MSG_QUEUED_NONE;
    }
}

static INT32 _player_query_action(PLAYER_T* pt_player)
{
    SVCTX_MSG_T t_msg = {0};

    if (pt_player->pf_nty)
    {
        t_msg.ui1_msg_id     = _player_get_msg_id(pt_player->e_action);
        t_msg.pf_invoke_func = _player_internal_msg;
        t_msg.pv_tag         = pt_player;

        if (SVCTXR_QUEUED == pt_player->pf_nty(
            pt_player,
            PLAYER_NTFY_CODE_QUERY_ACTION,
            pt_player->pv_tag,
            (UINT32) &t_msg))
        {
            pt_player->ui1_act_queued++;
            return SVCTXR_QUEUED;
        }
    }

    return SVCTXR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name
 *      _player_open_pipe
 * Description
 *      The API is used to open a pipe.
 * Input arguments
 *      pt_player            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK         the routine is successful
 *      SVCTXR_FAIL       the routine is failed
 *-----------------------------------------------------------------------------*/
static INT32 _player_open_pipe(
                    PLAYER_T*                   pt_player
                    )
{
    INT32 i4_ret = SVCTXR_OK;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_player->ui1_act_queued > 0)
    {
        DBG_INFO((_INFO_HEADER"%s: %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    if (x_handle_valid(pt_player->h_pipe) == TRUE) {
        return (pt_player->e_pipe_cond != CM_COND_OPENED) ? SVCTXR_PENDING : SVCTXR_OK;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_player->e_phase = PLAYER_ACTION_PIPE_OPEN;

    if (SVCTXR_QUEUED == _player_query_action(pt_player))
    {
        DBG_INFO((_INFO_HEADER"%s: _player_query_action() returned SVCTXR_QUEUED. %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    
    svctx_cli_print_channel_change_log((SVCTX_T*)(pt_player->pv_tag), CH_XNG_STAGE_REQUEST_CONNECT);

    DBG_INFO((_INFO_HEADER"%s(0x%x)\r\n", __func__, pt_player));

    CLI_FUNC_CALL_TRACE("x_cm_open_pipe");
    TMS_BEGIN(PLAYER_TMS_PIPE_OPEN);
    i4_ret = x_cm_open_pipe(
                    NULL,
#ifdef MW_TV_AV_BYPASS_SUPPORT
                    pt_player->ui1_priority,
#else
                    PLAYER_PIPE_PRIORITY,
#endif
                    PLAYER_PIPE_FLAG,
                    (VOID*)pt_player,
                    _player_pipe_nfy_fct,
                    &(pt_player->h_pipe),
                    &(pt_player->e_pipe_cond)
                    );
    /*x_dbg_stmt("[%s] x_cm_open_pipe(), cond=%d, handle=0x%x(%d)\r\n", 
        pt_player->s_snk_grp_name, pt_player->e_pipe_cond, pt_player->h_pipe, pt_player->h_pipe);*/

    if (i4_ret != CMR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_cm_open_pipe() failed - ret = %d\r\n", i4_ret));
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_player->e_action = PLAYER_ACTION_RECONNECT;
#endif
        i4_ret = _player_reconnect(pt_player);
        return i4_ret;
    }

    DBG_INFO((_INFO_HEADER"%s(), x_cm_open_pipe(h_conn=%d, %s)\r\n",
                    __func__,
                    pt_player->h_pipe,
                    s_g_cm_conds[pt_player->e_pipe_cond]
                    ));

    if (pt_player->e_pipe_cond == CM_COND_OPENED) {
        TMS_END(PLAYER_TMS_PIPE_OPEN);
        i4_ret = SVCTXR_OK;
    } else {
        i4_ret = SVCTXR_PENDING;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_close_pipe
 * Description
 *      The API is used to close a connected connection
 * Input arguments
 *      pt_player            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK
 *      SVCTXR_FAIL
 *-----------------------------------------------------------------------------*/
static INT32 _player_close_pipe(
                    PLAYER_T*                   pt_player
                    )
{
    INT32 i4_ret;

    pt_player->e_src_oper_type = SRC_OPER_TYPE_DISCONNECT;

    x_timer_stop(pt_player->h_connect_timer);

    i4_ret = _player_close_conn(pt_player);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_player->ui1_act_queued > 0)
    {
        DBG_INFO((_INFO_HEADER"%s: %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    if (pt_player->b_reuse_pipe == TRUE) {
        pt_player->e_src_oper_type = SRC_OPER_TYPE_NONE;
        return SVCTXR_OK;
    }

    if (x_handle_valid(pt_player->h_pipe) == FALSE) {
        return SVCTXR_OK;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_player->e_phase = PLAYER_ACTION_PIPE_CLOSE;

    if (SVCTXR_QUEUED == _player_query_action(pt_player))
    {
        DBG_INFO((_INFO_HEADER"%s: _player_query_action() returned SVCTXR_QUEUED. %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    DBG_API((_API_HEADER"%s(pt_player=0x%x)\r\n", __func__, pt_player));

    CLI_FUNC_CALL_TRACE("x_cm_close_pipe");
    TMS_BEGIN(PLAYER_TMS_PIPE_CLOSE);
    i4_ret = x_cm_close_pipe (
                    pt_player->h_pipe,
                    &(pt_player->e_pipe_cond)
                    );

    /*x_dbg_stmt("[%s] x_cm_close_pipe(), cond=%d, handle=0x%x(%d)\r\n", 
        pt_player->s_snk_grp_name, pt_player->e_pipe_cond, pt_player->h_pipe, pt_player->h_pipe);*/

    if (i4_ret != CMR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_cm_close_pipe() failed, i4_ret=%d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    DBG_INFO((_INFO_HEADER"x_cm_close_pipe(h_pipe=%d, e_pipe_cond=%d)\r\n",
                    pt_player->h_pipe,
                    s_g_cm_conds[pt_player->e_pipe_cond]
                    ));

    if (pt_player->e_pipe_cond != CM_COND_CLOSED) {
        return SVCTXR_PENDING;
    }

    _player_pipe_nfy_fct(
                    pt_player->h_pipe,
                    pt_player->e_pipe_cond,
                    (VOID*)pt_player,
                    0
                    );
    return SVCTXR_PENDING;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_pipe_nfy_fct
 * Description
 *      The API is used to receive the pipe's notification,
 *      and transfer to an internal event, and send to the attached
 *      application's event dispatcher for further processing.
 * Input arguments
 *      h_handle        the handle of the pipe that causes the notification
 *      e_nfy_cond      the state of the pipe
 *      pv_nfy_tag      an APP_EVN_SRC_T object
 *                      object
 *      ui4_data        additional data
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static VOID _player_pipe_nfy_fct(
                    HANDLE_T                    h_pipe,
                    CM_COND_T                   e_pipe_cond,
                    VOID*                       pv_nfy_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_MSG_T t_msg     = {0};
    PLAYER_T*   pt_player = (PLAYER_T*)pv_nfy_tag;

    if (pt_player == NULL || pt_player->pf_nty == NULL) {
        return;
    }

    t_msg.ui1_msg_id        = _PLAYER_MSG_PIPE_NTFY;
    t_msg.pf_invoke_func    = _player_internal_msg;
    t_msg.pv_tag            = pv_nfy_tag;
    t_msg.u.t_pipe.h_obj    = h_pipe;
    t_msg.u.t_pipe.e_cond   = e_pipe_cond;
    t_msg.u.t_pipe.ui4_data = ui4_data;

    pt_player->pf_nty(
                    pt_player,
                    PLAYER_NTFY_CODE_REQ_SEND_MSG,
                    pt_player->pv_tag,
                    (UINT32)&t_msg
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_open_conn
 * Description
 *      The API is used to connect a connection after the pipe is opened. If
 *      the connection request is failed, the application will receive a
 *      SVCTX_MSG_CONNECT_FAILED event.
 * Input arguments
 *      pt_player         the pointer points to an PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _player_open_conn(
                    PLAYER_T*                   pt_player
                    )
{
    CM_COMMAND_T t_cm_cmds[MAX_SIZE_OF_CM_COMMAND] = {{0}};
    INT32        i4_ret = SVCTXR_OK;
    CM_COND_T    e_conn_cond = pt_player->e_conn_cond;

    pt_player->e_src_oper_type = SRC_OPER_TYPE_CONNECT;

    i4_ret = _player_open_pipe(pt_player);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_player->ui1_act_queued > 0)
    {
        DBG_INFO((_INFO_HEADER"%s: %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    if (pt_player->b_reuse_pipe == FALSE
            && x_handle_valid(pt_player->h_conn) == TRUE) {
        return (e_conn_cond != CM_COND_CONNECTED) ? SVCTXR_PENDING : SVCTXR_OK;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_player->e_phase = PLAYER_ACTION_CONN_OPEN;

    if (SVCTXR_QUEUED == _player_query_action(pt_player))
    {
        DBG_INFO((_INFO_HEADER"%s: _player_query_action() returned SVCTXR_QUEUED. %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    DBG_INFO((_INFO_HEADER"%s(0x%x)\r\n", __func__, pt_player));

    t_cm_cmds[0].e_code = CONN_END;
    i4_ret = pt_player->pf_nty(
                    pt_player,
                    PLAYER_NTFY_CODE_PIPE_OPENED_GET_CMDS,
                    pt_player->pv_tag,
                    (UINT32)(t_cm_cmds)
                    );
    if (i4_ret != SVCTXR_OK) {
        DBG_ERROR((_ERROR_HEADER"%s() get CM commands failed!\r\n", __func__));
        i4_ret = player_stop(pt_player);
        return i4_ret;
    }

    if (pt_player->b_reuse_pipe == TRUE) {
        pt_player->b_reuse_pipe = FALSE;
    }

#if 1  /* for MW_NINJA_CHANGE_CHANNEL */
    if (pt_player->b_ninja_change)
    {
        pt_player->h_conn = pt_player->h_conn_bak;
        x_dbg_stmt("[%s] svl rec id = %d\r\n", pt_player->s_snk_grp_name,t_cm_cmds[3].u.ui2_number);
        x_cm_set(pt_player->h_conn, BRDCST_CTRL_SET_SVL_REC_IN_SAME_TS, (VOID*)(UINT32)t_cm_cmds[3].u.ui2_number);
        return SVCTXR_PENDING;
    }
#endif

    CLI_FUNC_CALL_TRACE("x_cm_connect");
    TMS_BEGIN(PLAYER_TMS_CM_CONN);
    i4_ret = x_cm_connect(
                    pt_player->h_pipe,
                    t_cm_cmds,
                    (VOID*)pt_player,
                    _player_conn_nfy_fct,
                    &(pt_player->h_conn),
                    &(e_conn_cond)
                    );
    /* x_dbg_stmt("[%s] x_cm_connect(), cond=%d, handle=0x%x(%d)\r\n", 
        pt_player->s_snk_grp_name, e_conn_cond, pt_player->h_conn, pt_player->h_conn);*/

    if (i4_ret != CMR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_cm_connect() failed, i4_ret=%d\r\n", i4_ret));
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_player->e_action = PLAYER_ACTION_RECONNECT;
#endif
        i4_ret = _player_reconnect(pt_player);
        return i4_ret;
    }

    DBG_INFO((_INFO_HEADER"%s(), x_cm_connect(h_conn=%d, %s)\r\n",
                    __func__,
                    pt_player->h_conn,
                    s_g_cm_conds[e_conn_cond]
                    ));

    if (e_conn_cond == CM_COND_CONNECTED) {
        _player_conn_nfy_fct(
                    pt_player->h_conn,
                    e_conn_cond,
                    (VOID*)pt_player,
                    0
                    );
        return SVCTXR_PENDING;
    } else {
        pt_player->e_conn_cond = e_conn_cond;
    }

    DBG_INFO((_INFO_HEADER"%s(), x_timer_stop(pt_player->h_connect_timer)\r\n", __func__));
    i4_ret = x_timer_stop(pt_player->h_connect_timer);
    if (i4_ret != OSR_OK) {
        DBG_ERROR((_ERROR_HEADER"%s(), x_timer_stop(%d) failed! i4_ret=%d\r\n",
                    __func__,
                    pt_player->h_connect_timer,
                    i4_ret
                    ));
        return SVCTXR_FAIL;
    }

    DBG_INFO((_INFO_HEADER"%s(), x_timer_start(pt_player->h_connect_timer, _player_connect_timeout_nfy_fct)\r\n", __func__));
    i4_ret = x_timer_start(
                    pt_player->h_connect_timer,
                    DEFAULT_CONNECT_TIMEOUT,
                    X_TIMER_FLAG_ONCE,
                    _player_connect_timeout_nfy_fct,
                    (VOID*)pt_player
                    );
    return SVCTXR_PENDING;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_close_conn
 * Description
 *      The API is used to close a connected connection
 * Input arguments
 *      pt_player            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK
 *      SVCTXR_FAIL
 *-----------------------------------------------------------------------------*/
static INT32 _player_close_conn(
                    PLAYER_T*                   pt_player
                    )
{
    CM_COND_T e_conn_cond = pt_player->e_conn_cond;
    INT32     i4_ret;

    i4_ret = _player_close_scdb(pt_player);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_player->ui1_act_queued > 0)
    {
        DBG_INFO((_INFO_HEADER"%s: %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    if (x_handle_valid(pt_player->h_conn) == FALSE) {
        pt_player->b_reuse_pipe = FALSE;
        return SVCTXR_OK;
    }

    if (pt_player->b_reuse_pipe == TRUE) {
        pt_player->e_src_oper_type = SRC_OPER_TYPE_NONE;
        return SVCTXR_OK;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_player->e_phase = PLAYER_ACTION_CONN_CLOSE;

    if (SVCTXR_QUEUED == _player_query_action(pt_player))
    {
        DBG_INFO((_INFO_HEADER"%s: _player_query_action() returned SVCTXR_QUEUED. %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    DBG_API((_API_HEADER"%s(pt_player=0x%x)\r\n", __func__, pt_player));

    CLI_FUNC_CALL_TRACE("x_cm_disconnect");
    TMS_BEGIN(PLAYER_TMS_CM_DISCONN)
    i4_ret = x_cm_disconnect(
                    pt_player->h_conn,
                    &e_conn_cond
                    );
    /* x_dbg_stmt("[%s] x_cm_disconnect(), cond=%d, handle=0x%x(%d)\r\n", 
        pt_player->s_snk_grp_name, e_conn_cond, pt_player->h_conn, pt_player->h_conn);*/

    
    if (i4_ret != CMR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_cm_disconnect() failed, i4_ret = %d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    DBG_INFO((_INFO_HEADER"x_cm_disconnect(h_conn=%d, %s)\r\n",
                    pt_player->h_conn,
                    s_g_cm_conds[e_conn_cond]
                    ));

    if (e_conn_cond == CM_COND_DISCONNECTED) {
        _player_conn_nfy_fct(
                    pt_player->h_conn,
                    e_conn_cond,
                    (VOID*)pt_player,
                    0
                    );
    } else {
        pt_player->e_conn_cond = e_conn_cond;
    }

    return SVCTXR_PENDING;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_conn_nfy_fct
 * Description
 *      The API is used to receive the connection's notification,
 *      and transfer to an internal event, and send to the attached
 *      application's event dispatcher for further processing.
 * Input arguments
 *      h_handle      the handle of the connection that causes the notification
 *      e_nfy_cond    the state of the connection
 *      pv_nfy_tag    an APP_EVN_SRC_T object
 *      ui4_data      additional data
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static VOID _player_conn_nfy_fct(
                    HANDLE_T                    h_conn,
                    CM_COND_T                   e_conn_cond,
                    VOID*                       pv_nfy_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_MSG_T t_msg     = {0};
    PLAYER_T*   pt_player = (PLAYER_T*)pv_nfy_tag;

    if (pt_player == NULL || pt_player->pf_nty == NULL) {
        return;
    }

    t_msg.ui1_msg_id        = _PLAYER_MSG_CONN_NTFY;
    t_msg.pf_invoke_func    = _player_internal_msg;
    t_msg.pv_tag            = pv_nfy_tag;
    t_msg.u.t_conn.h_obj    = h_conn;
    t_msg.u.t_conn.e_cond   = e_conn_cond;
    t_msg.u.t_conn.ui4_data = ui4_data;

    pt_player->pf_nty(
                    pt_player,
                    PLAYER_NTFY_CODE_REQ_SEND_MSG,
                    pt_player->pv_tag,
                    (UINT32)&t_msg
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_reconnect
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _player_reconnect(
                    PLAYER_T*                   pt_player
                    )
{
    INT32 i4_ret;

    DBG_INFO((_INFO_HEADER"%s(pt_player = 0x%x)\r\n\r\n", __func__, pt_player));

    pt_player->e_src_oper_type = SRC_OPER_TYPE_CONNECT;

    i4_ret = _player_close_conn(pt_player);
    if (i4_ret == SVCTXR_OK) {
        if (pt_player->ui4_reconnect_delay == 0) {
            DBG_INFO((_INFO_HEADER"invoke _player_reconnect_nfy_fct\r\n"));
            _player_reconnect_nfy_fct(
                    pt_player->h_connect_timer,
                    (VOID*)pt_player
                    );
            return SVCTXR_PENDING;
        }
    } else if (i4_ret == SVCTXR_PENDING) {
        DBG_INFO((_INFO_HEADER"%s(), invoke _player_close_conn() Pending\r\n", __func__));
        return i4_ret;
    } else {
        DBG_INFO((_INFO_HEADER"%s(), invoke _player_close_conn() failed, i4_ret=%d\r\n", __func__, i4_ret));
        return i4_ret;
    }

    DBG_INFO((_INFO_HEADER"%s(), x_timer_stop(h_connect_timer=%d)\r\n", __func__, pt_player->h_connect_timer));
    i4_ret = x_timer_stop(pt_player->h_connect_timer);
    if (i4_ret != OSR_OK) {
        return SVCTXR_FAIL;
    }

    DBG_INFO((_INFO_HEADER"%s(), x_timer_start(h_connect_timer=%d, _player_reconnect_nfy_fct)\r\n",
                    __func__,
                    pt_player->h_connect_timer
                    ));
    i4_ret = x_timer_start(
                    pt_player->h_connect_timer,
                    pt_player->ui4_reconnect_delay,
                    X_TIMER_FLAG_ONCE,
                    _player_reconnect_nfy_fct,
                    (VOID*)pt_player
                    );
    if (i4_ret != OSR_OK) {
        return SVCTXR_FAIL;
    }

    return SVCTXR_PENDING;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_reconnect_nfy_fct
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static VOID _player_reconnect_nfy_fct(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
    SVCTX_MSG_T t_msg     = {0};
    PLAYER_T*   pt_player = (PLAYER_T*)pv_tag;

    if (pt_player == NULL || pt_player->pf_nty == NULL) {
        return;
    }

    t_msg.ui1_msg_id      = _PLAYER_MSG_RECONN_DELAY_NTFY;
    t_msg.pf_invoke_func  = _player_internal_msg;
    t_msg.pv_tag          = pv_tag;
    t_msg.u.t_timer.h_obj = h_timer;

    pt_player->pf_nty(
                    pt_player,
                    PLAYER_NTFY_CODE_REQ_SEND_MSG,
                    pt_player->pv_tag,
                    (UINT32)&t_msg
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_connect_timeout_nfy_fct
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static VOID _player_connect_timeout_nfy_fct(
                    HANDLE_T                    h_timer,
                    VOID*                       pv_tag
                    )
{
    SVCTX_MSG_T t_msg     = {0};
    PLAYER_T*   pt_player = (PLAYER_T*)pv_tag;

    if (pt_player == NULL || pt_player->pf_nty == NULL) {
        return;
    }

    t_msg.ui1_msg_id      = _PLAYER_MSG_CONNECT_TIMEOUT_NTFY;
    t_msg.pf_invoke_func  = _player_internal_msg;
    t_msg.pv_tag          = pv_tag;
    t_msg.u.t_timer.h_obj = h_timer;

    pt_player->pf_nty(
                    pt_player,
                    PLAYER_NTFY_CODE_REQ_SEND_MSG,
                    pt_player->pv_tag,
                    (UINT32)&t_msg
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_handle_connect_timeout
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _player_handle_connect_timeout (
                    PLAYER_T*                   pt_player,
                    HANDLE_T                    h_timer
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s(pt_player=0x%x)\r\n", __func__, pt_player));

    if (h_timer != pt_player->h_connect_timer) {
        return SVCTXR_OK;
    }

    if (pt_player->e_conn_cond == CM_COND_CONNECTING) {
        i4_ret = pt_player->pf_nty(
                    pt_player,
                    PLAYER_NTFY_CODE_CONN_DISCONNECTED,
                    pt_player->pv_tag,
                    (UINT32)TRUE
                    );
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_open_scdb
 * Description
 *      The API is used to open a new SCDB.
 * Input arguments
 *      pt_player            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK
 *      SVCTXR_FAIL
 *-----------------------------------------------------------------------------*/
static INT32 _player_open_scdb(
                    PLAYER_T*                   pt_player
                    )
{
    INT32 i4_ret;

    i4_ret = _player_open_conn(pt_player);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_player->ui1_act_queued > 0)
    {
        DBG_INFO((_INFO_HEADER"%s: %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    if (x_handle_valid(pt_player->h_scdb) == TRUE) {
        return (pt_player->e_scdb_cond != SCDB_UPDATED) ? SVCTXR_PENDING : SVCTXR_OK;
    }

    DBG_INFO((_INFO_HEADER"%s(0x%x)\r\n", __func__, pt_player));

    CLI_FUNC_CALL_TRACE("x_scdb_open");
    TMS_BEGIN(PLAYER_TMS_SCDB_OPEN);
    i4_ret = x_scdb_open(
                    pt_player->h_conn,
                    &(pt_player->h_scdb),
                    (VOID*)pt_player,
                    _player_scdb_nfy_fct
                    );
    if (i4_ret != SCDBR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_scdb_open() failed, i4_ret=%d\r\n", i4_ret));
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_player->e_action = PLAYER_ACTION_RECONNECT;
#endif
        i4_ret = _player_reconnect(pt_player);
        return i4_ret;
    }

    pt_player->e_scdb_cond = SCDB_UPDATED;
    if(pt_player->pf_nty)
    {
        i4_ret = pt_player->pf_nty(
                pt_player,
                PLAYER_NTFY_CODE_SCDB_OPENED,
                pt_player->pv_tag,
                0
                );
    }
    

    DBG_INFO((_INFO_HEADER"%s(), x_scdb_open(h_scdb=%d, %s)\r\n",
                    __func__,
                    pt_player->h_scdb,
                    s_g_scdb_conds[pt_player->e_scdb_cond]
                    ));

    TMS_END(PLAYER_TMS_SCDB_OPEN);
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_close_scdb
 * Description
 *      The API is used to close a using SCDB.
 * Input arguments
 *      pt_player            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK
 *      SVCTXR_FAIL
 *-----------------------------------------------------------------------------*/
static INT32 _player_close_scdb(
                    PLAYER_T*                   pt_player
                    )
{
    INT32 i4_ret;

    i4_ret = _player_close_sess(pt_player);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_player->ui1_act_queued > 0)
    {
        DBG_INFO((_INFO_HEADER"%s: %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    /* close using scdb handle */
    if (x_handle_valid(pt_player->h_scdb) == FALSE) {
        return SVCTXR_OK;
    }

    DBG_API((_API_HEADER"%s(pt_player=0x%x)\r\n", __func__, pt_player));

    CLI_FUNC_CALL_TRACE("x_scdb_close");
    TMS_BEGIN(PLAYER_TMS_SCDB_CLOSE);
    i4_ret = x_scdb_close(pt_player->h_scdb);
    if (i4_ret != SCDBR_OK && i4_ret != SCDBR_INV_HANDLE) {
        return SVCTXR_FAIL;
    }

    DBG_INFO((_INFO_HEADER"%s(), x_scdb_close(h_scdb=%d)\r\n",
                    __func__,
                    pt_player->h_scdb
                    ));

    TMS_END(PLAYER_TMS_SCDB_CLOSE);
    pt_player->h_scdb = NULL_HANDLE;
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_scdb_nfy_fct
 * Description
 *      The API is used to receive the notification of the SCDB for a
 *      connection, and transfer to an internal event, and send to the
 *      attached application's event dispatcher for further processing.
 * Input arguments
 *      h_handle        the handle of the SCDB for a connection that causes the
 *                      notification
 *      e_cond          the state of the SCDB for a connection
 *      ui4_reason      reason
 *      pv_tag          an APP_EVN_SRC_T object
 *      ui4_data        additional data
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static VOID  _player_scdb_nfy_fct(
                    HANDLE_T                    h_scdb,
                    SCDB_COND_T                 e_scdb_cond,
                    UINT32                      ui4_reason,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data
                    )
{
    SVCTX_MSG_T t_msg     = {0};
    PLAYER_T*   pt_player = (PLAYER_T*)pv_tag;

    if (pt_player == NULL || pt_player->pf_nty == NULL) {
        return;
    }

    /* Ignore the unknown reason */
    if (SCDB_REASON_UNK == ui4_reason) {
        return;
    }

    t_msg.ui1_msg_id          = _PLAYER_MSG_SCDB_NTFY;
    t_msg.pf_invoke_func      = _player_internal_msg;
    t_msg.pv_tag              = pv_tag;
    t_msg.u.t_scdb.h_obj      = h_scdb;
    t_msg.u.t_scdb.e_cond     = e_scdb_cond;
    t_msg.u.t_scdb.ui4_reason = ui4_reason;
    t_msg.u.t_scdb.ui4_data   = ui4_data;

    pt_player->pf_nty(
                    pt_player,
                    PLAYER_NTFY_CODE_REQ_SEND_MSG,
                    pt_player->pv_tag,
                    (UINT32)&t_msg
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_open_sess
 * Description
 *
 * Input arguments
 *      pt_player        the pointer points to an PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK       the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _player_open_sess(
                    PLAYER_T*                   pt_player
                    )
{
    INT32 i4_ret;

    i4_ret = _player_open_scdb(pt_player);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

#if 1  /* for MW_NINJA_CHANGE_CHANNEL */
    if (pt_player->b_ninja_change)
    {
        return SVCTXR_OK;
    }
#endif

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_player->ui1_act_queued > 0)
    {
        DBG_INFO((_INFO_HEADER"%s: %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    if (x_handle_valid(pt_player->h_sess) == TRUE) {
        DBG_INFO((_INFO_HEADER"%s(%s), h_sess=%d is valid\r\n",
                    __func__,
                    s_g_sess_conds[pt_player->e_sess_cond],
                    pt_player->h_sess
                    ));
        if (pt_player->e_sess_cond == SM_SESS_COND_OPENING) {
            return SVCTXR_PENDING;
        } else {
            return SVCTXR_OK;
        }
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_player->e_phase = PLAYER_ACTION_SESS_OPEN;

    if (SVCTXR_QUEUED == _player_query_action(pt_player))
    {
        DBG_INFO((_INFO_HEADER"%s: _player_query_action() returned SVCTXR_QUEUED. %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    DBG_INFO((_INFO_HEADER"%s(pt_player = 0x%x, %s, %s, %s)\r\n",
                    __func__,
                    pt_player,
                    s_g_sess_conds[pt_player->e_sess_cond],
                    s_g_src_oper_types[pt_player->e_src_oper_type],
                    s_g_snk_oper_types[pt_player->e_snk_oper_type]
                    ));

    CLI_FUNC_CALL_TRACE("x_sm_open_sess");
    TMS_BEGIN(PLAYER_TMS_SESS_OPEN);
    /* open a new session */
    i4_ret = x_sm_open_sess(
                    pt_player->h_conn,
                    SM_GNRC_SESS_HDLR_NAME,
                    pt_player->s_snk_grp_name,
                    pt_player->pv_sess_data,
                    pt_player->z_sess_data_size,
                    pt_player->i4_speed,
                    _player_sess_nfy_fct,
                    (VOID*)pt_player,
                    &(pt_player->h_sess)
                    );

    /*x_dbg_stmt("[%s] x_sm_open_sess(), handle=0x%x(%d)\r\n", 
        pt_player->s_snk_grp_name, pt_player->h_sess, pt_player->h_sess);*/

        
    if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
        DBG_ERROR((_ERROR_HEADER"x_sm_open_sess failed (ret = %ld)\r\n", i4_ret));
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_player->e_action = PLAYER_ACTION_RECONNECT;
#endif
        i4_ret = _player_reconnect(pt_player);
        return i4_ret;
    }

    DBG_INFO((_INFO_HEADER"%s(), x_sm_open_sess(h_sess=%d)\r\n",
                    __func__,
                    pt_player->h_sess
                    ));

    pt_player->e_sess_cond     = SM_SESS_COND_OPENING;
    pt_player->e_snk_oper_type = SNK_OPER_TYPE_OPEN;
    return SVCTXR_PENDING;
}
/*-----------------------------------------------------------------------------
 * Name
 *     _player_play_sess
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _player_play_sess (
                    PLAYER_T*                   pt_player
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    i4_ret = _player_open_sess(pt_player);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_player->ui1_act_queued > 0)
    {
        DBG_INFO((_INFO_HEADER"%s: %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }

    pt_player->e_phase = PLAYER_ACTION_SESS_PLAY;

    if (SVCTXR_QUEUED == _player_query_action(pt_player))
    {
        DBG_INFO((_INFO_HEADER"%s: _player_query_action() returned SVCTXR_QUEUED. %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    DBG_API((_API_HEADER"%s(pt_player=0x%x)\r\n\r\n", __func__, pt_player));

    i4_ret = x_sm_play_sess(
                    pt_player->h_sess,
                    pt_player->i4_speed
                    );
    if (i4_ret == SMR_OK || i4_ret == SMR_PENDING) {
        pt_player->e_sess_cond     = SM_SESS_COND_STARTING;
        pt_player->e_snk_oper_type = SNK_OPER_TYPE_PLAY;
#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_ret = pt_player->pf_nty(
            pt_player,
            PLAYER_NTFY_CODE_CHECK_QUEUED_REQ,
            pt_player->pv_tag,
            0);
#endif

        DBG_INFO((_INFO_HEADER"%s(), x_sm_play_sess(h_sess=%d)\r\n",
                        __func__,
                        pt_player->h_sess
                        ));

        TMS_BEGIN(PLAYER_TMS_SESS_START);
        i4_ret = SVCTXR_PENDING;
    } else if (i4_ret == SMR_ALREADY_PLAYED) {
        DBG_INFO((_INFO_HEADER"%s() already played\r\n", __func__));
        pt_player->e_sess_cond        = SM_SESS_COND_STARTED;
        pt_player->e_snk_oper_type = SNK_OPER_TYPE_PLAY;
        i4_ret = SVCTXR_OK;
    } else {
        DBG_ERROR((_ERROR_HEADER"x_sm_play_sess() failed, i4_ret=%d\r\n", i4_ret));
#ifdef MW_TV_AV_BYPASS_SUPPORT
        pt_player->e_action = PLAYER_ACTION_RECONNECT;
#endif
        i4_ret = _player_reconnect(pt_player);
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_close_sess
 * Description
 *      The API is used to close a running session.
 * Input arguments
 *      pt_this            the pointer points to a PLAYER_T object
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK
 *      SVCTXR_PENDING
 *-----------------------------------------------------------------------------*/
static INT32 _player_close_sess(
                    PLAYER_T*                   pt_player
                    )
{
    INT32 i4_ret;

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (pt_player->ui1_act_queued > 0)
    {
        DBG_INFO((_INFO_HEADER"%s: %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    /* close ruinning session */
    if (x_handle_valid(pt_player->h_sess) == FALSE) {
        return SVCTXR_OK;
    }

    if (pt_player->e_sess_cond == SM_SESS_COND_CLOSED) {
        return SVCTXR_OK;
    }

    if (pt_player->e_sess_cond == SM_SESS_COND_CLOSING) {
        return SVCTXR_PENDING;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_player->e_phase = PLAYER_ACTION_SESS_CLOSE;

    if (SVCTXR_QUEUED == _player_query_action(pt_player))
    {
        DBG_INFO((_INFO_HEADER"%s: _player_query_action() returned SVCTXR_QUEUED. %d action(s) being queued.\r\n",
            __func__,
            pt_player->ui1_act_queued));
        return SVCTXR_PENDING;
    }
#endif

    DBG_API((_API_HEADER"%s(pt_player=0x%x, %s)\r\n", __func__, pt_player, s_g_sess_conds[pt_player->e_sess_cond]));

    CLI_FUNC_CALL_TRACE("x_sm_close_sess");
    TMS_BEGIN(PLAYER_TMS_SESS_CLOSE);
    i4_ret = x_sm_close_sess(pt_player->h_sess);

    /*x_dbg_stmt("[%s] x_sm_close_sess(), handle=0x%x(%d)\r\n", 
        pt_player->s_snk_grp_name, pt_player->h_sess, pt_player->h_sess);*/

#ifdef MW_TV_AV_BYPASS_SUPPORT
    if (SMR_INV_HANDLE == i4_ret)
    {
        DBG_ERROR(("{SVCTX_MGR} Close session failed due to INV_HANDLE(0x%x)\r\n", pt_player->h_sess));
        pt_player->e_snk_oper_type = SNK_OPER_TYPE_CLOSE;
        return SVCTXR_OK;
    }
    else
#endif
    if (i4_ret != SMR_OK && i4_ret != SMR_PENDING) {
        DBG_ERROR((_ERROR_HEADER"x_sm_close_sess(%d) failed, ret=%d\r\n", pt_player->h_sess, i4_ret));
        return SVCTXR_FAIL;
    }
#ifdef MW_TV_AV_BYPASS_SUPPORT
    else
#endif
    {
        DBG_INFO((_INFO_HEADER"%s(), x_sm_close_sess(h_sess=%d)\r\n",
                        __func__,
                        pt_player->h_sess
                        ));

        pt_player->e_snk_oper_type = SNK_OPER_TYPE_CLOSE;
        pt_player->e_sess_cond = SM_SESS_COND_CLOSING;

        i4_ret = pt_player->pf_nty(
                        pt_player,
                        PLAYER_NTFY_CODE_SESS_CLOSING,
                        pt_player->pv_tag,
                        0
                        );
    }

    return SVCTXR_PENDING;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_sess_nfy_fct
 * Description
 *      The API is used to receive the notification of a session, and transfer
 *      to an internal event, and send to the attached application's event
 *      dispatcher for further processing.
 * Input arguments
 *      h_sess        the handle of a session that causes the notification
 *      e_sess_cond   the state of a session
 *      pv_sess_tag   an APP_EVN_SRC_T object
 *      e_event       the specific event to indicate more detail information of
 *                    the notification
 *      ui4_data      additional data
 * Output arguments
 *      None
 * Returns
 *      SMR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _player_sess_nfy_fct(
                    HANDLE_T                    h_sess,
                    SM_SESS_COND_T              e_sess_cond,
                    VOID*                       pv_sess_tag,
                    SM_EVN_TYPE_T               e_event,
                    UINT32                      ui4_data
                    )
{
    SVCTX_MSG_T t_msg     = {0};
    PLAYER_T*   pt_player = (PLAYER_T*)pv_sess_tag;

    if (pt_player == NULL || pt_player->pf_nty == NULL) {
        return SMR_OK;
    }

    t_msg.ui1_msg_id        = _PLAYER_MSG_SESS_NTFY;
    t_msg.pf_invoke_func    = _player_internal_msg;
    t_msg.pv_tag            = pv_sess_tag;
    t_msg.u.t_sess.h_obj    = h_sess;
    t_msg.u.t_sess.e_cond   = e_sess_cond;
    t_msg.u.t_sess.e_event  = e_event;
    t_msg.u.t_sess.ui4_data = ui4_data;

    pt_player->pf_nty(
                    pt_player,
                    PLAYER_NTFY_CODE_REQ_SEND_MSG,
                    pt_player->pv_tag,
                    (UINT32)&t_msg
                    );
    return SMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_handle_pipe_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _player_handle_pipe_msg (
                    PLAYER_T*                   pt_player,
                    HANDLE_T                    h_pipe,
                    CM_COND_T                   e_pipe_cond,
                    UINT32                      ui4_data
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s(pt_player=0x%x, h_pipe=%d/%d, %s, %s)\r\n",
                    __func__,
                    pt_player,
                    pt_player->h_pipe,
                    h_pipe,
                    s_g_cm_conds[e_pipe_cond],
                    s_g_src_oper_types[pt_player->e_src_oper_type]
                    ));

    if (pt_player->h_pipe != h_pipe) {
        return SVCTXR_OK;
    }

    pt_player->e_pipe_cond = e_pipe_cond;

    switch (e_pipe_cond) {
    case CM_COND_OPENED:
        /*x_dbg_stmt("[%s] CM_COND_OPENED, handle=0x%x(%d)\r\n", 
            pt_player->s_snk_grp_name, pt_player->h_pipe, pt_player->h_pipe);*/

#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_ret = pt_player->pf_nty(
                pt_player,
                PLAYER_NTFY_CODE_CHECK_QUEUED_REQ,
                pt_player->pv_tag,
                0
                );
#endif
        switch(pt_player->e_src_oper_type) {
        case SRC_OPER_TYPE_CONNECT:
            TMS_END(PLAYER_TMS_PIPE_OPEN);
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_CONN_OPEN;
#endif
            i4_ret = _player_open_conn(pt_player);
            break;
        case SRC_OPER_TYPE_DISCONNECT:
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_PIPE_CLOSE;
#endif
            i4_ret = _player_close_pipe(pt_player);
            break;
        default:
            break;
        }
        break;
    case CM_COND_CLOSED:
        /*x_dbg_stmt("[%s] CM_COND_CLOSED, handle=0x%x(%d)\r\n", 
            pt_player->s_snk_grp_name,  pt_player->h_pipe, pt_player->h_pipe);*/
        
#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_ret = pt_player->pf_nty(
                pt_player,
                PLAYER_NTFY_CODE_CHECK_QUEUED_REQ,
                pt_player->pv_tag,
                0
                );
#endif
        pt_player->h_pipe = NULL_HANDLE;
        switch(pt_player->e_src_oper_type) {
        case SRC_OPER_TYPE_CONNECT:
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_RECONNECT;
#endif
            i4_ret = _player_reconnect(pt_player);
            break;
        case SRC_OPER_TYPE_DISCONNECT:
            TMS_END(PLAYER_TMS_PIPE_CLOSE);
            pt_player->e_src_oper_type = SRC_OPER_TYPE_NONE;
            i4_ret = pt_player->pf_nty(
                    pt_player,
                    PLAYER_NTFY_CODE_PIPE_CLOSED,
                    pt_player->pv_tag,
                    0
                    );
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_handle_conn_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _player_handle_conn_msg (
                    PLAYER_T*                   pt_player,
                    HANDLE_T                    h_conn,
                    CM_COND_T                   e_conn_cond,
                    UINT32                      ui4_data
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s(pt_player=0x%x, h_conn=%d/%d, %s, %s)\r\n",
                    __func__,
                    pt_player,
                    pt_player->h_conn,
                    h_conn,
                    s_g_cm_conds[e_conn_cond],
                    s_g_src_oper_types[pt_player->e_src_oper_type]
                    ));

    if (pt_player->h_conn != h_conn) 
    {
        return SVCTXR_OK;
    }

    if (e_conn_cond != CM_COND_CONNECTION_INFO) 
    {
        if (pt_player->e_conn_cond == e_conn_cond) 
        {
            if (e_conn_cond != CM_COND_CONNECTION_ANA_FREQ)
            {
                return SVCTXR_OK;
            }
        }
        if(e_conn_cond < 0x80000000)
        {
            pt_player->e_conn_cond = e_conn_cond;
        }
    }

    switch(e_conn_cond) {
    case CM_COND_CONNECTED:

    /*x_dbg_stmt("[%s] CM_COND_CONNECTED, handle=0x%x(%d)\r\n", 
        pt_player->s_snk_grp_name, i4_ret,  h_conn, h_conn);*/

        x_timer_stop(pt_player->h_connect_timer);
#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_ret = pt_player->pf_nty(
                pt_player,
                PLAYER_NTFY_CODE_CHECK_QUEUED_REQ,
                pt_player->pv_tag,
                0
                );
#endif
        switch(pt_player->e_src_oper_type) {
        case SRC_OPER_TYPE_CONNECT:
            TMS_END(PLAYER_TMS_CM_CONN);
            i4_ret = pt_player->pf_nty(
                        pt_player,
                        PLAYER_NTFY_CODE_CONN_CONNECTED,
                        pt_player->pv_tag,
                        ui4_data
                        );
            if (i4_ret == SVCTXR_DONE) {
                DBG_INFO((_INFO_HEADER"%s(CM_COND_CONNECTED) is done\r\n", __func__));
                i4_ret = SVCTXR_OK;
                break;
            }

            i4_ret = _player_open_scdb(pt_player);
            if (i4_ret != SVCTXR_OK) {
                DBG_ERROR((_ERROR_HEADER"%s() _player_open_scdb failed, i4_ret = %d\r\n",
                        __func__,
                        i4_ret
                        ));
                break;
            }
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_SESS_OPEN;
#endif
            i4_ret = _player_open_sess(pt_player);
            break;
        case SRC_OPER_TYPE_DISCONNECT:
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_PIPE_CLOSE;
#endif
            i4_ret = _player_close_pipe(pt_player);
            break;
        default:
            break;
        }
        break;
    case CM_COND_DISCONNECTED:

    /*x_dbg_stmt("[%s] CM_COND_DISCONNECTED, handle=0x%x(%d)\r\n", 
        pt_player->s_snk_grp_name, h_conn, h_conn);*/
        
        x_timer_stop(pt_player->h_connect_timer);
        pt_player->h_conn = NULL_HANDLE;

        if (ui4_data != ANALOG_BRDCST_DISC_COMP_BUSY && ui4_data != BRDCST_DISC_COMP_BUSY)
        {
#ifdef MW_TV_AV_BYPASS_SUPPORT
            i4_ret = pt_player->pf_nty(
                    pt_player,
                    PLAYER_NTFY_CODE_CHECK_QUEUED_REQ,
                    pt_player->pv_tag,
                    0
                    );
#endif
            i4_ret = pt_player->pf_nty(
                            pt_player,
                            PLAYER_NTFY_CODE_CONN_DISCONNECTED,
                            pt_player->pv_tag,
                            (UINT32)(pt_player->e_src_oper_type == SRC_OPER_TYPE_CONNECT ? TRUE : FALSE)
                            );
        }
        switch(pt_player->e_src_oper_type) {
        case SRC_OPER_TYPE_CONNECT:
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_RECONNECT;
#endif
            i4_ret = _player_reconnect(pt_player);
            break;
        case SRC_OPER_TYPE_DISCONNECT:
            TMS_END(PLAYER_TMS_CM_DISCONN);
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_PIPE_CLOSE;
#endif
            i4_ret = _player_close_pipe(pt_player);
            break;
        default:
            break;
        }
        break;
    case CM_COND_CONNECTION_INFO:
        i4_ret = pt_player->pf_nty(
                        pt_player,
                        PLAYER_NTFY_CODE_CONN_INFO_NTFY,
                        pt_player->pv_tag,
                        ui4_data
                        );
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (CM_COND_CONNECTED == pt_player->e_conn_cond)
        {
            if (CM_CONN_INFO_GOT_TUNER_SIGNAL == ui4_data)
            {
                i4_ret = pt_player->pf_nty(
                            pt_player,
                            PLAYER_NTFY_CODE_CONN_FAKE_CONNECTED,
                            pt_player->pv_tag,
                            0
                            );
            }
            else if (CM_CONN_INFO_LOST_TUNER_SIGNAL == ui4_data)
            {
                i4_ret = pt_player->pf_nty(
                                pt_player,
                                PLAYER_NTFY_CODE_CONN_FAKE_DISCONNECTED,
                                pt_player->pv_tag,
                                (UINT32)(pt_player->e_src_oper_type == SRC_OPER_TYPE_CONNECT ? TRUE : FALSE)
                                );
            }
        }
#endif
        break;
    case CM_COND_CONNECTION_ANA_FREQ:
        /*Printf(__FILE__"[%d] is CM_COND_CONNECTION_ANA_FREQ freq=%d\r\n",__LINE__,ui4_data);*/
        i4_ret = pt_player->pf_nty(
                        pt_player,
                        PLAYER_NTFY_CODE_CONN_ANA_FREQ,
                        pt_player->pv_tag,
                        ui4_data
                        );
        break;
    default:
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_handle_scdb_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _player_handle_scdb_msg (
                    PLAYER_T*                   pt_player,
                    HANDLE_T                    h_scdb,
                    SCDB_COND_T                 e_scdb_cond,
                    UINT32                      ui4_reason,
                    UINT32                      ui4_data
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s(pt_player=0x%x, h_scdb=%d/%d, %s, %s)\r\n",
                    __func__,
                    pt_player,
                    pt_player->h_scdb,
                    h_scdb,
                    s_g_scdb_conds[e_scdb_cond],
                    s_g_src_oper_types[pt_player->e_src_oper_type]
                    ));

    if (pt_player->h_scdb != h_scdb) {
        return SVCTXR_OK;
    }

    pt_player->e_scdb_cond = e_scdb_cond;

    switch(e_scdb_cond) {
    case SCDB_UPDATED:
        i4_ret = pt_player->pf_nty(
                    pt_player,
                    PLAYER_NTFY_CODE_SCDB_UPDATED,
                    pt_player->pv_tag,
                    ui4_reason
                    );
        break;
    case SCDB_CLOSED:
        pt_player->h_scdb = NULL_HANDLE;
        switch(pt_player->e_src_oper_type) {
        case SRC_OPER_TYPE_CONNECT:
            /* should be signal loss */
            break;
        case SRC_OPER_TYPE_DISCONNECT:
            TMS_END(PLAYER_TMS_SCDB_CLOSE);
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_PIPE_CLOSE;
#endif
            i4_ret = _player_close_pipe(pt_player);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_handle_sess_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _player_handle_sess_msg (
                    PLAYER_T*                   pt_player,
                    HANDLE_T                    h_sess,
                    SM_SESS_COND_T              e_sess_cond,
                    SM_EVN_TYPE_T               e_event,
                    UINT32                      ui4_data
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    DBG_INFO((_INFO_HEADER"%s(pt_player=0x%x, h_sess=%d/%d, %s, %s)\r\n",
                    __func__,
                    pt_player,
                    pt_player->h_sess,
                    h_sess,
                    s_g_sess_conds[e_sess_cond],
                    s_g_snk_oper_types[pt_player->e_snk_oper_type]
                    ));

    if (pt_player->h_sess != h_sess) {
        return SVCTXR_OK;
    }

    pt_player->e_sess_cond = e_sess_cond;

    switch(e_sess_cond) {
    case SM_SESS_COND_OPENED:
#ifdef MW_TV_AV_BYPASS_SUPPORT
        if (e_event != SM_EVN_SESS_DRV_STOPPED)
#endif
        {
#ifdef MW_TV_AV_BYPASS_SUPPORT
            i4_ret = pt_player->pf_nty(
                    pt_player,
                    PLAYER_NTFY_CODE_CHECK_QUEUED_REQ,
                    pt_player->pv_tag,
                    0
                    );
#endif
            switch(pt_player->e_snk_oper_type) {
            case SNK_OPER_TYPE_PLAY:
            case SNK_OPER_TYPE_OPEN:
                TMS_END(PLAYER_TMS_SESS_OPEN);
                i4_ret = pt_player->pf_nty(
                        pt_player,
                        PLAYER_NTFY_CODE_SESS_OPENED,
                        pt_player->pv_tag,
                        0
                        );
                break;
            default:
                break;
            }
        }
        break;
    case SM_SESS_COND_STARTED:
    /*x_dbg_stmt("[%s] SM_SESS_COND_STARTED, handle=0x%x(%d)\r\n", 
        pt_player->s_snk_grp_name,  h_sess, h_sess);*/
        
#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_ret = pt_player->pf_nty(
                pt_player,
                PLAYER_NTFY_CODE_CHECK_QUEUED_REQ,
                pt_player->pv_tag,
                0
                );
#endif
        switch(pt_player->e_snk_oper_type) {
        case SNK_OPER_TYPE_PLAY:
            TMS_END(PLAYER_TMS_SESS_START);
            i4_ret = pt_player->pf_nty(
                    pt_player,
                    PLAYER_NTFY_CODE_SESS_STARTED,
                    pt_player->pv_tag,
                    0
                    );
            break;
        case SNK_OPER_TYPE_STOP:
            DBG_INFO((_INFO_HEADER"received SM_SESS_COND_STARTED after stop session!\r\n"));
            i4_ret = player_pause(pt_player);
            break;
        case SNK_OPER_TYPE_CLOSE:
            DBG_INFO((_INFO_HEADER"received SM_SESS_COND_STARTED after close session!\r\n"));
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_SESS_CLOSE;
#endif
            i4_ret = _player_close_sess(pt_player);
            break;
        default:
            break;
        }
        break;
    case SM_SESS_COND_CLOSED:
        /*x_dbg_stmt("[%s] SM_SESS_COND_CLOSED, handle=0x%x(%d)\r\n", 
        pt_player->s_snk_grp_name,  h_sess, h_sess);*/
        
        pt_player->h_sess = NULL_HANDLE;
#ifdef MW_TV_AV_BYPASS_SUPPORT
        i4_ret = pt_player->pf_nty(
                pt_player,
                PLAYER_NTFY_CODE_CHECK_QUEUED_REQ,
                pt_player->pv_tag,
                0
                );
#endif
        switch(pt_player->e_snk_oper_type) {
        case SNK_OPER_TYPE_PLAY:
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_SESS_PLAY;
#endif
            i4_ret = _player_play_sess(pt_player);
            break;
        case SNK_OPER_TYPE_CLOSE:
            TMS_END(PLAYER_TMS_SESS_CLOSE);
            pt_player->e_snk_oper_type = SNK_OPER_TYPE_NONE;
            if (pt_player->e_src_oper_type == SRC_OPER_TYPE_CONNECT) {
#ifdef MW_TV_AV_BYPASS_SUPPORT
                pt_player->e_action = PLAYER_ACTION_RECONNECT;
#endif
                i4_ret = _player_reconnect(pt_player);
            } else {
#ifdef MW_TV_AV_BYPASS_SUPPORT
                pt_player->e_action = PLAYER_ACTION_PIPE_CLOSE;
#endif
                i4_ret = _player_close_pipe(pt_player);
                if (i4_ret == SVCTXR_OK) {
                    pt_player->e_src_oper_type = SRC_OPER_TYPE_NONE;
                    pt_player->pf_nty(
                            pt_player,
                            PLAYER_NTFY_CODE_PIPE_CLOSED,
                            pt_player->pv_tag,
                            0
                            );
                }
            }
            break;
        case SNK_OPER_TYPE_STOP:
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_SESS_OPEN;
#endif
            i4_ret = _player_open_sess(pt_player);
            pt_player->e_snk_oper_type = SNK_OPER_TYPE_STOP;
            break;
        case SNK_OPER_TYPE_OPEN:
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_SESS_OPEN;
#endif
            i4_ret = _player_open_sess(pt_player);
            break;
        default:
            break;
        }
        break;
    case SM_SESS_COND_ERROR:
        switch(pt_player->e_snk_oper_type) {
        case SNK_OPER_TYPE_PLAY:
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_SESS_CLOSE;
#endif
            i4_ret = _player_close_sess(pt_player);
            if (i4_ret == SVCTXR_OK) {
#ifdef MW_TV_AV_BYPASS_SUPPORT
                pt_player->e_action = PLAYER_ACTION_SESS_PLAY;
#endif
                i4_ret = _player_play_sess(pt_player);
            }
            break;
        case SNK_OPER_TYPE_OPEN:
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_SESS_CLOSE;
#endif
            i4_ret = _player_close_sess(pt_player);
            if (i4_ret == SVCTXR_OK) {
#ifdef MW_TV_AV_BYPASS_SUPPORT
                pt_player->e_action = PLAYER_ACTION_SESS_OPEN;
#endif
                i4_ret = _player_open_sess(pt_player);
            }
            break;
        case SNK_OPER_TYPE_CLOSE:
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_SESS_CLOSE;
#endif
            i4_ret = _player_close_sess(pt_player);
            break;
        case SNK_OPER_TYPE_STOP:
#ifdef MW_TV_AV_BYPASS_SUPPORT
            pt_player->e_action = PLAYER_ACTION_SESS_CLOSE;
#endif
            i4_ret = _player_close_sess(pt_player);
            if (i4_ret == SVCTXR_OK) {
#ifdef MW_TV_AV_BYPASS_SUPPORT
                pt_player->e_action = PLAYER_ACTION_SESS_OPEN;
#endif
                i4_ret = _player_open_sess(pt_player);
                pt_player->e_snk_oper_type = SNK_OPER_TYPE_STOP;
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_internal_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _player_internal_msg (
                    VOID*                       pv_tag,
                    SVCTX_MSG_T*                pt_msg
                    )
{
    PLAYER_T* pt_player = (PLAYER_T*)pv_tag;
    INT32     i4_ret    = SVCTXR_OK;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_EXIST;
    }

    if (player_valid(pt_player) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s(), invalid player = 0x%x\r\n", __func__, pt_player));
        return SVCTXR_NOT_EXIST;
    }

    svctx_cli_print_player_nfy(pt_player, pt_msg);

    switch(pt_msg->ui1_msg_id) {
    case _PLAYER_MSG_PIPE_NTFY:
        i4_ret = _player_handle_pipe_msg(
                    pt_player,
                    pt_msg->u.t_pipe.h_obj,
                    pt_msg->u.t_pipe.e_cond,
                    pt_msg->u.t_pipe.ui4_data
                    );
        break;
    case _PLAYER_MSG_CONN_NTFY:
        i4_ret = _player_handle_conn_msg(
                    pt_player,
                    pt_msg->u.t_conn.h_obj,
                    pt_msg->u.t_conn.e_cond,
                    pt_msg->u.t_conn.ui4_data
                    );
        break;
    case _PLAYER_MSG_SCDB_NTFY:
        i4_ret = _player_handle_scdb_msg(
                    pt_player,
                    pt_msg->u.t_scdb.h_obj,
                    pt_msg->u.t_scdb.e_cond,
                    pt_msg->u.t_scdb.ui4_reason,
                    pt_msg->u.t_scdb.ui4_data
                    );
        break;
    case _PLAYER_MSG_SESS_NTFY:
        i4_ret = _player_handle_sess_msg(
                    pt_player,
                    pt_msg->u.t_sess.h_obj,
                    pt_msg->u.t_sess.e_cond,
                    pt_msg->u.t_sess.e_event,
                    pt_msg->u.t_sess.ui4_data
                    );
        break;
    case _PLAYER_MSG_RECONN_DELAY_NTFY:
        i4_ret = _player_handle_reconnect_timeout(
                    pt_player,
                    pt_msg->u.t_timer.h_obj
                    );
        break;
    case _PLAYER_MSG_CONNECT_TIMEOUT_NTFY:
        i4_ret = _player_handle_connect_timeout(
                    pt_player,
                    pt_msg->u.t_timer.h_obj
                    );
        break;
#ifdef MW_TV_AV_BYPASS_SUPPORT
    case _PLAYER_MSG_QUEUED_PLAY_NTFY:
        pt_player->ui1_act_queued--;
        i4_ret = player_play(pt_player, pt_player->i4_speed);
        break;

    case _PLAYER_MSG_QUEUED_STOP_NTFY:
        pt_player->ui1_act_queued--;
        i4_ret = player_stop(pt_player);
        break;

    case _PLAYER_MSG_QUEUED_PAUSE_NTFY:
        pt_player->ui1_act_queued--;
        i4_ret = player_pause(pt_player);
        break;

    case _PLAYER_MSG_QUEUED_PIPE_OPEN_NTFY:
        pt_player->ui1_act_queued--;
        pt_player->e_action = PLAYER_ACTION_PIPE_OPEN;
        i4_ret = _player_open_pipe(pt_player);
        break;

    case _PLAYER_MSG_QUEUED_PIPE_CLOSE_NTFY:
        pt_player->ui1_act_queued--;
        pt_player->e_action = PLAYER_ACTION_PIPE_CLOSE;
        i4_ret = _player_close_pipe(pt_player);
        break;

    case _PLAYER_MSG_QUEUED_CONN_OPEN_NTFY:
        pt_player->ui1_act_queued--;
        pt_player->e_action = PLAYER_ACTION_CONN_OPEN;
        i4_ret = _player_open_conn(pt_player);
        break;

    case _PLAYER_MSG_QUEUED_CONN_CLOSE_NTFY:
        pt_player->ui1_act_queued--;
        pt_player->e_action = PLAYER_ACTION_CONN_CLOSE;
        i4_ret = _player_close_conn(pt_player);
        break;

    case _PLAYER_MSG_QUEUED_SESS_OPEN_NTFY:
        pt_player->ui1_act_queued--;
        pt_player->e_action = PLAYER_ACTION_SESS_OPEN;
        i4_ret = _player_open_sess(pt_player);
        break;

    case _PLAYER_MSG_QUEUED_SESS_PLAY_NTFY:
        pt_player->ui1_act_queued--;
        pt_player->e_action = PLAYER_ACTION_SESS_PLAY;
        i4_ret = _player_play_sess(pt_player);
        break;

    case _PLAYER_MSG_QUEUED_SESS_CLOSE_NTFY:
        pt_player->ui1_act_queued--;
        pt_player->e_action = PLAYER_ACTION_SESS_CLOSE;
        i4_ret = _player_close_sess(pt_player);
        break;

    case _PLAYER_MSG_QUEUED_RECONNECT_NTFY:
        pt_player->ui1_act_queued--;
        pt_player->e_action = PLAYER_ACTION_RECONNECT;
        i4_ret = _player_reconnect(pt_player);
        break;

    case _PLAYER_MSG_QUEUED_NONE:
        pt_player->ui1_act_queued--;
        break;
#endif
    default:
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _player_handle_reconnect_timeout
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _player_handle_reconnect_timeout (
                    PLAYER_T*                   pt_player,
                    HANDLE_T                    h_timer
                    )
{
    INT32 i4_ret;

    DBG_API((_API_HEADER"%s(pt_player=0x%x, %s, %s)\r\n",
                    __func__,
                    pt_player,
                    s_g_src_oper_types[pt_player->e_src_oper_type],
                    s_g_snk_oper_types[pt_player->e_snk_oper_type]
                    ));

    if (h_timer != pt_player->h_connect_timer) {
        return SVCTXR_OK;
    }

    if (pt_player->e_src_oper_type == SRC_OPER_TYPE_DISCONNECT) {
        return SVCTXR_OK;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_player->e_action = PLAYER_ACTION_CONN_CLOSE;
#endif
    i4_ret = _player_close_conn(pt_player);
    if (i4_ret != SVCTXR_OK) {
        return i4_ret;
    }

#ifdef MW_TV_AV_BYPASS_SUPPORT
    pt_player->e_action = PLAYER_ACTION_CONN_OPEN;
#endif
    i4_ret = _player_open_conn(pt_player);
    return i4_ret;
}

