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
/*! @file x_svl.h 
 *  $File: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/middleware/svl/x_svl.h $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *    This header file contains service list (SVL) database API definitions,
 *    which are exported to other Middleware components and applications.
 *         
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_SVL_H_
#define _X_SVL_H_
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_SVL
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "u_svl.h"

/*-------------------------------------------------------------------------
                    functions declarations
 --------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @brief  Create a SVL object and return a handle to this SVL object.
 *  @param  [in]  ps_svl_name       - Name of the SVL object.
 *  @param  [in]  ui2_svl_id        - Unique id for the SVL object.
 *                                    The limit for SVL ID is between 1 and
 *                                    65535.
 *  @param  [in]  e_mem_policy      - Memory policy: SVL_MEM_STATIC or
 *                                    SVL_MEM_DYNAMIC.
 *  @param  [in]  ui2_max_nb_rec    - Specifies the maximum number of SVL record
 *                                    (for SVL_MEM_STATIC), or it specifies
 *                                    number of SVL records per memory chunk.
 *
 *  @param  [in]  pv_tag            - Pointer to a private tag value provided by
 *                                    the caller.  This tag value is passed to
 *                                    the caller's notification function.
 *  @param  [in]  pf_nfy            - Notification function provided by the
 *                                    caller. This function is called to notify
 *                                    the owner of this SVL handle when the SVL
 *                                    object has been modified.  If NULL, then
 *                                    no notification functionwill be called.
 *  @param  [out] ph_svl            - Handle to the SVL object.
 *  @see    x_svl_open
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL object is created successfully.
 *  @retval SVLR_INV_ARG        - 'ui2_svl_id' is 0, or ph_svl is NULL, or
 *                                ps_svl_name is 0.
 *  @retval SVLR_OUT_OF_HANDLE  - Cannot get a handle.
 *  @retval SVLR_FAIL           - The specified 'ps_svl_name' or 'ui2_svl_id'
 *                                value is not unique (e.g., it is currently
 *                                been used), or limit ofthe number of SVL
 *                                objects that can be created is reached.
 *  @retval SVLR_OUT_OF_MEM     - Not enough memory to create SVL object.
 *  @retval SVLR_INV_ARG        - 'ps_svl_name' or 'ph_svl' is NULL.
 *  @retval SVLR_AEE_OUT_OF_RESOURCES   - Can not allocate AEE resources.
 *  @retval SVLR_AEE_NO_RIGHTS  - No right for the AEE resources.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_create
(
    const CHAR*      ps_svl_name,
    UINT16           ui2_svl_id,
    SVL_MEM_POLICY_T e_mem_policy,
    UINT16           ui2_max_nb_rec,
    VOID*            pv_tag,
    x_svl_nfy_fct    pf_nfy,
    HANDLE_T*        ph_svl
);

/*------------------------------------------------------------------*/
/*! @brief  This API deletes the specified SVL object associated
 *          with 'h_svl' handle.    Other clients with a handle to
 *          the deleted SVL object will receive a notification of
 *          the delete event.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @return Return the error code
 *  @retval SVLR_OK             - Specified SVL object is deleted.
 *  @retval SVLR_INV_HANDLE     - Bad handle.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_delete
(
    HANDLE_T    	h_svl
);

/*------------------------------------------------------------------*/
/*! @brief  This API removes all the service records from the
 *          specified SVL object pointed to by the 'h_svl' handle.
 *          All other handles to this SVL object will receive a
 *          notification.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @return Return the error code
 *  @retval SVLR_OK             - All records are successfully removed.
 *  @retval SVLR_INV_HANDLE     - Bad handle.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_clean
(
    HANDLE_T    	h_svl
);

/*------------------------------------------------------------------*/
/*! @brief  This function adds a new service record to the SVL
 *          database object.  The SVL library will make copy of
 *          the record content from the address location pointed to
 *          by the 'pt_svl_rec'.  The 'ui2_svl_rec_id' field in
 *          the 'pt_svl_rec' structure specifies the key field of the
 *          service record to be added to the database.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  pt_svl_rec        - Pointer to the service record structure
 *                                    to be added.
 *  @return Return the error code
 *  @retval SVLR_OK             - Record is successfully added.
 *  @retval SVLR_INV_HANDLE     - Bad SVL handle.
 *  @retval SVLR_OUT_OF_MEM     - Can not allocate memory, add record failed.
 *  @retval SVLR_INV_ARG        - 'pt_svl_rec' is null or the SVL record id is
 *                                invalid.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_add_rec
(
    HANDLE_T           h_svl,
    const SVL_REC_T*   pt_svl_rec
);

/*------------------------------------------------------------------*/
/*! @brief  This function deletes a specified service record from
 *          the SVL.  The record to be deleted is specified by the
 *          service record id.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  ui2_svl_rec_id    - Specifies id the service record to be
 *                                    deleted.
 *  @return Return the error code
 *  @retval SVLR_OK             - Specified record is deleted.
 *  @retval SVLR_INV_HANDLE     - Bad SVL handle.
 *  @retval SVLR_INV_ARG        - ui2_svl_rec_id is a NULL record id.
 *  @retval SVLR_REC_NOT_FOUND  - The specified record to be deleted is not in
 *                                the SVL object.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_del_rec
(
    HANDLE_T           h_svl,
    UINT16             ui2_svl_rec_id
);

/*------------------------------------------------------------------*/
/*! @brief  This function updates a service record to the SVL object.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  pt_svl_rec        - Pointer to service record structure.
 *  @param  [in]  b_must_exist      - Boolean flag specifying if the service
 *                                    record to be updated should exist in the
 *                                    SVL database.
 *  @note   If the key field value (e.g., 'ui2_svl_rec_id' field in
 *          the 'pt_svl_rec' structure) of the input service record
 *          matches an existing record in the SVL object, then the
 *          existing record is over-wrote with the new information,
 *          otherwise a new record is created in the SVL database.
 *          @par
 *          If the 'b_must_exist' flag is TRUE, then there must be an
 *          existing record with the same key field value, else a status
 *          of 'SVLR_REC_NOT_FOUND' is returned to the caller.
 *          If 'b_must_exist' flag is FALSE, then a new service record
 *          is created if no existing record matching the key field value
 *          is found in the SVL database.
 *          @par
 *          Call to this function must be protected, e.g., the SVL
 *          database object needs to be locked before the call to this
 *          function and unlock after returning from call to this function.
 *  @return Return the error code
 *  @retval SVLR_OK             - Record is successfully added/updated.
 *  @retval SVLR_INV_HANDLE     - Bad SVL handle.
 *  @retval SVLR_OUT_OF_MEM     - Not enough memory to create a new SVL record.
 *  @retval SVLR_REC_NOT_FOUND  - The specified record is not found
 *                                ('b_must_exist' == TRUE)
 *  @retval SVLR_INV_ARG        - 'pt_svl_rce' is NULL.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_update_rec
(
    HANDLE_T           h_svl,
    const SVL_REC_T*   pt_svl_rec,
    BOOL               b_must_exist	
);

/*------------------------------------------------------------------*/
/*! @brief  This API queries the range (FIRST, LAST, or NEXT) of
 *          valid service record id values used in the SVL object.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  e_svl_qry_option  - Flag to specifies the record id value to
 *                                    be returned. The values are:
 *                                    SVL_FIRST_REC_ID,  SVL_LAST_REC_ID,
 *                                    SVL_NEXT_REC_ID.
 *  @param  [out] pui2_rec_id       - Contain the record id value.
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL record id return.
 *  @retval SVLR_INV_HANDLE     - Bad SVL handle.
 *  @retval SVLR_INV_ARG        - 'pui2_rec_id' is NULL.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_qry_rec_id
(
    HANDLE_T           h_svl,
    SVL_REC_ID_QRY_T   e_svl_qry_option,
    UINT16*            pui2_rec_id	
);

/*------------------------------------------------------------------*/
/*! @brief  This function locks the SVL object.   The function blocks
 *          until the lock is acquired.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @see    x_svl_unlock, x_svl_read_lock
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL is locked.
 *  @retval SVLR_INV_HANDLE     - Bad SVL handle.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_lock
(
    HANDLE_T           h_svl
);

/*------------------------------------------------------------------*/
/*! @brief  This function unlocks the SVL database.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @see    x_svl_lock
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL database is unlocked.
 *  @retval SVLR_INV_HANDLE     - Bad SVL handle.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_unlock
(
    HANDLE_T           h_svl
);

/*------------------------------------------------------------------*/
/*! @brief  This function locks the SVL object for read operation.
 *          The function blocks until the lock is acquired.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @see    x_svl_read_unlock, x_svl_lock
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL is locked.
 *  @retval SVLR_INV_HANDLE     - Bad SVL handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svl_read_lock
(
    HANDLE_T           h_svl
);

/*------------------------------------------------------------------*/
/*! @brief  This function release read-lock on the SVL database.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @see    x_svl_read_lock
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL database is unlocked.
 *  @retval SVLR_INV_HANDLE     - Bad SVL handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svl_read_unlock
(
    HANDLE_T           h_svl
);

/*------------------------------------------------------------------*/
/*! @brief  This API loads a specified SVL object from file system
 *          into memory, and returns a handle to the newly created
 *          SVL object.
 *  @param  [in]  h_dir             - The handle to the directory.  The
 *                                    'ps_svl_path' will be relative to the
 *                                    specified 'h_dir'.
 *  @param  [in]  ps_svl_path       - Specifies the directory path of the SVL
 *                                    object in the file system. See File Path
 *                                    Specification, 10.1, for the
 *                                    types of file path that can be specified.
 *  @param  [in]  ui2_svl_id        - Specifies the SVL id to load from the file
 *                                    system. The limit for SVL ID is between 1
 *                                    and 65535.
 *  @param  [in]  ps_svl_name       - Specifies the name for the SVL object.  If
 *                                    null string is specified, then the default
 *                                    name stored in the file system is used to
 *                                    create the SVL object.
 *  @param  [in]  pv_tag            - Pointer to a private tag value provided by
 *                                    the caller. This tag value is passed to
 *                                    the caller's notification function.
 *  @param  [in]  pf_nfy            - Notification function provided by the
 *                                    caller.  This function is called to notify
 *                                    the owner of this SVL handle when the SVL
 *                                    object has been modified.  If NULL then no
 *                                    notification function will be called.
 *  @param  [out] ph_svl            - On return, contain the handle to the SVL
 *                                    object created from file system.
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL object is created.
 *  @retval SVLR_NOT_FOUND      - 'ui2_svl_id' is not found or 'ps_svl_path'
 *                                does not exist.
 *  @retval SVLR_OUT_OF_HANDLE  - Cannot get a handle.
 *  @retval SVLR_OUT_OF_MEM     - Not enough memory to create SVL object.
 *  @retval SVLR_FAIL           - The specified 'ps_svl_name' or 'ui2_svl_id'
 *                                value is not unique.
 *  @retval SVLR_INV_ARG        - 'ps_svl_name' or 'ph_svl' or 'ps_svl_path' is
 *                                NULL.
 *  @retval SVLR_INV_FILE_PATH  - Specified file path is not a directory or a
 *                                RAW character file.
 *  @retval SVLR_AEE_OUT_OF_RESOURCES   - Can not allocate AEE resources.
 *  @retval SVLR_AEE_NO_RIGHTS          - No right for the AEE resources.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_fs_load
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_svl_id,
    const CHAR*	    ps_svl_name,
    VOID*			pv_tag,
    x_svl_nfy_fct	pf_nfy,
    HANDLE_T*		ph_svl	
);

/*------------------------------------------------------------------*/
/*! @brief  This API stores the specified SVL object to the file system.
 *  @param  [in]  h_dir             - The handle to the directory.  The
 *                                    'ps_svl_path' will be relative to the
 *                                    specified 'h_dir'.
 *  @param  [in]  ps_svl_path       - Specifies the filename path to store the
 *                                    SVL object in the file system.
 *  @param  [in]  ui2_svl_id        - Specifies the SVL object to be stored to
 *                                    the file system. The limit for SVL ID is
 *                                    between 1 and 65535.
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL object is stored to the file system.
 *  @retval SVLR_NOT_FOUND      - The SVL object specified by the 'ui2_svl_id'
 *                                does not exist.
 *  @retval SVLR_INV_ARG        - `ps_svl_path' is NULL.
 *  @retval SVLR_INV_FILE_PATH  - Specified file path is not a directory or
 *                                a RAW character file.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_fs_store
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_svl_id	
);

/*------------------------------------------------------------------*/
/*! @brief  This API deletes a specified SVL object on the file system.
 *  @param  [in]  h_dir             - The handle to the directory.  The
 *                                    'ps_svl_path' will be relative to the
 *                                    specified 'h_dir'.
 *  @param  [in]  ps_svl_path       - Specifies the filename path of the SVL
 *                                    object in the file system.
 *  @param  [in]  ui2_svl_id        - Specifies the SVL object to be deleted
 *                                    from the file system. The limit for SVL
 *                                    ID is between 1 and 65535.
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL object is deleted from the file system.
 *  @retval SVLR_NOT_FOUND      - 'ui2_svl_id' is not found.
 *  @retval SVLR_INV_ARG        - `ps_svl_path' is NULL.
 *  @retval SVLR_INV_FILE_PATH  - Specified file path, 'ps_svl_path', does not
 *                                exist, or it is not a directory or a RAW
 *                                character file.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_fs_del
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_svl_id
);

/*------------------------------------------------------------------*/
/*! @brief  This API queries the SVL objects stored in file system.
 *          It return the SVL object name and id value.
 *  @param  [in]  h_dir             - The handle to the directory.  The
 *                                    'ps_svl_path' will be relative to the
 *                                    specified 'h_dir'.
 *  @param  [in]  ps_svl_path       - Specifies the directory path where SVL
 *                                    objects are stored.
 *  @param  [in]  ui2_idx           - Index value to get the next SVL object.
 *                                    0 means the first SVL, 1 is the second
 *                                    SVL, etc.
 *  @param  [out] pui2_svl_id       - Pointer to a UINT16 variable. On return,
 *                                    it contains SVL ID.
 *  @param  [out] ps_svl_name       - Pointer to a string buffer (minimum 17
 *                                    bytes) . On return, it contains the
 *                                    default SVL name stored on the file
 *                                    system.
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL object found.
 *  @retval SVLR_NOT_FOUND      - No SVL object found.
 *  @retval SVLR_INV_ARG        - 'ps_svl_name' or 'pui2_svl_id' is NULL.
 *  @retval SVLR_INV_FILE_PATH  - Specified file path is not a directory or a
 *                                RAW character file.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_fs_qry
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_idx,
    UINT16*	   	    pui2_svl_id,
    CHAR*           ps_svl_name	
);

/*------------------------------------------------------------------*/
/*! @brief  This API sync a specified SVL handle with SVL records
 *          from persistent file system.  The existing SVL records
 *          are replaced with new records read from file.
 *  @param  [in]  h_svl             - Specify the handle to the SVL object.
 *  @param  [in]  h_dir             - The handle to the directory.  The
 *                                    'ps_svl_path' will be relative to the
 *                                    specified 'h_dir'.
 *  @param  [in]  ps_svl_path       - Specifies the directory path of the SVL
 *                                    object in the file system.
 *  @param  [in]  ui2_svl_id        - Specifies the SVL id to load from the file
 *                                    system. The limit for SVL ID is between 1
 *                                    and 65535.
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL records are load from the persistent file
 *                                into the memory of SVL object.
 *  @retval SVLR_NOT_FOUND      - 'ui2_svl_id' is not found or 'ps_svl_path'
 *                                does not exist.
 *  @retval SVLR_FAIL           - Internal error
 *  @retval SVLR_INV_ARG        - 'ps_svl_path' is NULL, or 'h_svl' is
 *                                NULL_HANDLE, or ui2_svl_id is 0.
 *  @retval SVLR_INV_FILE_PATH  - Specified file path is not a directory or a
 *                                RAW character file.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_fs_sync
(
    HANDLE_T		h_svl,	
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_svl_id
);

/*------------------------------------------------------------------*/
/*! @brief  This function create a handle to the specified SVL database
 *          object by its ID value.
 *  @param  [in]  ui2_svl_id        - Specifies the SVL object identifier value.
 *                                    The limit for SVL ID is between 1 and
 *                                    65535.
 *  @param  [in]  pv_tag            - A private tag value associated with the
 *                                    calling client.
 *  @param  [in]  pf_nfy            - Pointer to the caller provided
 *                                    notification function. The notification
 *                                    function will be called in the context of
 *                                    the thread that modifies (update, add,
 *                                    delete) the SVL database.  If NULL, then
 *                                    no notification function will be called.
 *
 *  @param  [out] ph_svl            - Handle to the SVL object.
 *  @see    x_svl_open_by_name
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL handle is returned.
 *  @retval SVLR_OUT_OF_HANDLE  - No more handle available.
 *  @retval SVLR_OUT_OF_MEM     - Not enough memory to create the SVL control
 *                                structure.
 *  @retval SVLR_NOT_FOUND      - The specified SVL database object id (e.g.
 *                                'ui2_svl_id') is not found.
 *  @retval SVLR_INV_ARG        - 'ph_svl' is NULL.
 *  @retval SVLR_AEE_OUT_OF_RESOURCES   - Can not allocate AEE resources.
 *  @retval SVLR_AEE_NO_RIGHTS          - No right for the AEE resources.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_open
(
    UINT16		    ui2_svl_id,
    VOID*		    pv_tag,
    x_svl_nfy_fct   pf_nfy,
    HANDLE_T*		ph_svl
);

/*------------------------------------------------------------------*/
/*! @brief  This function creates a handle to the specified SVL database
 *          object by its name.
 *  @param  [in]  ps_svl_name       - Specifies the SVL object name (16 char
 *                                    max).
 *  @param  [in]  pv_tag            - A private tag value associated with the
 *                                    calling client.
 *  @param  [in]  pf_nfy            - Pointer to a caller provided notification
 *                                    function. The notification function is
 *                                    called in the context of the thread that
 *                                    modifies (update, add, delete) the SVL
 *                                    database.  If NULL, then no notification
 *                                    function will be called.
 *  @param  [out] ph_svl            - Handle to the SVL object.
 *  @see    x_svl_open
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL handle is returned.
 *  @retval SVLR_OUT_OF_HANDLE  - No more handle available.
 *  @retval SVLR_OUT_OF_MEM     - Not enough memory to create the SVL control
 *                                structure.
 *  @retval SVLR_NOT_FOUND      - The specified SVL database object name (e.g.,
 *                                ps_svl_name)  is not found.
 *  @retval SVLR_INV_ARG        - 'ph_svl' is NULL.
 *  @retval SVLR_AEE_OUT_OF_RESOURCES   - Can not allocate AEE resources.
 *  @retval SVLR_AEE_NO_RIGHTS          - No right for the AEE resources.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_open_by_name
(
    const CHAR*     ps_svl_name,
    VOID*		    pv_tag,
    x_svl_nfy_fct	pf_nfy,
    HANDLE_T*		ph_svl	
);

/*------------------------------------------------------------------*/
/*! @brief  This function frees the specified handle to the SVL
 *          database object.
 *  @param  [in]  h_svl             - Handle to SVL database object.
 *  @note   The SVL data object that the handle refer to remain active.
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL handle is freed.
 *  @retval SVLR_INV_HANDLE     - Bad handle.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_close
(
    HANDLE_T                  h_svl
);

/*------------------------------------------------------------------*/
/*! @brief  This function gets the SVL ID and SVL name for the
 *          specified handle.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [out] pui2_svl_id       - Pointer to SVL ID variable.  On return it
 *                                    contains the SVL ID value for this handle.
 *  @param  [out] ps_svl_name       - Pointer to a char array variable (the size
 *                                    of this char array should be at least
 *                                    MAX_SVL_NAME_LEN+1).  On return, this
 *                                    array contains the name of the SVL object.
 *  @return Return the error code
 *  @retval SVLR_OK             - Information about this handle (svl id and svl
 *                                name) is returned.
 *  @retval SVLR_INV_HANDLE     - Bad handle.
 *  @retval SVLR_INV_ARG        - 'pui2_svl_id' or 'ps_svl_name' is NULL.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_get_handle_info
(
    HANDLE_T		h_svl,
    UINT16*			pui2_svl_id,
    CHAR*			ps_svl_name
);

/*------------------------------------------------------------------*/
/*! @brief  This API gets a specific record stored in the SVL database
 *          object.    The search parameter is specified by the
 *          'ui2_svl_rec_id' value.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  ui2_svl_rec_id    - Specifies the service record id.
 *  @param  [out] pt_svl_rec        - Pointer to a service record structure.
 *                                    This structure must be allocated by the
 *                                    caller.  On return, it is filled with
 *                                    service record information.
 *  @param  [out] pui4_ver_id       - Pointer to a variable holding version id
 *                                    of the database object.   Note: version id
 *                                    of the SVL database object is incremented
 *                                    each time when a modification takes place.
 *  @return Return the error code
 *  @retval SVLR_OK             - Data from the specified record is returned.
 *  @retval SVLR_INV_HANDLE     - Bad handle.
 *  @retval SVLR_REC_NOT_FOUND  - No record matching the search parameter
 *                                ('ui2_svl_rec_id')  is found.
 *  @retval SVLR_INV_ARG        - 'pui4_ver_id' or 'pt_svl_rec' is NULL.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_get_rec
(
    HANDLE_T            h_svl,
    UINT16			    ui2_svl_rec_id,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id
);

/*----------------------------------------------------------------------
 * Name: x_svl_get_rec_by_rc_key
 *
 * Description: This API returns service record entry with a specified 
 *              remote_control_key_id. Only used for ISDB protocol
 *
 * Inputs: 
 *   h_svl          Handle to the SVL object.
 *   ui1_rc_key_id  Specifies the remote_control_key_id
 *
 * Outputs: None
 *   pt_svl_rec	  Pointer to a SVL record structure.
 *
 * Input/Output arguments
 *   pui4_ver_id  Pointer to a variable holding version id of the SVL 
 *                database object.  On entry, this variable contains 
 *                the version id of the database that the caller expects.  
 *                On the return, this variable contains the current version 
 *                id of the database.   If the expected version id and 
 *                current database version id do not match, an error status 
 *                code is returned.  If 'pui4_ver_id' is set to 
 *                'SVL_NULL_VER_ID' value, then no checking is done and 
 *                the latest version id is returned the application. 
 * Returns: 
 *   SVLR_OK             A SVL_REC_T structure is returned. 
 *   SVLR_INV_HANDLE     Bad 'h_svl' handle.
 *   SVLR_REC_NOT_FOUND  Specified record is not found.
 *   SVLR_INVALID_ARG    'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *   SVLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id' 
 *                       pass into the API and the internal database version 
 *                       id is not the same.
 *   
 -----------------------------------------------------------------------*/
