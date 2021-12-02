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
/*! @file x_cfg.h 
 *  $File: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/middleware/config/x_cfg.h $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Configuration Manager (CFG) specific
 *         definitions, which are exported.
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_CFG_H_
#define _X_CFG_H_
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_CONFIG
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
                    include files
-------------------------------------------------------------------------*/

#include "x_common.h"
#include "x_handle.h"
#include "x_sys_name.h"
#include "u_cfg.h"

/*-------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ------------------------------------------------------------------------*/
#define CFG_HANDLE        (HT_GROUP_CFG + ((HANDLE_TYPE_T) 0))  /**< Config handle type */
#define CFG_NOTIFY_HANDLE (HT_GROUP_CFG + ((HANDLE_TYPE_T) 1))  /**< Config notification handle type */

/*--------------------------------------------------------------------
                 Functions
 -------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief  This function creates one or more configuration information objects.
 *          This is typically called once at initial power-up to create the
 *          configuration settings database. The array of descriptors will
 *          probably be in ROM or FLASH.  Once the end-user has modified the
 *          configurations to suit their personal needs/tastes, the
 *          configurations can be saved to some persistent storage (EEPROM,
 *          FLASH, Memory Stick, hard disk) and subsequently loaded from
 *          persistent storage on power up.
 *  @param  [in]  pt_descr          - Pointer to information on fields
 *  @param  [in]  ui2_num           - Number of entries in at_field_info.
 *  @param  [in]  ps_config_name    - Name to associate with this configuration.
 *  @param  [out] ph_config         - Handle to configuration.
 *  @return Return the error code
 *  @retval CFGR_OK             - Operation successfull.
 *  @retval CFGR_INV_ARG        - ui2_entries is 0, pt_descr is NULL, or fields
 *                                in pt_descr have illegal values.
 *  @retval CFGR_OUT_OF_MEM     - Ran out of memory while creating settings.
 *  @retval CFGR_OUT_OF_HANDLE  - Ran out of handles while creating settings.
 *  @retval CFGR_CONFIG_EXISTS  - There are configuration settings for this
 *                                config_name in the database already.
 *  @retval CFGR_LIMIT_EXCEEDED - Too many settings.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_cfg_create(
    CFG_DESCR_T* pt_descr,
    UINT16       ui2_num,
    const CHAR*  ps_config_name,
    HANDLE_T*    ph_config
);

/*------------------------------------------------------------------*/
/*! @brief  This function creates a new handle to the specified configuration.
 *  @param  [in]  ps_config_name    - Name of configuration to open.
 *  @param  [out] ph_config         - Pointer to handle variable to
 *                                    configuration.
 *  @return Return the error code
 *  @retval CFGR_OK             - Operation successful.
 *  @retval CFGR_REC_NOT_FOUND  - Specified config name not found.
 *  @retval CFGR_OUT_OF_HANDLE  - Ran out of handles while opening settings.
 *  @retval CFGR_OUT_OF_MEM     - Ran out of memory while opening settings.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_cfg_open(
    const CHAR* ps_config_name,
    HANDLE_T*   ph_config
);

/*------------------------------------------------------------------*/
/*! @brief  This function gets the current value and size of the specified
 *          field.
 *  @param  [in]  h_config          - Handle to configuration.
 *  @param  [in]  ui2_id            - ID of configuration record to get.
 *  @param  [out] pv_variable       - Pointer to memory buffer to hold retrieved
 *                                    values. This can be NULL if the caller
 *                                    just wants the size required so the caller
 *                                    can allocate an array of an appropriate
 *                                    size then call x_cfg_get again with the
 *                                    proper size array.
 *  @param  [out] pz_size           - Pointer to variable to hold number of
 *                                    bytes required to hold value.
 *  @return Return the error code
 *  @retval CFGR_OK             - Operation successful.
 *  @retval CFGR_INV_ARG        - pz_size is NULL or ui2_id is not valid.
 *  @retval CFGR_INV_HANDLE     - h_config not valid.
 *  @retval CFGR_REC_NOT_FOUND  - Record ui2_id was not found.
 *  @retval CFGR_CANT_UNLOCK    - Can't unlock the read/write loc.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_cfg_get(
    HANDLE_T h_config,
    UINT16   ui2_id,
    VOID*    pv_variable,
    SIZE_T*  pz_size
);

/*------------------------------------------------------------------*/
/*! @brief  This function sets the specified field to the value given. If the
 *          group is not locked, the notification function for the group that
 *          this setting belongs to is sent by this function.
 *  @param  [in]  h_config          - Handle to configuration.
 *  @param  [in]  ui2_id            - ID of configuration record to set.
 *  @param  [in]  pv_variable       - Pointer to memory buffer with new values.
 *  @param  [in]  z_size            - Size of input array.
 *  @return Return the error code
 *  @retval CFGR_OK             - Operation successful.
 *  @retval CFGR_INV_ARG        - pv_variable is NULL or z_size is zero.
 *  @retval CFGR_INV_HANDLE     - h_config not valid.
 *  @retval CFGR_REC_NOT_FOUND  - Record ui2_id was not found.
 *  @retval CFGR_NO_WRITE_LOCK  - There is no read/write lock to lock
 */
