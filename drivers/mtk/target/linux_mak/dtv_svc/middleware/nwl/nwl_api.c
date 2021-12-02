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
 * $RCSfile: nwl_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/18 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: eeeee16d218b478d12d6bd39898cde8d $
 *
 * Description:
 *         This file contain implementation of Network List API's,
 *         which are exported to other Middleware components.
 *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/


#include "os/inc/x_os.h"
#include "os/inc/os.h"
#include "cdb/cdb_api.h"

#include "nwl/_nwl.h"
#include "nwl/nwl_rec.h"
#include "nwl/nwl_rec_id.h"
#include "nwl/nwl_fs.h"

/*-----------------------------------------------------------------------------
 * Name: x_nwl_create
 *
 * Description: 
 *
 * Inputs: 
 *   ps_nwl_name   Name of the NWL object. 
 *   ui2_nwl_id    Unique id for the NWL object.  
 *                 The limit for NWL ID is between 1 and 65535.
 *   e_mem_policy  Memory policy: NWL_MEM_STATIC or NWL_MEM_DYNAMIC
 *   ui2_max_nb_rec Specifies the maximum number of NWL record (for
 *                 NWL_MEM_STATIC), or it specifies number of
 *                 NWL records per memory chunk.
 *   pv_tag        Pointer to a private tag value provided by the 
 *                 caller.  This tag value is passed to the 
 *                 caller's notification function.
 *   pf_nfy        Notification function provided by the caller.  
 *                 This function is called to notify the owner of 
 *                 this NWL handle when the NWL object has been 
 *                 modified.  If NULL, then no notification function 
 *                 will be called.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           NWL object is created successfully.
 *   NWLR_INV_ARG      'ui2_nwl_id' is 0, or ph_nwl is NULL, or ps_nwl_name
 *                     is NULL.
 *   NWLR_OUT_OF_HANDLE Cannot get a handle.
 *   NWLR_FAIL         The specified 'ps_nwl_name' or 'ui2_nwl_id' value
 *                     is not unique (e.g., it is currently been used), or
 *                     limit of the number of NWL objects that can be 
 *                     created has been reached.
 *   NWLR_OUT_OF_MEM   Not enough memory to create NWL object.
 * 
 ----------------------------------------------------------------------------*/
