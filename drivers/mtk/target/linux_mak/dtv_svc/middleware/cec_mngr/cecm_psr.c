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
 * $RCSfile: cecm_psr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/14 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: a872122d22ab4b982b54b29e95d0ff55 $
 *
 * Description:
 *         This is CEC Manager parser source file.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "dbg/x_dbg.h"
#include "dbg/x_dbg_drv.h"
#include "dbg/def_dbg_level_mw.h"
#include "util/x_lnk_list.h"
#include "unicode/x_uc_str.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_cec.h"
#include "cec_mngr/cecm.h"
#include "cec_mngr/cecm_lock.h"
#include "cec_mngr/cecm_ctrl.h"
#include "cec_mngr/cecm_psr.h"
#include "cec_mngr/x_cecm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define CECM_TIMEOUT_FRAME                  ((UINT32) 3000) /* ms */
#define CECM_TIMEOUT_ONE_TOUCH_PLAY         ((UINT32) 2000) /* ms */

#ifdef CECM_PWR_ON_DEV_EX
#define CECM_TIMEOUT_PWR_ON_DEV             ((UINT32)  100) /* sec */
#define CECM_TIMEOUT_PWR_ON_DEV_RETRY       ((UINT32)    5) /* sec */
#else
#define CECM_PWR_STS_STANDBY_TO_ON_ITVL     ((UINT32)  2000) /* ms */
#endif


#define CECM_DEFAULT_PA_DISCONTINUITY 500


/* One touch play state */
typedef enum
{
    CECM_OTP_NONE        = 0,
    CECM_OTP_REQ_ACT_SRC = 1,
    CECM_OTP_GIVE_PA     = 2
}   CECM_OTP_STATE_T;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static HANDLE_T*                ph_cecm_msgq = NULL;
static HANDLE_T                 h_timer_opt = NULL_HANDLE;
static CECM_EVT_T*              pt_evt_one_touch_play = NULL;
static CECM_OTP_STATE_T         e_otp_state = CECM_OTP_NONE;
#if 0 /* Do not issue <Request Active Source> when <Set Stream Path> is failed */
static BOOL                     b_ras_4_ssp = FALSE;
#endif
static UINT8                    ui1_dev_post_polling_cnt = 0;
static BOOL                     b_pp_runner_dev_poll_next_dev = FALSE;
static CECM_DEV_DISCOVERY_CONFIG_T   t_dd_config;

#ifdef CECM_PWR_ON_DEV_EX
/* For select_device */
static HANDLE_T                 h_timer_pwr_on_dev = NULL_HANDLE;
static HANDLE_T                 h_timer_pwr_on_dev_retry = NULL_HANDLE;
static BOOL                     b_pwr_on_dev = FALSE;
static BOOL                     b_recv_1st_pwr_on_dev_ack = FALSE;
static BOOL                     b_recv_pwr_standby = FALSE;
static CECM_USER_CTRL_T         e_usr_ctrl = CECM_USER_CTRL_POWER;
#else

#endif

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
static VOID _cecm_timer_frame_cb(HANDLE_T h_timer,
                                 VOID*    pv_tag);

static VOID _cecm_timer_one_touch_play_cb(HANDLE_T h_timer,
                                          VOID*    pv_tag);

#ifdef CECM_PWR_ON_DEV_EX
static VOID _cecm_timer_pwr_on_dev_cb(HANDLE_T h_timer,
                                      VOID*    pv_tag);

static VOID _cecm_timer_pwr_on_dev_retry_cb(HANDLE_T h_timer,
                                            VOID*    pv_tag);
#endif

static BOOL _cecm_is_response_required(CECM_OPCODE_T e_opcode);

static VOID _cecm_notify_client(CECM_NFY_T* pt_nfy_data,
                                CECM_EVT_T* pt_evt,
                                BOOL        b_remove_evt);

static VOID _cecm_start_post_polling(VOID* pv_data,
                                     BOOL  b_evt);

static VOID _cecm_update_dev_discovery_mask(CECM_EVT_T*     pt_evt,
                                            CECM_DEV_REC_T* pt_dev_rec);

#if 0
static VOID _cecm_discover_a_dev(CECM_LOG_ADDR_T e_la);
#endif

#ifdef CECM_PWR_ON_DEV_EX
static BOOL _cecm_pwr_on_dev_send_msg(CECM_OPCODE_T e_opcode,
                                      CECM_EVT_T*   pt_evt,
                                      UINT32*       pui4_data);
#endif

static VOID _cecm_handle_otp_ssp(CECM_EVT_T* pt_evt);

static VOID _cecm_handle_ack_timeout(VOID* pv_tag);

static VOID _cecm_handle_frame_timeout(VOID* pv_tag);

static VOID _cecm_handle_one_touch_play_timeout(VOID* pv_tag);

#ifdef CECM_PWR_ON_DEV_EX
static VOID _cecm_handle_select_dev_timeout(VOID* pv_tag);

static VOID _cecm_handle_pwr_on_dev_timeout(VOID* pv_tag);

static VOID _cecm_handle_pwr_on_dev_retry_timeout(VOID* pv_tag);
#else
static VOID _cecm_handle_select_dev_timeout(VOID* pv_tag);
#endif