extern INT32 x_svl_get_rec_by_rc_key
(
    HANDLE_T           h_svl,
    UINT8              ui1_rc_key,
    SVL_REC_T*         pt_svl_rec,
    UINT32*            pui4_ver_id
);
/*------------------------------------------------------------------*/
/*! @brief  This API returns service record entry with a specified
 *          source id value and network mask parameter from the SVL
 *          database object.  If there are multiple service records
 *          matching the specified channel number, the caller can use
 *          'ui2_idx' to iterate over service records with the same
 *          channel number.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  ui2_source_id     - Specifies the source id of the service
 *                                    records to be returned.
 *  @param  [in]  ui4_nw_mask       - 32-bits mask specifying the network(s) of
 *                                    the service record to be returned.
 *  @param  [in]  ui2_idx           - Index for iterating the SVL for service
 *                                    records matching the specified channel
 *                                    number.  0 means the first record found in
 *                                    SVL list that matches the specified
 *                                    channel number and network mask parameter,
 *                                    and 1 is the next record found, and so on.
 *  @param  [out] pt_svl_rec        - Pointer to a SVL record structure.
 *  @param  [in,out]  pui4_ver_id   - Pointer to a variable holding version id
 *                                    of the SVL database object.  On entry,
 *                                    this variable contains the version id of
 *                                    the database that the caller expects. On
 *                                    the return, this variable contains the
 *                                    current version id of the database.   If
 *                                    the expected version id and current
 *                                    database version id do not match, an error
 *                                    status code is returned.  If 'pui4_ver_id'
 *                                    is set to 'SVL_NULL_VER_ID' value, then no
 *                                    checking is done and the latest version id
 *                                    is returned the application.
 *  @return Return the error code
 *  @retval SVLR_OK             - A SVL_REC_T structure is returned.
 *  @retval SVLR_INV_HANDLE     - Bad 'h_svl' handle.
 *  @retval SVLR_REC_NOT_FOUND  - Specified record is not found.
 *  @retval SVLR_INVALID_ARG    - 'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *  @retval SVLR_MODIFIED       - The database has been modified.  The
 *                                'pui4_ver_id' passed into the API and the
 *                                internal database version id are not the same.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svl_get_rec_by_src_id
(
    HANDLE_T           h_svl,
    UINT16             ui2_source_id,
    UINT32		       ui4_nw_mask,
    UINT16		       ui2_idx,
    SVL_REC_T*         pt_svl_rec,
    UINT32*            pui4_ver_id
);

/*------------------------------------------------------------------*/
/*! @brief  This API returns service record entry with a specified
 *          channel number and network mask parameter from the SVL
 *          database object.  If there are multiple service records
 *          matching the specified channel number, the caller can use
 *          'ui2_idx' to iterate over service records with the same
 *          channel number.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  ui4_channel_num   - Specifies the channel number of the
 *                                    service records to be returned.
 *  @param  [in]  ui4_nw_mask       - 32-bits mask specifying the network(s) of
 *                                    the service record to be returned.
 *  @param  [in]  ui2_idx           - Index for iterating the SVL for service
 *                                    records matching the specified channel
 *                                    number.  0 means the first record found in
 *                                    SVL list that matches the specified
 *                                    channel number and network mask parameter,
 *                                    and 1 is the next record found, and so on.
 *  @param  [out] pt_svl_rec        - Pointer to a SVL record structure.
 *  @param  [in,out]  pui4_ver_id   - Pointer to a variable holding version id
 *                                    of the SVL database object.  On entry,
 *                                    this variable contains the version id of
 *                                    the database that the caller expects. On
 *                                    the return, this variable contains the
 *                                    current version id of the database.   If
 *                                    the expected version id and current
 *                                    database version id do not match, an error
 *                                    status code is returned.  If 'pui4_ver_id'
 *                                    is set to 'SVL_NULL_VER_ID' value, then no
 *                                    checking is done and the latest version id
 *                                    is returned the application.
 *  @see    x_svl_get_num_rec_by_channel
 *  @return Return the error code
 *  @retval SVLR_OK             - A SVL_REC_T structure is returned.
 *  @retval SVLR_INV_HANDLE     - Bad 'h_svl' handle.
 *  @retval SVLR_REC_NOT_FOUND  - Specified record is not found.
 *  @retval SVLR_INVALID_ARG    - 'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *  @retval SVLR_MODIFIED       - The database has been modified.  The
 *                                'pui4_ver_id' passed into the API and the
 *                                internal database version id are not the same.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_get_rec_by_channel
(
    HANDLE_T			h_svl,
    UINT32			    ui4_channel_num,
    UINT32			    ui4_nw_mask,
    UINT16			    ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
);

/*------------------------------------------------------------------*/
/*! @brief  This function returns service record with a specified
 *          broadcast type (e.g., BRDCST_TYPE_ANALOG, BRDCST_TYPE_ATSC,
 *          or BRDCST_TYPE_DVB) and network.  If there are multiple
 *          service records matching the specified broadcast type, the client
 *          can use ui2_idx to iterate over service records with the same
 *          broadcast type.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  e_brdcst_type     - Specifies the broadcast type (ANALOG, DVB,
 *                                    or ATSC) of the service records to be
 *                                    returned.
 *  @param  [in]  ui4_nw_mask       - 32-bits mask specifying the network(s) of
 *                                    the service record to be returned.
 *                                    The 'ui4_nw_mask' value is compared with
 *                                    the service record's network value using
 *                                    bit-wise AND operation. The comparison is
 *                                    considered a positive match if the result
 *                                    is greater than 0.  For example, if
 *                                    SATELLITE, CABLE, and TERRESTRIAL networks
 *                                    are defined to be 0x01, 0x02 and 0x04.
 *                                    Then specifying 'ui4_nw_mask' of 0x04 will
 *                                    match service records with internal
 *                                    network mask values of 0x04, 0x06, and
 *                                    0x07.
 *  @param  [in]  ui2_idx           - Index for iterating the SVL for service
 *                                    records matching the specified broadcast
 *                                    type.  0 means the first record found that
 *                                    matches the specified broadcast type
 *                                    and network mask parameter, and 1 is the
 *                                    next record found, and so on.
 *  @param  [out] pt_svl_rec        - Pointer to a SVL record structure.
 *  @param  [in,out]  pui4_ver_id   - Pointer to a variable holding version id
 *                                    of the SVL database object.  On entry,
 *                                    this variable contains the version id of
 *                                    the database that the caller expects. On
 *                                    the return, this variable contains the
 *                                    current version id of the database.   If
 *                                    the expected version id and current
 *                                    database version id do not match, an error
 *                                    status code is returned.  If 'pui4_ver_id'
 *                                    is set to 'SVL_NULL_VER_ID' value, then no
 *                                    checking is done and the latest version id
 *                                    is returned the application.
 *  @see    x_svl_get_num_rec_by_brdcst_type
 *  @return Return the error code
 *  @retval SVLR_OK             - A SVL_REC_T structure is returned.
 *  @retval SVLR_INV_HANDLE     - Bad 'h_svl' handle.
 *  @retval SVLR_REC_NOT_FOUND  - Specified record is not found.
 *  @retval SVLR_INVALID_ARG    - 'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *  @retval SVLR_MODIFIED       - The database has been modified.  The
 *                                'pui4_ver_id' passed into the API and the
 *                                internal database version id are not the same.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_get_rec_by_brdcst_type
(
    HANDLE_T			h_svl,
    BRDCST_TYPE_T		e_brdcst_type,
    UINT32			    ui4_nw_mask,
    UINT16			    ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
);

/*------------------------------------------------------------------*/
/*! @brief  This function searches the SVL and returns service record
 *          with a specified Transport Stream List ID, Transport Stream
 *          record ID (e.g., TSL ID and TSL record ID) and network mask
 *          parameter.  If there are multiple service records matching
 *          the specified TSL ID, TSL record ID, and network mask, the
 *          caller can use ui2_idx to iterate over service records with
 *          the positive match.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  ui2_tsl_id        - Specifies the Transport Stream List ID
 *                                    value.
 *  @param  [in]  ui2_tsl_rec_id    - Specifies the Transport Stream record ID
 *                                    value.
 *  @param  [in]  ui4_nw_mask       - 32-bits mask specifying the network(s) of
 *                                    the service record to be returned.
 *                                    The 'ui4_nw_mask' value is compared with
 *                                    the service record's network value using
 *                                    bit-wise AND operation. The comparison is
 *                                    considered a positive match if the result
 *                                    is greater than 0.  For example, if
 *                                    SATELLITE, CABLE, and TERRESTRIAL networks
 *                                    are defined to be 0x01, 0x02 and 0x04.
 *                                    Then specifying 'ui4_nw_mask' of 0x04 will
 *                                    match service records with internal
 *                                    network mask values of 0x04, 0x06, and
 *                                    0x07.
 *  @param  [in]  ui2_idx           - Index for iterating the SVL for service
 *                                    records matching the specified TS.  0
 *                                    means the first record found that matches
 *                                    the specified TS and network mask
 *                                    parameter, and 1 is the next record found,
 *                                    and so on.
 *  @param  [out] pt_svl_rec        - Pointer to a SVL record structure.
 *  @param  [in,out]  pui4_ver_id   - Pointer to a variable holding version id
 *                                    of the SVL database object.  On entry,
 *                                    this variable contains the version id of
 *                                    the database that the caller expects. On
 *                                    the return, this variable contains the
 *                                    current version id of the database.   If
 *                                    the expected version id and current
 *                                    database version id do not match, an error
 *                                    status code is returned.  If 'pui4_ver_id'
 *                                    is set to 'SVL_NULL_VER_ID' value, then no
 *                                    checking is done and the latest version id
 *                                    is returned the application.
 *  @note   the combination of TSL ID and TSL record ID uniquely
 *          identifies a transport stream record within the Middleware.
 *          @par
 *          If another caller locks (e.g., to perform write operation)
 *          the SVL object reference by the 'h_svl' handle, then this
 *          function will block until the other caller has unlock the
 *          SVL object.
 *  @see    x_svl_get_num_rec_by_ts
 *  @return Return the error code
 *  @retval SVLR_OK             - A SVL_REC_T structure is returned.
 *  @retval SVLR_INV_HANDLE     - Bad 'h_svl' handle.
 *  @retval SVLR_REC_NOT_FOUND  - Specified record is not found.
 *  @retval SVLR_INVALID_ARG    - 'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *  @retval SVLR_MODIFIED       - The database has been modified.  The
 *                                'pui4_ver_id' passed into the API and the
 *                                internal database version id are not the same.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_get_rec_by_ts
(
    HANDLE_T			h_svl,
    UINT16			    ui2_tsl_id,
    UINT16			    ui2_tsl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16		  	    ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
);

/*------------------------------------------------------------------*/
/*! @brief  This function searches the SVL and returns service record
 *          with a specified Network List ID, Network record ID (e.g.,
 *          NWL ID and NWL record ID) and network mask parameter.  If
 *          there are multiple service records matching the specified
 *          NWL ID, NWL record ID, and network mask, the caller can use
 *          ui2_idx to iterate over service records with the positive
 *          match.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  ui2_nwl_id        - Specifies the Network List ID value.
 *  @param  [in]  ui2_nwl_rec_id    - Specifies the Network record ID value.
 *  @param  [in]  ui4_nw_mask       - 32-bits mask specifying the network(s) of
 *                                    the service record to be returned.
 *                                    The 'ui4_nw_mask' value is compared with
 *                                    the service record's network value using
 *                                    bit-wise AND operation. The comparison is
 *                                    considered a positive match if the result
 *                                    is greater than 0.  For example, if
 *                                    SATELLITE, CABLE, and TERRESTRIAL networks
 *                                    are defined to be 0x01, 0x02 and 0x04.
 *                                    Then specifying 'ui4_nw_mask' of 0x04 will
 *                                    match service records with internal
 *                                    network mask values of 0x04, 0x06, and
 *                                    0x07.
 *  @param  [in]  ui2_idx           - Index for iterating the SVL for service
 *                                    records matching the specified TS.  0
 *                                    means the first record found that matches
 *                                    the specified TS and network mask
 *                                    parameter, and 1 is the next record found,
 *                                    and so on.
 *  @param  [out] pt_svl_rec        - Pointer to a SVL record structure.
 *  @param  [in,out]  pui4_ver_id   - Pointer to a variable holding version id
 *                                    of the SVL database object.  On entry,
 *                                    this variable contains the version id of
 *                                    the database that the caller expects. On
 *                                    the return, this variable contains the
 *                                    current version id of the database.   If
 *                                    the expected version id and current
 *                                    database version id do not match, an error
 *                                    status code is returned.  If 'pui4_ver_id'
 *                                    is set to 'SVL_NULL_VER_ID' value, then no
 *                                    checking is done and the latest version id
 *                                    is returned the application.
 *  @note   the combination of NWL ID and NWL record ID uniquely
 *          identifies a network record within the Middleware.
 *          @par
 *          If another caller locks (e.g., to perform write operation)
 *          the SVL object reference by the 'h_svl' handle, then this
 *          function will block until the other caller has unlock the
 *          SVL object.
 *  @see    x_svl_get_num_rec_by_nw
 *  @return Return the error code
 *  @retval SVLR_OK             - A SVL_REC_T structure is returned.
 *  @retval SVLR_INV_HANDLE     - Bad 'h_svl' handle.
 *  @retval SVLR_REC_NOT_FOUND  - Specified record is not found.
 *  @retval SVLR_INVALID_ARG    - 'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *  @retval SVLR_MODIFIED       - The database has been modified.  The
 *                                'pui4_ver_id' passed into the API and the
 *                                internal database version id are not the same.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_get_rec_by_nw
(
    HANDLE_T			h_svl,
    UINT16			    ui2_nwl_id,
    UINT16			    ui2_nwl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16		  	    ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
);

/*------------------------------------------------------------------*/
/*! @brief  This function searches the SVL and returns service record
 *          with a specified Satellite List ID, Satellite record ID
 *          (e.g., SATL ID and SATL record ID) and network mask
 *          parameter.  If there are multiple service records matching
 *          the specified SATL ID, SATL record ID, and network mask, the
 *          caller can use ui2_idx to iterate over service records with
 *          the positive match.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  ui2_satl_id       - Specifies the Satellite List ID value.
 *  @param  [in]  ui2_satl_rec_id   - Specifies the Satellite record ID value.
 *  @param  [in]  ui4_nw_mask       - 32-bits mask specifying the network(s) of
 *                                    the service record to be returned.
 *                                    The 'ui4_nw_mask' value is compared with
 *                                    the service record's network value using
 *                                    bit-wise AND operation. The comparison is
 *                                    considered a positive match if the result
 *                                    is greater than 0.  For example, if
 *                                    SATELLITE, CABLE, and TERRESTRIAL networks
 *                                    are defined to be 0x01, 0x02 and 0x04.
 *                                    Then specifying 'ui4_nw_mask' of 0x04 will
 *                                    match service records with internal
 *                                    network mask values of 0x04, 0x06, and
 *                                    0x07.
 *  @param  [in]  ui2_idx           - Index for iterating the SVL for service
 *                                    records matching the specified TS.  0
 *                                    means the first record found that matches
 *                                    the specified TS and network mask
 *                                    parameter, and 1 is the next record found,
 *                                    and so on.
 *  @param  [out] pt_svl_rec        - Pointer to a SVL record structure.
 *  @param  [in,out]  pui4_ver_id   - Pointer to a variable holding version id
 *                                    of the SVL database object.  On entry,
 *                                    this variable contains the version id of
 *                                    the database that the caller expects. On
 *                                    the return, this variable contains the
 *                                    current version id of the database.   If
 *                                    the expected version id and current
 *                                    database version id do not match, an error
 *                                    status code is returned.  If 'pui4_ver_id'
 *                                    is set to 'SVL_NULL_VER_ID' value, then no
 *                                    checking is done and the latest version id
 *                                    is returned the application.
 *  @note   the combination of SATL ID and SATL record ID uniquely
 *          identifies a Satellite record within the Middleware.
 *          @par
 *          If another caller locks (e.g., to perform write operation)
 *          the SVL object reference by the 'h_svl' handle, then this
 *          function will block until the other caller has unlock the
 *          SVL object.
 *  @see    x_svl_get_num_rec_by_sat
 *  @return Return the error code
 *  @retval SVLR_OK             - A SVL_REC_T structure is returned.
 *  @retval SVLR_INV_HANDLE     - Bad 'h_svl' handle.
 *  @retval SVLR_REC_NOT_FOUND  - Specified record is not found.
 *  @retval SVLR_INVALID_ARG    - 'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *  @retval SVLR_MODIFIED       - The database has been modified.  The
 *                                'pui4_ver_id' passed into the API and the
 *                                internal database version id are not the same.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_get_rec_by_sat
(
    HANDLE_T			h_svl,
    UINT16			    ui2_satl_id,
    UINT16			    ui2_satl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16		  	    ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
);

/*------------------------------------------------------------------*/
/*! @brief  Return the number of SVL records matching the channel
 *          and network mask values.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  ui4_channel_num   - Specifies the channel number.
 *  @param  [in]  ui4_nw_mask       - 32-bits mask specifying the network(s) of
 *                                    the service record to be counted.  The
 *                                    'ui4_nw_mask' value is compared with the
 *                                    service record's network value using
 *                                    bit-wise AND operation.  The comparison is
 *                                    considered a positive match if the result
 *                                    is greater than 0.  For example, if
 *                                    SATELLITE, CABLE, and TERRESTRIAL networks
 *                                    are defined to be 0x01, 0x02 and 0x04.
 *                                    Then specifying 'ui4_nw_mask' of 0x04 will
 *                                    match service records with internal
 *                                    network mask values of 0x04, 0x06, and
 *                                    0x07.
 *  @param  [out] pui2_num_recs     - number of records.
 *  @param  [out] pui4_ver_id       - version id.
 *  @see    x_svl_get_rec_by_channel
 *  @return Return the error code
 *  @retval SVLR_OK             - Number of records found.
 *  @retval SVLR_INV_HANDLE     - Bad 'h_svl' handle.
 *  @retval SVLR_INV_ARG        - 'pui2_num_recs' or 'pui4_ver_id' is a NULL
 *                                pointer.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_get_num_rec_by_channel
(
    HANDLE_T			h_svl,
    UINT32			    ui4_channel_num,
    UINT32			    ui4_nw_mask,
    UINT16*			    pui2_num_recs,
    UINT32*			    pui4_ver_id
);

/*------------------------------------------------------------------*/
/*! @brief  Return the number of SVL records matching the broadcast
 *          type ((e.g., BRDCST_TYPE_ANALOG, BRDCST_TYPE_ATSC, or
 *          BRDCST_TYPE_DVB) and network mask values.
 *
 *          If specified broadcast type is BRDCST_TYPE_UNKNOWN, then it
 *          matches any broadcast types.
 *
 *          To get the number of all the records in the SVL, caller can
 *          specifies BRDCST_TYPE_UNKNOWN and 'network mask' parameter
 *          of 0xFFFF.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  e_brdcst_type     - Specifies the broadcast type (e.g.,
 *                                    BRDCST_TYPE_ANALOG, BRDCST_TYPE_ATSC,
 *                                    BRDCST_TYPE_DVB, or BRDCST_TYPE_UNKNOWN)
 *                                    of the service record to be counted.
 *  @param  [in]  ui4_nw_mask       - 32-bits mask specifying the network(s) of
 *                                    the service record to be counted.  The
 *                                    'ui4_nw_mask' value is compared with the
 *                                    service record's network value using
 *                                    bit-wise AND operation.  The comparison is
 *                                    considered a positive match if the result
 *                                    is greater than 0.  For example, if
 *                                    SATELLITE, CABLE, and TERRESTRIAL networks
 *                                    are defined to be 0x01, 0x02 and 0x04.
 *                                    Then specifying 'ui4_nw_mask' of 0x04 will
 *                                    match service records with internal
 *                                    network mask values of 0x04, 0x06, and
 *                                    0x07.
 *  @param  [out] pui2_num_recs     - Pointer to a variable for the number of
 *                                    records matching the specified stream type
 *                                    in the SVL.
 *  @param  [out] pui4_ver_id       - Pointer to a variable holding version id
 *                                    of the SVL database object.
 *  @see    x_svl_get_rec_by_brdcst_type
 *  @return Return the error code
 *  @retval SVLR_OK             - Number of records found is returned.
 *  @retval SVLR_INV_HANDLE     - Bad 'h_svl' handle.
 *  @retval SVLR_INV_ARG        - 'pui2_num_recs' or 'pui4_ver_id' is a NULL
 *                                pointer.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_get_num_rec_by_brdcst_type
(
    HANDLE_T			h_svl,
    BRDCST_TYPE_T		e_brdcst_type,
    UINT32			    ui4_nw_mask,
    UINT16*			    pui2_num_recs,
    UINT32*			    pui4_ver_id
);

/*------------------------------------------------------------------*/
/*! @brief  This function returns the number of records for the
 *          specified Transport Stream List ID, Transport Stream record
 *          ID (e.g., TSL ID and TSL record ID), and network mask
 *          parameter.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  ui2_tsl_id        - Specifies the Transport Stream List ID
 *                                    value.
 *  @param  [in]  ui2_tsl_rec_id    - Specifies the Transport Stream record ID
 *                                    value.
 *  @param  [in]  ui4_nw_mask       - 32-bits mask specifying the network(s) of
 *                                    the service record to be counted.  The
 *                                    'ui4_nw_mask' value is compared with the
 *                                    service record's network value using
 *                                    bit-wise AND operation.  The comparison is
 *                                    considered a positive match if the result
 *                                    is greater than 0.  For example, if
 *                                    SATELLITE, CABLE, and TERRESTRIAL networks
 *                                    are defined to be 0x01, 0x02 and 0x04.
 *                                    Then specifying 'ui4_nw_mask' of 0x04 will
 *                                    match service records with internal
 *                                    network mask values of 0x04, 0x06, and
 *                                    0x07.
 *  @param  [out] pui2_num_recs     - Pointer to a variable for the number of
 *                                    records matching the specified stream type
 *                                    in the SVL.
 *  @param  [out] pui4_ver_id       - Pointer to a variable holding version id
 *                                    of the SVL database object.
 *  @note   The combination of TSL ID and TSL record ID uniquely
 *          identifies a transport stream record within the Middleware.
 *  @see    x_svl_get_rec_by_ts
 *  @return Return the error code
 *  @retval SVLR_OK             - Number of records found.
 *  @retval SVLR_INV_HANDLE     - Bad 'h_svl' handle.
 *  @retval SVLR_INV_ARG        - 'pui2_num_recs' or 'pui4_ver_id' is a NULL
 *                                pointer.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_get_num_rec_by_ts
(
    HANDLE_T			h_svl,
    UINT16              ui2_tsl_id,
    UINT16              ui2_tsl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_ver_id
);

/*------------------------------------------------------------------*/
/*! @brief  This function returns the number of records for the
 *          specified Network List ID, Network record ID (e.g., NWL ID
 *          and NWL record ID), and network mask parameter.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  ui2_nwl_id        - Specifies the Network List ID value.
 *  @param  [in]  ui2_nwl_rec_id    - Specifies the Network record ID value.
 *  @param  [in]  ui4_nw_mask       - 32-bits mask specifying the network(s) of
 *                                    the service record to be counted.  The
 *                                    'ui4_nw_mask' value is compared with the
 *                                    service record's network value using
 *                                    bit-wise AND operation.  The comparison is
 *                                    considered a positive match if the result
 *                                    is greater than 0.  For example, if
 *                                    SATELLITE, CABLE, and TERRESTRIAL networks
 *                                    are defined to be 0x01, 0x02 and 0x04.
 *                                    Then specifying 'ui4_nw_mask' of 0x04 will
 *                                    match service records with internal
 *                                    network mask values of 0x04, 0x06, and
 *                                    0x07.
 *  @param  [out] pui2_num_recs     - Pointer to a variable for the number of
 *                                    records matching the specified stream type
 *                                    in the SVL.
 *  @param  [out] pui4_ver_id       - Pointer to a variable holding version id
 *                                    of the SVL database object.
 *  @note   The combination of NWL ID and NWL record ID uniquely
 *          identifies a network record within the Middleware.
 *  @see    x_svl_get_rec_by_nw
 *  @return Return the error code
 *  @retval SVLR_OK             - Number of records found.
 *  @retval SVLR_INV_HANDLE     - Bad 'h_svl' handle.
 *  @retval SVLR_INV_ARG        - 'pui2_num_recs' or 'pui4_ver_id' is a NULL
 *                                pointer.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_get_num_rec_by_nw
(
    HANDLE_T			h_svl,
    UINT16              ui2_nwl_id,
    UINT16              ui2_nwl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_ver_id
);

/*------------------------------------------------------------------*/
/*! @brief  This function returns the number of records for the
 *          specified Satellite List ID, Satellite record ID (e.g., SATL
 *          ID and SATL record ID), and network mask parameter.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  ui2_satl_id       - Specifies the Satellite List ID value.
 *  @param  [in]  ui2_satl_rec_id   - Specifies the Satellite record ID value.
 *  @param  [in]  ui4_nw_mask       - 32-bits mask specifying the network(s) of
 *                                    the service record to be counted.  The
 *                                    'ui4_nw_mask' value is compared with the
 *                                    service record's network value using
 *                                    bit-wise AND operation.  The comparison is
 *                                    considered a positive match if the result
 *                                    is greater than 0.  For example, if
 *                                    SATELLITE, CABLE, and TERRESTRIAL networks
 *                                    are defined to be 0x01, 0x02 and 0x04.
 *                                    Then specifying 'ui4_nw_mask' of 0x04 will
 *                                    match service records with internal
 *                                    network mask values of 0x04, 0x06, and
 *                                    0x07.
 *  @param  [out] pui2_num_recs     - Pointer to a variable for the number of
 *                                    records matching the specified stream type
 *                                    in the SVL.
 *  @param  [out] pui4_ver_id       - Pointer to a variable holding version id
 *                                    of the SVL database object.
 *  @note   The combination of SATL ID and SATL record ID uniquely
 *          identifies a satellite record within the Middleware.
 *  @see    x_svl_get_rec_by_sat
 *  @return Return the error code
 *  @retval SVLR_OK             - Number of records found.
 *  @retval SVLR_INV_HANDLE     - Bad 'h_svl' handle.
 *  @retval SVLR_INV_ARG        - 'pui2_num_recs' or 'pui4_ver_id' is a NULL
 *                                pointer.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_get_num_rec_by_sat
(
    HANDLE_T			h_svl,
    UINT16              ui2_satl_id,
    UINT16              ui2_satl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_ver_id
);

/*----------------------------------------------------------------------
 * Name: x_svl_get_total_num_recs
 *
 * Description: This API get total records number from all svl 
 * Inputs: 
 *    void
 *
 * Outputs: 
 *      
 
 * Returns: 
 *   total number            
 *   
 -----------------------------------------------------------------------*/
