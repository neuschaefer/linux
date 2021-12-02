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
 * $RCSfile: x_rrctx.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/1 $
 * $SWAuthor: Justin Deng $
 * $MD5HEX: 494e45120de18cd8749a98c6245c6c57 $
 *
 * Description:
 *         This contains RRCTX library macros, definitons, structures,
 *         and API's, which are exported to middleware modules.
 *---------------------------------------------------------------------------*/

#ifndef _X_RRCTX_H_
#define _X_RRCTX_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_rrctx.h"

#define x_lt_mem_alloc(z_size)          x_mem_alloc(z_size)
#define x_lt_mem_free(pv_mem_block)     x_mem_free(pv_mem_block)

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
 /*------------------------------------------------------------------*/
/*! @fn			x_rrctx_open_rating (
 *                 			RRCTX_RATING_TYPE_T e_rating_type,
 *                 			VOID*               pv_param,
 *                  			x_rrctx_nfy_fct     pf_nfy,
 *                 			VOID*               pv_nfy_tag,
 *                   		HANDLE_T*           ph_rating)
 *
 *  @brief			API to load data of a specific rating based on the given rating type
 *
 *  @param [in]	e_rating_type		-Contains the rating type of the specific rating
 *				pv_param		-Contrain the parameters needed for opening
 *				pv_nfy_tag		-Contains the private tag value associated with the calling application
 *				pf_nfy			-Contains the callback function, which is called when the condition 
 *								  or settings of the requested rating is changed
 *								  
 *  @param [out]	ph_rating			-Contains the handle to the RRCTX object
 *
 *  @note			This function need be called when user want to get the handle of the rating engine
 *  @see			x_rrctx.c
 
 *  @return		Return the error code
 
 *  @retval		RRCTXR_OK               			The routine was successful.
 *          			RRCTXR_ENG_NOT_FOUND    	The RRCTX engine was not found.
 *         			RRCTXR_INV_ARG          		Invalid arguments.
 *          			RRCTXR_NOT_INIT         		The RRCTX component is not inited.
 *          			RRCTXR_OUT_OF_HANDLE    	No free handle is available.
 *          			RRCTXR_OUT_OF_MEM       		Run out of memory.
 */
/*------------------------------------------------------------------*/
extern INT32 x_rrctx_open_rating (
                    RRCTX_RATING_TYPE_T e_rating_type,
                    VOID*               pv_param,
                    x_rrctx_nfy_fct     pf_nfy,
                    VOID*               pv_nfy_tag,
                    HANDLE_T*           ph_rating);

/*------------------------------------------------------------------*/
/*! @fn			x_rrctx_close_rating (HANDLE_T            h_rating)
 *
 *  @brief			API to delete a loaded rating object.
 *
 *  @param [in]	h_rating		-Contains the rating handle to be deleted or returned
 *                           			  or next use
 *								  
 *  @param [out]	-
 *
 *  @note			
 *  @see			x_rrctx.c
 *
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 *				RRCTXR_INV_HANDLE		 	Invalid handle value.
 * 		 		RRCTXR_NOT_INIT		 		The RRCTX component is not inited.
 */
/*------------------------------------------------------------------*/

extern INT32 x_rrctx_close_rating (
                    HANDLE_T            h_rating);

/*------------------------------------------------------------------*/
/*! @fn		INT32 x_rrctx_get_rating_info_len (
 *                  		HANDLE_T            h_rating,
 *                  		RRCTX_KEY_TYPE_T    e_key_type,
 *                   	VOID*               pv_key_param,
 *                   	SIZE_T*             pz_rating_info_len)
 *
 *  @brief		API to get the length of the requested rating information
 *
 *  @param [in]	h_rating			-Contains the rating handle to be queried.
 *				e_key_type		-Contains the specific information type to be retrieved.
 *				pv_key_param	-Contains the auxiliary data to get the requested rating information. 
 *								  Ths argument may be NULL, depending on the value of the
 *                          				  argument e_key_type.
 *								  
 *  @param [out]	pz_rating_info_len			-Contains the length of the requested rating information 
 *										  if the routine was successfully returned.
 *
 *  @note			
 *  @see			x_rrctx.c
 *
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 *          			RRCTXR_INV_ARG          		Invalid arguments.
 *         	 		RRCTXR_INV_HANDLE       		Invalid handle value.
 *          			RRCTXR_INFO_NOT_FOUND   	The requested rating information is not present.
 *          			RRCTXR_NOT_INIT         		The RRCTX component is not inited.
 */