INT32 x_nwl_create(const CHAR*      ps_nwl_name,
                   UINT16           ui2_nwl_id,
                   NWL_MEM_POLICY_T e_mem_policy,
                   UINT16           ui2_max_nb_rec,
                   VOID*            pv_tag,
                   x_nwl_nfy_fct    pf_nfy,
                   HANDLE_T*        ph_nwl)
{
    INT32  i4_rc=NWLR_FAIL;
    CHAR   s_nwl_name[MAX_NWL_NAME_LEN];
    
    if ( ui2_nwl_id != 0            &&
         ps_nwl_name != NULL        &&
         x_strlen(ps_nwl_name) > 0  &&
         ph_nwl != NULL )
    {
        x_strncpy(s_nwl_name,ps_nwl_name,MAX_NWL_NAME_LEN-1);
        s_nwl_name[MAX_NWL_NAME_LEN-1]='\0';
            
        nwl_sema_lock();
        /* check if the input NWL name and id has not been used. */
        /* Add the name and id to the internal list. */
        if ( nwl_check_id_name(ui2_nwl_id,s_nwl_name) == TRUE &&
             nwl_add_id_name(ui2_nwl_id,s_nwl_name) == NWLR_OK
            )
        {
            *ph_nwl=NULL_HANDLE;
            i4_rc=nwl_create_object(s_nwl_name,ui2_nwl_id,
                                    e_mem_policy,ui2_max_nb_rec,
                                    pv_tag,pf_nfy,ph_nwl);
            if ( i4_rc != NWLR_OK )
            {
                /* Remove the id name pair. */
                nwl_del_id(ui2_nwl_id);
            }
        }
        nwl_sema_unlock();
    }
    else
    {
        i4_rc=NWLR_INV_ARG;
    }

    if ( i4_rc == NWLR_OK )
    {
        DBG_API(("{NWL} [name: %s nwl_id: %d handle: %d] created\n",ps_nwl_name, ui2_nwl_id, *ph_nwl));
    }
    else
    {
        DBG_ERROR(("{NWL} [name: %s nwl_id: %d] failed.\n", ps_nwl_name, ui2_nwl_id));
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_delete
 *
 * Description: This API deletes the specified NWL object associated 
 *              with 'h_nwl' handle.    Other clients with a handle to 
 *              the deleted NWL object will receive a notification of 
 *              the delete event.   
 *
 * Inputs: 
 *   h_nwl	Handle to the NWL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           Specified NWL object is deleted.
 *   NWLR_INV_HANDLE   Bad handle.
 -----------------------------------------------------------------------*/
INT32 x_nwl_delete(HANDLE_T  h_nwl)
{
    INT32           i4_rc;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        nwl_sema_lock();
        /* remove nwl id/name pairs. */
        nwl_del_id(pt_nwl->ui2_nwl_id);
        nwl_sema_unlock();
        
        i4_rc=nwl_delete_object(pt_nwl);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_clean
 *
 * Description: This API removes all the network list records from the 
 *              specified NWL object pointed to by the 'h_nwl' handle.    
 *              All other handles to this NWL object will receive a 
 *notification.   
 *
 * Inputs: 
 *   h_nwl      Handle to the NWL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           All records are successfully removed. 
 *   NWLR_INV_HANDLE   Bad handle.
 -----------------------------------------------------------------------*/
INT32 x_nwl_clean(HANDLE_T         h_nwl)
{
    INT32     i4_rc;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        i4_rc=nwl_clean_object(pt_nwl);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_add_rec
 *
 * Description: This function adds a new service record to the NWL 
 *              database object.  The NWL library will make copy of 
 *              the record content from the address location pointed to 
 *              by the 'pt_nwl_rec'.  The 'ui2_nwl_rec_id' field in 
 *              the 'pt_nwl_rec' structure specifies the key field of the 
 *              service record to be added to the database.  
 *
 * Inputs: 
 *   h_nwl      Handle to the NWL object.
 *   pt_nwl_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           Record is successfully added. 
 *   NWLR_INV_HANDLE   Bad NWL handle.
 *   NWLR_OUT_OF_MEM   Can not allocate memory, add record failed,.
 *   NWLR_INV_ARG      'pt_nwl_rec' is null.  
 -----------------------------------------------------------------------*/
INT32 x_nwl_add_rec(HANDLE_T         h_nwl,
                    const NWL_REC_T* pt_nwl_rec)
{
    INT32           i4_rc;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        if ( pt_nwl_rec != NULL                             &&
             pt_nwl_rec->ui2_nwl_rec_id != NWL_NULL_REC_ID
           )
        {
            i4_rc=nwl_write_rec_to_object(pt_nwl,
                                          pt_nwl_rec,
                                          NWL_APPEND_RECORD);
        }
        else
        {
            i4_rc=NWLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}
/*----------------------------------------------------------------------
 * Name: x_nwl_del_rec
 *
 * Description: This function deletes a specified service record from 
 *              the NWL.  The record to be deleted is specified by the 
 *              transport stream record id.
 *
 * Inputs: 
 *   h_nwl           Handle to the NWL object.
 *   ui2_nwl_rec_id  Specifies the transport stream record to be deleted. 
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK            Specified record is deleted. 
 *   NWLR_INV_HANDLE    Bad NWL handle.
 *   NWLR_REC_NOT_FOUND The specified record to be deleted is not in 
 *                      the NWL object.
 *   NWLR_INV_ARG       'ui2_nwl_rec_id' is not a valid record id.
 -----------------------------------------------------------------------*/
INT32 x_nwl_del_rec(HANDLE_T         h_nwl,
                    UINT16           ui2_nwl_rec_id)
{
    INT32           i4_rc;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        if ( ui2_nwl_rec_id != NWL_NULL_REC_ID )
        {
            i4_rc=nwl_del_rec_from_object(pt_nwl, ui2_nwl_rec_id);
        }
        else
        {
            i4_rc = NWLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_update_rec
 *
 * Description: This function updates network list record to the NWL
 *              object. If the key field value (e.g., 'ui2_nwl_rec_id' field
 *              in the 'pt_nwl_rec' structure) of the input service record 
 *              matches an existing record in the NWL object, then the 
 *              existing record is over-wrote with the new information,  
 *              otherwise a new record is created in the NWL database. 
 * 
 *              If the 'b_must_exist' flag is TRUE, then there must be an 
 *              existing record with the same key field value, else a status 
 *              of 'NWLR_REC_NOT_FOUND' is returned to the caller.  
 *              If 'b_must_exist' flag is FALSE, then a new service record 
 *              is created if no existing record matching the key field value 
 *              is found in the NWL database.
 *
 *              Call to this function must be protected, e.g., the NWL 
 *              database object needs to be locked before the call to this 
 *              function and unlock after returning from call to this function.
 *
 * Inputs: 
 *   h_nwl      Handle to the NWL object.
 *   pt_nwl_rec Pointer to service record structure. 
 *   b_must_exist Boolean flag specifying if the service record to be 
 *                updated should exist in the NWL database
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           Record is successfully added/updated.
 *   NWLR_INV_HANDLE   Bad NWL handle. 
 *   NWLR_OUT_OF_MEM   Not enough memory to create a new NWL record.
 *   NWLR_REC_NOT_FOUND The specified record is not found 
 *                      ('b_must_exist' == TRUE)
 *   NWLR_INV_ARG      'pt_nwl_rec' is NULL or the pt_nwl_rec
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_update_rec(HANDLE_T         h_nwl,
                       const NWL_REC_T* pt_nwl_rec,
                       BOOL             b_must_exist)
{
    INT32           i4_rc;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;
    UINT8           ui1_write_mode;

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        if ( pt_nwl_rec != NULL                              &&
             pt_nwl_rec->ui2_nwl_rec_id != NWL_NULL_REC_ID
           )
        {
            if ( b_must_exist == TRUE )
            {
                ui1_write_mode=NWL_MODIFY_RECORD;
            }
            else
            {
                ui1_write_mode=NWL_UPDATE_RECORD;
            }
            
            i4_rc=nwl_write_rec_to_object(pt_nwl,
                                          pt_nwl_rec,
                                          ui1_write_mode);            
        }
        else
        {
            i4_rc=NWLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_qry_rec_id
 *
 * Description: This API queries the range (FIRST, LAST, or NEXT) of 
 *              valid transport stream record id values used in the NWL
 *              object.  
 *   
 * Inputs: 
 *   h_nwl             Handle to the NWL object.
 *
 *   e_nwl_qry_option  Flag to specifies the record id value to be returned.  
 *                     The values are:
 *
 *                     NWL_FIRST_REC_ID:  the smallest valid record id in 
 *                                        this NWL database.
 *                     NWL_LAST_REC_ID:   the largest valid record id in 
 *                                        this NWL database.
 *                     NWL_NEXT_REC_ID:   return next available record id that
 *                                        can be uesed to specify a new NWL
 *                                        record entry.
 *
 * Outputs: None
 *   pui2_rec_id       Contain the transport stream record id value.	
 *
 * Returns: 
 *   NWLR_OK           NWL record id return.
 *   NWLR_INV_HANDLE   Bad NWL handle. 
 *   NWLR_INV_ARG      'pui2_rec_id' is NULL.
 *
 -----------------------------------------------------------------------*/
INT32 x_nwl_qry_rec_id(HANDLE_T         h_nwl,
                       NWL_REC_ID_QRY_T e_nwl_qry_option,
                       UINT16*          pui2_rec_id)
{
    INT32               i4_rc;
    NWL_OBJ_T*  	    pt_nwl;
    HANDLE_TYPE_T       pe_type;

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        if ( pui2_rec_id != NULL )
        {
            switch (e_nwl_qry_option)
            {
                case NWL_FIRST_REC_ID:
                {
                    if ( nwl_get_smallest_rec_id(pt_nwl, pui2_rec_id) == TRUE )
                    {
                        i4_rc=NWLR_OK;
                    }
                    else
                    {
                        i4_rc=NWLR_REC_NOT_FOUND;
                    }
                }
                break;

                case NWL_LAST_REC_ID:
                {
                    if ( nwl_get_largest_rec_id(pt_nwl, pui2_rec_id) == TRUE )
                    {
                        i4_rc=NWLR_OK;
                    }
                    else
                    {
                        i4_rc=NWLR_REC_NOT_FOUND;
                    }
                }
                break;

                case NWL_NEXT_REC_ID:
                {
                    nwl_get_next_rec_id(pt_nwl, pui2_rec_id);
                    i4_rc=NWLR_OK;
                }
                break;

                default:
                {
                    *pui2_rec_id=NWL_NULL_REC_ID;
                    i4_rc=NWLR_INV_ARG;
                }
            }
        }
        else
        {
            i4_rc=NWLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}

    
/*----------------------------------------------------------------------
 * Name: x_nwl_lock
 *
 * Description: This function locks the NWL object.   The function blocks 
 *              until the lock is acquired.
 *
 * Inputs: 
 *   h_nwl	Handle to the NWL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           NWL is locked. 
 *   NWLR_INV_HANDLE   Bad NWL handle.
 *
 -----------------------------------------------------------------------*/
INT32 x_nwl_lock(HANDLE_T h_nwl)
{
    INT32           i4_rc;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        i4_rc=nwl_lock_object(pt_nwl);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_unlock
 *
 * Description: This function unlocks the NWL database. 
 *
 * Inputs: 
 *   h_nwl	Handle to the NWL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK          NWL database is unlocked. 
 *   NWLR_INV_HANDLE  Bad NWL handle.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_unlock(HANDLE_T  h_nwl)
{
    INT32           i4_rc;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        i4_rc=nwl_unlock_object(pt_nwl);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_read_lock
 *
 * Description: This function acquires read-lock on the specified NWL object.
 *    The function blocks until the read-lock is acquired.
 *
 * Inputs: 
 *   h_nwl	Handle to the NWL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           NWL is locked. 
 *   NWLR_INV_HANDLE   Bad NWL handle.
 *
 -----------------------------------------------------------------------*/
INT32 x_nwl_read_lock(HANDLE_T  h_nwl)
{
    INT32           i4_rc;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        i4_rc=nwl_lock_object_for_read(pt_nwl);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_read_unlock
 *
 * Description: This function releases the read-lock on the specified
 *      NWL database. 
 *
 * Inputs: 
 *   h_nwl	Handle to the NWL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK          NWL database is unlocked. 
 *   NWLR_INV_HANDLE  Bad NWL handle.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_read_unlock(HANDLE_T  h_nwl)
{
    INT32           i4_rc;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        i4_rc=nwl_unlock_object_for_read(pt_nwl);
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_open
 *
 * Description: This function gets a handle to the specified NWL database 
 *              object by its ID value.    
 *
 * Inputs: 
 *   ui2_nwl_id:  Specifies the NWL object identifier value.  
 *                The range for NWL ID is between 1 and 65535.
 *   pv_tag:      A private tag value associated with the calling client.
 *   pf_nfy:      Pointer to the caller provided notification function.  
 *                The notification function will be called in the context of 
 *                the thread that modifies (update, add, delete) the NWL 
 *                database.  If NULL, then no notification function will be 
 *                called.
 *
 * Outputs:
 *   ph_nwl:      Pointer to the handle created.
 *
 * Returns: 
 *   NWLR_OK               NWL handle is returned. 
 *   NWLR_OUT_OF_HANDLE    No more handle available.
 *   NWLR_OUT_OF_MEM       Not enough memory to create the NWL control 
 *                         structure.
 *   NWLR_NOT_FOUND        The specified NWL database object id (e.g. 
 *                         'ui2_nwl_id') is not found.
 *   NWLR_INV_ARG          'ph_nwl' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_open(UINT16          ui2_nwl_id,
                 VOID*           pv_tag,
                 x_nwl_nfy_fct   pf_nfy,
                 HANDLE_T*       ph_nwl)
{
    INT32           i4_rc;
    CHAR            ps_nwl_name[MAX_NWL_NAME_LEN];

    if ( ph_nwl  != NULL )
    {
        *ph_nwl=NULL_HANDLE;
        nwl_sema_lock();
        /* Find the id/name */
        if ( nwl_get_name_by_id(ui2_nwl_id, ps_nwl_name) == TRUE )
        {
            i4_rc=nwl_open_object(ui2_nwl_id,
                                  ps_nwl_name,
                                  pv_tag,
                                  pf_nfy,
                                  ph_nwl);
        }
        else
        {
            i4_rc=NWLR_NOT_FOUND;
        }
        nwl_sema_unlock();
    }
    else
    {
        i4_rc = NWLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_close
 *
 * Description: This function frees the specified handle to the NWL 
 *              database object.    
 *
 * Inputs: 
 *   h_nwl	Handle to NWL database object.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           NWL handle is freed. 
 *   NWLR_INV_HANDLE   Bad handle.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_close             (HANDLE_T        h_nwl)
{
    INT32           i4_rc;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        handle_free(h_nwl,TRUE);
        i4_rc=NWLR_OK;
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_get_rec
 *
 * Description: This API gets a specific record stored in the NWL database 
 *              object.    The search parameter is specified by the 
 *              'ui2_nwl_rec_id' value.
 *
 * Inputs: 
 *   h_nwl           Handle to the NWL object.
 *   ui2_nwl_rec_id  Specifies the service record id.
 *
 * Outputs: 
 *   pt_nwl_rec  Pointer to a network record structure.   This structure must 
 *               be allocated by the caller.  On return, it is filled with 
 *               network record information.
 *   pui4_ver_id Pointer to a variable holding version id of the database 
 *               object.   Note: version id of the NWL database object is 
 *               incremented each time when a modification takes place.
 * Returns: 
 *   NWLR_OK    Data from the specified record is returned. 
 *   NWLR_INV_HANDLE    Bad handle.
 *   NWLR_REC_NOT_FOUND No record matching the search parameter 
 *                      ('ui2_nwl_rec_id')  is found.
 *   NWLR_INV_ARG       'pui4_ver_id' or 'pt_nwl_rec' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_get_rec(HANDLE_T        h_nwl,
                    UINT16          ui2_nwl_rec_id,
                    NWL_REC_T*      pt_nwl_rec,
                    UINT32*         pui4_ver_id)
{
    INT32     i4_rc;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        if ( pui4_ver_id    != NULL &&
             pt_nwl_rec     != NULL &&
             ui2_nwl_rec_id != NWL_NULL_REC_ID   )
        {
            i4_rc=nwl_get_rec_from_obj(pt_nwl,
                                       ui2_nwl_rec_id,
                                       pt_nwl_rec,
                                       pui4_ver_id);
        }
        else
        {
            i4_rc=NWLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_get_rec_by_rec_idx
 *
 * Description: This API returns network list record entry from the NWL
 *              database object by its index. Maximum index is obtained
 *              by calling x_nwl_get_num_rec.
 *
 * Inputs: 
 *   h_nwl           Handle to the NWL object.
 *   
 *   ui2_idx      Index for iterating the NWL records matching 
 *                the specified network id number.  0 means the first record
 *                found in NWL list that matches the specified network id
 *                number, and 1 is the next record
 *                found, and so on.
 *
 * Outputs: None
 *   pt_nwl_rec	  Pointer to a NWL record structure.
 *
 * Input/Output arguments
 *   pui4_ver_id  Pointer to a variable holding version id of the NWL 
 *                database object.  On entry, this variable contains 
 *                the version id of the database that the caller expects.  
 *                On the return, this variable contains the current version 
 *                id of the database.   If the expected version id and 
 *                current database version id do not match, an error status 
 *                code is returned.  If 'pui4_ver_id' is set to 
 *                'NWL_NULL_VER_ID' value, then no checking is done and 
 *                the latest version id is returned the application. 
 * Returns: 
 *   NWLR_OK             A NWL_REC_T structure is returned. 
 *   NWLR_INV_HANDLE     Bad 'h_nwl' handle.
 *   NWLR_REC_NOT_FOUND  Specified record is not found.
 *   NWLR_INVALID_ARG    'pui4_ver_id' or 'pt_nwl_rec' is NULL.
 *   NWLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id' 
 *                       pass into the API and the internal database version 
 *                       id is not the same.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_get_rec_by_rec_idx(HANDLE_T      h_nwl,
                               UINT16        ui2_idx,
                               NWL_REC_T*    pt_nwl_rec,
                               UINT32*       pui4_ver_id)
{
    INT32               i4_rc;
    NWL_OBJ_T*  	    pt_nwl;
    HANDLE_TYPE_T       pe_type;
    
    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_nwl_rec     != NULL
            )
        {
            i4_rc=nwl_get_rec_by_flds(pt_nwl,
                                      0,
                                      NULL,
                                      NULL,
                                      NULL,
                                      pt_nwl_rec,
                                      ui2_idx,
                                      pui4_ver_id);            
        }
        else
        {
            i4_rc=NWLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_get_rec_by_nw_id
 *
 * Description: This API returns network list record entry with a specified 
 *              network id value from the NWL database object.  If there are
 *              multiple records matching the specified network id, the
 *              caller can use 'ui2_idx' to iterate over network records
 *              with the same network id.    
 *
 * Inputs: 
 *   h_nwl           Handle to the NWL object.
 *   ui2_network_id  Specifies the network id of the network record
 *                   to be returned.
 *   
 *   ui2_idx      Index for iterating the NWL records matching 
 *                the specified network id number.  0 means the first record
 *                found in NWL list that matches the specified network id
 *                number, and 1 is the next record
 *                found, and so on.
 *
 * Outputs: None
 *   pt_nwl_rec	  Pointer to a NWL record structure.
 *
 * Input/Output arguments
 *   pui4_ver_id  Pointer to a variable holding version id of the NWL 
 *                database object.  On entry, this variable contains 
 *                the version id of the database that the caller expects.  
 *                On the return, this variable contains the current version 
 *                id of the database.   If the expected version id and 
 *                current database version id do not match, an error status 
 *                code is returned.  If 'pui4_ver_id' is set to 
 *                'NWL_NULL_VER_ID' value, then no checking is done and 
 *                the latest version id is returned the application. 
 * Returns: 
 *   NWLR_OK             A NWL_REC_T structure is returned. 
 *   NWLR_INV_HANDLE     Bad 'h_nwl' handle.
 *   NWLR_REC_NOT_FOUND  Specified record is not found.
 *   NWLR_INVALID_ARG    'pui4_ver_id' or 'pt_nwl_rec' is NULL.
 *   NWLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id' 
 *                       pass into the API and the internal database version 
 *                       id is not the same.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_get_rec_by_nw_id  (HANDLE_T      h_nwl,
                               UINT16        ui2_network_id,
                               UINT16		 ui2_idx,
                               NWL_REC_T*    pt_nwl_rec,
                               UINT32*       pui4_ver_id)
{
    INT32               i4_rc;
    NWL_OBJ_T*  	    pt_nwl;
    HANDLE_TYPE_T       pe_type;
    
    UINT8               ui1_key_count;
    DB_FIELD_T          aui4_keyfld_type[1];
    const VOID*         apv_keyfld_addr[1];
    
    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_nwl_rec     != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            ui1_key_count = 1;
            aui4_keyfld_type[0]       = (DB_FIELD_T) NWL_CDB_TYPE_NETWORK_ID ;
            apv_keyfld_addr[0]        = &ui2_network_id;
            
            i4_rc=nwl_get_rec_by_flds(pt_nwl,
                                      ui1_key_count,
                                      aui4_keyfld_type,
                                      apv_keyfld_addr,
                                      NULL,
                                      pt_nwl_rec,
                                      ui2_idx,
                                      pui4_ver_id);            
        }
        else
        {
            i4_rc=NWLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_get_rec_by_sat
 *
 * Description: This API returns network list record entry with reference to
 *              the specified SATL record from the NWL database object.  If
 *              there are multiple records matching the specified network
 *              id, the caller can use 'ui2_idx' to iterate over network
 *              records with the reference to the SATL record.    
 *
 * Inputs: 
 *   h_nwl           Handle to the NWL object.
 *   ui2_satl_id     Specifies the SATL ID of the given SATL record.
 *   ui2_satl_rec_id Specifies the SATL record ID of the given SATL record.
 *   
 *   ui2_idx      Index for iterating the NWL records matching 
 *                the specified network id number.  0 means the first record
 *                found in NWL list that matches the specified SATL record,
 *                and 1 is the next record found, and so on.
 *
 * Outputs: None
 *   pt_nwl_rec   Pointer to a NWL record structure.
 *
 * Input/Output arguments
 *   pui4_ver_id  Pointer to a variable holding version id of the NWL 
 *                database object.  On entry, this variable contains 
 *                the version id of the database that the caller expects.  
 *                On the return, this variable contains the current version 
 *                id of the database.   If the expected version id and 
 *                current database version id do not match, an error status 
 *                code is returned.  If 'pui4_ver_id' is set to 
 *                'NWL_NULL_VER_ID' value, then no checking is done and 
 *                the latest version id is returned the application. 
 * Returns: 
 *   NWLR_OK             A NWL_REC_T structure is returned. 
 *   NWLR_INV_HANDLE     Bad 'h_nwl' handle.
 *   NWLR_REC_NOT_FOUND  Specified record is not found.
 *   NWLR_INVALID_ARG    'pui4_ver_id' or 'pt_nwl_rec' is NULL.
 *   NWLR_MODIFIED       The database has bee modified.  The 'pui4_ver_id' 
 *                       pass into the API and the internal database version 
 *                       id is not the same.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_get_rec_by_sat  (HANDLE_T       h_nwl,
                             UINT16         ui2_satl_id,
                             UINT16         ui2_satl_rec_id,
                             UINT16         ui2_idx,
                             NWL_REC_T*     pt_nwl_rec,
                             UINT32*        pui4_ver_id)
{
    INT32               i4_rc;
    NWL_OBJ_T*          pt_nwl;
    HANDLE_TYPE_T       pe_type;
    
    UINT8               ui1_key_count;
    DB_FIELD_T          aui4_keyfld_type[2];
    const VOID*         apv_keyfld_addr[2];
    
    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        if (
             pui4_ver_id    != NULL &&
             pt_nwl_rec     != NULL
            )
        {
            /* Set up the search parameter and address of the key value
               for the search.
            */
            ui1_key_count = 2;
            aui4_keyfld_type[0]       = (DB_FIELD_T) NWL_CDB_TYPE_SATL_ID ;
            apv_keyfld_addr[0]        = &ui2_satl_id;
            aui4_keyfld_type[1]       = (DB_FIELD_T) NWL_CDB_TYPE_SATL_REC_ID ;
            apv_keyfld_addr[1]        = &ui2_satl_rec_id;
            
            i4_rc=nwl_get_rec_by_flds(pt_nwl,
                                      ui1_key_count,
                                      aui4_keyfld_type,
                                      apv_keyfld_addr,
                                      NULL,
                                      pt_nwl_rec,
                                      ui2_idx,
                                      pui4_ver_id);            
        }
        else
        {
            i4_rc=NWLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_get_num_rec
 *
 * Description:    This function returns the number of records for the
 *                 specified NWL. 
 *
 * Inputs: 
 *   h_nwl	Handle to the NWL object.
 *
 * Outputs: 
 *   pui2_num_rec	Pointer to a variable for the number of records in
 *                  the NWL. 	
 *   pui4_ver_id    Pointer to a variable holding version id of the NWL
 *                  database object. 	
 * Returns: 
 *   NWLR_OK			Number of records in the specified NWL is returned. 
 *   NWLR_INV_HANDLE    Bad 'h_nwl' handle.
 *   NWLR_INV_ARG       'pui2_num_rec' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_get_num_rec(HANDLE_T            h_nwl,
                        UINT16*             pui2_num_rec,
                        UINT32*             pui4_ver_id)
{
    INT32           i4_rc;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;
    
    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        if (  pui4_ver_id  != NULL   &&
              pui2_num_rec != NULL
           )
        {
            i4_rc=nwl_get_num_rec(pt_nwl,pui2_num_rec,pui4_ver_id);
        }
        else
        {
            i4_rc=NWLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }    
    return i4_rc;
}

    
/*----------------------------------------------------------------------
 * Name: x_nwl_get_num_rec_by_nw_id
 *
 * Description: This function returns the number of records for the
 *              specified network id.           
 *
 * Inputs: 
 *   h_nwl	        Handle to the NWL object.
 *
 *   ui2_network_id Specifies the network id value.   
 *
 * Outputs: 
 *   pui2_num_recs	Pointer to a variable for the number of records
 *                  matching the specified stream type in the NWL. 	
 *   pui4_ver_id    Pointer to a variable holding version id of the NWL
 *                  database object. 	
 * Returns: 
 *   NWLR_OK         Number of records found.
 *   NWLR_INV_HANDLE Bad 'h_nwl' handle.
 *   NWLR_INV_ARG    'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_get_num_rec_by_nw_id(HANDLE_T            h_nwl,
                                 UINT16              ui2_network_id,
                                 UINT16*             pui2_num_recs,
                                 UINT32*             pui4_ver_id)
{
    INT32           i4_rc;
    UINT8           ui1_key_count;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;
    DB_FIELD_T      aui4_keyfld_type[1];
    const VOID*     apv_keyfld_addr[1];
    /*
    DB_SEARCH_T     ae_cmp_param[1];
    */
 
    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
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

            aui4_keyfld_type[0]      = (DB_FIELD_T) NWL_CDB_TYPE_NETWORK_ID ;
            apv_keyfld_addr[0]       = &ui2_network_id;

            /*
            ae_cmp_param[0].e_opcode = DB_OPS_EQUAL;
            */
            
            i4_rc=nwl_get_num_rec_by_flds(pt_nwl,
                                          ui1_key_count,
                                          aui4_keyfld_type,
                                          apv_keyfld_addr,
                                          NULL, /* ae_cmp_param, */
                                          pui2_num_recs,
                                          pui4_ver_id);
        }
        else
        {
            i4_rc=NWLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_get_num_rec_by_sat
 *
 * Description: This function returns the number of records for the
 *              specified satellite.           
 *
 * Inputs: 
 *   h_nwl	        Handle to the NWL object.
 *   ui2_satl_id     Specifies the SATL ID of the given SATL record.
 *   ui2_satl_rec_id Specifies the SATL record ID of the given SATL record.
 *
 *   ui2_network_id Specifies the network id value.   
 *
 * Outputs: 
 *   pui2_num_recs	Pointer to a variable for the number of records
 *                  matching the specified SATL record in the NWL. 	
 *   pui4_ver_id    Pointer to a variable holding version id of the NWL
 *                  database object. 	
 * Returns: 
 *   NWLR_OK         Number of records found.
 *   NWLR_INV_HANDLE Bad 'h_nwl' handle.
 *   NWLR_INV_ARG    'pui2_num_recs' or 'pui4_ver_id' is a NULL pointer.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_get_num_rec_by_sat(HANDLE_T            h_nwl,
                               UINT16              ui2_satl_id,
                               UINT16              ui2_satl_rec_id,
                               UINT16*             pui2_num_recs,
                               UINT32*             pui4_ver_id)
{
    INT32           i4_rc;
    UINT8           ui1_key_count;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;
    DB_FIELD_T      aui4_keyfld_type[2];
    const VOID*     apv_keyfld_addr[2];
 
    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
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
            ui1_key_count = 2;

            aui4_keyfld_type[0]      = (DB_FIELD_T) NWL_CDB_TYPE_SATL_ID ;
            apv_keyfld_addr[0]       = &ui2_satl_id;
            aui4_keyfld_type[1]      = (DB_FIELD_T) NWL_CDB_TYPE_SATL_REC_ID ;
            apv_keyfld_addr[1]       = &ui2_satl_rec_id;

            i4_rc=nwl_get_num_rec_by_flds(pt_nwl,
                                          ui1_key_count,
                                          aui4_keyfld_type,
                                          apv_keyfld_addr,
                                          NULL,
                                          pui2_num_recs,
                                          pui4_ver_id);
        }
        else
        {
            i4_rc=NWLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: x_nwl_rec_init
 *
 * Description: This API pre-sets the NWL_REC_T structure to a 
 *     pre-defined invalid value (e.g., 0xFF).   The pre-defined invalid 
 *     value can be used to detect changes in the NWL_REC_T structure for 
 *     backward compatibility in the future.
 *
 *     For example, if a new field is added to the NWL record structure.  
 *     At the runtime, if the new field in the NWL_REC_T structure passed 
 *     into NWL library contain 0xFF, then we know the caller has not been 
 *     re-compile with the new NWL module.   
 *
 * Inputs: 
 *   Pointer to a NWL record structure. 
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           NWL record is initialized successfully.
 *   NWLR_INV_ARG      'pt_nwl_rec' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 x_nwl_rec_init(NWL_REC_T*        pt_nwl_rec)
{
    INT32     i4_rc;

    if ( pt_nwl_rec  != NULL  )
    {
        x_memset(pt_nwl_rec,0,sizeof(NWL_REC_T));
        i4_rc = NWLR_OK;  
    }
    else
    {
        i4_rc = NWLR_INV_ARG;
    }
    return i4_rc;
}

/********************************************
   NWL data persistent storage related APIs
*********************************************/

/*----------------------------------------------------------------------
 * Name: x_nwl_fs_load
 *
 * Description: This API loads a specified NWL object from file system
 *              into memory, and returns a handle to the newly created
 *              NWL object.
 * Inputs: 
 *   h_dir	      Handle to the parent directory of the file specified by
 *                the path 'ps_nwl_path'.  If 'ps_nwl_path' is an absolute
 *                path, then this argument should be set to 'FM_ROOT_HANDLE'.
 *   ps_nwl_path  Specifies the path to the file where the NWL objects
 *                are stored in the file system.
 *   ui2_nwl_id   Specifies the NWL id to load from the file system.
 *                The limit for NWL ID is between 1 and 65535.
 *   ps_nwl_name  Specifies the name for the NWL object.  If null string
 *                is specified, then the default name stored in the file
 *                system is used to create the NWL object.
 *   pv_tag       Pointer to a private tag value provided by the caller.
 *                This tag value is passed to the caller's notification
 *                function.
 *   pf_nfy       Notification function provided by the caller.  This
 *                function is called to notify the owner of this NWL
 *                handle when the NWL object has been modified by another
 *                client.  If NULL, then no notification function will be
 *                called.
 * Outputs:
 *   ph_nwl	      On return, contain the handle to the NWL object created
 *                from file system.
 * Returns: 
 *   NWLR_OK            NWL object is created. 
 *   NWLR_NOT_FOUND     'ui2_nwl_id' is not found or 'ps_nwl_path' does
 *                      not exist.
 *   NWLR_OUT_OF_HANDLE Cannot get a handle.
 *   NWLR_OUT_OF_MEM    Not enough memory to create NWL object.
 *   NWLR_FAIL          The specified 'ps_nwl_name' or 'ui2_nwl_id' value
 *                      is not unique.
 *   NWLR_INV_ARG       'ph_nwl' or 'ps_nwl_path' is NULL.
 -----------------------------------------------------------------------*/
INT32 x_nwl_fs_load           (HANDLE_T            h_dir,
                               const CHAR*         ps_nwl_path,
                               UINT16              ui2_nwl_id,
                               const CHAR*	       ps_nwl_name,
                               VOID*               pv_tag,
                               x_nwl_nfy_fct       pf_nfy,
                               HANDLE_T*           ph_nwl)
{
    INT32           i4_rc;
    CHAR            s_nwl_name[MAX_NWL_NAME_LEN];
    
    if ( ps_nwl_path != NULL     &&
         ph_nwl      != NULL     &&
         ui2_nwl_id  != 0   
        )
    {
        *ph_nwl=NULL_HANDLE;

        x_memset(s_nwl_name,0,MAX_NWL_NAME_LEN);

        if ( ps_nwl_name != NULL )
        {
            x_strncpy(s_nwl_name,ps_nwl_name,MAX_NWL_NAME_LEN-1);
            s_nwl_name[MAX_NWL_NAME_LEN-1]='\0';
            ps_nwl_name=s_nwl_name;
        }

        DBG_API(("{NWL} loading NWL data from: %s / %s \n", ps_nwl_path, ps_nwl_name));
    
        i4_rc=nwl_fs_load_object(
            h_dir,
            ps_nwl_path,
            ui2_nwl_id,
            ps_nwl_name,
            pv_tag,
            pf_nfy,
            ph_nwl);
        
        DBG_API(("{NWL} done loading NWL data [ret code: %d]\n",i4_rc));
    }
    else
    {
        i4_rc = NWLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_fs_store
 *
 * Description: This API stores the specified NWL object to the file system.
 *
 * Inputs: 
 *   h_dir	      Handle to the parent directory of the file specified by
 *                the path 'ps_nwl_path'.  If 'ps_nwl_path' is an absolute
 *                path, then this argument should be set to 'FM_ROOT_HANDLE'.
 *   ps_nwl_path  Specifies the file path to the file where the NWL objects
 *                are stored in the file system.
 *   ui2_nwl_id   Specifies the  ID of the NWL object to be stored to the
 *                file system.  The limit for NWL ID is between 1 and 65535.
 *
 * Outputs: None 
 *
 * Returns: 
 *   NWLR_OK			NWL object is stored to the file system. 
 *   NWLR_NOT_FOUND		The NWL object specified by the 'ui2_nwl_id' does
 *                      not exist.
 *   NWLR_INV_ARG		'ps_nwl_path' is NULL.
 -----------------------------------------------------------------------*/
INT32 x_nwl_fs_store(HANDLE_T            h_dir,
                     const CHAR*         ps_nwl_path,
                     UINT16              ui2_nwl_id)
{
    INT32     i4_rc;
    
    if ( ps_nwl_path != NULL &&
         ui2_nwl_id  != 0
        )
    {
        DBG_API(("{NWL} saving NWL data.\n"));
        
        i4_rc=nwl_fs_store_object(h_dir,ps_nwl_path,ui2_nwl_id);      

        DBG_API(("{NWL} done saving NWL data [ret code: %d]\n",i4_rc));
    }
    else
    {
        i4_rc = NWLR_INV_ARG;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_fs_del
 *
 * Description: This API deletes a specified NWL object on the file system.
 *
 * Inputs: 
 *   h_dir	      Handle to the parent directory of the file specified by the
 *                path 'ps_nwl_path'.  If 'ps_nwl_path' is an absolute path,
 *                then this argument should be set to 'FM_ROOT_HANDLE'.
 *   ps_nwl_path  Specifies the file path to the file where the NWL objects
 *                are stored in the file system.
 *   ui2_nwl_id   Specifies the ID of the NWL object to be deleted from the
 *                file system.  The limit for NWL ID is between 1 and 65535.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK		  NWL object is deleted from the file system. 
 *   NWLR_NOT_FOUND	  'ui2_nwl_id' is not found or 'ps_nwl_path' does not
 *                    exist.
 *   NWLR_INV_ARG     'ps_nwl_path' is NULL.
 -----------------------------------------------------------------------*/
INT32 x_nwl_fs_del(HANDLE_T            h_dir,
                   const CHAR*         ps_nwl_path,
                   UINT16              ui2_nwl_id)
{
    INT32     i4_rc;

    if ( ps_nwl_path != NULL &&
         ui2_nwl_id  != 0
        )
    {
      
        i4_rc=nwl_fs_del_object(h_dir,ps_nwl_path, ui2_nwl_id);
        
    }
    else
    {
        i4_rc = NWLR_INV_ARG;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_fs_qry
 *
 * Description: This API queries the NWL objects stored in file system.
 *              It return the NWL object name and id value.
 * Inputs:
 *   h_dir         The handle to the directory.  The 'ps_nwl_path'
 *                 will be relative to the specified 'h_dir'.
 *
 *   ps_nwl_path   Specifies the directory path where NWL objects 
 *                 are stored.  
 *
 *   ui2_idx       Index value to get the next NWL object.  
 *                 0 means the first NWL, 1 is the second NWL, etc.
 *
 * Outputs: 
 *   pui2_nwl_id   Pointer to a UINT16 variable. On return, it contains 
 *                 NWL ID.
 *
 *   ps_nwl_name   Pointer to a string buffer (minimum 17 bytes) .  
 *                 On return, it contains the default NWL name stored on 
 *                 the file system. 
 *
 * Returns: 
 *   NWLR_OK              NWL object found. 
 *
 *   NWLR_NOT_FOUND       No NWL object found.
 *
 *   NWLR_INV_ARG         'ps_nwl_name' or 'pui2_nwl_id' is NULL.
 *
 *   NWLR_INV_FILE_PATH   Specified file path is not a directory or a 
 *                        RAW character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_fs_qry(HANDLE_T        h_dir,
                   const CHAR*     ps_nwl_path,
                   UINT16          ui2_idx,
                   UINT16*         pui2_nwl_id,
                   CHAR*           ps_nwl_name)
{
    INT32           i4_rc;
    
    if ( ps_nwl_path != NULL     &&
         pui2_nwl_id != NULL     &&
         ps_nwl_name != NULL
        )
    {
        *pui2_nwl_id = NWL_NULL_ID;
        ps_nwl_name[0] = '\0';
      
        i4_rc=nwl_qry_persistent_objects(
            h_dir,
            ps_nwl_path,
            ui2_idx,
            pui2_nwl_id,
            ps_nwl_name);
        
    }
    else
    {
        i4_rc = NWLR_INV_ARG;
    }
    return i4_rc;
}


/*--------------------------------------------------------------------
 * Name: x_nwl_fs_sync
 *
 * Description: This API sync a specified NWL handle with NWL records
 *              from persistent file system.  The existing NWL records
 *              are replaced with new records read from file. 
 *
 * Inputs:
 *
 *   h_nwl          Specify the handle to the NWL object.
 *
 *   h_dir          The handle to the directory.  The 'ps_nwl_path'
 *                  will be relative to the specified 'h_dir'.
 *
 *   ps_nwl_path    Specifies the directory path of the NWL object in the 
 *                  file system. See File Path Specification, 10.1, for the 
 *                  types of file path that can be specified.
 *
 *   ui2_nwl_id     Specifies the NWL id to load from the file system.  
 *                  The limit for NWL ID is between 1 and 65535.
 *
 * Outputs: 
 *
 *   None.
 *                 
 * Returns: 
 *   NWLR_OK             NWL records are load from the persistent file
 *                       into the memory of NWL object.
 *
 *   NWLR_NOT_FOUND      'ui2_nwl_id' is not found or 'ps_nwl_path' 
 *                       does not exist.
 *
 *   NWLR_FAIL           internal error
 *
 *   NWLR_INV_ARG        'ps_nwl_path' is NULL, or 'h_nwl' is NULL_HANDLE,
 *                       or ui2_nwl_id is 0.
 *
 *   NWLR_INV_FILE_PATH  Specified file path is not a directory or a RAW 
 *                       character file.
 *   
 ---------------------------------------------------------------------*/
INT32 x_nwl_fs_sync(HANDLE_T        h_nwl,
                    HANDLE_T        h_dir,
                    const CHAR*     ps_nwl_path,
                    UINT16          ui2_nwl_id)
{
    INT32           i4_rc;
    
    if ( ps_nwl_path != NULL            &&
         h_nwl       != NULL_HANDLE     &&
         ui2_nwl_id  != 0   
        )
    {
      
        i4_rc=nwl_fs_sync_object(h_nwl,
                                 h_dir,
                                 ps_nwl_path,
                                 ui2_nwl_id);
       
    }
    else
    {
        i4_rc = NWLR_INV_ARG;
    }
    return i4_rc;
}



/*----------------------------------------------------------------------
 * Name: x_nwl_get_info
 *
 * Description: 
 *   Get info (number of records, size of record, etc) for this NWL handle.
 *
 *
 * Inputs: 
 *   h_nwl        Handle to the NWL object.
 *
 * Outputs:
 *   None.
 *
 * Returns: 
 *   NWLR_OK         Version id is returned. 
 *   NWLR_INV_HANDLE Bad 'h_nwl' handle.
 *   NWLR_INV_ARG    'pf_sort_cmp' or 'pf_search_cmp' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 x_nwl_get_info
(
    HANDLE_T               h_nwl
)
{
    INT32               i4_rc;
    NWL_OBJ_T*  	    pt_nwl;
    HANDLE_TYPE_T       pe_type;
    DB_INFO_T           t_db_info;
    UINT32              ui4_persistent_size;
    UINT32              ui4_ver_id;
    
    if ( (handle_get_type_obj(h_nwl, &pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                      &&
         (pt_nwl  != NULL)
        )
    {
        x_dbg_stmt("<NWL ID:  %d  NWL_NAME: %s>\n",
                   pt_nwl->ui2_nwl_id, pt_nwl->ps_nwl_name);
        
        if ( db_get_info(pt_nwl->h_cdb, &t_db_info) == DBR_OK )
        {
            x_dbg_stmt("number of NWL records:   %d \n",
                       t_db_info.ui4_record_count);
            
            x_dbg_stmt("size of each record:     %d (bytes)\n",
                       t_db_info.ui2_record_width);

            x_dbg_stmt("Heap memory usage for string buffer:  allocated: %d (bytes) used: %d (bytes)\n",
                       t_db_info.ui4_str_buf_allocated,
                       t_db_info.ui4_str_buf_used);

            x_dbg_stmt("Heap memory usage for string obj array: allocated: %d (bytes) used: %d (bytes)\n",
                       t_db_info.ui2_nb_str_obj_allocated*sizeof(UINT16),
                       t_db_info.ui2_nb_str_obj_used*sizeof(UINT16));
            
            x_dbg_stmt("NWL object filessize [header+metadata+data]:  %d (bytes)\n",
                       nwl_persistent_size(pt_nwl));

            /* Size of CDB object. */
            i4_rc=db_get_persistent_size(pt_nwl->h_cdb,
                                         /* currently, NWL record contain no UNION field,
                                            so the endian conversion function to intrepret
                                            the UNION field is not needed. */
                                         /* (db_endian_conv_fct)nwl_union_end_conv_to_file, */
                                         NULL,
                                         &ui4_persistent_size,
                                         &ui4_ver_id);
            if ( i4_rc == DBR_OK )
            {
                x_dbg_stmt("[with CDB object using: [%d bytes]\n", ui4_persistent_size);
            }

            i4_rc = NWLR_OK;
        }
        else
        {
            x_dbg_stmt("Error: can get info on this NWL handle.\n");
            i4_rc = NWLR_FAIL;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}



/*----------------------------------------------------------------------
 * Name: nwl_set_cursor
 *
 * Description: This API specifies a entry position in NWL records, 
 *   which will read when API x_nwl_read_cursor() is called.
 * 
 *
 * Inputs: 
 *   h_nwl       Handle to the NWL object.
 *   e_pos       position of the NWL record to read.
 *
 * Outputs: 
 *   None.
 *               
 * Returns: 
 *   NWLR_OK    Data from the specified record is returned. 
 *   NWLR_INV_HANDLE    Bad handle.
 *   NWLR_INV_ARG       'e_pos' is invalid.
 *   
 -----------------------------------------------------------------------*/
INT32 nwl_set_cursor
(
    HANDLE_T            h_nwl,
    NWL_ITERATE_T       e_pos
)
{
    INT32           i4_rc;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;
    DB_CTRL_FLAG_T  e_cursor_ctrl;
    

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        i4_rc = NWLR_OK;
        
        switch (e_pos)
        {
            case NWL_FIRST_REC:
            {
                e_cursor_ctrl = DB_FIRST_RECORD;
            }
            break;

            case NWL_LAST_REC:
            {
                e_cursor_ctrl = DB_LAST_RECORD;
            }
            break;

            case NWL_NEXT_REC:
            {
                e_cursor_ctrl = DB_NEXT_RECORD;
            }
            break;

            case NWL_PREV_REC:
            {
                e_cursor_ctrl = DB_PREV_RECORD;
            }
            break;

            default:
            {
                i4_rc = NWLR_INV_ARG;
            }
            break;
        }

        if ( i4_rc == NWLR_OK )
        {
            i4_rc=db_set_cursor(pt_nwl->h_cdb,
                                e_cursor_ctrl,
                                DB_NULL_RECORD);
            /* translate the CDB error code to NWL error code */
            i4_rc=nwl_cdb_to_nwl_err_code(i4_rc);
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_read_cursor
 *
 * Description: This API read the content of a specific record
 *   stored in the NWL database.  The specific record is read
 *   is the record currently pointed by the cursor.   
 *              
 * Inputs: 
 *   h_nwl           Handle to the NWL object.
 *
 * Outputs: 
 *   pt_nwl_rec  Pointer to a service record structure.   This structure must 
 *               be allocated by the caller.  On return, it is filled with 
 *               service record information.
 *
 *   pui4_ver_id Pointer to a variable holding version id of the database 
 *               object.   Note: version id of the NWL database object is 
 *               incremented each time when a modification takes place.
 * Returns: 
 *   NWLR_OK    Data from the specified record is returned. 
 *   NWLR_INV_HANDLE    Bad handle.
 *   NWLR_INV_ARG       'pui4_ver_id' or 'pt_nwl_rec' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 nwl_read_cursor
(
    HANDLE_T            h_nwl,
    NWL_REC_T*          pt_nwl_rec,
    UINT32*             pui4_ver_id
)
{
    INT32           i4_rc;
    NWL_OBJ_T*  	pt_nwl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        if ( pui4_ver_id    != NULL &&
             pt_nwl_rec     != NULL
            )
        {          
            i4_rc=nwl_read_rec_at_cursor_pos(pt_nwl,
                                             pt_nwl_rec,
                                             pui4_ver_id);            
        }
        else
        {
            i4_rc=NWLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_sort_rec
 *
 * Description: This API applies the sort operation (using the
 *    per-handle compare fucntion, or the global compare
 *    function, or default compare function, to the NWL
 *    records.
 *             
 * Inputs: 
 *    h_nwl     Handle to the NWL object.
 *
 * Outputs: 
 *    pui4_ver_id   Contains the version id of the NWL database object.   
 *                  
 * Returns: 
 *   NWLR_OK           
 *   
 -----------------------------------------------------------------------*/
INT32 nwl_sort_rec
(
    HANDLE_T			h_nwl,
    UINT32*             pui4_ver_id 
)
{
    INT32               i4_rc;
    NWL_OBJ_T*  	    pt_nwl;
    HANDLE_TYPE_T       pe_type;
 
    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        if ( pui4_ver_id  != NULL )
        {
            i4_rc=nwl_sort_rec_by_network_id(pt_nwl,pui4_ver_id);
        }
        else
        {
            i4_rc=NWLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: x_nwl_init_mem_dump
 *
 * Description: This API initializes the memory dump structure, which 
 *              is used for x_nwl_dump_to_mem.
 *
 * Inputs: 
 *   None
 *
 * Outputs: 
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Returns: 
 *   NWLR_OK            NWL database successfully dumped.
 *   NWLR_INV_ARG       'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_init_mem_dump
(
    NWL_MEM_DUMP_T*        pt_mem_dump
)
{
    INT32           i4_rc;

    if ( pt_mem_dump != NULL )
    {          
        x_memset(pt_mem_dump, 0, sizeof(NWL_MEM_DUMP_T));
        i4_rc=NWLR_OK;
    }
    else
    {
        i4_rc=NWLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_free_mem_dump
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
 *   NWLR_OK            NWL database successfully dumped.
 *   NWLR_INV_ARG       'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_free_mem_dump
(
    NWL_MEM_DUMP_T*        pt_mem_dump
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
        i4_rc=NWLR_OK;
    }
    else
    {
        i4_rc=NWLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_dump_to_mem
 *
 * Description: This API dumps the specified NWL database into the specified
 *              structure, which could be later used for x_nwl_load_from_mem.
 *              The memory dump structure should be initialized before dumping.
 *
 * Inputs: 
 *   h_nwl           Handle to the NWL object.
 *
 * Outputs: 
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Returns: 
 *   NWLR_OK            NWL database successfully dumped.
 *   NWLR_INV_HANDLE    Bad handle.
 *   NWLR_INV_ARG       'pt_mem_dump' is NULL.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_dump_to_mem
(
    HANDLE_T               h_nwl,
    NWL_MEM_DUMP_T*        pt_mem_dump
)
{
    INT32           i4_rc;
    NWL_OBJ_T*      pt_nwl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        if ( pt_mem_dump != NULL )
        {
            pt_mem_dump->ui2_nwl_id = pt_nwl->ui2_nwl_id;
            x_strcpy(pt_mem_dump->ps_nwl_name, pt_nwl->ps_nwl_name);
            i4_rc = db_dump_to_mem(pt_nwl->h_cdb,
                                   TRUE,
                                   &(pt_mem_dump->pv_data),
                                   &(pt_mem_dump->ui4_data_len));
            i4_rc = nwl_cdb_to_nwl_err_code(i4_rc);
        }
        else
        {
            i4_rc=NWLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_load_from_mem
 *
 * Description: This API loads the NWL database from the specified memory
 *              dump structure, whose data comes from a previous call to
 *              x_nwl_load_from_mem.
 *
 * Inputs: 
 *   h_nwl           Handle to the NWL object.
 *   pt_mem_dump     Pointer to the memory dump structure.
 *
 * Outputs: 
 *
 * Returns: 
 *   NWLR_OK            NWL database successfully dumped.
 *   NWLR_INV_HANDLE    Bad handle.
 *   NWLR_INV_ARG       'pt_mem_dump' is NULL or NWL metadata not match.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_load_from_mem
(
    HANDLE_T               h_nwl,
    NWL_MEM_DUMP_T*        pt_mem_dump
)
{
    INT32           i4_rc;
    NWL_OBJ_T*      pt_nwl;
    HANDLE_TYPE_T   pe_type;

    if ( (handle_get_type_obj(h_nwl,&pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                    &&
         (pt_nwl  != NULL)
        )
    {
        if ( pt_mem_dump != NULL )
        {
            if (pt_mem_dump->ui2_nwl_id == pt_nwl->ui2_nwl_id &&
                x_strcmp(pt_mem_dump->ps_nwl_name, pt_nwl->ps_nwl_name) == 0)
            {
                i4_rc = db_load_from_mem(pt_nwl->h_cdb,
                                         pt_mem_dump->pv_data,
                                         pt_mem_dump->ui4_data_len);
                i4_rc = nwl_cdb_to_nwl_err_code(i4_rc);
            }
            else
            {
                i4_rc = NWLR_INV_ARG;
            }
        }
        else
        {
            i4_rc=NWLR_INV_ARG;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_fs_store_given_file_handle
 *
 * Description: This API stores the specified NWL object to the 
 *              file system.
 *
 * Inputs:
 *   h_dir         The handle to the file.
 *
 *  ui2_nwl_id     Specifies the NWL object to be stored to the file system.  
 *                 The limit for NWL ID is between 1 and 65535.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK             NWL object is stored to the file system. 
 *
 *   NWLR_NOT_FOUND      The NWL object specified by the 'ui2_nwl_id' 
 *                       does not exist.
 *
 *   NWLR_INV_ARG        ps_nwl_path' is NULL.
 *
 *   NWLR_INV_FILE_PATH  Specified file path is not a directory or 
 *                       a RAW character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_fs_store_given_file_handle
(
    HANDLE_T        h_file,
    UINT16          ui2_nwl_id  
)
{
    INT32       i4_rc;
    
    if ( ui2_nwl_id  != 0 )
    {
        DBG_API(("{NWL} saving NWL data.\n"));
        
        i4_rc=nwl_fs_store_object_given_file_handle(h_file,ui2_nwl_id);

        DBG_API(("{NWL} done saving NWL data [ret code: %d]\n",i4_rc));
    }
    else
    {
        i4_rc = NWLR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_nwl_fs_sync_given_file_handle
 *
 * Description: This API sync a specified NWL handle with NWL records
 *              from persistent file system.  The existing NWL records
 *              are replaced with new records read from file. 
 *
 * Inputs:
 *
 *   h_nwl          Specify the handle to the NWL object.
 *
 *   h_file         The handle to the file.
 *
 *   ui2_nwl_id     Specifies the NWL id to load from the file system.  
 *                  The limit for NWL ID is between 1 and 65535.
 *
 * Outputs: 
 *
 *   None.
 *                 
 * Returns: 
 *   NWLR_OK             NWL records are load from the persistent file
 *                       into the memory of NWL object.
 *
 *   NWLR_NOT_FOUND      'ui2_nwl_id' is not found or 'ps_nwl_path' 
 *                       does not exist.
 *
 *   NWLR_FAIL           internal error
 *
 *   NWLR_INV_ARG        'ps_nwl_path' is NULL, or 'h_nwl' is NULL_HANDLE,
 *                       or ui2_nwl_id is 0.
 *
 *   NWLR_INV_FILE_PATH  Specified file path is not a directory or a RAW 
 *                       character file.
 *   
 -----------------------------------------------------------------------*/
INT32 x_nwl_fs_sync_given_file_handle
(
    HANDLE_T		h_nwl,	
    HANDLE_T        h_file,
    UINT16		    ui2_nwl_id
)
{
    INT32           i4_rc;
    
    if ( h_nwl       != NULL_HANDLE     &&
         ui2_nwl_id  != 0   
        )
    {
        i4_rc=nwl_fs_sync_object_given_file_handle(h_nwl,
                                                   h_file,
                                                   ui2_nwl_id);
    }
    else
    {
        i4_rc = NWLR_INV_ARG;
    }
    return i4_rc;
}