extern
UINT32 x_svl_get_total_num_recs(HANDLE_T    h_svl);


/*------------------------------------------------------------------*/
/*! @brief  This API iterates over service records in the SVL list.
 *          It returns the next service record from a specified the
 *          channel number of the current service record location.
 *          The 'e_iter_dir' argument specifies direction of iteration,
 *          e.g., the next record's channel numeric value is plus or
 *          minus from the current record's channel numeric value.
 *          Service records that do not match the network mask
 *          specification will be skipped during the iteration.
 *
 *          If the current record is at the end or beginning of the
 *          service list, then 'CHANNEL_PLUS' or 'CHANNEL_MINUS' will
 *          return a status to indicate that end-point is reached. It is
 *          up the caller to decide if next service record should wrap
 *          around to the beginning service record or to the end of the
 *          service record.
 *
 *          Since the service record in the SVL is sorted by 'channel
 *          number', the iteration can be thought of as getting the next
 *          or the previous channel.  For ATSC broadcast type, this
 *          implies the returned records will be sorted by major number
 *          and minor number, in that order.
 *
 *          Specifying 'CHANNEL_SMALLEST' returns the record with the
 *          smallest channel number (regardless of the current service
 *          record position), and similarly specifying 'CHANNEL_LARGEST'
 *          returns the record with the largest channel number.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  ui4_current_channel   - Specifies the channel number of the
 *                                        current service record to begin for
 *                                        the search for the next service
 *                                        record.
 *  @param  [in]  ui4_nw_mask       - 32-bits mask specifying the network(s) of
 *                                    the service record to be returned.
 *  @param  [in]  e_iter_dir        - Specifies the direction of the iteration.
 *                                    The values are:
 *                                    SVL_CHANNEL_PLUS,
 *                                    SVL_CHANNEL_MINUS,
 *                                    SVL_CHANNEL_SMALLEST,
 *                                    SVL_CHANNEL_LARGEST.
 *                                    If SVL_CHANNEL_SMALLEST or
 *                                    SVL_CHANNEL_LARGEST is specified, then the
 *                                    record with the smallest (numerically)
 *                                    channel value is returned, and vice versa.
 *                                    If the iteration has reach an end point,
 *                                    i.e., the current record is point to the
 *                                    first or last service record, then the
 *                                    current service record is returned.
 *  @param  [out] pt_svl_rec        - Pointer to a SVL record structure.
 *  @param  [in,out]  pui4_ver_id   - Pointer to a variable holding version id
 *                                    of the SVL database object.  On entry,
 *                                    this variable contains the version id of
 *                                    the database that the caller expects.  On
 *                                    the return, this variable contains the
 *                                    current version id of the database.   If
 *                                    the expected version id and current
 *                                    database version id do not match, an error
 *                                    status code is returned.  If 'pui4_ver_id'
 *                                    is set to 'SVL_NULL_VER_ID' value, then no
 *                                    checking is done and the latest version id
 *                                    is returned the application.
 *  @see    x_svl_iterate_rec_by_custom_ord
 *  @return Return the error code
 *  @retval SVLR_OK             - A SVL_REC_T structure is returned.
 *  @retval SVLR_INV_HANDLE     - Invalid `h_svl' handl
 *  @retval SVLR_INV_ARG        - `pui4_ver_id' or `pt_svl_rec' is N
 *  @retval SVLR_END_OF_ITERATION - The iteration has reach an end-point.  The
 *                                end-points is either at the record with
 *                                smallest or the largest channel number.
 *  @retval SVLR_MODIFIED       - The database has bee modified.  The
 *                                `pui4_ver_id' pass into the API and the
 *                                internal database version id is not the same.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_iterate_rec
(
    HANDLE_T			h_svl,
    UINT32              ui4_current_channel,
    UINT32              ui4_nw_mask,
    SVL_ITERATE_T		e_iter_dir,  /* channel + or - */
    SVL_REC_T*			pt_svl_rec,
    UINT32*             pui4_ver_id 
);

