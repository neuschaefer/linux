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
 * $RCSfile: x_evctx_eng.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/3 $
 * $SWAuthor: Justin Deng $
 * $MD5HEX: 1fd90ebb03749cab35e47b87d8576a94 $
 *
 * Description:
 *         This contains Event Context library macros, definitons, structures,
 *         and API's, which are shared with other modules.
 *---------------------------------------------------------------------------*/

#ifndef _X_EVCTX_ENG_H_
#define _X_EVCTX_ENG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_evctx_eng.h"

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_EVCTX
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @brief  register the engine to the evctx selecter
 *
 *  @param  [in] e_brdcst_type          			- broadcast type
 *  @param  [in] pt_eng_fct_tbl          			- pointer of engine function table
 *  @param  [in] ui2_max_num_event_objs        - the max number of event jobs
 *  
 *  @note   
 *
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_evctx_reg_engine (
                BRDCST_TYPE_T           e_brdcst_type,
                EVCTX_ENG_FCT_TBL_T*    pt_eng_fct_tbl,
                UINT16                  ui2_max_num_event_objs);


/*----------------------------------------------------------------------------*/
/*! @brief  lock the h_evctx_base_ctrl_list
 *
 *  @note   
 *
 *  @see
 */
/*----------------------------------------------------------------------------*/

extern VOID x_evctx_base_ctrl_list_lock (VOID);

/*----------------------------------------------------------------------------*/
/*! @brief  unlock the h_evctx_base_ctrl_list
 *
 *  @note   
 *
 *  @see
 */
/*----------------------------------------------------------------------------*/

extern VOID x_evctx_base_ctrl_list_unlock (VOID);

/*----------------------------------------------------------------------------*/
/*! @brief  active the engine task
 *
 *  @param  [in] e_brdcst_type          			- broadcast type
 *  
 *  @note   
 *
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_evctx_activate_eng_task (BRDCST_TYPE_T e_brdcst_type);

/*----------------------------------------------------------------------------*/
/*! @brief  get the date of engine
 *
 *  @param  [in] h_event          		- handle of evctx object
 *  @param  [out] ppv_eng_data         - date of engine
 *  
 *  @note   
 *
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_evctx_get_engine_data (
                HANDLE_T                h_event,
                VOID**                  ppv_eng_data);

/*----------------------------------------------------------------------------*/
/*! @brief  a notification function that will be called by the Evctx selector when the condition or 
 *		   statement of engine changed.
 *
 *  @param  [in] h_event          		- handle of evctx object
 *  @param  [in] e_evctx_cond         	- new condition of the evctx object.
 *  @param  [in] ui4_data            		- determine the value according to e_evctx_cond.
 *  @note   
 *
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   				- Success
 *          			EVCTXR_INTERNAL_ERROR   - Cannot send message to the selector.
 *          			EVCTXR_NOT_INIT         	- The EVCTX component is not initialized yet.
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_evctx_eng_cond_change_nfy (
                HANDLE_T                h_event,
                EVCTX_COND_T            e_evctx_cond,
                UINT32                  ui4_data);


/*----------------------------------------------------------------------------*/
/*! @brief  a notification function that will be called by the Evctx selector when the Date/Time library
 *               calls back.
 *
 *  @param  [in] h_event          	- handle of evctx object
 *  @param  [in] pf_dt_nfy         	- Notification function provided by the caller.  which called when the system clock
 *                 					   is synchronized with the current time value from STT, or TOT, or TDT.
 *  @param  [in] pv_tag            	- A tag value provided by the caller. 
 *  @note   
 *
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   				- Success
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_evctx_eng_reg_dt_nfy_fct (
                HANDLE_T                h_event,
                x_evctx_dt_nfy_fct      pf_dt_nfy,
                VOID*                   pv_tag);


/*----------------------------------------------------------------------------*/
/*! @brief   un-registers a notification function that will be called by the Evctx selector when the Date/Time library
 *               calls back.
 *
 *  @param  [in] h_event          	- handle of evctx object
 *  @note   
 *
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   				- Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_evctx_eng_unreg_dt_nfy_fct (HANDLE_T   h_event);


/*----------------------------------------------------------------------------*/
/*! @brief   unifies the specified country code to the same case.
 *
 *  @param  [in] pt_country_code          - ISO-3166 country code
 *  @note   
 *
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   				- Success
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_evctx_eng_unify_country_code (
                ISO_3166_COUNT_T*   pt_country_code);

#endif /* _X_EVCTX_ENG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_EVCTX */
/*----------------------------------------------------------------------------*/

