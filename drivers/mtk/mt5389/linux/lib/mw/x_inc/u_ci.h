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
/*! @file u_ci.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Common Interface (CI) specific
 *         definitions, which are exported.
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_CI  CI Engine
 *
 *  @ingroup groupMW
 *  @brief Provides constant,type definitions and function of the CI Engine.. CI Engine is a middleware component that is responsible for communication between Host and CICAM and  descrambling the scrambled the stream components before passing them to demux
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_CI_H_
#define _U_CI_H_

/*-------------------------------------------------------------------------
                    include files
-------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_lnk_list.h"

/*-------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @enum CI_NFY_COND_T
 *  @brief Used in x_ci_nfy_fct() callback function to notify message received from Card
 *  @code
 *  typedef enum
 *  {
 *      CI_NFY_COND_CARD_INSERT = 0,
 *      CI_NFY_COND_CARD_NAME,
 *      CI_NFY_COND_CARD_REMOVE,
 *      CI_NFY_COND_MMI_ENQUIRY,
 *      CI_NFY_COND_MMI_MENU,
 *      CI_NFY_COND_MMI_LIST,
 *      CI_NFY_COND_MMI_CLOSE,
 *      CI_NFY_COND_HOST_TUNE,
 *      CI_NFY_COND_HOST_REPLACE,
 *      CI_NFY_COND_HOST_CLEAR_REPLACE,
 *      CI_NFY_COND_HOST_SESS_CLOSE,
 *      CI_NFY_COND_CA_SYSTEM_ID_WAIT,
 *      CI_NFY_COND_CA_SYSTEM_ID_READY,
 *      CI_NFY_COND_CA_SYSTEM_ID_MATCH
 * #ifdef CI_PLUS_SUPPORT
 *   ,
 *   CI_NFY_COND_FIRMWARE_UPGRADE,
 *   CI_NFY_COND_FIRMWARE_UPGRADE_PROGRESS,        
 *   CI_NFY_COND_REQUEST_START,
 *   CI_NFY_COND_APP_ABORT_REQUEST,
 *   CI_NFY_COND_CIPLUS_CAM_STATUS,
 *   CI_NFY_COND_ERROR
* #endif
 *  } CI_NFY_COND_T;
 *  @endcode
 *  @li@c  CI_NFY_COND_CARD_INSERT                           -notify that the Card is insterted, 
 *                                                                                     pv_data in x_ci_nfy_fct is NULL 
 *  @li@c  CI_NFY_COND_CARD_NAME                             -notify that  the Card's name is ready
 *                                                                                     pv_data in x_ci_nfy_fct is NULL 
 *  @li@c  CI_NFY_COND_CARD_REMOVE                           -notify that the Card is removed
 *                                                                                     pv_data in x_ci_nfy_fct is NULL 
 *  @li@c  CI_NFY_COND_MMI_ENQUIRY                           -notify the MMI Enq object 
 *                                                                                     pv_data in x_ci_nfy_fct is CI_MMI_ENQ_T pointer 
 *  @li@c  CI_NFY_COND_MMI_MENU                              -notify the MMI Menu object
 *                                                                                     pv_data in x_ci_nfy_fct is CI_MMI_MENU_T pointer 
 *  @li@c  CI_NFY_COND_MMI_LIST                              -notify the MMI List object
 *                                                                                     pv_data in x_ci_nfy_fct is CI_MMI_MENU_T pointer 
 *  @li@c  CI_NFY_COND_MMI_CLOSE                             -notify that  the MMI Menu or List  is required to close
 *                                                                                     pv_data in x_ci_nfy_fct is CI_MMI_ENQ_CLOSE_T pointer 
 *  @li@c  CI_NFY_COND_HOST_TUNE                             -notify that  the Card requires to tune service
 *                                                                                     pv_data in x_ci_nfy_fct is CI_HC_TUNE_T pointer 
 *  @li@c  CI_NFY_COND_HOST_REPLACE                          -notify that  the Card requires to replace one stream component for current service
 *                                                                                     pv_data in x_ci_nfy_fct is CI_HC_REPLACE_T pointer 
 *  @li@c  CI_NFY_COND_HOST_CLEAR_REPLACE                    -notify that the Card requires to recover the replaced  stream component for current service
 *                                                                                     pv_data in x_ci_nfy_fct is CI_HC_CLR_REPLACE_T pointer 
 *  @li@c  CI_NFY_COND_HOST_SESS_CLOSE                       -notify the session is closed
 *                                                                                     pv_data in x_ci_nfy_fct is UINT32 value indicate slot id
 *  @li@c  CI_NFY_COND_CA_SYSTEM_ID_WAIT                     -notify the ca system id is not ready while selecting program
 *                                                                                     pv_data in x_ci_nfy_fct is UINT32 value indicate slot id
 *  @li@c  CI_NFY_COND_CA_SYSTEM_ID_READY                    -notify that the CA system ID is ready
 *                                                                                     pv_data in x_ci_nfy_fct is UINT8 value indicate slot id
 *  @li@c  CI_NFY_COND_CA_SYSTEM_ID_MATCH                    -notify that the CA system ID is math with pmt's ca system id for current service
 *                                                                                     pv_data in x_ci_nfy_fct is CI_CA_SYSTEM_ID_MATCH_T* value indicates if is match.
 *  @li@c  CI_NFY_COND_FIRMWARE_UPGRADE                    -notify that the CAM request to upgrade software on Host
 *                                                                                     pv_data in x_ci_nfy_fct is CI_CAM_UPGRADE_T* value indicates upgrade type.
 *  @li@c  CI_NFY_COND_FIRMWARE_UPGRADE_PROGRESS    -notify that the CAM upgrade progress
 *                                                                                     pv_data in x_ci_nfy_fct is CI_CAM_UPGRADE_PROGRESS_T* value indicates upgrade progress.
 *  @li@c  CI_NFY_COND_REQUEST_START                            -notify that the CAM request to start APP
 *                                                                                     pv_data in x_ci_nfy_fct is CI_REQUEST_START_T* value indicates APP's infomation.
 *  @li@c  CI_NFY_COND_APP_ABORT_REQUEST                     -notify that the CAM request to abort APP
 *                                                                                     pv_data in x_ci_nfy_fct is CI_ABORT_REQUEST_T* value indicates APP abort reason.
 *  @li@c  CI_NFY_COND_CIPLUS_CAM_STATUS                      -notify that the CAM request to abort APP
 *                                                                                     pv_data in x_ci_nfy_fct is UINT8 value indicates slot id.
 *  @li@c  CI_NFY_COND_ERROR                                           -notify that the error occured
 *                                                                                     pv_data in x_ci_nfy_fct is CI_NFY_ERROR_DATA_T* value indicates error code.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CI_NFY_COND_CARD_INSERT = 0,
    CI_NFY_COND_CARD_NAME,
    CI_NFY_COND_CARD_REMOVE,
    CI_NFY_COND_MMI_ENQUIRY,
    CI_NFY_COND_MMI_MENU,
    CI_NFY_COND_MMI_LIST,
    CI_NFY_COND_MMI_CLOSE,
    CI_NFY_COND_HOST_TUNE,
    CI_NFY_COND_HOST_REPLACE,
    CI_NFY_COND_HOST_CLEAR_REPLACE,
    CI_NFY_COND_HOST_SESS_CLOSE,
    CI_NFY_COND_CA_SYSTEM_ID_WAIT,
    CI_NFY_COND_CA_SYSTEM_ID_READY,
    CI_NFY_COND_CA_SYSTEM_ID_MATCH
#ifdef CI_PLUS_SUPPORT
    ,
    CI_NFY_COND_FIRMWARE_UPGRADE,
    CI_NFY_COND_FIRMWARE_UPGRADE_PROGRESS,        
    CI_NFY_COND_REQUEST_START,
    CI_NFY_COND_APP_ABORT_REQUEST,
    CI_NFY_COND_CIPLUS_CAM_STATUS,
    CI_NFY_COND_ERROR,
    CI_NFY_COND_CARD_NEED_RESET
#endif
} CI_NFY_COND_T;

/*------------------------------------------------------------------*/
/*! @enum CA_SYSTEM_ID_STATUS_T
 *  @brief Specify the current CA resouce status
 *  @code
 *  typedef enum
 *  {
 *     CA_SYSTEM_ID_INVALID = 0, 
 *     CA_SYSTEM_ID_WAIT,
 *     CA_SYSTEM_ID_READY,
 *     CA_SYSTEM_ID_MATCH,
 *     CA_SYSTEM_ID_NOT_MATCH
 *  } CA_SYSTEM_ID_STATUS_T;
 *  @endcode
 *  @li@c  CA_SYSTEM_ID_INVALID           - the initial status
 *  @li@c  CA_SYSTEM_ID_WAIT              - can inserted, wait for ca resouce from CAM
 *  @li@c  CA_SYSTEM_ID_READY             - Get CA system id from ca_info command
 *  @li@c  CA_SYSTEM_ID_MATCH             - Current PMT's ca system id match ca_info 
 *  @li@c  CA_SYSTEM_ID_NOT_MATCH         - Current PMT's ca system id match ca_info 
 *
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CA_SYSTEM_ID_INVALID = 0, 
    CA_SYSTEM_ID_WAIT,
    CA_SYSTEM_ID_READY,
    CA_SYSTEM_ID_MATCH,
    CA_SYSTEM_ID_NOT_MATCH
} CA_SYSTEM_ID_STATUS_T;

#ifdef CI_PLUS_SUPPORT
/*------------------------------------------------------------------*/
/*! @enum CI_QUERY_TYPE_T
 *  @brief  specifies query type
 *  @code
 * typedef enum
 * {
 *   CI_QUERY_TYPE_HOST_COUNTY = 0,
 *   CI_QUERY_TYPE_HOST_LANGUAGE
 * } CI_QUERY_TYPE_T;
 *  @endcode
 *  @li@c  CI_QUERY_TYPE_HOST_COUNTY                              - This type means query host country code
 *  @li@c  CI_QUERY_TYPE_HOST_LANGUAGE                          - This type means query host language code
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CI_QUERY_TYPE_HOST_COUNTY = 0,
    CI_QUERY_TYPE_HOST_LANGUAGE
} CI_QUERY_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum CI_NFY_ERROR_CODE_T
 *  @brief  specifies CI Notify error code 
 *  @code
 *  typedef enum
 * {
 *   CI_NFY_ERR_FIRMWARE_UPGRADE_PROGRESS_TIMEOUT = 0,
 *   CI_NFY_ERR_FIRMWARE_UPGRADE_CANCEL = 1
 * }CI_NFY_ERROR_CODE_T;
 
 *  @endcode
 *  @li@c  CI_NFY_ERR_FIRMWARE_UPGRADE_PROGRESS_TIMEOUT                              - Indicate error reason is firmware upgrade progress timeout
 *  @li@c  CI_NFY_ERR_FIRMWARE_UPGRADE_CANCEL                              - Indicate error reason is firmware upgrade progress is canceled
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CI_NFY_ERR_FIRMWARE_UPGRADE_PROGRESS_TIMEOUT = 0,
    CI_NFY_ERR_FIRMWARE_UPGRADE_CANCEL = 1
}CI_NFY_ERROR_CODE_T;

typedef UINT8 CI_REQUEST_START_ACK_CODE_T;/**< Defines APP start request acknowledge code    */
#define CI_REQUEST_START_ACK_CODE_OK         ((CI_REQUEST_START_ACK_CODE_T) 0x01)/**<   Start request is ok   */
#define CI_REQUEST_START_ACK_CODE_WRONG_API ((CI_REQUEST_START_ACK_CODE_T) 0x02)/**<   Start request is failed because using wrong api  */
#define CI_REQUEST_START_ACK_CODE_API_BUSY ((CI_REQUEST_START_ACK_CODE_T) 0x03)/**<   Start request is failed because host is busy now  */