/*------------------------------------------------------------------*/
/*! @brief  This API iterates over service records in the SVL list with
 *          reference to the given NWL record.  It returns the next
 *          matching service record from a specified the channel number
 *          of the current service record location.  
 *          The 'e_iter_dir' argument specifies direction of iteration, 
 *          e.g., the next record's channel numeric value is plus or 
 *          minus from the current record's channel numeric value.  
 *          Service records that do not match the network mask and NWL
 *          ID / NWL record ID specified will be skipped during the
 *          iteration.  
 *
 *          If the current record is at the end or beginning of the
 *          service list, then 'CHANNEL_PLUS' or 'CHANNEL_MINUS' will
 *          return a status to indicate that end-point is reached. It is
 *          up the caller to decide if next service record should wrap
 *          around to the beginning service record or to the end of the
 *          service record.
 *
 *          Since the service record in the SVL is sorted by 'channel
 *          number', the iteration can be thought of as getting the next
 *          or the previous channel in the specified network.
 *
 *          Specifying 'CHANNEL_SMALLEST' returns the record with the
 *          smallest channel number (regardless of the current service
 *          record position), and similarly specifying 'CHANNEL_LARGEST'
 *          returns the record with the largest channel number.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  ui4_current_channel   - Specifies the channel number of the
 *                                        current service record to begin for
 *                                        the search for the next service
 *                                        record.
 *  @param  [in]  ui4_nw_mask       - 32-bits mask specifying the network(s) of
 *                                    the service record to be returned.
 *  @param  [in]  ui2_nwl_id        - Specifies the Network List ID value.
 *  @param  [in]  ui2_nwl_rec_id    - Specifies the Network record ID value.
 *  @param  [in]  e_iter_dir        - Specifies the direction of the iteration.
 *                                    The values are:
 *                                    SVL_CHANNEL_PLUS,
 *                                    SVL_CHANNEL_MINUS,
 *                                    SVL_CHANNEL_SMALLEST,
 *                                    SVL_CHANNEL_LARGEST.
 *                                    If SVL_CHANNEL_SMALLEST or
 *                                    SVL_CHANNEL_LARGEST is specified, then the
 *                                    record with the smallest (numerically)
 *                                    channel value is returned, and vice versa.
 *                                    If the iteration has reach an end point,
 *                                    i.e., the current record is point to the
 *                                    first or last service record, then the
 *                                    current service record is returned.
 *  @param  [out] pt_svl_rec        - Pointer to a SVL record structure.
 *  @param  [in,out]  pui4_ver_id   - Pointer to a variable holding version id
 *                                    of the SVL database object.  On entry,
 *                                    this variable contains the version id of
 *                                    the database that the caller expects.  On
 *                                    the return, this variable contains the
 *                                    current version id of the database.   If
 *                                    the expected version id and current
 *                                    database version id do not match, an error
 *                                    status code is returned.  If 'pui4_ver_id'
 *                                    is set to 'SVL_NULL_VER_ID' value, then no
 *                                    checking is done and the latest version id
 *                                    is returned the application.
 *  @see    x_svl_iterate_rec_by_custom_ord
 *  @return Return the error code
 *  @retval SVLR_OK             - A SVL_REC_T structure is returned.
 *  @retval SVLR_INV_HANDLE     - Invalid `h_svl' handl
 *  @retval SVLR_INV_ARG        - `pui4_ver_id' or `pt_svl_rec' is N
 *  @retval SVLR_END_OF_ITERATION - The iteration has reach an end-point.  The
 *                                end-points is either at the record with
 *                                smallest or the largest channel number.
 *  @retval SVLR_MODIFIED       - The database has bee modified.  The
 *                                `pui4_ver_id' pass into the API and the
 *                                internal database version id is not the same.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_iterate_by_nw
(
    HANDLE_T			h_svl,
    UINT32              ui4_current_channel,
    UINT32              ui4_nw_mask,
    UINT16              ui2_nwl_id,
    UINT16              ui2_nwl_rec_id,
    SVL_ITERATE_T		e_iter_dir,  /* channel + or - */
    SVL_REC_T*			pt_svl_rec,
    UINT32*             pui4_ver_id 
);

