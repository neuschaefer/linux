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
 * $RCSfile: cecm_ctrl.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/14 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: a872122d22ab4b982b54b29e95d0ff55 $
 *
 * Description:
 *         This is CEC Manager control source file.
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
#include "cec_mngr/x_cecm.h"
#include "cec_mngr/cecm.h"
#include "cec_mngr/cecm_lock.h"
#include "cec_mngr/cecm_psr.h"
#include "cec_mngr/cecm_ctrl.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define CECM_TIMOUT_ACK_FOR_DEV_DISCOVERY    ((UINT32) 5000) /* ms */
#define CECM_TIMEOUT_ACK                ((UINT32) 2000) /* ms */
#define CECM_MAX_SVC                    ((UINT8)     8)
#define CECM_MAX_EVT                    ((UINT8)   128)
#define CECM_SVC_RESERVED               ((UINT8)     0)
#define CECM_GET_INFO_MAX_RETRY_TIME    ((UINT32) 1000)

#ifdef CECM_PWR_ON_DEV_EX
#define CECM_TIMEOUT_SELECT_DEV         ((UINT32)  120) /* sec */
#else
#define CECM_TIMEOUT_SELECT_DEV         ((UINT32)  80) /* sec */
#endif

#define LOOP_FOR_EACH(var, start, stop, ascending)          \
    for ((var) = start;                                     \
         ascending ? ((var) <= stop) : ((var) >= stop);     \
         ascending ? (var)++ : (var)--)

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static HANDLE_T*                ph_cecm_msgq = NULL_HANDLE;
static HANDLE_T                 h_cecd = NULL_HANDLE;
static HANDLE_T                 h_svc_internal = NULL_HANDLE;
static HANDLE_T                 h_timer_dev_discovery = NULL_HANDLE;
static HANDLE_T                 h_timer_polling_tv_la  = NULL_HANDLE;
static BOOL                     b_cec_fct_enable = TRUE; /* For CECM_MSG_TYPE_SET_CEC_FCT be able to run initially */
static BOOL                     b_uP_ctrl_CEC = FALSE;
static BOOL                     b_dev_discovery_running = FALSE;
static BOOL                     b_batch_polling_done = FALSE;
static CECM_DD_INFO_T           t_dd_info;
static CECM_DD_INFO_T           t_dd_info_client;
static CECM_DEV_REC_T           at_dev_lst[CECM_LOG_ADDR_MAX];
static CECM_DEV_REC_T           at_dev_lst_history[CECM_LOG_ADDR_MAX];
static CECM_SVC_T               at_svc_lst[CECM_MAX_SVC];
static DLIST_T(_CECM_EVT_T)     t_free_evt_pool;
static UINT8                    ui1_hdmi_port_num = 0;
static UINT8                    b_dev_lst_nfy_history = FALSE;
static CECM_LOG_ADDR_T          e_tv_la = CECM_LOG_ADDR_TV;
static CECM_CEC_VER_T           e_support_cec_ver = CECM_CEC_VER_1_3A;
static UINT32                   ui4_polling_tv_la_itvl = CECM_PING_TV_LA_ITVL;
#ifdef CECM_SKIP_INFO_ACQUISITION_WHEN_PRESENT_BEFORE
static BOOL                     b_post_polling_required=FALSE;
#endif

static CECM_LOG_ADDR_T          e_pwr_on_dev_la = CECM_LOG_ADDR_MAX;

static BOOL                     b_set_cec_fct_delay = FALSE;
static CECM_SET_CEC_FCT_T       t_set_cec_fct;


/* For select_device */
static BOOL                     b_select_dev = FALSE;
static HANDLE_T                 h_timer_select_dev = NULL_HANDLE;
static CECM_SELECT_DEV_INFO_T   t_select_dev_info;


/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
static VOID _cecm_rm_nfy_fct(HANDLE_T  h_obj,
                             UINT8     ui1_port,
                             RM_COND_T e_nfy_cond,
                             VOID*     pv_tag,
                             UINT32    ui4_data);

static BOOL _cecm_handle_free_nfy_fct(HANDLE_T      h_handle,
                                      HANDLE_TYPE_T e_dev_type,
                                      VOID*         pv_obj,
                                      VOID*         pv_tag,
                                      BOOL          b_req_handle);

static INT32 _cecm_open_cec_drv(x_cec_nfy_fct pf_cec_nfy);

static VOID _cecm_timer_ack_cb(HANDLE_T h_timer,
                               VOID*    pv_tag);

static VOID _cecm_timer_dev_discovery_start_cb(HANDLE_T h_timer,
                                               VOID*    pv_tag);

#ifdef CECM_PWR_ON_DEV_EX
static VOID _cecm_timer_select_dev_cb(HANDLE_T h_timer,
                                      VOID*    pv_tag);
#endif

/*-----------------------------------------------------------------------------
 * Name: _cecm_rm_nfy_fct
 *
 * Description: This notify function is called by the Resource Manager.
 *
 * Inputs:
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_rm_nfy_fct(HANDLE_T  h_obj,
                             UINT8     ui1_port,
                             RM_COND_T e_nfy_cond,
                             VOID*     pv_tag,
                             UINT32    ui4_data)
{
    /* To be implemented */
}

/*-----------------------------------------------------------------------------
 * Name: _cecm_handle_free_nfy_fct
 *
 * Description: This is handle free notify function.
 *
 * Inputs:  h_handle        A handle referencing the event filter.
 *          e_dev_type      Contains the handle type.
 *          pv_obj          References the data object.
 *          pv_tag          Contains a private tag.
 *          b_req_handle    If this argument is set to TRUE then the free
 *                          operation is specifically requested on this handle.
 *                          If this argument is set to FALSE, the handle is
 *                          being freed but this operation occurred due some
 *                          other effect.
 *
 * Outputs: -
 *
 * Returns: TRUE        Successful.
 *          FALSE       Failed.
 ----------------------------------------------------------------------------*/
