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
/*----------------------------------------------------------------------------*/
/*! @file u_rrctx_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This contains Rating Region Context library macros, definitons, 
 *         structures, and API's, which are shared with other modules, and 
 *         customized engines.
 *          
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_RRCTX_ENG_H_
#define _U_RRCTX_ENG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_rrctx.h"

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_RRCTX
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @fn			INT32 (*x_rrctx_open_rating_obj_fct) (
 *   					HANDLE_T            	h_rating,
 *   					VOID*               	pv_param,
 *   					VOID**              	ppv_rating_obj_data)
 *
 *  @brief			Function type to load data of a specific rating based on the given rating type
 *
 *  @param [in]	h_rating            -Contains the handle of RRCTX object.
 *          			pv_param         -Contains tuner name used for opening RRT.
 *								  
 *  @param [out]	ppv_rating_obj_data		-NULL. In design of RRT engine, no object data.
 *
 *  @note			
 *  @see
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 *         			RRCTXR_INV_ARG          		Invalid arguments.
 */
/*------------------------------------------------------------------*/

typedef INT32 (*x_rrctx_open_rating_obj_fct) (
    HANDLE_T            h_rating,
    VOID*               pv_param,
    VOID**              ppv_rating_obj_data);

/*------------------------------------------------------------------*/
/*! @fn			INT32 (*x_rrctx_close_rating_obj_fct) (VOID*               pv_rating_obj_data)
 *
 *  @brief			Function type to delete a loaded rating object.
 *
 *  @param [in]	pv_rating_obj_data   	-NULL. In design of RRT engine, no object data.
 *								  
 *  @param [out]	-
 *
 *  @note			
 *  @see
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 */
/*------------------------------------------------------------------*/

typedef INT32 (*x_rrctx_close_rating_obj_fct) (
    VOID*               pv_rating_obj_data);


/*------------------------------------------------------------------*/
/*! @fn			INT32 (*x_rrctx_get_rating_obj_info_len_fct) (
 *   					HANDLE_T            		h_rating,
 *   					RRCTX_KEY_TYPE_T    	e_key_type,
 *   					VOID*               		pv_key_param,
 *   					SIZE_T*             		pz_rating_info_len)
 *
 *  @brief			Function type to get the length of the requested rating information
 *
 *  @param [in]	h_rating            	-Contains the handle of RRCTX object.
 *          			e_key_type	       -Contains specific information type to be retrieved.
 *				pv_key_param	-Contains the auxiliary data to get the requested rating info. 
 *								  This argument could be NULL, depending on the value of the
 *								  argument e_key_type.
 *								  
 *  @param [out]	pz_rating_info_len		-Contains the length of the requested raring information
 *									  if the routine was successfully returned.
 *
 *  @note			
 *  @see
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 *         			RRCTXR_INV_ARG          		Invalid arguments.
 *	`			RRCTXR_INFO_NOT_FOUND         The requested event information is not present.
 */
/*------------------------------------------------------------------*/

typedef INT32 (*x_rrctx_get_rating_obj_info_len_fct) (
    HANDLE_T            h_rating,
    RRCTX_KEY_TYPE_T    e_key_type,
    VOID*               pv_key_param,
    SIZE_T*             pz_rating_info_len);


