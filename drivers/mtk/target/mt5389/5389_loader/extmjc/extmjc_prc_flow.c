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
 * $RCSfile: extmjc_prc_flow.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         
 * History:
 *           
 *---------------------------------------------------------------------------*/

#ifdef DRV_SUPPORT_EXTMJC

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "extmjc_debug.h"

#include "extmjc_prc.h"
#include "extmjc_prc_8226_rtx.h"
#include "extmjc_prc_flow.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define EXTMJC_PRC_FLOW_MAX_MSG_CNT                7
#define EXTMJC_PRC_FLOW_SHORT_MSG_BODY_SIZE        50

#define EXTMJC_LONG_MSG_FIRST_MSG_TYPE                 ((UINT8) 0xD0)

typedef struct _EXTMJC_FLOW_MSGID_QUE_T
{
    UINT8       ui1_used_msgid_mask;

    UINT8       ui1_used_msgid_short_mask;

    UINT8       ui1_used_msgid_cnt;

    UINT8       ui1_max_used_msgid_cnt;
} EXTMJC_FLOW_MSGID_QUE_T;


#define EXTMJC_FLOW_TX_NR_REQ      0
#define EXTMJC_FLOW_TX_WR_REQ      1
#define EXTMJC_FLOW_TX_RESP        2

typedef struct
{
    UINT8   ui1_msgid;
    UINT8   ui1_msg_source;
} EXTMJC_FLOW_TX_INFO_T;


#define EXTMJC_FLOW_TX_INFO_QUE_APPEND_FAILED  0


typedef struct
{
    EXTMJC_FLOW_TX_INFO_T* pt_extmjc_flow_tx_info;    

    UINT8               ui1_max_cnt;
    UINT8               ui1_head;
    UINT8               ui1_tail;
    UINT8               ui1_cnt;
} EXTMJC_FLOW_TX_INFO_QUE_T;


#define EXTMJC_PRC_FLOW_SHORT_MSG_SIZE 50

typedef struct _EXTMJC_FLOW_NR_REQ_T
{
    UINT8       ui1_msgtype;

    UINT16      ui2_msg_body_len;

    UINT16      ui2_msg_body_max_len;

    UINT8*      pui1_msg_body;

} EXTMJC_FLOW_NR_REQ_T;

typedef struct _EXTMJC_FLOW_NR_REQ_QUE_T
{
    UINT8                   ui1_cnt;

    UINT8                   ui1_max_cnt;

    UINT8                   ui1_head;

    UINT8                   ui1_tail;

    EXTMJC_FLOW_NR_REQ_T*      pt_msg_que;

} EXTMJC_FLOW_NR_REQ_QUE_T;


typedef struct _EXTMJC_FLOW_WR_REQ_T
{
    UINT8       ui1_msgtype;

    UINT8       ui1_msgseq;

    UINT8       ui1_msgid;

    UINT8       ui1_free_msgid;

    UINT8       ui1_retry_cnt;

    BOOL        b_is_send;

    UINT16      ui2_retry_timeout;

    UINT16      ui2_cur_timeout;

    UINT16      ui2_msg_body_len;

    UINT16      ui2_msg_body_max_len;

    UINT8*      pui1_msg_body;

} EXTMJC_FLOW_WR_REQ_T;

typedef struct _EXTMJC_FLOW_WR_REQ_WAIT_QUE_T
{
    UINT8                   ui1_cnt;

    UINT8                   ui1_max_cnt;

    UINT8                   ui1_head;

    UINT8                   ui1_tail;

    EXTMJC_FLOW_WR_REQ_T*      pt_msg_que;
} EXTMJC_FLOW_WR_REQ_WAIT_QUE_T;

typedef struct _EXTMJC_FLOW_WR_REQ_QUE_T
{
    UINT8                   aui1_msg_idx[EXTMJC_PRC_FLOW_MAX_MSG_CNT + 1];

    UINT8                   ui1_msgid_mask;

    UINT8                   ui1_free_que_idx_mask;

    UINT8                   ui1_cnt;

    UINT8                   ui1_max_cnt;

    EXTMJC_FLOW_WR_REQ_T*      pt_msg_que;

} EXTMJC_FLOW_WR_REQ_QUE_T;

typedef struct _EXTMJC_FLOW_RESP_T
{
    UINT8       ui1_msgid;

    BOOL        b_short_msg;

    UINT16      ui2_msg_body_len;

    UINT16      ui2_msg_body_max_len;

    UINT8*      pui1_msg_body;

} EXTMJC_FLOW_RESP_T;

typedef struct _EXTMJC_FLOW_RESP_QUE_T
{
    UINT8                   aui1_msg_idx[EXTMJC_PRC_FLOW_MAX_MSG_CNT + 1];

    UINT8                   ui1_msgid_mask;

    UINT8                   ui1_free_que_idx_mask;

    UINT8                   ui1_cnt;

    UINT8                   ui1_max_cnt;

    EXTMJC_FLOW_RESP_T*        pt_msg_que;

} EXTMJC_FLOW_RESP_QUE_T;


#define EXTMJC_PRC_FLOW_DISCONNECT         ((UINT8) 0)
#define EXTMJC_PRC_FLOW_CONNECTING         ((UINT8) 1)
#define EXTMJC_PRC_FLOW_CONNECT            ((UINT8) 2)

#define EXTMJC_PRC_FLOW_INIT_CONNECT_PERIOD    ((UINT32) 1500) /* 1.5 sec */
#define EXTMJC_PRC_FLOW_CONNECT_TIMEOUT        ((UINT32) 5000) /* 5 sec */
#define EXTMJC_FLOW_LOCK_MTX_TIMEOUT           ((UINT32) 100)  /* 100 ms */
#define EXTMJC_FLOW_WR_REQ_SEND_FAIL_MAX_CNT   ((UINT8)  10)   
#define EXTMJC_PRC_FLOW_TIMEOUT_INTERVAL       ((UINT32) 100) /* 100 ms */


typedef struct _EXTMJC_PRC_FLOW_T
{
    EXTMJC_FLOW_MSGID_QUE_T        t_rec_req_id_que;

    EXTMJC_FLOW_MSGID_QUE_T        t_rec_resp_id_que;
    
    EXTMJC_FLOW_WR_REQ_WAIT_QUE_T  t_tx_wr_req_wait_que;

    EXTMJC_FLOW_WR_REQ_QUE_T       t_tx_wr_req_que;

    EXTMJC_FLOW_RESP_QUE_T         t_tx_resp_que;

    EXTMJC_FLOW_NR_REQ_QUE_T       t_tx_nr_req_que;
    
    EXTMJC_FLOW_TX_INFO_QUE_T      t_tx_info_que;

    HANDLE_T                    h_flow_mtx;

    HANDLE_T                    h_timeout_timer;

    UINT8                       ui1_req_msgid;

    UINT8                       ui1_req_seq;

    UINT8                       ui1_status;

    UINT8                       ui1_max_reliable_msg_cnt;

    UINT16                      ui2_connect_req_timeout;

    UINT8                       ui1_wr_req_send_fail_cnt;
} EXTMJC_PRC_FLOW_T;

#define EXTMJC_PRC_FLOW_INC_MSG_ID(ui1_id) ( (ui1_id + 1) % EXTMJC_PRC_FLOW_MAX_MSG_CNT )

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

extern EXTMJC_PRC_XLS_T                t_extmjc_prc_xls;

EXTMJC_PRC_FLOW_T*     pt_extmjc_prc_flow = NULL;

const UINT8 aui1_extmjc_flow_msg_que_mask[EXTMJC_PRC_FLOW_MAX_MSG_CNT + 1] =
{
    0x00,
    0x01,
    0x03,
    0x07,
    0x0F,
    0x1F,
    0x3E,
    0x7E
};

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/* Message ID QUEUE */
static VOID extmjc_flow_msgid_que_init(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this);

static VOID extmjc_flow_msgid_que_reset(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this,
    UINT8                   ui1_max_used_msgid_cnt);

static BOOL extmjc_flow_msgid_que_search_msgid(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this,
    UINT8                   ui1_msgid);

static BOOL extmjc_flow_msgid_que_search_msgid_info(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this,
    UINT8                   ui1_msgid,
    BOOL*                   pb_short_msg);

static UINT8 extmjc_flow_msgid_que_get_used_id_mask(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this);

static BOOL extmjc_flow_msgid_que_add_msg(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this,
    UINT8                   ui1_msgid,
    BOOL                    b_short_msg);

static BOOL extmjc_flow_msgid_que_remove_msg(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this,
    UINT8                   ui1_msgid);

static UINT8 extmjc_flow_msgid_que_get_and_remove_msg(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this);

/* TX INFO Queue */
static VOID extmjc_flow_tx_info_que_init(
    EXTMJC_FLOW_TX_INFO_QUE_T* pt_this);

static VOID extmjc_flow_tx_info_que_reset(
    EXTMJC_FLOW_TX_INFO_QUE_T* pt_this,
    UINT8                   ui1_max_cnt);

static UINT8 extmjc_flow_tx_info_que_append(
    EXTMJC_FLOW_TX_INFO_QUE_T* pt_this,
    UINT8                   ui1_msgid,
    UINT8                   ui1_source);

static BOOL extmjc_flow_tx_info_que_peek(
    EXTMJC_FLOW_TX_INFO_QUE_T* pt_this,
    UINT8*                  pui1_msgid,
    UINT8*                  pui1_source);

static BOOL extmjc_flow_tx_info_que_retrieve(
    EXTMJC_FLOW_TX_INFO_QUE_T* pt_this,
    UINT8*                  pui1_msgid,
    UINT8*                  pui1_source);


/* With Response Request Message */
static VOID extmjc_flow_wr_req_init(
    EXTMJC_FLOW_WR_REQ_T *pt_this);

static VOID extmjc_flow_wr_req_free(
    EXTMJC_FLOW_WR_REQ_T *pt_this);

static VOID extmjc_flow_wr_req_set(
    EXTMJC_FLOW_WR_REQ_T*      pt_this,
    UINT8                   ui1_retry_cnt,
    UINT16                  ui2_retry_timeout,
    UINT8                   ui1_msgseq,
    UINT8                   ui1_msgid,
    UINT8                   ui1_free_msgid,
    UINT8                   ui1_msgtype,
    UINT16                  ui2_msg_body_len,
    UINT8*                  pui1_msg_body);

static VOID extmjc_flow_wr_req_send(
    EXTMJC_FLOW_WR_REQ_T*      pt_this);

/* With Response Request Message Wait Queue */
static VOID extmjc_flow_wr_req_wait_que_init(
    EXTMJC_FLOW_WR_REQ_WAIT_QUE_T* pt_this,
    UINT8                       ui1_max_cnt);

static VOID extmjc_flow_wr_req_wait_que_reset(
    EXTMJC_FLOW_WR_REQ_WAIT_QUE_T* pt_this);

static BOOL extmjc_flow_wr_req_wait_que_has_free_entry(
    EXTMJC_FLOW_WR_REQ_WAIT_QUE_T*     pt_this);

static EXTMJC_FLOW_WR_REQ_T* extmjc_flow_wr_req_wait_que_append(
    EXTMJC_FLOW_WR_REQ_WAIT_QUE_T*  pt_this);

static EXTMJC_FLOW_WR_REQ_T* extmjc_flow_wr_req_wait_que_get_msg(
    EXTMJC_FLOW_WR_REQ_WAIT_QUE_T*  pt_this);

static VOID extmjc_flow_wr_req_wait_que_remove(
    EXTMJC_FLOW_WR_REQ_WAIT_QUE_T*  pt_this);

/* With response Request Message Queue */
static VOID extmjc_flow_wr_req_que_init(
    EXTMJC_FLOW_WR_REQ_QUE_T*     pt_this);

static BOOL extmjc_flow_wr_req_que_has_free_entry(
    EXTMJC_FLOW_WR_REQ_QUE_T*     pt_this);

static EXTMJC_FLOW_WR_REQ_T* extmjc_flow_wr_req_que_get_msg(
    EXTMJC_FLOW_WR_REQ_QUE_T*  pt_this,
    UINT8                   ui1_msgid);

static EXTMJC_FLOW_WR_REQ_T* extmjc_flow_wr_req_que_alloc(
    EXTMJC_FLOW_WR_REQ_QUE_T*  pt_this,
    UINT8                   ui1_msgid);

static VOID extmjc_flow_wr_req_que_free(
    EXTMJC_FLOW_WR_REQ_QUE_T*  pt_this,
    UINT8                   ui1_msgid);

/* Response Message */
static VOID extmjc_flow_resp_init(
    EXTMJC_FLOW_RESP_T *pt_this);

static VOID extmjc_flow_resp_free(
    EXTMJC_FLOW_RESP_T *pt_this);

static VOID extmjc_flow_resp_set(
    EXTMJC_FLOW_RESP_T*        pt_this,
    BOOL                    b_short_msg,
    UINT8                   ui1_msgid,
    UINT16                  ui2_msg_body_len,
    UINT8*                  pui1_msg_body);

static VOID extmjc_flow_resp_send(
    EXTMJC_FLOW_RESP_T*      pt_this);


/* Response Message Queue */
static VOID extmjc_flow_resp_que_init(
    EXTMJC_FLOW_RESP_QUE_T*     pt_this);

static VOID extmjc_flow_resp_que_reset(
    EXTMJC_FLOW_RESP_QUE_T*    pt_this,
    UINT8                   ui1_max_cnt);

static BOOL extmjc_flow_resp_que_has_free_entry(
    EXTMJC_FLOW_RESP_QUE_T*     pt_this);

static EXTMJC_FLOW_RESP_T* extmjc_flow_resp_que_get_msg(
    EXTMJC_FLOW_RESP_QUE_T*    pt_this,
    UINT8                   ui1_msgid);

static EXTMJC_FLOW_RESP_T* extmjc_flow_resp_que_alloc(
    EXTMJC_FLOW_RESP_QUE_T*    pt_this,
    UINT8                   ui1_msgid);

static VOID extmjc_flow_resp_que_free(
    EXTMJC_FLOW_RESP_QUE_T*    pt_this,
    UINT8                   ui1_msgid);

