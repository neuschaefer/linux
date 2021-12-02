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
/*! @file u_evctx_atsc_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains exported definitions and delcarations of
 *         Event Context ATSC engine.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef _U_EVCTX_ATSC_ENG_H_
#define _U_EVCTX_ATSC_ENG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
 
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_EVCTX_ATSC_ENG EVCTX ATSC Engine
 *
 *  @ingroup groupMW_EVCTX
 *  @brief The EVCTX ATSC engine module is used to get EVCTX ATSC information.
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could get event infromation
 *   from the exported API of EVCTX ATSC engine.
 *
 *  @see groupMW_EVCTX_ATSC_ENG
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @enum	EVCTX_ATSC_KEY_TYPE_T
 *  @brief		Key type of specific infomation
 *
 *  @code
 *	typedef enum _EVCTX_ATSC_KEY_TYPE_T
 *	{
 *		EVCTX_ATSC_KEY_TYPE_UNKNOWN = 0,
 *		EVCTX_ATSC_KEY_TYPE_TOTAL_AVAIL_TIME,
 *		EVCTX_ATSC_KEY_TYPE_RC_INFO,
 *		EVCTX_ATSC_KEY_TYPE_TOTAL_NUMBER
 *	}EVCTX_ATSC_KEY_TYPE_T;  
 *  @endcode
 *
 *  @li@c	EVCTX_ATSC_KEY_TYPE_UNKNOWN			- Unknown key type
 *  @li@c	EVCTX_ATSC_KEY_TYPE_TOTAL_AVAIL_TIME	- Key type of total available time
 *  @li@c	EVCTX_ATSC_KEY_TYPE_TOTAL_NUMBER		- Total number of key types
 */
/*------------------------------------------------------------------*/
typedef enum _EVCTX_ATSC_KEY_TYPE_T
{
    EVCTX_ATSC_KEY_TYPE_UNKNOWN = 0,
    EVCTX_ATSC_KEY_TYPE_TOTAL_AVAIL_TIME,   /* UINT16 type. In hours. */
    EVCTX_ATSC_KEY_TYPE_RC_INFO,
    EVCTX_ATSC_KEY_TYPE_TOTAL_NUMBER
} EVCTX_ATSC_KEY_TYPE_T;


/*------------------------------------------------------------------*/
/*! @struct	EVCTX_ATSC_KEY_INFO_T
 *  @brief		Key infomation structure
 *
 *  @code
 *	typedef struct _EVCTX_ATSC_KEY_INFO_T
 *	{
 *		EVCTX_ATSC_KEY_TYPE_T   e_atsc_key_type;
 *		VOID*                   pv_atsc_key_info;
 *	} EVCTX_ATSC_KEY_INFO_T;   
 *  @endcode
 *
 *  @li@c	e_atsc_key_type		-Key type of EVCTX
 *  @li@c	pv_atsc_key_info		-Key information of EVCTX
 */
/*------------------------------------------------------------------*/
typedef struct _EVCTX_ATSC_KEY_INFO_T
{
    EVCTX_ATSC_KEY_TYPE_T   e_atsc_key_type;
    VOID*                   pv_atsc_key_info;
} EVCTX_ATSC_KEY_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct	EVCTX_ATSC_RATING_T
 *  @brief		Rating structure
 *
 *  @code
 *	typedef struct _EVCTX_ATSC_RATING_T
 *	{
 *		UINT16      ui2_dimension;
 *		UINT16      ui2_rating_value;
 *	} EVCTX_ATSC_RATING_T;  
 *  @endcode
 *
 *  @li@c	ui2_dimension			-Dimension of the rating
 *  @li@c	ui2_rating_value		-Value of the rating
 */
/*------------------------------------------------------------------*/

typedef struct _EVCTX_ATSC_RATING_T
{
    UINT16      ui2_dimension;
    UINT16      ui2_rating_value;
} EVCTX_ATSC_RATING_T;


/*
    pt_ratings is a pointer to an array of EVCTX_ATSC_RATING_T type data.
    ui2_rated_dimensions contains the number of elements in the pt_ratings
    array.
    
    The following is an illustration of the layout, which a real instance
    of EVCTX_ATSC_RATING_LIST_T may be.
    
    Arrows in the left side shows that the pt_ratings array is allocated
    exactly below the pt_ratings field, and followed by the rating 
    description string.
    
    Arrows in the right side shows how to calculate the total size of
    an instance of EVCTX_ATSC_RATING_LIST_T data.
    
        ui2_rating_region           <---
        ui2_rated_dimensions            | sizeof(EVCTX_ATSC_RATING_LIST_T)
------- ps_rating_description           |
| ----- pt_ratings                      |
| |  -- pt_next                     <---                   +
| |  |
| ----->  pt_ratings[0]                      <---
|    |    pt_ratings[1]                          | ui2_rated_dimensions *
|    |    pt_ratings[2]                          | sizeof(EVCTX_ATSC_RATING_T)
|    |        :                                  |
|    |    pt_ratings[ui2_rated_dimensions-1] <---          +
--------> "TV-PG-L-S"                        <---  x_strlen(pt_rating_description)+1
     |
     |
     -> ui2_rating_region   (the next region)
        ui2_rated_dimensions
        pt_rating_description
        pt_ratings
        pt_next
*/        
/*------------------------------------------------------------------*/
/*! @struct	EVCTX_ATSC_RATING_LIST_T
 *  @brief		Rating list structure
 *
 *  @code
 *	typedef struct _EVCTX_ATSC_RATING_LIST_T
 *	{
 *		UINT16                  ui2_rating_region;
 *  		UINT16                  ui2_rated_dimensions;
 *   		CHAR*                   ps_rating_descrption;
 *   		EVCTX_ATSC_RATING_T*    pt_ratings;
 *
 *   		struct _EVCTX_ATSC_RATING_LIST_T*   pt_next;
 *	} EVCTX_ATSC_RATING_T;  
 *  @endcode
 *
 *  @li@c	ui2_rating_region			-Region index 
 *  @li@c	ui2_rated_dimensions		-Total dimension number
 *  @li@c	ps_rating_descrption		-Descrption of the rating
 *  @li@c	pt_ratings				-The one of rating
 *  @li@c	pt_next					-The point to the other _EVCTX_ATSC_RATING_LIST_T
 */
/*------------------------------------------------------------------*/

typedef struct _EVCTX_ATSC_RATING_LIST_T
{
    UINT16                  ui2_rating_region;
    UINT16                  ui2_rated_dimensions;
    CHAR*                   ps_rating_descrption;
    EVCTX_ATSC_RATING_T*    pt_ratings;

    struct _EVCTX_ATSC_RATING_LIST_T*   pt_next;
} EVCTX_ATSC_RATING_LIST_T;


#endif /* _U_EVCTX_ATSC_ENG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_EVCTX_ATSC_ENG          */
/*----------------------------------------------------------------------------*/

