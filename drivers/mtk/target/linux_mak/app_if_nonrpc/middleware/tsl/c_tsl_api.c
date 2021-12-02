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
 * $RCSfile: c_tsl_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file contains implmentation of the Transport Stream list (TSL)
 *    database application API.
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "tsl/x_tsl.h"
#include "handle/handle.h"
#include "aee/aee.h"
/*-------------------------------------------------------------------------
                    functions implementations
 --------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: c_tsl_create
 *
 * Description: 
 *
 * Inputs: 
 *   ps_tsl_name   Name of the TSL object. 
 *   ui2_tsl_id    Unique id for the TSL object.  
 *                 The limit for TSL ID is between 1 and 65535.
 *   e_mem_policy  Memory policy: TSL_MEM_STATIC or TSL_MEM_DYNAMIC
 *   ui2_max_nb_rec Specifies the maximum number of TSL record (for
 *                 TSL_MEM_STATIC), or it specifies number of
 *                 TSL records per memory chunk.
 *   pv_tag        Pointer to a private tag value provided by the 
 *                 caller.  This tag value is passed to the 
 *                 caller's notification function.
 *   pf_nfy        Notification function provided by the caller.  
 *                 This function is called to notify the owner of 
 *                 this TSL handle when the TSL object has been 
 *                 modified.  If NULL, then no notification function 
 *                 will be called.
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK           TSL object is created successfully.
 *   TSLR_INV_ARG      'ui2_tsl_id' is 0, or ph_tsl is NULL, or ps_tsl_name
 *                     is 0..
 *   TSLR_OUT_OF_HANDLE Cannot get a handle.
 *   TSLR_FAIL         The specified 'ps_tsl_name' or 'ui2_tsl_id' value is not
 *                     unique (e.g., it is currently been used), or limit of
 *                     the number of TSL objects that can be created is
 *                     reached.
 *   TSLR_OUT_OF_MEM   Not enough memory to create TSL object.
 *   TSLR_INV_ARG      'ps_tsl_name' or 'ph_tsl' is NULL.
 *   TSLR_AEE_OUT_OF_RESOURCES Can not allocate AEE resources.
 *   TSLR_AEE_NO_RIGHTS No right for the AEE resources. 
 ----------------------------------------------------------------------------*/
