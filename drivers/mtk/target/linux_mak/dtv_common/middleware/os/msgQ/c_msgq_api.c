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
 * $RCSfile: c_msgq_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "handle/handle.h"

/*----------------------------------------------------------------------------
 * Function: c_msg_q_create()
 *
 * Description:
 *      this API creates a msg Q. it performs
 *      1. check if msg Q mudule is initiated. 
 *      2. parse argumets.
 *      3. allocate memory for msgq struct and for the specified number of
 *         messages, each with the specified size.
 *      4. link the message buffers.
 *      5. check existence of the msg Q (based on msg Q name) and enqueue
 *         the msgq struct in the global msgq list.
 *      6. initialize, allocate, and link a messaage haandle.
 *
 * Inputs:
 *      ps_name: name of this msg Q, null ('\0') terminated, up to 16
 *               characters excluding '\0' will be used.
 *      z_msg_size: this msg Q accepts a message up to this size of bytes.
 *      ui2_msg_count: this msg Q accespts this many messaages.
 *
 * Outputs:
 *      *ph_msg_hdl: pointer to hold returned msg Q handle.
 *                   the handle is valid only if this API returns OSR_OK.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : message Q module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_NO_RESOURCE: no resources (memory).
 *      OSR_FAIL: OS driver related errors.
 *      OSR_EXIST: msg Q exists (ps_name collision).
 *      OSR_INV_HANDLE: invalid handle.
 *      OSR_OUT_OF_HANDLES: no handle.
 *---------------------------------------------------------------------------*/
INT32
c_msg_q_create(HANDLE_T     *ph_msg_hdl,    /* msg q handle */
               const CHAR   *ps_name,
               SIZE_T       z_msg_size,
               UINT16       ui2_msg_count)
{
    return x_msg_q_create(ph_msg_hdl, ps_name, z_msg_size, ui2_msg_count);
}


/*----------------------------------------------------------------------------
 * Function: c_msg_q_attach()
 *
 * Description:
 *      this API creates a msg Q. it performs
 *      1. check if msg Q mudule is initiated. 
 *      2. parse argumets.
 *      3. check existence of the msg Q (based on msg Q name).
 *      4. initialize, allocate, and link a messaage haandle.
 *
 * Inputs:
 *      ps_name: name of this msg Q, null ('\0') terminated, up to 16
 *               characters excluding '\0'.
 *
 * Outputs:
 *      *ph_msg_hdl: pointer to hold returned msg Q handle.
 *                   the handle is valid only if this API returns OSR_OK.
 *                   the handle is to be used to access the created message Q.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : message Q module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_NOT_EXIST: msg Q not exist.
 *      OSR_INV_HANDLE: invalid handle.
 *      OSR_OUT_OF_HANDLES: no handle.
 *---------------------------------------------------------------------------*/
INT32
c_msg_q_attach(HANDLE_T     *ph_msg_hdl,    /* msg q handle */
               const CHAR   *ps_name)
{
    return x_msg_q_attach(ph_msg_hdl, ps_name);
}


/*----------------------------------------------------------------------------
 * Function: c_msg_q_delete()
 *
 * Description:
 *      this API creates a msg Q. it performs
 *      1. check if msg Q mudule is initiated. 
 *      2. check validity of msg Q handle.
 *      3. mark internaal close flag and call handle_free().
 *      4. remove the msgq struct from the global msgq link list.
 *
 * Inputs:
 *      h_msg_hdl: msg Q handle returned via c_msg_q_create() or
 *                 c_msg_q_attach().
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : message Q module has not been initiated.
 *      OSR_INV_HANDLE: an error in handle operation.
 *      OSR_FAIL: OS driver related errors.
 *---------------------------------------------------------------------------*/
INT32
c_msg_q_delete(HANDLE_T h_msg_hdl)
{
    return x_msg_q_delete(h_msg_hdl);
}


/*----------------------------------------------------------------------------
 * Function: c_msg_q_send()
 *
 * Description:
 *      this API creates a msg Q. it performs
 *      1. check if msg Q mudule is initiated. 
 *      2. parse argumets.
 *      3. allocate memory for msg Q struct and the specified number of
 *         messages, each with the specified size.
 *      4. link the message buffers.
 *      5. initialize, allocate, and link a messaage haandle.
 *
 * Inputs:
 *      h_msg_hdl: msg Q handle returned via c_msg_q_create() or
 *                 c_msg_q_attach().
 *      pv_msg: pointer to a memory holding the message to be sent.
 *      z_size: size in bytes of "pv_msg".
 *      ui1_pri: priority of this sending message, 0 (highest) - 255 (lowest).
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : message Q module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_TOO_BIG: the sending message is bigger than allowed, specified
 *                   in c_msg_q_create().
 *      OSR_TOO_MANY: too many messages queued in the msg Q, more that allowed,
 *                    specified in c_msg_q_create().
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32
c_msg_q_send(HANDLE_T   h_msg_hdl,      /* msg q handle */
             const VOID *pv_msg,
             SIZE_T     z_size,
             UINT8      ui1_pri)
{
    return x_msg_q_send(h_msg_hdl, pv_msg, z_size, ui1_pri);
}

