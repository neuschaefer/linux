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
/*! @file u_cfg.h 
 *  $File: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/middleware/config/u_cfg.h $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Configuration Manager (CFG) specific
 *         definitions, which are exported.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_CFG_H_
#define _U_CFG_H_
/*----------------------------------------------------------------------------*/
/*! @defgroup   groupMW_CONFIG Configuration Manager
 *  @ingroup    groupMW
 *  @brief      The Configuration Manager provides client applications with
 *              a way to manage run-time changeable settings. It also provides a
 *              way for client applications to be notified when a particular
 *              group of settings is changed.
 *  @see        groupMW_CDB
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
                    include files
-------------------------------------------------------------------------*/

#include "u_handle.h"
#include "u_common.h"

/*-------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ------------------------------------------------------------------------*/

/* Return code status from API */
#define CFGR_OK                   ((INT32)  0)     /**< Operation succeeded. */
#define CFGR_OUT_OF_HANDLE        ((INT32) -1)     /**< Handle allocation failed. */
#define CFGR_OUT_OF_MEM           ((INT32) -2)     /**< Memory exhausted. */
#define CFGR_INV_HANDLE           ((INT32) -3)     /**< Bad handle. */
#define CFGR_INV_ARG              ((INT32) -4)     /**< Invalid argument. */
#define CFGR_REC_NOT_FOUND        ((INT32) -5)     /**< Record not found. */
#define CFGR_END_OF_ITERATION     ((INT32) -6)     /**< Iteration reached end. */
#define CFGR_CANT_CREATE_FILE     ((INT32) -7)     /**< Cannot create file. */
#define CFGR_CANT_OPEN_FILE       ((INT32) -8)     /**< Cannot open file. */
#define CFGR_NO_WRITE_LOCK        ((INT32) -9)     /**< Failed initializing RW Lock. */
#define CFGR_UNKNOWN              ((INT32) -10)    /**< Unknown error. */
#define CFGR_WRITE_LOCKED         ((INT32) -11)    /**< Failed to acquire lock as DB is write locked. */
#define CFGR_MODIFIED             ((INT32) -12)    /**< Database modified since last iteration. */
#define CFGR_INV_NAME             ((INT32) -13)    /**< DB name too long. */
#define CFGR_CONFIG_EXISTS        ((INT32) -14)    /**< DB name already in use. */
#define CFGR_CANT_CREATE_LOCK     ((INT32) -15)    /**< Cannot create RW lock. */
#define CFGR_NO_READ_LOCK         ((INT32) -16)    /**< Cannot acquire read lock. */
#define CFGR_CANT_UNLOCK          ((INT32) -17)    /**< Cannot release lock. */
#define CFGR_NO_SUCH_FILE         ((INT32) -18)    /**< Specified file path not exists. */
#define CFGR_NAME_TOO_LONG        ((INT32) -19)    /**< File manager name too long. */
#define CFGR_CORE                 ((INT32) -20)    /**< File manager internal error. */
#define CFGR_EXIST                ((INT32) -21)    /**< DB/File name already in use. */
#define CFGR_NO_FREE_SPACE        ((INT32) -22)    /**< No free space on file system. */
#define CFGR_NO_SUCH_DEVICE       ((INT32) -23)    /**< File manager no such device. */
#define CFGR_DEVICE_ERROR         ((INT32) -24)    /**< File manager device error. */
#define CFGR_NOT_DIR              ((INT32) -25)    /**< File manager not directory error. */
#define CFGR_LIMIT_EXCEEDED       ((INT32) -26)    /**< Reached maximum number of Config objects. */
#define CFGR_AEE_OUT_OF_RESOURCES ((INT32) -27)    /**< Can not allocate AEE resources. */
#define CFGR_AEE_NO_RIGHTS        ((INT32) -28)    /**< No right for the AEE resources. */
#define CFGR_FILESYS_FULL         ((INT32) -29)    /**< File system full. */
#define CFGR_INV_FILE_PATH        ((INT32) -30)    /**< Invalid file path. */
#define CFGR_FILE_READ_FAIL       ((INT32) -31)    /**< Failed to read from file. */
#define CFGR_FILE_WRITE_FAIL      ((INT32) -32)    /**< Failed to write to file. */

