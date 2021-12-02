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
/*! @file u_rec_hdlr.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Record Handler specific definitions.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_REC_HDLR_H_
#define _U_REC_HDLR_H_

/*----------------------------------------------------------------------------*/
/*! @defgroup   groupMW_SVCTX_REC_HDLR Record Handler
 *  @ingroup    groupMW_SVCTX
 *  @brief      Provides external definitions for record handler.
 *  @see        groupMW_SVCTX
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_svctx.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define SRC_TYPE_RECORD                    ((SRC_TYPE_T) 16)        /**< Defines the source type for REC_HDLR */

/* return value */
#define SVCTXR_REC_MSG_LAST_ENTRY   (SVCTXR_MSG_LAST_ENTRY   + ((INT32) 32)) /**< Defines the last entry of message type return value */

#define SVCTXR_REC_INTERNAL_ERROR    (SVCTXR_ERROR_LAST_ENTRY - ((INT32)  1)) /**< Defines the return value that indicates internal error */
#define SVCTXR_REC_FILE_NOT_FOUND    (SVCTXR_ERROR_LAST_ENTRY - ((INT32)  2)) /**< Defines the return value that indicates the file is not found. */
#define SVCTXR_REC_INV_META_KEY_TYPE (SVCTXR_ERROR_LAST_ENTRY - ((INT32)  3)) /**< Defines the return value that indicates the metadata key type is invalid. */
#define SVCTXR_REC_BUFF_TOO_SMALL    (SVCTXR_ERROR_LAST_ENTRY - ((INT32)  4)) /**< Defines the return value that indicates the buffer prepared by caller is too small. */
#define SVCTXR_REC_INFO_NOT_FOUND    (SVCTXR_ERROR_LAST_ENTRY - ((INT32)  5)) /**< Defines the return value that indicates the requested info is not found. */
#define SVCTXR_REC_EXTNL_MODULE_FAIL (SVCTXR_ERROR_LAST_ENTRY - ((INT32)  6)) /**< Defines the return value that indicates some operation of external module is fail. */

#define SVCTXR_REC_ERR_LAST_ENTRY   (SVCTXR_ERROR_LAST_ENTRY - ((INT32) 32)) /**< Defines the last entry of error type return value */


#define SVCTX_GRP_REC                      (5<<SVCTX_TYPE_CODE_BITS)    /**< Defines the set/get type group for REC_HDLR */

/* set commands */
#define SVCTX_REC_SET_TYPE_IGNORE          ((SVCTX_SET_TYPE_T) (SVCTX_GRP_REC | 0))  /**< (Ignore) */
#define SVCTX_REC_SET_TYPE_START_REC       ((SVCTX_SET_TYPE_T) (SVCTX_GRP_REC | 1))  /**< Defines the set type to start record, pv_set_info: (SVCTX_REC_CONFIG_T *) */
#define SVCTX_REC_SET_TYPE_STOP_REC        ((SVCTX_SET_TYPE_T) (SVCTX_GRP_REC | 2))  /**< Defines the set type to stop record, pv_set_info: NULL */
#define SVCTX_REC_SET_TYPE_PAUSE_REC       ((SVCTX_SET_TYPE_T) (SVCTX_GRP_REC | 3))  /**< Defines the set type to pause record, pv_set_info: NULL */
#define SVCTX_REC_SET_TYPE_RESUME_REC      ((SVCTX_SET_TYPE_T) (SVCTX_GRP_REC | 4))  /**< Defines the set type to resume record, pv_set_info: NULL */
#define SVCTX_REC_SET_TYPE_ES_LIST         ((SVCTX_SET_TYPE_T) (SVCTX_GRP_REC | 5))  /**< Defines the set type to set ES list, pv_set_info: (SVCTX_REC_ES_LIST_T *) */
#define SVCTX_REC_SET_TYPE_DEL_REC_DATA    ((SVCTX_SET_TYPE_T) (SVCTX_GRP_REC | 6))  /**< Defines the sst type to delete a specified set of record data, pv_set_info: (SVCTX_REC_DEL_DATA_T *) */
#define SVCTX_REC_SET_TYPE_META_DB_OPEN    ((SVCTX_SET_TYPE_T) (SVCTX_GRP_REC | 7))  /**< Defines the command to open metadata database, pv_set_info: (SVCTX_REC_META_DB_OPEN_T *) */
#define SVCTX_REC_SET_TYPE_META_DB_CLOSE   ((SVCTX_SET_TYPE_T) (SVCTX_GRP_REC | 8))  /**< Defines the command to close metadata database, pv_set_info: (SVCTX_REC_META_DB_CLOSE_T *) */
#define SVCTX_REC_SET_TYPE_META_DB_STORE   ((SVCTX_SET_TYPE_T) (SVCTX_GRP_REC | 9))  /**< Defines the command to store metadata database, pv_set_info: (SVCTX_REC_META_DB_STORE_T *) */
#define SVCTX_REC_SET_TYPE_META_INFO       ((SVCTX_SET_TYPE_T) (SVCTX_GRP_REC | 10))  /**< Defines the command to set an entry of metadata database. Note that the modification will not be physically stored into file before SVCTX_REC_SET_TYPE_META_DB_STORE or SVCTX_REC_SET_TYPE_META_DB_CLOSE is called, pv_set_info: (SVCTX_REC_SET_META_INFO_T *) */

