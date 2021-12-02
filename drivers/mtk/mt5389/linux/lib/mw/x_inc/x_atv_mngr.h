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
 * $RCSfile: x_atv_mngr.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/7 $
 * $SWAuthor: Albert Liu $
 * $MD5HEX: fca6199f7c434f05d3b618b3e53cdaa9 $
 *
 * Description: 
 *         This header file contains ATV Manager exported constants, macros,
 *         types, and functions.
 * History:
 *           
 *---------------------------------------------------------------------------*/

#ifndef _X_ATV_MNGR_H_
#define _X_ATV_MNGR_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_handle.h" 
#include "x_common.h"
#include "u_sys_name.h"
#include "u_atv_mngr.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Data length */
typedef enum
{
    ATV_RS_232_DATA_LEN_4 = 0,
    ATV_RS_232_DATA_LEN_5,
    ATV_RS_232_DATA_LEN_6,
    ATV_RS_232_DATA_LEN_7,
    ATV_RS_232_DATA_LEN_8,
    ATV_RS_232_DATA_LEN_9
}   ATV_RS_232_DATA_LEN_T;

/* Parity */
typedef enum
{
    ATV_RS_232_PARITY_NONE = 0,
    ATV_RS_232_PARITY_EVEN,
    ATV_RS_232_PARITY_ODD,
    ATV_RS_232_PARITY_MARK,
    ATV_RS_232_PARITY_SPACE
}   ATV_RS_232_PARITY_T;

/* Data speed */
typedef enum
{
    ATV_RS_232_SPEED_75 = 0,
    ATV_RS_232_SPEED_110,
    ATV_RS_232_SPEED_134,
    ATV_RS_232_SPEED_150,
    ATV_RS_232_SPEED_300,
    ATV_RS_232_SPEED_600,
    ATV_RS_232_SPEED_1200,
    ATV_RS_232_SPEED_1800,
    ATV_RS_232_SPEED_2400,
    ATV_RS_232_SPEED_4800,
    ATV_RS_232_SPEED_9600,
    ATV_RS_232_SPEED_14400,
    ATV_RS_232_SPEED_19200,
    ATV_RS_232_SPEED_38400,
    ATV_RS_232_SPEED_57600,
    ATV_RS_232_SPEED_115200,
    ATV_RS_232_SPEED_128000,
    ATV_RS_232_SPEED_921600,
    ATV_RS_232_SPEED_230400,
    ATV_RS_232_SPEED_460800
}   ATV_RS_232_SPEED_T;

/* Stop bit */
typedef enum
{
    ATV_RS_232_STOP_BIT_1 = 0,
    ATV_RS_232_STOP_BIT_1_5,
    ATV_RS_232_STOP_BIT_2
}   ATV_RS_232_STOP_BIT_T;


/* Setup structure */
typedef struct _ATV_RS_232_SETUP_INFO_T
{
    ATV_RS_232_SPEED_T     e_speed;
    ATV_RS_232_DATA_LEN_T  e_data_len;
    ATV_RS_232_PARITY_T    e_parity;
    ATV_RS_232_STOP_BIT_T  e_stop_bit;
}   ATV_RS_232_SETUP_INFO_T;


typedef struct _X_ATV_MANAGER_CONFIG_T
{
    UINT8               ui1_version;
    
    /* Setting of dispatcher */
    UINT8               ui1_noresp_req_handler_max_cnt;
    UINT8               ui1_withresp_req_handler_max_cnt;
    UINT8               ui1_resp_handler_max_cnt;

    /* Setting of RTX manager */
    THREAD_DESCR_T      t_rtx_thread_info;
    UINT8               ui1_tx_delay_time;
    UINT8               ui1_tx_delay_byte_cnt;
    
    /* Setting of Flow Manager */
    UINT8               ui1_max_reliable_msg_cnt;
    UINT8               ui1_noresp_req_buf_cnt;
    UINT8               ui1_withresp_req_buf_cnt;

    CHAR                s_hardware_name[SYS_NAME_LEN + 1];
    ATV_RS_232_SETUP_INFO_T t_rs_232_setup_info;
} X_ATV_MANAGER_CONFIG_T;


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern BOOL x_atv_manager_is_init(VOID);

extern BOOL x_atv_manager_is_connect(VOID);

extern INT32 x_atv_manager_open(VOID);

extern INT32 x_atv_manager_send_noresp_req(
    UINT8   ui1_inmsg_type,
    UINT16  ui2_inmsg_body_len,
    UINT8*  pui1_inmsg_body);

extern INT32 x_atv_manager_send_withresp_req(
    HANDLE_T    h_handle,
    UINT8       ui1_send_retry_cnt,
    UINT16      ui2_send_timeout,
    UINT8       ui1_inmsg_type,
    UINT16      ui2_inmsg_body_len,
    UINT8*      pui1_inmsg_body,
    UINT8*      pui1_inmsg_id);

extern INT32 x_atv_manager_send_resp(
    UINT8   ui1_inmsg_id,
    UINT8   ui1_inmsg_type,
    UINT16  ui2_inmsg_body_len,
    UINT8*  pui1_inmsg_body);

extern INT32 x_atv_manager_reg_noresp_req_handler(
    UINT32*                     pui4_inmsg_type_table,
    x_atv_msg_handler_nfy_fct   pf_atv_msg_handler,
    VOID *  pv_tag,
    HANDLE_T *  ph_handle);

extern INT32 x_atv_manager_reg_withresp_req_handler(
    UINT32*                     pui4_inmsg_type_table,
    x_atv_msg_handler_nfy_fct   pf_atv_msg_handler,
    VOID*                       pv_tag,
    HANDLE_T*                   ph_handle);

extern INT32 x_atv_manager_reg_resp_handler(
    UINT32*                     pui4_inmsg_type_table,
    x_atv_msg_handler_nfy_fct   pf_atv_msg_handler,
    x_atv_fail_handler_nfy_fct  pf_atv_fail_handler,
    VOID*                       pv_tag,
    HANDLE_T*                   ph_handle);


extern INT32 x_atv_manager_unreg_handler(
    HANDLE_T                    h_handle);


#endif /* _X_ATV_MNGR_H_ */
