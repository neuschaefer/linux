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
/*! @file u_evctx_isdb_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *
 *  @par Description:
 *         This header file contains exported definitions and delcarations of
 *         Event Context ISDB engine.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_EVCTX_EVCTX_ISDB_ENG EVCTX ISDB Engine
 *  @ingroup groupMW_EVCTX
 *  @brief Provides events operate function for ISDB
 *  %This module is Event Context ISDB instance.
 *  @see groupMW_EVCTX
 *  @{ 
 */
/*----------------------------------------------------------------------------*/ 
#ifndef _U_EVCTX_ISDB_ENG_H_
#define _U_EVCTX_ISDB_ENG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
 

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
typedef UINT32      EVCTX_ISDB_CONFIG_T;    /**< EVCTX DVB Engine config type       */

#define EVCTX_ISDB_CONFIG_PARSE_EXTEND_ITEMS                         (MAKE_BIT_MASK_32(0))    /**< Config of parse extended event items*/
#define EVCTX_ISDB_CONFIG_USE_MAX_EVENT_NAME                         (MAKE_BIT_MASK_32(1))    /**< Config of use max length retrive content*/
#define EVCTX_ISDB_CONFIG_NO_FAKE_AVAIL                              (MAKE_BIT_MASK_32(2))    /**< Config of no fake avail*/


/*------------------------------------------------------------------*/
/*! @enum EVCTX_ISDB_KEY_TYPE_T
 *  @brief key type struture
 *  @code
 *  typedef enum _EVCTX_ISDB_KEY_TYPE_T
 *  {
 *      EVCTX_ISDB_KEY_TYPE_UNKNOWN = 0,
 *      EVCTX_ISDB_KEY_TYPE_TOTAL_AVAIL_TIME,   
 *      EVCTX_ISDB_KEY_TYPE_TOTAL_NUMBER
 *  } EVCTX_ISDB_KEY_TYPE_T;
 *  @endcode
 *  @li@c  EVCTX_ISDB_KEY_TYPE_UNKNOWN                        - unknow key type
 *  @li@c  EVCTX_ISDB_KEY_TYPE_TOTAL_AVAIL_TIME               - key type for total available time
 *  @li@c  EVCTX_ISDB_KEY_TYPE_TOTAL_NUMBER                   - key type for totla number
 */
/*------------------------------------------------------------------*/
typedef enum _EVCTX_ISDB_KEY_TYPE_T
{
    EVCTX_ISDB_KEY_TYPE_UNKNOWN = 0,
    EVCTX_ISDB_KEY_TYPE_TOTAL_AVAIL_TIME,   /* UINT16 type. In hours. */
    EVCTX_ISDB_KEY_TYPE_TOTAL_NUMBER
} EVCTX_ISDB_KEY_TYPE_T;


/*------------------------------------------------------------------*/
/*! @struct EVCTX_ISDB_KEY_INFO_T
 *  @brief key info struture
 *  @code
 *  typedef struct _EVCTX_ISDB_KEY_INFO_T
 *  {
 *      EVCTX_ISDB_KEY_TYPE_T    e_isdb_key_type;
 *      VOID*                   pv_isdb_key_info;
 *  } EVCTX_ISDB_KEY_INFO_T;
 *  @endcode
 *  @li@c  e_isdb_key_type                                    - reference EVCTX_ISDB_KEY_TYPE_T
 *  @li@c  pv_isdb_key_info                                   - key information
 */
/*------------------------------------------------------------------*/
typedef struct _EVCTX_ISDB_KEY_INFO_T
{
    EVCTX_ISDB_KEY_TYPE_T    e_isdb_key_type;
    VOID*                   pv_isdb_key_info;
} EVCTX_ISDB_KEY_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct EVCTX_ISDB_RATING_T
 *  @brief ISDB rating structure
 *  @code
 *  typedef struct _EVCTX_ISDB_RATING_T
 *  {
 *      UINT16      ui2_dimension;
 *      UINT16      ui2_rating_value;
 *  } EVCTX_ISDB_RATING_T;
 *  @endcode
 *  @li@c  ui2_dimension                                     - ratine dimension 
 *  @li@c  ui2_rating_value                                  - rating value
 */