/*------------------------------------------------------------------*/

extern INT32 x_rrctx_get_rating_info_len (
                    HANDLE_T            h_rating,
                    RRCTX_KEY_TYPE_T    e_key_type,
                    VOID*               pv_key_param,
                    SIZE_T*             pz_rating_info_len);

/*------------------------------------------------------------------*/
/*! @fn		INT32 x_rrctx_get_rating_info (
 *                  		HANDLE_T            h_rating,
 *                  		RRCTX_KEY_TYPE_T    e_key_type,
 *                   	VOID*               pv_key_param,
 *                   	SIZE_T*             pz_rating_info_len,
 * 				VOID*               pv_rating_info)
 *
 *  @brief		API to get the requested rating information
 *
 *  @param [in]	h_rating			-Contains the rating handle to be queried.
 *				e_key_type		-Contains the specific information type to be retrieved.
 *				pv_key_param	-Contains the auxiliary data to get the requested rating information. 
 *								  Ths argument may be NULL, depending on the value of the
 *                          				  argument e_key_type.
 *				pz_rating_info_len	-Contains the length of the buffer pv_rating_info.
 *								  
 *  @param [out]	pz_rating_info_len			-Contains the length of the requested rating information 
 *										  if the routine was successfully returned.
 *				pv_rating_info				-Contains the requested information if the routine was
 *                          						  successfully returned.
 *
 *  @note			
 *  @see			x_rrctx.c
 *
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 *          			RRCTXR_INV_ARG          		Invalid arguments.
 *         	 		RRCTXR_INV_HANDLE       		Invalid handle value.
 *          			RRCTXR_INFO_NOT_FOUND   	The requested rating information is not present.
 *          			RRCTXR_NOT_INIT         		The RRCTX component is not inited.
 */
/*------------------------------------------------------------------*/

extern INT32 x_rrctx_get_rating_info (
                    HANDLE_T            h_rating,
                    RRCTX_KEY_TYPE_T    e_key_type,
                    VOID*               pv_key_param,
                    SIZE_T*             pz_rating_info_len,
                    VOID*               pv_rating_info);

/*------------------------------------------------------------------*/
/*
 *  @brief		API to check the initialization stauts of the rrctx.
 *
 *
 *  @note			
 *  @see			x_rrctx.c
 *
 *  @return		BOOL
 *
 *  @retval		TRUE               			rrctx is initialized.
 *          			FALSE		          		rrctx is not initialized.
 */
/*------------------------------------------------------------------*/

extern BOOL x_rrctx_is_inited (VOID);


/* Wrapper Functions */
/*------------------------------------------------------------------*/
/*! @fn		INT32 x_rrctx_rating_settings(
 *                   		HANDLE_T            h_rating,
 *                   		VOID*               pv_setting)
 *
 *  @brief		API to set up the settings of the RRCTX engine 
 *
 *  @param [in]	h_rating			-Contains the handle of RRCTX object to be set up.
 *				pv_settings		-Contains the setting values. 
 *								  
 *  @param [out]	-
 *
 *  @note			
 *  @see			c_rrctx.c
 *
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 *          			RRCTXR_INV_ARG          		Invalid arguments.
 *				RRCTXR_INTL_ERROR		 	Invalid rating handle. 
 *          			RRCTXR_NOT_INIT         		The RRCTX component is not inited.
 */
/*------------------------------------------------------------------*/