/* to be used to create an ID to be used for config routines */
#define CFG_MAKE_ID(ui1_group, ui1_setting) (((ui1_group) << 8) | (ui1_setting))    /**< Make up the 16-bit configuration ID of group number and setting number, to be used for config routines. */
#define CFG_GET_GROUP(ui2_id) ((ui2_id) >> 8)    /**< Return the group number out of the 16-bit config ID. */
#define CFG_GET_SETTING(ui2_id) ((ui2_id) & 0xff)    /**< Return the setting number out of the 16-bit config ID. */

#define CFG_NAME_LEN 31    /**< Specifies the maximum number of characters in a configuration list name. */

/*------------------------------------------------------------------*/
/*! @brief  Configuration setting element types.
 *  @code
 *  typedef  enum
 *  {
 *      CFG_NONE = 0,
 *      CFG_8BIT_T,
 *      CFG_16BIT_T,
 *      CFG_32BIT_T,
 *      CFG_64BIT_T
 *  } CFG_FIELD_T;
 *  @endcode
 *  @li@c  CFG_NONE                      - None type.
 *  @li@c  CFG_8BIT_T                    - 8-bit type.
 *  @li@c  CFG_16BIT_T                   - 16-bit type.
 *  @li@c  CFG_32BIT_T                   - 32-bit type.
 *  @li@c  CFG_64BIT_T                   - 64-bit type.
 */
/*------------------------------------------------------------------*/
typedef  enum
{
    CFG_NONE = 0,
    CFG_8BIT_T,
    CFG_16BIT_T,
    CFG_32BIT_T,
    CFG_64BIT_T
} CFG_FIELD_T;

/*------------------------------------------------------------------*/
/*! @brief  This type is used as the element of an array for describing all
 *          configuration settings. The array is passed to c_cfg_create.
 *  @code
 *  typedef struct {
 *      UINT16          ui2_id;       
 *      CFG_FIELD_T     e_type;       
 *      UINT16          ui2_num_elem; 
 *  } CFG_DESCR_T;
 *  @endcode
 *  @li@c  ui2_id                        - Setting ID.
 *  @li@c  e_type                        - Type of element.
 *  @li@c  ui2_num_elem                  - Number of elements.
 */
/*------------------------------------------------------------------*/
typedef struct {
    UINT16          ui2_id;       /* setting ID */
    CFG_FIELD_T     e_type;       /* type of element */
    UINT16          ui2_num_elem; /* number of elements */
} CFG_DESCR_T;

/* Notification condition */
/*------------------------------------------------------------------*/
/*! @brief  Configuration notification conditions.
 *  @code
 *  typedef  enum
 *  {
 *      CFG_UPDATED,       
 *      CFG_CREATED,       
 *      CFG_LOADED         
 *  } CFG_COND_T;
 *  @endcode
 *  @li@c  CFG_UPDATED                   - When x_cfg_set is called.
 *  @li@c  CFG_CREATED                   - When x_cfg_create is called.
 *  @li@c  CFG_LOADED                    - When x_cfg_load is called.
 */
/*------------------------------------------------------------------*/
typedef  enum
{
    CFG_UPDATED,       /* When x_cfg_set is called. */
    CFG_CREATED,       /* When x_cfg_create is called */
    CFG_LOADED         /* When x_cfg_load is called */
} CFG_COND_T;

/*------------------------------------------------------------------*/
/*! @brief  Prototype for the notification function registered with the CFG
 *          database object.   The notification function is called in the
 *          context of the thread that is performing the write operations
 *          (update, add, delete) to the CFG database object.
 *
 *          It is very important that the client MUST NOT try to read the CFG in
 *          the context of the notification function.  Otherwise, a `Dead lock'
 *          situation would arise.  The client should only send a message to
 *          itself from the notification function or set a condition flag, and
 *          execute the read operation outside the context of the notification
 *          function.
 *  @param  [in]  h_notify          - Handle to notification object.
 *  @param  [in]  e_cond            - Reason for notification.
 *  @param  [in]  pv_tag            - Client tag.
 *  @param  [in]  ui2_id            - ID of setting that changed.
 *  @note   Client should also avoid doing CPU intensive tasks in the
 *          notification function.  If CPU intensive tasks need to be performed,
 *          then the notification function should send a message to the client
 *          thread, and have the CPU intensive task executed in the client
 *          thread context.
 *  @return None
 */
