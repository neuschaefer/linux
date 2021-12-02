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
 * $RCSfile: x_rrctx_rrt_eng.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision:  $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: 2020cb37c9f4fb6d1974080187cb506c $
 *
 * Description:
 *         This header file contains exported definitions and delcarations of
 *         RRT engine of RRCTX.
 *---------------------------------------------------------------------------*/
 
#ifndef _X_RRCTX_RRT_ENG_H_
#define _X_RRCTX_RRT_ENG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_rrctx_rrt_eng.h"
 
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_RRCTX
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @brief	init the rrt engine of rrctx.
 *
 *	@note	
 *
 *	@see
 *	@return 	INT32
 *	@retval   RRCTXR_OK					- Success
 *			RRCTXR_DUP_BRDCST_TYPE	- There's already an engine registered with the same broadcast type.
 *			RRCTXR_INTERNAL_ERROR		- Internal error happened.		  
 *			RRCTXR_INV_ARG				- Invalid argument.
 *			RRCTXR_NOT_INIT 			- The RRCTX component is not initialized yet.
 */
/*----------------------------------------------------------------------------*/

INT32 x_rrctx_rrt_eng_init (VOID);

/*----------------------------------------------------------------------------*/
/*! @brief  get rrctx info length function

 *  @param  [in] h_rating                		- handle of rrctx object
 *  @param  [in] e_rrt_key_type             	- rrctx key type
 *  @param  [in] pv_rrt_key_param            - cntains the auxiliary data to get the requested rating info. 
 *
 *  @param  [out] pz_rating_info_len    	- pointer of rrctx information length
 *  
 *  @note   

 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

INT32 x_rrctx_rrt_get_info_len (
                    HANDLE_T                h_rating,
                    RRCTX_RRT_KEY_TYPE_T    e_rrt_key_type,
                    VOID*                   pv_rrt_key_param,
                    SIZE_T*                 pz_rating_info_len);

/*----------------------------------------------------------------------------*/
/*! @brief  get rrctx information  function

 *  @param  [in] h_rating                		- handle of rrctx object
 *  @param  [in] e_rrt_key_type             	- rrctx key type
 *  @param  [in] pv_rrt_key_param            - cntains the auxiliary data to get the requested rating info. 
 *
 *  @param  [out] pz_rating_info_len    	- pointer of rrctx information length
 *  @param  [out] pv_rating_info    		- pointer of rrctx information 
 *  
 *  @note   

 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

INT32 x_rrctx_rrt_get_info (
                    HANDLE_T                h_rating,
                    RRCTX_RRT_KEY_TYPE_T    e_rrt_key_type,
                    VOID*                   pv_rrt_key_param,
                    SIZE_T*                 pz_rating_info_len,
                    VOID*                   pv_rating_info);

/*----------------------------------------------------------------------------*/
/*! @brief  reload the TM root.

 *  @param  [in] ps_tuner_name                	- tuner name
 *  @param  [in] b_force_open             	- force to reload or not
 *
 *  
 *  @note   

 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

INT32 x_rrctx_rrt_reload_tm_root(
                    const CHAR*             ps_tuner_name,
                    BOOL                    b_force_open);

/*----------------------------------------------------------------------------*/
/*! @brief  reload the MGT table.
 *
 *  
 *  @note   

 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

INT32 x_rrctx_rrt_reload_mgt(VOID);

#endif /* _X_RRCTX_RRT_ENG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_RRCTX                            */
/*----------------------------------------------------------------------------*/


