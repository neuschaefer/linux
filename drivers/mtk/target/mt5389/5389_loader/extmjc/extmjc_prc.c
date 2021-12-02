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
 * $RCSfile: extmjc_prc.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         This file contains all implementations for the DVD communication 
 *         protocol API
 * History:
 *           
 *---------------------------------------------------------------------------*/

#ifdef DRV_SUPPORT_EXTMJC

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "extmjc_debug.h"

#include "extmjc_prc.h"
#include "extmjc_prc_flow.h"
#include "extmjc_prc_8226_rtx.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define EXTMJC_PRC_RESP_MSG_TYPE            0xFF
#define EXTMJC_PRC_CONNECT_MSG_TYPE         0x01

#define EXTMJC_PRC_THREAD_PRIORITY          70
#define EXTMJC_PRC_THREAD_STACK_SIZE        4096
#define EXTMJC_PRC_QUEUE_SIZE               32

#define EXTMJC_PRC_MSG_PRIORITY             0
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/ 

EXTMJC_PRC_XLS_T                    t_extmjc_prc_xls;

extmjc_prc_msg_handler_nfy_fct      g_extmjc_prc_noresp_msg_handler = NULL;
extmjc_prc_msg_handler_nfy_fct      g_extmjc_prc_withresp_msg_handler = NULL;
extmjc_prc_msg_handler_nfy_fct      g_extmjc_prc_resp_msg_handler = NULL;
extmjc_prc_fail_handler_nfy_fct     g_extmjc_prc_resp_fail_handler = NULL;
extmjc_prc_connect_nfy_fct          g_extmjc_prc_connect_handler = NULL;

EXTMJC_PRC_CONFIG_T*                pt_extmjc_prc_config;

HANDLE_T                            h_extmjc_prc_thread;
HANDLE_T                            h_extmjc_prc_que;

extern EXTMJC_PRC_RTX_T*            pt_extmjc_prc_rtx;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static VOID extmjc_prc_thread(VOID* pv_arg);
extern VOID extmjc_ping_fail_handler(VOID);