/*------------------------------------------------------------------*/
/*! @brief  This API iterates over service records in the SVL list with
 *          reference to the given SATL record.  It returns the next
 *          matching service record from a specified the channel number
 *          of the current service record location.  
 *          The 'e_iter_dir' argument specifies direction of iteration, 
 *          e.g., the next record's channel numeric value is plus or 
 *          minus from the current record's channel numeric value.  
 *          Service records that do not match the network mask and SATL
 *          ID / SATL record ID specified will be skipped during the
 *          iteration.  
 *
 *          If the current record is at the end or beginning of the
 *          service list, then 'CHANNEL_PLUS' or 'CHANNEL_MINUS' will
 *          return a status to indicate that end-point is reached. It is
 *          up the caller to decide if next service record should wrap
 *          around to the beginning service record or to the end of the
 *          service record.
 *
 *          Since the service record in the SVL is sorted by 'channel
 *          number', the iteration can be thought of as getting the next
 *          or the previous channel in the specified satellite.
 *
 *          Specifying 'CHANNEL_SMALLEST' returns the record with the
 *          smallest channel number (regardless of the current service
 *          record position), and similarly specifying 'CHANNEL_LARGEST'
 *          returns the record with the largest channel number.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  ui4_current_channel   - Specifies the channel number of the
 *                                        current service record to begin for
 *                                        the search for the next service
 *                                        record.
 *  @param  [in]  ui4_nw_mask       - 32-bits mask specifying the network(s) of
 *                                    the service record to be returned.
 *  @param  [in]  ui2_satl_id       - Specifies the Satellite List ID value.
 *  @param  [in]  ui2_satl_rec_id   - Specifies the Satellite record ID value.
 *  @param  [in]  e_iter_dir        - Specifies the direction of the iteration.
 *                                    The values are:
 *                                    SVL_CHANNEL_PLUS,
 *                                    SVL_CHANNEL_MINUS,
 *                                    SVL_CHANNEL_SMALLEST,
 *                                    SVL_CHANNEL_LARGEST.
 *                                    If SVL_CHANNEL_SMALLEST or
 *                                    SVL_CHANNEL_LARGEST is specified, then the
 *                                    record with the smallest (numerically)
 *                                    channel value is returned, and vice versa.
 *                                    If the iteration has reach an end point,
 *                                    i.e., the current record is point to the
 *                                    first or last service record, then the
 *                                    current service record is returned.
 *  @param  [out] pt_svl_rec        - Pointer to a SVL record structure.
 *  @param  [in,out]  pui4_ver_id   - Pointer to a variable holding version id
 *                                    of the SVL database object.  On entry,
 *                                    this variable contains the version id of
 *                                    the database that the caller expects.  On
 *                                    the return, this variable contains the
 *                                    current version id of the database.   If
 *                                    the expected version id and current
 *                                    database version id do not match, an error
 *                                    status code is returned.  If 'pui4_ver_id'
 *                                    is set to 'SVL_NULL_VER_ID' value, then no
 *                                    checking is done and the latest version id
 *                                    is returned the application.
 *  @see    x_svl_iterate_rec_by_custom_ord
 *  @return Return the error code
 *  @retval SVLR_OK             - A SVL_REC_T structure is returned.
 *  @retval SVLR_INV_HANDLE     - Invalid `h_svl' handl
 *  @retval SVLR_INV_ARG        - `pui4_ver_id' or `pt_svl_rec' is N
 *  @retval SVLR_END_OF_ITERATION - The iteration has reach an end-point.  The
 *                                end-points is either at the record with
 *                                smallest or the largest channel number.
 *  @retval SVLR_MODIFIED       - The database has bee modified.  The
 *                                `pui4_ver_id' pass into the API and the
 *                                internal database version id is not the same.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_iterate_by_sat
(
    HANDLE_T			h_svl,
    UINT32              ui4_current_channel,
    UINT32              ui4_nw_mask,
    UINT16              ui2_satl_id,
    UINT16              ui2_satl_rec_id,
    SVL_ITERATE_T		e_iter_dir,  /* channel + or - */
    SVL_REC_T*			pt_svl_rec,
    UINT32*             pui4_ver_id 
);