typedef UINT8 CI_ABORT_REQUEST_CODE_T;/**< Defines APP abort request code    */
#define CI_ABORT_CODE_USER_CANCEL ((CI_ABORT_REQUEST_CODE_T)    0x01)/**<   Abort  request is trigged by user cancel   */
#define CI_ABORT_CODE_SYSTEM_CANCEL ((CI_ABORT_REQUEST_CODE_T)    0x02)/**<   Abort request is trigged by system cancel  */

typedef UINT8 CI_ABORT_ACK_CODE_T;/**<Defines APP abort ack code. It is defines by host now. */
#define CI_ABORT_ACK_CODE_CAM_REMOVED ((CI_ABORT_ACK_CODE_T)    0x01)/**<   APP abort is because of CAM removed   */
#define CI_ABORT_ACK_CODE_CAM_ABORT_APP ((CI_ABORT_ACK_CODE_T)    0x02)/**<   APP abort is ok   */

typedef UINT8 CI_FILE_SEARCH_MODE_T;/**<This structure contains the file name search mode to update data from usb.     */
#define CI_FILE_SEARCH_MODE_INVALID    ((CI_FILE_SEARCH_MODE_T) 0x00)/**<  search mode invalid */
#define CI_FILE_SEARCH_MODE_FULL       ((CI_FILE_SEARCH_MODE_T) 0x01)/**<  search through full file name */
#define CI_FILE_SEARCH_MODE_PREFIX_EXT ((CI_FILE_SEARCH_MODE_T) 0x02)/**<  search through file extention and prefix file name*/
#define CI_FILE_SEARCH_MODE_PREFIX     ((CI_FILE_SEARCH_MODE_T) 0x03)/**<  search through file prefix name */
#define CI_FILE_SEARCH_MODE_EXT        ((CI_FILE_SEARCH_MODE_T) 0x04)/**<  search through file extention name*/
#endif

typedef UINT32 CI_SET_TYPE_CI_ON_OFF_T;    /**< indicates ci on/off set type       */
#define CI_SET_TYPE_CI_OFF              ((CI_SET_TYPE_CI_ON_OFF_T)    0)    /**<   turn off CI driver     */
#define CI_SET_TYPE_CI_ON               ((CI_SET_TYPE_CI_ON_OFF_T)    1)    /**<   turn on CI driver    */

typedef UINT32 CI_SET_TYPE_TS_ON_OFF_T;    /**< indicates ts on/off set type       */
#define CI_SET_TYPE_TS_OFF              ((CI_SET_TYPE_TS_ON_OFF_T)    0)  /**<disble TS, do not go through CAM        */
#define CI_SET_TYPE_TS_ON               ((CI_SET_TYPE_TS_ON_OFF_T)    1)  /**< enable TS to go through CAM       */

typedef UINT32 CI_SET_TYPE_INP_SRC_T;    /**< indicates input source set type */
#define CI_SET_TYPE_INVALID             ((CI_SET_TYPE_INP_SRC_T)    0)  /**   initial value   */
#define CI_SET_TYPE_NON_DTV             ((CI_SET_TYPE_INP_SRC_T)    1)  /**   other input source, not DTV    */
#define CI_SET_TYPE_DTV                 ((CI_SET_TYPE_INP_SRC_T)    2)  /**   DTV   */

typedef UINT32 CI_SET_TYPE_FORCE_THROUGH_T;    /**< indicates force TS path to go through CAM or not */
#define CI_SET_TYPE_THROUGH_CAM_OFF     ((CI_SET_TYPE_FORCE_THROUGH_T)    0)  /**   go through CAM   */
#define CI_SET_TYPE_THROUGH_CAM_ON      ((CI_SET_TYPE_FORCE_THROUGH_T)    1)  /**   not go through CAM    */

