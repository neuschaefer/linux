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
 * $RCSfile: extmjc_prc.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         This header file contains DVD protocol specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _EXTMJC_PRC_H_
#define _EXTMJC_PRC_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_common.h"
#include "x_typedef.h"
#include "x_assert.h"
#include "x_os.h"


/* EXTMJC Protocol Return Code */
#define EXTMJC_PRCR_OK                                    ((INT32)     0)  
#define EXTMJC_PRCR_INV_ARG                               ((INT32)    -1)
#define EXTMJC_PRCR_REG_HANDLER_NO_ENTRY                  ((INT32)    -2)
#define EXTMJC_PRCR_REG_HANDLER_ALLOC_FAILED              ((INT32)    -3)
#define EXTMJC_PRCR_UPDATE_HANDLER_NOT_FOUND              ((INT32)    -4)
#define EXTMJC_PRCR_UNREG_HANDLER_NOT_FOUND               ((INT32)    -5)
#define EXTMJC_PRCR_UART_NOT_SUPPORT                      ((INT32)    -6)
#define EXTMJC_PRCR_TX_MSG_QUE_FULL                       ((INT32)    -7)
#define EXTMJC_PRCR_TX_MSG_XLS_FAILED                     ((INT32)    -8)
#define EXTMJC_PRCR_CONNECT_NOT_READY                     ((INT32)    -9)
#define EXTMJC_PRCR_TX_INV_MSG                            ((INT32)    -10)
#define EXTMJC_PRCR_INV_MSG_BODY_LEN                      ((INT32)    -11)
#define EXTMJC_PRCR_INV_MSG_BODY_CONTENT                  ((INT32)    -12)

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* XLS relative definition */
typedef BOOL (*extmjc_prc_xls_compose_extmsg_fct)(UINT8 ui1_data);

typedef VOID (*extmjc_prc_xls_compose_extmsg_reset_fct) (VOID);

typedef INT32 (*extmjc_prc_xls_extmsg_to_inmsg_fct)(
    BOOL*       pb_reliable_inmsg,
    BOOL*       pb_short_resp,
    UINT8*      pui1_inmsg_id,
    UINT8*      pui1_inmsg_free_id,
    UINT8*      pui1_inmsg_type,
    UINT8*      pui1_inmsg_body,
    UINT16      ui2_inmsg_body_max_len,
    UINT16*     pui2_inmsg_body_len);

typedef INT32 (*extmjc_prc_xls_inmsg_to_extmsg_fct)(
    BOOL        b_reliable_inmsg,
    BOOL        b_short_resp,
    UINT8       ui1_inmsg_id,
    UINT8       ui1_inmsg_free_id,
    UINT8       ui1_inmsg_type,
    UINT16      ui2_inmsg_body_len,
    UINT8*      pui1_inmsg_body,
    UINT8*      pui1_extmsg,
    UINT32      ui4_extmsg_max_len,
    UINT32*     pui4_extmsg_len);

typedef enum 
{
    REQ_MSG,
    LONG_RESP_MSG,
    SHORT_RESP_MSG
} EXTMJC_PRC_XLS_VALID_MSG_TYPE;

typedef INT32 (*extmjc_prc_xls_valid_inmsg_fct)(
    BOOL                        b_reliable_inmsg,
    EXTMJC_PRC_XLS_VALID_MSG_TYPE  t_msg_type,
    UINT8                       ui1_inmsg_type,
    UINT16                      ui2_inmsg_body_len,
    UINT8*                      pui1_inmsg_body);

typedef struct _EXTMJC_PRC_XLS_T
{
    extmjc_prc_xls_compose_extmsg_reset_fct    pf_extmjc_prc_xls_compose_extmsg_reset;
    extmjc_prc_xls_compose_extmsg_fct          pf_extmjc_prc_xls_compose_extmsg;
    extmjc_prc_xls_extmsg_to_inmsg_fct         pf_extmjc_prc_xls_extmsg_to_inmsg;
    extmjc_prc_xls_inmsg_to_extmsg_fct         pf_extmjc_prc_xls_inmsg_to_extmsg;
    extmjc_prc_xls_valid_inmsg_fct             pf_extmjc_prc_xls_valid_inmsg;
} EXTMJC_PRC_XLS_T;


#define EXTMJC_PRC_XLS_OK                                ((INT32)    0)
#define EXTMJC_PRC_XLS_INSUFFICIENT_BUF                  ((INT32)    -1)
#define EXTMJC_PRC_XLS_UNKNOWN_MSG                       ((INT32)    -2)
#define EXTMJC_PRC_XLS_XLS_IN_WRONG_STATE                ((INT32)    -3)
#define EXTMJC_PRC_XLS_WRONG_BODY_LEN                    ((INT32)    -4)
#define EXTMJC_PRC_XLS_INV_MSG_TYPE                      ((INT32)    -5)