/*------------------------------------------------------------------*/
extern
INT32 x_cfg_set(
    HANDLE_T h_config,
    UINT16   ui2_id,
    VOID*    pv_variable,
    SIZE_T   z_size
);

/*------------------------------------------------------------------*/
/*! @brief  This function locks all the configuration settings to insure that
 *          the caller has exclusive access. This is most useful when multiple
 *          settings need to be changed atomically. After this is called,
 *          threads other than the current one are blocked when calling
 *          x_cfg_get/x_cfg_set until after the thread owning the lock calls
 *          x_cfg_unlock.
 *  @param  [in]  h_config          - Handle to configuration.
 *  @return Return the error code
 *  @retval CFGR_OK             - Operation successful.
 *  @retval CFGR_INV_HANDLE     - h_config not valid
 *  @retval CFGR_NO_WRITE_LOCK  - There is no read/write lock to lock.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_cfg_lock(
    HANDLE_T h_config
);

/*------------------------------------------------------------------*/
/*! @brief  This function unlocks the configuration settings for access.
 *  @param  [in]  h_config          - Handle to configuration.
 *  @return Return the error code
 *  @retval CFGR_OK             - Operation successful.
 *  @retval CFGR_INV_HANDLE     - h_config not valid
 *  @retval CFGR_CANT_UNLOCK    - Can't unlock the read/write lock.
 */
/*------------------------------------------------------------------*/
extern 
INT32 x_cfg_unlock(
    HANDLE_T h_config
);

