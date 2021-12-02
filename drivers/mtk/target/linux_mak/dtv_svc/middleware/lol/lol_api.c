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
 * $RCSfile: lol_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/18 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: eeeee16d218b478d12d6bd39898cde8d $
 *
 * Description:
 *         This file contain implementation of Large Object List API's,
 *         which are exported to other Middleware components.
 *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/


#include "os/inc/x_os.h"
#include "os/inc/os.h"
#include "cdb/cdb_api.h"

#include "lol/_lol.h"
#include "lol/lol_rec.h"
#include "lol/lol_rec_id.h"
#include "lol/lol_fs.h"

/*-----------------------------------------------------------------------------
 * Name: x_lol_create
 *
 * Description: 
 *
 * Inputs: 
 *   ps_lol_name   Name of the LOL object. 
 *   ui2_lol_id    Unique id for the LOL object.  
 *                 The limit for LOL ID is between 1 and 65535.
 *   e_mem_policy  Memory policy: LOL_MEM_STATIC or LOL_MEM_DYNAMIC
 *   ui2_max_nb_rec Specifies the maximum number of LOL record (for
 *                 LOL_MEM_STATIC), or it specifies number of
 *                 LOL records per memory chunk.
 *   pv_tag        Pointer to a private tag value provided by the 
 *                 caller.  This tag value is passed to the 
 *                 caller's notification function.
 *   pf_nfy        Notification function provided by the caller.  
 *                 This function is called to notify the owner of 
 *                 this LOL handle when the LOL object has been 
 *                 modified.  If NULL, then no notification function 
 *                 will be called.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           LOL object is created successfully.
 *   LOLR_INV_ARG      'ui2_lol_id' is 0, or ph_lol is NULL, or ps_lol_name
 *                     is NULL.
 *   LOLR_OUT_OF_HANDLE Cannot get a handle.
 *   LOLR_FAIL         The specified 'ps_lol_name' or 'ui2_lol_id' value
 *                     is not unique (e.g., it is currently been used), or
 *                     limit of the number of LOL objects that can be 
 *                     created has been reached.
 *   LOLR_OUT_OF_MEM   Not enough memory to create LOL object.
 * 
 ----------------------------------------------------------------------------*/