/* No Response Request */
static VOID extmjc_flow_nr_req_init(
    EXTMJC_FLOW_NR_REQ_T *pt_this);

static VOID extmjc_flow_nr_req_set(
    EXTMJC_FLOW_NR_REQ_T*      pt_this,
    UINT8                   ui1_msgtype,
    UINT16                  ui2_msg_body_len,
    UINT8*                  pui1_msg_body);

static VOID extmjc_flow_nr_req_send(
    EXTMJC_FLOW_NR_REQ_T*      pt_this);

/* No Response Request Queue */
static VOID extmjc_flow_nr_req_que_init(
    EXTMJC_FLOW_NR_REQ_QUE_T*  pt_this,
    UINT8                   ui1_max_cnt);

static VOID extmjc_flow_nr_req_que_reset(
    EXTMJC_FLOW_NR_REQ_QUE_T*  pt_this);

static BOOL extmjc_flow_nr_req_que_has_free_entry(
    EXTMJC_FLOW_NR_REQ_QUE_T*     pt_this);

static EXTMJC_FLOW_NR_REQ_T* extmjc_flow_nr_req_que_append(
    EXTMJC_FLOW_NR_REQ_QUE_T*  pt_this);

static EXTMJC_FLOW_NR_REQ_T* extmjc_flow_nr_req_que_get_msg(
    EXTMJC_FLOW_NR_REQ_QUE_T*  pt_this);

static VOID extmjc_flow_nr_req_que_remove(
    EXTMJC_FLOW_NR_REQ_QUE_T*  pt_this);

/* dvd flow manager */
static VOID extmjc_prc_flow_lock(EXTMJC_PRC_FLOW_T* pt_this);

static VOID extmjc_prc_flow_unlock(EXTMJC_PRC_FLOW_T* pt_this);

static VOID extmjc_prc_flow_establish_connect(
    EXTMJC_PRC_FLOW_T* pt_this,
    UINT16              ui2_period);

static BOOL extmjc_prc_flow_proc_connect_req(
    EXTMJC_PRC_FLOW_T* pt_this,
    UINT8               ui1_inmsg_id,
    UINT8               ui1_inmsg_type,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body);

static VOID extmjc_prc_flow_resend_wr_req(
    EXTMJC_PRC_FLOW_T*     pt_this,
    EXTMJC_FLOW_WR_REQ_QUE_T*  pt_wr_req_que,
    UINT16                  ui2_period);