static VOID _cecm_parse_act_src(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_inact_src(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_req_act_src(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_img_txt_view_on(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_routing_info(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_standby(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_give_phy_addr(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_report_phy_addr(CEC_FRAME_INFO_T* pt_frame_info,
                                        BOOL              b_mw_cec_nfy);

static VOID _cecm_parse_set_osd_string(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_set_osd_name(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_dev_vndr_id(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_give_dev_pwr_sts(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_report_pwr_sts(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_get_menu_lang(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_set_menu_lang(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_get_cec_ver(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_cec_ver(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_give_deck_sts(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_deck_sts(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_menu_sts(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_give_dev_vndr_id(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_set_sys_aud_mode(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_report_aud_sts(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_sys_aud_mode_sts(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_timer_sts(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_timer_clred_sts(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_tuner_dev_sts(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_rec_sts(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_rec_tv_screen(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_feature_abort(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_abort(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_routing_change(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_give_osd_name(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_user_ctrl_pressed(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_user_ctrl_released(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_vndr_specific_cmd(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_parse_handle_unsupported_opcode(CEC_FRAME_INFO_T* pt_frame_info);

static VOID _cecm_process_ack_msg(CEC_ACK_INFO_T* pt_ack_info);

static VOID _cecm_process_frame_msg(CEC_FRAME_INFO_T* pt_frame_info,
                                    BOOL              b_mw_cec_nfy);

/*-----------------------------------------------------------------------------
 * Name: _cecm_timer_frame_cb
 *
 * Description: This callback function is called when frame timer is
 *              expired.
 *
 * Inputs:  h_timer     A handle referencing the timer.
 *          pv_tag      References a private tag.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_timer_frame_cb(HANDLE_T h_timer,
                                 VOID*    pv_tag)
{
    CECM_EVT_T*     pt_evt;
    CECM_MSG_T      t_msg;

    /* Check if the event is valid */
    pt_evt = (CECM_EVT_T*)(pv_tag);
    if ((pt_evt == NULL) || (pt_evt->e_type == CECM_EVT_TYPE_NONE))
    {
        return;
    }

    t_msg.e_msg_type = CECM_MSG_TYPE_FRAME_TIMEOUT;
    t_msg.u.pv_tag = pv_tag;

    x_msg_q_send(*ph_cecm_msgq,
                 (VOID*)&t_msg,
                 sizeof(CECM_MSG_T),
                 CECM_MSGQ_DEFAULT_PRIORITY);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_timer_one_touch_play_cb
 *
 * Description: This callback function is called when one touch play timer is
 *              expired.
 *
 * Inputs:  h_timer     A handle referencing the timer.
 *          pv_tag      References a private tag.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_timer_one_touch_play_cb(HANDLE_T h_timer,
                                          VOID*    pv_tag)
{
    CECM_EVT_T*     pt_evt;
    CECM_MSG_T      t_msg;

    /* Check if the event is valid */
    pt_evt = (CECM_EVT_T*)(pv_tag);
    if ((pt_evt == NULL) || (pt_evt->e_type == CECM_EVT_TYPE_NONE))
    {
        return;
    }

    t_msg.e_msg_type = CECM_MSG_TYPE_ONE_TOUCH_PLAY_TIMEOUT;
    t_msg.u.pv_tag = pv_tag;

    x_msg_q_send(*ph_cecm_msgq,
                 (VOID*)&t_msg,
                 sizeof(CECM_MSG_T),
                 CECM_MSGQ_DEFAULT_PRIORITY);
}


#ifdef CECM_PWR_ON_DEV_EX
/*-----------------------------------------------------------------------------
 * Name: _cecm_timer_pwr_on_dev_cb
 *
 * Description: This callback function is called when power_on_device timer is
 *              expired.
 *
 * Inputs:  h_timer     A handle referencing the timer.
 *          pv_tag      References a private tag.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_timer_pwr_on_dev_cb(HANDLE_T h_timer,
                                      VOID*    pv_tag)
{
    CECM_MSG_T      t_msg;

    if (b_pwr_on_dev == FALSE)
    {
        return;
    }

    t_msg.e_msg_type = CECM_MSG_TYPE_PWR_ON_DEV_TIMEOUT;
    t_msg.u.pv_tag = pv_tag;

    x_msg_q_send(*ph_cecm_msgq,
                 (VOID*)&t_msg,
                 sizeof(CECM_MSG_T),
                 CECM_MSGQ_DEFAULT_PRIORITY);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_timer_pwr_on_dev_retry_cb
 *
 * Description: This callback function is called when power_on_device retry
 *              timer is expired.
 *
 * Inputs:  h_timer     A handle referencing the timer.
 *          pv_tag      References a private tag.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_timer_pwr_on_dev_retry_cb(HANDLE_T h_timer,
                                            VOID*    pv_tag)
{
    CECM_MSG_T      t_msg;

    if (!cecm_ctrl_is_select_dev_running())
    {
        return;
    }

    t_msg.e_msg_type = CECM_MSG_TYPE_PWR_ON_DEV_RETRY_TIMEOUT;
    t_msg.u.pv_tag = pv_tag;

    x_msg_q_send(*ph_cecm_msgq,
                 (VOID*)&t_msg,
                 sizeof(CECM_MSG_T),
                 CECM_MSGQ_DEFAULT_PRIORITY);

}
#endif


/*-----------------------------------------------------------------------------
 * Name: _cecm_is_response_required
 *
 * Description: This API checks if response is required for a specific opcode.
 *
 * Inputs:  e_opcode    Contains an opcode.
 *
 * Outputs: -
 *
 * Returns: TRUE        Response is required.
 *          FALSE       There is no response.
 ----------------------------------------------------------------------------*/
static BOOL _cecm_is_response_required(CECM_OPCODE_T e_opcode)
{
    BOOL    b_response;

    switch (e_opcode)
    {
        /* Response is required */
        case CECM_OPCODE_GIVE_TUNER_DEV_STS:
        case CECM_OPCODE_REC_ON:
        case CECM_OPCODE_GIVE_DECK_STS:
        case CECM_OPCODE_CLR_ANA_TIMER:
        case CECM_OPCODE_SET_ANA_TIMER:
        case CECM_OPCODE_GIVE_OSD_NAME:
        case CECM_OPCODE_SYS_AUD_MOD_REQ:
        case CECM_OPCODE_GIVE_AUD_STS:
        case CECM_OPCODE_GIVE_SYS_AUD_MOD_STS:
        case CECM_OPCODE_ROUTING_CHG:
        case CECM_OPCODE_GIVE_PHY_ADDR:
        case CECM_OPCODE_REQ_ACT_SRC:
        case CECM_OPCODE_SET_STREAM_PATH:
        case CECM_OPCODE_GIVE_DEV_VNDR_ID:
        case CECM_OPCODE_MENU_REQ:
        case CECM_OPCODE_GIVE_DEV_PWR_STS:
        case CECM_OPCODE_GET_MENU_LANG:
        case CECM_OPCODE_SET_DIG_TIMER:
        case CECM_OPCODE_CLR_DIG_TIMER:
        case CECM_OPCODE_GET_CEC_VER:
        case CECM_OPCODE_CLR_EXT_TIMER:
        case CECM_OPCODE_SET_EXT_TIMER:
            b_response = TRUE;
            break;

        /* Response is not required */
        case CECM_OPCODE_FEATURE_ABORT:
        case CECM_OPCODE_TUNER_STEP_INCR:
        case CECM_OPCODE_TUNER_STEP_DECR:
        case CECM_OPCODE_REC_OFF:
        case CECM_OPCODE_DECK_STS:
        case CECM_OPCODE_SET_MENU_LANG:
        case CECM_OPCODE_STANDBY:
        case CECM_OPCODE_PLAY:
        case CECM_OPCODE_DECK_CTRL:
        case CECM_OPCODE_USER_CTRL_PRESSED:
        case CECM_OPCODE_USER_CTRL_RELEASED:
        case CECM_OPCODE_SET_TIMER_PROG_TITLE:
        case CECM_OPCODE_ACT_SRC:
        case CECM_OPCODE_REPORT_PHY_ADDR:
        case CECM_OPCODE_DEV_VNDR_ID:
        case CECM_OPCODE_VNDR_CMD:
        case CECM_OPCODE_VNDR_CMD_WITH_ID:
        case CECM_OPCODE_VNDR_REMOTE_BTN_DOWN:
        case CECM_OPCODE_VNDR_REMOTE_BTN_UP:
        case CECM_OPCODE_REPORT_PWR_STS:
        case CECM_OPCODE_SELECT_ANA_SVC:
        case CECM_OPCODE_SELECT_DIG_SVC:
        case CECM_OPCODE_SET_AUD_RATE:
        case CECM_OPCODE_CEC_VER:
        case CECM_OPCODE_ABORT:
        case CECM_OPCODE_NONE:
        default:
            b_response = FALSE;
    }

    return (b_response);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_notify_client
 *
 * Description:
 *
 * Inputs:  pt_nfy_data     References the notification data.
 *          pt_evt          References an event.
 *          b_remove_evt    Indicates if the event should be removed.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_notify_client(CECM_NFY_T* pt_nfy_data,
                                CECM_EVT_T* pt_evt,
                                BOOL        b_remove_evt)
{
    if (!cecm_ctrl_is_cec_fct_enable() || (pt_nfy_data == NULL))
    {
        if (b_remove_evt && (pt_evt != NULL))
        {
            cecm_ctrl_evt_free(pt_evt);
        }

        return;
    }

    if (pt_evt == NULL)
    {
        cecm_ctrl_notify_all_clients(pt_nfy_data);
    }
    else
    {
        /* Check if the event is valid */
        if (pt_evt->e_type == CECM_EVT_TYPE_NONE)
        {
            return;
        }
        else if (pt_evt->e_type == CECM_EVT_TYPE_ONE_TOUCH_PLAY)
        {
            cecm_ctrl_notify_all_clients(pt_nfy_data);
        }
        else
        {
            if (pt_evt->pt_svc->pf_nfy != NULL)
            {
                (pt_evt->pt_svc->pf_nfy)(pt_nfy_data, pt_evt->pt_svc->pv_tag, 0);
            }
        }

        if (b_remove_evt)
        {
            cecm_ctrl_evt_free(pt_evt);
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_start_post_polling
 *
 * Description: This API starts post-polling operation (i.e. <Give Physical
 *              Address>, <Give Device Vendor ID>, <Give OSD Name>).
 *
 * Inputs:  pv_data     References an event or device discovery index.
 *          b_evt       Indicates pv_data referencing an event or a device
 *                      discovery index.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_start_post_polling(VOID* pv_data,
                                     BOOL  b_evt)
{
    INT32                   i4_return;
    CECM_EVT_T*             pt_evt = NULL;
    CECM_DEV_REC_T*         pt_dev_rec;
    CEC_FRAME_INFO_T        t_frame_info;
    HANDLE_T                h_svc;
    CECM_DEV_DISCOVERY_T    t_discvry_info;

    if (b_evt)
    {
        pt_evt = (CECM_EVT_T*)pv_data;
        pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_evt->e_la, FALSE);
        h_svc = pt_evt->pt_svc->h_svc;

        if ((pt_dev_rec == NULL) || (h_svc == NULL_HANDLE))
        {
            return;
        }

        t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
        t_frame_info.ui1_dest_addr = (UINT8)(pt_evt->e_la);
    }
    else
    {
        pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(*(UINT8*)pv_data), FALSE);
        h_svc = cecm_ctrl_get_internal_handle();

        t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
        t_frame_info.ui1_dest_addr = *(UINT8*)(pv_data);
    }

    cecm_lock_wr_db();
    if ((pt_dev_rec->ui4_update_mask & CECM_UPDATE_MASK_ALL) == CECM_UPDATE_MASK_NONE)
    {
        if (pt_dev_rec->b_post_polling_handled)
        {
            /* Issue next device discovery */
            cecm_unlock_db();
            cecm_ctrl_poll_next_dev();
            return;
        }
        else
        {
            /* TV runs post-polling operation */
            pt_dev_rec->b_post_polling_handled = TRUE;
        }

        if((t_dd_config.t_dd_get_info.b_phy_address == FALSE) ||
           ((pt_dev_rec->ui4_skip_update_mask & CECM_UPDATE_MASK_PA) == CECM_UPDATE_MASK_PA))
        {
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_PA;
            cecm_unlock_db();
            _cecm_start_post_polling(pv_data, b_evt);
            return;           
        }

        /* Query PA */
        t_frame_info.ui2_opcode = CECM_OPCODE_GIVE_PHY_ADDR;
        t_frame_info.aui1_operand[0] = 0;
        t_frame_info.z_operand_size = 0;

        i4_return = cecm_ctrl_send_msg(h_svc, CECM_EVT_TYPE_DEV_DISCOVERY, &t_frame_info, NULL);

        if (i4_return != TRUE)
        {
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_PA;
            cecm_unlock_db();
            _cecm_start_post_polling(pv_data, b_evt);
            return;
        }
        cecm_unlock_db();
    }
    else if ((pt_dev_rec->ui4_update_mask & CECM_UPDATE_MASK_ALL) == CECM_UPDATE_MASK_PA)
    {
        /* Query device vendor ID */
        if((t_dd_config.t_dd_get_info.b_dev_vndr_id == FALSE) || 
           ((pt_dev_rec->ui4_skip_update_mask & CECM_UPDATE_MASK_DEV_VNDR_ID) == CECM_UPDATE_MASK_DEV_VNDR_ID))
        {
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_DEV_VNDR_ID;
            cecm_unlock_db();
            _cecm_start_post_polling(pv_data, b_evt);
            return;           
        }
        
        t_frame_info.ui2_opcode = CECM_OPCODE_GIVE_DEV_VNDR_ID;
        t_frame_info.aui1_operand[0] = 0;
        t_frame_info.z_operand_size = 0;

        i4_return = cecm_ctrl_send_msg(h_svc, CECM_EVT_TYPE_DEV_DISCOVERY, &t_frame_info, NULL);

        if (i4_return != TRUE)
        {
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_DEV_VNDR_ID;
            cecm_unlock_db();
            _cecm_start_post_polling(pv_data, b_evt);
            return;
        }
        cecm_unlock_db();
    }
    else if((pt_dev_rec->ui4_update_mask & CECM_UPDATE_MASK_ALL) == 
            (CECM_UPDATE_MASK_PA | CECM_UPDATE_MASK_DEV_VNDR_ID))
    {
        
        /*Query vendor info*/      
        CECM_DEV_VNDR_ID_T      t_comp_dev_vndr_id;
        x_memset(&t_comp_dev_vndr_id, 0, CECM_DEV_VNDR_ID_SIZE);
        if((t_dd_config.t_dd_get_info.b_dev_vndr_info == FALSE) ||
           (x_memcmp((VOID*)&t_dd_config.t_dev_vndr_id, (VOID*)&pt_dev_rec->t_dev.t_dev_vndr_id, CECM_DEV_VNDR_ID_SIZE) != 0 && 
            x_memcmp((VOID*)&t_dd_config.t_dev_vndr_id, (VOID*)&t_comp_dev_vndr_id, CECM_DEV_VNDR_ID_SIZE) != 0) ||
           (pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)CECM_DD_VNDR_CMD_MAX_SIZE ) ||
           (pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)t_dd_config.t_dd_vndr_cmd.z_dd_vndr_cmd_size)  ||
           ((pt_dev_rec->ui4_skip_update_mask & CECM_UPDATE_MASK_DEV_VNDR_INFO) == CECM_UPDATE_MASK_DEV_VNDR_INFO))
        {
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_DEV_VNDR_INFO;
            cecm_unlock_db();
            _cecm_start_post_polling(pv_data, b_evt);
            return;           
        } 
        if(t_dd_config.t_dd_vndr_cmd.at_vndr_cmd_info[pt_dev_rec->ui1_vndr_cmd_idx].e_vndr_cmd_type == CECM_VNDR_CMD)
        {
            /* Send Device discovery vendro command*/
            t_frame_info.ui2_opcode = CECM_OPCODE_VNDR_CMD;
            x_memcpy((VOID*)&t_frame_info.aui1_operand[0],
                     (VOID*)&t_dd_config.t_dd_vndr_cmd.at_vndr_cmd_info[pt_dev_rec->ui1_vndr_cmd_idx].u_data.t_vndr_cmd_data.t_vndr_cmd,
                     t_dd_config.t_dd_vndr_cmd.at_vndr_cmd_info[pt_dev_rec->ui1_vndr_cmd_idx].u_data.t_vndr_cmd_data.z_vndr_cmd_size);
            t_frame_info.z_operand_size = t_dd_config.t_dd_vndr_cmd.at_vndr_cmd_info[pt_dev_rec->ui1_vndr_cmd_idx].u_data.t_vndr_cmd_data.z_vndr_cmd_size;       
        }
        else if(t_dd_config.t_dd_vndr_cmd.at_vndr_cmd_info[pt_dev_rec->ui1_vndr_cmd_idx].e_vndr_cmd_type == CECM_VNDR_CMD_WITH_ID)
        {
             /* Send Device discovery vendor command with id*/
            t_frame_info.ui2_opcode = CECM_OPCODE_VNDR_CMD_WITH_ID;
            x_memcpy((VOID*)&t_frame_info.aui1_operand[0],
                     (VOID*)&t_dd_config.t_dd_vndr_cmd.at_vndr_cmd_info[pt_dev_rec->ui1_vndr_cmd_idx].u_data.t_vndr_cmd_with_id_data.t_dev_vndr_id,
                     CECM_DEV_VNDR_ID_SIZE);
            
            x_memcpy((VOID*)&t_frame_info.aui1_operand[3],
                     (VOID*)&t_dd_config.t_dd_vndr_cmd.at_vndr_cmd_info[pt_dev_rec->ui1_vndr_cmd_idx].u_data.t_vndr_cmd_with_id_data.t_vndr_cmd_with_id,
                     t_dd_config.t_dd_vndr_cmd.at_vndr_cmd_info[pt_dev_rec->ui1_vndr_cmd_idx].u_data.t_vndr_cmd_with_id_data.z_vndr_cmd_with_id_size);
            t_frame_info.z_operand_size = CECM_DEV_VNDR_ID_SIZE + 
                                          t_dd_config.t_dd_vndr_cmd.at_vndr_cmd_info[pt_dev_rec->ui1_vndr_cmd_idx].u_data.t_vndr_cmd_with_id_data.z_vndr_cmd_with_id_size;          
        }

        i4_return = cecm_ctrl_send_msg(h_svc, CECM_EVT_TYPE_DEV_DISCOVERY, &t_frame_info, NULL);  
        if (i4_return != TRUE)
        {        
            pt_dev_rec->ui1_vndr_cmd_idx++;
            if((pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)CECM_DD_VNDR_CMD_MAX_SIZE) || 
               (pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)t_dd_config.t_dd_vndr_cmd.z_dd_vndr_cmd_size))
            {
                pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_DEV_VNDR_INFO;
            }
            cecm_unlock_db();
            _cecm_start_post_polling(pv_data, b_evt);
            return;
        }
        cecm_unlock_db();         
              
    }
    else if((pt_dev_rec->ui4_update_mask & CECM_UPDATE_MASK_ALL) == 
            (CECM_UPDATE_MASK_PA | CECM_UPDATE_MASK_DEV_VNDR_ID | CECM_UPDATE_MASK_DEV_VNDR_INFO))
    {
        /*Query cec version*/
        if((t_dd_config.t_dd_get_info.b_cec_ver == FALSE) ||
           ((pt_dev_rec->ui4_skip_update_mask & CECM_UPDATE_MASK_CEC_VER) == CECM_UPDATE_MASK_CEC_VER))
        {
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_CEC_VER;
            cecm_unlock_db();
            _cecm_start_post_polling(pv_data, b_evt);
            return;           
        }    
        t_frame_info.ui2_opcode = CECM_OPCODE_GET_CEC_VER;
        t_frame_info.aui1_operand[0] = 0;
        t_frame_info.z_operand_size = 0;
        
        i4_return = cecm_ctrl_send_msg(h_svc, CECM_EVT_TYPE_DEV_DISCOVERY, &t_frame_info, NULL);
        
        if (i4_return != TRUE)
        {
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_CEC_VER;            
            cecm_unlock_db();
            _cecm_start_post_polling(pv_data, b_evt);
            return;
        }
        cecm_unlock_db(); 

        
    }
    else if ((pt_dev_rec->ui4_update_mask & CECM_UPDATE_MASK_ALL) == 
              (CECM_UPDATE_MASK_PA | CECM_UPDATE_MASK_DEV_VNDR_ID | 
               CECM_UPDATE_MASK_DEV_VNDR_INFO | CECM_UPDATE_MASK_CEC_VER))
    {
        /* Query OSD name */
        if((t_dd_config.t_dd_get_info.b_osd_name == FALSE) ||
           ((pt_dev_rec->ui4_skip_update_mask & CECM_UPDATE_MASK_OSD_NAME) == CECM_UPDATE_MASK_OSD_NAME))
        {
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_OSD_NAME;
            cecm_unlock_db();
            _cecm_start_post_polling(pv_data, b_evt);
            return;           
        }         
        t_frame_info.ui2_opcode = CECM_OPCODE_GIVE_OSD_NAME;
        t_frame_info.aui1_operand[0] = 0;
        t_frame_info.z_operand_size = 0;

        i4_return = cecm_ctrl_send_msg(h_svc, CECM_EVT_TYPE_DEV_DISCOVERY, &t_frame_info, NULL);

        if (i4_return != TRUE)
        {
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_OSD_NAME;            
            cecm_unlock_db();
            _cecm_start_post_polling(pv_data, b_evt);
            return;
        }
        cecm_unlock_db();    
    }
    else if ((pt_dev_rec->ui4_update_mask & CECM_UPDATE_MASK_ALL) == 
             (CECM_UPDATE_MASK_PA | CECM_UPDATE_MASK_DEV_VNDR_ID |
             CECM_UPDATE_MASK_DEV_VNDR_INFO | CECM_UPDATE_MASK_CEC_VER |
             CECM_UPDATE_MASK_OSD_NAME))

    {
        /*Query power status , last query*/
        if((t_dd_config.t_dd_get_info.b_pwr_sts == FALSE) ||
           ((pt_dev_rec->ui4_skip_update_mask & CECM_UPDATE_MASK_PWR_STS) == CECM_UPDATE_MASK_PWR_STS))
        {
            /*If do not need query power status , will finish device discovery*/
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_PWR_STS;            
            if ((pt_dev_rec->ui4_update_mask & CECM_UPDATE_MASK_ALL) == CECM_UPDATE_MASK_ALL)
            {
                if ((pt_dev_rec->t_dev.e_dev_state != CECM_DEV_STATE_UPDATED) &&
                    (pt_dev_rec->e_post_polling_runner == CECM_PP_RUNNER_DEV))
                {
                    ui1_dev_post_polling_cnt--;                  
                }

                pt_dev_rec->t_dev.e_dev_state = CECM_DEV_STATE_UPDATED;
            }            
            cecm_unlock_db();
            _cecm_start_post_polling(pv_data, b_evt);
            return;           
        }     
        t_frame_info.ui2_opcode = CECM_OPCODE_GIVE_DEV_PWR_STS;
        t_frame_info.aui1_operand[0] = 0;
        t_frame_info.z_operand_size = 0;

        i4_return = cecm_ctrl_send_msg(h_svc, CECM_EVT_TYPE_DEV_DISCOVERY, &t_frame_info, NULL);

        if (i4_return != TRUE)
        {
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_PWR_STS;      
            if ((pt_dev_rec->ui4_update_mask & CECM_UPDATE_MASK_ALL) == CECM_UPDATE_MASK_ALL)
            {
                if ((pt_dev_rec->t_dev.e_dev_state != CECM_DEV_STATE_UPDATED) &&
                    (pt_dev_rec->e_post_polling_runner == CECM_PP_RUNNER_DEV))
                {
                    ui1_dev_post_polling_cnt--;                  
                }

                pt_dev_rec->t_dev.e_dev_state = CECM_DEV_STATE_UPDATED;
            }  

            cecm_unlock_db();
            _cecm_start_post_polling(pv_data, b_evt);
            return;
        }
        cecm_unlock_db();          
    }
    else if ((pt_dev_rec->ui4_update_mask & CECM_UPDATE_MASK_ALL) == CECM_UPDATE_MASK_ALL)
    {
        cecm_unlock_db();

        if (cecm_ctrl_is_dev_discovery_running())
        {
            if (pt_dev_rec->e_post_polling_runner == CECM_PP_RUNNER_DEV)
            {
                /* Check if this is a final operation in device discovery */
                if (cecm_ctrl_is_all_dev_discovered())
                {
                    t_discvry_info.e_start_la = (b_evt ? (pt_evt->e_la) : ((CECM_LOG_ADDR_T)(*(UINT8*)pv_data)));
                    t_discvry_info.e_stop_la = (b_evt ? (pt_evt->e_la) : ((CECM_LOG_ADDR_T)(*(UINT8*)pv_data)));
                    t_discvry_info.ui4_itvl = 0;
                    t_discvry_info.b_batch_polling = FALSE;
                    cecm_ctrl_chk_dev_discovery(&t_discvry_info);
                }
                else
                {
                    /* Device post-polling operation is finished */
                    if (b_pp_runner_dev_poll_next_dev)
                    {
                        b_pp_runner_dev_poll_next_dev = FALSE;   
                    }                                                                 
                    else
                    {                        
                         return;
                    }                        
                }
            }

            /* Issue next device discovery */
            cecm_ctrl_poll_next_dev();
        }
        else
        {
            /* Check if device discovery is finished */
            t_discvry_info.e_start_la = (b_evt ? (pt_evt->e_la) : ((CECM_LOG_ADDR_T)(*(UINT8*)pv_data)));
            t_discvry_info.e_stop_la = (b_evt ? (pt_evt->e_la) : ((CECM_LOG_ADDR_T)(*(UINT8*)pv_data)));
            t_discvry_info.ui4_itvl = 0;
            t_discvry_info.b_batch_polling = FALSE;
            cecm_ctrl_chk_dev_discovery(&t_discvry_info);
        }
    }
    else
    {
        cecm_unlock_db();
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_update_dev_discovery_mask
 *
 * Description: This API updates device discovery update mask and proceeds
 *              device discovery process.
 *
 * Inputs:  pt_evt          References an event.
 *          pt_dev_rec      Reference a device record.
 *
 * Outputs: -
 *
 * Returns: TRUE        Response is required.
 *          FALSE       There is no response.
 ----------------------------------------------------------------------------*/
static VOID _cecm_update_dev_discovery_mask(CECM_EVT_T*     pt_evt,
                                            CECM_DEV_REC_T* pt_dev_rec)
{
    cecm_lock_wr_db();
    switch (pt_evt->e_opcode)
    {
        case CECM_OPCODE_GIVE_PHY_ADDR:
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_PA;
            break;

        case CECM_OPCODE_GIVE_DEV_VNDR_ID:
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_DEV_VNDR_ID;
            break;
        case CECM_OPCODE_VNDR_CMD:
        case CECM_OPCODE_VNDR_CMD_WITH_ID:
            if((pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)CECM_DD_VNDR_CMD_MAX_SIZE) ||
               (pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)t_dd_config.t_dd_vndr_cmd.z_dd_vndr_cmd_size))
            {
                pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_DEV_VNDR_INFO;
            }           
            break;
        case CECM_OPCODE_GET_CEC_VER:
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_CEC_VER;
            break;
        case CECM_OPCODE_GIVE_OSD_NAME:
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_OSD_NAME;
            break;
        case CECM_OPCODE_GIVE_DEV_PWR_STS:
            pt_dev_rec->ui4_update_mask |= CECM_UPDATE_MASK_PWR_STS;
            break;
        case CECM_OPCODE_NONE: /* Device discovery polling message */
            if (!pt_dev_rec->t_dev.b_dev_present)
            {
                pt_dev_rec->ui4_update_mask = CECM_UPDATE_MASK_ALL;
            }
            break;
        default:
            break;
    }

    if ((pt_dev_rec->ui4_update_mask & CECM_UPDATE_MASK_ALL) == CECM_UPDATE_MASK_ALL)
    {
        if ((pt_dev_rec->t_dev.e_dev_state != CECM_DEV_STATE_UPDATED) &&
            (pt_dev_rec->e_post_polling_runner == CECM_PP_RUNNER_DEV))
        {
            ui1_dev_post_polling_cnt--;
        }

        pt_dev_rec->t_dev.e_dev_state = CECM_DEV_STATE_UPDATED;
    }
    cecm_unlock_db();

    if (pt_evt->e_opcode == CECM_OPCODE_NONE)
    {
        if (cecm_ctrl_is_batch_polling_mode())
        {
            /* Issue next device discovery */
            cecm_ctrl_poll_next_dev();
        }
        else
        {
            if (pt_dev_rec->t_dev.e_dev_state == CECM_DEV_STATE_UPDATED)
            {
                /* Issue next device discovery */
                cecm_ctrl_poll_next_dev();
            }
            else
            {
                if (pt_dev_rec->e_post_polling_runner == CECM_PP_RUNNER_DEV)
                {
                    /* Issue next device discovery */
                    cecm_ctrl_poll_next_dev();
                }
                else
                {
                    /* Continue device discovery process */
                    _cecm_start_post_polling((VOID*)pt_evt, TRUE);
                }
            }
        }
    }
    else
    {
        /* Continue device discovery process */
        _cecm_start_post_polling((VOID*)pt_evt, TRUE);
    }
}


#if 0
/*-----------------------------------------------------------------------------
 * Name: _cecm_discover_a_dev
 *
 * Description: This API issues a device discovery message to a specific
 *              device.
 *
 * Inputs:  e_la        Contains a logical address.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_discover_a_dev(CECM_LOG_ADDR_T e_la)
{
    CECM_MSG_T      t_msg;

    t_msg.e_msg_type = CECM_MSG_TYPE_DEV_DISCOVERY_START;
    t_msg.u.t_dev_discvry_info.e_start_la = e_la;
    t_msg.u.t_dev_discvry_info.e_stop_la = e_la;
    t_msg.u.t_dev_discvry_info.ui4_itvl = CECM_INV_DEV_DISCVRY_ITVL;
    t_msg.u.t_dev_discvry_info.b_batch_polling = FALSE;

    x_msg_q_send(*ph_cecm_msgq,
                 (VOID*)&t_msg,
                 sizeof(CECM_MSG_T),
                 CECM_MSGQ_DEFAULT_PRIORITY);
}
#endif


#ifdef CECM_PWR_ON_DEV_EX
/*-----------------------------------------------------------------------------
 * Name: _cecm_pwr_on_dev_send_msg
 *
 * Description: This API sends a CEC message to CEC driver.
 *
 * Inputs:  e_opcode        The message opcode.
 *          pt_evt          References a received event.
 *          ui4_data        References additional data.
 *
 * Outputs: -
 *
 * Returns: TRUE        Operation successful.
 *          FALSE       Operation failed.
 ----------------------------------------------------------------------------*/
BOOL _cecm_pwr_on_dev_send_msg(CECM_OPCODE_T e_opcode,
                               CECM_EVT_T*   pt_evt,
                               UINT32*       pui4_data)
{
    BOOL                b_error = FALSE;
    CECM_DEV_REC_T*     pt_dev_rec = NULL;
    CEC_FRAME_INFO_T    t_frame_info;
    CECM_NFY_T          t_nfy_data;

    if (pt_evt == NULL)
    {
        return FALSE;
    }

    t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
    t_frame_info.ui2_opcode = e_opcode;

    switch (e_opcode)
    {
        case CECM_OPCODE_GIVE_DEV_PWR_STS:
            t_frame_info.ui1_dest_addr = (UINT8)pt_evt->e_la;
            t_frame_info.aui1_operand[0] = 0;
            t_frame_info.z_operand_size = 0;

        case CECM_OPCODE_SET_STREAM_PATH:
            if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_evt->e_la, TRUE)) != NULL)
            {
                t_frame_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
                t_frame_info.aui1_operand[0] = (UINT8)(pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa >> 8);
                t_frame_info.aui1_operand[1] = (UINT8)pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa;
                t_frame_info.z_operand_size = CECM_OPCODE_SET_STREAM_PATH_SIZE;
                cecm_ctrl_pwr_on_dev_la(pt_evt->e_la);           
            }
            else
            {
                b_error = TRUE;
            }
            break;

        case CECM_OPCODE_USER_CTRL_PRESSED:
            t_frame_info.ui1_dest_addr = (UINT8)pt_evt->e_la;
            t_frame_info.aui1_operand[0] = (UINT8)(*(CECM_USER_CTRL_T*)pui4_data);
            t_frame_info.z_operand_size = CECM_OPCODE_USER_CTRL_PRESSED_SIZE;
            break;

        case CECM_OPCODE_USER_CTRL_RELEASED:
            t_frame_info.ui1_dest_addr = (UINT8)pt_evt->e_la;
            t_frame_info.aui1_operand[0] = 0;
            t_frame_info.z_operand_size = 0;
            break;

        default:
            b_error = TRUE;
    }

    if (!b_error)
    {
        if (cecm_ctrl_send_msg(pt_evt->pt_svc->h_svc, CECM_EVT_TYPE_PWR_STS, &t_frame_info, NULL) != TRUE)
        {
            b_error = TRUE;
        }
    }

    if (b_error)
    {
        t_nfy_data.e_sent_la = pt_evt->e_la;
        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
        t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
        t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
        t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
        t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_ERROR;
        t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = cecm_ctrl_get_pa_by_la(pt_evt->e_la);
        _cecm_notify_client(&t_nfy_data, pt_evt, FALSE);

        cecm_ctrl_select_dev(FALSE, NULL);
    }

    return (!b_error);
}
#endif


/*-----------------------------------------------------------------------------
 * Name: _cecm_handle_otp_ssp
 *
 * Description:
 *
 * Inputs:  pt_evt      References an event.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_handle_otp_ssp(CECM_EVT_T* pt_evt)
{
    CECM_NFY_T          t_nfy_data;
    CECM_EVT_T*         pt_evt_otp = NULL;
    CEC_FRAME_INFO_T    t_frame_info;

    if (pt_evt->e_type == CECM_EVT_TYPE_ONE_TOUCH_PLAY)
    {
        switch (e_otp_state)
        {
            case CECM_OTP_REQ_ACT_SRC:
                if ((pt_evt_one_touch_play != NULL) &&
                    (pt_evt_one_touch_play->e_la > CECM_LOG_ADDR_TV) &&
                    (pt_evt_one_touch_play->e_la < CECM_LOG_ADDR_MAX))
                {
                    /* No response in querying active source, query PA instead */
                    t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
                    t_frame_info.ui1_dest_addr = (UINT8)(pt_evt_one_touch_play->e_la);
                    t_frame_info.ui2_opcode = CECM_OPCODE_GIVE_PHY_ADDR;
                    t_frame_info.aui1_operand[0] = 0;
                    t_frame_info.z_operand_size = 0;

                    cecm_ctrl_send_msg(cecm_ctrl_get_internal_handle(), CECM_EVT_TYPE_ONE_TOUCH_PLAY, &t_frame_info, NULL);

                    e_otp_state = CECM_OTP_GIVE_PA;
                }
                else
                {
                    e_otp_state = CECM_OTP_NONE;
                }

                /* Remove <Request Active Source> event */
                cecm_ctrl_evt_free(pt_evt);
                pt_evt_one_touch_play = NULL;
                return;

            case CECM_OTP_GIVE_PA:
                /* No response in querying PA, notify client */
                t_nfy_data.e_sent_la = CECM_LOG_ADDR_MAX;
                t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
                t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
                t_nfy_data.ui4_attr = CECM_ATTR_QUERY_PHY_ADDR;
                t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_ONE_TOUCH_PLAY;
                t_nfy_data.u_data.t_one_touch_play.ui2_comp_id = CECM_INV_COMP_ID;

                if ((pt_evt_otp = cecm_ctrl_get_evt_by_la_opcode(pt_evt->e_la,
                                                                 CECM_OPCODE_IMG_VIEW_ON)) != NULL)
                {
                    t_nfy_data.u_data.t_one_touch_play.e_action = CECM_OTP_ACT_IMG_VIEW_ON;
                }
                else if ((pt_evt_otp = cecm_ctrl_get_evt_by_la_opcode(pt_evt->e_la,
                                                                      CECM_OPCODE_TXT_VIEW_ON)) != NULL)
                {
                    t_nfy_data.u_data.t_one_touch_play.e_action = CECM_OTP_ACT_TXT_VIEW_ON;
                }
                else
                {
                    return;
                }                    

                t_nfy_data.u_data.t_one_touch_play.e_la = pt_evt_otp->e_la;

                /* Remove one touch play event */
                cecm_ctrl_evt_free(pt_evt_otp);
                pt_evt_one_touch_play = NULL;
                e_otp_state = CECM_OTP_NONE;
                break;
            default :
                break;                
        }
    }
    else if (pt_evt->e_type == CECM_EVT_TYPE_SET_STREAM_PATH)
    {
        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
        t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
        t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;

#if 0 /* Do not issue <Request Active Source> when <Set Stream Path> is failed */
        if (b_ras_4_ssp)
        {
            CECM_EVT_T*     pt_evt_ssp = NULL;

            t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_WAIT_FOR_OPCODE_TIME_OUT;

            /* Remove <Set Stream Path> event */
            if ((pt_evt_ssp = cecm_ctrl_get_evt_by_la_opcode(pt_evt->e_la,
                                                             CECM_OPCODE_SET_STREAM_PATH)) != NULL)
            {
                t_nfy_data.e_sent_la = pt_evt_ssp->e_la;
                t_nfy_data.e_sent_opcode = pt_evt_ssp->e_opcode;

                cecm_ctrl_evt_free(pt_evt_ssp);
            }

            b_ras_4_ssp = FALSE;
        }
        else
#endif
        {
            t_nfy_data.e_sent_la = pt_evt->e_la;
            t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
            t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_ACK;
            t_nfy_data.u_data.e_ack_cond = CECM_ACK_COND_NO_RESPONSE;
        }
    }

    /* Notify client */
    _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_handle_ack_timeout
 *
 * Description:
 *
 * Inputs:  pv_tag      References a private tag.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_handle_ack_timeout(VOID* pv_tag)
{
    CECM_NFY_T          t_nfy_data;
    CECM_EVT_T*         pt_evt = NULL;
    CECM_DEV_REC_T*     pt_dev_rec = NULL;

    if (pv_tag != NULL)
    {
        pt_evt = (CECM_EVT_T*)pv_tag;

        if (pt_evt->e_la == CECM_LOG_ADDR_MAX)
        {
            return;
        }

        DBG_INFO(("{CECM} Ack timeout (la=%d, opcode=0x%x, type=%d, state=%d)\n\r",
                 pt_evt->e_la,
                 pt_evt->e_opcode,
                 pt_evt->e_type,
                 pt_evt->e_state));

        /*set cec fct delay*/
        if(cecm_ctrl_is_set_cec_fct_delay())
        {
            cecm_ctrl_set_cec_fct_delay(pt_evt);
        }

        if (pt_evt->e_type == CECM_EVT_TYPE_DEV_DISCOVERY)
        {
            if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_evt->e_la, FALSE)) == NULL)
            {
                return;
            }

            /* Update device record for device present status */
            cecm_lock_wr_db();
            if (pt_dev_rec->b_post_polling_retry)
            {
                switch (pt_evt->e_opcode)
                {
                    case CECM_OPCODE_REQ_ACT_SRC:
                        pt_dev_rec->t_dev.b_act_src = FALSE;
                        cecm_ctrl_evt_free(pt_evt);
                        cecm_unlock_db();
                        return;

                    case CECM_OPCODE_GIVE_PHY_ADDR:
                        pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa = CECM_INV_PA;
                        pt_dev_rec->t_dev.t_report_phy_addr.e_dev_type = CECM_DEV_TYPE_NONE;
                        pt_dev_rec->e_abort_reason_pa = CECM_ABORT_REASON_NONE;
                        pt_dev_rec->b_post_polling_retry = FALSE;
                        break;
                        
                    case CECM_OPCODE_GET_CEC_VER:                          
                        pt_dev_rec->t_dev.e_cec_ver = CECM_CEC_VER_INVALID;
                        pt_dev_rec->e_abort_reason_cev_ver = CECM_ABORT_REASON_NONE;
                        pt_dev_rec->b_post_polling_retry = FALSE;
                        break;

                    case CECM_OPCODE_GIVE_DEV_VNDR_ID:
                        x_memset(&pt_dev_rec->t_dev.t_dev_vndr_id, 0, CECM_DEV_VNDR_ID_SIZE);
                        pt_dev_rec->e_abort_reason_dev_vndr_id = CECM_ABORT_REASON_NONE;
                        pt_dev_rec->b_post_polling_retry = FALSE;
                        break;
                    case CECM_OPCODE_VNDR_CMD:
                         if((pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)CECM_DD_VNDR_CMD_MAX_SIZE) ||
                            (pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)t_dd_config.t_dd_vndr_cmd.z_dd_vndr_cmd_size))
                         {
                             DBG_ERROR(("{CECM} ERR: _cecm_handle_ack_timeout() CECM_OPCODE_VNDR_CMD failed\n\r"));                         
                         }
                         else
                         {
                             pt_dev_rec->b_post_polling_retry = FALSE;
                             pt_dev_rec->ae_abort_reason_vndr_info[pt_dev_rec->ui1_vndr_cmd_idx] = CECM_ABORT_REASON_NONE;
                             pt_dev_rec->ui1_vndr_cmd_idx++;                         
                         }
                        break;

                    case CECM_OPCODE_VNDR_CMD_WITH_ID:
                         if((pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)CECM_DD_VNDR_CMD_MAX_SIZE) ||
                            (pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)t_dd_config.t_dd_vndr_cmd.z_dd_vndr_cmd_size))
                         {
                             DBG_ERROR(("{CECM} ERR: _cecm_handle_ack_timeout() CECM_OPCODE_VNDR_CMD_WITH_ID failed\n\r"));                         
                         }
                         else
                         {
                             pt_dev_rec->b_post_polling_retry = FALSE;
                             pt_dev_rec->ae_abort_reason_vndr_info[pt_dev_rec->ui1_vndr_cmd_idx] = CECM_ABORT_REASON_NONE;
                             pt_dev_rec->ui1_vndr_cmd_idx++;                         
                         }

                        break;                        
                    case CECM_OPCODE_GIVE_OSD_NAME:
                        x_memset(&(pt_dev_rec->t_dev.aw2_osd_name)[0], 0, CECM_OSD_NAME_SIZE);
                        pt_dev_rec->e_abort_reason_osd_name = CECM_ABORT_REASON_NONE;
                        pt_dev_rec->b_post_polling_retry = FALSE;
                        break;
                        
                    case CECM_OPCODE_GIVE_DEV_PWR_STS:
                       pt_dev_rec->t_dev.e_pwr_sts = CECM_PWR_STS_ERROR;
                       pt_dev_rec->e_abort_reason_pwr_sts = CECM_ABORT_REASON_NONE;
                       pt_dev_rec->b_post_polling_retry = FALSE;
                       break;

                    case CECM_OPCODE_NONE: /* Device discovery polling message */
                        if (++pt_dev_rec->ui1_polling_retry < t_dd_config.ui1_polling_retry_num)
                        {
                            CEC_FRAME_INFO_T    t_frame_info;

                            /* Ping again */
                            t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
                            t_frame_info.ui1_dest_addr = (UINT8)(pt_evt->e_la);
                            t_frame_info.ui2_opcode = CECM_OPCODE_NONE;
                            t_frame_info.aui1_operand[0] = 0;
                            t_frame_info.z_operand_size = 0;

                            cecm_ctrl_send_msg(cecm_ctrl_get_internal_handle(),
                                               CECM_EVT_TYPE_DEV_DISCOVERY,
                                               &t_frame_info,
                                               NULL);

                            cecm_unlock_db();
                            cecm_ctrl_evt_free(pt_evt);
                            return;
                        }
                        else
                        {
                            pt_dev_rec->t_dev.b_dev_present = FALSE;
                        }
                        break;
                    default :
                        break;
                }
            }
            else
            {
                if ((pt_evt->e_opcode == CECM_OPCODE_GIVE_PHY_ADDR) ||
                    (pt_evt->e_opcode == CECM_OPCODE_GIVE_DEV_VNDR_ID) ||
                    (pt_evt->e_opcode == CECM_OPCODE_GIVE_OSD_NAME) ||
                    (pt_evt->e_opcode == CECM_OPCODE_GET_CEC_VER)  ||
                    (pt_evt->e_opcode == CECM_OPCODE_GIVE_DEV_PWR_STS) ||
                    (pt_evt->e_opcode == CECM_OPCODE_VNDR_CMD) ||
                    (pt_evt->e_opcode == CECM_OPCODE_VNDR_CMD_WITH_ID))
                {
                    /* Retry again */
                    CEC_FRAME_INFO_T    t_frame_info;

                    t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
                    t_frame_info.ui1_dest_addr = (UINT8)pt_evt->e_la;
                    t_frame_info.ui2_opcode = (UINT16)pt_evt->e_opcode;
                    t_frame_info.aui1_operand[0] = 0;
                    t_frame_info.z_operand_size = 0;

                    cecm_ctrl_send_msg(pt_evt->pt_svc->h_svc, CECM_EVT_TYPE_DEV_DISCOVERY, &t_frame_info, NULL);

                    pt_dev_rec->b_post_polling_retry = TRUE;
                    cecm_unlock_db();
                    cecm_ctrl_evt_free(pt_evt);
                    return;
                }
            }
            cecm_unlock_db();

            _cecm_update_dev_discovery_mask(pt_evt, pt_dev_rec);
            cecm_ctrl_evt_free(pt_evt);
        }
        else
        {

            if (pt_evt->h_sync)
            {
                pt_evt->pt_sync_data->e_sync_op = CECM_SYNC_OP_FAIL;
                cecm_sync_unlock(pt_evt->h_sync);
                cecm_ctrl_evt_free(pt_evt);
            }
            else if (pt_evt->e_type == CECM_EVT_TYPE_POLL_LA)
            {   
                cecm_ctrl_handle_tv_la(pt_evt, FALSE);
                
            }            
            else
            {
                if (pt_evt->e_type == CECM_EVT_TYPE_ONE_TOUCH_PLAY)
                {
                    _cecm_handle_otp_ssp(pt_evt);
                    return;
                }
                else if (pt_evt->e_type == CECM_EVT_TYPE_SET_STREAM_PATH)
                {
                    _cecm_handle_otp_ssp(pt_evt);
                    return;
                }
                else if (pt_evt->e_type == CECM_EVT_TYPE_PWR_STS)
                {
#ifdef CECM_PWR_ON_DEV_EX
                    b_recv_pwr_standby = FALSE;
                    x_timer_stop(h_timer_pwr_on_dev_retry);
                    cecm_ctrl_select_dev(FALSE, NULL);

                    t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_1ST_GIVE_DEV_PWR_STS_NO_ACK;
#else
                    cecm_ctrl_select_dev(FALSE, NULL);
                    t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_ERROR;
#endif
                    t_nfy_data.e_sent_la = pt_evt->e_la;
                    t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
                    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                    t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
                    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                    t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
                    t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = cecm_ctrl_get_pa_by_la(pt_evt->e_la);
                }
                else
                {
                    t_nfy_data.e_sent_la = pt_evt->e_la;
                    t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
                    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                    t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
                    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                    t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_ACK;
                    t_nfy_data.u_data.e_ack_cond = CECM_ACK_COND_NO_RESPONSE;
                }

                /* Notify client */
                _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
            }
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_handle_frame_timeout
 *
 * Description:
 *
 * Inputs:  pv_tag      References a private tag.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_handle_frame_timeout(VOID* pv_tag)
{
    CECM_NFY_T          t_nfy_data;
    CECM_EVT_T*         pt_evt = NULL;
    CECM_DEV_REC_T*     pt_dev_rec = NULL;

    if (pv_tag != NULL)
    {
        pt_evt = (CECM_EVT_T*)pv_tag;

        if (pt_evt->e_la == CECM_LOG_ADDR_MAX)
        {
            return;
        }

        DBG_INFO(("{CECM} Frame timeout (la=%d, opcode=0x%x, type=%d, state=%d)\n\r",
                  pt_evt->e_la,
                  pt_evt->e_opcode,
                  pt_evt->e_type,
                  pt_evt->e_state));

        if (pt_evt->e_type == CECM_EVT_TYPE_DEV_DISCOVERY)
        {
            if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_evt->e_la, FALSE)) == NULL)
            {
                return;
            }

            /* Update device record for device present status */
            cecm_lock_wr_db();
            if (pt_dev_rec->b_post_polling_retry)
            {
                switch (pt_evt->e_opcode)
                {
                    case CECM_OPCODE_REQ_ACT_SRC:
                        pt_dev_rec->t_dev.b_act_src = FALSE;
                        cecm_ctrl_evt_free(pt_evt);
                        cecm_unlock_db();
                        return;

                    case CECM_OPCODE_GIVE_PHY_ADDR:
                        pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa = CECM_INV_PA;
                        pt_dev_rec->t_dev.t_report_phy_addr.e_dev_type = CECM_DEV_TYPE_NONE;
                        pt_dev_rec->e_abort_reason_pa = CECM_ABORT_REASON_NONE;
                        pt_dev_rec->b_post_polling_retry = FALSE;
                        break;

                    case CECM_OPCODE_GIVE_DEV_VNDR_ID:
                        x_memset(&pt_dev_rec->t_dev.t_dev_vndr_id, 0, CECM_DEV_VNDR_ID_SIZE);
                        pt_dev_rec->e_abort_reason_dev_vndr_id = CECM_ABORT_REASON_NONE;
                        pt_dev_rec->b_post_polling_retry = FALSE;
                        break;

                    case CECM_OPCODE_VNDR_CMD:
                        if((pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)CECM_DD_VNDR_CMD_MAX_SIZE) ||
                           (pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)t_dd_config.t_dd_vndr_cmd.z_dd_vndr_cmd_size))
                        {
                            DBG_ERROR(("{CECM} ERR: _cecm_handle_frame_timeout() CECM_OPCODE_VNDR_CMD failed\n\r"));
                        }
                        else
                        {
                            pt_dev_rec->ae_abort_reason_vndr_info[pt_dev_rec->ui1_vndr_cmd_idx] = CECM_ABORT_REASON_NONE;
                            pt_dev_rec->b_post_polling_retry = FALSE;
                            pt_dev_rec->ui1_vndr_cmd_idx++;                     
                        }

                        break;
                    case CECM_OPCODE_VNDR_CMD_WITH_ID:
                        if((pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)CECM_DD_VNDR_CMD_MAX_SIZE) ||
                           (pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)t_dd_config.t_dd_vndr_cmd.z_dd_vndr_cmd_size))
                        {
                            DBG_ERROR(("{CECM} ERR: _cecm_handle_frame_timeout() CECM_OPCODE_VNDR_CMD_WITH_ID failed\n\r"));
                        }
                        else
                        {
                            pt_dev_rec->ae_abort_reason_vndr_info[pt_dev_rec->ui1_vndr_cmd_idx] = CECM_ABORT_REASON_NONE;
                            pt_dev_rec->b_post_polling_retry = FALSE;
                            pt_dev_rec->ui1_vndr_cmd_idx++;                     
                        }

                        break;                        
                    case CECM_OPCODE_GET_CEC_VER:                          
                        pt_dev_rec->t_dev.e_cec_ver = CECM_CEC_VER_INVALID;
                        pt_dev_rec->e_abort_reason_cev_ver = CECM_ABORT_REASON_NONE;
                        pt_dev_rec->b_post_polling_retry = FALSE;
                        break;

                    case CECM_OPCODE_GIVE_OSD_NAME:
                        x_memset(&(pt_dev_rec->t_dev.aw2_osd_name)[0], 0, CECM_OSD_NAME_SIZE);
                        pt_dev_rec->e_abort_reason_osd_name = CECM_ABORT_REASON_NONE;
                        pt_dev_rec->b_post_polling_retry = FALSE;
                        break;
                    case CECM_OPCODE_GIVE_DEV_PWR_STS:
                       pt_dev_rec->t_dev.e_pwr_sts = CECM_PWR_STS_ERROR;
                       pt_dev_rec->e_abort_reason_pwr_sts = CECM_ABORT_REASON_NONE;
                       pt_dev_rec->b_post_polling_retry = FALSE;
                       break;    

                    default :
                        break;
                }
            }
            else
            {
                if ((pt_evt->e_opcode == CECM_OPCODE_GIVE_PHY_ADDR) ||
                    (pt_evt->e_opcode == CECM_OPCODE_GIVE_DEV_VNDR_ID) ||
                    (pt_evt->e_opcode == CECM_OPCODE_GIVE_OSD_NAME) ||
                    (pt_evt->e_opcode == CECM_OPCODE_GET_CEC_VER)  ||
                    (pt_evt->e_opcode == CECM_OPCODE_GIVE_DEV_PWR_STS) ||
                    (pt_evt->e_opcode == CECM_OPCODE_VNDR_CMD) ||
                    (pt_evt->e_opcode == CECM_OPCODE_VNDR_CMD_WITH_ID))
                 {
                    /* Retry again */
                    CEC_FRAME_INFO_T    t_frame_info;

                    t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
                    t_frame_info.ui1_dest_addr = (UINT8)pt_evt->e_la;
                    t_frame_info.ui2_opcode = (UINT16)pt_evt->e_opcode;
                    t_frame_info.aui1_operand[0] = 0;
                    t_frame_info.z_operand_size = 0;

                    cecm_ctrl_send_msg(pt_evt->pt_svc->h_svc, CECM_EVT_TYPE_DEV_DISCOVERY, &t_frame_info, NULL);

                    pt_dev_rec->b_post_polling_retry = TRUE;
                    cecm_unlock_db();
                    cecm_ctrl_evt_free(pt_evt);
                    return;
                }
            }
            cecm_unlock_db();

            _cecm_update_dev_discovery_mask(pt_evt, pt_dev_rec);
            cecm_ctrl_evt_free(pt_evt);
        }
        else
        {
            if (pt_evt->h_sync)
            {
                pt_evt->pt_sync_data->e_sync_op = CECM_SYNC_OP_FAIL;
                cecm_sync_unlock(pt_evt->h_sync);
                cecm_ctrl_evt_free(pt_evt);
            }
            else
            {
                if (pt_evt->e_type == CECM_EVT_TYPE_ONE_TOUCH_PLAY)
                {
                    _cecm_handle_otp_ssp(pt_evt);
                    return;
                }
#if 0 /* Do not issue <Request Active Source> when <Set Stream Path> is failed */
                else if (pt_evt->e_type == CECM_EVT_TYPE_SET_STREAM_PATH)
                {
                    CECM_EVT_T*         pt_evt_ssp = NULL;
                    CEC_FRAME_INFO_T    t_frame_info;

                    if (b_ras_4_ssp)
                    {
                        /* Remove <Set Stream Path> event */
                        if ((pt_evt_ssp = cecm_ctrl_get_evt_by_la_opcode(pt_evt->e_la,
                                                                         CECM_OPCODE_SET_STREAM_PATH)) != NULL)
                        {
                            /* Notify client */
                            t_nfy_data.e_sent_la = pt_evt_ssp->e_la;
                            t_nfy_data.e_sent_opcode = pt_evt_ssp->e_opcode;
                            t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                            t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
                            t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                            t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_WAIT_FOR_OPCODE_TIME_OUT;
                            _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);

                            cecm_ctrl_evt_free(pt_evt_ssp);
                        }

                        b_ras_4_ssp = FALSE;
                        return;
                    }
                    else
                    {
                        /* No response from devices, query active source */
                        t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
                        t_frame_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
                        t_frame_info.ui2_opcode = CECM_OPCODE_REQ_ACT_SRC;
                        t_frame_info.aui1_operand[0] = 0;
                        t_frame_info.z_operand_size = 0;

                        cecm_ctrl_send_msg(pt_evt->pt_svc->h_svc, CECM_EVT_TYPE_SET_STREAM_PATH, &t_frame_info, NULL);

                        b_ras_4_ssp = TRUE;
                        return;
                    }
                }
#endif
                else if (pt_evt->e_opcode == CECM_OPCODE_ROUTING_CHG)
                {
                    /* Generate a fake routing info opcode */
                    t_nfy_data.e_sent_la = pt_evt->e_la;
                    t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
                    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
                    t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
                    t_nfy_data.ui4_attr = CECM_ATTR_OPCODE_GEN_BY_CECM;
                    t_nfy_data.u_code.e_opcode = CECM_OPCODE_ROUTING_INFO;
                    t_nfy_data.u_data.ui2_pa = pt_evt->u.t_routing_chg.ui2_new_pa;
                }
                else if (pt_evt->e_type == CECM_EVT_TYPE_PWR_STS)
                {
#ifdef CECM_PWR_ON_DEV_EX
                    b_recv_pwr_standby = FALSE;
                    x_timer_stop(h_timer_pwr_on_dev_retry);
                    cecm_psr_pwr_on_dev(FALSE);

                    /* Free the using event */
                    cecm_ctrl_evt_free(pt_evt);
                    return;
#else
                    INT32           i4_return;
                    CECM_EVT_T      t_evt;

                    CEC_FRAME_INFO_T    t_frame_info;
                                       
                    /* Free the using event */
                    t_evt = *pt_evt;
                    cecm_ctrl_evt_free(pt_evt);

                    x_thread_delay(CECM_PWR_STS_STANDBY_TO_ON_ITVL);

                    /* Query power status again */
                    t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
                    t_frame_info.ui1_dest_addr = (UINT8)t_evt.e_la;
                    t_frame_info.ui2_opcode = CECM_OPCODE_GIVE_DEV_PWR_STS;
                    t_frame_info.aui1_operand[0] = 0;
                    t_frame_info.z_operand_size = 0;

                    i4_return = cecm_ctrl_send_msg(t_evt.pt_svc->h_svc, CECM_EVT_TYPE_PWR_STS, &t_frame_info, NULL);

                    if (i4_return != TRUE)
                    {
                        t_nfy_data.e_sent_la = t_evt.e_la;
                        t_nfy_data.e_sent_opcode = t_evt.e_opcode;
                        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                        t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                        t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
                        t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_ERROR;
                        t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = cecm_ctrl_get_pa_by_la(t_evt.e_la);
                        _cecm_notify_client(&t_nfy_data, &t_evt, FALSE);
                    }
                    

                    return;
#endif
                }
                else
                {
                    t_nfy_data.e_sent_la = pt_evt->e_la;
                    t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
                    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                    t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
                    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                    t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_WAIT_FOR_OPCODE_TIME_OUT;
                }

                /* Notify client */
                _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
            }
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_handle_one_touch_play_timeout
 *
 * Description:
 *
 * Inputs:  pv_tag      References a private tag.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_handle_one_touch_play_timeout(VOID* pv_tag)
{
    

    /* Check if this is a valid event */
    if (pv_tag == NULL)
    {
        return;
    }

    DBG_INFO(("{CECM} One touch play timeout (la=%d, opcode=0x%x, type=%d, state=%d)\n\r",
             ((CECM_EVT_T*)pv_tag)->e_la,
             ((CECM_EVT_T*)pv_tag)->e_opcode,
             ((CECM_EVT_T*)pv_tag)->e_type,
             ((CECM_EVT_T*)pv_tag)->e_state));

    #ifdef CECM_OTP_TINE_OUT_NOT_REQ_ACT_SRC
    {
        CECM_NFY_T          t_nfy_data;
        CECM_EVT_T*         pt_evt = (CECM_EVT_T*)pv_tag;
        t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
        t_nfy_data.e_sent_la = pt_evt->e_la;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
        t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
        t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_ONE_TOUCH_PLAY;
        t_nfy_data.u_data.t_one_touch_play.e_la = pt_evt->e_la;
        t_nfy_data.u_data.t_one_touch_play.ui2_comp_id = CECM_INV_COMP_ID;
        if ((cecm_ctrl_get_evt_by_la_opcode(pt_evt->e_la,
                                            CECM_OPCODE_IMG_VIEW_ON)) != NULL)
        {
            t_nfy_data.u_data.t_one_touch_play.e_action = CECM_OTP_ACT_IMG_VIEW_ON;
        }
        else if ((cecm_ctrl_get_evt_by_la_opcode(pt_evt->e_la,
                                                 CECM_OPCODE_TXT_VIEW_ON)) != NULL)
        {
            t_nfy_data.u_data.t_one_touch_play.e_action = CECM_OTP_ACT_TXT_VIEW_ON;
        }

        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);    

    }
    #else
    {  
        CEC_FRAME_INFO_T    t_frame_info;
    /* Since <Active Source> is not received, query active source by ourselves */
    t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
    t_frame_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
    t_frame_info.ui2_opcode = CECM_OPCODE_REQ_ACT_SRC;
    t_frame_info.aui1_operand[0] = 0;
    t_frame_info.z_operand_size = 0;

    cecm_ctrl_send_msg(cecm_ctrl_get_internal_handle(), CECM_EVT_TYPE_ONE_TOUCH_PLAY, &t_frame_info, NULL);
    e_otp_state = CECM_OTP_REQ_ACT_SRC;
}
    #endif
       
}


#ifdef CECM_PWR_ON_DEV_EX
/*-----------------------------------------------------------------------------
 * Name: _cecm_handle_select_dev_timeout
 *
 * Description:
 *
 * Inputs:  pv_tag      References a private tag.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_handle_select_dev_timeout(VOID* pv_tag)
{
    INT32                       i4_return;
    VOID*                       pv_obj = NULL;
    HANDLE_TYPE_T               e_type;
    CECM_NFY_T                  t_nfy_data;
    CECM_SELECT_DEV_INFO_T*     pt_select_info;
    CECM_SVC_T*                 pt_svc;

    if (pv_tag != NULL)
    {
        pt_select_info = (CECM_SELECT_DEV_INFO_T*)pv_tag;

        i4_return = handle_get_type_obj(pt_select_info->h_cecm_svc, &e_type, &pv_obj);
        if (i4_return == HR_OK && e_type == CECM_HANDLE_TYPE)
        {       
            /* t_nfy_data.e_sent_la = pt_select_info->e_sent_la; */
            t_nfy_data.e_sent_la = pt_select_info->e_pwr_on_dev_la;
            t_nfy_data.e_sent_opcode = pt_select_info->e_sent_opcode;
            t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
            t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
            t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
            t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
            t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_TIMEOUT;
            t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = cecm_ctrl_get_pa_by_la(pt_select_info->e_pwr_on_dev_la);

            pt_svc = (CECM_SVC_T*)pv_obj;

            if (pt_svc->pf_nfy != NULL)
            {
                (pt_svc->pf_nfy)(&t_nfy_data, pt_svc->pv_tag, 0);
            }
        }
        else if (i4_return == HR_OK && e_type != CECM_HANDLE_TYPE)
        {
                #ifdef DEBUG
                dbg_abort(DBG_MOD_CEC_MNGR | DBG_CAT_HANDLE);
                #endif
        }
           
    }

    cecm_ctrl_select_dev(FALSE, NULL);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_handle_pwr_on_dev_timeout
 *
 * Description:
 *
 * Inputs:  pv_tag      References a private tag.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_handle_pwr_on_dev_timeout(VOID* pv_tag)
{
    INT32                       i4_return;
    VOID*                       pv_obj = NULL;
    HANDLE_TYPE_T               e_type;
    CECM_SELECT_DEV_INFO_T*     pt_select_info;
    CECM_SVC_T*                 pt_svc;
    CECM_DEV_REC_T*             pt_dev_rec = NULL;
    CEC_FRAME_INFO_T            t_frame_info;
    CECM_NFY_T                  t_nfy_data;

    cecm_psr_pwr_on_dev(FALSE);
    DBG_INFO(("{CECM} >>>> power_on_device timeout!\n\r"));

    if (pv_tag != NULL)
    {
        pt_select_info = (CECM_SELECT_DEV_INFO_T*)pv_tag;
        i4_return = handle_get_type_obj(pt_select_info->h_cecm_svc, &e_type, &pv_obj);

        if (i4_return == HR_OK && e_type == CECM_HANDLE_TYPE)
        {
            pt_svc = (CECM_SVC_T*)pv_obj;
            pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_select_info->e_sent_la, TRUE);

            if (pt_dev_rec != NULL)
            {
                /* Send <Set Stream Path> */
                t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
                t_frame_info.ui1_dest_addr = (UINT8)pt_select_info->e_sent_la;
                t_frame_info.ui2_opcode = CECM_OPCODE_SET_STREAM_PATH;
                t_frame_info.aui1_operand[0] = (UINT8)(pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa >> 8);
                t_frame_info.aui1_operand[1] = (UINT8)pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa;
                t_frame_info.z_operand_size = CECM_OPCODE_SET_STREAM_PATH_SIZE;

                i4_return = cecm_ctrl_send_msg(pt_svc->h_svc, CECM_EVT_TYPE_PWR_STS, &t_frame_info, NULL);
            }

            if ((pt_dev_rec == NULL) || (i4_return != TRUE))
            {
                t_nfy_data.e_sent_opcode = pt_select_info->e_sent_opcode;
                t_nfy_data.e_sent_la = pt_select_info->e_sent_la;
                t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
                t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
                t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_ERROR;
                t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = cecm_ctrl_get_pa_by_la(pt_select_info->e_pwr_on_dev_la);

                if (pt_svc->pf_nfy != NULL)
                {
                    (pt_svc->pf_nfy)(&t_nfy_data, pt_svc->pv_tag, 0);
                }
            }
        }
        else if (i4_return == HR_OK && e_type != CECM_HANDLE_TYPE)
        {
                #ifdef DEBUG
                dbg_abort(DBG_MOD_CEC_MNGR | DBG_CAT_HANDLE);
                #endif
        }
            
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_handle_pwr_on_dev_retry_timeout
 *
 * Description:
 *
 * Inputs:  pv_tag      References a private tag.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_handle_pwr_on_dev_retry_timeout(VOID* pv_tag)
{
    INT32                       i4_return;
    VOID*                       pv_obj = NULL;
    HANDLE_TYPE_T               e_type;
    CECM_SELECT_DEV_INFO_T*     pt_select_info;
    CECM_SVC_T*                 pt_svc;
    CECM_NFY_T                  t_nfy_data;
    CEC_FRAME_INFO_T            t_frame_info;

    if (pv_tag != NULL)
    {
        pt_select_info = (CECM_SELECT_DEV_INFO_T*)pv_tag;
        i4_return = handle_get_type_obj(pt_select_info->h_cecm_svc, &e_type, &pv_obj);

        if (i4_return == HR_OK && e_type == CECM_HANDLE_TYPE)
        {
            pt_svc = (CECM_SVC_T*)pv_obj;

            /* Send <Give Device Power Status> */
            t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
            t_frame_info.ui1_dest_addr = (UINT8)pt_select_info->e_sent_la;
            t_frame_info.ui2_opcode = CECM_OPCODE_GIVE_DEV_PWR_STS;
            t_frame_info.aui1_operand[0] = 0;
            t_frame_info.z_operand_size = 0;

            i4_return = cecm_ctrl_send_msg(pt_svc->h_svc, CECM_EVT_TYPE_PWR_STS, &t_frame_info, NULL);

            if (i4_return != TRUE)
            {
                t_nfy_data.e_sent_la = pt_select_info->e_sent_la;
                t_nfy_data.e_sent_opcode = pt_select_info->e_sent_opcode;
                t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
                t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
                t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_ERROR;
                t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = cecm_ctrl_get_pa_by_la(pt_select_info->e_pwr_on_dev_la);

                if (pt_svc->pf_nfy != NULL)
                {
                    (pt_svc->pf_nfy)(&t_nfy_data, pt_svc->pv_tag, 0);
                }
            }
        }
        else if (i4_return == HR_OK && e_type != CECM_HANDLE_TYPE)
        {
                #ifdef DEBUG
                dbg_abort(DBG_MOD_CEC_MNGR | DBG_CAT_HANDLE);
                #endif
        }
    }
}
#else
/*-----------------------------------------------------------------------------
 * Name: _cecm_handle_select_dev_timeout
 *
 * Description:
 *
 * Inputs:  pv_tag      References a private tag.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_handle_select_dev_timeout(VOID* pv_tag)
{
    INT32                       i4_return;
    VOID*                       pv_obj = NULL;
    HANDLE_TYPE_T               e_type;
    CECM_NFY_T                  t_nfy_data;
    CECM_SELECT_DEV_INFO_T*     pt_select_info;
    CECM_SVC_T*                 pt_svc;
    

    if (pv_tag != NULL)
    {
        pt_select_info = (CECM_SELECT_DEV_INFO_T*)pv_tag;      
        i4_return = handle_get_type_obj(pt_select_info->h_cecm_svc, &e_type, &pv_obj);

        if (i4_return == HR_OK && e_type == CECM_HANDLE_TYPE)
        {
            /* t_nfy_data.e_sent_la = pt_select_info->e_sent_la; */
            t_nfy_data.e_sent_la = pt_select_info->e_pwr_on_dev_la;
            t_nfy_data.e_sent_opcode = pt_select_info->e_sent_opcode;
            t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
            t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
            t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
            t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
            t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_TIMEOUT;
            t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = cecm_ctrl_get_pa_by_la(pt_select_info->e_pwr_on_dev_la);

            pt_svc = (CECM_SVC_T*)pv_obj;

            if (pt_svc->pf_nfy != NULL)
            {
                (pt_svc->pf_nfy)(&t_nfy_data, pt_svc->pv_tag, 0);
            }
        }
        else if (i4_return == HR_OK && e_type != CECM_HANDLE_TYPE)
        {
                #ifdef DEBUG
                dbg_abort(DBG_MOD_CEC_MNGR | DBG_CAT_HANDLE);
                #endif
        }
    }

    cecm_ctrl_select_dev(FALSE, NULL);
}

#endif


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_act_src
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_act_src(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_EVT_T*         pt_evt_ras = NULL;
    CECM_DEV_REC_T*     pt_dev_rec = NULL;
    CECM_NFY_T          t_nfy_data;
    BOOL                b_ssp_check = FALSE;
    CECM_LOG_ADDR_T     e_la;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr != CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_ACT_SRC_SIZE)
    {
        return;
    }

    DBG_INFO(("{CECM} >>>> Report Active Source: phy_addr = 0x%x\n\r", GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0])));

    for(e_la = CECM_LOG_ADDR_REC_DEV_1; e_la < CECM_LOG_ADDR_UNREGED_BRDCST; e_la++)
    {
        /* Update device record for inactive source change */
        if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(e_la, FALSE)) != NULL)
        {
            cecm_lock_wr_db();
            cecm_ctrl_update_act_src(pt_dev_rec, FALSE);
            cecm_unlock_db();
        }

        if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(e_la, TRUE)) != NULL)
        {
            cecm_lock_wr_db();
            cecm_ctrl_update_act_src(pt_dev_rec, FALSE);
            cecm_unlock_db();
        }
    }

    /* Update device record for active source change */
    if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr), FALSE)) != NULL)
    {
        cecm_lock_wr_db();
        cecm_ctrl_update_act_src(pt_dev_rec, TRUE);
        cecm_unlock_db();
    }

    if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr), TRUE)) != NULL)
    {
        cecm_lock_wr_db();
        cecm_ctrl_update_act_src(pt_dev_rec, TRUE);
        cecm_unlock_db();
    }

    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_IMG_VIEW_ON)) != NULL) /* Image view on */
    {
        switch (e_otp_state)
        {
            case CECM_OTP_NONE:
                x_timer_stop(h_timer_opt);
                t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                break;

            case CECM_OTP_REQ_ACT_SRC:
                e_otp_state = CECM_OTP_NONE;
                pt_evt_one_touch_play = NULL;
                t_nfy_data.ui4_attr = CECM_ATTR_REQ_ACT_SRC;

                /* Remove <Request Active Source> event */
                if ((pt_evt_ras = cecm_ctrl_get_evt_by_type_la_opcode(CECM_EVT_TYPE_ONE_TOUCH_PLAY,
                                                                      (CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                                      CECM_OPCODE_REQ_ACT_SRC)) != NULL)
                {
                    cecm_ctrl_evt_free(pt_evt_ras);
                }
                break;
            default :
                break ;
        }

        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
        t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
        t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_ONE_TOUCH_PLAY;
        t_nfy_data.u_data.t_one_touch_play.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
        t_nfy_data.u_data.t_one_touch_play.e_la = (CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr);
        t_nfy_data.u_data.t_one_touch_play.ui2_comp_id = cecm_ctrl_cal_comp_id_from_pa(GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]));
        t_nfy_data.u_data.t_one_touch_play.e_action = CECM_OTP_ACT_IMG_VIEW_ON;
        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);

        b_ssp_check = TRUE;
    }
    else if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                      CECM_OPCODE_TXT_VIEW_ON)) != NULL) /* Text view on */
    {
        switch (e_otp_state)
        {
            case CECM_OTP_NONE:
                x_timer_stop(h_timer_opt);
                t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                break;

            case CECM_OTP_REQ_ACT_SRC:
                e_otp_state = CECM_OTP_NONE;
                pt_evt_one_touch_play = NULL;
                t_nfy_data.ui4_attr = CECM_ATTR_REQ_ACT_SRC;

                /* Remove <Request Active Source> event */
                if ((pt_evt_ras = cecm_ctrl_get_evt_by_type_la_opcode(CECM_EVT_TYPE_ONE_TOUCH_PLAY,
                                                                      (CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                                      CECM_OPCODE_REQ_ACT_SRC)) != NULL)
                {
                    cecm_ctrl_evt_free(pt_evt_ras);
                }
                break;
            default :
                break;
        }

        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
        t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
        t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_ONE_TOUCH_PLAY;
        t_nfy_data.u_data.t_one_touch_play.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
        t_nfy_data.u_data.t_one_touch_play.e_la = (CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr);
        t_nfy_data.u_data.t_one_touch_play.ui2_comp_id = cecm_ctrl_cal_comp_id_from_pa(GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]));
        t_nfy_data.u_data.t_one_touch_play.e_action = CECM_OTP_ACT_TXT_VIEW_ON;
        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);

        b_ssp_check = TRUE;
    }
    else if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                      CECM_OPCODE_SET_STREAM_PATH)) != NULL) /* Set stream path */
    {
        if (pt_evt->e_type == CECM_EVT_TYPE_PWR_STS)
        {
#ifdef CECM_PWR_ON_DEV_EX
            if (cecm_ctrl_is_select_dev_running())
            {
                cecm_ctrl_select_dev(FALSE, NULL);
                t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr; /* Device LA */
                t_nfy_data.e_sent_opcode = pt_evt->e_opcode;

                /* <Active Source> notification */
                t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
                t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                            CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
                t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                t_nfy_data.u_code.e_opcode = CECM_OPCODE_ACT_SRC;
                t_nfy_data.u_data.t_act_src.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
                t_nfy_data.u_data.t_act_src.e_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
                t_nfy_data.u_data.t_act_src.ui2_comp_id = cecm_ctrl_cal_comp_id_from_pa(GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]));
                _cecm_notify_client(&t_nfy_data, NULL, FALSE);

                /* Select_device notification */
                t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
                t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
                t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_ON_THEN_ACT_SRC;
                t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
                _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
            }
            else
            {
                /* Free the using event */
                cecm_ctrl_evt_free(pt_evt);
            }
