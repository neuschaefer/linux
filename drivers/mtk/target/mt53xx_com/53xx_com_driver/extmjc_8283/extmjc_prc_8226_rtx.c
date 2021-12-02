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
 * $RCSfile: extmjc_prc_8226_rtx.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         This file implements the DVD receive and transmit manager.
 *
 * History:
 *           
 *---------------------------------------------------------------------------*/

#ifdef DRV_SUPPORT_EXTMJC
#ifdef DRV_SUPPORT_EXTMJC_UART_PROTOCOL

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "extmjc_debug.h"

#include "extmjc_prc_8226_rtx.h"
#include "extmjc_prc_flow.h"
#include "extmjc_prc.h"

#include "uart_if.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* message type */
#define EXTMJC_PRC_RTX_MSG_TYPE_RX         0
#define EXTMJC_PRC_RTX_MSG_TYPE_TX         1

/* Receive message id */
#define EXTMJC_PRC_RTX_MSG_ID_RX_TIMEOUT   0

/* Transmit message id */
#define EXTMJC_PRC_RTX_MSG_ID_TX_EMPTY     0


/* Receive polling interval */
#define EXTMJC_PRC_RTX_RX_POLL_INTERVAL     ((UINT32) 10) /* 10 ms */

#define EXTMJC_PRC_TX_MTX_TIMEOUT           ((UINT32) 100) /* 100 ms */
#define EXTMJC_PRC_RX_MTX_TIMEOUT           ((UINT32) 100) /* 100 ms */

/* RX thread priority */
#define EXTMJC_PRC_RX_THREAD_PRIORITY           70
#define EXTMJC_PRC_RX_THREAD_STACK_SIZE         4096

#define EXTMJC_PRC_UART_PORT                    1
#define EXTMJC_PRC_UART_SPEED                   RS_232_SPEED_115200
#define EXTMJC_PRC_UART_DATA_LEN                RS_232_DATA_LEN_8
#define EXTMJC_PRC_UART_PARITY                  RS_232_PARITY_NONE
#define EXTMJC_PRC_UART_STOP_BIT                RS_232_STOP_BIT_1

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/ 
extern EXTMJC_PRC_XLS_T            t_extmjc_prc_xls;
extern EXTMJC_PRC_CONFIG_T*        pt_extmjc_prc_config;

EXTMJC_PRC_RTX_T*                  pt_extmjc_prc_rtx;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/* Rx relative functions */
static VOID extmjc_prc_rx_thread(VOID* pv_arg);


/* Tx relative functions */
static VOID extmjc_prc_rtx_tx_lock(EXTMJC_PRC_RTX_T* pt_this);

static VOID extmjc_prc_rtx_tx_unlock(EXTMJC_PRC_RTX_T* pt_this);

static VOID extmjc_prc_rtx_tx_delay_timer_nfy(
    HANDLE_T  t_tm_handle,
    VOID*     pv_tag);



/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_rtx_rx_handler 
 *
 * Description: Receive processing funcion.
 *
 * Inputs:  pt_this     Pointer points to the DVD receive and transmit manager
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/

