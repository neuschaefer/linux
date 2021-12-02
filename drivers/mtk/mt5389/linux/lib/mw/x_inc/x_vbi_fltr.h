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
 * $RCSfile: x_vbi_fltr.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains public interfaces, constant and type 
 *         definitions of the vbi filter.
 *---------------------------------------------------------------------------*/

#ifndef _X_VBI_FLTR_H_
#define _X_VBI_FLTR_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_vbi_fltr.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: x_vbif_open
 *
 * Description: This API creates an instance of data filter dedicated to a specified 
 *              data type for a specified VBI source. Note that an instance of VBI 
 *              filter can only filter data with single type in a single VBI source.
 *              However, no limit is set on the number of filter instance for same
 *              data type in same VBI source. This makes multiple clients in the system 
 *              possible to acquire the same VBI data concurrently.
 *              
 *              When the opening process is finished, the notify function 
 *              (*x_vbif_nfy_fct) will be called with e_reason=VBIF_NFY_REASON_OPENED 
 *
 * Inputs:  e_fltr_type   Specifies the type of the data filter.
 *          pv_fltr_info  Contains the parameters for opening the specified data filter.
 *                        For example, if "e_fltr_type"=FLTR_TYPE_TTX_PKT, "pv_fltr_info"
 *                        shall point to a structure with type (FLTR_INIT_TTX_PKT_T *).
 *          e_src_type    Specifies the type of the VBI source.
 *          pv_src_info   Contains the parameters for opening the specified VBI source.
 *                        For example, if e_src_type == VBIF_SRC_TYPE_DIG_VBI_ES, 
 *                        pv_src_info should point to a structure 
 *                        of type (VBIF_SRC_DIG_VBI_ES_T *), which contains the parameters 
 *                        for opening the digital VBI elementary stream.
 *          e_fltr_mode   Specifies the filter mode, one of load, monitor, or Pump.
 *                           - VBIF_FLTR_MODE_LOAD : load the data, notify ONLY once and 
 *                                                   notify ONLY when data is first received.
 *                           - VBIF_FLTR_MODE_MONITOR : monitor the content of data, notify 
 *                                                      whenever data is changed
 *                           - VBIF_FLTR_MODE_PUMP : pump the data to client whenever data
 *                                                   is received.
 *          pv_nfy_tag    Contains the notify tag value.
 *          pf_nfy        References the notify function.
 *
 * Outputs: ph_filter     Contains a handle which references the newly created 
 *                        data filter instance.
 *          pe_cond       Contains the pointer to a variable to hold the newly created 
 *                        filter's condition. This argument can be NULL.
 *
 * Returns: VBIFR_OK                        Routine successful.
 *          VBIFR_INV_ARG                   Invalid arguments.
 *          VBIFR_NOT_SUPPORT_FLTR_TYPE     Filter type not supported.
 *          VBIFR_NOT_SUPPORT_SRC_TYPE      Source type not supported.
 *          VBIFR_NOT_SUPPORT_FLTR_MODE     Filter mode not supported.
 *          VBIFR_OUT_OF_MEMORY             Out of memory.
 *          VBIFR_INTERNAL_ERROR            Internal error.
 *          < 0                             Other error.
 ----------------------------------------------------------------------------*/
INT32 x_vbif_open(
    VBIF_FLTR_TYPE_T                e_fltr_type,
    const VOID*                     pv_fltr_info,
    VBIF_SRC_TYPE_T                 e_src_type,
    const VOID*                     pv_src_info,
    VBIF_FLTR_MODE_T                e_fltr_mode,
    const VOID*                     pv_nfy_tag,
    x_vbif_nfy_fct                  pf_nfy,
    HANDLE_T*                       ph_filter,
    VBIF_COND_T*                    pe_cond);