static BOOL _cecm_handle_free_nfy_fct(HANDLE_T      h_handle,
                                      HANDLE_TYPE_T e_dev_type,
                                      VOID*         pv_obj,
                                      VOID*         pv_tag,
                                      BOOL          b_req_handle)
{
    CECM_EVT_T*     pt_evt;

    if (!b_req_handle)
    {
        /* Remove all events in the list */
        while ((pt_evt = DLIST_HEAD(&(((CECM_SVC_T*)pv_obj)->t_evt_lst))) != NULL)
        {
            cecm_ctrl_evt_free(pt_evt);
        }

        /* Update service record */
        cecm_lock_wr_db();
        ((CECM_SVC_T*)pv_obj)->h_svc = NULL_HANDLE;
        ((CECM_SVC_T*)pv_obj)->pf_nfy = NULL;
        ((CECM_SVC_T*)pv_obj)->pv_tag = NULL;
        cecm_unlock_db();

        return TRUE;
    }
    else
    {
        /* Free is not allowed */
        return FALSE;
    }
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_open_cec_drv
 *
 * Description: This API opens CEC driver for future access.
 *
 * Inputs:  pf_cec_nfy      References a CEC notify function.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_open_cec_drv(x_cec_nfy_fct pf_cec_nfy)
{
    INT32               i4_return;
    RM_COND_T           e_cond;
    CEC_NFY_INFO_T      t_cec_nfy_info;

    /* Open CEC driver */
    i4_return = rm_open_comp(NULL_HANDLE,
                             OBJ_COMP_REL_IGNORE,
                             DRVT_HDMI_CEC,
                             ANY_COMP_ID,
                             NULL,
                             CTRL_TYPE_SINGLE,
                             NULL,
                             NULL,
                             NULL,
                             &_cecm_rm_nfy_fct,
                             &h_cecd,
                             &e_cond);

    if (i4_return != RMR_OK)
    {
        return CECMR_DRV_COMP_ERROR;
    }

    /* Set notify function to CEC driver */
    t_cec_nfy_info.pv_nfy_tag = NULL;
    t_cec_nfy_info.pf_cec_nfy = pf_cec_nfy;

    i4_return = rm_set(h_cecd,
                       FALSE,
                       ANY_PORT_NUM,
                       CEC_SET_TYPE_NFY_FCT,
                       (VOID*)&t_cec_nfy_info,
                       sizeof(CEC_NFY_INFO_T),
                       NULL);

    if (i4_return != RMR_OK)
    {
        return CECMR_DRV_COMP_ERROR;
    }

    return CECMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_timer_ack_cb
 *
 * Description: This callback function is called when ACK timer is expired.
 *
 * Inputs:  h_handle        A handle referencing the timer.
 *          pv_tag          Contains a private tag.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_timer_ack_cb(HANDLE_T h_timer,
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

    t_msg.e_msg_type = CECM_MSG_TYPE_ACK_TIMEOUT;
    t_msg.u.pv_tag = pv_tag;

    x_msg_q_send(*ph_cecm_msgq,
                 (VOID*)&t_msg,
                 sizeof(CECM_MSG_T),
                 CECM_MSGQ_DEFAULT_PRIORITY);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_timer_dev_discovery_start_cb
 *
 * Description: This callback function is called when device discovery timer
 *              is expired.
 *
 * Inputs:  h_handle        A handle referencing the timer.
 *          pv_tag          Contains a private tag.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_timer_dev_discovery_start_cb(HANDLE_T h_timer,
                                               VOID*    pv_tag)
{
    CECM_MSG_T      t_msg;

    t_msg.e_msg_type = CECM_MSG_TYPE_DEV_DISCOVERY_START;
    t_msg.u.t_dev_discvry_info = t_dd_info_client.t_discvry_info;

    x_msg_q_send(*ph_cecm_msgq,
                 (VOID*)&t_msg,
                 sizeof(CECM_MSG_T),
                 CECM_MSGQ_DEFAULT_PRIORITY);
}

/*-----------------------------------------------------------------------------
 * Name: _cecm_timer_select_dev_cb
 *
 * Description: This callback function is called when select_device timer is
 *              expired.
 *
 * Inputs:  h_timer     A handle referencing the timer.
 *          pv_tag      References a private tag.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_timer_select_dev_cb(HANDLE_T h_timer,
                                      VOID*    pv_tag)
{
    CECM_MSG_T      t_msg;

    if (b_select_dev == FALSE)
    {
        return;
    }

    t_msg.e_msg_type = CECM_MSG_TYPE_SELECT_DEV_TIMEOUT;
    t_msg.u.pv_tag = pv_tag;

    x_msg_q_send(*ph_cecm_msgq,
                 (VOID*)&t_msg,
                 sizeof(CECM_MSG_T),
                 CECM_MSGQ_DEFAULT_PRIORITY);
}
/*-----------------------------------------------------------------------------
 * Name: _cecm_timer_polling_la_cb
 *
 * Description: This callback function is called when polling la timer
 *              is expired.
 *
 * Inputs:  h_handle        A handle referencing the timer.
 *          pv_tag          Contains a private tag.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_timer_polling_la_cb(HANDLE_T h_timer,
                                           VOID*    pv_tag)
{
    CECM_MSG_T      t_msg;

    t_msg.e_msg_type = CECM_MSG_TYPE_POLLING_LA;
    
    x_msg_q_send(*ph_cecm_msgq,
                 (VOID*)&t_msg,
                 sizeof(CECM_MSG_T),
                 CECM_MSGQ_DEFAULT_PRIORITY);
}

/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_set_tv_la
 *
 * Description: This API set the current TV la.
 *
 * Inputs:  e_la      Current TV la.
 *
 * Outputs: -
 *
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_set_tv_la(CECM_LOG_ADDR_T e_la)
{
    INT32 i4_return;
    
    if(e_la >= CECM_LOG_ADDR_MAX)
    {
        return ;
    }
    i4_return = rm_set(cecm_ctrl_get_cec_drv_handle(),
                       FALSE,
                       ANY_PORT_NUM,
                       CEC_SET_TYPE_LOG_ADDR,
                       (VOID*)&e_la,
                       sizeof(CEC_LOG_ADDR_T),
                       NULL);
    
    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{CECM} ERR: cecm_ctrl_set_tv_la()\n\r"));
    }  
    e_tv_la = e_la;
}
/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_get_tv_la
 *
 * Description: This API get current TV la
 *
 * Inputs:  e_la      Current TV la.
 *
 * Outputs: -
 *
 ----------------------------------------------------------------------------*/

CECM_LOG_ADDR_T cecm_ctrl_get_tv_la(VOID)
{
    return e_tv_la;
}


VOID cecm_ctrl_set_support_cec_ver(CECM_CEC_VER_T   e_cec_ver)
{
    e_support_cec_ver = e_cec_ver;
}

CECM_CEC_VER_T cecm_ctrl_get_support_cec_ver(VOID)
{
    return e_support_cec_ver;
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_set_polling_tv_la_itvl
 *
 * Description: This API set polling tv la itvl
 *
 * Inputs:  e_la      Current TV la.
 *
 * Outputs: -
 *
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_set_polling_tv_la_itvl(UINT32 ui4_itvl)
{
    ui4_polling_tv_la_itvl = ui4_itvl;
}
/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_polling_tv_la
 *
 * Description: This API used for polling LA0
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 ----------------------------------------------------------------------------*/

VOID cecm_ctrl_polling_tv_la(VOID)
{
    CECM_SET_T              t_set_info;
    CECM_POLLING_INFO_T     t_polling_info = {0};
    CECM_LOG_ADDR_T         e_la = CECM_LOG_ADDR_TV;

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_NONE;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_POLL_LA;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;
    t_set_info.t_sync_info.t_sync_data.pv_data = (VOID*)&t_polling_info;
    
    cecm_ctrl_set(h_svc_internal, &t_set_info);
}
/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_handle_tv_la
 *
 * Description: This API used to check LA0 is used or not and trigger timer for polling LA0
 *
 * Inputs:  - pt_evt      References an using event.
            - b_ack       There is an ack or not for specify LA
 *
 * Outputs: -
 *
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_handle_tv_la(CECM_EVT_T* pt_evt,BOOL b_ack)
{
    CECM_NFY_T              t_nfy_data;
     
    if(pt_evt == NULL)
    {
    
        if (ui4_polling_tv_la_itvl != CECM_PING_TV_LA_ITVL && ui4_polling_tv_la_itvl > 0)
        {        
            /* Set a timer for trigger next polling LA0 */
            x_timer_start(h_timer_polling_tv_la,
                          (ui4_polling_tv_la_itvl * 1000),
                          X_TIMER_FLAG_ONCE,
                          _cecm_timer_polling_la_cb,
                          NULL);   
        }    
    }
    else
    {
        
        if(pt_evt->e_la == CECM_LOG_ADDR_TV && !b_ack)      
        {
            /*Polling LA0 no ack*/
            cecm_ctrl_set_tv_la(pt_evt->e_la);
        
            /*notify Ap the current TV la*/
            t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
            t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_TV_LA;
            t_nfy_data.u_data.e_la = pt_evt->e_la;         
            cecm_ctrl_notify_all_clients(&t_nfy_data);   
                          
        }
        else
        {
            /*Polling LA0 and there is ack, so need polling again*/
            if (ui4_polling_tv_la_itvl != CECM_PING_TV_LA_ITVL && ui4_polling_tv_la_itvl > 0)
            {        
                /* Set a timer for trigger next polling LA0 */
                x_timer_start(h_timer_polling_tv_la,
                              (ui4_polling_tv_la_itvl * 1000),
                              X_TIMER_FLAG_ONCE,
                              _cecm_timer_polling_la_cb,
                              NULL);   
            }         
        }        
        cecm_ctrl_evt_free(pt_evt);
    }
    
}
/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_is_handle_valid
 *
 * Description: This API checks if the handle is valid.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *
 * Outputs: -
 *
 * Returns: TRUE        Handle is valid.
 *          FALSE       Handle is invalid.
 ----------------------------------------------------------------------------*/
BOOL cecm_ctrl_is_handle_valid(HANDLE_T h_cecm_svc)
{
    INT32           i4_return;
    HANDLE_TYPE_T   e_hdl_type;

    i4_return = x_handle_get_type(h_cecm_svc, &e_hdl_type);

    if ((i4_return == HR_OK) && (e_hdl_type == CECM_HANDLE_TYPE))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


#ifdef CECM_SKIP_INFO_ACQUISITION_WHEN_PRESENT_BEFORE
/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_is_post_polling_required
 *
 * Description: This API checks if post-polling operation is required.
 *
 * Inputs:  pt_dev_rec      References a device record.
 *
 * Outputs: -
 *
 * Returns: TRUE        Post-polling operation is required.
 *          FALSE       Post-polling operation can be skipped.
 ----------------------------------------------------------------------------*/
BOOL cecm_ctrl_is_post_polling_required(CECM_DEV_REC_T* pt_dev_rec)
{
    CHAR    ac_osd_name[CECM_OSD_NAME_SIZE];

    x_memset(&ac_osd_name[0], 0, CECM_OSD_NAME_SIZE);
    x_uc_w2s_to_ps((const UTF16_T*)&(pt_dev_rec->t_dev.aw2_osd_name)[0],
                   &ac_osd_name[0],
                   x_uc_w2s_strlen((const UTF16_T*)&(pt_dev_rec->t_dev.aw2_osd_name)[0]));

    if (
        ((pt_dev_rec->t_dev.b_dev_present == TRUE) &&
        (((pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa == CECM_INV_PA) && /* Check physical address */
         (pt_dev_rec->t_dev.t_report_phy_addr.e_dev_type == CECM_DEV_TYPE_NONE) &&
         (pt_dev_rec->e_abort_reason_pa == CECM_ABORT_REASON_NONE)) ||
        ((pt_dev_rec->t_dev.t_dev_vndr_id[0] == 0) && /* Check device vendor ID */
         (pt_dev_rec->t_dev.t_dev_vndr_id[1] == 0) &&
         (pt_dev_rec->t_dev.t_dev_vndr_id[2] == 0) &&
         (pt_dev_rec->e_abort_reason_dev_vndr_id == CECM_ABORT_REASON_NONE)) ||
        ((ac_osd_name[0] == 0) && /* Check OSD name */
         (pt_dev_rec->e_abort_reason_osd_name == CECM_ABORT_REASON_NONE)) ||
         b_post_polling_required == TRUE))||(pt_dev_rec->t_dev.b_dev_present == FALSE))

    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#endif


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_get_cec_drv_handle
 *
 * Description: This API get the CEC driver handle.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: NULL_HANDLE     Failed
 *          Other           The CEC driver handle.
 ----------------------------------------------------------------------------*/
HANDLE_T cecm_ctrl_get_cec_drv_handle(VOID)
{
    return (h_cecd);
}


#ifdef CECM_PWR_ON_DEV_EX
/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_select_dev
 *
 * Description: This API enables/disables select_device operation.
 *
 * Inputs:  b_start         Contains the control for select_device operation.
 *          pt_select_dev   References a select_dev info (valid when b_start is
 *                          true).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_select_dev(BOOL                    b_start,
                          CECM_SELECT_DEV_INFO_T* pt_select_dev)
{
    CECM_SET_T      t_set_info;

    /* Stop running select_device operation first if a client would like to start it twice */
    if (cecm_ctrl_is_select_dev_running() && b_start)
    {
        b_select_dev = FALSE;
        cecm_psr_pwr_on_dev(FALSE);
        cecm_psr_set_1st_pwr_on_dev_ack_flag(FALSE);

        t_select_dev_info.h_cecm_svc = NULL_HANDLE;
        t_select_dev_info.e_sent_la = CECM_LOG_ADDR_MAX;
        t_select_dev_info.e_sent_opcode = CECM_OPCODE_NONE;

        /* Stop the timer for select_device operation */
        x_timer_stop(h_timer_select_dev);

        cecm_ctrl_evt_free_by_type(CECM_EVT_TYPE_PWR_STS);

        DBG_INFO(("{CECM} >>>> select_device operation is stopped!\n\r"));
    }

    if ((b_select_dev = b_start) == TRUE)
    {
        if (pt_select_dev != NULL)
        {
            t_select_dev_info = *pt_select_dev;

            /* Start a timer for select_device operation */
            x_timer_start(h_timer_select_dev,
                          (CECM_TIMEOUT_SELECT_DEV * 1000),
                          X_TIMER_FLAG_ONCE,
                          _cecm_timer_select_dev_cb,
                          (VOID*)&t_select_dev_info);

            /* Send <Give Device Power Status> message */
            t_set_info.ui1_dest_addr = (UINT8)pt_select_dev->e_pwr_on_dev_la;
            t_set_info.ui2_opcode = CECM_OPCODE_GIVE_DEV_PWR_STS;
            t_set_info.ui4_addi_info = CECM_ADDI_INFO_PWR_ON_DEV;
            t_set_info.t_sync_info.h_sync = NULL_HANDLE;
            t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;
            cecm_ctrl_set(pt_select_dev->h_cecm_svc, &t_set_info);
        }
    }
    else
    {
        cecm_psr_pwr_on_dev(FALSE);
        cecm_psr_set_1st_pwr_on_dev_ack_flag(FALSE);

        t_select_dev_info.h_cecm_svc = NULL_HANDLE;
        t_select_dev_info.e_sent_la = CECM_LOG_ADDR_MAX;
        t_select_dev_info.e_sent_opcode = CECM_OPCODE_NONE;

        /* Stop the timer for select_device operation */
        x_timer_stop(h_timer_select_dev);
    }
}





/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_get_select_dev_info
 *
 * Description: This API returns the select_device info.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The address of the select_device info.
 ----------------------------------------------------------------------------*/
CECM_SELECT_DEV_INFO_T* cecm_ctrl_get_select_dev_info(VOID)
{
    return (&t_select_dev_info);
}
#else
/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_select_dev
 *
 * Description: This API enables/disables select_device operation.
 *
 * Inputs:  b_start         Contains the control for select_device operation.
 *          pt_select_dev   References a select_dev info (valid when b_start is
 *                          true).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_select_dev(BOOL                    b_start,
                             CECM_SELECT_DEV_INFO_T* pt_select_dev)
{
    CECM_SET_T      t_set_info;

    /* Stop running select_device operation first if a client would like to start it twice */
    if (cecm_ctrl_is_select_dev_running() && b_start)
    {
        b_select_dev = FALSE;
        
        t_select_dev_info.h_cecm_svc = NULL_HANDLE;
        t_select_dev_info.e_sent_la = CECM_LOG_ADDR_MAX;
        t_select_dev_info.e_sent_opcode = CECM_OPCODE_NONE;

        /* Stop the timer for select_device operation */
        x_timer_stop(h_timer_select_dev);

        cecm_ctrl_evt_free_by_type(CECM_EVT_TYPE_PWR_STS);

        DBG_INFO(("{CECM} >>>> select_device operation is stopped!\n\r"));
    }

    if ((b_select_dev = b_start) == TRUE)
    {
        
        if (pt_select_dev != NULL)
        {
            t_select_dev_info = *pt_select_dev;

            /* Start a timer for select_device operation */
            x_timer_start(h_timer_select_dev,
                          (CECM_TIMEOUT_SELECT_DEV * 1000),
                          X_TIMER_FLAG_ONCE,
                          _cecm_timer_select_dev_cb,
                          (VOID*)&t_select_dev_info);
            
            /* Send <User Ctrl Pressed> message */
            t_set_info.ui1_dest_addr = (UINT8)pt_select_dev->e_pwr_on_dev_la;
            t_set_info.ui2_opcode = CECM_OPCODE_USER_CTRL_PRESSED;
            t_set_info.ui4_addi_info = CECM_ADDI_INFO_PWR_ON_DEV;
            t_set_info.u_operand.e_usr_ctrl = CECM_USER_CTRL_PWR_ON_FUNC;
            t_set_info.t_sync_info.h_sync = NULL_HANDLE;
            t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;
            cecm_ctrl_set(pt_select_dev->h_cecm_svc, &t_set_info);        


            /* Send <Set Stream Path> message */

            //mask for 2011_7_1
/*        t_set_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
            t_set_info.ui2_opcode = CECM_OPCODE_SET_STREAM_PATH;
            t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
            t_set_info.u_operand.e_la = pt_select_dev->e_pwr_on_dev_la;
            t_set_info.t_sync_info.h_sync = NULL_HANDLE;
            t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;
            cecm_ctrl_set(pt_select_dev->h_cecm_svc, &t_set_info);  

*/            
        }
    }
    else
    {
              
        t_select_dev_info.h_cecm_svc = NULL_HANDLE;
        t_select_dev_info.e_sent_la = CECM_LOG_ADDR_MAX;
        t_select_dev_info.e_sent_opcode = CECM_OPCODE_NONE;
        

        /* Stop the timer for select_device operation */
        x_timer_stop(h_timer_select_dev);
    }
}

#endif
/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_is_select_dev_running
 *
 * Description: This API checks if select_device operation is running.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE        Select_device operation is running.
 *          FALSE       Select_device operation is not running.
 ----------------------------------------------------------------------------*/
BOOL cecm_ctrl_is_select_dev_running(VOID)
{
    return (b_select_dev);
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_is_dev_discovery_running
 *
 * Description: This API checks if device discovery is running.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE        Device discovery is running.
 *          FALSE       Device discovery is not running.
 ----------------------------------------------------------------------------*/
BOOL cecm_ctrl_is_dev_discovery_running(VOID)
{
    return (b_dev_discovery_running);
}

/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_pwr_on_dev_la
 *
 * Description: This API set the power on device la
 *
 * Inputs:  - e_la     Power on device la
 ----------------------------------------------------------------------------*/

VOID cecm_ctrl_pwr_on_dev_la(CECM_LOG_ADDR_T e_la)
{
  
    e_pwr_on_dev_la = e_la;
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_is_all_dev_discovered
 *
 * Description: This API checks if all device is discovered.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE        All device is discovered.
 *          FALSE       All device is not discovered.
 ----------------------------------------------------------------------------*/
BOOL cecm_ctrl_is_all_dev_discovered(VOID)
{
    if ((t_dd_info.b_ascending && (t_dd_info.ui1_discvry_idx > (UINT8)t_dd_info.t_discvry_info.e_stop_la)) ||
        (!t_dd_info.b_ascending && (t_dd_info.ui1_discvry_idx < (UINT8)t_dd_info.t_discvry_info.e_stop_la)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_is_batch_polling_mode
 *
 * Description: This API checks if device discovery is in batch polling mode.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE        Batch polling mode.
 *          FALSE       Non-batch polling mode.
 ----------------------------------------------------------------------------*/
BOOL cecm_ctrl_is_batch_polling_mode(VOID)
{
    return (t_dd_info.t_discvry_info.b_batch_polling);
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_is_batch_polling_done
 *
 * Description: This API checks if batch polling is done.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE        Batch polling is done.
 *          FALSE       Batch polling is running.
 ----------------------------------------------------------------------------*/
BOOL cecm_ctrl_is_batch_polling_done(VOID)
{
    return (b_batch_polling_done);
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_cmp_dev_list
 *
 * Description: This API compare a device list info with history.
 *
 * Inputs:  ui1_discvry_idx         Contain a device discovery index;
 *
 * Outputs: -
 *
 * Returns: TRUE        Device list info is the same.
 *          FALSE       Device list info is the different.
 ----------------------------------------------------------------------------*/
BOOL cecm_ctrl_cmp_dev_list(UINT8 ui1_discvry_idx)
{
    CECM_DEV_REC_T*     pt_dev_rec;
    CECM_DEV_REC_T*     pt_dev_rec_history;

    pt_dev_rec = &at_dev_lst[ui1_discvry_idx];
    pt_dev_rec_history = &at_dev_lst_history[ui1_discvry_idx];

    if ((pt_dev_rec->t_dev.b_dev_present != pt_dev_rec_history->t_dev.b_dev_present) ||
        (pt_dev_rec->t_dev.b_act_src != pt_dev_rec_history->t_dev.b_act_src) ||
        (pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa != pt_dev_rec_history->t_dev.t_report_phy_addr.ui2_pa) ||
        (pt_dev_rec->t_dev.t_report_phy_addr.e_dev_type != pt_dev_rec_history->t_dev.t_report_phy_addr.e_dev_type) ||
        (pt_dev_rec->t_dev.e_cec_ver != pt_dev_rec_history->t_dev.e_cec_ver) ||
        (pt_dev_rec->t_dev.e_pwr_sts != pt_dev_rec_history->t_dev.e_pwr_sts) ||
        (x_memcmp(&pt_dev_rec->t_dev.t_dev_vndr_info,
                  &pt_dev_rec_history->t_dev.t_dev_vndr_info,
                  sizeof(CECM_DEV_VNDR_INFO_T)) != 0) ||            
        (x_memcmp(&pt_dev_rec->t_dev.t_dev_vndr_id,
                  &pt_dev_rec_history->t_dev.t_dev_vndr_id,
                  CECM_DEV_VNDR_ID_SIZE) != 0) ||
        (x_memcmp(&(pt_dev_rec->t_dev.aw2_osd_name)[0],
                  &(pt_dev_rec_history->t_dev.aw2_osd_name)[0],
                  CECM_OSD_NAME_SIZE) != 0) ||
        (pt_dev_rec_history->b_phy_addr_updated == TRUE ))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_poll_next_dev
 *
 * Description: This API sends polling message to ping next available device
 *              for device discovery.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_poll_next_dev(VOID)
{
    INT32               i4_return;
    CECM_MSG_T          t_msg;
    CECM_DEV_REC_T*     pt_dev_rec;
    CEC_FRAME_INFO_T    t_frame_info;
    BOOL                b_check = TRUE;

    if (!cecm_ctrl_is_dev_discovery_running())
    {
        return;
    }

    cecm_lock_wr_db();

    if (t_dd_info.b_ascending)
    {
        while (t_dd_info.ui1_discvry_idx <= (UINT8)t_dd_info.t_discvry_info.e_stop_la)
        {
            pt_dev_rec = &at_dev_lst[t_dd_info.ui1_discvry_idx];

            if (cecm_ctrl_is_batch_polling_mode() && cecm_ctrl_is_batch_polling_done())
            {
                if (pt_dev_rec->t_dev.b_dev_present && !pt_dev_rec->b_post_polling_handled)
                {
#ifdef CECM_SKIP_INFO_ACQUISITION_WHEN_PRESENT_BEFORE
                    CECM_DEV_REC_T*     pt_dev_rec_history;

                    pt_dev_rec_history = &at_dev_lst_history[t_dd_info.ui1_discvry_idx];

                    if (!cecm_ctrl_is_post_polling_required(pt_dev_rec_history))
                    {
                        /* Skip post-polling operation */
                        pt_dev_rec->b_post_polling_handled = TRUE;
                        *pt_dev_rec = *pt_dev_rec_history;
                    }
                    else
#endif
                    {
                        /* Perform operation after batch polling */
                        t_msg.e_msg_type = CECM_MSG_TYPE_DEV_DISCOVERY_POST_BATCH_POLLING;
                        t_msg.u.ui1_discvry_idx = t_dd_info.ui1_discvry_idx++;

                        x_msg_q_send(*ph_cecm_msgq,
                                     (VOID*)&t_msg,
                                     sizeof(CECM_MSG_T),
                                     CECM_MSGQ_DEFAULT_PRIORITY);

                        b_check = FALSE;
                        break;
                    }
                }

                t_dd_info.ui1_discvry_idx++;
            }
            else
            {
                /* Ping a device */
                t_frame_info.ui1_init_addr = (UINT8)e_tv_la;
                t_frame_info.ui1_dest_addr = t_dd_info.ui1_discvry_idx;
                t_frame_info.ui2_opcode = CECM_OPCODE_NONE;
                t_frame_info.aui1_operand[0] = 0;
                t_frame_info.z_operand_size = 0;

                i4_return = cecm_ctrl_send_msg(h_svc_internal, CECM_EVT_TYPE_DEV_DISCOVERY, &t_frame_info, NULL);
                t_dd_info.ui1_discvry_idx++;

                if (i4_return != TRUE)
                {
                    pt_dev_rec->ui4_update_mask = CECM_UPDATE_MASK_ALL;
                    pt_dev_rec->t_dev.e_dev_state = CECM_DEV_STATE_UPDATED;
                }
                else
                {
                    b_check = FALSE;
                    break;
                }
            }
        }
    }
    else
    {
        while (!CECM_EXCEED_MAX(t_dd_info.ui1_discvry_idx, CECM_LOG_ADDR_MAX) &&
               (t_dd_info.ui1_discvry_idx >= (UINT8)t_dd_info.t_discvry_info.e_stop_la))
        {
            pt_dev_rec = &at_dev_lst[t_dd_info.ui1_discvry_idx];

            if (cecm_ctrl_is_batch_polling_mode() && cecm_ctrl_is_batch_polling_done())
            {
                /* Perform operation after batch polling */
                if (pt_dev_rec->t_dev.b_dev_present && !pt_dev_rec->b_post_polling_handled)
                {
#ifdef CECM_SKIP_INFO_ACQUISITION_WHEN_PRESENT_BEFORE
                    CECM_DEV_REC_T*     pt_dev_rec_history;

                    pt_dev_rec_history = &at_dev_lst_history[t_dd_info.ui1_discvry_idx];

                    if (!cecm_ctrl_is_post_polling_required(pt_dev_rec_history) == TRUE)
                    {
                        /* Skip post-polling operation */
                        pt_dev_rec->b_post_polling_handled = TRUE;
                        *pt_dev_rec = *pt_dev_rec_history;
                    }
                    else
#endif
                    {
                        t_msg.e_msg_type = CECM_MSG_TYPE_DEV_DISCOVERY_POST_BATCH_POLLING;
                        t_msg.u.ui1_discvry_idx = t_dd_info.ui1_discvry_idx--;

                        x_msg_q_send(*ph_cecm_msgq,
                                     (VOID*)&t_msg,
                                     sizeof(CECM_MSG_T),
                                     CECM_MSGQ_DEFAULT_PRIORITY);

                        b_check = FALSE;
                        break;
                    }
                }

                t_dd_info.ui1_discvry_idx--;
            }
            else
            {
                /* Ping a device */
                t_frame_info.ui1_init_addr = (UINT8)e_tv_la;
                t_frame_info.ui1_dest_addr = t_dd_info.ui1_discvry_idx;
                t_frame_info.ui2_opcode = CECM_OPCODE_NONE;
                t_frame_info.aui1_operand[0] = 0;
                t_frame_info.z_operand_size = 0;

                i4_return = cecm_ctrl_send_msg(h_svc_internal, CECM_EVT_TYPE_DEV_DISCOVERY, &t_frame_info, NULL);
                t_dd_info.ui1_discvry_idx--;

                if (i4_return != TRUE)
                {
                    pt_dev_rec->ui4_update_mask = CECM_UPDATE_MASK_ALL;
                    pt_dev_rec->t_dev.e_dev_state = CECM_DEV_STATE_UPDATED;
                }
                else
                {
                    b_check = FALSE;
                    break;
                }
            }
        }
    }

    cecm_unlock_db();

    if (b_check)
    {
        if (cecm_ctrl_is_batch_polling_mode() &&
            !cecm_ctrl_is_batch_polling_done())
        {
            b_batch_polling_done = TRUE;
            t_dd_info.ui1_discvry_idx = (UINT8)t_dd_info.t_discvry_info.e_start_la;

            /* Start post-polling operation */
            cecm_ctrl_poll_next_dev();
        }
        else
        {
            cecm_ctrl_chk_dev_discovery(&t_dd_info.t_discvry_info);
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_stop_dev_discovery
 *
 * Description: This API stops current device discovery.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_stop_dev_discovery(VOID)
{
    UINT8               ui1_idx;
    CECM_DEV_REC_T*     pt_dev_rec;
    CECM_DEV_REC_T*     pt_dev_rec_history;

    if (cecm_ctrl_is_dev_discovery_running())
    {
        DBG_INFO(("{CECM} >>>>>>>>>> Device discovery is interrupted by client (%d->%d) <<<<<<<<<<\n\r",
                 t_dd_info.t_discvry_info.e_start_la,
                 t_dd_info.t_discvry_info.e_stop_la));

        b_dev_discovery_running = FALSE;
    }

    b_batch_polling_done = FALSE;
    x_timer_stop(h_timer_dev_discovery);
    cecm_psr_reset_dev_post_polling_ind();

    cecm_lock_wr_db();

    LOOP_FOR_EACH(ui1_idx, t_dd_info.t_discvry_info.e_start_la, t_dd_info.t_discvry_info.e_stop_la, t_dd_info.b_ascending)
    {
        if (CECM_EXCEED_MAX(ui1_idx, CECM_LOG_ADDR_MAX))
        {
            break;
        }
                                
        pt_dev_rec = &at_dev_lst[ui1_idx];
        pt_dev_rec_history = &at_dev_lst_history[ui1_idx];

        /* Sync history device list with current one */
        if (pt_dev_rec->t_dev.e_dev_state == CECM_DEV_STATE_UPDATED)
        {
            *pt_dev_rec_history = *pt_dev_rec;
        }
        else
        {
            pt_dev_rec->t_dev.e_dev_state = CECM_DEV_STATE_UPDATED;
        }
    }

    cecm_unlock_db();

    cecm_ctrl_evt_free_by_type(CECM_EVT_TYPE_DEV_DISCOVERY);
}

/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_config_dev_discovery
 *
 * Description: This API config device discovery
 *
 * Inputs:  - pt_dev_discovery_config  Reference device discovery
 *
 * Outputs: - 
 *
 * Returns: -
 ----------------------------------------------------------------------------*/

INT32 cecm_ctrl_config_dev_discovery(HANDLE_T        h_cecm_svc,
                                         CECM_DEV_DISCOVERY_CONFIG_T*   pt_dev_discovery_config)
{

    if(pt_dev_discovery_config == NULL)
    {
        return CECMR_INV_ARG;
    }
    if((pt_dev_discovery_config->t_dd_vndr_cmd.z_dd_vndr_cmd_size > 0 &&
        pt_dev_discovery_config->pf_cecm_parse_dd_vndr_cmd == NULL) ||
        pt_dev_discovery_config->t_dd_vndr_cmd.z_dd_vndr_cmd_size > CECM_DD_VNDR_CMD_MAX_SIZE)
    {
        return CECMR_INV_ARG;
    }
    cecm_psr_config_dev_discovery(pt_dev_discovery_config);

    return CECMR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_get_dev_discovery_config
 *
 * Description: This API get device discovery config
 *
 * Inputs:  -
 *
 * Outputs: - pt_dev_discovery_config  Reference device discovery
 *
 * Returns: -
 ----------------------------------------------------------------------------*/

INT32 cecm_ctrl_get_dev_discovery_config(HANDLE_T        h_cecm_svc,
                                               CECM_DEV_DISCOVERY_CONFIG_T*   pt_dev_discovery_config)
{
    if(pt_dev_discovery_config == NULL)
    {
        return CECMR_INV_ARG;
    }

    cecm_psr_get_dev_discovery_config(pt_dev_discovery_config);
    return CECMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_start_dev_discovery
 *
 * Description: This API refreshes the HDMI CEC network topology.
 *
 * Inputs:  pt_dev_discovery_info       References device discovery info.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 cecm_ctrl_start_dev_discovery(CECM_DEV_DISCOVERY_T* pt_dev_discovery_info)
{
    UINT8               ui1_idx;
    CECM_DEV_REC_T*     pt_dev_rec;
    CECM_MSG_T          t_msg;

    /* If device discovery is running, stop it first */
    if (cecm_ctrl_is_dev_discovery_running())
    {
        cecm_ctrl_stop_dev_discovery();
    }

    /* If device post-polling operation is running, postpond the request */
    if (cecm_psr_is_dev_post_polling_running())
    {
        t_msg.e_msg_type = CECM_MSG_TYPE_DEV_DISCOVERY_START;
        t_msg.u.t_dev_discvry_info = *pt_dev_discovery_info;

        x_msg_q_send(*ph_cecm_msgq,
                     (VOID*)&t_msg,
                     sizeof(CECM_MSG_T),
                     CECM_MSGQ_DEFAULT_PRIORITY);

        return CECMR_OK;
    }

    DBG_INFO(("{CECM} <<<<<<<<<< Device discovery begins (%d->%d/bp=%d) >>>>>>>>>>\n\r",
             pt_dev_discovery_info->e_start_la,
             pt_dev_discovery_info->e_stop_la,
             pt_dev_discovery_info->b_batch_polling));
    b_dev_discovery_running = TRUE;
    t_dd_info.t_discvry_info = *pt_dev_discovery_info;
    t_dd_info.b_ascending = (pt_dev_discovery_info->e_start_la > pt_dev_discovery_info->e_stop_la) ? FALSE : TRUE;
    t_dd_info.ui1_discvry_idx = pt_dev_discovery_info->e_start_la;
    t_dd_info_client.t_discvry_info = *pt_dev_discovery_info;

    LOOP_FOR_EACH(ui1_idx, t_dd_info.t_discvry_info.e_start_la, t_dd_info.t_discvry_info.e_stop_la, t_dd_info.b_ascending)
    {
        if (CECM_EXCEED_MAX(ui1_idx, CECM_LOG_ADDR_MAX))
        {
            break;
        }
                
        pt_dev_rec = &at_dev_lst[ui1_idx];

        cecm_lock_wr_db();
        /* Reset device list */
        pt_dev_rec->ui4_update_mask = CECM_UPDATE_MASK_NONE;
        pt_dev_rec->ui1_polling_retry = 0;
        pt_dev_rec->e_post_polling_runner = CECM_PP_RUNNER_TV;
        pt_dev_rec->b_post_polling_handled = FALSE;
        pt_dev_rec->b_post_polling_retry = FALSE;
        pt_dev_rec->t_dev.e_dev_state = CECM_DEV_STATE_UPDATING;
        pt_dev_rec->t_dev.b_dev_present = FALSE;
        pt_dev_rec->t_dev.b_act_src = FALSE;
        pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa = CECM_INV_PA;
        pt_dev_rec->t_dev.t_report_phy_addr.e_dev_type = CECM_DEV_TYPE_NONE;
        x_memset(&(pt_dev_rec->t_dev.aw2_osd_name)[0], 0, CECM_OSD_NAME_SIZE);
        x_memset(&pt_dev_rec->t_dev.t_dev_vndr_id, 0, CECM_DEV_VNDR_ID_SIZE);
        x_memset(&(pt_dev_rec->t_dev.t_dev_vndr_info), 0, sizeof(CECM_DEV_VNDR_INFO_T)); 
        pt_dev_rec->t_dev.e_cec_ver = CECM_CEC_VER_INVALID;
        pt_dev_rec->t_dev.e_pwr_sts = CECM_PWR_STS_ERROR;      
        pt_dev_rec->e_abort_reason_pa = CECM_ABORT_REASON_NONE;
        pt_dev_rec->e_abort_reason_dev_vndr_id = CECM_ABORT_REASON_NONE;
        pt_dev_rec->e_abort_reason_osd_name = CECM_ABORT_REASON_NONE;
        pt_dev_rec->e_abort_reason_cev_ver = CECM_ABORT_REASON_NONE;
        pt_dev_rec->e_abort_reason_pwr_sts = CECM_ABORT_REASON_NONE;
        x_memset(&(pt_dev_rec->ae_abort_reason_vndr_info[0]),CECM_ABORT_REASON_NONE, CECM_DD_VNDR_CMD_MAX_SIZE);   
        pt_dev_rec->b_phy_addr_updated = FALSE;
        pt_dev_rec->ui4_skip_update_mask = CECM_UPDATE_MASK_NONE;
        pt_dev_rec->ui1_vndr_cmd_idx = 0;
        pt_dev_rec->t_msg.e_msg_type = CECM_MSG_TYPE_NONE;
        cecm_unlock_db();
    }

#if 0
    /* Query active source */
    t_frame_info.ui1_init_addr = (UINT8)e_tv_la;
    t_frame_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
    t_frame_info.ui2_opcode = CECM_OPCODE_REQ_ACT_SRC;
    t_frame_info.aui1_operand[0] = 0;
    t_frame_info.z_operand_size = 0;

    i4_return = cecm_ctrl_send_msg(h_svc_internal, CECM_EVT_TYPE_DEV_DISCOVERY, &t_frame_info, NULL);

    if (i4_return != TRUE)
    {
        cecm_ctrl_poll_next_dev();
        return CECMR_ERROR;
    }
#endif

    /* Ping a device */
    cecm_ctrl_poll_next_dev();

    return CECMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_chk_dev_discovery
 *
 * Description: This API checks and notifies clients the result of device
 *              discovery if different.
 *
 * Inputs:  pt_discvry_info     References device discovery info.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_chk_dev_discovery(CECM_DEV_DISCOVERY_T* pt_discvry_info)
{
    UINT8               ui1_idx;
    BOOL                b_dev_discvry_ready = TRUE;
    CECM_NFY_T          t_nfy_data;
    BOOL                b_updated = FALSE;

    if(b_dev_lst_nfy_history == TRUE)
    {
        b_dev_lst_nfy_history = FALSE;
        CECM_DEV_REC_T*     pt_dev_rec;
        CECM_DEV_REC_T*     pt_dev_rec_history;
        LOOP_FOR_EACH(ui1_idx, t_dd_info.t_discvry_info.e_start_la, t_dd_info.t_discvry_info.e_stop_la, t_dd_info.b_ascending)
        {
            if (CECM_EXCEED_MAX(ui1_idx, CECM_LOG_ADDR_MAX))
            {
                break;             
            }
                        
            pt_dev_rec = &at_dev_lst[ui1_idx];
            pt_dev_rec_history = &at_dev_lst_history[ui1_idx];

            cecm_lock_wr_db();
            *pt_dev_rec = *pt_dev_rec_history;
            cecm_unlock_db();
        }    
    }
    
    if (!cecm_ctrl_is_dev_discovery_running())
    {
        ui1_idx = (UINT8)pt_discvry_info->e_start_la;            
        if (CECM_EXCEED_MAX(ui1_idx, CECM_LOG_ADDR_MAX))
        {
            return;             
        }
            
        if (!cecm_ctrl_cmp_dev_list(ui1_idx))
        {
            b_updated = TRUE;
        }

        if (!CECM_EXCEED_MAX((UINT8)pt_discvry_info->e_start_la, CECM_LOG_ADDR_MAX) &&
            (at_dev_lst[(UINT8)pt_discvry_info->e_start_la].t_dev.e_dev_state == CECM_DEV_STATE_UPDATED))
        {
            /* Sync history device list with current one */
            cecm_lock_wr_db();
            at_dev_lst_history[(UINT8)pt_discvry_info->e_start_la] = at_dev_lst[(UINT8)pt_discvry_info->e_start_la];
            cecm_unlock_db();

            DBG_INFO(("{CECM} >>>>>>>>>> Dev post-polling finished (%d->%d/%s) <<<<<<<<<<\n\r\n\r",
                     pt_discvry_info->e_start_la,
                     pt_discvry_info->e_stop_la,
                     (b_updated ? "updated" : "no_chg")));
            t_nfy_data.e_sent_la = CECM_LOG_ADDR_MAX;
            t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
            t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
            t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
            t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
            t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_DEV_LIST_NFY;
            t_nfy_data.u_data.t_dev_lst_nfy.e_dev_lst_sts = (b_updated ? CECM_DEV_LIST_STS_UPDATED : CECM_DEV_LIST_STS_NO_CHANGE);
            t_nfy_data.u_data.t_dev_lst_nfy.e_start_la = pt_discvry_info->e_start_la;
            t_nfy_data.u_data.t_dev_lst_nfy.e_stop_la = pt_discvry_info->e_stop_la;
            cecm_ctrl_notify_all_clients(&t_nfy_data);
#ifdef CECM_SKIP_INFO_ACQUISITION_WHEN_PRESENT_BEFORE
            b_post_polling_required = FALSE;
#endif
            
        }
    }
    else
    {
        /* Check if device discovery is ready */
        LOOP_FOR_EACH(ui1_idx, t_dd_info.t_discvry_info.e_start_la, t_dd_info.t_discvry_info.e_stop_la, t_dd_info.b_ascending)
        {
            if (CECM_EXCEED_MAX(ui1_idx, CECM_LOG_ADDR_MAX))
            {
                break;             
            }
            
            if (at_dev_lst[ui1_idx].t_dev.e_dev_state != CECM_DEV_STATE_UPDATED)
            {
                b_dev_discvry_ready = FALSE;
                break;
            }
        }

        if (b_dev_discvry_ready)
        {
            CECM_DEV_REC_T*     pt_dev_rec;
            CECM_DEV_REC_T*     pt_dev_rec_history;

            LOOP_FOR_EACH(ui1_idx, t_dd_info.t_discvry_info.e_start_la, t_dd_info.t_discvry_info.e_stop_la, t_dd_info.b_ascending)
            {
                if (CECM_EXCEED_MAX(ui1_idx, CECM_LOG_ADDR_MAX))
                {
                    break;             
                }
                            
                if (!cecm_ctrl_cmp_dev_list(ui1_idx))
                {
                    b_updated = TRUE;
                    break;
                }
            }

            /* Sync history device list with current one */
            LOOP_FOR_EACH(ui1_idx, t_dd_info.t_discvry_info.e_start_la, t_dd_info.t_discvry_info.e_stop_la, t_dd_info.b_ascending)
            {
                if (CECM_EXCEED_MAX(ui1_idx, CECM_LOG_ADDR_MAX))
                {
                    break;             
                }
                            
                pt_dev_rec = &at_dev_lst[ui1_idx];
                pt_dev_rec_history = &at_dev_lst_history[ui1_idx];

                cecm_lock_wr_db();
                *pt_dev_rec_history = *pt_dev_rec;
                cecm_unlock_db();
            }

            DBG_INFO(("{CECM} >>>>>>>>>> Device discovery finished (%d->%d/%s) <<<<<<<<<<\n\r\n\r",
                     t_dd_info.t_discvry_info.e_start_la,
                     t_dd_info.t_discvry_info.e_stop_la,
                     (b_updated ? "updated" : "no_change")));
            t_nfy_data.e_sent_la = CECM_LOG_ADDR_MAX;
            t_nfy_data.e_sent_opcode = CECM_OPCODE_NONE;
            t_nfy_data.e_nfy_type = CECM_NFY_TYPE_MSGCODE;
            t_nfy_data.e_brdcst_type = CECM_BRDCST_TYPE_NONE;
            t_nfy_data.ui4_attr = CECM_ATTR_IGNORE;
            t_nfy_data.u_code.e_msgcode = CECM_MSGCODE_DEV_LIST_NFY;
            t_nfy_data.u_data.t_dev_lst_nfy.e_dev_lst_sts = (b_updated ? CECM_DEV_LIST_STS_UPDATED : CECM_DEV_LIST_STS_NO_CHANGE);
            t_nfy_data.u_data.t_dev_lst_nfy.e_start_la = t_dd_info.t_discvry_info.e_start_la;
            t_nfy_data.u_data.t_dev_lst_nfy.e_stop_la = t_dd_info.t_discvry_info.e_stop_la;
            cecm_ctrl_notify_all_clients(&t_nfy_data);
#ifdef CECM_SKIP_INFO_ACQUISITION_WHEN_PRESENT_BEFORE
            b_post_polling_required = FALSE;
#endif

            b_dev_discovery_running = FALSE;
            b_batch_polling_done = FALSE;

            /*send msg to msgq when device discovery completed*/
            LOOP_FOR_EACH(ui1_idx, t_dd_info.t_discvry_info.e_start_la, t_dd_info.t_discvry_info.e_stop_la, t_dd_info.b_ascending)
            {
                if (CECM_EXCEED_MAX(ui1_idx, CECM_LOG_ADDR_MAX))
                {
                    break;             
                }          
                pt_dev_rec = &at_dev_lst[ui1_idx];

                cecm_lock_rd_db();
                if(pt_dev_rec->t_msg.e_msg_type != CECM_MSG_TYPE_NONE)
                {
                    x_msg_q_send(*ph_cecm_msgq,
                                 (VOID*)&pt_dev_rec->t_msg,
                                 sizeof(CECM_MSG_T),
                                 CECM_MSGQ_DEFAULT_PRIORITY);                
                }              
                cecm_unlock_db();
            }

            /* Update the status for device discovery */
            if (t_dd_info.t_discvry_info.ui4_itvl != CECM_INV_DEV_DISCVRY_ITVL)
            {
                if (t_dd_info.t_discvry_info.ui4_itvl > 0) /* Periodical */
                {
                    /* Set a timer for triggering next devive discovery */
                    x_timer_start(h_timer_dev_discovery,
                                  (t_dd_info.t_discvry_info.ui4_itvl * 1000),
                                  X_TIMER_FLAG_ONCE,
                                  _cecm_timer_dev_discovery_start_cb,
                                  NULL);
                }
            }
        }
    }
}

#ifdef CECM_5V_POWER_DEVICE_DISCOVERY

/*-----------------------------------------------------------------------------

 * Name: cecm_ctrl_5v_power_dev_discovery
 *
 * Description: This API start device discovery
 *
 * Outputs: -
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_5v_power_dev_discovery()
{
    INT32           i4_return;
    CECM_MSG_T      t_msg;
    t_msg.e_msg_type = CECM_MSG_TYPE_DEV_DISCOVERY_START;
    t_msg.u.t_dev_discvry_info.e_start_la = CECM_LOG_ADDR_REC_DEV_1;
    t_msg.u.t_dev_discvry_info.e_stop_la = CECM_LOG_ADDR_FREE_USE;
    t_msg.u.t_dev_discvry_info.ui4_itvl = t_dd_info.t_discvry_info.ui4_itvl;
    t_msg.u.t_dev_discvry_info.b_batch_polling = t_dd_info.t_discvry_info.b_batch_polling;
    
    i4_return = x_msg_q_send(*ph_cecm_msgq,
                             (VOID*)&t_msg,
                             sizeof(CECM_MSG_T),
                             CECM_MSGQ_DEFAULT_PRIORITY);

    if (i4_return != OSR_OK)
    {
        DBG_ERROR(("{CECM} ERR: cecm_ctrl_5v_power_dev_discovery() device discovery failed\n\r"));
    }    
}
#endif
/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_q_evt
 *
 * Description: This API queues a CEC event.
 *
 * Inputs:  pt_svc          References a CECM service.
 *          e_evt_type      Contains the event type.
 *          pt_frame_info   References a frame info.
 *          pt_sync_info    References a sync info.
 *
 * Outputs: -
 *
 * Returns: NULL        No free event available.
 *          Other       References a free event.
 ----------------------------------------------------------------------------*/
CECM_EVT_T* cecm_ctrl_q_evt(CECM_SVC_T*       pt_svc,
                            CECM_EVT_TYPE_T   e_evt_type,
                            CEC_FRAME_INFO_T* pt_frame_info,
                            CECM_SYNC_INFO_T* pt_sync_info)
{
    CECM_EVT_T*     pt_evt = NULL;

    pt_evt = cecm_ctrl_evt_alloc(pt_svc);

    /* Update event structure */
    if (pt_evt != NULL)
    {
        cecm_lock_wr_db();

        pt_evt->e_type = e_evt_type;
        pt_evt->e_opcode = (CECM_OPCODE_T)(pt_frame_info->ui2_opcode);

        if ((e_evt_type == CECM_EVT_TYPE_ONE_TOUCH_PLAY) &&
            ((pt_evt->e_opcode == CECM_OPCODE_IMG_VIEW_ON) || (pt_evt->e_opcode == CECM_OPCODE_TXT_VIEW_ON)))
        {
            pt_evt->e_state = CECM_EVT_STATE_WAIT_FOR_FRAME;
            pt_evt->e_la = (CECM_LOG_ADDR_T)(pt_frame_info->ui1_init_addr);
        }
        else
        {
            pt_evt->e_state = CECM_EVT_STATE_WAIT_FOR_ACK;
            if((e_evt_type == CECM_EVT_TYPE_PWR_STS) && 
               ((CECM_OPCODE_T)(pt_frame_info->ui2_opcode) == CECM_OPCODE_SET_STREAM_PATH))
            {
                pt_evt->e_la = e_pwr_on_dev_la;     
            }
            else
            {
                pt_evt->e_la = (CECM_LOG_ADDR_T)(pt_frame_info->ui1_dest_addr);
            }

            if (e_evt_type == CECM_EVT_TYPE_DEV_DISCOVERY)
            {
                 if (x_timer_start(pt_evt->h_timer,
                              CECM_TIMOUT_ACK_FOR_DEV_DISCOVERY,
                              X_TIMER_FLAG_ONCE,
                              _cecm_timer_ack_cb,
                              (VOID*)pt_evt) != OSR_OK)
                {
                    cecm_unlock_db();
                    return NULL;
                }
            }
            else
            {
                if (x_timer_start(pt_evt->h_timer,
                                  CECM_TIMEOUT_ACK,
                                  X_TIMER_FLAG_ONCE,
                                  _cecm_timer_ack_cb,
                                  (VOID*)pt_evt) != OSR_OK)
                {
                    cecm_unlock_db();
                    return NULL;
                }
            }

        }

        /* Synchronous operations */
        if (pt_sync_info == NULL)
        {
            pt_evt->h_sync = NULL_HANDLE;
            pt_evt->pt_sync_data = NULL;
        }
        else
        {
            pt_evt->h_sync = pt_sync_info->h_sync;
            pt_evt->pt_sync_data = &(pt_sync_info->t_sync_data);
        }

        /* Store the info for generating fake routing info if required */
        if (pt_evt->e_opcode == CECM_OPCODE_ROUTING_CHG)
        {
            pt_evt->u.t_routing_chg.ui2_orig_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[0]);
            pt_evt->u.t_routing_chg.ui2_new_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_frame_info->aui1_operand[2]);
        }

        cecm_unlock_db();
    }

    return (pt_evt);
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_send_msg
 *
 * Description: This API sends a CEC message to CEC driver.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_evt_type      The event type associated with the message.
 *          pt_frame_info   References a frame info.
 *          pt_sync_info    References a sync info.
 *
 * Outputs: -
 *
 * Returns: TRUE        Operation successful.
 *          FALSE       Operation failed.
 ----------------------------------------------------------------------------*/
BOOL cecm_ctrl_send_msg(HANDLE_T          h_cecm_svc,
                        CECM_EVT_TYPE_T   e_evt_type,
                        CEC_FRAME_INFO_T* pt_frame_info,
                        CECM_SYNC_INFO_T* pt_sync_info)
{
    INT32               i4_return;
    VOID*               pv_obj = NULL;
    HANDLE_TYPE_T       e_type;
    CECM_EVT_T*         pt_evt = NULL;
    CEC_SEND_MSG_T      t_send_msg;
    CEC_GET_MSG_ENQUEUE_T   t_get_msg_enqueue;
    SIZE_T  z_get_info_len;

    /* Check system status */
    if (!cecm_ctrl_is_cec_fct_enable() ||
        (!cecm_ctrl_is_dev_discovery_running() &&
         !cecm_psr_is_dev_post_polling_running() &&
         (e_evt_type == CECM_EVT_TYPE_DEV_DISCOVERY))
#ifdef CECM_NOT_SEND_MSG_FOR_ERR_LA        
         || ((e_tv_la == CECM_LOG_ADDR_FREE_USE || e_tv_la == CECM_LOG_ADDR_UNREGED_BRDCST) &&
             (e_evt_type != CECM_EVT_TYPE_POLL_LA))
         /* Do not send message if LA is 14 or 15 and not the poll message*/
#endif
        )   
    {
        return FALSE;
    }

#ifdef CECM_PWR_ON_DEV_EX
    if (!cecm_ctrl_is_select_dev_running() && (e_evt_type == CECM_EVT_TYPE_PWR_STS))
    {
        return FALSE;
    }
#endif

    /* Determine if the event should be queued */
    if (h_cecm_svc != NULL_HANDLE)
    {
        /* Obtain CECM internal service */
        i4_return = handle_get_type_obj(h_cecm_svc, &e_type,  &pv_obj);

        if (i4_return != HR_OK )
        {
            return FALSE;
        }
        else if(e_type != CECM_HANDLE_TYPE)
        {
            #ifdef DEBUG
            dbg_abort(DBG_MOD_CEC_MNGR | DBG_CAT_HANDLE);
            #endif
            return FALSE;
            
        }
        

        pt_evt = cecm_ctrl_q_evt((CECM_SVC_T*)pv_obj, e_evt_type, pt_frame_info, pt_sync_info);
    }

    /* Send out the CEC message */
    t_send_msg.pv_tag = (VOID*)pt_evt;
    t_send_msg.t_frame_info = *pt_frame_info;
    t_send_msg.b_enqueue_ok = FALSE;

    i4_return = rm_set(h_cecd,
                     FALSE,
                     ANY_PORT_NUM,
                     CEC_SET_TYPE_SEND_MSG,
                     (VOID*)&t_send_msg,
                     sizeof(CEC_SEND_MSG_T),
                     NULL);
    if ((i4_return != RMR_OK))
    {
        if (pt_evt)
        {
            cecm_ctrl_evt_free(pt_evt);
        }

        if (e_evt_type == CECM_EVT_TYPE_DEV_DISCOVERY)
        {         
        if (t_dd_info.b_ascending)
        {
            t_dd_info.ui1_discvry_idx = t_dd_info.t_discvry_info.e_stop_la;
        }
        else
        {
            t_dd_info.ui1_discvry_idx = t_dd_info.t_discvry_info.e_start_la;
        }
        b_dev_lst_nfy_history = TRUE;
        return TRUE;
        }
        else
        {
        DBG_ERROR(("{CECM} Msg enqueue failed (ret=%d)\n\r", i4_return));
        return FALSE;
        }   
    }    
    t_get_msg_enqueue.b_enqueue_ok = FALSE;
    z_get_info_len = sizeof(CEC_GET_MSG_ENQUEUE_T);  
    i4_return = rm_get(h_cecd,
                     FALSE,
                     ANY_PORT_NUM,
                     CEC_GET_TYPE_MSG_ENQUEUE,
                     (VOID*)&t_get_msg_enqueue,
                     &z_get_info_len,
                     NULL);
    
    if (!t_send_msg.b_enqueue_ok && !t_get_msg_enqueue.b_enqueue_ok)
      {
          if (pt_evt)
          {
              cecm_ctrl_evt_free(pt_evt);
          }
          if (e_evt_type == CECM_EVT_TYPE_DEV_DISCOVERY)
          {         
            if (t_dd_info.b_ascending)
            {
                t_dd_info.ui1_discvry_idx = t_dd_info.t_discvry_info.e_stop_la;
            }
            else
            {
                t_dd_info.ui1_discvry_idx = t_dd_info.t_discvry_info.e_start_la;
            }
            b_dev_lst_nfy_history = TRUE;
            return TRUE;
          }
          else
          {
              DBG_ERROR(("{CECM} Msg enqueue failed (ret=%d/t_send_msg.enqueue_ok=%d/t_get_msg_enqueue.enqueue_ok=%d)\n\r", i4_return,t_send_msg.b_enqueue_ok,t_get_msg_enqueue.b_enqueue_ok));
              return FALSE;
          }

      }

    if (e_evt_type == CECM_EVT_TYPE_PWR_STS)
    {
        /* Store select_device info */
        t_select_dev_info.e_sent_la = (CECM_LOG_ADDR_T)t_send_msg.t_frame_info.ui1_dest_addr;
        t_select_dev_info.e_sent_opcode = (CECM_OPCODE_T)t_send_msg.t_frame_info.ui2_opcode;
    }

    DBG_INFO(("{CECM} Frame (sent): 0x%x (la=%d, type=%d)\n\r",
             pt_frame_info->ui2_opcode,
             pt_frame_info->ui1_dest_addr,
             e_evt_type));

    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_evt_alloc
 *
 * Description: This API gets an event from free event pool.
 *
 * Inputs:  pt_src      References a service that requests the event.
 *
 * Outputs: -
 *
 * Returns: NULL        No free event available.
 *          Other       References a free event.
 ----------------------------------------------------------------------------*/
CECM_EVT_T* cecm_ctrl_evt_alloc(CECM_SVC_T* pt_svc)
{
    CECM_EVT_T*     pt_evt;

    cecm_lock_wr_db();

    if (((pt_evt = DLIST_HEAD(&t_free_evt_pool)) == NULL) ||
        (pt_svc == NULL))
    {
        cecm_unlock_db();
        return NULL;
    }

    DLIST_REMOVE(pt_evt, &t_free_evt_pool, t_dlink);
    DLIST_INSERT_TAIL(pt_evt, &pt_svc->t_evt_lst, t_dlink);

    /* Update event structure */
    pt_evt->pt_svc = pt_svc;

    cecm_unlock_db();

    return (pt_evt);
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_evt_free
 *
 * Description: This API frees an event back to free event pool.
 *
 * Inputs:  pt_evt      References an using event.
 *
 * Outputs: -
 *
 * Returns: TRUE        Operation successful.
 *          FALSE       Operation failed.
 ----------------------------------------------------------------------------*/
BOOL cecm_ctrl_evt_free(CECM_EVT_T* pt_evt)
{
    if ((pt_evt == NULL) || (pt_evt->pt_svc == NULL))
    {
        return FALSE;
    }

    cecm_lock_wr_db();

    DLIST_REMOVE(pt_evt, &pt_evt->pt_svc->t_evt_lst, t_dlink);
    DLIST_INSERT_TAIL(pt_evt, &t_free_evt_pool, t_dlink);

    /* Reset event structure */
    x_timer_stop(pt_evt->h_timer);
    pt_evt->e_type = CECM_EVT_TYPE_NONE;
    pt_evt->e_state = CECM_EVT_STATE_NONE;
    pt_evt->e_la = CECM_LOG_ADDR_MAX;
    pt_evt->e_opcode = CECM_OPCODE_NONE;
    pt_evt->pt_svc = NULL;
    pt_evt->h_sync = NULL_HANDLE;
    pt_evt->pt_sync_data = NULL;

    cecm_unlock_db();

    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_evt_free_by_type
 *
 * Description: This API frees all using events.
 *
 * Inputs:  e_type      Contains event type.
 *
 * Outputs: -
 *
 * Returns: TRUE        Operation successful.
 *          FALSE       Operation failed.
 ----------------------------------------------------------------------------*/
BOOL cecm_ctrl_evt_free_by_type(CECM_EVT_TYPE_T e_type)
{
    UINT8           ui1_idx;
    CECM_EVT_T*     pt_evt = NULL;

    for (ui1_idx = CECM_SVC_RESERVED; ui1_idx < CECM_MAX_SVC; ui1_idx++)
    {
        if (at_svc_lst[ui1_idx].h_svc == NULL_HANDLE)
        {
            continue;
        }
        else
        {
            pt_evt = DLIST_HEAD(&at_svc_lst[ui1_idx].t_evt_lst);

            while (pt_evt != NULL)
            {
                if ((e_type == CECM_EVT_TYPE_NONE) ||
                    (pt_evt->e_type == e_type))
                {
                    if (!cecm_ctrl_evt_free(pt_evt))
                    {
                        return FALSE;
                    }

                    pt_evt = DLIST_HEAD(&at_svc_lst[ui1_idx].t_evt_lst);
                }
                else
                {
                    pt_evt = DLIST_NEXT(pt_evt, t_dlink);
                }
            }
        }
    }

    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_cal_comp_id_from_pa
 *
 * Description: This API calculates a component ID based on a physical address.
 *
 * Inputs:  ui2_pa          A physical address.
 *
 * Outputs: -
 *
 * Returns: Component ID
 ----------------------------------------------------------------------------*/
UINT16 cecm_ctrl_cal_comp_id_from_pa(UINT16 ui2_pa)
{
    UINT16      ui2_comp_id;

    ui2_comp_id = (UINT16)((ui2_pa >> 12) - 1);

    if (ui2_comp_id >= ui1_hdmi_port_num)
    {
        ui2_comp_id = CECM_INV_COMP_ID;
    }

    return (ui2_comp_id);
}

UINT8 cecm_ctrl_get_hdmi_port_num(VOID)
{
    return ui1_hdmi_port_num;
}

/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_get_dev_rec_by_la
 *
 * Description: This API gets device record by logical address.
 *
 * Inputs:  e_la                A logical address.
 *          b_dev_rec_history   Indicates source of obtained device record.
 *
 * Outputs: -
 *
 * Returns: The pointer of Device record.
 ----------------------------------------------------------------------------*/
CECM_DEV_REC_T* cecm_ctrl_get_dev_rec_by_la(CECM_LOG_ADDR_T e_la,
                                            BOOL            b_dev_rec_history)
{
    if (b_dev_rec_history)
    {
        return (&at_dev_lst_history[e_la]);
    }
    else
    {
        return (&at_dev_lst[e_la]);
    }
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_get_la_by_pa
 *
 * Description: This API gets the logical address of a physical address.
 *
 * Inputs:  ui2_pa          A physical address.
 *          b_1st_matched   Indicates a first-matched searching or 
 *                          every-matched searching.
 *
 * Outputs: pv_data         References a logical address or logical address 
 *                          list.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 ----------------------------------------------------------------------------*/
INT32 cecm_ctrl_get_la_by_pa(UINT16 ui2_pa,
                             BOOL   b_1st_matched,
                             VOID*  pv_data)
{
    UINT32      ui4_retry;
    UINT8       ui1_idx;

    if (pv_data == NULL)
    {
        return CECMR_INV_ARG;
    }

    if (!b_1st_matched)
    {
        ((CECM_LOG_ADD_LST_T*)pv_data)->ui1_num = 0;
    }
    
    for (ui1_idx = (UINT8)CECM_LOG_ADDR_REC_DEV_1; ui1_idx < (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST; ui1_idx++)
    {
        cecm_lock_rd_db();
        if (at_dev_lst_history[ui1_idx].t_dev.t_report_phy_addr.ui2_pa != ui2_pa)
        {
            cecm_unlock_db();
            continue;
        }
        cecm_unlock_db();

        ui4_retry = 0;
        while (ui4_retry < CECM_GET_INFO_MAX_RETRY_TIME)
        {
            cecm_lock_rd_db();
            if (at_dev_lst_history[ui1_idx].t_dev.e_dev_state != CECM_DEV_STATE_UPDATED)
            {
                cecm_unlock_db();
                x_thread_delay(10);
                ui4_retry++;
            }
            else
            {
                if (b_1st_matched)
                {
                    *(CECM_LOG_ADDR_T*)pv_data = at_dev_lst_history[ui1_idx].t_dev.e_la;
                    cecm_unlock_db();
                    break;
                }
                else
                {
                    if (!CECM_EXCEED_MAX(((CECM_LOG_ADD_LST_T*)pv_data)->ui1_num, CECM_LOG_ADDR_MAX))
                    {                        
                        ((CECM_LOG_ADD_LST_T*)pv_data)->ae_la[((CECM_LOG_ADD_LST_T*)pv_data)->ui1_num++] = at_dev_lst_history[ui1_idx].t_dev.e_la;
                        cecm_unlock_db();
                        break;
                    }                        
                }
                cecm_unlock_db();
                break;
            }
        }
    }

    return CECMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_get_pa_by_la
 *
 * Description: This API gets the physical address of a logical address.
 *
 * Inputs:  e_la        A logical address.
 *
 * Outputs: -
 *
 * Returns: It's logical address.
 ----------------------------------------------------------------------------*/
UINT16 cecm_ctrl_get_pa_by_la(CECM_LOG_ADDR_T e_la)
{
    UINT32              ui4_retry = 0;
    UINT16              ui2_pa = CECM_INV_PA;
    CECM_DEV_REC_T*     pt_dev_rec;

    cecm_lock_rd_db();
    if (at_dev_lst_history[e_la].t_dev.e_dev_state == CECM_DEV_STATE_INIT)
    {
        pt_dev_rec = &at_dev_lst[e_la];
    }
    else
    {
        pt_dev_rec = &at_dev_lst_history[e_la];
    }
    cecm_unlock_db();

    while (ui4_retry < CECM_GET_INFO_MAX_RETRY_TIME)
    {
        cecm_lock_rd_db();
        if (pt_dev_rec->t_dev.e_dev_state != CECM_DEV_STATE_UPDATED)
        {
            cecm_unlock_db();
            x_thread_delay(10);
            ui4_retry++;
        }
        else
        {
            ui2_pa = pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa;
            cecm_unlock_db();
            break;
        }
    }

    return (ui2_pa);
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_get_evt_by_la_opcode
 *
 * Description: This API gets an event by logical address and opcode.
 *
 * Inputs:  e_la        A logical address.
 *          e_opcode    A opcode.
 *
 * Outputs: -
 *
 * Returns: The first found event.
 ----------------------------------------------------------------------------*/
CECM_EVT_T* cecm_ctrl_get_evt_by_la_opcode(CECM_LOG_ADDR_T e_la,
                                           CECM_OPCODE_T   e_opcode)
{
    UINT8           ui1_idx;
    CECM_EVT_T*     pt_evt = NULL;
    BOOL            b_found = FALSE;

    cecm_lock_rd_db();

    /* Surf client service list first */
    for (ui1_idx = CECM_SVC_RESERVED + 1; ui1_idx < CECM_MAX_SVC; ui1_idx++)
    {
        if (at_svc_lst[ui1_idx].h_svc == NULL_HANDLE)
        {
            continue;
        }
        else
        {
            DLIST_FOR_EACH(pt_evt, &at_svc_lst[ui1_idx].t_evt_lst, t_dlink)
            {
                if (((pt_evt->e_la == e_la) || (pt_evt->e_la == CECM_LOG_ADDR_UNREGED_BRDCST)) &&
                    ((pt_evt->e_opcode == e_opcode) ||
                     ((e_opcode == CECM_OPCODE_DECK_STS) &&
                      ((pt_evt->e_opcode == CECM_OPCODE_PLAY) ||
                       (pt_evt->e_opcode == CECM_OPCODE_GIVE_DECK_STS) ||
                       (pt_evt->e_opcode == CECM_OPCODE_DECK_CTRL)))))
                {
                    b_found = TRUE;
                    break;
                }
            }

            if (b_found)
            {
                break;
            }
        }
    }

    /* Surf reserved service list */
    if (!b_found)
    {
        if (at_svc_lst[CECM_SVC_RESERVED].h_svc != NULL_HANDLE)
        {
            DLIST_FOR_EACH(pt_evt, &at_svc_lst[CECM_SVC_RESERVED].t_evt_lst, t_dlink)
            {
                if (((pt_evt->e_la == e_la) || (pt_evt->e_la == CECM_LOG_ADDR_UNREGED_BRDCST)) &&
                    ((pt_evt->e_opcode == e_opcode) ||
                     ((e_opcode == CECM_OPCODE_DECK_STS) &&
                      ((pt_evt->e_opcode == CECM_OPCODE_PLAY) ||
                       (pt_evt->e_opcode == CECM_OPCODE_GIVE_DECK_STS) ||
                       (pt_evt->e_opcode == CECM_OPCODE_DECK_CTRL)))))
                {
                    break;
                }
            }
        }
    }

    cecm_unlock_db();

    return (pt_evt);
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_get_evt_by_type_la_opcode
 *
 * Description: This API gets an event by logical address and opcode.
 *
 * Inputs:  e_type      A event type.
 *          e_la        A logical address.
 *          e_opcode    A opcode.
 *
 * Outputs: -
 *
 * Returns: The first found event.
 ----------------------------------------------------------------------------*/
CECM_EVT_T* cecm_ctrl_get_evt_by_type_la_opcode(CECM_EVT_TYPE_T e_type,
                                                CECM_LOG_ADDR_T e_la,
                                                CECM_OPCODE_T   e_opcode)
{
    UINT8           ui1_idx;
    CECM_EVT_T*     pt_evt = NULL;
    BOOL            b_found = FALSE;

    cecm_lock_rd_db();

    /* Surf client service list first */
    for (ui1_idx = CECM_SVC_RESERVED + 1; ui1_idx < CECM_MAX_SVC; ui1_idx++)
    {
        if (at_svc_lst[ui1_idx].h_svc == NULL_HANDLE)
        {
            continue;
        }
        else
        {
            DLIST_FOR_EACH(pt_evt, &at_svc_lst[ui1_idx].t_evt_lst, t_dlink)
            {
                if ((pt_evt->e_type == e_type) &&
                    ((pt_evt->e_la == e_la) || (pt_evt->e_la == CECM_LOG_ADDR_UNREGED_BRDCST)) &&
                    ((pt_evt->e_opcode == e_opcode) ||
                     ((e_opcode == CECM_OPCODE_DECK_STS) &&
                      ((pt_evt->e_opcode == CECM_OPCODE_PLAY) ||
                       (pt_evt->e_opcode == CECM_OPCODE_GIVE_DECK_STS) ||
                       (pt_evt->e_opcode == CECM_OPCODE_DECK_CTRL)))))
                {
                    b_found = TRUE;
                    break;
                }
            }

            if (b_found)
            {
                break;
            }
        }
    }

    /* Surf reserved service list */
    if (!b_found)
    {
        if (at_svc_lst[CECM_SVC_RESERVED].h_svc != NULL_HANDLE)
        {
            DLIST_FOR_EACH(pt_evt, &at_svc_lst[CECM_SVC_RESERVED].t_evt_lst, t_dlink)
            {
                if ((pt_evt->e_type == e_type) &&
                    ((pt_evt->e_la == e_la) || (pt_evt->e_la == CECM_LOG_ADDR_UNREGED_BRDCST)) &&
                    ((pt_evt->e_opcode == e_opcode) ||
                     ((e_opcode == CECM_OPCODE_DECK_STS) &&
                      ((pt_evt->e_opcode == CECM_OPCODE_PLAY) ||
                       (pt_evt->e_opcode == CECM_OPCODE_GIVE_DECK_STS) ||
                       (pt_evt->e_opcode == CECM_OPCODE_DECK_CTRL)))))
                {
                    break;
                }
            }
        }
    }

    cecm_unlock_db();

    return (pt_evt);
}

/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_get_evt_by_type_la
 *
 * Description: This API gets an event by logical address and opcode.
 *
 * Inputs:  e_type      A event type.
 *          e_la        A logical address.
 *          e_opcode    A opcode.
 *
 * Outputs: -
 *
 * Returns: The first found event.
 ----------------------------------------------------------------------------*/
CECM_EVT_T* cecm_ctrl_get_evt_by_type_la(CECM_EVT_TYPE_T e_type,
                                                CECM_LOG_ADDR_T e_la)
{
    UINT8           ui1_idx;
    CECM_EVT_T*     pt_evt = NULL;
    BOOL            b_found = FALSE;

    cecm_lock_rd_db();

    /* Surf client service list first */
    for (ui1_idx = CECM_SVC_RESERVED + 1; ui1_idx < CECM_MAX_SVC; ui1_idx++)
    {
        if (at_svc_lst[ui1_idx].h_svc == NULL_HANDLE)
        {
            continue;
        }
        else
        {
            DLIST_FOR_EACH(pt_evt, &at_svc_lst[ui1_idx].t_evt_lst, t_dlink)
            {
                if ((pt_evt->e_type == e_type) &&
                    ((pt_evt->e_la == e_la) || (pt_evt->e_la == CECM_LOG_ADDR_UNREGED_BRDCST)))
                {
                    b_found = TRUE;
                    break;
                }
            }

            if (b_found)
            {
                break;
            }
        }
    }

    /* Surf reserved service list */
    if (!b_found)
    {
        if (at_svc_lst[CECM_SVC_RESERVED].h_svc != NULL_HANDLE)
        {
            DLIST_FOR_EACH(pt_evt, &at_svc_lst[CECM_SVC_RESERVED].t_evt_lst, t_dlink)
            {
                if ((pt_evt->e_type == e_type) &&
                    ((pt_evt->e_la == e_la) || (pt_evt->e_la == CECM_LOG_ADDR_UNREGED_BRDCST)))
                {
                    break;
                }
            }
        }
    }

    cecm_unlock_db();

    return (pt_evt);
}

/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_get_dev_info_by_la
 *
 * Description: The API gets device info by logical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device Logical address.
 *          pt_ctrl         References the control info.
 *
 * Outputs: pt_dev_info     References the device info.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 ----------------------------------------------------------------------------*/
INT32 cecm_ctrl_get_dev_info_by_la(HANDLE_T        h_cecm_svc,
                                   CECM_LOG_ADDR_T e_la,
                                   CECM_CTRL_T*    pt_ctrl,
                                   CECM_DEV_T*     pt_dev_info)
{
    UINT32              ui4_retry = 0;
    CECM_DEV_REC_T*     pt_dev_rec;

    if (!cecm_ctrl_is_handle_valid(h_cecm_svc))
    {
        return CECMR_INV_ARG;
    }

    cecm_lock_rd_db();
    if (at_dev_lst_history[e_la].t_dev.e_dev_state == CECM_DEV_STATE_INIT)
    {
        pt_dev_rec = &at_dev_lst[e_la];
    }
    else
    {
        pt_dev_rec = &at_dev_lst_history[e_la];
    }
    cecm_unlock_db();

    if (pt_ctrl->b_sync)
    {
        while (ui4_retry < CECM_GET_INFO_MAX_RETRY_TIME)
        {
            cecm_lock_rd_db();
            if (pt_dev_rec->t_dev.e_dev_state != CECM_DEV_STATE_UPDATED)
            {
                cecm_unlock_db();
                x_thread_delay(10);
                ui4_retry++;
            }
            else
            {
                *pt_dev_info = pt_dev_rec->t_dev;
                cecm_unlock_db();
                break;
            }
        }

        if (ui4_retry == CECM_GET_INFO_MAX_RETRY_TIME)
        {
            return CECMR_DEV_NOT_AVIL;
        }
    }
    else
    {
        cecm_lock_rd_db();
        if (pt_dev_rec->t_dev.e_dev_state != CECM_DEV_STATE_UPDATED)
        {
            cecm_unlock_db();
            return CECMR_DEV_NOT_AVIL;
        }
        else
        {
            *pt_dev_info = pt_dev_rec->t_dev;
        }
        cecm_unlock_db();
    }

    return CECMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_get_comp_id_by_la
 *
 * Description: he API gets component ID by logical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device Logical address.
 *
 * Outputs: pui2_comp_id    References the HDMI component ID.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 ----------------------------------------------------------------------------*/
INT32 cecm_ctrl_get_comp_id_by_la(HANDLE_T        h_cecm_svc,
                                  CECM_LOG_ADDR_T e_la,
                                  UINT16*         pui2_comp_id, 
                                  BOOL  b_retry)
{
    UINT32              ui4_retry = 0;
    CECM_DEV_REC_T*     pt_dev_rec;

    if (!cecm_ctrl_is_handle_valid(h_cecm_svc))
    {
        return CECMR_INV_ARG;
    }

    cecm_lock_rd_db();
    if (at_dev_lst_history[e_la].t_dev.e_dev_state == CECM_DEV_STATE_INIT)
    {
        pt_dev_rec = &at_dev_lst[e_la];
    }
    else
    {
        pt_dev_rec = &at_dev_lst_history[e_la];
    }
    cecm_unlock_db();

    if(b_retry)
    {
    while (ui4_retry < CECM_GET_INFO_MAX_RETRY_TIME)
    {
        cecm_lock_rd_db();
        if (pt_dev_rec->t_dev.e_dev_state != CECM_DEV_STATE_UPDATED)
        {
            cecm_unlock_db();
            x_thread_delay(10);
            ui4_retry++;
        }
        else
        {
            if (pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa != CECM_INV_PA)
            {
                *pui2_comp_id = cecm_ctrl_cal_comp_id_from_pa(pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa);
                cecm_unlock_db();
                break;
            }
            else
            {
                cecm_unlock_db();
                return CECMR_DEV_NOT_AVIL;
            }
        }
    }
    }
    else
    {
        cecm_lock_rd_db();

        if (pt_dev_rec->t_dev.e_dev_state != CECM_DEV_STATE_UPDATED)
        {
            cecm_unlock_db();
            return CECMR_DEV_NOT_AVIL;
        }
        else
        {
            if (pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa != CECM_INV_PA)
            {
                *pui2_comp_id = cecm_ctrl_cal_comp_id_from_pa(pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa);
                cecm_unlock_db();
            }
            else
            {
                cecm_unlock_db();
                return CECMR_DEV_NOT_AVIL;
            }
        }
    }
    return CECMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_notify_all_clients
 *
 * Description: This API perfroms a notification to all clients.
 *
 * Inputs:  pt_nfy_data         Referneces the nofiy data.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_notify_all_clients(CECM_NFY_T* pt_nfy_data)
{
    UINT8       ui1_idx;

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return;
    }

    for (ui1_idx = 1; ui1_idx < CECM_MAX_SVC; ui1_idx++)
    {
        if ((at_svc_lst[ui1_idx].h_svc != NULL_HANDLE) &&
            (at_svc_lst[ui1_idx].pf_nfy != NULL))
        {    
            (at_svc_lst[ui1_idx].pf_nfy)(pt_nfy_data, at_svc_lst[ui1_idx].pv_tag, 0);
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_update_act_src
 *
 * Description: This API updates active source of device list and notifies
 *              clients if there is a change occurred.
 *
 * Inputs:  pt_dev_rec      References a device record.
 *          b_act_src       Indicats if the device is active or not.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_update_act_src(CECM_DEV_REC_T* pt_dev_rec,
                              BOOL            b_act_src)
{
    pt_dev_rec->t_dev.b_act_src = b_act_src;
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_is_cec_fct_enable
 *
 * Description: This API checks if CEC function is enabled.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE        CEC function is enabled.
 *          FALSE       CEC funxtion is disabled.
 ----------------------------------------------------------------------------*/
BOOL cecm_ctrl_is_cec_fct_enable(VOID)
{
    return (b_cec_fct_enable);
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_get_internal_handle
 *
 * Description: This API get the internal handle reserved for CEC Manager.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE        Handle is valid.
 *          FALSE       Handle is invalid.
 ----------------------------------------------------------------------------*/
HANDLE_T cecm_ctrl_get_internal_handle(VOID)
{
    return (h_svc_internal);
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_open_svc
 *
 * Description: This API opens a CEC service.
 *
 * Inputs:  pf_nfy          References the caller's notify function.
 *          pv_tag          Private tag value which must be returned in the
 *                          notify function. It can be set to NULL.
 *
 * Outputs: ph_cecm_svc         A handle referencing the opened service.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_OUT_OF_RESOURCE       Not enough CECM resource.
 *          CECMR_OUT_OF_HANDLE         No handle available.
 ----------------------------------------------------------------------------*/
INT32 cecm_ctrl_open_svc(x_cecm_nfy_fct pf_nfy,
                         VOID*          pv_tag,
                         HANDLE_T*      ph_cecm_svc)
{
    INT32       i4_return;
    UINT8       ui1_idx;

    cecm_lock_wr_db();

    /* Search for available CECM service */
    for (ui1_idx = (CECM_SVC_RESERVED + 1); ui1_idx < CECM_MAX_SVC; ui1_idx++)
    {
        if (at_svc_lst[ui1_idx].h_svc == NULL_HANDLE)
        {
            break;
        }
    }

    if (ui1_idx == CECM_MAX_SVC)
    {
        cecm_unlock_db();
        return CECMR_OUT_OF_RESOURCE;
    }
    else
    {
        i4_return = handle_alloc(CECM_HANDLE_TYPE,
                                 (VOID*)&at_svc_lst[ui1_idx],
                                 NULL,
                                 &_cecm_handle_free_nfy_fct,
                                 ph_cecm_svc);

        if (i4_return != HR_OK)
        {
            cecm_unlock_db();
            return CECMR_OUT_OF_HANDLE;
        }
    }

    /* Update service record */
    at_svc_lst[ui1_idx].h_svc = *ph_cecm_svc;
    at_svc_lst[ui1_idx].pf_nfy = pf_nfy;
    at_svc_lst[ui1_idx].pv_tag = pv_tag;

    cecm_unlock_db();

    return CECMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_close_svc
 *
 * Description: This API closes a CEC service.
 *
 * Inputs:  ph_cecm_svc         A handle referencing the service.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_HANDLE_FREE_ERROR     Handle free error.
 ----------------------------------------------------------------------------*/
INT32 cecm_ctrl_close_svc(HANDLE_T h_cecm_svc)
{
    INT32       i4_return;

    if (!cecm_ctrl_is_handle_valid(h_cecm_svc))
    {
        return CECMR_INV_ARG;
    }
    
    /* we have to lock this operation when free handle at close service procedure.*/
    cecm_lock_wr_db();
    i4_return = handle_free(h_cecm_svc, FALSE);
    cecm_unlock_db();

    if (i4_return != HR_OK)
    {
        return CECMR_HANDLE_FREE_ERROR;
    }

    return CECMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_signal_cec_ready
 *
 * Description: The API is used to switch CEC control between CEC driver and
 *              uP.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          b_ready         Indicates the current control right (TRUE => CEC
 *                          driver, FALSE => uP).
 *
 * Outputs:
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error.
 ----------------------------------------------------------------------------*/
INT32 cecm_ctrl_signal_cec_ready(HANDLE_T h_cecm_svc,
                                 BOOL     b_ready)
{
    INT32       i4_return;

    if (!cecm_ctrl_is_handle_valid(h_cecm_svc))
    {
        return CECMR_INV_ARG;
    }

    cecm_lock_wr_db();
    b_uP_ctrl_CEC = !b_ready;
    cecm_unlock_db();

    i4_return = rm_set(h_cecd,
                       FALSE,
                       ANY_PORT_NUM,
                       CEC_SET_TYPE_SIGNAL_CEC_READY,
                       (VOID*)&b_ready,
                       sizeof(BOOL),
                       NULL);

    if (i4_return != RMR_OK)
    {
        return CECMR_DRV_COMP_ERROR;
    }

    return CECMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_set_cec_wakeup_ctrl
 *
 * Description: This API sets uP CEC wakeup control.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          pt_wakeup_ctrl      References the wakeup control.
 *
 * Outputs:
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error.
 ----------------------------------------------------------------------------*/
INT32 cecm_ctrl_set_cec_wakeup_ctrl(HANDLE_T             h_cecm_svc,
                                    CECM_WAKE_UP_CTRL_T* pt_wakeup_ctrl)
{
    INT32               i4_return;
    CEC_WAKE_UP_CTRL_T  t_wakeup_ctrl;

    if (!cecm_ctrl_is_handle_valid(h_cecm_svc))
    {
        return CECMR_INV_ARG;
    }

    cecm_lock_wr_db();
    t_wakeup_ctrl = *(CEC_WAKE_UP_CTRL_T*)pt_wakeup_ctrl;
    cecm_unlock_db();

    cecm_ctrl_set_support_cec_ver(pt_wakeup_ctrl->e_cec_ver);


    i4_return = rm_set(h_cecd,
                       FALSE,
                       ANY_PORT_NUM,
                       CEC_SET_TYPE_WAKE_UP_CTRL,
                       (VOID*)&t_wakeup_ctrl,
                       sizeof(CEC_WAKE_UP_CTRL_T),
                       NULL);

    if (i4_return != RMR_OK)
    {
        return CECMR_DRV_COMP_ERROR;
    }

    return CECMR_OK;
}

INT32 cecm_ctrl_set_arc_ctrl(HANDLE_T                  h_cecm_svc,
                               CECM_ARC_CTRL_T*          pt_arc_ctrl)
{
    INT32               i4_return;
    CECM_ARC_CTRL_T     t_arc_ctrl;

    if (!cecm_ctrl_is_handle_valid(h_cecm_svc))
    {
        return CECMR_INV_ARG;
    }
    cecm_lock_wr_db();
    t_arc_ctrl = *(CECM_ARC_CTRL_T*)pt_arc_ctrl;
    cecm_unlock_db();
    
    i4_return = rm_set(h_cecd,
                       FALSE,
                       ANY_PORT_NUM,
                       CEC_SET_TYPE_ARC_CTRL,
                       (VOID*)&t_arc_ctrl,
                       sizeof(CEC_WAKE_UP_CTRL_T),
                       NULL);

    if (i4_return != RMR_OK)
    {
        return CECMR_DRV_COMP_ERROR;
    }

    return CECMR_OK;

}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_is_act_src_reved
 *
 * Description: This API get if received active source
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          pt_act_src_reved    References the wakeup control.
 *
 * Outputs:
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error.
 ----------------------------------------------------------------------------*/

INT32 cecm_ctrl_is_act_src_reved(CECM_ACT_SRC_REVED_T* pt_act_src_reved)
{
    INT32   i4_return;
    SIZE_T  z_get_info_len;
    
    z_get_info_len = sizeof(CEC_ACT_SRC_REVED_T);  
    i4_return = rm_get(h_cecd,
                       FALSE,
                       ANY_PORT_NUM,
                       CEC_GET_TYPE_IS_ACT_SRC_RECEIVED,
                       (VOID*)pt_act_src_reved,
                       &z_get_info_len,
                       NULL);
 
    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{CECM} Get if active source received error: ret=%d\n\r", i4_return));
    }  
    else
    {
        pt_act_src_reved->ui2_pa = GET_UINT16_FROM_PTR_BIG_END(&pt_act_src_reved->ui2_pa);
        DBG_INFO(("%s/", (pt_act_src_reved->b_act_src_reved) ? "True" : "False"));
        DBG_INFO(("0x%x", pt_act_src_reved->ui2_pa));
    }
     

         
    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_is_set_cec_fct_delay
 *
 * Description: The API  set cec functions delay send
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error.
 *          CECMR_ERROR                 Internal error.
----------------------------------------------------------------------------*/
BOOL cecm_ctrl_is_set_cec_fct_delay(VOID)
{
    return b_set_cec_fct_delay;
}

/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_set_cec_fct_delay
 *
 * Description: The API  set cec functions delay send
 *
 * Inputs:  
 *
 * Outputs: pt_evt  References  information.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_set_cec_fct_delay(CECM_EVT_T* pt_evt)
{
    if (pt_evt != NULL && b_set_cec_fct_delay)
    {
       if (pt_evt->e_opcode == CECM_OPCODE_VNDR_CMD_WITH_ID || pt_evt->e_opcode == CECM_OPCODE_SYS_AUD_MOD_REQ)
        {
        
            CECM_MSG_T      t_msg;
            INT32           i4_return;
            /* Send cec_fct set message */
            t_msg.e_msg_type = CECM_MSG_TYPE_SET_CEC_FCT;
            t_msg.u.t_set_cec_fct.t_params = t_set_cec_fct.t_params;
            t_msg.u.t_set_cec_fct.t_sync_info.t_sync_data.e_sync_op = t_set_cec_fct.t_sync_info.t_sync_data.e_sync_op;
            t_msg.u.t_set_cec_fct.t_sync_info.h_sync = t_set_cec_fct.t_sync_info.h_sync; 
        
            i4_return = x_msg_q_send(*ph_cecm_msgq,
                                 (VOID*)&t_msg,
                                 sizeof(CECM_MSG_T),
                                 CECM_MSGQ_DEFAULT_PRIORITY);
            if(i4_return != OSR_OK)
            {
                t_set_cec_fct.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_FAIL;
                cecm_sync_unlock(t_set_cec_fct.t_sync_info.h_sync);
            }
        
        }    
    }
}
/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_set_cec_fct
 *
 * Description: The API enables/disables CEC functions.
 *
 * Inputs:  pt_set_cec_fct      References cec_set_fct information.
 *
 * Outputs:
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 cecm_ctrl_set_cec_fct(CECM_SET_CEC_FCT_T* pt_set_cec_fct)
{
    INT32       i4_return;

    b_set_cec_fct_delay = FALSE;

    CECM_EVT_T*     pt_evt = NULL;
    if ((!pt_set_cec_fct->t_params.b_enable && ((pt_evt = cecm_ctrl_get_evt_by_la_opcode(CECM_LOG_ADDR_UNREGED_BRDCST, CECM_OPCODE_VNDR_CMD_WITH_ID)) != NULL)) ||
        (!pt_set_cec_fct->t_params.b_enable && ((pt_evt = cecm_ctrl_get_evt_by_la_opcode(CECM_LOG_ADDR_AUD_SYS, CECM_OPCODE_SYS_AUD_MOD_REQ)) != NULL)) ) 
    {
        
        if(pt_evt->e_state == CECM_EVT_STATE_WAIT_FOR_ACK)
        {
            /*Delay disable cec function until vndr cmd with id was sent*/
            t_set_cec_fct = *pt_set_cec_fct;
            b_set_cec_fct_delay = TRUE ;
            DBG_INFO(("{CECM} >>>>>>>>>> b_set_cec_fct_delay\n\r"));  
            
            return CECMR_OK;        
        }
    }

    b_cec_fct_enable = pt_set_cec_fct->t_params.b_enable;
    ui1_hdmi_port_num = pt_set_cec_fct->t_params.ui1_hdmi_port_num;

    /* Stop device discovery */
    cecm_ctrl_stop_dev_discovery();

    /* Stop power_on_device operation */
    cecm_ctrl_select_dev(FALSE, NULL);

    /* Free all using events */
    if (!b_cec_fct_enable)
    {
        if (!cecm_ctrl_evt_free_by_type(CECM_EVT_TYPE_NONE))
        {
            pt_set_cec_fct->t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_FAIL;
            cecm_sync_unlock(pt_set_cec_fct->t_sync_info.h_sync);
            return CECMR_ERROR;
        }
    }
#ifdef CECM_SKIP_INFO_ACQUISITION_WHEN_PRESENT_BEFORE    
    else
    {
        b_post_polling_required = TRUE;
    }
#endif    


    /* Setup global CEC control */
    /* This operation enables/disables CEC function in driver layer */
    i4_return = rm_set(h_cecd,
                       FALSE,
                       ANY_PORT_NUM,
                       CEC_SET_TYPE_GLOBAL_CEC_CTRL,
                       (VOID*)&b_cec_fct_enable,
                       sizeof(BOOL),
                       NULL);

    if (i4_return != RMR_OK)
    {
        return CECMR_DRV_COMP_ERROR;
    }

    if (b_cec_fct_enable)
    {
        DBG_INFO(("\n\r"));
    }
    DBG_INFO(("{CECM} >>>>>>>>>> cec_fct = %s\n\r", (b_cec_fct_enable ? "ON" : "OFF")));

    pt_set_cec_fct->t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_SUCCESS;
    cecm_sync_unlock(pt_set_cec_fct->t_sync_info.h_sync);
    return CECMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_set
 *
 * Description: This API performs CEC set operation.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          pt_set_info     References CECM set information.
 * Outputs:
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 cecm_ctrl_set(HANDLE_T    h_cecm_svc,
                    CECM_SET_T* pt_set_info)
{
    CEC_FRAME_INFO_T        t_frame_info;
    CECM_DEV_REC_T*         pt_dev_rec;
    CECM_EVT_TYPE_T         e_evt_type = CECM_EVT_TYPE_NORMAL;
    CECM_TIMER_ANA_T*       pt_timer_ana;
    CECM_TIMER_EXT_T*       pt_timer_ext;
    CECM_TUNER_ANA_T*       pt_tuner_ana;
    CECM_TUNER_DIG_T*       pt_tuner_dig;
    CECM_ANA_SVC_T*         pt_ana_svc;
    CECM_DIG_SVC_T*         pt_dig_svc;
#if 0
    UINT8                   ui1_idx;
#endif

    if (!cecm_ctrl_is_handle_valid(h_cecm_svc))
    {
        return CECMR_INV_ARG;
    }

    /* Fill in frame message */
    t_frame_info.ui1_init_addr = (UINT8)e_tv_la;
    t_frame_info.ui1_dest_addr = pt_set_info->ui1_dest_addr;
    t_frame_info.ui2_opcode = pt_set_info->ui2_opcode;

    switch (pt_set_info->ui2_opcode)
    {
        case CECM_OPCODE_NONE:
        case CECM_OPCODE_STANDBY:
        case CECM_OPCODE_GET_MENU_LANG:
        case CECM_OPCODE_USER_CTRL_RELEASED:
        case CECM_OPCODE_TUNER_STEP_INCR:
        case CECM_OPCODE_TUNER_STEP_DECR:
        case CECM_OPCODE_REQ_ACT_SRC:
        case CECM_OPCODE_GET_CEC_VER:
        case CECM_OPCODE_GIVE_PHY_ADDR:
        case CECM_OPCODE_GIVE_OSD_NAME:
        case CECM_OPCODE_GIVE_DEV_VNDR_ID:            
        case CECM_OPCODE_GIVE_AUD_STS:
        case CECM_OPCODE_GIVE_SYS_AUD_MOD_STS:
        case CECM_OPCODE_REC_OFF:
        case CECM_OPCODE_VNDR_REMOTE_BTN_UP:
        case CECM_OPCODE_REQ_INIT_ARC:
        case CECM_OPCODE_REQ_TERMINATE_ARC:
        case CECM_OPCODE_REPORT_INIT_ARC:
        case CECM_OPCODE_REPORT_TERMINATE_ARC:
            t_frame_info.aui1_operand[0] = 0;
            t_frame_info.z_operand_size = 0;
            if(pt_set_info->ui4_addi_info == CECM_ADDI_INFO_POLL_LA)
            {
                e_evt_type = CECM_EVT_TYPE_POLL_LA;
            }            
            break;

        case CECM_OPCODE_ACT_SRC:
            t_frame_info.aui1_operand[0] = 0;
            t_frame_info.aui1_operand[1] = 0;
            t_frame_info.z_operand_size = CECM_OPCODE_ACT_SRC_SIZE;
            break;

        case CECM_OPCODE_ROUTING_CHG:

            if(pt_set_info->ui4_addi_info == CECM_ADDI_INFO_ROUTING_CHG)
            {
                t_frame_info.aui1_operand[0] = (UINT8)(pt_set_info->u_operand.t_routing_chg_info.ui2_orig_pa >> 8);
                t_frame_info.aui1_operand[1] = (UINT8)(pt_set_info->u_operand.t_routing_chg_info.ui2_orig_pa);
                t_frame_info.aui1_operand[2] = (UINT8)(pt_set_info->u_operand.t_routing_chg_info.ui2_new_pa >> 8);
                t_frame_info.aui1_operand[3] = (UINT8)(pt_set_info->u_operand.t_routing_chg_info.ui2_new_pa);
                t_frame_info.z_operand_size  = CECM_OPCODE_ROUTING_CHG_SIZE;
                
            }
            else if(pt_set_info->ui4_addi_info == CECM_ADDI_INFO_NONE)
            {       
                if (((pt_set_info->u_operand.t_routing_chg_info.ui2_orig_comp_id >= ui1_hdmi_port_num) &&
                     (pt_set_info->u_operand.t_routing_chg_info.ui2_orig_comp_id != CECM_TV_COMP_ID)) ||
                    ((pt_set_info->u_operand.t_routing_chg_info.ui2_new_comp_id >= ui1_hdmi_port_num) &&
                     (pt_set_info->u_operand.t_routing_chg_info.ui2_new_comp_id != CECM_TV_COMP_ID)))
                {
                    return CECMR_INV_ARG;
                }

                t_frame_info.aui1_operand[0] = (UINT8)(((pt_set_info->u_operand.t_routing_chg_info.ui2_orig_comp_id) + 1) << 4);
                t_frame_info.aui1_operand[1] = 0;
                t_frame_info.aui1_operand[2] = (UINT8)(((pt_set_info->u_operand.t_routing_chg_info.ui2_new_comp_id) + 1) << 4);
                t_frame_info.aui1_operand[3] = 0;
                t_frame_info.z_operand_size = CECM_OPCODE_ROUTING_CHG_SIZE;

            }
            break;

        case CECM_OPCODE_SET_STREAM_PATH:
            if (pt_set_info->ui4_addi_info == CECM_ADDI_INFO_SET_STREAM_PATH_BY_PA)
            {
                t_frame_info.aui1_operand[0] = (UINT8)(pt_set_info->u_operand.ui2_pa >> 8);
                t_frame_info.aui1_operand[1] = (UINT8)pt_set_info->u_operand.ui2_pa;
            }
            else
            {
                if (((pt_dev_rec = cecm_ctrl_get_dev_rec_by_la(pt_set_info->u_operand.e_la, TRUE)) != NULL) &&
                    (pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa != CECM_INV_PA))
                {
                    t_frame_info.aui1_operand[0] = (UINT8)(pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa >> 8);
                    t_frame_info.aui1_operand[1] = (UINT8)pt_dev_rec->t_dev.t_report_phy_addr.ui2_pa;
                }
                else
                {
                    return CECMR_DEV_NOT_AVIL;
                }
            }

            e_evt_type = CECM_EVT_TYPE_SET_STREAM_PATH;
            t_frame_info.z_operand_size = CECM_OPCODE_SET_STREAM_PATH_SIZE;
            break;

        case CECM_OPCODE_REPORT_PHY_ADDR:
            t_frame_info.aui1_operand[0] = (UINT8)(pt_set_info->u_operand.t_report_phy_addr.ui2_pa >> 8);
            t_frame_info.aui1_operand[1] = (UINT8)pt_set_info->u_operand.t_report_phy_addr.ui2_pa;
            t_frame_info.aui1_operand[2] = (UINT8)pt_set_info->u_operand.t_report_phy_addr.e_dev_type;
            t_frame_info.z_operand_size = CECM_OPCODE_REPORT_PHY_ADDR_SIZE;
            break;

        case CECM_OPCODE_SET_MENU_LANG:
            x_memcpy(&t_frame_info.aui1_operand[0], &pt_set_info->u_operand.t_lang, CECM_OPCODE_SET_MENU_LANG_SIZE);
            t_frame_info.z_operand_size = CECM_OPCODE_SET_MENU_LANG_SIZE;
            break;

        case CECM_OPCODE_GIVE_DECK_STS:
        case CECM_OPCODE_GIVE_TUNER_DEV_STS:
            t_frame_info.aui1_operand[0] = (UINT8)(pt_set_info->u_operand.e_sts_req);
            t_frame_info.z_operand_size = CECM_OPCODE_GIVE_DECK_STS_SIZE;
            break;

        case CECM_OPCODE_DECK_CTRL:
            t_frame_info.aui1_operand[0] = (UINT8)(pt_set_info->u_operand.e_deck_ctrl_mod);
            t_frame_info.z_operand_size = CECM_OPCODE_DECK_CTRL_SIZE;
            break;

        case CECM_OPCODE_PLAY:
            t_frame_info.aui1_operand[0] = (UINT8)(pt_set_info->u_operand.e_play_mod);
            t_frame_info.z_operand_size = CECM_OPCODE_PLAY_SIZE;
            break;

        case CECM_OPCODE_MENU_REQ:
            t_frame_info.aui1_operand[0] = (UINT8)(pt_set_info->u_operand.e_menu_req_type);
            t_frame_info.z_operand_size = CECM_OPCODE_MENU_REQ_SIZE;
            break;

        case CECM_OPCODE_DEV_VNDR_ID:
            x_memcpy(&t_frame_info.aui1_operand[0], &pt_set_info->u_operand.t_dev_vndr_id, CECM_OPCODE_DEV_VNDR_ID_SIZE);
            t_frame_info.z_operand_size = CECM_OPCODE_DEV_VNDR_ID_SIZE;
            break;

        case CECM_OPCODE_USER_CTRL_PRESSED:
            if(pt_set_info->ui4_addi_info == CECM_ADDI_INFO_PWR_ON_DEV)
            {
                e_evt_type = CECM_EVT_TYPE_PWR_STS;
            }
            if(pt_set_info->ui4_addi_info == CECM_ADDI_INFO_USER_CTRL)
            {
                if(pt_set_info->u_operand.t_usr_ctrl_with_operand.t_user_ctrl_operand_info.t_user_ctrl_operand_size > CECM_USER_CTRL_OPERAND_SIZE)
                {
                    return CECMR_INV_ARG;
                }                 
                t_frame_info.aui1_operand[0] = (UINT8)(pt_set_info->u_operand.t_usr_ctrl_with_operand.e_user_ctrl);
                x_memcpy(&t_frame_info.aui1_operand[1], 
                         &pt_set_info->u_operand.t_usr_ctrl_with_operand.t_user_ctrl_operand_info.t_user_ctrl_operand, 
                         pt_set_info->u_operand.t_usr_ctrl_with_operand.t_user_ctrl_operand_info.t_user_ctrl_operand_size);
                t_frame_info.z_operand_size = CECM_OPCODE_USER_CTRL_PRESSED_SIZE + 
                                              pt_set_info->u_operand.t_usr_ctrl_with_operand.t_user_ctrl_operand_info.t_user_ctrl_operand_size; 

            }
            else
            {
                t_frame_info.aui1_operand[0] = (UINT8)(pt_set_info->u_operand.e_usr_ctrl);
                t_frame_info.z_operand_size = CECM_OPCODE_USER_CTRL_PRESSED_SIZE;            
            }

            break;

        case CECM_OPCODE_GIVE_DEV_PWR_STS:
            if (pt_set_info->ui4_addi_info == CECM_ADDI_INFO_PWR_ON_DEV)
            {
                e_evt_type = CECM_EVT_TYPE_PWR_STS;
            }

            t_frame_info.aui1_operand[0] = 0;
            t_frame_info.z_operand_size = 0;
            break;

        case CECM_OPCODE_REPORT_PWR_STS:
            t_frame_info.aui1_operand[0] = (UINT8)(pt_set_info->u_operand.e_pwr_sts);
            t_frame_info.z_operand_size = CECM_OPCODE_REPORT_PWR_STS_SIZE;
            break;

        case CECM_OPCODE_SYS_AUD_MOD_REQ:
            if (pt_set_info->u_operand.t_sys_aud_ctrl.e_aud_ctrl == CECM_SYS_AUD_STS_ON)
            {
                if (pt_set_info->u_operand.t_sys_aud_ctrl.ui2_comp_id == CECM_TV_COMP_ID) /* TV */
                {
                    t_frame_info.aui1_operand[0] = 0; /* TV physical address = 0x0000 */
                    t_frame_info.aui1_operand[1] = 0;
                }
                else /* Other devices */
                {
                    t_frame_info.aui1_operand[0] = (UINT8)(pt_set_info->u_operand.t_sys_aud_ctrl.ui2_pa >> 8);
                    t_frame_info.aui1_operand[1] = (UINT8)pt_set_info->u_operand.t_sys_aud_ctrl.ui2_pa;
                }

                t_frame_info.z_operand_size = CECM_OPCODE_SYS_AUD_MOD_REQ_SIZE;
            }
            else if (pt_set_info->u_operand.t_sys_aud_ctrl.e_aud_ctrl == CECM_SYS_AUD_STS_OFF)
            {
                t_frame_info.aui1_operand[0] = 0;
                t_frame_info.z_operand_size = 0;
            }
            else
            {
                return CECMR_INV_ARG;
            }
            break;
            
        case CECM_OPCODE_REQUEST_SAD: 
            if(pt_set_info->u_operand.t_aud_fmt.ui1_aud_fmt_code_num < 1 || pt_set_info->u_operand.t_aud_fmt.ui1_aud_fmt_code_num >4)
            {
                return CECMR_INV_ARG;
            }
            else
            {
                UINT8 ui1_idx;
                t_frame_info.z_operand_size = pt_set_info->u_operand.t_aud_fmt.ui1_aud_fmt_code_num;
                for(ui1_idx = 0; ui1_idx < pt_set_info->u_operand.t_aud_fmt.ui1_aud_fmt_code_num; ui1_idx++)
                {
                    t_frame_info.aui1_operand[ui1_idx] = (UINT8)pt_set_info->u_operand.t_aud_fmt.ae_aud_fmt_code[ui1_idx];
                }                         
            }
         
            
            break;
        case CECM_OPCODE_SET_AUD_RATE:
            t_frame_info.aui1_operand[0] = (UINT8)(pt_set_info->u_operand.e_aud_rate);
            t_frame_info.z_operand_size = CECM_OPCODE_SET_AUD_RATE_SIZE;
            break;

        case CECM_OPCODE_CLR_ANA_TIMER:
        case CECM_OPCODE_SET_ANA_TIMER:
            x_memset((VOID*)&t_frame_info.aui1_operand[0],0,CECM_OPCODE_SET_CLR_ANA_TIMER_SIZE);
            x_memcpy((VOID*)&t_frame_info.aui1_operand[0],
                     (VOID*)&pt_set_info->u_operand.t_timer_info.ui1_day_of_month,
                     sizeof(UINT8));

            pt_timer_ana = &(pt_set_info->u_operand.t_timer_info.u.t_ana_info);
            t_frame_info.aui1_operand[7] = (UINT8)pt_timer_ana->e_ana_brdcst_type;
            t_frame_info.aui1_operand[8] = (UINT8)(pt_timer_ana->ui2_freq >> 8);
            t_frame_info.aui1_operand[9] = (UINT8)pt_timer_ana->ui2_freq;
            t_frame_info.aui1_operand[10] = pt_timer_ana->ui1_brdcst_sys;
            t_frame_info.z_operand_size = CECM_OPCODE_SET_CLR_ANA_TIMER_SIZE;
            break;

        case CECM_OPCODE_CLR_DIG_TIMER:
        case CECM_OPCODE_SET_DIG_TIMER:
            x_memset((VOID*)&t_frame_info.aui1_operand[0],0,CECM_OPCODE_SET_CLR_DIG_TIMER_SIZE);
            x_memcpy((VOID*)&t_frame_info.aui1_operand[0],
                     (VOID*)&pt_set_info->u_operand.t_timer_info.ui1_day_of_month,
                     sizeof(UINT8));
            t_frame_info.z_operand_size = CECM_OPCODE_SET_CLR_DIG_TIMER_SIZE;
            break;

        case CECM_OPCODE_CLR_EXT_TIMER:
        case CECM_OPCODE_SET_EXT_TIMER:
            x_memset((VOID*)&t_frame_info.aui1_operand[0],0,7); // two opcode size are different, just reset pre-7-bytes
            x_memcpy((VOID*)&t_frame_info.aui1_operand[0],
                     (VOID*)&pt_set_info->u_operand.t_timer_info.ui1_day_of_month,
                     sizeof(UINT8));

            pt_timer_ext = &(pt_set_info->u_operand.t_timer_info.u.t_ext_info);
            t_frame_info.aui1_operand[7] = (UINT8)pt_timer_ext->e_src_specfer;

            if (pt_timer_ext->e_src_specfer == CECM_TIMER_EXT_SRC_SPECFER_EXT_PLUG)
            {
                t_frame_info.aui1_operand[8] = (UINT8)pt_timer_ext->ui2_phy_addr_or_plug;
                t_frame_info.z_operand_size = CECM_OPCODE_SET_CLR_EXT_TIMER_PLUG_SIZE;
            }
            else if (pt_timer_ext->e_src_specfer == CECM_TIMER_EXT_SRC_SPECFER_EXT_PHY_ADDR)
            {
                t_frame_info.aui1_operand[8] = (UINT8)(pt_timer_ext->ui2_phy_addr_or_plug >> 8);
                t_frame_info.aui1_operand[9] = (UINT8)pt_timer_ext->ui2_phy_addr_or_plug;
                t_frame_info.z_operand_size = CECM_OPCODE_SET_CLR_EXT_TIMER_PHY_ADDR_SIZE;
            }
            else
            {
                return CECMR_INV_ARG;
            }
            break;

        case CECM_OPCODE_SET_TIMER_PROG_TITLE:
            x_uc_w2s_to_ps((const UTF16_T*)&pt_set_info->u_operand.t_prog_title_str,
                           (CHAR*)t_frame_info.aui1_operand,
                           CECM_MAX_OPERAND_SIZE);
            t_frame_info.z_operand_size = x_uc_w2s_strlen((const UTF16_T*)&pt_set_info->u_operand.t_prog_title_str);
            break;

        case CECM_OPCODE_SELECT_ANA_SVC:
            pt_tuner_ana = &(pt_set_info->u_operand.t_tuner_svc_info.u.t_ana_info);
            t_frame_info.aui1_operand[0] = (UINT8)pt_tuner_ana->e_ana_brdcst_type;
            t_frame_info.aui1_operand[1] = (UINT8)(pt_tuner_ana->ui2_freq >> 8);
            t_frame_info.aui1_operand[2] = (UINT8)pt_tuner_ana->ui2_freq;
            t_frame_info.aui1_operand[3] = pt_tuner_ana->ui1_brdcst_sys;
            t_frame_info.z_operand_size = CECM_OPCODE_SELECT_ANA_SVC_SIZE;
            break;

        case CECM_OPCODE_SELECT_DIG_SVC:
            pt_tuner_dig = &(pt_set_info->u_operand.t_tuner_svc_info.u.t_dig_info);
            t_frame_info.aui1_operand[0] = ((UINT8)(pt_tuner_dig->e_dig_id_method << 7)) | ((UINT8)pt_tuner_dig->e_dig_brdcst_sys);
            x_memcpy((VOID*)&t_frame_info.aui1_operand[1],
                     (VOID*)&pt_tuner_dig->aui1_svc_id[0],
                     6);
            t_frame_info.z_operand_size = CECM_OPCODE_SELECT_DIG_SVC_SIZE;
            break;

        case CECM_OPCODE_REC_ON:
            t_frame_info.aui1_operand[0] = (UINT8)pt_set_info->u_operand.t_rec_src.e_rec_src_type;

            switch(pt_set_info->u_operand.t_rec_src.e_rec_src_type)
            {
                case CECM_REC_SRC_TYPE_OWN_SRC:
                    t_frame_info.z_operand_size = CECM_OPCODE_REC_ON_OWN_SRC_SIZE;
                    break;

                case CECM_REC_SRC_TYPE_DIG_SVC:
                    pt_dig_svc = &(pt_set_info->u_operand.t_rec_src.u.t_dig_svc);
                    t_frame_info.aui1_operand[1] = ((UINT8)(pt_dig_svc->e_dig_id_method << 7)) | ((UINT8)pt_dig_svc->e_dig_brdcst_sys);
                    x_memcpy((VOID*)&t_frame_info.aui1_operand[2],
                             (VOID*)&pt_dig_svc->aui1_svc_id[0],
                             6);
                    t_frame_info.z_operand_size = CECM_OPCODE_REC_ON_DIG_SVC_SIZE;
                    break;

                case CECM_REC_SRC_TYPE_ANA_SVC:
                    pt_ana_svc = &(pt_set_info->u_operand.t_rec_src.u.t_ana_svc);
                    t_frame_info.aui1_operand[1] = (UINT8)pt_ana_svc->e_ana_brdcst_type;
                    t_frame_info.aui1_operand[2] = (UINT8)(pt_ana_svc->ui2_freq >> 8);
                    t_frame_info.aui1_operand[3] = (UINT8)pt_ana_svc->ui2_freq;
                    t_frame_info.aui1_operand[4] = pt_ana_svc->ui1_brdcst_sys;
                    t_frame_info.z_operand_size = CECM_OPCODE_REC_ON_ANA_SVC_SIZE;
                    break;

                case CECM_REC_SRC_TYPE_EXT_PLUG:
                    t_frame_info.aui1_operand[1] = pt_set_info->u_operand.t_rec_src.u.ui1_plug;
                    t_frame_info.z_operand_size = CECM_OPCODE_REC_ON_EXT_PLUG_SIZE;
                    break;

                case CECM_REC_SRC_TYPE_EXT_PHY_ADDR:
                    t_frame_info.aui1_operand[1] = (UINT8)(pt_set_info->u_operand.t_rec_src.u.ui2_phy_addr >> 8);
                    t_frame_info.aui1_operand[2] = (UINT8)(pt_set_info->u_operand.t_rec_src.u.ui2_phy_addr);
                    t_frame_info.z_operand_size = CECM_OPCODE_REC_ON_EXT_PHY_ADDR_SIZE;
                    break;

                default:
                    return CECMR_INV_ARG;
            }
            break;

        case CECM_OPCODE_FEATURE_ABORT:
            t_frame_info.aui1_operand[0] = (UINT8)(pt_set_info->u_operand.t_feature_abort.e_opcode);
            t_frame_info.aui1_operand[1] = (UINT8)(pt_set_info->u_operand.t_feature_abort.e_abort_reason);
            t_frame_info.z_operand_size = CECM_OPCODE_FEATURE_ABORT_SIZE;
            break;

        case CECM_OPCODE_VNDR_CMD:
            x_memcpy((VOID*)&t_frame_info.aui1_operand[0],
                     (VOID*)&pt_set_info->u_operand.t_vndr_cmd_data.t_vndr_cmd,
                     pt_set_info->u_operand.t_vndr_cmd_data.z_vndr_cmd_size);
            t_frame_info.z_operand_size = pt_set_info->u_operand.t_vndr_cmd_data.z_vndr_cmd_size;
            break;

        case CECM_OPCODE_VNDR_CMD_WITH_ID:
            x_memcpy((VOID*)&t_frame_info.aui1_operand[0],
                     (VOID*)&pt_set_info->u_operand.t_vndr_cmd_with_id_data.t_dev_vndr_id,
                     CECM_DEV_VNDR_ID_SIZE);
            x_memcpy((VOID*)&t_frame_info.aui1_operand[3],
                     (VOID*)&pt_set_info->u_operand.t_vndr_cmd_with_id_data.t_vndr_cmd_with_id,
                     pt_set_info->u_operand.t_vndr_cmd_with_id_data.z_vndr_cmd_with_id_size);
            t_frame_info.z_operand_size = CECM_DEV_VNDR_ID_SIZE + pt_set_info->u_operand.t_vndr_cmd_with_id_data.z_vndr_cmd_with_id_size;
            break;

        case CECM_OPCODE_VNDR_REMOTE_BTN_DOWN:
            x_memcpy((VOID*)&t_frame_info.aui1_operand[0],
                     (VOID*)&pt_set_info->u_operand.t_vndr_rc_code_data.t_vndr_rc_code,
                     pt_set_info->u_operand.t_vndr_rc_code_data.z_vndr_rc_code_size);
            t_frame_info.z_operand_size = pt_set_info->u_operand.t_vndr_rc_code_data.z_vndr_rc_code_size;
            break;

        case CECM_OPCODE_SET_OSD_NAME:
            x_uc_w2s_to_ps((const UTF16_T*)&pt_set_info->u_operand.aw2_osd_name[0],
                           (CHAR*)t_frame_info.aui1_operand,
                           CECM_MAX_OPERAND_SIZE);
            t_frame_info.z_operand_size = x_uc_w2s_strlen((const UTF16_T*)&pt_set_info->u_operand.aw2_osd_name[0]);
            break;

        case CECM_OPCODE_DECK_STS:

        /* The following opcodes are commanded by CECM and not exported */
        case CECM_OPCODE_CEC_VER:
        case CECM_OPCODE_ABORT:
            return CECMR_OK;

        default:
            return CECMR_INV_ARG;
    }

    if (!cecm_ctrl_send_msg(h_cecm_svc, e_evt_type, &t_frame_info, &pt_set_info->t_sync_info))
    {
        return CECMR_MSG_SEND_ERROR;
    }

#if 0
    /* Disable all active source indicators in device list */
    if (pt_set_info->ui2_opcode == CECM_OPCODE_ACT_SRC)
    {
        for (ui1_idx = (UINT8)CECM_LOG_ADDR_REC_DEV_1; ui1_idx < (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST; ui1_idx++)
        {
            cecm_lock_wr_db();
            cecm_ctrl_update_act_src(&at_dev_lst[ui1_idx], FALSE);
            cecm_unlock_db();
        }
    }
#endif

    return CECMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_show_dev_list
 *
 * Description: This API shows the current status of device list.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_show_dev_list(VOID)
{
    UINT8       ui1_idx;
    UINT8       ui1_vndr_info_idx;
    CHAR        ac_osd_name[CECM_OSD_NAME_SIZE];
    CECM_DEV_DISCOVERY_CONFIG_T   t_dd_config;

    cecm_psr_get_dev_discovery_config(&t_dd_config);

    for (ui1_idx = (UINT8)CECM_LOG_ADDR_REC_DEV_1; ui1_idx < (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST; ui1_idx++)
    {
        x_dbg_stmt("===================================\n\r");
        x_dbg_stmt("dev %d ", ui1_idx);
        if (at_dev_lst[ui1_idx].t_dev.e_dev_state == CECM_DEV_STATE_INIT)
        {
            x_dbg_stmt("[%s] ", "INIT");
        }
        else if (at_dev_lst[ui1_idx].t_dev.e_dev_state == CECM_DEV_STATE_UPDATING)
        {
            x_dbg_stmt("[%s] ", "UPDATING");
        }
        else if (at_dev_lst[ui1_idx].t_dev.e_dev_state == CECM_DEV_STATE_UPDATED)
        {
            x_dbg_stmt("[%s] ", "UPDATED");
        }
        else
        {
            x_dbg_stmt("[%s] ", "UNKNOWN");
        }

        x_dbg_stmt("%s", (at_dev_lst[ui1_idx].t_dev.b_dev_present) ? "P" : "  ");
        x_dbg_stmt("%s", (at_dev_lst[ui1_idx].t_dev.b_act_src) ? "A" : "  ");
        x_dbg_stmt("%s", (at_dev_lst[ui1_idx].e_post_polling_runner == CECM_PP_RUNNER_DEV) ? "X" : "  ");
        x_dbg_stmt("\n\r");

        x_dbg_stmt("===================================\n\r");

        if (at_dev_lst[ui1_idx].t_dev.b_dev_present)
        {
            x_dbg_stmt("update_mask = 0x%x\n\r", at_dev_lst[ui1_idx].ui4_update_mask);

            /* Physical address */
            if (at_dev_lst[ui1_idx].e_abort_reason_pa == CECM_ABORT_REASON_NONE)
            {
                x_dbg_stmt("phy_addr    = 0x%x (dev_type = %d)\n\r",
                           at_dev_lst[ui1_idx].t_dev.t_report_phy_addr.ui2_pa,
                           at_dev_lst[ui1_idx].t_dev.t_report_phy_addr.e_dev_type);
            }
            else
            {
                x_dbg_stmt("phy_addr    = FEATURE_ABORT (%d) \n\r", at_dev_lst[ui1_idx].e_abort_reason_pa);
            }

            /* Logical address */
            x_dbg_stmt("la          = %d\n\r", at_dev_lst[ui1_idx].t_dev.e_la);
            
            if (at_dev_lst[ui1_idx].e_abort_reason_cev_ver == CECM_ABORT_REASON_NONE)
            {
                x_dbg_stmt("CEC ver     = 0x%x \n\r",at_dev_lst[ui1_idx].t_dev.e_cec_ver);
            }
            else
            {
                x_dbg_stmt("CEC ver     = FEATURE_ABORT (%d) \n\r", at_dev_lst[ui1_idx].e_abort_reason_cev_ver);
            }         
            if (at_dev_lst[ui1_idx].e_abort_reason_pwr_sts == CECM_ABORT_REASON_NONE)
            {
                x_dbg_stmt("pwr_sts     = 0x%x \n\r",at_dev_lst[ui1_idx].t_dev.e_pwr_sts);
            }
            else
            {
                x_dbg_stmt("Pwr sts     = FEATURE_ABORT (%d) \n\r", at_dev_lst[ui1_idx].e_abort_reason_pwr_sts);
            }           
            /* OSD name */
            x_memset(&ac_osd_name[0], 0, CECM_OSD_NAME_SIZE);
            x_uc_w2s_to_ps((const UTF16_T*)&(at_dev_lst[ui1_idx].t_dev.aw2_osd_name)[0],
                           &ac_osd_name[0],
                           x_uc_w2s_strlen((const UTF16_T*)&(at_dev_lst[ui1_idx].t_dev.aw2_osd_name)[0]));

            if (at_dev_lst[ui1_idx].e_abort_reason_osd_name == CECM_ABORT_REASON_NONE)
            {
                if (ac_osd_name[0] == 0)
                {
                    x_dbg_stmt("osd_name    = %s\n\r", "----");
                }
                else
                {
                    x_dbg_stmt("osd_name    = %s\n\r", ac_osd_name);
                }
            }
            else
            {
                x_dbg_stmt("osd_name    = FEATURE_ABORT %d \n\r", at_dev_lst[ui1_idx].e_abort_reason_osd_name);
            }

            /* Device vendor ID */
            if (at_dev_lst[ui1_idx].e_abort_reason_dev_vndr_id == CECM_ABORT_REASON_NONE)
            {
                if ((at_dev_lst[ui1_idx].t_dev.t_dev_vndr_id[0] == 0) &&
                    (at_dev_lst[ui1_idx].t_dev.t_dev_vndr_id[1] == 0) &&
                    (at_dev_lst[ui1_idx].t_dev.t_dev_vndr_id[2] == 0))
                {
                    x_dbg_stmt("vndr_id     = UNKNOWN\n\r");
                }
                else
                {
                    x_dbg_stmt("vndr_id     = 0x%02x%02x%02x\n\r", at_dev_lst[ui1_idx].t_dev.t_dev_vndr_id[0],
                                                                       at_dev_lst[ui1_idx].t_dev.t_dev_vndr_id[1],
                                                                       at_dev_lst[ui1_idx].t_dev.t_dev_vndr_id[2]);
                }
            }
            else
            {
                x_dbg_stmt("vndr_id     = FEATURE_ABORT %d \n\r", at_dev_lst[ui1_idx].e_abort_reason_dev_vndr_id);
            }
            
            /*Device vendor info*/
            if(t_dd_config.t_dd_get_info.b_dev_vndr_info == TRUE)
            {
                /* for each vndr info */
                for(ui1_vndr_info_idx = 0; 
                    ui1_vndr_info_idx < (UINT8)CECM_DD_VNDR_CMD_MAX_SIZE && ui1_vndr_info_idx < (UINT8)t_dd_config.t_dd_vndr_cmd.z_dd_vndr_cmd_size ; 
                    ui1_vndr_info_idx++)
                {
                    /*  feature abort reason none */
                    if(at_dev_lst[ui1_idx].ae_abort_reason_vndr_info[ui1_vndr_info_idx] == CECM_ABORT_REASON_NONE)
                    {
                        /* vndr cmd*/
                        if(((UINT8)at_dev_lst[ui1_idx].t_dev.t_dev_vndr_info.at_vndr_cmd_info[ui1_vndr_info_idx].u_data.t_vndr_cmd_data.z_vndr_cmd_size > 0) &&
                           (at_dev_lst[ui1_idx].t_dev.t_dev_vndr_info.at_vndr_cmd_info[ui1_vndr_info_idx].e_vndr_cmd_type == CECM_VNDR_CMD))
                        {
                            UINT8 ui1_vndr_cmd_data_idx;
                            x_dbg_stmt("vndr_info   = 0x");
                            /* for each vndr cmd  */
                            for(ui1_vndr_cmd_data_idx = 0; 
                                ui1_vndr_cmd_data_idx < (UINT8)CECM_MAX_OPERAND_SIZE && ui1_vndr_cmd_data_idx < (UINT8)at_dev_lst[ui1_idx].t_dev.t_dev_vndr_info.at_vndr_cmd_info[0].u_data.t_vndr_cmd_data.z_vndr_cmd_size;
                                ui1_vndr_cmd_data_idx++)
                                {
                                    x_dbg_stmt("%02x", at_dev_lst[ui1_idx].t_dev.t_dev_vndr_info.at_vndr_cmd_info[ui1_vndr_info_idx].u_data.t_vndr_cmd_data.t_vndr_cmd[ui1_vndr_cmd_data_idx]); 
                                }
                             x_dbg_stmt("\n\r");   
                        }
                        /* vndr cmd with id */
                        else if(((UINT8)at_dev_lst[ui1_idx].t_dev.t_dev_vndr_info.at_vndr_cmd_info[ui1_vndr_info_idx].u_data.t_vndr_cmd_with_id_data.z_vndr_cmd_with_id_size > 0) &&
                                (at_dev_lst[ui1_idx].t_dev.t_dev_vndr_info.at_vndr_cmd_info[ui1_vndr_info_idx].e_vndr_cmd_type == CECM_VNDR_CMD_WITH_ID))
                        {
                            UINT8 ui1_vndr_cmd_data_idx;
                            x_dbg_stmt("vndr_info   = 0x");
                            /*vndr id */
                            x_dbg_stmt("%02x%02x%02x", at_dev_lst[ui1_idx].t_dev.t_dev_vndr_info.at_vndr_cmd_info[ui1_vndr_info_idx].u_data.t_vndr_cmd_with_id_data.t_dev_vndr_id[0],
                                                       at_dev_lst[ui1_idx].t_dev.t_dev_vndr_info.at_vndr_cmd_info[ui1_vndr_info_idx].u_data.t_vndr_cmd_with_id_data.t_dev_vndr_id[1],
                                                       at_dev_lst[ui1_idx].t_dev.t_dev_vndr_info.at_vndr_cmd_info[ui1_vndr_info_idx].u_data.t_vndr_cmd_with_id_data.t_dev_vndr_id[2]); 
                            /* for each vndr cmd */
                            for(ui1_vndr_cmd_data_idx = 0; 
                                ui1_vndr_cmd_data_idx < (UINT8)CECM_VNDR_CMD_WITH_ID_SIZE && ui1_vndr_cmd_data_idx < (UINT8)at_dev_lst[ui1_idx].t_dev.t_dev_vndr_info.at_vndr_cmd_info[0].u_data.t_vndr_cmd_with_id_data.z_vndr_cmd_with_id_size;
                                ui1_vndr_cmd_data_idx++)
                                {
                                    x_dbg_stmt("%02x", at_dev_lst[ui1_idx].t_dev.t_dev_vndr_info.at_vndr_cmd_info[ui1_vndr_info_idx].u_data.t_vndr_cmd_with_id_data.t_vndr_cmd_with_id[ui1_vndr_cmd_data_idx]); 
                                }
                             x_dbg_stmt("\n\r");  
                        }
                        else
                        {
                            x_dbg_stmt("vndr_info   = UNKNOWN\n\r");
                        }
                        
                    }
                    else
                    {
                        x_dbg_stmt("vndr_info    = FEATURE_ABORT %d \n\r", at_dev_lst[ui1_idx].ae_abort_reason_vndr_info[ui1_vndr_info_idx]);
                    }
                }            
            }
            

            
            
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_show_svc_list
 *
 * Description: This API shows the current status of source list.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_show_svc_list(VOID)
{
    UINT8           ui1_svc_idx;
    UINT8           ui1_evt_idx;
    CECM_EVT_T*     pt_evt = NULL;
    BOOL            b_avil_evt;

    for (ui1_svc_idx = CECM_SVC_RESERVED; ui1_svc_idx < CECM_MAX_SVC; ui1_svc_idx++)
    {
        if (at_svc_lst[ui1_svc_idx].h_svc == NULL_HANDLE)
        {
            continue;
        }

        x_dbg_stmt("svc_id = %d, h_svc = 0x%x\n\r", ui1_svc_idx, at_svc_lst[ui1_svc_idx].h_svc);
        x_dbg_stmt("============================\n\r");

        b_avil_evt = FALSE;
        ui1_evt_idx = 0;
        DLIST_FOR_EACH(pt_evt, &at_svc_lst[ui1_svc_idx].t_evt_lst, t_dlink)
        {
            b_avil_evt = TRUE;
            x_dbg_stmt("[%d]--------------\n\r", ui1_evt_idx++);
            switch (pt_evt->e_type)
            {
                case CECM_EVT_TYPE_NONE:
                    x_dbg_stmt("evt_type  = %s\n\r", "CECM_EVT_TYPE_NONE");
                    break;

                case CECM_EVT_TYPE_NORMAL:
                    x_dbg_stmt("evt_type  = %s\n\r", "CECM_EVT_TYPE_NORMAL");
                    break;

                case CECM_EVT_TYPE_DEV_DISCOVERY:
                    x_dbg_stmt("evt_type  = %s\n\r", "CECM_EVT_TYPE_DEV_DISCOVERY");
                    break;

                case CECM_EVT_TYPE_ONE_TOUCH_PLAY:
                    x_dbg_stmt("evt_type  = %s\n\r", "CECM_EVT_TYPE_ONE_TOUCH_PLAY");
                    break;

                case CECM_EVT_TYPE_SET_STREAM_PATH:
                    x_dbg_stmt("evt_type  = %s\n\r", "CECM_EVT_TYPE_SET_STREAM_PATH");
                    break;

                case CECM_EVT_TYPE_PWR_STS:
                    x_dbg_stmt("evt_type  = %s\n\r", "CECM_EVT_TYPE_PWR_STS");
                    break;

                default:
                    x_dbg_stmt("evt_type  = %s\n\r", "KNOWN");
            }

            switch (pt_evt->e_state)
            {
                case CECM_EVT_STATE_NONE:
                    x_dbg_stmt("evt_state = %s\n\r", "CECM_EVT_STATE_NONE");
                    break;

                case CECM_EVT_STATE_WAIT_FOR_ACK:
                    x_dbg_stmt("evt_state = %s\n\r", "CECM_EVT_STATE_WAIT_FOR_ACK");
                    break;

                case CECM_EVT_STATE_WAIT_FOR_FRAME:
                    x_dbg_stmt("evt_state = %s\n\r", "CECM_EVT_STATE_WAIT_FOR_FRAME");
                    break;

                default:
                    x_dbg_stmt("evt_state = %s\n\r", "KNOWN");
            }

            x_dbg_stmt("la        = %d\n\r", pt_evt->e_la);
            x_dbg_stmt("opcode    = 0x%x\n\r\n\r", pt_evt->e_opcode);
        }

        if (!b_avil_evt)
        {
            x_dbg_stmt("No event is running!\n\r\n\r");
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_query_cec_fct
 *
 * Description: This API shows the current status of CEC function control.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_query_cec_fct(VOID)
{
    x_dbg_stmt("CEC control status\n\r");
    x_dbg_stmt("===========================\n\r");
    x_dbg_stmt("CEC function: %s\n\r", (cecm_ctrl_is_cec_fct_enable() ? "On" : "Off"));
    x_dbg_stmt("Is CEC controlled by uP: %s\n\r", (b_uP_ctrl_CEC ? "Yes" : "Off"));
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_show_dd_setting
 *
 * Description: This API shows the current device discovery setting.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_show_dd_setting(VOID)
{
    x_dbg_stmt("CECM device discovery setting\n\r");
    x_dbg_stmt("===============================\n\r");
    x_dbg_stmt("start_la: %d\n\r", t_dd_info.t_discvry_info.e_start_la);
    x_dbg_stmt("stop_la: %d\n\r", t_dd_info.t_discvry_info.e_stop_la);
    x_dbg_stmt("itvl: %d\n\r", t_dd_info.t_discvry_info.ui4_itvl);
    x_dbg_stmt("batch_polling: %d\n\r", t_dd_info.t_discvry_info.b_batch_polling);
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_thread
 *
 * Description: This is CECM control thread for handling CEC related
 *              activities.
 *
 * Inputs: pv_arg       Input argument.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cecm_ctrl_thread(VOID* pv_arg)
{
    INT32           i4_return;
    CECM_MSG_T      t_msg;
    SIZE_T          z_size;
    UINT16          ui2_idx;

    /* Check argument */
    if (pv_arg == NULL)
    {
        dbg_abort(DBG_MOD_CEC_MNGR | DBG_CAT_THREAD);
    }

    /* CEC Manager message queue handle */
    ph_cecm_msgq = (HANDLE_T*)pv_arg;

    while (1)
    {
        z_size = sizeof(CECM_MSG_T);
        i4_return = x_msg_q_receive(&ui2_idx,
                                    &t_msg,
                                    &z_size,
                                    ph_cecm_msgq,
                                    1,
                                    X_MSGQ_OPTION_WAIT);

        if (i4_return == OSR_OK)
        {
            cecm_psr_process_message(&t_msg);
        }
    } /* while */
}


/*-----------------------------------------------------------------------------
 * Name: cecm_ctrl_init
 *
 * Description: This API initializes the CEC Manager control module.
 *
 * Inputs:  pf_cec_nfy      References a CEC notify function.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_ERROR                 Internal error.
 *          CECMR_OUT_OF_HANDLE         No handle available.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error.
 ----------------------------------------------------------------------------*/
INT32 cecm_ctrl_init(x_cec_nfy_fct pf_cec_nfy)
{
    UINT8           ui1_idx;
    CECM_EVT_T*     pt_evt;

    cecm_lock_wr_db();

    /* Initialize device list */
    for (ui1_idx = (UINT8)CECM_LOG_ADDR_REC_DEV_1; ui1_idx < (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST; ui1_idx++)
    {
        at_dev_lst[ui1_idx].ui4_update_mask = CECM_UPDATE_MASK_NONE;
        at_dev_lst[ui1_idx].ui1_polling_retry = 0;
        at_dev_lst[ui1_idx].e_post_polling_runner = CECM_PP_RUNNER_TV;
        at_dev_lst[ui1_idx].b_post_polling_handled = FALSE;
        at_dev_lst[ui1_idx].b_post_polling_retry = FALSE;
        at_dev_lst[ui1_idx].t_dev.e_dev_state = CECM_DEV_STATE_INIT;
        at_dev_lst[ui1_idx].t_dev.b_dev_present = FALSE;
        at_dev_lst[ui1_idx].t_dev.b_act_src = FALSE;
        at_dev_lst[ui1_idx].t_dev.t_report_phy_addr.ui2_pa = CECM_INV_PA;
        at_dev_lst[ui1_idx].t_dev.t_report_phy_addr.e_dev_type = CECM_DEV_TYPE_NONE;
        at_dev_lst[ui1_idx].t_dev.e_la = (CECM_LOG_ADDR_T)ui1_idx;
        x_memset(&at_dev_lst[ui1_idx].t_dev.t_dev_vndr_id, 0, CECM_DEV_VNDR_ID_SIZE);
        x_memset(&(at_dev_lst[ui1_idx].t_dev.t_dev_vndr_info), 0, sizeof(CECM_DEV_VNDR_INFO_T));            
        x_memset(&(at_dev_lst[ui1_idx].t_dev.aw2_osd_name)[0], 0, CECM_OSD_NAME_SIZE);
        at_dev_lst[ui1_idx].t_dev.e_cec_ver = CECM_CEC_VER_INVALID;
        at_dev_lst[ui1_idx].t_dev.e_pwr_sts = CECM_PWR_STS_ERROR;
        at_dev_lst[ui1_idx].t_dev.e_menu_req_state = CECM_MENU_REQ_STATE_ERR;
        at_dev_lst[ui1_idx].e_abort_reason_pa = CECM_ABORT_REASON_NONE;
        at_dev_lst[ui1_idx].e_abort_reason_dev_vndr_id = CECM_ABORT_REASON_NONE;
        at_dev_lst[ui1_idx].e_abort_reason_osd_name = CECM_ABORT_REASON_NONE;
        at_dev_lst[ui1_idx].e_abort_reason_pwr_sts = CECM_ABORT_REASON_NONE;
        at_dev_lst[ui1_idx].e_abort_reason_cev_ver = CECM_ABORT_REASON_NONE;
        x_memset(&(at_dev_lst[ui1_idx].ae_abort_reason_vndr_info[0]),CECM_ABORT_REASON_NONE, CECM_DD_VNDR_CMD_MAX_SIZE);   
        at_dev_lst[ui1_idx].ui1_vndr_cmd_idx = 0;
        at_dev_lst[ui1_idx].ui4_skip_update_mask = CECM_UPDATE_MASK_NONE;
        at_dev_lst[ui1_idx].b_phy_addr_updated = FALSE;
        at_dev_lst[ui1_idx].t_msg.e_msg_type = CECM_MSG_TYPE_NONE;

        at_dev_lst_history[ui1_idx].ui4_update_mask = CECM_UPDATE_MASK_NONE;
        at_dev_lst_history[ui1_idx].ui1_polling_retry = 0;
        at_dev_lst_history[ui1_idx].e_post_polling_runner = CECM_PP_RUNNER_TV;
        at_dev_lst_history[ui1_idx].b_post_polling_handled = FALSE;
        at_dev_lst_history[ui1_idx].b_post_polling_retry = FALSE;
        at_dev_lst_history[ui1_idx].t_dev.e_dev_state = CECM_DEV_STATE_INIT;
        at_dev_lst_history[ui1_idx].t_dev.b_dev_present = FALSE;
        at_dev_lst_history[ui1_idx].t_dev.b_act_src = FALSE;
        at_dev_lst_history[ui1_idx].t_dev.t_report_phy_addr.ui2_pa = CECM_INV_PA;
        at_dev_lst_history[ui1_idx].t_dev.t_report_phy_addr.e_dev_type = CECM_DEV_TYPE_NONE;
        at_dev_lst_history[ui1_idx].t_dev.e_la = (CECM_LOG_ADDR_T)ui1_idx;
        x_memset(&at_dev_lst_history[ui1_idx].t_dev.t_dev_vndr_id, 0, CECM_DEV_VNDR_ID_SIZE);
        x_memset(&(at_dev_lst_history[ui1_idx].t_dev.t_dev_vndr_info), 0, sizeof(CECM_DEV_VNDR_INFO_T));         
        x_memset(&(at_dev_lst_history[ui1_idx].t_dev.aw2_osd_name)[0], 0, CECM_OSD_NAME_SIZE);
        at_dev_lst[ui1_idx].t_dev.e_cec_ver = CECM_CEC_VER_INVALID;
        at_dev_lst_history[ui1_idx].t_dev.e_pwr_sts = CECM_PWR_STS_ERROR;
        at_dev_lst_history[ui1_idx].t_dev.e_menu_req_state = CECM_MENU_REQ_STATE_ERR;
        at_dev_lst_history[ui1_idx].e_abort_reason_pa = CECM_ABORT_REASON_NONE;
        at_dev_lst_history[ui1_idx].e_abort_reason_dev_vndr_id = CECM_ABORT_REASON_NONE;
        at_dev_lst_history[ui1_idx].e_abort_reason_osd_name = CECM_ABORT_REASON_NONE;
        at_dev_lst_history[ui1_idx].e_abort_reason_pwr_sts = CECM_ABORT_REASON_NONE;
        at_dev_lst_history[ui1_idx].e_abort_reason_cev_ver = CECM_ABORT_REASON_NONE; 
        x_memset(&(at_dev_lst_history[ui1_idx].ae_abort_reason_vndr_info[0]),CECM_ABORT_REASON_NONE, CECM_DD_VNDR_CMD_MAX_SIZE);     
        at_dev_lst_history[ui1_idx].ui1_vndr_cmd_idx = 0;
        at_dev_lst_history[ui1_idx].ui4_skip_update_mask = CECM_UPDATE_MASK_NONE;
        at_dev_lst_history[ui1_idx].b_phy_addr_updated = FALSE;
        at_dev_lst_history[ui1_idx].t_msg.e_msg_type = CECM_MSG_TYPE_NONE;
        
    }

    /* Initialize service list */
    for (ui1_idx = 0; ui1_idx < CECM_MAX_SVC; ui1_idx++)
    {
        at_svc_lst[ui1_idx].h_svc = NULL_HANDLE;
        at_svc_lst[ui1_idx].pf_nfy = NULL;
        at_svc_lst[ui1_idx].pv_tag = NULL;
        DLIST_INIT(&at_svc_lst[ui1_idx].t_evt_lst);
    }

    /* Initialize device discovery info */
    t_dd_info.t_discvry_info.e_start_la = CECM_LOG_ADDR_REC_DEV_1;
    t_dd_info.t_discvry_info.e_stop_la = CECM_LOG_ADDR_REC_DEV_1;
    t_dd_info.t_discvry_info.ui4_itvl= 0;
    t_dd_info.t_discvry_info.b_batch_polling = 0;



    /* Initialize free event pool */
    DLIST_INIT(&t_free_evt_pool);

    for (ui1_idx = 0; ui1_idx < CECM_MAX_EVT; ui1_idx++)
    {
        if ((pt_evt = (CECM_EVT_T*)(x_mem_alloc(sizeof(CECM_EVT_T)))) != NULL)
        {            
            pt_evt->e_type = CECM_EVT_TYPE_NONE;
            pt_evt->e_state = CECM_EVT_STATE_NONE;
            pt_evt->e_la = CECM_LOG_ADDR_MAX;
            pt_evt->e_opcode = CECM_OPCODE_NONE;
            pt_evt->pt_svc = NULL;
            pt_evt->h_sync = NULL_HANDLE;
            pt_evt->pt_sync_data = NULL;
        }
        else
        {
            cecm_unlock_db();
            x_mem_free((VOID*) pt_evt);
            return CECMR_ERROR;
        }                        

        if (x_timer_create(&pt_evt->h_timer) != OSR_OK)
        {
            cecm_unlock_db();
            x_mem_free((VOID*) pt_evt);
            return CECMR_ERROR;
        }

        DLIST_INSERT_TAIL(pt_evt, &t_free_evt_pool, t_dlink);
    }

    /* Initialize CECM reserved service for internal use */
    if (handle_alloc(CECM_HANDLE_TYPE,
                     (VOID*)&at_svc_lst[CECM_SVC_RESERVED],
                     NULL,
                     &_cecm_handle_free_nfy_fct,
                     &h_svc_internal) != HR_OK)
    {
        cecm_unlock_db();
        return CECMR_OUT_OF_HANDLE;
    }

    at_svc_lst[CECM_SVC_RESERVED].h_svc = h_svc_internal;
    at_svc_lst[CECM_SVC_RESERVED].pf_nfy = NULL;
    at_svc_lst[CECM_SVC_RESERVED].pv_tag = NULL;

    /* Create timers */
    /* For device discovery control */
    if (x_timer_create(&h_timer_dev_discovery) != OSR_OK)
    {
        cecm_unlock_db();
        return CECMR_ERROR;
    }
    
    if (x_timer_create(&h_timer_polling_tv_la) != OSR_OK)
    {
        cecm_unlock_db();
        return CECMR_ERROR;
    }
    /* For select_device operation */
    if (x_timer_create(&h_timer_select_dev) != OSR_OK)
    {
        cecm_unlock_db();
        return CECMR_ERROR;
    }

    t_select_dev_info.h_cecm_svc = NULL_HANDLE;
    t_select_dev_info.e_sent_la = CECM_LOG_ADDR_MAX;
    t_select_dev_info.e_sent_opcode = CECM_OPCODE_NONE;

    cecm_unlock_db();

    return (_cecm_open_cec_drv(pf_cec_nfy));
}

