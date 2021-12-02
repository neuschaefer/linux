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
 * $RCSfile: av_monitor.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/26 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: 9cacb426bc6af60b7f3e5a1ad05af25e $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "dsm/x_dsm.h"
#include "inc/u_device_type.h"
#include "os/inc/x_os.h"

#include "svctx/util/av_monitor.h"
#include "svctx/svctx.h"
#include "svctx/svctx_dbg.h"
#include "handle/handle.h"
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef  _INFO_HEADER
#undef  _API_HEADER
#undef  _ERROR_HEADER
#define _INFO_HEADER        "{SVCTX:AV_MONITOR}Info: "
#define _API_HEADER         "{SVCTX:AV_MONITOR}API: "
#define _ERROR_HEADER       "{SVCTX:AV_MONITOR}Err: "


#if 0
#undef  DBG_ERROR
#undef  DBG_API
#undef  DBG_INFO
#define DBG_ERROR(_stmt)    x_dbg_stmt _stmt
#define DBG_API(_stmt)      x_dbg_stmt _stmt
#define DBG_INFO(_stmt)     x_dbg_stmt _stmt
#endif

#define _AVM_MSG_AV_GROUP_DSM_NTFY   ((UINT8)0)
#define _AVM_MSG_AV_DEVICE_DSM_NTFY  ((UINT8)1)

/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static VOID _avm_dsm_dev_nfy(
                    HANDLE_T                    h_dsm_dev,
                    DSM_COND_DEV_T              e_cond,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data_1,
                    UINT32                      ui4_data_2
                    );
/*-----------------------------------------------------------------------------
 * private variable declarations
 *---------------------------------------------------------------------------*/