typedef UINT32 CI_GET_TYPE_T;    /**<  get info type used in  c_ci_get() function     */
typedef UINT32 CI_SET_TYPE_T;    /**<   set info type used in  c_ci_set() function     */
typedef SLIST_T(_CI_MMI_MENU_ITEM_T) CI_MMI_MENU_ITEM_LIST_T;    /**<        */

#define CI_GET_TYPE_APP_NAME            ((CI_GET_TYPE_T)    0)    /**<  get Card's name, related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 value means slot id,

                                                                                                                             pv_data:input&output UTF_16* value means Card's name,

                                                                                                                             pz_data_len:input&output SIZE_T* value means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_SLOT_NUM            ((CI_GET_TYPE_T)    1)    /**<  get slot number, related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input NULL,

                                                                                                                             pv_data:input&output UINT8* value means slot number,

                                                                                                                             pz_data_len:input&output SIZE_T*  value means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_SLOT_INFO           ((CI_GET_TYPE_T)    2)    /**< get slot info, related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 means slot id,

                                                                                                                             pv_data:input&output CI_SLOT_INFO_T* value,

                                                                                                                             pz_data_len:input&output SIZE_T* value means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_PREVIOUS_MMI        ((CI_GET_TYPE_T)    3)    /**<  get MMI menu/list object which is previously notified through x_ci_nfy_fct(),

                                                                                                                             related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input NULL,

                                                                                                                             pv_data:input&output CI_MMI_MENU_T* value,

                                                                                                                             pz_data_len:input&output SIZE_T* value means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_PREVIOUS_MMI_ENQ    ((CI_GET_TYPE_T)    4)    /**<  get MMI enq object which is previously notified through x_ci_nfy_fct(),

                                                                                                                             related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input NULL,

                                                                                                                             pv_data:input&output CI_MMI_ENQ_T* value,

                                                                                                                             pz_data_len:input&output SIZE_T* value means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_CA_SYSTEM_ID        ((CI_GET_TYPE_T)    5)    /**<  get CA system id array, related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 value means slot id,

                                                                                                                             pv_data:input&output UINT16* value means system id array,

                                                                                                                             pz_data_len:input&output  SIZE_T* value means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_CA_SYSTEM_ID_NUM    ((CI_GET_TYPE_T)    6)    /**<  get CA system id number, related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 means slot id,

                                                                                                                             pv_data:input&output UINT16* value means CA system id number ,

                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_MENU_LIST_ID        ((CI_GET_TYPE_T)    7)    /**<  get id of menu or list related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 means slot id,

                                                                                                                             pv_data:input&output UINT16* value means the id ,

                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_ENQ_ID              ((CI_GET_TYPE_T)    8)    /**<  get id of enquiry related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 means slot id,

                                                                                                                             pv_data:input&output UINT16* value means the id ,

                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_CIPLUS_CAM_STATUS   ((CI_GET_TYPE_T)    9)    /**<  get CAM type if is CI plus or not related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 means slot id,

                                                                                                                             pv_data:input&output BOOL* value means the id ,

                                                                                                                             pz_data_len:input&output  BOOL* value, means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_CA_SYSTEM_ID_MATCH   ((CI_GET_TYPE_T)   10)    /**<  get id of enquiry related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 means slot id,

                                                                                                                             pv_data:input&output UINT16* value means the id ,

                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length.
                                                                                                                    */

#define CI_GET_TYPE_CA_SYSTEM_ID_STATUS   ((CI_GET_TYPE_T)   11)    /**<  get id of enquiry related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 means slot id,

                                                                                                                             pv_data:input&output UINT16* value means the id ,

                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length.
                                                                                                                    */


#define CI_GET_TYPE_LAST_ENTRY          ((CI_GET_TYPE_T)    32)    /**<        */

#define CI_SET_TYPE_CLOSE               ((CI_SET_TYPE_T)    0)    /**<  close MMI, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:input UINT32 value means slot number

                                                                                                                             pv_data:NULL.
                                                                                                                    */
#define CI_SET_TYPE_ENTER_MENU          ((CI_SET_TYPE_T)    1)     /**<  enter MMI menu, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:input UINT32 value means slot number

                                                                                                                             pv_data:NULL.
                                                                                                                    */
#define CI_SET_TYPE_MMI_ENQ_ANS         ((CI_SET_TYPE_T)    2)     /**<  answer ENQ object, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:NULL,

                                                                                                                             pv_data:input CI_MMI_ENQ_ANS_T * value.
                                                                                                                    */
#define CI_SET_TYPE_MMI_MENU_ANS        ((CI_SET_TYPE_T)    3)    /**<  answer Menu object, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:NULL,

                                                                                                                             pv_data:input CI_MMI_MENU_ANS_T * value.
                                                                                                                    */
#define CI_SET_TYPE_HC_ASK_RELEASE      ((CI_SET_TYPE_T)    4)     /**<  ask CAM to release control, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:input UINT32 value means slot number

                                                                                                                             pv_data:NULL.
                                                                                                                    */
#define CI_SET_TYPE_CI_ON_OFF           ((CI_SET_TYPE_T)    5)     /**<  control CI driver, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:input CI_SET_TYPE_CI_ON_OFF_T value,

                                                                                                                             pv_data:NULL.
                                                                                                                    */
#define CI_SET_TYPE_NFY_COLSE_DONE      ((CI_SET_TYPE_T)    6)     /**<  notify MMI close done, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:input UINT32 value means slot number,

                                                                                                                             pv_data:NULL.
                                                                                                                    */
#define CI_SET_TYPE_TS_ON_OFF           ((CI_SET_TYPE_T)    7)     /**<  control TS path, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:input CI_SET_TYPE_TS_ON_OFF_T value,

                                                                                                                             pv_data:NULL.
                                                                                                                    */
#ifdef CI_PLUS_SUPPORT
#define CI_SET_TYPE_CAM_UPGRADE_REPLY   ((CI_SET_TYPE_T)    8)/**<  notify CAM upgrade reply, related arguments in c_ci_set(),

                                                                                                                       pv_set_info:NULL,

                                                                                                                       pv_data:input CI_CAM_UPGRADE_PROGRESS_REPLY_T* data contains reply data.
                                                                                                                */
#define CI_SET_TYPE_REQUEST_START_ACK   ((CI_SET_TYPE_T)    9)/**<  notify APP about start request reply, related arguments in c_ci_set(),

                                                                                                                       pv_set_info:NULL,

                                                                                                                       pv_data:input CI_REQUEST_START_ACK_T* data contains reply data.
                                                                                                                */
#define CI_SET_TYPE_APP_ABORT_REQUEST   ((CI_SET_TYPE_T)    10)/**<  request CAM to abort APP, related arguments in c_ci_set(),

                                                                                                                       pv_set_info:NULL,

                                                                                                                       pv_data:input CI_ABORT_REQUEST_T* data means abort data.
                                                                                                                */
#define CI_SET_TYPE_APP_ABORT_ACK       ((CI_SET_TYPE_T)    11)/**<  notify CAM that APP abort finished, related arguments in c_ci_set(),

                                                                                                                       pv_set_info:NULL,

                                                                                                                       pv_data:NULL
                                                                                                                */
#define CI_SET_TYPE_HOST_COUNTRY        ((CI_SET_TYPE_T)    12)/**<  notify CAM host country code, related arguments in c_ci_set(),

                                                                                                                       pv_set_info:NULL,

                                                                                                                       pv_data:input ISO_3166_COUNT_T* data contains host country code.
                                                                                                                */
#define CI_SET_TYPE_HOST_LANGUAGE       ((CI_SET_TYPE_T)    13)/**<  notify CAM host language code, related arguments in c_ci_set(),

                                                                                                                       pv_set_info:NULL,

                                                                                                                       pv_data:input ISO_639_LANG_T* data contains host language code.
                                                                                                                */
#define CI_SET_TYPE_SCAN_END_AND_DT_SYNC       ((CI_SET_TYPE_T)    14)/**<  notify that scan is complete and dt is sync status,

                                                                                                                       pv_set_info:NULL,

                                                                                                                       pv_data:NULL.
                                                                                                                */