/*------------------------------------------------------------------*/
typedef  VOID (*pf_cfg_nfy_func)
(
    HANDLE_T   h_notify_handle,
    CFG_COND_T e_cond,
    VOID*      pv_tag,
    UINT16     ui2_id
);

/*
   Function prototype and function table for application to register its
   own read/write/query function for pesistent storage (e.g., EEPROM)
   operations.

   The registered read/write/query functions will be executed when
   Config APIs c_cfs_fs_load, c_cfs_fs_store, c_cfs_fs_query are called.
*/

/*------------------------------------------------------------------*/
/*! @brief  Prototype for the application specified load function registered
 *          with the CFG database object.   The `load' function is called in the
 *          context of the thread that is performing the read operations from
 *          the persistent storage to populate the CFG database object (e.g.,
 *          x_cfg_fs_load).
 *  @param  [in]  h_dir             - Specifies the directory (It can be
 *                                    NULL_HANDLE) under which the  persistent
 *                                    storage is located.
 *  @param  [in]  ps_pathname       - Specifies the file pathname to the
 *                                    persistent storage location under `h_dir'
 *                                    location.
 *  @param  [in]  ps_config_name    - Specifies the name of this configuration
 *                                    to load from persistent storage.  Note:
 *                                    this parameter can be NULL. If NULL is
 *                                    specified, then a no-name configuration
 *                                    database is used.
 *  @param  [in]  pv_tag            - Optional tag value specified by the
 *                                    application when registering the `pf_load'
 *                                    function.  See custom function table
 *                                    registration structure (CFG_FILE_API_T()).
 *  @param  [out] ph_config         - Handle to the configuration database.
 *                                    Note: If `ps_config_name' is NULL, then
 *                                    this parameter is a NULL_HANDLE.
 *  @see    CFG_FILE_API_T
 *  @return Return the error code
 *  @retval CFGR_OK             - Configuration data are loaded from the
 *                                persistent storage location.
 *  @retval CFGR_FILE_READ_FAIL - Can not load configuration data from the
 *                                specified persistent storage location.
 *  @retval CFGR_INV_FILE_PATH  - `ps_pathname' does not exist.
 *  @retval CFGR_INV_HANDLE     - `h_config' is not a valid handle.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_cfg_load)
(
    HANDLE_T             h_dir,
    CHAR*                ps_pathname,
    const CHAR*          ps_config_name,
    HANDLE_T*            ph_config,
    VOID*                pv_tag
);

/*------------------------------------------------------------------*/
/*! @brief  Prototype for the application specified store function registered
 *          with the CFG database object.   The `store' function is called in
 *          the context of the thread that is performing the write operations
 *          from the configuration data in memory to the persistent storage
 *          (e.g.,x_cfg_fs_store)
 *  @param  [in]  h_dir             - Specifies the directory (It can be
 *                                    NULL_HANDLE) under which the  persistent
 *                                    storage is located. 
 *  @param  [in]  ps_pathname       - Specifies the file pathname to the
 *                                    persistent storage location under `h_dir'
 *                                    location.
 *  @param  [in]  h_config          - Handle to the configuration database.
 *                                    Note: this parameter can be a NULL_HANDLE.
 *                                    If NULL_HANDLE is specified, then a `
 *                                    no-name' configuration database is being
 *                                    store to the persistent storage location.
 *  @param  [in]  pv_tag            - Optional tag value specified by the
 *                                    application when registering the `
 *                                    pf_store' function.  See custom function
 *                                    table registration structure
 *                                    (CFG_FILE_API_T())
 *  @return Return the error code
 *  @retval CFGR_OK             - Configuration data are stored to the
 *                                persistent storage location.
 *  @retval CFGR_FILE_WRITE_FAIL - Can not store configuration data to the
 *                                specified persistent storage location.
 *  @retval CFGR_INV_FILE_PATH  - `ps_pathname' does not exist.
 *  @retval CFGR_INV_HANDLE     - `h_config' is not a valid handle.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_cfg_store)
(
    HANDLE_T             h_dir,
    CHAR*                ps_pathname,
    HANDLE_T             h_config,
    VOID*                pv_tag
);

/*------------------------------------------------------------------*/
/*! @brief  Prototype for the application specified query function registered
 *          with the CFG database object.   The `query' function is called in
 *          the context of the thread that is performing the query operation on
 *          the persistent storage (e.g.,x_cfg_fs_qry).   
 *
 *          This function should return OK (CFGR_OK) status if the configuration
 *          database with specified `ps_config_name' exist on the persistent
 *          storage.
 *  @param  [in]  h_dir             - Specifies the directory (It can be
 *                                    NULL_HANDLE) under which the  persistent
 *                                    storage is located. 
 *  @param  [in]  ps_pathname       - Specifies the file pathname to the
 *                                    persistent storage location under `h_dir'
 *                                    location.
 *  @param  [in]  ps_config_name    - Specifies the name of this configuration
 *                                    to query from persistent storage.  Note:
 *                                    this parameter can be NULL. If NULL is
 *                                    specified, then a no-name configuration
 *                                    database was stored to the persistent
 *                                    storage location.
 *  @param  [in]  pv_tag            - Optional tag value specified by the
 *                                    application when registering the `
 *                                    pf_query' function.  See custom function
 *                                    table registration structure
 *                                    (CFG_FILE_API_T()).
 *  @return Return the error code
 *  @retval CFGR_OK             - Configuration data existed in the persistent
 *                                storage location.
 *  @retval CFGR_FILE_READ_FAIL - Can not query configuration data to the
 *                                specified persistent storage location.
 *  @retval CFGR_INV_FILE_PATH  - `ps_pathname' does not exist.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_cfg_query)
(
    HANDLE_T             h_dir,
    CHAR*                ps_pathname,
    const CHAR*          ps_config_name,
    VOID*                pv_tag
);

/*
  Function table for the application specified load/store/query
  persistent storage APIs.
*/
/*------------------------------------------------------------------*/
/*! @brief  Function table structure for the application specified
 *          load/store/query persistent storage APIs.
 *  @code
 *  typedef struct _CFG_FILE_API_T_
 *  {
 *      x_cfg_load           pf_load;
 *      VOID*                pv_load_tag;
 *      x_cfg_store          pf_store;
 *      VOID*                pv_store_tag;
 *      x_cfg_query          pf_query;
 *      VOID*                pv_query_tag;
 *  } CFG_FILE_API_T ;
 *  @endcode
 *  @li@c  pf_load                       - Application specified load function
 *                                         to read configuration data from
 *                                         persistent storage.
 *  @li@c  pv_load_tag                   - Specifies the optional tag value to
 *                                         pass to `pf_load' function. 
 *  @li@c  pf_store                      - Application specified store function
 *                                         to write configuration data to
 *                                         persistent storage.
 *  @li@c  pv_store_tag                  - Specifies the optional tag value to
 *                                         pass to `pf_store' function.
 *  @li@c  pf_query                      - Application specified query function
 *                                         to get the status of configuration
 *                                         data on the persistent storage.
 *  @li@c  pv_query_tag                  - Specifies the optional tag value to
 *                                         pass to `pf_query' function.
 */
/*------------------------------------------------------------------*/
typedef struct _CFG_FILE_API_T_
{
    x_cfg_load           pf_load;
    VOID*                pv_load_tag;
    
    x_cfg_store          pf_store;
    VOID*                pv_store_tag;
    
    x_cfg_query          pf_query;
    VOID*                pv_query_tag;
    
} CFG_FILE_API_T ;

/*----------------------------------------------------------------------------*/
/*! @} */
/*----------------------------------------------------------------------------*/
#endif