extern INT32 x_rrctx_rating_settings(
                    HANDLE_T            h_rating,
                    VOID*               pv_setting);

/*------------------------------------------------------------------*/
/*! @fn		INT32 x_rrctx_rating_enable(
 *                    		HANDLE_T            h_rating,
 *                   		BOOL                b_enable);
 *
 *  @brief		API to enable or disable the RRCTX engine 
 *
 *  @param [in]	h_rating			-Contains the handle of RRCTX object to be set up.
 *				b_enable			-Contains the enable/disable value. 
 *								  
 *  @param [out]	-
 *
 *  @note			
 *  @see			c_rrctx.c
 *
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 *          			RRCTXR_INV_ARG          		Invalid arguments.
 *				RRCTXR_INTL_ERROR		 	Invalid rating handle. 
 *          			RRCTXR_NOT_INIT         		The RRCTX component is not inited.
 */
/*------------------------------------------------------------------*/

extern INT32 x_rrctx_rating_enable(
                    HANDLE_T            h_rating,
                    BOOL                b_enable);

/*------------------------------------------------------------------*/
/*! @fn		INT32 x_rrctx_rating_exceeds(
 *                   		HANDLE_T            h_rating,
 *                   		VOID*               pt_rating_lst,  
 *                   		BOOL*               pb_exceed)
 *
 *  @brief		API to return if the given rating exceeds the current parental setttings or not.
 *
 *  @param [in]	h_rating			-Contains the handle of RRCTX object to be set up.
 *				pt_rating_lst		-Contains the specific structure of rating list for the queried engine. 
 *								  
 *  @param [out]	pb_enable		-return if the given rating exceeds the current parental setttings or not.  
 *
 *  @note			
 *  @see			c_rrctx.c
 *
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 *          			RRCTXR_INV_ARG          		Invalid arguments.
 *				RRCTXR_INTL_ERROR		 	Invalid rating handle. 
 *          			RRCTXR_NOT_INIT         		The RRCTX component is not inited.
 */
/*------------------------------------------------------------------*/

extern INT32 x_rrctx_rating_exceeds(
                    HANDLE_T            h_rating,
                    VOID*               pt_rating_lst,  /* V-Chip: EVCTX_ATSC_RATING_LIST_T */
                    BOOL*               pb_exceed);

/*------------------------------------------------------------------*/
/*! @fn		INT32 x_rrctx_rating_get_text(
 *                  		HANDLE_T            h_rating,
 *                  		VOID*               pt_rating_lst, 
 *                  		UTF16_T*            w2s_str,
 *                   		SIZE_T              z_len)
 *
 *  @brief		API to get the text of the given rating.
 *
 *  @param [in]	h_rating			-Contains the handle of RRCTX object to be set up.
 *				pt_rating_lst		-Contains the specific structure of rating list for the queried engine. 
 *				z_len			-The length of w2s_str 
 *								  
 *  @param [out]	w2s_str			-Return the text of the given rating.
 *
 *  @note			
 *  @see			c_rrctx.c
 *
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 *          			RRCTXR_INV_ARG          		Invalid arguments.
 *				RRCTXR_INTL_ERROR		 	Invalid rating handle. 
 *          			RRCTXR_NOT_INIT         		The RRCTX component is not inited.
 */
/*------------------------------------------------------------------*/

extern INT32 x_rrctx_rating_get_text(
                    HANDLE_T            h_rating,
                    VOID*               pt_rating_lst,  /* V-Chip: EVCTX_ATSC_RATING_LIST_T */
                    UTF16_T*            w2s_str,
                    SIZE_T              z_len);
/*------------------------------------------------------------------*/
/*! 
 *  @brief      API to check if rating is availale.
 *
 *  @param [in]	h_rating		-Contains the handle of RRCTX object to be set up.
 *								  
 *  @param [out]   	pb_available	-Return if the rating is available
 *
 *  @note
 *  @see            	c_rrctx.c
 *
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK                   	The routine was successful.
 *              		RRCTXR_INV_ARG          	Invalid arguments.
 *              		RRCTXR_INTL_ERROR          Invalid rating handle. 
 *              		RRCTXR_NOT_INIT             	The RRCTX component is not inited.
 */