/* get commands */
#define SVCTX_REC_GET_TYPE_IGNORE          ((SVCTX_GET_TYPE_T) (SVCTX_GRP_REC | 0))  /**< (Ignore) */
#define SVCTX_REC_GET_TYPE_TICK_PERIOD     ((SVCTX_GET_TYPE_T) (SVCTX_GRP_REC | 1))  /**< Defines the get type for tick period, pv_get_info: (UINT32 *) */
#define SVCTX_REC_GET_TYPE_CAPABILITY      ((SVCTX_GET_TYPE_T) (SVCTX_GRP_REC | 2))  /**< Defines the get type for capability, pv_get_info: (SVCTX_REC_CAP_INFO_T *) */
#define SVCTX_REC_GET_TYPE_SYS_RES_INFO    ((SVCTX_GET_TYPE_T) (SVCTX_GRP_REC | 3))  /**< Defines the get type for system resource info, pv_get_info: (SVCTX_REC_SYS_RES_INFO_T *) */
#define SVCTX_REC_GET_TYPE_STATUS          ((SVCTX_GET_TYPE_T) (SVCTX_GRP_REC | 4))  /**< Defines the get type for current record status, pv_get_info: (SVCTX_REC_STATUS_T *) */
#define SVCTX_REC_GET_TYPE_ESL_AUTH_STATUS ((SVCTX_GET_TYPE_T) (SVCTX_GRP_REC | 5))  /**< Defines the get type for authentication status of each ES, pv_get_info: (SVCTX_REC_ESL_AUTH_STATUS_T *) */
#define SVCTX_REC_GET_TYPE_FILE_SIZE       ((SVCTX_GET_TYPE_T) (SVCTX_GRP_REC | 6))  /**< Defines the get type for capacity consume of device pv_get_info:UINT64 consumed data size*/
#define SVCTX_REC_GET_TYPE_META_INFO       ((SVCTX_GET_TYPE_T) (SVCTX_GRP_REC | 7))  /**< Defines the command to get the data of a specified entry in metadata database, pv_get_info: (SVCTX_REC_GET_META_INFO_T *) */
#define SVCTX_REC_GET_TYPE_META_INFO_LEN   ((SVCTX_GET_TYPE_T) (SVCTX_GRP_REC | 8))  /**< Defines the command to get the length of a specified entry in metadata database, pv_get_info: (SVCTX_REC_GET_META_INFO_LEN_T *) */

/* metadata key type */
typedef UINT32 SVCTX_REC_META_KEY_TYPE_T;

/* metadata key types */
#define SVCTX_REC_META_KEY_TYPE_UNKNOWN       ((SVCTX_REC_META_KEY_TYPE_T) 0)   /**< (Unknown) */
#define SVCTX_REC_META_KEY_TYPE_CH_ID         ((SVCTX_REC_META_KEY_TYPE_T) 1)   /**< Defines the metadata key type for channel ID. Data type: (UINT32) */
#define SVCTX_REC_META_KEY_TYPE_CH_NAME       ((SVCTX_REC_META_KEY_TYPE_T) 2)   /**< Defines the metadata key type for channel name. Data type: (CHAR *) */
#define SVCTX_REC_META_KEY_TYPE_FST_EVN_TITLE ((SVCTX_REC_META_KEY_TYPE_T) 3)   /**< Defines the metadata key type for the title of the first covered event. Data type: (CHAR *) */
#define SVCTX_REC_META_KEY_TYPE_START_TIME    ((SVCTX_REC_META_KEY_TYPE_T) 4)   /**< Defines the metadata key type for record start date & time in UTC time frame. Data type: (TIME_T) */
#define SVCTX_REC_META_KEY_TYPE_DURATION      ((SVCTX_REC_META_KEY_TYPE_T) 5)   /**< Defines the metadata key type for recorded duration in seconds. Note that the entry of this key type is available only when the recording session is stopped. Data type: (UINT32) */
#define SVCTX_REC_META_KEY_TYPE_EXPIRE_TIME   ((SVCTX_REC_META_KEY_TYPE_T) 6)   /**< Defines the metadata key type for the scheduled expiration date & time (in UTC time frame) for the sake of copy protection. Data type: (TIME_T) */

#define SVCTX_REC_META_KEY_TYPE_LAST_ENTRY    ((SVCTX_REC_META_KEY_TYPE_T) 6)   /**< (Ignore) */

#define SVCTX_REC_META_KEY_TYPE_CUST_DFN      ((SVCTX_REC_META_KEY_TYPE_T) 0x00800000)  /**< Defines the start code for customer-defined key types. */


/* notification function */
/*------------------------------------------------------------------*/
/*! @brief This notify function is called by the REC_HDLR to make a 
 *         notification during recording.
 *
 *  @param [in] h_svctx_rec - Contains the handle of the record svctx.
 *  @param [in] pv_nfy_tag  - Contains the tag provided by client.
 *  @param [in] e_code      - Contains the notification code.
 *  @param [in] ui4_data    - Contains the data along with the notification code.
 *  @note  This callback should return as soon as possible.
 *  @see
 *  @return  Returns the boolean flag according to the definition of the notification code (e_code).
 *  @retval  TRUE  - The meaning relates to the notification code (e_code).
 *  @retval  FALSE - The meaning relates to the notification code (e_code).
 */
/*------------------------------------------------------------------*/
typedef BOOL (*x_svctx_rec_nfy_fct) (HANDLE_T               h_svctx_rec,
                                     VOID*                  pv_nfy_tag,
                                     SVCTX_NTFY_CODE_T      e_code,
                                     UINT32                 ui4_data);

/* record condition */
/*------------------------------------------------------------------*/
/*! @enum   SVCTX_REC_COND_T
 *  @brief  Specifies the record condition.
 *  @code
 *  typedef enum _SVCTX_REC_COND_T
 *  {
 *    SVCTX_REC_COND_STOPPED,
 *    SVCTX_REC_COND_STARTED,
 *    SVCTX_REC_COND_PAUSED,
 *    SVCTX_REC_COND_ERROR
 *  }  SVCTX_REC_COND_T;
 *  @endcode
 *  @li@c SVCTX_REC_COND_STOPPED - Specifies STOPPED state
 *  @li@c SVCTX_REC_COND_STARTED - Specifies STARTED state
 *  @li@c SVCTX_REC_COND_PAUSED  - Specifies PAUSED state
 *  @li@c SVCTX_REC_COND_ERROR   - Specifies ERROR state
 */
/*------------------------------------------------------------------*/
typedef enum _SVCTX_REC_COND_T
{
    SVCTX_REC_COND_STOPPED,
    SVCTX_REC_COND_STARTED,
    SVCTX_REC_COND_PAUSED,
    SVCTX_REC_COND_ERROR
} SVCTX_REC_COND_T;

/* record source type */
/*------------------------------------------------------------------*/
/*! @enum   SVCTX_REC_SRC_TYPE_T
 *  @brief  Specifies the record source type.
 *  @code
 *  typedef enum _SVCTX_REC_SRC_TYPE_T
 *  {
 *      SVCTX_REC_SRC_TYPE_TV,
 *      SVCTX_REC_SRC_TYPE_AVC
 *  } SVCTX_REC_SRC_TYPE_T;
 *  @endcode
 *  @li@c SVCTX_REC_SRC_TYPE_TV  - Specifies the TV source.
 *  @li@c SVCTX_REC_SRC_TYPE_AVC - Specifies the AVC source
 */