/*------------------------------------------------------------------*/
/*! @brief  This function sets the notification function for a group using a
 *          handle to the configuration and a group number.
 *  @param  [in]  h_config          - Handle to configuration.
 *  @param  [in]  ui2_group         - Configuration group to register for.
 *  @param  [in]  pv_tag            - Pointer to a tag to be passed back to use
 *                                    via notification routine.
 *  @param  [in]  pf_cfg_notify     - Pointer to notification function.
 *  @param  [out] ph_notify_handle  - Handle to notification object.
 *  @return Return the error code
 *  @retval CFGR_OK             - Operation successful.
 *  @retval CFGR_INV_ARG        - pf_nfy_func or ph_notify are NULL
 *  @retval CFGR_INV_HANDLE     - h_config not valid.
 *  @retval CFGR_OUT_OF_MEM     - Ran out of memory when creating notification
 *                                node.
 *  @retval CFGR_REC_NOT_FOUND  - There is no such group.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_cfg_notify_reg(
    HANDLE_T        h_config,
    UINT16          ui2_group,
    VOID*           pv_tag,
    pf_cfg_nfy_func pf_cfg_notify,
    HANDLE_T*       ph_notify_handle
);

/*------------------------------------------------------------------*/
/*! @brief  This function sets the notification function for a group using a
 *          configuration name and a group number.
 *  @param  [in]  ps_config_name    - Name of configuration.
 *  @param  [in]  ui2_group         - Configuration group to register for.
 *  @param  [in]  pv_tag            - Pointer to a tag to be passed back to use
 *                                    via notification routine.
 *  @param  [in]  pf_cfg_notify     - Pointer to notification function.
 *  @param  [out] ph_notify_handle  - Handle to notification object.
 *  @return Return the error code
 *  @retval CFGR_OK             - Operation successful.
 *  @retval CFGR_INV_ARG        - pf_nfy_func or ph_notify are NULL
 *  @retval CFGR_OUT_OF_MEM     - Ran out of memory when creating notification
 *                                node.
 *  @retval CFGR_REC_NOT_FOUND  - There is no such group.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_cfg_notify_reg_by_name(
    const CHAR*     ps_config_name,
    UINT16          ui2_group,
    VOID*           pv_tag,
    pf_cfg_nfy_func pf_cfg_notify,
    HANDLE_T*       ph_notify_handle
);

/*------------------------------------------------------------------*/
/*! @brief  This function loads configuration information from persistent
 *          storage. If a configuration setting exists in persistent storage,
 *          the value(s) for that setting will be loaded. If a configuration
 *          setting is not in persistent storage (it was added after the
 *          previous configuration was stored), then the setting is untouched.
 *
 *          A LOADED notification will be sent for all settings for which a
 *          notification routine was registered.
 *  @param  [in]  h_dir             - Handle to directory to load settings from.
 *  @param  [in]  ps_pathname       - File or device name to load settings from.
 *  @param  [in]  ps_config_name    - Name of the configuration to load.
 *  @param  [out] ph_config         - Handle to configuration.
 *  @return Return the error code
 *  @retval CFGR_OK             - Operation successful.
 *  @retval CFGR_CANT_OPEN_FILE - Could not open the file.
 *  @retval CFGR_NO_SUCH_FILE   - Could not find the file.
 *  @retval CFGR_OUT_OF_MEM     - Ran out of memory while loading settings.
 *  @retval CFGR_INV_ARG        - ps_name is NULL.
 *  @retval CFGR_INV_HANDLE     - h_dir is not valid.
 *  @retval CFGR_NOT_DIR        - File manager not directory error.
 *  @retval CFGR_CORE           - Internal Error while loading settings.
 *  @retval CFGR_NAME_TOO_LONG  - ps_name is too long.
 *  @retval CFGR_DEVICE_ERROR   - A low-level device error occurred while
 *                                loading.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_cfg_fs_load(
    HANDLE_T    h_dir,
    CHAR*       ps_pathname,
    const CHAR* ps_config_name,
    HANDLE_T*   ph_config
);

/*------------------------------------------------------------------*/
/*! @brief  This function saves all configuration information to persistent
 *          storage.
 *  @param  [in]  h_dir             - Handle to directory to save settings to.
 *  @param  [in]  ps_pathname       - Name of file or device to save settings
 *                                    to.
 *  @param  [in]  h_config          - Handle to configuration.
 *  @return Return the error code
 *  @retval CFGR_OK             - Operation successful.
 *  @retval CFGR_CANT_CREATE_FILE   - Could not create the file.
 *  @retval CFGR_INV_ARG        - ps_name is NULL.
 *  @retval CFGR_INV_HANDLE     - h_dir is not a valid handle.
 *  @retval CFGR_NOT_DIR        - File manager not directory error.
 *  @retval CFGR_CORE           - Internal error.
 *  @retval CFGR_NAME_TOO_LONG  - ps_name is too long.
 *  @retval CFGR_DEVICE_ERROR   - Low-level error.
 *  @retval CFGR_EXIST          - A file by that name exists already.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_cfg_fs_store(
    HANDLE_T h_dir,
    CHAR*    ps_pathname,
    HANDLE_T h_config
);

/*------------------------------------------------------------------*/
/*! @brief  This function queries if any configuration information is on
 *          persistent storage.
 *  @param  [in]  h_dir             - Handle to directory to query.
 *  @param  [in]  ps_pathname       - Name of file or device to query.
 *  @param  [in]  ps_config_name    - Name of configuration to query for.
 *  @return Return the error code
 *  @retval CFGR_OK             - Operation successful.
 *  @retval CFGR_INV_ARG        - ps_pathname is NULL.
 *  @retval CFGR_INV_HANDLE     - h_dir is not a valid handle.
 *  @retval CFGR_INV_FILE_PATH  - Raw file system not inited on given path.
 *  @retval CFGR_NO_SUCH_FILE   - Configuration file was not found.
 *  @retval CFGR_NOT_DIR        - File manager not directory error.
 *  @retval CFGR_CORE           - Internal error.
 *  @retval CFGR_NAME_TOO_LONG  - CFGR_NAME_TOO_LONG.
 *  @retval CFGR_DEVICE_ERROR   - Low-level error.
 *  @retval CFGR_EXIST          - A file by that name exists already.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_cfg_fs_qry(
    HANDLE_T    h_dir,
    CHAR*       ps_pathname,
    const CHAR* ps_config_name
);

/*------------------------------------------------------------------*/
/*! @brief  This function deletes all configuration information from persistent
 *          storage.
 *  @param  [in]  h_dir             - Handle to directory to delete from.
 *  @param  [in]  ps_pathname       - Name of file or device to delete from.
 *  @param  [in]  ps_config_name    - Name of configuration to delete.
 *  @return Return the error code
 *  @retval CFGR_OK             - Operation successful.
 *  @retval CFGR_INV_ARG        - ps_pathname is NULL.
 *  @retval CFGR_INV_HANDLE     - h_dir is not a valid handle.
 *  @retval CFGR_INV_FILE_PATH  - Raw file system not inited on given path.
 *  @retval CFGR_CORE           - Internal error.
 *  @retval CFGR_NAME_TOO_LONG  - CFGR_NAME_TOO_LONG.
 *  @retval CFGR_DEVICE_ERROR   - Low-level error.
 *  @retval CFGR_EXIST          - A file by that name exists already.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_cfg_fs_del(
    HANDLE_T    h_dir,
    CHAR*       ps_pathname,
    const CHAR* ps_config_name
);

/*------------------------------------------------------------------*/
/*! @brief  This API let application to specify its own implementation of I/O
 *          functions to read/write/query configuration from persistent storage.
 *          The registered I/O functions will replace the default implementation
 *          when x_cfg_fs_load()/x_cfg_fs_store()/x_cfg_fs_query() are called. 
 *
 *          The function table (see CFG_FILE_API_T) contains 3 function pointers
 *          and optional tag data.  The 3 function pointers are `pf_load',
 *          `pf_store' , and `pf_query'.  The function pointer are invoked
 *          respectively when the corresponding exported API x_cfg_fs_load(),
 *          x_cfg_fs_store(), and x_cfg_fs_query() are called.   The prototype
 *          for the 3 function pointers are described by:
 *          x_cfg_load, x_cfg_store, x_cfg_query.
 *
 *          To un-register customized I/O functions, simply specifies a null
 *          CFG_FILE_API_T pointer.
 *  @param  [in]  h_config          - Handle to configuration.
 *  @param  [in]  pt_fct_tbl        - Pointer to the function table structure.
 *                                    If NULL pointer is specified, then the
 *                                    previously registered function table is
 *                                    un-registered.
 *  @return Return the error code
 *  @retval CFGR_OK             - Operation successful.
 *  @retval CFGR_INV_ARG        - The function table contains null function
 *                                pointer (e.g, `pf_load' or `pf_store' or
 *                                `pf_query' is null).
 *  @retval CFGR_INV_HANDLE     - h_config is not a valid handle.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_cfg_fs_reg_fcts(
    HANDLE_T        h_config,
    CFG_FILE_API_T* pt_fct_tbl
);

/*----------------------------------------------------------------------------*/
/*! @} */
/*----------------------------------------------------------------------------*/
#endif
