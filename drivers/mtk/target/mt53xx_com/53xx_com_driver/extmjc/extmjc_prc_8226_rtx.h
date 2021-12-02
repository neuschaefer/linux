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
 * $RCSfile: extmjc_prc_8226_rtx.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         This header file contains DVD receive and transmit manager (DVD_PRC_RTX)
 *         constants, macros, types, and functions.
 * History:
 *           
 *---------------------------------------------------------------------------*/

#ifndef __EXTMJC_PRC_8226_RTX_H_
#define __EXTMJC_PRC_8226_RTX_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_common.h"
#include "x_typedef.h"
#include "x_assert.h"
#include "x_os.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define EXTMJC_PRC_RTX_TX_EXTMSG_BUF_SIZE  ((UINT16) 220)

#define EXTMJC_PRC_RTX_RX_BUF_SIZE         ((UINT16) 256)
#define EXTMJC_PRC_RTX_RX_INMSG_BUF_SIZE   ((UINT16) 50)
 
typedef struct _EXTMJC_PRC_RTX_T
{
    BOOL                    b_is_mjc_upgrade;

    /* Transmit data members */
    HANDLE_T                h_tx_mtx;
    HANDLE_T                h_tx_delay_timer;
    UINT8*                  pui1_tx_extmsg_buf;
    UINT32                  ui4_tx_extmsg_buf_max_size;
    UINT32                  ui4_tx_extmsg_size;
    UINT32                  ui4_tx_extmsg_idx;
    BOOL                    b_in_empty_nfy;

    /* Receive data members */
    HANDLE_T                h_rx_thread;
    UINT8                   aui1_rx_exmsg_buf[2][EXTMJC_PRC_RTX_RX_BUF_SIZE];
    UINT8*                  pui1_rx_rec_buf;
    UINT16                  ui2_rx_rec_data_size;
    UINT8*                  pui1_rx_prc_buf;
    UINT16                  ui2_rx_prc_data_size;
    UINT16                  ui2_rx_inmsg_buf_max_size;
    UINT8*                  pui1_rx_inmsg_buf;
} EXTMJC_PRC_RTX_T;


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
INT32 extmjc_prc_rtx_init(EXTMJC_PRC_RTX_T *pt_this);

VOID extmjc_prc_rtx_set_upgrade(BOOL b_is_mjc_upgrade);

INT32 extmjc_prc_rtx_send_msg(
    BOOL                b_reliable_inmsg,
    BOOL                b_short_resp,
    UINT8               ui1_inmsg_id,
    UINT8               ui1_inmsg_free_id,
    UINT8               ui1_inmsg_type,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body);

VOID extmjc_prc_rtx_tx_handler(VOID* pv_tag);


#endif /* __EXTMJC_PRC_8226_RTX_H_ */


