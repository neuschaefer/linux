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
 * $RCSfile: x_sb.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/11 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: b8a039cd49cf517913bb5080795df307 $
 *
 * Description:
 *         This header file contains SVL Builder specific
 *         definitions, which are shared with other modules.
 *---------------------------------------------------------------------------*/
#ifndef _X_SB_H_
#define _X_SB_H_

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_SVL_BLDR
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_sb.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

BOOL x_sb_is_inited(VOID);

INT32 x_sb_open_builder(
    const CHAR*         ps_builder_name,
    VOID*               pv_open_data,
    VOID*               pv_nfy_tag,
    x_sb_nfy_fct        pf_nfy,
    HANDLE_T*           ph_builder,
    SB_COND_T*          pe_builder_cond);

INT32 x_sb_close_builder(
    HANDLE_T            h_builder);

INT32 x_sb_start_scan(
    HANDLE_T            h_builder,
    VOID*               pv_scan_data,
    VOID*               pv_nfy_tag,
    x_sb_scan_nfy_fct   pf_nfy,
    SB_COND_T*          pe_builder_cond);

INT32 x_sb_cancel_scan(
    HANDLE_T            h_builder);

INT32 x_sb_save_lists(
    HANDLE_T            h_builder,
    const CHAR*         ps_file_name);

INT32 x_sb_load_lists(
    HANDLE_T            h_builder,
    const CHAR*         ps_file_name);

INT32 x_sb_clean_lists(
    HANDLE_T            h_builder,
    const CHAR*         ps_file_name);

INT32 x_sb_get(
    HANDLE_T            h_builder,
    SB_KEY_TYPE_T       e_key_type,
    VOID*               pv_key_info,
    SIZE_T*             pz_key_data_len,
    VOID*               pv_key_data);

INT32 x_sb_set(
    HANDLE_T            h_builder,
    SB_KEY_TYPE_T       e_key_type,
    VOID*               pv_key_info,
    SIZE_T              z_key_data_len,
    VOID*               pv_key_data);

/* cliu 20090706,  Declaration*/
INT32 x_sb_cancel_scan_sync(
    HANDLE_T            h_builder);

INT32 x_sb_reg_scan_map(
    const CHAR*          ps_name,
    VOID*                pv_nfy_fct,
    VOID*                pv_data);


INT32 x_sb_get_cond(
    HANDLE_T            h_builder,
    SB_COND_T*          pe_builder_cond);

UINT16 x_sb_get_dbg_level(VOID);

BOOL x_sb_set_dbg_level(
    UINT16              ui2_level);


/*----------------------------------------------------------------------------*/
/*! @} */                                          /* end of groupMW_SVL_BLDR */
/*----------------------------------------------------------------------------*/

#endif /* _X_SB_H_ */