/* 
    Message ID Queue 
*/

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_msgid_que_init 
 *
 * Description: Initialize DVD message ID queue.
 *
 * Inputs:  pt_this     Pointer points to the DVD mesage ID queue
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_flow_msgid_que_init(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this)
{
    pt_this->ui1_used_msgid_mask = 0;
    pt_this->ui1_used_msgid_cnt = 0;
    pt_this->ui1_max_used_msgid_cnt = 0;
    pt_this->ui1_used_msgid_short_mask = 0;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_msgid_que_reset 
 *
 * Description: Reset DVD message ID queue.
 *
 * Inputs:  pt_this                 Pointer points to the DVD mesage ID queue
 *          ui1_max_used_msgid_cnt  Maximum number of message id in the queue.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_flow_msgid_que_reset(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this,
    UINT8                   ui1_max_used_msgid_cnt)
{
    pt_this->ui1_used_msgid_mask = 0;
    pt_this->ui1_used_msgid_cnt = 0;
    pt_this->ui1_max_used_msgid_cnt = ui1_max_used_msgid_cnt;
    pt_this->ui1_used_msgid_short_mask = 0;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_msgid_que_search_msgid 
 *
 * Description: Search a message by message id in DVD message ID queue.
 *
 * Inputs:  pt_this    Pointer points to the DVD mesage ID queue
 *          ui1_msgid  ID field of the message
 *
 * Outputs: -
 *
 * Returns: If the message id is in the queue, TRUE is return. 
 *          Otherwise, FALSE is return.
 ----------------------------------------------------------------------------*/
static BOOL extmjc_flow_msgid_que_search_msgid(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this,
    UINT8                   ui1_msgid)
{
    UINT8 ui1_mask;

    ui1_mask = (UINT8) 1 << ui1_msgid;
    if (0 != (ui1_mask & pt_this->ui1_used_msgid_mask))
    {
        return (TRUE);
    }

    return (FALSE);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_msgid_que_search_msgid_info 
 *
 * Description: Search a message by message id in DVD message ID queue.
 *
 * Inputs:  pt_this    Pointer points to the DVD mesage ID queue
 *          ui1_msgid  ID field of the message
 *
 * Outputs: -
 *
 * Returns: If the message id is in the queue, TRUE is return. 
 *          Otherwise, FALSE is return.
 ----------------------------------------------------------------------------*/
static BOOL extmjc_flow_msgid_que_search_msgid_info(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this,
    UINT8                   ui1_msgid,
    BOOL*                   pb_short_msg)
{
    UINT8 ui1_mask;

    *pb_short_msg = FALSE;

    ui1_mask = (UINT8) 1 << ui1_msgid;
    if (0 != (ui1_mask & pt_this->ui1_used_msgid_mask))
    {
        if (0 != (ui1_mask & pt_this->ui1_used_msgid_short_mask))
        {
            *pb_short_msg = TRUE;
        }

        return (TRUE);
    }

    return (FALSE);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_msgid_que_get_used_id_mask 
 *
 * Description: Get a mask of message ids in the queue.
 *
 * Inputs:  pt_this    Pointer points to the DVD mesage ID queue
 *
 * Outputs: -
 *
 * Returns: The mask of message ids
 ----------------------------------------------------------------------------*/
static UINT8 extmjc_flow_msgid_que_get_used_id_mask(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this)
{
    return (pt_this->ui1_used_msgid_mask);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_msgid_que_add_msg 
 *
 * Description: Append a message id into the message id queue.
 *
 * Inputs:  pt_this    Pointer points to the DVD mesage ID queue
 *          ui1_msgid  ID field of the message
 *
 * Outputs: -
 *
 * Returns: If the action is successfully, TRUE is return. Otherwise, FALSE is return.
 ----------------------------------------------------------------------------*/
static BOOL extmjc_flow_msgid_que_add_msg(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this,
    UINT8                   ui1_msgid,
    BOOL                    b_short_msg)
{
    UINT8 ui1_mask;

    if (pt_this->ui1_used_msgid_cnt >= pt_this->ui1_max_used_msgid_cnt)
    {
        return (FALSE);
    }

    ui1_mask = (UINT8) 1 << ui1_msgid;
    
    if (0 == (ui1_mask & pt_this->ui1_used_msgid_mask))
    {
        pt_this->ui1_used_msgid_cnt++;
        pt_this->ui1_used_msgid_mask = 
            pt_this->ui1_used_msgid_mask | ui1_mask;

        if (TRUE == b_short_msg)
        {
            pt_this->ui1_used_msgid_short_mask = 
                pt_this->ui1_used_msgid_short_mask | ui1_mask;
        }
        else
        {
            pt_this->ui1_used_msgid_short_mask = 
                pt_this->ui1_used_msgid_short_mask & (~ui1_mask);
        }

        return (TRUE);
    }
    
    return (FALSE);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_msgid_que_remove_msg 
 *
 * Description: Remove a message id from the message id queue.
 *
 * Inputs:  pt_this    Pointer points to the DVD mesage ID queue
 *          ui1_msgid  ID field of the message
 *
 * Outputs: -
 *
 * Returns: If the action is successfully, TRUE is returned. Otherwise, FALSE is returned.
 ----------------------------------------------------------------------------*/
static BOOL extmjc_flow_msgid_que_remove_msg(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this,
    UINT8                   ui1_msgid)
{
    UINT8 ui1_mask;

    if (0 == pt_this->ui1_used_msgid_cnt)
    {
        return (FALSE);
    }

    ui1_mask = ((UINT8) 1) << ui1_msgid;

    if (0 == (ui1_mask & pt_this->ui1_used_msgid_mask))
    {
        return (FALSE);
    }

    pt_this->ui1_used_msgid_mask =
        pt_this->ui1_used_msgid_mask & (~ui1_mask);

    pt_this->ui1_used_msgid_cnt--;

    return (TRUE);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_msgid_que_remove_msg 
 *
 * Description: Get a message id from the message id queue and remove it from the
 *              queue.
 *
 * Inputs:  pt_this    Pointer points to the DVD mesage ID queue
 *
 * Outputs: -
 *
 * Returns: Return the message id. If no message in the message id queue, 
 *          0 is returned.
 ----------------------------------------------------------------------------*/
static UINT8 extmjc_flow_msgid_que_get_and_remove_msg(
    EXTMJC_FLOW_MSGID_QUE_T*   pt_this)
{
    UINT8 i;
    UINT8 ui1_used_msgid_mask;
    UINT8 ui1_msgid_mask;

    if ( (0 == pt_this->ui1_used_msgid_cnt) ||
         (0 == pt_this->ui1_used_msgid_mask) )
    {
        return (0);
    }

    ui1_used_msgid_mask = pt_this->ui1_used_msgid_mask >> 1;
    for (i = 0; i < EXTMJC_PRC_FLOW_MAX_MSG_CNT; i++)
    {
        if (0 != (ui1_used_msgid_mask & 0x01))
        {
            ui1_msgid_mask = (UINT8) 1 << (i + 1);

            pt_this->ui1_used_msgid_mask = pt_this->ui1_used_msgid_mask 
                & ~ui1_msgid_mask;

            pt_this->ui1_used_msgid_cnt--;

            return (i + 1);
        }
        ui1_used_msgid_mask = ui1_used_msgid_mask >> 1;
    }

    ASSERT(FALSE);

    return (0);
}

/*
    TX INFO QUEUE
*/
/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_tx_info_que_init 
 *
 * Description: Initialize DVD transmit information queue
 *
 * Inputs:  pt_this    Pointer points to the DVD transmit information queue
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_flow_tx_info_que_init(
    EXTMJC_FLOW_TX_INFO_QUE_T* pt_this)
{
    pt_this->pt_extmjc_flow_tx_info = NULL;
    pt_this->ui1_head = 0;
    pt_this->ui1_tail = 0;
    pt_this->ui1_cnt = 0;
    pt_this->ui1_max_cnt = 0;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_tx_info_que_reset 
 *
 * Description: Reset DVD transmit information queue
 *
 * Inputs:  pt_this     Pointer points to the DVD transmit information queue
 *          ui1_max_cnt Maximum number of message in the transmit information queue
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_flow_tx_info_que_reset(
    EXTMJC_FLOW_TX_INFO_QUE_T* pt_this,
    UINT8                   ui1_max_cnt)
{
    if (NULL != pt_this->pt_extmjc_flow_tx_info)
    {
        x_mem_free(pt_this->pt_extmjc_flow_tx_info);
    }

    pt_this->pt_extmjc_flow_tx_info = x_mem_alloc( sizeof(EXTMJC_FLOW_TX_INFO_T) * ui1_max_cnt);

    ASSERT((NULL != pt_this->pt_extmjc_flow_tx_info));

    pt_this->ui1_head = 0;
    pt_this->ui1_tail = 0;
    pt_this->ui1_cnt = 0;
    pt_this->ui1_max_cnt = ui1_max_cnt;
}


/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_tx_info_que_append 
 *
 * Description: Append a transmit info at DVD transmit information queue
 *
 * Inputs:  pt_this     Pointer points to the DVD transmit information queue
 *          ui1_msgid   Message id of the message
 *          ui1_source  Types of the message.
 *
 * Outputs: -
 *
 * Returns: Return the index of the message in the queue. If action is failed,
 *          0 is returned.
 ----------------------------------------------------------------------------*/
static UINT8 extmjc_flow_tx_info_que_append(
    EXTMJC_FLOW_TX_INFO_QUE_T* pt_this,
    UINT8                   ui1_msgid,
    UINT8                   ui1_source)
{
    UINT8 ui1_ret;
    
    /* buffer full */
    if (pt_this->ui1_cnt == pt_this->ui1_max_cnt)
    {
        return (0);
    }

    pt_this->pt_extmjc_flow_tx_info[pt_this->ui1_head].ui1_msgid = ui1_msgid;
    pt_this->pt_extmjc_flow_tx_info[pt_this->ui1_head].ui1_msg_source = ui1_source;
    pt_this->ui1_head = (pt_this->ui1_head + 1) % pt_this->ui1_max_cnt;
    pt_this->ui1_cnt++;
    
    /*
    DVDM_DBG_STMT("Module:%08x Sub:%02x tx que append msgid=%d source=%d",
               DBG_MOD_EXTMJC_MNGR, EXTMJC_MNGR_FLOW_ID, ui1_msgid, ui1_source);
    */
    ui1_ret = pt_this->ui1_cnt;
    return (ui1_ret);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_tx_info_que_peek 
 *
 * Description: Peek first transmit info at DVD transmit information queue
 *
 * Inputs:  pt_this     Pointer points to the DVD transmit information queue
 *
 * Outputs: 
 *          pui1_msgid   Message id of the message
 *          pui1_source  Types of the message.
 *
 * Returns: If no entry is in the queue, FALSE is returned. Otherwise, 
 *          TRUE is returned.
 ----------------------------------------------------------------------------*/
static BOOL extmjc_flow_tx_info_que_peek(
    EXTMJC_FLOW_TX_INFO_QUE_T* pt_this,
    UINT8*                  pui1_msgid,
    UINT8*                  pui1_source)
{
    if (0 == pt_this->ui1_cnt)
    {
        return (FALSE);
    }

    *pui1_msgid = pt_this->pt_extmjc_flow_tx_info[pt_this->ui1_tail].ui1_msgid;
    *pui1_source = pt_this->pt_extmjc_flow_tx_info[pt_this->ui1_tail].ui1_msg_source;

    return (TRUE);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_tx_info_que_retrieve 
 *
 * Description: Remove first transmit info at DVD transmit information queue
 *
 * Inputs:  pt_this     Pointer points to the DVD transmit information queue
 *
 * Outputs: 
 *          pui1_msgid   Message id of the message
 *          pui1_source  Types of the message.
 * 
 * Returns: If no entry is in the queue, FALSE is returned. Otherwise, 
 *          TRUE is returned.
 ----------------------------------------------------------------------------*/
static BOOL extmjc_flow_tx_info_que_retrieve(
    EXTMJC_FLOW_TX_INFO_QUE_T* pt_this,
    UINT8*                  pui1_msgid,
    UINT8*                  pui1_source)
{
    if (0 == pt_this->ui1_cnt)
    {
        return (FALSE);
    }

    *pui1_msgid = pt_this->pt_extmjc_flow_tx_info[pt_this->ui1_tail].ui1_msgid;
    *pui1_source = pt_this->pt_extmjc_flow_tx_info[pt_this->ui1_tail].ui1_msg_source;
    pt_this->ui1_tail = (pt_this->ui1_tail + 1) % pt_this->ui1_max_cnt;
    pt_this->ui1_cnt--;

    /*
    DVDM_DBG_STMT("Module:%08x Sub:%02x tx que retrieve msgid=%d source=%d",
               DBG_MOD_EXTMJC_MNGR, EXTMJC_MNGR_FLOW_ID, *pui1_msgid, *pui1_source);
    */

    return (TRUE);
}
    

/* With Response Request Message */

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_init 
 *
 * Description: Inititalize a with-response request
 *
 * Inputs:  pt_this     Pointer points to the with-response request
 *
 * Outputs: -
 * 
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_flow_wr_req_init(
    EXTMJC_FLOW_WR_REQ_T *pt_this)
{
    pt_this->ui1_retry_cnt = 0;
    pt_this->ui2_retry_timeout = 0;
    pt_this->ui2_cur_timeout = 0;
    pt_this->b_is_send = FALSE;

    pt_this->ui1_msgid = 0;
    pt_this->ui1_free_msgid = 0;
    pt_this->ui1_msgtype = 0;
    pt_this->ui2_msg_body_len = 0;
    pt_this->ui2_msg_body_max_len = EXTMJC_PRC_FLOW_SHORT_MSG_SIZE;
    pt_this->pui1_msg_body = (UINT8 *) x_mem_alloc(EXTMJC_PRC_FLOW_SHORT_MSG_SIZE);

    ASSERT((NULL != pt_this->pui1_msg_body));
    return ;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_free 
 *
 * Description: Free a with-response request
 *
 * Inputs:  pt_this     Pointer points to the with-response request
 *
 * Outputs: -
 * 
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_flow_wr_req_free(
    EXTMJC_FLOW_WR_REQ_T *pt_this)
{
    pt_this->ui1_retry_cnt = 0;
    pt_this->ui2_retry_timeout = 0;
    pt_this->ui2_cur_timeout = 0;
    pt_this->b_is_send = FALSE;

    pt_this->ui1_msgid = 0;
    pt_this->ui1_free_msgid = 0;
    pt_this->ui1_msgtype = 0;
    pt_this->ui2_msg_body_len = 0;
    pt_this->ui2_msg_body_max_len = 0;

    x_mem_free(pt_this->pui1_msg_body);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_free 
 *
 * Description: Set attributes of a with-response request
 *
 * Inputs:  pt_this             Pointer points to the with-response request
 *          ui1_retry_cnt       Retry count of the with-response request
 *          ui2_retry_timeout   Retry timeout of the with-response request
 *          ui1_msgseq          Sequence number of the with-response request
 *          ui1_msgid           Message id field of the with-response request
 *          ui1_free_msgid      Free message id field of the with-response request
 *          ui1_msgtype         Message type of the with-response request
 *          ui2_msg_body_len    Message body length of the with-response request
 *          pui1_msg_body       Pointer points to the body of the with-response request
 * Outputs: -
 * 
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_flow_wr_req_set(
    EXTMJC_FLOW_WR_REQ_T*      pt_this,
    UINT8                   ui1_retry_cnt,
    UINT16                  ui2_retry_timeout,
    UINT8                   ui1_msgseq,
    UINT8                   ui1_msgid,
    UINT8                   ui1_free_msgid,
    UINT8                   ui1_msgtype,
    UINT16                  ui2_msg_body_len,
    UINT8*                  pui1_msg_body)
{
    pt_this->ui1_retry_cnt = ui1_retry_cnt;
    pt_this->ui2_retry_timeout = ui2_retry_timeout;
    pt_this->b_is_send = FALSE;

    pt_this->ui1_msgseq = ui1_msgseq;

    pt_this->ui1_msgid = ui1_msgid;
    pt_this->ui1_free_msgid = ui1_free_msgid;
    pt_this->ui1_msgtype = ui1_msgtype;
    pt_this->ui2_msg_body_len = ui2_msg_body_len;

    if (pt_this->ui2_msg_body_max_len < ui2_msg_body_len)
    {
        /* resize*/
        x_mem_free(pt_this->pui1_msg_body);
        
        pt_this->pui1_msg_body = x_mem_alloc(ui2_msg_body_len);

        ASSERT((NULL != pt_this->pui1_msg_body));

        pt_this->ui2_msg_body_max_len = ui2_msg_body_len;
    }

    x_memcpy(pt_this->pui1_msg_body, 
             pui1_msg_body, 
             ui2_msg_body_len);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_send 
 *
 * Description: Sends the with-response request to UART
 *
 * Inputs:  pt_this     Pointer points to the with-response request
 *
 * Outputs: -
 * 
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_flow_wr_req_send(
    EXTMJC_FLOW_WR_REQ_T*      pt_this)
{
    extmjc_prc_rtx_send_msg(
        TRUE,
        TRUE,
        pt_this->ui1_msgid,
        pt_this->ui1_free_msgid,
        pt_this->ui1_msgtype,
        pt_this->ui2_msg_body_len,
        pt_this->pui1_msg_body);
    
}


/* With Response Request Message Wait Queue */
/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_wait_que_init 
 *
 * Description: Initialize with-response request wait queue 
 *
 * Inputs:  pt_this     Pointer points to the with-response request waiting queue.
 *          ui1_max_cnt Maximum number of with-response request in the queue
 *
 * Outputs: -
 * 
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_flow_wr_req_wait_que_init(
    EXTMJC_FLOW_WR_REQ_WAIT_QUE_T* pt_this,
    UINT8                       ui1_max_cnt)
{
    INT32 i;

    pt_this->pt_msg_que = NULL;

    pt_this->ui1_head = 0;
    pt_this->ui1_tail = 0;

    pt_this->ui1_cnt = 0;
    
    /* allocate messages */
    pt_this->pt_msg_que = (EXTMJC_FLOW_WR_REQ_T *) x_mem_alloc(
        sizeof(EXTMJC_FLOW_WR_REQ_T) * ui1_max_cnt);

    ASSERT((NULL != pt_this->pt_msg_que));

    for (i = 0; i < ui1_max_cnt; i++)
    {
        extmjc_flow_wr_req_init( &(pt_this->pt_msg_que[i]) );
    }

    pt_this->ui1_max_cnt = ui1_max_cnt;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_wait_que_init 
 *
 * Description: Initialize with-response request wait queue 
 *
 * Inputs:  pt_this     Pointer points to the with-response request waiting queue.
 *          ui1_max_cnt Maximum number of with-response request in the queue
 *
 * Outputs: -
 * 
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_flow_wr_req_wait_que_reset(
    EXTMJC_FLOW_WR_REQ_WAIT_QUE_T* pt_this)
{
    pt_this->ui1_head = 0;
    pt_this->ui1_tail = 0;
    pt_this->ui1_cnt = 0;
}
/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_wait_que_has_free_entry 
 *
 * Description: Check whether a with-response request wait queue has free entry
 *
 * Inputs:  pt_this     Pointer points to the with-response request waiting queue.
 *
 * Outputs: -
 * 
 * Returns: If the with-response request wait queue has free entry, TRUE is returned.
 *          Otherwise, FALSE is returned.
 ----------------------------------------------------------------------------*/
static BOOL extmjc_flow_wr_req_wait_que_has_free_entry(
    EXTMJC_FLOW_WR_REQ_WAIT_QUE_T*     pt_this)
{
    if (pt_this->ui1_max_cnt == pt_this->ui1_cnt)
    {
        return(FALSE);
    }

    return(TRUE);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_wait_que_append 
 *
 * Description: Allocate an entry at the end of a with-response request wait queue
 *
 * Inputs:  pt_this     Pointer points to the with-response request waiting queue.
 *
 * Outputs: -
 * 
 * Returns: If the with-response request wait queue has free entry, a pointer points
 *          to the entry is returned. Otherwise, NULL is returned.
 ----------------------------------------------------------------------------*/
static EXTMJC_FLOW_WR_REQ_T* extmjc_flow_wr_req_wait_que_append(
    EXTMJC_FLOW_WR_REQ_WAIT_QUE_T*  pt_this)
{
    EXTMJC_FLOW_WR_REQ_T *pt_msg;
    if (pt_this->ui1_cnt == pt_this->ui1_max_cnt)
    {
        return (NULL);
    }

    pt_msg = & (pt_this->pt_msg_que[pt_this->ui1_head]);

    pt_this->ui1_head = (pt_this->ui1_head + 1) % pt_this->ui1_max_cnt;
    pt_this->ui1_cnt++;
    return (pt_msg);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_wait_que_get_msg 
 *
 * Description: Retrieve an entry at the front of a with-response request wait queue
 *
 * Inputs:  pt_this     Pointer points to the with-response request waiting queue.
 *
 * Outputs: -
 * 
 * Returns: If the with-response request wait queue has entries, a pointer points
 *          to the entry is returned. Otherwise, NULL is returned.
 ----------------------------------------------------------------------------*/
static EXTMJC_FLOW_WR_REQ_T* extmjc_flow_wr_req_wait_que_get_msg(
    EXTMJC_FLOW_WR_REQ_WAIT_QUE_T*  pt_this)
{
    EXTMJC_FLOW_WR_REQ_T *pt_msg;
    if (0 == pt_this->ui1_cnt)
    {
        return (NULL);
    }
    
    pt_msg = & (pt_this->pt_msg_que[pt_this->ui1_tail]);
    return (pt_msg);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_wait_que_remove 
 *
 * Description: Remove an entry from the front of a with-response request wait queue
 *
 * Inputs:  pt_this     Pointer points to the with-response request waiting queue.
 *
 * Outputs: -
 * 
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_flow_wr_req_wait_que_remove(
    EXTMJC_FLOW_WR_REQ_WAIT_QUE_T*  pt_this)
{
    if (0 == pt_this->ui1_cnt)
    {
        return;
    }

    pt_this->ui1_tail = (pt_this->ui1_tail + 1) % pt_this->ui1_max_cnt;
    pt_this->ui1_cnt--;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_wait_que_remove 
 *
 * Description: Remove an entry from the front of a with-response request wait queue
 *
 * Inputs:  pt_this     Pointer points to the with-response request waiting queue.
 *
 * Outputs: -
 * 
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_flow_wr_req_wait_que_fail_notify_all(
    EXTMJC_FLOW_WR_REQ_WAIT_QUE_T*  pt_this)
{
    UINT8 i;
    UINT8 msg_idx;
    EXTMJC_FLOW_WR_REQ_T *pt_msg;

    if (0 == pt_this->ui1_cnt)
    {
        return;
    }

    msg_idx = pt_this->ui1_tail;
    for (i = 0; i < pt_this->ui1_cnt; i++)
    {
        pt_msg = &(pt_this->pt_msg_que[msg_idx]);
        msg_idx = (msg_idx + 1) % pt_this->ui1_max_cnt;

        extmjc_resp_disp_fail_notify(pt_msg->ui1_msgseq,
                                  pt_msg->ui1_msgtype);
    }
}

/* With Response Request Message Queue */
/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_que_init 
 *
 * Description: Initialize a with-response request queue
 *
 * Inputs:  pt_this     Pointer points to the with-response request queue.
 *
 * Outputs: -
 * 
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_flow_wr_req_que_init(
    EXTMJC_FLOW_WR_REQ_QUE_T*     pt_this)
{
    pt_this->pt_msg_que = NULL;

    pt_this->ui1_msgid_mask = 0;
    pt_this->ui1_cnt = 0;
    pt_this->ui1_max_cnt = 0;

    pt_this->ui1_free_que_idx_mask = 0;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_que_reset 
 *
 * Description: Reset a with-response request queue
 *
 * Inputs:  pt_this     Pointer points to the with-response request queue.
 *          ui1_max_cnt Maximum number of entries in the queue
 *
 * Outputs: -
 * 
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID extmjc_flow_wr_req_que_reset(
    EXTMJC_FLOW_WR_REQ_QUE_T*  pt_this,
    UINT8                   ui1_max_cnt)
{
    UINT8 i;

    /* re-allocate */
    if (pt_this->ui1_max_cnt != ui1_max_cnt)
    {
        /* Free old messages */
        for (i = 0; i < pt_this->ui1_max_cnt; i++)
        {
            extmjc_flow_wr_req_free( &(pt_this->pt_msg_que[i]) );
        }

        if (NULL != pt_this->pt_msg_que)
        {
            x_mem_free(pt_this->pt_msg_que);
        }

        /* allocate new messages */
        pt_this->pt_msg_que = (EXTMJC_FLOW_WR_REQ_T *) x_mem_alloc(
                                sizeof(EXTMJC_FLOW_WR_REQ_T) * ui1_max_cnt);

        ASSERT((NULL != pt_this->pt_msg_que));

        for (i = 0; i < ui1_max_cnt; i++)
        {
            extmjc_flow_wr_req_init( &(pt_this->pt_msg_que[i]) );
        }

        pt_this->ui1_max_cnt = ui1_max_cnt;
    }

    pt_this->ui1_msgid_mask = 0;
    pt_this->ui1_cnt = 0;
    pt_this->ui1_max_cnt = ui1_max_cnt;

    pt_this->ui1_free_que_idx_mask = aui1_extmjc_flow_msg_que_mask[ui1_max_cnt];
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_que_get_used_id_mask 
 *
 * Description: Get message id mask from a with-response request queue
 *
 * Inputs:  pt_this     Pointer points to the with-response request waiting queue.
 *
 * Outputs: -
 * 
 * Returns: Return the mask
 ----------------------------------------------------------------------------*/
static UINT8 extmjc_flow_wr_req_que_get_used_id_mask(
    EXTMJC_FLOW_WR_REQ_QUE_T*     pt_this)
{
    return(pt_this->ui1_msgid_mask);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_que_has_free_entry 
 *
 * Description: Check whether a with-response request queue has free entries
 *
 * Inputs:  pt_this     Pointer points to the with-response request waiting queue.
 *
 * Outputs: -
 * 
 * Returns: If the queue has free entries, TRUE is returned. Otherwise,
 *          FALSE is returned.
 ----------------------------------------------------------------------------*/
static BOOL extmjc_flow_wr_req_que_has_free_entry(
    EXTMJC_FLOW_WR_REQ_QUE_T*     pt_this)
{
    if (pt_this->ui1_max_cnt == pt_this->ui1_cnt)
    {
        return(FALSE);
    }

    return(TRUE);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_que_get_msg 
 *
 * Description: Get a entry from a with-response request by message id.
 *
 * Inputs:  pt_this     Pointer points to the with-response request waiting queue.
 *          ui1_msgid   ID field of the message
 *
 * Outputs: -
 * 
 * Returns: If the queue has the entry, pointer points to the entry of the message 
 *          is returned. Otherwise, NULL is returned.
----------------------------------------------------------------------------*/
static EXTMJC_FLOW_WR_REQ_T* extmjc_flow_wr_req_que_get_msg(
    EXTMJC_FLOW_WR_REQ_QUE_T*  pt_this,
    UINT8                   ui1_msgid)
{
    UINT8 ui1_mask;
    UINT8 ui1_msg_idx;

    if (0 == pt_this->ui1_cnt)
    {
        return(NULL);
    }

    ui1_mask = (UINT8) 1 << ui1_msgid;
    if (0 != (ui1_mask & pt_this->ui1_msgid_mask))
    {
        ui1_msg_idx = pt_this->aui1_msg_idx[ui1_msgid];

        return( &(pt_this->pt_msg_que[ui1_msg_idx]) );
    }

    return(NULL);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_que_alloc 
 *
 * Description: Allocate a entry from a with-response request by message id.
 *
 * Inputs:  pt_this     Pointer points to the with-response request waiting queue.
 *          ui1_msgid   ID field of the message
 *
 * Outputs: -
 * 
 * Returns: If the queue has the entry, pointer points to the entry of the message 
 *          is returned. Otherwise, NULL is returned.
----------------------------------------------------------------------------*/
static EXTMJC_FLOW_WR_REQ_T* extmjc_flow_wr_req_que_alloc(
    EXTMJC_FLOW_WR_REQ_QUE_T*  pt_this,
    UINT8                   ui1_msgid)
{
    UINT8               ui1_msgid_mask;
    UINT8               ui1_que_idx;
    UINT8               ui1_free_que_idx_mask;
    UINT8               ui1_que_mask;
    EXTMJC_FLOW_WR_REQ_T*  pt_msg;
    UINT8               i;

    if (pt_this->ui1_max_cnt == pt_this->ui1_cnt)
    {
        return(NULL);
    }

    ui1_msgid_mask = (UINT8) 1 << ui1_msgid;
    if (0 != (ui1_msgid_mask & pt_this->ui1_msgid_mask))
    {
        return(NULL);
    }

    /* Search an un-used message */
    ui1_free_que_idx_mask = pt_this->ui1_free_que_idx_mask;
    ui1_que_idx = 0;

    for (i = 0; i < pt_this->ui1_max_cnt; i++)
    {
        if (0 != (ui1_free_que_idx_mask & 0x01))
        {
            /* use this entry */
            pt_this->ui1_cnt++;

            pt_msg = &(pt_this->pt_msg_que[ui1_que_idx]);
            ui1_que_mask = (UINT8) 1 << ui1_que_idx;

            pt_this->ui1_free_que_idx_mask =
            pt_this->ui1_free_que_idx_mask & (~ui1_que_mask);

            pt_this->ui1_msgid_mask
            = pt_this->ui1_msgid_mask | ui1_msgid_mask;

            pt_this->aui1_msg_idx[ui1_msgid] = ui1_que_idx;

            return(pt_msg);
        }

        ui1_que_idx ++;
        ui1_free_que_idx_mask = ui1_free_que_idx_mask >> 1;
    }

    return(NULL);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_que_free 
 *
 * Description: Free a entry from a with-response request by message id.
 *
 * Inputs:  pt_this     Pointer points to the with-response request waiting queue.
 *          ui1_msgid   ID field of the message
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_flow_wr_req_que_free(
    EXTMJC_FLOW_WR_REQ_QUE_T*  pt_this,
    UINT8                   ui1_msgid)
{
    UINT8                   ui1_msgid_mask;
    UINT8                   ui1_msg_que_idx;
    UINT8                   ui1_msg_que_mask;

    if (0 == pt_this->ui1_cnt)
    {
        return ;
    }

    ui1_msgid_mask = (UINT8) 1 << ui1_msgid;
    if (0 != (ui1_msgid_mask & pt_this->ui1_msgid_mask))
    {
        ui1_msg_que_idx = pt_this->aui1_msg_idx[ui1_msgid];

        ui1_msg_que_mask = (UINT8) 1 << ui1_msg_que_idx;

        pt_this->ui1_msgid_mask 
        = pt_this->ui1_msgid_mask & (~ui1_msgid_mask);

        pt_this->ui1_free_que_idx_mask 
        = pt_this->ui1_free_que_idx_mask | ui1_msg_que_mask;

        pt_this->ui1_cnt--;
    }
    return ;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_wr_req_que_fail_notify_all 
 *
 * Description: Call all fail notify functions. This function is called as connection
 *              is closed.
 *
 * Inputs:  pt_this     Pointer points to the with-response request waiting queue.
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_flow_wr_req_que_fail_notify_all(
    EXTMJC_FLOW_WR_REQ_QUE_T*  pt_this)
{
    UINT8               ui1_free_que_idx_mask;
    EXTMJC_FLOW_WR_REQ_T*  pt_msg;
    UINT8               i;
    
    ui1_free_que_idx_mask = pt_this->ui1_free_que_idx_mask;

    for (i = 0; i < pt_this->ui1_max_cnt; i++)
    {
        /* it's a used entry */
        if (0 == (ui1_free_que_idx_mask & 0x01))
        {
            pt_msg = &(pt_this->pt_msg_que[i]);

            extmjc_resp_disp_fail_notify(pt_msg->ui1_msgseq,
                                      pt_msg->ui1_msgtype);
        }
        
        ui1_free_que_idx_mask = ui1_free_que_idx_mask >> 1;
    }

}

/* Response Message */
/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_resp_init 
 *
 * Description: Initialize an DVD response message.
 *
 * Inputs:  pt_this     Pointer points to the DVD response message.
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_flow_resp_init(
    EXTMJC_FLOW_RESP_T *pt_this)
{
    pt_this->b_short_msg = TRUE;
    pt_this->ui1_msgid = 0;
    pt_this->ui2_msg_body_len = 0;
    pt_this->ui2_msg_body_max_len = EXTMJC_PRC_FLOW_SHORT_MSG_SIZE;
    pt_this->pui1_msg_body = (UINT8 *) x_mem_alloc(EXTMJC_PRC_FLOW_SHORT_MSG_SIZE);

    ASSERT((NULL != pt_this->pui1_msg_body));
    return ;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_resp_free 
 *
 * Description: Free an DVD response message.
 *
 * Inputs:  pt_this     Pointer points to the DVD response message.
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_flow_resp_free(
    EXTMJC_FLOW_RESP_T *pt_this)
{
    pt_this->ui1_msgid = 0;
    pt_this->ui2_msg_body_len = 0;
    pt_this->ui2_msg_body_max_len = 0;
    pt_this->b_short_msg = TRUE;

    x_mem_free(pt_this->pui1_msg_body);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_resp_set 
 *
 * Description: Set attributes of an DVD response message.
 *
 * Inputs:  pt_this          Pointer points to the DVD response message.
 *          ui1_msgid        Message id field
 *          ui2_msg_body_len Length of the response message body
 *          pui1_msg_body    Pointer points to the response message body
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_flow_resp_set(
    EXTMJC_FLOW_RESP_T*        pt_this,
    BOOL                    b_short_msg,
    UINT8                   ui1_msgid,
    UINT16                  ui2_msg_body_len,
    UINT8*                  pui1_msg_body)
{
    pt_this->ui1_msgid = ui1_msgid;
    pt_this->ui2_msg_body_len = ui2_msg_body_len;
    pt_this->b_short_msg = b_short_msg;

    if (pt_this->ui2_msg_body_max_len < ui2_msg_body_len)
    {
        x_mem_free(pt_this->pui1_msg_body);

        pt_this->pui1_msg_body = x_mem_alloc(ui2_msg_body_len);

        pt_this->ui2_msg_body_max_len = ui2_msg_body_len;
    }

    x_memcpy(pt_this->pui1_msg_body, 
             pui1_msg_body, 
             ui2_msg_body_len);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_resp_send 
 *
 * Description: Send an DVD response message to UART
 *
 * Inputs:  pt_this          Pointer points to the DVD response message.
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_flow_resp_send(
    EXTMJC_FLOW_RESP_T*      pt_this)
{
    extmjc_prc_rtx_send_msg(
        TRUE,
        pt_this->b_short_msg, /* is short */
        pt_this->ui1_msgid,
        0,
        0xFF,
        pt_this->ui2_msg_body_len,
        pt_this->pui1_msg_body);
    
}

/* Response Message Queue */
/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_resp_que_init 
 *
 * Description: Initialize an DVD response message queue
 *
 * Inputs:  pt_this          Pointer points to the DVD response message queue.
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_flow_resp_que_init(
    EXTMJC_FLOW_RESP_QUE_T*     pt_this)
{
    pt_this->pt_msg_que = NULL;

    pt_this->ui1_msgid_mask = 0;
    pt_this->ui1_cnt = 0;
    pt_this->ui1_max_cnt = 0;

    pt_this->ui1_free_que_idx_mask = 0;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_resp_que_init 
 *
 * Description: Reset an DVD response message queue
 *
 * Inputs:  pt_this         Pointer points to the DVD response message queue.
 *          ui1_max_cnt     Maximum number of entries in the queue.
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_flow_resp_que_reset(
    EXTMJC_FLOW_RESP_QUE_T*    pt_this,
    UINT8                   ui1_max_cnt)
{
    UINT8 i;

    /* re-allocate */
    if (pt_this->ui1_max_cnt != ui1_max_cnt)
    {
        /* Free old messages */
        for (i = 0; i < pt_this->ui1_max_cnt; i++)
        {
            extmjc_flow_resp_free( &(pt_this->pt_msg_que[i]) );
        }

        if (NULL != pt_this->pt_msg_que)
        {
            x_mem_free(pt_this->pt_msg_que);
        }

        /* allocate new messages */
        pt_this->pt_msg_que = (EXTMJC_FLOW_RESP_T *) x_mem_alloc(
                                                            sizeof(EXTMJC_FLOW_RESP_T) * ui1_max_cnt);

        ASSERT((NULL != pt_this->pt_msg_que));

        for (i = 0; i < ui1_max_cnt; i++)
        {
            extmjc_flow_resp_init( &(pt_this->pt_msg_que[i]) );
        }

        pt_this->ui1_max_cnt = ui1_max_cnt;
    }

    pt_this->ui1_msgid_mask = 0;
    pt_this->ui1_cnt = 0;
    pt_this->ui1_max_cnt = ui1_max_cnt;

    pt_this->ui1_free_que_idx_mask = aui1_extmjc_flow_msg_que_mask[ui1_max_cnt];
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_resp_que_has_free_entry 
 *
 * Description: Check whether an DVD response message queue has free entries
 *
 * Inputs:  pt_this         Pointer points to the DVD response message queue.
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static BOOL extmjc_flow_resp_que_has_free_entry(
    EXTMJC_FLOW_RESP_QUE_T*     pt_this)
{
    if (pt_this->ui1_max_cnt == pt_this->ui1_cnt)
    {
        return(FALSE);
    }

    return(TRUE);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_resp_que_get_msg 
 *
 * Description: Get an entry from an DVD response message queue by message id
 *
 * Inputs:  pt_this      Pointer points to the DVD response message queue.
 *          ui1_msgid    Message id   
 *
 * Outputs: -
 * 
 * Returns: If the DVD response message queue has the entry, a pointer points to
 *          the entry is returned. Otherwise, NULL is returned.
----------------------------------------------------------------------------*/
static EXTMJC_FLOW_RESP_T* extmjc_flow_resp_que_get_msg(
    EXTMJC_FLOW_RESP_QUE_T*    pt_this,
    UINT8                   ui1_msgid)
{
    UINT8 ui1_mask;
    UINT8 ui1_msg_idx;

    if (0 == pt_this->ui1_cnt)
    {
        return(NULL);
    }

    ui1_mask = (UINT8) 1 << ui1_msgid;
    if (0 != (ui1_mask & pt_this->ui1_msgid_mask))
    {
        ui1_msg_idx = pt_this->aui1_msg_idx[ui1_msgid];

        return( &(pt_this->pt_msg_que[ui1_msg_idx]) );
    }

    return(NULL);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_resp_que_alloc 
 *
 * Description: Allocate an entry from an DVD response message queue by message id
 *
 * Inputs:  pt_this      Pointer points to the DVD response message queue.
 *          ui1_msgid    Message id   
 *
 * Outputs: -
 * 
 * Returns: If allocation is successful, a pointer points to the entry is returned. 
 *          Otherwise, NULL is returned.
----------------------------------------------------------------------------*/
static EXTMJC_FLOW_RESP_T* extmjc_flow_resp_que_alloc(
    EXTMJC_FLOW_RESP_QUE_T*    pt_this,
    UINT8                   ui1_msgid)
{
    UINT8               ui1_msgid_mask;
    UINT8               ui1_que_idx;
    UINT8               ui1_free_que_idx_mask;
    UINT8               ui1_que_mask;
    EXTMJC_FLOW_RESP_T*    pt_msg;
    UINT8               i;

    if (pt_this->ui1_max_cnt == pt_this->ui1_cnt)
    {
        return(NULL);
    }

    ui1_msgid_mask = (UINT8) 1 << ui1_msgid;
    if (0 != (ui1_msgid_mask & pt_this->ui1_msgid_mask))
    {
        return(NULL);
    }

    /* Search an un-used message */
    ui1_free_que_idx_mask = pt_this->ui1_free_que_idx_mask;
    ui1_que_idx = 0;

    for (i = 0; i < pt_this->ui1_max_cnt; i++)
    {
        if (0 != (ui1_free_que_idx_mask & 0x01))
        {
            /* use this entry */
            pt_this->ui1_cnt++;

            pt_msg = &(pt_this->pt_msg_que[ui1_que_idx]);
            ui1_que_mask = (UINT8) 1 << ui1_que_idx;

            pt_this->ui1_free_que_idx_mask =
            pt_this->ui1_free_que_idx_mask & (~ui1_que_mask);

            pt_this->ui1_msgid_mask
                = pt_this->ui1_msgid_mask | ui1_msgid_mask;

            pt_this->aui1_msg_idx[ui1_msgid] = ui1_que_idx;

            return(pt_msg);
        }

        ui1_que_idx ++;
        ui1_free_que_idx_mask = ui1_free_que_idx_mask >> 1;
    }

    return(NULL);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_resp_que_free 
 *
 * Description: Free an entry from an DVD response message queue by message id
 *
 * Inputs:  pt_this      Pointer points to the DVD response message queue.
 *          ui1_msgid    Message id   
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_flow_resp_que_free(
    EXTMJC_FLOW_RESP_QUE_T*    pt_this,
    UINT8                   ui1_msgid)
{
    UINT8                   ui1_msgid_mask;
    UINT8                   ui1_msg_que_idx;
    UINT8                   ui1_msg_que_mask;

    if (0 == pt_this->ui1_cnt)
    {
        return ;
    }

    ui1_msgid_mask = (UINT8) 1 << ui1_msgid;
    if (0 != (ui1_msgid_mask & pt_this->ui1_msgid_mask))
    {
        ui1_msg_que_idx = pt_this->aui1_msg_idx[ui1_msgid];

        ui1_msg_que_mask = (UINT8) 1 << ui1_msg_que_idx;

        pt_this->ui1_msgid_mask 
        = pt_this->ui1_msgid_mask & (~ui1_msgid_mask);

        pt_this->ui1_free_que_idx_mask 
        = pt_this->ui1_free_que_idx_mask | ui1_msg_que_mask;

        pt_this->ui1_cnt--;
    }
    return ;
}



/* No Response Request */
/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_nr_req_init 
 *
 * Description: Initialize an DVD no-response request message
 *
 * Inputs:  pt_this      Pointer points to the DVD no-response request message.
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_flow_nr_req_init(
    EXTMJC_FLOW_NR_REQ_T *pt_this)
{
    pt_this->ui1_msgtype = 0;
    pt_this->ui2_msg_body_len = 0;
    pt_this->ui2_msg_body_max_len = EXTMJC_PRC_FLOW_SHORT_MSG_SIZE;
    pt_this->pui1_msg_body = (UINT8 *) x_mem_alloc(EXTMJC_PRC_FLOW_SHORT_MSG_SIZE);

    ASSERT((NULL != pt_this->pui1_msg_body));
    return ;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_nr_req_set 
 *
 * Description: Set attributes of an DVD no-response request message
 *
 * Inputs:  pt_this          Pointer points to the DVD no-response request message.
 *          ui1_msgtype      Types field of message
 *          ui2_msg_body_len Length of message body
 *          pui1_msg_body    Pointer points to message body
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_flow_nr_req_set(
    EXTMJC_FLOW_NR_REQ_T*      pt_this,
    UINT8                   ui1_msgtype,
    UINT16                  ui2_msg_body_len,
    UINT8*                  pui1_msg_body)
{
    pt_this->ui1_msgtype = ui1_msgtype;
    pt_this->ui2_msg_body_len = ui2_msg_body_len;

    if (pt_this->ui2_msg_body_max_len < ui2_msg_body_len)
    {
        x_mem_free(pt_this->pui1_msg_body);
        
        pt_this->pui1_msg_body = x_mem_alloc(ui2_msg_body_len);

        ASSERT((NULL != pt_this->pui1_msg_body));
        pt_this->ui2_msg_body_max_len = ui2_msg_body_len;
    }

    x_memcpy(pt_this->pui1_msg_body, 
             pui1_msg_body, 
             ui2_msg_body_len);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_nr_req_send 
 *
 * Description: Sends an DVD no-response request message to UART
 *
 * Inputs:  pt_this          Pointer points to the DVD no-response request message.
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_flow_nr_req_send(
    EXTMJC_FLOW_NR_REQ_T*      pt_this)
{
    extmjc_prc_rtx_send_msg(
        FALSE,
        TRUE,
        0,
        0,
        pt_this->ui1_msgtype,
        pt_this->ui2_msg_body_len,
        pt_this->pui1_msg_body);
    
}


/* No Response Request Message Queue */
/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_nr_req_que_init 
 *
 * Description: Initialize an DVD no-response request message queue
 *
 * Inputs:  pt_this     Pointer points to the no-response request message queue 
 *          ui1_max_cnt Maximum number of entries in the queue
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_flow_nr_req_que_init(
    EXTMJC_FLOW_NR_REQ_QUE_T*  pt_this,
    UINT8                   ui1_max_cnt)
{
    INT32 i;

    pt_this->pt_msg_que = NULL;

    pt_this->ui1_head = 0;
    pt_this->ui1_tail = 0;

    pt_this->ui1_cnt = 0;
    
    /* allocate messages */
    pt_this->pt_msg_que = (EXTMJC_FLOW_NR_REQ_T *) x_mem_alloc(
        sizeof(EXTMJC_FLOW_NR_REQ_T) * ui1_max_cnt);

    ASSERT((NULL != pt_this->pt_msg_que));

    for (i = 0; i < ui1_max_cnt; i++)
    {
        extmjc_flow_nr_req_init( &(pt_this->pt_msg_que[i]) );
    }

    pt_this->ui1_max_cnt = ui1_max_cnt;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_nr_req_que_reset
 *
 * Description: Initialize an DVD no-response request message queue
 *
 * Inputs:  pt_this     Pointer points to the no-response request message queue 
 *          ui1_max_cnt Maximum number of entries in the queue
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_flow_nr_req_que_reset(
    EXTMJC_FLOW_NR_REQ_QUE_T*  pt_this)
{
    pt_this->ui1_head = 0;
    pt_this->ui1_tail = 0;
    pt_this->ui1_cnt = 0;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_nr_req_que_has_free_entry 
 *
 * Description: Check whether an DVD no-response request message queue has free entries
 *
 * Inputs:  pt_this     Pointer points to the no-response request message queue 
 *
 * Outputs: -
 * 
 * Returns: If the queue has free entries, TRUE is returned. Otherwise, FALSE is returned.
----------------------------------------------------------------------------*/
static BOOL extmjc_flow_nr_req_que_has_free_entry(
    EXTMJC_FLOW_NR_REQ_QUE_T*     pt_this)
{
    if (pt_this->ui1_max_cnt == pt_this->ui1_cnt)
    {
        return(FALSE);
    }

    return(TRUE);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_nr_req_que_append 
 *
 * Description: Allocate an entry from the end of an DVD no-response request message queue
 *
 * Inputs:  pt_this     Pointer points to the no-response request message queue 
 *
 * Outputs: -
 * 
 * Returns: If the queue has free entries, a pointer points to the entry is returned.
 *          Otherwise, NULL is returned.
----------------------------------------------------------------------------*/
static EXTMJC_FLOW_NR_REQ_T* extmjc_flow_nr_req_que_append(
    EXTMJC_FLOW_NR_REQ_QUE_T*  pt_this)
{
    EXTMJC_FLOW_NR_REQ_T *pt_msg;
    if (pt_this->ui1_cnt == pt_this->ui1_max_cnt)
    {
        return (NULL);
    }

    pt_msg = & (pt_this->pt_msg_que[pt_this->ui1_head]);

    pt_this->ui1_head = (pt_this->ui1_head + 1) % pt_this->ui1_max_cnt;
    pt_this->ui1_cnt++;
    return (pt_msg);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_nr_req_que_get_msg 
 *
 * Description: Get an entry from the head of an DVD no-response request message queue
 *
 * Inputs:  pt_this     Pointer points to the no-response request message queue 
 *
 * Outputs: -
 * 
 * Returns: If the queue has entries, a pointer points to the entry is returned.
 *          Otherwise, NULL is returned.
----------------------------------------------------------------------------*/
static EXTMJC_FLOW_NR_REQ_T* extmjc_flow_nr_req_que_get_msg(
    EXTMJC_FLOW_NR_REQ_QUE_T*  pt_this)
{
    EXTMJC_FLOW_NR_REQ_T *pt_msg;
    if (0 == pt_this->ui1_cnt)
    {
        return (NULL);
    }
    
    pt_msg = & (pt_this->pt_msg_que[pt_this->ui1_tail]);
    return (pt_msg);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_flow_nr_req_que_remove 
 *
 * Description: Remove an entry from the head of an DVD no-response request message queue
 *
 * Inputs:  pt_this     Pointer points to the no-response request message queue 
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_flow_nr_req_que_remove(
    EXTMJC_FLOW_NR_REQ_QUE_T*  pt_this)
{
    if (0 == pt_this->ui1_cnt)
    {
        return;
    }

    pt_this->ui1_tail = (pt_this->ui1_tail + 1) % pt_this->ui1_max_cnt;
    pt_this->ui1_cnt--;
}

/* dvd flow manager */
/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_lock 
 *
 * Description: Lock the DVD flow manager semaphore
 *
 * Inputs:  pt_this     Pointer points to the DVD flow manager 
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_prc_flow_lock(EXTMJC_PRC_FLOW_T* pt_this)
{
    INT32 i4_ret = 0;
    
    /* Lock mutex */
    while (TRUE)
    {
        i4_ret = x_sema_lock_timeout( pt_this->h_flow_mtx, EXTMJC_FLOW_LOCK_MTX_TIMEOUT );
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
 * Name: extmjc_prc_flow_unlock 
 *
 * Description: Unlock the DVD flow manager semaphore
 *
 * Inputs:  pt_this     Pointer points to the DVD flow manager 
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_prc_flow_unlock(EXTMJC_PRC_FLOW_T* pt_this)
{
    INT32 i4_ret = 0;
    
    /* Unlock mutex */
    i4_ret = x_sema_unlock( pt_this->h_flow_mtx );

    if (i4_ret != OSR_OK)
    {
        ASSERT(FALSE);
    }
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_establish_connect 
 *
 * Description: Eastablish connection to DVD controller
 *
 * Inputs:  pt_this     Pointer points to the DVD flow manager 
 *          ui2_period  Period between current time and previous function call.
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_prc_flow_establish_connect(
    EXTMJC_PRC_FLOW_T*     pt_this,
    UINT16              ui2_period)
{
    UINT8           ui1_status;
    UINT8           ui1_max_reliable_msg_cnt;

    ui1_status = pt_this->ui1_status;

    /* in connected state */
    if ( EXTMJC_PRC_FLOW_CONNECT == ui1_status ) 
    {
        return;
    }

    /* In connecting state */
    if (EXTMJC_PRC_FLOW_CONNECTING == ui1_status)
    {
        /* if timeout, retry */
        if (ui2_period < pt_this->ui2_connect_req_timeout)
        {
            pt_this->ui2_connect_req_timeout = 
                pt_this->ui2_connect_req_timeout - ui2_period;
            return;
        }
    }

    /* in dis-connect state or wait timeout in connecting state  */
    ui1_max_reliable_msg_cnt = pt_this->ui1_max_reliable_msg_cnt;
    extmjc_prc_rtx_send_msg(TRUE, 
                         TRUE,
                         1, 
                         0,
                         1,
                         1,
                         &ui1_max_reliable_msg_cnt);

    pt_this->ui2_connect_req_timeout = EXTMJC_PRC_FLOW_CONNECT_TIMEOUT;
    
    pt_this->ui1_status = EXTMJC_PRC_FLOW_CONNECTING;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_proc_connect_req 
 *
 * Description: Handle connection request message.
 *
 * Inputs:  pt_this             Pointer points to the DVD flow manager 
 *          ui1_inmsg_id        ID field of the message.
 *          ui1_inmsg_type      Type field of the message.
 *          ui2_inmsg_body_len  Length of the message body
 *          pui1_inmsg_body     Pointer points to the message body
 *
 * Outputs: -
 * 
 * Returns: If this function doesn't handle the message and the caller function needs
 *          handle the message, FALSE is returned. Otherwise, TRUE is returned.
----------------------------------------------------------------------------*/
static BOOL extmjc_prc_flow_proc_connect_req(
    EXTMJC_PRC_FLOW_T*     pt_this,
    UINT8               ui1_inmsg_id,
    UINT8               ui1_inmsg_type,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body)
{
    UINT8           ui1_max_reliable_msg_cnt;
    UINT8           ui1_status;

    ui1_status = pt_this->ui1_status;

    /* in connected state */
    if (EXTMJC_PRC_FLOW_CONNECT == ui1_status)
    {
        /* it's not a connect message */
        if (1 != ui1_inmsg_type)
        {
            return (FALSE);
        }
    }

    /* in disconnect state or receiving a connect message in connected/ connecting state*/
    if (1 != ui1_inmsg_type)
    {
        return (TRUE);
    }

    if (1 != ui2_inmsg_body_len)
    {
        return (TRUE);
    }
    
    /* disconnect process */
    pt_this->ui1_status = EXTMJC_PRC_FLOW_DISCONNECT;
    extmjc_flow_wr_req_que_fail_notify_all(&(pt_this->t_tx_wr_req_que));
    extmjc_flow_wr_req_wait_que_fail_notify_all(&(pt_this->t_tx_wr_req_wait_que));
    
    extmjc_prc_connect_notify(FALSE);


    /* send the response message */
    ui1_max_reliable_msg_cnt = pui1_inmsg_body[0];
    
    if (pt_this->ui1_max_reliable_msg_cnt < ui1_max_reliable_msg_cnt)
    {
        ui1_max_reliable_msg_cnt = pt_this->ui1_max_reliable_msg_cnt;
    }
    
    /* send the response message */
    extmjc_prc_rtx_send_msg(TRUE, 
                             TRUE,
                             ui1_inmsg_id, 
                             0,
                             0xFF,
                             1,
                             &ui1_max_reliable_msg_cnt);

    extmjc_flow_msgid_que_reset(&(pt_this->t_rec_req_id_que),
                             ui1_max_reliable_msg_cnt);

    extmjc_flow_msgid_que_reset(&(pt_this->t_rec_resp_id_que),
                             ui1_max_reliable_msg_cnt);

    extmjc_flow_wr_req_que_reset(&(pt_this->t_tx_wr_req_que),
                              ui1_max_reliable_msg_cnt);

    extmjc_flow_wr_req_wait_que_reset(&(pt_this->t_tx_wr_req_wait_que));

    extmjc_flow_nr_req_que_reset(&(pt_this->t_tx_nr_req_que));

    extmjc_flow_resp_que_reset(&(pt_this->t_tx_resp_que),
                            ui1_max_reliable_msg_cnt);

    extmjc_flow_tx_info_que_reset(
        &(pt_this->t_tx_info_que), 
        (ui1_max_reliable_msg_cnt * 2) + pt_this->t_tx_nr_req_que.ui1_max_cnt);

    pt_this->ui1_req_msgid = 0;
    
    pt_this->ui1_wr_req_send_fail_cnt = EXTMJC_FLOW_WR_REQ_SEND_FAIL_MAX_CNT;

    pt_this->ui1_status = EXTMJC_PRC_FLOW_CONNECT;
    
    extmjc_prc_connect_notify(TRUE);
    
    return (TRUE);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_proc_connect_resp 
 *
 * Description: Handle connection response message.
 *
 * Inputs:  pt_this             Pointer points to the DVD flow manager 
 *          ui1_inmsg_id        ID field of the message.
 *          ui2_inmsg_body_len  Length of the message body
 *          pui1_inmsg_body     Pointer points to the message body
 *
 * Outputs: -
 * 
 * Returns: If this function doesn't handle the message and the caller function needs
 *          handle the message, FALSE is returned. Otherwise, TRUE is returned.
----------------------------------------------------------------------------*/
static BOOL extmjc_prc_flow_proc_connect_resp(
    EXTMJC_PRC_FLOW_T*     pt_this,
    UINT8               ui1_inmsg_id,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body)
{
    UINT8           ui1_max_reliable_msg_cnt;
    UINT8           ui1_status;

    ui1_status = pt_this->ui1_status;

    /* in disconnect state */
    if (EXTMJC_PRC_FLOW_DISCONNECT == ui1_status)
    {
        return (TRUE);
    }

    /* in connected state */
    if (EXTMJC_PRC_FLOW_CONNECT == ui1_status)
    {
        return (FALSE);
    }

    /* in connecting state */
    if (1 != ui1_inmsg_id)
    {
        return (TRUE);
    }

    if (1 != ui2_inmsg_body_len)
    {
        return (TRUE);
    }
    
    ui1_max_reliable_msg_cnt = pui1_inmsg_body[0];
    
    extmjc_flow_msgid_que_reset(&(pt_this->t_rec_req_id_que),
                             ui1_max_reliable_msg_cnt);

    extmjc_flow_msgid_que_reset(&(pt_this->t_rec_resp_id_que),
                             ui1_max_reliable_msg_cnt);

    extmjc_flow_wr_req_que_reset(&(pt_this->t_tx_wr_req_que),
                              ui1_max_reliable_msg_cnt);
    
    extmjc_flow_wr_req_wait_que_reset(&(pt_this->t_tx_wr_req_wait_que));

    extmjc_flow_nr_req_que_reset(&(pt_this->t_tx_nr_req_que));


    extmjc_flow_resp_que_reset(&(pt_this->t_tx_resp_que),
                            ui1_max_reliable_msg_cnt);

    extmjc_flow_tx_info_que_reset(
        &(pt_this->t_tx_info_que), 
        (ui1_max_reliable_msg_cnt * 2) + pt_this->t_tx_nr_req_que.ui1_max_cnt);

    pt_this->ui1_req_msgid = 0;

    pt_this->ui1_wr_req_send_fail_cnt = EXTMJC_FLOW_WR_REQ_SEND_FAIL_MAX_CNT;

    pt_this->ui1_status = EXTMJC_PRC_FLOW_CONNECT;
    extmjc_prc_connect_notify(TRUE);
    
    return (TRUE);
}


/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_resend_wr_req 
 *
 * Description: Resend function of with-reponse request message. This function is
 *              invoked periodically. This function will resend timeout with-response
 *              request message.
 *
 * Inputs:  pt_this             Pointer points to the DVD flow manager 
 *          pt_wr_req_que       Pointer points to the with-response request queue
 *          ui2_period          Period between function call.
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
static VOID extmjc_prc_flow_resend_wr_req(
    EXTMJC_PRC_FLOW_T*     pt_this,
    EXTMJC_FLOW_WR_REQ_QUE_T*  pt_wr_req_que,
    UINT16                  ui2_period)
{
    UINT8               i;
    UINT8               ui1_msgid_mask;
    UINT8               ui1_msg_idx;
    UINT8               ui1_tx_info_que_idx;
    EXTMJC_FLOW_WR_REQ_T*  pt_wr_req;
    BOOL                b_short_msg;

    ui1_msgid_mask = pt_wr_req_que->ui1_msgid_mask;

    for (i = 0; i < EXTMJC_PRC_FLOW_MAX_MSG_CNT; i++)
    {
        ui1_msgid_mask = ui1_msgid_mask >> 1;
        
        if (0 == (ui1_msgid_mask & 0x01))
        {
            continue;
        }
        
        ui1_msg_idx = pt_wr_req_que->aui1_msg_idx[i + 1];
        pt_wr_req = & (pt_wr_req_que->pt_msg_que[ui1_msg_idx]);

        if (FALSE == pt_wr_req->b_is_send)
        {
            continue;
        }

        if (pt_wr_req->ui2_cur_timeout > ui2_period)
        {
            pt_wr_req->ui2_cur_timeout = 
                pt_wr_req->ui2_cur_timeout - ui2_period;
        }
        else
        {
            /* Close connection */
            if (0 == pt_this->ui1_wr_req_send_fail_cnt)
            {
                pt_this->ui1_status = EXTMJC_PRC_FLOW_DISCONNECT;
                extmjc_flow_wr_req_que_fail_notify_all(pt_wr_req_que);
                extmjc_flow_wr_req_wait_que_fail_notify_all(&(pt_this->t_tx_wr_req_wait_que));

                extmjc_prc_connect_notify(FALSE);
                return ;
            }
            
            pt_this->ui1_wr_req_send_fail_cnt--;

            if (0 != pt_wr_req->ui1_retry_cnt)
            {
                /* resend the message */
                ui1_tx_info_que_idx = 
                    extmjc_flow_tx_info_que_append(&(pt_this->t_tx_info_que), 
                                                pt_wr_req->ui1_msgid, 
                                                EXTMJC_FLOW_TX_WR_REQ);
    
                ASSERT((0 != ui1_tx_info_que_idx));

                if (0 == ui1_tx_info_que_idx)
                {
                    return;
                }
    
                pt_wr_req->ui1_retry_cnt--;
                pt_wr_req->b_is_send = FALSE;

                /* Send message */
                if (1 == ui1_tx_info_que_idx)
                {
                    extmjc_flow_wr_req_send(pt_wr_req);
                }
            }
            else
            {
                /* Call the notify function */
                extmjc_resp_disp_fail_notify(pt_wr_req->ui1_msgseq,
                                          pt_wr_req->ui1_msgtype);

                
                /* remove message from wr req que */
                extmjc_flow_wr_req_que_free(pt_wr_req_que, i + 1);
            
                /* add the message id of the request to free msgid queue */
                if (pt_wr_req->ui1_msgtype < EXTMJC_LONG_MSG_FIRST_MSG_TYPE)
                {
                    b_short_msg = TRUE;
                }
                else
                {
                    b_short_msg = FALSE;
                }

                extmjc_flow_msgid_que_add_msg(
                    &(pt_this->t_rec_resp_id_que), i + 1, b_short_msg);
            }
        }
    }
}

static VOID extmjc_prc_flow_timeout_timer_nfy(
    HANDLE_T  t_tm_handle,
    VOID*     pv_tag)
{
    EXTMJC_PRC_MSG_T        t_msg;
    
    ASSERT((NULL != pv_tag));

    t_msg.ui1_msg_type = EXTMJC_PRC_MSG_TYPE_TX_RETRY;
    t_msg.pt_obj = pv_tag;

    if (OSR_OK != extmjc_prc_send_msg(&t_msg))
    {
        LOG(1, "ERR: Failed sending tx empty message in delay timer\n");
    }
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_thread 
 *
 * Description: Thread function of DVD flow manager thread. The main purpose of this
 *              thread is resending timeout messages.
 *
 * Inputs:  pv_arg              Pointer points to the DVD flow manager 
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
VOID extmjc_prc_flow_timeout_handler(VOID* pv_arg)
{
    EXTMJC_PRC_FLOW_T* pt_this = (EXTMJC_PRC_FLOW_T *) pv_arg;

    /* Connection */
    extmjc_prc_flow_lock(pt_this);

    extmjc_prc_flow_establish_connect(pt_this, 
                                   EXTMJC_PRC_FLOW_TIMEOUT_INTERVAL);

    if (EXTMJC_PRC_FLOW_CONNECT == pt_this->ui1_status)
    {
        extmjc_prc_flow_resend_wr_req(pt_this,
                                   &(pt_this->t_tx_wr_req_que), 
                                   EXTMJC_PRC_FLOW_TIMEOUT_INTERVAL);
    }

    if (OSR_OK != x_timer_start (pt_this->h_timeout_timer,
                                 EXTMJC_PRC_FLOW_TIMEOUT_INTERVAL,
                                 X_TIMER_FLAG_ONCE,
                                 extmjc_prc_flow_timeout_timer_nfy,
                                 (VOID *) pt_extmjc_prc_flow) )
    {
        ASSERT(FALSE);
    }
    
    extmjc_prc_flow_unlock(pt_this);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_init 
 *
 * Description: Initialize DVD flow manager.
 *
 * Inputs:  ui1_max_reliable_msg_cnt  Maximum number of reliable request. 
 *          ui1_noresp_req_buf_cnt    Maximum number of no-response request.  
 *          ui1_withresp_req_buf_cnt  Maximum entries of with-response request 
 *                                    waiting queue.
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
VOID extmjc_prc_flow_init(
    UINT8 ui1_max_reliable_msg_cnt,
    UINT8 ui1_noresp_req_buf_cnt,
    UINT8 ui1_withresp_req_buf_cnt)
{
    pt_extmjc_prc_flow = (EXTMJC_PRC_FLOW_T *) x_mem_alloc( 
        sizeof (EXTMJC_PRC_FLOW_T) );

    ASSERT((NULL != pt_extmjc_prc_flow));

    if (OSR_OK != x_sema_create( &(pt_extmjc_prc_flow->h_flow_mtx), X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK))
    {
        ASSERT(FALSE);
        return;
    }

    /* Init receive member data */
    if (OSR_OK != x_timer_create(& (pt_extmjc_prc_flow->h_timeout_timer) ) )
    {
        ASSERT(FALSE);
        return;
    }

    extmjc_flow_msgid_que_init(&(pt_extmjc_prc_flow->t_rec_req_id_que));

    extmjc_flow_msgid_que_init(&(pt_extmjc_prc_flow->t_rec_resp_id_que));

    extmjc_flow_wr_req_que_init(&(pt_extmjc_prc_flow->t_tx_wr_req_que));

    extmjc_flow_resp_que_init(&(pt_extmjc_prc_flow->t_tx_resp_que));

    extmjc_flow_nr_req_que_init(
        &(pt_extmjc_prc_flow->t_tx_nr_req_que), ui1_noresp_req_buf_cnt);

    extmjc_flow_tx_info_que_init(&(pt_extmjc_prc_flow->t_tx_info_que));

    extmjc_flow_wr_req_wait_que_init(
        &(pt_extmjc_prc_flow->t_tx_wr_req_wait_que), ui1_withresp_req_buf_cnt);

    pt_extmjc_prc_flow->ui1_max_reliable_msg_cnt = ui1_max_reliable_msg_cnt;
    pt_extmjc_prc_flow->ui2_connect_req_timeout = EXTMJC_PRC_FLOW_INIT_CONNECT_PERIOD;
    pt_extmjc_prc_flow->ui1_status = EXTMJC_PRC_FLOW_DISCONNECT;
    pt_extmjc_prc_flow->ui1_req_seq = 0;
    pt_extmjc_prc_flow->ui1_wr_req_send_fail_cnt = EXTMJC_FLOW_WR_REQ_SEND_FAIL_MAX_CNT;

    /* Start retry */
    if (OSR_OK != x_timer_start (pt_extmjc_prc_flow->h_timeout_timer,
                                 EXTMJC_PRC_FLOW_TIMEOUT_INTERVAL,
                                 X_TIMER_FLAG_ONCE,
                                 extmjc_prc_flow_timeout_timer_nfy,
                                 (VOID *) pt_extmjc_prc_flow) )
    {
        ASSERT(FALSE);
    }
}


/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_tx_empty_nfy 
 *
 * Description: Transmit empty call-back function. This function is called as
 *              previous transmitted message is sent to UART.
 *
 * Inputs:  -
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
VOID extmjc_prc_flow_tx_empty_nfy(VOID)
{
    EXTMJC_PRC_FLOW_T* pt_this = pt_extmjc_prc_flow;
    UINT8               ui1_msgid;
    UINT8               ui1_source;
    EXTMJC_FLOW_NR_REQ_T*  pt_nr_req;
    EXTMJC_FLOW_WR_REQ_T*  pt_wr_req;
    EXTMJC_FLOW_RESP_T*    pt_resp;

    /* DBG_ERROR(("[DVD Demon]: flow manager sends a message completed\n")); */

    extmjc_prc_flow_lock(pt_this);

    if (EXTMJC_PRC_FLOW_CONNECT != pt_this->ui1_status)
    {
        extmjc_prc_flow_unlock(pt_this);
        return ;
    }
    
    /* remove one message from tx info queue */
    if (FALSE == extmjc_flow_tx_info_que_retrieve(
                    &(pt_this->t_tx_info_que), 
                    &ui1_msgid, 
                    &ui1_source))
    {
        extmjc_prc_flow_unlock(pt_this);
        return ;
    }


    switch (ui1_source)
    {
        case EXTMJC_FLOW_TX_NR_REQ:
            extmjc_flow_nr_req_que_remove(&(pt_this->t_tx_nr_req_que));
            break;

        case EXTMJC_FLOW_TX_WR_REQ:
            pt_wr_req = extmjc_flow_wr_req_que_get_msg(
                &(pt_this->t_tx_wr_req_que), ui1_msgid);

            if (NULL != pt_wr_req)
            {
                pt_wr_req->b_is_send = TRUE;
                pt_wr_req->ui2_cur_timeout = pt_wr_req->ui2_retry_timeout;
            }
            
            break;

        case EXTMJC_FLOW_TX_RESP: /* do nothing */
            break;

        default:
            ASSERT(FALSE);
            break;
    }


    /* Send next message in tx info queue */
    if (FALSE == extmjc_flow_tx_info_que_peek(
                    &(pt_this->t_tx_info_que), 
                    &ui1_msgid, 
                    &ui1_source))
    {
        /* No more message */
        extmjc_prc_flow_unlock(pt_this);
        return ;
    }

    switch (ui1_source)
    {
        case EXTMJC_FLOW_TX_NR_REQ:
            pt_nr_req = extmjc_flow_nr_req_que_get_msg(&(pt_this->t_tx_nr_req_que));
            
            if (NULL != pt_nr_req)
            {
                extmjc_flow_nr_req_send(pt_nr_req);
            }
            else
            {
                /* Something wrong, re-establish connection */
                pt_this->ui1_status = EXTMJC_PRC_FLOW_DISCONNECT;
                extmjc_flow_wr_req_que_fail_notify_all(&(pt_this->t_tx_wr_req_que));
                extmjc_flow_wr_req_wait_que_fail_notify_all(&(pt_this->t_tx_wr_req_wait_que));
                extmjc_prc_connect_notify(FALSE);
            }

            
            break;

        case EXTMJC_FLOW_TX_WR_REQ:
            pt_wr_req = extmjc_flow_wr_req_que_get_msg(
                &(pt_this->t_tx_wr_req_que), ui1_msgid);

            if (NULL != pt_wr_req)
            {
                extmjc_flow_wr_req_send(pt_wr_req);
            }
            else
            {
                /* Something wrong, re-establish connection */
                pt_this->ui1_status = EXTMJC_PRC_FLOW_DISCONNECT;
                extmjc_flow_wr_req_que_fail_notify_all(&(pt_this->t_tx_wr_req_que));
                extmjc_flow_wr_req_wait_que_fail_notify_all(&(pt_this->t_tx_wr_req_wait_que));
                extmjc_prc_connect_notify(FALSE);
            }

            break;

        case EXTMJC_FLOW_TX_RESP:
            pt_resp = extmjc_flow_resp_que_get_msg(&(pt_this->t_tx_resp_que),ui1_msgid);
            
            if (NULL != pt_resp)
            {
                extmjc_flow_resp_send(pt_resp);
            }
            else
            {
                /* Something wrong, re-establish connection */
                pt_this->ui1_status = EXTMJC_PRC_FLOW_DISCONNECT;
                extmjc_flow_wr_req_que_fail_notify_all(&(pt_this->t_tx_wr_req_que));
                extmjc_flow_wr_req_wait_que_fail_notify_all(&(pt_this->t_tx_wr_req_wait_que));
                extmjc_prc_connect_notify(FALSE);
            }
            
            break;

        default:
            ASSERT(FALSE);
            break;
    }
    
    extmjc_prc_flow_unlock(pt_this);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_send_noresp_req 
 *
 * Description: Send a no-response request message.
 *
 * Inputs:  ui1_inmsg_type      Message type field
 *          ui2_inmsg_body_len  Length of the message body
 *          pui1_inmsg_body     Pointer points to the message body
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
INT32 extmjc_prc_flow_send_noresp_req (
    UINT8               ui1_inmsg_type,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body)
{
    EXTMJC_PRC_FLOW_T* pt_this = pt_extmjc_prc_flow;
    INT32               i4_ret;
    UINT8               ui1_tx_info_que_idx;
    EXTMJC_FLOW_NR_REQ_T*  pt_msg;
    EXTMJC_PRC_XLS_T*          pt_extmjc_prc_xls = &t_extmjc_prc_xls;

    /* Verify state */
    extmjc_prc_flow_lock(pt_this);

    if (EXTMJC_PRC_FLOW_CONNECT != pt_this->ui1_status)
    {
        extmjc_prc_flow_unlock(pt_this);
        return (EXTMJC_PRCR_CONNECT_NOT_READY);
    }

    /* Validate Message */
    if (EXTMJC_PRC_XLS_OK != pt_extmjc_prc_xls->pf_extmjc_prc_xls_valid_inmsg(
                                    FALSE,
                                    REQ_MSG,
                                    ui1_inmsg_type,
                                    ui2_inmsg_body_len,
                                    pui1_inmsg_body))
    {
        extmjc_prc_flow_unlock(pt_this);
        return (EXTMJC_PRCR_INV_ARG);
    }
    
    /* Push message into nr req queue and tx info queue */
    if (FALSE == extmjc_flow_nr_req_que_has_free_entry( &(pt_this->t_tx_nr_req_que) ) )
    {
        extmjc_prc_flow_unlock(pt_this);
        return (EXTMJC_PRCR_TX_MSG_QUE_FULL);
    }

    ui1_tx_info_que_idx = extmjc_flow_tx_info_que_append(&(pt_this->t_tx_info_que), 
                                                      0, 
                                                      EXTMJC_FLOW_TX_NR_REQ);
    
    switch (ui1_tx_info_que_idx)
    {
        case 0: /* no entry */
            extmjc_prc_flow_unlock(pt_this);
            return (EXTMJC_PRCR_TX_MSG_QUE_FULL);
        
        case 1: /* first entry */
            pt_msg = extmjc_flow_nr_req_que_append(&(pt_this->t_tx_nr_req_que));

            ASSERT((NULL != pt_msg));
            extmjc_prc_flow_unlock(pt_this);
            
            i4_ret = extmjc_prc_rtx_send_msg(FALSE, 
                                              TRUE,
                                              0, 
                                              0,
                                              ui1_inmsg_type,
                                              ui2_inmsg_body_len,
                                              pui1_inmsg_body);
            return (i4_ret);
        
        default: /* wait sending */
            pt_msg = extmjc_flow_nr_req_que_append(&(pt_this->t_tx_nr_req_que));

            ASSERT((NULL != pt_msg));
            
            extmjc_flow_nr_req_set(pt_msg, 
                                ui1_inmsg_type,
                                ui2_inmsg_body_len,
                                pui1_inmsg_body);
    }
    
    extmjc_prc_flow_unlock(pt_this);
    return (0);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_get_next_req_msgid 
 *
 * Description: Get a useable message id for next with-response request message.
 *
 * Inputs:  
 *          pt_this     Pointer points to the DVD flow manager
 *
 * Outputs: -
 * 
 * Returns: Message id
----------------------------------------------------------------------------*/
static UINT8 extmjc_prc_flow_get_next_req_msgid(
    EXTMJC_PRC_FLOW_T  *pt_this)
{
    UINT8   ui1_msgid_mask;
    UINT8   ui1_free_msgid_mask;
    UINT8   ui1_total_mask;
    UINT8   ui1_mask;
    UINT8   i;
    UINT8   ui1_msgid;

    /* Get msg id and free msg id */
    ui1_free_msgid_mask = 
        extmjc_flow_msgid_que_get_used_id_mask( &(pt_this->t_rec_resp_id_que) );

    ui1_msgid_mask = 
        extmjc_flow_wr_req_que_get_used_id_mask( &(pt_this->t_tx_wr_req_que) );

    ui1_total_mask = ui1_free_msgid_mask | ui1_msgid_mask;
    
    ui1_msgid = EXTMJC_PRC_FLOW_INC_MSG_ID(pt_this->ui1_req_msgid);

    for (i = 0; i < EXTMJC_PRC_FLOW_MAX_MSG_CNT; i++)
    {
        ui1_mask = ((UINT8) 1) << (ui1_msgid + 1);
        
        if (0 == (ui1_mask & ui1_total_mask))
        {
            pt_this->ui1_req_msgid = ui1_msgid;
            return (ui1_msgid + 1);
        }

        ui1_msgid = EXTMJC_PRC_FLOW_INC_MSG_ID(ui1_msgid);
    }
    
    return (0);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_is_connect 
 *
 * Description: Indicates whether the connection is ready.
 *
 * Inputs:  -
 *
 * Outputs: -
 * 
 * Returns: If the connection is ready, TRUE is returned. Otherwise, FALSE is returned.
----------------------------------------------------------------------------*/
BOOL extmjc_prc_flow_is_connect (VOID)
{
    EXTMJC_PRC_FLOW_T* pt_this = pt_extmjc_prc_flow;
    
    extmjc_prc_flow_lock(pt_this);
    if (EXTMJC_PRC_FLOW_CONNECT != pt_this->ui1_status)
    {
        extmjc_prc_flow_unlock(pt_this);
        return (FALSE);
    }
    
    extmjc_prc_flow_unlock(pt_this);
    return (TRUE);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_send_withresp_req 
 *
 * Description: Send a with-response request message
 *
 * Inputs: 
 *          ui1_send_retry_cnt  Resend count of the message
 *          ui2_send_timeout    Resend timeout value 
 *          ui1_inmsg_type      Message type
 *          ui2_inmsg_body_len  Length of the message body
 *          pui1_inmsg_body     Pointer points to the message body
 *
 * Outputs: pui1_inmsg_id       ID of the message
 * 
 * Returns: 
----------------------------------------------------------------------------*/
INT32 extmjc_prc_flow_send_withresp_req (
    UINT8               ui1_send_retry_cnt,
    UINT16              ui2_send_timeout,
    UINT8               ui1_inmsg_type,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body,
    UINT8*              pui1_inmsg_id)
{
    EXTMJC_PRC_FLOW_T* pt_this = pt_extmjc_prc_flow;
    EXTMJC_FLOW_WR_REQ_T*  pt_msg;
    UINT8               ui1_inmsg_id;
    UINT8               ui1_free_inmsg_id;
    UINT8               ui1_tx_info_que_idx;
    UINT8               ui1_req_seq;
    EXTMJC_PRC_XLS_T*          pt_extmjc_prc_xls = &t_extmjc_prc_xls;

    if (EXTMJC_FLOW_WR_REQ_SEND_FAIL_MAX_CNT < ui1_send_retry_cnt)
    {
        ui1_send_retry_cnt = EXTMJC_FLOW_WR_REQ_SEND_FAIL_MAX_CNT;
    }

    extmjc_prc_flow_lock(pt_this);

    if (EXTMJC_PRC_FLOW_CONNECT != pt_this->ui1_status)
    {
        extmjc_prc_flow_unlock(pt_this);
        return (EXTMJC_PRCR_CONNECT_NOT_READY);
    }
    
    /* Validate Message */
    if (EXTMJC_PRC_XLS_OK != pt_extmjc_prc_xls->pf_extmjc_prc_xls_valid_inmsg(
                                    TRUE,
                                    REQ_MSG,
                                    ui1_inmsg_type,
                                    ui2_inmsg_body_len,
                                    pui1_inmsg_body))
    {
        extmjc_prc_flow_unlock(pt_this);
        return (EXTMJC_PRCR_INV_ARG);
    }

    /* push the message into wr req queue and tx info queue */
    if (FALSE == extmjc_flow_wr_req_wait_que_has_free_entry( &(pt_this->t_tx_wr_req_wait_que) ))
    {
        extmjc_prc_flow_unlock(pt_this);
        return (EXTMJC_PRCR_TX_MSG_QUE_FULL);
    }

    /* get sequence */
    ui1_req_seq = pt_this->ui1_req_seq;
    pt_this->ui1_req_seq++;
    
    *pui1_inmsg_id = ui1_req_seq;

    if (FALSE == extmjc_flow_wr_req_que_has_free_entry( &(pt_this->t_tx_wr_req_que) ) )
    {
        pt_msg = extmjc_flow_wr_req_wait_que_append(&(pt_this->t_tx_wr_req_wait_que));

        extmjc_flow_wr_req_set(pt_msg,
                            ui1_send_retry_cnt,
                            ui2_send_timeout,
                            ui1_req_seq,
                            0,
                            0,
                            ui1_inmsg_type,
                            ui2_inmsg_body_len,
                            pui1_inmsg_body);
        
        extmjc_prc_flow_unlock(pt_this);

        return (0);
    }

    /* Get the request message id */
    ui1_inmsg_id = extmjc_prc_flow_get_next_req_msgid(pt_this);

    /* Get one free msg id */
    ui1_free_inmsg_id = 
        extmjc_flow_msgid_que_get_and_remove_msg(&(pt_this->t_rec_resp_id_que));

    ui1_tx_info_que_idx = extmjc_flow_tx_info_que_append(&(pt_this->t_tx_info_que), 
                                                      ui1_inmsg_id, 
                                                      EXTMJC_FLOW_TX_WR_REQ);
    
    ASSERT((0 != ui1_tx_info_que_idx));

    /* allocate a message */
    pt_msg = extmjc_flow_wr_req_que_alloc(
                &(pt_this->t_tx_wr_req_que), 
                ui1_inmsg_id);
    
    ASSERT((NULL != pt_msg));
    
    extmjc_flow_wr_req_set(pt_msg,
                        ui1_send_retry_cnt,
                        ui2_send_timeout,
                        ui1_req_seq,
                        ui1_inmsg_id,
                        ui1_free_inmsg_id,
                        ui1_inmsg_type,
                        ui2_inmsg_body_len,
                        pui1_inmsg_body);

    /* Send message */
    if (1 == ui1_tx_info_que_idx)
    {
        extmjc_flow_wr_req_send(pt_msg);
    }

    extmjc_prc_flow_unlock(pt_this);

    return (0);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_send_resp 
 *
 * Description: Send a response message
 *
 * Inputs: 
 *          ui1_inmsg_id        Message ID 
 *          ui1_inmsg_type      Message type
 *          ui2_inmsg_body_len  Length of the message body
 *          pui1_inmsg_body     Pointer points to the message body
 *
 * Outputs: -
 * 
 * Returns: 
----------------------------------------------------------------------------*/
INT32 extmjc_prc_flow_send_resp (
    UINT8               ui1_inmsg_id,
    UINT8               ui1_inmsg_type,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body)
{
    EXTMJC_PRC_FLOW_T*     pt_this = pt_extmjc_prc_flow;
    EXTMJC_FLOW_RESP_T*        pt_msg;
    UINT8                   ui1_tx_info_que_idx;
    EXTMJC_PRC_XLS_T*              pt_extmjc_prc_xls = &t_extmjc_prc_xls;
    BOOL                    b_short_msg;
    EXTMJC_PRC_XLS_VALID_MSG_TYPE  t_msg_type;
    
    extmjc_prc_flow_lock(pt_this);

    if (EXTMJC_PRC_FLOW_CONNECT != pt_this->ui1_status)
    {
        extmjc_prc_flow_unlock(pt_this);
        return (EXTMJC_PRCR_CONNECT_NOT_READY);
    }

    /* Verify whether has sufficient buffer */
    if (FALSE == extmjc_flow_resp_que_has_free_entry( &(pt_this->t_tx_resp_que) ) )
    {
        extmjc_prc_flow_unlock(pt_this);
        return (EXTMJC_PRCR_TX_MSG_QUE_FULL);
    }

    /* get corresponding request */
    if (FALSE == extmjc_flow_msgid_que_search_msgid_info(&(pt_this->t_rec_req_id_que), 
                                                      ui1_inmsg_id,
                                                      &b_short_msg))
    {
        extmjc_prc_flow_unlock(pt_this);
        return (EXTMJC_PRCR_TX_INV_MSG);
    }

    if (TRUE == b_short_msg)
    {
        t_msg_type = SHORT_RESP_MSG;
    }
    else
    {
        t_msg_type = LONG_RESP_MSG;
    }

    /* Validate Message */
    if (EXTMJC_PRC_XLS_OK != pt_extmjc_prc_xls->pf_extmjc_prc_xls_valid_inmsg(
                                    TRUE,
                                    t_msg_type,
                                    ui1_inmsg_type,
                                    ui2_inmsg_body_len,
                                    pui1_inmsg_body))
    {
        extmjc_prc_flow_unlock(pt_this);
        return (EXTMJC_PRCR_INV_ARG);
    }

    /* Push message into nr req queue and tx info queue */
    if (FALSE == extmjc_flow_resp_que_has_free_entry(&(pt_this->t_tx_resp_que)))
    {
        extmjc_prc_flow_unlock(pt_this);
        return (EXTMJC_PRCR_TX_MSG_QUE_FULL);
    }

    ui1_tx_info_que_idx = extmjc_flow_tx_info_que_append(&(pt_this->t_tx_info_que), 
                                                      ui1_inmsg_id, 
                                                      EXTMJC_FLOW_TX_RESP);
    
    if (0 == ui1_tx_info_que_idx)
    {
        extmjc_prc_flow_unlock(pt_this);
        return (EXTMJC_PRCR_TX_MSG_QUE_FULL);
    }
    
    pt_msg = extmjc_flow_resp_que_alloc(
                &(pt_this->t_tx_resp_que), 
                ui1_inmsg_id);

    ASSERT((NULL != pt_msg));

    /* Copy data into buffer */
    extmjc_flow_resp_set(pt_msg, 
                      b_short_msg,
                      ui1_inmsg_id,
                      ui2_inmsg_body_len,
                      pui1_inmsg_body);

    /* remove message id from rec msgid que */
    extmjc_flow_msgid_que_remove_msg(&(pt_this->t_rec_req_id_que), ui1_inmsg_id);

    /* Send message */
    extmjc_flow_resp_send(pt_msg);

    extmjc_prc_flow_unlock(pt_this);
    return (0);
}


/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_rec_noresp_req 
 *
 * Description: Process no-response request message
 *
 * Inputs: 
 *          ui1_inmsg_type      Message type
 *          ui2_inmsg_body_len  Length of the message body
 *          pui1_inmsg_body     Pointer points to the message body
 *
 * Outputs: -
 * 
 * Returns: 
----------------------------------------------------------------------------*/
VOID extmjc_prc_flow_rec_noresp_req(
    UINT8               ui1_inmsg_type,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body)
{
    EXTMJC_PRC_FLOW_T* pt_this = pt_extmjc_prc_flow;

    extmjc_prc_flow_lock(pt_this);

    if (EXTMJC_PRC_FLOW_CONNECT != pt_this->ui1_status)
    {
        extmjc_prc_flow_unlock(pt_this);
        return;
    }
    
    extmjc_prc_flow_unlock(pt_this);
    
    extmjc_req_disp_disp_noresp_req(ui1_inmsg_type,
                                 ui2_inmsg_body_len,
                                 pui1_inmsg_body);

}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_rec_withresp_req 
 *
 * Description: Process with-response request message
 *
 * Inputs: 
 *          ui1_inmsg_id        ID field
 *          ui1_free_inmsg_id   Free message id field
 *          ui1_inmsg_type      Message type
 *          ui2_inmsg_body_len  Length of the message body
 *          pui1_inmsg_body     Pointer points to the message body
 *
 * Outputs: -
 * 
 * Returns: 
----------------------------------------------------------------------------*/
VOID extmjc_prc_flow_rec_withresp_req(
    BOOL                b_short_inmsg,
    UINT8               ui1_inmsg_id,
    UINT8               ui1_free_inmsg_id,
    UINT8               ui1_inmsg_type,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body)
{
    EXTMJC_PRC_FLOW_T* pt_this = pt_extmjc_prc_flow;
    UINT8               ui1_tx_info_que_idx;
    EXTMJC_FLOW_RESP_T     *pt_resp;

    extmjc_prc_flow_lock(pt_this);

    if (TRUE == extmjc_prc_flow_proc_connect_req(
                pt_this,
                ui1_inmsg_id,
                ui1_inmsg_type,
                ui2_inmsg_body_len,
                pui1_inmsg_body))
    {

        extmjc_prc_flow_unlock(pt_this);
        return;
    }
    
    /* If it's a repeated request, drop it */
    if (TRUE == extmjc_flow_msgid_que_search_msgid(
                &(pt_this->t_rec_req_id_que), ui1_inmsg_id))
    {
        extmjc_prc_flow_unlock(pt_this);
        return;
    }

    /* This request has response now, resend the response message */
    pt_resp = extmjc_flow_resp_que_get_msg(&(pt_this->t_tx_resp_que), ui1_inmsg_id);
    if (NULL != pt_resp)
    {
        ui1_tx_info_que_idx = extmjc_flow_tx_info_que_append(&(pt_this->t_tx_info_que), 
                                                          ui1_inmsg_id, 
                                                          EXTMJC_FLOW_TX_RESP);

        /* first message, send it */
        if (1 == ui1_tx_info_que_idx)
        {
            extmjc_flow_resp_send(pt_resp);
        }
        
        extmjc_prc_flow_unlock(pt_this);
        return;
    }

    /* Use free inmsg id to free relative request */
    if (0 != ui1_free_inmsg_id)
    {
        extmjc_flow_resp_que_free(
            &(pt_this->t_tx_resp_que), ui1_free_inmsg_id);
    }

    extmjc_flow_msgid_que_add_msg(
        &(pt_this->t_rec_req_id_que), ui1_inmsg_id, b_short_inmsg);

    extmjc_prc_flow_unlock(pt_this);
    
    /* dispatch message to relative message handler */
    extmjc_req_disp_disp_withresp_req(ui1_inmsg_id,
                                   ui1_inmsg_type,
                                   ui2_inmsg_body_len,
                                   pui1_inmsg_body);

}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_rec_resp 
 *
 * Description: Process response message
 *
 * Inputs: 
 *          ui1_inmsg_id        ID field
 *          ui2_inmsg_body_len  Length of the message body
 *          pui1_inmsg_body     Pointer points to the message body
 *
 * Outputs: -
 * 
 * Returns: 
----------------------------------------------------------------------------*/
VOID extmjc_prc_flow_rec_resp (
    BOOL                b_short_inmsg,
    UINT8               ui1_inmsg_id,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body)
{
    EXTMJC_PRC_FLOW_T* pt_this = pt_extmjc_prc_flow;
    EXTMJC_FLOW_WR_REQ_T*  pt_cormsg;
    
    UINT8               ui1_new_msg_id;
    UINT8               ui1_new_free_msg_id;
    UINT8               ui1_tx_info_que_idx;
    EXTMJC_FLOW_WR_REQ_T*  pt_waitmsg;
    EXTMJC_FLOW_WR_REQ_T*  pt_msg;
    
    extmjc_prc_flow_lock(pt_this);

    if (TRUE == extmjc_prc_flow_proc_connect_resp(
                pt_this,
                ui1_inmsg_id,
                ui2_inmsg_body_len,
                pui1_inmsg_body))
    {
        extmjc_prc_flow_unlock(pt_this);
        return;
    }
    
    /* drop a response without corresponding request */
    pt_cormsg = extmjc_flow_wr_req_que_get_msg(
                    &(pt_this->t_tx_wr_req_que), ui1_inmsg_id);

    if (NULL == pt_cormsg)
    {
        extmjc_prc_flow_unlock(pt_this);
        return;
    }

    /* Verify long / short format */
    if ( (pt_cormsg->ui1_msgtype < EXTMJC_LONG_MSG_FIRST_MSG_TYPE) &&
         (FALSE == b_short_inmsg) )
    {
        extmjc_prc_flow_unlock(pt_this);
        return;
    }

    if ( (pt_cormsg->ui1_msgtype >= EXTMJC_LONG_MSG_FIRST_MSG_TYPE) &&
         (TRUE == b_short_inmsg) )
    {
        extmjc_prc_flow_unlock(pt_this);
        return;
    }

    /* dispatch the response message relative message handler */
    extmjc_resp_disp_disp_resp(pt_cormsg->ui1_msgseq,
                            pt_cormsg->ui1_msgtype,
                            ui2_inmsg_body_len,
                            pui1_inmsg_body);

    /* Remove relative request */
    pt_this->ui1_wr_req_send_fail_cnt = EXTMJC_FLOW_WR_REQ_SEND_FAIL_MAX_CNT;

    extmjc_flow_wr_req_que_free(
        &(pt_this->t_tx_wr_req_que), ui1_inmsg_id);

    /* add the message id of the request to free msgid queue */
    extmjc_flow_msgid_que_add_msg(
        &(pt_this->t_rec_resp_id_que), ui1_inmsg_id, b_short_inmsg);

    /* If a wr req is in the wr req wait queue, send it to wr req que */
    pt_waitmsg = extmjc_flow_wr_req_wait_que_get_msg(
        &(pt_this->t_tx_wr_req_wait_que));
    
    if (NULL != pt_waitmsg)
    {
        if (FALSE == extmjc_flow_wr_req_que_has_free_entry(&(pt_this->t_tx_wr_req_que)))
        {
            ASSERT(FALSE);
        }

        /* Get the request message id */
        ui1_new_msg_id = extmjc_prc_flow_get_next_req_msgid(pt_this);

        /* Get one free msg id */
        ui1_new_free_msg_id = 
            extmjc_flow_msgid_que_get_and_remove_msg(&(pt_this->t_rec_resp_id_que));

        /* append to tx queue */
        ui1_tx_info_que_idx = 
            extmjc_flow_tx_info_que_append(&(pt_this->t_tx_info_que), 
                                        ui1_new_msg_id, 
                                        EXTMJC_FLOW_TX_WR_REQ);
    
        if (0 == ui1_tx_info_que_idx)
        {
            ASSERT(FALSE);
        }

        /* allocate a message */
        pt_msg = extmjc_flow_wr_req_que_alloc(&(pt_this->t_tx_wr_req_que), 
                                           ui1_new_msg_id);
    
        extmjc_flow_wr_req_set(pt_msg,
                            pt_waitmsg->ui1_retry_cnt,
                            pt_waitmsg->ui2_retry_timeout,
                            pt_waitmsg->ui1_msgseq,
                            ui1_new_msg_id,
                            ui1_new_free_msg_id,
                            pt_waitmsg->ui1_msgtype,
                            pt_waitmsg->ui2_msg_body_len,
                            pt_waitmsg->pui1_msg_body);

        /* Send message */
        if (1 == ui1_tx_info_que_idx)
        {
            extmjc_flow_wr_req_send(pt_msg);
        }
        
        extmjc_flow_wr_req_wait_que_remove(&(pt_this->t_tx_wr_req_wait_que));
    }

    extmjc_prc_flow_unlock(pt_this);
    
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_flow_dump_status 
 *
 * Description: Dump dvd flow manager current status
 *
 * Inputs: -
 *
 * Outputs: -
 * 
 * Returns: -
----------------------------------------------------------------------------*/
VOID extmjc_prc_flow_dump_status(VOID)
{
    EXTMJC_PRC_FLOW_T* pt_this = pt_extmjc_prc_flow;

    extmjc_prc_flow_lock(pt_this);

    /*
    c_dbg_stmt("\tConnect Status = %d, msg id = %d, req seq = %d, send fail count = %d\n",
               pt_this->ui1_status,
               pt_this->ui1_req_msgid,
               pt_this->ui1_req_seq,
               pt_this->ui1_wr_req_send_fail_cnt);
    */
    extmjc_prc_flow_unlock(pt_this);
}

#endif /* DRV_SUPPORT_EXTMJC */