#define CI_SET_TYPE_NEED_RESET_CAM            ((CI_SET_TYPE_T)    15)/**<  notify CI driver to reset CI plus CAM to do authentication again ,
    
                                                                                                                           pv_set_info:NULL,
    
                                                                                                                           pv_data:NULL.
                                                                                                                    */
#endif
#define CI_SET_TYPE_INP_SRC              ((CI_SET_TYPE_T)   16)/**<  notify CAM about current source setting, DTV or not, related arguments in c_ci_set(),

                                                                                                                       pv_set_info: CI_SET_TYPE_INP_SRC_T

                                                                                                                       pv_data:. NULL
                                                                                                                */

#define CI_SET_TYPE_FORCE_THROUGH_CAM    ((CI_SET_TYPE_T)   17)/**<  notify CI driver to rough TS path no matter CAM insert or not. , related arguments in c_ci_set(),

                                                                                                                       pv_set_info: CI_SET_TYPE_FORCE_THROUGH_T

                                                                                                                       pv_data:. NULL
                                                                                                                */
                                                                                                                
#define CI_SET_TYPE_LAST_ENTRY          ((CI_SET_TYPE_T)    32)    /**<        */

#define CIR_OK                      ((INT32)    0)    /**< The routine is successful     */
#define CIR_FAILED                  ((INT32)    -1)    /**<The routine is failed         */
#define CIR_ALREADY_INIT            ((INT32)    -2)    /**<  CI engine has been initialized       */
#define CIR_NOT_INIT                ((INT32)    -3)    /**< CI engine has not initialized       */
#define CIR_INV_ARG                 ((INT32)    -4)    /**< Indicates at least one argument is invalid. */
#define CIR_OUT_OF_HANDLES          ((INT32)    -5)    /**<There isn't enough handles or the caller's  handle resource is exhausted.        */
#define CIR_INV_GET_TYPE            ((INT32)    -6)    /**< e_get_type value is invalid in c_ci_get()       */
#define CIR_INV_GET_INFO            ((INT32)    -7)    /**< pv_get_info value is invalid in c_ci_get()      */
#define CIR_INV_GET_DATA            ((INT32)    -8)    /**<  pv_data value is invalid in c_ci_get()      */
#define CIR_INV_SET_TYPE            ((INT32)    -9)    /**< e_set_type value is invalid in c_ci_set()       */
#define CIR_INV_SET_INFO            ((INT32)    -10)    /**<  pv_set_info value is invalid in c_ci_set()      */
#define CIR_INV_SET_DATA            ((INT32)    -11)    /**<   pv_data value is invalid in c_ci_set()          */
#define CIR_NOT_ENOUGH_SPACE        ((INT32)    -12)    /**<There is not enough memory to complete the operation.        */
#define CIR_CLI_ERROR               ((INT32)    -13)    /**< cli command occur error       */
#define CIR_INIT_FAILED             ((INT32)    -14)    /**<CI engine initialized failed     */
#define CIR_NOT_REGISTERED          ((INT32)    -15)    /**<No related callback function registered        */
#define CIR_ALREADY_REGISTERED      ((INT32)    -16)    /**<Related callback function already registered        */
#define CIR_INV_NAME                ((INT32)    -17)    /**< CI engine's name is invalid       */
#define CIR_ALREADY_SET_ENTER_MENU  ((INT32)    -18)    /**< MMI Menu has been already entered       */
#define CIR_ENC_DEC_FAILED          ((INT32)    -19)    /**< Encryption or decryption fail     */
#define CIR_SESSION_NOT_ESTABLISHED ((INT32)    -20)    /**< Session not open     */
#define CIR_CHECKSUM_ERROR          ((INT32)    -21)    /**< checksum error     */

typedef UINT32 CI_APPEAR_COND_T;
#define CI_APPEAR_COND_INSERT     ((CI_APPEAR_COND_T) 0)  /**< Card is insterted     */
#define CI_APPEAR_COND_CARD_NAME  ((CI_APPEAR_COND_T) 1)  /**< Card's name is ready     */
#define CI_APPEAR_COND_REMOVE     ((CI_APPEAR_COND_T) 2)  /**< Card is removed    */
#define CI_APPEAR_COND_MAX        ((CI_APPEAR_COND_T) 32) /**< Max condition, debug usage, not valid status   */

#ifdef ANDROID
#define MAX_MENU_ITEMS 10
#define MAX_STRING 256
#endif

/*Command Category Prefix*/
#define CI_CMD_NOTIFY            ((UINT32)0x10000000)    /**< NOTIFY Command Category Prefix       */
#define CI_CMD_QUERY             ((UINT32)0x20000000)    /**< QUERY Command Category Prefix      */

/* Notify functions */
#define CI_REG_EVENT_NFY_MENU          ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(0)))  /**< notify tag from menu application */
#define CI_REG_EVENT_NFY_NAV_CORE      ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(1)))  /**< notify tag from nav core application */
#define CI_REG_EVENT_NFY_NAV_CI        ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(2)))  /**< notify tag from nav ci application */
#define CI_REG_EVENT_NFY_EPG           ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(3)))  /**< notify tag from epg application */
#define CI_REG_EVENT_NFY_NAV_MHEG      ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(4)))  /**< notify tag from mheg application */
#define CI_REG_EVENT_NFY_ANDROID       ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(5)))  /**< notify tag from mheg application */
#define CI_REG_EVENT_NFY_LAST          ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(23)))  /**< last index, not used  */

/* Query functions */
#define CI_REG_EVENT_QUERY_NAV_CI      ((UINT32)(CI_CMD_QUERY | MAKE_BIT_MASK_32(0)))  /**< query tag from nav ci application  */
#define CI_REG_EVENT_QUERY_LAST        ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(23)))  /**< last index, not used  */


/*------------------------------------------------------------------*/
/*! @struct CI_MMI_ENQ_T
 *  @brief This structure contains the MMI enquiry object information. 
 *  @code
 *  typedef struct _CI_MMI_ENQ_T
 *  {
 *      UINT32      ui4_id;
 *      UINT8       ui1_ans_txt_len;
 *      BOOL        b_blind_ans;
 *      UTF16_T*    w2s_text;
 *  } CI_MMI_ENQ_T;
 *  @endcode
 *  @li@c  ui4_id                                            -enqery object  id that will be used in CI_MMI_ENQ_ANS_T
 *  @li@c  ui1_ans_txt_len                                   -expected length of the answer. Set to hex 'FF' if unknown by the Application.
 *  @li@c  b_blind_ans                                       -set to 1 means that the user input has not to be displayed when entered. The host has the
*                                                                       choice of the replacement character used (star, ...).
 *  @li@c  w2s_text                                          -Text information is coded using the character sets and methods described in [4].
 */
/*------------------------------------------------------------------*/
typedef struct _CI_MMI_ENQ_T
{
    UINT32      ui4_id;
    UINT8       ui1_ans_txt_len;
    BOOL        b_blind_ans;
    UTF16_T*    w2s_text;
} CI_MMI_ENQ_T;