INT32 c_tsl_create
(
    const CHAR*         ps_tsl_name,
    UINT16              ui2_tsl_id,
    TSL_MEM_POLICY_T    e_mem_policy,
    UINT16              ui2_max_nb_rec,
    VOID*               pv_tag,
    x_tsl_nfy_fct       pf_nfy,
    HANDLE_T*           ph_tsl
)
{
    HANDLE_T  h_aux;
    INT32     i4_rc;

    i4_rc = aee_grab_handle_resource(AEE_FLAG,
                                     1,
                                     &h_aux);

    switch (i4_rc)
    {
        case AEER_OK:
        {
            i4_rc = x_tsl_create(ps_tsl_name, ui2_tsl_id, e_mem_policy,
                                  ui2_max_nb_rec, pv_tag, pf_nfy, ph_tsl);
            
            if (i4_rc == TSLR_OK)
            {
                handle_link_to_aux(h_aux, *ph_tsl);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_rc = TSLR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_rc = TSLR_AEE_NO_RIGHTS;
        }
        break;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: c_tsl_delete
 *
 * Description: This API deletes the specified TSL object associated 
 *              with 'h_tsl' handle.    Other clients with a handle to 
 *              the deleted TSL object will receive a notification of 
 *              the delete event.   
 *
 * Inputs: 
 *   h_tsl	Handle to the TSL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK           Specified TSL object is deleted.
 *   TSLR_INV_HANDLE   Bad handle.
 -----------------------------------------------------------------------*/
INT32 c_tsl_delete
(
    HANDLE_T    	h_tsl
)
{
    INT32  i4_rc;
    
    i4_rc = x_tsl_delete(h_tsl);
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: c_tsl_clean
 *
 * Description: This API removes all the transport stream records from the 
 *              specified TSL object pointed to by the 'h_tsl' handle.    
 *              All other handles to this TSL object will receive a 
 *notification.   
 *
 * Inputs: 
 *   h_tsl      Handle to the TSL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK           All records are successfully removed. 
 *   TSLR_INV_HANDLE   Bad handle.
 -----------------------------------------------------------------------*/
INT32 c_tsl_clean
(
    HANDLE_T    	h_tsl
)
{
    return x_tsl_clean(h_tsl);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_add_rec
 *
 * Description: This function adds a new service record to the TSL 
 *              database object.  The TSL library will make copy of 
 *              the record content from the address location pointed to 
 *              by the 'pt_tsl_rec'.  The 'ui2_tsl_rec_id' field in 
 *              the 'pt_tsl_rec' structure specifies the key field of the 
 *              service record to be added to the database.  
 *
 * Inputs: 
 *   h_tsl      Handle to the TSL object.
 *   pt_tsl_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK           Record is successfully added. 
 *   TSLR_INV_HANDLE   Bad TSL handle.
 *   TSLR_OUT_OF_MEM   Can not allocate memory, add record failed,.
 *   TSLR_INV_ARG      'pt_tsl_rec' is null.  
 -----------------------------------------------------------------------*/
INT32 c_tsl_add_rec
(
    HANDLE_T            h_tsl,
    const TSL_REC_T*    pt_tsl_rec
)
{
    return x_tsl_add_rec(h_tsl, pt_tsl_rec);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_del_rec
 *
 * Description: This function deletes a specified service record from 
 *              the TSL.  The record to be deleted is specified by the 
 *              transport stream record id.
 *
 * Inputs: 
 *   h_tsl           Handle to the TSL object.
 *   ui2_tsl_rec_id  Specifies the transport stream record to be deleted. 
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK            Specified record is deleted. 
 *   TSLR_INV_HANDLE    Bad TSL handle.
 *   TSLR_REC_NOT_FOUND The specified record to be deleted is not in 
 *                      the TSL object.
 *   TSLR_INV_ARG       'ui2_tsl_rec_id' is not a valid record id.
 -----------------------------------------------------------------------*/
INT32 c_tsl_del_rec
(
    HANDLE_T            h_tsl,
    UINT16              ui2_tsl_rec_id
)
{
    return x_tsl_del_rec(h_tsl, ui2_tsl_rec_id);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_update_rec
 *
 * Description: This function updates a Transport stream record to the TSL
 *              object. If the key field value (e.g., 'ui2_tsl_rec_id' field
 *              in the 'pt_tsl_rec' structure) of the input service record 
 *              matches an existing record in the TSL object, then the 
 *              existing record is over-wrote with the new information,  
 *              otherwise a new record is created in the TSL database. 
 * 
 *              If the 'b_must_exist' flag is TRUE, then there must be an 
 *              existing record with the same key field value, else a status 
 *              of 'TSLR_REC_NOT_FOUND' is returned to the caller.  
 *              If 'b_must_exist' flag is FALSE, then a new service record 
 *              is created if no existing record matching the key field value 
 *              is found in the TSL database.
 *
 *              Call to this function must be protected, e.g., the TSL 
 *              database object needs to be locked before the call to this 
 *              function and unlock after returning from call to this function.
 *
 * Inputs: 
 *   h_tsl      Handle to the TSL object.
 *   pt_tsl_rec Pointer to service record structure. 
 *   b_must_exist Boolean flag specifying if the service record to be 
 *                updated should exist in the TSL database
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK           Record is successfully added/updated.
 *   TSLR_INV_HANDLE   Bad TSL handle. 
 *   TSLR_OUT_OF_MEM   Not enough memory to create a new TSL record.
 *   TSLR_REC_NOT_FOUND The specified record is not found 
 *                      ('b_must_exist' == TRUE)
 *   TSLR_INV_ARG      'pt_tsl_rec' is NULL or the pt_tsl_rec
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_update_rec
(
    HANDLE_T            h_tsl,
    const TSL_REC_T*    pt_tsl_rec,
    BOOL                b_must_exist	
)
{
    return x_tsl_update_rec(h_tsl, pt_tsl_rec, b_must_exist);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_qry_rec_id
 *
 * Description: This API queries the range (FIRST, LAST, or NEXT) of 
 *              valid transport stream record id values used in the TSL
 *              object.  
 *   
 * Inputs: 
 *   h_tsl             Handle to the TSL object.
 *   e_tsl_qry_option  Flag to specifies the record id value to be returned.  
 *                     The values are:
 *                     TSL_FIRST_REC_ID,  TSL_LAST_REC_ID, TSL_NEXT_REC_ID.
 *
 * Outputs: None
 *   pui2_rec_id       Contain the transport stream record id value.	
 *
 * Returns: 
 *   TSLR_OK           TSL record id return.
 *   TSLR_INV_HANDLE   Bad TSL handle. 
 *   TSLR_INV_ARG      'pui2_rec_id' is NULL.
 *
 -----------------------------------------------------------------------*/
INT32 c_tsl_qry_rec_id
(
    HANDLE_T            h_tsl,
    TSL_REC_ID_QRY_T    e_tsl_qry_option,
    UINT16*             pui2_rec_id	
)
{
    return x_tsl_qry_rec_id(h_tsl, e_tsl_qry_option, pui2_rec_id);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_lock
 *
 * Description: This function locks the TSL object.   The function blocks 
 *              until the lock is acquired.
 *
 * Inputs: 
 *   h_tsl	Handle to the TSL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK           TSL is locked. 
 *   TSLR_INV_HANDLE   Bad TSL handle.
 *
 -----------------------------------------------------------------------*/
INT32 c_tsl_lock
(
    HANDLE_T            h_tsl
)
{
    return x_tsl_lock(h_tsl);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_unlock
 *
 * Description: This function unlocks the TSL database. 
 *
 * Inputs: 
 *   h_tsl	Handle to the TSL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK          TSL database is unlocked. 
 *   TSLR_INV_HANDLE  Bad TSL handle.
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_unlock
(
    HANDLE_T            h_tsl
)
{
    return x_tsl_unlock(h_tsl);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_read_lock
 *
 * Description: This function acquires the read-lock on the TSL object.
 *    The function blocks until the lock is acquired.
 *
 * Inputs: 
 *   h_tsl	Handle to the TSL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK           TSL is locked. 
 *   TSLR_INV_HANDLE   Bad TSL handle.
 *
 -----------------------------------------------------------------------*/
INT32 c_tsl_read_lock
(
    HANDLE_T            h_tsl
)
{
    return x_tsl_read_lock(h_tsl);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_read_unlock
 *
 * Description: This function releases the read-lock on the TSL database. 
 *
 * Inputs: 
 *   h_tsl	Handle to the TSL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK          TSL database is unlocked. 
 *   TSLR_INV_HANDLE  Bad TSL handle.
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_read_unlock
(
    HANDLE_T            h_tsl
)
{
    return x_tsl_read_unlock(h_tsl);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_fs_load
 *
 * Description: This API loads a specified TSL object from file system
 *              into memory, and returns a handle to the newly created
 *              TSL object.
 * Inputs: 
 *   h_dir	      Handle to the parent directory of the file specified by
 *                the path 'ps_tsl_path'.  If 'ps_tsl_path' is an absolute
 *                path, then this argument should be set to 'FM_ROOT_HANDLE'.
 *   ps_tsl_path  Specifies the path to the file where the TSL objects
 *                are stored in the file system.
 *   ui2_tsl_id   Specifies the TSL id to load from the file system.
 *                The limit for TSL ID is between 1 and 65535.
 *   ps_tsl_name  Specifies the name for the TSL object.  If null string
 *                is specified, then the default name stored in the file
 *                system is used to create the TSL object.
 *   pv_tag       Pointer to a private tag value provided by the caller.
 *                This tag value is passed to the caller's notification
 *                function.
 *   pf_nfy       Notification function provided by the caller.  This
 *                function is called to notify the owner of this TSL
 *                handle when the TSL object has been modified by another
 *                client.  If NULL, then no notification function will be
 *                called.
 * Outputs:
 *   ph_tsl	      On return, contain the handle to the TSL object created
 *                from file system.
 * Returns: 
 *   TSLR_OK            TSL object is created. 
 *   TSLR_NOT_FOUND     'ui2_tsl_id' is not found or 'ps_tsl_path' does
 *                      not exist.
 *   TSLR_OUT_OF_HANDLE Cannot get a handle.
 *   TSLR_OUT_OF_MEM    Not enough memory to create TSL object.
 *   TSLR_FAIL          The specified 'ps_tsl_name' or 'ui2_tsl_id' value
 *                      is not unique.
 *   TSLR_INV_ARG       'ph_tsl' or 'ps_tsl_path' is NULL.
 *   TSLR_AEE_OUT_OF_RESOURCES Can not allocate AEE resources.
 *   TSLR_AEE_NO_RIGHTS No right for the AEE resources. 
 -----------------------------------------------------------------------*/
INT32 c_tsl_fs_load
(
    HANDLE_T            h_dir,
    const CHAR*         ps_tsl_path,
    UINT16              ui2_tsl_id,
    const CHAR*	        ps_tsl_name,
    VOID*               pv_tag,
    x_tsl_nfy_fct       pf_nfy,
    HANDLE_T*           ph_tsl	
)
{
    HANDLE_T  h_aux;
    INT32     i4_rc;

    i4_rc = aee_grab_handle_resource(AEE_FLAG,
                                     1,
                                     &h_aux);

    switch (i4_rc)
    {
        case AEER_OK:
        {
            i4_rc = x_tsl_fs_load(h_dir, ps_tsl_path, ui2_tsl_id,
                                  ps_tsl_name, pv_tag, pf_nfy, ph_tsl);
            
            if (i4_rc == TSLR_OK)
            {
                handle_link_to_aux(h_aux, *ph_tsl);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_rc = TSLR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_rc = TSLR_AEE_NO_RIGHTS;
        }
        break;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: c_tsl_fs_store
 *
 * Description: This API stores the specified TSL object to the file system.
 *
 * Inputs: 
 *   h_dir	      Handle to the parent directory of the file specified by
 *                the path 'ps_tsl_path'.  If 'ps_tsl_path' is an absolute
 *                path, then this argument should be set to 'FM_ROOT_HANDLE'.
 *   ps_tsl_path  Specifies the file path to the file where the TSL objects
 *                are stored in the file system.
 *   ui2_tsl_id   Specifies the  ID of the TSL object to be stored to the
 *                file system.  The limit for TSL ID is between 1 and 65535.
 *
 * Outputs: None 
 *
 * Returns: 
 *   TSLR_OK			TSL object is stored to the file system. 
 *   TSLR_NOT_FOUND		The TSL object specified by the 'ui2_tsl_id' does
 *                      not exist.
 *   TSLR_INV_ARG		'ps_tsl_path' is NULL.
 -----------------------------------------------------------------------*/
INT32 c_tsl_fs_store
(
    HANDLE_T            h_dir,
    const CHAR*         ps_tsl_path,
    UINT16              ui2_tsl_id	
)
{
    return x_tsl_fs_store(h_dir, ps_tsl_path, ui2_tsl_id);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_fs_del
 *
 * Description: This API deletes a specified TSL object on the file system.
 *
 * Inputs: 
 *   h_dir	      Handle to the parent directory of the file specified by the
 *                path 'ps_tsl_path'.  If 'ps_tsl_path' is an absolute path,
 *                then this argument should be set to 'FM_ROOT_HANDLE'.
 *   ps_tsl_path  Specifies the file path to the file where the TSL objects
 *                are stored in the file system.
 *   ui2_tsl_id   Specifies the ID of the TSL object to be deleted from the
 *                file system.  The limit for TSL ID is between 1 and 65535.
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK		  TSL object is deleted from the file system. 
 *   TSLR_NOT_FOUND	  'ui2_tsl_id' is not found or 'ps_tsl_path' does not
 *                    exist.
 *   TSLR_INV_ARG     'ps_tsl_path' is NULL.
 -----------------------------------------------------------------------*/
INT32 c_tsl_fs_del
(
    HANDLE_T            h_dir,
    const CHAR*         ps_tsl_path,
    UINT16              ui2_tsl_id
)
{
    return x_tsl_fs_del(h_dir, ps_tsl_path, ui2_tsl_id);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_fs_qry
 *
 * Description: This API queries the TSL objects stored in file system.
 *
 * Inputs: 
 *   h_dir	      Handle to the parent directory of the file specified
 *                by the path 'ps_tsl_path'.  If 'ps_tsl_path' is an
 *                absolute path, then this argument should be set to
 *                'FM_ROOT_HANDLE'.
 *   ps_tsl_path  Specifies the file path to the file where the TSL
 *                objects are stored in the file system..
 *   ui2_idx      Index value to get the next TSL object.  0 means the
 *                first TSL, 1 is the second TSL, etc.
 *
 * Outputs: 
 *   pui2_tsl_id  Pointer to a UINT16 variable. On return, it contains TSL ID.
 *
 *   ps_tsl_name  Pointer to a string buffer (minimum 17 bytes) .
 *                On return, it contains the default TSL name stored on the
 *                file system. 
 * Returns: 
 *   TSLR_OK          TSL object found. 
 *   TSLR_NOT_FOUND   No TSL object found.
 *   TSLR_INV_ARG     'ps_tsl_name' or 'pui2_tsl_id' is NULL.
 -----------------------------------------------------------------------*/
INT32 c_tsl_fs_qry
(
    HANDLE_T            h_dir,
    const CHAR*         ps_tsl_path,
    UINT16              ui2_idx,
    UINT16*             pui2_tsl_id,
    CHAR*               ps_tsl_name	
)
{
    return x_tsl_fs_qry(h_dir, ps_tsl_path, ui2_idx, pui2_tsl_id, ps_tsl_name);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_fs_sync
 *
 * Description: This API sync a specified TSL handle with TSL records
 *              from persistent file system.  The existing TSL records
 *              are replaced with new records read from file. 
 *
 * Inputs:
 *
 *   h_tsl          Specify the handle to the TSL object.
 *
 *   h_dir          The handle to the directory.  The 'ps_tsl_path'
 *                  will be relative to the specified 'h_dir'.
 *
 *   ps_tsl_path    Specifies the directory path of the TSL object in the 
 *                  file system. See File Path Specification, 10.1, for the 
 *                  types of file path that can be specified.
 *
 *   ui2_tsl_id     Specifies the TSL id to load from the file system.  
 *                  The limit for TSL ID is between 1 and 65535.
 *
 * Outputs: 
 *
 *   None.
 *                 
 * Returns: 
 *   TSLR_OK             TSL records are load from the persistent file
 *                       into the memory of TSL object.
 *
 *   TSLR_NOT_FOUND      'ui2_tsl_id' is not found or 'ps_tsl_path' 
 *                       does not exist.
 *
 *   TSLR_FAIL           internal error
 *
 *   TSLR_INV_ARG        'ps_tsl_path' is NULL, or 'h_tsl' is NULL_HANDLE,
 *                       or ui2_tsl_id is 0.
 *
 *   TSLR_INV_FILE_PATH  Specified file path is not a directory or a RAW 
 *                       character file.
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_fs_sync
(
    HANDLE_T		h_tsl,	
    HANDLE_T        h_dir,
    const CHAR*	    ps_tsl_path,
    UINT16		    ui2_tsl_id
)
{
    return x_tsl_fs_sync(h_tsl, h_dir, ps_tsl_path, ui2_tsl_id);
}

/*----------------------------------------------------------------------
 * Name: x_tsl_open
 *
 * Description: This function gets a handle to the specified TSL database 
 *              object by its ID value.    
 *
 * Inputs: 
 *   ui2_tsl_id:  Specifies the TSL object identifier value.  
 *                The limit for TSL ID is between 1 and 65535.
 *   pv_tag:      A private tag value associated with the calling client.
 *   pf_nfy:      Pointer to the caller provided notification function.  
 *                The notification function will be called in the context of 
 *                the thread that modifies (update, add, delete) the TSL 
 *                database.  If NULL, then no notification function will be 
 *                called.
 *
 * Outputs:
 *   ph_tsl:      Pointer to the handle created.
 *
 * Returns: 
 *   TSLR_OK                    TSL handle is returned. 
 *   TSLR_OUT_OF_HANDLE         No more handle available.
 *   TSLR_OUT_OF_MEM            Not enough memory to create the TSL control 
 *                              structure.
 *   TSLR_NOT_FOUND             The specified TSL database object id (e.g. 
 *                              'ui2_tsl_id') is not found.
 *   TSLR_INV_ARG               'ph_tsl' is NULL.
 *   TSLR_AEE_OUT_OF_RESOURCES  Can not allocate AEE resources.
 *   TSLR_AEE_NO_RIGHTS         No right for the AEE resources.  
 -----------------------------------------------------------------------*/
INT32 c_tsl_open
(
    UINT16              ui2_tsl_id,
    VOID*               pv_tag,
    x_tsl_nfy_fct       pf_nfy,
    HANDLE_T*           ph_tsl
)
{
    HANDLE_T  h_aux;
    INT32     i4_rc;

    i4_rc = aee_grab_handle_resource(AEE_FLAG,
                                     1,
                                     &h_aux);

    switch (i4_rc)
    {
        case AEER_OK:
        {
            i4_rc = x_tsl_open(ui2_tsl_id, pv_tag, pf_nfy, ph_tsl);
            
            if (i4_rc == TSLR_OK)
            {
                handle_link_to_aux(h_aux, *ph_tsl);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_rc = TSLR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_rc = TSLR_AEE_NO_RIGHTS;
        }
        break;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: c_tsl_open_by_name
 *
 * Description: This function gets a handle to the specified TSL database 
 *              object by its name.    
 *
 * Inputs: 
 *   ps_tsl_name    Specifies the TSL object name (16 char max).
 *   pv_tag         A private tag value associated with the calling client.
 *   pf_nfy         Pointer to a caller provided notification function.  
 *                  The notification function is called in the context of 
 *                  the thread that modifies (update, add, delete) the TSL 
 *                  database.  If NULL, then no notification function will 
 *                  be called.
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK                    TSL handle is returned. 
 *   TSLR_OUT_OF_HANDLE         No more handle available.
 *   TSLR_OUT_OF_MEM            Not enough memory to create the TSL
 *                              handle structure.
 *   TSLR_NOT_FOUND             The specified TSL database object name (e.g., 
 *                              ps_tsl_name)  is not found.
 *   TSLR_INV_ARG               'ph_tsl' or 'ps_tsl_name' is NULL.
 *   TSLR_AEE_OUT_OF_RESOURCES  Can not allocate AEE resources.
 *   TSLR_AEE_NO_RIGHTS         No right for the AEE resources.     
 -----------------------------------------------------------------------*/
INT32 c_tsl_open_by_name
(
    const CHAR*         ps_tsl_name,
    VOID*               pv_tag,
    x_tsl_nfy_fct       pf_nfy,
    HANDLE_T*		    ph_tsl	
)
{
    HANDLE_T  h_aux;
    INT32     i4_rc;

    i4_rc = aee_grab_handle_resource(AEE_FLAG,
                                     1,
                                     &h_aux);

    switch (i4_rc)
    {
        case AEER_OK:
        {
            i4_rc = x_tsl_open_by_name(ps_tsl_name, pv_tag, pf_nfy, ph_tsl);
            
            if (i4_rc == TSLR_OK)
            {
                handle_link_to_aux(h_aux, *ph_tsl);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_rc = TSLR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_rc = TSLR_AEE_NO_RIGHTS;
        }
        break;
    }
    
    return i4_rc;    
}

/*----------------------------------------------------------------------
 * Name: c_tsl_close
 *
 * Description: This function frees the specified handle to the TSL 
 *              database object.    
 *
 * Inputs: 
 *   h_tsl	Handle to TSL database object.
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK           TSL handle is freed. 
 *   TSLR_INV_HANDLE   Bad handle.
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_close
(
    HANDLE_T            h_tsl
)
{
    INT32  i4_rc;
    
    i4_rc = x_tsl_close(h_tsl);
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: c_tsl_get_handle_info
 *
 * Description: This function gets the TSL ID and TSL name for the 
 *              specified handle.    
 *
 * Inputs: 
 *   h_tsl	Handle to the TSL object.
 *
 * Outputs: 
 *   pui2_tsl_id  Pointer to TSL ID variable.  On return it contains 
 *                the TSL ID value for this handle.
 *   ps_tsl_name  Pointer to a char array variable (the size of this char 
 *                array should be at least MAX_TSL_NAME_LEN+1).  On return, 
 *                this array contains the name of the TSL object.
 * Returns: 
 *   TSLR_OK           Information about this handle (tsl id and tsl name)
 *                     is returned.
 *   TSLR_INV_HANDLE   Bad handle.
 *   TSLR_INV_ARG      'pui2_tsl_id' or 'ps_tsl_name' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_get_handle_info
(
    HANDLE_T            h_tsl,
    UINT16*             pui2_tsl_id,
    CHAR*               ps_tsl_name
)
{
    return x_tsl_get_handle_info(h_tsl, pui2_tsl_id, ps_tsl_name);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_get_rec
 *
 * Description: This API gets a specific record stored in the TSL database 
 *              object.    The search parameter is specified by the 
 *              'ui2_tsl_rec_id' value.
 *
 * Inputs: 
 *   h_tsl           Handle to the TSL object.
 *   ui2_tsl_rec_id  Specifies the service record id.
 *
 * Outputs: 
 *   pt_tsl_rec   Pointer to a service record structure.   This structure must 
 *                be allocated by the caller.  On return, it is filled with 
 *                service record information.
 *   pui4_ver_id  Pointer to a variable holding version id of the database 
 *                object.   Note: version id of the TSL database object is 
 *                incremented each time when a modification takes place.
 * Returns: 
 *   TSLR_OK    Data from the specified record is returned. 
 *   TSLR_INV_HANDLE    Bad handle.
 *   TSLR_REC_NOT_FOUND No record matching the search parameter 
 *                      ('ui2_tsl_rec_id')  is found.
 *   TSLR_INV_ARG       'pui4_ver_id' or 'pt_tsl_rec' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_get_rec
(
    HANDLE_T            h_tsl,
    UINT16              ui2_tsl_rec_id,
    TSL_REC_T*          pt_tsl_rec,
    UINT32*             pui4_ver_id
)
{
    return x_tsl_get_rec(h_tsl, ui2_tsl_rec_id, pt_tsl_rec, pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_get_num_recs
 *
 * Description:    This function returns the number of records for the
 *                 specified TSL. 
 *
 * Inputs: 
 *   h_tsl	Handle to the TSL object.
 *
 * Outputs: 
 *   pui2_num_recs	Pointer to a variable for the number of records in
 *                  the TSL. 	
 *   pui4_ver_id    Pointer to a variable holding version id of the TSL
 *                  database object. 	
 * Returns: 
 *   TSLR_OK			Number of records in the specified TSL is returned. 
 *   TSLR_INV_HANDLE    Bad 'h_tsl' handle.
 *   TSLR_INV_ARG       'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_get_num_recs
(
    HANDLE_T            h_tsl,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_ver_id
)
{
    return x_tsl_get_num_recs(h_tsl, pui2_num_recs, pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_get_rec_by_rec_idx
 *
 * Description: This API returns transport stream list record entry from the
 *              TSL database object by its index. Maximum index is obtained
 *              by calling c_tsl_get_num_recs.
 *             
 * Inputs: 
 *    h_tsl             Handle to the TSL object.
 *    ui2_idx           an idx to get the next TSL record.
 *                      For example: 0 will the first TSL record, and 1
 *                      will be second, and so on.
 *
 * Outputs:
 *    pt_tsl_rec    Ref to a TSL_REC_T structure containing the transport
 *                  stream data.
 *    pui4_ver_id   Contains the version id of the TSL database object.   
 *                  
 * Returns: 
 *   TSLR_OK           
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_get_rec_by_rec_idx
(
    HANDLE_T            h_tsl,
    UINT16              ui2_idx,
    TSL_REC_T*          pt_tsl_rec,
    UINT32*             pui4_ver_id
)
{
    return x_tsl_get_rec_by_rec_idx(h_tsl, ui2_idx, pt_tsl_rec, pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_rec_init
 *
 * Description: This API pre-sets the TSL_REC_T structure to a 
 *     pre-defined invalid value (e.g., 0xFF).   The pre-defined invalid 
 *     value can be used to detect changes in the TSL_REC_T structure for 
 *     backward compatibility in the future.
 *
 *     For example, if a new field is added to the TSL record structure.  
 *     At the runtime, if the new field in the TSL_REC_T structure passed 
 *     into TSL library contain 0xFF, then we know the caller has not been 
 *     re-compile with the new TSL module.   
 *
 * Inputs: 
 *   Pointer to a TSL record structure. 
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK           TSL library is initialized successfully.
 *   TSLR_INV_ARG      'pt_tsl_rec' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 c_tsl_rec_init
(
    TSL_REC_T*			pt_tsl_rec 
)
{
    return x_tsl_rec_init(pt_tsl_rec);
}


/*----------------------------------------------------------------------
 * Name: c_tsl_set_cursor
 *
 * Description: This API specifies a entry position in TSL records, 
 *   which will read when API c_tsl_read_cursor() is called.
 * 
 *
 * Inputs: 
 *   h_tsl       Handle to the TSL object.
 *   e_pos       position of the TSL record to read.
 *
 * Outputs: 
 *   None.
 *               
 * Returns: 
 *   TSLR_OK    Data from the specified record is returned. 
 *   TSLR_INV_HANDLE    Bad handle.
 *   TSLR_INV_ARG       'e_pos' is invalid.
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_set_cursor
(
    HANDLE_T            h_tsl,
    TSL_ITERATE_T       e_pos
)
{
    return x_tsl_set_cursor(h_tsl, e_pos);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_read_cursor
 *
 * Description: This API read the content of a specific record
 *   stored in the TSL database.  The specific record is read
 *   is the record currently pointed by the cursor.   
 *              
 * Inputs: 
 *   h_tsl           Handle to the TSL object.
 *
 * Outputs: 
 *   pt_tsl_rec  Pointer to a service record structure.   This structure must 
 *               be allocated by the caller.  On return, it is filled with 
 *               service record information.
 *
 *   pui4_ver_id Pointer to a variable holding version id of the database 
 *               object.   Note: version id of the TSL database object is 
 *               incremented each time when a modification takes place.
 * Returns: 
 *   TSLR_OK    Data from the specified record is returned. 
 *   TSLR_INV_HANDLE    Bad handle.
 *   TSLR_INV_ARG       'pui4_ver_id' or 'pt_tsl_rec' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_read_cursor
(
    HANDLE_T            h_tsl,
    TSL_REC_T*          pt_tsl_rec,
    UINT32*             pui4_ver_id
)
{
    return x_tsl_read_cursor(h_tsl, pt_tsl_rec, pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_sort_rec
 *
 * Description: This API applies the sort operation (using the
 *    per-handle compare fucntion, or the global compare
 *    function, or default compare function, to the TSL
 *    records.
 *             
 * Inputs: 
 *    h_tsl     Handle to the TSL object.
 *
 * Outputs: 
 *    pui4_ver_id   Contains the version id of the TSL database object.   
 *                  
 * Returns: 
 *   TSLR_OK           
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_sort_rec
(
    HANDLE_T			h_tsl,
    UINT32*             pui4_ver_id 
)
{
    return x_tsl_sort_rec(h_tsl, pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_get_num_rec_by_sat
 *
 * Description: This API get the numbers of record matching specific
 *   SATL id, SATL record id and specified bit mask.
 *             
 * Inputs: 
 *    h_tsl     Handle to the TSL object.
 *    ui4_mask          Specify the bit mask.
 *    ui2_satl_id       Specify the SATL id.
 *    ui2_satl_rec_id   Specify the SATL record id.
 *
 * Outputs:
 *    pui2_num_recs Contains the number of TSL record with specified
 *                  SATL id and SATL record id.
 *    pui4_ver_id   Contains the version id of the TSL database object.   
 *                  
 * Returns: 
 *   TSLR_OK           
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_get_num_rec_by_sat
(
    HANDLE_T            h_tsl,
    UINT32              ui4_mask,
    UINT16              ui2_satl_id,
    UINT16              ui2_satl_rec_id,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_ver_id
)
{
    return x_tsl_get_num_rec_by_sat(h_tsl, ui4_mask,
                                    ui2_satl_id, ui2_satl_rec_id,
                                    pui2_num_recs, pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_get_rec_by_sat
 *
 * Description: This API return a TSL record for a specified SATL id and
 *              SATL record id.  
 *             
 * Inputs: 
 *    h_tsl             Handle to the TSL object.
 *    ui4_mask          Specify the bit mask.
 *    ui2_satl_id       Specify the SATL id.
 *    ui2_satl_rec_id   Specify the SATL record id.
 *    ui2_idx           an idx to get the next TSL record with specified
 *                      SATL id and SATL record id. For example: 0 will
 *                      the first TSL record, and 1 will be second, and so
 *                      on
 *
 * Outputs:
 *    pt_tsl_rec    Ref to a TSL_REC_T structure containing the transport
 *                  stream data.
 *    pui4_ver_id   Contains the version id of the TSL database object.   
 *                  
 * Returns: 
 *   TSLR_OK           
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_get_rec_by_sat
(
    HANDLE_T            h_tsl,
    UINT32              ui4_mask,
    UINT16              ui2_satl_id,
    UINT16              ui2_satl_rec_id,
    UINT16              ui2_idx,
    TSL_REC_T*          pt_tsl_rec,
    UINT32*             pui4_ver_id
)
{
    return x_tsl_get_rec_by_sat(h_tsl, ui4_mask,
                                ui2_satl_id, ui2_satl_rec_id, ui2_idx,
                                pt_tsl_rec, pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_init_mem_dump
 *
 * Description: This API initializes the memory dump structure, which 
 *              is used for c_tsl_dump_to_mem.
 *
 * Inputs: 
 *   None
 *
 * Outputs: 
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Returns: 
 *   TSLR_OK            TSL database successfully dumped.
 *   TSLR_INV_ARG       'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_init_mem_dump
(
    TSL_MEM_DUMP_T*        pt_mem_dump
)
{
    return x_tsl_init_mem_dump(pt_mem_dump);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_free_mem_dump
 *
 * Description: This API deinitializes the memory dump structure. 
 *
 * Inputs: 
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Outputs: 
 *   None
 *
 * Returns: 
 *   TSLR_OK            TSL database successfully dumped.
 *   TSLR_INV_ARG       'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_free_mem_dump
(
    TSL_MEM_DUMP_T*        pt_mem_dump
)
{
    return x_tsl_free_mem_dump(pt_mem_dump);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_dump_to_mem
 *
 * Description: This API dumps the specified TSL database into the specified
 *              structure, which could be later used for c_tsl_load_from_mem.
 *              The memory dump structure should be initialized before dumping.
 *
 * Inputs: 
 *   h_tsl           Handle to the TSL object.
 *
 * Outputs: 
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Returns: 
 *   TSLR_OK            TSL database successfully dumped.
 *   TSLR_INV_HANDLE    Bad handle.
 *   TSLR_INV_ARG       'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_dump_to_mem
(
    HANDLE_T               h_tsl,
    TSL_MEM_DUMP_T*        pt_mem_dump
)
{
    return x_tsl_dump_to_mem(h_tsl, pt_mem_dump);
}

/*----------------------------------------------------------------------
 * Name: c_tsl_load_from_mem
 *
 * Description: This API loads the TSL database from the specified memory
 *              dump structure, whose data comes from a previous call to
 *              c_tsl_load_from_mem.
 *
 * Inputs: 
 *   h_tsl           Handle to the TSL object.
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Outputs: 
 *
 * Returns: 
 *   TSLR_OK            TSL database successfully dumped.
 *   TSLR_INV_HANDLE    Bad handle.
 *   TSLR_INV_ARG       'pt_mem_dump' is NULL or TSL metadata not match.
 *   
 -----------------------------------------------------------------------*/
INT32 c_tsl_load_from_mem
(
    HANDLE_T               h_tsl,
    TSL_MEM_DUMP_T*        pt_mem_dump
)
{
    return x_tsl_load_from_mem(h_tsl, pt_mem_dump);
}