/*----------------------------------------------------------------------------
 * Function: c_msg_q_receive()
 *
 * Description:
 *      this API creates a msg Q. it performs
 *      1. check if msg Q mudule is initiated. 
 *      2. parse argumets.
 *      3. allocate memory for polling in case of no message available.
 *      4. check msg Q handles' validity in "ph_msgq_hdl_list".
 *      5. call msg_q_get_msg() to go through each msg Q to get a message.
 *
 * Inputs:
 *      pv_msg: pointer to memory buffer.
 *      z_size: size in bytes of "pv_msg".
 *      *ph_msgq_hdl_list: array of message handles.
 *      ui2_msgq_hdl_count: array entry count.
 *      e_option: either X_MSGQ_OPTION_WAIT or X_MSGQ_OPTION_NOWAIT.
 *
 * Outputs:
 *      *pui2_index: index to array "ph_msgq_hdl_list", indicating which
 *                   msg Q handle returns a message or has an error.
 *      pv_msg: pointer to a memory holding the received message.
 *      z_size: size in bytes of the received message in "pv_msg".
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : message Q module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_INV_HANDLE: an error in handle operation.
 *      OSR_NO_RESOURCE: no resources (memory).
 *      OSR_FAIL: OS driver related errors.
 *      OSR_NO_MSG: no message to receive.
 *      OSR_BUSY: already a receiver waiting for a message.
 *---------------------------------------------------------------------------*/
INT32
c_msg_q_receive(UINT16          *pui2_index,
                VOID            *pv_msg,
                SIZE_T          *pz_size,
                HANDLE_T        *ph_msgq_hdl,
                UINT16          ui2_msgq_hdl_count,
                MSGQ_OPTION_T   e_option)
{
    return x_msg_q_receive(pui2_index, pv_msg, pz_size, ph_msgq_hdl, ui2_msgq_hdl_count, e_option);

}


/*----------------------------------------------------------------------------
 * Function: c_msg_q_receive_timeout()
 *
 * Description:
 *      this API creates a msg Q. it performs
 *      1. check if msg Q mudule is initiated. 
 *      2. parse argumets.
 *      3. allocate memory for polling in case of no message available.
 *      4. check msg Q handles' validity in "ph_msgq_hdl_list".
 *      5. call msg_q_get_msg() to go through each msg Q to get a message.
 *
 * Inputs:
 *      pv_msg: pointer to memory buffer.
 *      z_size: size in bytes of "pv_msg".
 *      *ph_msgq_hdl_list: array of message handles.
 *      ui2_msgq_hdl_count: array entry count.
 *      ui4_time: time in milliseconds before returning OSR_TIMEOUT
 *
 * Outputs:
 *      *pui2_index: index to array "ph_msgq_hdl_list", indicating which
 *                   msg Q handle returns a message or has an error.
 *      pv_msg: pointer to a memory holding the received message.
 *      z_size: size in bytes of the received message in "pv_msg".
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : message Q module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_INV_HANDLE: an error in handle operation.
 *      OSR_NO_RESOURCE: no resources (memory).
 *      OSR_FAIL: OS driver related errors.
 *      OSR_TIMEOUT: receive timed out.
 *      OSR_BUSY: already a receiver waiting for a message.
 *---------------------------------------------------------------------------*/
INT32
c_msg_q_receive_timeout(UINT16          *pui2_index,
                        VOID            *pv_msg,
                        SIZE_T          *pz_size,
                        HANDLE_T        *ph_msgq_hdl,
                        UINT16          ui2_msgq_hdl_count,
                        UINT32          ui4_time)
{
    return x_msg_q_receive_timeout(pui2_index, pv_msg, pz_size, ph_msgq_hdl, ui2_msgq_hdl_count, ui4_time);
}


/*----------------------------------------------------------------------------
 * Function: c_msg_q_num_msgs()
 *
 * Description:
 *      this API creates a msg Q. it performs
 *      1. check if msg Q mudule is initiated. 
 *      2. parse argumets.
 *      3. return message count.
 *
 * Inputs:
 *      h_msg_hdl: msg Q handle returned via c_msg_q_create() or
 *                 c_msg_q_attach().
 *
 * Outputs:
 *      *pui2_num_msgs: pointer which contains message counts.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : message Q module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32
c_msg_q_num_msgs(HANDLE_T   h_msg_hdl,      /* msg q handle */
                 UINT16     *pui2_num_msgs)
{
    return x_msg_q_num_msgs(h_msg_hdl, pui2_num_msgs);
}


/*----------------------------------------------------------------------------
 * Function: c_msg_q_get_max_msg_size()
 *
 * Description:
 *      this API creates a msg Q. it performs
 *      1. check if msg Q mudule is initiated. 
 *      2. parse argumets.
 *      3. return max. message size.
 *
 * Inputs:
 *      h_msg_hdl: msg Q handle returned via c_msg_q_create() or
 *                 c_msg_q_attach().
 *
 * Outputs:
 *      *pz_maxsize: pointer which contains max. msg size.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : message Q module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32
c_msg_q_get_max_msg_size(HANDLE_T   h_msg_hdl,      /* msg q handle */
                         SIZE_T     *pz_max_size)
{
    return x_msg_q_get_max_msg_size(h_msg_hdl, pz_max_size);
}