/*------------------------------------------------------------------*/
typedef struct _EVCTX_ISDB_RATING_T
{
    UINT16      ui2_dimension;
    UINT16      ui2_rating_value;
} EVCTX_ISDB_RATING_T;

/*
*            ui2_rating_region           <---
*            ui2_rated_dimensions            |
*     ------ ps_rating_description           | sizeof(EVCTX_DVB_RATING_LIST_T)
*    |  ---- pt_ratings                      |
*    |  |    pt_next -> NULL (for compatible with generic rating structure)
*    |  |                                    |
*    |  -----> pt_ratings[0]                 |
*     -----> "GBR"                       <---  (ISO_3166_COUNT_LEN)
*/

    
/*------------------------------------------------------------------*/
/*! @struct EVCTX_ISDB_RATING_LIST_T
 *  
 *    @brief
 *         The field ui2_rated_dimensions is used to backward compatible with 
 *    EVCTX_ATSC_RATING_LIST_T, but always set to 1.
 *    
 *    The following is an illustration of the layout, which a real instance
 *    of EVCTX_ISDB_RATING_LIST_T may be.
 *        
 *    Arrows in the right side shows how to calculate the total size of
 *    an instance of EVCTX_ISDB_RATING_LIST_T data.
 *    
 *  @code
 *  typedef struct _EVCTX_ISDB_RATING_LIST_T
 *  {
 *      UINT16                  ui2_rating_region;
 *      UINT16                  ui2_rated_dimensions;
 *      CHAR*                   ps_country_code;
 *      EVCTX_ISDB_RATING_T*     pt_ratings;
 *      struct _EVCTX_ISDB_RATING_LIST_T*  pt_next;
 *      EVCTX_ISDB_RATING_T      t_rating;
 *      ISO_3166_COUNT_T        t_country_code;
 *  } EVCTX_ISDB_RATING_LIST_T;
 *  @endcode
 *  @li@c  ui2_rating_region                                 - ratine region
 *  @li@c  ui2_rated_dimensions                              - ratine dimensions
 *  @li@c  ps_country_code                                   - country code
 *  @li@c  pt_ratings                                        - ratings 
 *  @li@c  pt_next                                           - point to next
 *  @li@c  t_rating                                          - structure of EVCTX_ISDB_RATING_T
 *  @li@c  t_country_code                                    - structure of ISO_3166_COUNT_T
*/
/*------------------------------------------------------------------*/
typedef struct _EVCTX_ISDB_RATING_LIST_T
{
    UINT16                  ui2_rating_region;
    UINT16                  ui2_rated_dimensions;
    CHAR*                   ps_country_code;
    EVCTX_ISDB_RATING_T*     pt_ratings;

    struct _EVCTX_ISDB_RATING_LIST_T*  pt_next;

    EVCTX_ISDB_RATING_T      t_rating;
    ISO_3166_COUNT_T        t_country_code;
} EVCTX_ISDB_RATING_LIST_T;


/*------------------------------------------------------------------*/
/*! @struct EVCTX_ISDB_TIME_SHIFTED_EVT_T
 *  @brief for time shift used
 *  @code
 *  typedef struct _EVCTX_ISDB_TIME_SHIFTED_EVT_T
 *  {
 *      UINT16                  ui2_ref_svc_id;
 *      UINT16                  ui2_ref_evt_id;
 *  } EVCTX_ISDB_TIME_SHIFTED_EVT_T;
 *  @endcode
 *  @li@c  ui2_ref_svc_id                                    - reference service id
 *  @li@c  ui2_ref_evt_id                                    - reference event id
 */
/*------------------------------------------------------------------*/
typedef struct _EVCTX_ISDB_TIME_SHIFTED_EVT_T
{
    UINT16                  ui2_ref_svc_id;
    UINT16                  ui2_ref_evt_id;
} EVCTX_ISDB_TIME_SHIFTED_EVT_T;

#endif /* _U_EVCTX_ISDB_ENG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                 /* end of groupMW_CONN_MNGR_BRDCST */
/*----------------------------------------------------------------------------*/

