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
 * $RCSfile: x_rrctx_eng.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/1 $
 * $SWAuthor: Justin Deng $
 * $MD5HEX: 9969fe0123011f2e59aede7557663436 $
 *
 * Description:
 *         This contains RRCTX library macros, definitons, structures,
 *         and API's, which are shared with other modules.
 *---------------------------------------------------------------------------*/

#ifndef _X_RRCTX_ENG_H_
#define _X_RRCTX_ENG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_rrctx_eng.h"

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_RRCTX
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
/*! @brief  register the engine to the rrctx selecter
 *
 *  @param  [in] e_rating_type          			- rating type
 *  @param  [in] pt_eng_fct_tbl          			- pointer of engine function table
 *  
 *  @note   
 *
 *  @see
 *  @return		INT32
 *  @retval		RRCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_rrctx_reg_engine (
                RRCTX_RATING_TYPE_T     e_rating_type,
                RRCTX_ENG_FCT_TBL_T*    pt_eng_fct_tbl);

/*----------------------------------------------------------------------------*/
/*! @brief  lock the h_rrctx_base_ctrl_list
 *
 *  @note   
 *
 *  @see
 */
/*----------------------------------------------------------------------------*/

extern VOID x_rrctx_base_ctrl_list_lock (VOID);

/*----------------------------------------------------------------------------*/
/*! @brief  unlock the h_rrctx_base_ctrl_list
 *
 *  @note   
 *
 *  @see
 */
/*----------------------------------------------------------------------------*/

extern VOID x_rrctx_base_ctrl_list_unlock (VOID);

/*----------------------------------------------------------------------------*/
/*! @brief  active the engine task
 *
 *  @param  [in] e_rating_type          			- rating type
 *  
 *  @note   
 *
 *  @see
 *  @return		INT32
 *  @retval		RRCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_rrctx_activate_eng_task (RRCTX_RATING_TYPE_T e_rating_type);

/*----------------------------------------------------------------------------*/
/*! @brief  get the date of engine
 *
 *  @param  [in] h_rating          		- handle of rrctx object
 *  @param  [out] ppv_eng_data         - date of engine
 *  
 *  @note   
 *
 *  @see
 *  @return		INT32
 *  @retval		RRCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_rrctx_get_engine_data (
                HANDLE_T                h_rating,
                VOID**                  ppv_eng_data);

/*----------------------------------------------------------------------------*/
/*! @brief  a notification function that will be called by the Rrctx selector when the condition or 
 *		   statement of engine changed.
 *
 *  @param  [in] h_rating          		- handle of rrctx object
 *  @param  [in] ui4_reason         	- reason of the condition changed.
 *  @note   
 *
 *  @see
 *  @return		INT32
 *  @retval		RRCTXR_OK   				- Success
 *          			RRCTXR_INTERNAL_ERROR   	- Cannot send message to the selector.
 *          			RRCTXR_NOT_INIT         		- The RRCTX component is not initialized yet.
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_rrctx_eng_cond_change_nfy (
                HANDLE_T                h_rating,
                UINT32                  ui4_reason);

/*----------------------------------------------------------------------------*/
/*! @brief  a notification function that will be called by the Rrctx selector when the brdcast type changed.
 *
 *  @param  [in] e_rating_type          		- rating type
 *  @param  [in] ui4_reason         		- reason of the brdcast type changed.
 *  @note   
 *
 *  @see
 *  @return		INT32
 *  @retval		RRCTXR_OK   				- Success
 *          			RRCTXR_INTERNAL_ERROR   	- Cannot send message to the selector.
 *          			RRCTXR_NOT_INIT         		- The RRCTX component is not initialized yet.
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_rrctx_eng_cfg_change_brdct_nfy (
                RRCTX_RATING_TYPE_T     e_rating_type,
                UINT32                  ui4_reason);

#endif /* _X_RRCTX_ENG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_RRCTX                            */
/*----------------------------------------------------------------------------*/