/*------------------------------------------------------------------*/
/*! @struct CI_MMI_ENQ_ANS_T
 *  @brief This structure contains the answer information of MMI enquiry object. 
 *  @code
 *  typedef struct _CI_MMI_ENQ_ANS_T
 *  {
 *      UINT32      ui4_id;
 *      BOOL        b_answer;
 *      UTF16_T*    w2s_text;
 *  } CI_MMI_ENQ_ANS_T;
 *  @endcode
 *  @li@c  ui4_id                                            -indicate answer which Enq object got before
 *  @li@c  b_answer                                          -true means that the object contains the user input (which may be of zero length).
 *                                                                      false means that the user wishes to abort the dialogue.
 *  @li@c  w2s_text                                          -answer content which shall be coded using the same character coding scheme as that used its associated Enq object.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_MMI_ENQ_ANS_T
{
    UINT32      ui4_id;
    BOOL        b_answer;
    UTF16_T*    w2s_text;
} CI_MMI_ENQ_ANS_T;

/*------------------------------------------------------------------*/
/*! @struct CI_MMI_ENQ_CLOSE_T
 *  @brief This structure contains the answer information of MMI enquiry close object. 
 *  @code
 *  typedef struct _CI_MMI_ENQ_CLOSE_T
 *  {
 *      UINT8       ui1_ci_slot;
 *      UINT8       ui1_mmi_close_delay;
 *  } CI_MMI_ENQ_CLOSE_T;
 *  @endcode
 *  @li@c  ui1_ci_slot                                       -slot id, value range[0,slot num-1]
 *  @li@c  ui1_mmi_close_delay                               -MMI close delay time,0 means close immediately
 */
/*------------------------------------------------------------------*/
typedef struct _CI_MMI_ENQ_CLOSE_T
{
    UINT8       ui1_ci_slot;
    UINT8       ui1_mmi_close_delay;
} CI_MMI_ENQ_CLOSE_T;

/*------------------------------------------------------------------*/
/*! @struct  CI_MMI_MENU_ITEM_T
 *  @brief his structure contains the MMI menu item information. 
 *  @code
 *  typedef struct _CI_MMI_MENU_ITEM_T
 *  {
 *      SLIST_ENTRY_T(_CI_MMI_MENU_ITEM_T)  t_link;
 *      UTF16_T*                            w2s_item;
 *  } CI_MMI_MENU_ITEM_T;
 *  @endcode
 *  @li@c  t_link                                            -next menu item pointer
 *  @li@c  w2s_item                                          -menu item content, max length is 256
 */
/*------------------------------------------------------------------*/
typedef struct _CI_MMI_MENU_ITEM_T
{
    SLIST_ENTRY_T(_CI_MMI_MENU_ITEM_T)  t_link; 
    UTF16_T*                            w2s_item;
} CI_MMI_MENU_ITEM_T;

/* This structure contains the MMI menu or list object information. */
/*------------------------------------------------------------------*/
/*! @struct CI_MMI_MENU_T
 *  @brief This structure contains the MMI menu or list object information.
 *  @code
 *  typedef struct _CI_MMI_MENU_T
 *  {
 *      UINT32              ui4_id;
 *      UTF16_T*            w2s_title;
 *      UTF16_T*            w2s_subtitle;
 *      UTF16_T*            w2s_bottom;
 *      UINT8               ui1_choice_nb;
 *      CI_MMI_MENU_ITEM_LIST_T  t_list;
 *  } CI_MMI_MENU_T;
 *  @endcode
 *  @li@c  ui4_id                                            -menu/list id
 *  @li@c  w2s_title                                         -title content,max length is 256
 *  @li@c  w2s_subtitle                                      -subtitle content,max length is 256
 *  @li@c  w2s_bottom                                        -bottom content,max length is 256
 *  @li@c  ui1_choice_nb                                     -choice number,'FF' means that this field does not carry the number of choices information.
 *  @li@c  t_list                                            -menu item list 
 */
/*------------------------------------------------------------------*/
typedef struct _CI_MMI_MENU_T
{
    UINT32              ui4_id;
    UTF16_T*            w2s_title;
    UTF16_T*            w2s_subtitle;
    UTF16_T*            w2s_bottom;
    UINT8               ui1_choice_nb;
    #ifdef ANDROID
    UTF16_T*            t_list[MAX_MENU_ITEMS];
    #else
    CI_MMI_MENU_ITEM_LIST_T  t_list;
    #endif
} CI_MMI_MENU_T;

/* This structure contains the answer information of MMI menu object. */
/*------------------------------------------------------------------*/
/*! @struct CI_MMI_MENU_ANS_T
 *  @brief This structure contains the answer information of MMI menu object.
 *  @code
 *  typedef struct _CI_MMI_MENU_ANS_T
 *  {
 *      UINT32              ui4_id;
 *      UINT8               ui1_answer;
 *  } CI_MMI_MENU_ANS_T;
 *  @endcode
 *  @li@c  ui4_id                                            -menu id used to indicate which menu object to answer.
 *  @li@c  ui1_answer                                    -the number of the choice selected by the user. 
 *                                                                  01 corresponds to the first choice that had been presented by the application in that object 
 *                                                                  02 corresponds to the second choice text presented by the application.
 *                                                                  00 indicates that the user has cancelled the preceding menu or list object without making a choice.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_MMI_MENU_ANS_T
{
    UINT32              ui4_id;
    UINT8               ui1_answer;
} CI_MMI_MENU_ANS_T;

/*------------------------------------------------------------------*/
/*! @struct CI_SLOT_INFO_T
 *  @brief This structure contains the slot information. 
 *  @code
 *  typedef struct _CI_SLOT_INFO_T
 *  {
 *      BOOL                b_active;
 *      UINT32              ui4_app_id;
 *  } CI_SLOT_INFO_T;
 *  @endcode
 *  @li@c  b_active                                          -true means CAM is plugged in the slot and CAM's name is ready,otherwise false
 *  @li@c  ui4_app_id                                        -slot id, value range[0,slot num-1].
 */
/*------------------------------------------------------------------*/
typedef struct _CI_SLOT_INFO_T
{
    BOOL                b_active;
    UINT32              ui4_app_id;
} CI_SLOT_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct CI_HC_TUNE_T
 *  @brief This structure contains the Tune object information.
 *  @code
 *  typedef struct _CI_HC_TUNE_T
 *  {
 *      UINT32              ui4_id;
 *      UINT16              ui2_network_id;
 *      UINT16              ui2_orig_network_id;
 *      UINT16              ui2_ts_id;
 *      UINT16              ui2_service_id;
 *  } CI_HC_TUNE_T;
 *  @endcode
 *  @li@c  ui4_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  ui2_network_id                               -Network ID
 *  @li@c  ui2_orig_network_id                        -Original Network ID
 *  @li@c  ui2_ts_id                                         -Transport Stream ID
 *  @li@c  ui2_service_id                                 -Service ID
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HC_TUNE_T
{
    UINT32              ui4_id;
    UINT16              ui2_network_id;
    UINT16              ui2_orig_network_id;
    UINT16              ui2_ts_id;
    UINT16              ui2_service_id;
} CI_HC_TUNE_T;

/*------------------------------------------------------------------*/
/*! @struct CI_HC_REPLACE_T
 *  @brief This structure contains the replace object information
 *  @code
 *  typedef struct _CI_HC_REPLACE_T
 *  {
 *      UINT32              ui4_id;
 *      UINT8               ui1_ref;
 *      UINT16              ui2_replaced_pid;
 *      UINT16              ui2_replacement_pid;
 *  } CI_HC_REPLACE_T;
 *  @endcode
 *  @li@c  ui4_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  ui1_ref                                           -replace object id,which is used to match a Clear Replace object with one or more previous Replace objects.
 *                                                                   Several Replace objects can use the same value of ui1_ref.
 *  @li@c  ui2_replaced_pid                                  -Stream PID: video, audio, teletext or subtitles,
                                                                           replaced by the component being transmitted in the ui2_replacement_pid
 *  @li@c  ui2_replacement_pid                               -replacement stream PID.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HC_REPLACE_T
{
    UINT32              ui4_id;
    UINT8               ui1_ref;
    UINT16              ui2_replaced_pid;
    UINT16              ui2_replacement_pid;
} CI_HC_REPLACE_T;

/*------------------------------------------------------------------*/
/*! @struct CI_HC_CLR_REPLACE_T
 *  @brief This structure contains the clear replace object information 
 *  @code
 *  typedef struct _CI_HC_CLR_REPLACE_T
 *  {
 *      UINT32              ui4_id;
 *      UINT8               ui1_ref;
 *  } CI_HC_CLR_REPLACE_T;
 *  @endcode
 *  @li@c  ui4_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  ui1_ref                                           -replace object id which will be cleared
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HC_CLR_REPLACE_T
{
    UINT32              ui4_id;
    UINT8               ui1_ref;
} CI_HC_CLR_REPLACE_T;

/*------------------------------------------------------------------*/
/*! @struct CI_CA_SYSTEM_ID_MATCH_T
 *  @brief This structure contains the ca system id match information. 
 *  @code
 *  typedef struct _CI_CA_SYSTEM_ID_MATCH_T
 *  {
 *      UINT8 ui1_slot_id;
 *      BOOL b_match;
 *  } CI_CA_SYSTEM_ID_MATCH_T;
 *  @endcode
 *  @li@c  ui1_slot_id                                       -slot id, value range[0,slot num-1].
 *  @li@c  b_match                                           -ture if ca system id in CAM match pmt's
 */
