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
 * $RCSfile: x_smc.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: $
 *
 * Description:
 *         This header file contains Smart Card driver specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_SMC_H_
#define _X_SMC_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_common.h"
#include "x_rm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Get operations */
#define SMC_GET_TYPE_CARD_PRESENCE          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define SMC_GET_TYPE_STATUS                 (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))
#define SMC_GET_TYPE_PROTOCOL               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))
#define SMC_GET_TYPE_CHECKSUM_TYPE          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 3))
#define SMC_GET_TYPE_RECEIVE_CMD            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 4))

/* Set operations */
#define SMC_SET_TYPE_INIT                   (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0))
#define SMC_SET_TYPE_TERMINATE              (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1))
#define SMC_SET_TYPE_RESET                  (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2))
#define SMC_SET_TYPE_WARM_RESET             (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3))
#define SMC_SET_TYPE_NFY_FCT                (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4))
#define SMC_SET_TYPE_PARAM                  (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5))
#define SMC_SET_TYPE_SEND_CMD               (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6))
#define SMC_SET_TYPE_SEND_RECEIVE_CMD       (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 7))

/* Card status */
typedef enum
{
    SMC_CARD_STS_PRESENT = 0,    
    SMC_CARD_STS_ABSENT,
    SMC_CARD_STS_ERROR,
    SMC_CARD_STS_INIT_FAIL
}   SMC_CARD_STS_T;

/* SMC command */
typedef struct _SMC_CMD_T
{
    UINT8       ui1_blk_type;   /* Block type (e.g. info, receive, supervisory) */
    UINT8       ui1_ins_code;   /* Instruction code */
    UINT8       ui1_ns;         /* Send sequence number */
}   SMC_CMD_T;

/* SMC response */
typedef struct _SMC_RESPONSE_T
{
    UINT8       ui1_protocol_num;   /* Version of IC card */
    UINT8       ui1_unit_len;       /* Length of payload */
    UINT16      ui1_card_ins;       /* Instruction from smart card */
    UINT16      ui1_rtn_code;       /* Process result of smart card */
}   SMC_RESPONSE_T;
    
/* SMC octet */
typedef struct _SMC_OCTET_T
{
    UINT8*      pui1_data;
    SIZE_T      z_data_size;
}   SMC_OCTET_T;

/* SMC send/receive command info */
typedef struct _SMC_COMMAND_INFO_T
{
    SMC_CMD_T       t_cmd;
    SMC_OCTET_T     t_data;
}   SMC_COMMAND_INFO_T;

/* SMC send & receive info */
typedef struct _SMC_TX_RX_INFO_T
{
    SMC_OCTET_T         t_send_data;
    SMC_RESPONSE_T      t_response;
    SMC_OCTET_T         t_response_data;
}   SMC_TX_RX_INFO_T;

/* Notify function */
typedef VOID (*x_smc_nfy_fct)(VOID*             pv_nfy_tag,
                              SMC_CARD_STS_T    e_nfy_cond,
                              UINT32            ui4_data);

/* Notify setting structure */
typedef struct _SMC_NFY_INFO_T
{
    VOID*           pv_tag;
    x_smc_nfy_fct   pf_nfy;
}   SMC_NFY_INFO_T;       

#endif /* _X_SMC_H_ */

