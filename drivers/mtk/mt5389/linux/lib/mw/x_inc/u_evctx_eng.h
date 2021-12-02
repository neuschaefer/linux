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
/*-----------------------------------------------------------------------------*/
/*! @file u_evctx_eng.h 
 * $RCSfile: u_evctx_eng.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/1 $
 * $SWAuthor: Justin Deng $
 * $MD5HEX: 837b3912ec66833817b2c33cd904c44f $
 *
 *  @par Description:
 *         This contains Event Context library macros, definitons, structures,
 *         and API's, which are shared with other modules, and customized engines.
 *---------------------------------------------------------------------------*/

#ifndef _U_EVCTX_ENG_H_
#define _U_EVCTX_ENG_H_

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_EVCTX
 *  @{
 */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_evctx.h"
#include "u_dt.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @brief  evctx create object function

 *  @param  [in] h_event            - handle of event object
 *  @param  [in] pt_svc_cmd         - event command
 *  @param  [in] pt_offset          - evctx offset
 *  @param  [out] ppv_event_obj_data - pointer of evctx obj data
 *  @param  [out] pe_cond            - pointer of evctx condition
 *  
 *  @note   Evctx Engine should implement such function pointer and register to evctx
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK           - Success
 */
/*----------------------------------------------------------------------------*/
typedef INT32 (*x_evctx_create_event_obj_fct) (
    HANDLE_T            h_event,
    EVCTX_COMMAND_T*    pt_svc_cmd,
    EVCTX_OFFSET_T*     pt_offset,
    VOID**              ppv_event_obj_data,
    EVCTX_COND_T*       pe_cond);

/*----------------------------------------------------------------------------*/
/*! @brief  evctx delete object function

 *  @param  [in] pv_event_obj_data  - event object to delete
 *  
 *  @note   Evctx Engine should implement such function pointer and register to evctx
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK           - Success
 */
/*----------------------------------------------------------------------------*/
typedef INT32 (*x_evctx_delete_event_obj_fct) (
    VOID*               pv_event_obj_data);

/*----------------------------------------------------------------------------*/
/*! @brief  evctx get object information length function

 *  @param  [in] h_event                - handle of event object
 *  @param  [in] e_key_type             - pointer event key information
 *  @param  [in] pv_key_info            - event information
 *  @param  [out] pz_event_info_len     - pointer of evctx obj length
 *  @param  [out] pe_cond               - pointer of evctx condition
 *  
 *  @note   Evctx Engine should implement such function pointer and register to evctx
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK           - Success
 */
/*----------------------------------------------------------------------------*/
typedef INT32 (*x_evctx_get_event_obj_info_len_fct) (
    HANDLE_T            h_event,
    EVCTX_KEY_TYPE_T    e_key_type,
    VOID*               pv_key_info,
    SIZE_T*             pz_event_info_len,
    EVCTX_COND_T*       pe_cond);

/*----------------------------------------------------------------------------*/
/*! @brief  evctx get object information function

 *  @param  [in] h_event                - handle of event object
 *  @param  [in] e_key_type             - event type to query
 *  @param  [in] pv_key_info            - pointer event key information
 *  @param  [in] pz_event_info_len      - pointer of evctx obj length
 *  @param  [out] pz_event_info_len     - pointer of evctx obj length
 *  @param  [out] pv_event_info         - pointer of evctx obj information
 *  @param  [out] pe_cond               - pointer of evctx condition
 *  
 *  @note   Evctx Engine should implement such function pointer and register to evctx
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK           - Success
 */
/*----------------------------------------------------------------------------*/
typedef INT32 (*x_evctx_get_event_obj_info_fct) (
    HANDLE_T            h_event,
    EVCTX_KEY_TYPE_T    e_key_type,
    VOID*               pv_key_info,
    SIZE_T*             pz_event_info_len,
    VOID*               pv_event_info,
    EVCTX_COND_T*       pe_cond);

/*----------------------------------------------------------------------------*/
/*! @brief  evctx task function
 *  
 *  @note   Evctx Engine should implement such function pointer and register to evctx
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK           - Success
 */
/*----------------------------------------------------------------------------*/
typedef INT32 (*x_evctx_engine_task_fct) ( VOID );

/*----------------------------------------------------------------------------*/
/*! @brief  evctx set function

 *  @param  [in] pt_set_cmd         - pointer to the set command
 *  
 *  @note   Evctx Engine should implement such function pointer and register to evctx
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK           - Success
 */
/*----------------------------------------------------------------------------*/
typedef INT32 (*x_evctx_set_fct) (
    EVCTX_COMMAND_T*    pt_set_cmd);

/*----------------------------------------------------------------------------*/
/*! @brief  evctx get function

 *  @param  [in] pt_get_cmd         - pointer to the get command
 *  
 *  @note   Evctx Engine should implement such function pointer and register to evctx
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK           - Success
 */
/*----------------------------------------------------------------------------*/
typedef INT32 (*x_evctx_get_fct) (
    EVCTX_COMMAND_T*    pt_get_cmd);

/*----------------------------------------------------------------------------*/
/*! @brief  date time notify function

 *  @param  [in] h_event            - handle of event object
 *  @param  [in] pv_tag             - notify tag
 *  @param  [in] e_dt_cond          - DT condition
 *  @param  [in] t_delta            - delta time
 *  
 *  @note   Evctx Engine should implement such function pointer and register to evctx
 *  @see
 *  @return		VOID
 *  @retval		VOID
 */
/*----------------------------------------------------------------------------*/
typedef VOID (*x_evctx_dt_nfy_fct) (
    HANDLE_T            h_event,
    VOID*               pv_tag,
    DT_COND_T           e_dt_cond,
    TIME_T              t_delta);

/*------------------------------------------------------------------*/
/*! @struct EVCTX_ENG_FCT_TBL_T
 *  @brief  Structure for the EVCTX engine function table
 
 *  @code
 * typedef struct _EVCTX_ENG_FCT_TBL_T
 * {
 *     x_evctx_create_event_obj_fct        pf_create_event_obj;
 *     x_evctx_delete_event_obj_fct        pf_delete_event_obj;
 *     x_evctx_get_event_obj_info_len_fct  pf_get_event_obj_info_len;
 *     x_evctx_get_event_obj_info_fct      pf_get_event_obj_info;
 *     x_evctx_engine_task_fct             pf_engine_task;
 *     x_evctx_set_fct                     pf_set;
 *     x_evctx_get_fct                     pf_get;
 * } EVCTX_ENG_FCT_TBL_T;
 *  @endcode
 
 *  @li@c   pf_create_event_obj         - evctx create fucntion pointer
 *  @li@c   pf_delete_event_obj         - evctx delete fucntion pointer     
 *  @li@c   pf_get_event_obj_info_len   - evctx get info length fucntion pointer
 *  @li@c   pf_get_event_obj_info       - evctx get info fucntion pointer
 *  @li@c   pf_engine_task              - evctx task fucntion pointer
 *  @li@c   pf_set                      - evctx set fucntion pointer                  
 *  @li@c   pf_get                      - evctx get fucntion pointer                  
 */
/*------------------------------------------------------------------*/
/* Structure for the EVCTX engine function table*/
typedef struct _EVCTX_ENG_FCT_TBL_T
{
    x_evctx_create_event_obj_fct        pf_create_event_obj;
    x_evctx_delete_event_obj_fct        pf_delete_event_obj;
    x_evctx_get_event_obj_info_len_fct  pf_get_event_obj_info_len;
    x_evctx_get_event_obj_info_fct      pf_get_event_obj_info;
    x_evctx_engine_task_fct             pf_engine_task;
    x_evctx_set_fct                     pf_set;
    x_evctx_get_fct                     pf_get;
} EVCTX_ENG_FCT_TBL_T;

/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_EVCTX */
/*----------------------------------------------------------------------------*/


#endif /* _U_EVCTX_ENG_H_ */
