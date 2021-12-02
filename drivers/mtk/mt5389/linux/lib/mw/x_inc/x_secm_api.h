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
 * $RCSfile: x_secm_api.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_SECM_API_H_
#define _X_SECM_API_H_

#include "os.h"
#include "secm_api.h"
#include "secm.h"
#include "secm_cli.h"


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*
 * ph_src : a placeholder of a handle
 * pt_src : contains a reference which specifies the array size and array
 *          of SECM_SRC_INFO_T 
 */
extern INT32 x_secm_alloc_source(HANDLE_T            *ph_src,
                        SRC_COMP_TYPE_T     e_src_type,
                        VOID                *pv_src_comp);


/*
 * ph_logical : contains a reference to a handle
 * h_src : handle returned from secm_alloc_src()
 * t_pid: contains PID value
 * pf_nfy: notification fuction
 */ 
extern INT32 x_secm_alloc_raw_filter(HANDLE_T            *ph_logical,
                            HANDLE_T            h_src,
                            MPEG_2_PID_T        t_pid,
                            UINT8               ui1_flag,
                            SIZE_T              z_buf_size,
                            secm_nfy_fct        pf_nfy);

/*
 * h_logical : contains a handle, returned via secm_alloc_acq_filter() or
 *            secm_alloc_mon_filter().
 * b_ctrl: control operation, TRUE to enable the filter, FALSE to disable
 *         the filter
 */
extern INT32 x_secm_ctrl_filter(HANDLE_T h_logical,
                       BOOL     b_ctrl);

/*
 * t_pid: pid value.
 * b_ctrl: control operation, TRUE to enable the filter, FALSE to disable
 *         the filter
 */
extern INT32 x_secm_ctrl_pid_filter(MPEG_2_PID_T t_pid,
                           BOOL         b_ctrl);

/*
 * h_obj : contains a handle of a logical filter or a section filter or
 *         a source object.
 */
extern INT32 x_secm_close_obj(HANDLE_T   h_obj);


/*
 * h_logical : contains a handle, returned via secm_alloc_acq_filter() or
 *            secm_alloc_mon_filter().
 * ph_sec_req : contains a reference to a handle for this section request
 * h_src : handle return from secm_alloc_src()
 * pt_ts_descr: contains a reference to TS_DESCR_T
 * pv_nfy_tag: notification tag to pf_nfy in secm_alloc_acq_filter() or
 *             secm_alloc_mon_filter().
 * pt_sec_req: contains a reference to SECM_MON_REQ_T. this conatins bytes
 *             combination of the first 16 bytes of a section table.
 */ 
extern INT32 x_secm_set_raw_filter(HANDLE_T          h_logical,
                          HANDLE_T          *ph_sec_req,
                          TS_DESCR_T        *pt_ts_descr,
                          VOID              *pv_nfy_tag,
                          MPEG_2_PID_T      t_pid,
                          SECM_RAW_REQ_T    *pt_raw_req);

#endif /* _SECM_API_H_ */

