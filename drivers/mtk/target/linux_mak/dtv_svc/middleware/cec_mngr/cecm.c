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
 * $RCSfile: cecm.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/14 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: a872122d22ab4b982b54b29e95d0ff55 $
 *
 * Description:
 *         This is CEC Manager API source file.
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
#include "res_mngr/rm.h"
#include "res_mngr/x_rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_cec.h"
#include "unicode/x_uc_str.h"
#include "cec_mngr/cecm_lock.h"
#include "cec_mngr/cecm_ctrl.h"
#include "cec_mngr/cecm_psr.h"
#include "cec_mngr/cecm_cli.h"
#include "cec_mngr/cecm.h"
#include "cec_mngr/x_cecm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static BOOL                 b_cecm_init = FALSE;
static HANDLE_T             h_cecm_thread = NULL_HANDLE;
static HANDLE_T             h_cecm_msgq = NULL_HANDLE;
static UINT16               ui2_cecm_dbg_level;
static CECM_LOG_ADDR_T      e_max_tv_la = CECM_LOG_ADDR_UNREGED_BRDCST;
static THREAD_DESCR_T       t_cecm_thread_descr = {DEFAULT_STACK_SIZE,
                                                   DEFAULT_PRIORITY,
                                                   DEFAULT_NUM_MSGS};

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
static VOID _cecm_cec_nfy_fct(CEC_NFY_T* pt_nfy_data,
                              VOID*      pv_nfy_tag,
                              UINT32	 ui4_data);

static INT32 _cecm_set_sync(HANDLE_T    h_cecm_svc,
                            CECM_SET_T* pt_set_info);

static INT32 _cecm_chk_args(HANDLE_T        h_cecm_svc,
                            CECM_LOG_ADDR_T e_la);
                            