/*------------------------------------------------------------------*/
/*! @brief  This function returns the current version id of the
 *          SVL database object.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [out] pui4_ver_id       - Pointer to a variable holding version id
 *                                    of the SVL database object.
 *  @return Return the error code
 *  @retval SVLR_OK             - Version id is returned. 
 *  @retval SVLR_INV_HANDLE     - Bad 'h_svl' handle.
 *  @retval SVLR_INV_ARG        - 'pui4_ver_id' is a NULL pointer.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_get_ver_id
(
    HANDLE_T			h_svl,
    UINT32*			    pui4_ver_id
);

/*------------------------------------------------------------------*/
/*! @brief  This API pre-sets the SVL_REC_T structure to a pre-defined invalid
 *          value (e.g., 0xFF).   The pre-defined invalid value can be used to
 *          detect changes in the SVL_REC_T structure for backward compatibility
 *          in the future.
 *  @param  [out] pt_svl_rec        - Pointer to a SVL record structure.
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL library is initialized successfully.
 *  @retval SVLR_INV_ARG        - 'pt_svl_rec' is a NULL pointer.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_rec_init
(
    SVL_REC_T*			pt_svl_rec 
);

/*------------------------------------------------------------------*/
/*! @brief  This function register compare functions for
 *          sorting and for searching operation.
 *
 *          If global flag is TRUE, then this compare functions
 *          apply to all SVL handle points to the same CDB database
 *          object.
 *
 *          Otherwise, the compare function are SVL handle specific.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  b_global          - TRUE:  compare functions are global scope.
 *                                    FALSE: compare functions are SVL handle
 *                                    specific.
 *  @param  [in]  pv_tag            - Specified the optional tag info that is
 *                                    passed to `pf_sort_cmp' and
 *                                    `pf_search_cmp' functions.
 *  @param  [in]  pf_sort_cmp       - Compare function used for sorting.
 *  @param  [in]  pf_search_cmp     - Compare function used for searching.
 *  @see    x_svl_iterate_rec, x_svl_get_rec_by_brdcst_type,
 *          x_svl_get_rec_by_ts, x_svl_sort_rec, x_svl_unreg_cmp_fcts
 *  @return Return the error code
 *  @retval SVLR_OK             - Success.
 *  @retval SVLR_INV_HANDLE     - Bad 'h_svl' handle.
 *  @retval SVLR_INV_ARG        - 'pf_sort_cmp' or 'pf_search_cmp' is a NULL
 *                                pointer.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_reg_cmp_fcts
(
    HANDLE_T               h_svl,
    BOOL                   b_global,
    VOID*                  pv_tag,
    x_svl_sort_cmp         pf_sort_cmp,
    x_svl_search_cmp       pf_search_cmp
);

/*------------------------------------------------------------------*/
/*! @brief  This function un-register compare functions for
 *          sorting and for searching operation.
 *
 *          If global flag is TRUE, then this compare functions
 *          apply to all SVL handle points to the same CDB database
 *          object.
 *
 *          Otherwise, the compare function are SVL handle specific.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  b_global          - TRUE:  compare functions are global scope.
 *                                    FALSE: compare functions are SVL handle
 *                                    specific.
 *  @see    x_svl_reg_cmp_fcts
 *  @return Return the error code
 *  @retval SVLR_OK             - Success.
 *  @retval SVLR_INV_HANDLE     - Bad 'h_svl' handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svl_unreg_cmp_fcts
(
    HANDLE_T               h_svl,
    BOOL                   b_global
);

/*------------------------------------------------------------------*/
/*! @brief  This API applies the sort operation (using the
 *          per-handle compare fucntion, or the global compare
 *          function, or default compare function, to the SVL
 *          records.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [out] pui4_ver_id       - Contains the version id of the SVL
 *                                    database object.
 *  @return Return the error code
 *  @retval SVLR_OK             - Success.
 *  @retval SVLR_INV_ARG        - `h_svl' is not a valid handle or `pui4_ver_id'
 *                                is null.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svl_sort_rec
(
    HANDLE_T			h_svl,
    UINT32*             pui4_ver_id 
);

/*------------------------------------------------------------------*/
/*! @brief  Get info (number of records, size of record, etc) for
 *          this SVL handle. The output is printed on the terminal.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @return Return the error code
 *  @retval SVLR_OK             - Success.
 *  @retval SVLR_INV_HANDLE     - Bad handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svl_get_info
(
    HANDLE_T               h_svl
);

/*------------------------------------------------------------------*/
/*! @brief  This API gets customer's private data associated with
 *          the specified SVL record (e.g.,'ui2_svl_rec_id')
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  ui2_svl_rec_id    - Specifies the service record id.
 *  @param  [out] pt_client_data    - Pointer to a structure containing customer
 *                                    data If the buffer is too small, only data
 *                                    up to the len of
 *                                    'pt_client_data->ui1_buf_len' is copied
 *                                    out.  The paramter
 *                                    'pt_client_data->ui1_data_len' will be set
 *                                    to indicate the total len of customer data
 *                                    in the internal SVL record.
 *  @param  [out] pui4_ver_id       - Pointer to a variable holding version id
 *                                    of the database object.
 *  @note   Version id of the SVL database object is incremented each time when
 *          a modification takes place.
 *  @see    x_svl_set_client_data
 *  @return Return the error code
 *  @retval SVLR_OK             - Customer's private data from the specified
 *                                record is returned.
 *  @retval SVLR_INV_HANDLE     - Bad handle.
 *  @retval SVLR_REC_NOT_FOUND  - No record matching the search parameter
 *                                ('ui2_svl_rec_id')  is found.
 *  @retval SVLR_INV_ARG        - 'pui4_ver_id' or 'pt_client_data' is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svl_get_client_data
(
    HANDLE_T               h_svl,
    UINT16                 ui2_svl_rec_id,
    SVL_CLIENT_DATA_T*     pt_client_data,
    UINT32*                pui4_ver_id
);

/*------------------------------------------------------------------*/
/*! @brief  This API copies customer's private data associated with
 *          the specified SVL record (e.g.,'ui2_svl_rec_id') to SVL database.
 *  @param  [in]  h_svl         - Handle to the SVL object.
 *  @param  [in] ui2_svl_rec_id - Specifies the service record id.
 *  @param  [in] pt_client_data - Pointer to a structure containing customer
 *                                data The paramter
 *                                'pt_client_data->ui1_data_len' indicates the
 *                                total len of customer data in the
 *                                'pt_client_data-> pui1_data_buf' buffer.
 *  @see    x_svl_get_client_data
 *  @return Return the error code
 *  @retval SVLR_OK             - Customer's private data is copied into SVL
 *                                database.
 *  @retval SVLR_INV_HANDLE     - Bad handle.
 *  @retval SVLR_REC_NOT_FOUND  - No record matching the search parameter
 *                                ('ui2_svl_rec_id')  is found.
 *  @retval SVLR_INV_ARG        - 'pt_client_data' is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svl_set_client_data
(
    HANDLE_T               h_svl,
    UINT16                 ui2_svl_rec_id,
    SVL_CLIENT_DATA_T*     pt_client_data
);

/*------------------------------------------------------------------*/
/*! @brief  This API iterates over service records in the SVL list.
 *          It returns the next/prev service record from a given
 *          service record location specified by the pv_data.
 *
 *          The 'e_iter_dir' argument specifies direction of iteration,
 *          e.g., the next record or prev record from the current
 *          record location identified by the 'pv_data'
 *
 *          Service records that do not match the network mask
 *          specification will be skipped during the iteration.
 *
 *          If the current record is at the end or beginning of the
 *          sorted service list, then 'SVL_NEXT_REC' or 'SVL_PREV_REC'
 *          returns a status to indicate that end-point is reached. It is
 *          up the caller to decide if next service record should wrap
 *          around to the beginning service record or to the end of the
 *          service record.
 *
 *          Since the service record in the SVL is sorted by the order
 *          specified in the client provided compare function and search
 *          compare function, the iteration can be thought of as getting
 *          the next or the previous record in a customized ordered list.
 *
 *          Specifying 'SVL_FIRST_REC' returns the first record in
 *          sorted list (regardless of the current service
 *          record position), and similarly specifying 'SVL_LAST_REC'
 *          returns the last record in the customized sorted list.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  pv_data           - Specifies the 'pv_data' of the current
 *                                    service record to begin for the search for
 *                                    the next service record.
 *  @param  [in]  ui4_nw_mask       - 32-bits mask specifying the network(s) of
 *                                    the service record to be returned.
 *  @param  [in]  e_iter_dir        - Specifies the direction of the iteration.
 *                                    The values are:
 *                                    SVL_CHANNEL_PLUS,
 *                                    SVL_CHANNEL_MINUS,
 *                                    SVL_CHANNEL_SMALLEST,
 *                                    SVL_CHANNEL_LARGEST.
 *                                    If SVL_CHANNEL_SMALLEST or
 *                                    SVL_CHANNEL_LARGEST is specified, then the
 *                                    record with the smallest (numerically)
 *                                    channel value is returned, and vice versa.
 *                                    If the iteration has reach an end point,
 *                                    i.e., the current record is point to the
 *                                    first or last service record, then the
 *                                    current service record is returned.
 *  @param  [out] pt_svl_rec        - Pointer to a SVL record structure.
 *  @param  [in,out]  pui4_ver_id   - Pointer to a variable holding version id
 *                                    of the SVL database object.  On entry,
 *                                    this variable contains the version id of
 *                                    the database that the caller expects.  On
 *                                    the return, this variable contains the
 *                                    current version id of the database.   If
 *                                    the expected version id and current
 *                                    database version id do not match, an error
 *                                    status code is returned.  If 'pui4_ver_id'
 *                                    is set to 'SVL_NULL_VER_ID' value, then no
 *                                    checking is done and the latest version id
 *                                    is returned the application.
 *  @see    x_svl_iterate_rec
 *  @return Return the error code
 *  @retval SVLR_OK             - A SVL_REC_T structure is returned.
 *  @retval SVLR_INV_HANDLE     - Invalid `h_svl' handl
 *  @retval SVLR_INV_ARG        - `pui4_ver_id' or `pt_svl_rec' is N
 *  @retval SVLR_END_OF_ITERATION - The iteration has reach an end-point.  The
 *                                end-points is either at the record with
 *                                smallest or the largest channel number.
 *  @retval SVLR_MODIFIED       - The database has bee modified.  The
 *                                `pui4_ver_id' pass into the API and the
 *                                internal database version id is not the same.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svl_iterate_rec_by_custom_ord
(
    HANDLE_T			h_svl,
    VOID*               pv_data, 
    UINT32              ui4_nw_mask,
    SVL_ITERATE_T		e_iter_dir,  /* channel + or - */
    SVL_REC_T*			pt_svl_rec,
    UINT32*             pui4_ver_id 
);