/*------------------------------------------------------------------*/
typedef struct _CI_CA_SYSTEM_ID_MATCH_T
{
    UINT8    ui1_slot_id;
    BOOL     b_match;
} CI_CA_SYSTEM_ID_MATCH_T;

/*------------------------------------------------------------------*/
/*! @struct CI_CA_SYSTEM_ID_STATUS_T
 *  @brief This structure contains current ca resouce status. 
 *  @code
 *  typedef struct _CI_CA_SYSTEM_ID_STATUS_T
 *  {
 *      UINT8 ui1_slot_id;
 *      CA_SYSTEM_ID_STATUS_T e_ca_sys_id_status;
 *  } CI_CA_SYSTEM_ID_STATUS_T;
 *  @endcode
 *  @li@c  ui1_slot_id                           -slot id, value range[0,slot num-1].
 *  @li@c  e_ca_sys_id_status                    -current ca resouce status
 */
/*------------------------------------------------------------------*/
typedef struct _CI_CA_SYSTEM_ID_STATUS_T
{
    UINT8    ui1_slot_id;
    CA_SYSTEM_ID_STATUS_T e_ca_sys_id_status;
} CI_CA_SYSTEM_ID_STATUS_T;

#ifdef CI_PLUS_SUPPORT
/*------------------------------------------------------------------*/
/*! @struct CI_CAM_UPGRADE_T
 *  @brief This structure contains the cam upgrade message. 
 *  @code
 *  typedef struct _CI_CAM_UPGRADE_T
 *{
 *   UINT8 ui1_slot_id;
 *   UINT8 ui1_upgrade_type;
 *   UINT16 ui2_download_time;
 * } CI_CAM_UPGRADE_T;
 *  @endcode
 *  @li@c  ui1_slot_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  ui1_upgrade_type                                  -this parameter identifies the type of CAM firmware upgrade requested:
 *                                                                          0x00: Delayed Upgrade mode
 *                                                                          0x01: Immediate Upgrade mode
 *  @li@c  ui2_download_time                                -The time in seconds, estimated to complete the firmware upgrade process. If the value is 0x0000 then the duration is unknown.
 */
/*------------------------------------------------------------------*/

typedef struct _CI_CAM_UPGRADE_T
{
    UINT8 ui1_slot_id;
    UINT8 ui1_upgrade_type;
    UINT16 ui2_download_time;
} CI_CAM_UPGRADE_T;

/*------------------------------------------------------------------*/
/*! @struct CI_CAM_UPGRADE_PROGRESS_T
 *  @brief This structure contains the cam upgrade progress message.
 *  @code
 * typedef struct _CI_CAM_UPGRADE_PROGRESS_T
 * {
 *    UINT8 ui1_slot_id;
 *    UINT8 ui1_download_progress_status;
 * } CI_CAM_UPGRADE_PROGRESS_T;
 *  @endcode
 *  @li@c  ui1_slot_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  ui1_download_progress_status                -The percentage value of the CAM upgrade progress,in the range 0 to 100.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_CAM_UPGRADE_PROGRESS_T
{
    UINT8 ui1_slot_id;
    UINT8 ui1_download_progress_status;
} CI_CAM_UPGRADE_PROGRESS_T;

/*------------------------------------------------------------------*/
/*! @struct CI_NFY_ERROR_DATA_T
 *  @brief This structure contains the error notification related data.
 *  @code
 *typedef struct _CI_NFY_ERROR_DATA_T
 *{
 *   UINT8 ui1_slot_id;
 *   CI_NFY_ERROR_CODE_T     e_nfy_err_code;
 *}CI_NFY_ERROR_DATA_T;
 *  @endcode
 *  @li@c  ui1_slot_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  e_nfy_err_code                -error code.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_NFY_ERROR_DATA_T
{
    UINT8 ui1_slot_id;
    CI_NFY_ERROR_CODE_T     e_nfy_err_code;
}CI_NFY_ERROR_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct CI_CAM_UPGRADE_PROGRESS_REPLY_T
 *  @brief This structure contains the answer information of MMI menu object. 
 *  @code
 *typedef struct _CI_CAM_UPGRADE_PROGRESS_REPLY_T
 *{
 *   UINT32              ui4_id;
 *   UINT8               ui1_reply;
 *} CI_CAM_UPGRADE_PROGRESS_REPLY_T;
 *  @endcode
 *  @li@c  ui1_slot_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  e_nfy_err_code                -0x00:means NO,0x01:means YES,0x02:means ASK the user
 */
/*------------------------------------------------------------------*/
typedef struct _CI_CAM_UPGRADE_PROGRESS_REPLY_T
{
    UINT32              ui4_id;
    UINT8               ui1_reply;
} CI_CAM_UPGRADE_PROGRESS_REPLY_T;

/*------------------------------------------------------------------*/
/*! @struct CI_HOST_COUNTRY_T
 *  @brief This structure contains the host country message.
 *  @code
 *typedef struct _CI_HOST_COUNTRY_T
 *{
 *   UINT8 ui1_slot_id;
 *   ISO_3166_COUNT_T    s3166_country_code;
 *} CI_HOST_COUNTRY_T;
 *  @endcode
 *  @li@c  ui1_slot_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  s3166_country_code                -define country code
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HOST_COUNTRY_T
{
    UINT8 ui1_slot_id;
    ISO_3166_COUNT_T    s3166_country_code;
} CI_HOST_COUNTRY_T;

/*------------------------------------------------------------------*/
/*! @struct CI_HOST_LANGUAGE_T
 *  @brief This structure contains the host language message.
 *  @code
 *typedef struct _CI_HOST_LANGUAGE_T
 *{
 *   UINT8 ui1_slot_id;
 *   ISO_639_LANG_T      s639_lang_code;
 * } CI_HOST_LANGUAGE_T;
 *  @endcode
 *  @li@c  ui1_slot_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  s639_lang_code                -define host language code
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HOST_LANGUAGE_T
{
    UINT8 ui1_slot_id;
    ISO_639_LANG_T      s639_lang_code;
} CI_HOST_LANGUAGE_T;

/*------------------------------------------------------------------*/
/*! @struct CI_APP_DOMAIN_ID_T
 *  @brief This structure defines the application identifier value sent from RequestStart. 
 *  @code
 * typedef struct _CI_APP_DOMAIN_ID_T
 *{
 *    UINT8 *pu1_appDomainID;
 *    UINT8 ui1_ssm; 
 * } CI_APP_DOMAIN_ID_T ; 
 *  @endcode
 *  @li@c  pu1_appDomainID      -is a null-terminated string. It should be "CIMHEGP1".
 *  @li@c  ui1_ssm                     -Refer to ci plus spec Table12.8 to handle
 */
/*------------------------------------------------------------------*/
typedef struct _CI_APP_DOMAIN_ID_T
{
    UINT8 *pu1_appDomainID;
    UINT8 ui1_ssm; 
} CI_APP_DOMAIN_ID_T ;