VOID extmjc_prc_rx_thread(VOID* pv_tag)
{
    UINT32                      i, j;
    UINT8                       ui1_msg_type;
    UINT16                      ui2_msg_len;
    BOOL                        b_reliable_msg;
    UINT8                       ui1_msg_id;
    UINT8                       ui1_msg_free_id;
    INT32                       i4_xls_ret;
    BOOL                        b_short_msg;
    EXTMJC_PRC_RTX_T*           pt_this;
    UINT8*                      pui1_tmp_buf;

    pt_this = pt_extmjc_prc_rtx;

    while (TRUE)
    {
        x_thread_delay(10);
        /* Read data from buffer */

        if (pt_this->b_is_mjc_upgrade == TRUE)
        {
            continue;
        }

        pt_this->ui2_rx_rec_data_size = UART_Read(EXTMJC_PRC_UART_PORT,
                                                  pt_this->pui1_rx_rec_buf,
                                                  EXTMJC_PRC_RTX_RX_BUF_SIZE,
                                                  FALSE);

        /* Switch buffer */
        pui1_tmp_buf = pt_this->pui1_rx_prc_buf;
        pt_this->pui1_rx_prc_buf = pt_this->pui1_rx_rec_buf;
        pt_this->pui1_rx_rec_buf = pui1_tmp_buf;

        pt_this->ui2_rx_prc_data_size = pt_this->ui2_rx_rec_data_size;
        pt_this->ui2_rx_rec_data_size = 0;

        /* Parse data */
        for (i = 0; i < pt_this->ui2_rx_prc_data_size; i++)
        {
            /* To the end of the message */
            if (TRUE == 
                t_extmjc_prc_xls.pf_extmjc_prc_xls_compose_extmsg(pt_this->pui1_rx_prc_buf[i]) )
            {
                 //Printf("\nRTX receive a complete message ");

                while (TRUE)
                {
                    i4_xls_ret = t_extmjc_prc_xls.pf_extmjc_prc_xls_extmsg_to_inmsg(
                                &b_reliable_msg,
                                &b_short_msg,
                                &ui1_msg_id,
                                &ui1_msg_free_id,
                                &ui1_msg_type,
                                pt_this->pui1_rx_inmsg_buf, 
                                pt_this->ui2_rx_inmsg_buf_max_size,
                                &ui2_msg_len);

                    /* Translate the message */
                    if (EXTMJC_PRC_XLS_OK == i4_xls_ret)
                    {
                        LOG(1, "Receive a message type=0x%x, id=%d, free id=%d, len=%d\n", ui1_msg_type, ui1_msg_id, ui1_msg_free_id, ui2_msg_len);
                        //Printf(" type=0x%x, id=%d, free id=%d, len=%d\n\t", ui1_msg_type, ui1_msg_id, ui1_msg_free_id, ui2_msg_len);
                        for (j = 0; j < ui2_msg_len; j++)
                        {
                            //Printf(" 0x%x", pt_this->pui1_rx_inmsg_buf[j]);
                        }

                        //Printf("\n");

                        /* dispatch the message to flow manager */
                        if (0xFF != ui1_msg_type)
                        {
                            if (TRUE == b_reliable_msg)
                            {

                                extmjc_prc_flow_rec_withresp_req(b_short_msg,
                                                                  ui1_msg_id,
                                                                  ui1_msg_free_id,
                                                                  ui1_msg_type,
                                                                  ui2_msg_len,
                                                                  pt_this->pui1_rx_inmsg_buf);
                            }
                            else
                            {
                                extmjc_prc_flow_rec_noresp_req(ui1_msg_type,
                                                                ui2_msg_len,
                                                                pt_this->pui1_rx_inmsg_buf);
                            }
                        }
                        else
                        {
                            /* response message */
                            extmjc_prc_flow_rec_resp(b_short_msg,
                                                   ui1_msg_id, 
                                                   ui2_msg_len,
                                                   pt_this->pui1_rx_inmsg_buf);
                        }

                        break;
                    }
                    else if (EXTMJC_PRC_XLS_INSUFFICIENT_BUF == i4_xls_ret)
                    {
                        x_mem_free(pt_this->pui1_rx_inmsg_buf);

                        pt_this->ui2_rx_inmsg_buf_max_size = ui2_msg_len;

                        pt_this->pui1_rx_inmsg_buf = x_mem_alloc( (SIZE_T) (pt_this->ui2_rx_inmsg_buf_max_size) );

                        ASSERT((NULL != pt_this->pui1_rx_inmsg_buf));

                    }
                    else
                    {
                        LOG(1, "Rece a translate failed msg\n");
                        break;
                    }
                }

                /* Translation failed, reset message compose state machine */
                t_extmjc_prc_xls.pf_extmjc_prc_xls_compose_extmsg_reset();
            }
        }
    }

}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_rtx_tx_lock 
 *
 * Description: Lock the transmit mutex.
 *
 * Inputs:  pt_this     Pointer points to the DVD receive and transmit manager
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_prc_rtx_tx_lock(EXTMJC_PRC_RTX_T* pt_this)
{
    INT32 i4_ret = 0;
    
    /* Lock mutex */
    while (TRUE)
    {
        i4_ret = x_sema_lock_timeout( pt_this->h_tx_mtx, EXTMJC_PRC_TX_MTX_TIMEOUT );
        if (OSR_OK == i4_ret)
        {
            return;
        }

        if (OSR_TIMEOUT == i4_ret)
        {
            continue;
        }

        ASSERT(FALSE);
    }
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_rtx_tx_unlock 
 *
 * Description: Unlock the transmit mutex.
 *
 * Inputs:  pt_this     Pointer points to the DVD receive and transmit manager
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_prc_rtx_tx_unlock(EXTMJC_PRC_RTX_T* pt_this)
{
    INT32 i4_ret = 0;
    
    /* Unlock mutex */
    i4_ret = x_sema_unlock( pt_this->h_tx_mtx );

    if (i4_ret != OSR_OK)
    {
        ASSERT(FALSE); 
    }
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_rtx_tx_delay_timer_nfy 
 *
 * Description: Callback function of the delay timer.
 *
 * Inputs:  t_tm_handle     Handle of the tiemr
 *          pv_tag          Pointer points to the DVD receive and transmit manager
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_prc_rtx_tx_delay_timer_nfy(
    HANDLE_T  t_tm_handle,
    VOID*     pv_tag)
{
    EXTMJC_PRC_MSG_T        t_msg;
    
    ASSERT((NULL != pv_tag));

    t_msg.ui1_msg_type = EXTMJC_PRC_MSG_TYPE_TX_EMPTY;
    t_msg.pt_obj = (VOID*) pv_tag;

    if (OSR_OK != extmjc_prc_send_msg(&t_msg))
    {
        LOG(1, "ERR: Failed sending tx empty message in delay timer\n");
    }
}


VOID extmjc_prc_rtx_set_upgrade(BOOL b_is_mjc_upgrade)
{
    EXTMJC_PRC_RTX_T*       pt_this = pt_extmjc_prc_rtx;
    
    pt_this->b_is_mjc_upgrade = b_is_mjc_upgrade;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_rtx_tx_handler 
 *
 * Description: This function sends data to UART driver
 *
 * Inputs:  pt_this     Pointer points to the DVD receive and transmit manager
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID extmjc_prc_rtx_tx_handler(VOID* pv_tag)
{
    UINT32                  ui4_req_data_size;
    UINT32                  ui4_get_data_size;
    UINT32                  ui4_send_data_size;
    EXTMJC_PRC_RTX_T*       pt_this;
    EXTMJC_PRC_MSG_T        t_msg;

    ASSERT((NULL != pv_tag));

    pt_this = (EXTMJC_PRC_RTX_T *) pv_tag;

    extmjc_prc_rtx_tx_lock(pt_this);
    
    if (0 == pt_this->ui4_tx_extmsg_size)
    {
        extmjc_prc_rtx_tx_unlock(pt_this);
        return;
    }
    
    ui4_req_data_size = 0xFFFFFFFF;
    if (0 < pt_extmjc_prc_config->ui1_tx_delay_time)
    {
        ui4_req_data_size = pt_extmjc_prc_config->ui1_tx_delay_byte_cnt;
    }

    ui4_get_data_size = pt_this->ui4_tx_extmsg_size - pt_this->ui4_tx_extmsg_idx;
    if ( ui4_get_data_size > ui4_req_data_size)
    {
        ui4_get_data_size = ui4_req_data_size;
    }

    
    ui4_send_data_size = UART_Write(EXTMJC_PRC_UART_PORT, 
                                    pt_this->pui1_tx_extmsg_buf + pt_this->ui4_tx_extmsg_idx,
                                    ui4_get_data_size,
                                    TRUE);

    pt_this->ui4_tx_extmsg_idx = pt_this->ui4_tx_extmsg_idx + ui4_send_data_size;

    /* empty */
    if (pt_this->ui4_tx_extmsg_idx == pt_this->ui4_tx_extmsg_size)
    {
        pt_this->b_in_empty_nfy = TRUE;
        pt_this->ui4_tx_extmsg_idx = 0;
        pt_this->ui4_tx_extmsg_size = 0;

        extmjc_prc_flow_tx_empty_nfy();

        pt_this->b_in_empty_nfy = FALSE;
    }
    
    /* transfer complete */
    if (0 == pt_extmjc_prc_config->ui1_tx_delay_time)
    {
        t_msg.ui1_msg_type = EXTMJC_PRC_MSG_TYPE_TX_EMPTY;
        t_msg.pt_obj = (VOID*) pt_this;

        if (OSR_OK != extmjc_prc_send_msg(&t_msg))
        {
            LOG(1, "ERR: Failed sending tx empty message in delay timer\n");
        }
    }
    else
    {
        /* Start the tx_delay timer */
        if (OSR_OK != x_timer_start(pt_this->h_tx_delay_timer, 
                                    (UINT32) pt_extmjc_prc_config->ui1_tx_delay_time,
                                    X_TIMER_FLAG_ONCE, 
                                    extmjc_prc_rtx_tx_delay_timer_nfy,
                                    (VOID *) pt_this))
        {
           LOG(1, "ERR: Failed start delay timer in iom nfy\n"); 
        }
    }
    
    extmjc_prc_rtx_tx_unlock(pt_this);
    return ;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_rtx_send_msg 
 *
 * Description: Sends a message to UART driver.
 *
 * Inputs:  pt_this         Pointer points to the DVD receive and transmit manager
 *          pt_extmjc_prc_xls      Pointer points to the DVD message translator
 *          ui1_inmsg_type  Message type of the message
 *          ui2_inmsg_body_len   Length of byte of data part of the message.
 *          pui1_inmsg_body Pointer points to the data part of the message.
 *
 * Outputs: -
 *
 * Returns: EXTMJC_PRCR_OK                Routine was successful
 *          EXTMJC_PRCR_TX_MSG_QUE_FULL   The output message queue is full
 *          EXTMJC_PRCR_TX_MSG_XLS_FAILED The input data can not be translated into 
 *                                  external format
 ----------------------------------------------------------------------------*/
INT32 extmjc_prc_rtx_send_msg(
    BOOL                b_reliable_inmsg,
    BOOL                b_short_resp,
    UINT8               ui1_inmsg_id,
    UINT8               ui1_inmsg_free_id,
    UINT8               ui1_inmsg_type,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body)
{
    UINT32                  ui4_extmsg_size;
    INT32                   i4_result;
    EXTMJC_PRC_XLS_T*       pt_extmjc_prc_xls = &t_extmjc_prc_xls;
    EXTMJC_PRC_RTX_T*       pt_this = pt_extmjc_prc_rtx;
    INT32                   j;
    

    if (pt_this->b_is_mjc_upgrade == TRUE)
    {
        return (EXTMJC_PRCR_OK);
    }

    LOG(1, "Transmit a message type=0x%x, id=%d, free id=%d, len=%d\n", ui1_inmsg_type, ui1_inmsg_id, ui1_inmsg_free_id, ui2_inmsg_body_len);

    //Printf("\nRTX transmit a message ");
    //Printf(" type=0x%x, id=%d, free id=%d, len=%d\n\t", ui1_inmsg_type, ui1_inmsg_id, ui1_inmsg_free_id, ui2_inmsg_body_len);

    for (j = 0; j < ui2_inmsg_body_len; j++)
    {
        // LOG(0, " 0x%x", pui1_inmsg_body[j]);
        //Printf(" 0x%x", pui1_inmsg_body[j]);
    }
    
    //Printf("\n");
    
    ASSERT((NULL != pt_this));
    
    ASSERT((NULL != pt_extmjc_prc_xls));
    
    extmjc_prc_rtx_tx_lock(pt_this);

    /* verify no message is transmitted */
    if (0 != pt_this->ui4_tx_extmsg_size )
    {
        extmjc_prc_rtx_tx_unlock(pt_this);
        return (EXTMJC_PRCR_TX_MSG_QUE_FULL);
    }

    i4_result = pt_extmjc_prc_xls->pf_extmjc_prc_xls_inmsg_to_extmsg(
        b_reliable_inmsg,
        b_short_resp,
        ui1_inmsg_id,
        ui1_inmsg_free_id,
        ui1_inmsg_type,
        ui2_inmsg_body_len,
        pui1_inmsg_body,
        pt_this->pui1_tx_extmsg_buf,
        pt_this->ui4_tx_extmsg_buf_max_size,
        &ui4_extmsg_size);

    if (EXTMJC_PRC_XLS_INSUFFICIENT_BUF == i4_result)
    {
        /* reallocate */
        x_mem_free(pt_this->pui1_tx_extmsg_buf);

        pt_this->pui1_tx_extmsg_buf = x_mem_alloc(ui4_extmsg_size);

        ASSERT((NULL != pt_this->pui1_tx_extmsg_buf));

        pt_this->ui4_tx_extmsg_buf_max_size = ui4_extmsg_size;

        i4_result = pt_extmjc_prc_xls->pf_extmjc_prc_xls_inmsg_to_extmsg(
            b_reliable_inmsg,
            b_short_resp,
            ui1_inmsg_id,
            ui1_inmsg_free_id,
            ui1_inmsg_type,
            ui2_inmsg_body_len,
            pui1_inmsg_body,
            pt_this->pui1_tx_extmsg_buf,
            pt_this->ui4_tx_extmsg_buf_max_size,
            &ui4_extmsg_size);
    }
    
    if (EXTMJC_PRC_XLS_OK != i4_result)
    {
        extmjc_prc_rtx_tx_unlock(pt_this);
        
        ASSERT(FALSE);

        return (EXTMJC_PRCR_TX_MSG_XLS_FAILED);
    }

    pt_this->ui4_tx_extmsg_size = ui4_extmsg_size;
    pt_this->ui4_tx_extmsg_idx = 0;

    /* send to UART */
    if (FALSE == pt_this->b_in_empty_nfy)
    {
        extmjc_prc_rtx_tx_handler(pt_this);
    }

    extmjc_prc_rtx_tx_unlock(pt_this);

    return (EXTMJC_PRCR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_rtx_init 
 *
 * Description: Initialize the DVD receive and transmit manager.
 *
 * Inputs:  pt_this     Pointer points to the receive and transmit manager.
 *
 * Outputs: -
 *
 * Returns: EXTMJC_PRCR_OK                    Routine successful.
 *          EXTMJC_PRCR_UART_NOT_SUPPORT.     Failed to requested UART 
 ----------------------------------------------------------------------------*/
INT32 extmjc_prc_rtx_init(EXTMJC_PRC_RTX_T* pt_this)
{
    RS_232_SETUP_INFO_T     t_rs232_setup_info;

    /* mjc upgrade */
    pt_this->b_is_mjc_upgrade = FALSE;

    /* Set UART parameter */
    t_rs232_setup_info.e_data_len = EXTMJC_PRC_UART_DATA_LEN;
    t_rs232_setup_info.e_parity = EXTMJC_PRC_UART_PARITY;
    t_rs232_setup_info.e_speed = EXTMJC_PRC_UART_SPEED;
    t_rs232_setup_info.e_stop_bit = EXTMJC_PRC_UART_STOP_BIT;
    UART_SetParameter(EXTMJC_PRC_UART_PORT, &t_rs232_setup_info);
    
    /* Init transmit member data */
    if (OSR_OK != x_sema_create( &(pt_this->h_tx_mtx), X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK))
    {
        ASSERT(FALSE);
    }

    if (0 != pt_extmjc_prc_config->ui1_tx_delay_time)
    {
        if (OSR_OK != x_timer_create( &(pt_this->h_tx_delay_timer) ) )
        {
            ASSERT(FALSE);
        }
    }

    pt_this->pui1_tx_extmsg_buf = x_mem_alloc(EXTMJC_PRC_RTX_TX_EXTMSG_BUF_SIZE);
    if (NULL == pt_this->pui1_tx_extmsg_buf)
    {
        ASSERT(FALSE);
    }

    pt_this->ui4_tx_extmsg_buf_max_size = EXTMJC_PRC_RTX_TX_EXTMSG_BUF_SIZE;
    pt_this->ui4_tx_extmsg_size = 0;
    pt_this->ui4_tx_extmsg_idx = 0;

    pt_this->b_in_empty_nfy = FALSE;

    pt_this->pui1_rx_rec_buf = pt_this->aui1_rx_exmsg_buf[0];
    pt_this->ui2_rx_rec_data_size = 0;
    pt_this->pui1_rx_prc_buf = pt_this->aui1_rx_exmsg_buf[1];
    pt_this->ui2_rx_prc_data_size = 0;
    
    pt_this->ui2_rx_inmsg_buf_max_size = EXTMJC_PRC_RTX_RX_INMSG_BUF_SIZE;
    pt_this->pui1_rx_inmsg_buf = x_mem_alloc( (SIZE_T) (pt_this->ui2_rx_inmsg_buf_max_size) );
    if (NULL == pt_this->pui1_rx_inmsg_buf)
    {
        ASSERT(FALSE);
    }

    /* Init receive member data */
    if (OSR_OK != x_thread_create( &(pt_this->h_rx_thread),
                                   "extmjc_rx_thread",
                                   EXTMJC_PRC_RX_THREAD_STACK_SIZE, /* z_stack_size */
                                   EXTMJC_PRC_RX_THREAD_PRIORITY, /* ui1_priority */
                                   extmjc_prc_rx_thread, /* thread API */
                                   sizeof(pt_this), 
                                   (VOID*) pt_this))

    {
        ASSERT(FALSE);
    }

    return (EXTMJC_PRCR_OK);
}

#endif /* DRV_SUPPORT_EXTMJC_UART_PROTOCOL */

#endif /* DRV_SUPPORT_EXTMJC */