#endif
        }
        else
        {
            t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr; /* Device LA */
            t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
            t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
            t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                        CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
            t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
            t_nfy_data.u_code.e_opcode = CECM_OPCODE_ACT_SRC;
            t_nfy_data.u_data.t_act_src.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
            t_nfy_data.u_data.t_act_src.e_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
            t_nfy_data.u_data.t_act_src.ui2_comp_id = cecm_ctrl_cal_comp_id_from_pa(GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]));
            _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);

#if 0 /* Do not issue <Request Active Source> when <Set Stream Path> is failed */
            if (b_ras_4_ssp)
            {
                /* Remove <Request Active Source> event */
                if ((pt_evt = cecm_ctrl_get_evt_by_type_la_opcode(CECM_EVT_TYPE_SET_STREAM_PATH,
                                                                  (CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                                  CECM_OPCODE_REQ_ACT_SRC)) != NULL)
                {
                    cecm_ctrl_evt_free(pt_evt);
                }

                b_ras_4_ssp = FALSE;
            }
#endif
        }
    }
    else if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                      CECM_OPCODE_REQ_ACT_SRC)) != NULL) /* Request active source */
    {
        if (pt_evt->e_type == CECM_EVT_TYPE_DEV_DISCOVERY)
        {
            t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
            t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
            t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
            t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                        CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
            t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
            t_nfy_data.u_code.e_opcode = CECM_OPCODE_ACT_SRC;
            t_nfy_data.u_data.t_act_src.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
            t_nfy_data.u_data.t_act_src.e_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
            t_nfy_data.u_data.t_act_src.ui2_comp_id = cecm_ctrl_cal_comp_id_from_pa(GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]));
            _cecm_notify_client(&t_nfy_data, NULL, FALSE);

            /* Free the using event */
            cecm_ctrl_evt_free(pt_evt);
        }
        else
        {
            t_nfy_data.e_sent_la = pt_evt->e_la;
            t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
            t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
            t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                        CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
            t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
            t_nfy_data.u_code.e_opcode = CECM_OPCODE_ACT_SRC;
            t_nfy_data.u_data.t_act_src.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
            t_nfy_data.u_data.t_act_src.e_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
            t_nfy_data.u_data.t_act_src.ui2_comp_id = cecm_ctrl_cal_comp_id_from_pa(GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]));
            _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
        }
    }
    else /* Other */
    {
        BOOL b_menu_state_act = FALSE;
        if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr), TRUE)) != NULL)
        {
            cecm_lock_rd_db();
            if(pt_dev_rec->t_dev.e_menu_req_state == CECM_MENU_REQ_STATE_ACT)
            {
                b_menu_state_act = TRUE;
            }
            cecm_unlock_db();
        }
        /* ignore active source if menu status is active */
        if(!b_menu_state_act)
        {
            t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
            t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
            t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
            t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                        CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
            t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
            t_nfy_data.u_code.e_opcode = CECM_OPCODE_ACT_SRC;
            t_nfy_data.u_data.t_act_src.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
            t_nfy_data.u_data.t_act_src.e_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
            t_nfy_data.u_data.t_act_src.ui2_comp_id = cecm_ctrl_cal_comp_id_from_pa(GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]));
            _cecm_notify_client(&t_nfy_data, NULL, FALSE);
        }

    }

    /* Fix the issue that <Text View On>/<Image View On> is received before <Active Source> after <Set Stream Path> for select_device is sent */
    if (b_ssp_check)
    {
#ifdef CECM_PWR_ON_DEV_EX
        if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                      CECM_OPCODE_SET_STREAM_PATH)) != NULL) /* Set stream path */
        {
            if ((pt_evt->e_type == CECM_EVT_TYPE_PWR_STS) &&
                cecm_ctrl_is_select_dev_running())
            {
                cecm_ctrl_select_dev(FALSE, NULL);
                t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr; /* Device LA */
                t_nfy_data.e_sent_opcode = pt_evt->e_opcode;

                /* <Active Source> notification */
                t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
                t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                            CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
                t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                t_nfy_data.u_code.e_opcode = CECM_OPCODE_ACT_SRC;
                t_nfy_data.u_data.t_act_src.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
                t_nfy_data.u_data.t_act_src.e_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
                t_nfy_data.u_data.t_act_src.ui2_comp_id = cecm_ctrl_cal_comp_id_from_pa(GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]));
                _cecm_notify_client(&t_nfy_data, NULL, FALSE);

                /* Select_device notification */
                t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
                t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
                t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_ON_THEN_ACT_SRC;
                t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
                _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
            }
        }