/*------------------------------------------------------------------*/
/*! @struct CI_REQUEST_START_T
 *  @brief This structure defines App Start Request message 
 *  @code
 * typedef struct _CI_REQUEST_START_T
 *{
 *   UINT8 *filename;
 *   CI_APP_DOMAIN_ID_T t_appDomainID ;
* } CI_REQUEST_START_T;
 *  @endcode
 *  @li@c  filename      -is a null-terminated string. 
 *  @li@c  t_appDomainID                     -contains app domain id
 */
/*------------------------------------------------------------------*/
typedef struct _CI_REQUEST_START_T
{
    UINT8 *filename;
    CI_APP_DOMAIN_ID_T t_appDomainID ;
} CI_REQUEST_START_T;


/*------------------------------------------------------------------*/
/*! @struct CI_REQUEST_START_ACK_T
 *  @brief This structure defines App Start Request ackowledge message, used in c_ci_set() with set type: CI_SET_TYPE_REQUEST_START_ACK
 *  @code
 *typedef struct _CI_REQUEST_START_ACK_T
 *{
 *   CI_REQUEST_START_ACK_CODE_T  t_ack_code;
 *} CI_REQUEST_START_ACK_T;
 *  @endcode
 *  @li@c  t_ack_code      -contains start app request ackowledge code. 
 */
/*------------------------------------------------------------------*/
typedef struct _CI_REQUEST_START_ACK_T
{
    CI_REQUEST_START_ACK_CODE_T  t_ack_code;
} CI_REQUEST_START_ACK_T;

/*------------------------------------------------------------------*/
/*! @struct CI_ABORT_REQUEST_T
 *  @brief This structure defines app abort request message,set data by CI_SET_TYPE_ABORT_REQUEST or CI_NFY_COND_APP_ABORT_REQUEST.
 *  @code
 *typedef struct _CI_ABORT_REQUEST_T
 *{
 *   CI_ABORT_REQUEST_CODE_T t_abort_code;
 *} CI_ABORT_REQUEST_T;
 *  @endcode
 *  @li@c  t_abort_code      -contains abort app request code. 
 */
/*------------------------------------------------------------------*/
typedef struct _CI_ABORT_REQUEST_T
{
    CI_ABORT_REQUEST_CODE_T t_abort_code;
} CI_ABORT_REQUEST_T;

/*------------------------------------------------------------------*/
/*!@struct CI_ABORT_ARC_T
 *  @brief This structure contains abort ack code. It is used to indicate that the MHEG5 CI+ application is stopped successfully.
 *typedef struct _CI_ABORT_ACK_T
 *{
 *   CI_ABORT_ACK_CODE_T t_abort_ack_code;
 *} CI_ABORT_ARC_T;
 *  @endcode
 *  @li@c  t_abort_ack_code      -contains abort app request ackowledge code. 
 */
/*------------------------------------------------------------------*/
typedef struct _CI_ABORT_ACK_T
{
    CI_ABORT_ACK_CODE_T t_abort_ack_code;
} CI_ABORT_ARC_T;


/*------------------------------------------------------------------*/
/*!@struct CI_FILE_REQUEST_T
 *  @brief This structure defines the data sent from RequestFile.
 * typedef struct _CI_FILE_REQUEST_T
 *{
 *    UINT8 *pu1fileReq;
 *    UINT32 ui4_length; 
 *} CI_FILE_REQUEST_T;
 
 *  @endcode
 *  @li@c  pu1fileReq      -contains request data buffer pointer.  
 *  @li@c  ui4_length      -contains request data length 
 */
/*------------------------------------------------------------------*/
typedef struct _CI_FILE_REQUEST_T
{
    UINT8 *pu1fileReq;
    UINT32 ui4_length; 
} CI_FILE_REQUEST_T;

/*------------------------------------------------------------------*/
/*!@struct CI_FILE_ACK_T
 *  @brief This structure defines the data sent from RequestFileAck.
 * typedef struct _CI_FILE_ACK_T
 *{
 *   BOOL b_file_ok;
 *   UINT8 *pu1fileAck;
 *   UINT32 ui4_length; 
 *} CI_FILE_ACK_T;
 
 *  @endcode
 *  @li@c  b_file_ok      -A 1 bit field is set to "1" if the file is available or this is an acknowledgement response to a FileRequest
 *                                 message with a RequestType of data, otherwise it shall be "0". 
 *  @li@c  pu1fileReq      -contains request ack data  buffer pointer.  
 *  @li@c  ui4_length      -contains request ack data length 
 */
/*------------------------------------------------------------------*/
typedef struct _CI_FILE_ACK_T
{
    BOOL b_file_ok;
    UINT8 *pu1fileAck;
    UINT32 ui4_length; 
} CI_FILE_ACK_T;