/*-----------------------------------------------------------------------------
 * Name: x_vbif_close
 *
 * Description: This API closes a data filter instance. No matter this API is called 
 *              in which condition, VBI filter will automatically close itself. 
 *              For example, if this API is called when VBI filter is in VBIF_COND_STARTED,
 *              VBI filter would then automatically go through VBIF_COND_STOPPING, 
 *              VBIF_COND_OPENED, VBIF_COND_CLOSING, and finally VBIF_COND_CLOSED.
 *              During the closing process, the notify function (*x_vbif_nfy_fct) will be 
 *              called with e_cond == <VBIF_COND_***> and e_reason == NFY_REASON_COND_CHANGED, 
 *              corresponding to each state change.
 *
 * Inputs:  h_filter  Specifies the handle of the data filter's instance.
 *
 * Outputs: pe_cond   Contains the condition after this API is called. 
 *                    *pe_cond would be undetermined if the function fails.
 *                    This argument can be NULL.
 *
 * Returns: VBIFR_OK                        Routine successful.
 *          VBIFR_INV_HANDLE                Invalid VBI filter handle.
 *          VBIFR_LOCK_ERROR                Internal lock error.
 *          VBIFR_MSG_Q_OVERFLOW            Message queue overflow.
 *          VBIFR_INTERNAL_ERROR            Internal error.
 *          < 0                             Other error.
 ----------------------------------------------------------------------------*/
INT32 x_vbif_close(
    HANDLE_T                        h_filter,
    VBIF_COND_T*                    pe_cond);

/*-----------------------------------------------------------------------------
 * Name: x_vbif_close_sync
 *
 * Description: This API synchronously closes a VBI filter instance. The behavior
 *              is similar to x_vbif_close () API except that the caller thread 
 *              would be blocked until the VBI filter instance finishes 
 *              the close process.
 *
 * Inputs:  h_filter  Specifies the handle of the data filter's instance.
 *
 * Outputs: pe_cond   Contains the condition after this API is called. 
 *                    If this API successfully returns, the condition shall always  
 *                    be VBIF_COND_CLOSED.
 *
 * Returns: VBIFR_OK                        Routine successful.
 *          VBIFR_INV_HANDLE                Invalid VBI filter handle.
 *          VBIFR_LOCK_ERROR                Internal lock error.
 *          VBIFR_MSG_Q_OVERFLOW            Message queue overflow.
 *          VBIFR_INTERNAL_ERROR            Internal error.
 *          < 0                             Other error.
 ----------------------------------------------------------------------------*/
INT32 x_vbif_close_sync (
    HANDLE_T                        h_filter,
    VBIF_COND_T*                    pe_cond);

/*-----------------------------------------------------------------------------
 * Name: x_vbif_start
 *
 * Description: This API starts data filtering. This API is effective ONLY when the 
 *              filter is in one of the following condition: 
 *                  - VBIF_COND_OPENING, 
 *                  - VBIF_COND_OPENED, 
 *                  - VBIF_COND_STARTING, 
 *                  - VBIF_COND_STOPPING 
 *              During the starting process, the notify function (*x_vbif_nfy_fct)
 *              will be called with e_cond == <VBIF_COND_***> 
 *              and e_reason == NFY_REASON_COND_CHANGED, 
 *              corresponding to each state change.
 *
 * Inputs:  h_filter  Specifies the handle of the filter's instance.
 *          
 * Outputs: pe_cond   Contains the condition after this API is called.
 *
 * Returns: VBIFR_OK                        Routine successful.
 *          VBIFR_INV_HANDLE                Invalid VBI filter handle.
 *          VBIFR_LOCK_ERROR                Internal lock error.
 *          VBIFR_MSG_Q_OVERFLOW            Message queue overflow.
 *          VBIFR_INTERNAL_ERROR            Internal error.
 *          < 0                             Other error.
 ----------------------------------------------------------------------------*/
INT32 x_vbif_start(
    HANDLE_T                        h_filter,
    VBIF_COND_T*                    pe_cond);