#endif
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_inact_src
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_inact_src(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_DEV_REC_T*     pt_dev_rec;
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_INACT_SRC_SIZE)
    {
        return;
    }
    
    DBG_INFO(("{CECM} >>>> InActive Source: phy_addr = 0x%x\n\r", GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0])));

    /* Update device record for inactive source change */
    if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr), FALSE)) != NULL)
    {
        cecm_lock_wr_db();
        cecm_ctrl_update_act_src(pt_dev_rec, FALSE);
        cecm_unlock_db();
    }

    if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr), TRUE)) != NULL)
    {
        cecm_lock_wr_db();
        cecm_ctrl_update_act_src(pt_dev_rec, FALSE);
        cecm_unlock_db();
    }

    t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_INACT_SRC;
    t_nfy_data.u_data.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
    _cecm_notify_client(&t_nfy_data, NULL, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_req_act_src
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_req_act_src(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr != CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != 0)
    {
        return;
    }

    t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_REQ_ACT_SRC;
    t_nfy_data.u_data.e_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    _cecm_notify_client(&t_nfy_data, NULL, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_img_txt_view_on
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_img_txt_view_on(CEC_FRAME_INFO_T* pt_frame_info)
{
    VOID*               pv_obj = NULL;
    CECM_NFY_T          t_nfy_data;
    HANDLE_TYPE_T       e_type;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != 0)
    {
        return;
    }

    /* Ignore duplicated image_view_on or text_view_on messages */
    if ((cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                        CECM_OPCODE_IMG_VIEW_ON) != NULL) ||
        (cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                        CECM_OPCODE_TXT_VIEW_ON) != NULL))
    {
        return;
    }

    /* Obtain CECM internal service */
    if (handle_get_type_obj(cecm_ctrl_get_internal_handle(), &e_type, &pv_obj) != HR_OK)
    {
        return;
    }
    else if(e_type != CECM_HANDLE_TYPE)
    {
            #ifdef DEBUG
            dbg_abort(DBG_MOD_CEC_MNGR | DBG_CAT_HANDLE);
            #endif
            return;
    }

    /* Queue the event */
    if ((pt_evt_one_touch_play = cecm_ctrl_q_evt((CECM_SVC_T*)pv_obj, CECM_EVT_TYPE_ONE_TOUCH_PLAY, pt_frame_info, NULL)) == NULL)
    {
        DBG_ERROR(("{CECM} ERR: _cecm_parse_img_txt_view_on() failed\n\r"));
        return;
    }
    else
    {
        /* Set a timer to wait for active source signal */
        x_timer_start(h_timer_opt,
                      CECM_TIMEOUT_ONE_TOUCH_PLAY,
                      X_TIMER_FLAG_ONCE,
                      _cecm_timer_one_touch_play_cb,
                      (VOID*)pt_evt_one_touch_play);
    }

    /* Notify clients */
    t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = (CECM_OPCODE_T)pt_frame_info->ui2_opcode;;
    _cecm_notify_client(&t_nfy_data, NULL, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_routing_info
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_routing_info(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr != CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_ROUTING_INFO_SIZE)
    {
        return;
    }
    DBG_INFO(("{CECM} >>>> Routing Info: phy_addr = 0x%x\n\r", GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0])));

    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_ROUTING_INFO;
    t_nfy_data.u_data.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);

    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_ROUTING_CHG)) != NULL)
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr; /* Device LA */
        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_standby
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_standby(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if (pt_frame_info->z_operand_size != 0)
    {
        return;
    }

    t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_STANDBY;
    _cecm_notify_client(&t_nfy_data, NULL, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_give_phy_addr
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_give_phy_addr(CEC_FRAME_INFO_T* pt_frame_info)
{
    INT32               i4_return;
    CEC_FRAME_INFO_T    t_frame_info;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != 0)
    {
        return;
    }

    t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
    t_frame_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
    t_frame_info.ui2_opcode = CECM_OPCODE_REPORT_PHY_ADDR;
    t_frame_info.aui1_operand[0] = 0; /* TV physical address = 0x0000 */
    t_frame_info.aui1_operand[1] = 0;
    t_frame_info.aui1_operand[2] = CECM_DEV_TYPE_TV;
    t_frame_info.z_operand_size = CECM_OPCODE_REPORT_PHY_ADDR_SIZE;

    i4_return = cecm_ctrl_send_msg(NULL_HANDLE, CECM_EVT_TYPE_NORMAL, &t_frame_info, NULL);

    if (i4_return != TRUE)
    {
        DBG_ERROR(("{CECM} ERR: _cecm_parse_give_phy_addr() failed\n\r"));
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_report_phy_addr
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *          b_mw_cec_nfy        Indicates whether the message is issued by MW 
 *                              or driver. 
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_report_phy_addr(CEC_FRAME_INFO_T* pt_frame_info,
                                        BOOL              b_mw_cec_nfy)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_EVT_T*         pt_evt_gpa = NULL;
    CECM_DEV_REC_T*     pt_dev_rec = NULL;
    CECM_NFY_T          t_nfy_data;
    CECM_MSG_T          t_msg;
    BOOL                b_dev_updated = FALSE;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        (CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr != CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_REPORT_PHY_ADDR_SIZE)
    {
        return;
    }
	if (b_mw_cec_nfy)
	{
		DBG_INFO(("{CECM} >>>> Report phy addr: phy_addr = 0x%x mw_cec_nfy\n\r", GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0])));
	}
	else
	{
		DBG_INFO(("{CECM} >>>> Report phy addr: phy_addr = 0x%x\n\r", GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0])));
	}

    /* Update device record for physical address report */
    if (((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr), FALSE)) != NULL)
		 && !b_mw_cec_nfy)      
    {
        cecm_lock_wr_db();
        pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
        pt_dev_rec->t_dev.t_report_phy_addr.e_dev_type = (CECM_DEV_TYPE_T)pt_frame_info->aui1_operand[2];
        cecm_unlock_db();
    }

    if (((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr), TRUE)) != NULL)
		&& !b_mw_cec_nfy)      
    {
        cecm_lock_wr_db();
        if(pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa != GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]))
        {
            pt_dev_rec->b_phy_addr_updated = TRUE;
            b_dev_updated = TRUE;
        }
        pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
        pt_dev_rec->t_dev.t_report_phy_addr.e_dev_type = (CECM_DEV_TYPE_T)pt_frame_info->aui1_operand[2];
        cecm_unlock_db();
    }
    
    /* The following two statements are for one touch play enhancement */
    if (((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_IMG_VIEW_ON)) != NULL) /* Image view on */
        && !b_mw_cec_nfy)                                                        
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
        t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
        t_nfy_data.ui4_attr = CECM_ATTR_QUERY_PHY_ADDR;
        t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_ONE_TOUCH_PLAY;
        t_nfy_data.u_data.t_one_touch_play.e_la = (CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr);
        t_nfy_data.u_data.t_one_touch_play.ui2_comp_id = cecm_ctrl_cal_comp_id_from_pa(GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]));
        t_nfy_data.u_data.t_one_touch_play.e_action = CECM_OTP_ACT_IMG_VIEW_ON;
        t_nfy_data.u_data.t_one_touch_play.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);

        e_otp_state = CECM_OTP_NONE;
        pt_evt_one_touch_play = NULL;
        t_nfy_data.ui4_attr = CECM_ATTR_REQ_ACT_SRC;

        /* Remove <Give Physical Address> event */
        if ((pt_evt_gpa = cecm_ctrl_get_evt_by_type_la_opcode(CECM_EVT_TYPE_ONE_TOUCH_PLAY,
                                                              (CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                              CECM_OPCODE_GIVE_PHY_ADDR)) != NULL)
        {
            cecm_ctrl_evt_free(pt_evt_gpa);
        }
    }
    else if (((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                      CECM_OPCODE_TXT_VIEW_ON)) != NULL) /* Text view on */
             && !b_mw_cec_nfy)                                                         
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
        t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
        t_nfy_data.ui4_attr = CECM_ATTR_QUERY_PHY_ADDR;
        t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_ONE_TOUCH_PLAY;
        t_nfy_data.u_data.t_one_touch_play.e_la = (CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr);
        t_nfy_data.u_data.t_one_touch_play.ui2_comp_id = cecm_ctrl_cal_comp_id_from_pa(GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]));
        t_nfy_data.u_data.t_one_touch_play.e_action = CECM_OTP_ACT_TXT_VIEW_ON;
        t_nfy_data.u_data.t_one_touch_play.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);

        e_otp_state = CECM_OTP_NONE;
        pt_evt_one_touch_play = NULL;
        t_nfy_data.ui4_attr = CECM_ATTR_REQ_ACT_SRC;

        /* Remove <Give Physical Address> event */
        if ((pt_evt_gpa = cecm_ctrl_get_evt_by_type_la_opcode(CECM_EVT_TYPE_ONE_TOUCH_PLAY,
                                                              (CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                              CECM_OPCODE_GIVE_PHY_ADDR)) != NULL)
        {
            cecm_ctrl_evt_free(pt_evt_gpa);
        }
    }
    else
    {
        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
        t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                    CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
        t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
        t_nfy_data.u_code.e_opcode = CECM_OPCODE_REPORT_PHY_ADDR;
        t_nfy_data.u_data.t_report_phy_addr.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
        t_nfy_data.u_data.t_report_phy_addr.e_dev_type = (CECM_DEV_TYPE_T)pt_frame_info->aui1_operand[2];

        if (((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                     CECM_OPCODE_GIVE_PHY_ADDR)) != NULL)
            && !b_mw_cec_nfy)                                                        
        {
            if (pt_evt->e_type == CECM_EVT_TYPE_DEV_DISCOVERY)
            {
                if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_evt->e_la, FALSE)) == NULL)
                {
                    return;
                }

                cecm_lock_wr_db();
                pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
                pt_dev_rec->t_dev.t_report_phy_addr.e_dev_type = (CECM_DEV_TYPE_T)pt_frame_info->aui1_operand[2];
                pt_dev_rec->b_post_polling_retry = FALSE;
                cecm_unlock_db();

                _cecm_update_dev_discovery_mask(pt_evt, pt_dev_rec);

                /* Free the using event */
                cecm_ctrl_evt_free(pt_evt);
            }
            else
            {
                t_nfy_data.e_sent_la = pt_evt->e_la;
                t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
                _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
            }
        }
        else
        {
            if (!b_mw_cec_nfy)
            {                
                t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
                t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
                _cecm_notify_client(&t_nfy_data, NULL, FALSE);
                /*Fix the issue found in compliance test section 11.1.11 [LA 1]*/
                /*Send <Give OSD Name > when discovery new device <Report Physical Address>*/
                CEC_FRAME_INFO_T t_frame_info;
                t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
                t_frame_info.ui1_dest_addr = pt_frame_info->ui1_init_addr;
                t_frame_info.ui2_opcode = CECM_OPCODE_GIVE_OSD_NAME;
                t_frame_info.aui1_operand[0] = 0;
                t_frame_info.z_operand_size = 0;   
                cecm_ctrl_send_msg(cecm_ctrl_get_internal_handle(), CECM_EVT_TYPE_NORMAL, &t_frame_info, NULL);
           }                  

#if 1 /* Fix the issue found in compliance test section: 11.1.11-1 [LA 14] */
            /* Update device record for physical address report */

            if (((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr), FALSE)) == NULL) ||
                (!b_dev_updated && pt_dev_rec->e_post_polling_runner == CECM_PP_RUNNER_DEV))
            {
                return;
            }

            cecm_lock_wr_db();
            if (cecm_ctrl_is_dev_discovery_running())
            {
                DBG_INFO(("{CECM} Send report phy_add msg to msgq : 0x%x (la=%d)\n\r",
                           pt_frame_info->ui2_opcode,
                           pt_frame_info->ui1_init_addr)); 

                /*send msg to msgq if discovery is running  and will triger discovery for this device when completed*/
                pt_dev_rec->t_msg.e_msg_type = CECM_MSG_TYPE_CEC_NFY_MW;
                pt_dev_rec->t_msg.u.t_cec_nfy.e_nfy_type = CEC_NFY_TYPE_FRAME;
                pt_dev_rec->t_msg.u.t_cec_nfy.u.t_frame_info = *pt_frame_info;
                cecm_unlock_db();

            }
            else
            {
                /* stop device disovery first*/
                if ((pt_dev_rec->t_dev.e_dev_state == CECM_DEV_STATE_UPDATING))
                {       
                    if ((pt_dev_rec->t_dev.e_dev_state != CECM_DEV_STATE_UPDATED) &&
                        (pt_dev_rec->e_post_polling_runner == CECM_PP_RUNNER_DEV))
                    {
                         ui1_dev_post_polling_cnt--;                           
                    }
                    pt_dev_rec->t_dev.e_dev_state = CECM_DEV_STATE_UPDATED;
                    /*free device discovery event*/
                    if ((pt_evt = cecm_ctrl_get_evt_by_type_la(CECM_EVT_TYPE_DEV_DISCOVERY,
                                                               (CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr))) != NULL)
                    {
                        cecm_ctrl_evt_free(pt_evt);
                    }  
                    DBG_INFO(("{CECM} <<<<<<<<<< Dev post-polling stop (%d->%d) >>>>>>>>>>\n\r",
                             pt_frame_info->ui1_init_addr,
                             pt_frame_info->ui1_init_addr));                    
                }
                /*triger device discovery*/
                if ((pt_dev_rec->t_dev.e_dev_state == CECM_DEV_STATE_INIT) ||
                    (pt_dev_rec->t_dev.e_dev_state == CECM_DEV_STATE_UPDATED))
                {
                    pt_dev_rec->ui1_polling_retry = 0;
                    pt_dev_rec->e_post_polling_runner = CECM_PP_RUNNER_DEV;
                    pt_dev_rec->b_post_polling_handled = FALSE;
                    pt_dev_rec->b_post_polling_retry = FALSE;
                    pt_dev_rec->ui4_update_mask = CECM_UPDATE_MASK_NONE;
                    pt_dev_rec->t_dev.e_dev_state = CECM_DEV_STATE_UPDATING;
                    pt_dev_rec->t_dev.b_dev_present = TRUE;
                    pt_dev_rec->t_dev.b_act_src = FALSE;
                    pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
                    pt_dev_rec->t_dev.t_report_phy_addr.e_dev_type = (CECM_DEV_TYPE_T)pt_frame_info->aui1_operand[2];
                    x_memset(&(pt_dev_rec->t_dev.aw2_osd_name)[0], 0, CECM_OSD_NAME_SIZE);
                    x_memset(&pt_dev_rec->t_dev.t_dev_vndr_id, 0, CECM_DEV_VNDR_ID_SIZE);                  
                    pt_dev_rec->t_dev.e_cec_ver = CECM_CEC_VER_INVALID;
                    pt_dev_rec->t_dev.e_pwr_sts = CECM_PWR_STS_ERROR;
                    x_memset(&(pt_dev_rec->t_dev.t_dev_vndr_info), 0, sizeof(CECM_DEV_VNDR_INFO_T)); 
                    pt_dev_rec->e_abort_reason_cev_ver = CECM_ABORT_REASON_NONE;
                    pt_dev_rec->e_abort_reason_pwr_sts = CECM_ABORT_REASON_NONE;                   
                    pt_dev_rec->e_abort_reason_pa = CECM_ABORT_REASON_NONE;
                    pt_dev_rec->e_abort_reason_dev_vndr_id = CECM_ABORT_REASON_NONE;
                    pt_dev_rec->e_abort_reason_osd_name = CECM_ABORT_REASON_NONE;
                    pt_dev_rec->ui1_vndr_cmd_idx = 0;
                    pt_dev_rec->ui4_skip_update_mask = CECM_UPDATE_MASK_NONE;
                    x_memset(&(pt_dev_rec->ae_abort_reason_vndr_info[0]),CECM_ABORT_REASON_NONE, CECM_DD_VNDR_CMD_MAX_SIZE);   
                    ui1_dev_post_polling_cnt++;
                    cecm_unlock_db();

                    DBG_INFO(("{CECM} <<<<<<<<<< Dev post-polling begins (%d->%d) >>>>>>>>>>\n\r",
                             pt_frame_info->ui1_init_addr,
                             pt_frame_info->ui1_init_addr));
                    _cecm_start_post_polling((VOID*)&(pt_frame_info->ui1_init_addr), FALSE);
                }
                else
                {
                    cecm_unlock_db();
                    t_msg.e_msg_type = CECM_MSG_TYPE_CEC_NFY_MW;
                    t_msg.u.t_cec_nfy.e_nfy_type = CEC_NFY_TYPE_FRAME;
                    t_msg.u.t_cec_nfy.u.t_frame_info = *pt_frame_info;

                    x_msg_q_send(*ph_cecm_msgq,
                                 (VOID*)&t_msg,
                                 sizeof(CECM_MSG_T),
                                 CECM_MSGQ_DEFAULT_PRIORITY);
                }
            }