/*-----------------------------------------------------------------------------
 * Name: _cecm_cec_nfy_fct
 *
 * Description: This notify function is called by the CEC driver in response
 *              to some frame messages or ACK conditions.
 *
 * Inputs:  pt_nfy_data         Notification data.
 *          pv_nfy_tag          Private tag value.
 *          ui4_data            Additional data (0 or ui4_tid of corresponding
 *                              frame message that was sent previously).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _cecm_cec_nfy_fct(CEC_NFY_T* pt_nfy_data,
                              VOID*      pv_nfy_tag,
                              UINT32	 ui4_data)
{
    CECM_EVT_T*     pt_evt;
    CECM_MSG_T      t_msg;

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return;
    }

    switch (pt_nfy_data->e_nfy_type)
    {
        case CEC_NFY_TYPE_FRAME:
            if ((pt_nfy_data->u.t_frame_info.ui1_dest_addr != CEC_LOG_ADDR_TV) &&
                (pt_nfy_data->u.t_frame_info.ui1_dest_addr != CEC_LOG_ADDR_UNREGED_BRDCST))
            {
                return;
            }
            break;

        case CEC_NFY_TYPE_ACK:
            /* Check if the event is valid */
            pt_evt = (CECM_EVT_T*)(pt_nfy_data->u.t_ack_info.pv_tag);
            if ((pt_evt == NULL) || (pt_evt->e_type == CECM_EVT_TYPE_NONE))
            {
                return;
            }
            break;
        /*Check if plug or unplug device*/
        case CEC_NFY_TYPE_5V_POWER:
            if(pt_nfy_data->u.t_5v_power_info.ui2_comp_id > cecm_ctrl_get_hdmi_port_num())
            {
                
                return;
            }
            
    }

    t_msg.e_msg_type = CECM_MSG_TYPE_CEC_NFY;
    t_msg.u.t_cec_nfy = *pt_nfy_data;

    x_msg_q_send(h_cecm_msgq,
                 (VOID*)&t_msg,
                 sizeof(CECM_MSG_T),
                 CECM_MSGQ_DEFAULT_PRIORITY);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_set_sync
 *
 * Description: This API performs synchronous CEC set operation.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          pt_set_info     References CECM set information.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Operation error.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_set_sync(HANDLE_T    h_cecm_svc,
                            CECM_SET_T* pt_set_info)
{
    INT32           i4_return;

    /* Create sync semaphore */
    i4_return = x_sema_create(&(pt_set_info->t_sync_info.h_sync),
                              X_SEMA_TYPE_BINARY,
                              X_SEMA_STATE_LOCK);

    if (i4_return != OSR_OK)
    {
        return CECMR_SEMA_ERROR;
    }

    /* Perform set operation */
    i4_return = cecm_ctrl_set(h_cecm_svc, pt_set_info);

    if (i4_return == CECMR_OK)
    {
        if (cecm_sync_lock(pt_set_info->t_sync_info.h_sync) != TRUE)
        {
            x_sema_delete(pt_set_info->t_sync_info.h_sync);
            return CECMR_SEMA_ERROR;
        }

        if (pt_set_info->t_sync_info.t_sync_data.e_sync_op == CECM_SYNC_OP_FAIL)
        {
            x_sema_delete(pt_set_info->t_sync_info.h_sync);
            return CECMR_ERROR;
        }
    }
    else
    {
        x_sema_delete(pt_set_info->t_sync_info.h_sync);
        return CECMR_ERROR;
    }

    /* Delete sync semaphore */
    i4_return = x_sema_delete(pt_set_info->t_sync_info.h_sync);

    if (i4_return != OSR_OK)
    {
        return CECMR_SEMA_ERROR;
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: _cecm_chk_args
 *
 * Description: This API checks common arguments.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 ----------------------------------------------------------------------------*/
static INT32 _cecm_chk_args(HANDLE_T        h_cecm_svc,
                            CECM_LOG_ADDR_T e_la)
{
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return CECMR_CEC_FCT_DISABLED;
    }

    if (h_cecm_svc == NULL_HANDLE)
    {
        return CECMR_INV_ARG;
    }

    if (e_la >= CECM_LOG_ADDR_MAX)
    {
        return CECMR_INV_ARG;
    }
    
    return CECMR_OK;
}
        
    
/*-----------------------------------------------------------------------------
 * Name: cecm_sync_lock
 *
 * Description: This API locks the sync operation semaphore.
 *
 * Inputs:  h_sync      References the sync operation semaphore.
 *
 * Outputs: -
 *
 * Returns: TRUE        Successful.
 *          FALSE       Failed.
 ----------------------------------------------------------------------------*/
BOOL cecm_sync_lock(HANDLE_T h_sync)
{
    return ((x_sema_lock(h_sync, X_SEMA_OPTION_WAIT) == OSR_OK) ? TRUE : FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: cecm_sync_unlock
 *
 * Description: This API unlocks the sync operation semaphore.
 *
 * Inputs:  h_sync      References the sync operation semaphore.
 *
 * Outputs: -
 *
 * Returns: TRUE        Successful.
 *          FALSE       Failed.
 ----------------------------------------------------------------------------*/
BOOL cecm_sync_unlock(HANDLE_T h_sync)
{
    return ((x_sema_unlock(h_sync) == OSR_OK) ? TRUE : FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: cecm_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
UINT16 cecm_get_dbg_level(VOID)
{
    if (b_cecm_init)
    {
        return ui2_cecm_dbg_level;
    }
    else
    {
        return 0;
    }
}


/*-----------------------------------------------------------------------------
 * Name: cecm_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs:  ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 *          FALSE           Routine failed.
 ----------------------------------------------------------------------------*/
BOOL cecm_set_dbg_level(UINT16 ui2_level)
{
    if (b_cecm_init)
    {
        ui2_cecm_dbg_level = ui2_level;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_init
 *
 * Description: This API initializes the CEC Manager.
 *
 * Inputs:  pt_cecm_thread_descr        References a thread descriptor
 *                                      structure.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_ALREADY_INIT          The CEC Manager has already been
 *                                      initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_ERROR                 Internal error.
 *          CECMR_OUT_OF_HANDLE         No handle available.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error.
 *          CECMR_CLI_ERROR             CLI operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_init(THREAD_DESCR_T* pt_cecm_thread_descr)
{
    INT32           i4_return;

    /* Check arguments */
    if (b_cecm_init)
    {
        return CECMR_ALREADY_INIT;
    }

    if (pt_cecm_thread_descr == NULL)
    {
        return CECMR_INV_ARG;
    }

    /* Initialize CEC Manager debug level */
    ui2_cecm_dbg_level = DBG_INIT_LEVEL_MW_CEC_MNGR;

    /* Initialize CEC Manager thread descriptor structure */
    if (pt_cecm_thread_descr == NULL)
    {
        t_cecm_thread_descr.ui1_priority = CECM_THREAD_DEFAULT_PRIORITY;
        t_cecm_thread_descr.ui2_num_msgs = CECM_NUM_OF_MSGS;
        t_cecm_thread_descr.z_stack_size = CECM_THREAD_DEFAULT_STACK_SIZE;
    }
    else
    {
        t_cecm_thread_descr = (*pt_cecm_thread_descr);

        /* Convert default thread values to actual ones if needed */
        if (t_cecm_thread_descr.ui1_priority == DEFAULT_PRIORITY)
        {
            t_cecm_thread_descr.ui1_priority = CECM_THREAD_DEFAULT_PRIORITY;
        }

        if (t_cecm_thread_descr.ui2_num_msgs == DEFAULT_NUM_MSGS)
        {
            t_cecm_thread_descr.ui2_num_msgs = CECM_NUM_OF_MSGS;
        }

        if (t_cecm_thread_descr.z_stack_size == DEFAULT_STACK_SIZE)
        {
            t_cecm_thread_descr.z_stack_size = CECM_THREAD_DEFAULT_STACK_SIZE;
        }
    }

    /* Initialize CECM lock module */
    cecm_lock_init();

    /* Initialize CECM control module */
    i4_return = cecm_ctrl_init(_cecm_cec_nfy_fct);

    if (i4_return != CECMR_OK)
    {
        DBG_ERROR(("{CECM} ERR: cecm_ctrl_init() failed\n\r"));
        dbg_abort(DBG_MOD_CEC_MNGR | DBG_CAT_INIT);
    }

    /* Create CECM message queue */
    i4_return = x_msg_q_create(&h_cecm_msgq,
                               CECM_MSGQ_NAME,
                               sizeof(CECM_MSG_T),
                               t_cecm_thread_descr.ui2_num_msgs);

    if (i4_return != OSR_OK)
    {
        DBG_ERROR(("{CECM} ERR: x_msg_q_create() failed\n\r"));
        dbg_abort(DBG_MOD_CEC_MNGR | DBG_CAT_MESSAGE);
    }

    /* Initialize CECM parser module */
    i4_return = cecm_psr_init(&h_cecm_msgq);

    if (i4_return != CECMR_OK)
    {
        DBG_ERROR(("{CECM} ERR: cecm_psr_init() failed\n\r"));
        dbg_abort(DBG_MOD_CEC_MNGR | DBG_CAT_INIT);
    }

    /* Create CEC Manager thread */
    i4_return = x_thread_create(&h_cecm_thread,
                                CECM_THREAD_NAME,
                                t_cecm_thread_descr.z_stack_size,
                                t_cecm_thread_descr.ui1_priority,
                                cecm_ctrl_thread,
                                sizeof(HANDLE_T),
                                (VOID*)&h_cecm_msgq);

    if (i4_return != OSR_OK)
    {
        if (x_msg_q_delete(h_cecm_msgq) != OSR_OK)
        {
            DBG_ERROR(("{CECM} ERR: x_msg_q_delete() failed\n\r"));
            dbg_abort(DBG_MOD_CEC_MNGR | DBG_CAT_MESSAGE);
        }

        DBG_ERROR(("{CECM} ERR: x_thread_create() failed\n\r"));
        dbg_abort(DBG_MOD_CEC_MNGR | DBG_CAT_THREAD);
    }

    /* Initialize CEC Manager CLI component */
    i4_return = cecm_cli_init();

    if (i4_return != CECMR_OK)
    {
        return CECMR_CLI_ERROR;
    }

    b_cecm_init = TRUE;

    return CECMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_open_svc
 *
 * Description: This API is used to open a CECM service.
 *
 * Inputs:  pf_nfy          References the caller's notify function.
 *          pv_tag          Private tag value which must be returned in the
 *                          notify function. It can be set to NULL.
 *
 * Outputs: ph_cecm_svc     A handle referencing the opened service.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_OUT_OF_RESOURCE       Not enough CECM resource.
 *          CECMR_OUT_OF_HANDLE         No handle available.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_open_svc(x_cecm_nfy_fct pf_nfy,
                      VOID*          pv_tag,
                      HANDLE_T*      ph_cecm_svc)
{
    DBG_API(("{CECM} x_cecm_open_svc\n\r"));

    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if ((pf_nfy == NULL) || (ph_cecm_svc == NULL))
    {
        return CECMR_INV_ARG;
    }

    return (cecm_ctrl_open_svc(pf_nfy,
                               pv_tag,
                               ph_cecm_svc));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_close_svc
 *
 * Description: This API is used to close a CECM service.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_HANDLE_FREE_ERROR     Handle free error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_close_svc(HANDLE_T h_cecm_svc)
{
    DBG_API(("{CECM} x_cecm_close_svc\n\r"));

    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if (h_cecm_svc == NULL_HANDLE)
    {
        return CECMR_INV_ARG;
    }

    return (cecm_ctrl_close_svc(h_cecm_svc));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_signal_cec_ready
 *
 * Description: The API is used to switch CEC control between CEC driver and
 *              uP.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          b_ready         Indicates the current control right (TRUE => CEC
 *                          driver, FALSE => uP).
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_signal_cec_ready(HANDLE_T h_cecm_svc,
                              BOOL     b_ready)
{
    DBG_API(("{CECM} x_cecm_signal_cec_ready: ready=%d\n\r", b_ready));
    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if (h_cecm_svc == NULL_HANDLE)
    {
        return CECMR_INV_ARG;
    }

    return (cecm_ctrl_signal_cec_ready(h_cecm_svc,
                                       b_ready));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_cec_wakeup_ctrl
 *
 * Description: This API sets uP CEC wakeup control.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          pt_wakeup_ctrl      Wake-up control info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_cec_wakeup_ctrl(HANDLE_T             h_cecm_svc,
                                 CECM_WAKE_UP_CTRL_T* pt_wakeup_ctrl)
{
    DBG_API(("{CECM} x_cecm_set_cec_wakeup_ctrl\n\r"));

    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return CECMR_CEC_FCT_DISABLED;
    }

    if ((h_cecm_svc == NULL_HANDLE) || (pt_wakeup_ctrl == NULL))
    {
        return CECMR_INV_ARG;
    }

    return (cecm_ctrl_set_cec_wakeup_ctrl(h_cecm_svc,
                                          pt_wakeup_ctrl));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_cec_fct
 *
 * Description: The API enables/disables CEC functions.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          t_params        Contains cec_set_fct parameters.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_SEMA_ERROR            Semaphore operation error.
 *          CECMR_DRV_COMP_ERROR        CEC driver operation error.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_cec_fct(HANDLE_T                  h_cecm_svc,
                         CECM_SET_CEC_FCT_PARAMS_T t_params)
{
    INT32           i4_return = CECMR_OK;
    CECM_MSG_T      t_msg;

    DBG_API(("{CECM} x_cecm_set_cec_fct: enable=%d, hdmi_port_num=%d\n\r",
            t_params.b_enable,
            t_params.ui1_hdmi_port_num));

    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if ((h_cecm_svc == NULL_HANDLE) ||
        !cecm_ctrl_is_handle_valid(h_cecm_svc))
    {
        return CECMR_INV_ARG;
    }

    /* Create sync semaphore */
    i4_return = x_sema_create(&t_msg.u.t_set_cec_fct.t_sync_info.h_sync,
                              X_SEMA_TYPE_BINARY,
                              X_SEMA_STATE_LOCK);

    if (i4_return != OSR_OK)
    {
        return CECMR_SEMA_ERROR;
    }

    /* Send cec_fct set message */
    t_msg.e_msg_type = CECM_MSG_TYPE_SET_CEC_FCT;
    t_msg.u.t_set_cec_fct.t_params = t_params;
    t_msg.u.t_set_cec_fct.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    if (x_msg_q_send(h_cecm_msgq,
                     (VOID*)&t_msg,
                     sizeof(CECM_MSG_T),
                     CECM_MSGQ_DEFAULT_PRIORITY) == OSR_OK)
    {
        if (cecm_sync_lock(t_msg.u.t_set_cec_fct.t_sync_info.h_sync) != TRUE)
        {
            x_sema_delete(t_msg.u.t_set_cec_fct.t_sync_info.h_sync);
            return CECMR_SEMA_ERROR;
        }

        if (t_msg.u.t_set_cec_fct.t_sync_info.t_sync_data.e_sync_op == CECM_SYNC_OP_FAIL)
        {
            x_sema_delete(t_msg.u.t_set_cec_fct.t_sync_info.h_sync);
            return CECMR_ERROR;
        }
    }
    else
    {
        x_sema_delete(t_msg.u.t_set_cec_fct.t_sync_info.h_sync);
        return CECMR_MSG_SEND_ERROR;
    }

    /* Delete sync semaphore */
    i4_return = x_sema_delete(t_msg.u.t_set_cec_fct.t_sync_info.h_sync);

    if (i4_return != OSR_OK)
    {
        return CECMR_SEMA_ERROR;
    }

    return (i4_return);
}

INT32 x_cecm_is_act_src_reved(HANDLE_T h_cecm_svc , CECM_ACT_SRC_REVED_T* pt_act_src_reved)
{
    INT32           i4_return = CECMR_OK;


    DBG_API(("{CECM} x_cecm_is_act_src_reved: \n\r"));

    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if ((h_cecm_svc == NULL_HANDLE) || (pt_act_src_reved == NULL))
    {
        return CECMR_INV_ARG;
    }  
    
    i4_return = cecm_ctrl_is_act_src_reved(pt_act_src_reved);

    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: x_cecm_get_la_by_pa
 *
 * Description: This API gets the first logical address in the list which is  
 *              mapped to a physical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          ui2_pa          A physical address.
 *
 * Outputs: -
 *
 * Returns: CECM_LOG_ADDR_MAX   Failed.
 *          Other               It's logical address.
 ----------------------------------------------------------------------------*/
CECM_LOG_ADDR_T x_cecm_get_la_by_pa(HANDLE_T h_cecm_svc,
                                    UINT16   ui2_pa)
{
    CECM_LOG_ADDR_T     e_la = CECM_LOG_ADDR_MAX;
    
    DBG_API(("{CECM} cecm_ctrl_get_la_by_pa: pa=0x%x\n\r", ui2_pa));

    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECM_LOG_ADDR_MAX;
    }

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return CECM_LOG_ADDR_MAX;
    }

    if ((h_cecm_svc == NULL_HANDLE) ||
        !cecm_ctrl_is_handle_valid(h_cecm_svc))
    {
        return CECM_LOG_ADDR_MAX;
    }

    cecm_ctrl_get_la_by_pa(ui2_pa, 
                           TRUE, 
                           (VOID*)&e_la);
    
    return (e_la);
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_get_la_by_pa_ex
 *
 * Description: This API gets the logical address(es) which is(are) mapped to 
 *              a physical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          ui2_pa          A physical address.
 *          pt_la_lst       References a logical address list.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK            Routine successful.
 *          CECMR_INV_ARG       One or more of the arguments contain invalid
 *                              data.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_get_la_by_pa_ex(HANDLE_T            h_cecm_svc,
                             UINT16              ui2_pa,
                             CECM_LOG_ADD_LST_T* pt_la_lst)
{
    DBG_API(("{CECM} x_cecm_get_la_by_pa_ex: pa=0x%x\n\r", ui2_pa));

    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return CECMR_CEC_FCT_DISABLED;
    }

    if ((h_cecm_svc == NULL_HANDLE) ||
        !cecm_ctrl_is_handle_valid(h_cecm_svc))
    {
        return CECMR_INV_ARG;
    }

    return (cecm_ctrl_get_la_by_pa(ui2_pa, 
                                   FALSE, 
                                   (VOID*)pt_la_lst));
    
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_get_pa_by_la
 *
 * Description: This API gets the physical address of a logical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECM_LOG_ADDR_MAX   Failed.
 *          Other               It's logical address.
 ----------------------------------------------------------------------------*/
UINT16 x_cecm_get_pa_by_la(HANDLE_T        h_cecm_svc,
                           CECM_LOG_ADDR_T e_la)
{
    DBG_API(("{CECM} x_cecm_get_la_by_pa: la=%d\n\r", e_la));

    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECM_INV_PA;
    }

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return CECM_INV_PA;
    }

    if ((h_cecm_svc == NULL_HANDLE) ||
        !cecm_ctrl_is_handle_valid(h_cecm_svc))
    {
        return CECM_INV_PA;
    }

    return (cecm_ctrl_get_pa_by_la(e_la));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_get_dev_info_by_la
 *
 * Description: The API gets device info by logical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          pt_ctrl         References the control info.
 *
 * Outputs: pt_dev_info     Referecnes device info.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_get_dev_info_by_la(HANDLE_T        h_cecm_svc,
                                CECM_LOG_ADDR_T e_la,
                                CECM_CTRL_T*    pt_ctrl,
                                CECM_DEV_T*     pt_dev_info)
{
    DBG_API(("{CECM} x_cecm_get_dev_info_by_la: la=%d\n\r", e_la));

    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return CECMR_CEC_FCT_DISABLED;
    }

    if ((h_cecm_svc == NULL_HANDLE) ||
        (pt_ctrl == NULL) ||
        (pt_dev_info == NULL))
    {
        return CECMR_INV_ARG;
    }

    if (e_la >= CECM_LOG_ADDR_MAX)
    {
        return CECMR_INV_ARG;
    }

    return (cecm_ctrl_get_dev_info_by_la(h_cecm_svc,
                                         e_la,
                                         pt_ctrl,
                                         pt_dev_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_get_comp_id_by_la
 *
 * Description: The API gets component ID by logical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: pui2_comp_id    HDMI component ID.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_get_comp_id_by_la(HANDLE_T        h_cecm_svc,
                               CECM_LOG_ADDR_T e_la,
                               UINT16*         pui2_comp_id)
{
    DBG_API(("{CECM} x_cecm_get_comp_id: la=%d\n\r", e_la));

    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return CECMR_CEC_FCT_DISABLED;
    }

    if ((h_cecm_svc == NULL_HANDLE) ||
        (pui2_comp_id == NULL))
    {
        return CECMR_INV_ARG;
    }

    if (e_la >= CECM_LOG_ADDR_MAX)
    {
        return CECMR_INV_ARG;
    }

    return (cecm_ctrl_get_comp_id_by_la(h_cecm_svc,
                                        e_la,
                                        pui2_comp_id,
                                        TRUE));
}

/*-----------------------------------------------------------------------------
 * Name: x_cecm_get_comp_id_by_la_ex
 *
 * Description: The API gets component ID by logical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          b_retry         Do retry if device list not avliable
 * Outputs: pui2_comp_id    HDMI component ID.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_get_comp_id_by_la_ex(HANDLE_T        h_cecm_svc,
                                          CECM_LOG_ADDR_T e_la,
                                          UINT16*         pui2_comp_id,
                                          BOOL b_retry)
{
    DBG_API(("{CECM} x_cecm_get_comp_id: la=%d\n\r", e_la));

    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return CECMR_CEC_FCT_DISABLED;
    }

    if ((h_cecm_svc == NULL_HANDLE) ||
        (pui2_comp_id == NULL))
    {
        return CECMR_INV_ARG;
    }

    if (e_la >= CECM_LOG_ADDR_MAX)
    {
        return CECMR_INV_ARG;
    }

    return (cecm_ctrl_get_comp_id_by_la(h_cecm_svc,
                                        e_la,
                                        pui2_comp_id,
                                        b_retry));      

}

/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_max_tv_la
 *
 * Description: The API gets set the max TV la
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            Max TV la.
 *
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 ----------------------------------------------------------------------------*/

INT32 x_cecm_set_max_tv_la(HANDLE_T        h_cecm_svc,
                          CECM_LOG_ADDR_T   e_la)
{   
    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return CECMR_CEC_FCT_DISABLED;
    }    

    e_max_tv_la = e_la;
    
    return CECMR_OK;
}

INT32 x_cecm_set_support_cec_ver(HANDLE_T        h_cecm_svc,
                              CECM_CEC_VER_T   e_cec_ver)
{
    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    cecm_ctrl_set_support_cec_ver(e_cec_ver);
    
    return CECMR_OK;    
}
/*-----------------------------------------------------------------------------
 * Name: x_cecm_find_tv_la_alloc
 *
 * Description: The API finds the TV logical address allocation.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *
 * Outputs: -
 *
 * Returns: CECM_LOG_ADDR_MAX   Failed.
 *          Other               It's logical address.
 ----------------------------------------------------------------------------*/
CECM_LOG_ADDR_T x_cecm_find_tv_la_alloc(HANDLE_T h_cecm_svc)
{
    UINT8                   b_retry;
    BOOL                    b_la_obtained = FALSE;
    CECM_LOG_ADDR_T         e_la;
    CECM_POLLING_INFO_T     t_polling_info = {0};
   
    DBG_API(("{CECM} x_cecm_find_tv_la_alloc\n\r"));

    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECM_LOG_ADDR_MAX;
    }

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return CECM_LOG_ADDR_MAX;
    }

    if (h_cecm_svc == NULL_HANDLE)
    {
        return CECM_LOG_ADDR_MAX;
    }

    /* Ping TV device */
    b_retry = 0;
    e_la = CECM_LOG_ADDR_TV;
    while (b_retry++ < CECM_MAX_POLLING_LA_RETRY)
    {
        if (x_cecm_set_polling_message(h_cecm_svc, e_la, TRUE, &t_polling_info) == CECMR_OK)
        {
            if (!t_polling_info.b_ack_ok)
            {
                b_la_obtained = TRUE;
                
            }
            break;
        }
        else
        {
            /*ping LA fail , retry*/
            continue;
        }
    }
    DBG_INFO(("{CECM}t_polling_info.b_ack_ok = %d \n",t_polling_info.b_ack_ok));

    if(CECM_MAX_POLLING_LA_RETRY == b_retry)
    {
        DBG_INFO(("{CECM}set obtained= TRUE!!!\n"));
        b_la_obtained = TRUE;
    }

    /* Ping free use device */
    if (!b_la_obtained)
    {
        b_retry = 0;
        e_la = CECM_LOG_ADDR_FREE_USE;
        while (b_retry++ < CECM_MAX_POLLING_LA_RETRY)
        {
            if (x_cecm_set_polling_message(h_cecm_svc, e_la, TRUE, &t_polling_info) == CECMR_OK)
            {
                if (!t_polling_info.b_ack_ok)
                {
                    b_la_obtained = TRUE;
                }
                break;
            }
            else
            {
                /*ping LA fail , retry*/
                continue;
            }
        }
        DBG_INFO(("{CECM}FREE_USE t_polling_info.b_ack_ok = %d \n",t_polling_info.b_ack_ok));
    }

    if (b_la_obtained)
    {        
        cecm_ctrl_set_tv_la(e_la);    
        return (e_la);
    }
    else
    {
        cecm_ctrl_set_tv_la(e_max_tv_la);
        return e_max_tv_la;
    }
}

/*-----------------------------------------------------------------------------
 * Name: x_cecm_find_tv_la_alloc_ex
 *
 * Description: The API finds the TV logical address allocation.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *
 * Outputs: -
 *
 * Returns: CECM_LOG_ADDR_MAX   Failed.
 *          Other               It's logical address.
 ----------------------------------------------------------------------------*/
CECM_LOG_ADDR_T x_cecm_find_tv_la_alloc_ex(HANDLE_T h_cecm_svc, UINT32 ui4_itvl)
{
    CECM_LOG_ADDR_T e_la;

    DBG_API(("{CECM} x_cecm_find_tv_la_alloc_ex \n\r"));
    e_la = x_cecm_find_tv_la_alloc(h_cecm_svc);
    
    if(e_la != CECM_LOG_ADDR_TV)
    {
        cecm_ctrl_set_polling_tv_la_itvl(ui4_itvl);

        /*b_polling set as false means dont polling la immediate
          but start a timer for polling tv la */
        cecm_ctrl_handle_tv_la(NULL, TRUE);
    }
    
    return e_la;
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_start_dev_discovery
 *
 * Description: The API starts CEC device discovery.
 *
 * Inputs:  h_cecm_svc                  A handle referencing the service.
 *          ui4_dev_discovery_itvl      Device discovery interval in second.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 ----------------------------------------------------------------------------*/

INT32 x_cecm_config_dev_discovery(HANDLE_T        h_cecm_svc,
                                         CECM_DEV_DISCOVERY_CONFIG_T*   pt_dev_discovery_config)
{
    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return CECMR_CEC_FCT_DISABLED;
    }

    if ((h_cecm_svc == NULL_HANDLE))
    {
        return CECMR_INV_ARG;
    }

    return (cecm_ctrl_config_dev_discovery(h_cecm_svc, pt_dev_discovery_config));

}
/*-----------------------------------------------------------------------------
 * Name: x_cecm_get_dev_discovery_config
 *
 * Description: The API get device discovery confi.
 *
 * Inputs:  h_cecm_svc                  A handle referencing the service.
 *          ui4_dev_discovery_itvl      Device discovery interval in second.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 ----------------------------------------------------------------------------*/

INT32 x_cecm_get_dev_discovery_config(HANDLE_T        h_cecm_svc,
                                             CECM_DEV_DISCOVERY_CONFIG_T*   pt_dev_discovery_config)
{
    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return CECMR_CEC_FCT_DISABLED;
    }

    if ((h_cecm_svc == NULL_HANDLE))
    {
        return CECMR_INV_ARG;
    }
    
    return (cecm_ctrl_get_dev_discovery_config(h_cecm_svc, pt_dev_discovery_config));

}

/*-----------------------------------------------------------------------------
 * Name: x_cecm_start_dev_discovery
 *
 * Description: The API starts CEC device discovery.
 *
 * Inputs:  h_cecm_svc                  A handle referencing the service.
 *          ui4_dev_discovery_itvl      Device discovery interval in second.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_start_dev_discovery(HANDLE_T h_cecm_svc,
                                 UINT32   ui4_dev_discovery_itvl)
{
    CECM_DEV_DISCOVERY_T    t_dev_discovery_info;

    DBG_API(("{CECM} x_cecm_start_dev_discovery: itvl=%d\n\r", ui4_dev_discovery_itvl));

    /* Device discovery from 1 to 14 with b_batch_polling set to OFF */
    t_dev_discovery_info.e_start_la = CECM_LOG_ADDR_REC_DEV_1;
    t_dev_discovery_info.e_stop_la = CECM_LOG_ADDR_FREE_USE;
    t_dev_discovery_info.ui4_itvl = ui4_dev_discovery_itvl;
    t_dev_discovery_info.b_batch_polling = FALSE;

    return (x_cecm_start_dev_discovery_ex(h_cecm_svc,
                                          &t_dev_discovery_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_start_dev_discovery_ex
 *
 * Description: The API starts CEC device discovery.
 *
 * Inputs:  h_cecm_svc                  A handle referencing the service.
 *          pt_dev_discovery_info       References a device discovery
 *                                      information.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 ----------------------------------------------------------------------------*/

INT32 x_cecm_start_dev_discovery_ex(HANDLE_T              h_cecm_svc,
                                    CECM_DEV_DISCOVERY_T* pt_dev_discovery_info)
{
    INT32           i4_return;
    CECM_MSG_T      t_msg;
    
    DBG_API(("{CECM} x_cecm_start_dev_discovery_ex: start_la=%d, stop_la=%d, itvl=%d, batch_polling=%d\n\r",
            pt_dev_discovery_info->e_start_la,
            pt_dev_discovery_info->e_stop_la,
            pt_dev_discovery_info->ui4_itvl,
            pt_dev_discovery_info->b_batch_polling));

    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return CECMR_CEC_FCT_DISABLED;
    }

    if ((h_cecm_svc == NULL_HANDLE) ||
        (pt_dev_discovery_info == NULL))
    {
        return CECMR_INV_ARG;
    }

    if (((pt_dev_discovery_info->e_start_la < CECM_LOG_ADDR_REC_DEV_1) ||
         (pt_dev_discovery_info->e_start_la > CECM_LOG_ADDR_FREE_USE)) ||
        ((pt_dev_discovery_info->e_stop_la < CECM_LOG_ADDR_REC_DEV_1) ||
         (pt_dev_discovery_info->e_stop_la > CECM_LOG_ADDR_FREE_USE)))
    {
        return CECMR_INV_ARG;
    }

    /* Send device discovery start message */
    t_msg.e_msg_type = CECM_MSG_TYPE_DEV_DISCOVERY_START;
    t_msg.u.t_dev_discvry_info = *pt_dev_discovery_info;

    i4_return = x_msg_q_send(h_cecm_msgq,
                             (VOID*)&t_msg,
                             sizeof(CECM_MSG_T),
                             CECM_MSGQ_DEFAULT_PRIORITY);
    
    if (i4_return == OSR_OK)
    {
        return CECMR_OK;
    }
    else
    {
        return CECMR_MSG_SEND_ERROR;
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_stop_dev_discovery
 *
 * Description: The API stops CEC device discovery.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_stop_dev_discovery(HANDLE_T h_cecm_svc)
{
    INT32           i4_return;
    CECM_MSG_T      t_msg;

    DBG_API(("{CECM} x_cecm_stop_dev_discovery\n\r"));

    /* Check arguments */
    if (!b_cecm_init)
    {
        return CECMR_NOT_INIT;
    }

    if (!cecm_ctrl_is_cec_fct_enable())
    {
        return CECMR_CEC_FCT_DISABLED;
    }

    if (h_cecm_svc == NULL_HANDLE)
    {
        return CECMR_INV_ARG;
    }

    /* Send device discovery stop message */
    t_msg.e_msg_type = CECM_MSG_TYPE_DEV_DISCOVERY_STOP;

    i4_return = x_msg_q_send(h_cecm_msgq,
                             (VOID*)&t_msg,
                             sizeof(CECM_MSG_T),
                             CECM_MSGQ_DEFAULT_PRIORITY);

    if (i4_return == OSR_OK)
    {
        return CECMR_OK;
    }
    else
    {
        return CECMR_MSG_SEND_ERROR;
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_active_source
 *
 * Description: This API is used to response to a <Request Active Source>.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_active_source(HANDLE_T h_cecm_svc)
{
    INT32           i4_return;
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_active_source\n\r"));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, CECM_LOG_ADDR_UNREGED_BRDCST);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }        

    t_set_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
    t_set_info.ui2_opcode = CECM_OPCODE_ACT_SRC;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_request_active_source
 *
 * Description: This API discovers the current active source of the system.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_request_active_source(HANDLE_T h_cecm_svc)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_request_active_source\n\r"));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, CECM_LOG_ADDR_UNREGED_BRDCST);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    } 

    t_set_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
    t_set_info.ui2_opcode = CECM_OPCODE_REQ_ACT_SRC;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_standby
 *
 * Description: The API switches a device into standby mode.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_standby(HANDLE_T        h_cecm_svc,
                         CECM_LOG_ADDR_T e_la)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_standby: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    } 

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_STANDBY;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_standby_all
 *
 * Description: The API switches all devices into standby mode.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          b_sync          Indicates if this is a synchronous operation.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_standby_all(HANDLE_T h_cecm_svc,
                             BOOL     b_sync)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_standby_all: sync=%d\n\r", b_sync));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, CECM_LOG_ADDR_UNREGED_BRDCST);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    } 

    t_set_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
    t_set_info.ui2_opcode = CECM_OPCODE_STANDBY;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    if (b_sync)
    {
        return (_cecm_set_sync(h_cecm_svc,
                               &t_set_info));
    }
    else
    {
        return (cecm_ctrl_set(h_cecm_svc,
                              &t_set_info));
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_routing_change
 *
 * Description: Ths API informs all devices on the newwork that the active
 *              route had been changed.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          ui1_orig_comp_id    Original HDMI component ID.
 *          ui1_new_comp_id     New HDMI component ID.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_routing_change(HANDLE_T h_cecm_svc,
                                UINT16   ui2_orig_comp_id,
                                UINT16   ui2_new_comp_id)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_routing_change: orig_comp_id=%d, new_comp_id=%d\n\r",
            ui2_orig_comp_id, ui2_new_comp_id));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, CECM_LOG_ADDR_UNREGED_BRDCST);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    } 

    t_set_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
    t_set_info.ui2_opcode = CECM_OPCODE_ROUTING_CHG;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.t_routing_chg_info.ui2_orig_comp_id = ui2_orig_comp_id;
    t_set_info.u_operand.t_routing_chg_info.ui2_new_comp_id = ui2_new_comp_id;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_routing_change_ex
 *
 * Description: Ths API informs all devices on the newwork that the active
 *              route had been changed.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          ui1_orig_comp_id      Original CEC device PA.
 *          ui1_new_comp_id     New CEC device PA.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_routing_change_ex(HANDLE_T h_cecm_svc,
                                    UINT16   ui2_orig_pa,
                                    UINT16   ui2_new_pa)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;
  
    DBG_API(("{CECM} x_cecm_set_routing_change_ex: orig_pa=%d, new_pa=%d\n\r",
                                ui2_orig_pa, ui2_new_pa));
  
    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, CECM_LOG_ADDR_UNREGED_BRDCST);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    } 
  
    t_set_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
    t_set_info.ui2_opcode = CECM_OPCODE_ROUTING_CHG;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_ROUTING_CHG;              //add additional info to differ with component id method(above)
    t_set_info.u_operand.t_routing_chg_info.ui2_orig_pa = ui2_orig_pa;
    t_set_info.u_operand.t_routing_chg_info.ui2_new_pa = ui2_new_pa;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;
  
    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
    }   




/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_stream_path
 *
 * Description: This API requests a streaming path from the specified logical
 *              device.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_stream_path(HANDLE_T        h_cecm_svc,
                             CECM_LOG_ADDR_T e_la)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_stream_path: la=%d\n\r", e_la));
    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    } 

    t_set_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
    t_set_info.ui2_opcode = CECM_OPCODE_SET_STREAM_PATH;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.e_la = e_la;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_stream_path_ex
 *
 * Description: This API requests a streaming path from the specified logical
 *              device.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          ui2_pa              A physical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_stream_path_ex(HANDLE_T h_cecm_svc,
                                UINT16   ui2_pa)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_stream_path_ex: pa=0x%x\n\r", ui2_pa));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, CECM_LOG_ADDR_UNREGED_BRDCST);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    } 

    t_set_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
    t_set_info.ui2_opcode = CECM_OPCODE_SET_STREAM_PATH;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_SET_STREAM_PATH_BY_PA;
    t_set_info.u_operand.ui2_pa = ui2_pa;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_get_cec_version
 *
 * Description: This API enquires which version of CEC the target supports.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_get_cec_version(HANDLE_T        h_cecm_svc,
                             CECM_LOG_ADDR_T e_la)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_get_cec_version: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    } 

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_GET_CEC_VER;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_give_physical_address
 *
 * Description: This API requests a device to return its physical address.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_give_physical_address(HANDLE_T        h_cecm_svc,
                                   CECM_LOG_ADDR_T e_la)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_give_physical_address: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    } 

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_GIVE_PHY_ADDR;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_get_menu_lang
 *
 * Description: This API discovers the currently set menu language of a device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_get_menu_lang(HANDLE_T        h_cecm_svc,
                           CECM_LOG_ADDR_T e_la)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_get_menu_lang: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    } 

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_GET_MENU_LANG;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_polling_message
 *
 * Description: This API pings a CEC device.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          b_sync              Indicates if this is a synchronous operation.
 *
 * Outputs: pt_polling_info     References polling info.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_polling_message(HANDLE_T             h_cecm_svc,
                                 CECM_LOG_ADDR_T      e_la,
                                 BOOL                 b_sync,
                                 CECM_POLLING_INFO_T* pt_polling_info)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_polling_message: la=%d, sync=%d\n\r", e_la, b_sync)); 

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    } 
    
    if (b_sync && (pt_polling_info == NULL))
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_NONE;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;
    t_set_info.t_sync_info.t_sync_data.pv_data = (VOID*)pt_polling_info;

    if (b_sync)
    {
        return (_cecm_set_sync(h_cecm_svc,
                               &t_set_info));
    }
    else
    {
        return (cecm_ctrl_set(h_cecm_svc,
                              &t_set_info));
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_report_physical_address
 *
 * Description: This API informs all other devices of the mapping between
 *              physical and logical address.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          pt_report_phy_addr  References the report physical address data
 *                              structure.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_report_physical_address(HANDLE_T                h_cecm_svc,
                                     CECM_REPORT_PHY_ADDR_T* pt_report_phy_addr)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_report_physical_address\n\r"));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, CECM_LOG_ADDR_UNREGED_BRDCST);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   
    
    if (pt_report_phy_addr == NULL)
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
    t_set_info.ui2_opcode = CECM_OPCODE_REPORT_PHY_ADDR;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.t_report_phy_addr = *pt_report_phy_addr;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_menu_lang
 *
 * Description: This API indicates the menu language.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          pt_lang         References ISO-639 language.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_menu_lang(HANDLE_T        h_cecm_svc,
                           ISO_639_LANG_T* pt_lang)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_menu_lang\n\r"));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, CECM_LOG_ADDR_UNREGED_BRDCST);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   

    if (pt_lang == NULL)
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = CECM_LOG_ADDR_UNREGED_BRDCST;
    t_set_info.ui2_opcode = CECM_OPCODE_SET_MENU_LANG;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    x_memcpy(&t_set_info.u_operand.t_lang, pt_lang, sizeof(ISO_639_LANG_T));
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_give_deck_status
 *
 * Description: This API requests the status of a device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          e_sts_req       Contains status request.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_give_deck_status(HANDLE_T        h_cecm_svc,
                              CECM_LOG_ADDR_T e_la,
                              CECM_STS_REQ_T  e_sts_req)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_give_deck_status: la=%d, sts_req=%d\n\r", e_la, e_sts_req));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_GIVE_DECK_STS;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.e_sts_req = e_sts_req;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_deck_ctrl
 *
 * Description: This API is used to control a device's media functions.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          e_deck_ctrl_mod     Contains deck control mode.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_deck_ctrl(HANDLE_T             h_cecm_svc,
                           CECM_LOG_ADDR_T      e_la,
                           CECM_DECK_CTRL_MOD_T e_deck_ctrl_mod)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_deck_ctrl: la=%d, ctrl_mod=%d\n\r", e_la, e_deck_ctrl_mod));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_DECK_CTRL;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.e_deck_ctrl_mod = e_deck_ctrl_mod;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_deck_play
 *
 * Description: This API is used to control the playback behavior of a source
 *              device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          e_play_mod      Contains play mode.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_deck_play(HANDLE_T        h_cecm_svc,
                           CECM_LOG_ADDR_T e_la,
                           CECM_PLAY_MOD_T e_play_mod)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_deck_play: la=%d, play_mod=%d\n\r", e_la, e_play_mod));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_PLAY;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.e_play_mod = e_play_mod;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_osd_name
 *
 * Description: This API sets the preferred OSD name of a device for use in
 *              menus associated with that device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          paw2_osd_name   References a OSD name.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_osd_name(HANDLE_T        h_cecm_svc,
                          CECM_LOG_ADDR_T e_la,
                          UTF16_T*        paw2_osd_name)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_osd_name: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_SET_OSD_NAME;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    x_uc_w2s_strcpy((UTF16_T*)&(t_set_info.u_operand.aw2_osd_name)[0], (const UTF16_T*)paw2_osd_name);
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_give_osd_name
 *
 * Description: This API requests the preferred OSD name of a device for use in
 *              menus associated with that device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_give_osd_name(HANDLE_T        h_cecm_svc,
                           CECM_LOG_ADDR_T e_la)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_give_osd_name: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_GIVE_OSD_NAME;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_menu_request
 *
 * Description: This API is used to request a device to show/remove a menu or
 *              to query if a device is currently showing a menu.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          e_menu_req_type     Contains menu request type.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_menu_request(HANDLE_T              h_cecm_svc,
                              CECM_LOG_ADDR_T       e_la,
                              CECM_MENU_REQ_STATE_T e_menu_req_type)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_menu_request: la=%d, menu_req_type=%d\n\r", e_la, e_menu_req_type));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_MENU_REQ;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.e_menu_req_type = e_menu_req_type;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_device_vendor_id
 *
 * Description: This API reports the vendor ID of this device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          pt_dev_vndr_id  References a device vendor ID.
 *          b_sync          Indicates if this is a synchronous operation.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_device_vendor_id(HANDLE_T            h_cecm_svc,
                                  CECM_DEV_VNDR_ID_T* pt_dev_vndr_id,
                                  BOOL                b_sync)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_device_vendor_id: dev_vndr_id=0x%x%x%x, sync=%d\n\r", 
            pt_dev_vndr_id[0], pt_dev_vndr_id[1], pt_dev_vndr_id[2], b_sync));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, CECM_LOG_ADDR_UNREGED_BRDCST);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   

    if (pt_dev_vndr_id == NULL)
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)CECM_LOG_ADDR_UNREGED_BRDCST;
    t_set_info.ui2_opcode = CECM_OPCODE_DEV_VNDR_ID;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;    
    x_memcpy(&t_set_info.u_operand.t_dev_vndr_id, pt_dev_vndr_id, CECM_DEV_VNDR_ID_SIZE);
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    if (b_sync)
    {
        return (_cecm_set_sync(h_cecm_svc,
                               &t_set_info));
    }
    else
    {
        return (cecm_ctrl_set(h_cecm_svc,
                              &t_set_info));
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_give_device_vendor_id
 *
 * Description: This API requests the vendor ID from a device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          b_sync          Indicates if this is a synchronous operation.
 *
 * Outputs: pt_dev_vndr_id  References a device vendor ID.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_give_device_vendor_id(HANDLE_T            h_cecm_svc,
                                   CECM_LOG_ADDR_T     e_la,
                                   BOOL                b_sync,
                                   CECM_DEV_VNDR_ID_T* pt_dev_vndr_id)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_give_device_vendor_id: la=%d, sync=%d\n\r", e_la, b_sync)); 

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   
    
    if (b_sync && (pt_dev_vndr_id == NULL))
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_GIVE_DEV_VNDR_ID;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;
    t_set_info.t_sync_info.t_sync_data.pv_data = (VOID*)pt_dev_vndr_id;

    if (b_sync)
    {
        return (_cecm_set_sync(h_cecm_svc,
                               &t_set_info));
    }
    else
    {
        return (cecm_ctrl_set(h_cecm_svc,
                              &t_set_info));
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_user_ctrl_pressed
 *
 * Description: This API indicates that the user pressed a remote control
 *              button or switched from one remote control button to another.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          e_usr_ctrl      Contains user control info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_user_ctrl_pressed(HANDLE_T         h_cecm_svc,
                                   CECM_LOG_ADDR_T  e_la,
                                   CECM_USER_CTRL_T e_usr_ctrl)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_user_ctrl_pressed: la=%d, usr_ctrl=%d\n\r", e_la, e_usr_ctrl));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_USER_CTRL_PRESSED;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.e_usr_ctrl = e_usr_ctrl;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}

/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_user_ctrl_pressed_ex
 *
 * Description: This API indicates that the user pressed a remote control
 *              button or switched from one remote control button to another,
 *              and can specify additional information.
 *
 * Inputs:  h_cecm_svc                  A handle referencing the service.
 *          e_la                        CEC device logical address.
 *          pt_usr_ctrl_operand_info    Reference of  user control info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_user_ctrl_pressed_ex(HANDLE_T         h_cecm_svc,
                                   CECM_LOG_ADDR_T  e_la,
                                   CECM_USER_CTRL_WITH_OPERAND_T* pt_usr_ctrl_operand_info)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_user_ctrl_pressed_ex: la=%d, usr_ctrl=%d\n\r", 
              e_la, pt_usr_ctrl_operand_info->e_user_ctrl));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }  
    
    if (pt_usr_ctrl_operand_info == NULL)
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_USER_CTRL_PRESSED;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_USER_CTRL;
    t_set_info.u_operand.t_usr_ctrl_with_operand = *pt_usr_ctrl_operand_info;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}