/*------------------------------------------------------------------*/ 
typedef enum _SVCTX_REC_SRC_TYPE_T
{
    SVCTX_REC_SRC_TYPE_TV,
    SVCTX_REC_SRC_TYPE_AVC
} SVCTX_REC_SRC_TYPE_T;

/* record profile type */
/*------------------------------------------------------------------*/
/*! @enum   SVCTX_REC_PROFILE_T
 *  @brief  Specifies the record profile type.
 *  @code
 *  typedef enum _SVCTX_REC_PROFILE_T
 *  {
 *      SVCTX_REC_PROFILE_SIMPLE = 0,
 *      SVCTX_REC_PROFILE_ADVANCED
 *  } SVCTX_REC_PROFILE_T;    
 *  @endcode
 *  @li@c SVCTX_REC_PROFILE_SIMPLE   - Specifies the simple profile.
 *  @li@c SVCTX_REC_PROFILE_ADVANCED - Specifies the advanced profile.
 */
/*------------------------------------------------------------------*/ 
typedef enum _SVCTX_REC_PROFILE_T
{
    SVCTX_REC_PROFILE_SIMPLE = 0,
    SVCTX_REC_PROFILE_ADVANCED
} SVCTX_REC_PROFILE_T;    

/* record buffer type */
/*------------------------------------------------------------------*/
/*! @enum   SVCTX_REC_BUF_MODE_T
 *  @brief  Specifies the record buffer mode.
 *  @code
 *  typedef enum _SVCTX_REC_BUF_MODE_T
 *  {
 *      SVCTX_REC_BUF_MODE_PLAIN,
 *      SVCTX_REC_BUF_MODE_RING
 *  } SVCTX_REC_BUF_MODE_T;
 *  @endcode
 *  @li@c SVCTX_REC_BUF_MODE_PLAIN - Specifies the plain buffer mode 
 *  @li@c SVCTX_REC_BUF_MODE_RING  - Specifies the ring buffer mode 
 */
/*------------------------------------------------------------------*/ 
typedef enum _SVCTX_REC_BUF_MODE_T
{
    SVCTX_REC_BUF_MODE_PLAIN,
    SVCTX_REC_BUF_MODE_RING
} SVCTX_REC_BUF_MODE_T;

/* record buffer bound type */
/*------------------------------------------------------------------*/
/*! @enum   SVCTX_REC_BOUND_TYPE_T
 *  @brief  Specifies the record bound type.
 *  @code
 *  typedef enum _SVCTX_REC_BOUND_TYPE_T
 *  {
 *      SVCTX_REC_BOUND_TYPE_TIME,
 *      SVCTX_REC_BOUND_TYPE_SPACE
 *  } SVCTX_REC_BOUND_TYPE_T;
 *  @endcode
 *  @li@c SVCTX_REC_BOUND_TYPE_TIME  - Specifies the time bound type.
 *  @li@c SVCTX_REC_BOUND_TYPE_SPACE - Sepcifies the space bound type.
 */
/*------------------------------------------------------------------*/ 
typedef enum _SVCTX_REC_BOUND_TYPE_T
{
    SVCTX_REC_BOUND_TYPE_TIME,
    SVCTX_REC_BOUND_TYPE_SPACE
} SVCTX_REC_BOUND_TYPE_T;

/* record stop option */
/*------------------------------------------------------------------*/
/*! @enum   SVCTX_REC_STOP_OPT_T
 *  @brief  Specifies the action when stop recording.
 *  @code 
 *  typedef enum _SVCTX_REC_STOP_OPT_T
 *  {
 *      SVCTX_REC_STOP_OPT_NO_CARE,
 *      SVCTX_REC_STOP_OPT_FINALIZE
 *  } SVCTX_REC_STOP_OPT_T;
 *  @endcode
 *  @li@c SVCTX_REC_STOP_OPT_NO_CARE  - No action when stop recording.
 *  @li@c SVCTX_REC_STOP_OPT_FINALIZE - Finalize the recorded file when stop recording.
 */
/*------------------------------------------------------------------*/ 
typedef enum _SVCTX_REC_STOP_OPT_T
{
    SVCTX_REC_STOP_OPT_NO_CARE,
    SVCTX_REC_STOP_OPT_FINALIZE
} SVCTX_REC_STOP_OPT_T;

/* continuous option */
/*------------------------------------------------------------------*/
/*! @enum   SVCTX_REC_CONT_OPT_T
 *  @brief  Specifies the continuous option.
 *  @code
 *  typedef enum _SVCTX_REC_CONT_OPT_T
 *  {
 *      SVCTX_REC_CONT_OPT_NORMAL,
 *      SVCTX_REC_CONT_OPT_CONT
 *  } SVCTX_REC_CONT_OPT_T;    
 *  @endcode
 *  @li@c SVCTX_REC_CONT_OPT_NORMAL - Specifies the normal option
 *  @li@c SVCTX_REC_CONT_OPT_CONT   - Specifies the continuous option
 */
/*------------------------------------------------------------------*/ 
typedef enum _SVCTX_REC_CONT_OPT_T
{
    SVCTX_REC_CONT_OPT_NORMAL,
    SVCTX_REC_CONT_OPT_CONT
} SVCTX_REC_CONT_OPT_T;    

/* options for no data handling */
/*------------------------------------------------------------------*/
/*! @enum   SVCTX_REC_NO_DATA_OPT_T
 *  @brief  Specifies the option for no-data tick handling.
 *  @code
 *  typedef enum _SVCTX_REC_NO_DATA_OPT_T
 *  {
 *      SVCTX_REC_NO_DATA_OPT_KEEP_REC,
 *      SVCTX_REC_NO_DATA_OPT_SKIP_REC
 *  } SVCTX_REC_NO_DATA_OPT_T;
 *  @endcode
 *  @li@c SVCTX_REC_NO_DATA_OPT_KEEP_REC - Specifies the option to keep the no-data tick.
 *  @li@c SVCTX_REC_NO_DATA_OPT_SKIP_REC - Specifies the option to skip the no-data tick.
 */
/*------------------------------------------------------------------*/ 
typedef enum _SVCTX_REC_NO_DATA_OPT_T
{
    SVCTX_REC_NO_DATA_OPT_KEEP_REC,
    SVCTX_REC_NO_DATA_OPT_SKIP_REC
} SVCTX_REC_NO_DATA_OPT_T;