#endif
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_set_osd_string
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_set_osd_string(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_NFY_T          t_nfy_data;
    UINT8               aui1_operand[CEC_MAX_OPERAND_SIZE];
    SIZE_T              z_operand_size;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size < 1 ||
        pt_frame_info->z_operand_size > CEC_MAX_OPERAND_SIZE)
    {
        return;
    }

    t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_SET_OSD_STRING;
    t_nfy_data.u_data.t_osd_string_info.e_display_ctrl = (CECM_DISPLAY_CTRL_T)(pt_frame_info->aui1_operand[0]);

    x_memset(&aui1_operand[0], 0, CEC_MAX_OPERAND_SIZE);  //reset string init

    x_memcpy(&aui1_operand[0], &pt_frame_info->aui1_operand[1], pt_frame_info->z_operand_size - 1);
    	pt_frame_info->aui1_operand[pt_frame_info->z_operand_size - 1] = (UINT8)'\0';
	z_operand_size = pt_frame_info->z_operand_size - 1;

    x_uc_ps_to_w2s((const CHAR*)&aui1_operand[0],
                   &(t_nfy_data.u_data.t_osd_string_info.aw2_osd_string)[0],
                   z_operand_size);

    _cecm_notify_client(&t_nfy_data, NULL, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_set_osd_name
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_set_osd_name(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;
    CECM_DEV_REC_T*     pt_dev_rec = NULL;
    UINT8               aui1_operand[CECM_PROG_TITLE_STRING_SIZE];
    SIZE_T              z_operand_size;

    x_memset(&t_nfy_data, 0, sizeof(CECM_NFY_T)); 


    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size < 1 ||
        pt_frame_info->z_operand_size >= CECM_PROG_TITLE_STRING_SIZE)
    {
        return;
    }

    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_SET_OSD_NAME;

    cecm_lock_wr_db();
    x_memset(&aui1_operand[0], 0, CECM_OSD_NAME_SIZE);  //reset string init
    x_memcpy(&aui1_operand[0], &pt_frame_info->aui1_operand[0], pt_frame_info->z_operand_size);
	aui1_operand[pt_frame_info->z_operand_size] = (UINT8)'\0';
	z_operand_size = pt_frame_info->z_operand_size + 1;

    x_uc_ps_to_w2s((const CHAR*)&aui1_operand[0],
                   &(t_nfy_data.u_data.aw2_osd_name)[0],
                   z_operand_size);
    cecm_unlock_db();
    
    /* Update device record for OSD name report */
    if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr), FALSE)) != NULL)
    {
        cecm_lock_wr_db();
        x_memset(&(pt_dev_rec->t_dev.aw2_osd_name)[0], 0, CECM_OSD_NAME_SIZE);
        x_uc_ps_to_w2s((const CHAR*)&aui1_operand[0],
                       &(pt_dev_rec->t_dev.aw2_osd_name)[0],
                       z_operand_size);
        cecm_unlock_db();
    }

    if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr), TRUE)) != NULL)
    {
        cecm_lock_wr_db();
        x_memset(&(pt_dev_rec->t_dev.aw2_osd_name)[0], 0, CECM_OSD_NAME_SIZE);
        x_uc_ps_to_w2s((const CHAR*)&aui1_operand[0],
                       &(pt_dev_rec->t_dev.aw2_osd_name)[0],
                       pt_frame_info->z_operand_size);
        cecm_unlock_db();
    }

    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_GIVE_OSD_NAME)) != NULL)
    {
        if (pt_evt->e_type == CECM_EVT_TYPE_DEV_DISCOVERY)
        {
            if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_evt->e_la, FALSE)) != NULL)
            {
                cecm_lock_wr_db();
                pt_dev_rec->b_post_polling_retry = FALSE;
                cecm_unlock_db();

                _cecm_update_dev_discovery_mask(pt_evt, pt_dev_rec);

                /* Free the using event */
                cecm_ctrl_evt_free(pt_evt);
            }                
        }
        else
        {
            t_nfy_data.e_sent_la = pt_evt->e_la;
            t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
            _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
        }
    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_dev_vndr_id
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_dev_vndr_id(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;
    CECM_DEV_REC_T*     pt_dev_rec = NULL;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr != CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_DEV_VNDR_ID_SIZE)
    {
        return;
    }

    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_DEV_VNDR_ID;
    x_memcpy(&t_nfy_data.u_data.t_dev_vndr_id, &pt_frame_info->aui1_operand[0], CECM_OPCODE_DEV_VNDR_ID_SIZE);

    /* Update device record for vendor ID report */
    if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr), FALSE)) != NULL)
    {
        cecm_lock_wr_db();
        x_memcpy(&pt_dev_rec->t_dev.t_dev_vndr_id, &pt_frame_info->aui1_operand[0], CECM_OPCODE_DEV_VNDR_ID_SIZE);
        cecm_unlock_db();
    }

    if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr), TRUE)) != NULL)
    {
        cecm_lock_wr_db();
        x_memcpy(&pt_dev_rec->t_dev.t_dev_vndr_id, &pt_frame_info->aui1_operand[0], CECM_OPCODE_DEV_VNDR_ID_SIZE);
        cecm_unlock_db();
    }
    
    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_GIVE_DEV_VNDR_ID)) != NULL)
    {
        if (pt_evt->e_type == CECM_EVT_TYPE_DEV_DISCOVERY)
        {
            if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_evt->e_la, FALSE)) != NULL)
            {
                cecm_lock_wr_db();
                pt_dev_rec->b_post_polling_retry = FALSE;
                cecm_unlock_db();
    
                _cecm_update_dev_discovery_mask(pt_evt, pt_dev_rec);
    
                /* Free the using event */
                cecm_ctrl_evt_free(pt_evt);
            }                
        }
        else
        {
            if (pt_evt->h_sync)
            {
                pt_evt->pt_sync_data->e_sync_op = CECM_SYNC_OP_SUCCESS;
                x_memcpy(pt_evt->pt_sync_data->pv_data, &pt_frame_info->aui1_operand[0], CECM_OPCODE_DEV_VNDR_ID_SIZE);
                cecm_sync_unlock(pt_evt->h_sync);
                cecm_ctrl_evt_free(pt_evt);
            }
            else
            {            
                t_nfy_data.e_sent_la = pt_evt->e_la;
                t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
                _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
            }                
        }
    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_give_dev_pwr_sts
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_give_dev_pwr_sts(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != 0)
    {
        return;
    }

    t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_GIVE_DEV_PWR_STS;
    t_nfy_data.u_data.e_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    _cecm_notify_client(&t_nfy_data, NULL, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_report_pwr_sts
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_report_pwr_sts(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;
    CECM_PWR_STS_T      e_pwr_sts;
    CECM_DEV_REC_T*     pt_dev_rec = NULL;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_REPORT_PWR_STS_SIZE)
    {
        return;
    }

    if (((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr), FALSE)) != NULL)&& 
        (t_dd_config.t_dd_get_info.b_pwr_sts == TRUE))
    {
        cecm_lock_wr_db();
        pt_dev_rec->t_dev.e_pwr_sts = (CECM_PWR_STS_T)(pt_frame_info->aui1_operand[0]);
        cecm_unlock_db();        
    }
    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_GIVE_DEV_PWR_STS)) != NULL)
    {
        if (pt_evt->e_type == CECM_EVT_TYPE_PWR_STS) /* Select_device operation */
        {
            e_pwr_sts = (CECM_PWR_STS_T)(pt_frame_info->aui1_operand[0]);
            DBG_INFO(("{CECM} >>>> pwr_sts = %d\n\r", e_pwr_sts));

#ifdef CECM_PWR_ON_DEV_EX
            switch (e_pwr_sts)
            {
                case CECM_PWR_STS_ON:
                    if (b_pwr_on_dev)
                    {
                        cecm_psr_pwr_on_dev(FALSE);
                    }

                    /* Send <Set Stream Path> */
                    _cecm_pwr_on_dev_send_msg(CECM_OPCODE_SET_STREAM_PATH, pt_evt, NULL);
                    break;

                case CECM_PWR_STS_STANDBY:
                    if (b_recv_pwr_standby)
                    {
                        b_recv_pwr_standby = FALSE;

                        /* Send <Set Stream Path> */
                        _cecm_pwr_on_dev_send_msg(CECM_OPCODE_SET_STREAM_PATH, pt_evt, NULL);
                    }
                    else
                    {
                        b_recv_pwr_standby = TRUE;

                        /* Send <User Control Pressed> - POWER */
                        e_usr_ctrl = CECM_USER_CTRL_POWER;
                        if (_cecm_pwr_on_dev_send_msg(CECM_OPCODE_USER_CTRL_PRESSED, pt_evt, (UINT32*)&e_usr_ctrl) == TRUE)
                        {
                            /* Start timers for power_on_device */
                            cecm_psr_pwr_on_dev(TRUE);
                        }
                    }
                    break;

                case CECM_PWR_STS_STANDBY_TO_ON:
                case CECM_PWR_STS_ON_TO_STANDBY:
                    b_recv_pwr_standby = FALSE;

                    /* Start timers for power_on_device retry */
                    x_timer_start(h_timer_pwr_on_dev_retry,
                                  (CECM_TIMEOUT_PWR_ON_DEV_RETRY * 1000),
                                  X_TIMER_FLAG_ONCE,
                                  _cecm_timer_pwr_on_dev_retry_cb,
                                  (VOID*)(cecm_ctrl_get_select_dev_info()));
                    break;
                default :
                    break;
                 
            }

            /* Free the using event */
            cecm_ctrl_evt_free(pt_evt);
#else
            INT32               i4_return;
            CEC_FRAME_INFO_T    t_frame_info;
            CECM_EVT_T          t_evt;
            CECM_DEV_REC_T*     pt_dev_rec = NULL;

            switch (e_pwr_sts)
            {
                case CECM_PWR_STS_ON:                    
                    if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_evt->e_la, TRUE)) != NULL)
                    {
                        t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
                        t_frame_info.ui2_opcode = CECM_OPCODE_SET_STREAM_PATH;                        
                        t_frame_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
                        t_frame_info.aui1_operand[0] = (UINT8)(pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa >> 8);
                        t_frame_info.aui1_operand[1] = (UINT8)pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa;
                        t_frame_info.z_operand_size = CECM_OPCODE_SET_STREAM_PATH_SIZE;  
                        cecm_ctrl_send_msg(pt_evt->pt_svc->h_svc, CECM_EVT_TYPE_NORMAL, &t_frame_info, NULL);
                    }
                    cecm_ctrl_select_dev(FALSE, NULL);                    
                    t_nfy_data.e_sent_la = pt_evt->e_la;
                    t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
                    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                    t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
                    t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = (CECM_PWR_STS_T)(pt_frame_info->aui1_operand[0]);
                    t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = cecm_ctrl_get_pa_by_la(pt_evt->e_la);
                    _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
                    
                    break;

                case CECM_PWR_STS_STANDBY:
                    /* Send user control pressed */
                    t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
                    t_frame_info.ui1_dest_addr = pt_frame_info->ui1_init_addr;
                    t_frame_info.ui2_opcode = CECM_OPCODE_USER_CTRL_PRESSED;
                    t_frame_info.aui1_operand[0] = (UINT8)CECM_USER_CTRL_POWER;
                    t_frame_info.z_operand_size = CECM_OPCODE_USER_CTRL_PRESSED_SIZE;

                    i4_return = cecm_ctrl_send_msg(pt_evt->pt_svc->h_svc, CECM_EVT_TYPE_PWR_STS, &t_frame_info, NULL);

                    if (i4_return != TRUE)
                    {
                        t_nfy_data.e_sent_la = pt_evt->e_la;
                        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
                        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                        t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                        t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
                        t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_ERROR;
                        t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = cecm_ctrl_get_pa_by_la(pt_evt->e_la);
                        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
                    }
                    /* Free the using event */
                    cecm_ctrl_evt_free(pt_evt);
                    break;
                case CECM_PWR_STS_ON_TO_STANDBY:
                case CECM_PWR_STS_STANDBY_TO_ON:

                    /* Free the using event */
                    t_evt = *pt_evt;
                    cecm_ctrl_evt_free(pt_evt);

                    x_thread_delay(CECM_PWR_STS_STANDBY_TO_ON_ITVL);

                    /* Query power status again */
                    t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
                    t_frame_info.ui1_dest_addr = pt_frame_info->ui1_init_addr;
                    t_frame_info.ui2_opcode = CECM_OPCODE_GIVE_DEV_PWR_STS;
                    t_frame_info.aui1_operand[0] = 0;
                    t_frame_info.z_operand_size = 0;

                    i4_return = cecm_ctrl_send_msg(t_evt.pt_svc->h_svc, CECM_EVT_TYPE_PWR_STS, &t_frame_info, NULL);

                    if (i4_return != TRUE)
                    {
                        t_nfy_data.e_sent_la = t_evt.e_la;
                        t_nfy_data.e_sent_opcode = t_evt.e_opcode;
                        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                        t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                        t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
                        t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_ERROR;
                        t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = cecm_ctrl_get_pa_by_la(t_evt.e_la);
                        _cecm_notify_client(&t_nfy_data, &t_evt, FALSE);
                    }
                    
                    break;
                default :
                    break;             
            }