/*------------------------------------------------------------------*/

extern INT32 x_rrctx_rating_available(
                    HANDLE_T            h_rating,
                    BOOL*               pb_available);

/*------------------------------------------------------------------*/
/*! @fn		INT32 x_rrctx_rating_valid(
 *                    		HANDLE_T            h_rating,
 *                   		VOID*               pt_rating_lst, 
 *                   		BOOL*               pb_valid)
 *
 *  @brief		API to get the type of the given rating.
 *
 *  @param [in]	h_rating			-Contains the handle of RRCTX object to be set up.
 *				pt_rating_lst		-Contains the specific structure of rating list for the queried engine. 
 *								  
 *  @param [out]	pb_valid			-Return if the given rating is valid.
 *
 *  @note			
 *  @see			c_rrctx.c
 *
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 *          			RRCTXR_INV_ARG          		Invalid arguments.
 *				RRCTXR_INTL_ERROR		 	Invalid rating handle. 
 *          			RRCTXR_NOT_INIT         		The RRCTX component is not inited.
 */
/*------------------------------------------------------------------*/

extern INT32 x_rrctx_rating_valid(
                    HANDLE_T            h_rating,
                    VOID*               pt_rating_lst, /* EVCTX_ATSC_RATING_LIST_T */
                    BOOL*               pb_valid);

/*------------------------------------------------------------------*/
/*! @fn		INT32 x_rrctx_rating_get_type(
 *                   		HANDLE_T            	h_rating,
 *                   		VOID*               	pt_rating_lst, 
 *                   		RRCTX_VCHIP_TYPE_T* pt_rating_type);
 *
 *  @brief		API to get the type of the given rating.
 *
 *  @param [in]	h_rating			-Contains the handle of RRCTX object to be set up.
 *				pt_rating_lst		-Contains the specific structure of rating list for the queried engine. 
 *								  
 *  @param [out]	pt_rating_type		-Return the type of the given rating.
 *
 *  @note			
 *  @see			c_rrctx.c
 *
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 *          			RRCTXR_INV_ARG          		Invalid arguments.
 *				RRCTXR_INTL_ERROR		 	Invalid rating handle. 
 *          			RRCTXR_NOT_INIT         		The RRCTX component is not inited.
 */
/*------------------------------------------------------------------*/

extern INT32 x_rrctx_rating_get_type(
                    HANDLE_T            h_rating,
                    VOID*               pt_rating_lst,  /* V-Chip: EVCTX_ATSC_RATING_LIST_T */
                    RRCTX_VCHIP_TYPE_T* pt_rating_type);

/* Store or Clear Rating Data */
/*------------------------------------------------------------------*/
/*! @fn		INT32 x_rrctx_rating_storage(
 *                  	 		HANDLE_T            	h_rating,
 *                   		BOOL                	b_store)
 *
 *  @brief		API to store or clear the data of the given rating.
 *
 *  @param [in]	h_rating		-Contains the handle of RRCTX object .
 				b_store		-Contains to store or not
 *								  
 *  @param [out]	-
 *
 *  @note			Store or Clear Rating Data
 *  @see			c_rrctx.c
 *
 *  @return		Return the error code
 *
 *  @retval		RRCTXR_OK               			The routine was successful.
 *          			RRCTXR_INV_ARG          		Invalid arguments.
 *				RRCTXR_INTL_ERROR		 	Invalid rating handle. 
 *          			RRCTXR_NOT_INIT         		The RRCTX component is not inited.
 */
/*------------------------------------------------------------------*/

extern INT32 x_rrctx_rating_storage(
                    HANDLE_T            h_rating,
                    BOOL                b_store);


#endif /* _X_RRCTX_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_RRCTX                            */
/*----------------------------------------------------------------------------*/