/* recording authorization status */
/*------------------------------------------------------------------*/
/*! @enum   SVCTX_REC_AUTH_STATUS_T
 *  @brief  Defines the recording authentication status.
 *  @code
 *  typedef enum _SVCTX_REC_AUTH_STATUS_T
 *  {
 *      SVCTX_REC_AUTH_STATUS_UNKNOWN,
 *      SVCTX_REC_AUTH_STATUS_AUTHORIZED,
 *      SVCTX_REC_AUTH_STATUS_UNAUTHORIZED
 *  } SVCTX_REC_AUTH_STATUS_T;    
 *  @endcode
 *  @li@c SVCTX_REC_AUTH_STATUS_UNKNOWN      - Specifies the unknown status.
 *  @li@c SVCTX_REC_AUTH_STATUS_AUTHORIZED   - Specifies the stream is authorized to record.
 *  @li@c SVCTX_REC_AUTH_STATUS_UNAUTHORIZED - Specifies the stream is unauthorized to record.
 */
/*------------------------------------------------------------------*/ 
typedef enum _SVCTX_REC_AUTH_STATUS_T
{
    SVCTX_REC_AUTH_STATUS_UNKNOWN,
    SVCTX_REC_AUTH_STATUS_AUTHORIZED,
    SVCTX_REC_AUTH_STATUS_UNAUTHORIZED
} SVCTX_REC_AUTH_STATUS_T;    
    
/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/* special constant of SVCTX_REC_ES_LIST_T.ui2_idx_prim_video */
#define SVCTX_REC_ES_LIST_IDX_NULL          ((UINT16) 0xFFFF)   /**< Defines the NULL ES-index, could be used 
                                                                     in SVCTX_REC_ES_LIST_T.ui2_idx_prim_video, for example */

/* elementary stream info */
/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_ES_INFO_T
 *  @brief  Defines the information of the ES to be recorded.
 *  @code
 *  typedef struct _SVCTX_REC_ES_INFO_T
 *  {
 *      MPEG_2_PID_T        t_pid;           
 *      UINT32              ui4_type_mask;   
 *  } SVCTX_REC_ES_INFO_T;
 *  @endcode
 *  @li@c t_pid         - Contains the pid of the ES.
 *  @li@c ui4_type_mask - Specifies the stream types the ES belongs to.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_ES_INFO_T
{
    MPEG_2_PID_T        t_pid;               /* pid */
    UINT32              ui4_type_mask;       /* mask for stream types (ST_MASK_*) */
} SVCTX_REC_ES_INFO_T;

/* elementary stream list to be recorded (SVCTX_REC_SET_ES_LIST) */
/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_ES_LIST_T
 *  @brief  Contains the ES list.
 *  @code
 *  typedef struct _SVCTX_REC_ES_LIST_T
 *  {
 *      UINT32               ui4_strct_ver;      
 *      UINT16               ui2_es_num;          
 *      SVCTX_REC_ES_INFO_T* at_es_info;     
 *      UINT16               ui2_idx_prim_video;  
 *  } SVCTX_REC_ES_LIST_T ;
 *  @endcode
 *  @li@c ui4_strct_ver      - Specifies the structure version, should be 1
 *  @li@c ui2_es_num         - Specifies the number of ES to record
 *  @li@c at_es_info         - Specifies the information array of all ES to be record.
 *  @li@c ui2_idx_prim_video - Specifies the index of primary video ES, specify SVCTX_REC_ES_LIST_IDX_NULL if no video ES
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_ES_LIST_T
{
    UINT32               ui4_strct_ver;       /* structure version, should be 1 */
    UINT16               ui2_es_num;          /* number of ES to record */
    SVCTX_REC_ES_INFO_T* at_es_info;          /* list of ES to record */
    UINT16               ui2_idx_prim_video;  /* index of primary video ES, 
                                                 specify SVCTX_REC_ES_LIST_IDX_NULL if no video ES */
} SVCTX_REC_ES_LIST_T ;
 
/* parameter of SVCTX_REC_SET_TYPE_DEL_REC_DATA */
/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_DEL_DATA_T
 *  @brief  Contains the pv_set_info of the set type SVCTX_REC_SET_TYPE_DEL_REC_DATA
 *  @code
 *  typedef struct _SVCTX_REC_DEL_DATA_T
 *  {
 *      UINT32               ui4_strct_ver;     
 *      CHAR*                s_head_file;       
 *  } SVCTX_REC_DEL_DATA_T;     
 *  @endcode
 *  @li@c ui4_strct_ver - Specifies the struct version
 *  @li@c s_head_file   - Specifies the head file of the record data to be deleted.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_DEL_DATA_T
{
    UINT32               ui4_strct_ver;     /* structure version, should be 1 */
    CHAR*                s_head_file;       /* path of the head file */
} SVCTX_REC_DEL_DATA_T;     
 
/* permanent attributes of record service context */
/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_ATTR_T
 *  @brief  Contains the attribute for creating a Record SVCTX object.
 *  @code
 *  typedef struct _SVCTX_REC_ATTR_T
 *  {
 *      UINT32                  ui4_strct_ver; 
 *      UINT32                  ui4_flags;     
 *      HANDLE_T                h_svctx_lead;  
 *      SVCTX_REC_SRC_TYPE_T    e_rec_src_type;
 *      INT8                    i1_prio_diff;  
 *  } SVCTX_REC_ATTR_T;
 *  @endcode
 *  @li@c ui4_strct_ver  - Specifies the structure version, should be 1
 *  @li@c ui4_flags      - Reserved, should be 0
 *  @li@c h_svctx_lead   - Specifies the svctx to be followed
 *  @li@c e_rec_src_type - Specifies the record source type
 *  @li@c i1_prio_diff   - Specifies the pipe priority diff with the SVCTX controlling the sink display group
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_ATTR_T
{
    UINT32                  ui4_strct_ver;   /* structure version, should be 1 */
    UINT32                  ui4_flags;       /* reserved, should be 0 */
    HANDLE_T                h_svctx_lead;    /* svctx to be followed */
    SVCTX_REC_SRC_TYPE_T    e_rec_src_type;  /* record source type */
    INT8                    i1_prio_diff;    /* pipe priority diff with the SVCTX controlling the sink display group */
} SVCTX_REC_ATTR_T;