#endif
        }
        else if (pt_evt->e_type == CECM_EVT_TYPE_DEV_DISCOVERY) /* Device discovery  operation */
        {
            CECM_DEV_REC_T*     pt_dev_rec = NULL;
            if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_evt->e_la, FALSE)) != NULL)
            {
                cecm_lock_wr_db();
                pt_dev_rec->b_post_polling_retry = FALSE;
                pt_dev_rec->t_dev.e_pwr_sts = (CECM_PWR_STS_T)(pt_frame_info->aui1_operand[0]);
                cecm_unlock_db();

                _cecm_update_dev_discovery_mask(pt_evt, pt_dev_rec);

                /* Free the using event */
                cecm_ctrl_evt_free(pt_evt);
            }  
        }
        else /* Response to <Give Device Power Status> query from a client */
        {
            if (pt_evt->h_sync)
            {
                pt_evt->pt_sync_data->e_sync_op = CECM_SYNC_OP_SUCCESS;
                *((CECM_PWR_STS_T*)(pt_evt->pt_sync_data->pv_data)) = (CECM_PWR_STS_T)pt_frame_info->aui1_operand[0];
                cecm_sync_unlock(pt_evt->h_sync);
                cecm_ctrl_evt_free(pt_evt);
            }
            else
            {
                t_nfy_data.e_sent_la = pt_evt->e_la;
                t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
                t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
                t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
                t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                t_nfy_data.u_code.e_opcode = CECM_OPCODE_REPORT_PWR_STS;
                t_nfy_data.u_data.e_pwr_sts = (CECM_PWR_STS_T)(pt_frame_info->aui1_operand[0]);
                _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
            }
        }
    }
    else /* <Report Power Status> message from device actively */
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
        t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                    CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
        t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
        t_nfy_data.u_code.e_opcode = CECM_OPCODE_REPORT_PWR_STS;
        t_nfy_data.u_data.e_pwr_sts = (CECM_PWR_STS_T)(pt_frame_info->aui1_operand[0]);
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_get_menu_lang
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_get_menu_lang(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != 0)
    {
        return;
    }

    t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_GET_MENU_LANG;
    t_nfy_data.u_data.e_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    _cecm_notify_client(&t_nfy_data, NULL, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_set_menu_lang
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_set_menu_lang(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr != CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_SET_MENU_LANG_SIZE)
    {
        return;
    }

    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_SET_MENU_LANG;
    x_memcpy(&t_nfy_data.u_data.t_menu_lang, &pt_frame_info->aui1_operand[0], CECM_OPCODE_SET_MENU_LANG_SIZE);

    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_GET_MENU_LANG)) != NULL)
    {
        t_nfy_data.e_sent_la = pt_evt->e_la;
        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_get_cec_ver
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_get_cec_ver(CEC_FRAME_INFO_T* pt_frame_info)
{
    INT32               i4_return;
    CEC_FRAME_INFO_T    t_frame_info;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != 0)
    {
        return;
    }

    t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
    t_frame_info.ui1_dest_addr = pt_frame_info->ui1_init_addr;
    t_frame_info.ui2_opcode = CECM_OPCODE_CEC_VER;
    t_frame_info.aui1_operand[0] = (UINT8)cecm_ctrl_get_support_cec_ver();
    t_frame_info.z_operand_size = CECM_OPCODE_CEC_VER_SIZE;

    i4_return = cecm_ctrl_send_msg(NULL_HANDLE, CECM_EVT_TYPE_NORMAL, &t_frame_info, NULL);

    if (i4_return != TRUE)
    {
        DBG_ERROR(("{CECM} ERR: _cecm_parse_get_cec_ver() failed\n\r"));
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_cec_ver
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_cec_ver(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;
    CECM_DEV_REC_T*     pt_dev_rec = NULL;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_CEC_VER_SIZE)
    {
        return;
    }

    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_CEC_VER;
    t_nfy_data.u_data.e_cec_ver = (CECM_CEC_VER_T)(pt_frame_info->aui1_operand[0]);

    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_GET_CEC_VER)) != NULL)
    {

        if (pt_evt->e_type == CECM_EVT_TYPE_DEV_DISCOVERY)
        {
            if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_evt->e_la, FALSE)) != NULL)
            {
                cecm_lock_wr_db();
                pt_dev_rec->b_post_polling_retry = FALSE;
                pt_dev_rec->t_dev.e_cec_ver = (CECM_CEC_VER_T)(pt_frame_info->aui1_operand[0]);
                cecm_unlock_db();

                _cecm_update_dev_discovery_mask(pt_evt, pt_dev_rec);

                /* Free the using event */
                cecm_ctrl_evt_free(pt_evt);
            }                
        }
        else
        {
            t_nfy_data.e_sent_la = pt_evt->e_la;
            t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
            _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);        
        }


    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_give_deck_sts
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_give_deck_sts(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_GIVE_DECK_STS_SIZE)
    {
        return;
    }

    t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_GIVE_DECK_STS;
    t_nfy_data.u_data.e_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    _cecm_notify_client(&t_nfy_data, NULL, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_deck_sts
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_deck_sts(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_DECK_STS_SIZE)
    {
        return;
    }

    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_DECK_STS;
    t_nfy_data.u_data.e_deck_info = (CECM_DECK_INFO_T)(pt_frame_info->aui1_operand[0]);

    /* This opcode may responds to CECM_OPCODE_PLAY, CECM_OPCODE_GIVE_DECK_STS and CECM_OPCODE_DECK_CTRL */
    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_DECK_STS)) != NULL)
    {
        t_nfy_data.e_sent_la = pt_evt->e_la;
        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_menu_sts
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_menu_sts(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;
    CECM_DEV_REC_T*     pt_dev_rec = NULL;
    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_MENU_STS_SIZE)
    {
        return;
    }
    /* Update device record for active source change */
    if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr), FALSE)) != NULL)
    {
        cecm_lock_wr_db();
        pt_dev_rec->t_dev.e_menu_req_state = (CECM_MENU_REQ_STATE_T)(pt_frame_info->aui1_operand[0]);;
        cecm_unlock_db();
    }

    if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr), TRUE)) != NULL)
    {
        cecm_lock_wr_db();
        pt_dev_rec->t_dev.e_menu_req_state = (CECM_MENU_REQ_STATE_T)(pt_frame_info->aui1_operand[0]);;
        cecm_unlock_db();
    }


    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_MENU_STS;
    t_nfy_data.u_data.e_menu_req_state = (CECM_MENU_REQ_STATE_T)(pt_frame_info->aui1_operand[0]);

    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_MENU_REQ)) != NULL)
    {
        t_nfy_data.e_sent_la = pt_evt->e_la;
        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_dev_vndr_id
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_give_dev_vndr_id(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != 0)
    {
        return;
    }

    t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_GIVE_DEV_VNDR_ID;
    t_nfy_data.u_data.e_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    _cecm_notify_client(&t_nfy_data, NULL, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_set_sys_aud_mode
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_set_sys_aud_mode(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if (pt_frame_info->z_operand_size != CECM_OPCODE_SET_SYS_AUD_MODE_SIZE)
    {
        return;
    }

    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_SYS_AUD_MOD_REQ)) != NULL)
    {
        if (pt_evt->h_sync)
        {
            pt_evt->pt_sync_data->e_sync_op = CECM_SYNC_OP_SUCCESS;
            *((CECM_SYS_AUD_STS_T*)(pt_evt->pt_sync_data->pv_data)) = (CECM_SYS_AUD_STS_T)pt_frame_info->aui1_operand[0];
            cecm_sync_unlock(pt_evt->h_sync);
            cecm_ctrl_evt_free(pt_evt);
        }
        else
        {
            t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
            t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                        CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
            t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
            t_nfy_data.u_code.e_opcode = CECM_OPCODE_SET_SYS_AUD_MOD;
            t_nfy_data.u_data.e_sys_aud_sts = (CECM_SYS_AUD_STS_T)(pt_frame_info->aui1_operand[0]);
            t_nfy_data.e_sent_la = pt_evt->e_la;
            t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
            _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
        }
    }
    else
    {
        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
        t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                    CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
        t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
        t_nfy_data.u_code.e_opcode = CECM_OPCODE_SET_SYS_AUD_MOD;
        t_nfy_data.u_data.e_sys_aud_sts = (CECM_SYS_AUD_STS_T)(pt_frame_info->aui1_operand[0]);
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_report_aud_sts
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_report_aud_sts(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_REPORT_AUD_STS_SIZE)
    {
        return;
    }
    DBG_INFO(("{CECM} >>>> Report aud sts: aud_sts = %d\n\r",pt_frame_info->aui1_operand[0]));

    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_GIVE_AUD_STS)) != NULL)
    {
        if (pt_evt->h_sync)
        {
            pt_evt->pt_sync_data->e_sync_op = CECM_SYNC_OP_SUCCESS;
            *((UINT8*)(pt_evt->pt_sync_data->pv_data)) = pt_frame_info->aui1_operand[0];
            cecm_sync_unlock(pt_evt->h_sync);
            cecm_ctrl_evt_free(pt_evt);
        }
        else
        {
            t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
            t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                        CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
            t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
            t_nfy_data.u_code.e_opcode = CECM_OPCODE_REPORT_AUD_STS;
            t_nfy_data.u_data.ui1_aud_sts = pt_frame_info->aui1_operand[0];
            t_nfy_data.e_sent_la = pt_evt->e_la;
            t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
            _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
        }
    }
    else
    {
        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
        t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                    CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
        t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
        t_nfy_data.u_code.e_opcode = CECM_OPCODE_REPORT_AUD_STS;
        t_nfy_data.u_data.ui1_aud_sts = pt_frame_info->aui1_operand[0];
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_sys_aud_mode_sts
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_sys_aud_mode_sts(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_SYS_AUD_MODE_SIZE)
    {
        return;
    }

    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_SYS_AUD_MOD_STS;
    t_nfy_data.u_data.e_sys_aud_sts = (CECM_SYS_AUD_STS_T)(pt_frame_info->aui1_operand[0]);

    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_GIVE_SYS_AUD_MOD_STS)) != NULL)
    {
        t_nfy_data.e_sent_la = pt_evt->e_la;
        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_report_audio_descriptor
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_report_audio_descriptor(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size >  CECM_OPCODE_REPORT_AUD_DESCRIPTOR_MAX_SIZE)
    {
        return;
    }

    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_REPORT_SAD;
    t_nfy_data.u_data.t_aud_descriptor.ui1_short_aud_descriptor_num = pt_frame_info->z_operand_size/3;
    x_memcpy(&t_nfy_data.u_data.t_aud_descriptor.t_aud_short_aud_descriptor, &pt_frame_info->aui1_operand[0], pt_frame_info->z_operand_size);  
    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                  CECM_OPCODE_REQUEST_SAD)) != NULL)
    {
        t_nfy_data.e_sent_la = pt_evt->e_la;
        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_init_arc
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/

static VOID _cecm_parse_init_arc(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != 0)
    {
        return;
    }

    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_INIT_ARC;
    
    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_REQ_INIT_ARC)) != NULL)
    {
        t_nfy_data.e_sent_la = pt_evt->e_la;
        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }

}
/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_terminate_arc
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_terminate_arc(CEC_FRAME_INFO_T* pt_frame_info)

{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != 0)
    {
        return;
    }

    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_TERMINATE_ARC;

    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_REQ_TERMINATE_ARC)) != NULL)
    {
        t_nfy_data.e_sent_la = pt_evt->e_la;
        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }

}

