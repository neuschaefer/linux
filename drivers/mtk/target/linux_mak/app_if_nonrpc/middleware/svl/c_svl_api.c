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
 * $RCSfile: c_svl_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This  file contains implementation of the SVL's application API's. 
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "svl/c_svl.h"
#include "svl/x_svl.h"
#include "handle/handle.h"
#include "aee/aee.h"

/*-------------------------------------------------------------------------
                    functions implementations
 --------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: c_svl_create
 *
 * Description:    Create a SVL object and return a handle to this SVL object.
 *
 * Inputs: 
 *   ps_svl_name   Name of the SVL object. 
 *   ui2_svl_id    Unique id for the SVL object.  
 *                 The limit for SVL ID is between 1 and 65535.
 *   e_mem_policy  Memory policy: SVL_MEM_STATIC or SVL_MEM_DYNAMIC
 *   ui2_max_nb_rec Specifies the maximum number of SVL record (for
 *                 SVL_MEM_STATIC), or it specifies number of
 *                 SVL records per memory chunk.
 *   pv_tag        Pointer to a private tag value provided by the 
 *                 caller.  This tag value is passed to the 
 *                 caller's notification function.
 *   pf_nfy        Notification function provided by the caller.  
 *                 This function is called to notify the owner of 
 *                 this SVL handle when the SVL object has been 
 *                 modified.  If NULL, then no notification function 
 *                 will be called.
 *
 * Outputs:
 *   ph_svl        Handle to the SVL object.
 *
 * Returns: 
 *   SVLR_OK           SVL object is created successfully.
 *   SVLR_INV_ARG      'ui2_svl_id' is 0, or ph_svl is NULL, or ps_svl_name
 *                     is 0..
 *   SVLR_OUT_OF_HANDLE Cannot get a handle.
 *   SVLR_FAIL         The specified 'ps_svl_name' or 'ui2_svl_id' value is not *                     unique (e.g., it is currently been used), or limit of
 *                     the number of SVL objects that can be created is
 *                     reached.
 *   SVLR_OUT_OF_MEM   Not enough memory to create SVL object.
 *   SVLR_INV_ARG      'ps_svl_name' or 'ph_svl' is NULL.
 *   SVLR_AEE_OUT_OF_RESOURCES Can not allocate AEE resources.
 *   SVLR_AEE_NO_RIGHTS No right for the AEE resources.
 ----------------------------------------------------------------------------*/