/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_user_ctrl_released
 *
 * Description: This API indicates that the user released a remote control
 *              button (the last one indicated by <User Control Pressed>
 *              message).
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_user_ctrl_released(HANDLE_T        h_cecm_svc,
                                    CECM_LOG_ADDR_T e_la)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_user_ctrl_released: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_USER_CTRL_RELEASED;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_power_on_dev
 *
 * Description: This API powers on a device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_ERROR                 Operation error.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_power_on_dev(HANDLE_T        h_cecm_svc,
                          CECM_LOG_ADDR_T e_la)
{
    INT32           i4_return;
        
    DBG_API(("{CECM} x_cecm_power_on_dev: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   

#ifdef CECM_PWR_ON_DEV_EX
    CECM_MSG_T      t_msg;

    /* Send select_device message */
    t_msg.e_msg_type = CECM_MSG_TYPE_SELECT_DEV;
    t_msg.u.t_select_dev.h_cecm_svc = h_cecm_svc;
    t_msg.u.t_select_dev.e_pwr_on_dev_la = e_la;
    t_msg.u.t_select_dev.e_sent_la = CECM_LOG_ADDR_MAX;
    t_msg.u.t_select_dev.e_sent_opcode = CECM_OPCODE_NONE;

    i4_return = x_msg_q_send(h_cecm_msgq,
                             (VOID*)&t_msg,
                             sizeof(CECM_MSG_T),
                             CECM_MSGQ_DEFAULT_PRIORITY);

    if (i4_return == OSR_OK)
    {
        return CECMR_OK;
    }
    else
    {
        return CECMR_MSG_SEND_ERROR;
    }
#else

    CECM_MSG_T      t_msg;

    /* Send select_device message */
    t_msg.e_msg_type = CECM_MSG_TYPE_SELECT_DEV;
    t_msg.u.t_select_dev.h_cecm_svc = h_cecm_svc;
    t_msg.u.t_select_dev.e_pwr_on_dev_la = e_la;
    t_msg.u.t_select_dev.e_sent_la = CECM_LOG_ADDR_MAX;
    t_msg.u.t_select_dev.e_sent_opcode = CECM_OPCODE_NONE;

    i4_return = x_msg_q_send(h_cecm_msgq,
                             (VOID*)&t_msg,
                             sizeof(CECM_MSG_T),
                             CECM_MSGQ_DEFAULT_PRIORITY);

    if (i4_return == OSR_OK)
    {
        return CECMR_OK;
    }
    else
    {
        return CECMR_MSG_SEND_ERROR;
    }

#endif
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_give_device_power_status
 *
 * Description: This API is used to determine the current power status of a
 *              target device.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          b_sync          Indicates if this is a synchronous operation.
 *
 * Outputs: pe_pwr_sts      References power status.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_give_device_power_status(HANDLE_T        h_cecm_svc,
                                      CECM_LOG_ADDR_T e_la,
                                      BOOL            b_sync,
                                      CECM_PWR_STS_T* pe_pwr_sts)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_give_device_power_status: la=%d, sync=%d\n\r", e_la, b_sync)); 

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   

    if (b_sync && (pe_pwr_sts == NULL))
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_GIVE_DEV_PWR_STS;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;
    t_set_info.t_sync_info.t_sync_data.pv_data = (VOID*)pe_pwr_sts;

    if (b_sync)
    {
        return (_cecm_set_sync(h_cecm_svc,
                               &t_set_info));
    }
    else
    {
        return (cecm_ctrl_set(h_cecm_svc,
                              &t_set_info));
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_report_power_status
 *
 * Description: This API reports current power status.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *          e_pwr_sts       Contains power status.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_report_power_status(HANDLE_T        h_cecm_svc,
                                 CECM_LOG_ADDR_T e_la,
                                 CECM_PWR_STS_T  e_pwr_sts)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_report_power_status: la=%d, pwr_sts=%d\n\r", e_la, e_pwr_sts));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_REPORT_PWR_STS;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.e_pwr_sts = e_pwr_sts;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_system_audio_mode_request
 *
 * Description: This API requests to use System Audio Mode to the amplifier.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          pt_sys_aud_ctrl     References the system audio control info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_system_audio_mode_request(HANDLE_T             h_cecm_svc,
                                           CECM_SYS_AUD_CTRL_T* pt_sys_aud_ctrl)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_system_audio_mode_request\n\r"));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, CECM_LOG_ADDR_AUD_SYS);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }   
        
    t_set_info.ui1_dest_addr = CECM_LOG_ADDR_AUD_SYS;
    t_set_info.ui2_opcode = CECM_OPCODE_SYS_AUD_MOD_REQ;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.t_sys_aud_ctrl = *pt_sys_aud_ctrl;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_system_audio_mode_request_ex
 *
 * Description: This API requests to use System Audio Mode to the amplifier.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          pt_sys_aud_ctrl     References the system audio control info.
 *          b_sync              Indicates if this is a synchronous operation.
 *
 * Outputs: pe_sys_aud_sts      References system audio status.
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_system_audio_mode_request_ex(HANDLE_T             h_cecm_svc,
                                              CECM_SYS_AUD_CTRL_T* pt_sys_aud_ctrl,
                                              BOOL                 b_sync,
                                              CECM_SYS_AUD_STS_T*  pe_sys_aud_sts)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_system_audio_mode_request: sync=%d\n\r", b_sync)); 

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, CECM_LOG_ADDR_AUD_SYS);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    

    if (pt_sys_aud_ctrl == NULL)
    {
        return CECMR_INV_ARG;
    }

    if (b_sync && (pe_sys_aud_sts == NULL))
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = CECM_LOG_ADDR_AUD_SYS;
    t_set_info.ui2_opcode = CECM_OPCODE_SYS_AUD_MOD_REQ;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.t_sys_aud_ctrl = *pt_sys_aud_ctrl;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;
    t_set_info.t_sync_info.t_sync_data.pv_data = (VOID*)pe_sys_aud_sts;

    if (b_sync)
    {
        return (_cecm_set_sync(h_cecm_svc,
                               &t_set_info));
    }
    else
    {
        return (cecm_ctrl_set(h_cecm_svc,
                              &t_set_info));
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_give_audio_status
 *
 * Description: This API requests an amplifier to send its volume and mute
 *              status.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          b_sync              Indicates if this is a synchronous operation.
 *
 * Outputs: pui1_aud_sts        References audio status of an amplifier (This
 *                              argument is valid when b_sync = TRUE and is
 *                              ignored if b_sync = FALSE).
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_give_audio_status(HANDLE_T        h_cecm_svc,
                               CECM_LOG_ADDR_T e_la,
                               BOOL            b_sync,
                               UINT8*          pui1_aud_sts)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_give_audio_status: la=%d, sync=%d\n\r", e_la, b_sync));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    
    
    if (b_sync && (pui1_aud_sts == NULL))
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_GIVE_AUD_STS;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;
    t_set_info.t_sync_info.t_sync_data.pv_data = (VOID*)pui1_aud_sts;

    if (b_sync)
    {
        return (_cecm_set_sync(h_cecm_svc,
                               &t_set_info));
    }
    else
    {
        return (cecm_ctrl_set(h_cecm_svc,
                              &t_set_info));
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_give_system_audio_mode_status
 *
 * Description: This API requests the status of the System Audio Mode.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_give_system_audio_mode_status(HANDLE_T        h_cecm_svc,
                                           CECM_LOG_ADDR_T e_la)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_give_system_audio_mode_status: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_GIVE_SYS_AUD_MOD_STS;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}
/*-----------------------------------------------------------------------------
 * Name: x_cecm_request_audio_descriptor
 *
 * Description: This API requests the status of the System Audio Mode.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_aud_fmt          Reference audio format code
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/

INT32 x_cecm_request_audio_descriptor(HANDLE_T h_cecm_svc,
                                             CECM_LOG_ADDR_T e_la,
                                             CECM_AUD_FMT_T*   pt_aud_fmt,
                                             BOOL  b_sync)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_request_audio_descriptor: la=%d, sync=%d\n\r", e_la, b_sync));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    


    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_REQUEST_SAD;
    t_set_info.u_operand.t_aud_fmt = *pt_aud_fmt;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    if (b_sync)
    {
        return (_cecm_set_sync(h_cecm_svc,
                               &t_set_info));
    }
    else
    {
        return (cecm_ctrl_set(h_cecm_svc,
                              &t_set_info));
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_audio_rate
 *
 * Description: This API is used to control audio rate from source device.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          e_aud_rate          Contains the audio rate info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_audio_rate(HANDLE_T        h_cecm_svc,
                            CECM_LOG_ADDR_T e_la,
                            CECM_AUD_RATE_T e_aud_rate)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_audio_rate: aud_rate=%d\n\r", e_aud_rate));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    
    
    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_SET_AUD_RATE;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.e_aud_rate = e_aud_rate;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_clear_timer
 *
 * Description: This API is used to clear a timer block of a device.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_timer_info       References the timer info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_clear_timer(HANDLE_T           h_cecm_svc,
                         CECM_LOG_ADDR_T    e_la,
                         CECM_TIMER_INFO_T* pt_timer_info)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_clear_timer: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    
    
    if (pt_timer_info == NULL)
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)e_la;

    switch (pt_timer_info->e_dev)
    {
        case CECM_TIMER_DEV_ANA:
            t_set_info.ui2_opcode = CECM_OPCODE_CLR_ANA_TIMER;
            break;

        case CECM_TIMER_DEV_DIG:
            t_set_info.ui2_opcode = CECM_OPCODE_CLR_DIG_TIMER;
            break;

        case CECM_TIMER_DEV_EXT:
            t_set_info.ui2_opcode = CECM_OPCODE_CLR_EXT_TIMER;
            break;

        default:
            return CECMR_INV_ARG;
    }

    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.t_timer_info = *pt_timer_info;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_timer
 *
 * Description: This API is used to set a single timer block of a device.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_timer_info       References the timer info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_timer(HANDLE_T           h_cecm_svc,
                       CECM_LOG_ADDR_T    e_la,
                       CECM_TIMER_INFO_T* pt_timer_info)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_timer: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    
    
    if (pt_timer_info == NULL)
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)e_la;

    switch (pt_timer_info->e_dev)
    {
        case CECM_TIMER_DEV_ANA:
            t_set_info.ui2_opcode = CECM_OPCODE_SET_ANA_TIMER;
            break;

        case CECM_TIMER_DEV_DIG:
            t_set_info.ui2_opcode = CECM_OPCODE_SET_DIG_TIMER;
            break;

        case CECM_TIMER_DEV_EXT:
            t_set_info.ui2_opcode = CECM_OPCODE_SET_EXT_TIMER;
            break;

        default:
            return CECMR_INV_ARG;
    }

    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.t_timer_info = *pt_timer_info;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_timer_prog_title
 *
 * Description: This API is used to set the name of a program associated with a
 *              timer block.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_prog_title_str   References a program title string.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_timer_prog_title(HANDLE_T           h_cecm_svc,
                                  CECM_LOG_ADDR_T    e_la,
                                  CECM_PROG_TITLE_T* pt_prog_title_str)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_timer_prog_title: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    
    
    if (pt_prog_title_str == NULL)
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_SET_TIMER_PROG_TITLE;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    x_uc_w2s_strcpy((UTF16_T*)&t_set_info.u_operand.t_prog_title_str, (const UTF16_T*)pt_prog_title_str);
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_give_tuner_status
 *
 * Description: This API is used to request the status of a tuner.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          e_sts_req           Contains status request.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_give_tuner_status(HANDLE_T        h_cecm_svc,
                               CECM_LOG_ADDR_T e_la,
                               CECM_STS_REQ_T  e_sts_req)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_give_tuner_status: la=%d, sts_req=%d\n\r", e_la, e_sts_req));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    
    
    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_GIVE_TUNER_DEV_STS;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;    
    t_set_info.u_operand.e_sts_req = e_sts_req;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_select_service
 *
 * Description: This API is used to directly select an analog TV, digital TV,
 *              radio or data broadcast service.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_svc_info         References the tuner service info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_select_service(HANDLE_T               h_cecm_svc,
                            CECM_LOG_ADDR_T        e_la,
                            CECM_TUNER_SVC_INFO_T* pt_svc_info)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_select_service: la=%d\n\r"));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    
    
    if (pt_svc_info == NULL)
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)e_la;

    switch (pt_svc_info->e_svc)
    {
        case CECM_TUNER_SVC_ANA:
            t_set_info.ui2_opcode = CECM_OPCODE_SELECT_ANA_SVC;
            break;

        case CECM_TUNER_SVC_DIG:
            t_set_info.ui2_opcode = CECM_OPCODE_SELECT_DIG_SVC;
            break;

        default:
            return CECMR_INV_ARG;
    }

    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.t_tuner_svc_info = *pt_svc_info;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_tuner_step
 *
 * Description: This API is used to tune to next lowest/highest service in a
 *              tuner's service list.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          e_tuner_step        Contains tuner step info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_tuner_step(HANDLE_T          h_cecm_svc,
                            CECM_LOG_ADDR_T   e_la,
                            CECM_TUNER_STEP_T e_tuner_step)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_tuner_step: la=%d, tuner_step=%d\n\r", e_la, e_tuner_step));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    

    t_set_info.ui1_dest_addr = (UINT8)e_la;

    switch (e_tuner_step)
    {
        case CECM_TUNER_STEP_DECREMENT:
            t_set_info.ui2_opcode = CECM_OPCODE_TUNER_STEP_DECR;
            break;

        case CECM_TUNER_STEP_INCREMENT:
            t_set_info.ui2_opcode = CECM_OPCODE_TUNER_STEP_INCR;
            break;

        default:
            return CECMR_INV_ARG;
    }

    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_record_on
 *
 * Description: This API attempts to record the specific source.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_rec_src          References the record source info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_record_on(HANDLE_T        h_cecm_svc,
                           CECM_LOG_ADDR_T e_la,
                           CECM_REC_SRC_T* pt_rec_src)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_record_on: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    
    
    if (pt_rec_src == NULL)
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_REC_ON;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.t_rec_src = *pt_rec_src;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_record_off
 *
 * Description: This API requests a device to stop a recording.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_record_off(HANDLE_T        h_cecm_svc,
                            CECM_LOG_ADDR_T e_la)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_record_off\n\r"));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_REC_OFF;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_feature_abort
 *
 * Description: This API is used as a response to indicate that the requested
 *              message type is not supported.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_feature_abort    References the feature abort info.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_feature_abort(HANDLE_T              h_cecm_svc,
                               CECM_LOG_ADDR_T       e_la,
                               CECM_FEATURE_ABORT_T* pt_feature_abort)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_feature_abort: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    

    if (pt_feature_abort == NULL)
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_FEATURE_ABORT;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.t_feature_abort = *pt_feature_abort;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_vendor_cmd
 *
 * Description: This API allows vendor specific commands to be sent between
 *              two devices.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_vndr_cmd_data    References vendor specific data.
 *          b_sync              Indicates if this is a synchronous operation.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_vendor_cmd(HANDLE_T              h_cecm_svc,
                            CECM_LOG_ADDR_T       e_la,
                            CECM_VNDR_CMD_DATA_T* pt_vndr_cmd_data,
                            BOOL                  b_sync)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_vendor_cmd: la=%d, sync=%d\n\r", e_la, b_sync)); 

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    

    if (pt_vndr_cmd_data == NULL)
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_VNDR_CMD;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.t_vndr_cmd_data = *pt_vndr_cmd_data;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    if (b_sync)
    {
        return (_cecm_set_sync(h_cecm_svc,
                               &t_set_info));
    }
    else
    {
        return (cecm_ctrl_set(h_cecm_svc,
                              &t_set_info));
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_specific_cmd
 *
 * Description: This API allows vendor specific commands to be sent between
 *              two devices.
 *
 * Inputs:  h_cecm_svc          A handle referencing the service.
 *          e_la                CEC device logical address.
 *          pt_vndr_cmd_data    References vendor specific data.
 *          b_sync              Indicates if this is a synchronous operation.  
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_specific_cmd(HANDLE_T              h_cecm_svc,
                            CECM_LOG_ADDR_T       e_la,
                            CECM_OPCODE_T         e_opcode,
                            CECM_SPEC_CMD_DATA_T* pt_spec_cmd_data,
                            BOOL                  b_sync)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_specific_cmd: la=%d, sync=%d , e_opcode = %d\n\r", e_la, b_sync, e_opcode)); 

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    

    if (pt_spec_cmd_data == NULL)
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = (UINT16)e_opcode;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.t_spec_cmd_data = *pt_spec_cmd_data;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    if (b_sync)
    {
        return (_cecm_set_sync(h_cecm_svc,
                               &t_set_info));
    }
    else
    {
        return (cecm_ctrl_set(h_cecm_svc,
                              &t_set_info));
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_vendor_cmd_with_id
 *
 * Description: This API allows vendor specific commands to be sent between
 *              two devices or broadcast.
 *
 * Inputs:  h_cecm_svc                  A handle referencing the service.
 *          e_la                        CEC device logical address.
 *          pt_vndr_cmd_with_id_data    References vendor specific data with
 *                                      ID.
 *          b_sync                      Indicates if this is a synchronous
 *                                      operation.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_vendor_cmd_with_id(HANDLE_T                      h_cecm_svc,
                                    CECM_LOG_ADDR_T               e_la,
                                    CECM_VNDR_CMD_WITH_ID_DATA_T* pt_vndr_cmd_with_id_data,
                                    BOOL                          b_sync)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_vendor_cmd_with_id: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    

    if (pt_vndr_cmd_with_id_data == NULL)
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_VNDR_CMD_WITH_ID;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.t_vndr_cmd_with_id_data = *pt_vndr_cmd_with_id_data;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    if (b_sync)
    {
        return (_cecm_set_sync(h_cecm_svc,
                               &t_set_info));
    }
    else
    {
        return (cecm_ctrl_set(h_cecm_svc,
                              &t_set_info));
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_vendor_remote_btn_down
 *
 * Description: This API indicates that a remote control button has been
 *              depressed.
 *
 * Inputs:  h_cecm_svc              A handle referencing the service.
 *          e_la                    CEC device logical address.
 *          pt_vndr_rc_code_data    References vendor specific RC code.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_vendor_remote_btn_down(HANDLE_T                  h_cecm_svc,
                                        CECM_LOG_ADDR_T           e_la,
                                        CECM_VNDR_RC_CODE_DATA_T* pt_vndr_rc_code_data)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_vendor_remote_btn_down: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    

    if (pt_vndr_rc_code_data == NULL)
    {
        return CECMR_INV_ARG;
    }

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_VNDR_REMOTE_BTN_DOWN;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.u_operand.t_vndr_rc_code_data = *pt_vndr_rc_code_data;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}


/*-----------------------------------------------------------------------------
 * Name: x_cecm_set_vendor_remote_btn_up
 *
 * Description: This API indicates that a remote control button (the last
 *              button pressed indicated by the Vendor Remote Button Down
 *              message) Button has been released.
 *
 * Inputs:  h_cecm_svc      A handle referencing the service.
 *          e_la            CEC device logical address.
 *
 * Outputs: -
 *
 * Returns: CECMR_OK                    Routine successful.
 *          CECMR_NOT_INIT              The CEC Manager is not initialized.
 *          CECMR_INV_ARG               One or more of the arguments contain
 *                                      invalid data.
 *          CECMR_CEC_FCT_DISABLED      CEC function is disabled.
 *          CECMR_DEV_NOT_AVIL          Device is not available.
 *          CECMR_MSG_SEND_ERROR        Send message failed.
 *          CECMR_ERROR                 Internal error.
 ----------------------------------------------------------------------------*/
INT32 x_cecm_set_vendor_remote_btn_up(HANDLE_T        h_cecm_svc,
                                      CECM_LOG_ADDR_T e_la)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_set_vendor_remote_btn_up: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_VNDR_REMOTE_BTN_UP;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    return (cecm_ctrl_set(h_cecm_svc,
                          &t_set_info));
}

INT32 x_cecm_arc_ctrl(HANDLE_T                  h_cecm_svc,
                         CECM_ARC_CTRL_T*          pt_arc_ctrl)
{
    if ((h_cecm_svc == NULL_HANDLE) || (pt_arc_ctrl == NULL))
    {
        return CECMR_INV_ARG;
    }

    return cecm_ctrl_set_arc_ctrl(h_cecm_svc, pt_arc_ctrl);
}
    

INT32 x_cecm_request_init_arc(HANDLE_T h_cecm_svc,
                                  CECM_LOG_ADDR_T  e_la,
                                  BOOL    b_sync)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_request_init_arc: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_REQ_INIT_ARC;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    if (b_sync)
    {
        return (_cecm_set_sync(h_cecm_svc,
                               &t_set_info));
    }
    else
    {
        return (cecm_ctrl_set(h_cecm_svc,
                              &t_set_info));
    }    
}
INT32 x_cecm_request_terminate_arc(HANDLE_T h_cecm_svc, 
                                          CECM_LOG_ADDR_T e_la,
                                          BOOL    b_sync)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_request_terminate_arc: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_REQ_TERMINATE_ARC;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    if (b_sync)
    {
        return (_cecm_set_sync(h_cecm_svc,
                               &t_set_info));
    }
    else
    {
        return (cecm_ctrl_set(h_cecm_svc,
                              &t_set_info));
    }      
}
INT32 x_cecm_report_init_arc(HANDLE_T h_cecm_svc, 
                                    CECM_LOG_ADDR_T  e_la,
                                    BOOL    b_sync)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_report_init_arc: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_REPORT_INIT_ARC;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    if (b_sync)
    {
        return (_cecm_set_sync(h_cecm_svc,
                               &t_set_info));
    }
    else
    {
        return (cecm_ctrl_set(h_cecm_svc,
                              &t_set_info));
    }      
}
INT32 x_cecm_report_terminate_arc(HANDLE_T h_cecm_svc,
                                         CECM_LOG_ADDR_T e_la,
                                         BOOL    b_sync)
{
    INT32           i4_return;    
    CECM_SET_T      t_set_info;

    DBG_API(("{CECM} x_cecm_report_terminate_arc: la=%d\n\r", e_la));

    /* Check arguments */
    i4_return = _cecm_chk_args(h_cecm_svc, e_la);
    
    if (i4_return != CECMR_OK)
    {
        return (i4_return);
    }    

    t_set_info.ui1_dest_addr = (UINT8)e_la;
    t_set_info.ui2_opcode = CECM_OPCODE_REPORT_TERMINATE_ARC;
    t_set_info.ui4_addi_info = CECM_ADDI_INFO_NONE;
    t_set_info.t_sync_info.h_sync = NULL_HANDLE;
    t_set_info.t_sync_info.t_sync_data.e_sync_op = CECM_SYNC_OP_NONE;

    if (b_sync)
    {
        return (_cecm_set_sync(h_cecm_svc,
                               &t_set_info));
    }
    else
    {
        return (cecm_ctrl_set(h_cecm_svc,
                              &t_set_info));
    }      
}


