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
 * $RCSfile: x_rrctx_db.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains RRCTX DB specific definitions, which are 
 *         exported.
 *          
 *---------------------------------------------------------------------------*/

#ifndef _X_RRCTX_DB_H_
#define _X_RRCTX_DB_H_

/*-------------------------------------------------------------------------
                    include files
-------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_sys_name.h"
#include "x_fm.h"
#include "u_cdb.h"

#include "u_rrctx.h"

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_RRCTX
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ------------------------------------------------------------------------*/

#define RRCTX_DB_NULL_VER_ID           (CDB_NULL_VER_ID)
#define RRCTX_DB_NULL_INDEX            ((UINT8) 0xff)

/* MAX length of the string of RRCTX_DB_DATA_TEXT_FIELD_T */
#define RRCTX_DB_REC_TEXT_LEN           32

/* MAX length of the string of RRCTX_DB_DATA_TEXT_EX_FIELD_T */
#define RRCTX_DB_REC_TEXT_EX_LEN        64

#define RRCTX_DB_DIM_FLAG_NULL          (0x00)
#define RRCTX_DB_DIM_FLAG_INVALID       (0x01 << 0)

/*-------------------------------------------------------------------------
                    functions
 ------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @brief	initialization the rrctx database
 *
 *	@param  [in] pt_rrctx_thread_descr			-pt_rrctx_thread_descr References 
 *	@note	
 *
 *	@see
 *	@return		INT32
 *	@retval		RRCTXR_OK	- Success
 */
/*----------------------------------------------------------------------------*/

extern INT32  x_rrctx_db_init(
        UINT16                  ui2_num_rec_of_engine);
        