/* SVCTX_REC_BOUND_TYPE_TIME bound info (SVCTX_REC_CONFIG_T.pv_bound_info) */
/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_TIME_BOUND_INFO_T
 *  @brief  Contains the information for time-bound mode.
 *  @code
 *  typedef struct _SVCTX_REC_TIME_BOUND_INFO_T
 *  {
 *      UINT32                  ui4_strct_ver; 
 *      UINT32                  ui4_flags;     
 *      UINT32                  ui4_ticks; 
 *      UINT64                  ui8_tsb_cap;    
 *  } SVCTX_REC_TIME_BOUND_INFO_T;
 *  @endcode
 *  @li@c ui4_strct_ver - Specifies the structure version, should be 1
 *  @li@c ui4_flags     - Reserved, should be 0
 *  @li@c ui4_ticks     - Specifies the time bound in ticks
 *  @li@c ui8_tsb_cap   - Specifies the ts buffer capacity in bytes (must be multiple of ui4_sec_size)
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_TIME_BOUND_INFO_T
{
    UINT32                  ui4_strct_ver;   /* structure version, should be 1 */
    UINT32                  ui4_flags;       /* reserved, should be 0 */
    UINT32                  ui4_ticks;       /* the time bound in ticks */
    UINT64                  ui8_tsb_cap;     /* ts buffer capacity in bytes (must be multiple of ui4_sec_size) */
} SVCTX_REC_TIME_BOUND_INFO_T;

/* SVCTX_REC_BOUND_TYPE_SPACE bound size */
#define SVCTX_REC_SPACE_BOUND_UNLIMIT        ((UINT64) 0)  /**< Defines the constant used in SVCTX_REC_SPACE_BOUND_INFO_T.ui8_size */

/* time limit of space bound */
#define SVCTX_REC_SPACE_BOUND_TIME_UNLIMIT   ((UINT32) 0)  /**< Defines the constant used in SVCTX_REC_SPACE_BOUND_INFO_T.ui4_tick_cap */

/* SVCTX_REC_BOUND_TYPE_SPACE bound info (SVCTX_REC_CONFIG_T.pv_bound_info) */
/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_SPACE_BOUND_INFO_T
 *  @brief  Contains the information for space-bound mode.
 *  @code
 *  typedef struct _SVCTX_REC_SPACE_BOUND_INFO_T
 *  {
 *      UINT32                  ui4_strct_ver;
 *      UINT32                  ui4_flags;    
 *      UINT64                  ui8_size;  
 *      UINT32                  ui4_tick_cap;
 *  } SVCTX_REC_SPACE_BOUND_INFO_T;
 *  @endcode
 *  @li@c ui4_strct_ver - Specifies the structure version, should be 1
 *  @li@c ui4_flags     - Reserved, should be 0
 *  @li@c ui8_size      - Specifies the space bound in bytes
 *  @li@c ui4_tick_cap  - Specifies the time capacity in ticks
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_SPACE_BOUND_INFO_T
{
    UINT32                  ui4_strct_ver;   /* structure version, should be 1 */
    UINT32                  ui4_flags;       /* reserved, should be 0 */
    UINT64                  ui8_size;        /* the space bound in bytes */
    UINT32                  ui4_tick_cap;    /* time capacity in ticks */
} SVCTX_REC_SPACE_BOUND_INFO_T;

/* storage description */
/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_STRG_DESC_T
 *  @brief  Contains the storage information.
 *  @code
 *  typedef struct _SVCTX_REC_STRG_DESC_T
 *  {
 *      UINT32       ui4_strct_ver;          
 *      CHAR*        s_strg_path;            
 *      UINT64       ui8_tsb_offset;         
 *      UINT32       ui4_mem_align;          
 *      UINT32       ui4_sec_size;           
 *      UINT32       ui4_min_access_sec_ns;  
 *      UINT32       ui4_max_access_sec_ns;  
 *      UINT32       ui4_best_access_sec_ns; 
 *  } SVCTX_REC_STRG_DESC_T;
 *  @endcode
 *  @li@c ui4_strct_ver          - Specifies the structure version, should be 1
 *  @li@c s_strg_path            - Specifies the storage path for TS Buffer.
 *  @li@c ui8_tsb_offset         - Specifies offset for TS Buffer from begin of storage in bytes (must be multiple of ui4_sec_size)
 *  @li@c ui4_mem_align          - Specifies memory alignment of device
 *  @li@c ui4_sec_size           - Specifies sector size in bytes
 *  @li@c ui4_min_access_sec_ns  - Specifies minimal access size (in sector number) of device
 *  @li@c ui4_max_access_sec_ns  - Specifies maximal access size (in sector number) of device
 *  @li@c ui4_best_access_sec_ns - Specifies best access size (in sector nunber) suggested by device
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_STRG_DESC_T
{
    UINT32       ui4_strct_ver;           /* structure version, should be 1 */
    
    CHAR*        s_strg_path;             /* storage path for TS Buffer */
    UINT64       ui8_tsb_offset;          /* offset for TS Buffer from begin of storage in bytes (must be multiple of ui4_sec_size) */
    
    UINT32       ui4_mem_align;           /* memory alignment of device */
    UINT32       ui4_sec_size;            /* sector size in bytes */
    UINT32       ui4_min_access_sec_ns;   /* minimal access size (in sector number) of device */
    UINT32       ui4_max_access_sec_ns;   /* maximal access size (in sector number) of device */
    UINT32       ui4_best_access_sec_ns;  /* best access size (in sector nunber) suggested by device */
    
} SVCTX_REC_STRG_DESC_T;

/* normal recording info (SVCTX_REC_CONFIG_T.pv_cont_info) for SVCTX_REC_CONT_OPT_NORMAL */
/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_CONT_INFO_NORMAL_T
 *  @brief  Contains the normal recording info (SVCTX_REC_CONFIG_T.pv_cont_info) for SVCTX_REC_CONT_OPT_NORMAL
 *  @code
 *  typedef struct _SVCTX_REC_CONT_INFO_NORMAL_T
 *  {
 *      UINT32                  ui4_strct_ver;
 *      UINT32                  ui4_flags;    
 *  } SVCTX_REC_CONT_INFO_NORMAL_T;
 *  @endcode
 *  @li@c ui4_strct_ver   - Specifies structure version, should be 1
 *  @li@c ui4_flags       - Reserved, should be 0
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_CONT_INFO_NORMAL_T
{
    UINT32                  ui4_strct_ver;      /* structure version, should be 1 */
    UINT32                  ui4_flags;          /* reserved */
} SVCTX_REC_CONT_INFO_NORMAL_T;