/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_timer_sts
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_timer_sts(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        !(pt_frame_info->z_operand_size == CECM_OPCODE_TIMER_STS_1_SIZE ||
        pt_frame_info->z_operand_size == CECM_OPCODE_TIMER_STS_3_SIZE))
    {
        return;
    }

    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_TIMER_STS;
    t_nfy_data.u_data.t_timer_sts.e_overlap = (CECM_TIMER_OVERLAP_T)((pt_frame_info->aui1_operand[0]) & 0x1);
    t_nfy_data.u_data.t_timer_sts.e_media_info = (CECM_TIMER_MEDIA_INFO_T)(((pt_frame_info->aui1_operand[0]) >> 1) & 0x3);
    t_nfy_data.u_data.t_timer_sts.e_prog_ind = (CECM_TIMER_PROG_IND_T)(((pt_frame_info->aui1_operand[0]) >> 3) & 0x1);

    if (t_nfy_data.u_data.t_timer_sts.e_prog_ind == CECM_TIMER_PROG_IND_PROGRAMMED)
    {
        t_nfy_data.u_data.t_timer_sts.e_prog_info = (CECM_TIMER_PROG_INFO_T)(((pt_frame_info->aui1_operand[0]) >> 4) & 0xf);
    }
    else if (t_nfy_data.u_data.t_timer_sts.e_prog_ind == CECM_TIMER_PROG_IND_NOT_PROGRAMMED)
    {
        t_nfy_data.u_data.t_timer_sts.e_not_prog_err_info = (CECM_TIMER_PROG_ERR_INFO_T)(((pt_frame_info->aui1_operand[0]) >> 4) & 0xf);
    }

    if ((t_nfy_data.u_data.t_timer_sts.e_prog_info == CECM_TIMER_PROG_INFO_NOT_ENOUGH_SPACE) ||
        (t_nfy_data.u_data.t_timer_sts.e_prog_info == CECM_TIMER_PROG_INFO_MAY_NOT_BE_ENOUGH_SPACE) ||
        (t_nfy_data.u_data.t_timer_sts.e_not_prog_err_info == CECM_TIMER_PROG_ERR_INFO_DUPLICATE))
    {
        t_nfy_data.u_data.t_timer_sts.ui2_duration = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
    }

    if (((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                  CECM_OPCODE_SET_ANA_TIMER)) != NULL) ||
        ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                  CECM_OPCODE_SET_DIG_TIMER)) != NULL) ||
        ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                  CECM_OPCODE_SET_EXT_TIMER)) != NULL))
    {
        t_nfy_data.e_sent_la = pt_evt->e_la;
        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_timer_clred_sts
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_timer_clred_sts(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_TIMER_CLRED_STS_SIZE)
    {
        return;
    }

    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_TIMER_CLRED_STS;
    t_nfy_data.u_data.e_timer_clred_sts = (CECM_TIMER_CLRED_STS_T)(pt_frame_info->aui1_operand[0]);

    if (((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                  CECM_OPCODE_CLR_ANA_TIMER)) != NULL) ||
        ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                  CECM_OPCODE_CLR_DIG_TIMER)) != NULL) ||
        ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                  CECM_OPCODE_CLR_EXT_TIMER)) != NULL))
    {
        t_nfy_data.e_sent_la = pt_evt->e_la;
        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_tuner_dev_sts
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_tuner_dev_sts(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;
    CECM_TUNER_ANA_T*   pt_tuner_ana;
    CECM_TUNER_DIG_T*   pt_tuner_dig;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        !(pt_frame_info->z_operand_size == CECM_OPCODE_TUNER_DEV_STS_ANA_SIZE ||
        pt_frame_info->z_operand_size == CECM_OPCODE_TUNER_DEV_STS_DIG_SIZE))
    {
        return;
    }

    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_TUNER_DEV_STS;

    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                  CECM_OPCODE_GIVE_TUNER_DEV_STS)) != NULL)
    {
        t_nfy_data.e_sent_la = pt_evt->e_la;
        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;

        if (pt_frame_info->z_operand_size == CECM_OPCODE_TUNER_DEV_STS_ANA_SIZE)
        {
            t_nfy_data.u_data.t_tuner_dev_info.e_svc = CECM_TUNER_SVC_ANA;
            t_nfy_data.u_data.t_tuner_dev_info.e_rec_flag = (CECM_TUNER_REC_FLAG_T)(pt_frame_info->aui1_operand[0] >> 7);
            t_nfy_data.u_data.t_tuner_dev_info.e_display_info = (CECM_TUNER_DISPLAY_INFO_T)(pt_frame_info->aui1_operand[0] & 0x7f);
            pt_tuner_ana = &(t_nfy_data.u_data.t_tuner_dev_info.u.t_ana_info);
            pt_tuner_ana->e_ana_brdcst_type = (CECM_ANA_BRDCST_T)(pt_frame_info->aui1_operand[1]);
            pt_tuner_ana->ui2_freq = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[2]);
            pt_tuner_ana->ui1_brdcst_sys = (UINT8)(pt_frame_info->aui1_operand[4]);

        }
        else if (pt_frame_info->z_operand_size == CECM_OPCODE_TUNER_DEV_STS_DIG_SIZE)
        {
            t_nfy_data.u_data.t_tuner_dev_info.e_svc = CECM_TUNER_SVC_DIG;
            t_nfy_data.u_data.t_tuner_dev_info.e_rec_flag = (CECM_TUNER_REC_FLAG_T)(pt_frame_info->aui1_operand[0] >> 7);
            t_nfy_data.u_data.t_tuner_dev_info.e_display_info = (CECM_TUNER_DISPLAY_INFO_T)(pt_frame_info->aui1_operand[0] & 0x7f);
            pt_tuner_dig = &(t_nfy_data.u_data.t_tuner_dev_info.u.t_dig_info);
            pt_tuner_dig->e_dig_id_method = (CECM_DIG_ID_METHOD_T)(pt_frame_info->aui1_operand[1] >> 7);
            pt_tuner_dig->e_dig_brdcst_sys = (CECM_DIG_BRDCST_T)(pt_frame_info->aui1_operand[1] & 0x7f);
            x_memcpy((VOID*)&pt_tuner_dig->aui1_svc_id[0],
                     (VOID*)&pt_frame_info->aui1_operand[2],
                     6);
        }
        else
        {
            return;
        }

        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_rec_sts
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_rec_sts(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_REC_STS_SIZE)
    {
        return;
    }

    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_REC_STS;
    t_nfy_data.u_data.e_rec_sts_info = (CECM_REC_STS_INFO_T)(pt_frame_info->aui1_operand[0]);

    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_REC_ON)) != NULL)
    {
        t_nfy_data.e_sent_la = pt_evt->e_la;
        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_rec_tv_screen
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_rec_tv_screen(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != 0)
    {
        return;
    }

    t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_REC_TV_SCREEN;
    t_nfy_data.u_data.e_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    _cecm_notify_client(&t_nfy_data, NULL, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_feature_abort
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_feature_abort(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_EVT_T*         pt_evt = NULL;
    CECM_NFY_T          t_nfy_data;
    CECM_DEV_REC_T*     pt_dev_rec;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_FEATURE_ABORT_SIZE)
    {
        return;
    }

    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_FEATURE_ABORT;
    t_nfy_data.u_data.e_abort_reason = (CECM_ABORT_REASON_T)(pt_frame_info->aui1_operand[1]);

    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 (CECM_OPCODE_T)pt_frame_info->aui1_operand[0])) != NULL)
    {
        if (pt_evt->e_type == CECM_EVT_TYPE_DEV_DISCOVERY)
        {
            if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_evt->e_la, FALSE)) == NULL)
            {
                return;
            }

            /* For storing feature abort reason */
            cecm_lock_wr_db();
            switch ((CECM_OPCODE_T)pt_frame_info->aui1_operand[0])
            {
                
                case CECM_OPCODE_GIVE_PHY_ADDR:
                    pt_dev_rec->e_abort_reason_pa = (CECM_ABORT_REASON_T)(pt_frame_info->aui1_operand[1]);
                    pt_dev_rec->b_post_polling_retry = FALSE;
                    break;

                case CECM_OPCODE_GIVE_DEV_VNDR_ID:
                    pt_dev_rec->e_abort_reason_dev_vndr_id = (CECM_ABORT_REASON_T)(pt_frame_info->aui1_operand[1]);
                    pt_dev_rec->b_post_polling_retry = FALSE;
                    break;
                case CECM_OPCODE_GIVE_OSD_NAME:
                    pt_dev_rec->e_abort_reason_osd_name = (CECM_ABORT_REASON_T)(pt_frame_info->aui1_operand[1]);
                    pt_dev_rec->b_post_polling_retry = FALSE;
                    break;
                case CECM_OPCODE_CEC_VER:
                    pt_dev_rec->e_abort_reason_cev_ver = (CECM_ABORT_REASON_T)(pt_frame_info->aui1_operand[1]);
                    pt_dev_rec->b_post_polling_retry = FALSE;
                    break;
                case CECM_OPCODE_GIVE_DEV_PWR_STS:
                    pt_dev_rec->e_abort_reason_pwr_sts = (CECM_ABORT_REASON_T)(pt_frame_info->aui1_operand[1]);
                    pt_dev_rec->b_post_polling_retry = FALSE;
                    break;
                case CECM_OPCODE_VNDR_CMD:   
                    if((pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)CECM_DD_VNDR_CMD_MAX_SIZE) ||
                       (pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)t_dd_config.t_dd_vndr_cmd.z_dd_vndr_cmd_size))
                    {
                        DBG_ERROR(("{CECM} ERR: _cecm_parse_feature_abort() CECM_OPCODE_VNDR_CMD failed\n\r"));
                    }
                    else
                    {
                        pt_dev_rec->ae_abort_reason_vndr_info[pt_dev_rec->ui1_vndr_cmd_idx] = (CECM_ABORT_REASON_T)(pt_frame_info->aui1_operand[1]);
                        pt_dev_rec->b_post_polling_retry = FALSE;
                        pt_dev_rec->ui1_vndr_cmd_idx++;                     
                    }                   
                    break;
                case CECM_OPCODE_VNDR_CMD_WITH_ID:   
                    if((pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)CECM_DD_VNDR_CMD_MAX_SIZE) ||
                       (pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)t_dd_config.t_dd_vndr_cmd.z_dd_vndr_cmd_size))
                    {
                        DBG_ERROR(("{CECM} ERR: _cecm_parse_feature_abort() CECM_OPCODE_VNDR_CMD_WITH_ID failed\n\r"));
                    }
                    else
                    {
                        pt_dev_rec->ae_abort_reason_vndr_info[pt_dev_rec->ui1_vndr_cmd_idx] = (CECM_ABORT_REASON_T)(pt_frame_info->aui1_operand[1]);
                        pt_dev_rec->b_post_polling_retry = FALSE;
                        pt_dev_rec->ui1_vndr_cmd_idx++;                     
                    }                 
                    break;      
                default :
                    break;
            }
            cecm_unlock_db();
            _cecm_update_dev_discovery_mask(pt_evt, pt_dev_rec);
            cecm_ctrl_evt_free(pt_evt);            
        }
        else
        {
            if (pt_evt->e_type == CECM_EVT_TYPE_PWR_STS)
            {
                if (pt_evt->e_opcode == CECM_OPCODE_GIVE_DEV_PWR_STS)
                {
#ifdef CECM_PWR_ON_DEV_EX
                    /* Send <Set Stream Path> */
                    _cecm_pwr_on_dev_send_msg(CECM_OPCODE_SET_STREAM_PATH, pt_evt, NULL);
#endif
                    /* Free the using event */
                    cecm_ctrl_evt_free(pt_evt);
                }
            }
            else
            {
                if(pt_evt->h_sync)
                {
                    pt_evt->pt_sync_data->e_sync_op = CECM_SYNC_OP_FAIL;
                    cecm_sync_unlock(pt_evt->h_sync);
                    cecm_ctrl_evt_free(pt_evt);
                }
                else
                {
                    t_nfy_data.e_sent_la = pt_evt->e_la;
                    t_nfy_data.e_sent_opcode = pt_evt->e_opcode;

                    /*if response unrequired, the event should removed already when ack*/
                    /*so do not need removed this event*/
                    if (_cecm_is_response_required(pt_evt->e_opcode))
                    {
                        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
                    }
                    else 
                    {
                        _cecm_notify_client(&t_nfy_data, pt_evt, FALSE);
                    }

                    
                }
            }
        }
    }
    else
    {
        t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
        t_nfy_data.e_sent_opcode = (CECM_OPCODE_T)pt_frame_info->aui1_operand[0];
        _cecm_notify_client(&t_nfy_data, NULL, FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_abort
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_abort(CEC_FRAME_INFO_T* pt_frame_info)
{
    INT32               i4_return;
    CEC_FRAME_INFO_T    t_frame_info;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != 0)
    {
        return;
    }

    t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
    t_frame_info.ui1_dest_addr = pt_frame_info->ui1_init_addr;
    t_frame_info.ui2_opcode = CECM_OPCODE_FEATURE_ABORT;
    t_frame_info.aui1_operand[0] = (UINT8)(pt_frame_info->ui2_opcode);
    t_frame_info.aui1_operand[1] = CECM_ABORT_REASON_REFUSED;
    t_frame_info.z_operand_size = CECM_OPCODE_FEATURE_ABORT_SIZE;

    i4_return = cecm_ctrl_send_msg(NULL_HANDLE, CECM_EVT_TYPE_NORMAL, &t_frame_info, NULL);

    if (i4_return != TRUE)
    {
        DBG_ERROR(("{CECM} ERR: _cecm_parse_abort() failed\n\r"));
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_routing_change
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_routing_change(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr != CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_ROUTING_CHG_SIZE)
    {
        return;
    }
    DBG_INFO(("{CECM} >>>> Routing Change: orig_pa =  0x%x new_pa =  0x%x \n\r", 
              GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]), GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[2])));

    t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_ROUTING_CHG;
    t_nfy_data.u_data.t_routing_chg_info.ui2_orig_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
    t_nfy_data.u_data.t_routing_chg_info.ui2_new_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[2]);
    _cecm_notify_client(&t_nfy_data, NULL, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_give_osd_name
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_give_osd_name(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != 0)
    {
        return;
    }

    t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_GIVE_OSD_NAME;
    _cecm_notify_client(&t_nfy_data, NULL, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_user_ctrl_pressed
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_user_ctrl_pressed(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != CECM_OPCODE_USER_CTRL_PRESSED_SIZE)
    {
        return;
    }

    t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_USER_CTRL_PRESSED;
    t_nfy_data.u_data.e_usr_ctrl = (CECM_USER_CTRL_T)pt_frame_info->aui1_operand[0];
    _cecm_notify_client(&t_nfy_data, NULL, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_user_ctrl_released
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_user_ctrl_released(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_NFY_T          t_nfy_data;

    /* Check frame arguments */
    if ((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST ||
        pt_frame_info->z_operand_size != 0)
    {
        return;
    }

    t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = CECM_OPCODE_USER_CTRL_RELEASED;
    _cecm_notify_client(&t_nfy_data, NULL, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_vndr_specific_cmd
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_vndr_specific_cmd(CEC_FRAME_INFO_T* pt_frame_info)
{
    CECM_NFY_T          t_nfy_data;
    CECM_EVT_T*         pt_evt = NULL;
    CECM_DEV_REC_T*     pt_dev_rec = NULL;
    
    /* Check frame arguments */
    if (((CECM_OPCODE_T)pt_frame_info->ui2_opcode == CECM_OPCODE_VNDR_CMD) &&
        (((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr == CECM_LOG_ADDR_UNREGED_BRDCST) ||
         (pt_frame_info->z_operand_size == 0) ||
         (pt_frame_info->z_operand_size > CECM_OPCODE_VNDR_SPECIFIC_CMD_MAX_SIZE)))
    {
        return;
    }

    if (((CECM_OPCODE_T)pt_frame_info->ui2_opcode == CECM_OPCODE_VNDR_CMD_WITH_ID) &&
        ((pt_frame_info->z_operand_size < (CECM_OPCODE_DEV_VNDR_ID_SIZE + 1)) ||
         (pt_frame_info->z_operand_size > CECM_OPCODE_VNDR_SPECIFIC_CMD_MAX_SIZE)))
    {
        return;
    }

    if (((CECM_OPCODE_T)pt_frame_info->ui2_opcode == CECM_OPCODE_VNDR_REMOTE_BTN_DOWN) &&
        ((pt_frame_info->z_operand_size == 0) ||
         (pt_frame_info->z_operand_size > CECM_OPCODE_VNDR_SPECIFIC_CMD_MAX_SIZE)))
    {
        return;
    }

    if (((CECM_OPCODE_T)pt_frame_info->ui2_opcode == CECM_OPCODE_VNDR_REMOTE_BTN_UP) &&
        (pt_frame_info->z_operand_size != 0))
    {
        return;
    }

    t_nfy_data.e_sent_la = (CECM_LOG_ADDR_T)pt_frame_info->ui1_init_addr;
    t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_OPCODE;
    t_nfy_data.e_brdcst_type = ((((CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr) ==
                                CECM_LOG_ADDR_UNREGED_BRDCST) ? CECM_BRDCST_TYPE_BRDCST : CECM_BRDCST_TYPE_DIRECT);
    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
    t_nfy_data.u_code.e_opcode = (CECM_OPCODE_T)pt_frame_info->ui2_opcode;

    /*Parse device discovery vendor cmd*/
    if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_VNDR_CMD)) != NULL)
    {
        if (pt_evt->e_type == CECM_EVT_TYPE_DEV_DISCOVERY)
        {
            if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_evt->e_la, FALSE)) != NULL)
             {
                CECM_DD_VNDR_CMD_INFO_T      t_vndr_cmd_info;
                CECM_DD_VNDR_CMD_PSR_INFO_T  t_vndr_cmd_psr_info;
                t_vndr_cmd_psr_info.b_save_vndr_cmd = FALSE;
                t_vndr_cmd_psr_info.ui4_update_mask = CECM_UPDATE_MASK_NONE;
                
                if(t_dd_config.pf_cecm_parse_dd_vndr_cmd !=  NULL)
                {               
                    t_vndr_cmd_info.e_vndr_cmd_type = CECM_VNDR_CMD;
                    x_memcpy((VOID*)&t_vndr_cmd_info.u_data.t_vndr_cmd_data.t_vndr_cmd,
                                         (VOID*)&pt_frame_info->aui1_operand[0],
                                         pt_frame_info->z_operand_size);                                                       
                    t_vndr_cmd_info.u_data.t_vndr_cmd_data.z_vndr_cmd_size = pt_frame_info->z_operand_size;
                    
                    /*Parse vendor cmd*/
                    (t_dd_config.pf_cecm_parse_dd_vndr_cmd)(&t_vndr_cmd_info, &t_vndr_cmd_psr_info);
                    if(t_vndr_cmd_psr_info.b_save_vndr_cmd == TRUE)
                    {
                          if((pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)CECM_DD_VNDR_CMD_MAX_SIZE) ||
                             (pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)t_dd_config.t_dd_vndr_cmd.z_dd_vndr_cmd_size))
                          {
                                DBG_ERROR(("{CECM} ERR: _cecm_parse_vndr_specific_cmd() CECM_OPCODE_VNDR_CMD failed\n\r"));
                          }
                          else
                          {
                              cecm_lock_wr_db();  
                              pt_dev_rec->t_dev.t_dev_vndr_info.at_vndr_cmd_info[pt_dev_rec->ui1_vndr_cmd_idx].e_vndr_cmd_type = CECM_VNDR_CMD;
                              x_memcpy((VOID*)&pt_dev_rec->t_dev.t_dev_vndr_info.at_vndr_cmd_info[pt_dev_rec->ui1_vndr_cmd_idx].u_data.t_vndr_cmd_data.t_vndr_cmd,
                                               (VOID*)&pt_frame_info->aui1_operand[0],
                                               pt_frame_info->z_operand_size); 
                               pt_dev_rec->t_dev.t_dev_vndr_info.at_vndr_cmd_info[pt_dev_rec->ui1_vndr_cmd_idx].u_data.t_vndr_cmd_data.z_vndr_cmd_size= pt_frame_info->z_operand_size;
                               
                               pt_dev_rec->ui4_skip_update_mask |= t_vndr_cmd_psr_info.ui4_update_mask;
                               cecm_unlock_db();                   
                          }      
                          
                    }     
                        
                }
                cecm_lock_wr_db();  
                pt_dev_rec->ui1_vndr_cmd_idx++;
                cecm_unlock_db();                
                _cecm_update_dev_discovery_mask(pt_evt, pt_dev_rec);     
                /* Free the using event */
                cecm_ctrl_evt_free(pt_evt);  
            }                       
        }

    }
    else if ((pt_evt = cecm_ctrl_get_evt_by_la_opcode((CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr),
                                                 CECM_OPCODE_VNDR_CMD_WITH_ID)) != NULL)
    {
        /*Parse device discovery vendor cmd with id*/
        if (pt_evt->e_type == CECM_EVT_TYPE_DEV_DISCOVERY)
        {
            if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_evt->e_la, FALSE)) != NULL)
             {
                CECM_DD_VNDR_CMD_INFO_T       t_vndr_cmd_info;
                CECM_DD_VNDR_CMD_PSR_INFO_T   t_vndr_cmd_psr_info;
                t_vndr_cmd_psr_info.b_save_vndr_cmd = FALSE;
                t_vndr_cmd_psr_info.ui4_update_mask = CECM_UPDATE_MASK_NONE;
                
                if(t_dd_config.pf_cecm_parse_dd_vndr_cmd !=  NULL)
                {               
                    t_vndr_cmd_info.e_vndr_cmd_type = CECM_VNDR_CMD_WITH_ID;
                    x_memcpy((VOID*)&t_vndr_cmd_info.u_data.t_vndr_cmd_with_id_data.t_dev_vndr_id,
                                         (VOID*)&pt_frame_info->aui1_operand[0],
                                         pt_frame_info->z_operand_size);                                                                                                               
                    t_vndr_cmd_info.u_data.t_vndr_cmd_with_id_data.z_vndr_cmd_with_id_size = pt_frame_info->z_operand_size -  CECM_DEV_VNDR_ID_SIZE;    
                    
                    /*Parse vendor cmd with id*/
                    (t_dd_config.pf_cecm_parse_dd_vndr_cmd)(&t_vndr_cmd_info, &t_vndr_cmd_psr_info);
                    if(t_vndr_cmd_psr_info.b_save_vndr_cmd == TRUE)
                    {
                          if((pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)CECM_DD_VNDR_CMD_MAX_SIZE) ||
                             (pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)t_dd_config.t_dd_vndr_cmd.z_dd_vndr_cmd_size))
                          {
                                DBG_ERROR(("{CECM} ERR: _cecm_parse_vndr_specific_cmd() CECM_OPCODE_VNDR_CMD_WITH_ID failed\n\r"));
                          }
                          else
                          {
                              cecm_lock_wr_db();  
                              pt_dev_rec->t_dev.t_dev_vndr_info.at_vndr_cmd_info[pt_dev_rec->ui1_vndr_cmd_idx].e_vndr_cmd_type = CECM_VNDR_CMD_WITH_ID;
                              x_memcpy((VOID*)&pt_dev_rec->t_dev.t_dev_vndr_info.at_vndr_cmd_info[pt_dev_rec->ui1_vndr_cmd_idx].u_data.t_vndr_cmd_with_id_data.t_dev_vndr_id,
                                               (VOID*)&pt_frame_info->aui1_operand[0],
                                               pt_frame_info->z_operand_size); 
                               pt_dev_rec->t_dev.t_dev_vndr_info.at_vndr_cmd_info[pt_dev_rec->ui1_vndr_cmd_idx].u_data.t_vndr_cmd_with_id_data.z_vndr_cmd_with_id_size = pt_frame_info->z_operand_size -  CECM_DEV_VNDR_ID_SIZE;
                               
                               pt_dev_rec->ui4_skip_update_mask |= t_vndr_cmd_psr_info.ui4_update_mask;
                               cecm_unlock_db();
                          }                   
                    }                            
                }
                cecm_lock_wr_db();  
                pt_dev_rec->ui1_vndr_cmd_idx++;
                cecm_unlock_db();
                _cecm_update_dev_discovery_mask(pt_evt, pt_dev_rec);               
                /* Free the using event */
                cecm_ctrl_evt_free(pt_evt);              
            }                       
        }
    }

    switch (pt_frame_info->ui2_opcode)
    {
        case CECM_OPCODE_VNDR_CMD:
            t_nfy_data.u_data.t_vndr_cmd_data.z_vndr_cmd_size = pt_frame_info->z_operand_size;
            x_memcpy((VOID*)&t_nfy_data.u_data.t_vndr_cmd_data.t_vndr_cmd,
                     (VOID*)&pt_frame_info->aui1_operand[0],
                     pt_frame_info->z_operand_size);
            break;

        case CECM_OPCODE_VNDR_CMD_WITH_ID:
            t_nfy_data.u_data.t_vndr_cmd_with_id_data.z_vndr_cmd_with_id_size = pt_frame_info->z_operand_size - CECM_DEV_VNDR_ID_SIZE;
            x_memcpy((VOID*)&t_nfy_data.u_data.t_vndr_cmd_with_id_data.t_dev_vndr_id,
                     (VOID*)&pt_frame_info->aui1_operand[0],
                     pt_frame_info->z_operand_size);
            break;

        case CECM_OPCODE_VNDR_REMOTE_BTN_DOWN:
            t_nfy_data.u_data.t_vndr_rc_code_data.z_vndr_rc_code_size = pt_frame_info->z_operand_size;
            x_memcpy((VOID*)&t_nfy_data.u_data.t_vndr_rc_code_data.t_vndr_rc_code,
                     (VOID*)&pt_frame_info->aui1_operand[0],
                     pt_frame_info->z_operand_size);
            break;

        case CECM_OPCODE_VNDR_REMOTE_BTN_UP:
            break;
        default :
            break;

    }

    _cecm_notify_client(&t_nfy_data, NULL, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_parse_handle_unsupported_opcode
 *
 * Description:
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *
 * Outputs:
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_parse_handle_unsupported_opcode(CEC_FRAME_INFO_T* pt_frame_info)
{
    INT32               i4_return;
    CEC_FRAME_INFO_T    t_frame_info;

    /* Check if response is required for received opcode */
    if (_cecm_is_response_required((CECM_OPCODE_T)pt_frame_info->ui2_opcode) == TRUE &&
        (CECM_LOG_ADDR_T)pt_frame_info->ui1_dest_addr != CECM_LOG_ADDR_UNREGED_BRDCST)
    {
        t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
        t_frame_info.ui1_dest_addr = pt_frame_info->ui1_init_addr;
        t_frame_info.ui2_opcode = CECM_OPCODE_FEATURE_ABORT;
        t_frame_info.aui1_operand[0] = (UINT8)(pt_frame_info->ui2_opcode);
        t_frame_info.aui1_operand[1] = CECM_ABORT_REASON_UNRECOGNIZED;
        t_frame_info.z_operand_size = CECM_OPCODE_FEATURE_ABORT_SIZE;

        i4_return = cecm_ctrl_send_msg(NULL_HANDLE, CECM_EVT_TYPE_NORMAL, &t_frame_info, NULL);

        if (i4_return != TRUE)
        {
            DBG_ERROR(("{CECM} ERR: _cecm_parse_handle_unsupported_opcode() failed\n\r"));
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_process_ack_msg
 *
 * Description: This API processes ACK type messages.
 *
 * Inputs: pt_ack_info      References ACK information.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_process_ack_msg(CEC_ACK_INFO_T* pt_ack_info)
{
    CECM_EVT_T*         pt_evt;
    CECM_NFY_T          t_nfy_data;
    CECM_DEV_REC_T*     pt_dev_rec;
    CEC_FRAME_INFO_T    t_frame_info;
    INT32               i4_return;
    if (pt_ack_info == NULL)
    {
        return;
    }

    /* Check if the event is valid */
    pt_evt = (CECM_EVT_T*)(pt_ack_info->pv_tag);
    if (pt_evt->e_type == CECM_EVT_TYPE_NONE)
    {
        return;
    }

    if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_evt->e_la, FALSE)) == NULL)
    {
        return;
    }

    /*set cec fct delay*/
    if(cecm_ctrl_is_set_cec_fct_delay())
    {
        cecm_ctrl_set_cec_fct_delay(pt_evt);
    }
 
 

#ifdef CECM_SKIP_INFO_ACQUISITION_WHEN_PRESENT_BEFORE
    CECM_DEV_REC_T*     pt_dev_rec_history;

    if ((pt_dev_rec_history = cecm_ctrl_get_dev_rec_by_la(pt_evt->e_la, TRUE)) == NULL)
    {
        return;
    }
#endif

    switch (pt_ack_info->e_ack_cond)
    {
        case CEC_ACK_COND_OK:
            DBG_INFO(("{CECM} ACK_OK: 0x%x (la=%d, type=%d)\n\r",
                     pt_evt->e_opcode,
                     pt_evt->e_la,
                     pt_evt->e_type));

            /* Handle power status event type */
            if (pt_evt->e_type == CECM_EVT_TYPE_PWR_STS)
            {
#ifdef CECM_PWR_ON_DEV_EX
                if (pt_evt->e_opcode == CECM_OPCODE_USER_CTRL_PRESSED)
                {
                    /* Send <User Control Released> */
                    _cecm_pwr_on_dev_send_msg(CECM_OPCODE_USER_CTRL_RELEASED, pt_evt, NULL);

                    /* Free the using event */
                    cecm_ctrl_evt_free(pt_evt);
                    break;
                }
                else if (pt_evt->e_opcode == CECM_OPCODE_USER_CTRL_RELEASED)
                {
                    if (e_usr_ctrl == CECM_USER_CTRL_POWER)
                    {
                        /* Send <User Control Pressed> - Power On Function */
                        e_usr_ctrl = CECM_USER_CTRL_PWR_ON_FUNC;
                        _cecm_pwr_on_dev_send_msg(CECM_OPCODE_USER_CTRL_PRESSED, pt_evt, (UINT32*)&e_usr_ctrl);
                    }
                    else
                    {
                        e_usr_ctrl = CECM_USER_CTRL_PWR_ON_FUNC;

                        /* Start timers for power_on_device retry */
                        x_timer_start(h_timer_pwr_on_dev_retry,
                                      (CECM_TIMEOUT_PWR_ON_DEV_RETRY * 1000),
                                      X_TIMER_FLAG_ONCE,
                                      _cecm_timer_pwr_on_dev_retry_cb,
                                      (VOID*)(cecm_ctrl_get_select_dev_info()));
                    }

                    /* Free the using event */
                    cecm_ctrl_evt_free(pt_evt);
                    break;
                }
                else if (pt_evt->e_opcode == CECM_OPCODE_GIVE_DEV_PWR_STS)
                {
                    if (!b_recv_1st_pwr_on_dev_ack)
                    {
                        t_nfy_data.e_sent_la = pt_evt->e_la;
                        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
                        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                        t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
                        t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                        t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
                        t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_1ST_GIVE_DEV_PWR_STS_ACK_OK;
                        t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = cecm_ctrl_get_pa_by_la(pt_evt->e_la);
                        _cecm_notify_client(&t_nfy_data, pt_evt, FALSE);

                        b_recv_1st_pwr_on_dev_ack = TRUE;
                    }
                }
#else
                if (pt_evt->e_opcode == CECM_OPCODE_USER_CTRL_PRESSED)
                {
                    t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
                    t_frame_info.ui1_dest_addr = (UINT8)pt_evt->e_la;
                    t_frame_info.ui2_opcode = CECM_OPCODE_GIVE_DEV_PWR_STS;
                    t_frame_info.aui1_operand[0] = 0;
                    t_frame_info.z_operand_size = 0;

                    i4_return = cecm_ctrl_send_msg(pt_evt->pt_svc->h_svc, CECM_EVT_TYPE_PWR_STS, &t_frame_info, NULL);

                    if (i4_return != TRUE)
                    {
                        t_nfy_data.e_sent_la = pt_evt->e_la;
                        t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
                        t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                        t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                        t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
                        t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_ERROR;
                        t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = cecm_ctrl_get_pa_by_la(pt_evt->e_la);
                        _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
                    }

                    /* Free the using event */
                    cecm_ctrl_evt_free(pt_evt);
                    break;
                }
#endif
            }

            if (_cecm_is_response_required(pt_evt->e_opcode)) /* Response is required */
            {
                if ((pt_evt->e_type != CECM_EVT_TYPE_DEV_DISCOVERY) &&
                    (pt_evt->e_type != CECM_EVT_TYPE_ONE_TOUCH_PLAY) &&
                    ((pt_evt->e_type != CECM_EVT_TYPE_SET_STREAM_PATH) &&
                     (pt_evt->e_opcode != CECM_OPCODE_REQ_ACT_SRC)) &&
                    (pt_evt->e_type != CECM_EVT_TYPE_PWR_STS))
                {
                    /* Notify client */
                    t_nfy_data.e_sent_la = pt_evt->e_la;
                    t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
                    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                    t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
                    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                    t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_ACK;
                    t_nfy_data.u_data.e_ack_cond = CECM_ACK_COND_OK;
                    _cecm_notify_client(&t_nfy_data, pt_evt, FALSE);
                }

                cecm_lock_wr_db();
                pt_evt->e_state = CECM_EVT_STATE_WAIT_FOR_FRAME;

                if (x_timer_start(pt_evt->h_timer,
                                  CECM_TIMEOUT_FRAME,
                                  X_TIMER_FLAG_ONCE,
                                  _cecm_timer_frame_cb,
                                  (VOID*)pt_evt) != OSR_OK)
                {
                    cecm_unlock_db();
                    return;
                }
                cecm_unlock_db();
            }
            else /* Response is not required */
            {
                /* Check if this is a device discovery polling message */
                if (pt_evt->e_type == CECM_EVT_TYPE_DEV_DISCOVERY)
                {
                    if (pt_evt->e_opcode == CECM_OPCODE_NONE)
                    {
                        cecm_lock_wr_db();
#ifdef CECM_SKIP_INFO_ACQUISITION_WHEN_PRESENT_BEFORE
                        if (!pt_dev_rec->b_post_polling_handled &&
                            !cecm_ctrl_is_post_polling_required(pt_dev_rec_history))
                        {
                            /* Skip post-polling operation */
                            pt_dev_rec->b_post_polling_handled = TRUE;
                            *pt_dev_rec = *pt_dev_rec_history;
                        }
                        else
#endif
                        {
                            /* Update device record for device present status */
                            pt_dev_rec->t_dev.b_dev_present = TRUE;
                        }
                        cecm_unlock_db();

                        _cecm_update_dev_discovery_mask(pt_evt, pt_dev_rec);
                        cecm_ctrl_evt_free(pt_evt);
                    }
                    else if((pt_evt->e_opcode == CECM_OPCODE_VNDR_CMD) || 
                            (pt_evt->e_opcode == CECM_OPCODE_VNDR_CMD_WITH_ID))
                    {
                        cecm_lock_wr_db();
                        pt_evt->e_state = CECM_EVT_STATE_WAIT_FOR_FRAME;

                        if (x_timer_start(pt_evt->h_timer,
                                          CECM_TIMEOUT_FRAME,
                                          X_TIMER_FLAG_ONCE,
                                          _cecm_timer_frame_cb,
                                          (VOID*)pt_evt) != OSR_OK)
                        {
                            cecm_unlock_db();
                            return;
                        }
                        cecm_unlock_db();
                    }
                     
                }
                else if (pt_evt->h_sync)
                {
                    if (pt_evt->e_opcode == CECM_OPCODE_NONE) /* Polling message */
                    {
                        pt_evt->pt_sync_data->e_sync_op = CECM_SYNC_OP_SUCCESS;
                        ((CECM_POLLING_INFO_T*)(pt_evt->pt_sync_data->pv_data))->b_ack_ok = TRUE;
                    }
                    else
                    {
                        pt_evt->pt_sync_data->e_sync_op = CECM_SYNC_OP_SUCCESS;
                    }
                    cecm_sync_unlock(pt_evt->h_sync);
                    cecm_ctrl_evt_free(pt_evt);
                }
                else if (pt_evt->e_type == CECM_EVT_TYPE_POLL_LA)
                {   
                    cecm_ctrl_handle_tv_la(pt_evt, TRUE);
                    
                }                
                else
                {
                    /* Notify client */
                    t_nfy_data.e_sent_la = pt_evt->e_la;
                    t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
                    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                    t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
                    t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                    t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_ACK;
                    t_nfy_data.u_data.e_ack_cond = CECM_ACK_COND_OK;
                    _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
                }
            }
            break;

        case CEC_ACK_COND_NO_RESPONSE:
            DBG_INFO(("{CECM} NO_ACK: 0x%x (la=%d, type=%d)\n\r",
                     pt_evt->e_opcode,
                     pt_evt->e_la,
                     pt_evt->e_type));

            /* Check if this is a device discovery polling message */
            if (pt_evt->e_type == CECM_EVT_TYPE_DEV_DISCOVERY)
            {
                if (pt_evt->e_opcode == CECM_OPCODE_NONE)
                {
                    if (++pt_dev_rec->ui1_polling_retry < t_dd_config.ui1_polling_retry_num)
                    {
                        /* Ping again */
                        t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
                        t_frame_info.ui1_dest_addr = (UINT8)(pt_evt->e_la);
                        t_frame_info.ui2_opcode = CECM_OPCODE_NONE;
                        t_frame_info.aui1_operand[0] = 0;
                        t_frame_info.z_operand_size = 0;

                        i4_return = cecm_ctrl_send_msg(cecm_ctrl_get_internal_handle(),
                                                       CECM_EVT_TYPE_DEV_DISCOVERY,
                                                       &t_frame_info,
                                                       NULL);
                        cecm_ctrl_evt_free(pt_evt);

                        if (i4_return == TRUE)
                        {
                            break;
                        }
                    }
                    else
                    {
                        /* Update device record for device present status */
                        cecm_lock_wr_db();
                        pt_dev_rec->t_dev.b_dev_present = FALSE;
                        cecm_unlock_db();
                    }
                }
                else
                {
                    if (pt_dev_rec->b_post_polling_retry)
                    {
                        switch (pt_evt->e_opcode)
                        {
                            case CECM_OPCODE_GIVE_PHY_ADDR:
                                pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa = CECM_INV_PA;
                                pt_dev_rec->t_dev.t_report_phy_addr.e_dev_type = CECM_DEV_TYPE_NONE;
                                pt_dev_rec->e_abort_reason_pa = CECM_ABORT_REASON_NONE;
                                pt_dev_rec->b_post_polling_retry = FALSE;
                                break;
                            case CECM_OPCODE_GET_CEC_VER:                          
                                pt_dev_rec->t_dev.e_cec_ver = CECM_CEC_VER_INVALID;
                                pt_dev_rec->e_abort_reason_cev_ver = CECM_ABORT_REASON_NONE;
                                pt_dev_rec->b_post_polling_retry = FALSE;
                                break;
                            case CECM_OPCODE_GIVE_DEV_VNDR_ID:
                                x_memset(&pt_dev_rec->t_dev.t_dev_vndr_id, 0, CECM_DEV_VNDR_ID_SIZE);
                                pt_dev_rec->e_abort_reason_dev_vndr_id = CECM_ABORT_REASON_NONE;
                                pt_dev_rec->b_post_polling_retry = FALSE;
                                break;
                            case CECM_OPCODE_VNDR_CMD:
                                if((pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)CECM_DD_VNDR_CMD_MAX_SIZE) ||
                                   (pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)t_dd_config.t_dd_vndr_cmd.z_dd_vndr_cmd_size))
                                {
                                    DBG_ERROR(("{CECM} ERR: _cecm_process_ack_msg() CECM_OPCODE_VNDR_CMD failed\n\r"));
                                }
                                else
                                {
                                    pt_dev_rec->ae_abort_reason_vndr_info[pt_dev_rec->ui1_vndr_cmd_idx] = CECM_ABORT_REASON_NONE;
                                    pt_dev_rec->b_post_polling_retry = FALSE;
                                    pt_dev_rec->ui1_vndr_cmd_idx++;                               
                                }
                                break;
                            case CECM_OPCODE_VNDR_CMD_WITH_ID:
                                if((pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)CECM_DD_VNDR_CMD_MAX_SIZE) ||
                                   (pt_dev_rec->ui1_vndr_cmd_idx >= (UINT8)t_dd_config.t_dd_vndr_cmd.z_dd_vndr_cmd_size))
                                {
                                    DBG_ERROR(("{CECM} ERR: _cecm_process_ack_msg() CECM_OPCODE_VNDR_CMD_WITH_ID failed\n\r"));
                                }
                                else
                                {
                                    pt_dev_rec->ae_abort_reason_vndr_info[pt_dev_rec->ui1_vndr_cmd_idx] = CECM_ABORT_REASON_NONE;
                                    pt_dev_rec->b_post_polling_retry = FALSE;
                                    pt_dev_rec->ui1_vndr_cmd_idx++;                               
                                }
                                break;                               
                            case CECM_OPCODE_GIVE_OSD_NAME:
                                x_memset(&(pt_dev_rec->t_dev.aw2_osd_name)[0], 0, CECM_OSD_NAME_SIZE);
                                pt_dev_rec->e_abort_reason_osd_name = CECM_ABORT_REASON_NONE;
                                pt_dev_rec->b_post_polling_retry = FALSE;
                                break;         
                             case CECM_OPCODE_GIVE_DEV_PWR_STS:
                                pt_dev_rec->t_dev.e_pwr_sts = CECM_PWR_STS_ERROR;
                                pt_dev_rec->e_abort_reason_pwr_sts = CECM_ABORT_REASON_NONE;
                                pt_dev_rec->b_post_polling_retry = FALSE;
                                break;
                             default:
                                break;
                        }                                              
                    }
                    else
                    {                                                
                        /* Retry again */
                        t_frame_info.ui1_init_addr = (UINT8)cecm_ctrl_get_tv_la();
                        t_frame_info.ui1_dest_addr = (UINT8)pt_evt->e_la;
                        t_frame_info.ui2_opcode = (UINT16)pt_evt->e_opcode;
                        t_frame_info.aui1_operand[0] = 0;
                        t_frame_info.z_operand_size = 0;
    
                        cecm_ctrl_send_msg(pt_evt->pt_svc->h_svc, CECM_EVT_TYPE_DEV_DISCOVERY, &t_frame_info, NULL);
    
                        pt_dev_rec->b_post_polling_retry = TRUE;
                        cecm_ctrl_evt_free(pt_evt);
                        break;
                    }                        
                }

                _cecm_update_dev_discovery_mask(pt_evt, pt_dev_rec);
                cecm_ctrl_evt_free(pt_evt);
            }
            else if ((pt_evt->e_type == CECM_EVT_TYPE_ONE_TOUCH_PLAY) ||
                     (pt_evt->e_type == CECM_EVT_TYPE_SET_STREAM_PATH))
            {
                _cecm_handle_otp_ssp(pt_evt);
            }
            else if (pt_evt->h_sync)
            {
                if (pt_evt->e_opcode == CECM_OPCODE_NONE) /* Polling message */
                {
                    pt_evt->pt_sync_data->e_sync_op = CECM_SYNC_OP_SUCCESS;
                    ((CECM_POLLING_INFO_T*)(pt_evt->pt_sync_data->pv_data))->b_ack_ok = FALSE;
                }
                else
                {
                    pt_evt->pt_sync_data->e_sync_op = CECM_SYNC_OP_FAIL;
                }

                cecm_sync_unlock(pt_evt->h_sync);
                cecm_ctrl_evt_free(pt_evt);
            }
            else if (pt_evt->e_type == CECM_EVT_TYPE_POLL_LA)
            {   
                cecm_ctrl_handle_tv_la(pt_evt, FALSE);
                
            }
            else
            {
#ifdef CECM_PWR_ON_DEV_EX
                if (pt_evt->e_type == CECM_EVT_TYPE_PWR_STS)
                {
                    b_recv_pwr_standby = FALSE;
                    x_timer_stop(h_timer_pwr_on_dev_retry);
                    cecm_ctrl_select_dev(FALSE, NULL);

                    t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
                    t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_1ST_GIVE_DEV_PWR_STS_NO_ACK;
                    t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = cecm_ctrl_get_pa_by_la(pt_evt->e_la);
                }
                else
                {
                    t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_ACK;
                    t_nfy_data.u_data.e_ack_cond = CECM_ACK_COND_NO_RESPONSE;
                }
#else
                if (pt_evt->e_type == CECM_EVT_TYPE_PWR_STS)
                {
                    cecm_ctrl_select_dev(FALSE, NULL);
                    t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_PWR_ON_DEV;
                    t_nfy_data.u_data.t_pwr_on_dev_sts.e_pwr_sts = CECM_PWR_STS_ERROR;
                    t_nfy_data.u_data.t_pwr_on_dev_sts.ui2_pa = cecm_ctrl_get_pa_by_la(pt_evt->e_la);
                }
                else
                {
                    t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_ACK;
                    t_nfy_data.u_data.e_ack_cond = CECM_ACK_COND_NO_RESPONSE;
                }
#endif

                /* Notify client */
                t_nfy_data.e_sent_la = pt_evt->e_la;
                t_nfy_data.e_sent_opcode = pt_evt->e_opcode;
                t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
                t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
                t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
                _cecm_notify_client(&t_nfy_data, pt_evt, TRUE);
            }
            break;
        default :
            break;

    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_process_frame_msg
 *
 * Description: This API processes frame type messages.
 *
 * Inputs:  pt_frame_info       References a CEC fram info.
 *          b_mw_cec_nfy        Indicates whether the message is issued by MW 
 *                              or driver.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/

 
