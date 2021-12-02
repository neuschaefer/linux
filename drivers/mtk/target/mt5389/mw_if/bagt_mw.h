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
 * $RCSfile: bagt_mw.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains buffer agent driver specific
 *         definitions, which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _BAGT_MW_H_
#define _BAGT_MW_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_common.h"
#include "x_buf_agt.h"
#include "drv_comp_id.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define BAGTR_OK                                ((INT32)  0)
#define BAGTR_INV_ID                            ((INT32) -1)
#define BAGTR_INV_ARG                           ((INT32) -2)
#define BAGTR_NOT_INIT                          ((INT32) -3)
#define BAGTR_ALREADY_CONN                      ((INT32) -4)
#define BAGTR_NOT_REGISTERED                    ((INT32) -5)
#define BAGTR_FAILED                            ((INT32) -6)
#define BAGTR_CLI_ERROR                         ((INT32) -7)

typedef enum
{
    BUF_AGT_MW_NFY_COND_UNKNOWN = 0,
    BUF_AGT_MW_NFY_COND_DATA_REQ_DONE,
    BUF_AGT_MW_NFY_COND_DATA_REQ_ERROR,
    BUF_AGT_MW_NFY_COND_DATA_REQ_CANCELED,
    BUF_AGT_MW_NFY_COND_FLUSH_DONE,
    BUF_AGT_MW_NFY_COND_ALIGN_INFO
} BUF_AGT_MW_NFY_COND_T;

#ifdef LINUX_TURNKEY_SOLUTION
typedef  BOOL (*buf_agt_reg_act_fct)(
    BAGT_COMP_ID_T                      ui2_buf_agt_id,
    BUF_AGT_DATA_REQ_ACK_COND_T e_ack_cond,
    BUF_AGT_DATA_BLK_T*         pt_data_blk);
#endif

typedef BOOL (*buf_agt_mw_nfy_fct) (
    VOID*                     pv_tag,
    BUF_AGT_MW_NFY_COND_T     e_nfy_cond,
    UINT32                    ui4_data_1,
    UINT32                    ui4_data_2);

typedef BOOL (*buf_agt_mw_get_position_fct) (
    VOID*       pv_tag,
    UINT32*     pui4_current_req_id,
    UINT32*     pui4_current_byte_offset);

extern INT32 buf_agt_mw_connect(
    BAGT_COMP_ID_T                      ui2_buf_agt_id,
    buf_agt_mw_nfy_fct          pf_nfy,
    buf_agt_mw_get_position_fct pf_get_pos, 
    VOID*                       pv_tag);

extern INT32 buf_agt_mw_disconnect(
    BAGT_COMP_ID_T  ui2_buf_agt_id);

extern INT32 buf_agt_mw_get_source_info( 
    BAGT_COMP_ID_T                    ui2_buf_agt_id,
    BUF_AGT_SRC_ALIGN_INFO_T* pt_data_blk);

extern INT32 buf_agt_mw_data_req(
    BAGT_COMP_ID_T                    ui2_buf_agt_id,
    BUF_AGT_DATA_BLK_T*       pt_data_blk);

extern INT32 buf_agt_mw_flush(
    BAGT_COMP_ID_T                    ui2_buf_agt_id);

extern INT32 buf_agt_mw_data_consumed(
    BAGT_COMP_ID_T                  ui2_buf_agt_id,
    BUF_AGT_DATA_BLK_T*     pt_data_blk);

extern INT32 buf_agt_mw_data_overflow(
    BAGT_COMP_ID_T                  ui2_buf_agt_id);

extern INT32 buf_agt_mw_data_underflow(
    BAGT_COMP_ID_T                  ui2_buf_agt_id);

extern INT32 buf_agt_dmx_connect(
    BAGT_COMP_ID_T                      ui2_buf_agt_id);

extern INT32 buf_agt_dmx_disconnect(
    BAGT_COMP_ID_T  ui2_buf_agt_id);

#ifdef CC_53XX_SWDMX_V2
extern INT32 buf_agt_set_feedersrc(
    BAGT_COMP_ID_T ui2_buf_agt_id, 
    UINT32 u4SrcType,
    UINT8 u1SwdmxId);
#endif

#ifdef LINUX_TURNKEY_SOLUTION
extern BOOL buf_agt_req_ack_fct(
    UINT16                      ui2_buf_agt_id,
    BUF_AGT_DATA_REQ_ACK_COND_T e_ack_cond,
    BUF_AGT_DATA_BLK_T*         pt_data_blk);
#endif

#endif /* _BAGT_MW_H_ */