/* continued recording info (SVCTX_REC_CONFIG_T.pv_cont_info) for SVCTX_REC_CONT_OPT_CONT */
/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_CONT_INFO_CONT_T
 *  @brief  Contains the continued recording info (SVCTX_REC_CONFIG_T.pv_cont_info) for SVCTX_REC_CONT_OPT_CONT
 *  @code
 *  typedef struct _SVCTX_REC_CONT_INFO_CONT_T
 *  {
 *      UINT32                   ui4_strct_ver;  
 *      UINT32                   ui4_flags;      
 *      CHAR*                    s_strg_path;    
 *      UINT64                   ui8_strg_offset;
 *  } SVCTX_REC_CONT_INFO_CONT_T;
 *  @endcode
 *  @li@c ui4_strct_ver   - Specifies structure version, should be 1.
 *  @li@c ui4_flags       - Reserved, should be 0.
 *  @li@c s_strg_path     - Specifies the path for previously stored metadata.
 *  @li@c ui8_strg_offset - Specifies the offset for store location.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_CONT_INFO_CONT_T
{
    UINT32                   ui4_strct_ver;      /* structure version, should be 1 */
    UINT32                   ui4_flags;          /* reserved, should be 0 */
    CHAR*                    s_strg_path;        /* path for previously stored metadata */
    UINT64                   ui8_strg_offset;    /* offset for store location */
} SVCTX_REC_CONT_INFO_CONT_T;

/* record config */
/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_CONFIG_T
 *  @brief  Contains the record configuration.
 *  @code
 *  typedef struct _SVCTX_REC_CONFIG_T
 *  {
 *      UINT32                      ui4_strct_ver;     
 *      UINT32                      ui4_flags;         
 *      x_svctx_rec_nfy_fct         pf_rec_nfy;        
 *      VOID*                       pv_nfy_tag;        
 *      HANDLE_T                    h_svctx_nfy;       
 *      VOID*                       pv_rec_purpose;    
 *      UINT32                      ui4_ms_tick_period;
 *      SVCTX_REC_PROFILE_T         e_profile;
 *      SVCTX_REC_BUF_MODE_T        e_buf_mode;        
 *      SVCTX_REC_BOUND_TYPE_T      e_bound_type;      
 *      VOID*                       pv_bound_info;     
 *      CHAR*                       s_db_name;
 *      SVCTX_REC_STRG_DESC_T       t_strg_desc;       
 *      SVCTX_REC_STOP_OPT_T        e_stop_opt;
 *      SVCTX_REC_CONT_OPT_T        e_cont_opt;        
 *      VOID*                       pv_cont_info;      
 *      SVCTX_REC_NO_DATA_OPT_T     e_no_data_opt;     
 *      ISO_639_LANG_T              s_lang;            
 *  } SVCTX_REC_CONFIG_T;
 *  @endcode
 *  @li@c ui4_strct_ver      - Specifies structure version, should be 1.
 *  @li@c ui4_flags          - Reserved, should be 0.
 *  @li@c pf_rec_nfy         - Specifies the record notification function.
 *  @li@c pv_nfy_tag         - Specifies the tag for pf_rec_nfy.
 *  @li@c h_svctx_nfy        - Specifies the the listener of record notification.
 *  @li@c pv_rec_purpose     - Specifies the record purpose.
 *  @li@c ui4_ms_tick_period - Specifies the tick period in ms.
 *  @li@c e_profile          - Specifies the record profile.
 *  @li@c e_buf_mode         - Specifies the buffer operation mode.
 *  @li@c e_bound_type       - Specifies the record bound type.
 *  @li@c pv_bound_info      - Specifies the record bound info.
 *  @li@c s_db_name          - Specifies the record DB name.
 *  @li@c t_strg_desc        - Specifies the storage description.
 *  @li@c e_stop_opt         - Specifies the action option when stop recording.
 *  @li@c e_cont_opt         - Specifies the options for continuous recording.
 *  @li@c pv_cont_info       - Specifies the continuous recording info.
 *  @li@c e_no_data_opt      - Specifies the options for handling no-data case.
 *  @li@c s_lang             - Specifies the system language for recording.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_CONFIG_T
{
    UINT32                      ui4_strct_ver;      /* structure version, should be 1 */
    UINT32                      ui4_flags;          /* reserved, should be 0 */
    x_svctx_rec_nfy_fct         pf_rec_nfy;         /* record notify */
    VOID*                       pv_nfy_tag;         /* record notify tag */
    HANDLE_T                    h_svctx_nfy;        /* the listener of record notify */
    VOID*                       pv_rec_purpose;     /* record purpose */
    UINT32                      ui4_ms_tick_period; /* tick period in ms */
    SVCTX_REC_PROFILE_T         e_profile;          /* record profile */
    SVCTX_REC_BUF_MODE_T        e_buf_mode;         /* buffer operation mode */
    SVCTX_REC_BOUND_TYPE_T      e_bound_type;       /* record bound type */
    VOID*                       pv_bound_info;      /* record bound info */
    CHAR*                       s_db_name;          /* record database name */
    SVCTX_REC_STRG_DESC_T       t_strg_desc;        /* storage description */
    SVCTX_REC_STOP_OPT_T        e_stop_opt;         /* options for stop recording */
    SVCTX_REC_CONT_OPT_T        e_cont_opt;         /* options for continuous recording */
    VOID*                       pv_cont_info;       /* continued recording info */
    SVCTX_REC_NO_DATA_OPT_T     e_no_data_opt;      /* options for handling no-data case */
    ISO_639_LANG_T              s_lang;             /* system language for recording */
} SVCTX_REC_CONFIG_T;

/* record capability (SVCTX_REC_GET_TYPE_CAPABILITY) */
/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_CAP_INFO_T
 *  @brief  Contains the information about record capability.
 *  @code
 *  typedef struct _SVCTX_REC_CAP_INFO_T
 *  {
 *      BOOL        b_recordable;
 *  } SVCTX_REC_CAP_INFO_T;    
 *  @endcode
 *  @li@c b_recordable - Specifies if recordable or not.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_CAP_INFO_T
{
    BOOL        b_recordable;               /* capability to record this service */

} SVCTX_REC_CAP_INFO_T;    