/* Handler relative definition */
typedef VOID (*extmjc_prc_msg_handler_nfy_fct)(
    UINT8           ui1_inmsg_id,
    UINT8           ui1_inmsg_type,
    UINT16          ui2_inmsg_body_len,
    const UINT8*    pui1_inmsg_body);

typedef VOID (*extmjc_prc_fail_handler_nfy_fct)(
    UINT8           ui1_inmsg_id,
    UINT8           ui1_inmsg_type);

typedef VOID (*extmjc_prc_connect_nfy_fct) (BOOL   b_connect);
                                         

/* protocol initial configuration */
typedef struct _EXTMJC_PRC_CONFIG_T
{
    /* Setting of receive and transmit */
    UINT8                           ui1_tx_delay_time;
    UINT8                           ui1_tx_delay_byte_cnt;
    
    /* Setting of Flow control */
    UINT8                           ui1_max_reliable_msg_cnt;
    UINT8                           ui1_noresp_req_buf_cnt;
    UINT8                           ui1_withresp_req_buf_cnt;
} EXTMJC_PRC_CONFIG_T;

/* message */
#define EXTMJC_PRC_MSG_TYPE_TX_EMPTY    0
#define EXTMJC_PRC_MSG_TYPE_TX_RETRY    1
#define EXTMJC_PRC_MSG_TYPE_PING        2
#define EXTMJC_PRC_MSG_TYPE_PING_FAIL   3

typedef struct _EXTMJC_PRC_MSG_T
{
    /* */
    UINT8                           ui1_msg_type;
    VOID*                           pt_obj;
} EXTMJC_PRC_MSG_T;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
INT32 extmjc_prc_init(const EXTMJC_PRC_CONFIG_T* pt_extmjc_prc_cfg);

INT32 extmjc_prc_send_msg(EXTMJC_PRC_MSG_T* pt_msg);

INT32 extmjc_prc_reg_xls(
    const EXTMJC_PRC_XLS_T * pt_extmjc_prc_xls);

BOOL extmjc_prc_is_connect(VOID);

VOID extmjc_prc_flow_timeout_handler(VOID* pv_arg);

INT32 extmjc_prc_send_noresp_req(
    UINT8       ui1_inmsg_type,
    UINT16      ui2_inmsg_body_len,
    UINT8*      pui1_inmsg_body);

INT32 extmjc_prc_send_withresp_req(
    UINT8       ui1_send_retry_cnt,
    UINT16      ui2_send_timeout,
    UINT8       ui1_inmsg_type,
    UINT16      ui2_inmsg_body_len,
    UINT8*      pui1_inmsg_body,
    UINT8*      pui1_inmsg_id);

INT32 extmjc_prc_send_resp(
    UINT8       ui1_inmsg_id,
    UINT8       ui1_inmsg_type,
    UINT16      ui2_inmsg_body_len,
    UINT8*      pui1_inmsg_body);

INT32 extmjc_prc_reg_handler(
    extmjc_prc_msg_handler_nfy_fct     pf_extmjc_prc_noresp_msg_handler,
    extmjc_prc_msg_handler_nfy_fct     pf_extmjc_prc_withresp_msg_handler,
    extmjc_prc_msg_handler_nfy_fct     pf_extmjc_prc_resp_msg_handler,
    extmjc_prc_fail_handler_nfy_fct    pf_extmjc_prc_resp_fail_handler,
    extmjc_prc_connect_nfy_fct         pf_extmjc_prc_connect_handler);

INT32 extmjc_req_disp_disp_noresp_req(
    UINT8               ui1_msg_type,
    UINT16              ui2_msg_body_len,
    UINT8*              pui1_msg_body);

INT32 extmjc_req_disp_disp_withresp_req(
    UINT8               ui1_msg_id,
    UINT8               ui1_msg_type,
    UINT16              ui2_msg_body_len,
    UINT8*              pui1_msg_body);

INT32 extmjc_resp_disp_disp_resp(
    UINT8               ui1_msg_id,
    UINT8               ui1_msg_type,
    UINT16              ui2_msg_body_len,
    UINT8*              pui1_msg_body);

INT32 extmjc_resp_disp_fail_notify(
    UINT8               ui1_msg_id,
    UINT8               ui1_msg_type);

INT32 extmjc_prc_connect_notify(
    BOOL                b_connect);

#endif /* _EXTMJC_PRC_H_ */