/* Get Data */ 
/*------------------------------------------------------------------*/
/*!@brief		get the number of region in the database 
 *
 *  @param [in]	e_rating_type			-rating type
 *								  
 *  @param [out]	pui1_num_of_region	- number of region
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/

extern INT32 x_rrctx_db_get_num_of_region(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8*                  pui1_num_of_region);

/*------------------------------------------------------------------*/
/*!@brief		get the number of dimensions in the database 
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_region_idx			-region index
 *								  
 *  @param [out]	pui1_num_of_dim			- number of total dimension
 *  @param [out]	pui1_num_of_valid_dim		- number of valid dimension
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/

extern INT32 x_rrctx_db_get_num_of_dim(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_region_idx,
        UINT8*                  pui1_num_of_dim,
        UINT8*                  pui1_num_of_valid_dim);

/*------------------------------------------------------------------*/
/*!@brief		get the number of valid dimensions in the database 
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_region_idx			-region index
 *								  
 *  @param [out]	pui2_dim_idx				- pointer to the array that to store the dimsion index
 *  @param [out]	pui1_num_of_valid_dim		- number of valid dimension
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/

extern INT32 x_rrctx_db_get_valid_dim(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        UINT16*                 pui2_dim_idx,
        UINT8*                  pui1_valid_dim_num);

/*------------------------------------------------------------------*/
/*!@brief		gets the number of levels of the required dimension 
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *  @param [in]	pui2_dim_idx				-dimension index
 *								  
 *  @param [out]	pui1_num_of_lvl			-number of levels
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/

extern INT32 x_rrctx_db_get_num_of_lvl(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        UINT8                   ui1_dim_idx,
        UINT8*                  pui1_num_of_lvl);

/*------------------------------------------------------------------*/
/*!@brief		gets the text of existing regions
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *								  
 *  @param [out]	ps_text					-pointer to variable to hold text 
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/        
extern INT32 x_rrctx_db_get_text_of_region(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        CHAR*                   ps_text);

/*------------------------------------------------------------------*/
/*!@brief		gets the text of existing dimensions
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *  @param [in]	ui1_dim_idx				-dimension index
 *								  
 *  @param [out]	ps_text					-pointer to variable to hold text 
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/   

extern INT32 x_rrctx_db_get_text_of_dim(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        UINT8                   ui1_dim_idx,
        CHAR*                   ps_text);

/*------------------------------------------------------------------*/
/*!@brief		gets the abbr text of levels
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *  @param [in]	ui1_dim_idx				-dimension index
 *  @param [in]	ui1_lvl_idx				-level index
 *								  
 *  @param [out]	ps_abbr					-pointer to variable to hold text 
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/   

extern INT32 x_rrctx_db_get_abbr_of_lvl(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        UINT8                   ui1_dim_idx,
        UINT8                   ui1_lvl_idx,
        CHAR*                   ps_abbr);

/*------------------------------------------------------------------*/
/*!@brief		gets the text of levels
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *  @param [in]	ui1_dim_idx				-dimension index
 *  @param [in]	ui1_lvl_idx				-level index
 *								  
 *  @param [out]	ps_text					-pointer to variable to hold text 
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 
extern INT32 x_rrctx_db_get_text_of_lvl(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        UINT8                   ui1_dim_idx,
        UINT8                   ui1_lvl_idx,
        CHAR*                   ps_text);

/*------------------------------------------------------------------*/
/*!@brief		gets the version id of the existing regions
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *	
 *  @param [out]	pui1_id_of_region			-pointer to variable to hold region index
 *  @param [out]	pui1_version				-pointer to variable to hold version
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_get_id_version_of_region(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        UINT8*                  pui1_id_of_region,
        UINT8*                  pui1_version);

/*------------------------------------------------------------------*/
/*!@brief		gets the index of existing region record
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *  @param [in]	ui1_max_num_of_region	-max number of region records
 *	
 *  @param [out]	pui1_idx_of_region			-pointer to variable to hold region index
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_get_idx_by_rgn_id(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_id,
        UINT8                   ui1_max_num_of_region,
        UINT8*                  pui1_idx_of_region);

/*------------------------------------------------------------------*/
/*!@brief		gets the graduated scale of the required dimension
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *  @param [in]	ui1_dim_idx				-dimension index
 *	
 *  @param [out]	pb_scale					-exist graduated scale or not
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_get_scale_of_dim(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        UINT8                   ui1_dim_idx,
        BOOL*                   pb_scale);


/*------------------------------------------------------------------*/
/*!@brief		gets the flag of the required dimension
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *  @param [in]	ui1_dim_idx				-dimension index
 *	
 *  @param [out]	pui1_flag					-pointer to variable to hold flag
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_get_flag_of_dim(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        UINT8                   ui1_dim_idx,
        UINT8*                  pui1_flag);

/* Update Text Data */
/*------------------------------------------------------------------*/
/*!@brief		updates the text of the records (region/dimension/level)
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *  @param [in]	ui1_dim_idx				-dimension index
 *  @param [in]	ui1_lvl_idx				-level index
 *
 *  @param [out]	ps_text					-pointer to variable to hold text 
 *  @param [out]	ps_text_ex				-pointer to variable to hold text 
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_udpate_text_of_rec(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        UINT8                   ui1_dim_idx,
        UINT8                   ui1_lvl_idx,
        CHAR*                   ps_text,
        CHAR*                   ps_text_ex);

/* Rating Settings */
/*------------------------------------------------------------------*/
/*!@brief		sets the rating level 
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_bu					-the value of the block unrate 
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_set_bu_setting(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_bu);

/*------------------------------------------------------------------*/
/*!@brief		gets the rating level 
 *
 *  @param [in]	e_rating_type				-rating type 
 *
 *  @param [out]	ui1_bu					-pointer to variable to hold value of the block unrate 
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_get_bu_setting(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8*                  pui1_bu);

/*------------------------------------------------------------------*/
/*!@brief		sets the rating level of the required dimension
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *  @param [in]	ui1_dim_idx				-dimension index
 *  @param [in]	pb_lvl_block				-block flags of the rating levels 
 *  @param [in]	ui1_max_lvl_num			-max number of levels
 *
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_set_rating_setting(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        UINT8                   ui1_dim_idx,
        BOOL*                   pb_lvl_block,
        UINT8                   ui1_max_lvl_num);

/*------------------------------------------------------------------*/
/*!@brief		gets the rating level of the required dimension
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *  @param [in]	ui1_dim_idx				-dimension index
 *  @param [in]	ui1_max_lvl_num			-max number of levels
 *
 *
 *  @param [out]	pb_lvl_block				-block flags of the rating levels 
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_get_rating_setting(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        UINT8                   ui1_dim_idx,
        BOOL*                   pb_lvl_block,
        UINT8                   ui1_max_lvl_num);

/* Information of RRCTX DB */
/*------------------------------------------------------------------*/
/*!@brief		gets the number of records in the rrctx database
 *
 *  @param [out]	pui2_num_recs			-pointer to memory to receive number of records
 *  @param [out]	pui4_version				-pointer to memory to receive version of the rrctx database.
 *
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_get_num_recs(
        UINT16*                 pui2_num_recs,
        UINT32*                 pui4_version);
        
/* Lock/unlock RRCTX DB */
/*------------------------------------------------------------------*/
/*!@brief		locks the rrctx database
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_lock(VOID);

/*------------------------------------------------------------------*/
/*!@brief		unlocks the rrctx database
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_unlock(VOID);


/* Add record to RRCTX DB */
/*------------------------------------------------------------------*/
/*!@brief		adds one rating record into the rrctx database
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_num				-number of regions
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_add_rating_rec(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_num);

/*------------------------------------------------------------------*/
/*!@brief		adds one rating region record into the rrctx database
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *  @param [in]	ui1_rgn_num				-number of regions
 *  @param [in]	ui1_num_invalid_of_dim		-number of  invalid dimensions 
 *  @param [in]	ui1_id_of_rgn				-region index
 *  @param [in]	ps_rgn_text				-region text
 *
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_add_rgn_rec(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        UINT8                   ui1_num_of_dim,
        UINT8                   ui1_num_invalid_of_dim,
        UINT8                   ui1_id_of_rgn,
        UINT8                   ui1_version,
        CHAR*                   ps_rgn_text);

/*------------------------------------------------------------------*/
/*!@brief		adds one rating dimension record into the rrctx database
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *  @param [in]	ui1_dim_idx				-dimension index
 *  @param [in]	ui1_num_of_lvl			-number of  levels 
 *  @param [in]	ui1_dim_flag				-dimension flag
 *  @param [in]	b_scale					-whether or not the specific dimension is graduated scale
 *  @param [in]	ps_dim_text				-dimension text
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_add_dim_rec(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        UINT8                   ui1_dim_idx, 
        UINT8                   ui1_num_of_lvl,       
        UINT8                   ui1_dim_flag,       
        BOOL                    b_scale,
        CHAR*                   ps_dim_text);

/*------------------------------------------------------------------*/
/*!@brief		adds one rating level record into the rrctx database
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *  @param [in]	ui1_dim_idx				-dimension index
 *  @param [in]	ui1_lvl_idx				-level index
 *  @param [in]	ps_lvl_abbr				-level abbr text
 *  @param [in]	ps_lvl_text				-level text
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_add_lvl_rec(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        UINT8                   ui1_dim_idx,
        UINT8                   ui1_lvl_idx,         
        CHAR*                   ps_lvl_abbr,
        CHAR*                   ps_lvl_text);

/* Delete record in RRCTX DB */
/*------------------------------------------------------------------*/
/*!@brief		delete the record dependent on the input parameters.
 *
 *  @param [in]	e_rating_type				-rating type 
 *  @param [in]	ui1_rgn_idx				-region index
 *  @param [in]	ui1_dim_idx				-dimension index
 *  @param [in]	ui1_lvl_idx				-level index
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_del_rec(
        RRCTX_RATING_TYPE_T     e_rating_type,
        UINT8                   ui1_rgn_idx,
        UINT8                   ui1_dim_idx,
        UINT8                   ui1_lvl_idx);

/*------------------------------------------------------------------*/
/*!@brief		delete the all records of the specific region in rrctx database
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_del_all_rec(VOID);

/*------------------------------------------------------------------*/
/*!@brief		print the field data of all records
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 


extern INT32 x_rrctx_db_print_all_rec(VOID);

/*------------------------------------------------------------------*/
/*!@brief		stores the specified engine records to a raw device.
 *
 *  @note			
 *  @see			
 *
 *  @return		INT32
 *
 *  @retval		RRCTXR_OK               		- The routine was successful. 
 *    				RRCTXR_INV_ARG 		- pui1_num_of_dim is NULL
 */
/*------------------------------------------------------------------*/ 

extern INT32 x_rrctx_db_raw_store_rec(VOID);

#endif

/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_RRCTX                            */
/*----------------------------------------------------------------------------*/