/* system resource information (SVCTX_REC_GET_TYPE_SYS_RES_INFO) */
/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_SYS_RES_INFO_T
 *  @brief  Contains the information abount system resources.
 *  @code
 *  typedef struct _SVCTX_REC_SYS_RES_INFO_T
 *  {
 *      UINT16      ui2_max_es_num;
 *  } SVCTX_REC_SYS_RES_INFO_T;
 *  @endcode
 *  @li@c ui2_max_es_num - Specifies the maximum number of ES that can be concurrently recorded.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_SYS_RES_INFO_T
{
    UINT16      ui2_max_es_num;             /* max concurrent recording es number */
    
} SVCTX_REC_SYS_RES_INFO_T;    

/* recording status (SVCTX_REC_GET_TYPE_STATUS) */
/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_STATUS_T
 *  @brief  Contains the current recording status.
 *  @code
 *  typedef struct _SVCTX_REC_STATUS_T
 *  {
 *      SVCTX_REC_COND_T  e_rec_cond;    
 *      UINT64            ui8_avail;     
 *      UINT64            ui8_fullness;  
 *      UINT32            ui4_time_bound;
 *      UINT32            ui4_rec_start; 
 *      UINT32            ui4_rec_end;   
 *  } SVCTX_REC_STATUS_T;
 *  @endcode
 *  @li@c e_rec_cond     - Specifies the recording condition.
 *  @li@c ui8_avail      - Specifies the available space to record in bytes.
 *  @li@c ui8_fullness   - Specifies the recorded data size in bytes.
 *  @li@c ui4_time_bound - Specifies the recording time bound in ticks, 0 if SPACE bound.
 *  @li@c ui4_rec_start  - Specifies the recorded range start in tick.
 *  @li@c ui4_rec_end    - Specifies the recorded range end in tick.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_STATUS_T
{
    SVCTX_REC_COND_T  e_rec_cond;           /* recording condition */
    UINT64            ui8_avail;            /* available space to record in bytes */
    UINT64            ui8_fullness;         /* recorded data size in bytes */
    UINT32            ui4_time_bound;       /* recording time bound in ticks, 0 if SPACE bound */
    UINT32            ui4_rec_start;        /* recorded range start in tick */
    UINT32            ui4_rec_end;          /* recorded range end in tick */
} SVCTX_REC_STATUS_T;    

/* authorization info of an es */
/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_ES_AUTH_INFO_T
 *  @brief  Contains the authentication info of an ES. 
 *  @code
 *  typedef struct _SVCTX_REC_ES_AUTH_INFO_T
 *  {
 *      SVCTX_REC_ES_INFO_T         t_info;     
 *      SVCTX_REC_AUTH_STATUS_T     e_auth_st;  
 *  } SVCTX_REC_ES_AUTH_INFO_T;
 *  @endcode
 *  @li@c t_info    - Contains the ES info.
 *  @li@c e_auth_st - Contains the authentication status.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_ES_AUTH_INFO_T
{
    SVCTX_REC_ES_INFO_T         t_info;     /* es info */
    SVCTX_REC_AUTH_STATUS_T     e_auth_st;  /* recording authorization status */
} SVCTX_REC_ES_AUTH_INFO_T;

/* authorization status of es list (SVCTX_REC_GET_TYPE_ESL_AUTH_STATUS) */
/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_ESL_AUTH_STATUS_T
 *  @brief  Contains the authentication status of all ES.
 *  @code
 *  typedef struct _SVCTX_REC_ESL_AUTH_STATUS_T
 *  {
 *      UINT16                      ui2_es_num_cap; 
 *      SVCTX_REC_ES_AUTH_INFO_T*   at_es_auth_info;
 *      UINT16                      ui2_es_num_ret; 
 *  } SVCTX_REC_ESL_AUTH_STATUS_T;
 *  @endcode
 *  @li@c ui2_es_num_cap  - (in) Contains the capacity of the array at_es_auth_info.
 *  @li@c at_es_auth_info - (in/out) Contains the authentication info of all ES.
 *  @li@c ui2_es_num_ret  - (out) Contains the returned number of ES authentication info entry.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_ESL_AUTH_STATUS_T
{
    UINT16                      ui2_es_num_cap;     /* IN, capacity of at_es_auth_info */
    SVCTX_REC_ES_AUTH_INFO_T*   at_es_auth_info;    /* IN/OUT, authorization info for es */
    UINT16                      ui2_es_num_ret;     /* OUT, actual es number */
} SVCTX_REC_ESL_AUTH_STATUS_T;

/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_META_DB_OPEN_T
 *  @brief  Defines the parameter structure used in pv_set_info when e_set_type is SVCTX_REC_SET_TYPE_META_DB_OPEN.
 *  @code
 *  typedef struct _SVCTX_REC_META_DB_OPEN_T
 *  {
 *      UINT32                      ui4_strct_ver; 
 *      CHAR*                       s_head_file;   
 *      HANDLE_T                    h_meta_db;     
 *  } SVCTX_REC_META_DB_OPEN_T;
 *  @endcode
 *
 *  @li@c ui4_strct_ver  - (in)  Specifies the structure version, should be 1
 *  @li@c s_head_file    - (in)  Specifies the full path of the PVR head file.
 *  @li@c h_meta_db      - (out) Returns the handle of the opened metadata database.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_META_DB_OPEN_T
{
    UINT32                      ui4_strct_ver;      /* structure version, should be 1 */
    CHAR*                       s_head_file;        /* in */
    HANDLE_T                    h_meta_db;          /* out */
} SVCTX_REC_META_DB_OPEN_T;

/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_META_DB_CLOSE_T
 *  @brief  Defines the parameter structure used in pv_set_info when e_set_type is SVCTX_REC_SET_TYPE_META_DB_CLOSE.
 *  @code
 *  typedef struct _SVCTX_REC_META_DB_CLOSE_T
 *  {
 *      UINT32                      ui4_strct_ver;  
 *      HANDLE_T                    h_meta_db;      
 *  } SVCTX_REC_META_DB_CLOSE_T;
 *  @endcode
 *
 *  @li@c ui4_strct_ver  - (in) Specifies the structure version, should be 1
 *  @li@c h_meta_db      - (in) Specifies the handle of the metadata database.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_META_DB_CLOSE_T
{
    UINT32                      ui4_strct_ver;      /* structure version, should be 1 */
    HANDLE_T                    h_meta_db;          /* in */
} SVCTX_REC_META_DB_CLOSE_T;