/*-----------------------------------------------------------------------------
 * Name: x_vbif_stop
 *
 * Description: This API stops data filtering. This API takes effect ONLY when 
 *              the filter is in VBIF_COND_STARTED condition. When the filter finishes 
 *              the stopping process, the notify function (*x_vbif_nfy_fct) will be 
 *              called with e_cond == VBIF_COND_OPENED and e_reason == NFY_REASON_COND_CHANGED.
 *
 * Inputs:  h_filter  Specifies the handle of the filter's instance.
 *          
 * Outputs: pe_cond   Contains the condition after this API is called.
 *
 * Returns: VBIFR_OK                        Routine successful.
 *          VBIFR_INV_HANDLE                Invalid VBI filter handle.
 *          VBIFR_LOCK_ERROR                Internal lock error.
 *          VBIFR_MSG_Q_OVERFLOW            Message queue overflow.
 *          VBIFR_INTERNAL_ERROR            Internal error.
 *          < 0                             Other error.
 ----------------------------------------------------------------------------*/
INT32 x_vbif_stop(
    HANDLE_T                        h_filter,
    VBIF_COND_T*                    pe_cond);

/*-----------------------------------------------------------------------------
 * Name: x_vbif_set
 *
 * Description: This API performs a set operation to the filter.
 *
 * Inputs:  h_filter        Specifies the handle of the filter's instance.
 *          e_set_type      Specifies the type of information which is
 *                          referenced by argument pv_set_info.
 *          pv_set_info     Contains the set information.
 *          z_set_info_len  Indicates the length of the data in the buffer
 *                          referenced by argument pv_set_info.
 *          
 * Outputs: pe_cond         Contains the condition after this API is called.
 *
 * Returns: VBIFR_OK                        Routine successful.
 *          VBIFR_INV_ARG                   Invalid arguments.
 *          VBIFR_INV_HANDLE                Invalid VBI filter handle.
 *          VBIFR_LOCK_ERROR                Internal lock error.
 *          VBIFR_MSG_Q_OVERFLOW            Message queue overflow.
 *          VBIFR_INTERNAL_ERROR            Internal error.
 *          < 0                             Other error.
 ----------------------------------------------------------------------------*/
INT32 x_vbif_set(
    HANDLE_T                        h_filter,
    VBIF_SET_TYPE_T                 e_set_type,
    const VOID*                     pv_set_info,
    SIZE_T                          z_set_info_len,
    VBIF_COND_T*                    pe_cond);


/*-----------------------------------------------------------------------------
 * Name: x_vbif_get
 *
 * Description: This API performs a get operation to the filter.
 *
 * Inputs:  h_filter         Specifies the handle of the filter's instance.
 *          e_get_type       Specifies the type of information which shall be
 *                           returned in the buffer, referenced by argument
 *                           pv_get_info.
 *          pz_get_info_len  Contains the length of the buffer which is
 *                           referenced by argument pv_get_info.
 *          
 * Outputs: pv_get_info      Contains the retrieved information.
 *          pz_get_info_len  Indicates the length of the valid data in the
 *                           buffer referenced by argument pv_get_info.
 *          pe_cond          Contains the condition or state of the filter.
 *
 * Returns: VBIFR_OK                        Routine successful.
 *          VBIFR_INV_ARG                   Invalid arguments.
 *          VBIFR_INV_HANDLE                Invalid VBI filter handle.
 *          VBIFR_LOCK_ERROR                Internal lock error.
 *          VBIFR_INTERNAL_ERROR            Internal error.
 *          < 0                             Other error.
 ----------------------------------------------------------------------------*/
INT32 x_vbif_get(
    HANDLE_T                        h_filter,
    VBIF_GET_TYPE_T                 e_get_type,
    VOID*                           pv_get_info,
    SIZE_T*                         pz_get_info_len,
    VBIF_COND_T*                    pe_cond);


#ifdef __cplusplus
}
#endif


#endif