static VOID extmjc_prc_thread(VOID* pv_arg)
{
    EXTMJC_PRC_MSG_T        t_msg;
    SIZE_T                  z_msg_size;
    UINT16                  ui2_index;

    z_msg_size = sizeof(EXTMJC_PRC_MSG_T);

    while (TRUE)
    {
        if (OSR_OK != x_msg_q_receive(&ui2_index,
                                      (VOID*) (&t_msg),
                                      &z_msg_size,
                                      &h_extmjc_prc_que,
                                      1,
                                      X_MSGQ_OPTION_WAIT))
        {
            LOG(1, "ERR: RTX thread rec message failed\n");
            continue;
        }


        switch (t_msg.ui1_msg_type)
        {
        case EXTMJC_PRC_MSG_TYPE_TX_EMPTY:
            extmjc_prc_rtx_tx_handler(t_msg.pt_obj);
            break;

        case EXTMJC_PRC_MSG_TYPE_TX_RETRY:
            extmjc_prc_flow_timeout_handler(t_msg.pt_obj);
            break;

        case EXTMJC_PRC_MSG_TYPE_PING:
            /* Send Request Here */
            extmjc_prc_send_noresp_req(0x08,
                                       0,
                                       NULL);
            break;

        case EXTMJC_PRC_MSG_TYPE_PING_FAIL:
            extmjc_ping_fail_handler();
            break;

        default:
            break;
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_reg_xls 
 *
 * Description: Register EXTMJC_PRC translator to EXTMJC_PRC manager.
 *
 * Inputs:  pt_extmjc_prc_xls      Pointer points to the interface of EXTMJC_PRC translator
 *
 * Outputs: -
 *
 * Returns: EXTMJC_PRCR_OK        Routine successful.
 ----------------------------------------------------------------------------*/
INT32 extmjc_prc_init(const EXTMJC_PRC_CONFIG_T* pt_extmjc_prc_cfg)
{
    pt_extmjc_prc_config = (EXTMJC_PRC_CONFIG_T *) pt_extmjc_prc_cfg;


    /* Init Process Thread */
    if (OSR_OK != x_msg_q_create( &h_extmjc_prc_que,
                                  "extmjc_prc_que",
                                  sizeof (EXTMJC_PRC_MSG_T), /* message size */
                                  EXTMJC_PRC_QUEUE_SIZE))
    {
        ASSERT(FALSE);
    }
    
    if (OSR_OK != x_thread_create( &h_extmjc_prc_thread,
                                   "extmjc_prc_thread",
                                   EXTMJC_PRC_THREAD_STACK_SIZE, /* z_stack_size */
                                   EXTMJC_PRC_THREAD_PRIORITY, /* ui1_priority */
                                   extmjc_prc_thread, /* thread API */
                                   0, 
                                   NULL))
    {
        ASSERT(FALSE);
    }

    /* Init RTX */
    pt_extmjc_prc_rtx = x_mem_alloc( sizeof(EXTMJC_PRC_RTX_T) );
    extmjc_prc_rtx_init(pt_extmjc_prc_rtx);

    /* Initialize flow manager */
    extmjc_prc_flow_init(pt_extmjc_prc_config->ui1_max_reliable_msg_cnt,
                         pt_extmjc_prc_config->ui1_noresp_req_buf_cnt,
                         pt_extmjc_prc_config->ui1_withresp_req_buf_cnt);
    
    return (EXTMJC_PRCR_OK);
}


INT32 extmjc_prc_send_msg(EXTMJC_PRC_MSG_T* pt_msg)
{
    INT32 i4_ret;

    i4_ret = x_msg_q_send(h_extmjc_prc_que, 
                          (VOID *) pt_msg, 
                          sizeof (EXTMJC_PRC_MSG_T), 
                          EXTMJC_PRC_MSG_PRIORITY);
    
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_reg_xls 
 *
 * Description: Register EXTMJC_PRC translator to EXTMJC_PRC manager.
 *
 * Inputs:  pt_extmjc_prc_xls      Pointer points to the interface of EXTMJC_PRC translator
 *
 * Outputs: -
 *
 * Returns: EXTMJC_PRCR_OK        Routine successful.
 ----------------------------------------------------------------------------*/
INT32 extmjc_prc_reg_xls(const EXTMJC_PRC_XLS_T * pt_extmjc_prc_xls)
{
    if ((NULL == pt_extmjc_prc_xls) ||
        (NULL == pt_extmjc_prc_xls->pf_extmjc_prc_xls_compose_extmsg) ||
        (NULL == pt_extmjc_prc_xls->pf_extmjc_prc_xls_compose_extmsg_reset) ||
        (NULL == pt_extmjc_prc_xls->pf_extmjc_prc_xls_extmsg_to_inmsg) ||
        (NULL == pt_extmjc_prc_xls->pf_extmjc_prc_xls_valid_inmsg) ||
        (NULL == pt_extmjc_prc_xls->pf_extmjc_prc_xls_inmsg_to_extmsg))
    {
        return (EXTMJC_PRCR_INV_ARG);
    }

    t_extmjc_prc_xls = *pt_extmjc_prc_xls;

    return (EXTMJC_PRCR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_is_connect 
 *
 * Description: This function is called by AP and middleware modules to know
 *              the connection status.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE        Connection is ready.
 *          FALSE       Connection is not ready.
 ----------------------------------------------------------------------------*/
BOOL extmjc_prc_is_connect(VOID)
{
    BOOL b_ret = FALSE;

    b_ret = extmjc_prc_flow_is_connect();
    return (b_ret);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_send_noresp_req 
 *
 * Description: This function is called by AP and middleware modules to send 
 *              an internal message to the EXTMJC_PRC controller..
 *
 * Inputs:  ui1_msg_type        Type of the message
 *          ui2_inmsg_body_len         Length of the data part of the message.
 *          pui1_inmsg_body       Pointer points to the data part of the message.
 *
 * Outputs: -
 *
 * Returns: EXTMJC_PRCR_OK                Routine was successful
 *          EXTMJC_PRCR_INV_ARG           An input argument is wrong.
 *          EXTMJC_PRCR_TX_MSG_QUE_FULL   The output message queue is full
 *          EXTMJC_PRCR_TX_MSG_XLS_FAILED The input data can not be translated into 
 *                                  external format
 ----------------------------------------------------------------------------*/
INT32 extmjc_prc_send_noresp_req(
    UINT8   ui1_inmsg_type,
    UINT16  ui2_inmsg_body_len,
    UINT8*  pui1_inmsg_body)
{
    INT32 i4_ret;
    
    /* Verify the input parameters */
    if ((NULL == pui1_inmsg_body) && (0 != ui2_inmsg_body_len))
    {
        LOG(1, "ERR: Send NULL pointer message\n");
        
        return (EXTMJC_PRCR_INV_ARG);
    }

    if ( (EXTMJC_PRC_RESP_MSG_TYPE == ui1_inmsg_type) ||
         (EXTMJC_PRC_CONNECT_MSG_TYPE == ui1_inmsg_type) )
    {
        LOG(1, "ERR: Send wrong type message\n");
        return (EXTMJC_PRCR_INV_ARG);
    }

    i4_ret = extmjc_prc_flow_send_noresp_req(ui1_inmsg_type,
                                          ui2_inmsg_body_len,
                                          pui1_inmsg_body);

    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_send_withresp_req 
 *
 * Description: This function is called by AP and middleware modules to send 
 *              an internal message to the EXTMJC_PRC controller..
 *
 * Inputs:  ui1_msg_type        Type of the message
 *          ui2_inmsg_body_len         Length of the data part of the message.
 *          pui1_inmsg_body       Pointer points to the data part of the message.
 *
 * Outputs: -
 *
 * Returns: EXTMJC_PRCR_OK                Routine was successful
 *          EXTMJC_PRCR_INV_ARG           An input argument is wrong.
 *          EXTMJC_PRCR_TX_MSG_QUE_FULL   The output message queue is full
 *          EXTMJC_PRCR_TX_MSG_XLS_FAILED The input data can not be translated into 
 *                                  external format
 ----------------------------------------------------------------------------*/
INT32 extmjc_prc_send_withresp_req(
    UINT8       ui1_send_retry_cnt,
    UINT16      ui2_send_timeout,
    UINT8       ui1_inmsg_type,
    UINT16      ui2_inmsg_body_len,
    UINT8*      pui1_inmsg_body,
    UINT8*      pui1_inmsg_id)
{
    INT32 i4_ret;
    
    /* Verify the input parameters */
    if ((NULL == pui1_inmsg_body) && (0 != ui2_inmsg_body_len))
    {
        LOG(1, "ERR: Send NULL pointer message\n");

        return (EXTMJC_PRCR_INV_ARG);
    }

    if ( (EXTMJC_PRC_RESP_MSG_TYPE == ui1_inmsg_type) ||
         (EXTMJC_PRC_CONNECT_MSG_TYPE == ui1_inmsg_type) )
    {
        LOG(1, "ERR: Send wrong type message\n");

        return (EXTMJC_PRCR_INV_ARG);
    }

    if (ui2_send_timeout < 200)
    {
        ui2_send_timeout = 200;
    }

    i4_ret = extmjc_prc_flow_send_withresp_req(ui1_send_retry_cnt,
                                            ui2_send_timeout,
                                            ui1_inmsg_type,
                                            ui2_inmsg_body_len,
                                            pui1_inmsg_body,
                                            pui1_inmsg_id);

    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_send_resp 
 *
 * Description: This function is called by AP and middleware modules to send 
 *              an internal message to the EXTMJC_PRC controller..
 *
 * Inputs:  ui1_msg_type        Type of the message
 *          ui2_inmsg_body_len         Length of the data part of the message.
 *          pui1_inmsg_body       Pointer points to the data part of the message.
 *
 * Outputs: -
 *
 * Returns: EXTMJC_PRCR_OK                Routine was successful
 *          EXTMJC_PRCR_INV_ARG           An input argument is wrong.
 *          EXTMJC_PRCR_TX_MSG_QUE_FULL   The output message queue is full
 *          EXTMJC_PRCR_TX_MSG_XLS_FAILED The input data can not be translated into 
 *                                  external format
 ----------------------------------------------------------------------------*/
INT32 extmjc_prc_send_resp(
    UINT8   ui1_inmsg_id,
    UINT8   ui1_inmsg_type,
    UINT16  ui2_inmsg_body_len,
    UINT8*  pui1_inmsg_body)
{
    INT32 i4_ret;
    
    /* Verify the input parameters */
    if (NULL == pui1_inmsg_body)
    {
        LOG(1, "ERR: Send NULL pointer message\n");
        
        return (EXTMJC_PRCR_INV_ARG);
    }

    if ( (EXTMJC_PRC_RESP_MSG_TYPE == ui1_inmsg_type) ||
         (EXTMJC_PRC_CONNECT_MSG_TYPE == ui1_inmsg_type) )
    {
        LOG(1, "ERR: Send wrong type message\n");
        
        return (EXTMJC_PRCR_INV_ARG);
    }

    i4_ret = extmjc_prc_flow_send_resp(ui1_inmsg_id,
                                    ui1_inmsg_type,
                                    ui2_inmsg_body_len,
                                    pui1_inmsg_body);
                                    
    return (i4_ret);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_reg_handler 
 *
 * Description: This function is called by AP and middleware modules to 
 *              register a no-response request message handler to the EXTMJC_PRC manager.
 *
 * Inputs:  pui4_msg_type_table   Pointer points to the message type table of 
 *                                the handler. The message type table is an 
 *                                array composed of eight 32-bits entries.
 *          pf_extmjc_prc_msg_handler    Function pointer of the message handler. 
 *                                This argument can't be NULL.
 *          pv_tag                Pointer points to the tag object. This 
 *                                object is used in the message handler.
 *
 * Outputs: ph_handle   Pointer points to the handle of message handler
 *
 * Returns: EXTMJC_PRCR_OK                        Routine was successful
 *          EXTMJC_PRCR_INV_ARG                   An input argument is wrong
 *          EXTMJC_PRCR_REG_HANDLER_NO_ENTRY      The message handler table is full.
 *          EXTMJC_PRCR_REG_HANDLER_ALLOC_FAILED  Failed to create handle for the message 
 *                                          handler
 ----------------------------------------------------------------------------*/
INT32 extmjc_prc_reg_handler(
    extmjc_prc_msg_handler_nfy_fct     pf_extmjc_prc_noresp_msg_handler,
    extmjc_prc_msg_handler_nfy_fct     pf_extmjc_prc_withresp_msg_handler,
    extmjc_prc_msg_handler_nfy_fct     pf_extmjc_prc_resp_msg_handler,
    extmjc_prc_fail_handler_nfy_fct    pf_extmjc_prc_resp_fail_handler,
    extmjc_prc_connect_nfy_fct         pf_extmjc_prc_connect_handler)
{
    /* Verify the input parameters */
    if ( (NULL == pf_extmjc_prc_noresp_msg_handler) ||
         (NULL == pf_extmjc_prc_withresp_msg_handler) ||
         (NULL == pf_extmjc_prc_resp_msg_handler) ||
         (NULL == pf_extmjc_prc_resp_fail_handler) ||
         (NULL == pf_extmjc_prc_connect_handler) )
    {
        return (EXTMJC_PRCR_INV_ARG);
    }

    g_extmjc_prc_noresp_msg_handler = pf_extmjc_prc_noresp_msg_handler;
    g_extmjc_prc_withresp_msg_handler = pf_extmjc_prc_withresp_msg_handler;
    g_extmjc_prc_resp_msg_handler = pf_extmjc_prc_resp_msg_handler;
    g_extmjc_prc_resp_fail_handler = pf_extmjc_prc_resp_fail_handler;
    g_extmjc_prc_connect_handler = pf_extmjc_prc_connect_handler;

    return (EXTMJC_PRCR_OK);
}

INT32 extmjc_req_disp_disp_noresp_req(
    UINT8                ui1_msg_type,
    UINT16               ui2_msg_body_len,
    UINT8*               pui1_msg_body)
{
    if (NULL == g_extmjc_prc_noresp_msg_handler)
    {
        return (EXTMJC_PRCR_REG_HANDLER_NO_ENTRY);
    }
    
    g_extmjc_prc_noresp_msg_handler(
        0,
        ui1_msg_type,
        ui2_msg_body_len,
        pui1_msg_body);
    return (EXTMJC_PRCR_OK);
}

INT32 extmjc_req_disp_disp_withresp_req(
    UINT8                ui1_msg_id,
    UINT8                ui1_msg_type,
    UINT16               ui2_msg_body_len,
    UINT8*               pui1_msg_body)
{
    if (NULL == g_extmjc_prc_withresp_msg_handler)
    {
        return (EXTMJC_PRCR_REG_HANDLER_NO_ENTRY);
    }
    
    g_extmjc_prc_withresp_msg_handler(
        ui1_msg_id,
        ui1_msg_type,
        ui2_msg_body_len,
        pui1_msg_body);

    return (EXTMJC_PRCR_OK);
}

INT32 extmjc_resp_disp_disp_resp(
    UINT8                ui1_msg_id,
    UINT8                ui1_msg_type,
    UINT16               ui2_msg_body_len,
    UINT8*               pui1_msg_body)
{
    if (NULL == g_extmjc_prc_resp_msg_handler)
    {
        return (EXTMJC_PRCR_REG_HANDLER_NO_ENTRY);
    }
    
    g_extmjc_prc_resp_msg_handler(
        ui1_msg_id,
        ui1_msg_type,
        ui2_msg_body_len,
        pui1_msg_body);
    
    return (EXTMJC_PRCR_OK);
}

INT32 extmjc_resp_disp_fail_notify(
    UINT8                ui1_msg_id,
    UINT8                ui1_msg_type)
{
    if (NULL == g_extmjc_prc_resp_fail_handler)
    {
        return (EXTMJC_PRCR_REG_HANDLER_NO_ENTRY);
    }
    
    g_extmjc_prc_resp_fail_handler(
        ui1_msg_id,
        ui1_msg_type);

    return (EXTMJC_PRCR_OK);
}


INT32 extmjc_prc_connect_notify(
    BOOL                b_connect)
{
    if (NULL == g_extmjc_prc_connect_handler)
    {
        return (EXTMJC_PRCR_REG_HANDLER_NO_ENTRY);
    }

    g_extmjc_prc_connect_handler(b_connect);
    return (EXTMJC_PRCR_OK);
}

#endif /* DRV_SUPPORT_EXTMJC */