INT32 c_svl_create
(
    const CHAR*      ps_svl_name,
    UINT16           ui2_svl_id,
    SVL_MEM_POLICY_T e_mem_policy,
    UINT16           ui2_max_nb_rec,
    VOID*            pv_tag,
    x_svl_nfy_fct    pf_nfy,
    HANDLE_T*        ph_svl
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
            i4_rc = x_svl_create(ps_svl_name, ui2_svl_id, e_mem_policy,
                                  ui2_max_nb_rec, pv_tag, pf_nfy, ph_svl);
            
            if (i4_rc == SVLR_OK)
            {
                handle_link_to_aux(h_aux, *ph_svl);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_rc = SVLR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_rc = SVLR_AEE_NO_RIGHTS;
        }
        break;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: c_svl_open
 *
 * Description: This function create a handle to the specified SVL database 
 *              object by its ID value.    
 *
 * Inputs: 
 *   ui2_svl_id:  Specifies the SVL object identifier value.  
 *                The limit for SVL ID is between 1 and 65535.
 *   pv_tag:      A private tag value associated with the calling client.
 *   pf_nfy:      Pointer to the caller provided notification function.  
 *                The notification function will be called in the context of 
 *                the thread that modifies (update, add, delete) the SVL 
 *                database.  If NULL, then no notification function will be 
 *                called.
 *
 * Outputs:
 *   ph_svl:      Handle to the SVL object.
 *
 * Returns: 
 *   SVLR_OK               SVL handle is returned. 
 *   SVLR_OUT_OF_HANDLE    No more handle available.
 *   SVLR_OUT_OF_MEM       Not enough memory to create the SVL control 
 *                         structure.
 *   SVLR_NOT_FOUND        The specified SVL database object id (e.g. 
 *                         'ui2_svl_id') is not found.
 *   SVLR_INV_ARG          'ph_svl' is NULL.
 *   SVLR_AEE_OUT_OF_RESOURCES Can not allocate AEE resources.
 *   SVLR_AEE_NO_RIGHTS        No right for the AEE resources.  
 -----------------------------------------------------------------------*/
INT32 c_svl_open
(
    UINT16		    ui2_svl_id,
    VOID*		    pv_tag,
    x_svl_nfy_fct   pf_nfy,
    HANDLE_T*		ph_svl
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
            i4_rc = x_svl_open(ui2_svl_id, pv_tag, pf_nfy, ph_svl);
            
            if (i4_rc == SVLR_OK)
            {
                handle_link_to_aux(h_aux, *ph_svl);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_rc = SVLR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_rc = SVLR_AEE_NO_RIGHTS;
        }
        break;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: c_svl_open_by_name
 *
 * Description: This function creates a handle to the specified SVL database 
 *              object by its name.    
 *
 * Inputs: 
 *   ps_svl_name    Specifies the SVL object name (16 char max).
 *   pv_tag         A private tag value associated with the calling client.
 *   pf_nfy         Pointer to a caller provided notification function.  
 *                  The notification function is called in the context of 
 *                  the thread that modifies (update, add, delete) the SVL 
 *                  database.  If NULL, then no notification function will 
 *                  be called.
 *
 * Outputs:
 *   ph_svl:        Handle to the SVL object.
 *
 * Returns: 
 *   SVLR_OK             SVL handle is returned. 
 *   SVLR_OUT_OF_HANDLE  No more handle available.
 *   SVLR_OUT_OF_MEM     Not enough memory to create the SVL handle structure.
 *   SVLR_NOT_FOUND      The specified SVL database object name (e.g., 
 *                       ps_svl_name)  is not found.
 *   SVLR_INV_ARG        'ph_svl' or 'ps_svl_name' is NULL.
 *   SVLR_AEE_OUT_OF_RESOURCES Can not allocate AEE resources.
 *   SVLR_AEE_NO_RIGHTS        No right for the AEE resources.   
 -----------------------------------------------------------------------*/
INT32 c_svl_open_by_name
(
    const CHAR*     ps_svl_name,
    VOID*		    pv_tag,
    x_svl_nfy_fct	pf_nfy,
    HANDLE_T*		ph_svl	
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
            i4_rc = x_svl_open_by_name(ps_svl_name, pv_tag, pf_nfy, ph_svl);
            
            if (i4_rc == SVLR_OK)
            {
                handle_link_to_aux(h_aux, *ph_svl);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_rc = SVLR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_rc = SVLR_AEE_NO_RIGHTS;
        }
        break;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: c_svl_fs_load
 *
 * Description: This API loads a specified SVL object from file system 
 *              into memory, and returns a handle to the newly created 
 *              SVL object.
 *
 * Inputs:
 *
 *  h_dir          The handle to the directory.  The 'ps_svl_path'
 *                 will be relative to the specified 'h_dir'.
 *
 *  ps_svl_path    Specifies the directory path of the SVL object in the 
 *                 file system. See File Path Specification, 10.1, for the 
 *                 types of file path that can be specified.
 *
 *  ui2_svl_id     Specifies the SVL id to load from the file system.  
 *                 The limit for SVL ID is between 1 and 65535.
 *
 *  ps_svl_name    Specifies the name for the SVL object.  If null string 
 *                 is specified, then the default name stored in the file 
 *                 system is used to create the SVL object.
 *
 *  pv_tag         Pointer to a private tag value provided by the caller.  
 *                 This tag value is passed to the caller's notification 
 *                 function.
 *
 *  pf_nfy         Notification function provided by the caller.  This 
 *                 function is called to notify the owner of this SVL handle 
 *                 when the SVL object has been modified.  If NULL then no 
 *                 notification function will be called.
 *
 * Outputs: 
 *  ph_svl         On return, contain the handle to the SVL object created 
 *                 from file system.
 * Returns: 
 *   SVLR_OK             SVL object is created. 
 *
 *   SVLR_NOT_FOUND      'ui2_svl_id' is not found or 'ps_svl_path' 
 *                       does not exist.
 *
 *   SVLR_OUT_OF_HANDLE  Cannot get a handle.
 *
 *   SVLR_OUT_OF_MEM     Not enough memory to create SVL object.
 *
 *   SVLR_FAIL           The specified 'ps_svl_name' or 'ui2_svl_id' value is 
 *                       not unique.
 *
 *   SVLR_INV_ARG        'ps_svl_name' or 'ph_svl' or 'ps_svl_path' is NULL.
 *
 *   SVLR_INV_FILE_PATH  Specified file path is not a directory or a RAW 
 *                       character file.
 *   SVLR_AEE_OUT_OF_RESOURCES Can not allocate AEE resources.
 *   SVLR_AEE_NO_RIGHTS        No right for the AEE resources.    
 -----------------------------------------------------------------------*/
INT32 c_svl_fs_load
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_svl_id,
    const CHAR*	    ps_svl_name,
    VOID*			pv_tag,
    x_svl_nfy_fct	pf_nfy,
    HANDLE_T*		ph_svl	
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
            i4_rc = x_svl_fs_load(h_dir, ps_svl_path, ui2_svl_id,
                                   ps_svl_name, pv_tag, pf_nfy, ph_svl);
            
            if (i4_rc == SVLR_OK)
            {
                handle_link_to_aux(h_aux, *ph_svl);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_rc = SVLR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_rc = SVLR_AEE_NO_RIGHTS;
        }
        break;
    }
    
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: c_svl_delete
 *
 * Description: This API deletes the specified SVL object associated 
 *              with 'h_svl' handle.    Other clients with a handle to 
 *              the deleted SVL object will receive a notification of 
 *              the delete event.   
 *
 * Inputs: 
 *   h_svl	Handle to the SVL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           Specified SVL object is deleted.
 *   SVLR_INV_HANDLE   Bad handle.
 -----------------------------------------------------------------------*/
INT32 c_svl_delete
(
    HANDLE_T    	h_svl
)
{
    INT32 i4_rc;
    
    i4_rc = x_svl_delete(h_svl);
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: c_svl_close
 *
 * Description: This function frees the specified handle to the SVL 
 *              database object.  Note: the SVL data object that this
 *              handle refer to remain active. 
 *
 * Inputs: 
 *   h_svl	Handle to SVL database object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           SVL handle is freed. 
 *   SVLR_INV_HANDLE   Bad handle.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_close
(
    HANDLE_T                  h_svl
)
{
    INT32 i4_rc;
    
    i4_rc = x_svl_close(h_svl);
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: c_svl_clean
 *
 * Description: This API removes all the service records from the 
 *              specified SVL object pointed to by the 'h_svl' handle.    
 *              All other handles to this SVL object will receive a 
 *              notification.   
 *
 * Inputs: 
 *   h_svl      Handle to the SVL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           All records are successfully removed. 
 *   SVLR_INV_HANDLE   Bad handle.
 -----------------------------------------------------------------------*/
INT32 c_svl_clean
(
    HANDLE_T    	h_svl
)
{
    return x_svl_clean(h_svl);
}

/*----------------------------------------------------------------------
 * Name: x_svl_add_rec
 *
 * Description: This function adds a new service record to the SVL 
 *              database object.  The SVL library will make copy of 
 *              the record content from the address location pointed to 
 *              by the 'pt_svl_rec'.  The 'ui2_svl_rec_id' field in 
 *              the 'pt_svl_rec' structure specifies the key field of the 
 *              service record to be added to the database.  
 *
 * Inputs: 
 *   h_svl      Handle to the SVL object.
 *   pt_svl_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           Record is successfully added. 
 *   SVLR_INV_HANDLE   Bad SVL handle.
 *   SVLR_OUT_OF_MEM   Can not allocate memory, add record failed,.
 *   SVLR_INV_ARG      'pt_svl_rec' is null or the SVL record id is a invalid.
 -----------------------------------------------------------------------*/
INT32 c_svl_add_rec
(
    HANDLE_T           h_svl,
    const SVL_REC_T*   pt_svl_rec
)
{
    return x_svl_add_rec(h_svl, pt_svl_rec);
}

/*----------------------------------------------------------------------
 * Name: c_svl_del_rec
 *
 * Description: This function deletes a specified service record from 
 *              the SVL.  The record to be deleted is specified by the 
 *              service record id.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   ui2_svl_rec_id  Specifies id the service record to be deleted. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK            Specified record is deleted. 
 *   SVLR_INV_HANDLE    Bad SVL handle.
 *   SVLR_INV_ARG       ui2_svl_rec_id is a NULL record id.
 *   SVLR_REC_NOT_FOUND The specified record to be deleted is not in 
 *                      the SVL object.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_del_rec
(
    HANDLE_T           h_svl,
    UINT16             ui2_svl_rec_id
)
{
    return x_svl_del_rec(h_svl, ui2_svl_rec_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_update_rec
 *
 * Description: This function updates a service record to the SVL object.   
 *              If the key field value (e.g., 'ui2_svl_rec_id' field in 
 *              the 'pt_svl_rec' structure) of the input service record 
 *              matches an existing record in the SVL object, then the 
 *              existing record is over-wrote with the new information,  
 *              otherwise a new record is created in the SVL database. 
 * 
 *              If the 'b_must_exist' flag is TRUE, then there must be an 
 *              existing record with the same key field value, else a status 
 *              of 'SVLR_REC_NOT_FOUND' is returned to the caller.  
 *              If 'b_must_exist' flag is FALSE, then a new service record 
 *              is created if no existing record matching the key field value 
 *              is found in the SVL database.
 *
 *              Call to this function must be protected, e.g., the SVL 
 *              database object needs to be locked before the call to this 
 *              function and unlock after returning from call to this function.
 *
 * Inputs: 
 *   h_svl      Handle to the SVL object.
 *   pt_svl_rec Pointer to service record structure. 
 *   b_must_exist Boolean flag specifying if the service record to be 
 *                updated should exist in the SVL database
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           Record is successfully added/updated.
 *   SVLR_INV_HANDLE   Bad SVL handle. 
 *   SVLR_OUT_OF_MEM   Not enough memory to create a new SVL record.
 *   SVLR_REC_NOT_FOUND The specified record is not found 
 *                      ('b_must_exist' == TRUE)
 *   SVLR_INV_ARG      'pt_svl_rce' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_update_rec
(
    HANDLE_T           h_svl,
    const SVL_REC_T*   pt_svl_rec,
    BOOL               b_must_exist	
)
{
    return x_svl_update_rec(h_svl, pt_svl_rec, b_must_exist);
}

/*----------------------------------------------------------------------
 * Name: c_svl_qry_rec_id
 *
 * Description: This API queries the range (FIRST, LAST, or NEXT) of 
 *              valid service record id values used in the SVL object.  
 *   
 * Inputs: 
 *   h_svl             Handle to the SVL object.
 *   e_svl_qry_option  Flag to specifies the record id value to be returned.  
 *                     The values are:
 *                     SVL_FIRST_REC_ID,  SVL_LAST_REC_ID, SVL_NEXT_REC_ID.
 *
 * Outputs: None
 *   pui2_rec_id       Contain the record id value.	
 *
 * Returns: 
 *   SVLR_OK           SVL record id return.
 *   SVLR_INV_HANDLE   Bad SVL handle. 
 *   SVLR_INV_ARG      'pui2_rec_id' is NULL.

 -----------------------------------------------------------------------*/
INT32 c_svl_qry_rec_id
(
    HANDLE_T           h_svl,
    SVL_REC_ID_QRY_T   e_svl_qry_option,
    UINT16*            pui2_rec_id	
)
{
    return x_svl_qry_rec_id(h_svl, e_svl_qry_option, pui2_rec_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_lock
 *
 * Description: This function locks the SVL object.   The function blocks 
 *              until the lock is acquired.
 *
 * Inputs: 
 *   h_svl	Handle to the SVL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           SVL is locked. 
 *   SVLR_INV_HANDLE   Bad SVL handle.
 *
 -----------------------------------------------------------------------*/
INT32 c_svl_lock
(
    HANDLE_T           h_svl
)
{
    return x_svl_lock(h_svl);
}

/*----------------------------------------------------------------------
 * Name: c_svl_unlock
 *
 * Description: This function unlocks the SVL database. 
 *
 * Inputs: 
 *   h_svl	Handle to the SVL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK          SVL database is unlocked. 
 *   SVLR_INV_HANDLE  Bad SVL handle.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_unlock
(
    HANDLE_T           h_svl
)
{
    return x_svl_unlock(h_svl);
}

/*----------------------------------------------------------------------
 * Name: c_svl_read_lock
 *
 * Description: This function locks the SVL object for read operation.
 *    The function blocks until the lock is acquired.
 *
 * Inputs: 
 *   h_svl	Handle to the SVL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           SVL is locked. 
 *   SVLR_INV_HANDLE   Bad SVL handle.
 *
 -----------------------------------------------------------------------*/
INT32 c_svl_read_lock
(
    HANDLE_T           h_svl
)
{
    return x_svl_read_lock(h_svl);
}

/*----------------------------------------------------------------------
 * Name: c_svl_read_unlock
 *
 * Description: This function release read-lock on the SVL database. 
 *
 * Inputs: 
 *   h_svl	Handle to the SVL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK          SVL database is unlocked. 
 *   SVLR_INV_HANDLE  Bad SVL handle.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_read_unlock
(
    HANDLE_T           h_svl
)
{
    return x_svl_read_unlock(h_svl);
}

/*----------------------------------------------------------------------
 * Name: c_svl_fs_store
 *
 * Description: This API stores the specified SVL object to the 
 *              file system.
 *
 * Inputs:
 *   h_dir         The handle to the directory.  The 'ps_svl_path'
 *                 will be relative to the specified 'h_dir'.
 *
 *   ps_svl_path   Specifies the filename path to store the SVL 
 *                 object in the file system.  
 *
 *  ui2_svl_id     Specifies the SVL object to be stored to the file system.  
 *                 The limit for SVL ID is between 1 and 65535.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK             SVL object is stored to the file system. 
 *
 *   SVLR_NOT_FOUND      The SVL object specified by the 'ui2_svl_id' 
 *                       does not exist.
 *
 *   SVLR_INV_ARG        ps_svl_path' is NULL.
 *
 *   SVLR_INV_FILE_PATH  Specified file path is not a directory or 
 *                       a RAW character file.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_fs_store
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_svl_id	
)
{
    return x_svl_fs_store(h_dir, ps_svl_path, ui2_svl_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_fs_del
 *
 * Description: This API deletes a specified SVL object on the file system.
 *
 * Inputs:
 *   h_dir          The handle to the directory.  The 'ps_svl_path'
 *                  will be relative to the specified 'h_dir'.
 *
 *   ps_svl_path    Specifies the filename path of the SVL object in 
 *                  the file system. 
 *
 *   ui2_svl_id     Specifies the SVL object to be deleted from the 
 *                  file system.  The limit for SVL ID is between 1 
 *                  and 65535.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK             SVL object is deleted from the file system. 
 *
 *   SVLR_NOT_FOUND      'ui2_svl_id' is not found.
 *
 *   SVLR_INV_ARG        'ps_svl_path' is NULL.
 *
 *   SVLR_INV_FILE_PATH  Specified file path, 'ps_svl_path', does not exist, or
 *                       it is not a directory or a RAW character file.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_fs_del
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_svl_id
)
{
    return x_svl_fs_del(h_dir, ps_svl_path, ui2_svl_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_fs_qry
 *
 * Description: This API queries the SVL objects stored in file system.
 *              It return the SVL object name and id value.
 * Inputs:
 *   h_dir         The handle to the directory.  The 'ps_svl_path'
 *                 will be relative to the specified 'h_dir'.
 *
 *   ps_svl_path   Specifies the directory path where SVL objects 
 *                 are stored.  
 *
 *   ui2_idx       Index value to get the next SVL object.  
 *                 0 means the first SVL, 1 is the second SVL, etc.
 *
 * Outputs: 
 *   pui2_svl_id   Pointer to a UINT16 variable. On return, it contains 
 *                 SVL ID.
 *
 *   ps_svl_name   Pointer to a string buffer (minimum 17 bytes) .  
 *                 On return, it contains the default SVL name stored on 
 *                 the file system. 
 *
 * Returns: 
 *   SVLR_OK              SVL object found. 
 *
 *   SVLR_NOT_FOUND       No SVL object found.
 *
 *   SVLR_INV_ARG         'ps_svl_name' or 'pui2_svl_id' is NULL.
 *
 *   SVLR_INV_FILE_PATH   Specified file path is not a directory or a 
 *                        RAW character file.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_fs_qry
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_idx,
    UINT16*	   	    pui4_svl_id,
    CHAR*           ps_svl_name	
)
{
    return x_svl_fs_qry(h_dir, ps_svl_path, ui2_idx,
                        pui4_svl_id, ps_svl_name);
}

/*----------------------------------------------------------------------
 * Name: c_svl_fs_sync
 *
 * Description: This API sync a specified SVL handle with SVL records
 *              from persistent file system.  The existing SVL records
 *              are replaced with new records read from file. 
 *
 * Inputs:
 *
 *   h_svl          Specify the handle to the SVL object.
 *
 *   h_dir          The handle to the directory.  The 'ps_svl_path'
 *                  will be relative to the specified 'h_dir'.
 *
 *   ps_svl_path    Specifies the directory path of the SVL object in the 
 *                  file system. See File Path Specification, 10.1, for the 
 *                  types of file path that can be specified.
 *
 *   ui2_svl_id     Specifies the SVL id to load from the file system.  
 *                  The limit for SVL ID is between 1 and 65535.
 *
 * Outputs: 
 *
 *   None.
 *                 
 * Returns: 
 *   SVLR_OK             SVL records are load from the persistent file
 *                       into the memory of SVL object.
 *
 *   SVLR_NOT_FOUND      'ui2_svl_id' is not found or 'ps_svl_path' 
 *                       does not exist.
 *
 *   SVLR_FAIL           internal error
 *
 *   SVLR_INV_ARG        'ps_svl_path' is NULL, or 'h_svl' is NULL_HANDLE,
 *                       or ui2_svl_id is 0.
 *
 *   SVLR_INV_FILE_PATH  Specified file path is not a directory or a RAW 
 *                       character file.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_fs_sync
(
    HANDLE_T		h_svl,	
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_svl_id
)
{
    return x_svl_fs_sync(h_svl, h_dir, ps_svl_path, ui2_svl_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_get_handle_info
 *
 * Description: This function gets the SVL ID and SVL name for the 
 *              specified handle.    
 *
 * Inputs: 
 *   h_svl	Handle to the SVL object.
 *
 * Outputs: 
 *   pui2_svl_id  Pointer to SVL ID variable.  On return it contains 
 *                the SVL ID value for this handle.
 *   ps_svl_name  Pointer to a char array variable (the size of this char 
 *                array should be at least MAX_SVL_NAME_LEN+1).  On return, 
 *                this array contains the name of the SVL object.
 * Returns: 
 *   SVLR_OK           Information about this handle (svl id and svl name)
 *                     is returned.
 *   SVLR_INV_HANDLE   Bad handle.
 *   SVLR_INV_ARG      'pui2_svl_id' or 'ps_svl_name' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_get_handle_info
(
    HANDLE_T		h_svl,
    UINT16*			pui2_svl_id,
    CHAR*			ps_svl_name
)
{
    return x_svl_get_handle_info(h_svl, pui2_svl_id, ps_svl_name);
}

/*----------------------------------------------------------------------
 * Name: c_svl_get_rec
 *
 * Description: This API gets a specific record stored in the SVL database 
 *              object.    The search parameter is specified by the 
 *              'ui2_svl_rec_id' value.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   ui2_svl_rec_id  Specifies the service record id.
 *
 * Outputs: 
 *   pt_svl_rec Pointer to a service record structure.   This structure must 
 *              be allocated by the caller.  On return, it is filled with 
 *              service record information.
 *
 *   pui4_ver_id Pointer to a variable holding version id of the database 
 *              object.   Note: version id of the SVL database object is 
 *              incremented each time when a modification takes place.
 * Returns: 
 *   SVLR_OK    Data from the specified record is returned. 
 *   SVLR_INV_HANDLE    Bad handle.
 *   SVLR_REC_NOT_FOUND No record matching the search parameter 
 *                      ('ui2_svl_rec_id')  is found.
 *    SVLR_INV_ARG       'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_get_rec
(
    HANDLE_T            h_svl,
    UINT16			    ui2_svl_rec_id,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id
)
{
    return x_svl_get_rec(h_svl, ui2_svl_rec_id, pt_svl_rec, pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_get_rec_by_src_id
 *
 * Description: This API returns service record entry with a specified 
 *              source id value and network mask parameter from the SVL 
 *              database object.  If there are multiple service records 
 *              matching the specified channel number, the caller can use 
 *              'ui2_idx' to iterate over service records with the same 
 *              channel number.    
 *
 * Inputs: 
 *   h_svl            Handle to the SVL object.
 *   ui2_source_id  Specifies the source id of the service records 
 *                  to be returned.
 *   ui4_nw_mask  32-bits mask specifying the network(s) of the service 
 *                record to be returned.  
 *   ui2_idx      Index for iterating the SVL for service records matching 
 *                the specified channel number.  0 means the first record
 *                found in SVL list that matches the specified channel 
 *                number and network mask parameter, and 1 is the next record
 *                found, and so on.
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
INT32 c_svl_get_rec_by_src_id
(
    HANDLE_T           h_svl,
    UINT16             ui2_source_id,
    UINT32		       ui4_nw_mask,
    UINT16		       ui2_idx,
    SVL_REC_T*         pt_svl_rec,
    UINT32*            pui4_ver_id
)
{
    return x_svl_get_rec_by_src_id(h_svl,ui2_source_id,ui4_nw_mask,
                                   ui2_idx, pt_svl_rec,pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_get_rec_by_channel
 *
 * Description: This API returns service record entry with a specified 
 *              channel number and network mask parameter from the SVL 
 *              database object.  If there are multiple service records 
 *              matching the specified channel number, the caller can use 
 *              'ui2_idx' to iterate over service records with the same 
 *              channel number.    
 *
 * Inputs: 
 *   h_svl            Handle to the SVL object.
 *   ui2_channel_num  Specifies the channel number of the service records 
 *                    to be returned.
 *   ui4_nw_mask  32-bits mask specifying the network(s) of the service 
 *                record to be returned.  
 *   ui2_idx      Index for iterating the SVL for service records matching 
 *                the specified channel number.  0 means the first record
 *                found in SVL list that matches the specified channel 
 *                number and network mask parameter, and 1 is the next record
 *                found, and so on.
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
INT32 c_svl_get_rec_by_channel
(
    HANDLE_T			h_svl,
    UINT32			    ui4_channel_num,
    UINT32			    ui4_nw_mask,
    UINT16			    ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
)
{
    return x_svl_get_rec_by_channel(h_svl, ui4_channel_num, ui4_nw_mask,
                                    ui2_idx, pt_svl_rec, pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_get_rec_by_brdcst_type
 *
 * Description: This function returns service record with a specified
 *              broadcast type (e.g., BRDCST_TYPE_ANALOG, BRDCST_TYPE_ATSC,
 *              or BRDCST_TYPE_DVB) and network.  If there are multiple
 *              service records matching the specified broadcast type
 *              (see 9.3.10, x_svl_get_num_rec_by_brdcst_type), the client
 *              can use ui2_idx to iterate over service records with the same
 *              broadcast type.
 * 
 *              If the specified broadcast type is BRDCST_TYPE_UNKNOWN, then
 *              all the service records for the specified networks are matched.
 *
 *              Note: the returned records will be sorted by channel number
 *              (ascending order).  For ATSC broadcast type, the returned
 *              records will be sorted by major number and minor number, in
 *              that order (see section 4.2.1 on specifying the channel number
 *              field in the service list record for ATSC broadcast).
 *
 *              If another caller locks (e.g., to perform write operation)
 *              the SVL object reference by the 'h_svl' handle, then this
 *              function will block until the other caller has unlock the
 *              SVL object.
 *
 * Inputs: 
 *   h_svl	        Handle to the SVL object.
 *   e_brdcst_type	Specifies the broadcast type (ANALOG, DVB, or ATSC) of
 *                  the service records to be returned.
 *   ui4_nw_mask    32-bits mask specifying the network(s) of the service
 *                  record to be returned.
 *                  The 'ui4_nw_mask' value is compared with the service
 *                  record's network value using bit-wise AND operation.
 *                  The comparison is considered a positive match if the
 *                  result is greater than 0.  For example, if SATELLITE,
 *                  CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                  0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04 will
 *                  match service records with internal network mask values
 *                  of 0x04, 0x06, and 0x07.    
 *                  
 *  ui2_idx         Index for iterating the SVL for service records matching
 *                  the specified broadcast type.  0 means the first record
 *                  found that matches the specified broadcast type and
 *                  network mask parameter, and 1 is the next record found,
 *                  and so on.	
 *
 * Outputs: 
 *  pt_svl_rec	    Pointer to a SVL record structure.
 *
 * Input/Output
 *  pui4_ver_id     Pointer to a variable holding version id of the SVL
 *                  database object.  On entry, this variable contains the
 *                  version id of the database that the caller expects.
 *                  On the return, this variable contains the current version
 *                  id of the database.   If the expected version id and
 *                  current database version id do not match, an error status
 *                  code is returned.  If 'pui4_ver_id' is set to
 *                  'SVL_NULL_VER_ID' value, then no checking is done and
 *                  the latest version id is returned the application.
 *
 * Returns: 
 *   SVLR_OK             A SVL_REC_T structure is returned. 
 *
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *
 *   SVLR_REC_NOT_FOUND  Specified record is not found.
 *
 *   SVLR_INV_ARG        'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *
 *   SVLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id'
 *                       pass into the API and the internal database version
 *                       id is not the same.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_get_rec_by_brdcst_type
(
    HANDLE_T			h_svl,
    BRDCST_TYPE_T		e_brdcst_type,
    UINT32			    ui4_nw_mask,
    UINT16			    ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
)
{
    return x_svl_get_rec_by_brdcst_type(h_svl, e_brdcst_type, ui4_nw_mask,
                                        ui2_idx, pt_svl_rec, pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_get_rec_by_ts
 *
 * Description: This function searches the SVL and returns service record
 *              with a specified Transport Stream List ID, Transport Stream
 *              record ID (e.g., TSL ID and TSL record ID) and network mask
 *              parameter.  If there are multiple service records matching
 *              the specified TSL ID, TSL record ID, and network mask, the
 *              caller can use ui2_idx to iterate over service records with
 *              the positive match.
 * 
 *              Note: the combination of TSL ID and TSL record ID uniquely
 *              identifies a transport stream record within the Middleware.
 *
 *              If another caller locks (e.g., to perform write operation)
 *              the SVL object reference by the 'h_svl' handle, then this
 *              function will block until the other caller has unlock the
 *              SVL object.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   ui2_tsl_id	     Specifies the Transport Stream List ID value.
 *   ui2_tsl_rec_id  Specifies the Transport Stream record ID value. 
 *   ui4_nw_mask     32-bits mask specifying the network(s) of the service
 *                   record to be returned.  
 *                   The 'ui4_nw_mask' value is compared with the service
 *                   record's network value using bit-wise AND operation.
 *                   The comparison is considered a positive match if the
 *                   result is greater than 0.  For example, if SATELLITE,
 *                   CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                   0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04
 *                   will match service records with internal network mask
 *                   values of 0x04, 0x06, and 0x07.    
 *                   
 *   ui2_idx         Index for iterating the SVL for service records matching
 *                   the specified broadcast type.  0 means the first record
 *                   found that matches the specified broadcast type and
 *                   network mask parameter, and 1 is the next record found,
 *                   and so on.	  
 *
 * Outputs: 
 *   pt_svl_rec	     Pointer to a SVL record structure.
 *
 * Input/Output
 *   pui4_ver_id     Pointer to a variable holding version id of the SVL
 *                   database object.  On entry, this variable contains the
 *                   version id of the database that the caller expects.
 *                   On the return, this variable contains the current
 *                   version id of the database.   If the expected version id
 *                   and current database version id do not match, an error
 *                   status code is returned.  If 'pui4_ver_id' is set to
 *                   'SVL_NULL_VER_ID' value, then no checking is done and
 *                   the latest version id is returned the application.
 * Returns: 
 *   SVLR_OK             A SVL_REC_T structure is returned. 
 *
 *   SVLR_INV_HANDLE     Bad 'h_svl' handle.
 *
 *   SVLR_REC_NOT_FOUND  Specified record is not found.
 *
 *   SVLR_INV_ARG        'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *
 *   SVLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id'
 *                       pass into the API and the internal database version
 *                       id is not the same.
 -----------------------------------------------------------------------*/
INT32 c_svl_get_rec_by_ts
(
    HANDLE_T			h_svl,
    UINT16			    ui2_tsl_id,
    UINT16			    ui2_tsl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16		  	    ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
)
{
    return x_svl_get_rec_by_ts(h_svl, ui2_tsl_id, ui2_tsl_rec_id,
                               ui4_nw_mask, ui2_idx, pt_svl_rec,
                               pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_get_rec_by_nw
 *
 * Description: This function searches the SVL and returns service record
 *              with a specified Network List ID, Network record ID (e.g.,
 *              NWL ID and NWL record ID) and network mask parameter.  If
 *              there are multiple service records matching the specified
 *              NWL ID, NWL record ID, and network mask, the caller can use
 *              ui2_idx to iterate over service records with the positive
 *              match.
 * 
 *              Note: the combination of NWL ID and NWL record ID uniquely
 *              identifies a network record within the Middleware.
 *
 *              If another caller locks (e.g., to perform write operation)
 *              the SVL object reference by the 'h_svl' handle, then this
 *              function will block until the other caller has unlock the
 *              SVL object.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   ui2_nwl_id	     Specifies the Network List ID value.
 *   ui2_nwl_rec_id  Specifies the Network record ID value. 
 *   ui4_nw_mask     32-bits mask specifying the network(s) of the service
 *                   record to be returned.  
 *                   The 'ui4_nw_mask' value is compared with the service
 *                   record's network value using bit-wise AND operation.
 *                   The comparison is considered a positive match if the
 *                   result is greater than 0.  For example, if SATELLITE,
 *                   CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                   0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04
 *                   will match service records with internal network mask
 *                   values of 0x04, 0x06, and 0x07.    
 *                   
 *   ui2_idx         Index for iterating the SVL for service records matching
 *                   the specified broadcast type.  0 means the first record
 *                   found that matches the specified broadcast type and
 *                   network mask parameter, and 1 is the next record found,
 *                   and so on.	  
 *
 * Outputs: 
 *   pt_svl_rec	     Pointer to a SVL record structure.
 *
 * Input/Output
 *   pui4_ver_id     Pointer to a variable holding version id of the SVL
 *                   database object.  On entry, this variable contains the
 *                   version id of the database that the caller expects.
 *                   On the return, this variable contains the current
 *                   version id of the database.   If the expected version id
 *                   and current database version id do not match, an error
 *                   status code is returned.  If 'pui4_ver_id' is set to
 *                   'SVL_NULL_VER_ID' value, then no checking is done and
 *                   the latest version id is returned the application.
 * Returns: 
 *   SVLR_OK             A SVL_REC_T structure is returned. 
 *
 *   SVLR_INV_HANDLE     Bad 'h_svl' handle.
 *
 *   SVLR_REC_NOT_FOUND  Specified record is not found.
 *
 *   SVLR_INV_ARG        'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *
 *   SVLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id'
 *                       pass into the API and the internal database version
 *                       id is not the same.
 -----------------------------------------------------------------------*/
INT32 c_svl_get_rec_by_nw
(
    HANDLE_T			h_svl,
    UINT16			    ui2_nwl_id,
    UINT16			    ui2_nwl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16		  	    ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
)
{
    return x_svl_get_rec_by_nw(h_svl, ui2_nwl_id, ui2_nwl_rec_id,
                               ui4_nw_mask, ui2_idx, pt_svl_rec,
                               pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_get_rec_by_sat
 *
 * Description: This function searches the SVL and returns service record
 *              with a specified Satellite List ID, Satellite record ID
 *              (e.g., SATL ID and SATL record ID) and network mask
 *              parameter.  If there are multiple service records matching
 *              the specified SATL ID, SATL record ID, and network mask, the
 *              caller can use ui2_idx to iterate over service records with
 *              the positive match.
 * 
 *              Note: the combination of SATL ID and SATL record ID uniquely
 *              identifies a satellite record within the Middleware.
 *
 *              If another caller locks (e.g., to perform write operation)
 *              the SVL object reference by the 'h_svl' handle, then this
 *              function will block until the other caller has unlock the
 *              SVL object.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   ui2_satl_id     Specifies the Satellite List ID value.
 *   ui2_satl_rec_id Specifies the Satellite record ID value. 
 *   ui4_nw_mask     32-bits mask specifying the network(s) of the service
 *                   record to be returned.  
 *                   The 'ui4_nw_mask' value is compared with the service
 *                   record's network value using bit-wise AND operation.
 *                   The comparison is considered a positive match if the
 *                   result is greater than 0.  For example, if SATELLITE,
 *                   CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                   0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04
 *                   will match service records with internal network mask
 *                   values of 0x04, 0x06, and 0x07.    
 *                   
 *   ui2_idx         Index for iterating the SVL for service records matching
 *                   the specified broadcast type.  0 means the first record
 *                   found that matches the specified broadcast type and
 *                   network mask parameter, and 1 is the next record found,
 *                   and so on.	  
 *
 * Outputs: 
 *   pt_svl_rec	     Pointer to a SVL record structure.
 *
 * Input/Output
 *   pui4_ver_id     Pointer to a variable holding version id of the SVL
 *                   database object.  On entry, this variable contains the
 *                   version id of the database that the caller expects.
 *                   On the return, this variable contains the current
 *                   version id of the database.   If the expected version id
 *                   and current database version id do not match, an error
 *                   status code is returned.  If 'pui4_ver_id' is set to
 *                   'SVL_NULL_VER_ID' value, then no checking is done and
 *                   the latest version id is returned the application.
 * Returns: 
 *   SVLR_OK             A SVL_REC_T structure is returned. 
 *
 *   SVLR_INV_HANDLE     Bad 'h_svl' handle.
 *
 *   SVLR_REC_NOT_FOUND  Specified record is not found.
 *
 *   SVLR_INV_ARG        'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *
 *   SVLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id'
 *                       pass into the API and the internal database version
 *                       id is not the same.
 -----------------------------------------------------------------------*/
INT32 c_svl_get_rec_by_sat
(
    HANDLE_T			h_svl,
    UINT16			    ui2_satl_id,
    UINT16			    ui2_satl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16		  	    ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
)
{
    return x_svl_get_rec_by_sat(h_svl, ui2_satl_id, ui2_satl_rec_id,
                                ui4_nw_mask, ui2_idx, pt_svl_rec,
                                pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_get_num_rec_by_channel
 *
 * Description: Return the number of SVL records matching the channel
 *              and network mask values.
 *
 * Inputs: 
 *   h_svl	          Handle to the SVL object.
 *
 *   ui4_channel_num  Specifies the channel number.
 *
 *   ui4_nw_mask      32-bits mask specifying the network(s) of the
 *                    service record to be counted.  
 *                    The 'ui4_nw_mask' value is compared with the service
 *                    record's network value using bit-wise AND operation.
 *                    The comparison is considered a positive match if the
 *                    result is greater than 0.  For example, if SATELLITE,
 *                    CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                    0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04
 *                    will match service records with internal network mask
 *                    values of 0x04, 0x06, and 0x07.    
 *
 * Outputs: 
 *   pui2_num_recs:    number of records.
 *   pui4_ver_id:      version id.
 *
 * Returns: 
 *   SVLR_OK           Number of records found. 
 *
 *   SVLR_INV_HANDLE   Bad 'h_svl' handle.
 *
 *   SVLR_INV_ARG      'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_get_num_rec_by_channel
(
    HANDLE_T			h_svl,
    UINT32			    ui4_channel_num,
    UINT32			    ui4_nw_mask,
    UINT16*			    pui2_num_recs,
    UINT32*			    pui4_ver_id
)
{
    return x_svl_get_num_rec_by_channel(h_svl, ui4_channel_num, ui4_nw_mask,
                                        pui2_num_recs, pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_get_num_rec_by_brdcst_type
 *
 * Description: Return the number of SVL records matching the broadcast
 *              type ((e.g., BRDCST_TYPE_ANALOG, BRDCST_TYPE_ATSC, or
 *              BRDCST_TYPE_DVB) and network mask values.
 *
 *              If specified broadcast type is BRDCST_TYPE_UNKNOWN, then it
 *              matches any broadcast types.
 *
 *              To get the number of all the records in the SVL, caller can
 *              specifies BRDCST_TYPE_UNKNOWN and 'network mask' parameter
 *              of 0xFFFF.
 * Inputs: 
 *   h_svl	        Handle to the SVL object.
 *   e_brdcstl_type	Specifies the broadcast type (e.g., BRDCST_TYPE_ANALOG,
 *                  BRDCST_TYPE_ATSC, BRDCST_TYPE_DVB, or BRDCST_TYPE_UNKNOWN)
 *                  of the service record to be counted.
 *
 *   ui4_nw_mask    32-bits mask specifying the network(s) of the service
 *                  record to be counted.  
 *                  The 'ui4_nw_mask' value is compared with the service
 *                  record's network value using bit-wise AND operation.
 *                  The comparison is considered a positive match if the
 *                  result is greater than 0.  For example, if SATELLITE,
 *                  CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                  0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04 will
 *                  match service records with internal network mask values
 *                  of 0x04, 0x06, and 0x07.
 *
 * Outputs: 
 *   pui2_num_recs	Pointer to a variable for the number of records matching
 *                  the specified stream type in the SVL. 	
 *
 *   pui4_ver_id    Pointer to a variable holding version id of the SVL
 *                  database object.
 * Returns: 
 *   SVLR_OK          Number of records found is returned.
 *   SVLR_INV_HANDLE  Bad 'h_svl' handle.
 *   SVLR_INV_ARG     'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 c_svl_get_num_rec_by_brdcst_type
(
    HANDLE_T			h_svl,
    BRDCST_TYPE_T		e_brdcst_type,
    UINT32			    ui4_nw_mask,
    UINT16*			    pui2_num_recs,
    UINT32*			    pui4_ver_id
)
{
    return x_svl_get_num_rec_by_brdcst_type(h_svl, e_brdcst_type,
                                            ui4_nw_mask, pui2_num_recs,
                                            pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_get_num_rec_by_ts
 *
 * Description: This function returns the number of records for the
 *              specified Transport Stream List ID, Transport Stream record
 *              ID (e.g., TSL ID and TSL record ID), and network mask
 *              parameter.
 *              Note: the combination of TSL ID and TSL record ID uniquely
 *              identifies a transport stream record within the Middleware.
 *
 * Inputs: 
 *   h_svl	    Handle to the SVL object.
 *
 *   ui2_tsl_id Specifies the Transport Stream List ID value.
 *   ui2_tsl_rec_id	Specifies the Transport Stream record ID value.  
 *
 *   ui4_nw_mask    32-bits mask specifying the network(s) of the service
 *                  record to be counted.  
 *                  The 'ui4_nw_mask' value is compared with the service
 *                  record's network value using bit-wise AND operation.
 *                  The comparison is considered a positive match if the
 *                  result is greater than 0.  For example, if SATELLITE,
 *                  CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                  0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04 will
 *                  match service records with internal network mask values
 *                  of 0x04, 0x06, and 0x07.    
 *
 * Outputs: 
 *   pui2_num_recs	Pointer to a variable for the number of records
 *                  matching the specified stream type in the SVL. 	
 *   pui4_ver_id    Pointer to a variable holding version id of the SVL
 *                  database object. 	
 * Returns: 
 *   SVLR_OK         Number of records found.
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *   SVLR_INV_ARG    'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_get_num_rec_by_ts
(
    HANDLE_T			h_svl,
    UINT16              ui2_tsl_id,
    UINT16              ui2_tsl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_ver_id
)
{
    return x_svl_get_num_rec_by_ts(h_svl, ui2_tsl_id, ui2_tsl_rec_id,
                                   ui4_nw_mask, pui2_num_recs,
                                   pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_get_num_rec_by_nw
 *
 * Description: This function returns the number of records for the
 *              specified Network List ID, Network record ID (e.g., NWL ID
 *              and NWL record ID), and network mask parameter.
 *              Note: the combination of NWL ID and NWL record ID uniquely
 *              identifies a transport stream record within the Middleware.
 *
 * Inputs: 
 *   h_svl	    Handle to the SVL object.
 *
 *   ui2_nwl_id Specifies the Network List ID value.
 *   ui2_nwl_rec_id	Specifies the Network record ID value.  
 *
 *   ui4_nw_mask    32-bits mask specifying the network(s) of the service
 *                  record to be counted.  
 *                  The 'ui4_nw_mask' value is compared with the service
 *                  record's network value using bit-wise AND operation.
 *                  The comparison is considered a positive match if the
 *                  result is greater than 0.  For example, if SATELLITE,
 *                  CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                  0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04 will
 *                  match service records with internal network mask values
 *                  of 0x04, 0x06, and 0x07.    
 *
 * Outputs: 
 *   pui2_num_recs	Pointer to a variable for the number of records
 *                  matching the specified stream type in the SVL. 	
 *   pui4_ver_id    Pointer to a variable holding version id of the SVL
 *                  database object. 	
 * Returns: 
 *   SVLR_OK         Number of records found.
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *   SVLR_INV_ARG    'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_get_num_rec_by_nw
(
    HANDLE_T			h_svl,
    UINT16              ui2_nwl_id,
    UINT16              ui2_nwl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_ver_id
)
{
    return x_svl_get_num_rec_by_nw(h_svl, ui2_nwl_id, ui2_nwl_rec_id,
                                   ui4_nw_mask, pui2_num_recs,
                                   pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_get_num_rec_by_sat
 *
 * Description: This function returns the number of records for the
 *              specified Satellite List ID, Satellite record ID (e.g., SATL
 *              ID and SATL record ID), and network mask parameter.
 *              Note: the combination of SATL ID and SATL record ID uniquely
 *              identifies a transport stream record within the Middleware.
 *
 * Inputs: 
 *   h_svl	    Handle to the SVL object.
 *
 *   ui2_satl_id        Specifies the Satellite List ID value.
 *   ui2_satl_rec_id	Specifies the Satellite record ID value.  
 *
 *   ui4_nw_mask    32-bits mask specifying the network(s) of the service
 *                  record to be counted.  
 *                  The 'ui4_nw_mask' value is compared with the service
 *                  record's network value using bit-wise AND operation.
 *                  The comparison is considered a positive match if the
 *                  result is greater than 0.  For example, if SATELLITE,
 *                  CABLE, and TERRESTRIAL networks are defined to be 0x01,
 *                  0x02 and 0x04. Then specifying 'ui4_nw_mask' of 0x04 will
 *                  match service records with internal network mask values
 *                  of 0x04, 0x06, and 0x07.    
 *
 * Outputs: 
 *   pui2_num_recs	Pointer to a variable for the number of records
 *                  matching the specified stream type in the SVL. 	
 *   pui4_ver_id    Pointer to a variable holding version id of the SVL
 *                  database object. 	
 * Returns: 
 *   SVLR_OK         Number of records found.
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *   SVLR_INV_ARG    'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_get_num_rec_by_sat
(
    HANDLE_T			h_svl,
    UINT16              ui2_satl_id,
    UINT16              ui2_satl_rec_id,
    UINT32			    ui4_nw_mask,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_ver_id
)
{
    return x_svl_get_num_rec_by_sat(h_svl, ui2_satl_id, ui2_satl_rec_id,
                                    ui4_nw_mask, pui2_num_recs,
                                    pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_iterate_rec
 *
 * Description: This API iterates over service records in the SVL list.  
 *              It returns the next service record from a specified the 
 *              channel number of the current service record location.  
 *              The 'e_iter_dir' argument specifies direction of iteration, 
 *              e.g., the next record's channel numeric value is plus or 
 *              minus from the current record's channel numeric value.  
 *              Service records that do not match the network mask 
 *              specification will be skipped during the iteration.  
 *
 *              If the current record is at the end or beginning of the 
 *              service list, then 'CHANNEL_PLUS' or 'CHANNEL_MINUS' will 
 *              return a status to indicate that end-point is reached. It is 
 *              up the caller to decide if next service record should wrap 
 *              around to the beginning service record or to the end of the 
 *              service record.
 *
 *              Since the service record in the SVL is sorted by 'channel
 *              number', the iteration can be thought of as getting the next 
 *              or the previous channel.  For ATSC broadcast type, this 
 *              implies the returned records will be sorted by major number 
 *              and minor number, in that order.
 *
 *              Specifying 'CHANNEL_SMALLEST' returns the record with the 
 *              smallest channel number (regardless of the current service 
 *              record position), and similarly specifying 'CHANNEL_LARGEST' 
 *              returns the record with the largest channel number.
 *
 * Inputs: 
 *    h_svl     Handle to the SVL object.
 *    ui4_current_channel   Specifies the channel number of the current 
 *                          service record to begin for the search for 
 *                          the next service record.
 *    ui4_nw_mask   32-bits mask specifying the network(s) of the service 
 *                  record to be returned.  
 *
 *    e_iter_dir    Specifies the direction of the iteration.  The values are:
 *
 *                        SVL_CHANNEL_PLUS	
 *                        SVL_CHANNEL_MINUS
 *                        SVL_CHANNEL_SMALLEST
 *                        SVL_CHANNEL_LARGEST
 *
 *                  If SVL_CHANNEL_SMALLEST or SVL_CHANNEL_LARGEST is 
 *                  specified, then the record with the smallest (numerically)
 *                  channel value is returned, and vice versa.  If the 
 *                  iteration has reach an end point, i.e., the current record
 *                  is point to the first or last service record, then the 
 *                  current service record is returned.
 *
 * Outputs: 
 *    pt_svl_rec    Pointer to a SVL record structure.
 *
 * In/Output:
 *    pui4_ver_id   Pointer to a variable holding version id of the SVL
 *                  database object.  On entry, this variable contains the
 *                  version id of the database that the caller expects.  
 *                  On the return, this variable contains the current version 
 *                  id of the database.   If the expected version id and 
 *                  current database version id do not match, an error status 
 *                  code is returned.  If 'pui4_ver_id' is set to 
 *                  'SVL_NULL_VER_ID' value, then no checking is done and the 
 *                  latest version id is returned the application.
 * Returns: 
 *   SVLR_OK           
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_iterate_rec
(
    HANDLE_T			h_svl,
    UINT32              ui4_current_channel,
    UINT32              ui4_nw_mask,
    SVL_ITERATE_T		e_iter_dir,  /* channel + or - */
    SVL_REC_T*			pt_svl_rec,
    UINT32*             pui4_ver_id 
)
{    
    return x_svl_iterate_rec(h_svl, ui4_current_channel, ui4_nw_mask,
                             e_iter_dir, pt_svl_rec, pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_iterate_by_nw
 *
 * Description: This API iterates over service records in the SVL list with
 *              reference to the given NWL record.  It returns the next
 *              matching service record from a specified the channel number
 *              of the current service record location.  
 *              The 'e_iter_dir' argument specifies direction of iteration, 
 *              e.g., the next record's channel numeric value is plus or 
 *              minus from the current record's channel numeric value.  
 *              Service records that do not match the network mask and NWL
 *              ID / NWL record ID specified will be skipped during the
 *              iteration.  
 *
 *              If the current record is at the end or beginning of the 
 *              service list, then 'CHANNEL_PLUS' or 'CHANNEL_MINUS' will 
 *              return a status to indicate that end-point is reached. It is 
 *              up the caller to decide if next service record should wrap 
 *              around to the beginning service record or to the end of the 
 *              service record.
 *
 *              Since the service record in the SVL is sorted by 'channel
 *              number', the iteration can be thought of as getting the next 
 *              or the previous channel in the specified network.
 *
 *              Specifying 'CHANNEL_SMALLEST' returns the record with the 
 *              smallest channel number (regardless of the current service 
 *              record position), and similarly specifying 'CHANNEL_LARGEST' 
 *              returns the record with the largest channel number.
 *
 * Inputs: 
 *    h_svl     Handle to the SVL object.
 *    ui4_current_channel   Specifies the channel number of the current 
 *                          service record to begin for the search for 
 *                          the next service record.
 *    ui4_nw_mask   32-bits mask specifying the network(s) of the service 
 *                  record to be returned.  
 *    ui2_nwl_id        Specifies the Network List ID value.
 *    ui2_nwl_rec_id	Specifies the Network record ID value.  
 *    e_iter_dir    Specifies the direction of the iteration.  The values are:
 *
 *                        SVL_CHANNEL_PLUS	
 *                        SVL_CHANNEL_MINUS
 *                        SVL_CHANNEL_SMALLEST
 *                        SVL_CHANNEL_LARGEST
 *
 *                  If SVL_CHANNEL_SMALLEST or SVL_CHANNEL_LARGEST is 
 *                  specified, then the record with the smallest (numerically)
 *                  channel value is returned, and vice versa.  If the 
 *                  iteration has reach an end point, i.e., the current record
 *                  is point to the first or last service record, then the 
 *                  current service record is returned.
 *
 * Outputs: 
 *    pt_svl_rec    Pointer to a SVL record structure.
 *
 * In/Output:
 *    pui4_ver_id   Pointer to a variable holding version id of the SVL
 *                  database object.  On entry, this variable contains the
 *                  version id of the database that the caller expects.  
 *                  On the return, this variable contains the current version 
 *                  id of the database.   If the expected version id and 
 *                  current database version id do not match, an error status 
 *                  code is returned.  If 'pui4_ver_id' is set to 
 *                  'SVL_NULL_VER_ID' value, then no checking is done and the 
 *                  latest version id is returned the application.
 * Returns: 
 *   SVLR_OK           
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_iterate_by_nw
(
    HANDLE_T			h_svl,
    UINT32              ui4_current_channel,
    UINT32              ui4_nw_mask,
    UINT16              ui2_nwl_id,
    UINT16              ui2_nwl_rec_id,
    SVL_ITERATE_T		e_iter_dir,  /* channel + or - */
    SVL_REC_T*			pt_svl_rec,
    UINT32*             pui4_ver_id 
)
{    
    return x_svl_iterate_by_nw(h_svl, ui4_current_channel, ui4_nw_mask,
                               ui2_nwl_id, ui2_nwl_rec_id,
                               e_iter_dir, pt_svl_rec, pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_iterate_by_sat
 *
 * Description: This API iterates over service records in the SVL list with
 *              reference to the given SATL record.  It returns the next
 *              matching service record from a specified the channel number
 *              of the current service record location.  
 *              The 'e_iter_dir' argument specifies direction of iteration, 
 *              e.g., the next record's channel numeric value is plus or 
 *              minus from the current record's channel numeric value.  
 *              Service records that do not match the network mask and SATL
 *              ID / SATL record ID specified will be skipped during the
 *              iteration.  
 *
 *              If the current record is at the end or beginning of the 
 *              service list, then 'CHANNEL_PLUS' or 'CHANNEL_MINUS' will 
 *              return a status to indicate that end-point is reached. It is 
 *              up the caller to decide if next service record should wrap 
 *              around to the beginning service record or to the end of the 
 *              service record.
 *
 *              Since the service record in the SVL is sorted by 'channel
 *              number', the iteration can be thought of as getting the next 
 *              or the previous channel in the specified satellite.
 *
 *              Specifying 'CHANNEL_SMALLEST' returns the record with the 
 *              smallest channel number (regardless of the current service 
 *              record position), and similarly specifying 'CHANNEL_LARGEST' 
 *              returns the record with the largest channel number.
 *
 * Inputs: 
 *    h_svl     Handle to the SVL object.
 *    ui4_current_channel   Specifies the channel number of the current 
 *                          service record to begin for the search for 
 *                          the next service record.
 *    ui4_nw_mask   32-bits mask specifying the network(s) of the service 
 *                  record to be returned.  
 *    ui2_satl_id       Specifies the Satellite List ID value.
 *    ui2_satl_rec_id	Specifies the Satellite record ID value.  
 *    e_iter_dir    Specifies the direction of the iteration.  The values are:
 *
 *                        SVL_CHANNEL_PLUS	
 *                        SVL_CHANNEL_MINUS
 *                        SVL_CHANNEL_SMALLEST
 *                        SVL_CHANNEL_LARGEST
 *
 *                  If SVL_CHANNEL_SMALLEST or SVL_CHANNEL_LARGEST is 
 *                  specified, then the record with the smallest (numerically)
 *                  channel value is returned, and vice versa.  If the 
 *                  iteration has reach an end point, i.e., the current record
 *                  is point to the first or last service record, then the 
 *                  current service record is returned.
 *
 * Outputs: 
 *    pt_svl_rec    Pointer to a SVL record structure.
 *
 * In/Output:
 *    pui4_ver_id   Pointer to a variable holding version id of the SVL
 *                  database object.  On entry, this variable contains the
 *                  version id of the database that the caller expects.  
 *                  On the return, this variable contains the current version 
 *                  id of the database.   If the expected version id and 
 *                  current database version id do not match, an error status 
 *                  code is returned.  If 'pui4_ver_id' is set to 
 *                  'SVL_NULL_VER_ID' value, then no checking is done and the 
 *                  latest version id is returned the application.
 * Returns: 
 *   SVLR_OK           
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_iterate_by_sat
(
    HANDLE_T			h_svl,
    UINT32              ui4_current_channel,
    UINT32              ui4_nw_mask,
    UINT16              ui2_satl_id,
    UINT16              ui2_satl_rec_id,
    SVL_ITERATE_T		e_iter_dir,  /* channel + or - */
    SVL_REC_T*			pt_svl_rec,
    UINT32*             pui4_ver_id 
)
{    
    return x_svl_iterate_by_sat(h_svl, ui4_current_channel, ui4_nw_mask,
                                ui2_satl_id, ui2_satl_rec_id,
                                e_iter_dir, pt_svl_rec, pui4_ver_id);
}


/*----------------------------------------------------------------------
 * Name: c_svl_sort_rec
 *
 * Description: This API applies the sort operation (using the
 *              per-handle compare fucntion, or the global compare
 *              function, or default compare function, to the SVL
 *              records.
 *             
 * Inputs: 
 *    h_svl     Handle to the SVL object.
 *
 * Outputs: 
 *    pui4_ver_id   Contains the version id of the SVL database object.   
 *                  
 * Returns: 
 *   SVLR_OK           
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_sort_rec
(
    HANDLE_T			h_svl,
    UINT32*             pui4_ver_id 
)
{
    return x_svl_sort_rec(h_svl, pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_get_ver_id
 *
 * Description: This function returns the current version id of the
 *              SVL database object.
 *
 * Inputs: 
 *   h_svl        Handle to the SVL object.
 *
 * Outputs:
 *   pui4_ver_id  Pointer to a variable holding version id of the SVL
 *                database object.
 * Returns: 
 *   SVLR_OK         Version id is returned. 
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *   SVLR_INV_ARG    'pui4_ver_id' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 c_svl_get_ver_id
(
    HANDLE_T			h_svl,
    UINT32*			    pui4_ver_id
)
{
    return x_svl_get_ver_id(h_svl, pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_rec_init
 *
 * Description: This API pre-sets the SVL_REC_T structure to a 
 * pre-defined invalid value (e.g., 0xFF).   The pre-defined invalid 
 * value can be used to detect changes in the SVL_REC_T structure for 
 * backward compatibility in the future.
 *
 * For example, if a new field is added to the SVL record structure.  
 * At the runtime, if the new field in the SVL_REC_T structure passed 
 * into SVL library contain 0xFF, then we know the caller has not been 
 * re-compile with the new SVL module.   
 *
 * Inputs: 
 *   Pointer to a SVL record structure. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           SVL library is initialized successfully.
 *   SVLR_INV_ARG      'pt_svl_rec' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 c_svl_rec_init
(
    SVL_REC_T*			pt_svl_rec 
)
{
    return x_svl_rec_init(pt_svl_rec);
}

/*----------------------------------------------------------------------
 * Name: c_svl_reg_cmp_fcts
 *
 * Description: This function register compare functions for
 *              sorting and for searching operation.
 *
 *              If global flag is TRUE, then this compare functions
 *              apply to all SVL handle points to the same CDB database
 *              object.
 *
 *              Otherwise, the compare function are SVL handle specific.
 *
 * Inputs: 
 *   h_svl        Handle to the SVL object.
 *
 *   b_global     TRUE:  compare functions are global scope.
 *                FALSE: compare functions are SVL handle specific.
 *
 *   pf_sort_cmp  Compare function used for sorting.
 *
 *   pv_sort_info
 *
 *   pf_search_cmp Compare function used for searching.
 *
 * Outputs:
 *   None.
 *
 * Returns: 
 *   SVLR_OK         Version id is returned. 
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *   SVLR_INV_ARG    'pf_sort_cmp' or 'pf_search_cmp' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 c_svl_reg_cmp_fcts
(
    HANDLE_T               h_svl,
    BOOL                   b_global,
    VOID*                  pv_tag,
    x_svl_sort_cmp         pf_sort_cmp,
    x_svl_search_cmp       pf_search_cmp
)
{
    return x_svl_reg_cmp_fcts(h_svl,
                              b_global,
                              pv_tag,
                              pf_sort_cmp,
                              pf_search_cmp);
}

/*----------------------------------------------------------------------
 * Name: c_svl_unreg_cmp_fcts
 *
 * Description: This function un-register compare functions for
 *              sorting and for searching operation.
 *
 *              If global flag is TRUE, then this compare functions
 *              apply to all SVL handle points to the same CDB database
 *              object.
 *
 *              Otherwise, the compare function are SVL handle specific.
 *
 * Inputs: 
 *   h_svl        Handle to the SVL object.
 *
 *   b_global     TRUE:  compare functions are global scope.
 *                FALSE: compare functions are SVL handle specific.
 *
 * Outputs:
 *   None.
 *
 * Returns: 
 *   SVLR_OK         Version id is returned. 
 *   SVLR_INV_HANDLE Bad 'h_svl' handle.
 *   SVLR_INV_ARG    'pf_sort_cmp' or 'pf_search_cmp' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 c_svl_unreg_cmp_fcts
(
    HANDLE_T               h_svl,
    BOOL                   b_global
)
{
    return x_svl_unreg_cmp_fcts(h_svl, b_global);
}

/*----------------------------------------------------------------------
 * Name: c_svl_get_client_data
 *
 * Description: This API gets customer's private data associated with
 *    the specified SVL record (e.g.,'ui2_svl_rec_id')
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   ui2_svl_rec_id  Specifies the service record id.
 *
 * Outputs: 
 *   pt_client_data  Pointer to a structure containing customer data
 *                   If the buffer is too small, only data up to the
 *                   len of 'pt_client_data->ui1_buf_len' is copied out.
 *                   The paramter 'pt_client_data->ui1_data_len' will be
 *                   set to indicate the total len of customer data in
 *                   the internal SVL record. 
 *
 *   pui4_ver_id     Pointer to a variable holding version id of the
 *                   database object.   Note: version id of the SVL
 *                   database object is incremented each time when a
 *                   modification takes place.
 * Returns: 
 *   SVLR_OK    Customer's private data from the specified record is returned. 
 *   SVLR_INV_HANDLE    Bad handle.
 *   SVLR_REC_NOT_FOUND No record matching the search parameter 
 *                      ('ui2_svl_rec_id')  is found.
 *   SVLR_INV_ARG       'pui4_ver_id' or 'pt_svl_rec' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_get_client_data
(
    HANDLE_T               h_svl,
    UINT16                 ui2_svl_rec_id,
    SVL_CLIENT_DATA_T*     pt_client_data,
    UINT32*                pui4_ver_id
)
{
    return x_svl_get_client_data(h_svl,
                                 ui2_svl_rec_id,
                                 pt_client_data,
                                 pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_set_client_data
 *
 * Description: This API copies customer's private data associated with
 *    the specified SVL record (e.g.,'ui2_svl_rec_id') to SVL database.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   ui2_svl_rec_id  Specifies the service record id.
 *   pt_client_data  Pointer to a structure containing customer data
 *                   The paramter 'pt_client_data->ui1_data_len' indicates
 *                   the total len of customer data in the 'pt_client_data->
 *                   pui1_data_buf' buffer.
 *
 * Outputs: 
 *   None
 *
 * Returns: 
 *   SVLR_OK            Customer's private data is copied into SVL database. 
 *   SVLR_INV_HANDLE    Bad handle.
 *   SVLR_REC_NOT_FOUND No record matching the search parameter 
 *                      ('ui2_svl_rec_id')  is found.
 *   SVLR_INV_ARG       'pt_client_data' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_set_client_data
(
    HANDLE_T               h_svl,
    UINT16                 ui2_svl_rec_id,
    SVL_CLIENT_DATA_T*     pt_client_data
)
{
    return x_svl_set_client_data(h_svl,
                                 ui2_svl_rec_id,
                                 pt_client_data);
}

/*----------------------------------------------------------------------
 * Name: c_svl_iterate_rec_by_custom_ord
 *
 * Description: This API iterates over service records in the SVL list.  
 *              It returns the next/prev service record from a given
 *              service record location specified by the pv_data.
 *              
 *              The 'e_iter_dir' argument specifies direction of iteration, 
 *              e.g., the next record or prev record from the current
 *              record location identified by the 'pv_data'
 *
 *              Service records that do not match the network mask 
 *              specification will be skipped during the iteration.  
 *
 *              If the current record is at the end or beginning of the 
 *              sorted service list, then 'SVL_NEXT_REC' or 'SVL_PREV_REC'  
 *              returns a status to indicate that end-point is reached. It is 
 *              up the caller to decide if next service record should wrap 
 *              around to the beginning service record or to the end of the 
 *              service record.
 *
 *              Since the service record in the SVL is sorted by the order
 *              specified in the client provided compare function and search
 *              compare function, the iteration can be thought of as getting
 *              the next or the previous record in a customized ordered list.
 *
 *              Specifying 'SVL_FIRST_REC' returns the first record in 
 *              sorted list (regardless of the current service 
 *              record position), and similarly specifying 'SVL_LAST_REC' 
 *              returns the last record in the customized sorted list.
 *
 * Inputs: 
 *    h_svl     Handle to the SVL object.
 *    pv_data   Specifies the 'pv_data' of the current service record
 *              to begin for the search for the next service record.
 *
 *    ui4_nw_mask   32-bits mask specifying the network(s) of the service 
 *                  record to be returned.  
 *
 *    e_iter_dir    Specifies the direction of the iteration.  The values are:
 *
 *                        SVL_NEXT_REC	
 *                        SVL_PREV_REC
 *                        SVL_FIRST_REC
 *                        SVL_LAST_REC
 *
 *                  If SVL_FIRST_REC or SVL_LAST_REC is specified, then
 *                  the first record in the sorted list is returned.  If the 
 *                  iteration has reach an end point, i.e., the current record
 *                  is point to the first or last service record, then the 
 *                  current service record is returned.
 *
 * Outputs: 
 *    pt_svl_rec    Pointer to a SVL record structure.
 *
 * In/Output:
 *    pui4_ver_id   Pointer to a variable holding version id of the SVL
 *                  database object.  On entry, this variable contains the
 *                  version id of the database that the caller expects.  
 *                  On the return, this variable contains the current version 
 *                  id of the database.   If the expected version id and 
 *                  current database version id do not match, an error status 
 *                  code is returned.  If 'pui4_ver_id' is set to 
 *                  'SVL_NULL_VER_ID' value, then no checking is done and the 
 *                  latest version id is returned the application.
 * Returns: 
 *   SVLR_OK           
 *   
 ---------------------------------------------------------------------*/
INT32 c_svl_iterate_rec_by_custom_ord
(
    HANDLE_T			h_svl,
    VOID*               pv_data, 
    UINT32              ui4_nw_mask,
    SVL_ITERATE_T		e_iter_dir,  /* first, last, next or prev rec */
    SVL_REC_T*			pt_svl_rec,
    UINT32*             pui4_ver_id 
)
{
    return x_svl_iterate_rec_by_custom_ord(h_svl,
                                           pv_data,
                                           ui4_nw_mask,
                                           e_iter_dir,  
                                           pt_svl_rec,
                                           pui4_ver_id);
}

/*----------------------------------------------------------------------
 * Name: c_svl_init_mem_dump
 *
 * Description: This API initializes the memory dump structure, which 
 *              is used for c_svl_dump_to_mem.
 *
 * Inputs: 
 *   None
 *
 * Outputs: 
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Returns: 
 *   SVLR_OK            SVL database successfully dumped.
 *   SVLR_INV_ARG       'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_init_mem_dump
(
    SVL_MEM_DUMP_T*        pt_mem_dump
)
{
    return x_svl_init_mem_dump(pt_mem_dump);
}

/*----------------------------------------------------------------------
 * Name: c_svl_free_mem_dump
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
 *   SVLR_OK            SVL database successfully dumped.
 *   SVLR_INV_ARG       'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_free_mem_dump
(
    SVL_MEM_DUMP_T*        pt_mem_dump
)
{
    return x_svl_free_mem_dump(pt_mem_dump);
}

/*----------------------------------------------------------------------
 * Name: c_svl_dump_to_mem
 *
 * Description: This API dumps the specified SVL database into the specified
 *              structure, which could be later used for c_svl_load_from_mem.
 *              The memory dump structure should be initialized before dumping.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *
 * Outputs: 
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Returns: 
 *   SVLR_OK            SVL database successfully dumped.
 *   SVLR_INV_HANDLE    Bad handle.
 *   SVLR_INV_ARG       'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_dump_to_mem
(
    HANDLE_T               h_svl,
    SVL_MEM_DUMP_T*        pt_mem_dump
)
{
    return x_svl_dump_to_mem(h_svl, pt_mem_dump);
}

/*----------------------------------------------------------------------
 * Name: c_svl_load_from_mem
 *
 * Description: This API loads the SVL database from the specified memory
 *              dump structure, whose data comes from a previous call to
 *              c_svl_load_from_mem.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Outputs: 
 *
 * Returns: 
 *   SVLR_OK            SVL database successfully dumped.
 *   SVLR_INV_HANDLE    Bad handle.
 *   SVLR_INV_ARG       'pt_mem_dump' is NULL or SVL metadata not match.
 *   
 -----------------------------------------------------------------------*/
INT32 c_svl_load_from_mem
(
    HANDLE_T               h_svl,
    SVL_MEM_DUMP_T*        pt_mem_dump
)
{
    return x_svl_load_from_mem(h_svl, pt_mem_dump);
}
    
    