/*------------------------------------------------------------------*/
/*! @brief  This API initializes the memory dump structure, which
 *          is used for x_svl_dump_to_mem.
 *  @param  [out] pt_mem_dump       - Pointer to the memory dump structure.
 *  @see    x_svl_dump_to_mem
 *  @return Return the error code
 *  @retval SVLR_OK             - Success.
 *  @retval SVLR_INV_ARG        - 'pt_mem_dump' is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svl_init_mem_dump
(
    SVL_MEM_DUMP_T*        pt_mem_dump
);

/*------------------------------------------------------------------*/
/*! @brief  This API deinitializes the memory dump structure.
 *  @param  [in]  pt_mem_dump       - Pointer to the memory dump structure.
 *  @return Return the error code
 *  @retval SVLR_OK             - Success.
 *  @retval SVLR_INV_ARG        - 'pt_mem_dump' is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svl_free_mem_dump
(
    SVL_MEM_DUMP_T*        pt_mem_dump
);

/*------------------------------------------------------------------*/
/*! @brief  This API dumps the specified SVL database into the specified
 *          structure, which could be later used for x_svl_load_from_mem.
 *          The memory dump structure should be initialized before dumping.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [out] pt_mem_dump       - Pointer to the memory dump structure.
 *  @see    x_svl_load_from_mem
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL database successfully dumped.
 *  @retval SVLR_INV_HANDLE     - Bad handle.
 *  @retval SVLR_INV_ARG        - 'pt_mem_dump' is NULL.
 *  @retval SVLR_FAIL           - Error has occured in dumping the database.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svl_dump_to_mem
(
    HANDLE_T               h_svl,
    SVL_MEM_DUMP_T*        pt_mem_dump
);


/*------------------------------------------------------------------*/
/*! @brief  This API loads the SVL database from the specified memory
 *          dump structure, whose data comes from a previous call to
 *          x_svl_load_from_mem.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  pt_mem_dump       - Pointer to the memory dump structure.
 *  @see    x_svl_dump_to_mem, x_svl_free_mem_dump
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL database successfully loaded.
 *  @retval SVLR_INV_HANDLE     - Bad handle.
 *  @retval SVLR_INV_ARG        - 'pt_mem_dump' is NULL.
 *  @retval SVLR_FAIL           - Error has occured in loading the database.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svl_load_from_mem
(
    HANDLE_T               h_svl,
    SVL_MEM_DUMP_T*        pt_mem_dump
);

/*------------------------------------------------------------------*/
/*! @brief  This API stores the specified SVL object to the file system.
 *  @param  [in]  h_file            - The handle to the file.
 *  @param  [in]  ui2_svl_id        - Specifies the SVL object to be stored to
 *                                    the file system. The limit for SVL ID is
 *                                    between 1 and 65535.
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL object is stored to the file system.
 *  @retval SVLR_NOT_FOUND      - The SVL object specified by the 'ui2_svl_id'
 *                                does not exist.
 */