INT32 x_lol_create(const CHAR*      ps_lol_name,
                   UINT16           ui2_lol_id,
                   LOL_MEM_POLICY_T e_mem_policy,
                   UINT16           ui2_max_nb_rec,
                   VOID*            pv_tag,
                   x_lol_nfy_fct    pf_nfy,
                   HANDLE_T*        ph_lol)
{
    INT32  i4_rc=LOLR_FAIL;
    CHAR   s_lol_name[MAX_LOL_NAME_LEN];
    
    if ( ui2_lol_id != 0            &&
         ps_lol_name != NULL        &&
         x_strlen(ps_lol_name) > 0  &&
         ph_lol != NULL )
    {
        x_strncpy(s_lol_name,ps_lol_name,MAX_LOL_NAME_LEN-1);
        s_lol_name[MAX_LOL_NAME_LEN-1]='\0';
            
        lol_sema_lock();
        /* check if the input LOL name and id has not been used. */
        /* Add the name and id to the internal list. */
        if ( lol_check_id_name(ui2_lol_id,s_lol_name) == TRUE &&
             lol_add_id_name(ui2_lol_id,s_lol_name) == LOLR_OK
            )
        {
            *ph_lol=NULL_HANDLE;
            i4_rc=lol_create_object(s_lol_name,ui2_lol_id,
                                    e_mem_policy,ui2_max_nb_rec,
                                    pv_tag,pf_nfy,ph_lol);
            if ( i4_rc != LOLR_OK )
            {
                /* Remove the id name pair. */
                lol_del_id(ui2_lol_id);
            }
        }
        lol_sema_unlock();
    }
    else
    {
        i4_rc=LOLR_INV_ARG;
    }

    if ( i4_rc == LOLR_OK )
    {
        DBG_API(("{LOL} [name: %s lol_id: %d handle: %d] created\n",ps_lol_name, ui2_lol_id, *ph_lol));
    }
    else
    {
        DBG_ERROR(("{LOL} [name: %s lol_id: %d] failed.\n", ps_lol_name, ui2_lol_id));
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_delete
 *
 * Description: This API deletes the specified LOL object associated 
 *              with 'h_lol' handle.    Other clients with a handle to 
 *              the deleted LOL object will receive a notification of 
 *              the delete event.   
 *
 * Inputs: 
 *   h_lol	Handle to the LOL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           Specified LOL object is deleted.
 *   LOLR_INV_HANDLE   Bad handle.
 -----------------------------------------------------------------------*/
INT32 x_lol_delete(HANDLE_T  h_lol)
{
    INT32           i4_rc;
    LOL_OBJ_T*  	pt_lol;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        lol_sema_lock();
        /* remove lol id/name pairs. */
        lol_del_id(pt_lol->ui2_lol_id);
        lol_sema_unlock();
        
        i4_rc=lol_delete_object(pt_lol);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_clean
 *
 * Description: This API removes all the large object list records from the 
 *              specified LOL object pointed to by the 'h_lol' handle.    
 *              All other handles to this LOL object will receive a 
 *notification.   
 *
 * Inputs: 
 *   h_lol      Handle to the LOL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           All records are successfully removed. 
 *   LOLR_INV_HANDLE   Bad handle.
 -----------------------------------------------------------------------*/
INT32 x_lol_clean(HANDLE_T         h_lol)
{
    INT32     i4_rc;
    LOL_OBJ_T*  	pt_lol;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        i4_rc=lol_clean_object(pt_lol);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_add_rec
 *
 * Description: This function adds a new service record to the LOL 
 *              database object.  The LOL library will make copy of 
 *              the record content from the address location pointed to 
 *              by the 'pt_lol_rec'.  The 'ui2_lol_rec_id' field in 
 *              the 'pt_lol_rec' structure specifies the key field of the 
 *              service record to be added to the database.  
 *
 * Inputs: 
 *   h_lol      Handle to the LOL object.
 *   pt_lol_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           Record is successfully added. 
 *   LOLR_INV_HANDLE   Bad LOL handle.
 *   LOLR_OUT_OF_MEM   Can not allocate memory, add record failed,.
 *   LOLR_INV_ARG      'pt_lol_rec' is null.  
 -----------------------------------------------------------------------*/
INT32 x_lol_add_rec(HANDLE_T         h_lol,
                    const LOL_REC_T* pt_lol_rec)
{
    INT32           i4_rc;
    LOL_OBJ_T*  	pt_lol;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        if ( pt_lol_rec != NULL                             &&
             pt_lol_rec->ui2_lol_rec_id != LOL_NULL_REC_ID
           )
        {
            i4_rc=lol_write_rec_to_object(pt_lol,
                                          pt_lol_rec,
                                          LOL_APPEND_RECORD);
        }
        else
        {
            i4_rc=LOLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}
/*----------------------------------------------------------------------
 * Name: x_lol_del_rec
 *
 * Description: This function deletes a specified service record from 
 *              the LOL.  The record to be deleted is specified by the 
 *              transport stream record id.
 *
 * Inputs: 
 *   h_lol           Handle to the LOL object.
 *   ui2_lol_rec_id  Specifies the transport stream record to be deleted. 
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK            Specified record is deleted. 
 *   LOLR_INV_HANDLE    Bad LOL handle.
 *   LOLR_REC_NOT_FOUND The specified record to be deleted is not in 
 *                      the LOL object.
 *   LOLR_INV_ARG       'ui2_lol_rec_id' is not a valid record id.
 -----------------------------------------------------------------------*/
INT32 x_lol_del_rec(HANDLE_T         h_lol,
                    UINT16           ui2_lol_rec_id)
{
    INT32           i4_rc;
    LOL_OBJ_T*  	pt_lol;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        if ( ui2_lol_rec_id != LOL_NULL_REC_ID )
        {
            i4_rc=lol_del_rec_from_object(pt_lol, ui2_lol_rec_id);
        }
        else
        {
            i4_rc = LOLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_update_rec
 *
 * Description: This function updates large object list record to the LOL
 *              object. If the key field value (e.g., 'ui2_lol_rec_id' field
 *              in the 'pt_lol_rec' structure) of the input service record 
 *              matches an existing record in the LOL object, then the 
 *              existing record is over-wrote with the new information,  
 *              otherwise a new record is created in the LOL database. 
 * 
 *              If the 'b_must_exist' flag is TRUE, then there must be an 
 *              existing record with the same key field value, else a status 
 *              of 'LOLR_REC_NOT_FOUND' is returned to the caller.  
 *              If 'b_must_exist' flag is FALSE, then a new service record 
 *              is created if no existing record matching the key field value 
 *              is found in the LOL database.
 *
 *              Call to this function must be protected, e.g., the LOL 
 *              database object needs to be locked before the call to this 
 *              function and unlock after returning from call to this function.
 *
 * Inputs: 
 *   h_lol      Handle to the LOL object.
 *   pt_lol_rec Pointer to service record structure. 
 *   b_must_exist Boolean flag specifying if the service record to be 
 *                updated should exist in the LOL database
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           Record is successfully added/updated.
 *   LOLR_INV_HANDLE   Bad LOL handle. 
 *   LOLR_OUT_OF_MEM   Not enough memory to create a new LOL record.
 *   LOLR_REC_NOT_FOUND The specified record is not found 
 *                      ('b_must_exist' == TRUE)
 *   LOLR_INV_ARG      'pt_lol_rec' is NULL or the pt_lol_rec
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_update_rec(HANDLE_T         h_lol,
                       const LOL_REC_T* pt_lol_rec,
                       BOOL             b_must_exist)
{
    INT32           i4_rc;
    LOL_OBJ_T*  	pt_lol;
    HANDLE_TYPE_T   pe_type;
    UINT8           ui1_write_mode;

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        if ( pt_lol_rec != NULL                              &&
             pt_lol_rec->ui2_lol_rec_id != LOL_NULL_REC_ID
           )
        {
            if ( b_must_exist == TRUE )
            {
                ui1_write_mode=LOL_MODIFY_RECORD;
            }
            else
            {
                ui1_write_mode=LOL_UPDATE_RECORD;
            }
            
            i4_rc=lol_write_rec_to_object(pt_lol,
                                          pt_lol_rec,
                                          ui1_write_mode);            
        }
        else
        {
            i4_rc=LOLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_qry_rec_id
 *
 * Description: This API queries the range (FIRST, LAST, or NEXT) of 
 *              valid transport stream record id values used in the LOL
 *              object.  
 *   
 * Inputs: 
 *   h_lol             Handle to the LOL object.
 *
 *   e_lol_qry_option  Flag to specifies the record id value to be returned.  
 *                     The values are:
 *
 *                     LOL_FIRST_REC_ID:  the smallest valid record id in 
 *                                        this LOL database.
 *                     LOL_LAST_REC_ID:   the largest valid record id in 
 *                                        this LOL database.
 *                     LOL_NEXT_REC_ID:   return next available record id that
 *                                        can be uesed to specify a new LOL
 *                                        record entry.
 *
 * Outputs: None
 *   pui2_rec_id       Contain the transport stream record id value.	
 *
 * Returns: 
 *   LOLR_OK           LOL record id return.
 *   LOLR_INV_HANDLE   Bad LOL handle. 
 *   LOLR_INV_ARG      'pui2_rec_id' is NULL.
 *
 -----------------------------------------------------------------------*/
INT32 x_lol_qry_rec_id(HANDLE_T         h_lol,
                       LOL_REC_ID_QRY_T e_lol_qry_option,
                       UINT16*          pui2_rec_id)
{
    INT32               i4_rc;
    LOL_OBJ_T*  	    pt_lol;
    HANDLE_TYPE_T       pe_type;

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        if ( pui2_rec_id != NULL )
        {
            switch (e_lol_qry_option)
            {
                case LOL_FIRST_REC_ID:
                {
                    if ( lol_get_smallest_rec_id(pt_lol, pui2_rec_id) == TRUE )
                    {
                        i4_rc=LOLR_OK;
                    }
                    else
                    {
                        i4_rc=LOLR_REC_NOT_FOUND;
                    }
                }
                break;

                case LOL_LAST_REC_ID:
                {
                    if ( lol_get_largest_rec_id(pt_lol, pui2_rec_id) == TRUE )
                    {
                        i4_rc=LOLR_OK;
                    }
                    else
                    {
                        i4_rc=LOLR_REC_NOT_FOUND;
                    }
                }
                break;

                case LOL_NEXT_REC_ID:
                {
                    lol_get_next_rec_id(pt_lol, pui2_rec_id);
                    i4_rc=LOLR_OK;
                }
                break;

                default:
                {
                    *pui2_rec_id=LOL_NULL_REC_ID;
                    i4_rc=LOLR_INV_ARG;
                }
            }
        }
        else
        {
            i4_rc=LOLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}

    
/*----------------------------------------------------------------------
 * Name: x_lol_lock
 *
 * Description: This function locks the LOL object.   The function blocks 
 *              until the lock is acquired.
 *
 * Inputs: 
 *   h_lol	Handle to the LOL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           LOL is locked. 
 *   LOLR_INV_HANDLE   Bad LOL handle.
 *
 -----------------------------------------------------------------------*/
INT32 x_lol_lock(HANDLE_T h_lol)
{
    INT32           i4_rc;
    LOL_OBJ_T*  	pt_lol;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        i4_rc=lol_lock_object(pt_lol);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_unlock
 *
 * Description: This function unlocks the LOL database. 
 *
 * Inputs: 
 *   h_lol	Handle to the LOL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK          LOL database is unlocked. 
 *   LOLR_INV_HANDLE  Bad LOL handle.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_unlock(HANDLE_T  h_lol)
{
    INT32           i4_rc;
    LOL_OBJ_T*  	pt_lol;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        i4_rc=lol_unlock_object(pt_lol);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_read_lock
 *
 * Description: This function acquires read-lock on the specified LOL object.
 *    The function blocks until the read-lock is acquired.
 *
 * Inputs: 
 *   h_lol	Handle to the LOL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           LOL is locked. 
 *   LOLR_INV_HANDLE   Bad LOL handle.
 *
 -----------------------------------------------------------------------*/
INT32 x_lol_read_lock(HANDLE_T  h_lol)
{
    INT32           i4_rc;
    LOL_OBJ_T*  	pt_lol;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        i4_rc=lol_lock_object_for_read(pt_lol);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_read_unlock
 *
 * Description: This function releases the read-lock on the specified
 *      LOL database. 
 *
 * Inputs: 
 *   h_lol	Handle to the LOL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK          LOL database is unlocked. 
 *   LOLR_INV_HANDLE  Bad LOL handle.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_read_unlock(HANDLE_T  h_lol)
{
    INT32           i4_rc;
    LOL_OBJ_T*  	pt_lol;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        i4_rc=lol_unlock_object_for_read(pt_lol);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_open
 *
 * Description: This function gets a handle to the specified LOL database 
 *              object by its ID value.    
 *
 * Inputs: 
 *   ui2_lol_id:  Specifies the LOL object identifier value.  
 *                The range for LOL ID is between 1 and 65535.
 *   pv_tag:      A private tag value associated with the calling client.
 *   pf_nfy:      Pointer to the caller provided notification function.  
 *                The notification function will be called in the context of 
 *                the thread that modifies (update, add, delete) the LOL 
 *                database.  If NULL, then no notification function will be 
 *                called.
 *
 * Outputs:
 *   ph_lol:      Pointer to the handle created.
 *
 * Returns: 
 *   LOLR_OK               LOL handle is returned. 
 *   LOLR_OUT_OF_HANDLE    No more handle available.
 *   LOLR_OUT_OF_MEM       Not enough memory to create the LOL control 
 *                         structure.
 *   LOLR_NOT_FOUND        The specified LOL database object id (e.g. 
 *                         'ui2_lol_id') is not found.
 *   LOLR_INV_ARG          'ph_lol' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_open(UINT16          ui2_lol_id,
                 VOID*           pv_tag,
                 x_lol_nfy_fct   pf_nfy,
                 HANDLE_T*       ph_lol)
{
    INT32           i4_rc;
    CHAR            ps_lol_name[MAX_LOL_NAME_LEN];

    if ( ph_lol  != NULL )
    {
        *ph_lol=NULL_HANDLE;
        lol_sema_lock();
        /* Find the id/name */
        if ( lol_get_name_by_id(ui2_lol_id, ps_lol_name) == TRUE )
        {
            i4_rc=lol_open_object(ui2_lol_id,
                                  ps_lol_name,
                                  pv_tag,
                                  pf_nfy,
                                  ph_lol);
        }
        else
        {
            i4_rc=LOLR_NOT_FOUND;
        }
        lol_sema_unlock();
    }
    else
    {
        i4_rc = LOLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_close
 *
 * Description: This function frees the specified handle to the LOL 
 *              database object.    
 *
 * Inputs: 
 *   h_lol	Handle to LOL database object.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           LOL handle is freed. 
 *   LOLR_INV_HANDLE   Bad handle.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_close             (HANDLE_T        h_lol)
{
    INT32           i4_rc;
    LOL_OBJ_T*  	pt_lol;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        handle_free(h_lol,TRUE);
        i4_rc=LOLR_OK;
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_get_rec
 *
 * Description: This API gets a specific record stored in the LOL database 
 *              object.    The search parameter is specified by the 
 *              'ui2_lol_rec_id' value.
 *
 * Inputs: 
 *   h_lol           Handle to the LOL object.
 *   ui2_lol_rec_id  Specifies the service record id.
 *
 * Outputs: 
 *   pt_lol_rec  Pointer to a LOL record structure.   This structure must 
 *               be allocated by the caller.  On return, it is filled with 
 *               LOL record information.
 *   pui4_ver_id Pointer to a variable holding version id of the database 
 *               object.   Note: version id of the LOL database object is 
 *               incremented each time when a modification takes place.
 * Returns: 
 *   LOLR_OK    Data from the specified record is returned. 
 *   LOLR_INV_HANDLE    Bad handle.
 *   LOLR_REC_NOT_FOUND No record matching the search parameter 
 *                      ('ui2_lol_rec_id')  is found.
 *   LOLR_INV_ARG       'pui4_ver_id' or 'pt_lol_rec' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_get_rec(HANDLE_T        h_lol,
                    UINT16          ui2_lol_rec_id,
                    LOL_REC_T*      pt_lol_rec,
                    UINT32*         pui4_ver_id)
{
    INT32     i4_rc;
    LOL_OBJ_T*  	pt_lol;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        if ( pui4_ver_id    != NULL &&
             pt_lol_rec     != NULL &&
             ui2_lol_rec_id != LOL_NULL_REC_ID   )
        {
            i4_rc=lol_get_rec_from_obj(pt_lol,
                                       ui2_lol_rec_id,
                                       pt_lol_rec,
                                       pui4_ver_id);
        }
        else
        {
            i4_rc=LOLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_get_rec_by_data_tag
 *
 * Description: This API returns large object list record entry with a
 *              specified data tag value from the LOL database object.
 *              If there are multiple records matching the specified data
 *              tag, the caller can use 'ui2_idx' to iterate over LOL
 *              records with the same data tag.    
 *
 * Inputs: 
 *   h_lol           Handle to the LOL object.
 *   ui4_data_tag  Specifies the data tag of the LOL record
 *                   to be returned.
 *   
 *   ui2_idx      Index for iterating the LOL records matching 
 *                the specified data tag number.  0 means the first record
 *                found in LOL list that matches the specified data tag
 *                number, and 1 is the next record
 *                found, and so on.
 *
 * Outputs: None
 *   pt_lol_rec	  Pointer to a LOL record structure.
 *
 * Input/Output arguments
 *   pui4_ver_id  Pointer to a variable holding version id of the LOL 
 *                database object.  On entry, this variable contains 
 *                the version id of the database that the caller expects.  
 *                On the return, this variable contains the current version 
 *                id of the database.   If the expected version id and 
 *                current database version id do not match, an error status 
 *                code is returned.  If 'pui4_ver_id' is set to 
 *                'LOL_NULL_VER_ID' value, then no checking is done and 
 *                the latest version id is returned the application. 
 * Returns: 
 *   LOLR_OK             A LOL_REC_T structure is returned. 
 *   LOLR_INV_HANDLE     Bad 'h_lol' handle.
 *   LOLR_REC_NOT_FOUND  Specified record is not found.
 *   LOLR_INVALID_ARG    'pui4_ver_id' or 'pt_lol_rec' is NULL.
 *   LOLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id' 
 *                       pass into the API and the internal database version 
 *                       id is not the same.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_get_rec_by_data_tag  (HANDLE_T      h_lol,
                                  UINT32        ui4_data_tag,
                                  UINT16		 ui2_idx,
                                  LOL_REC_T*    pt_lol_rec,
                                  UINT32*       pui4_ver_id)
{
    INT32               i4_rc;
    LOL_OBJ_T*  	    pt_lol;
    HANDLE_TYPE_T       pe_type;
    
    UINT8               ui1_key_count;
    DB_FIELD_T          aui4_keyfld_type[1];
    const VOID*         apv_keyfld_addr[1];
    
    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_lol_rec     != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            ui1_key_count = 1;
            aui4_keyfld_type[0]       = (DB_FIELD_T) LOL_CDB_TYPE_DATA_TAG;
            apv_keyfld_addr[0]        = &ui4_data_tag;
            
            i4_rc=lol_get_rec_by_flds(pt_lol,
                                      ui1_key_count,
                                      aui4_keyfld_type,
                                      apv_keyfld_addr,
                                      NULL,
                                      pt_lol_rec,
                                      ui2_idx,
                                      pui4_ver_id);            
        }
        else
        {
            i4_rc=LOLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_get_num_rec
 *
 * Description:    This function returns the number of records for the
 *                 specified LOL. 
 *
 * Inputs: 
 *   h_lol	Handle to the LOL object.
 *
 * Outputs: 
 *   pui2_num_rec	Pointer to a variable for the number of records in
 *                  the LOL. 	
 *   pui4_ver_id    Pointer to a variable holding version id of the LOL
 *                  database object. 	
 * Returns: 
 *   LOLR_OK			Number of records in the specified LOL is returned. 
 *   LOLR_INV_HANDLE    Bad 'h_lol' handle.
 *   LOLR_INV_ARG       'pui2_num_rec' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_get_num_rec(HANDLE_T            h_lol,
                        UINT16*             pui2_num_rec,
                        UINT32*             pui4_ver_id)
{
    INT32           i4_rc;
    LOL_OBJ_T*  	pt_lol;
    HANDLE_TYPE_T   pe_type;
    
    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        if (  pui4_ver_id  != NULL   &&
              pui2_num_rec != NULL
           )
        {
            i4_rc=lol_get_num_rec(pt_lol,pui2_num_rec,pui4_ver_id);
        }
        else
        {
            i4_rc=LOLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }    
    return i4_rc;
}

    
/*----------------------------------------------------------------------
 * Name: x_lol_get_num_rec_by_data_tag
 *
 * Description: This function returns the number of records for the
 *              specified data tag.           
 *
 * Inputs: 
 *   h_lol	        Handle to the LOL object.
 *
 *   ui4_data_tag Specifies the data tag value.   
 *
 * Outputs: 
 *   pui2_num_recs	Pointer to a variable for the number of records
 *                  matching the specified stream type in the LOL. 	
 *   pui4_ver_id    Pointer to a variable holding version id of the LOL
 *                  database object. 	
 * Returns: 
 *   LOLR_OK         Number of records found.
 *   LOLR_INV_HANDLE Bad 'h_lol' handle.
 *   LOLR_INV_ARG    'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_get_num_rec_by_data_tag(HANDLE_T            h_lol,
                                    UINT32              ui4_data_tag,
                                    UINT16*             pui2_num_recs,
                                    UINT32*             pui4_ver_id)
{
    INT32           i4_rc;
    UINT8           ui1_key_count;
    LOL_OBJ_T*  	pt_lol;
    HANDLE_TYPE_T   pe_type;
    DB_FIELD_T      aui4_keyfld_type[1];
    const VOID*     apv_keyfld_addr[1];
    /*
    DB_SEARCH_T     ae_cmp_param[1];
    */
 
    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pui2_num_recs  != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            ui1_key_count = 1;

            aui4_keyfld_type[0]      = (DB_FIELD_T) LOL_CDB_TYPE_DATA_TAG ;
            apv_keyfld_addr[0]       = &ui4_data_tag;

            /*
            ae_cmp_param[0].e_opcode = DB_OPS_EQUAL;
            */
            
            i4_rc=lol_get_num_rec_by_flds(pt_lol,
                                          ui1_key_count,
                                          aui4_keyfld_type,
                                          apv_keyfld_addr,
                                          NULL, /* ae_cmp_param, */
                                          pui2_num_recs,
                                          pui4_ver_id);
        }
        else
        {
            i4_rc=LOLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: x_lol_rec_init
 *
 * Description: This API pre-sets the LOL_REC_T structure to a 
 *     pre-defined invalid value (e.g., 0xFF).   The pre-defined invalid 
 *     value can be used to detect changes in the LOL_REC_T structure for 
 *     backward compatibility in the future.
 *
 *     For example, if a new field is added to the LOL record structure.  
 *     At the runtime, if the new field in the LOL_REC_T structure passed 
 *     into LOL library contain 0xFF, then we know the caller has not been 
 *     re-compile with the new LOL module.   
 *
 * Inputs: 
 *   Pointer to a LOL record structure. 
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           LOL record is initialized successfully.
 *   LOLR_INV_ARG      'pt_lol_rec' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 x_lol_rec_init(LOL_REC_T*        pt_lol_rec)
{
    INT32     i4_rc;

    if ( pt_lol_rec  != NULL  )
    {
        x_memset(pt_lol_rec,0,sizeof(LOL_REC_T));
        i4_rc = LOLR_OK;  
    }
    else
    {
        i4_rc = LOLR_INV_ARG;
    }
    return i4_rc;
}

/********************************************
   LOL data persistent storage related APIs
*********************************************/

/*----------------------------------------------------------------------
 * Name: x_lol_fs_load
 *
 * Description: This API loads a specified LOL object from file system
 *              into memory, and returns a handle to the newly created
 *              LOL object.
 * Inputs: 
 *   h_dir	      Handle to the parent directory of the file specified by
 *                the path 'ps_lol_path'.  If 'ps_lol_path' is an absolute
 *                path, then this argument should be set to 'FM_ROOT_HANDLE'.
 *   ps_lol_path  Specifies the path to the file where the LOL objects
 *                are stored in the file system.
 *   ui2_lol_id   Specifies the LOL id to load from the file system.
 *                The limit for LOL ID is between 1 and 65535.
 *   ps_lol_name  Specifies the name for the LOL object.  If null string
 *                is specified, then the default name stored in the file
 *                system is used to create the LOL object.
 *   pv_tag       Pointer to a private tag value provided by the caller.
 *                This tag value is passed to the caller's notification
 *                function.
 *   pf_nfy       Notification function provided by the caller.  This
 *                function is called to notify the owner of this LOL
 *                handle when the LOL object has been modified by another
 *                client.  If NULL, then no notification function will be
 *                called.
 * Outputs:
 *   ph_lol	      On return, contain the handle to the LOL object created
 *                from file system.
 * Returns: 
 *   LOLR_OK            LOL object is created. 
 *   LOLR_NOT_FOUND     'ui2_lol_id' is not found or 'ps_lol_path' does
 *                      not exist.
 *   LOLR_OUT_OF_HANDLE Cannot get a handle.
 *   LOLR_OUT_OF_MEM    Not enough memory to create LOL object.
 *   LOLR_FAIL          The specified 'ps_lol_name' or 'ui2_lol_id' value
 *                      is not unique.
 *   LOLR_INV_ARG       'ph_lol' or 'ps_lol_path' is NULL.
 -----------------------------------------------------------------------*/
INT32 x_lol_fs_load           (HANDLE_T            h_dir,
                               const CHAR*         ps_lol_path,
                               UINT16              ui2_lol_id,
                               const CHAR*	       ps_lol_name,
                               VOID*               pv_tag,
                               x_lol_nfy_fct       pf_nfy,
                               HANDLE_T*           ph_lol)
{
    INT32           i4_rc;
    CHAR            s_lol_name[MAX_LOL_NAME_LEN];
    
    if ( ps_lol_path != NULL     &&
         ph_lol      != NULL     &&
         ui2_lol_id  != 0   
        )
    {
        *ph_lol=NULL_HANDLE;

        x_memset(s_lol_name,0,MAX_LOL_NAME_LEN);

        if ( ps_lol_name != NULL )
        {
            x_strncpy(s_lol_name,ps_lol_name,MAX_LOL_NAME_LEN-1);
            s_lol_name[MAX_LOL_NAME_LEN-1]='\0';
            ps_lol_name=s_lol_name;
        }

        DBG_API(("{LOL} loading LOL data from: %s / %s \n", ps_lol_path, ps_lol_name));
    
        i4_rc=lol_fs_load_object(
            h_dir,
            ps_lol_path,
            ui2_lol_id,
            ps_lol_name,
            pv_tag,
            pf_nfy,
            ph_lol);
        
        DBG_API(("{LOL} done loading LOL data [ret code: %d]\n",i4_rc));
    }
    else
    {
        i4_rc = LOLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_fs_store
 *
 * Description: This API stores the specified LOL object to the file system.
 *
 * Inputs: 
 *   h_dir	      Handle to the parent directory of the file specified by
 *                the path 'ps_lol_path'.  If 'ps_lol_path' is an absolute
 *                path, then this argument should be set to 'FM_ROOT_HANDLE'.
 *   ps_lol_path  Specifies the file path to the file where the LOL objects
 *                are stored in the file system.
 *   ui2_lol_id   Specifies the  ID of the LOL object to be stored to the
 *                file system.  The limit for LOL ID is between 1 and 65535.
 *
 * Outputs: None 
 *
 * Returns: 
 *   LOLR_OK			LOL object is stored to the file system. 
 *   LOLR_NOT_FOUND		The LOL object specified by the 'ui2_lol_id' does
 *                      not exist.
 *   LOLR_INV_ARG		'ps_lol_path' is NULL.
 -----------------------------------------------------------------------*/
INT32 x_lol_fs_store(HANDLE_T            h_dir,
                     const CHAR*         ps_lol_path,
                     UINT16              ui2_lol_id)
{
    INT32     i4_rc;
    
    if ( ps_lol_path != NULL &&
         ui2_lol_id  != 0
        )
    {
        DBG_API(("{LOL} saving LOL data.\n"));
        
        i4_rc=lol_fs_store_object(h_dir,ps_lol_path,ui2_lol_id);      

        DBG_API(("{LOL} done saving LOL data [ret code: %d]\n",i4_rc));
    }
    else
    {
        i4_rc = LOLR_INV_ARG;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_fs_del
 *
 * Description: This API deletes a specified LOL object on the file system.
 *
 * Inputs: 
 *   h_dir	      Handle to the parent directory of the file specified by the
 *                path 'ps_lol_path'.  If 'ps_lol_path' is an absolute path,
 *                then this argument should be set to 'FM_ROOT_HANDLE'.
 *   ps_lol_path  Specifies the file path to the file where the LOL objects
 *                are stored in the file system.
 *   ui2_lol_id   Specifies the ID of the LOL object to be deleted from the
 *                file system.  The limit for LOL ID is between 1 and 65535.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK		  LOL object is deleted from the file system. 
 *   LOLR_NOT_FOUND	  'ui2_lol_id' is not found or 'ps_lol_path' does not
 *                    exist.
 *   LOLR_INV_ARG     'ps_lol_path' is NULL.
 -----------------------------------------------------------------------*/
INT32 x_lol_fs_del(HANDLE_T            h_dir,
                   const CHAR*         ps_lol_path,
                   UINT16              ui2_lol_id)
{
    INT32     i4_rc;

    if ( ps_lol_path != NULL &&
         ui2_lol_id  != 0
        )
    {
      
        i4_rc=lol_fs_del_object(h_dir,ps_lol_path, ui2_lol_id);
        
    }
    else
    {
        i4_rc = LOLR_INV_ARG;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_fs_qry
 *
 * Description: This API queries the LOL objects stored in file system.
 *              It return the LOL object name and id value.
 * Inputs:
 *   h_dir         The handle to the directory.  The 'ps_lol_path'
 *                 will be relative to the specified 'h_dir'.
 *
 *   ps_lol_path   Specifies the directory path where LOL objects 
 *                 are stored.  
 *
 *   ui2_idx       Index value to get the next LOL object.  
 *                 0 means the first LOL, 1 is the second LOL, etc.
 *
 * Outputs: 
 *   pui2_lol_id   Pointer to a UINT16 variable. On return, it contains 
 *                 LOL ID.
 *
 *   ps_lol_name   Pointer to a string buffer (minimum 17 bytes) .  
 *                 On return, it contains the default LOL name stored on 
 *                 the file system. 
 *
 * Returns: 
 *   LOLR_OK              LOL object found. 
 *
 *   LOLR_NOT_FOUND       No LOL object found.
 *
 *   LOLR_INV_ARG         'ps_lol_name' or 'pui2_lol_id' is NULL.
 *
 *   LOLR_INV_FILE_PATH   Specified file path is not a directory or a 
 *                        RAW character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_fs_qry(HANDLE_T        h_dir,
                   const CHAR*     ps_lol_path,
                   UINT16          ui2_idx,
                   UINT16*         pui2_lol_id,
                   CHAR*           ps_lol_name)
{
    INT32           i4_rc;
    
    if ( ps_lol_path != NULL     &&
         pui2_lol_id != NULL     &&
         ps_lol_name != NULL
        )
    {
        *pui2_lol_id = LOL_NULL_ID;
        ps_lol_name[0] = '\0';
      
        i4_rc=lol_qry_persistent_objects(
            h_dir,
            ps_lol_path,
            ui2_idx,
            pui2_lol_id,
            ps_lol_name);
        
    }
    else
    {
        i4_rc = LOLR_INV_ARG;
    }
    return i4_rc;
}


/*--------------------------------------------------------------------
 * Name: x_lol_fs_sync
 *
 * Description: This API sync a specified LOL handle with LOL records
 *              from persistent file system.  The existing LOL records
 *              are replaced with new records read from file. 
 *
 * Inputs:
 *
 *   h_lol          Specify the handle to the LOL object.
 *
 *   h_dir          The handle to the directory.  The 'ps_lol_path'
 *                  will be relative to the specified 'h_dir'.
 *
 *   ps_lol_path    Specifies the directory path of the LOL object in the 
 *                  file system. See File Path Specification, 10.1, for the 
 *                  types of file path that can be specified.
 *
 *   ui2_lol_id     Specifies the LOL id to load from the file system.  
 *                  The limit for LOL ID is between 1 and 65535.
 *
 * Outputs: 
 *
 *   None.
 *                 
 * Returns: 
 *   LOLR_OK             LOL records are load from the persistent file
 *                       into the memory of LOL object.
 *
 *   LOLR_NOT_FOUND      'ui2_lol_id' is not found or 'ps_lol_path' 
 *                       does not exist.
 *
 *   LOLR_FAIL           internal error
 *
 *   LOLR_INV_ARG        'ps_lol_path' is NULL, or 'h_lol' is NULL_HANDLE,
 *                       or ui2_lol_id is 0.
 *
 *   LOLR_INV_FILE_PATH  Specified file path is not a directory or a RAW 
 *                       character file.
 *   
 ---------------------------------------------------------------------*/
INT32 x_lol_fs_sync(HANDLE_T        h_lol,
                    HANDLE_T        h_dir,
                    const CHAR*     ps_lol_path,
                    UINT16          ui2_lol_id)
{
    INT32           i4_rc;
    
    if ( ps_lol_path != NULL            &&
         h_lol       != NULL_HANDLE     &&
         ui2_lol_id  != 0   
        )
    {
      
        i4_rc=lol_fs_sync_object(h_lol,
                                 h_dir,
                                 ps_lol_path,
                                 ui2_lol_id);
       
    }
    else
    {
        i4_rc = LOLR_INV_ARG;
    }
    return i4_rc;
}



/*----------------------------------------------------------------------
 * Name: x_lol_get_info
 *
 * Description: 
 *   Get info (number of records, size of record, etc) for this LOL handle.
 *
 *
 * Inputs: 
 *   h_lol        Handle to the LOL object.
 *
 * Outputs:
 *   None.
 *
 * Returns: 
 *   LOLR_OK         Version id is returned. 
 *   LOLR_INV_HANDLE Bad 'h_lol' handle.
 *   LOLR_INV_ARG    'pf_sort_cmp' or 'pf_search_cmp' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 x_lol_get_info
(
    HANDLE_T               h_lol
)
{
    INT32               i4_rc;
    LOL_OBJ_T*  	    pt_lol;
    HANDLE_TYPE_T       pe_type;
    DB_INFO_T           t_db_info;
    UINT32              ui4_persistent_size;
    UINT32              ui4_ver_id;
    
    if ( (handle_get_type_obj(h_lol, &pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                      &&
         (pt_lol  != NULL)
        )
    {
        x_dbg_stmt("<LOL ID:  %d  LOL_NAME: %s>\n",
                   pt_lol->ui2_lol_id, pt_lol->ps_lol_name);
        
        if ( db_get_info(pt_lol->h_cdb, &t_db_info) == DBR_OK )
        {
            x_dbg_stmt("number of LOL records:   %d \n",
                       t_db_info.ui4_record_count);
            
            x_dbg_stmt("size of each record:     %d (bytes)\n",
                       t_db_info.ui2_record_width);

            x_dbg_stmt("Heap memory usage for string buffer:  allocated: %d (bytes) used: %d (bytes)\n",
                       t_db_info.ui4_str_buf_allocated,
                       t_db_info.ui4_str_buf_used);

            x_dbg_stmt("Heap memory usage for string obj array: allocated: %d (bytes) used: %d (bytes)\n",
                       t_db_info.ui2_nb_str_obj_allocated*sizeof(UINT16),
                       t_db_info.ui2_nb_str_obj_used*sizeof(UINT16));
            
            x_dbg_stmt("LOL object filessize [header+metadata+data]:  %d (bytes)\n",
                       lol_persistent_size(pt_lol));

            /* Size of CDB object. */
            i4_rc=db_get_persistent_size(pt_lol->h_cdb,
                                         /* currently, LOL record contain no UNION field,
                                            so the endian conversion function to intrepret
                                            the UNION field is not needed. */
                                         /* (db_endian_conv_fct)lol_union_end_conv_to_file, */
                                         NULL,
                                         &ui4_persistent_size,
                                         &ui4_ver_id);
            if ( i4_rc == DBR_OK )
            {
                x_dbg_stmt("[with CDB object using: [%d bytes]\n", ui4_persistent_size);
            }

            i4_rc = LOLR_OK;
        }
        else
        {
            x_dbg_stmt("Error: can get info on this LOL handle.\n");
            i4_rc = LOLR_FAIL;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}



/*----------------------------------------------------------------------
 * Name: x_lol_set_cursor
 *
 * Description: This API specifies a entry position in LOL records, 
 *   which will read when API x_lol_read_cursor() is called.
 * 
 *
 * Inputs: 
 *   h_lol       Handle to the LOL object.
 *   e_pos       position of the LOL record to read.
 *
 * Outputs: 
 *   None.
 *               
 * Returns: 
 *   LOLR_OK    Data from the specified record is returned. 
 *   LOLR_INV_HANDLE    Bad handle.
 *   LOLR_INV_ARG       'e_pos' is invalid.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_set_cursor
(
    HANDLE_T            h_lol,
    LOL_ITERATE_T       e_pos
)
{
    INT32           i4_rc;
    LOL_OBJ_T*  	pt_lol;
    HANDLE_TYPE_T   pe_type;
    DB_CTRL_FLAG_T  e_cursor_ctrl;
    

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        i4_rc = LOLR_OK;
        
        switch (e_pos)
        {
            case LOL_FIRST_REC:
            {
                e_cursor_ctrl = DB_FIRST_RECORD;
            }
            break;

            case LOL_LAST_REC:
            {
                e_cursor_ctrl = DB_LAST_RECORD;
            }
            break;

            case LOL_NEXT_REC:
            {
                e_cursor_ctrl = DB_NEXT_RECORD;
            }
            break;

            case LOL_PREV_REC:
            {
                e_cursor_ctrl = DB_PREV_RECORD;
            }
            break;

            default:
            {
                i4_rc = LOLR_INV_ARG;
            }
            break;
        }

        if ( i4_rc == LOLR_OK )
        {
            i4_rc=db_set_cursor(pt_lol->h_cdb,
                                e_cursor_ctrl,
                                DB_NULL_RECORD);
            /* translate the CDB error code to LOL error code */
            i4_rc=lol_cdb_to_lol_err_code(i4_rc);
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_read_cursor
 *
 * Description: This API read the content of a specific record
 *   stored in the LOL database.  The specific record is read
 *   is the record currently pointed by the cursor.   
 *              
 * Inputs: 
 *   h_lol           Handle to the LOL object.
 *
 * Outputs: 
 *   pt_lol_rec  Pointer to a service record structure.   This structure must 
 *               be allocated by the caller.  On return, it is filled with 
 *               service record information.
 *
 *   pui4_ver_id Pointer to a variable holding version id of the database 
 *               object.   Note: version id of the LOL database object is 
 *               incremented each time when a modification takes place.
 * Returns: 
 *   LOLR_OK    Data from the specified record is returned. 
 *   LOLR_INV_HANDLE    Bad handle.
 *   LOLR_INV_ARG       'pui4_ver_id' or 'pt_lol_rec' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_read_cursor
(
    HANDLE_T            h_lol,
    LOL_REC_T*          pt_lol_rec,
    UINT32*             pui4_ver_id
)
{
    INT32           i4_rc;
    LOL_OBJ_T*  	pt_lol;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        if ( pui4_ver_id    != NULL &&
             pt_lol_rec     != NULL
            )
        {          
            i4_rc=lol_read_rec_at_cursor_pos(pt_lol,
                                             pt_lol_rec,
                                             pui4_ver_id);            
        }
        else
        {
            i4_rc=LOLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_sort_rec
 *
 * Description: This API applies the sort operation (using the
 *    per-handle compare fucntion, or the global compare
 *    function, or default compare function, to the LOL
 *    records.
 *             
 * Inputs: 
 *    h_lol     Handle to the LOL object.
 *
 * Outputs: 
 *    pui4_ver_id   Contains the version id of the LOL database object.   
 *                  
 * Returns: 
 *   LOLR_OK           
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_sort_rec
(
    HANDLE_T			h_lol,
    UINT32*             pui4_ver_id 
)
{
    INT32               i4_rc;
    LOL_OBJ_T*  	    pt_lol;
    HANDLE_TYPE_T       pe_type;
 
    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                    &&
         (pt_lol  != NULL)
        )
    {
        if ( pui4_ver_id  != NULL )
        {
            i4_rc=lol_sort_rec_by_rec_id(pt_lol,pui4_ver_id);
        }
        else
        {
            i4_rc=LOLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: x_lol_init_mem_dump
 *
 * Description: This API initializes the memory dump structure, which 
 *              is used for x_lol_dump_to_mem.
 *
 * Inputs: 
 *   None
 *
 * Outputs: 
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Returns: 
 *   LOLR_OK            LOL database successfully dumped.
 *   LOLR_INV_ARG       'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_init_mem_dump
(
    LOL_MEM_DUMP_T*        pt_mem_dump
)
{
    INT32           i4_rc;

    if ( pt_mem_dump != NULL )
    {          
        x_memset(pt_mem_dump, 0, sizeof(LOL_MEM_DUMP_T));
        i4_rc=LOLR_OK;
    }
    else
    {
        i4_rc=LOLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_free_mem_dump
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
 *   LOLR_OK            LOL database successfully dumped.
 *   LOLR_INV_ARG       'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_free_mem_dump
(
    LOL_MEM_DUMP_T*        pt_mem_dump
)
{
    INT32           i4_rc;

    if ( pt_mem_dump != NULL )
    {          
        if (pt_mem_dump->pv_data != NULL)
        {
            x_mem_free(pt_mem_dump->pv_data);
            pt_mem_dump->pv_data = NULL;
        }
        i4_rc=LOLR_OK;
    }
    else
    {
        i4_rc=LOLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_dump_to_mem
 *
 * Description: This API dumps the specified LOL database into the specified
 *              structure, which could be later used for x_lol_load_from_mem.
 *              The memory dump structure should be initialized before dumping.
 *
 * Inputs: 
 *   h_lol           Handle to the LOL object.
 *
 * Outputs: 
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Returns: 
 *   LOLR_OK            LOL database successfully dumped.
 *   LOLR_INV_HANDLE    Bad handle.
 *   LOLR_INV_ARG       'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_dump_to_mem
(
    HANDLE_T               h_lol,
    LOL_MEM_DUMP_T*        pt_mem_dump
)
{
    INT32           i4_rc;
    LOL_OBJ_T*      pt_lol;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                               &&
         (pt_lol  != NULL)
        )
    {
        if ( pt_mem_dump != NULL )
        {
            pt_mem_dump->ui2_lol_id = pt_lol->ui2_lol_id;
            x_strcpy(pt_mem_dump->ps_lol_name, pt_lol->ps_lol_name);
            i4_rc = db_dump_to_mem(pt_lol->h_cdb,
                                   TRUE,
                                   &(pt_mem_dump->pv_data),
                                   &(pt_mem_dump->ui4_data_len));
            i4_rc = lol_cdb_to_lol_err_code(i4_rc);
        }
        else
        {
            i4_rc=LOLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_load_from_mem
 *
 * Description: This API loads the LOL database from the specified memory
 *              dump structure, whose data comes from a previous call to
 *              x_lol_load_from_mem.
 *
 * Inputs: 
 *   h_lol           Handle to the LOL object.
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Outputs: 
 *
 * Returns: 
 *   LOLR_OK            LOL database successfully dumped.
 *   LOLR_INV_HANDLE    Bad handle.
 *   LOLR_INV_ARG       'pt_mem_dump' is NULL or LOL metadata not match.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_load_from_mem
(
    HANDLE_T               h_lol,
    LOL_MEM_DUMP_T*        pt_mem_dump
)
{
    INT32           i4_rc;
    LOL_OBJ_T*      pt_lol;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_lol,&pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                               &&
         (pt_lol  != NULL)
        )
    {
        if ( pt_mem_dump != NULL )
        {
            if (pt_mem_dump->ui2_lol_id == pt_lol->ui2_lol_id &&
                x_strcmp(pt_mem_dump->ps_lol_name, pt_lol->ps_lol_name) == 0)
            {
                i4_rc = db_load_from_mem(pt_lol->h_cdb,
                                         pt_mem_dump->pv_data,
                                         pt_mem_dump->ui4_data_len);
                i4_rc = lol_cdb_to_lol_err_code(i4_rc);
            }
            else
            {
                i4_rc = LOLR_INV_ARG;
            }
        }
        else
        {
            i4_rc=LOLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: x_lol_fs_store_given_file_handle
 *
 * Description: This API stores the specified LOL object to the 
 *              file system.
 *
 * Inputs:
 *   h_dir         The handle to the file.
 *
 *  ui2_lol_id     Specifies the LOL object to be stored to the file system.  
 *                 The limit for LOL ID is between 1 and 65535.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK             LOL object is stored to the file system. 
 *
 *   LOLR_NOT_FOUND      The LOL object specified by the 'ui2_lol_id' 
 *                       does not exist.
 *
 *   LOLR_INV_ARG        ps_lol_path' is NULL.
 *
 *   LOLR_INV_FILE_PATH  Specified file path is not a directory or 
 *                       a RAW character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_fs_store_given_file_handle
(
    HANDLE_T        h_file,
    UINT16          ui2_lol_id  
)
{
    INT32       i4_rc;
    
    if ( ui2_lol_id  != 0 )
    {
        DBG_API(("{LOL} saving LOL data.\n"));
        
        i4_rc=lol_fs_store_object_given_file_handle(h_file,ui2_lol_id);

        DBG_API(("{LOL} done saving LOL data [ret code: %d]\n",i4_rc));
    }
    else
    {
        i4_rc = LOLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lol_fs_sync_given_file_handle
 *
 * Description: This API sync a specified LOL handle with LOL records
 *              from persistent file system.  The existing LOL records
 *              are replaced with new records read from file. 
 *
 * Inputs:
 *
 *   h_lol          Specify the handle to the LOL object.
 *
 *   h_file         The handle to the file.
 *
 *   ui2_lol_id     Specifies the LOL id to load from the file system.  
 *                  The limit for LOL ID is between 1 and 65535.
 *
 * Outputs: 
 *
 *   None.
 *                 
 * Returns: 
 *   LOLR_OK             LOL records are load from the persistent file
 *                       into the memory of LOL object.
 *
 *   LOLR_NOT_FOUND      'ui2_lol_id' is not found or 'ps_lol_path' 
 *                       does not exist.
 *
 *   LOLR_FAIL           internal error
 *
 *   LOLR_INV_ARG        'ps_lol_path' is NULL, or 'h_lol' is NULL_HANDLE,
 *                       or ui2_lol_id is 0.
 *
 *   LOLR_INV_FILE_PATH  Specified file path is not a directory or a RAW 
 *                       character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_lol_fs_sync_given_file_handle
(
    HANDLE_T		h_lol,	
    HANDLE_T        h_file,
    UINT16		    ui2_lol_id
)
{
    INT32           i4_rc;
    
    if ( h_lol       != NULL_HANDLE     &&
         ui2_lol_id  != 0   
        )
    {
        i4_rc=lol_fs_sync_object_given_file_handle(h_lol,
                                                   h_file,
                                                   ui2_lol_id);
    }
    else
    {
        i4_rc = LOLR_INV_ARG;
    }
    return i4_rc;
}