/*------------------------------------------------------------------*/
/*!@struct CI_FREE_ACK_DATA_T
 *  @brief This structure defines set data by CI_SET_TYPE_FREE_ACK_DATA.
 * typedef struct _CI_FREE_ACK_DATA_T
 *{
 *   UINT8 *pu1fileAck;
 *} CI_FREE_ACK_DATA_T; 
 *  @endcode
 *  @li@c  pu1fileAck      -contains request ack data  buffer pointer to be free.  If is NULL then means free all request ack data buffers.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_FREE_ACK_DATA_T
{
    UINT8 *pu1fileAck;
} CI_FREE_ACK_DATA_T;

/*------------------------------------------------------------------*/
/*!@struct CI_SEARCH_MODE_FULL_T
 *  @brief This structure defines full file name search mode data.
 * typedef struct _CI_SEARCH_MODE_FULL_T
 *{
 *  UINT16  ui2_filename_len;
 *   CHAR    *pc_filename;
 *} CI_SEARCH_MODE_FULL_T;
 *  @endcode
 *  @li@c  ui2_filename_len      -contains file full name's length.
 *  @li@c  pc_filename      -contains file full name string.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_SEARCH_MODE_FULL_T
{
    UINT16  ui2_filename_len;
    CHAR    *pc_filename;
} CI_SEARCH_MODE_FULL_T;


/*------------------------------------------------------------------*/
/*!@struct CI_SEARCH_MODE_PREFIX_EXT_T
 *  @brief This structure defines prefix and ext file name search mode data.
 * typedef struct _CI_SEARCH_MODE_PREFIX_EXT_T
 *{
 *   UINT16  ui2_prefix_len;
 *   CHAR    *pc_prefix;
 *   UINT16  ui2_extension_len;    
 *   CHAR    *pc_extension;
 *} CI_SEARCH_MODE_PREFIX_EXT_T;
 *  @endcode
 *  @li@c  ui2_prefix_len      -contains prefix of file name length.
 *  @li@c  pc_prefix      -contains prefix string.
  *  @li@c  ui2_extension_len      -contains file name extention name length.
 *  @li@c  pc_extension      -contains file extention name.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_SEARCH_MODE_PREFIX_EXT_T
{
    UINT16  ui2_prefix_len;
    CHAR    *pc_prefix;
    UINT16  ui2_extension_len;    
    CHAR    *pc_extension;
} CI_SEARCH_MODE_PREFIX_EXT_T;


/*------------------------------------------------------------------*/
/*!@struct CI_SEARCH_MODE_PREFIX_T
 *  @brief This structure defines  file prefix name search mode data.
 * typedef struct _CI_SEARCH_MODE_PREFIX_T
 *{
 *   UINT16  ui2_prefix_len;
 *   CHAR    *pc_prefix;
 *} CI_SEARCH_MODE_PREFIX_T;
 *  @endcode
 *  @li@c  ui2_prefix_len      -contains prefix of file name length.
 *  @li@c  pc_prefix      -contains prefix string.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_SEARCH_MODE_PREFIX_T
{
    UINT16  ui2_prefix_len;
    CHAR    *pc_prefix;
} CI_SEARCH_MODE_PREFIX_T;

/*------------------------------------------------------------------*/
/*!@struct CI_SEARCH_MODE_EXT_T
 *  @brief This structure defines  file extention name search mode data.
 * typedef struct _CI_SEARCH_MODE_EXT_T
 *{
 *   UINT16  ui2_extension_len;
 *    CHAR    *pc_extension;
 *} CI_SEARCH_MODE_EXT_T;
 *  @endcode
  *  @li@c  ui2_extension_len      -contains file name extention name length.
 *  @li@c  pc_extension      -contains file extention name.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_SEARCH_MODE_EXT_T
{
    UINT16  ui2_extension_len;
    CHAR    *pc_extension;
} CI_SEARCH_MODE_EXT_T;

/*------------------------------------------------------------------*/
/*!@struct CI_SEARCH_MODE_DATA_T
 *  @brief This structure defines file search mode and its return data to update data from USB.
 * typedef struct _CI_SEARCH_MODE_DATA_T
 * {
 *   CI_FILE_SEARCH_MODE_T t_search_mode;
 *   union
 *   {
 *       CI_SEARCH_MODE_FULL_T          t_fullname;
 *       CI_SEARCH_MODE_PREFIX_EXT_T    t_prefix_ext;
 *       CI_SEARCH_MODE_PREFIX_T        t_prefix;
 *       CI_SEARCH_MODE_EXT_T           t_ext;            
 *   }u_para;
 * } CI_SEARCH_MODE_DATA_T;
 *  @endcode
  *  @li@c  t_search_mode      -contains file search mode.
 *  @li@c  t_fullname      -contains full file name search data.
 *  @li@c  t_prefix_ext      -contains prefix and extention  file name search data.
 *  @li@c  t_prefix      -contains  file prefix name search data.
 *  @li@c  t_ext      -contains file extention name search data.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_SEARCH_MODE_DATA_T
{
    CI_FILE_SEARCH_MODE_T t_search_mode;
    union
    {
        CI_SEARCH_MODE_FULL_T          t_fullname;
        CI_SEARCH_MODE_PREFIX_EXT_T    t_prefix_ext;
        CI_SEARCH_MODE_PREFIX_T        t_prefix;
        CI_SEARCH_MODE_EXT_T           t_ext;            
    }u_para;
} CI_SEARCH_MODE_DATA_T;


/*------------------------------------------------------------------*/
/*!@struct CI_PLUS_CRED_INFO_T
 *  @brief This structure defines ci plus credential infomation data.
 *typedef struct _CI_PLUS_CRED_INFO_T
 *{
 *   BOOL    b_valid;                
 *   UINT16  ui2_custom_code_len;    
 *   CHAR    *pc_custom_code;        
 *   UINT16  ui2_serial_num_len;    
 *   CHAR    *pc_serial_num;        
 *   UINT8   ui1_device_id_len;     
 *   CHAR    ac_device_id[16];       
 * } CI_PLUS_CRED_INFO_T;
 *  @endcode
  *  @li@c  b_valid      -contains the flag it is true if valid.
 *  @li@c  ui2_custom_code_len      -contains length of custom code .
 *  @li@c  pc_custom_code      -contains pointer of custom code buffer .
 *  @li@c  ui2_serial_num_len      -contains length of serial number, it is addition info for remember the filename when key is input through USB .
 *  @li@c  pc_serial_num      -contains pointer of serial number.
 *  @li@c  ui1_device_id_len      -contains length of device id.
 *  @li@c  ac_device_id      -contains pointer of device id .
 */
/*------------------------------------------------------------------*/
typedef struct _CI_PLUS_CRED_INFO_T
{
    BOOL    b_valid;                
    UINT16  ui2_custom_code_len;    
    CHAR    *pc_custom_code;        
    UINT16  ui2_serial_num_len;    
    CHAR    *pc_serial_num;        
    UINT8   ui1_device_id_len;     
    CHAR    ac_device_id[16];       
} CI_PLUS_CRED_INFO_T;
#endif
/*------------------------------------------------------------------*/
/*! @brief This notify function is called by the CI Abstraction Layer to notify 
 * the status or condition change.
 *  @param [in] h_ci   -handle returned in c_ci_reg_nfy()
 *  @param [in] pv_nfy_tag   -private data set  in c_ci_reg_nfy()
 *  @param [in] e_nfy_cond   -notify condition
 *  @param [in] pv_data   -notify info,concrete data type is related to CI_NFY_COND_T e_nfy_cond
 *  @param [out] NONE
 *  @note
 *  @see c_ci_reg_nfy(),CI_NFY_COND_T
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_ci_nfy_fct)(
    HANDLE_T            h_ci,
    VOID*               pv_nfy_tag,
    CI_NFY_COND_T       e_nfy_cond,
    VOID*               pv_data);
#ifdef CI_PLUS_SUPPORT

/*------------------------------------------------------------------*/
/*! @brief This query function is called by the CI Abstraction Layer to query 
 *  the data of system
 *  @param [in] h_ci   -handle returned in c_ci_reg_query_nfy()
 *  @param [in] pv_tag   -private data set  in c_ci_reg_query_nfy()
 *  @param [in] e_query_type   -query type
 *  @param[in] [out] pv_data   -query data buffer,concrete data type is related to CI_QUERY_TYPE_T e_query_type
 *  @param [in][out] pz_data_len - query data len
 *  @note
 *  @see c_ci_reg_query_nfy(),CI_QUERY_TYPE_T
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_ci_query_info_fct)(
    HANDLE_T            h_ci,
    VOID*               pv_tag,
    CI_QUERY_TYPE_T     e_query_type,
    VOID*               pv_data,
    SIZE_T*             pz_data_len);

/******************************************************************************
structures  between MHEG5 module and CI module
********************************************************************************/
/*------------------------------------------------------------------*/
/*! @enum CI_MHEG5_NFY_COND_T
 *  @brief  specifies MHEG5 Notify error code 
 *  @code
 *  typedef enum
 *{
 *   CI_NFY_COND_FILE_ACK
 *} CI_MHEG5_NFY_COND_T;
 *  @endcode
 *  @li@c  CI_NFY_COND_FILE_ACK            -  mheg5 notify file acklowdge
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CI_NFY_COND_FILE_ACK
} CI_MHEG5_NFY_COND_T;


/*------------------------------------------------------------------*/
/*! @enum CI_MHEG5_SET_TYPE
 *  @brief  specifies mheg5 set type message
 *  @code
 * typedef enum
 *{
 *   CI_MHEG5_SET_TYPE_FILE_REQUEST,
 *   CI_MHEG5_SET_TYPE_FREE_ACK_DATA
 *} CI_MHEG5_SET_TYPE ;
 
 *  @endcode
 *  @li@c  CI_MHEG5_SET_TYPE_FILE_REQUEST                    - Indicate file request set type
 *  @li@c  CI_MHEG5_SET_TYPE_FREE_ACK_DATA                - Indicate free file data acknowledge set type
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CI_MHEG5_SET_TYPE_FILE_REQUEST,
    CI_MHEG5_SET_TYPE_FREE_ACK_DATA
} CI_MHEG5_SET_TYPE ;

/*------------------------------------------------------------------*/
/*! @brief  The CI engine will use this API to notify information to AP. Argument e_nfy_cond indicates the type of notification. 
 *  @param [in] h_ci   -handle returned in c_ci_reg_query_nfy()
 *  @param [in] pv_nfy_tag   -private data set  in x_ci_mheg5_reg_nfy()
 *  @param [in] e_nfy_cond   -notify condition
 *  @param[in] [out] pv_data   -pv_data data set in x_ci_mheg5_set
 *  @note
 *  @see x_ci_mheg5_reg_nfy(),CI_MHEG5_NFY_COND_T
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_ci_mheg5_nfy_fct)(   
    HANDLE_T            h_ci,
    VOID*               pv_nfy_tag,
    CI_MHEG5_NFY_COND_T e_nfy_cond,
    VOID*               pv_data);
#endif
#endif /* _U_CI_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_CI*/
/*----------------------------------------------------------------------------*/
