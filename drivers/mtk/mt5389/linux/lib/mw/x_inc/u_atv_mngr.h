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
 * $RCSfile: u_atv_mngr.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/5 $
 * $SWAuthor: Albert Liu $
 * $MD5HEX: bcd09d703137357b24d195d8f5601a3a $
 *
 * Description: 
 *         This header file contains ATV Manager specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _U_ATV_MNGR_H_
#define _U_ATV_MNGR_H_


#include "u_common.h"
#include "u_handle.h"
#include "u_sys_name.h"
#include "u_handle_grp.h"

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/


/* ATV Manager Return Code */
#define ATVMR_OK                                    ((INT32)     0)  
#define ATVMR_INV_ARG                               ((INT32)    -1)
#define ATVMR_REG_HANDLER_NO_ENTRY                  ((INT32)    -2)
#define ATVMR_REG_HANDLER_ALLOC_FAILED              ((INT32)    -3)
#define ATVMR_UPDATE_HANDLER_NOT_FOUND              ((INT32)    -4)
#define ATVMR_UNREG_HANDLER_NOT_FOUND               ((INT32)    -5)
#define ATVMR_UART_NOT_SUPPORT                      ((INT32)    -6)
#define ATVMR_TX_MSG_QUE_FULL                       ((INT32)    -7)
#define ATVMR_TX_MSG_XLS_FAILED                     ((INT32)    -8)
#define ATVMR_CONNECT_NOT_READY                     ((INT32)    -9)
#define ATVMR_TX_INV_MSG                            ((INT32)    -10)
#define ATVMR_INV_MSG_BODY_LEN                      ((INT32)    -11)
#define ATVMR_INV_MSG_BODY_CONTENT                  ((INT32)    -12)
#define ATVMR_AEE_OUT_OF_RESOURCES                  ((INT32)    -13)
#define ATVMR_AEE_NO_RIGHTS                         ((INT32)    -14)
#define ATVMR_INV_STATE                             ((INT32)    -15)

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

typedef VOID (*x_atv_msg_handler_nfy_fct)(
    HANDLE_T        h_handle,
    VOID*           pv_tag,
    UINT8           ui1_inmsg_id,
    UINT8   ui1_inmsg_type,
    UINT16          ui2_inmsg_body_len,
    const UINT8*    pui1_inmsg_body);

typedef VOID (*x_atv_fail_handler_nfy_fct)(
    HANDLE_T        h_handle,
    VOID*           pv_tag,
    UINT8           ui1_inmsg_id,
    UINT8           ui1_inmsg_type);



#endif /* _U_ATV_MNGR_H_ */