/*------------------------------------------------------------------*/
/*! @fn			INT32 (*x_rrctx_get_rating_obj_info_fct) (
 *   					HANDLE_T            		h_rating,
 *   					RRCTX_KEY_TYPE_T    	e_key_type,
 *   					VOID*               		pv_key_param,
 *   					SIZE_T*             		pz_rating_info_len,
 *					VOID*				 pv_rating_info)
 *
 *  @brief			Function type to get the requested rating information
 *
 *  @param [in]	h_rating            		-Contains the handle of RRCTX object.
 *          			e_key_type	       	-Contains specific information type to be retrieved.
 *				pv_key_param		-Contains the auxiliary data to get the requested rating info. 
 *									  This argument could be NULL, depending on the value of the
 *								  	  argument e_key_type.								  
 *  				pz_rating_info_len		-Contains the length of the requested raring information
 *									  if the routine was successfully returned.
 *  @param [out]	pv_rating_info      		-Contains the requested rating information if the routine 
 *									  was successfully returned.
 *
 *  @note			
 *  @see
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 *         			RRCTXR_INV_ARG          		Invalid arguments.
 *	`			RRCTXR_INFO_NOT_FOUND         The requested event information is not present.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_rrctx_get_rating_obj_info_fct) (
    HANDLE_T            h_rating,
    RRCTX_KEY_TYPE_T    e_key_type,
    VOID*               pv_key_param,
    SIZE_T*             pz_rating_info_len,
    VOID*               pv_rating_info);


/*------------------------------------------------------------------*/
/*! @fn			INT32 (*x_rrctx_storage_rating_obj_fct) (
 *  					HANDLE_T            	h_rating,
 *   					BOOL                	b_store)
 *
 *  @brief			Function type to store or clear the data of the given rating.
 *
 *  @param [in]	h_rating		-Contains the handle of RRCTX object.
 *				b_store		-Contains to store or not
 *								  
 *  @param [out]	-
 *
 *  @note			
 *  @see
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 *          			RRCTXR_INV_ARG          		Invalid arguments.
 *				RRCTXR_INTL_ERROR		 	Invalid rating handle. 
 *          			RRCTXR_NOT_INIT         		The RRCTX component is not inited.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_rrctx_storage_rating_obj_fct) (
    HANDLE_T            h_rating,
    BOOL                b_store);


/*------------------------------------------------------------------*/
/*! @fn			INT32 (*x_rrctx_engine_task_fct) ( VOID )
 *
 *  @brief			Function type of the task of the rating engine.
 *
 *  @param [in]	-
 *								  
 *  @param [out]	-
 *
 *  @note			
 *  @see
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 *				RRCTXR_INTL_ERROR		 	Invalid rating handle. 
 */
/*------------------------------------------------------------------*/

typedef INT32 (*x_rrctx_engine_task_fct) ( VOID );


/* Structure for the RRCTX engine function table*/
/*------------------------------------------------------------------*/
/*! @struct	RRCTX_ENG_FCT_TBL_T
 *  @brief		Function table of the rating engine
 *
 *  @code
 *  	typedef struct _RRCTX_ENG_FCT_TBL_T
 *	{
 *	    x_rrctx_open_rating_obj_fct          	pf_open_rating_obj;
 *	    x_rrctx_close_rating_obj_fct         	pf_close_rating_obj;
 *	    x_rrctx_get_rating_obj_info_len_fct  pf_get_rating_obj_info_len;
 *	    x_rrctx_get_rating_obj_info_fct      	pf_get_rating_obj_info;
 *	    x_rrctx_storage_rating_obj_fct       	pf_storage_rating_obj;
 *	    x_rrctx_engine_task_fct              	pf_engine_task;
 *	} RRCTX_ENG_FCT_TBL_T;
 *  @endcode
 *
 *  @li@c	pf_open_rating_obj				-Function type of openning the rating object
 *  @li@c	pf_close_rating_obj			-Function type of closing the rating object
 *  @li@c	pf_get_rating_obj_info_len		-Function type of getting the length of rating information
 *  @li@c	pf_get_rating_obj_info			-Function type of getting the rating information
 *  @li@c	pf_storage_rating_obj			-Function type of storing or cleaning the rating data.
 *  @li@c	pf_engine_task				-Function type of the task of the rating engine
 */
/*------------------------------------------------------------------*/
typedef struct _RRCTX_ENG_FCT_TBL_T
{
    x_rrctx_open_rating_obj_fct          pf_open_rating_obj;
    x_rrctx_close_rating_obj_fct         pf_close_rating_obj;
    x_rrctx_get_rating_obj_info_len_fct  pf_get_rating_obj_info_len;
    x_rrctx_get_rating_obj_info_fct      pf_get_rating_obj_info;
    x_rrctx_storage_rating_obj_fct       pf_storage_rating_obj;
    x_rrctx_engine_task_fct              pf_engine_task;
} RRCTX_ENG_FCT_TBL_T;


#endif /* _U_RRCTX_ENG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_RRCTX_ENG                    */
/*----------------------------------------------------------------------------*/