static UINT64 g_last_pa_arrival = 0;

static VOID _cecm_process_frame_msg(CEC_FRAME_INFO_T* pt_frame_info,
                                    BOOL              b_mw_cec_nfy)
{
    if (pt_frame_info == NULL)
    {
        return;
    }

    DBG_INFO(("{CECM} Frame (reved): 0x%x (la=%d)\n\r",
             pt_frame_info->ui2_opcode,
             pt_frame_info->ui1_init_addr));
             
    /*set cec fct delay, to prevent hung up when cec ON->OFF, due to system audio request frame did not handle in cecm*/
    if(cecm_ctrl_is_set_cec_fct_delay())
    {
        return ;
    }

    switch (pt_frame_info->ui2_opcode)
    {
        case CECM_OPCODE_ACT_SRC:
            _cecm_parse_act_src(pt_frame_info);
            break;

        case CECM_OPCODE_INACT_SRC:
            _cecm_parse_inact_src(pt_frame_info);
            break;

        case CECM_OPCODE_REQ_ACT_SRC:
            _cecm_parse_req_act_src(pt_frame_info);
            break;

        case CECM_OPCODE_IMG_VIEW_ON:
        case CECM_OPCODE_TXT_VIEW_ON:
            _cecm_parse_img_txt_view_on(pt_frame_info);
            break;

        case CECM_OPCODE_ROUTING_INFO:
            _cecm_parse_routing_info(pt_frame_info);
            break;

        case CECM_OPCODE_STANDBY:
            _cecm_parse_standby(pt_frame_info);
            break;

        case CECM_OPCODE_GIVE_PHY_ADDR:
            _cecm_parse_give_phy_addr(pt_frame_info);
            break;

        case CECM_OPCODE_REPORT_PHY_ADDR:
        {
            UINT64 ui8_last_pa_time = 0;
            
            ui8_last_pa_time = (x_os_get_sys_tick() * x_os_get_sys_tick_period());
            
            if (ui8_last_pa_time <= (g_last_pa_arrival + CECM_DEFAULT_PA_DISCONTINUITY))
            {
                break;
            }
            
            g_last_pa_arrival = ui8_last_pa_time ;

            _cecm_parse_report_phy_addr(pt_frame_info, b_mw_cec_nfy);
            break;

        }
        case CECM_OPCODE_SET_OSD_STRING:
            _cecm_parse_set_osd_string(pt_frame_info);
            break;

        case CECM_OPCODE_SET_OSD_NAME:
            _cecm_parse_set_osd_name(pt_frame_info);
            break;

        case CECM_OPCODE_DEV_VNDR_ID:
            _cecm_parse_dev_vndr_id(pt_frame_info);
            break;

        case CECM_OPCODE_GIVE_DEV_PWR_STS:
            _cecm_parse_give_dev_pwr_sts(pt_frame_info);
            break;

        case CECM_OPCODE_REPORT_PWR_STS:
            _cecm_parse_report_pwr_sts(pt_frame_info);
            break;

        case CECM_OPCODE_GET_MENU_LANG:
            _cecm_parse_get_menu_lang(pt_frame_info);
            break;

        case CECM_OPCODE_SET_MENU_LANG:
            _cecm_parse_set_menu_lang(pt_frame_info);
            break;

        case CECM_OPCODE_GET_CEC_VER:
            _cecm_parse_get_cec_ver(pt_frame_info);
            break;

        case CECM_OPCODE_CEC_VER:
            _cecm_parse_cec_ver(pt_frame_info);
            break;

        case CECM_OPCODE_GIVE_DECK_STS:
            _cecm_parse_give_deck_sts(pt_frame_info);
            break;

        case CECM_OPCODE_DECK_STS:
            _cecm_parse_deck_sts(pt_frame_info);
            break;

        case CECM_OPCODE_MENU_STS:
            _cecm_parse_menu_sts(pt_frame_info);
            break;

        case CECM_OPCODE_GIVE_DEV_VNDR_ID:
            _cecm_parse_give_dev_vndr_id(pt_frame_info);
            break;

        case CECM_OPCODE_SET_SYS_AUD_MOD:
            _cecm_parse_set_sys_aud_mode(pt_frame_info);
            break;

        case CECM_OPCODE_REPORT_AUD_STS:
            _cecm_parse_report_aud_sts(pt_frame_info);
            break;

        case CECM_OPCODE_SYS_AUD_MOD_STS:
            _cecm_parse_sys_aud_mode_sts(pt_frame_info);
            break;

        case CECM_OPCODE_REPORT_SAD:
            _cecm_parse_report_audio_descriptor(pt_frame_info);
            break;

        case CECM_OPCODE_INIT_ARC:
            _cecm_parse_init_arc(pt_frame_info);
            break;

        case CECM_OPCODE_TERMINATE_ARC:
            _cecm_parse_terminate_arc(pt_frame_info);
            break;
            
        case CECM_OPCODE_TIMER_STS:
            _cecm_parse_timer_sts(pt_frame_info);
            break;

        case CECM_OPCODE_TIMER_CLRED_STS:
            _cecm_parse_timer_clred_sts(pt_frame_info);
            break;

        case CECM_OPCODE_TUNER_DEV_STS:
            _cecm_parse_tuner_dev_sts(pt_frame_info);
            break;

        case CECM_OPCODE_REC_STS:
            _cecm_parse_rec_sts(pt_frame_info);
            break;

        case CECM_OPCODE_REC_TV_SCREEN:
            _cecm_parse_rec_tv_screen(pt_frame_info);
            break;

        case CECM_OPCODE_FEATURE_ABORT:
            _cecm_parse_feature_abort(pt_frame_info);
            break;

        case CECM_OPCODE_ABORT:
            _cecm_parse_abort(pt_frame_info);
            break;

        case CECM_OPCODE_ROUTING_CHG:
            _cecm_parse_routing_change(pt_frame_info);
            break;

        case CECM_OPCODE_GIVE_OSD_NAME:
            _cecm_parse_give_osd_name(pt_frame_info);
            break;

        case CECM_OPCODE_USER_CTRL_PRESSED:
            _cecm_parse_user_ctrl_pressed(pt_frame_info);
            break;

        case CECM_OPCODE_USER_CTRL_RELEASED:
            _cecm_parse_user_ctrl_released(pt_frame_info);
            break;

        case CECM_OPCODE_VNDR_CMD:
        case CECM_OPCODE_VNDR_CMD_WITH_ID:
        case CECM_OPCODE_VNDR_REMOTE_BTN_DOWN:
        case CECM_OPCODE_VNDR_REMOTE_BTN_UP:
            _cecm_parse_vndr_specific_cmd(pt_frame_info);
            break;

        default:
            _cecm_parse_handle_unsupported_opcode(pt_frame_info);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _cecm_process_5v_power
 *
 * Description: This API processes 5v power.
 *
 * Inputs:  pt_5v_pwr_info       References 5v power info.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_process_5v_power(CEC_5V_POWER_INFO_T* pt_5v_pwr_info)
{
    CECM_NFY_T          t_nfy_data;
    
    
    if (pt_5v_pwr_info->ui2_comp_id > cecm_ctrl_get_hdmi_port_num())
    {
        return;
    }
    t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
    t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_5V_PWR;
    t_nfy_data.u_data.t_5v_pwr_info.b_5v_power = pt_5v_pwr_info->b_5v_power;
    t_nfy_data.u_data.t_5v_pwr_info.ui2_comp_id = pt_5v_pwr_info->ui2_comp_id;
    _cecm_notify_client(&t_nfy_data, NULL, FALSE);


#ifdef CECM_5V_POWER_DEVICE_DISCOVERY
    /*5v power triger device discovery, */ 
    cecm_ctrl_5v_power_dev_discovery();

#endif    
}
/*-----------------------------------------------------------------------------
 * Name: cecm_psr_config_dev_discovery
 *
 * Description: This API config device discovery
 *
 * Inputs:  -  pt_dev_discovery_config  Reference device discovery
 *
 * Outputs: -
 *
 * Returns: None
 ----------------------------------------------------------------------------*/

VOID cecm_psr_config_dev_discovery(CECM_DEV_DISCOVERY_CONFIG_T*   pt_dev_discovery_config)
{
    
    t_dd_config = *pt_dev_discovery_config;
    if(t_dd_config.ui1_polling_retry_num > 10 || t_dd_config.ui1_polling_retry_num <= 0)
    {
        t_dd_config.ui1_polling_retry_num = CECM_MAX_POLLING_RETRY;
    }

}
/*-----------------------------------------------------------------------------
 * Name: cecm_psr_get_dev_discovery_config
 *
 * Description: This API get device discovery config
 *
 * Inputs:  -
 *
 * Outputs: - pt_dev_discovery_config  Reference for device discovery config
 *
 * Returns: None
 ----------------------------------------------------------------------------*/

VOID cecm_psr_get_dev_discovery_config(CECM_DEV_DISCOVERY_CONFIG_T*   pt_dev_discovery_config)
{
    
    *pt_dev_discovery_config = t_dd_config;
}

/*-----------------------------------------------------------------------------
 * Name: cecm_psr_is_dev_post_polling_running
 *
 * Description: This API checks if device is performing device post-polling
 *              operation.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE        Post_polling_by_dev is running.
 *          FALSE       Post_polling_by_dev is not running.
 ----------------------------------------------------------------------------*/
BOOL cecm_psr_is_dev_post_polling_running(VOID)
{
    BOOL    b_dev_post_polling_running;

    cecm_lock_rd_db();
    b_dev_post_polling_running = ((ui1_dev_post_polling_cnt > 0) ? TRUE : FALSE);
    cecm_unlock_db();

    return (b_dev_post_polling_running);
}


/*-----------------------------------------------------------------------------
 * Name: cecm_psr_reset_dev_post_polling_ind
 *
 * Description: This API resets device post-polling operation indicator.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cecm_psr_reset_dev_post_polling_ind(VOID)
{
    cecm_lock_wr_db();
    ui1_dev_post_polling_cnt = 0;
    cecm_unlock_db();
}


#ifdef CECM_PWR_ON_DEV_EX
/*-----------------------------------------------------------------------------
 * Name: cecm_psr_pwr_on_dev
 *
 * Description: This API enables/disables power_on_device operation.
 *
 * Inputs:  b_start     Contains the control for power_on_device operation.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cecm_psr_pwr_on_dev(BOOL b_start)
{
    if ((b_pwr_on_dev = b_start) == TRUE)
    {
        /* Start a timer for power_on_device operation */
        x_timer_start(h_timer_pwr_on_dev,
                      (CECM_TIMEOUT_PWR_ON_DEV * 1000),
                      X_TIMER_FLAG_ONCE,
                      _cecm_timer_pwr_on_dev_cb,
                      (VOID*)cecm_ctrl_get_select_dev_info());
    }
    else
    {
        /* Stop the timer for power_on_device operation */
        x_timer_stop(h_timer_pwr_on_dev_retry);
        x_timer_stop(h_timer_pwr_on_dev);
        b_recv_pwr_standby = FALSE;
    }
}


/*-----------------------------------------------------------------------------
 * Name: cecm_psr_set_1st_pwr_on_dev_ack_flag
 *
 * Description: This API sets the flag of first power_on_device ACK
 *              (i.e. <Give Device Power Status> ACK).
 *              Note: It is requested that first <Give Device Power Status>
 *              ACK should notify to clients.
 *
 * Inputs:  b_ctrl      Contains the control for the first power_on_device
 *                      ACK.
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
VOID cecm_psr_set_1st_pwr_on_dev_ack_flag(BOOL b_ctrl)
{
    b_recv_1st_pwr_on_dev_ack = b_ctrl;
}
#endif


/*-----------------------------------------------------------------------------
 * Name: cecm_psr_process_message
 *
 * Description: This API processes CECM RX messages.
 *
 * Inputs:  pt_msg      References a CECM message.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cecm_psr_process_message(CECM_MSG_T* pt_msg)
{
    CECM_EVT_T*         pt_evt;
    CEC_NFY_T           t_cec_nfy;
    CECM_DEV_REC_T*     pt_dev_rec = NULL;

    if ((pt_msg == NULL) ||
        (!cecm_ctrl_is_cec_fct_enable() &&
         (pt_msg->e_msg_type != CECM_MSG_TYPE_SET_CEC_FCT)))
    {
        return;
    }

    switch (pt_msg->e_msg_type)
    {
        case CECM_MSG_TYPE_SET_CEC_FCT:
            cecm_ctrl_set_cec_fct(&(pt_msg->u.t_set_cec_fct));
            break;

        case CECM_MSG_TYPE_DEV_DISCOVERY_START:
            cecm_ctrl_start_dev_discovery(&(pt_msg->u.t_dev_discvry_info));
            break;

        case CECM_MSG_TYPE_DEV_DISCOVERY_STOP:
            cecm_ctrl_stop_dev_discovery();
            break;
        case CECM_MSG_TYPE_POLLING_LA:
            cecm_ctrl_polling_tv_la();
            break;

        case CECM_MSG_TYPE_DEV_DISCOVERY_POST_BATCH_POLLING:
            if ((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la((CECM_LOG_ADDR_T)pt_msg->u.ui1_discvry_idx, FALSE)) != NULL)
            {        
                if (pt_dev_rec->b_post_polling_handled)
                {
                    if ((pt_dev_rec->ui4_update_mask & CECM_UPDATE_MASK_ALL) == CECM_UPDATE_MASK_ALL)
                    {      
                        cecm_ctrl_poll_next_dev();
                    }                                    
                    else
                    {                        
                        b_pp_runner_dev_poll_next_dev = TRUE;
                    }                        
                }
                else
                {                    
                    DBG_INFO(("{CECM} >>>>>>>>>> post-polling (%d) <<<<<<<<<<\n\r",
                             pt_msg->u.ui1_discvry_idx));
                    /* Continue device discovery process */
                    _cecm_start_post_polling((VOID*)&(pt_msg->u.ui1_discvry_idx), FALSE);
                }
            }                
            break;

        case CECM_MSG_TYPE_CEC_NFY:
        case CECM_MSG_TYPE_CEC_NFY_MW:            
            t_cec_nfy = pt_msg->u.t_cec_nfy;

            switch(t_cec_nfy.e_nfy_type)
            {
                case CEC_NFY_TYPE_ACK:
                    /* Stop ACK timer */
                    pt_evt = (CECM_EVT_T*)(t_cec_nfy.u.t_ack_info.pv_tag);
                    x_timer_stop(pt_evt->h_timer);

                    _cecm_process_ack_msg(&(t_cec_nfy.u.t_ack_info));
                    break;

                case CEC_NFY_TYPE_FRAME:
                    _cecm_process_frame_msg(&(t_cec_nfy.u.t_frame_info), 
                                            (pt_msg->e_msg_type == CECM_MSG_TYPE_CEC_NFY_MW) ? TRUE : FALSE);
                    break;

                case CEC_NFY_TYPE_5V_POWER:
                    _cecm_process_5v_power(&(t_cec_nfy.u.t_5v_power_info));
            }
            break;

        case CECM_MSG_TYPE_ACK_TIMEOUT:
            _cecm_handle_ack_timeout(pt_msg->u.pv_tag);
            break;

        case CECM_MSG_TYPE_FRAME_TIMEOUT:
            _cecm_handle_frame_timeout(pt_msg->u.pv_tag);
            break;

        case CECM_MSG_TYPE_ONE_TOUCH_PLAY_TIMEOUT:
            _cecm_handle_one_touch_play_timeout(pt_msg->u.pv_tag);
            break;

#ifdef CECM_PWR_ON_DEV_EX
        case CECM_MSG_TYPE_SELECT_DEV:
            cecm_ctrl_select_dev(TRUE, &(pt_msg->u.t_select_dev));
            break;

        case CECM_MSG_TYPE_SELECT_DEV_TIMEOUT:
            _cecm_handle_select_dev_timeout(pt_msg->u.pv_tag);
            break;

        case CECM_MSG_TYPE_PWR_ON_DEV_TIMEOUT:
            _cecm_handle_pwr_on_dev_timeout(pt_msg->u.pv_tag);
            break;

        case CECM_MSG_TYPE_PWR_ON_DEV_RETRY_TIMEOUT:
            _cecm_handle_pwr_on_dev_retry_timeout(pt_msg->u.pv_tag);
            break;
#else
        case CECM_MSG_TYPE_SELECT_DEV:
            cecm_ctrl_select_dev(TRUE, &(pt_msg->u.t_select_dev));
            break;
        case CECM_MSG_TYPE_SELECT_DEV_TIMEOUT:
            _cecm_handle_select_dev_timeout(pt_msg->u.pv_tag);
            break;        
#endif
    default :
        break;

    }
}

/*-----------------------------------------------------------------------------
 * Name: cecm_psr_init
 *
 * Description: This API initializes the CEC Manager parser module.
 *
 * Inputs:  pv_arg      Input argument.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                Routine successful.
 *          CECMR_ERROR             Internal error.
 ----------------------------------------------------------------------------*/
INT32 cecm_psr_init(VOID* pv_arg)
{
    INT32   i4_return = CECMR_OK;

    cecm_lock_wr_db();

    /* CEC Manager message queue handle */
    ph_cecm_msgq = (HANDLE_T*)pv_arg;

    /* Create a timer for one touch play */
    if (x_timer_create(&h_timer_opt) != OSR_OK)
    {
        i4_return = CECMR_ERROR;
    }
    /*Init dev discovery config*/
    t_dd_config.t_dd_get_info.b_cec_ver = FALSE;
    t_dd_config.t_dd_get_info.b_dev_vndr_id = TRUE;
    t_dd_config.t_dd_get_info.b_osd_name = TRUE;
    t_dd_config.t_dd_get_info.b_phy_address = TRUE;
    t_dd_config.t_dd_get_info.b_pwr_sts = FALSE;
    t_dd_config.t_dd_get_info.b_dev_vndr_info = FALSE;
    t_dd_config.pf_cecm_parse_dd_vndr_cmd = NULL;  
    t_dd_config.ui1_polling_retry_num = CECM_MAX_POLLING_RETRY;
    x_memset(&(t_dd_config.t_dd_vndr_cmd), 0, sizeof(CECM_DD_VNDR_CMD_T)); 
    t_dd_config.t_dd_vndr_cmd.z_dd_vndr_cmd_size = 0;
    

   

#ifdef CECM_PWR_ON_DEV_EX
    /* For power_on_device operation */
    if (x_timer_create(&h_timer_pwr_on_dev) != OSR_OK)
    {
        i4_return = CECMR_ERROR;
    }

    if (x_timer_create(&h_timer_pwr_on_dev_retry) != OSR_OK)
    {
        i4_return = CECMR_ERROR;
    }
#endif

    cecm_unlock_db();

    return (i4_return);
}