static DLIST_T(_AV_MONITOR_T)   t_g_using_avm_q;
static DLIST_T(_AV_MONITOR_T)   t_g_pooled_avm_q;
static AV_MONITOR_T*            pt_g_avm_objs = NULL;
static BOOL                     b_g_init      = FALSE;
/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      _avm_valid
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static BOOL _avm_valid (
                    AV_MONITOR_T*               pt_chker
                    )
{
    AV_MONITOR_T* pt_chkee;

    if (pt_chker == NULL) {
        return FALSE;
    }

    DLIST_FOR_EACH(pt_chkee, &t_g_using_avm_q, t_dlink) {
        if (pt_chkee == pt_chker) {
            return TRUE;
        }
    }

    DBG_INFO((_INFO_HEADER"%s() - pt_chker=0x%08x not existed\r\n ", __func__, pt_chker));

    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _avm_internal_msg
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns  -
 *---------------------------------------------------------------------------*/
static INT32 _avm_internal_msg (
                    VOID*                       pv_tag,
                    SVCTX_MSG_T*                pt_msg
                    )
{
    AV_MONITOR_T* pt_this = (AV_MONITOR_T*) pv_tag;
    INT32         i4_ret  = SVCTXR_OK;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_EXIST;
    }

    if (_avm_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s(), invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_NOT_EXIST;
    }

    switch(pt_msg->ui1_msg_id) {
    case _AVM_MSG_AV_DEVICE_DSM_NTFY:
        if (pt_this->h_dsm_dev != pt_msg->u.t_generic.h_obj) {
            DBG_ERROR((_ERROR_HEADER"%s() failed! pt_this->h_dsm_dev(%d) != pt_msg->u.t_generic.h_obj(%d)\r\n",
                    __func__,
                    pt_this->h_dsm_dev,
                    pt_msg->u.t_generic.h_obj
                    ));
            return SVCTXR_OK;
        }

        switch((DSM_COND_DEV_T)pt_msg->u.t_generic.ui4_cond) {
        case DSM_COND_DEV_VIDEO_SIG_STATUS:
            if (pt_this->e_detection_mode == DETECTION_MODE_BY_SIGNAL_STATUS) {
                DBG_INFO((_INFO_HEADER"%s(DSM_COND_DEV_VIDEO_SIG_STATUS), pt_this = 0x%x\r\n", __func__, pt_this));
                pt_this->pf_nty(
                        pt_this,
                        AVM_NTFY_CODE_VIDEO_SIGNAL_STATUS,
                        pt_this->pv_tag,
                        pt_msg->u.t_generic.ui4_data1
                        );
            }
            break;
        case DSM_COND_DEV_AUDIO_SIG_STATUS:
            if (pt_this->e_detection_mode == DETECTION_MODE_BY_SIGNAL_STATUS) {
                DBG_INFO((_INFO_HEADER"%s(DSM_COND_DEV_AUDIO_SIG_STATUS), pt_this = 0x%x\r\n", __func__, pt_this));
                pt_this->pf_nty(
                        pt_this,
                        AVM_NTFY_CODE_AUDIO_SIGNAL_STATUS,
                        pt_this->pv_tag,
                        pt_msg->u.t_generic.ui4_data1
                        );
            }
            break;
#ifdef DSM_COND_DEV_CABLE_STATUS
        case DSM_COND_DEV_CABLE_STATUS:
            if (pt_this->e_detection_mode == DETECTION_MODE_BY_CABLE_STATUS) {
                DBG_INFO((_INFO_HEADER"%s(DSM_COND_DEV_CABLE_STATUS), pt_this = 0x%x\r\n", __func__, pt_this));
                pt_this->pf_nty(
                        pt_this,
                        AVM_NTFY_CODE_CABLE_STATUS,
                        pt_this->pv_tag,
                        pt_msg->u.t_generic.ui4_data1
                        );
            }
            break;
#endif
        case DSM_COND_DEV_HANDLE_CLOSED:
            DBG_INFO((_INFO_HEADER"%s(DSM_COND_DEV_HANDLE_CLOSED), pt_this = 0x%x\r\n", __func__, pt_this));
            i4_ret = x_dsm_dev_open(
                        pt_this->h_dsm_grp,
                        pt_this->e_dev_type,
                        pt_this->s_av_src_name,
                        0, /* should export this to interface QQQQQQQQ */
                        pt_this,
                        _avm_dsm_dev_nfy,
                        &pt_this->h_dsm_dev
                        );
            if (i4_ret != DSMR_OK) {
                DBG_ERROR((_ERROR_HEADER"%s(), invoke x_dsm_dev_open(%s) failed, i4_ret = %d\r\n",
                        __func__,
                        pt_this->s_av_src_name,
                        i4_ret
                        ));
                i4_ret = SVCTXR_FAIL;
                break;
            }

            /* on purpose to discard break command */
        case DSM_COND_DEV_MONITOR_STOP:
            DBG_INFO((_INFO_HEADER"%s(DSM_COND_DEV_MONITOR_STOP), pt_this = 0x%x\r\n", __func__, pt_this));
            if (pt_this->b_start_monitor == FALSE) {
                break;
            }

            i4_ret = x_dsm_dev_set(
                        pt_this->h_dsm_dev,
                        DSM_SET_AVC_START_MONITOR,
                        NULL,
                        0
                        );
            if (i4_ret < DSMR_OK) {
                DBG_ERROR((_ERROR_HEADER"%s(), invoke x_dsm_dev_set(START_MONITOR) failed, i4_ret = %d\r\n",
                        __func__,
                        i4_ret
                        ));
                i4_ret = SVCTXR_FAIL;
                break;
            }

            pt_this->b_start_monitor = TRUE;
            break;
        default:
            break;
        }
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _avm_dsm_dev_nfy
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static VOID _avm_dsm_dev_nfy(
                    HANDLE_T                    h_dsm_dev,
                    DSM_COND_DEV_T              e_cond,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data_1,
                    UINT32                      ui4_data_2
                    )
{
    AV_MONITOR_T* pt_this = (AV_MONITOR_T*) pv_tag;
    SVCTX_MSG_T   t_msg   = {0};

    DBG_API((_API_HEADER"%s(e_cond = %d, ui4_data_1=0x%x, ui4_data_2=0x%x)\r\n",
                    __func__,
                    e_cond,
                    ui4_data_1,
                    ui4_data_2
                    ));

    if (_avm_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s(), invalid pt_this = 0x%.8X\r\n", __func__, pt_this));
        return;
    }

    if( !x_handle_valid(h_dsm_dev))
    {
        DBG_ERROR((_ERROR_HEADER"%s(), invalid h_dsm_dev =0x%.8X\r\n",
                   __func__, h_dsm_dev ));
        return;
    }

    t_msg.ui1_msg_id            = _AVM_MSG_AV_DEVICE_DSM_NTFY;
    t_msg.u.t_generic.h_obj     = h_dsm_dev;
    t_msg.u.t_generic.ui4_cond  = (UINT32)e_cond;
    t_msg.u.t_generic.ui4_data1 = ui4_data_1;
    t_msg.u.t_generic.ui4_data2 = ui4_data_2;
    t_msg.pf_invoke_func        = _avm_internal_msg;
    t_msg.pv_tag                = pv_tag;

    if( pt_this->pf_nty == NULL )
    {
        DBG_ERROR((_ERROR_HEADER"%s(), error: pt_this->pf_nty == NULL!\r\n",
                   __func__, h_dsm_dev, pt_this ));

        return;
    }
    
    pt_this->pf_nty(
                    pt_this,
                    AVM_NTFY_CODE_REQ_SEND_MSG,
                    pt_this->pv_tag,
                    (UINT32)&t_msg
                    );
}
/*-----------------------------------------------------------------------------
 * Name
 *      _avm_dsm_grp_nfy
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static VOID _avm_dsm_grp_nfy(
                    HANDLE_T                    h_dsm,
                    DSM_COND_DEV_T              e_cond,
                    VOID*                       pv_tag,
                    UINT32                      ui4_data_1,
                    UINT32                      ui4_data_2
                    )
{
    /* QQQQQQQQ */
    DBG_API((_API_HEADER"%s(e_cond = %d, ui4_data_1=0x%x, ui4_data_2=0x%x)\r\n",
                    __func__,
                    e_cond,
                    ui4_data_1,
                    ui4_data_2
                    ));
}

#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name
 *      _avm_signal_status_parser
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *---------------------------------------------------------------------------*/
static INT32 _avm_signal_status_parser(
                    AV_MONITOR_T*               pt_this,
                    UINT16                      ui2_num_dev,
                    UINT16                      ui2_idx,
                    DEVICE_TYPE_T               e_dev_type,
                    AVM_SIGNAL_STATUS_T         e_video_signal_status,
                    AVM_SIGNAL_STATUS_T         e_audio_signal_status,
                    VOID*                       pv_tag
                    )
{
    x_dbg_stmt("%2d/%-2d - %-14s [%s video/%s audio]\r\n",
                    ui2_idx+1,
                    ui2_num_dev,
                    s_g_avc_comp_types[DEV_GET_DEV_ID(e_dev_type)],
                    (e_video_signal_status == AVM_SIGNAL_STATUS_WITH_SIGNAL ? "O": (e_video_signal_status == AVM_SIGNAL_STATUS_NO_SIGNAL ? "X" : "?")),
                    (e_audio_signal_status == AVM_SIGNAL_STATUS_WITH_SIGNAL ? "O": (e_audio_signal_status == AVM_SIGNAL_STATUS_NO_SIGNAL ? "X" : "?"))
                    );
    return SVCTXR_CONTINUE;
}
#endif
/*-----------------------------------------------------------------------------
 * Name
 *      _avm_iterate_devs_for_signal_status
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _avm_iterate_devs_for_signal_status (
                    AV_MONITOR_T*               pt_this,
                    UINT16                      ui2_num_dev,
                    avm_signal_status_parse_fct pf_parse_fct,
                    VOID*                       pv_tag
                    )
{
    DEVICE_TYPE_T       e_dev_type            = DEV_UNKNOWN;
    UINT32              ui4_video_signal[2]   = {0};
    UINT32              ui4_audio_signal[2]   = {0};
    SIZE_T              z_size                = 0;
    AVM_SIGNAL_STATUS_T e_video_signal_status = AVM_SIGNAL_STATUS_UNKNOWN;
    AVM_SIGNAL_STATUS_T e_audio_signal_status = AVM_SIGNAL_STATUS_UNKNOWN;
    UINT16              ui2_idx               = 0;
    INT32               i4_ret                = SVCTXR_OK;

    z_size = sizeof(ui4_video_signal);
    i4_ret = x_dsm_dev_get(
                    pt_this->h_dsm_dev,
                    DSM_GET_AVC_VIDEO_SIG_STATUS,
                    NULL,
                    &z_size,
                    ui4_video_signal
                    );
    if (i4_ret != DSMR_OK) {
        DBG_ERROR((_ERROR_HEADER"%s(), invoke x_dsm_dev_get(VIDEO_SIG_STATUS) failed, i4_ret = %d\r\n",
                    __func__,
                    i4_ret
                    ));
    }

    z_size = sizeof(ui4_audio_signal);
    i4_ret = x_dsm_dev_get(
                    pt_this->h_dsm_dev,
                    DSM_GET_AVC_AUDIO_SIG_STATUS,
                    NULL,
                    &z_size,
                    ui4_audio_signal
                    );
    if (i4_ret != DSMR_OK) {
        DBG_ERROR((_ERROR_HEADER"%s(), invoke x_dsm_dev_get(AUDIO_SIG_STATUS) failed, i4_ret = %d\r\n",
                    __func__,
                    i4_ret
                    ));
    }

    DBG_INFO((_INFO_HEADER"%s(), total %d devices - video signal status = 0x%x/0x%x, audio signal status = 0x%x/0x%x\r\n",
                    __func__,
                    ui2_num_dev,
                    ui4_video_signal[0],
                    ui4_video_signal[1],
                    ui4_audio_signal[0],
                    ui4_audio_signal[1]
                    ));

    z_size = sizeof(e_dev_type);
    for (ui2_idx = 0; ui2_idx < ui2_num_dev; ui2_idx++) {
        i4_ret = x_dsm_dev_get(
                    pt_this->h_dsm_dev,
                    DSM_GET_AVC_DEV_TYPE,
                    (VOID*)(UINT32)ui2_idx,
                    &z_size,
                    &e_dev_type
                    );
        if (i4_ret != DSMR_OK) {
            DBG_ERROR((_ERROR_HEADER"%s(), invoke x_dsm_dev_get(DEV_TYPE, idx=%d) failed, i4_ret = %d\r\n",
                    __func__,
                    ui2_idx,
                    i4_ret
                    ));
            return SVCTXR_FAIL;
        }

        if ((MAKE_BIT_MASK_32(ui2_idx) & ui4_video_signal[1]) > 0) {
            /* status is known */
            if ((MAKE_BIT_MASK_32(ui2_idx) & ui4_video_signal[0]) > 0) {
                /* with signal */
                e_video_signal_status = AVM_SIGNAL_STATUS_WITH_SIGNAL;
            } else {
                /* no signal */
                e_video_signal_status = AVM_SIGNAL_STATUS_NO_SIGNAL;
            }
        } else {
            /* status is unknown */
            e_video_signal_status = AVM_SIGNAL_STATUS_UNKNOWN;
        }

        if ((MAKE_BIT_MASK_32(ui2_idx) & ui4_audio_signal[1]) > 0) {
            /* status is known */
            if ((MAKE_BIT_MASK_32(ui2_idx) & ui4_audio_signal[0]) > 0) {
                /* with signal */
                e_audio_signal_status = AVM_SIGNAL_STATUS_WITH_SIGNAL;
            } else {
                /* no signal */
                e_audio_signal_status = AVM_SIGNAL_STATUS_NO_SIGNAL;
            }
        } else {
            /* status is unknown */
            e_audio_signal_status = AVM_SIGNAL_STATUS_UNKNOWN;
        }

        i4_ret = pf_parse_fct(
                    pt_this,
                    ui2_num_dev,
                    ui2_idx,
                    e_dev_type,
                    e_video_signal_status,
                    e_audio_signal_status,
                    pv_tag
                    );
        if (i4_ret == SVCTXR_DONE) {
            /* don't need to iterate more records */
            return i4_ret;
        }
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _avm_iterate_devs_for_cable_status
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
static INT32 _avm_iterate_devs_for_cable_status (
                    AV_MONITOR_T*               pt_this,
                    UINT16                      ui2_num_dev,
                    avm_signal_status_parse_fct pf_parse_fct,
                    VOID*                       pv_tag
                    )
{
#ifdef DSM_GET_AVC_CABLE_STATUS
    DEVICE_TYPE_T       e_dev_type            = DEV_UNKNOWN;
    UINT32              ui4_cable_status[2]   = {0};
    SIZE_T              z_size                = 0;
    AVM_SIGNAL_STATUS_T e_cable_status        = AVM_SIGNAL_STATUS_UNKNOWN;
    UINT16              ui2_idx               = 0;
    INT32               i4_ret                = SVCTXR_OK;

    z_size = sizeof(ui4_cable_status);
    i4_ret = x_dsm_dev_get(
                    pt_this->h_dsm_dev,
                    DSM_GET_AVC_CABLE_STATUS,
                    NULL,
                    &z_size,
                    ui4_cable_status
                    );
    if (i4_ret != DSMR_OK) {
        DBG_ERROR((_ERROR_HEADER"%s(), invoke x_dsm_dev_get(CABLE_STATUS) failed, i4_ret = %d\r\n",
                    __func__,
                    i4_ret
                    ));
    }

    DBG_INFO((_INFO_HEADER"%s(), total %d devices - cable status = 0x%x/0x%x\r\n",
                    __func__,
                    ui2_num_dev,
                    ui4_cable_status[0],
                    ui4_cable_status[1]
                    ));

    z_size = sizeof(e_dev_type);
    for (ui2_idx = 0; ui2_idx < ui2_num_dev; ui2_idx++) {
        i4_ret = x_dsm_dev_get(
                    pt_this->h_dsm_dev,
                    DSM_GET_AVC_DEV_TYPE,
                    (VOID*)(UINT32)ui2_idx,
                    &z_size,
                    &e_dev_type
                    );
        if (i4_ret != DSMR_OK) {
            DBG_ERROR((_ERROR_HEADER"%s(), invoke x_dsm_dev_get(DEV_TYPE, idx=%d) failed, i4_ret = %d\r\n",
                    __func__,
                    ui2_idx,
                    i4_ret
                    ));
            return SVCTXR_FAIL;
        }

        if ((MAKE_BIT_MASK_32(ui2_idx) & ui4_cable_status[1]) > 0) {
            /* status is known */
            if ((MAKE_BIT_MASK_32(ui2_idx) & ui4_cable_status[0]) > 0) {
                /* with cable plug-in */
                e_cable_status = AVM_SIGNAL_STATUS_WITH_CABLE_PLUG_IN;
            } else {
                /* no cable plug-in */
                e_cable_status = AVM_SIGNAL_STATUS_NO_CABLE_PLUG_IN;
            }
        } else {
            /* status is unknown */
            e_cable_status = AVM_SIGNAL_STATUS_UNKNOWN;
        }

        i4_ret = pf_parse_fct(
                    pt_this,
                    ui2_num_dev,
                    ui2_idx,
                    e_dev_type,
                    e_cable_status,
                    AVM_SIGNAL_STATUS_UNKNOWN,
                    pv_tag
                    );
        if (i4_ret == SVCTXR_DONE) {
            /* don't need to iterate more records */
            return i4_ret;
        }
    }
#endif
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: avm_init
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *-----------------------------------------------------------------------------*/
INT32 avm_init(UINT16                           ui2_amount)
{
    UINT16 ui2_idx;

    if (b_g_init == TRUE) {
        return SVCTXR_OK;
    }

    DBG_API((_API_HEADER"%s("
                    "ui2_amount=%d, "
                    "each size=%d)\r\n",
                    __func__,
                    ui2_amount,
                    sizeof(AV_MONITOR_T)
                    ));

   if(ui2_amount == 0){
        DBG_ERROR((_ERROR_HEADER"create memory for AV_MONITOR_T pool failed for ui2_amount = 0\r\n"));
        return SVCTXR_FAIL; 
    }
   
    pt_g_avm_objs = (AV_MONITOR_T*) x_mem_alloc(sizeof(AV_MONITOR_T) * ui2_amount);
    if(pt_g_avm_objs == NULL) {
        DBG_ERROR((_ERROR_HEADER"create memory for AV_MONITOR_T pool failed\r\n"));
        return SVCTXR_FAIL;
    }

    x_memset(pt_g_avm_objs, 0, sizeof(AV_MONITOR_T) * ui2_amount);

    DLIST_INIT(&t_g_using_avm_q);
    DLIST_INIT(&t_g_pooled_avm_q);

    for (ui2_idx = 0; ui2_idx < ui2_amount; ui2_idx++) {
        AV_MONITOR_T* pt_avm = &(pt_g_avm_objs[ui2_idx]);

        DLIST_INSERT_HEAD(pt_avm, &t_g_pooled_avm_q, t_dlink);
    }

    b_g_init = TRUE;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: avm_deinit
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *-----------------------------------------------------------------------------*/
VOID avm_deinit(VOID)
{
    if (b_g_init == FALSE) {
        return;
    }

    x_mem_free(pt_g_avm_objs);

    DLIST_INIT(&t_g_using_avm_q);
    DLIST_INIT(&t_g_pooled_avm_q);

    pt_g_avm_objs = NULL;
    b_g_init      = FALSE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      avm_open
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 avm_open(const CHAR*                      ps_av_src_name,
                    const CHAR*                 ps_sink_name,
                    DEVICE_TYPE_T               e_dev_type,
                    avm_nty_fct                 pf_nty,
                    VOID*                       pv_tag,
                    AV_MONITOR_T**              ppt_this
                    )
{
    AV_MONITOR_T* pt_this;
    INT32         i4_ret;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (x_strlen(ps_av_src_name) == 0
            || pf_nty == NULL
            || ppt_this == NULL
            || DEV_GET_GROUP_ID(e_dev_type) != DEV_AVC) {
        return SVCTXR_INV_ARG;
    }

    do {
        /* check the pool is empty or not */
        pt_this = DLIST_HEAD(&t_g_pooled_avm_q);
        if (pt_this == NULL) {
            DBG_ERROR((_ERROR_HEADER"%s() failed! ret=SVCTXR_NO_RESOURCE\r\n", __func__));
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }

        /* move the instance from pooled to used */
        DLIST_REMOVE(pt_this, &t_g_pooled_avm_q, t_dlink);
        DLIST_INSERT_HEAD(pt_this, &t_g_using_avm_q, t_dlink);

        pt_this->e_dev_type       = e_dev_type;
        pt_this->e_detection_mode = DETECTION_MODE_BY_SIGNAL_STATUS;
        pt_this->pf_nty           = pf_nty;
        pt_this->pv_tag           = pv_tag;
        pt_this->b_start_monitor  = FALSE;

        i4_ret = x_dsm_open(
                    DEV_GET_GROUP_ID(pt_this->e_dev_type),
                    ps_sink_name,
                    pt_this,
                    _avm_dsm_grp_nfy,
                    &pt_this->h_dsm_grp
                    );
        if (i4_ret != DSMR_OK) {
            DBG_ERROR((_ERROR_HEADER"%s(), invoke x_dsm_open() failed, i4_ret = %d\r\n",
                    __func__,
                    i4_ret
                    ));
            i4_ret = SVCTXR_FAIL;
            break;
        }

        x_strcpy(pt_this->s_av_src_name, ps_av_src_name);

        i4_ret = x_dsm_dev_open(
                    pt_this->h_dsm_grp,
                    pt_this->e_dev_type,
                    pt_this->s_av_src_name,
                    0, /* should export this to interface QQQQQQQQ */
                    pt_this,
                    _avm_dsm_dev_nfy,
                    &pt_this->h_dsm_dev
                    );
        if (i4_ret != DSMR_OK) {
            DBG_ERROR((_ERROR_HEADER"%s(), invoke x_dsm_dev_open(%s) failed, i4_ret = %d\r\n",
                    __func__,
                    pt_this->s_av_src_name,
                    i4_ret
                    ));
            i4_ret = SVCTXR_FAIL;
            break;
        }

        DBG_INFO((_INFO_HEADER"%s(), avm = 0x%x (%s) is opened!\r\n",
                    __func__,
                    pt_this,
                    pt_this->s_av_src_name
                    ));

        *ppt_this = pt_this;
        return SVCTXR_OK;
    } while (0);

    DBG_ERROR((_ERROR_HEADER"----------------%s() failed!\r\n\r\n", __func__));

    /* error handling */
    if (pt_this != NULL) {
        DBG_ERROR((_ERROR_HEADER"%s() failed, return pt_this = 0x%x to pool\r\n",
                        __func__,
                        pt_this
                        ));

        i4_ret = avm_close(pt_this);
        if (i4_ret != SVCTXR_OK) {
            DBG_ERROR((_ERROR_HEADER"%s(), avm_close() failed! ret=%d\r\n", __func__, i4_ret));
        }
    }

    *ppt_this = NULL;
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      avm_close
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 avm_close(AV_MONITOR_T*                   pt_this)
{
    INT32 i4_ret;

    DBG_API((_API_HEADER"%s("
                    "pt_this = 0x%x "
                    ")\r\n",
                    __func__,
                    pt_this
                    ));

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_avm_valid(pt_this) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid avm = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_OK;
    }

    i4_ret = x_handle_free(pt_this->h_dsm_dev);
    if (i4_ret != HR_OK && i4_ret != HR_INV_HANDLE) {
        DBG_ERROR((_ERROR_HEADER"%s(), invoke x_handle_free(h_dsm_dev=%d) failed! ret=%d\r\n",
                    __func__,
                    pt_this->h_dsm_dev,
                    i4_ret
                    ));
    }

    i4_ret = x_handle_free(pt_this->h_dsm_grp);
    if (i4_ret != HR_OK && i4_ret != HR_INV_HANDLE) {
        DBG_ERROR((_ERROR_HEADER"%s(), invoke x_handle_free(h_dsm_grp=%d) failed! ret=%d\r\n",
                    __func__,
                    pt_this->h_dsm_grp,
                    i4_ret
                    ));
    }

    DBG_INFO((_INFO_HEADER"%s(), avm = 0x%x (%s) is closed!\r\n",
                    __func__,
                    pt_this,
                    pt_this->s_av_src_name
                    ));

    pt_this->h_dsm_grp        = NULL_HANDLE;
    pt_this->h_dsm_dev        = NULL_HANDLE;
    pt_this->e_dev_type       = DEV_UNKNOWN;
    pt_this->b_start_monitor  = FALSE;
    pt_this->pf_nty           = NULL;
    pt_this->pv_tag           = NULL;
    pt_this->s_av_src_name[0] = '\0';

    DLIST_REMOVE(pt_this, &t_g_using_avm_q, t_dlink);
    DLIST_INSERT_HEAD(pt_this, &t_g_pooled_avm_q, t_dlink);

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      avm_stop
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 avm_stop(AV_MONITOR_T*                    pt_this)
{
    INT32 i4_ret;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_avm_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    i4_ret = x_dsm_dev_set(
                    pt_this->h_dsm_dev,
                    DSM_SET_AVC_STOP_MONITOR,
                    NULL,
                    0
                    );
    if (i4_ret < DSMR_OK) {
        DBG_ERROR((_ERROR_HEADER"%s(), invoke x_dsm_dev_set(STOP_MONITOR) failed, i4_ret = %d\r\n",
                    __func__,
                    i4_ret
                    ));
        return SVCTXR_FAIL;
    }

    DBG_INFO((_INFO_HEADER"%s(), avm = 0x%x (%s) is stopped!\r\n",
                    __func__,
                    pt_this,
                    pt_this->s_av_src_name
                    ));

    pt_this->b_start_monitor = FALSE;

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      avm_start
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 avm_start(AV_MONITOR_T*               pt_this)
{
    INT32 i4_ret;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_avm_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    i4_ret = x_dsm_dev_set(
                    pt_this->h_dsm_dev,
                    DSM_SET_AVC_START_MONITOR,
                    NULL,
                    0
                    );
    if (i4_ret < DSMR_OK) {
        DBG_ERROR((_ERROR_HEADER"%s(), invoke x_dsm_dev_set(START_MONITOR) failed, i4_ret = %d\r\n",
                    __func__,
                    i4_ret
                    ));
        return SVCTXR_FAIL;
    }

    DBG_INFO((_INFO_HEADER"%s(), avm = 0x%x (%s) is started!\r\n",
                    __func__,
                    pt_this,
                    pt_this->s_av_src_name
                    ));

    pt_this->b_start_monitor = TRUE;
    
    if (pt_this->e_detection_mode == DETECTION_MODE_BY_CABLE_STATUS) {
        avm_notify_cable_status(pt_this);
    }
    
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      avm_set_detection_mode
 * Description
 *      The API is used to set detection mode of AVM - by signal status, or by
 *      cable plug-in status. The set operation will not take effect until 
 *      next status change.
 * Input arguments
 *      pt_this             The avm instance
 *      e_detection_mode    The detection mode
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK
 *      SVCTXR_NOT_INIT
 *      SVCTXR_INV_ARG
 *-----------------------------------------------------------------------------*/
INT32 avm_set_detection_mode(
                    AV_MONITOR_T*               pt_this,
                    DETECTION_MODE_T            e_detection_mode
                    )
{
    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_avm_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    pt_this->e_detection_mode = e_detection_mode;
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      avm_notify_video_status
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 avm_notify_video_status(
                    AV_MONITOR_T*               pt_this
                    )
{
    UINT32 ui4_signal_status[2];
    SIZE_T z_size = sizeof(UINT32) * 2;
    INT32  i4_ret;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_avm_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    i4_ret = x_dsm_dev_get(
                    pt_this->h_dsm_dev,
                    DSM_GET_AVC_VIDEO_SIG_STATUS,
                    NULL,
                    &z_size,
                    ui4_signal_status
                    );
    if (i4_ret < DSMR_OK) {
        DBG_ERROR((_ERROR_HEADER"%s(), invoke x_dsm_dev_set(START_MONITOR) failed, i4_ret = %d\r\n",
                    __func__,
                    i4_ret
                    ));
        return SVCTXR_FAIL;
    }

    _avm_dsm_dev_nfy(
                    pt_this->h_dsm_dev,
                    DSM_COND_DEV_VIDEO_SIG_STATUS,
                    pt_this,
                    ui4_signal_status[0],
                    ui4_signal_status[1]
                    );
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      avm_notify_cable_status
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 avm_notify_cable_status(
                    AV_MONITOR_T*               pt_this
                    )
{
#ifdef DSM_GET_AVC_CABLE_STATUS
    UINT32 ui4_cable_status[2];
    SIZE_T z_size = sizeof(UINT32) * 2;
    INT32  i4_ret;

    if (b_g_init == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (_avm_valid(pt_this) == FALSE) {
        DBG_ERROR((_ERROR_HEADER"%s() invalid pt_this = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    i4_ret = x_dsm_dev_get(
                    pt_this->h_dsm_dev,
                    DSM_GET_AVC_CABLE_STATUS,
                    NULL,
                    &z_size,
                    ui4_cable_status
                    );
    if (i4_ret < DSMR_OK) {
        DBG_ERROR((_ERROR_HEADER"%s(), invoke x_dsm_dev_set(START_MONITOR) failed, i4_ret = %d\r\n",
                    __func__,
                    i4_ret
                    ));
        _avm_dsm_dev_nfy(
                    pt_this->h_dsm_dev,
                    DSM_COND_DEV_CABLE_STATUS,
                    pt_this,
                    0,
                    0
                    );
    } else {
        _avm_dsm_dev_nfy(
                    pt_this->h_dsm_dev,
                    DSM_COND_DEV_CABLE_STATUS,
                    pt_this,
                    ui4_cable_status[0],
                    ui4_cable_status[1]
                    );
    }
#endif
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      avm_iterate_devs
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      SVCTXR_OK          the routine is successfully
 *-----------------------------------------------------------------------------*/
INT32 avm_iterate_devs (
                    AV_MONITOR_T*               pt_this,
                    avm_signal_status_parse_fct pf_parse_fct,
                    VOID*                       pv_tag
                    )
{
    UINT16 ui2_num_dev = 0;
    SIZE_T z_size      = 0;
    INT32  i4_ret      = SVCTXR_OK;

    if (_avm_valid(pt_this) == FALSE) {
        DBG_INFO((_INFO_HEADER"%s(), invalid avm = 0x%x\r\n", __func__, pt_this));
        return SVCTXR_INV_ARG;
    }

    if (pf_parse_fct == NULL) {
        return SVCTXR_INV_ARG;
    }

    z_size = sizeof(ui2_num_dev);
    i4_ret = x_dsm_dev_get(
                    pt_this->h_dsm_dev,
                    DSM_GET_AVC_NUM_OF_DEV,
                    NULL,
                    &z_size,
                    &ui2_num_dev
                    );
    if (i4_ret != DSMR_OK) {
        DBG_ERROR((_ERROR_HEADER"%s(), invoke x_dsm_dev_get(NUM_OF_DEV) failed, i4_ret = %d\r\n",
                    __func__,
                    i4_ret
                    ));
        return SVCTXR_FAIL;
    }

    if (ui2_num_dev == 0) {
        return SVCTXR_NO_RECORD;
    }
    
    i4_ret = SVCTXR_OK;
    
    switch (pt_this->e_detection_mode) {
    case DETECTION_MODE_BY_SIGNAL_STATUS:
        i4_ret = _avm_iterate_devs_for_signal_status (
                    pt_this,
                    ui2_num_dev,
                    pf_parse_fct,
                    pv_tag
                    );
        break;
    case DETECTION_MODE_BY_CABLE_STATUS:
        i4_ret = _avm_iterate_devs_for_cable_status (
                    pt_this,
                    ui2_num_dev,
                    pf_parse_fct,
                    pv_tag
                    );
        break;
    default:
        i4_ret = SVCTXR_NOT_SUPPORT;
        break;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      avm_get_using_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
AV_MONITOR_T* avm_get_using_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_using_avm_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      avm_get_pooled_head
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
AV_MONITOR_T* avm_get_pooled_head(
                    VOID
                    )
{
    return DLIST_HEAD(&t_g_pooled_avm_q);
}
/*-----------------------------------------------------------------------------
 * Name
 *      avm_next
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
AV_MONITOR_T* avm_next(
                    AV_MONITOR_T*             pt_this
                    )
{
    if (b_g_init == FALSE || pt_this == NULL) {
        return NULL;
    }

    return (DLIST_NEXT(pt_this, t_dlink));
}
/*-----------------------------------------------------------------------------
 * Name
 *      avm_print_self
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      HANDLE_T
 *-----------------------------------------------------------------------------*/
VOID avm_print_self(
                    AV_MONITOR_T*             pt_this
                    )
{
#ifdef CLI_LVL_ALL
    x_dbg_stmt("------------------------------------ size = %d\r\n", sizeof(AV_MONITOR_T));
    x_dbg_stmt("pt_avm              - 0x%x\r\n", pt_this);
    if (pt_this != NULL) {
        x_dbg_stmt("h_dsm_grp           - %d\r\n", pt_this->h_dsm_grp);
        x_dbg_stmt("h_dsm_dev           - %d\r\n", pt_this->h_dsm_dev);
        x_dbg_stmt("e_dev_type          - %s\r\n", s_g_avc_comp_types[DEV_GET_DEV_ID(pt_this->e_dev_type)]);
        x_dbg_stmt("b_start_monitor     - %s\r\n", (pt_this->b_start_monitor == TRUE ? "Y" : "N"));
        x_dbg_stmt("e_detection_mode    - %s\r\n", (pt_this->e_detection_mode == DETECTION_MODE_BY_SIGNAL_STATUS ? "Signal" : "Cable"));           
        x_dbg_stmt("pf_nty              - 0x%x\r\n",  pt_this->pf_nty);
        x_dbg_stmt("pv_tag              - 0x%x\r\n",  pt_this->pv_tag);
        x_dbg_stmt("s_av_src_name       - %s\r\n",    pt_this->s_av_src_name);
        avm_iterate_devs(pt_this, _avm_signal_status_parser, NULL);
    }
    x_dbg_stmt("====================================\r\n");
#endif
}