/*------------------------------------------------------------------*/
extern INT32 x_svl_fs_store_given_file_handle
(
    HANDLE_T        h_file,
    UINT16          ui2_svl_id  
);

/*------------------------------------------------------------------*/
/*! @brief  This API sync a specified SVL handle with SVL records
 *          from persistent file system.  The existing SVL records
 *          are replaced with new records read from file.
 *  @param  [in]  h_svl             - Specify the handle to the SVL object.
 *  @param  [in]  h_file            - The handle to the file.
 *  @param  [in]  ui2_svl_id        - Specifies the SVL object to load from
 *                                    the file system. The limit for SVL ID is
 *                                    between 1 and 65535.
 *  @return Return the error code
 *  @retval SVLR_OK             - SVL object is loaded from the file system.
 *  @retval SVLR_NOT_FOUND      - The SVL object specified by the 'ui2_svl_id'
 *                                does not exist.
 */
/*------------------------------------------------------------------*/
extern
INT32 x_svl_fs_sync_given_file_handle
(
    HANDLE_T		h_svl,	
    HANDLE_T        h_file,
    UINT16		    ui2_svl_id
);

extern INT32 x_svl_get_multi_lang
(
    HANDLE_T               h_svl,
    UINT16                 ui2_svl_rec_id,
    SVL_MULTI_LANG_T *     pt_multi_lang,
    UINT32*                pui4_ver_id
);

extern INT32 x_svl_set_multi_lang
(
    HANDLE_T               h_svl,
    UINT16                 ui2_svl_rec_id,
    SVL_MULTI_LANG_T *     pt_multi_lang
);

/*----------------------------------------------------------------------------*/
/*! @} */
/*----------------------------------------------------------------------------*/
#endif /* _X_SVL_H_ */