/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_META_DB_STORE_T
 *  @brief  Defines the parameter structure used in pv_set_info when e_set_type is SVCTX_REC_SET_TYPE_META_DB_STORE.
 *  @code
 *  typedef struct _SVCTX_REC_META_DB_STORE_T
 *  {
 *      UINT32                      ui4_strct_ver;
 *      HANDLE_T                    h_meta_db;    
 *  } SVCTX_REC_META_DB_STORE_T;
 *  @endcode
 *
 *  @li@c ui4_strct_ver  - (in) Specifies the structure version, should be 1
 *  @li@c h_meta_db      - (in) Specifies the handle of the metadata database.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_META_DB_STORE_T
{
    UINT32                      ui4_strct_ver;      /* structure version, should be 1 */
    HANDLE_T                    h_meta_db;          /* in */
} SVCTX_REC_META_DB_STORE_T;

/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_SET_META_INFO_T
 *  @brief  Defines the parameter structure used in pv_set_info when e_set_type is SVCTX_REC_SET_TYPE_META_INFO.
 *  @code
 *  typedef struct _SVCTX_REC_SET_META_INFO_T
 *  {
 *      UINT32                      ui4_strct_ver;   
 *      HANDLE_T                    h_meta_db;       
 *      SVCTX_REC_META_KEY_TYPE_T   e_key_type;    
 *      VOID*                       pv_data;         
 *      SIZE_T                      z_data_len;      
 *  } SVCTX_REC_SET_META_INFO_T;
 *  @endcode
 *
 *  @li@c ui4_strct_ver  - (in) Specifies the structure version, should be 1
 *  @li@c h_meta_db      - (in) Specifies the handle of the metadata database.
 *  @li@c e_key_type     - (in) Specifies the customer-defined key type. Note that the key type here must be a customer-defined key type.
 *  @li@c pv_data        - (in) Contains the data to be set. Specify NULL if z_data_len = 0.
 *  @li@c z_data_len     - (in) Specifies the length of the data to be set. If 0, pv_data can be NULL.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_SET_META_INFO_T
{
    UINT32                      ui4_strct_ver;      /* structure version, should be 1 */
    HANDLE_T                    h_meta_db;          /* in */
    SVCTX_REC_META_KEY_TYPE_T   e_key_type;         /* in */
    VOID*                       pv_data;            /* in */
    SIZE_T                      z_data_len;         /* in */
} SVCTX_REC_SET_META_INFO_T;

/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_GET_META_INFO_LEN_T
 *  @brief  Defines the parameter structure used in pv_set_info when e_set_type is SVCTX_REC_GET_TYPE_META_INFO_LEN.
 *  @code
 *  typedef struct _SVCTX_REC_GET_META_INFO_LEN_T
 *  {
 *      UINT32                      ui4_strct_ver; 
 *      HANDLE_T                    h_meta_db;     
 *      SVCTX_REC_META_KEY_TYPE_T   e_key_type;  
 *      SIZE_T                      z_data_len;    
 *  } SVCTX_REC_GET_META_INFO_LEN_T;
 *  @endcode
 *
 *  @li@c ui4_strct_ver  - (in)  Specifies the structure version, should be 1
 *  @li@c h_meta_db      - (in)  Specifies the handle of the metadata database.
 *  @li@c e_key_type     - (in)  Specifies the key type.
 *  @li@c z_data_len     - (out) Returns the data length of the specified key type.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_GET_META_INFO_LEN_T
{
    UINT32                      ui4_strct_ver;      /* structure version, should be 1 */
    HANDLE_T                    h_meta_db;          /* in */
    SVCTX_REC_META_KEY_TYPE_T   e_key_type;         /* in */
    SIZE_T                      z_data_len;         /* out */
} SVCTX_REC_GET_META_INFO_LEN_T;

/*----------------------------------------------------------------------------*/
/*! @struct SVCTX_REC_GET_META_INFO_T
 *  @brief  Defines the parameter structure used in pv_set_info when e_set_type is SVCTX_REC_GET_TYPE_META_INFO.
 *  @code
 *  typedef struct _SVCTX_REC_GET_META_INFO_T
 *  {
 *      UINT32                      ui4_strct_ver; 
 *      HANDLE_T                    h_meta_db;     
 *      SVCTX_REC_META_KEY_TYPE_T   e_key_type;  
 *      VOID*                       pv_buf;        
 *      SIZE_T                      z_buf_len;     
 *      SIZE_T                      z_data_len;    
 *  } SVCTX_REC_GET_META_INFO_T;
 *  @endcode
 *
 *  @li@c ui4_strct_ver  - (in)  Specifies the structure version, should be 1
 *  @li@c h_meta_db      - (in)  Specifies the handle of the metadata database.
 *  @li@c e_key_type     - (in)  Specifies the key type.
 *  @li@c pv_buf         - (in/out) Spcifies the buffer address to receive the data.
 *  @li@c z_buf_len      - (in)  Specifies the capacity of the buffer.
 *  @li@c z_data_len     - (out) Specifies the received data length.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SVCTX_REC_GET_META_INFO_T
{
    UINT32                      ui4_strct_ver;      /* structure version, should be 1 */
    HANDLE_T                    h_meta_db;          /* in */
    SVCTX_REC_META_KEY_TYPE_T   e_key_type;         /* in */
    VOID*                       pv_buf;             /* in/out */
    SIZE_T                      z_buf_len;          /* in */
    SIZE_T                      z_data_len;         /* out */
} SVCTX_REC_GET_META_INFO_T;

/* record info, SRC_DESC_T's pv_details when e_type = SRC_TYPE_RECORD */
/*----------------------------------------------------------------------------*/
/*! @struct SRC_REC_INFO_T
 *  @brief  Contains the record info. It's SRC_DESC_T's pv_details when e_type = SRC_TYPE_RECORD
 *  @code
 *  typedef struct _SRC_REC_INFO_T
 *  {
 *      SRC_TYPE_T        e_src_rec;
 *      VOID*             pv_detail;  
 *  } SRC_REC_INFO_T;
 *  @endcode
 *  @li@c e_src_rec - Specifies the source type of target content to record
 *  @li@c pv_detail - Specifies the detail information corresponding to e_src_rec.
 */
/*----------------------------------------------------------------------------*/
typedef struct _SRC_REC_INFO_T
{
    SRC_TYPE_T        e_src_rec;            /* record source */
    VOID*             pv_detail;            /* detail info */
} SRC_REC_INFO_T;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
 

#endif  /* _U_REC_HDLR_H_ */
